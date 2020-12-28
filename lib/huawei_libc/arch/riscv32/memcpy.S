/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Memcpy
 * Author: Huawei LiteOS Team
 * Create: 2019-12-01
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

.macro MEMCPY_FIRST_32BYTE
#ifdef LOS_COMPILE_LDM
    ldmia {t1-t2, a6-a7, t3-t6}, (a1)
    stmia {t1-t2, a6-a7, t3-t6}, (t0)
#else
    lw    t1, 0(a1)
    lw    t2, 4(a1)
    lw    a6, 8(a1)
    lw    a7, 12(a1)
    lw    t3, 16(a1)
    lw    t4, 20(a1)
    lw    t5, 24(a1)
    lw    t6, 28(a1)
    sw    t1, 0(t0)
    sw    t2, 4(t0)
    sw    a6, 8(t0)
    sw    a7, 12(t0)
    sw    t3, 16(t0)
    sw    t4, 20(t0)
    sw    t5, 24(t0)
    sw    t6, 28(t0)
#endif
.endm

.macro MEMCPY_LAST_32BYTE
#ifdef LOS_COMPILE_LDM
    ldmia {t1-t2, a6-a7, t3-t6}, (a4)
    stmia {t1-t2, a6-a7, t3-t6}, (a5)
#else
    lw    t1, 0(a4)
    lw    t2, 4(a4)
    lw    a6, 8(a4)
    lw    a7, 12(a4)
    lw    t3, 16(a4)
    lw    t4, 20(a4)
    lw    t5, 24(a4)
    lw    t6, 28(a4)
    sw    t1, 0(a5)
    sw    t2, 4(a5)
    sw    a6, 8(a5)
    sw    a7, 12(a5)
    sw    t3, 16(a5)
    sw    t4, 20(a5)
    sw    t5, 24(a5)
    sw    t6, 28(a5)
#endif
.endm

.section .text
.globl memcpy
.type memcpy, @function
memcpy:
#ifdef LOSCFG_BASE_MEM_NODE_SIZE_CHECK
    addi    sp, sp, -20
    sw      a0, 0(sp)
    sw      a1, 4(sp)
    sw      a2, 8(sp)
    sw      a3, 12(sp)
    sw      ra, 16(sp) /* save registers. */
    li      a3, 1 /* notice OsMemSysNodeCheck this is memcpy. */
    jal     ra, OsMemSysNodeCheck
    beqz    a0, 1f /* if OsMemSysNodeCheck return LOS_OK, do memcpy as usual. */
    lw      ra, 16(sp)
    lw      a3, 12(sp)
    lw      a2, 8(sp)
    lw      a1, 4(sp)
    lw      a0, 0(sp) /* restore registers. */
    addi    sp, sp, 20
    li      a0, 0 /* if OsMemSysNodeCheck return LOS_NOK, memcpy return NULL. */
    ret
1:
    lw      ra, 16(sp)
    lw      a3, 12(sp)
    lw      a2, 8(sp)
    lw      a1, 4(sp)
    lw      a0, 0(sp)
    addi    sp, sp, 20
#endif
    add     a6, a0, a2
    li      a5, 3
    mv      t0, a0
    bleu    a2, a5, 11f
    xor     a5, a1, t0
    andi    a5, a5, 3
    bnez    a5, 1f

    andi    a4, t0, 3
    bnez    a4, 4f
5:
    addi    a3, a6, -32
    addi    a5, a6, -4
    bgtu    t0, a3, 7f
    addi    a3, a6, -64
    bgtu    t0, a3, 9f
8:
    addi    a4, a1, 32
#ifdef LOSCFG_DCACHE
    PREFD   0(a4)
#endif
    addi    a5, t0, 32
#ifdef LOSCFG_DCACHE
    PREFD   0(a5)
#endif

    MEMCPY_FIRST_32BYTE

    addi    a1, a1, 64
#ifdef LOSCFG_DCACHE
    PREFD   0(a1)
#endif
    addi    t0, t0, 64
#ifdef LOSCFG_DCACHE
    PREFD   0(t0)
#endif

    MEMCPY_LAST_32BYTE

    bleu    t0, a3, 8b
    add     a6, a0, a2
    j       5b
9:
    MEMCPY_FIRST_32BYTE
    addi    a1, a1, 32
    addi    t0, t0, 32

    add     a6, a0, a2
    addi    a5, a6, -4
    j       7f
4:
    lbu     a4, 0(a1)
    addi    t0, t0, 1
    addi    a1, a1, 1
    sb      a4, -1(t0)
    bleu    a6, t0, 2f
    andi    a4, t0, 3
    bnez    a4, 4b

    j       5b
6:
    lw      a4, 0(a1)
    addi    t0, t0, 4
    addi    a1, a1, 4
    sw      a4, -4(t0)
    bleu    t0, a5, 6b
7:
    bleu    a6, t0, 2f
    bgtu    t0, a5, 4b
    j       6b
1:
    bleu    a6, a0, 2f
3:
    andi    a7, a1, 3
    beqz    a7, 10f

    lbu     a4, 0(a1)
    addi    t0, t0, 1
    addi    a1, a1, 1
    sb      a4, -1(t0)

    bleu    a6, t0, 2f
    j       3b
10:
    addi    a7, t0, 36
    bleu    a7, a6, 12f
11:
    bleu    a6, t0, 2f
    lbu     a4, 0(a1)
    addi    t0, t0, 1
    addi    a1, a1, 1
    sb      a4, -1(t0)
    j       11b
12:
    andi    a7, t0, 3
    li      a5, 2
    beq     a7, a5, 13f
    li      a5, 1
    beq     a7, a5, 15f
    li      a5, 3
    beq     a7, a5, 17f
13:
    lw      t5, 0(a1)
    addi    a1, a1, 4
    sh      t5, 0(t0)
    addi    t6, t0, 32
    addi    t0, t0, 2
    srli    t5, t5, 16
14:
#ifdef LOS_COMPILE_LDM
    ldmia   {t1-t2, a3-a5, a7, t3-t4}, (a1)
#else
    lw      t4, 0(a1)
    lw      t3, 4(a1)
    lw      a7, 8(a1)
    lw      a5, 12(a1)
    lw      a4, 16(a1)
    lw      a3, 20(a1)
    lw      t2, 24(a1)
    lw      t1, 28(a1)
#endif

#ifdef LOSCFG_DCACHE
    PREFD   0(t6)
#endif
    addi    t6, a1, 64
#ifdef LOSCFG_DCACHE
    PREFD   0(t6)
#endif

    slli    t6, t4, 16
    or      t5, t5, t6

    srli    t4, t4, 16
    slli    t6, t3, 16
    or      t4, t4, t6

    srli    t3, t3, 16
    slli    t6, a7, 16
    or      t3, t3, t6

    srli    a7, a7, 16
    slli    t6, a5, 16
    or      a7, a7, t6

    srli    a5, a5, 16
    slli    t6, a4, 16
    or      a5, a5, t6

    srli    a4, a4, 16
    slli    t6, a3, 16
    or      a4, a4, t6

    srli    a3, a3, 16
    slli    t6, t2, 16
    or      a3, a3, t6

    srli    t2, t2, 16
    slli    t6, t1, 16
    or      t2, t2, t6

    addi    a1, a1, 32
    addi    t6, t0, 66

#ifdef LOS_COMPILE_LDM
    stmia   {t2, a3-a5,a7, t3-t5}, (t0)
#else
    sw      t5, 0(t0)
    sw      t4, 4(t0)
    sw      t3, 8(t0)
    sw      a7, 12(t0)
    sw      a5, 16(t0)
    sw      a4, 20(t0)
    sw      a3, 24(t0)
    sw      t2, 28(t0)
#endif
    addi    t0, t0, 32
    srli    t5, t1, 16

    bleu    t6, a6, 14b
    sh      t5, 0(t0)
    addi    t0, t0, 2
    j       11b
15:
    lw      t5, 0(a1)
    addi    a1, a1, 4
    sb      t5, 0(t0)
    srli    t5, t5, 8
    sh      t5, 1(t0)
    srli    t5, t5, 16
    addi    t6, t0, 32
    addi    t0, t0, 3
16:
#ifdef LOS_COMPILE_LDM
    ldmia   {t1-t2, a3-a5,a7, t3-t4}, (a1)
#else
    lw      t4, 0(a1)
    lw      t3, 4(a1)
    lw      a7, 8(a1)
    lw      a5, 12(a1)
    lw      a4, 16(a1)
    lw      a3, 20(a1)
    lw      t2, 24(a1)
    lw      t1, 28(a1)
#endif
#ifdef LOSCFG_DCACHE
    PREFD   0(t6)
#endif
    addi    t6, a1, 64
#ifdef LOSCFG_DCACHE
    PREFD   0(t6)
#endif

    slli    t6, t4, 8
    or      t5, t5, t6

    srli    t4, t4, 24
    slli    t6, t3, 8
    or      t4, t4, t6

    srli    t3, t3, 24
    slli    t6, a7, 8
    or      t3, t3, t6

    srli    a7, a7, 24
    slli    t6, a5, 8
    or      a7, a7, t6

    srli    a5, a5, 24
    slli    t6, a4, 8
    or      a5, a5, t6

    srli    a4, a4, 24
    slli    t6, a3, 8
    or      a4, a4, t6

    srli    a3, a3, 24
    slli    t6, t2, 8
    or      a3, a3, t6

    srli    t2, t2, 24
    slli    t6, t1, 8
    or      t2, t2, t6

    addi    a1, a1, 32
    addi    t6, t0, 65

#ifdef LOS_COMPILE_LDM
    stmia   {t2, a3-a5,a7, t3-t5}, (t0)
#else
    sw      t5, 0(t0)
    sw      t4, 4(t0)
    sw      t3, 8(t0)
    sw      a7, 12(t0)
    sw      a5, 16(t0)
    sw      a4, 20(t0)
    sw      a3, 24(t0)
    sw      t2, 28(t0)
#endif
    addi    t0, t0, 32
    srli    t5, t1, 24

    bleu    t6, a6, 16b
    sb      t5, 0(t0)
    addi    t0, t0, 1
    j       11b
17:
    lw      t5, 0(a1)
    addi    a1, a1, 4
    sb      t5, 0(t0)
    addi    t6, t0, 32
    addi    t0, t0, 1
    srli    t5, t5, 8
18:
#ifdef LOS_COMPILE_LDM
    ldmia   {t1-t2, a3-a5,a7, t3-t4}, (a1)
#else
    lw      t4, 0(a1)
    lw      t3, 4(a1)
    lw      a7, 8(a1)
    lw      a5, 12(a1)
    lw      a4, 16(a1)
    lw      a3, 20(a1)
    lw      t2, 24(a1)
    lw      t1, 28(a1)
#endif
#ifdef LOSCFG_DCACHE
    PREFD   0(t6)
#endif
    addi    t6, a1, 64
#ifdef LOSCFG_DCACHE
    PREFD   0(t6)
#endif

    slli    t6, t4, 24
    or      t5, t5, t6

    srli    t4, t4, 8
    slli    t6, t3, 24
    or      t4, t4, t6

    srli    t3, t3, 8
    slli    t6, a7, 24
    or      t3, t3, t6

    srli    a7, a7, 8
    slli    t6, a5, 24
    or      a7, a7, t6

    srli    a5, a5, 8
    slli    t6, a4, 24
    or      a5, a5, t6

    srli    a4, a4, 8
    slli    t6, a3, 24
    or      a4, a4, t6

    srli    a3, a3, 8
    slli    t6, t2, 24
    or      a3, a3, t6

    srli    t2, t2, 8
    slli    t6, t1, 24
    or      t2, t2, t6

    addi    a1, a1, 32
    addi    t6, t0, 67

#ifdef LOS_COMPILE_LDM
    stmia   {t2, a3-a5,a7, t3-t5}, (t0)
#else
    sw      t5, 0(t0)
    sw      t4, 4(t0)
    sw      t3, 8(t0)
    sw      a7, 12(t0)
    sw      a5, 16(t0)
    sw      a4, 20(t0)
    sw      a3, 24(t0)
    sw      t2, 28(t0)
#endif
    addi    t0, t0, 32
    srli    t5, t1, 8

    bleu    t6, a6, 18b
    sh      t5, 0(t0)
    srli    t5, t5, 16
    sb      t5, 2(t0)
    addi    t0, t0, 3
    j       11b
2:
    ret

.size memcpy, .-memcpy
