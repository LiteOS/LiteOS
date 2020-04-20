/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Queue Debug
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

#include "los_queue_debug_pri.h"
#include "los_hw_pri.h"
#include "los_ipcdebug_pri.h"
#ifdef LOSCFG_SHELL
#include "shcmd.h"
#endif /* LOSCFG_SHELL */

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#ifdef LOSCFG_DEBUG_QUEUE

typedef struct {
    TSK_ENTRY_FUNC creater; /* The task entry who created this queue */
    UINT64  lastAccessTime; /* The last access time */
} QueueDebugCB;

#if (LOSCFG_BASE_IPC_DYNAMIC_MEM == YES)
STATIC QueueDebugCB *g_queueDebugArray = NULL;
#else
STATIC QueueDebugCB g_queueDebugArray[LOSCFG_BASE_IPC_QUEUE_LIMIT];
#endif

STATIC BOOL QueueCompareValue(const IpcSortParam *sortParam, UINT32 left, UINT32 right)
{
    return (*((UINT64 *)(VOID *)SORT_ELEM_ADDR(sortParam, left)) >
            *((UINT64 *)(VOID *)SORT_ELEM_ADDR(sortParam, right)));
}

UINT32 OsQueueDbgInit(VOID)
{
#if (LOSCFG_BASE_IPC_DYNAMIC_MEM == YES)
    UINT32 size = LOSCFG_BASE_IPC_QUEUE_LIMIT * sizeof(QueueDebugCB);
    /* system resident memory, don't free */
    g_queueDebugArray = (QueueDebugCB *)LOS_MemAlloc(m_aucSysMem1, size);
    if (g_queueDebugArray == NULL) {
        PRINT_ERR("%s: malloc failed!\n", __FUNCTION__);
        return LOS_NOK;
    }
    (VOID)memset_s(g_queueDebugArray, size, 0, size);
#endif

    return LOS_OK;
}

VOID OsQueueDbgTimeUpdate(UINT32 queueID)
{
    QueueDebugCB *queueDebug = &g_queueDebugArray[GET_QUEUE_INDEX(queueID)];
    queueDebug->lastAccessTime = LOS_TickCountGet();
    return;
}

VOID OsQueueDbgUpdate(UINT32 queueID, TSK_ENTRY_FUNC entry)
{
    QueueDebugCB *queueDebug = &g_queueDebugArray[GET_QUEUE_INDEX(queueID)];
    queueDebug->creater = entry;
    queueDebug->lastAccessTime = LOS_TickCountGet();
    return;
}

STATIC INLINE VOID OsQueueInfoOutPut(const LosQueueCB *node)
{
    PRINTK("Queue ID <0x%x> may leak, queue len is 0x%x, "
           "readable cnt:0x%x, writeable cnt:0x%x, ",
           node->queueID,
           node->queueLen,
           node->readWriteableCnt[OS_QUEUE_READ],
           node->readWriteableCnt[OS_QUEUE_WRITE]);
}

STATIC INLINE VOID OsQueueOpsOutput(const QueueDebugCB *node)
{
    PRINTK("TaskEntry of creater:0x%p, Latest operation time: 0x%llx\n",
           node->creater, node->lastAccessTime);
}

STATIC VOID SortQueueIndexArray(UINT32 *indexArray, UINT32 count)
{
    LosQueueCB queueNode = {0};
    QueueDebugCB queueDebugNode = {0};
    UINT32 index, intSave;
    IpcSortParam queueSortParam;
    queueSortParam.buf = (CHAR *)g_queueDebugArray;
    queueSortParam.ipcDebugCBSize = sizeof(QueueDebugCB);
    queueSortParam.ipcDebugCBCnt = LOSCFG_BASE_IPC_SEM_LIMIT;
    queueSortParam.sortElemOff = OFFSET_OF_FIELD(QueueDebugCB, lastAccessTime);

    if (count > 0) {
        SCHEDULER_LOCK(intSave);
        OsArraySortByTime(indexArray, 0, count - 1, &queueSortParam, QueueCompareValue);
        SCHEDULER_UNLOCK(intSave);
        for (index = 0; index < count; index++) {
            SCHEDULER_LOCK(intSave);
            (VOID)memcpy_s(&queueNode, sizeof(LosQueueCB),
                           GET_QUEUE_HANDLE(indexArray[index]), sizeof(LosQueueCB));
            (VOID)memcpy_s(&queueDebugNode, sizeof(QueueDebugCB),
                           &g_queueDebugArray[indexArray[index]], sizeof(QueueDebugCB));
            SCHEDULER_UNLOCK(intSave);
            if (queueNode.queueState == OS_QUEUE_UNUSED) {
                continue;
            }
            OsQueueInfoOutPut(&queueNode);
            OsQueueOpsOutput(&queueDebugNode);
        }
    }
    (VOID)LOS_MemFree((VOID *)OS_SYS_MEM_ADDR, indexArray);
}

VOID OsQueueCheck(VOID)
{
    LosQueueCB queueNode = {0};
    QueueDebugCB queueDebugNode = {0};
    UINT32 index, intSave;
    UINT32 count = 0;

    /*
     * This return value does not need to be judged immediately,
     * and the following code logic has already distinguished the return value from null and non-empty,
     * and there is no case of accessing the null pointer.
     */
    UINT32 *indexArray = (UINT32 *)LOS_MemAlloc((VOID *)OS_SYS_MEM_ADDR, LOSCFG_BASE_IPC_QUEUE_LIMIT * sizeof(UINT32));

    for (index = 0; index < LOSCFG_BASE_IPC_QUEUE_LIMIT; index++) {
        SCHEDULER_LOCK(intSave);
        (VOID)memcpy_s(&queueNode, sizeof(LosQueueCB),
                       GET_QUEUE_HANDLE(index), sizeof(LosQueueCB));
        (VOID)memcpy_s(&queueDebugNode, sizeof(QueueDebugCB),
                       &g_queueDebugArray[index], sizeof(QueueDebugCB));
        SCHEDULER_UNLOCK(intSave);
        if ((queueNode.queueState == OS_QUEUE_UNUSED) ||
            ((queueNode.queueState == OS_QUEUE_INUSED) && (queueDebugNode.creater == NULL))) {
            continue;
        }
        if ((queueNode.queueState == OS_QUEUE_INUSED) &&
            (queueNode.queueLen == queueNode.readWriteableCnt[OS_QUEUE_WRITE]) &&
            LOS_ListEmpty(&queueNode.readWriteList[OS_QUEUE_READ]) &&
            LOS_ListEmpty(&queueNode.readWriteList[OS_QUEUE_WRITE]) &&
            LOS_ListEmpty(&queueNode.memList)) {
            PRINTK("Queue ID <0x%x> may leak, No task uses it, "
                   "QueueLen is 0x%x, ",
                   queueNode.queueID,
                   queueNode.queueLen);
            OsQueueOpsOutput(&queueDebugNode);
        } else {
            if (indexArray != NULL) {
                *(indexArray + count) = index;
                count++;
            } else {
                OsQueueInfoOutPut(&queueNode);
                OsQueueOpsOutput(&queueDebugNode);
            }
        }
    }

    if (indexArray != NULL) {
        SortQueueIndexArray(indexArray, count);
    }

    return;
}

#ifdef LOSCFG_SHELL
LITE_OS_SEC_TEXT_MINOR UINT32 OsShellCmdQueueInfoGet(UINT32 argc, const CHAR **argv)
{
    if (argc > 0) {
        PRINTK("\nUsage: queue\n");
        return OS_ERROR;
    }
    PRINTK("used queues information: \n");
    OsQueueCheck();
    return LOS_OK;
}

SHELLCMD_ENTRY(queue_shellcmd, CMD_TYPE_EX, "queue", 0, (CmdCallBackFunc)OsShellCmdQueueInfoGet);
#endif /* LOSCFG_SHELL */
#endif /* LOSCFG_DEBUG_QUEUE */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
