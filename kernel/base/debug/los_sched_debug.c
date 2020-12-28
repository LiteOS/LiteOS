/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
 * Description: Schedule Statistics
 * Author: Huawei LiteOS Team
 * Create: 2018-11-16
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

#include "los_task_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_DEBUG_SCHED_STATISTICS
#define HIGHTASKPRI           16
#define NS_PER_MS             1000000
#define DECIMAL_TO_PERCENTAGE 100

typedef struct {
    UINT64      idleRuntime;
    UINT64      idleStarttime;
    UINT64      highTaskRuntime;
    UINT64      highTaskStarttime;
    UINT64      sumPriority;
    UINT32      prioritySwitch;
    UINT32      highTaskSwitch;
    UINT32      contexSwitch;
    UINT32      hwiNum;
#ifdef LOSCFG_KERNEL_SMP
    UINT32      ipiIrqNum;
#endif
} StatPercpu;

STATIC BOOL g_statisticsStartFlag = FALSE;
STATIC UINT64 g_statisticsStartTime;
STATIC StatPercpu g_statPercpu[LOSCFG_KERNEL_CORE_NUM] = {0};

STATIC VOID OsSchedStatisticsPerCpu(const LosTaskCB *runTask, const LosTaskCB *newTask)
{
    UINT32 cpuId;
    UINT32 idleTaskId;
    UINT64 now, runtime;

    if (g_statisticsStartFlag != TRUE) {
        return;
    }

    cpuId = ArchCurrCpuid();
    idleTaskId = OsGetIdleTaskId();
    now = LOS_CurrNanosec();

    g_statPercpu[cpuId].contexSwitch++;

    if ((runTask->taskId != idleTaskId) && (newTask->taskId == idleTaskId)) {
        g_statPercpu[cpuId].idleStarttime = now;
    }

    if ((runTask->taskId == idleTaskId) && (newTask->taskId != idleTaskId)) {
        runtime = now - g_statPercpu[cpuId].idleStarttime;
        g_statPercpu[cpuId].idleRuntime += runtime;
        g_statPercpu[cpuId].idleStarttime = 0;
    }

    if ((runTask->priority >= HIGHTASKPRI) && (newTask->priority < HIGHTASKPRI)) {
        g_statPercpu[cpuId].highTaskStarttime = now;
    }

    if ((runTask->priority < HIGHTASKPRI) && (newTask->priority >= HIGHTASKPRI)) {
        runtime = now - g_statPercpu[cpuId].highTaskStarttime;
        g_statPercpu[cpuId].highTaskRuntime += runtime;
        g_statPercpu[cpuId].highTaskStarttime = 0;
    }

    if (newTask->priority < HIGHTASKPRI) {
        g_statPercpu[cpuId].highTaskSwitch++;
    }

    if (newTask->taskId != idleTaskId) {
        g_statPercpu[cpuId].sumPriority += newTask->priority;
        g_statPercpu[cpuId].prioritySwitch++;
    }

    return;
}

LITE_OS_SEC_TEXT_MINOR VOID OsSchedStatistics(LosTaskCB *runTask, LosTaskCB *newTask)
{
    UINT64 runtime;
    UINT32 cpuId = ArchCurrCpuid();
    UINT64 now = LOS_CurrNanosec();

    SchedStat *schedRun = &runTask->schedStat;
    SchedStat *schedNew = &newTask->schedStat;
    SchedPercpu *cpuRun = &schedRun->schedPercpu[cpuId];
    SchedPercpu *cpuNew = &schedNew->schedPercpu[cpuId];

    /* calculate one chance of running time */
    runtime = now - schedRun->startRuntime;

    /* add running timer to running task statistics */
    cpuRun->runtime += runtime;
    schedRun->allRuntime += runtime;

    /* add context switch counters and schedule start time */
    cpuNew->contexSwitch++;
    schedNew->allContextSwitch++;
    schedNew->startRuntime = now;
    OsSchedStatisticsPerCpu(runTask, newTask);
}

LITE_OS_SEC_TEXT_MINOR VOID OsHwiStatistics(size_t intNum)
{
    UINT32 cpuId = ArchCurrCpuid();

    if ((g_statisticsStartFlag != TRUE) || (intNum == OS_TICK_INT_NUM)) {
        return;
    }

    g_statPercpu[cpuId].hwiNum++;
#ifdef LOSCFG_KERNEL_SMP
    /* 16: 0~15 is ipi interrupts */
    if (intNum < 16) {
        g_statPercpu[cpuId].ipiIrqNum++;
    }
#endif
    return;
}

LITE_OS_SEC_TEXT_MINOR VOID OsShellCmdDumpSched(VOID)
{
    LosTaskCB *taskCB = NULL;
    UINT32 loop;
    UINT32 cpuId;
#ifdef LOSCFG_KERNEL_SMP
    UINT32 affinity;
#endif

    PRINTK("\n");
    PRINTK("Task                          TID              Total Time     Total CST     "
           "CPU                   Time           CST\n");
    PRINTK("----                          ---      ------------------    ----------    -"
           "---     ------------------    ----------\n");

    for (loop = 0; loop < g_taskMaxNum; loop++) {
        taskCB = (((LosTaskCB *)g_taskCBArray) + loop);
        if (taskCB->taskStatus & OS_TASK_STATUS_UNUSED) {
            continue;
        }
#ifdef LOSCFG_KERNEL_SMP
        affinity = (UINT32)taskCB->cpuAffiMask;
#endif
        PRINTK("%-30s0x%-6x%+16lf ms  %10d\n", taskCB->taskName, taskCB->taskId,
               (DOUBLE)(taskCB->schedStat.allRuntime) / NS_PER_MS,
               taskCB->schedStat.allContextSwitch);

        for (cpuId = 0; cpuId < LOSCFG_KERNEL_CORE_NUM; cpuId++) {
#ifdef LOSCFG_KERNEL_SMP
            if (!((1U << cpuId) & affinity)) {
                continue;
            }
#endif
            PRINTK("                                                                           "
                   "CPU%d    %+16lf ms  %12d\n", cpuId,
                   (DOUBLE)(taskCB->schedStat.schedPercpu[cpuId].runtime) / NS_PER_MS,
                   taskCB->schedStat.schedPercpu[cpuId].contexSwitch);
        }
    }

    PRINTK("\n");
}

LITE_OS_SEC_TEXT_MINOR VOID OsStatisticsShow(UINT64 statisticsPastTime)
{
    UINT32 cpuId;
    PRINTK("\n");
    PRINTK("Passed Time: %+16lf ms\n", ((DOUBLE)statisticsPastTime / NS_PER_MS));
    PRINTK("--------------------------------\n");
    PRINTK("CPU       Idle(%%)      ContexSwitch    HwiNum       "
           "Avg Pri      HiTask(%%)	   HiTask SwiNum       HiTask P(ms)"
#ifdef LOSCFG_KERNEL_SMP
           "      MP Hwi\n");
#else
           "\n");
#endif
    PRINTK("----    ---------      -----------    --------    ---------     "
           "----------         ------------       ----------"
#ifdef LOSCFG_KERNEL_SMP
           "        ------\n");
#else
           "\n");
#endif

    for (cpuId = 0; cpuId < LOSCFG_KERNEL_CORE_NUM; cpuId++) {
#ifdef LOSCFG_KERNEL_SMP
        PRINTK("CPU%d   %+10lf%14d%14d   %+11lf   %+11lf%14d              %+11lf  %11d\n", cpuId,
#else
        PRINTK("CPU%d   %+10lf%14d%14d   %+11lf   %+11lf%14d              %+11lf\n", cpuId,
#endif
               ((DOUBLE)(g_statPercpu[cpuId].idleRuntime) / statisticsPastTime) * DECIMAL_TO_PERCENTAGE,
               g_statPercpu[cpuId].contexSwitch,
               g_statPercpu[cpuId].hwiNum,
               (g_statPercpu[cpuId].prioritySwitch == 0) ? OS_TASK_PRIORITY_LOWEST :
               ((DOUBLE)(g_statPercpu[cpuId].sumPriority) / (g_statPercpu[cpuId].prioritySwitch)),
               ((DOUBLE)(g_statPercpu[cpuId].highTaskRuntime) / statisticsPastTime) * DECIMAL_TO_PERCENTAGE,
               g_statPercpu[cpuId].highTaskSwitch,
               (g_statPercpu[cpuId].highTaskSwitch == 0) ? 0 :
               ((DOUBLE)(g_statPercpu[cpuId].highTaskRuntime) / (g_statPercpu[cpuId].highTaskSwitch)) / NS_PER_MS
#ifdef LOSCFG_KERNEL_SMP
               , g_statPercpu[cpuId].ipiIrqNum);
#else
               );
#endif
    }

    PRINTK("\n");
}

LITE_OS_SEC_TEXT_MINOR VOID OsShellStatisticsStart(VOID)
{
    LosTaskCB *taskCB = NULL;
    UINT32 loop;
    UINT32 cpuId = 0;
    UINT32 intSave;

    SCHEDULER_LOCK(intSave);

    if (g_statisticsStartFlag) {
        PRINT_WARN("mp static has started\n");
        SCHEDULER_UNLOCK(intSave);
        return;
    }

    g_statisticsStartTime = LOS_CurrNanosec();

    for (loop = 0; loop < g_taskMaxNum; loop++) {
        taskCB = (((LosTaskCB *)g_taskCBArray) + loop);
        if (taskCB->taskStatus & OS_TASK_STATUS_RUNNING) {
#ifdef LOSCFG_KERNEL_SMP
            cpuId = taskCB->currCpu;
#endif
            if ((UINT32)(OS_TASK_INVALID_CPUID) == cpuId) {
                continue;
            }
            if (!strcmp(taskCB->taskName, "IdleCore000")) {
                g_statPercpu[cpuId].idleStarttime = g_statisticsStartTime;
            }
            if (taskCB->priority < HIGHTASKPRI) {
                g_statPercpu[cpuId].highTaskStarttime = g_statisticsStartTime;
                g_statPercpu[cpuId].highTaskSwitch++;
            }
            if (strcmp(taskCB->taskName, "IdleCore000")) {
                g_statPercpu[cpuId].sumPriority += taskCB->priority;
                g_statPercpu[cpuId].prioritySwitch++;
            }
        }
    }
    g_statisticsStartFlag = TRUE;
    SCHEDULER_UNLOCK(intSave);

    PRINTK("mp static start\n");

    return;
}

LITE_OS_SEC_TEXT_MINOR VOID OsShellStatisticsStop(VOID)
{
    LosTaskCB *taskCB = NULL;
    UINT32 loop;
    UINT32 cpuId = 0;
    UINT64 statisticsStopTime;
    UINT64 statisticsPastTime;
    UINT64 runtime;
    UINT32 intSave;

    SCHEDULER_LOCK(intSave);

    if (g_statisticsStartFlag != TRUE) {
        PRINT_WARN("Please set mp static start\n");
        SCHEDULER_UNLOCK(intSave);
        return;
    }

    g_statisticsStartFlag = FALSE;
    statisticsStopTime = LOS_CurrNanosec();
    statisticsPastTime = statisticsStopTime - g_statisticsStartTime;

    for (loop = 0; loop < g_taskMaxNum; loop++) {
        taskCB = (((LosTaskCB *)g_taskCBArray) + loop);
        if (taskCB->taskStatus & OS_TASK_STATUS_RUNNING) {
#ifdef LOSCFG_KERNEL_SMP
            cpuId = taskCB->currCpu;
#endif
            if (cpuId == (UINT32)(OS_TASK_INVALID_CPUID)) {
                continue;
            }
            if (!strcmp(taskCB->taskName, "IdleCore000")) {
                runtime = statisticsStopTime - g_statPercpu[cpuId].idleStarttime;
                g_statPercpu[cpuId].idleRuntime += runtime;
                g_statPercpu[cpuId].idleStarttime = 0;
            }
            if (taskCB->priority < HIGHTASKPRI) {
                runtime = statisticsStopTime - g_statPercpu[cpuId].highTaskStarttime;
                g_statPercpu[cpuId].highTaskRuntime += runtime;
                g_statPercpu[cpuId].highTaskStarttime = 0;
            }
        }
    }
    SCHEDULER_UNLOCK(intSave);
    OsStatisticsShow(statisticsPastTime);

    (VOID)memset_s(g_statPercpu, sizeof(g_statPercpu), 0, sizeof(g_statPercpu));
    g_statisticsStartTime = 0;
    return;
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
