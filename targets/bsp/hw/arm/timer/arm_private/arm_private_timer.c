/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
 * Description: ARM private timer.
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

#include "asm/platform.h"
#include "los_hwi.h"
#include "los_tick_pri.h"

#define OS_CYCLE_PER_TICK (TIMER_FREQ / LOSCFG_BASE_CORE_TICK_PER_SECOND)

typedef struct {
    UINT32 load;        /* Private Timer Load Register */
    UINT32 count;       /* Private Timer Counter Register */
    UINT32 control;     /* Private Timer Control Register */
    UINT32 intStatus;   /* Private Timer Interrupt Status Register */
} PrivateTimer;

typedef struct {
    UINT32 low;         /* Global Timer Counter Registers, low bits */
    UINT32 high;        /* Global Timer Counter Registers, high bits */
    UINT32 control;     /* Global Timer Control Register */
    UINT32 intStatus;   /* Global Timer Interrupt Status Register */
    UINT32 compareLow;  /* Comparator Value Registers, low bits */
    UINT32 compareHigh; /* Comparator Value Registers, high bits */
    UINT32 increment;   /* Auto-increment Register */
} GlobalTimer;

PrivateTimer *g_privateTimer = (PrivateTimer *)PRVTIMER_BASE_ADDR;
GlobalTimer *g_globalTimer = (GlobalTimer *)GTIMER_BASE_ADDR;

UINT32 HalClockFreqRead(VOID)
{
    return TIMER_FREQ;
}

VOID HalClockFreqWrite(UINT32 freq)
{
    PRINT_WARN("private timer does not support setting frequency\n");
}

VOID HalClockStart(VOID)
{
    HalIrqUnmask(PRVTIMER_INT_NUM);

    g_privateTimer->load = OS_CYCLE_PER_TICK;
    g_privateTimer->control = 0x06; /* IAE bits = 110, not eanbled yet */
    g_privateTimer->control |= 0x01; /* enable private timer */
}

VOID OsTickEntry(VOID)
{
    OsTickHandler();

    /* clear private timer */
    g_privateTimer->intStatus = 0x01;
}

VOID HalClockInit(VOID)
{
    UINT32 ret;

    ret =  LOS_HwiCreate(PRVTIMER_INT_NUM, 0xa0, 0, OsTickEntry, NULL);
    if (ret != LOS_OK) {
        PRINT_ERR("%s, %d create tick irq failed, ret:0x%x\n", __FUNCTION__, __LINE__, ret);
    }
}

UINT64 HalClockGetCycles(VOID)
{
    UINT32 low, high;

    do {
        high = g_globalTimer->high;
        low = g_globalTimer->low;
    } while (g_globalTimer->high != high);

    /* combine high and low into 8 bytes cycles */
    return (((UINT64)high << 32) | low);
}

VOID HalDelayUs(UINT32 usecs)
{
    UINT64 tmo = LOS_CurrNanosec() + usecs * 1000;

    while (LOS_CurrNanosec() < tmo) {
        __asm__ volatile ("nop");
    }
}

UINT64 HalSchedClock(VOID)
{
    return LOS_CurrNanosec();
}

UINT32 HalClockGetTickTimerCycles(VOID)
{
    return g_privateTimer->count;
}

VOID HalClockTickTimerReload(UINT32 period)
{
    HalIrqUnmask(PRVTIMER_INT_NUM);

    /* set control counter regs to defaults */
    g_privateTimer->load = period;
    g_privateTimer->control = 0x06; /* IAE bits = 110, not eanbled yet */
    g_privateTimer->control |= 0x01; /* reenable private timer */
}
