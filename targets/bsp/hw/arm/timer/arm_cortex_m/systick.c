/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: ARM cortex-m timer
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

#include "asm/platform.h"
#include "hal_hwi.h"
#include "los_hwi.h"
#include "los_tick_pri.h"
#include "los_hwi_pri.h"
#include "nvic.h"

#if defined (LOSCFG_ARCH_ARM_CORTEX_M) && (LOSCFG_KERNEL_CPUP)
#include "timer.h"
#endif

#define CYCLE_REG_MASK    0xFFFFFFFFU

#define TICK_INTR_CHECK   0x4000000U

#define OS_CYCLE_PER_TICK (OS_SYS_CLOCK / LOSCFG_BASE_CORE_TICK_PER_SECOND)

#define TIMER_CYCLE_SWITCH     (OS_CYCLE_PER_TICK / 10)

LITE_OS_SEC_BSS UINT32     g_cyclesPerTick;

#define M_INT_NUM  15 /* SysTick_IRQn + OS_SYS_VECOTRE */

VOID HalClockInit(VOID)
{
    UINT32 ret = LOS_HwiCreate(M_INT_NUM, 0, 0, OsTickHandler, 0);
    if (ret != 0) {
        PRINTK("ret of LOS_HwiCreate = %#x\n", ret);
    }
#if defined (LOSCFG_ARCH_ARM_CORTEX_M) && (LOSCFG_KERNEL_CPUP)
    TimerHwiCreate();
#endif
}

VOID HalClockStart(VOID)
{
    if ((OS_SYS_CLOCK == 0) ||
        (LOSCFG_BASE_CORE_TICK_PER_SECOND == 0) ||
        (LOSCFG_BASE_CORE_TICK_PER_SECOND > OS_SYS_CLOCK)) {
        return;
    }

    g_cyclesPerTick = OS_CYCLE_PER_TICK;

    (VOID)SysTick_Config(OS_CYCLE_PER_TICK);

    UINT32 ret = LOS_HwiEnable(M_INT_NUM);
    if (ret != 0) {
        PRINTK("LOS_HwiEnable failed. ret = %#x\n", ret);
    }
}

#if defined (LOSCFG_ARCH_ARM_CORTEX_M) && (LOSCFG_KERNEL_CPUP)
UINT64 HalClockGetCpupCycles(VOID)
{
    return GetTimerCpupCycles();
}
#endif

UINT64 HalClockGetCycles(VOID)
{
    UINT64 swTick;
    UINT64 cycle;
    UINT32 hwCycle;
    UINT32 intSave;

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
#if defined (LOSCFG_ARCH_ARM_CORTEX_M) && (LOSCFG_KERNEL_CPUP)
    cycle = HalClockGetCpupCycles() * TIMER_CYCLE_SWITCH;
#endif
    return cycle;
}

UINT64 HalClockGetSysTickCycles(VOID)
{
    UINT64 swTick;
    UINT64 cycle;
    UINT32 hwCycle;
    UINT32 intSave;
    UINT32 systickLoad;
    UINT32 systickCur;

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

    return cycle;
}

VOID HalDelayUs(UINT32 usecs)
{
    UINT64 tmo = LOS_CurrNanosec() + usecs * OS_SYS_NS_PER_US;

    while (LOS_CurrNanosec() < tmo) {
        __asm__ volatile ("nop");
    }
}

UINT64 hi_sched_clock(VOID)
{
    return LOS_CurrNanosec();
}

/* just stub api for tickless */
UINT32 HalClockGetTickTimerCycles(VOID)
{
    return 0;
}

/* just stub api for tickless */
VOID HalClockTickTimerReload(UINT32 cycles)
{
    (VOID)cycles;
}
