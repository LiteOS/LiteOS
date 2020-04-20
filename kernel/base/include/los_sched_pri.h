/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
 * Description: Scheduler Private HeadFile
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

#ifndef _LOS_SCHED_PRI_H
#define _LOS_SCHED_PRI_H

#include "los_percpu_pri.h"
#include "los_hwi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

extern UINT32 g_taskScheduled;

/*
 * Schedule flag, one bit represents one core.
 * This flag is used to prevent kernel scheduling before OSStartToRun.
 */
#define OS_SCHEDULER_SET(cpuid) do {     \
    g_taskScheduled |= (1U << (cpuid));  \
} while (0);

#define OS_SCHEDULER_CLR(cpuid) do {     \
    g_taskScheduled &= ~(1U << (cpuid)); \
} while (0);

#define OS_SCHEDULER_ACTIVE (g_taskScheduled & (1U << ArchCurrCpuid()))

typedef enum {
    INT_NO_RESCH = 0,   /* no needs to schedule */
    INT_PEND_RESCH,     /* pending schedule flag */
} SchedFlag;

/* Check if preemptable with counter flag */
STATIC INLINE BOOL OsPreemptable(VOID)
{
    /*
     * Unlike OsPreemptableInSched, the int may be not disabled when OsPreemptable
     * is called, needs mannually disable interrupt, to prevent current task from
     * being migrated to another core, and get the wrong preeptable status.
     */
    UINT32 intSave = LOS_IntLock();
    BOOL preemptable = (OsPercpuGet()->taskLockCnt == 0);
    if (!preemptable) {
        /* Set schedule flag if preemption is disabled */
        OsPercpuGet()->schedFlag = INT_PEND_RESCH;
    }
    LOS_IntRestore(intSave);
    return preemptable;
}

STATIC INLINE BOOL OsPreemptableInSched(VOID)
{
    BOOL preemptable = FALSE;

    preemptable = (OsPercpuGet()->taskLockCnt == 0);
    if (!preemptable) {
        /* Set schedule flag if preemption is disabled */
        OsPercpuGet()->schedFlag = INT_PEND_RESCH;
    }

    return preemptable;
}

/*
 * This function simply picks the next task and switches to it.
 * Current task needs to already be in the right state or the right
 * queues it needs to be in.
 */
extern VOID OsSchedResched(VOID);

/*
 * This function put the current task back to the ready queue and
 * try to do the schedule. However, the schedule won't be definitely
 * taken place while there're no other higher priority tasks or locked.
 */
extern VOID OsSchedPreempt(VOID);

/*
 * Just like OsSchedPreempt, except this function will do the OS_INT_ACTIVE
 * check, in case the schedule taken place in the middle of an interrupt.
 */
STATIC INLINE VOID LOS_Schedule(VOID)
{
#if (LOSCFG_PLATFORM_HWI == YES)
    if (OS_INT_ACTIVE) {
        OsPercpuGet()->schedFlag = INT_PEND_RESCH;
        return;
    }
#endif

    /*
     * trigger schedule in task will also do the slice check
     * if neccessary, it will give up the timeslice more in time.
     * otherwhise, there's no other side effects.
     */
    OsSchedPreempt();
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_SCHED_PRI_H */
