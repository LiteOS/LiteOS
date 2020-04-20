/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: ARMv7 Hw Exc Implementation
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
/* ----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 * --------------------------------------------------------------------------- */
#include "arch_config.h"

    .extern   g_losTask
    .extern   g_intCount
    .extern   g_curNestCount
    .extern   OsExcHandleEntry
    .extern   __svc_stack_top
    .extern   __exc_stack_top
    .extern   __stack_chk_guard
    .extern   OsRandomStackGuard
#ifdef LOSCFG_GDB
    .extern   OsUndefIncExcHandleEntry
#if __LINUX_ARM_ARCH__ >= 7
    .extern   OsPrefetchAbortExcHandleEntry
    .extern   OsDataAbortExcHandleEntry
#endif
#endif

    .global   _osExceptFiqHdl
    .global   _osExceptAddrAbortHdl
    .global   _osExceptDataAbortHdl
    .global   _osExceptPrefetchAbortHdl
    .global   _osExceptSwiHdl
    .global   _osExceptUndefInstrHdl
    .global   __stack_chk_guard_setup

    .equ MPIDR_CPUID_MASK, 0xffU

    .fpu vfpv4

/* param0 is stack bottom, param1 is stack size, R4 hold cpu id */
.macro EXC_SP_SET param0, param1
    LDR    R6, =\param0
    MOV    R7, \param1
    MUL    R7, R7, R4
    SUB    R6, R6, R7
    MOV    SP, R6
.endm

#ifdef LOSCFG_GDB
.macro GDB_HANDLE fun
    SUB     SP, SP, #12

    STMFD   SP!, {R0-R12}
    MRS     R1, SPSR
    STMFD   SP!, {R1}  @save spsr

    ADD     R0, SP, #14 * 4
    MOV     R3, LR  @save pc

    MRS     R1, CPSR
    MRS     R2, SPSR
    MOV     R4, SP

    ORR     R2, R2, #(CPSR_INT_DISABLE)
    MSR     CPSR_c, R2

    STR     SP, [R0]  @SP
    STR     LR, [R0, #4]  @LR
    STR     R3, [R0, #8]  @PC

    ORR     R1, R1, #(CPSR_INT_DISABLE)
    BIC     R1, R1, #OS_PSR_THUMB
    MSR     CPSR_c, R1
    MOV     R0, R4

    BL \fun

    ADD     SP, SP, #4
    LDMFD   SP!, {R0-R12}

    MOV     R0, SP
    ADD     SP, SP, #8

    LDR     R1, [R0, #8]  @get pc
    STMFD   SP!, {R1}

    AND     R1, R1, #0x03
    CMP     R1, #0
    BEQ     1f
    LDR     R1, [R0, #-14 * 4]
    ORR     R1, R1, #OS_PSR_THUMB
    B       2f
1:
    LDR     R1, [R0, #-14 * 4]

2:
    MSR     SPSR, R1

    LDR     R1, [R0, #-12 * 4]  @get R1
    STMFD   SP!, {R1}
    LDR     R1, [R0,#-13 * 4]  @get R0
    STMFD   SP!, {R1}

    LDMFD   SP!, {R0-R1, PC}^
.endm
#endif

@ Description: Stack-Protector Init
__stack_chk_guard_setup:
    PUSH    {FP, LR}
    BL      OsRandomStackGuard
    LDR     R1, =__stack_chk_guard
    MOV     R3, R0
    ORR     R2, R3, #0X80000000
    STR     R2, [R1]
    POP     {FP, PC}

@ Description: Undefined instruction exception handler
_osExceptUndefInstrHdl:
#ifdef LOSCFG_GDB
    GDB_HANDLE OsUndefIncExcHandleEntry
#else
                                                              @ LR offset to return from this exception:  0.
    STMFD   SP, {R0-R7}                                       @ Push working registers, but don`t change SP.

    MOV     R0, #OS_EXCEPT_UNDEF_INSTR                        @ Set exception ID to OS_EXCEPT_UNDEF_INSTR.

    B       _osExceptDispatch                                 @ Branch to global exception handler.

#endif

@ Description: Software interrupt exception handler
_osExceptSwiHdl:
    STMFD   SP!, {LR}                                         @ Store PC
    STMFD   SP!, {LR}
    STMFD   SP!, {SP}
    STMFD   SP!, {R0-R12}                                     @ Store SP,LR,R0-R12

    MRS     R1, SPSR                                          @ Save exception`s CPSR.
    STMFD   SP!, {R1}                                         @ Push task`s CPSR (i.e. exception SPSR).

    MOV     R0, #OS_EXCEPT_SWI                                @ Set exception ID to OS_EXCEPT_SWI.
    MOV     R5, SP

    B       _osExceptionSwi                                   @ Branch to global exception handler.

@ Description: Prefectch abort exception handler
_osExceptPrefetchAbortHdl:
#ifdef LOSCFG_GDB
#if __LINUX_ARM_ARCH__ >= 7
    GDB_HANDLE OsPrefetchAbortExcHandleEntry
#endif
#else
    SUB     LR, LR, #4                                        @ LR offset to return from this exception: -4.
    STMFD   SP, {R0-R7}                                       @ Push working registers, but don`t change SP.

    MOV     R0, #OS_EXCEPT_PREFETCH_ABORT                     @ Set exception ID to OS_EXCEPT_PREFETCH_ABORT.

    B       _osExceptDispatch                                 @ Branch to global exception handler.
#endif

@ Description: Data abort exception handler
_osExceptDataAbortHdl:
#ifdef LOSCFG_GDB
#if __LINUX_ARM_ARCH__ >= 7
    GDB_HANDLE OsDataAbortExcHandleEntry
#endif
#else
    SUB     LR, LR, #8                                        @ LR offset to return from this exception: -8.
    STMFD   SP, {R0-R7}                                       @ Push working registers, but don`t change SP.

    MOV     R0, #OS_EXCEPT_DATA_ABORT                         @ Set exception ID to OS_EXCEPT_DATA_ABORT.

    B       _osExceptDispatch                                 @ Branch to global exception handler.
#endif

@ Description: Address abort exception handler
_osExceptAddrAbortHdl:
    SUB     LR, LR, #8                                        @ LR offset to return from this exception: -8.
    STMFD   SP, {R0-R7}                                       @ Push working registers, but don`t change SP.

    MOV     R0, #OS_EXCEPT_ADDR_ABORT                         @ Set exception ID to OS_EXCEPT_ADDR_ABORT.

    B       _osExceptDispatch                                 @ Branch to global exception handler.

@ Description: Fast interrupt request exception handler
_osExceptFiqHdl:
    SUB     LR, LR, #4                                        @ LR offset to return from this exception: -4.
    STMFD   SP, {R0-R7}                                       @ Push working registers.

    MOV     R0, #OS_EXCEPT_FIQ                                @ Set exception ID to OS_EXCEPT_FIQ.

    B       _osExceptDispatch                                 @ Branch to global exception handler.

@ Description: Exception handler
@ Parameter  : R0     Exception Type
@ Regs Hold  : R3     Exception`s CPSR
_osExceptDispatch:
    MRS     R1, SPSR                                          @ Save CPSR before exception.
    MOV     R2, LR                                            @ Save PC before exception.
    SUB     R3, SP, #(8 * 4)                                  @ Save the start address of working registers.

    MSR     CPSR_c, #(CPSR_INT_DISABLE | CPSR_SVC_MODE)       @ Switch to SVC mode, and disable all interrupts
    MOV     R5, SP
    MRC     P15, 0, R4, C0, C0, 5
    AND     R4,  R4, #MPIDR_CPUID_MASK                        @ Get Current cpu id
    EXC_SP_SET __exc_stack_top, #OS_EXC_STACK_SIZE

    STMFD   SP!, {R2}                                         @ Push Exception PC
    STMFD   SP!, {LR}
    STMFD   SP!, {R5}                                         @ Push original SP,
    STMFD   SP!, {R8-R12}                                     @ Push original R12-R8,
    LDMFD   R3!, {R4-R11}                                     @ Move original R7-R0 from exception stack to original stack.
    STMFD   SP!, {R4-R11}
    STMFD   SP!, {R1}                                         @ Push task`s CPSR (i.e. exception SPSR).

_osExceptionSwi:
    MOV     R1, SP

    LDR     R2, =g_curNestCount                               @ if(g_curNestCount > 0) dump to _osExceptionGetSP
    LDR     R4, [R2]

    CMP     R4, #0
    BNE     _osExceptionGetSP

    MRC     P15, 0, R4, C0, C0, 5
    AND     R4,  R4, #MPIDR_CPUID_MASK                        @ Get Current cpu id
    LSL     R2,  R4, #2
    LDR     R3, =g_intCount                                   @ Judge the exception is occur in task stack or system stack
    ADD     R3, R3, R2
    LDR     R2, [R3]

    CMP     R2, #0                                            @ if (g_intCount[ArchCurrCpuid()] > 0)
    BNE     _osExceptionGetSP                                 @ can not switch svc stack

    EXC_SP_SET __svc_stack_top, #OS_EXC_SVC_STACK_SIZE        @ Switch to unified exception stack.
    ADD     R2, R2, #1
    STR     R2, [R3]

_osExceptionGetSP:
    LDR     R2, =OsExcHandleEntry                             @ OsExcHandleEntry(UINT32 excType, ExcContext * excBufAddr)

    MOV     LR, PC
    BX      R2

    .end
