/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: memset
 * Author: Huawei LiteOS Team
 * Create: 2020-09-03
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
// To avoid warning about deprecated instructions, add an explicit
// arch. The code generated is exactly the same.
.arch armv7-a
.fpu        neon

.global memset
.type memset,%function
memset:
#ifdef LOSCFG_BASE_MEM_NODE_SIZE_CHECK
        stmfd   sp!, {r0-r3, lr}

        mov r3, #0 /* notice OsMemSysNodeCheck this is memset */
        bl OsMemSysNodeCheck
        cmp r0, #0 /* if OsMemSysNodeCheck return LOS_OK, do memset as usual */
        ble 1f
        ldmfd   sp!, {r0-r3, lr}
        mov r0, #0 /* if OsMemSysNodeCheck return LOS_NOK, memset return NULL */
        mov pc, lr

1:      ldmfd   sp!, {r0-r3, lr}
#endif
        and     r3, r1, #0xff
        vdup.8  q0, r3 /* Copy r3 to q0 in an 8-bit cycle. */
        mov     ip, r0
        cmp     r2, #0x4
        it      lt
        blt     .set_less_than_4_bytes

.check_alignment:
        ands    r1, ip, #0x03 /* Get non aligned length. */
        bne     .set_unaligned_bytes

.memset_wordaligned:
        vmov.u32 r1, d0[0] /* Save four bytes to r1 in d0. */
        vmov     q1, q0

        cmp      r2, #0x10
        it       lt
        blt      .set_less_than_16_bytes
        cmp      r2, #0x20
        it       lt
        blt      .set_less_than_32_bytes

        vmov    q2, q0
        vmov    q3, q0 /* Save for vstmia. */
        cmp     r2, #0x80
        it      lt
        blt     .set_less_than_128_bytes

/* Main loop stores 128 bytes at a time. */
.set_128_bytes_loop:
        subs     r2, r2, #0x80
        vstmiage ip!, {d0 - d7}
        vstmiage ip!, {d0 - d7} /* Set 128 bytes. */
        bgt      .set_128_bytes_loop
        it       eq
        bxeq     lr
        add      r2, r2, #0x80 /* Adjust for extra sub. */

.set_less_than_128_bytes:
        cmp     r2,  #0x40
        bcc     .set_less_than_64_bytes
        sub     r2,  #0x40
        vstmia  ip!, {d0 - d7} /* Set 64 bytes. */

.set_less_than_64_bytes:
        cmp     r2,  #0x20
        bcc     .set_less_than_32_bytes
        sub     r2,  #0x20
        vstmia  ip!, {d0 - d3} /* Set 32 bytes. */

.set_less_than_32_bytes:
        cmp     r2,  #0x10
        bcc     .set_less_than_16_bytes
        sub     r2,  #0x10
        vstmia  ip!, {d0 - d1} /* Set 16 bytes. */

.set_less_than_16_bytes:
        cmp     r2,  #0x08
        bcc     .set_less_than_8_bytes
        sub     r2,  #0x08
        vstmia  ip!, {d0} /* Set 8 bytes. */

.set_less_than_8_bytes:
        cmp     r2,  #0x04
        bcc     .set_less_than_4_bytes
        sub     r2,  #0x04
        stmia   ip!, {r1} /* Set 4 bytes. */

.set_less_than_4_bytes:
        cmp     r2, #0x02
        it      lt
        blt     .set_less_than_2_bytes
        it      ge
        strbge  r3, [ip], #0x01
        it      gt
        strbgt  r3, [ip], #0x01

.set_less_than_2_bytes:
        cmp     r2, #0x0
        it      eq
        bxeq    lr
        strb    r3, [ip]
        bx      lr

.set_unaligned_bytes:
        rsb     r1, r1, #0x04 /* Get unaligned length. */
        cmp     r1, #0x02
        it      ge
        strbge  r3, [ip], #0x01
        it      gt
        strbgt  r3, [ip], #0x01
        strb    r3, [ip], #0x01
        sub     r2, r2, r1
        cmp     r2, #0x04
        it      lt
        blt     .set_less_than_4_bytes
        b       .memset_wordaligned