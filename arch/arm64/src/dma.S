/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: LiteOS dma implementaton
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

    .global v8_dma_inv_range
    .global v8_dma_clean_range
    .global setjmp
    .global longjmp

    .equ CACHE_LINE, 64

.macro CACHE_RANGE_OP, cache op
    bic     x3, x0, #(CACHE_LINE - 1)  /* align the start with a cache line */
.L_CACHE_RANGE_OP_LOOP\@:
    \cache  \op, x3
    add     x3, x3, #CACHE_LINE
    cmp     x3, x1
    blo     .L_CACHE_RANGE_OP_LOOP\@
    dsb     sy
.endm

    .type v8_dma_inv_range, %function;
    .size v8_dma_inv_range, .-v8_dma_inv_range
v8_dma_inv_range:
    CACHE_RANGE_OP dc ivac  /* invalidate dcache to PoC by MVA */
    ret

    .type v8_dma_clean_range, %function;
    .size v8_dma_clean_range, .-v8_dma_clean_range
v8_dma_clean_range:
    CACHE_RANGE_OP dc cvac  /* clean cache to PoC by MVA */
    ret

    .type longjmp, %function;
    .size longjmp, .-longjmp
longjmp:
    ldp     x19, x20, [x0, #0]
    ldp     x21, x22, [x0, #16]
    ldp     x23, x24, [x0, #32]
    ldp     x25, x26, [x0, #48]
    ldp     x27, x28, [x0, #64]
    ldp     x29, x30, [x0, #80]
    ldr     x2, [x0, #96]
    ldp     d8, d9,   [x0, #104]
    ldp     d10, d11, [x0, #120]
    ldp     d12, d13, [x0, #136]
    ldp     d14, d15, [x0, #152]
    mov     sp, x2
    mov     x0, x1
    cbnz    x1, 1f
    mov     x0, #1
1:
    br      x30

    .type setjmp, %function;
    .size setjmp, .-setjmp
setjmp:
    mov     x1, sp
    stp     x19, x20, [x0, #0]
    stp     x21, x22, [x0, #16]
    stp     x23, x24, [x0, #32]
    stp     x25, x26, [x0, #48]
    stp     x27, x28, [x0, #64]
    stp     x29, x30, [x0, #80]
    str     x1, [x0, #96]
    stp     d8, d9, [x0, #104]
    stp     d10, d11, [x0, #120]
    stp     d12, d13, [x0, #136]
    stp     d14, d15, [x0, #152]
    mov     x0, #0 /* set the return value of setjmp */
    br      x30
