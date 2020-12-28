/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Aarch64 Hw Runstop Implementation
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
 * -------------------------------------------------------------------------- */

#include "arch/regs.h"

    .extern g_saveAR
    .extern g_saveSRContext

    .global OsSRSaveRegister
    .global OsSRRestoreRegister

    .arch armv8-a
    .text

/*
 * what we save:
 * SP
 * LR
 * DAIF
 * NZCV
 * X0 - X29 [30]
 */
OsSRSaveRegister:
    STP X2, X3, [SP,#-16]!
    LDR X3, =g_saveAR
    STR X0, [X3]
    STR X1, [X3, #8]
    STR X2, [X3, #16]
    LDP X2, X3, [SP], #16

    LDR X0, =g_saveSRContext
    ADD X0, X0, #272

    /* PUSH {SP, LR} */
    MOV X1, SP
    STP X1, X30, [X0,#-16]!

    /* PUSH {DAIF, NZCV} */
    MRS X1, DAIF
    MRS X2, NZCV
    STP X1, X2, [X0,#-16]!

    STP X28, X29, [X0,#-16]!
    STP X26, X27, [X0,#-16]!
    STP X24, X25, [X0,#-16]!
    STP X22, X23, [X0,#-16]!
    STP X20, X21, [X0,#-16]!
    STP X18, X19, [X0,#-16]!
    STP X16, X17, [X0,#-16]!
    STP X14, X15, [X0,#-16]!
    STP X12, X13, [X0,#-16]!
    STP X10, X11, [X0,#-16]!
    STP X8, X9, [X0,#-16]!
    STP X6, X7, [X0,#-16]!
    STP X4, X5, [X0,#-16]!

    MOV X29, X0

    LDR X0, =g_saveAR
    LDR X0, [X0]
    LDR X1, =g_saveAR
    LDR X1, [X1, #8]
    LDR X2, =g_saveAR
    LDR X2, [X2, #16]

    STP X2, X3, [X29,#-16]!
    STP X0, X1, [X29,#-16]!

    RET

OsSRRestoreRegister:
    CLREX

    LDR X29, =g_saveSRContext

    LDP X0, X1, [X29], #16
    LDP X2, X3, [X29], #16
    LDP X4, X5, [X29], #16
    LDP X6, X7, [X29], #16
    LDP X8, X9, [X29], #16
    LDP X10, X11, [X29], #16
    LDP X12, X13, [X29], #16
    LDP X14, X15, [X29], #16
    LDP X16, X17, [X29], #16
    LDP X18, X19, [X29], #16
    LDP X20, X21, [X29], #16
    LDP X22, X23, [X29], #16
    LDP X24, X25, [X29], #16
    LDP X26, X27, [X29], #16

    STP X2, X3, [SP,#-16]!
    LDR X3, =g_saveAR
    STR X0, [X3]
    STR X1, [X3, #8]
    STR X2, [X3, #16]
    LDP X2, X3, [SP], #16

    MOV X0, X29

    LDP X28, X29, [X0], #16

    /* POP {DAIF, NZCV} */
    LDP X1, X2, [X0], #16
    MRS X1, DAIF
    MRS X2, NZCV
    ORR X1, X1, X2
    ORR X1, X1, #1
    ORR X1, X1, #RUNLVL
    MSR SPSR_ELx, X1

    /* POP {SP, LR} */
    LDP X1, X30, [X0], #16
    MOV SP, X1
    MSR ELR_ELx, X30

    LDR X0, =g_saveAR
    LDR X0, [X0]
    LDR X1, =g_saveAR
    LDR X1, [X1, #8]
    LDR X2, =g_saveAR
    LDR X2, [X2, #16]

    RET

    .end
