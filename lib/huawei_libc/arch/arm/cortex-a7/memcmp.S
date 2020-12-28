/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: memcmp
 * Author: Huawei LiteOS Team
 * Create: 2020-10-30
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

.syntax unified

#define CACHE_LINE_SIZE     32

.global memcmp
.type memcmp,%function
memcmp:
.macro  unaligned_bytes_compare offsetlo offsethi
        ldr     r4, [r1], #4
10:
        /* r1 is unaligned, so it need to execute lsr and lsl opts to get full four bytes. */
        mov     ip, r4, lsr \offsetlo
        ldr     r3, [r0], #4
        ldr     r4, [r1], #4
        orr     ip, ip, r4, lsl \offsethi
        eors    ip, r3, ip /* compare r3 and ip, if it is not equal to 0, go to compare bt byte-wise. */
        bne     12f
        moveq   ip, r4, lsr \offsetlo
        ldreq   r3, [r0], #4
        ldreq   r4, [r1], #4
        orreq   ip, ip, r4, lsl \offsethi
        eorseq  ip, r3, ip
        bne     11f
        subs    r2, r2, #8
        bhi     10b /* if size is bigger than 0, continuing 8-bytes comparison in a cycle. */
        cmp     r2, #0
        beq     8f
        add     r2, r2, #8 /* sub is executed once more and size needs to be added back. */
11:
        sub     r0, r0, #8
        sub     r1, r1, #8 /* reback the address because the ldr instruction will add address. */
        b       13f
12:
        sub     r0, r0, #4
        sub     r1, r1, #4
13: /* r1 is executed bic instruction to aligne address, so needs to reback the unaligned bytes. */
        rsb     r5, r5, #4
        sub     r1, r1, r5
        b       7f
.endm

        cmp     r2, #0 /* if size is 0, return 0. */
        moveq   r0, #0
        bxeq     lr
        cmp     r0, r1 /* if r0 and r1 is same, return 0. */
        moveq   r0, #0
        bxeq     lr

        pld     [r0, #(CACHE_LINE_SIZE * 0)]
        pld     [r0, #(CACHE_LINE_SIZE * 1)]
        pld     [r1, #(CACHE_LINE_SIZE * 0)]
        pld     [r1, #(CACHE_LINE_SIZE * 1)]
        cmp     r2, #4 /* r2 is less 4 bytes, byte-wise compare. */
        bmi     .compare_1_byte_by_loop

        stmfd   sp!, {r4-r6, lr} /* save registers. */
        eor     ip, r0, r1
        tst     ip, #3
        /* r0 and r1 address is not aligned and their unaligned size is not same, then byte-wise compare. */
        bne     .compare_unaligned

        ands    ip,  r0, #3
        beq     .Laligned4_r0
        rsb     r4,  ip, #4
        sub     r2,  r2, r4
1: /* word-align the r0 address. */
        ldrb    r3,  [r0], #1
        ldrb    ip,  [r1], #1
        cmp     r3,  ip
        bne     9f
        subs    r4, r4, #1
        bhi     1b
        cmp     r2, #4 /* size is less 4 bytes, then byte-wise compare. */
        bmi     7f
        
.Laligned4_r0: /* here, r0 and r1 are both aligned. */
        cmp     r2, #32 /* size is less 32 bytes, 4-bytes compare. */
        bmi     .compare_4_bytes_by_loop
        mov     r6, r2

.compare_32_bytes_by_loop:
        pld     [r0, #(CACHE_LINE_SIZE * 2)]
        pld     [r1, #(CACHE_LINE_SIZE * 2)]
        ldrd    r2, r3, [r0], #8
        ldrd    r4, r5, [r1], #8
        eors    ip, r2, r4 /* is r2 and r4 same? */
        eorseq  ip, r3, r5 /* is r3 and r5 same? */
        ldrdeq  r2, r3, [r0], #8
        ldrdeq  r4, r5, [r1], #8
        eorseq  ip, r2, r4
        eorseq  ip, r3, r5
        ldrdeq  r2, r3, [r0], #8
        ldrdeq  r4, r5, [r1], #8
        eorseq  ip, r2, r4
        eorseq  ip, r3, r5
        ldrdeq  r2, r3, [r0], #8
        ldrdeq  r4, r5, [r1], #8
        eorseq  ip, r2, r4
        eorseq  ip, r3, r5
        bne     3f /* 32-bytes comparation, if it is not eqaul to 0, the value is not same, then byte-wise compare. */
        subs    r6, r6, #32
        cmp     r6, #32
        bmi     2f /* if size is less 32 bytes, exit 32-bytes compare. */
        bhi     .compare_32_bytes_by_loop /* 32-bytes comparation in a cycle. */
2:
        mov     r2, r6
        cmp     r2, #0
        beq     8f /* if size is 0, the value is same, return 0. */
        cmp     r2, #4
        bmi     7f /* if size is less 4 bytes, then byte-wise compare. otherwise, 4-bytes comparation in a cycle. */
        b       .compare_4_bytes_by_loop
3:
        sub    r0, r0, #8
        sub    r1, r1, #8 /* reback the address. */
        mov    r2, r6
        b      7f

.compare_unaligned:
        ands   ip, r0, #3
        beq    5f /* if r0 is aligned, go to execute unaligned bytes comparation. */
        rsb    r4, ip, #4
        sub    r2, r2, r4
4:
        ldrb   r3, [r0], #1
        ldrb   ip, [r1], #1
        cmp    r3, ip
        bne    9f
        subs   r4, r4, #1
        bhi    4b /* align the r0 address. */
5: /* here r0 is aligned at least. */
        cmp    r2, #8
        bmi    7f
        ands   r5, r1, #3
        bic    r1, r1, #3 /* aligne the r2 address. */
        cmp    r5, #2
        beq    .compare_aligned_2
        bge    .compare_aligned_3
.compare_aligned_1:
        unaligned_bytes_compare offsetlo=8, offsethi=24
.compare_aligned_2:
        unaligned_bytes_compare offsetlo=16, offsethi=16
.compare_aligned_3:
        unaligned_bytes_compare offsetlo=24, offsethi=8

.compare_4_bytes_by_loop:
        ldr    r3, [r0], #4
        ldr    ip, [r1], #4
        eors   ip, r3, ip
        bne    6f
        subs   r2, r2, #4
        bhi    .compare_4_bytes_by_loop
        cmp    r2, #0
        beq    8f
        add    r2, r2, #4
6:
        sub    r0, r0, #4
        sub    r1, r1, #4

7:
        ldmfd  sp!, {r4-r6, lr}
.compare_1_byte_by_loop:
        ldrb   r3, [r0], #1
        ldrb   ip, [r1], #1
        cmp    r3, ip
        bne    .return_value
        subs   r2, r2, #1
        bhi    .compare_1_byte_by_loop
        b      .return_0

8:
        ldmfd  sp!, {r4-r6, lr}
.return_0:
        mov    r0, #0
        bx     lr

9:
        ldmfd  sp!, {r4-r6, lr}
.return_value:
        sub    r0, r3, ip
        bx     lr
