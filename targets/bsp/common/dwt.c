/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Dwt
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

#include "dwt.h"

static UINT32 cpuclkfeq;

void dwt_delay_init(UINT32 clk)
{
    cpuclkfeq = clk;
    DEM_CR |= DEM_CR_TRCENA;
    DWT_CYCCNT = 0u;
    DWT_CR |= DWT_CR_CYCCNTENA;
}

VOID delayus(UINT32 usec)
{
    UINT32 startts, endts, ts;
    UINT32 uwIntSave;
    startts = DWT_CYCCNT;

    ts = usec * (cpuclkfeq / (1000 * 1000));
    endts = startts + ts;
    uwIntSave = LOS_IntLock();
    if (endts > startts) {
        while (DWT_CYCCNT < endts) {
        }
    } else {
        while (DWT_CYCCNT > endts) {
        }
        while (DWT_CYCCNT < endts) {
        }
    }
    (VOID)LOS_IntRestore(uwIntSave);
}

VOID delay10ms(__IO UINT32 nTime)
{
    delayus(1000 * nTime);
}
