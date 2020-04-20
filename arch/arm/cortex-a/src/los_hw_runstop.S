/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Aarch32 Hw Runstop handle
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
/* ----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 * --------------------------------------------------------------------------- */

#include "arch_config.h"

    .equ MPIDR_CPUID_MASK, 0xffU

    .extern  g_saveAR
    .extern  g_saveSRContext

    .global  OsSRSaveRegister
    .global  OsSRRestoreRegister

    .fpu vfpv4
    @.fpu neon
    .arch armv7a

    .text

OsSRSaveRegister:
    PUSH    {R2}
    LDR     R2, =g_saveAR
    STR     R0, [R2]
    STR     R1, [R2, #4]
    POP     {R2}

    MRC     P15, 0, R0, c0, c0, 5
    AND     R0, R0, #MPIDR_CPUID_MASK
    MOV     R1, #72 @This number is the total number of bytes in the task context register(R0~R15, SPSR, CPSR).
    MUL     R1, R1, R0

    LDR     R0, =g_saveSRContext
    ADD     R0, R0, R1
    ADD     R0, R0, #72

    MOV     R1, SP
    STMFD   R0!, {R1}

    MRS     R1,  SPSR
    STMFD   R0!, {R1}

    MOV     R1,  LR
    STMFD   R0!, {R1} @PC
    STMFD   R0!, {R1} @LR

    STMFD   R0!, {R12}

    MOV     R12,  R0

    LDR     R0, =g_saveAR
    LDR     R0, [R0]
    LDR     R1, =g_saveAR
    LDR     R1, [R1, #4]

    STMFD   R12!, {R0-R3}
    STMFD   R12!, {R4-R11}

    MRS     R0,  CPSR
    STMFD   R12!,  {R0}

    BX      LR

OsSRRestoreRegister:
    MRC     P15, 0, R0, c0, c0, 5
    AND     R0, R0, #MPIDR_CPUID_MASK
    MOV     R1, #72 @This number is the total number of bytes in the task context register(R0~R15, SPSR, CPSR).
    MUL     R1, R1, R0

    LDR     R12, =g_saveSRContext
    ADD     R12, R12, R1

    LDMFD   R12!, {R0}
    MSR     CPSR_cxsf, R0

    LDMFD   R12!, {R4-R11}
    LDMFD   R12!, {R0-R3}

    PUSH    {R2}
    LDR     R2, =g_saveAR
    STR     R0, [R2]
    STR     R1, [R2, #4]
    POP     {R2}

    MOV     R0, R12
    LDMFD   R0!, {R12}
    LDMFD   R0!, {R1} @LR
    LDMFD   R0!, {R1} @PC

    MOV     LR, R1

    LDMFD   R0!, {R1}
    MSR     SPSR_cxsf, R1

    LDMFD   R0!, {R1}
    MOV     SP, R1

    LDR     R0, =g_saveAR
    LDR     R0, [R0]
    LDR     R1, =g_saveAR
    LDR     R1, [R1, #4]

    BX      LR

    .end
