/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Low-power Framework.
 * Author: Huawei LiteOS Team
 * Create: 2020-09-19
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

#include "los_lowpower_impl_pri.h"
#include "los_hwi.h"
#include "los_sortlink_pri.h"
#include "los_swtmr_pri.h"
#include "los_task_pri.h"
#include "los_tickless_pri.h"
#include "los_atomic.h"
#include "los_mp_pri.h"

#ifdef LOSCFG_KERNEL_RUNSTOP
#include "los_runstop_pri.h"
#endif

#ifdef LOSCFG_KERNEL_CPUP
#include "los_cpup_pri.h"
#endif

#if defined(LOSCFG_KERNEL_RUNSTOP) || defined(LOSCFG_KERNEL_DEEPSLEEP)
/* Is system is up from the memory image, then this flag should be 1; else 0 */
#ifdef LOSCFG_AARCH64
__attribute__((section(".data"))) INT64 g_resumeFromImg = LOS_COLD_RESET;
__attribute__((section(".data"))) STATIC INT64 g_otherCoreResume = 0;
#else
__attribute__((section(".data"))) INT32 g_resumeFromImg = LOS_COLD_RESET;
__attribute__((section(".data"))) INT32 g_otherCoreResume = 0;
#endif

#ifdef LOSCFG_AARCH64
/*
 * 16: The number of aligned memory,
 * 34: The number of task context registers(X0~X30, SP, DAIF, NZCV)
 */
LITE_OS_SEC_DATA_MINOR __attribute__((aligned(16))) UINT64 g_saveSRContext[34];
/* 3: The number of available universal registers(X0, X1, X2) temporarily saved */
LITE_OS_SEC_DATA_MINOR UINT64 g_saveAR[3];
#else
/* 18: The number of task context registers(R0~R15, SPSR, CPSR) */
LITE_OS_SEC_DATA_MINOR UINT32 g_saveSRContext[LOSCFG_KERNEL_CORE_NUM][18];
/* 2: The number of available universal registers(R0, R1) temporarily saved */
LITE_OS_SEC_DATA_MINOR UINT32 g_saveAR[2];
LITE_OS_SEC_DATA_MINOR LosTaskCB *g_saveTsk[LOSCFG_KERNEL_CORE_NUM];
#endif
#endif

#ifndef OFFSET_OF_FIELD
#define OFFSET_OF_FIELD(type, field) ((UINTPTR) & ((type *)0)->field)
#endif

#define RUNOPS_CALL_FUNC_VOID(func, ...) CALL_FUNC_NO_RETURN(g_pmRunOps, func, __VA_ARGS__)
#define CALL_RUN_OPS_FUNC_NO_RETURN(func) CallVoidFunction(&g_pmRunOps, OFFSET_OF_FIELD(PowerMgrRunOps, func));
#ifdef LOSCFG_KERNEL_DEEPSLEEP
#define DEEPOPS_CALL_FUNC_VOID(func, ...) CALL_FUNC_NO_RETURN(g_deepSleepOps, func, __VA_ARGS__)
#else
#define DEEPOPS_CALL_FUNC_VOID(func, ...)
#endif

#ifdef LOSCFG_LOWPOWER_TRACE_DEBUG
#define TRACE_FUNC_CALL() PRINTK("trace: %s, %s, %d\n", __FILE__, __FUNCTION__, __LINE__)
#else
#define TRACE_FUNC_CALL()
#endif

#define MAX_SLEEP_TIME      10000
#define MIN_DEEP_SLEEP_TIME 1000
#define MIN_SLEEP_TIME      100

typedef struct {
    PowerMgrDeepSleepOps *deepSleepOps;
    LowpowerExternalVoterHandle exVoterHandle;
    Atomic sleepVoteCount;
    UINT32 minSleepTicks;
    UINT32 maxSleepCount;
    UINT32 minDeepSleepTicks;
    UINT32 sleepTime[LOSCFG_KERNEL_CORE_NUM];
    Atomic deepSleepCores;
    Atomic resumeSleepCores;
    Atomic freq;
    Atomic freqPending;
    Atomic deepSleepDelay;
    Atomic freeLock;
    SPIN_LOCK_S lock;
    UINT8 sleepMode[LOSCFG_KERNEL_CORE_NUM];
} PowerMgr;

enum { LOCK_OFF = 0, LOCK_ON };

__attribute__((section(".data"))) STATIC PowerMgr g_pmMgr = {
    .deepSleepOps = NULL,
    .exVoterHandle = NULL,
    .sleepVoteCount = 0,
    .maxSleepCount = MAX_SLEEP_TIME,
    .minSleepTicks = MIN_SLEEP_TIME,
    .minDeepSleepTicks = MIN_DEEP_SLEEP_TIME,
    .sleepTime = {0},
    .deepSleepCores = 0,
    .resumeSleepCores = 0,
    .freq = 1,
    .freqPending = 0,
    .deepSleepDelay = 0,
    .freeLock = 0,
    .sleepMode = {0},
};

STATIC VOID OsChangeFreq(VOID);

STATIC VOID OsLightSleepDefault(VOID);

STATIC VOID OsSetWakeUpTimerDefault(UINT32 sleepTick);

STATIC UINT32 OsWithrawWakeUpTimerDefault(VOID);

STATIC UINT32 OsGetSleepTimeDefault(VOID);

STATIC UINT32 OsSelectSleepModeDefault(UINT32 sleepTicks);

STATIC VOID OsChangeFreqDefault(UINT8 freq);

STATIC VOID OsEnterDeepSleepDefault(VOID);

STATIC UINT32 OsPreConfigDefault(VOID);

STATIC VOID OsPostConfigDefault(VOID);

STATIC PowerMgrRunOps g_pmRunOps = {
    .changeFreq = OsChangeFreqDefault,
    .enterLightSleep = OsLightSleepDefault,
    .enterDeepSleep = OsEnterDeepSleepDefault,
    .setWakeUpTimer = OsSetWakeUpTimerDefault,
    .withdrawWakeUpTimer = OsWithrawWakeUpTimerDefault,
    .getSleepTime = OsGetSleepTimeDefault,
    .selectSleepMode = OsSelectSleepModeDefault,
    .preConfig = OsPreConfigDefault,
    .postConfig = OsPostConfigDefault,
};

STATIC VOID OsLightSleepDefault(VOID)
{
    TRACE_FUNC_CALL();
    wfi();
}

STATIC VOID OsSetWakeUpTimerDefault(UINT32 sleepTick)
{
    TRACE_FUNC_CALL();
}

STATIC UINT32 OsWithrawWakeUpTimerDefault(VOID)
{
    TRACE_FUNC_CALL();
    return 0;
}

STATIC UINT32 OsGetSleepTimeDefault(VOID)
{
#ifdef LOSCFG_KERNEL_TICKLESS
    return OsSleepTicksGet();
#else
    return 0;
#endif
}

STATIC UINT32 OsSelectSleepModeDefault(UINT32 sleepTicks)
{
    if (sleepTicks < g_pmMgr.minSleepTicks) {
        return LOS_INTERMIT_NONE;
    }

    if (g_pmMgr.deepSleepOps != NULL && sleepTicks >= g_pmMgr.minDeepSleepTicks &&
        g_pmRunOps.getDeepSleepVoteCount() == 0) {
        return LOS_INTERMIT_DEEP_SLEEP;
    }

    return LOS_INTERMIT_LIGHT_SLEEP;
}

STATIC VOID OsChangeFreqDefault(UINT8 freq)
{
    (VOID)freq;
    TRACE_FUNC_CALL();
}

STATIC VOID OsEnterDeepSleepDefault(VOID)
{
    TRACE_FUNC_CALL();
    wfi();
}

STATIC UINT32 OsPreConfigDefault(VOID)
{
    TRACE_FUNC_CALL();
    return 1;
}

STATIC VOID OsPostConfigDefault(VOID)
{
}

#ifdef LOSCFG_KERNEL_DEEPSLEEP

STATIC BOOL OsCouldDeepSleepDefault(VOID)
{
    TRACE_FUNC_CALL();
    return true;
}

STATIC BOOL OsSuspendPreConfigDefault(VOID)
{
    TRACE_FUNC_CALL();
    return true;
}

STATIC VOID OsSuspendDeviceDefault(VOID)
{
    TRACE_FUNC_CALL();
}

STATIC VOID OsRollBackDefault(VOID)
{
    TRACE_FUNC_CALL();
}

STATIC VOID OsResumeDeviceDefault(VOID)
{
    TRACE_FUNC_CALL();
}

STATIC VOID OsResumePostConfigDefault(VOID)
{
    TRACE_FUNC_CALL();
}

STATIC VOID OsSystemWakeupDefault(VOID)
{
    TRACE_FUNC_CALL();
}

STATIC VOID OsResumeCallBackDefault(VOID)
{
    TRACE_FUNC_CALL();
}

STATIC VOID OsOtherCoreResumeDefault(VOID)
{
    TRACE_FUNC_CALL();
}

STATIC VOID OsDeepSleepResume(VOID);

STATIC PowerMgrDeepSleepOps g_deepSleepOps = {
    .couldDeepSleep = OsCouldDeepSleepDefault,
    .systemWakeup = OsSystemWakeupDefault,
    .suspendPreConfig = OsSuspendPreConfigDefault,
    .suspendDevice = OsSuspendDeviceDefault,
    .rollback = OsRollBackDefault,
    .resumeDevice = OsResumeDeviceDefault,
    .resumePostConfig = OsResumePostConfigDefault,
    .resumeCallBack = OsResumeCallBackDefault,
    .otherCoreResume = OsOtherCoreResumeDefault
};

STATIC INLINE VOID OsTickResume(UINT32 sleepTicks)
{
    UINT32 cpuid = ArchCurrCpuid();
    if (sleepTicks > g_pmMgr.sleepTime[cpuid]) {
        sleepTicks -= g_pmMgr.sleepTime[cpuid];
    } else {
        sleepTicks = 0;
    }
    OsSysTimeUpdate(sleepTicks);
}

STATIC VOID OsDeepSleepResume(VOID)
{
    DEEPOPS_CALL_FUNC_VOID(resumeFromReset);
    LOS_AtomicSet(&g_pmMgr.resumeSleepCores, OS_MP_CPU_ALL);

#ifdef LOSCFG_KERNEL_CPUP
    OsSetCpuCycle(0);
#endif

#if (LOSCFG_KERNEL_SMP == YES)
    release_secondary_cores();
#endif
    OsSRRestoreRegister();
}

STATIC INLINE VOID OsEnterDeepSleepMainCore(VOID)
{
    LOS_AtomicAdd(&g_pmMgr.deepSleepCores, 1);
    g_deepSleepOps.suspendPreConfig();

    if (g_pmMgr.deepSleepCores == LOSCFG_KERNEL_CORE_NUM && g_deepSleepOps.couldDeepSleep()) {
        g_deepSleepOps.suspendDevice();
        g_pmRunOps.setWakeUpTimer(g_pmMgr.sleepTime[0]);
        g_resumeFromImg = LOS_COLD_RESET;
        OsSRSaveRegister();

        if (g_resumeFromImg == LOS_COLD_RESET) {
            g_resumeFromImg = LOS_DEEP_SLEEP_RESET;
            CALL_RUN_OPS_FUNC_NO_RETURN(contextSave);
            g_pmRunOps.enterDeepSleep();
            g_deepSleepOps.rollback();
        }
        g_deepSleepOps.resumeDevice();
        UINT32 sleepTicks = g_pmRunOps.withdrawWakeUpTimer();
        OsSysTimeUpdate(sleepTicks);
    } else {
#ifdef LOSCFG_KERNEL_TICKLESS
        OsTicklessOpen();
#endif
        g_pmRunOps.enterLightSleep();
    }
    g_deepSleepOps.resumePostConfig();
    LOS_AtomicSub(&g_pmMgr.deepSleepCores, 1);
}

STATIC INLINE VOID OsEnterSleepMode(VOID)
{
#ifdef LOSCFG_KERNEL_SMP
    UINT32 currCpuid = ArchCurrCpuid();
    if (currCpuid == 0) {
#endif
        OsEnterDeepSleepMainCore();
#ifdef LOSCFG_KERNEL_SMP
        return;
    }

    UINT32 cpuMask = 1 << currCpuid;
    LOS_AtomicAdd(&g_pmMgr.deepSleepCores, 1);
    OsSRSaveRegister();
    if (g_pmMgr.resumeSleepCores & cpuMask) {
        INT32 val;
        do {
            val = LOS_AtomicRead(&g_pmMgr.resumeSleepCores);
        } while (LOS_AtomicCmpXchg32bits(&g_pmMgr.resumeSleepCores, val & (~cpuMask), val));
        g_deepSleepOps.otherCoreResume();
        UINT32 sleepTicks = g_pmRunOps.withdrawWakeUpTimer();
        OsTickResume(sleepTicks);
    } else {
        if (g_pmMgr.deepSleepCores == LOSCFG_KERNEL_CORE_NUM) {
            LOS_MpSchedule(1 << 0);
        }
#ifdef LOSCFG_KERNEL_TICKLESS
        OsTicklessOpen();
#endif
        g_pmRunOps.enterLightSleep();
    }
    LOS_AtomicSub(&g_pmMgr.deepSleepCores, 1);
#endif // LOSCFG_KERNEL_SMP
}

STATIC INLINE VOID OsSystemSuspend(LosIntermitMode *mode)
{
    // If enterShutdownMode is not defined, will fall through to standby mode
    // If enterStandbyMode is not defined, will fall through to stop mode
    // If enterStopMode is not defined, will fall through to sleep mode
    switch (*mode) {
        case LOS_INTERMIT_SHUTDOWN:
            /* fall through */
        case LOS_INTERMIT_STANDBY:
            /* fall through */
        case LOS_INTERMIT_DEEP_SLEEP:
#ifdef LOSCFG_KERNEL_DEEPSLEEP
            if (g_pmRunOps.enterDeepSleep != NULL) {
                OsEnterSleepMode();
                break;
            }
#endif
            /* fall through */
        default:
            *mode = LOS_INTERMIT_LIGHT_SLEEP;
            break;
    }
}
#endif

STATIC VOID OsLowpowerLightSleep(UINT32 mode, UINT32 cpuid, UINT32 sleepTicks)
{
    if (g_pmRunOps.preConfig != NULL) {
        sleepTicks = g_pmRunOps.getSleepTime();
    }
    if (sleepTicks > 1) {
        g_pmMgr.sleepMode[cpuid] = (mode & 0x0FF);
        g_pmMgr.sleepTime[cpuid] = (sleepTicks & 0x0FF);
#ifdef LOSCFG_KERNEL_TICKLESS
        OsTicklessOpen();
#endif
        if (mode == LOS_INTERMIT_LIGHT_SLEEP && g_pmRunOps.enterLightSleep != NULL) {
            g_pmRunOps.enterLightSleep();
        } else {
            wfi();
        }
    } else {
        g_pmMgr.sleepMode[cpuid] = LOS_INTERMIT_NONE;
        g_pmMgr.sleepTime[cpuid] = 0;
        wfi();
    }
}

STATIC VOID OsLowpowerDeepSleep(LosIntermitMode *mode, UINT32 cpuid, UINT32 sleepTicks)
{
#ifdef LOSCFG_KERNEL_DEEPSLEEP
    if (g_pmRunOps.enterDeepSleep == NULL) {
        *mode = LOS_INTERMIT_LIGHT_SLEEP;
    } else {
        *mode = LOS_INTERMIT_DEEP_SLEEP;
        g_pmMgr.sleepMode[cpuid] = *mode;
        g_pmMgr.sleepTime[cpuid] = sleepTicks;

        OsSystemSuspend(mode);
    }
#else
    *mode = LOS_INTERMIT_LIGHT_SLEEP;
#endif
}

STATIC VOID OsLowpowerProcess(VOID)
{
#ifdef LOSCFG_KERNEL_RUNSTOP
    if (OsWowSysDoneFlagGet() == OS_STORE_SYSTEM) {
        OsStoreSystemInfoBeforeSuspend();
    }
#endif
    /* Change frequency is pended, need to change the freq here. */
    if ((g_pmRunOps.changeFreq != NULL)) {
        OsChangeFreq();
    }

    UINT32 intSave = LOS_IntLock();
    LOS_TaskLock();
    RUNOPS_CALL_FUNC_VOID(preConfig);
    UINT32 cpuid = ArchCurrCpuid();
    UINT32 sleepTicks = g_pmRunOps.getSleepTime();
    if (sleepTicks <= g_pmMgr.minSleepTicks || LOS_PowerMgrGetDeepSleepVoteCount() != 0) {
        g_pmMgr.sleepMode[cpuid] = LOS_INTERMIT_NONE;
        g_pmMgr.sleepTime[cpuid] = 0;

        RUNOPS_CALL_FUNC_VOID(postConfig);

#ifdef LOSCFG_KERNEL_TICKLESS
        OsTicklessOpen();
#endif

        wfi();

        PRINT_WARN("\r\n [PM] Application is runing in work mode, powermgr do not process.\n");
    } else {
        if (sleepTicks > g_pmMgr.maxSleepCount) {
            sleepTicks = g_pmMgr.maxSleepCount;
        }
        UINT32 mode = g_pmRunOps.selectSleepMode(sleepTicks);
        if (mode >= LOS_INTERMIT_DEEP_SLEEP) {
            g_pmMgr.sleepTime[cpuid] = g_pmRunOps.withdrawWakeUpTimer();
            OsLowpowerDeepSleep(&mode, cpuid, sleepTicks);
        }

        RUNOPS_CALL_FUNC_VOID(postConfig);

        if (mode < LOS_INTERMIT_DEEP_SLEEP) {
            OsLowpowerLightSleep(mode, cpuid, sleepTicks);
        }
    }

    LOS_TaskUnlock();
    LOS_IntRestore(intSave);
}

STATIC VOID OsLowpowerWakeupFromReset(VOID)
{
#ifdef LOSCFG_KERNEL_RUNSTOP
    if (g_resumeFromImg == LOS_RUN_STOP_RESET) {
        OsSystemWakeup();
        return;
    }
#endif
#ifdef LOSCFG_KERNEL_DEEPSLEEP
    if (g_resumeFromImg == LOS_DEEP_SLEEP_RESET) {
        OsDeepSleepResume();
    }
#endif
}

STATIC VOID OsLowpowerWakeupFromInterrupt(UINT32 intNum)
{
#ifdef LOSCFG_KERNEL_TICKLESS
    OsTicklessUpdate(intNum);
#else
    (VOID)intNum;
#endif
}

STATIC VOID OsChangeFreq(VOID)
{
    UINT32 freq;
    BOOL ret;

    do {
        ret = LOS_AtomicCmpXchg32bits(&g_pmMgr.freeLock, LOCK_ON, LOCK_OFF);
        if (ret) {
            return;
        }
        freq = (UINT32)g_pmMgr.freqPending;
        if (freq != (UINT32)g_pmMgr.freq) {
            g_pmRunOps.changeFreq(freq);
            LOS_AtomicSet(&g_pmMgr.freq, (INT32)freq);
        }
        LOS_AtomicSet(&g_pmMgr.freeLock, LOCK_OFF);
    } while (FreqHigher(g_pmMgr.freqPending, freq));
}

STATIC VOID OsLowpowerChangeFreq(LosFreqMode freq)
{
    TRACE_FUNC_CALL();
    if (g_pmRunOps.changeFreq == NULL) {
        PRINT_DEBUG("\r\n [PM] freq function is null.\n");
        return;
    }

    if (freq >= LOS_SYS_FREQ_MAX) {
        PRINT_DEBUG("\r\n [PM] freq(%d) is invalid.\n", freq);
        return;
    }

    // The OsChangeFreq procedure deals with the latest setting only.
    LOS_AtomicSet(&g_pmMgr.freqPending, (INT32)freq);

    // We get a high frequency request, then change it
    if (FreqHigher(g_pmMgr.freqPending, g_pmMgr.freq) && g_pmRunOps.changeFreq != NULL) {
        OsChangeFreq();
    }
}

STATIC VOID OsLowpowerDeepSleepVoteBegin(VOID)
{
    TRACE_FUNC_CALL();
    LOS_AtomicInc(&g_pmMgr.sleepVoteCount);
    LOS_ASSERT(g_pmMgr.sleepVoteCount > 0);
}

STATIC VOID OsLowpowerDeepSleepVoteEnd(VOID)
{
    TRACE_FUNC_CALL();
    LOS_AtomicDec(&g_pmMgr.sleepVoteCount);
    LOS_ASSERT(g_pmMgr.sleepVoteCount >= 0);
}

STATIC VOID OsLowpowerDeepSleepVoteDelay(UINT32 delayTicks)
{
    TRACE_FUNC_CALL();
}

STATIC VOID OsLowpowerRegisterExternalVoter(LowpowerExternalVoterHandle callback)
{
    TRACE_FUNC_CALL();
    g_pmMgr.exVoterHandle = callback;
}

STATIC UINT32 OsLowpowerGetDeepSleepVoteCount(VOID)
{
    if (g_pmMgr.exVoterHandle == NULL) {
        return (UINT32)g_pmMgr.sleepVoteCount;
    } else {
        return (UINT32)g_pmMgr.sleepVoteCount + (UINT32)g_pmMgr.exVoterHandle();
    }
}

STATIC PowerMgrOps g_pmOps = {
    .process = OsLowpowerProcess,
    .wakeupFromReset = OsLowpowerWakeupFromReset,
    .resumeFromInterrupt = OsLowpowerWakeupFromInterrupt,
    .changeFreq = OsLowpowerChangeFreq,
    .deepSleepVoteBegin = OsLowpowerDeepSleepVoteBegin,
    .deepSleepVoteEnd = OsLowpowerDeepSleepVoteEnd,
    .deepSleepVoteDelay = OsLowpowerDeepSleepVoteDelay,
    .registerExternalVoter = OsLowpowerRegisterExternalVoter,
    .getDeepSleepVoteCount = OsLowpowerGetDeepSleepVoteCount,
    .getSleepMode = NULL,
    .setSleepMode = NULL,
};

#define FORCE_NULL_CALLBACK (void *)0x3f3f3f3f

#define ASSIGN_MEMBER(lhs, rhs, member)             \
    do {                                            \
        if ((rhs)->member == FORCE_NULL_CALLBACK) { \
            (lhs)->member = NULL;                   \
        } else if ((rhs)->member != NULL) {         \
            (lhs)->member = (rhs)->member;          \
        }                                           \
    } while (0)

#define ASSIGN_MEMBER_NOT_NULL(lhs, rhs, member) \
    do {                                         \
        ASSIGN_MEMBER(lhs, rhs, member);         \
        if ((lhs)->member == NULL) {             \
            PRINT_ERR("\r\n [PM] %s must be non-null.\n", __FUNCTION__); \
            while (1) {};                        \
        }                                        \
    } while (0)

VOID LOS_PowerMgrInit(const PowerMgrParameter *para)
{
    const PowerMgrRunOps *runOps = NULL;
    const PowerMgrDeepSleepOps *deepSleepOps = NULL;
    (void)deepSleepOps;
    if (para != NULL) {
        runOps = &para->runOps;
#ifdef LOSCFG_KERNEL_DEEPSLEEP
        deepSleepOps = &para->deepSleepOps;
#endif
        g_pmMgr.minSleepTicks = para->config.minLightSleepTicks;
        g_pmMgr.maxSleepCount = para->config.maxDeepSleepTicks;
        g_pmMgr.minDeepSleepTicks = para->config.minDeepSleepTicks;
    }

    LOS_AtomicSet(&g_pmMgr.resumeSleepCores, 0);

    LOS_SpinInit(&g_pmMgr.lock);
    LOS_AtomicSet(&g_pmMgr.freeLock, LOCK_OFF);

    // verify and assign input operators.
    if (runOps != NULL) {
        ASSIGN_MEMBER(&g_pmRunOps, runOps, changeFreq);
        ASSIGN_MEMBER(&g_pmRunOps, runOps, enterLightSleep);
#ifdef LOSCFG_KERNEL_DEEPSLEEP
        ASSIGN_MEMBER_NOT_NULL(&g_pmRunOps, runOps, enterDeepSleep);
        ASSIGN_MEMBER_NOT_NULL(&g_pmRunOps, runOps, setWakeUpTimer);
        ASSIGN_MEMBER_NOT_NULL(&g_pmRunOps, runOps, withdrawWakeUpTimer);
#else
        ASSIGN_MEMBER(&g_pmRunOps, runOps, enterDeepSleep);
        ASSIGN_MEMBER(&g_pmRunOps, runOps, setWakeUpTimer);
        ASSIGN_MEMBER(&g_pmRunOps, runOps, withdrawWakeUpTimer);
#endif
        ASSIGN_MEMBER_NOT_NULL(&g_pmRunOps, runOps, getSleepTime);
        ASSIGN_MEMBER_NOT_NULL(&g_pmRunOps, runOps, selectSleepMode);
        ASSIGN_MEMBER(&g_pmRunOps, runOps, preConfig);
        ASSIGN_MEMBER(&g_pmRunOps, runOps, postConfig);
    }

#ifdef LOSCFG_KERNEL_DEEPSLEEP
    if (deepSleepOps != NULL) {
        ASSIGN_MEMBER(&g_deepSleepOps, deepSleepOps, couldDeepSleep);
        ASSIGN_MEMBER(&g_deepSleepOps, deepSleepOps, systemWakeup);
        ASSIGN_MEMBER(&g_deepSleepOps, deepSleepOps, suspendPreConfig);
        ASSIGN_MEMBER(&g_deepSleepOps, deepSleepOps, suspendDevice);
        ASSIGN_MEMBER(&g_deepSleepOps, deepSleepOps, rollback);
        ASSIGN_MEMBER(&g_deepSleepOps, deepSleepOps, resumeDevice);
        ASSIGN_MEMBER(&g_deepSleepOps, deepSleepOps, resumePostConfig);
        ASSIGN_MEMBER(&g_deepSleepOps, deepSleepOps, resumeCallBack);
        ASSIGN_MEMBER(&g_deepSleepOps, deepSleepOps, otherCoreResume);
    }
#endif
    // Register PowerMgr to Low-Power Framework.
    LOS_LowpowerInit(&g_pmOps);
}
