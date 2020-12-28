/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: strcmp
 * Author: Huawei LiteOS Team
 * Create: 2020-10-22
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

.syntax         unified
.arch armv7-a
.fpu        neon

#ifdef __ARMEB__
#define S2LOMEM lsl
#define S2HIMEM lsr
#else
#define S2LOMEM lsr
#define S2HIMEM lsl
#endif

.global strcmp
.type strcmp,%function
strcmp:
.macro compare_word_bytes offsetlo offsethi
    mov     r6, #0
    ldr     r5, [r1], 4
    .p2align        2
9:
    ldr     r3, [r0], 4
    and     r2, r3, r7, S2LOMEM #\offsetlo
    cmp     r2, r5, S2LOMEM #\offsetlo /* compare 4-offsetlo bytes. */
    ittt    eq
    uadd8eq r8, r3, r7
    seleq   r8, r6, r7 /* is there a zero byte in r3? */
    cmpeq   r8, #0 /* if r8 is not equal to 0, it has zero byte in r3.*/
    bne     10f
    ldr     r5, [r1], 4
    eor     r2, r2, r3
    cmp     r2, r5, S2HIMEM #\offsethi /* compare offsetlo bytes. */
    bne     11f
    b       9b
.endm

.macro find_zero_or_different_bytes w1 w2 branch
    uadd8 ip, \w1, r7
    eor   ip, \w1, \w2
    sel   ip, ip, r7
    cmp   ip, #0 /* there's a zero or a different byte in the word if ip is not equal to 0. */
    bne   \branch
.endm

    stmfd   sp!, {r4-r8, lr}
    mvn     r7, #0 /* r7 are used to to calc zero byte. */
    pld     [r0, #0]
    pld     [r1, #0]

    eor     ip, r0, r1
    tst     ip, #3 /* it is aligned or has the same unaligned size, if not, go to .Lstrcmp_unaligned. */
    bne     .Lstrcmp_unaligned

    ands    ip, r0, #7
    beq     .Laligned8_r0
    ands    ip, r0, #3
    beq     .Laligned4_r0
    rsb     ip, ip, #4 /* ip = 4 - ip. */

1:
    ldrb    r3, [r0], #1
    ldrb    r4, [r1], #1
    cmp     r3, #0
    beq     .Lfast_return
    cmp     r3, r4
    bne     .Lfast_return
    subs    ip, ip, #1
    bne     1b /* compare byte by byte until the first string is word-aligned. */
    orr     ip, r0, r1
    tst     ip, #7 /* are both 8-bytes aligned. */
    beq     .Laligned8_r0_aligned8_r1
    b       .Laligned4_r0 /* the first string is word-aligned here. */

.Laligned8_r0:
    ands ip, r1, #7 /* here, r0 is aligned 8 bytes. */
    beq  .Laligned8_r0_aligned8_r1
    ands ip, r1, #3
    beq  .Laligned8_r0_aligned4_r1

.Laligned8_r0_aligned8_r1: /* double-word bytes compare in loop. */
    .p2align        2
2:
    pld [r0, #16]
    pld [r1, #16]
    ldrd  r2, r3, [r0], #8
    ldrd  r4, r5, [r1], #8
    find_zero_or_different_bytes w1=r2 w2=r4 branch=.return_88
    find_zero_or_different_bytes w1=r3 w2=r5 branch=.return_44
    b  2b

.Laligned8_r0_aligned4_r1: /* word bytes compare in loop. */
    ldr  r5, [r1], #4
    .p2align 2
3:
    pld [r0, #16]
    pld [r1, #16]
    ldrd r2, r3, [r0], #8
    find_zero_or_different_bytes w1=r2 w2=r5 branch=.return_84
    ldrd r4, r5, [r1], #8
    find_zero_or_different_bytes w1=r3 w2=r4 branch=.return_48
    b    3b

.Lstrcmp_unaligned:
    ands ip, r0, #3
    beq  5f /* if r0 is aligned, go to execute unaligned compare. */
    rsb   ip, ip, #4
4: /* align r0 address and execute unaligned compare. */
    ldrb  r3, [r0], #1
    ldrb  r4, [r1], #1
    cmp   r3, r4
    bne  .Lfast_return
    cmp   r3, #0
    beq  .Lfast_return
    subs ip, ip, #1 /* compare unaligned bytes to align r0 address. */
    bne    4b

5:
    ands    ip, r1, #3
    bic     r1, r1, #3 /* align unaligned r1 address. */
    cmp     ip, #2
    beq     .Laligned_2
    bge     .Laligned_3
.Laligned_1:
    compare_word_bytes offsetlo=8 offsethi=24
.Laligned_2:
    compare_word_bytes offsetlo=16 offsethi=16
.Laligned_3:
    compare_word_bytes offsetlo=24 offsethi=8

.Laligned4_r0:
    ands ip, r1, #7
    beq  .Laligned4_r0_aligned8_r1
    b .Laligned4_r0_aligned4_r1

.Laligned4_r0_aligned8_r1: /* word bytes compare in loop. */
    ldr  r5, [r0], #4
    .p2align        2
6:
    pld [r0, #16]
    pld [r1, #16]
    ldrd r2, r3, [r1], #8
    find_zero_or_different_bytes w1=r5 w2=r2 branch=.return_48
    ldrd r4, r5, [r0], #8
    find_zero_or_different_bytes w1=r4 w2=r3 branch=.return_84
    b    6b

.Laligned4_r0_aligned4_r1: /* word bytes compare in loop. */
    ldr  r3, [r0], #4
    ldr  r5, [r1], #4
    find_zero_or_different_bytes w1=r3 w2=r5 branch=.return_44
    b .Laligned8_r0_aligned8_r1

8:
    ldrb  r3, [r0], #1
    ldrb  r4, [r1], #1 /* one byte compare in loop. */
    cmp   r3, #0
    beq   .Lfast_return
    cmp   r3, r4
    bne   .Lfast_return /* if result is 0, continuing to compare until there is zero byte or result is not 0. */
    b     8b

10:
    rsb ip, ip, #4
    sub r0, r0, #4
    sub r1, r1, ip
    b    8b

11:
    sub r0, r0, ip
    sub r1, r1, #4
    b    8b

.return_88:
    sub r0, r0, #8
    sub r1, r1, #8
    b    8b

.return_44:
    sub r0, r0, #4
    sub r1, r1, #4
    b    8b

.return_84:
    sub r0, r0, #8
    sub r1, r1, #4
    b    8b

.return_48:
    sub r0, r0, #4
    sub r1, r1, #8
    b    8b

.Lfast_return:
    sub   r0, r3, r4
    ldmfd sp!, {r4-r8, lr}
    bx      lr
