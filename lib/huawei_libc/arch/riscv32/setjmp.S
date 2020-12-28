/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: setjmp/longjmp
 * Author: Huawei LiteOS Team
 * Create: 2020-10-13
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

  .globl  setjmp
  .type   setjmp, @function
setjmp:
#ifdef LOS_COMPILE_LDM
    stmia {ra, sp, s0-s1}, (a0) /* store value in a0 address to registers. */
#else
    sw ra,  0(a0)
    sw s0,  4(a0)
    sw s1,  8(a0)
    sw sp,  12(a0)
#endif

    li      a0, 0
    ret
    .size   setjmp, .-setjmp

  .globl  longjmp
  .type   longjmp, @function
longjmp:
#ifdef LOS_COMPILE_LDM
    ldmia {ra, sp, s0-s1}, (a0)
#else
    lw ra,  0(a0)
    lw s0,  4(a0)
    lw s1,  8(a0)
    lw sp,  12(a0)
#endif

    beqz a1, 1f /* if a1 is equal to 0, go to 1f and return 1. */
    move a0, a1 /* return a1. */
    ret
1:
    li   a0, 1
    ret
    .size   longjmp, .-longjmp
