/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Mutex Deadlock Check
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --------------------------------------------------------------------------- */

#include "los_mux_debug_pri.h"
#include "los_typedef.h"
#include "los_task.h"
#include "arch/exception.h"
#ifdef LOSCFG_SHELL
#include "shcmd.h"
#endif /* LOSCFG_SHELL */

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef struct {
    LOS_DL_LIST muxListHead;    /* Task-held mutexs list */
    UINT64      lastAccessTime; /* The last operation time */
} MuxDLinkCB;

typedef struct {
    LOS_DL_LIST muxList; /* Insert mutex into the owner task CB */
    VOID        *muxCB;  /* The Mutex CB pointer */
} MuxDLinkNode;

STATIC MuxDLinkCB *g_muxDeadlockCBArray = NULL;

/*
 * Mutex deadlock detection time threshold, will print out task information
 * that has not been scheduled within this time.
 * The unit is tick.
 */
#define OS_MUX_DEADLOCK_CHECK_THRESHOLD 60000

UINT32 OsMuxDlockCheckInit(VOID)
{
    UINT32 index;
    UINT32 size = (LOSCFG_BASE_CORE_TSK_LIMIT + 1) * sizeof(MuxDLinkCB);

    /* system resident memory, don't free */
    g_muxDeadlockCBArray = (MuxDLinkCB *)LOS_MemAlloc(m_aucSysMem1, size);
    if (g_muxDeadlockCBArray == NULL) {
        PRINT_ERR("%s: malloc failed!\n", __FUNCTION__);
        return LOS_NOK;
    }

    for (index = 0; index < LOSCFG_BASE_CORE_TSK_LIMIT + 1; index++) {
        g_muxDeadlockCBArray[index].lastAccessTime = 0;
        LOS_ListInit(&g_muxDeadlockCBArray[index].muxListHead);
    }
    return LOS_OK;
}

VOID OsMuxDlockNodeInsert(UINT32 taskId, VOID *muxCB)
{
    MuxDLinkNode *muxDLNode = NULL;

    if ((taskId > LOSCFG_BASE_CORE_TSK_LIMIT) || (muxCB == NULL)) {
        PRINT_ERR("%s: Argument is invalid!\n", __FUNCTION__);
        return;
    }

    muxDLNode = (MuxDLinkNode *)LOS_MemAlloc(m_aucSysMem1, sizeof(MuxDLinkNode));
    if (muxDLNode == NULL) {
        PRINT_ERR("%s: malloc failed!\n", __FUNCTION__);
        return;
    }
    (VOID)memset_s(muxDLNode, sizeof(MuxDLinkNode), 0, sizeof(MuxDLinkNode));
    muxDLNode->muxCB = muxCB;

    LOS_ListTailInsert(&g_muxDeadlockCBArray[taskId].muxListHead, &muxDLNode->muxList);
}

VOID OsMuxDlockNodeDelete(UINT32 taskId, const VOID *muxCB)
{
    MuxDLinkCB *muxDLCB = NULL;
    LOS_DL_LIST *list = NULL;
    MuxDLinkNode *muxDLNode = NULL;

    if ((taskId > LOSCFG_BASE_CORE_TSK_LIMIT) || (muxCB == NULL)) {
        PRINT_ERR("%s: Argument is invalid!\n", __FUNCTION__);
        return;
    }

    muxDLCB = &g_muxDeadlockCBArray[taskId];
    LOS_DL_LIST_FOR_EACH(list, &muxDLCB->muxListHead) {
        muxDLNode = LOS_DL_LIST_ENTRY(list, MuxDLinkNode, muxList);
        if (muxDLNode->muxCB == muxCB) {
            LOS_ListDelete(&muxDLNode->muxList);
            (VOID)LOS_MemFree(m_aucSysMem1, muxDLNode);
            return;
        }
    }

    PRINT_ERR("%s: find mutex deadlock node failed!\n", __FUNCTION__);
}

VOID OsTaskTimeUpdate(UINT32 taskId, UINT64 tickCount)
{
    if (taskId > LOSCFG_BASE_CORE_TSK_LIMIT) {
        return;
    }

    g_muxDeadlockCBArray[taskId].lastAccessTime = tickCount;
}

STATIC VOID OsDeadlockBackTrace(const LosTaskCB *taskCB)
{
    TaskContext *context = NULL;

    PRINTK("*******backtrace begin*******\n");
    context = (TaskContext *)taskCB->stackPointer;
    ArchBackTraceWithSp(context);
    PRINTK("********backtrace end********\n");
    return;
}

STATIC VOID OsMutexPendTaskList(LOS_DL_LIST *list)
{
    LOS_DL_LIST *listTmp = NULL;
    LosTaskCB *pendedTask = NULL;
    CHAR *name = NULL;
    UINT32 index = 0;
    UINT32 id, intSave;

    SCHEDULER_LOCK(intSave);
    if (LOS_ListEmpty(list) == TRUE) {
        SCHEDULER_UNLOCK(intSave);
        PRINTK("Pended Task: null\n");
        return;
    }

    LOS_DL_LIST_FOR_EACH(listTmp, list) {
        pendedTask = OS_TCB_FROM_PENDLIST(listTmp);
        name = pendedTask->taskName;
        id = pendedTask->taskId;
        SCHEDULER_UNLOCK(intSave);
        if (index == 0) {
            PRINTK("Pended task: %u. name:%-15s, id:0x%-5x\n", index, name, id);
        } else {
            PRINTK("             %u. name:%-15s, id:0x%-5x\n", index, name, id);
        }
        index++;
        SCHEDULER_LOCK(intSave);
    }
    SCHEDULER_UNLOCK(intSave);
}

STATIC VOID OsTaskHoldMutexList(MuxDLinkCB *muxDLCB)
{
    UINT32 index = 0;
    MuxDLinkNode *muxDLNode = NULL;
    CHAR *ownerName = NULL;
    LosMuxCB *muxCB = NULL;
    LOS_DL_LIST *list = NULL;
    LOS_DL_LIST *listTmp = NULL;
    UINT32 count, intSave;

    SCHEDULER_LOCK(intSave);
    if (LOS_ListEmpty(&muxDLCB->muxListHead) == TRUE) {
        SCHEDULER_UNLOCK(intSave);
        PRINTK("null\n");
    } else {
        LOS_DL_LIST_FOR_EACH(list, &muxDLCB->muxListHead) {
            muxDLNode = LOS_DL_LIST_ENTRY(list, MuxDLinkNode, muxList);
            muxCB = (LosMuxCB *)muxDLNode->muxCB;
            count = muxCB->muxCount;
            ownerName = muxCB->owner->taskName;
            SCHEDULER_UNLOCK(intSave);
            PRINTK("<Mutex%u info>\n", index);
            PRINTK("Ptr handle:%p\n", muxCB);
            PRINTK("Owner:%s\n", ownerName);
            PRINTK("Count:%u\n", count);

            listTmp = &muxCB->muxList;
            OsMutexPendTaskList(listTmp);

            index++;
            SCHEDULER_LOCK(intSave);
        }
        SCHEDULER_UNLOCK(intSave);
    }
}

VOID OsMutexDlockCheck(VOID)
{
    UINT32 loop, intSave;
    UINT32 taskId;
    CHAR *name = NULL;
    LosTaskCB *taskCB = NULL;
    MuxDLinkCB *muxDLCB = NULL;

    SCHEDULER_LOCK(intSave);
    for (loop = 0; loop < g_taskMaxNum; loop++) {
        taskCB = (LosTaskCB *)g_taskCBArray + loop;
        if (taskCB->taskStatus & OS_TASK_STATUS_UNUSED) {
            continue;
        }

        muxDLCB = &g_muxDeadlockCBArray[taskCB->taskId];
        if ((LOS_TickCountGet() - muxDLCB->lastAccessTime) > OS_MUX_DEADLOCK_CHECK_THRESHOLD) {
            name = taskCB->taskName;
            taskId = taskCB->taskId;
            SCHEDULER_UNLOCK(intSave);
            PRINTK("Task_name:%s, ID:0x%x, holds the Mutexs below:\n", name, taskId);
            OsTaskHoldMutexList(muxDLCB);
            OsDeadlockBackTrace(taskCB);
            PRINTK("\n");
            SCHEDULER_LOCK(intSave);
        }
    }
    SCHEDULER_UNLOCK(intSave);
}

#ifdef LOSCFG_SHELL
UINT32 OsShellCmdMuxDeadlockCheck(UINT32 argc, const CHAR **argv)
{
    if (argc > 0) {
        PRINTK("\nUsage: dlock\n");
        return OS_ERROR;
    }
    PRINTK("Start mutexs deadlock check: \n");
    OsMutexDlockCheck();
    PRINTK("-----------End-----------\n");
    return LOS_OK;
}
SHELLCMD_ENTRY(deadlock_shellcmd, CMD_TYPE_EX, "dlock", 0, (CmdCallBackFunc)OsShellCmdMuxDeadlockCheck);
#endif /* LOSCFG_SHELL */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
