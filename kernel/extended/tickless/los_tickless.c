/*----------------------------------------------------------------------------
 * Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
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
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/


#include "los_hwi.h"
#include "los_tick.ph"
#include "los_tickless.inc"
#include "los_hw.h"

#if (LOSCFG_KERNEL_TICKLESS == YES)

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

BOOL g_bTicklessFlag = 1;
BOOL g_bTickIrqFlag = 0;
BOOL g_bReloadSysTickFlag = 0;

volatile UINT32 g_uwSleepTicks = 0;

LITE_OS_SEC_TEXT VOID LOS_TicklessEnable(VOID)
{
    g_bTicklessFlag = 1;
}

LITE_OS_SEC_TEXT VOID LOS_TicklessDisable(VOID)
{
    g_bTicklessFlag = 0;
}

static inline UINT32 osSleepTicksGet(VOID)
{
    UINT32 uwTskSortLinkTicks = 0;
    UINT32 uwSwtmrSortLinkTicks = 0;
    UINT32 uwSleepTicks = 0;

    /** Context guarantees that the interrupt has been closed */
    uwTskSortLinkTicks  = osTaskNextSwitchTimeGet();
    uwSwtmrSortLinkTicks = osSwTmrGetNextTimeout();

    uwSleepTicks = (uwTskSortLinkTicks < uwSwtmrSortLinkTicks) ? uwTskSortLinkTicks : uwSwtmrSortLinkTicks;
    return uwSleepTicks;
}

inline VOID osUpdateKernelTickCount(UINT32 uwHwiIndex)
{
    /** this function must be called in interrupt context */
    if (g_uwSleepTicks > 1)
    {
        UINT32 uwCyclesPerTick = OS_SYS_CLOCK / LOSCFG_BASE_CORE_TICK_PER_SECOND;
        UINT32 uwCurrSysCycles, uwElapseSysCycles, uwElapseTicks, uwRemainSysCycles;

        g_bReloadSysTickFlag = 0;
        if(uwHwiIndex == (SysTick_IRQn + OS_SYS_VECTOR_CNT))
        {
            uwElapseTicks = (g_uwSleepTicks - 1);
            LOS_SysTickReload(OS_SYS_CLOCK / LOSCFG_BASE_CORE_TICK_PER_SECOND);
        }
        else
        {
            uwCurrSysCycles = LOS_SysTickCurrCycleGet();
            uwElapseSysCycles = ((g_uwSleepTicks * uwCyclesPerTick) - uwCurrSysCycles);
            uwElapseTicks = uwElapseSysCycles / uwCyclesPerTick;
            uwRemainSysCycles = uwElapseSysCycles % uwCyclesPerTick;
            if (uwRemainSysCycles > 0)
            {
                LOS_SysTickReload(uwRemainSysCycles);
                g_bReloadSysTickFlag = 1;
            }
            else
            {
                LOS_SysTickReload(uwCyclesPerTick);
            }
        }
        osTickHandlerLoop(uwElapseTicks);
        g_uwSleepTicks = 0;
    }
}

VOID osTicklessStart(VOID)
{
    UINT32 uwCyclesPerTick = OS_SYS_CLOCK / LOSCFG_BASE_CORE_TICK_PER_SECOND;
    UINT32 uwMaxTicks = SysTick_LOAD_RELOAD_Msk / uwCyclesPerTick;
    UINTPTR uvIntSave = 0;
    UINT32 uwSleepTicks = 0;

    uvIntSave = LOS_IntLock();
    uwSleepTicks = osSleepTicksGet();
    if (uwSleepTicks > 1)
    {
        UINT32 uwSleepCycles, uwCurrSysCycles;
        if (uwSleepTicks >= uwMaxTicks)
        {
            uwSleepTicks = uwMaxTicks;
        }

        uwSleepCycles = uwSleepTicks * uwCyclesPerTick;
        uwCurrSysCycles = LOS_SysTickCurrCycleGet();
        LOS_SysTickReload(uwSleepCycles - uwCurrSysCycles);
        g_uwSleepTicks = uwSleepTicks;
    }
    LOS_IntRestore(uvIntSave);

    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif  /* end of #if (LOSCFG_KERNEL_TICKLESS == YES) */

