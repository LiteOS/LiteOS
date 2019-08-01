/*----------------------------------------------------------------------------
 * Copyright (c) <2019>, <Huawei Technologies Co., Ltd>
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

    .syntax unified
    .thumb

/**
 * @brief  This is the code that gets called when the processor first
 *         starts execution following a reset event. Only the absolutely
 *         necessary set is performed, after which the application
 *         supplied main() routine is called.
 *         This the default implementation for gcc, this routine is 
 *         defined as weak so it can be overrided.
 * @param  None
 * @retval : None
 */
    .text

    .weak   Reset_Handler
    .type   Reset_Handler, %function
Reset_Handler:
    cpsid   i
    ldr     sp, =_estack            /* set msp, as it is not set in the vector */

#if defined (__VFP_FP__) && !defined(__SOFTFP__)    /* if FPU exist. */
    /* enable FPU */

    ldr     r0, =0xE000ED88
    ldr     r1, [r0]
    orr     r1, r1, #(0xf << 20)                    /* enable cp10 cp11 both */
    str     r1, [r0]
#endif

    /* init the data section */

    ldr     r1, =_sdata
    ldr     r2, =_edata
    ldr     r3, =_sidata
0:
    cmp     r1, r2
    beq     0f
    ldr     r0, [r3], #4
    str     r0, [r1], #4
    b       0b
0:

    /* init the bss section */

    mov     r0, #0
    ldr     r1, =_sbss
    ldr     r2, =_ebss
0:
    cmp     r1, r2
    beq     0f
    str     r0, [r1], #4
    b       0b
0:
    bl      SystemInit
    bl      __libc_init_array
    bl      main

    b       .

.size  Reset_Handler, . - Reset_Handler
