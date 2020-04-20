/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Mutex Debug
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
/* ----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 * --------------------------------------------------------------------------- */

#include "los_mux_debug_pri.h"
#include "los_typedef.h"
#include "los_task.h"
#include "los_hw_pri.h"
#include "los_ipcdebug_pri.h"
#include "los_exc_pri.h"
#ifdef LOSCFG_SHELL
#include "shcmd.h"
#endif /* LOSCFG_SHELL */

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#ifdef LOSCFG_DEBUG_MUTEX

typedef struct {
    TSK_ENTRY_FUNC creater; /* The task entry who created this mutex */
    UINT64  lastAccessTime; /* The last access time */
} MuxDebugCB;

#if (LOSCFG_BASE_IPC_MUX_DYN_MEM == YES)
STATIC MuxDebugCB *g_muxDebugArray = NULL;
#else
STATIC MuxDebugCB g_muxDebugArray[LOSCFG_BASE_IPC_MUX_LIMIT];
#endif

STATIC BOOL MuxCompareValue(const IpcSortParam *sortParam, UINT32 left, UINT32 right)
{
    return (*((UINT64 *)(VOID *)SORT_ELEM_ADDR(sortParam, left)) >
            *((UINT64 *)(VOID *)SORT_ELEM_ADDR(sortParam, right)));
}

UINT32 OsMuxDbgInit(VOID)
{
#if (LOSCFG_BASE_IPC_MUX_DYN_MEM == YES)
    UINT32 size = LOSCFG_BASE_IPC_MUX_LIMIT * sizeof(MuxDebugCB);
    /* system resident memory, don't free */
    g_muxDebugArray = (MuxDebugCB *)LOS_MemAlloc(m_aucSysMem1, size);
    if (g_muxDebugArray == NULL) {
        PRINT_ERR("%s: malloc failed!\n", __FUNCTION__);
        return LOS_NOK;
    }
    (VOID)memset_s(g_muxDebugArray, size, 0, size);
#endif
    return LOS_OK;
}

VOID OsMuxDbgTimeUpdate(UINT32 muxID)
{
    MuxDebugCB *muxDebug = &g_muxDebugArray[GET_MUX_INDEX(muxID)];
    muxDebug->lastAccessTime = LOS_TickCountGet();
}

VOID OsMuxDbgUpdate(UINT32 muxID, TSK_ENTRY_FUNC creater)
{
    MuxDebugCB *muxDebug = &g_muxDebugArray[GET_MUX_INDEX(muxID)];
    muxDebug->creater = creater;
    muxDebug->lastAccessTime = LOS_TickCountGet();
}

STATIC VOID SortMuxIndexArray(UINT32 *indexArray, UINT32 count)
{
    LosMuxCB muxNode = {0};
    MuxDebugCB muxDebugNode = {0};
    UINT32 index, intSave;
    IpcSortParam muxSortParam;
    muxSortParam.buf = (CHAR *)g_muxDebugArray;
    muxSortParam.ipcDebugCBSize = sizeof(MuxDebugCB);
    muxSortParam.ipcDebugCBCnt = LOSCFG_BASE_IPC_MUX_LIMIT;
    muxSortParam.sortElemOff = OFFSET_OF_FIELD(MuxDebugCB, lastAccessTime);

    if (count > 0) {
        SCHEDULER_LOCK(intSave);
        OsArraySortByTime(indexArray, 0, count - 1, &muxSortParam, MuxCompareValue);
        SCHEDULER_UNLOCK(intSave);
        for (index = 0; index < count; index++) {
            SCHEDULER_LOCK(intSave);
            (VOID)memcpy_s(&muxNode, sizeof(LosMuxCB),
                           GET_MUX(indexArray[index]), sizeof(LosMuxCB));
            (VOID)memcpy_s(&muxDebugNode, sizeof(MuxDebugCB),
                           &g_muxDebugArray[indexArray[index]], sizeof(MuxDebugCB));
            SCHEDULER_UNLOCK(intSave);
            if ((muxNode.muxStat != OS_MUX_USED) ||
                ((muxNode.muxStat == OS_MUX_USED) && (muxDebugNode.creater == NULL))) {
                continue;
            }
            PRINTK("Mutex ID <0x%x> may leak, TaskID of owner:0x%x, TaskEntry of owner: %p,"
                   "TaskEntry of creater: %p,Latest operation time: 0x%llx\n",
                   muxNode.muxID, muxNode.owner->taskID, muxNode.owner->taskEntry, muxDebugNode.creater,
                   muxDebugNode.lastAccessTime);
        }
    }
    (VOID)LOS_MemFree((VOID *)OS_SYS_MEM_ADDR, indexArray);
}

VOID OsMutexCheck(VOID)
{
    LosMuxCB muxNode = {0};
    MuxDebugCB muxDebugNode = {0};
    UINT32 index, intSave;
    UINT32 count = 0;

    /*
     * This return value does not need to be judged immediately,
     * and the following code logic has already distinguished the return value from null and non-empty,
     * and there is no case of accessing the null pointer.
     */
    UINT32 *indexArray = (UINT32 *)LOS_MemAlloc((VOID *)OS_SYS_MEM_ADDR, LOSCFG_BASE_IPC_MUX_LIMIT * sizeof(UINT32));

    for (index = 0; index < LOSCFG_BASE_IPC_MUX_LIMIT; index++) {
        SCHEDULER_LOCK(intSave);
        (VOID)memcpy_s(&muxNode, sizeof(LosMuxCB), GET_MUX(index), sizeof(LosMuxCB));
        (VOID)memcpy_s(&muxDebugNode, sizeof(MuxDebugCB), &g_muxDebugArray[index], sizeof(MuxDebugCB));
        SCHEDULER_UNLOCK(intSave);

        if ((muxNode.muxStat != OS_MUX_USED) ||
            ((muxNode.muxStat == OS_MUX_USED) && (muxDebugNode.creater == NULL))) {
            continue;
        } else if ((muxNode.muxStat == OS_MUX_USED) && (muxNode.owner == NULL)) {
            PRINTK("Mutex ID <0x%x> may leak, Owner is null, TaskEntry of creater: %p,"
                   "Latest operation time: 0x%llx\n",
                   muxNode.muxID, muxDebugNode.creater, muxDebugNode.lastAccessTime);
        } else {
            if (indexArray != NULL) {
                *(indexArray + count) = index;
                count++;
            } else {
                PRINTK("Mutex ID <0x%x> may leak, TaskID of owner:0x%x, TaskEntry of owner: %p,"
                       "TaskEntry of creater: %p,Latest operation time: 0x%llx\n",
                       muxNode.muxID, muxNode.owner->taskID, muxNode.owner->taskEntry, muxDebugNode.creater,
                       muxDebugNode.lastAccessTime);
            }
        }
    }

    if (indexArray != NULL) {
        SortMuxIndexArray(indexArray, count);
    }
}

#ifdef LOSCFG_SHELL
LITE_OS_SEC_TEXT_MINOR UINT32 OsShellCmdMuxInfoGet(UINT32 argc, const CHAR **argv)
{
    if (argc > 0) {
        PRINTK("\nUsage: mutex\n");
        return OS_ERROR;
    }
    PRINTK("used mutexs information: \n");
    OsMutexCheck();
    return LOS_OK;
}
SHELLCMD_ENTRY(mutex_shellcmd, CMD_TYPE_EX, "mutex", 0, (CmdCallBackFunc)OsShellCmdMuxInfoGet);
#endif /* LOSCFG_SHELL */
#endif /* LOSCFG_DEBUG_MUTEX */

#ifdef LOSCFG_DEBUG_DEADLOCK
typedef struct {
    LOS_DL_LIST muxListHead;    /* Task-held mutexs list */
    UINT64      lastAccessTime; /* The last operation time */
} MuxDLinkCB;

typedef struct {
    LOS_DL_LIST muxList; /* Insert mutex into the owner task CB */
    VOID        *muxCB;  /* The Mutex CB pointer */
} MuxDLinkNode;

#if (LOSCFG_BASE_IPC_DYNAMIC_MEM == YES)
STATIC MuxDLinkCB *g_muxDeadlockCBArray;
#else
STATIC MuxDLinkCB g_muxDeadlockCBArray[LOSCFG_BASE_CORE_TSK_LIMIT + 1];
#endif


/*
 * Mutex deadlock detection time threshold, will print out task information
 * that has not been scheduled within this time.
 * The unit is tick.
 */
#define OS_MUX_DEADLOCK_CHECK_THRESHOLD 60000

UINT32 OsMuxDlockCheckInit(VOID)
{
    UINT32 index;

    /* system resident memory, don't free */
#if (LOSCFG_BASE_IPC_DYNAMIC_MEM == YES)
    UINT32 size = (g_taskMaxNum + 1) * sizeof(MuxDLinkCB);
    g_muxDeadlockCBArray = (MuxDLinkCB *)LOS_MemAlloc(m_aucSysMem1, size);
    if (g_muxDeadlockCBArray == NULL) {
        PRINT_ERR("%s: malloc failed!\n", __FUNCTION__);
        return LOS_NOK;
    }
#endif

    for (index = 0; index <= LOSCFG_BASE_CORE_TSK_LIMIT; index++) {
        g_muxDeadlockCBArray[index].lastAccessTime = 0;
        LOS_ListInit(&g_muxDeadlockCBArray[index].muxListHead);
    }
    return LOS_OK;
}

VOID OsMuxDlockNodeInsert(UINT32 taskID, VOID *muxCB)
{
    MuxDLinkNode *muxDLNode = NULL;

    if ((taskID > LOSCFG_BASE_CORE_TSK_LIMIT) || (muxCB == NULL)) {
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

    LOS_ListTailInsert(&g_muxDeadlockCBArray[taskID].muxListHead, &muxDLNode->muxList);
}

VOID OsMuxDlockNodeDelete(UINT32 taskID, const VOID *muxCB)
{
    MuxDLinkCB *muxDLCB = NULL;
    LOS_DL_LIST *list = NULL;
    MuxDLinkNode *muxDLNode = NULL;

    if ((taskID > LOSCFG_BASE_CORE_TSK_LIMIT) || (muxCB == NULL)) {
        PRINT_ERR("%s: Argument is invalid!\n", __FUNCTION__);
        return;
    }

    muxDLCB = &g_muxDeadlockCBArray[taskID];
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

VOID OsTaskTimeUpdate(UINT32 taskID, UINT64 tickCount)
{
    if (taskID > LOSCFG_BASE_CORE_TSK_LIMIT) {
        return;
    }

    g_muxDeadlockCBArray[taskID].lastAccessTime = tickCount;
}

STATIC VOID OsDeadlockBackTrace(const LosTaskCB *taskCB)
{
    UINTPTR FP;
    TaskContext *context = NULL;

    PRINTK("*******backtrace begin*******\n");
    context = (TaskContext *)taskCB->stackPointer;
#ifdef __LP64__
    FP = context->X[29];  /* X29:FP */
#else
    FP = context->R[11];  /* R11:FP */
#endif
    PRINTK("pc = %p  lr = %p\n", context->PC, context->LR);

    BackTraceSub(FP);

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
        id = pendedTask->taskID;
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
    UINT32 taskID;
    CHAR *name = NULL;
    LosTaskCB *taskCB = NULL;
    MuxDLinkCB *muxDLCB = NULL;

    SCHEDULER_LOCK(intSave);
    for (loop = 0; loop < g_taskMaxNum; loop++) {
        taskCB = (LosTaskCB *)g_taskCBArray + loop;
        if (taskCB->taskStatus & OS_TASK_STATUS_UNUSED) {
            continue;
        }

        muxDLCB = &g_muxDeadlockCBArray[taskCB->taskID];
        if ((LOS_TickCountGet() - muxDLCB->lastAccessTime) > OS_MUX_DEADLOCK_CHECK_THRESHOLD) {
            name = taskCB->taskName;
            taskID = taskCB->taskID;
            SCHEDULER_UNLOCK(intSave);
            PRINTK("Task_name:%s, ID:0x%x, holds the Mutexs below:\n", name, taskID);
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
#endif /* LOSCFG_DEBUG_DEADLOCK */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
