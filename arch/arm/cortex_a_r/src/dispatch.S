/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: ARMv7 Dispatch Implementation
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

#include "arch/regs.h"
#include "arch/asm.h"
#include "arch/exception.h"

.global   OsStartToRun
.global   OsTaskSchedule
.global   OsIrqHandler

.equ OS_TASK_STATUS_RUNNING,       0x0010U

.fpu vfpv4

/* macros to align and unalign the stack on 8 byte boundary for ABI compliance */
.macro STACK_ALIGN, reg
    MOV     \reg, sp
    TST     SP, #4
    SUBEQ   SP, #4
    PUSH    { \reg }
.endm

.macro STACK_RESTORE, reg
    POP     { \reg }
    MOV     sp, \reg
.endm

/* macros to save and restore fpu regs */
.macro PUSH_FPU_REGS reg1
#if !defined(LOSCFG_ARCH_FPU_DISABLE)
    VMRS    \reg1, FPEXC
    PUSH    {\reg1}
    VMRS    \reg1, FPSCR
    PUSH    {\reg1}
#if defined(LOSCFG_ARCH_FPU_VFP_D32)
    VPUSH   {D16-D31}
#endif
    VPUSH   {D0-D15}
#endif
.endm

.macro POP_FPU_REGS reg1
#if !defined(LOSCFG_ARCH_FPU_DISABLE)
    VPOP    {D0-D15}
#if defined(LOSCFG_ARCH_FPU_VFP_D32)
    VPOP    {D16-D31}
#endif
    POP     {\reg1}
    VMSR    FPSCR, \reg1
    POP     {\reg1}
    VMSR    FPEXC, \reg1
#endif
.endm

/*
 * R0: new task 
 */
OsStartToRun:
    MSR     CPSR_c, #(CPSR_INT_DISABLE | CPSR_SVC_MODE)

    LDRH    R1, [R0, #4]
    MOV     R1, #OS_TASK_STATUS_RUNNING
    STRH    R1, [R0, #4]

    /* R0 is new task, save it on tpidrprw */
    MCR     p15, 0, R0, c13, c0, 4
    ISB

    /* fpu */
    VPUSH   {S0}
    VPOP    {S0}
    VPUSH   {D0}
    VPOP    {D0}

    B       OsTaskContextLoad

/*
 * R0: new task
 * R1: run task
 */
OsTaskSchedule:
    MRS     R2, CPSR
    PUSH    {R2}
    PUSH    {LR}

    /* push r0-r12, lr */
    PUSH    {R0-R12, LR}

    /* save fpu registers */
    PUSH_FPU_REGS   R2

    /* store sp on running task */
    STR     SP, [R1]

OsTaskContextLoad:
    /* clear the flag of ldrex */
    CLREX

    /* switch to new task's sp */
    LDR     SP, [R0]

    /* restore fpu registers */
    POP_FPU_REGS    R2

    /* restore r0-r12, lr */
    POP     {R0-R12, LR}

    POP     {R1}
    POP     {R2}
    MSR     CPSR, R2
    BX      R1

OsIrqHandler:
    SUB     LR, LR, #4
#ifdef LOSCFG_KERNEL_PERF
    PUSH    {R0-R3, R12, LR}
    MOV     R0, LR
    MOV     R1, FP
    BL      OsPerfSetIrqRegs
    POP     {R0-R3, R12, LR}
#endif

    /* save spsr and lr(svc's pc) onto the svc stack */
    SRSDB   #0x13!

    /* disable irq, switch to svc mode */
    CPSID   i, #0x13

    /* push caller saved regs as trashed regs */
    PUSH    {R0-R3, R12, LR}

    /* 8 bytes stack align */
    STACK_ALIGN     R0

    /*
     * save fpu regs in case in case those been
     * altered in interrupt handlers.
     */
    PUSH_FPU_REGS   R0
#ifdef LOSCFG_IRQ_USE_STANDALONE_STACK
    PUSH    {R4}
    MOV     R4, SP
    EXC_SP_SET __svc_stack_top, OS_EXC_SVC_STACK_SIZE, R1, R2
#endif

    BLX     OsIntEntry

#ifdef LOSCFG_IRQ_USE_STANDALONE_STACK
    MOV     SP, R4
    POP     {R4}
#endif

    /* process pending signals */
    BL      OsTaskProcSignal

    /* check if needs to schedule */
    CMP     R0, #0
    BLNE    OsSchedPreempt

    /* restore fpu regs */
    POP_FPU_REGS    R0

    STACK_RESTORE   R0

OsIrqContextRestore:
    POP     {R0-R3, R12, LR}
    RFEIA   SP!
