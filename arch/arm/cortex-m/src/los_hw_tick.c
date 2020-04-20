/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2018. All rights reserved.
 * Description: hw_tick
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

#include "los_hwi_pri.h"
#include "los_tick_pri.h"
#include "los_base.h"
#include "los_task_pri.h"
#include "los_swtmr.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define CYCLE_REG_MASK 0xFFFFFFFFU

#define TICK_INTR_CHECK 0x4000000U

#define OS_CYCLE_PER_TICK (OS_SYS_CLOCK / LOSCFG_BASE_CORE_TICK_PER_SECOND)

LITE_OS_SEC_BSS UINT32      g_cyclesPerTick;

LITE_OS_SEC_TEXT_INIT UINT32 OsTickInit(UINT32 systemClock, UINT32 tickPerSecond)
{
    return LOS_OK;
}

/*
 * Description : Configure Tick Interrupt Start
 * return      : LOS_OK - Success , or LOS_ERRNO_TICK_CFG_INVALID - failed
 */
LITE_OS_SEC_TEXT_INIT VOID OsTickStart(VOID)
{
    if ((OS_SYS_CLOCK == 0) ||
        (LOSCFG_BASE_CORE_TICK_PER_SECOND == 0) ||
        (LOSCFG_BASE_CORE_TICK_PER_SECOND > OS_SYS_CLOCK)) {
        return;
    }

#if (LOSCFG_PLATFORM_HWI == YES)
#if (OS_HWI_WITH_ARG == YES)
    OsSetVector(SysTick_IRQn, (HWI_PROC_FUNC)OsTickHandler, NULL);
#else
    OsSetVector(SysTick_IRQn, OsTickHandler);
#endif
#endif

    g_cyclesPerTick = OS_CYCLE_PER_TICK;

    (VOID)SysTick_Config(OS_CYCLE_PER_TICK);

    return;
}

/*
 * Description : Get System cycle count
 * output      : highCnt  --- CpuTick High 4 byte
 *               lowCnt   --- CpuTick Low 4 byte
 */
LITE_OS_SEC_TEXT_MINOR VOID LOS_GetCpuCycle(UINT32 *highCnt, UINT32 *lowCnt)
{
    UINT64 swTick;
    UINT64 cycle;
    UINT32 hwCycle;
    UINT32 intSave;

    if ((highCnt == NULL) || (lowCnt == NULL)) {
        return;
    }

    intSave = LOS_IntLock();

    swTick = LOS_TickCountGet();
    hwCycle = SysTick->VAL;

    /* tick has come, but may interrupt environment, not counting the Tick interrupt response */
    if ((SCB->ICSR & TICK_INTR_CHECK) != 0) {
        hwCycle = SysTick->VAL;
        swTick++;
    }

    cycle = (swTick * g_cyclesPerTick) + (g_cyclesPerTick - hwCycle);
    LOS_IntRestore(intSave);

    *highCnt = cycle >> 32; /* 32:get high 32 bits */
    *lowCnt = cycle & CYCLE_REG_MASK;

    return;
}

/*
 * Description : Get Sys tick cycle count
 * output      : highCnt  --- SysTick count High 4 byte
 *               lowCnt   --- SysTick count Low 4 byte
 */
LITE_OS_SEC_TEXT_MINOR VOID LOS_GetSystickCycle(UINT32 *highCnt, UINT32 *lowCnt)
{
    UINT64 swTick;
    UINT64 cycle;
    UINT32 hwCycle;
    UINT32 intSave;
    UINT32 systickLoad;
    UINT32 systickCur;

    if ((highCnt == NULL) || (lowCnt == NULL)) {
        return;
    }

    intSave = LOS_IntLock();

    swTick = LOS_TickCountGet();

    systickLoad = SysTick->LOAD;
    systickCur = SysTick->VAL;
    hwCycle = systickLoad - systickCur;

    /* tick has come, but may interrupt environment, not counting the Tick interrupt response */
    if ((SCB->ICSR & TICK_INTR_CHECK) != 0) {
        hwCycle = systickLoad - systickCur;
        swTick++;
    }

    cycle = swTick * systickLoad + hwCycle;

    LOS_IntRestore(intSave);

    *highCnt = cycle >> 32; /* 32:get high 32 bits */
    *lowCnt = cycle & CYCLE_REG_MASK;

    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
