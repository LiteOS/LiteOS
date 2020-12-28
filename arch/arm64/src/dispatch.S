/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Aarch64 Dispatch Implementation
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
#include "arch/exception.h"

#define FUNCTION(x) .global x; .text; x:

.extern HalIrqClear

.global _osExceptIrqHdl
.global OsStartToRun
.global OsTaskSchedule

.equ TASK_STATUS_RUNNING, 0x0010

.arch armv8-a

#define fp  x29
#define lr  x30

/*
 * Register usage:
 *  x0-x3   Arguments
 *  x9-x15  Scratch
 *  x19-x28 Globals
 */
.macro PUSH reg1, reg2
    stp     \reg1, \reg2, [sp, #-16]!
.endm

.macro POP reg1, reg2
    ldp     \reg1, \reg2, [sp], #16
.endm

.macro FPU_PUSH ra, rb
    stp     \ra, \rb, [sp, #-32]!
.endm

.macro FPU_POP ra, rb
    ldp     \ra, \rb, [sp], #32
.endm

.macro PUSH_REGS
    PUSH    x28, x29
    PUSH    x26, x27
    PUSH    x24, x25
    PUSH    x22, x23
    PUSH    x20, x21
    PUSH    x18, x19
    PUSH    x16, x17
    PUSH    x14, x15
    PUSH    x12, x13
    PUSH    x10, x11
    PUSH    x8,  x9
    PUSH    x6,  x7
    PUSH    x4,  x5
    PUSH    x2,  x3
    PUSH    x0,  x1
.endm

.macro POP_REGS
    POP     x0,  x1
    POP     x2,  x3
    POP     x4,  x5
    POP     x6,  x7
    POP     x8,  x9
    POP     x10, x11
    POP     x12, x13
    POP     x14, x15
    POP     x16, x17
    POP     x18, x19
    POP     x20, x21
    POP     x22, x23
    POP     x24, x25
    POP     x26, x27
    POP     x28, x29
.endm

.macro PUSH_AARCH64
    sub     sp, sp, #32
    PUSH_REGS
    mrs     x2, daif
    mrs     x3, nzcv
    stp     lr, lr, [sp, #(15 * 16)]
    stp     x2, x3, [sp, #(15 * 16 + 16)]
.endm

.macro POP_AARCH64
    ldp     x1, x2, [sp, #16 * 16]
    orr     x1, x1, x2
    orr     x1, x1, #1
    orr     x1, x1, #RUNLVL
    msr     SPSR_ELx, x1
    POP_REGS
    ldp     lr, x1, [sp], #16
    add     sp, sp, #16
.endm

.macro IRQ_PUSH_REGS
    sub     sp,  sp, #32
    PUSH    x18, x19
    PUSH    x16, x17
    PUSH    x14, x15
    PUSH    x12, x13
    PUSH    x10, x11
    PUSH    x8,  x9
    PUSH    x6,  x7
    PUSH    x4,  x5
    PUSH    x2,  x3
    PUSH    x0,  x1
    mrs     x3,  SPSR_ELx
    and     x0,  x3, #0xf0000000 /* CPSR.NZCV */
    and     x1,  x3, #0x3c0      /* CPSR.DAIF */
    mrs     x2,  ELR_ELx
    stp     x0,  x1, [sp, #(10 * 16)]
    stp     lr,  x2, [sp, #(10 * 16 + 16)]
.endm

.macro IRQ_POP_REGS
    ldp     lr,  x2, [sp, #(10 * 16 + 16)]
    ldp     x0,  x1, [sp, #(10 * 16)]
    msr     ELR_ELx, x2
    orr     x0,  x0, x1
    orr     x0,  x0, #1
    orr     x0,  x0, #RUNLVL
    msr     SPSR_ELx, x0
    POP     x0,  x1
    POP     x2,  x3
    POP     x4,  x5
    POP     x6,  x7
    POP     x8,  x9
    POP     x10, x11
    POP     x12, x13
    POP     x14, x15
    POP     x16, x17
    POP     x18, x19
    add     sp,  sp, #32
.endm

/* FPU/SIMD Registers */
.macro FPU_PUSH_REGS
    mrs         x2,  fpcr
    mrs         x3,  fpsr
    stp         x2,  x3, [sp, #-16]!
    FPU_PUSH    q30, q31
    FPU_PUSH    q28, q29
    FPU_PUSH    q26, q27
    FPU_PUSH    q24, q25
    FPU_PUSH    q22, q23
    FPU_PUSH    q20, q21
    FPU_PUSH    q18, q19
    FPU_PUSH    q16, q17
    FPU_PUSH    q14, q15
    FPU_PUSH    q12, q13
    FPU_PUSH    q10, q11
    FPU_PUSH    q8,  q9
    FPU_PUSH    q6,  q7
    FPU_PUSH    q4,  q5
    FPU_PUSH    q2,  q3
    FPU_PUSH    q0,  q1
.endm

.macro FPU_POP_REGS
    FPU_POP     q0,  q1
    FPU_POP     q2,  q3
    FPU_POP     q4,  q5
    FPU_POP     q6,  q7
    FPU_POP     q8,  q9
    FPU_POP     q10, q11
    FPU_POP     q12, q13
    FPU_POP     q14, q15
    FPU_POP     q16, q17
    FPU_POP     q18, q19
    FPU_POP     q20, q21
    FPU_POP     q22, q23
    FPU_POP     q24, q25
    FPU_POP     q26, q27
    FPU_POP     q28, q29
    FPU_POP     q30, q31
    ldp         x2,  x3, [sp], #16
    msr         fpsr, x3
    msr         fpcr, x2
.endm

/*
 * x0: pstNewTask
 */
OsStartToRun:
    msr     daifset, #3

    ldr     x2, [x0, #8]
    mov     x3, #TASK_STATUS_RUNNING
    orr     x2, x2, x3
    str     x2, [x0, #8]

    /* x0 is new task, save it on tpidr_elx */
    msr     TPIDR_ELx, X0
    isb

    b       OsTaskContextLoad

/*
 * x0: pstNewTask
 * x1: pstRunTask
 */
OsTaskSchedule:
    PUSH_AARCH64

#ifdef LOSCFG_AARCH64_FPU
    FPU_PUSH_REGS
#endif

    msr     daifset, #3

    /* store sp on running task */
    mov     x2, sp
    str     x2, [x1]

OsTaskContextLoad:
    /* clear the flag of ldrex */
    clrex

    /* switch to new task's sp */
    ldr     x2, [x0]
    mov     sp, x2

osThrdContextLoad:
#ifdef LOSCFG_AARCH64_FPU
    FPU_POP_REGS
#endif

    POP_AARCH64

    /* x0 is parameter, use scratch x1 */
    ret     x1

/* Description: Interrupt request exception handler */
_osExceptIrqHdl:
    msr     daifset, #3

#ifdef LOSCFG_KERNEL_PERF
    PUSH_REGS
    PUSH    lr, fp
    mrs     x0, ELR_EL1
    MOV     x1, fp
    BL      OsPerfSetIrqRegs
    POP     lr, fp
    POP_REGS
#endif

    /* save irq stack */
    IRQ_PUSH_REGS

#ifdef LOSCFG_IRQ_USE_STANDALONE_STACK
#ifdef LOSCFG_KERNEL_SMP
    /* read core id from mpidr_el1 */
    mrs     x1, mpidr_el1
    and     w2, w1, #0xff
    ubfx    x0, x1, #8, #8
    tst     x1, #0x1000000
    csel    w0, w0, w2, ne
#else
    mov     x0, #0
#endif

    /* check nesting count, if 0, switch sp to irq stack */
    ldr     x3, =g_intCount
    ldr     x4, [x3, x0, lsl #3]
    cbnz    w4, ALREADY_IRQ_SP

    mrs     x6, TPIDR_ELx
    mov     x9, sp
    str     x9, [x6]

    ldr     x9, =__irq_stack_top
    mov     x7, #OS_EXC_IRQ_STACK_SIZE
    mul     x7, x7, x0 /* w0 is core id */
    sub     x9, x9, x7
    mov     sp, x9

ALREADY_IRQ_SP:
#endif

    /* call interrupt entry */
    bl      OsIntEntry

#ifdef LOSCFG_ARCH_INTERRUPT_PREEMPTION
    bl      OsIrqNestingCntGet
    cbnz    w0, exit
#endif

#ifdef LOSCFG_IRQ_USE_STANDALONE_STACK
    mrs     x6, TPIDR_ELx
    ldr     x9, [x6] /* restore task sp */
    mov     sp, x9
#endif

    /* process pending signals */
    bl      OsTaskProcSignal

    /* check if needs to schedule */
    cbz     w0, exit
    bl      OsSchedPreempt

exit:
    IRQ_POP_REGS
    eret

    .end
