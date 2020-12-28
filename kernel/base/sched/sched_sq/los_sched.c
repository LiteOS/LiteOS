/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
 * Description: Scheduler
 * Author: Huawei LiteOS Team
 * Create: 2018-08-29
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
#include "los_task_pri.h"
#include "los_mux_debug_pri.h"
#ifdef LOSCFG_KERNEL_CPUP
#include "los_cpup_pri.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

VOID OsSchedResched(VOID)
{
    LosTaskCB *runTask = NULL;
    LosTaskCB *newTask = NULL;

    LOS_ASSERT(LOS_SpinHeld(&g_taskSpin));

    if (!OsPreemptableInSched()) {
        return;
    }

    runTask = OsCurrTaskGet();
    newTask = OsGetTopTask();

    /* always be able to get one task */
    LOS_ASSERT(newTask != NULL);

    newTask->taskStatus &= ~OS_TASK_STATUS_READY;

    if (runTask == newTask) {
        return;
    }

    runTask->taskStatus &= ~OS_TASK_STATUS_RUNNING;
    newTask->taskStatus |= OS_TASK_STATUS_RUNNING;

#ifdef LOSCFG_KERNEL_SMP
    /* mask new running task's owner processor */
    runTask->currCpu = OS_TASK_INVALID_CPUID;
    newTask->currCpu = ArchCurrCpuid();
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

    PRINT_TRACE("cpu%u (%s) status: %x -> (%s) status:%x\n", ArchCurrCpuid(),
                runTask->taskName, runTask->taskStatus,
                newTask->taskName, newTask->taskStatus);

#ifdef LOSCFG_BASE_CORE_TIMESLICE
    if (newTask->timeSlice == 0) {
        newTask->timeSlice = LOSCFG_BASE_CORE_TIMESLICE_TIMEOUT;
    }
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
