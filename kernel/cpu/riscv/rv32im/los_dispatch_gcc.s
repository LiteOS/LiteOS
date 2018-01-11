/*
;----------------------------------------------------------------------------
 ; Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
 ; All rights reserved.
 ; Redistribution and use in source and binary forms, with or without modification,
 ; are permitted provided that the following conditions are met:
 ; 1. Redistributions of source code must retain the above copyright notice, this list of
 ; conditions and the following disclaimer.
 ; 2. Redistributions in binary form must reproduce the above copyright notice, this list
 ; of conditions and the following disclaimer in the documentation and/or other materials
 ; provided with the distribution.
 ; 3. Neither the name of the copyright holder nor the names of its contributors may be used
 ; to endorse or promote products derived from this software without specific prior written
 ; permission.
 ; THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ; "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 ; THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 ; PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 ; CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 ; EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 ; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 ; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 ; WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 ; OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 ; ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ;---------------------------------------------------------------------------
;----------------------------------------------------------------------------
 ; Notice of Export Control Law
 ; ===============================================
 ; Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 ; include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 ; Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 ; applicable export control laws and regulations.
 ;---------------------------------------------------------------------------
*/

#include "../../../../platform/M2S150_RV32/riscv_hal/encoding.h"

    .globl  LOS_IntLock
    .globl  LOS_IntUnLock
    .globl  LOS_IntRestore
    .globl  LOS_StartToRun
    .globl  osTaskSchedule
    .globl  Software_IRQHandler
    .globl  LOS_IntNumGet
    .globl  osDisableIRQ
        
    .equ        OS_TASK_STATUS_RUNNING,     0x0010
    .equ        OS_TASK_STATUS_NOT_RUNNING, 0xEF

    #define     PRCI_BASE_ADDR              0x44000000
    #define     PLIC_CLAIM_REG              0x40200004

.section .text

LOS_StartToRun:
//disable interrupts
    li      t0, 0x08
    csrrc   zero, mstatus, t0

//indicate that sheduler is ON by setting g_bTaskScheduled=1
    la      t0, g_bTaskScheduled
    li      t1, 0x1
    sw      t1, 0x0(t0)

//no user mode. we are always in machine mode.

//copy pstNewTask into pstRunTask
    la      t0, g_stLosTask
    lw      t1, 4(t0)
    sw      t1, 0(t0)

// set the task running bit=1
    la      t0, g_stLosTask
    lw      t1, (t0)
    lh      t2, 0x4(t1)
    ori     t2, t2,  OS_TASK_STATUS_RUNNING
    sh      t2, 0x4(t1)

//retrieve stack pointer
    lw      sp, 0x0(t1)

//retrieve the location where to jump
    lw      t0, 31 * 4(sp)
    csrw    mepc, t0

//retrieve the registers
    lw      ra, 0 * 4(sp)

    lw      t0, 4 * 4(sp)
    lw      t1, 5 * 4(sp)
    lw      t2, 6 * 4(sp)
    lw      s0, 7 * 4(sp)
    lw      s1, 8 * 4(sp)
    lw      a0, 9 * 4(sp)
    lw      a1, 10 * 4(sp)
    lw      a2, 11 * 4(sp)
    lw      a3, 12 * 4(sp)
    lw      a4, 13 * 4(sp)
    lw      a5, 14 * 4(sp)
    lw      a6, 15 * 4(sp)
    lw      a7, 16 * 4(sp)
    lw      s2, 17 * 4(sp)
    lw      s3, 18 * 4(sp)
    lw      s4, 19 * 4(sp)
    lw      s5, 20 * 4(sp)
    lw      s6, 21 * 4(sp)
    lw      s7, 22 * 4(sp)
    lw      s8, 23 * 4(sp)
    lw      s9, 24 * 4(sp)
    lw      s10, 25 * 4(sp)
    lw      s11, 26 * 4(sp)
    lw      t3, 27 * 4(sp)
    lw      t4, 28 * 4(sp)
    lw      t5, 29 * 4(sp)
    lw      t6, 30 * 4(sp)

//compensate for the stack pointer
    addi    sp, sp, 32*4

//use register t6 to jump to OS start
    csrr    t6, mepc

//enable global interrupts
    li      t0, 0x08
    csrrs   zero, mstatus, t0

//jump to osTaskEntry function.
    jalr    x0, t6, 0

LOS_IntNumGet:
    li  t0, PLIC_CLAIM_REG
    lw  t0, 0x0(t0)
    ret

osDisableIRQ:
    li      t0, 0x88              //mpie|mie
    csrrc   zero, mstatus, t0
    ret

LOS_IntLock:
    csrr    a0, mstatus           //return value
    li      t0, 0x08              //mie
    csrrc   zero, mstatus, t0
    ret

LOS_IntUnLock:
    csrr    a0, mstatus           //return value
    li      t0, 0x08              //mie
    csrrs   zero, mstatus, t0
    ret

LOS_IntRestore:
    csrw mstatus, a0
    ret

osTaskSchedule:
//MIP_MSIE -- enable software interrupt bit
    li      t0, MIP_MSIP
    csrrs   zero, mie, t0

//this will cause the software interrupt
    li      t0, PRCI_BASE_ADDR
    li      t1, 0x1
    sw      t1, 0x0(t0)
    ret

Software_IRQHandler:
//disable interrupts globaly
    li      t0, 0x08                                //mie
    csrrc   zero, mstatus, t0

    la     t0, g_pfnTskSwitchHook                   //this is not defined. We are not jumping on this function as of now.
    lw     t1, 0x0(t0)
    beqz   t1, TaskSwitch                           //Branch if = zero

TaskSwitch:
//stack pointer was passed by entry.s in register a2. store it in pstRunTask structure
    la      t0, g_stLosTask
    lw      t1, 0(t0)
    sw      a2, 0(t1)

//Clear the task running bit of pstRunTask.
    la      t0, g_stLosTask
    lw      t1, (t0)
    lb      t2, 0x4(t1)
    andi    t2, t2, OS_TASK_STATUS_NOT_RUNNING
    sb      t2, 0x4(t1)

//copy pstNewTask into pstRunTask
    la      t0, g_stLosTask
    lw      t1, 0x4(t0)
    sw      t1, 0x0(t0)

//set the task running bit=1
    lh      t2, 0x4(t1)
    ori     t2, t2, OS_TASK_STATUS_RUNNING
    sh      t2, 0x4(t1)

//retireve stack pointer
    lw      sp, (t1)

//retrieve the address at which exception happened
    lw      t0, 31 * 4(sp)
    csrw    mepc, t0

//retrieve the registers
    lw      ra, 0 * 4(sp)

    lw      t0, 4 * 4(sp)
    lw      t1, 5 * 4(sp)
    lw      t2, 6 * 4(sp)
    lw      s0, 7 * 4(sp)
    lw      s1, 8 * 4(sp)
    lw      a0, 9 * 4(sp)
    lw      a1, 10 * 4(sp)
    lw      a2, 11 * 4(sp)
    lw      a3, 12 * 4(sp)
    lw      a4, 13 * 4(sp)
    lw      a5, 14 * 4(sp)
    lw      a6, 15 * 4(sp)
    lw      a7, 16 * 4(sp)
    lw      s2, 17 * 4(sp)
    lw      s3, 18 * 4(sp)
    lw      s4, 19 * 4(sp)
    lw      s5, 20 * 4(sp)
    lw      s6, 21 * 4(sp)
    lw      s7, 22 * 4(sp)
    lw      s8, 23 * 4(sp)
    lw      s9, 24 * 4(sp)
    lw      s10, 25 * 4(sp)
    lw      s11, 26 * 4(sp)
    lw      t3, 27 * 4(sp)
    lw      t4, 28 * 4(sp)
    lw      t5, 29 * 4(sp)
    lw      t6, 30 * 4(sp)

    addi    sp, sp, 4 * 32

    mret
