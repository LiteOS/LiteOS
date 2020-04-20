/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
 * Description: Scheduler
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

#include "los_base.h"
#include "los_task_pri.h"
#include "los_priqueue_pri.h"

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

    (VOID)OsTaskSwitchCheck(runTask, newTask);

    PRINT_TRACE("cpu%d (%s) status: %x -> (%s) status:%x\n", ArchCurrCpuid(),
                runTask->taskName, runTask->taskStatus,
                newTask->taskName, newTask->taskStatus);

#if (LOSCFG_BASE_CORE_TIMESLICE == YES)
    if (newTask->timeSlice == 0) {
        newTask->timeSlice = LOSCFG_BASE_CORE_TIMESLICE_TIMEOUT;
    }
#endif

    OsCurrTaskSet(newTask);

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

#if (LOSCFG_BASE_CORE_TIMESLICE == YES)
    if (runTask->timeSlice == 0) {
        OsPriQueueEnqueue(&runTask->pendList, runTask->priority);
    } else {
#endif
        OsPriQueueEnqueueHead(&runTask->pendList, runTask->priority);
#if (LOSCFG_BASE_CORE_TIMESLICE == YES)
    }
#endif

    /* reschedule to new thread */
    OsSchedResched();

    SCHEDULER_UNLOCK(intSave);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
