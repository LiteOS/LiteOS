/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Aarch64 Hw Exc Implementation
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

    .extern OsExceptSyncExcHdl
    .extern OsCurrentELGet
    .extern _osExceptIrqHdl

#include "arch/regs.h"

#define ALIGN .align
#define ORG .org
#define MACRO   .macro
#define ENDM    .endm

    LR  .req    x30

    .arch armv8-a
    .text
    ALIGN 12

MACRO push_reg reg1, reg2
    STP   \reg1, \reg2, [SP,#-16]!
ENDM

MACRO pop_reg reg1, reg2
    LDP   \reg1, \reg2, [SP], #16
ENDM

MACRO push_all
    SUB  SP, SP, #32
    push_reg X28, X29
    push_reg X26, X27
    push_reg X24, X25
    push_reg X22, X23
    push_reg X20, X21
    push_reg X18, X19
    push_reg X16, X17
    push_reg X14, X15
    push_reg X12, X13
    push_reg X10, X11
    push_reg X8, X9
    push_reg X6, X7
    push_reg X4, X5
    push_reg X2, X3
    push_reg X0, X1
    MOV  X0, X0  //STANDBY
    MRS  X1, ELR_ELx
    MRS  X2, SPSR_ELx
    STP  LR, X0, [SP, #0xf0]
    STP  X1, X2, [SP, #0xf0 + 16]
ENDM

MACRO pop_all
    LDP  LR, X0, [SP, #0xf0]
    LDP  X1, X2, [SP, #0xf0 + 16]
    MOV  SP, X0
    MSR  ELR_ELx, X1
    MSR  SPSR_ELx, X2
    pop_reg X0, X1
    pop_reg X2, X3
    pop_reg X4, X5
    pop_reg X6, X7
    pop_reg X8, X9
    pop_reg X10, X11
    pop_reg X12, X13
    pop_reg X14, X15
    pop_reg X16, X17
    pop_reg X18, X19
    pop_reg X20, X21
    pop_reg X22, X23
    pop_reg X24, X25
    pop_reg X26, X27
    pop_reg X28, X29
    ADD SP, SP, #32
ENDM

    .global arm64_exception_handlers
    .type   arm64_exception_handlers,function
arm64_exception_handlers:
    .type   arm64_exc_cur_el_SP0,function
arm64_exc_cur_el_SP0:
    MRS X0, CurrentEL
    BL  OsCurrentELGet
    B   .

    ORG 0x080
    .type arm64_irq_cur_el_SP0,function
arm64_irq_cur_el_SP0:
    MRS X0, CurrentEL
    BL  OsCurrentELGet
    B   .

    ORG 0x100
    .type arm64_fiq_cur_el_SP0,function
arm64_fiq_cur_el_SP0:
    MRS X0, CurrentEL
    BL  OsCurrentELGet
    B   .

    ORG 0x180
    .type arm64_err_exc_cur_el_SP0,function
arm64_err_exc_cur_el_SP0:
    MRS X0, CurrentEL
    BL  OsCurrentELGet
    B   .

    ORG 0x200
    .type arm64_sync_exc_cur_el_SPx,function
arm64_sync_exc_cur_el_SPx:
    push_all
    MSR   DAIFSET, #3
    MRS   X0, CurrentEL
    BL    OsCurrentELGet
    MOV   X0, SP
    BL    OsExceptSyncExcHdl
    B     arm64_exc_pop_all

    ORG 0x280
    .type arm64_irq_cur_el_SPx,function
arm64_irq_cur_el_SPx:
    B _osExceptIrqHdl

    ORG 0x300
    .type arm64_fiq_cur_el_SPx,function
arm64_fiq_cur_el_SPx:
    MRS X0, CurrentEL
    BL  OsCurrentELGet
    B   .

    ORG 0x380
    .type arm64_err_exc_cur_el_SPx,function
arm64_err_exc_cur_el_SPx:
    MRS X0, CurrentEL
    BL  OsCurrentELGet
    B   .

    ORG 0x400
    .type arm64_sync_exc_lower_el_64,function
arm64_sync_exc_lower_el_64:
    MRS X0, CurrentEL
    BL  OsCurrentELGet
    B   .

    ORG 0x480
    .type arm64_irq_lower_el_64,function
arm64_irq_lower_el_64:
    MRS X0, CurrentEL
    BL  OsCurrentELGet
    B   .

    ORG 0x500
    .type arm64_fiq_lower_el_64,function
arm64_fiq_lower_el_64:
    MRS X0, CurrentEL
    BL  OsCurrentELGet
    B   .

    ORG 0x580
    .type arm64_err_exc_lower_el_64,function
arm64_err_exc_lower_el_64:
    MRS X0, CurrentEL
    BL  OsCurrentELGet
    B   .

    ORG 0x600
    .type arm64_sync_exc_lower_el_32,function
arm64_sync_exc_lower_el_32:
    MRS X0, CurrentEL
    BL  OsCurrentELGet
    B   .

    ORG 0x680
    .type arm64_irq_lower_el_32,function
arm64_irq_lower_el_32:
    MRS X0, CurrentEL
    BL  OsCurrentELGet
    B   .

    ORG 0x700
    .type arm64_fiq_lower_el_32,function
arm64_fiq_lower_el_32:
    MRS X0, CurrentEL
    BL  OsCurrentELGet
    B   .

    ORG 0x780
    .type arm64_err_exc_lower_el_32,function
arm64_err_exc_lower_el_32:
    MRS X0, CurrentEL
    BL  OsCurrentELGet
    B   .

    .type arm64_exc_push_all,function
arm64_exc_push_all:
    push_all
    RET

    .type arm64_exc_pop_all,function
arm64_exc_pop_all:
    pop_all
    ERET
