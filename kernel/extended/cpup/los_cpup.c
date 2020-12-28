/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description : LiteOS Cpu Usage Calculation Module Implementation
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

#include "los_cpup_pri.h"
#include "los_task_pri.h"
#include "los_base.h"
#include "los_swtmr.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_KERNEL_CPUP

LITE_OS_SEC_BSS STATIC UINT16 g_cpupSwtmrId;
LITE_OS_SEC_BSS STATIC UINT16 g_cpupInitFlg = 0;
LITE_OS_SEC_BSS OsCpupCB *g_cpup = NULL;
LITE_OS_SEC_BSS STATIC UINT16 g_cpupMaxNum;
LITE_OS_SEC_BSS STATIC UINT16 g_cpupTaskMaxNum;
LITE_OS_SEC_BSS STATIC UINT16 g_hisPos = 0; /* current Sampling point of historyTime */
LITE_OS_SEC_DATA_INIT STATIC UINT32 runningTasks[LOSCFG_KERNEL_CORE_NUM] = {
    [0 ... (LOSCFG_KERNEL_CORE_NUM - 1)] = (UINT32)-1
};
LITE_OS_SEC_BSS STATIC UINT64 cpuHistoryTime[OS_CPUP_HISTORY_RECORD_NUM + 1];

LITE_OS_SEC_BSS STATIC UINT64 g_startCycles = 0;
#ifdef LOSCFG_CPUP_INCLUDE_IRQ
LITE_OS_SEC_BSS UINT64 g_timeInIrqPerTskSwitch[LOSCFG_KERNEL_CORE_NUM];
LITE_OS_SEC_BSS STATIC UINT64 g_intTimeStart[LOSCFG_KERNEL_CORE_NUM];
#endif

#define HIGH_BITS 32

#define CPUP_PRE_POS(pos) (((pos) == 0) ? (OS_CPUP_HISTORY_RECORD_NUM - 1) : ((pos) - 1))
#define CPUP_POST_POS(pos) (((pos) == (OS_CPUP_HISTORY_RECORD_NUM - 1)) ? 0 : ((pos) + 1))

LITE_OS_SEC_TEXT_INIT OsCpupCB *OsCpupCBGet(UINT32 index)
{
    return &g_cpup[index];
}

LITE_OS_SEC_TEXT_INIT VOID OsCpupGuard(VOID)
{
    UINT16 prevPos = g_hisPos;
    UINT16 loop;
    UINT16 runTaskId;
    UINT64 curCycle;
    UINT32 intSave;

    if (g_cpupInitFlg == 0) {
        return;
    }
    intSave = LOS_IntLock();
    curCycle = OsGetCpuCycle();

    g_hisPos = CPUP_POST_POS(g_hisPos);
    cpuHistoryTime[prevPos] = curCycle;

    for (loop = 0; loop < g_cpupMaxNum; loop++) {
        g_cpup[loop].historyTime[prevPos] = g_cpup[loop].allTime;
    }

    for (loop = 0; loop < LOSCFG_KERNEL_CORE_NUM; loop++) {
        runTaskId = runningTasks[loop];
        /* reacquire the cycle to prevent flip */
        curCycle = OsGetCpuCycle();
        g_cpup[runTaskId].historyTime[prevPos] += curCycle - g_cpup[runTaskId].startTime;
#ifdef LOSCFG_CPUP_INCLUDE_IRQ
        g_cpup[runTaskId].historyTime[prevPos] -= g_timeInIrqPerTskSwitch[loop];
#endif
    }

    LOS_IntRestore(intSave);
}

LITE_OS_SEC_TEXT_INIT VOID OsCpupGuardCreator(VOID)
{
    (VOID)LOS_SwtmrCreate(LOSCFG_BASE_CORE_TICK_PER_SECOND, LOS_SWTMR_MODE_PERIOD,
                          (SWTMR_PROC_FUNC)OsCpupGuard, &g_cpupSwtmrId, 0);

    (VOID)LOS_SwtmrStart(g_cpupSwtmrId);
}

LITE_OS_SEC_TEXT_INIT VOID OsCpupGuardInit(VOID)
{
    TSK_INIT_PARAM_S taskInitParam;
    UINT32 tempId;

    (VOID)memset_s((void *)(&taskInitParam), sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    taskInitParam.pfnTaskEntry  = (TSK_ENTRY_FUNC)OsCpupGuardCreator;
    taskInitParam.uwStackSize   = LOS_TASK_MIN_STACK_SIZE;
    taskInitParam.pcName        = "CpupGuardCreator";
    taskInitParam.usTaskPrio    = OS_TASK_PRIORITY_HIGHEST;
    taskInitParam.uwResved      = LOS_TASK_STATUS_DETACHED;
#if (LOSCFG_KERNEL_SMP == YES)
    taskInitParam.usCpuAffiMask = CPUID_TO_AFFI_MASK(ArchCurrCpuid());
#endif
    (VOID)LOS_TaskCreate(&tempId, &taskInitParam);
}

/*
 * Description: initialization of CPUP
 * Return     : LOS_OK or Error Information
 */
LITE_OS_SEC_TEXT_INIT UINT32 OsCpupInit(VOID)
{
    UINT32 size;

    g_cpupTaskMaxNum = g_taskMaxNum;
    g_cpupMaxNum = g_cpupTaskMaxNum;
#ifdef LOSCFG_CPUP_INCLUDE_IRQ
    g_cpupMaxNum += OS_HWI_MAX_NUM;
#endif

    /* every task has only one record, and it won't operated at the same time */
    size = g_cpupMaxNum * sizeof(OsCpupCB);
    g_cpup = (OsCpupCB *)LOS_MemAlloc(m_aucSysMem0, size);
    if (g_cpup == NULL) {
        return LOS_ERRNO_CPUP_NO_MEMORY;
    }

    OsCpupGuardInit();

    (VOID)memset_s(g_cpup, size, 0, size);
    g_cpupInitFlg = 1;

    return LOS_OK;
}

LITE_OS_SEC_TEXT_INIT VOID LOS_CpupReset(VOID)
{
    UINT32 cpupIndex;
    UINT32  maxNum = g_cpupMaxNum;
    UINT64 curCycle;
    UINT16 loop;
    UINT32 intSave;

    if (g_cpup == NULL) {
        return;
    }

    g_cpupInitFlg = 0;
    intSave = LOS_IntLock();
    (VOID)LOS_SwtmrStop(g_cpupSwtmrId);
    curCycle = OsGetCpuCycle();

    for (loop = 0; loop < (OS_CPUP_HISTORY_RECORD_NUM + 1); loop++) {
        cpuHistoryTime[loop] = curCycle;
    }

    for (cpupIndex = 0; cpupIndex < maxNum; cpupIndex++) {
        g_cpup[cpupIndex].startTime = curCycle;
        g_cpup[cpupIndex].allTime = curCycle;
        for (loop = 0; loop < (OS_CPUP_HISTORY_RECORD_NUM + 1); loop++) {
            g_cpup[cpupIndex].historyTime[loop] = curCycle;
        }
    }

#ifdef LOSCFG_CPUP_INCLUDE_IRQ
    for (loop = 0; loop < LOSCFG_KERNEL_CORE_NUM; loop++) {
        g_timeInIrqPerTskSwitch[loop] = 0;
    }
#endif

    (VOID)LOS_SwtmrStart(g_cpupSwtmrId);
    LOS_IntRestore(intSave);
    g_cpupInitFlg = 1;

    return;
}

LITE_OS_SEC_TEXT_MINOR VOID OsSetCpuCycle(UINT64 startCycles)
{
    g_startCycles = startCycles;
    return;
}

/*
 * Description: get current cycles count
 * Return     : current cycles count
 */
LITE_OS_SEC_TEXT_MINOR UINT64 OsGetCpuCycle(VOID)
{
    UINT32 high;
    UINT32 low;
    UINT64 cycles;

    LOS_GetCpuCycle(&high, &low);
    cycles = ((UINT64)high << HIGH_BITS) + low;
    if (g_startCycles == 0) {
        g_startCycles = cycles;
    }

    /*
     * The cycles should keep growing, if the checking failed,
     * it mean LOS_GetCpuCycle has the problem which should be fixed.
     */
    LOS_ASSERT(cycles >= g_startCycles);

    return (cycles - g_startCycles);
}

/*
 * Description: start task to get cycles count in current task beginning
 */
LITE_OS_SEC_TEXT_MINOR VOID OsTaskCycleStart(VOID)
{
    UINT32 taskId;
    LosTaskCB *runTask = NULL;

    if (g_cpupInitFlg == 0) {
        return;
    }

    runTask = OsCurrTaskGet();
    taskId = runTask->taskId;

    g_cpup[taskId].id = taskId;
    g_cpup[taskId].startTime = OsGetCpuCycle();

    return;
}

/*
 * Description: quit task and get cycle count
 */
LITE_OS_SEC_TEXT_MINOR VOID OsTaskCycleEnd(VOID)
{
    UINT32 taskId;
    UINT64 cpuCycle;
    LosTaskCB *runTask = NULL;

    if (g_cpupInitFlg == 0) {
        return;
    }

    runTask = OsCurrTaskGet();
    taskId = runTask->taskId;

    if (g_cpup[taskId].startTime == 0) {
        return;
    }

    cpuCycle = OsGetCpuCycle();
    g_cpup[taskId].allTime += cpuCycle - g_cpup[taskId].startTime;

#ifdef LOSCFG_CPUP_INCLUDE_IRQ
    UINT32 cpuId = ArchCurrCpuid();
    g_cpup[taskId].allTime -= g_timeInIrqPerTskSwitch[cpuId];
    g_timeInIrqPerTskSwitch[cpuId] = 0;
#endif
    g_cpup[taskId].startTime = 0;

    return;
}

/*
 * Description: start task to get cycles count in current task ending
 */
LITE_OS_SEC_TEXT_MINOR VOID OsTaskCycleEndStart(const LosTaskCB *newTask)
{
    UINT64 cpuCycle;
    LosTaskCB *runTask = NULL;
    OsCpupCB *cpup = NULL;
    UINT32 cpuId = ArchCurrCpuid();

    if ((g_cpupInitFlg == 0) || (newTask == NULL)) {
        return;
    }

    runTask = OsCurrTaskGet();
    cpuCycle = OsGetCpuCycle();

    cpup = &g_cpup[runTask->taskId];
    if (cpup->startTime != 0) {
        cpup->allTime += cpuCycle - cpup->startTime;
#ifdef LOSCFG_CPUP_INCLUDE_IRQ
        cpup->allTime -= g_timeInIrqPerTskSwitch[cpuId];
        g_timeInIrqPerTskSwitch[cpuId] = 0;
#endif
    }

    cpup = &g_cpup[newTask->taskId];
    cpup->id = newTask->taskId;
    cpup->startTime = cpuCycle;
    runningTasks[cpuId] = newTask->taskId;

    return;
}

LITE_OS_SEC_TEXT_MINOR STATIC VOID OsCpupGetPos(UINT32 mode, UINT16 *curPosPointer, UINT16 *prePosPointer)
{
    UINT16 curPos;
    UINT16 tmpPos;
    UINT16 prePos;

    tmpPos = g_hisPos;
    curPos = CPUP_PRE_POS(tmpPos);

    /*
     * The current position has nothing to do with the CPUP modes,
     * however, the previous position differs.
     */
    switch (mode) {
        case CPUP_LAST_ONE_SECONDS:
            prePos = CPUP_PRE_POS(curPos);
            break;
        case CPUP_LAST_TEN_SECONDS:
            prePos = tmpPos;
            break;
        case CPUP_ALL_TIME:
            /* fall-through */
        default:
            prePos = OS_CPUP_HISTORY_RECORD_NUM;
            break;
    }

    *curPosPointer = curPos;
    *prePosPointer = prePos;

    return;
}

LITE_OS_SEC_TEXT_MINOR STATIC INLINE UINT32 OsCpuUsageParaCheck(UINT32 taskId)
{
    if (g_cpupInitFlg == 0) {
        return LOS_ERRNO_CPUP_NO_INIT;
    }

    if (OS_TSK_GET_INDEX(taskId) >= g_taskMaxNum) {
        return LOS_ERRNO_CPUP_TSK_ID_INVALID;
    }

    /* weather the task is created */
    if (g_cpup[taskId].id != taskId) {
        return LOS_ERRNO_CPUP_THREAD_NO_CREATED;
    }

    if ((g_cpup[taskId].status & OS_TASK_STATUS_UNUSED) || (g_cpup[taskId].status == 0)) {
        return LOS_ERRNO_CPUP_THREAD_NO_CREATED;
    }

    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 LOS_HistorySysCpuUsage(UINT32 mode)
{
    UINT64 cpuCycleAll;
    UINT64 idleCycleAll = 0;
    UINT32 cpup = 0;
    UINT16 pos;
    UINT16 prePos;
    UINT32 intSave;
    UINT32 idleTaskId;
#if (LOSCFG_KERNEL_SMP == YES)
    UINT32 cpuId = 0;
#endif

    if (g_cpupInitFlg == 0) {
        return LOS_ERRNO_CPUP_NO_INIT;
    }

    /* get end time of current task */
    intSave = LOS_IntLock();
    OsTaskCycleEnd();

    OsCpupGetPos(mode, &pos, &prePos);
    cpuCycleAll = cpuHistoryTime[pos] - cpuHistoryTime[prePos];

#if (LOSCFG_KERNEL_SMP == YES)
    /* For SMP system, each idle task needs to be accounted */
    while (cpuId < LOSCFG_KERNEL_CORE_NUM) {
        idleTaskId = g_percpu[cpuId].idleTaskId;
        idleCycleAll += g_cpup[idleTaskId].historyTime[pos] - g_cpup[idleTaskId].historyTime[prePos];
        cpuId++;
    }
    cpuCycleAll *= LOSCFG_KERNEL_CORE_NUM;
#else
    idleTaskId = OsGetIdleTaskId();
    idleCycleAll = g_cpup[idleTaskId].historyTime[pos] - g_cpup[idleTaskId].historyTime[prePos];
#endif

    if (cpuCycleAll) {
        cpup = (LOS_CPUP_PRECISION - (UINT32)((LOS_CPUP_PRECISION * idleCycleAll) / cpuCycleAll));
    }

    OsTaskCycleStart();
    LOS_IntRestore(intSave);

    return cpup;
}

LITE_OS_SEC_TEXT_MINOR UINT32 LOS_HistoryTaskCpuUsage(UINT32 taskId, UINT32 mode)
{
    UINT64 cpuCycleAll;
    UINT64 cpuCycleCurTask;
    UINT16 pos;
    UINT16 prePos;
    UINT32 intSave;
    UINT32 cpup = 0;
    UINT32 ret;

    if (g_cpupInitFlg == 0) {
        return LOS_ERRNO_CPUP_NO_INIT;
    }

    ret = OsCpuUsageParaCheck(taskId);
    if (ret != LOS_OK) {
        return ret;
    }
    OsCpupCB *taskCpup = &g_cpup[taskId];

    intSave = LOS_IntLock();
    OsTaskCycleEnd();

    OsCpupGetPos(mode, &pos, &prePos);
    cpuCycleAll = cpuHistoryTime[pos] - cpuHistoryTime[prePos];
    cpuCycleCurTask = taskCpup->historyTime[pos] - taskCpup->historyTime[prePos];
    if (cpuCycleAll) {
        cpup = (UINT32)((LOS_CPUP_PRECISION * cpuCycleCurTask) / cpuCycleAll);
    }

    OsTaskCycleStart();
    LOS_IntRestore(intSave);

    return cpup;
}

LITE_OS_SEC_TEXT_MINOR UINT32 LOS_AllCpuUsage(UINT16 maxNum, CPUP_INFO_S *cpupInfo, UINT32 mode, UINT16 flag)
{
    UINT16 loop;
    UINT16 pos;
    UINT16 prePos;
    UINT32 intSave;
    UINT64 cpuCycleAll;
    UINT64 cpuCycleCurTask;
    UINT16 numTmpMax = maxNum;
    UINT16 numTmpMin = 0;
    UINT16 numMax = g_cpupTaskMaxNum;

    if (g_cpupInitFlg == 0) {
        return  LOS_ERRNO_CPUP_NO_INIT;
    }

    if (cpupInfo == NULL) {
        return LOS_ERRNO_CPUP_TASK_PTR_NULL;
    }

    if (maxNum == 0) {
        return  LOS_ERRNO_CPUP_MAXNUM_INVALID;
    }

#ifdef LOSCFG_CPUP_INCLUDE_IRQ
    if (flag == 0) {
        numTmpMax += g_cpupTaskMaxNum;
        numTmpMin += g_cpupTaskMaxNum;
        numMax = g_cpupMaxNum;
    }
#endif

    if (numTmpMax > numMax) {
        numTmpMax = numMax;
    }

    intSave = LOS_IntLock();
    OsTaskCycleEnd();

    OsCpupGetPos(mode, &pos, &prePos);
    cpuCycleAll = cpuHistoryTime[pos] - cpuHistoryTime[prePos];

    for (loop = numTmpMin; loop < numTmpMax; loop++) {
        if ((g_cpup[loop].status & OS_TASK_STATUS_UNUSED) || (g_cpup[loop].status == 0)) {
            continue;
        }

        cpuCycleCurTask = g_cpup[loop].historyTime[pos] - g_cpup[loop].historyTime[prePos];
        cpupInfo[loop - numTmpMin].usStatus = g_cpup[loop].status;

        if (cpuCycleAll) {
            cpupInfo[loop - numTmpMin].uwUsage = (UINT32)((LOS_CPUP_PRECISION * cpuCycleCurTask) / cpuCycleAll);
        }
    }

    OsTaskCycleStart();
    LOS_IntRestore(intSave);

    return LOS_OK;
}

#ifdef LOSCFG_CPUP_INCLUDE_IRQ
LITE_OS_SEC_TEXT_MINOR VOID OsCpupIrqStart(VOID)
{
    g_intTimeStart[ArchCurrCpuid()] = OsGetCpuCycle();
    return;
}

LITE_OS_SEC_TEXT_MINOR VOID OsCpupIrqEnd(UINT32 intNum)
{
    UINT64 intTimeEnd = OsGetCpuCycle();
    UINT32 cpuId = ArchCurrCpuid();

    if (g_cpupInitFlg == 0) {
        return;
    }

    g_cpup[g_taskMaxNum + intNum].id = intNum;
    g_cpup[g_taskMaxNum + intNum].status = OS_TASK_STATUS_RUNNING;
    g_timeInIrqPerTskSwitch[cpuId] += (intTimeEnd - g_intTimeStart[cpuId]);
    g_cpup[g_taskMaxNum + intNum].allTime += (intTimeEnd - g_intTimeStart[cpuId]);

    return;
}
#endif

#endif /* LOSCFG_KERNEL_CPUP */
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

