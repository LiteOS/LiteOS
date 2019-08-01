/*----------------------------------------------------------------------------
 * Copyright (c) <2019>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
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
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

#include <los_config.h>
#include <los_typedef.h>

#define OS_NVIC_INT_CTRL            0xE000ED04  // Interrupt Control and State Register.
#define OS_NVIC_PENDSVSET           0x10000000  // Value to trigger PendSV exception.

#define OS_NVIC_SYSPRI2             0xE000ED20  // System Handler Priority Register 2.
#define OS_NVIC_PENDSV_SYSTICK_PRI  0xC0C00000  // SysTick + PendSV priority level (lowest).

#define OS_TASK_STATUS_RUNNING      0x0010      // Task Status Flag (RUNNING).

/*******************************************************************************
 Function:
        VOID LOS_StartToRun(VOID);
 Description:
        Start the first task, which is the highest priority task in the priority queue.
        Other tasks are started by task scheduling.
*******************************************************************************/
__asm VOID LOS_StartToRun(VOID)
{
    PRESERVE8

    IMPORT  g_stLosTask
    IMPORT  g_bTaskScheduled

    CPSID   I

    ;/**
    ; * Set PendSV and SysTick prority to the lowest.
    ; * read ---> modify ---> write-back.
    ; */
    LDR     R0, =OS_NVIC_SYSPRI2
    LDR     R1, =OS_NVIC_PENDSV_SYSTICK_PRI
    LDR     R2, [R0]
    ORRS    R1, R1, R2
    STR     R1, [R0]

    ;/**
    ; * Set g_bTaskScheduled = 1.
    ; */
    LDR     R0, =g_bTaskScheduled
    MOVS    R1, #1
    STR     R1, [R0]

    ;/**
    ; * Set g_stLosTask.pstRunTask = g_stLosTask.pstNewTask.
    ; */
    LDR     R0, =g_stLosTask
    LDR     R1, [R0, #4]
    STR     R1, [R0]

    ;/**
    ; * Set g_stLosTask.pstRunTask->usTaskStatus |= OS_TASK_STATUS_RUNNING.
    ; */
    LDR     R1, [R0]
    LDRH    R2, [R1, #4]
    MOVS    R3, #OS_TASK_STATUS_RUNNING
    ORRS    R2, R2, R3
    STRH    R2, [R1, #4]

    ;/**
    ; * Restore the default stack frame(R0-R3,R12,LR,PC,xPSR) of g_stLosTask.pstRunTask to R0-R7.
    ; * Return by setting the CONTROL register.
    ; *
    ; * The initial stack of the current running task is as follows:
    ; *
    ; *                 POP: Restore the context of the current running task ===>|
    ; *                                                             High addr--->|
    ; *                                                   Bottom of the stack--->|
    ; * ----------+-----------------------+--------------------------------------+
    ; *           |   R4-R11,   PRIMASK   |   R0-R3,   R12,   LR,   PC,   xPSR   |
    ; * ----------+-----------------------+--------------------------------------+
    ; *           |<---Top of the stack, restored from g_stLosTask.pstRunTask->pStackPointer
    ; *           |<---      skip     --->|<---        copy to R0-R7         --->|
    ; *                                                             R3 to PSP--->|
    ; *                                    Stack pointer after LOS_StartToRun--->|
    ; */
    LDR     R3, [R1]

#if (LOSCFG_ENABLE_MPU == YES)
    LDR     R0, [R1, #8]            ; TCB->pMpuSettings
    LDR     R1, =0xe000ed9c
    MOVS    R4, #6                  ; 6 regions
0
    LDM     R0!, {R2, R5}
    STR     R2, [r1]
    STR     R5, [r1, #4]
    SUBS    R4, R4, #1
    BNE     %B0

    LDR     R2, [R3]
    ADDS    R3, R3, #4
    ORRS    R2, #2                  ; regset->control do not have CONTROL.SPSEL bit
#else
    MOVS    R2, #2
#endif

    ADDS    R3, R3, #36             ; skip R4-R11, PRIMASK.

    LDR     R0, [R3]
    LDR     R1, [r3, #4 * 5]
    MOV     LR, R1
    LDR     R1, [R3, #4 * 6]

    ADDS    R3, R3, #4 * 8

    ;/**
    ; * Set the stack pointer of g_stLosTask.pstRunTask to PSP.
    ; */
    MSR     PSP, R3

    ;/**
    ; * Enable interrupt. (The default PRIMASK value is 0, so enable directly)
    ; */
    CPSIE   I

    ;/**
    ; * Set the CONTROL register, after schedule start, privilege level and stack = PSP.
    ; */
    MSR     CONTROL, R2

    ;/**
    ; * Jump directly to the default PC of g_stLosTask.pstRunTask, the field information
    ; * of the main function will be destroyed and will never be returned.
    ; */
    BX      R1

    ALIGN
}

/*******************************************************************************
 Function:
        UINTPTR LOS_IntLock(VOID);
 Description:
        Disable all interrupts except Reset,NMI and HardFault.
        The value of currnet interruption state will be returned to the caller to save.

 Function:
        VOID LOS_IntRestore(UINTPTR uvIntSave);
 Description:
        Restore the locked interruption of LOS_IntLock.
        The caller must pass in the value of interruption state previously saved.
*******************************************************************************/
__asm UINTPTR LOS_IntLock(VOID)
{
    PRESERVE8

    MRS     R0, PRIMASK
    CPSID   I
    BX      LR

    ALIGN
}

__asm UINTPTR LOS_IntUnLock(VOID)
{
    PRESERVE8

    MRS     R0, PRIMASK
    CPSIE   I
    BX      LR

    ALIGN
}

__asm VOID LOS_IntRestore(UINTPTR flag)
{
    PRESERVE8

    MSR     PRIMASK, R0
    BX      LR

    ALIGN
}

/*******************************************************************************
 Function:
        VOID osTaskSchedule(VOID);
 Description:
        Start the task swtich process by software trigger PendSV interrupt.
*******************************************************************************/
__asm VOID osTaskSchedule(VOID)
{
    PRESERVE8

    LDR     R0, =OS_NVIC_INT_CTRL
    LDR     R1, =OS_NVIC_PENDSVSET
    STR     R1, [R0]
    BX      LR

    ALIGN
}

/*******************************************************************************
 Function:
        VOID PendSV_Handler(VOID);
 Description:
        PendSV interrupt handler, switch the context of the task.
        First: Save the context of the current running task(g_stLosTask.pstRunTask)
               to its own stack.
        Second: Restore the context of the next running task(g_stLosTask.pstNewTask)
                from its own stack.
*******************************************************************************/
__asm VOID PendSV_Handler(VOID)
{
    PRESERVE8

    IMPORT  g_pfnTskSwitchHook

    ;/**
    ; * R12: Save the interruption state of the current running task.
    ; * Disable all interrupts except Reset,NMI and HardFault
    ; */
    MRS     R12, PRIMASK
    CPSID   I

    ;/**
    ; * Call task switch hook.
    ; */
    LDR     R2, =g_pfnTskSwitchHook
    LDR     R2, [R2]
    CMP     R2, #0
    BEQ     TaskSwitch
    MOV     R1, LR
    PUSH    {R0, R1}
    BLX     R2
    POP     {R0, R1}
    MOV     LR, R1

TaskSwitch
    ;/**
    ; * R0 = now stack pointer of the current running task.
    ; */
    MRS     R0, PSP

    ;/**
    ; * Save the stack frame(R4-R11) of the current running task.
    ; * R12 save the PRIMASK value of the current running task.
    ; * NOTE: Before entering the exception handler function, these registers
    ; *       (xPSR,PC,LR,R12,R3-R0) have been automatically
    ; *       saved by the CPU in the stack of the current running task.
    ; *
    ; * The stack of the current running task is as follows:
    ; *
    ; *           |<=== PUSH: Save the context of the current running task
    ; *           |                                                      High addr--->|
    ; * ----------+-----------------------+--------------------------------------+-----
    ; *           |   R4-R11,   PRIMASK   |   R0-R3,   R12,   LR,   PC,   xPSR   |
    ; * ----------+-----------------------+--------------------------------------+-----
    ; *                               Stack pointer before entering exception--->|
    ; *                                   |<---        cpu auto saved        --->|
    ; *                                   |<---PSP to R0
    ; *           |<---Top of the stack, save to g_stLosTask.pstRunTask->pStackPointer
    ; */
    SUBS    R0, #36
    STMIA   R0!, {R4-R7}            ; save R4-R7.
    MOV     R3, R8                  ; copy R8-r12 to R3-r7.
    MOV     R4, R9
    MOV     R5, R10
    MOV     R6, R11
    MOV     R7, R12
    STMIA   R0!, {R3-R7}            ; save R8-R12.
    SUBS    R0, #36

#if (LOSCFG_ENABLE_MPU == YES)
    SUBS    R0, R0, #4
    MRS     R1, CONTROL
    STR     R1, [R0]
#endif

    ;/**
    ; * R5,R3.
    ; */
    LDR     R5, =g_stLosTask
    MOVS    R3, #OS_TASK_STATUS_RUNNING

    ;/**
    ; * Save the stack pointer of the current running task to TCB.
    ; * (g_stLosTask.pstRunTask->pStackPointer = R0)
    ; */
    LDR     R6, [R5]
    STR     R0, [R6]

    ;/**
    ; * Clear the RUNNING state of the current running task.
    ; * (g_stLosTask.pstRunTask->usTaskStatus &= ~OS_TASK_STATUS_RUNNING)
    ; */
    LDRH    R7, [R6, #4]
    BICS    R7, R7, R3
    STRH    R7, [R6, #4]

    ;/**
    ; * Switch the current running task to the next running task.
    ; * (g_stLosTask.pstRunTask = g_stLosTask.pstNewTask)
    ; */
    LDR     R0, [R5, #4]
    STR     R0, [R5]

    ;/**
    ; * Set the RUNNING state of the next running task.
    ; * (g_stLosTask.pstNewTask->usTaskStatus |= OS_TASK_STATUS_RUNNING)
    ; */
    LDRH    R7, [R0, #4]
    ORRS    R7, R7, R3
    STRH    R7, [R0, #4]

    ;/**
    ; * Restore the stack pointer of the next running task from TCB.
    ; * (R1 = g_stLosTask.pstNewTask->pStackPointer)
    ; */
    LDR     R1, [R0]

#if (LOSCFG_ENABLE_MPU == YES)
    LDR     R4, [R0, #8]            ; TCB->pMpuSettings
    LDR     R5, =0xe000ed9c
    MOVS    R6, #6                  ; 6 regions, cortex-m0+ do not have alias reg
0
    LDM     R4!, {R2, R3}
    STM     R5!, {R2, R3}
    SUBS    R5, R5, #8
    SUBS    R6, R6, #1
    BNE     %B0

    LDR     R4, [R1]
    ADDS    R1, R1, #4
    MSR     CONTROL, R4
#endif

    ;/**
    ; * Restore the stack frame(R4-R11) of the next running task.
    ; * R12 restore the PRIMASK value of the next running task.
    ; * NOTE: After exiting the exception handler function, these registers
    ; *       (xPSR,R0-R3,R12,LR,PC) will be automatically
    ; *       restored by the CPU from the stack of the next running task.
    ; *
    ; * 1. The stack of the next running task is as follows:
    ; *
    ; *                    POP: Restore the context of the next running task ===>|
    ; *                                                                  High addr--->|
    ; * ----------+-----------------------+--------------------------------------+-----
    ; *           |   R4-R11,   PRIMASK   |   R0-R3,   R12,   LR,   PC,   xPSR   |
    ; * ----------+-----------------------+--------------------------------------+-----
    ; *           |<---Top of the stack, restored from g_stLosTask.pstNewTask->pStackPointer
    ; *                      R1 to PSP--->|
    ; *                                   |<---      cpu auto restoring      --->|
    ; *                                 Stack pointer after exiting exception--->|
    ; *
    ; * 2. If the next running task is run for the first time, the stack is as follows:
    ; *
    ; *                    POP: Restore the context of the next running task ===>|
    ; *                                                             High addr--->|
    ; *                                                   Bottom of the stack--->|
    ; * ----------+-----------------------+--------------------------------------+
    ; *           |   R4-R11,   PRIMASK   |   R0-R3,   R12,   LR,   PC,   xPSR   |
    ; * ----------+-----------------------+--------------------------------------+
    ; *           |<---Top of the stack, restored from g_stLosTask.pstNewTask->pStackPointer
    ; *                      R1 to PSP--->|
    ; *                                   |<---      cpu auto restoring      --->|
    ; *                                 Stack pointer after exiting exception--->|
    ; */
    ADDS    R1, #16
    LDMFD   R1!, {R3-R7}            ; restore R8-R12 to R3-R7.
    MOV     R8, R3                  ; copy R3-R7 to R8-R12.
    MOV     R9, R4
    MOV     R10, R5
    MOV     R11, R6
    MOV     R12, R7
    ;/**
    ; * Set the stack pointer of the next running task to PSP.
    ; */
    MSR     PSP, R1
    SUBS    R1, #36
    LDMFD   R1!, {R4-R7}            ; restore R4-R7.

    ;/**
    ; * Restore the interruption state of the next running task.
    ; */
    MSR     PRIMASK, R12
    BX      LR

    ALIGN
}

