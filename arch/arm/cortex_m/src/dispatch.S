/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Cortex-M Dispatch Implementation
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
/* ----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 * --------------------------------------------------------------------------- */

.syntax unified
.arch armv7e-m
.thumb

.equ OS_NVIC_INT_CTRL,           0xE000ED04
.equ OS_NVIC_SYSPRI2,            0xE000ED20
.equ OS_NVIC_PENDSV_PRI,         0xF0F00000
.equ OS_NVIC_PENDSVSET,          0x10000000
.equ OS_TASK_STATUS_RUNNING,     0x0010

    .section .text
    .thumb

    .type OsStartToRun, %function
    .global OsStartToRun
OsStartToRun:
    .fnstart
    .cantunwind
    ldr     r4, =OS_NVIC_SYSPRI2
    ldr     r5, =OS_NVIC_PENDSV_PRI
    str     r5, [r4]

    ldr     r1, =g_oldTask
    str     r0, [r1]

    ldr     r1, =g_runTask
    str     r0, [r1]

    mov     r1, #2
    msr     CONTROL, r1


    ldrh    r7, [r0 , #4]
    mov     r8,  #OS_TASK_STATUS_RUNNING
    orr     r7,  r7,  r8
    strh    r7,  [r0 , #4]

    ldr     r12, [r0]
    ADD     r12, r12, #100

    ldmfd   r12!, {R0-R7}
    add     r12, r12, #72
    msr     psp, r12
    vpush   {s0};
    vpop    {s0};

    mov     lr, r5
   ;msr     xPSR, R7

    cpsie   I
    bx      r6
    .fnend

    .type OsIntLock, %function
    .global OsIntLock
OsIntLock:
    .fnstart
    .cantunwind
    mrs     r0, PRIMASK
    cpsid   I
    bx      lr
    .fnend

    .type OsIntUnLock, %function
    .global OsIntUnLock
OsIntUnLock:
    .fnstart
    .cantunwind
    mrs     r0, PRIMASK
    cpsie   I
    bx      lr
    .fnend

    .type OsIntRestore, %function
    .global OsIntRestore
OsIntRestore:
    .fnstart
    .cantunwind
    msr     PRIMASK, r0
    bx      lr
    .fnend

    .type OsTaskSchedule, %function
    .global OsTaskSchedule
OsTaskSchedule:
    .fnstart
    .cantunwind
    ldr     r2, =OS_NVIC_INT_CTRL
    ldr     r3, =OS_NVIC_PENDSVSET
    str     r3, [r2]
    bx      lr
    .fnend

    .type osPendSV, %function
    .global osPendSV
osPendSV:
    .fnstart
    .cantunwind
    mrs     r12, PRIMASK
    cpsid   I


TaskSwitch:
    mrs     r0, psp

    stmfd   r0!, {r4-r12}
    vstmdb  r0!, {d8-d15}

    ldr     r5, =g_oldTask
    ldr     r1, [r5]
    str     r0, [r1]

    ldr     r0, =g_runTask
    ldr     r0, [r0]
    /* g_oldTask = g_runTask */
    str     r0, [r5]
    ldr     r1, [r0]

    vldmia  r1!, {d8-d15}
    ldmfd   r1!, {r4-r12}
    msr     psp,  r1

    msr     PRIMASK, r12
    bx      lr

    .fnend
