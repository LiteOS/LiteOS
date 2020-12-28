/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Timer Implementation
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

#include "los_hwi.h"
#include "hisoc/sys_ctrl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define TIMER_ENABLE_BIT           7
#define TIMER_COUNTING_MODE_BIT    6
#define TIMER_INTERRUPT_ENABLE_BIT 5
#define TIMER_SIZE_SELECT_BIT      1
#define TIMER_ONESHOT_BIT          0

VOID HrtimerClockIrqClear(VOID)
{
    WRITE_UINT32(0x1, HRTIMER_TIMER_REG_BASE + TIMER_INT_CLR);
}

VOID HrtimerClockInit(VOID)
{
    UINT32 temp;

    /* enable timer here */
    READ_UINT32(temp, SYS_CTRL_REG_BASE + REG_SC_CTRL);
    temp |= HRTIMER_TIMER_ENABLE;
    WRITE_UINT32(temp, SYS_CTRL_REG_BASE + REG_SC_CTRL);

    /* disable timer */
    WRITE_UINT32(0x0, HRTIMER_TIMER_REG_BASE + TIMER_CONTROL);

    /*
     * Timing mode:oneshot [bit 0 set as 1]
     * timersize:32bits [bit 1 set as 1]
     * ticking with 1/1 clock frequency [bit 3 set as 0, bit 2 set as 0]
     * interrupt enabled [bit 5 set as 1]
     * timer mode periodic[bit 6 set as 1]
     */
    temp = (1U << TIMER_COUNTING_MODE_BIT) | (1U << TIMER_INTERRUPT_ENABLE_BIT) |
           (1U << TIMER_SIZE_SELECT_BIT) | (1U << TIMER_ONESHOT_BIT);
    WRITE_UINT32(temp, HRTIMER_TIMER_REG_BASE + TIMER_CONTROL);
}

VOID HrtimerClockStart(UINT32 period)
{
    UINT32 temp;

    /* set init value as period */
    WRITE_UINT32(period, HRTIMER_TIMER_REG_BASE + TIMER_LOAD);

    READ_UINT32(temp, HRTIMER_TIMER_REG_BASE + TIMER_CONTROL);
    /* timer enabled [bit 7 set as 1] */
    temp |= 1U << TIMER_ENABLE_BIT;
    WRITE_UINT32(temp, HRTIMER_TIMER_REG_BASE + TIMER_CONTROL);
}

VOID HrtimerClockStop(VOID)
{
    UINT32 temp;

    READ_UINT32(temp, HRTIMER_TIMER_REG_BASE + TIMER_CONTROL);
    /* timer disabled [bit 7 set as 0] */
    temp &= ~(1U << TIMER_ENABLE_BIT);
    WRITE_UINT32(temp, HRTIMER_TIMER_REG_BASE + TIMER_CONTROL);
}

UINT32 HrtimerClockValueGet(VOID)
{
    UINT32 temp;

    /* Read the current value of the timer3 */
    READ_UINT32(temp, HRTIMER_TIMER_REG_BASE + TIMER_VALUE);
    return temp;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
