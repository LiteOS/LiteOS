/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Tickless
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
#include "los_tickless_pri.h"
#include "los_hwi_pri.h"
#include "los_tick_pri.h"
#include "los_sortlink_pri.h"
#include "los_swtmr_pri.h"
#include "los_task_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

STATIC BOOL g_ticklessFlag = FALSE;
STATIC BOOL g_tickIrqFlag[LOSCFG_KERNEL_CORE_NUM] = {FALSE};
STATIC volatile UINT32 g_sleepTicks[LOSCFG_KERNEL_CORE_NUM] = {0};

#define OS_GET_CYCLECOMPENSATE(cyclesPre,cyclesCur) (((cyclesPre) > (cyclesCur)) ? \
    ((g_sysClock / g_tickPerSecond) - (cyclesCur)) : (((g_sysClock / g_tickPerSecond) << 1) - (cyclesCur)))

LITE_OS_SEC_TEXT VOID LOS_TicklessEnable(VOID)
{
    g_ticklessFlag = TRUE;
}

LITE_OS_SEC_TEXT VOID LOS_TicklessDisable(VOID)
{
    g_ticklessFlag = FALSE;
}

LITE_OS_SEC_TEXT BOOL OsTicklessFlagGet(VOID)
{
    return g_ticklessFlag;
}

LITE_OS_SEC_TEXT BOOL OsTickIrqFlagGet(VOID)
{
    return g_tickIrqFlag[ArchCurrCpuid()];
}

LITE_OS_SEC_TEXT VOID OsTickIrqFlagSet(BOOL tickIrqFlag)
{
    g_tickIrqFlag[ArchCurrCpuid()] = tickIrqFlag;
}

LITE_OS_SEC_TEXT UINT32 OsTicklessSleepTickGet(VOID)
{
    return g_sleepTicks[ArchCurrCpuid()];
}

LITE_OS_SEC_TEXT VOID OsTicklessSleepTickSet(UINT32 sleeptick)
{
    g_sleepTicks[ArchCurrCpuid()] = sleeptick;
}

STATIC INLINE UINT32 OsSleepTicksGet(VOID)
{
    UINT32 tskSortLinkTicks, swtmrSortLinkTicks, sleepTicks;

    UINT32 intSave = LOS_IntLock();
    LOS_SpinLock(&g_taskSpin);
    tskSortLinkTicks = OsSortLinkGetNextExpireTime(&OsPercpuGet()->taskSortLink);
    LOS_SpinUnlock(&g_taskSpin);
    LOS_SpinLock(&g_swtmrSpin);
    swtmrSortLinkTicks = OsSortLinkGetNextExpireTime(&OsPercpuGet()->swtmrSortLink);
    LOS_SpinUnlock(&g_swtmrSpin);
    sleepTicks = (tskSortLinkTicks < swtmrSortLinkTicks) ? tskSortLinkTicks : swtmrSortLinkTicks;
    LOS_IntRestore(intSave);
    return sleepTicks;
}

LITE_OS_SEC_TEXT VOID OsSysTimeUpdate(UINT32 sleepTicks)
{
    UINT32 intSave;

    if (sleepTicks == 0) {
        return;
    }

    intSave = LOS_IntLock();
    g_tickCount[ArchCurrCpuid()] += (sleepTicks - 1);
    LOS_SpinLock(&g_taskSpin);
    OsSortLinkUpdateExpireTime(sleepTicks, &OsPercpuGet()->taskSortLink);
    LOS_SpinUnlock(&g_taskSpin);
    LOS_SpinLock(&g_swtmrSpin);
    OsSortLinkUpdateExpireTime(sleepTicks, &OsPercpuGet()->swtmrSortLink);
    LOS_SpinUnlock(&g_swtmrSpin);
    LOS_IntRestore(intSave);
}

VOID OsTicklessUpdate(UINT32 irqnum)
{
    UINT32 cycles, ticks;
    UINT32 cyclesPertick;
    UINT32 sleepTicks;
    UINT32 intSave = LOS_IntLock();

    sleepTicks = OsTicklessSleepTickGet();
    if (sleepTicks == 0) {
        LOS_IntRestore(intSave);
        return;
    }

    cyclesPertick = g_sysClock / LOSCFG_BASE_CORE_TICK_PER_SECOND;
    if (irqnum == OS_TICK_INT_NUM) {
        OsSysTimeUpdate(sleepTicks);
    } else {
        cycles = HalClockGetTickTimerCycles();
        cycles = (sleepTicks * cyclesPertick) - cycles;
        ticks = cycles / cyclesPertick;
        if (ticks < sleepTicks) {
            cycles = cycles % cyclesPertick;
            OsSysTimeUpdate(ticks + 1);
            HalClockTickTimerReload(cyclesPertick - cycles);
        } else {
            /*
             * If ticks is greater or equal to sleepTicks, it means the tick has already
             * arrived, it should compensate with the sleepTicks just as that will be done
             * in tick handler.
             */
            OsSysTimeUpdate(sleepTicks);
        }
    }
    OsTicklessSleepTickSet(0);

    LOS_IntRestore(intSave);
}

VOID OsTicklessStart(VOID)
{
    UINT32 intSave;
    /*
     * The system has already started, the g_sysClock is non-zero and greater or equal to
     * LOSCFG_BASE_CORE_TICK_PER_SECOND (see OsTickInit). So the cyclesPerTick won't be zero.
     */
    UINT32 cyclesPerTick = g_sysClock / LOSCFG_BASE_CORE_TICK_PER_SECOND;
    UINT32 maxTicks = OS_NULL_INT / cyclesPerTick;
    UINT32 sleepTicks;
    UINT32 cycles, cyclesPre, cyclesCur, cycleCompensate;

    intSave = LOS_IntLock();
    /*
     * The sleep tick may be changed afterwards, cause interrupt has been disabled, the sleep tick
     * may increase but cannot decrease. Thus there's no need to spin here.
     */
    sleepTicks = OsSleepTicksGet();
    cyclesPre = HalClockGetTickTimerCycles();

    if (sleepTicks > 1) {
        if (sleepTicks >= maxTicks) {
            sleepTicks = maxTicks;
        }
        cycles = sleepTicks * cyclesPerTick;
        cyclesCur = HalClockGetTickTimerCycles();
        cycleCompensate = OS_GET_CYCLECOMPENSATE(cyclesPre, cyclesCur);
        HalClockTickTimerReload(cycles - cycleCompensate);
        OsTicklessSleepTickSet(sleepTicks);
        LOS_IntRestore(intSave);

        return;
    }
    LOS_IntRestore(intSave);
    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
