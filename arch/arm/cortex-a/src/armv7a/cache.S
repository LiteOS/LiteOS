/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: cache
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

#include "asm.h"

    .fpu vfpv4
    .arch armv7a
.macro  DCACHE_LINE_SIZE, reg, tmp
    mrc     p15, 0, \tmp, c0, c0, 1
    lsr     \tmp, \tmp, #16
    and     \tmp, \tmp, #0xf
    mov     \reg, #4
    mov     \reg, \reg, lsl \tmp
.endm


FUNCTION(arm_inv_cache_range)
    push    {r2, r3}
    DCACHE_LINE_SIZE r2, r3
    sub    r3, r2, #1
    tst    r0, r3
    bic    r0, r0, r3

    mcrne  p15, 0, r0, c7, c14, 1

    tst    r1, r3
    bic    r1, r1, r3
    mcrne  p15, 0, r1, c7, c14, 1
1:
    mcr    p15, 0,  r0, c7, c6, 1
    add    r0,  r0, r2
    cmp    r0,  r1
    blo    1b
    dsb
    pop    {r2, r3}
    mov    pc, lr

FUNCTION(arm_clean_cache_range)
    push   {r2, r3}
    DCACHE_LINE_SIZE r2, r3
    sub    r3, r2, #1
    bic    r0, r0, r3

1:
    mcr    p15, 0,  r0, c7, c10, 1
    add    r0,  r0, r2
    cmp    r0,  r1
    blo    1b
    dsb
    pop    {r2, r3}
    mov    pc, lr
