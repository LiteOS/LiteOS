/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Priority Queue
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

#include "los_priqueue_pri.h"
#include "los_task_pri.h"
#include "los_memory.h"
#include "los_toolchain.h"
#include "los_spinlock.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define OS_PRIORITY_QUEUE_NUM 32
#define PRIQUEUE_PRIOR0_BIT   0x80000000U

LITE_OS_SEC_BSS LOS_DL_LIST *g_priQueueList = NULL;
STATIC LITE_OS_SEC_BSS UINT32 g_priQueueBitmap;

UINT32 OsPriQueueInit(VOID)
{
    UINT32 priority;

    /* system resident resource */
    g_priQueueList = (LOS_DL_LIST *)LOS_MemAlloc(m_aucSysMem0, (OS_PRIORITY_QUEUE_NUM * sizeof(LOS_DL_LIST)));
    if (g_priQueueList == NULL) {
        return LOS_NOK;
    }

    for (priority = 0; priority < OS_PRIORITY_QUEUE_NUM; ++priority) {
        LOS_ListInit(&g_priQueueList[priority]);
    }

    return LOS_OK;
}

VOID OsPriQueueEnqueueHead(LOS_DL_LIST *priqueueItem, UINT32 priority)
{
    /*
     * Task control blocks are initd as zero. And when task is deleted,
     * and at the same time would be deleted from priority queue or
     * other lists, task pend node will restored as zero.
     */
    LOS_ASSERT(priqueueItem->pstNext == NULL);

    if (LOS_ListEmpty(&g_priQueueList[priority])) {
        g_priQueueBitmap |= PRIQUEUE_PRIOR0_BIT >> priority;
    }

    LOS_ListHeadInsert(&g_priQueueList[priority], priqueueItem);
}

VOID OsPriQueueEnqueue(LOS_DL_LIST *priqueueItem, UINT32 priority)
{
    /*
     * Task control blocks are initd as zero. And when task is deleted,
     * and at the same time would be deleted from priority queue or
     * other lists, task pend node will restored as zero.
     */
    LOS_ASSERT(priqueueItem->pstNext == NULL);

    if (LOS_ListEmpty(&g_priQueueList[priority])) {
        g_priQueueBitmap |= PRIQUEUE_PRIOR0_BIT >> priority;
    }

    LOS_ListTailInsert(&g_priQueueList[priority], priqueueItem);
}

VOID OsPriQueueDequeue(LOS_DL_LIST *priqueueItem)
{
    LosTaskCB *runTask = NULL;
    LOS_ListDelete(priqueueItem);

    runTask = LOS_DL_LIST_ENTRY(priqueueItem, LosTaskCB, pendList);
    if (LOS_ListEmpty(&g_priQueueList[runTask->priority])) {
        g_priQueueBitmap &= ~(PRIQUEUE_PRIOR0_BIT >> runTask->priority);
    }
}

LOS_DL_LIST *OsPriQueueTop(VOID)
{
    UINT32 priority;

    if (g_priQueueBitmap != 0) {
        priority = CLZ(g_priQueueBitmap);
        return LOS_DL_LIST_FIRST(&g_priQueueList[priority]);
    }

    return NULL;
}

UINT32 OsPriQueueSize(UINT32 priority)
{
    UINT32 itemCnt = 0;
    LOS_DL_LIST *curNode = NULL;
#ifdef LOSCFG_KERNEL_SMP
    LosTaskCB *task = NULL;
    UINT32 cpuId = ArchCurrCpuid();
#endif

    LOS_ASSERT(ArchIntLocked());
    LOS_ASSERT(LOS_SpinHeld(&g_taskSpin));

    LOS_DL_LIST_FOR_EACH(curNode, &g_priQueueList[priority]) {
#ifdef LOSCFG_KERNEL_SMP
        task = OS_TCB_FROM_PENDLIST(curNode);
        if (!(task->cpuAffiMask & (1U << cpuId))) {
            continue;
        }
#endif
        ++itemCnt;
    }

    return itemCnt;
}

LITE_OS_SEC_TEXT_MINOR LosTaskCB *OsGetTopTask(VOID)
{
    UINT32 priority;
    UINT32 bitmap;
    LosTaskCB *newTask = NULL;
#ifdef LOSCFG_KERNEL_SMP
    UINT32 cpuid = ArchCurrCpuid();
#endif

    bitmap = g_priQueueBitmap;

    while (bitmap) {
        priority = CLZ(bitmap);
        LOS_DL_LIST_FOR_EACH_ENTRY(newTask, &g_priQueueList[priority], LosTaskCB, pendList) {
#ifdef LOSCFG_KERNEL_SMP
            if (newTask->cpuAffiMask & (1U << cpuid)) {
#endif
                OsPriQueueDequeue(&newTask->pendList);
                goto OUT;
#ifdef LOSCFG_KERNEL_SMP
            }
#endif
        }
        bitmap &= ~(1U << (OS_PRIORITY_QUEUE_NUM - priority - 1));
    }

OUT:
    return newTask;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

