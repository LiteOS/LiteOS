/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Tick
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

#include "los_tick_pri.h"
#include "los_swtmr_pri.h"
#include "los_task_pri.h"
#include "los_timeslice_pri.h"
#ifdef LOSCFG_KERNEL_TICKLESS
#include "los_tickless_pri.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

LITE_OS_SEC_BSS volatile UINT64 g_tickCount[LOSCFG_KERNEL_CORE_NUM] = {0};
LITE_OS_SEC_DATA_INIT UINT32 g_sysClock;
LITE_OS_SEC_DATA_INIT UINT32 g_tickPerSecond;
LITE_OS_SEC_BSS DOUBLE g_cycle2NsScale;

/* spinlock for task module */
LITE_OS_SEC_BSS SPIN_LOCK_INIT(g_tickSpin);

/*
 * Description : Tick interruption handler
 */
LITE_OS_SEC_TEXT VOID OsTickHandler(VOID)
{
    UINT32 intSave;

    TICK_LOCK(intSave);
    g_tickCount[ArchCurrCpuid()]++;
    TICK_UNLOCK(intSave);

#ifdef LOSCFG_KERNEL_TICKLESS
    OsTickIrqFlagSet(OsTicklessFlagGet());
#endif

#if (LOSCFG_BASE_CORE_TICK_HW_TIME == YES)
    HalClockIrqClear(); /* diff from every platform */
#endif

#if (LOSCFG_BASE_CORE_TIMESLICE == YES)
    OsTimesliceCheck();
#endif

    OsTaskScan(); /* task timeout scan */

#if (LOSCFG_BASE_CORE_SWTMR == YES)
    OsSwtmrScan();
#endif
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
