/** ----------------------------------------------------------------------------
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
/** ----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/
 
#include <los_config.h>
#include <los_typedef.h>

#define NVIC_INT_CTRL                   0xE000ED04  /* Interrupt Control and State Register. */
#define NVIC_PENDSVSET                  0x10000000  /* Value to trigger PendSV exception. */

#define NVIC_SYSPRI2                    0xE000ED20  /* System Handler Priority Register 2. */
#define NVIC_PENDSV_SYSTICK_PRI         0xFFFF0000  /* SysTick + PendSV priority level (lowest). */
#define MPU_RBAR                        0xE000ED9C  /* MPU Region Base Address register */

#define OS_TASK_STATUS_RUNNING          0x00000010  /* Task Status Flag (RUNNING). */

/****************************************************************************************
* Function:
*        VOID LOS_StartToRun(VOID);
* Description:
*        Start the first task, which is the highest priority task in the priority queue.
*        Other tasks are started by task scheduling.
****************************************************************************************/
__attribute__((naked)) void LOS_StartToRun(void)
{
    asm (
    ".syntax unified\n\t"

    "CPSID  I\n\t"

    /**
     * Set PendSV and SysTick prority to the lowest.
     * read ---> modify ---> write-back.
     */

    "LDR    R0, =%0                 @ NVIC_SYSPRI2\n\t"
    "LDR    R1, =%1                 @ NVIC_PENDSV_SYSTICK_PRI\n\t"
    "LDR    R2, [R0]\n\t"
    "ORR    R1, R1, R2\n\t"
    "STR    R1, [R0]\n\t"

    /**
     * Set g_bTaskScheduled = 1.
     */

    "LDR    R0, =g_bTaskScheduled\n\t"
    "MOV    R1, #1\n\t"
    "STR    R1, [R0]\n\t"

    /**
     * Set g_stLosTask.pstRunTask = g_stLosTask.pstNewTask.
     */
    "LDR    R0, =g_stLosTask\n\t"
    "LDR    R1, [R0, #4]\n\t"
    "STR    R1, [R0]\n\t"

    /**
     * Set g_stLosTask.pstRunTask->usTaskStatus |= OS_TASK_STATUS_RUNNING.
     */
    "LDR    R1, [R0]\n\t"
    "LDRH   R2, [R1, #4]\n\t"
    "MOV    R3, %2                  @ OS_TASK_STATUS_RUNNING\n\t"
    "ORR    R2, R2, R3\n\t"
    "STRH   R2, [R1, #4]\n\t"

    /**
     * Restore the default stack frame(R0-R3,R12,LR,PC,xPSR) of g_stLosTask.pstRunTask to R0-R7.
     * [Initial EXC_RETURN ignore, if FPU supported] return by setting the CONTROL register.
     *
     * The initial stack of the current running task is as follows:
     *
     *       POP: Restore the context of the current running task ===>|
     *                                                   High addr--->|
     *                                         Bottom of the stack--->|
     * ----------+-----------------------------+----------------------+
     *           | R4-R11,PRIMASK[,EXC_RETURN] | R0-R3,R12,LR,PC,xPSR |
     * ----------+-----------------------------+----------------------+
     *           |<---Top of the stack, restored from g_stLosTask.pstRunTask->pStackPointer
     *           |<---         skip        --->|<--- get r0,lr,pc --->|
     *                                                  R12 to PSP--->|
     *                          Stack pointer after LOS_StartToRun--->|
     */

    "LDR    R12, [R1]\n\t"

#if (LOSCFG_ENABLE_MPU == YES)
    "LDR    R0, [R1, #8]            @ TCB->pMpuSettings\n\t"
    "LDR    R1, =%3                 @ MPU_RBAR\n\t"
    "LDM    R0!, {R2-R7}\n\t"
    "STM    R1, {R2-R7}\n\t"
    "LDM    R0!, {R2-R7}\n\t"
    "STM    R1, {R2-R7}\n\t"

    "LDR    R3, [R12], #4           @ get the CONTROL value\n\t"
    "ORR    R3, #2                  @ set CONTROL.SPSEL\n\t"
#else
    "MOV    R3, #2\n\t"
#endif

    "ADD    R12, R12, #36           @ skip R4-R11, PRIMASK.\n\t"

#if defined (__VFP_FP__) && !defined(__SOFTFP__)
    "ADD    R12, R12, #4            @ if FPU exist, skip EXC_RETURN.\n\t"
#endif

    "LDR    R0, [R12]\n\t"
    "LDR    LR, [R12, #4 * 5]\n\t"
    "LDR    R2, [R12, #4 * 6]\n\t"

    "ADD    R12, R12, #4 * 8\n\t"

    /**
     * Set the stack pointer of g_stLosTask.pstRunTask to PSP.
     */
    "MSR    PSP, R12\n\t"

    /**
     * Enable interrupt. (The default PRIMASK value is 0, so enable directly)
     */
    "CPSIE  I\n\t"

    /**
     * Set the CONTROL register, after schedule start, privilege level and stack = PSP.
     */
    "MSR    CONTROL, R3\n\t"

    /**
     * Jump directly to the default PC of g_stLosTask.pstRunTask, the field information
     * of the main function will be destroyed and will never be returned.
     */
    "BX     R2\n\t"
    :
    : "X" (NVIC_SYSPRI2), "X" (NVIC_PENDSV_SYSTICK_PRI),
      "I" (OS_TASK_STATUS_RUNNING), "X" (MPU_RBAR)
    );
}

/****************************************************************************************
* Function:
*        UINTPTR LOS_IntLock(VOID);
* Description:
*        Disable all interrupts except Reset,NMI and HardFault.
*        The value of currnet interruption state will be returned to the caller to save.
*
* Function:
*        VOID LOS_IntRestore(UINTPTR uvIntSave);
* Description:
*        Restore the locked interruption of LOS_IntLock.
*        The caller must pass in the value of interruption state previously saved.
****************************************************************************************/
__attribute__((naked)) UINTPTR LOS_IntLock(void)
{
    asm (
    ".syntax unified\n\t"

    "MRS    R0, PRIMASK\n\t"
    "CPSID  I\n\t"
    "BX     LR\n\t"
    );
}

__attribute__((naked)) UINTPTR LOS_IntUnLock(void)
{
    asm (
    ".syntax unified\n\t"

    "MRS    R0, PRIMASK\n\t"
    "CPSIE  I\n\t"
    "BX     LR\n\t"
    );
}

__attribute__((naked)) VOID LOS_IntRestore(UINTPTR key)
{
    asm (
    ".syntax unified\n\t"

    "MSR    PRIMASK, R0\n\t"
    "BX     LR\n\t"
    );
}

/****************************************************************************************
* Function:
*        VOID osTaskSchedule(VOID);
* Description:
*        Start the task swtich process by software trigger PendSV interrupt.
****************************************************************************************/
__attribute__((naked)) VOID osTaskSchedule(VOID)
{
    asm (
    ".syntax unified\n\t"

    "LDR    R0, =%0                 @ NVIC_INT_CTRL\n\t"
    "LDR    R1, =%1                 @ NVIC_PENDSVSET\n\t"
    "STR    R1, [R0]\n\t"
    "BX     LR\n\t"
    :
    : "X" (NVIC_INT_CTRL), "X" (NVIC_PENDSVSET)
    );
}

/****************************************************************************************
* Function:
*        VOID PendSV_Handler(VOID);
* Description:
*        PendSV interrupt handler, switch the context of the task.
*        First: Save the context of the current running task(g_stLosTask.pstRunTask)
*               to its own stack.
*        Second: Restore the context of the next running task(g_stLosTask.pstNewTask)
*                from its own stack.
****************************************************************************************/
__attribute__((naked)) VOID PendSV_Handler(VOID)
{
    asm (
    ".syntax unified\n\t"

    /**
     * R12: Save the interruption state of the current running task.
     * Disable all interrupts except Reset,NMI and HardFault
     */
    "MRS    R12, PRIMASK\n\t"
    "CPSID  I\n\t"

    /**
     * Call task switch hook.
     */
    "LDR    R2, =g_pfnTskSwitchHook\n\t"
    "LDR    R2, [R2]\n\t"
    "CBZ    R2, 0f\n\t"
    "PUSH   {R12, LR}\n\t"
    "BLX    R2\n\t"
    "POP    {R12, LR}\n\t"

    "0:\n\t"

    /**
     * R0 = now stack pointer of the current running task.
     */
    "MRS    R0, PSP\n\t"

    /**
     * Save the stack frame([S16-S31,]R4-R11) of the current running task.
     * R12 save the PRIMASK value of the current running task.
     * NOTE: 1. Before entering the exception handler function, these registers
     *       ([NO_NAME,FPSCR,S15-S0],xPSR,PC,LR,R12,R3-R0) have been automatically
     *       saved by the CPU in the stack of the current running task.
     *       2. If lazy stacking is enabled, space is reserved on the stack for
     *       the floating-point context(FPSCR,S15-S0), but the floating-point state
     *       is not saved. when the floating-point instruction(VSTMDBEQ  R0!, {D8-D15})
     *       is executed, the floating-point context(FPSCR,S15-S0) is first saved into
     *       the space reserved on the stack. In other words, the instruction
     *       'VSTMDBEQ  R0!, {D8-D15}' will trigger the CPU to save 'FPSCR,S15-S0' first.
     *
     * The stack of the current running task is as follows:
     *
     *   |<=== PUSH: Save the context of the current running task
     *   |                                                                        High addr--->|
     * --+---------------------------------------+---------------------------------------------+---
     *   | R4-R11,PRIMASK[,EXC_RETURN[,S16-S31]] | R0-R3,R12,LR,PC,xPSR[,S0-S15,FPSCR,NO_NAME] |
     *   |                                                             [    lazy stacking    ] |
     * --+---------------------------------------+---------------------------------------------+---
     *                                              Stack pointer before entering exception--->|
     *                                           |<---            cpu auto saved           --->|
     *                                           |<---PSP to R0
     *   |<---Top of the stack, save to g_stLosTask.pstRunTask->pStackPointer
     */
#if defined (__VFP_FP__) && !defined(__SOFTFP__)       /* if FPU exist. */
    "TST    R14, #0x10              @ if the task using the FPU context\n\t"
    "BNE    0f\n\t"
    "VSTMDB R0!, {D8-D15}\n\t"
    "0:\n\t"
    "STMFD  R0!, {R14}              @ save EXC_RETURN\n\t"
#endif

    "STMFD  R0!, {R4-R12}           @ save the core registers and PRIMASK.\n\t"

#if (LOSCFG_ENABLE_MPU == YES)
    "MRS    R1, CONTROL\n\t"
    "STR    R1, [R0, #-4]!\n\t"
#endif

    /**
     * R5,R8.
     */
    "LDR    R5, =g_stLosTask\n\t"
    "MOV    R8, %0                 @ OS_TASK_STATUS_RUNNING\n\t"

    /**
     * Save the stack pointer of the current running task to TCB.
     * (g_stLosTask.pstRunTask->pStackPointer = R0)
     */
    "LDR    R6, [R5]\n\t"
    "STR    R0, [R6]\n\t"

    /**
     * Clear the RUNNING state of the current running task.
     * (g_stLosTask.pstRunTask->usTaskStatus &= ~OS_TASK_STATUS_RUNNING)
     */
    "LDRH   R7, [R6, #4]\n\t"
    "BIC    R7, R7, R8\n\t"
    "STRH   R7, [R6, #4]\n\t"

    /**
     * Switch the current running task to the next running task.
     * (g_stLosTask.pstRunTask = g_stLosTask.pstNewTask)
     */
    "LDR    R0, [R5, #4]\n\t"
    "STR    R0, [R5]\n\t"

    /**
     * Set the RUNNING state of the next running task.
     * (g_stLosTask.pstNewTask->usTaskStatus |= OS_TASK_STATUS_RUNNING)
     */
    "LDRH   R7, [R0, #4]\n\t"
    "ORR    R7, R7, R8\n\t"
    "STRH   R7, [R0, #4]\n\t"

    /**
     * Restore the stack pointer of the next running task from TCB.
     * (R1 = g_stLosTask.pstNewTask->pStackPointer)
     */
    "LDR    R1, [R0]\n\t"

#if (LOSCFG_ENABLE_MPU == YES)
    /**
     * Set the CONTROL register
     */

    "LDR    R4, [R0, #8]            @ TCB->pMpuSettings\n\t"
    "LDR    R5, =%1                 @ MPU_RBAR\n\t"
    "LDM    R4!, {R6-R11}\n\t"
    "STM    R5, {R6-R11}\n\t"
    "LDM    R4!, {R6-R11}\n\t"
    "STM    R5, {R6-R11}\n\t"
    "LDR    R4, [R1], #4\n\t"
    "MSR    CONTROL, R4\n\t"
#endif

    /**
     * Restore the stack frame(R4-R11[,S16-S31]) of the next running task.
     * R12 restore the PRIMASK value of the next running task.
     * NOTE: After exiting the exception handler function, these registers
     *       (PC,xPSR,R0-R3,R12,LR[,S0-S15,FPSCR,NO_NAME]) will be automatically
     *       restored by the CPU from the stack of the next running task.
     *
     * 1. The stack of the next running task is as follows:
     *
     *                                   POP: Restore the context of the next running task ===>|
     *                                                                            High addr--->|
     * --+---------------------------------------+---------------------------------------------+---
     *   | R4-R11,PRIMASK[,EXC_RETURN[,S16-S31]] | R0-R3,R12,LR,PC,xPSR[,S0-S15,FPSCR,NO_NAME] |
     * --+---------------------------------------+---------------------------------------------+---
     *   |<---Top of the stack, restored from g_stLosTask.pstNewTask->pStackPointer
     *                              R1 to PSP--->|
     *                                           |<---         cpu auto restoring          --->|
     *                                                Stack pointer after exiting exception--->|
     *
     * 2. If the next running task is run for the first time, the stack is as follows:
     *
     *          POP: Restore the context of the next running task ===>|
     *                                                   High addr--->|
     *                                         Bottom of the stack--->|
     * ----------+-----------------------------+----------------------+
     *           | R4-R11,PRIMASK[,EXC_RETURN] | R0-R3,R12,LR,PC,xPSR |
     * ----------+-----------------------------+----------------------+
     *           |<---Top of the stack, restored from g_stLosTask.pstNewTask->pStackPointer
     *                            R1 to PSP--->|
     *                                         |<---auto restoring--->|
     *                       Stack pointer after exiting exception--->|
     */
    "LDMFD  R1!, {R4-R12}           @ restore the core registers and PRIMASK\n\t"

#if defined (__VFP_FP__) && !defined(__SOFTFP__)       /* if FPU exist. */
    "LDMFD  R1!, {R14}              @ restore EXC_RETURN\n\t"
    "TST    R14, #0x10              @ if the task using the FPU context\n\t"
    "BNE    0f\n\t"
    "VLDMIA R1!, {D8-D15}\n\t"
    "0:\n\t"
#endif

    /**
     * Set the stack pointer of the next running task to PSP.
     */
    "MSR    PSP, R1\n\t"

    /**
     * Restore the interruption state of the next running task.
     */
    "MSR    PRIMASK, R12\n\t"
    "BX     LR\n\t"
    :
    : "I" (OS_TASK_STATUS_RUNNING), "X" (MPU_RBAR)
    );
}

