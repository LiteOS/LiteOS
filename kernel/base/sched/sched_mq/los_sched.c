/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
 * Description: Scheduler
 * Author: Huawei LiteOS Team
 * Create: 2018-12-06
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

#include "los_base.h"
#include "los_trace.h"
#include "los_task_pri.h"
#include "los_priqueue_pri.h"
#include "los_percpu_pri.h"
#include "los_mux_debug_pri.h"
#ifdef LOSCFG_KERNEL_CPUP
#include "los_cpup_pri.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* number of priority queues */
#define NPQS            32
#define PRI0_BIT        (UINT32)(0x1U << (NPQS - 1))

#define BALANCE_PERIOD  200

typedef enum {
    QUEUE_HEAD = 1,
    QUEUE_TAIL = 2,
} QueueOps;

typedef struct {
    UINT32          bitmap;           /* priority-queue bitmap */
    LOS_DL_LIST     queues[NPQS];     /* one for each priority */
    SPIN_LOCK_S     lock;             /* private spinlock */
} PriQueue;

/* priority queue per-cpu */
STATIC PriQueue g_priqueue[LOSCFG_KERNEL_CORE_NUM];

STATIC UINT32 g_activeCpu = LOSCFG_KERNEL_CPU_MASK;

STATIC INLINE UINT32 OsMpIdleMask(VOID)
{
    return ((~g_activeCpu) & LOSCFG_KERNEL_CPU_MASK);
}

STATIC INLINE VOID OsMpSetIdle(UINT32 cpu)
{
    g_activeCpu &= ~CPUID_TO_AFFI_MASK(cpu);
}

STATIC INLINE VOID OsMpSetActive(UINT32 cpu)
{
    g_activeCpu |= CPUID_TO_AFFI_MASK(cpu);
}

VOID OsPriQueueInit(VOID)
{
    LOS_DL_LIST *priQueues = NULL;
    UINT32 priority;
    UINT32 cpuid = 0;

    (VOID)memset_s(g_priqueue, sizeof(PriQueue) * LOSCFG_KERNEL_CORE_NUM,
                   0, sizeof(PriQueue) * LOSCFG_KERNEL_CORE_NUM);

    while (cpuid < LOSCFG_KERNEL_CORE_NUM) {
        priority = 0;
        priQueues = &g_priqueue[cpuid++].queues[0];

        while (priority < NPQS) {
            LOS_ListInit(&priQueues[priority++]);
        }
    }
}

UINT32 OsPriQueueSize(UINT32 priority)
{
    UINT32      itemCnt = 0;
    LOS_DL_LIST *queueNode = NULL;
    LOS_DL_LIST *priQueues = NULL;

    priQueues = &g_priqueue[ArchCurrCpuid()].queues[0];

    LOS_DL_LIST_FOR_EACH(queueNode, &priQueues[priority]) {
        ++itemCnt;
    }

    return itemCnt;
}

UINT32 OsPriQueueTotalSize(VOID)
{
    UINT32 priority;
    UINT32 totalSize = 0;

    /* current pri-queue */
    for (priority = 0; priority < NPQS; ++priority) {
        totalSize += OsPriQueueSize(priority);
    }

    return totalSize;
}

UINT32 OsSchedPickCpu(LosTaskCB* task)
{
    UINT32 cpuid, lastMask, cpuidMask, affinity;
    UINT32 idleMask = OsMpIdleMask();

    affinity  = (UINT32)(task->cpuAffiMask);
    cpuid     = CTZ(affinity);
    lastMask  = (task->lastCpu != -1) ?
                CPUID_TO_AFFI_MASK(task->lastCpu) : 0;
    cpuidMask = CPUID_TO_AFFI_MASK(cpuid);
    /* 1 binded on one core, directly return */
    if (!(affinity & ~cpuidMask)) {
        return cpuid;
    }
    /* 2 there's idled core, choose from them */
    if (idleMask) {
        if (idleMask & lastMask) {
            /* 2.1 last ever runned cpu is primary choice */
            cpuid = task->lastCpu;
        } else {
            /* 2.2 never ever runned, choose first idled core,
               To be supported: check if randomness is needed */
            cpuid = CTZ(idleMask);
        }
    } else {
        /* To be supported: */
        /* there's no idled core, needs choose from affinitied core's loads */
        cpuid = 0;
    }

    PRINT_TRACE("cpu(%u) pick cpu(%u) to run task:%s\n", ArchCurrCpuid(), cpuid, task->taskName);

    return cpuid;
}

VOID OsPriQueueAdd(PriQueue *priQueue, LosTaskCB* task, QueueOps flag)
{
    LOS_DL_LIST *priQueues = NULL;
    UINT32      *bitmap = NULL;
    UINT32      priority;

    priQueues = &priQueue->queues[0];
    bitmap    = &priQueue->bitmap;
    priority  = task->priority;

    if (LOS_ListEmpty(&priQueues[priority])) {
        (*bitmap) |= (PRI0_BIT >> priority);
    }

    if (flag == QUEUE_HEAD) {
        LOS_ListHeadInsert(&priQueues[priority], &task->pendList);
    } else {
        LOS_ListTailInsert(&priQueues[priority], &task->pendList);
    }
}

/*
 * Insert a task to the head of priority queue, only available
 * when putting running task back to the ready queue.
 */
VOID OsPriQueueEnqueueHead(LOS_DL_LIST *queueNode, UINT32 priority)
{
    LosTaskCB *task = LOS_DL_LIST_ENTRY(queueNode, LosTaskCB, pendList);
    OsPriQueueAdd(&g_priqueue[ArchCurrCpuid()], task, QUEUE_HEAD);
}


VOID OsPriQueueEnqueueTail(LOS_DL_LIST *queueNode, UINT32 priority)
{
    LosTaskCB *task = LOS_DL_LIST_ENTRY(queueNode, LosTaskCB, pendList);
    OsPriQueueAdd(&g_priqueue[ArchCurrCpuid()], task, QUEUE_TAIL);
}

/*
 * Insert a task to the tail of priority queue. The pri-queue might be
 * this or another core's.
 */
VOID OsPriQueueEnqueue(LOS_DL_LIST *queueNode, UINT32 priority)
{
    LosTaskCB   *task = NULL;
    LOS_DL_LIST *priQueues = NULL;
    UINT32      cpuid;

    task = LOS_DL_LIST_ENTRY(queueNode, LosTaskCB, pendList);

    /* choose an appropriate cpu to run */
    cpuid = OsSchedPickCpu(task);

    /* add the task to the cpu per-core pri-queue */
    OsPriQueueAdd(&g_priqueue[cpuid], task, QUEUE_TAIL);
}

VOID OsPriQueueDequeue(LOS_DL_LIST *queueNode)
{
    LosTaskCB   *task = NULL;
    LOS_DL_LIST *priQueues = NULL;
    UINT32      *bitmap = NULL;
    UINT32      cpuid;

    task = LOS_DL_LIST_ENTRY(queueNode, LosTaskCB, pendList);

    cpuid     = ArchCurrCpuid();
    priQueues = &g_priqueue[cpuid].queues[0];
    bitmap    = &g_priqueue[cpuid].bitmap;

    LOS_ListDelete(queueNode);
    if (LOS_ListEmpty(&priQueues[task->priority])) {
        (*bitmap) &= ~(PRI0_BIT >> task->priority);
    }
}

LITE_OS_SEC_TEXT_MINOR LosTaskCB *OsGetTopTask(VOID)
{
    LosTaskCB *newTask = NULL;
    LOS_DL_LIST *priQueues = NULL;
    UINT32 priority;
    UINT32 bitmap, cpuid;

    cpuid     = ArchCurrCpuid();
    priQueues = g_priqueue[cpuid].queues;
    bitmap    = g_priqueue[cpuid].bitmap;

    while (bitmap) {
        priority = CLZ(bitmap);

        LOS_DL_LIST_FOR_EACH_ENTRY(newTask, &priQueues[priority], LosTaskCB, pendList) {
            if (newTask->cpuAffiMask & (1U << cpuid)) {
                OsPriQueueDequeue(&newTask->pendList);
                goto out;
            }
        }

        bitmap &= ~(1U << (NPQS - priority - 1));
    }

out:
    return newTask;
}

VOID OsSchedResched(VOID)
{
    LosTaskCB *runTask = NULL;
    LosTaskCB *newTask = NULL;
    UINT32 cpuid = ArchCurrCpuid();

    LOS_ASSERT(LOS_SpinHeld(&g_taskSpin));

    if (!OsPreemptableInSched()) {
        return;
    }

    runTask = OsCurrTaskGet();
    newTask = OsGetTopTask();

    newTask->taskStatus &= ~OS_TASK_STATUS_READY;

    if (runTask == newTask) {
        return;
    }

    runTask->taskStatus &= ~OS_TASK_STATUS_RUNNING;
    newTask->taskStatus |= OS_TASK_STATUS_RUNNING;

#ifdef LOSCFG_KERNEL_SMP
    /* mask new running task's owner processor */
    runTask->currCpu = -1;
    newTask->currCpu = cpuid;
    newTask->lastCpu = cpuid;
#endif

    OsTaskTimeUpdateHook(runTask->taskId, LOS_TickCountGet());

#ifdef LOSCFG_KERNEL_CPUP
    OsTaskCycleEndStart(newTask);
#endif

#ifdef LOSCFG_BASE_CORE_TSK_MONITOR
    OsTaskSwitchCheck(runTask, newTask);
#endif

    LOS_TRACE(TASK_SWITCH, newTask->taskId, runTask->priority, runTask->taskStatus, newTask->priority,
        newTask->taskStatus);

#ifdef LOSCFG_DEBUG_SCHED_STATISTICS
    OsSchedStatistics(runTask, newTask);
#endif

    if (newTask->taskId == OsPercpuGet()->idleTaskId) {
        OsMpSetIdle(cpuid);
    } else {
        OsMpSetActive(cpuid);
    }

    PRINT_TRACE("cpu%u (%s) status: %x -> (%s) status:%x\n", ArchCurrCpuid(),
                runTask->taskName, runTask->taskStatus,
                newTask->taskName, newTask->taskStatus);

#ifdef LOSCFG_BASE_CORE_TIMESLICE
    newTask->timeSlice = LOSCFG_BASE_CORE_TIMESLICE_TIMEOUT;
#endif

    OsCurrTaskSet((VOID*)newTask);

    /* do the task context switch */
    OsTaskSchedule(newTask, runTask);
}

VOID OsSchedPreempt(VOID)
{
    LosTaskCB *runTask = NULL;
    UINT32 intSave;

    if (!OsPreemptable()) {
        return;
    }

    SCHEDULER_LOCK(intSave);

    /* add run task back to ready queue */
    runTask = OsCurrTaskGet();
    runTask->taskStatus |= OS_TASK_STATUS_READY;

#ifdef LOSCFG_BASE_CORE_TIMESLICE
    if (runTask->timeSlice == 0) {
        OsPriQueueEnqueue(&runTask->pendList, runTask->priority);
    } else {
#endif
        OsPriQueueEnqueueHead(&runTask->pendList, runTask->priority);
#ifdef LOSCFG_BASE_CORE_TIMESLICE
    }
#endif

    /* reschedule to new thread */
    OsSchedResched();

    SCHEDULER_UNLOCK(intSave);
}

#ifdef LOSCFG_BASE_CORE_TIMESLICE
LITE_OS_SEC_TEXT VOID OsTimesliceCheck(VOID)
{
    LosTaskCB *runTask = OsCurrTaskGet();
    if (runTask->timeSlice != 0) {
        runTask->timeSlice--;
        if (runTask->timeSlice == 0) {
            LOS_Schedule();
        }
    }
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
