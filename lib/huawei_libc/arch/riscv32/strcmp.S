/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: strcmp
 * Author: Huawei LiteOS Team
 * Create: 2020-10-10
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

#if BYTE_ORDER != LITTLE_ENDIAN
# error
#endif

.text
.globl strcmp
.type  strcmp, @function
strcmp:
    andi t0, a0, 3
    andi t1, a1, 3
    bnez t0, .misaligned0 /* a0 address is unaligned, go to .misaligned0. */
    bnez t1, .misaligned1 /* a1 address is unaligned, go to .misaligned1. */

.loop_ready:
    li   a6, 0x7f7f7f7f
    li   t6, -1

.loop_four_bytes:
    lw   t0, 0(a0)
    lw   t1, 0(a1)
    and  t2, t0, a6
    or   t3, t0, a6
    add  t2, t2, a6
    or   t2, t2, t3 /* calc whether there is null byte in the word. */
    bne  t2, t6, .null_byte
    bne  t0, t1, .mismatch
    addi a0, a0, 4
    addi a1, a1, 4
    j    .loop_four_bytes

.mismatch:
/* word don't match, but t0 has no null byte. */
    andi  a5, t0, 0xff
    andi  a4, t1, 0xff
    bne   a5, a4, .ret_sub_value /* compare the first byte. */
    addi  a0, a0, 1
    addi  a1, a1, 1
    srl   a5, t0, 8
    srl   a4, t1, 8
    andi  a5, a5, 0xff
    andi  a4, a4, 0xff
    bne   a5, a4, .ret_sub_value /* compare the second byte. */
    addi  a0, a0, 1
    addi  a1, a1, 1
    srl   a5, t0, 16
    srl   a4, t1, 16
    andi  a5, a5, 0xff
    andi  a4, a4, 0xff
    bne   a5, a4, .ret_sub_value /* compare the third byte. */
    addi  a0, a0, 1
    addi  a1, a1, 1
    srl   a5, t0, 24
    srl   a4, t1, 24
    andi  a5, a5, 0xff
    andi  a4, a4, 0xff
    bne   a5, a4, .ret_sub_value /* compare the fourth byte. */
    j     .ret_0

.null_byte:
    bne   t0, t1, .misaligned1
    j     .ret_0


.misaligned0:
    beq  t0, t1, .misaligned_types /* if the unaliged size is equal, process unaligned bytes first. */
    j    .misaligned1

.misaligned_types:
    li    t2, 4
    sub   t3, t2, t0 /* calc unaligend size. */
    add   a3, a0, t3

.loop_unaligned_types:
    lbu  a5, 0(a0)
    lbu  a4, 0(a1)
    addi a0, a0, 1
    addi a1, a1, 1
    beqz a5, .ret_sub_value
    bne  a5, a4, .ret_sub_value
    bltu a0, a3, .loop_unaligned_types /* compare unaligend bytes by loop. */
    j   .loop_ready

.misaligned1: /* r0 is aligned address, r1 is not aligned address. */
    lbu  a5, 0(a0)
    lbu  a4, 0(a1)
    addi a0, a0, 1
    addi a1, a1, 1
    bne  a5, a4, .ret_sub_value
    bnez a5, .misaligned1 /* compare string with one byte by loop. */

.ret_0:
    li   a0, 0
    ret

.ret_sub_value:
    sub  a0, a5, a4
    ret

.size   strcmp, .-strcmp
