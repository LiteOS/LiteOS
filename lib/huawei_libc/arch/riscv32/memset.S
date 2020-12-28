/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: memset
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

.global memset
.type memset,%function
memset:
#ifdef LOSCFG_BASE_MEM_NODE_SIZE_CHECK
    addi    sp, sp, -20
    sw      a0, 0(sp)
    sw      a1, 4(sp)
    sw      a2, 8(sp)
    sw      a3, 12(sp)
    sw      ra, 16(sp) /* save registers. */
    li      a3, 0 /* notice OsMemSysNodeCheck this is memset. */
    jal     ra, OsMemSysNodeCheck
    beqz    a0, 1f /* if OsMemSysNodeCheck return LOS_OK, do memset as usual. */
    lw      ra, 16(sp)
    lw      a3, 12(sp)
    lw      a2, 8(sp)
    lw      a1, 4(sp)
    lw      a0, 0(sp) /* restore registers. */
    addi    sp, sp, 20
    li      a0, 0 /* if OsMemSysNodeCheck return LOS_OK, do memset as usual. */
    ret
1:
    lw      ra, 16(sp)
    lw      a3, 12(sp)
    lw      a2, 8(sp)
    lw      a1, 4(sp)
    lw      a0, 0(sp)
    addi    sp, sp, 20
#endif
    move    t0, a0
    beqz    a2, 5f /* size is zero, go to 5f. */

    li      a3, 32
    bltu    a2, a3, 3f /* if size < 32, go to 3f. */

    neg     a5, t0 /* a5 = -t0 */
    andi    a5, a5, 3 /* calculate unalined size. */
    bnez    a5, 6f /* not eqaul to zero , go 6f. */
1:
    andi    a4, a2, ~3 /* remove non-aligned bytes temporarily. */
    add     a7, t0, a4 /* calculate end address. */
    andi    a5, a1, 255
    slli    a6, a5, 8
    or      a5, a6, a5
    slli    a6, a5, 16
    or      a5, a6, a5 /* update data in four bytes. */
    rem     a4, a4, a3 /* calculate the reminder. */
    bnez    a4, 8f /* if a4 is not zero, go to deal reminder. */
2:
    sw      a5, 0(t0)
    sw      a5, 4(t0)
    sw      a5, 8(t0)
    sw      a5, 12(t0)
    sw      a5, 16(t0)
    sw      a5, 20(t0)
    sw      a5, 24(t0)
    sw      a5, 28(t0)
    addi    t0, t0, 32
    bltu    t0, a7, 2b /* circular write operation with 32 bytes. */
    andi    a2, a2, 3 /* get unalined size. */
    beqz    a2, 5f
3:
    add     a6, t0, a2 /* if size < 32, go here. */
4:
    sb      a1, 0(t0)
    addi    t0, t0, 1
    bltu    t0, a6, 4b /* circular write operation with one byyte. */
5:
    ret

6:
    add     a6, t0, a5 /* a6 is end address. */
7:
    sb      a1, 0(t0) /* store low byte of a1 to [t0+0]. */
    addi    t0, t0, 1
    bltu    t0, a6, 7b /* circular write operation. */
    sub     a2, a2, a5 /* update size. */
    j       1b

8:
    add     a6, t0, a4
9:
    sb      a1, 0(t0)
    addi    t0, t0, 1
    bltu    t0, a6, 9b /* circular write operation. */
    j       2b
