/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: mmu
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

    .global _platform_setup
    .fpu vfpv4
    .arch armv7a

.macro INVALIDATE_DCACHE_ALL
    /*
     * Invalidate Data cache
     * to make the code general purpose, we calculate the
     * cache size first and loop through each set + way
     */
    mrc    p15, 1, r0, c0, c0, 0           /* Read Cache Size ID */
    mov    r3, #0x1ff
    and    r0, r3, r0, lsr #13             /* r0 = no. of sets - 1 */
    mov    r1, #0                          /* r1 = way counter way_loop */
way_loop:
    mov    r3, #0                          /* r3 = set counter set_loop */
set_loop:
    mov    r2, r1, lsl #30
    orr    r2, r3, lsl #5                  /* r2 = set/way cache operation format */
    mcr    p15, 0, r2, c7, c6, 2           /* Invalidate line described by r2 */
    add    r3, r3, #1                      /* Increment set counter */
    cmp    r0, r3                          /* Last set reached yet */
    bgt    set_loop                        /* if not, iterate set_loop */
    add    r1, r1, #1                      /* else, next */
    cmp    r1, #4                          /* Last way reached yet */
    bne    way_loop                        /* if not, iterate way_loop */

    mcr    p15, 0, r1, c8, c7, 0           /* Invalidate TLB */
.endm

_platform_setup:
    push   {lr}

    cmp    r11, #0
    bleq   board_config

    mrc    p15, 0, r0, c1, c0, 0
    bic    r0, r0, #0x1000                 /* disable ICache [SCTRL:bit 12 set as 0] */
    bic    r0, r0, #0x000f                 /* disable DCache, write buffer */
    mcr    p15, 0, r0, c1, c0, 0
    ISB

    INVALIDATE_DCACHE_ALL

    mov    r0, #0                          /* TTBCR, Translation Table Base Control Register, select TTBR0 */
    mcr    p15, 0, r0, c2, c0, 2
    ISB

    bl     OsMmuInit                       /* MMU init */

    mrc    p15, 0, r0, c1, c0, 1           /* ACTLR, Auxlliary Control Register */
    orr    r0, r0, #(1U << 6)              /* SMP, Enables coherent requests to the processor. */
    orr    r0, r0, #(1U << 2)              /* Enable D-side prefetch */
    orr    r0, r0, #(1U << 11)             /* Global BP Enable bit */
    mcr    p15, 0, r0, c1, c0, 1           /* ACTLR, Auxlliary Control Register */

    ldr    r2, =10f

    DSB
    mov    r0, #0
    tst    r11, #0xf
    mcrne  p15, 0, r0, c8, c7, 0           /* TBLIALL, invalidate unified TLB */

    mrc    p15, 0, r0, c1, c0, 0           /* SCTLB, System Control Register */
    bic    r0, #((1U << 29) | (1U << 28))  /* Disable TRE/AFE */
    orr    r0, r0, #(1U << 12)             /* enable ICache */
    orr    r0, r0, #(1U << 2)              /* Dcache enable */
    orr    r0, r0, #(1U << 8)              /* 'S' bit */
    orr    r0, r0, #(1U << 5)              /* CP15BEN bit */
    orr    r0, r0, #1                      /* mmu enable */
    mcr    p15, 0, r0, c1, c0, 0           /* SCTLB, System Control Register */
    ISB

    mov    pc, r2
    nop
    nop
    nop
10:
    pop   {pc}
