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
#define NVIC_PENDSV_SYSTICK_PRI         0xC0C00000  /* SysTick + PendSV priority level (lowest). */
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
    "ORRS   R1, R2\n\t"
    "STR    R1, [R0]\n\t"

    /**
     * Set g_bTaskScheduled = 1.
     */
    "LDR    R0, =g_bTaskScheduled\n\t"
    "MOVS   R1, #1\n\t"
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
    "MOVS   R3, %2                  @ OS_TASK_STATUS_RUNNING\n\t"
    "ORRS   R2, R3\n\t"
    "STRH   R2, [R1, #4]\n\t"

    /**
     * Restore the default stack frame(R0-R3,R12,LR,PC,xPSR) of g_stLosTask.pstRunTask to R0-R7.
     * Return by setting the CONTROL register.
     *
     * The initial stack of the current running task is as follows:
     *
     *                 POP: Restore the context of the current running task ===>|
     *                                                             High addr--->|
     *                                                   Bottom of the stack--->|
     * ----------+-----------------------+--------------------------------------+
     *           |   R4-R11,   PRIMASK   |   R0-R3,   R12,   LR,   PC,   xPSR   |
     * ----------+-----------------------+--------------------------------------+
     *           |<---Top of the stack, restored from g_stLosTask.pstRunTask->pStackPointer
     *           |<---      skip     --->|<---        copy to R0-R7         --->|
     *                                                             R3 to PSP--->|
     *                                    Stack pointer after LOS_StartToRun--->|
     */
    "LDR    R3, [R1]\n\t"

#if (LOSCFG_ENABLE_MPU == YES)
    "LDR    R0, [R1, #8]            @ TCB->pMpuSettings\n\t"
    "LDR    R1, =%3                 @ MPU_RBAR\n\t"
    "MOVS   R4, #6                  @ 6 regions \n\t"
    "0:\n\t"
    "LDM    R0!, {R2, R5}\n\t"
    "STR    R2, [r1]\n\t"
    "STR    R5, [r1, #4]\n\t"
    "SUBS   R4, R4, #1\n\t"
    "BNE    0b\n\t"

    "LDR    R2, [R3]\n\t"
    "ADDS   R3, R3, #4\n\t"
    "ORRS   R2, #2                  @ regset->control do not have CONTROL.SPSEL bit\n\t"
#else
    "MOVS   R2, #2\n\t"
#endif

    "ADDS   R3, R3, #36             @ skip R4-R11, PRIMASK.\n\t"

    "LDR    R0, [R3]\n\t"
    "LDR    R1, [r3, #4 * 5]\n\t"
    "MOV    LR, R1\n\t"
    "LDR    R1, [R3, #4 * 6]\n\t"

    "ADDS   R3, R3, #4 * 8\n\t"

    /**
     * Set the stack pointer of g_stLosTask.pstRunTask to PSP.
     */
    "MSR    PSP, R3\n\t"

    /**
     * Enable interrupt. (The default PRIMASK value is 0, so enable directly)
     */
    "CPSIE  I\n\t"

    /**
     * Set the CONTROL register, after schedule start, privilege level and stack = PSP.
     */
    "MSR    CONTROL, R2\n\t"

    /**
     * Jump directly to the default PC of g_stLosTask.pstRunTask, the field information
     * of the main function will be destroyed and will never be returned.
     */
    "BX     R1\n\t"
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
    "CMP    R2, #0\n\t"
    "BEQ    0f\n\t"
    "MOV    R1, LR\n\t"
    "PUSH   {R0, R1}\n\t"
    "BLX    R2\n\t"
    "POP    {R0, R1}\n\t"
    "MOV    LR, R1\n\t"

    "0:\n\t"

    /**
     * R0 = now stack pointer of the current running task.
     */
    "MRS    R0, PSP\n\t"

    /**
     * Save the stack frame(R4-R11) of the current running task.
     * R12 save the PRIMASK value of the current running task.
     * NOTE: Before entering the exception handler function, these registers
     *       (xPSR,PC,LR,R12,R3-R0) have been automatically
     *       saved by the CPU in the stack of the current running task.
     *
     * The stack of the current running task is as follows:
     *
     *           |<=== PUSH: Save the context of the current running task
     *           |                                                      High addr--->|
     * ----------+-----------------------+--------------------------------------+-----
     *           |   R4-R11,   PRIMASK   |   R0-R3,   R12,   LR,   PC,   xPSR   |
     * ----------+-----------------------+--------------------------------------+-----
     *                               Stack pointer before entering exception--->|
     *                                   |<---        cpu auto saved        --->|
     *                                   |<---PSP to R0
     *           |<---Top of the stack, save to g_stLosTask.pstRunTask->pStackPointer
     */
    "SUBS   R0, #36\n\t"
    "STMIA  R0!, {R4-R7}            @ save R4-R7.\n\t"
    "MOV    R3, R8                  @ copy R8-r12 to R3-r7.\n\t"
    "MOV    R4, R9\n\t"
    "MOV    R5, R10\n\t"
    "MOV    R6, R11\n\t"
    "MOV    R7, R12\n\t"
    "STMIA  R0!, {R3-R7}            @ save R8-R12.\n\t"
    "SUBS   R0, #36\n\t"

#if (LOSCFG_ENABLE_MPU == YES)
    "SUBS   R0, R0, #4\n\t"
    "MRS    R1, CONTROL\n\t"
    "STR    R1, [R0]\n\t"
#endif

    /**
     * R5,R3.
     */
    "LDR    R5, =g_stLosTask\n\t"
    "MOVS   R3, %0                  @ OS_TASK_STATUS_RUNNING\n\t"

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
    "BICS   R7, R7, R3\n\t"
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
    "ORRS   R7, R3\n\t"
    "STRH   R7, [R0, #4]\n\t"

    /**
     * Restore the stack pointer of the next running task from TCB.
     * (R1 = g_stLosTask.pstNewTask->pStackPointer)
     */
    "LDR    R1, [R0]\n\t"

#if (LOSCFG_ENABLE_MPU == YES)
    "LDR    R4, [R0, #8]            @ TCB->pMpuSettings\n\t"
    "LDR    R5, =%1                 @ MPU_RBAR\n\t"
    "MOVS   R6, #6                  @ 6 regions, cortex-m0+ do not have alias reg\n\t"
    "0:\n\t"
    "LDM    R4!, {R2, R3}\n\t"
    "STM    R5!, {R2, R3}\n\t"
    "SUBS   R5, R5, #8\n\t"
    "SUBS   R6, R6, #1\n\t"
    "BNE    0b\n\t"

    "LDR    R4, [R1]\n\t"
    "ADDS   R1, R1, #4\n\t"
    "MSR    CONTROL, R4\n\t"
#endif

    /**
     * Restore the stack frame(R4-R11) of the next running task.
     * R12 restore the PRIMASK value of the next running task.
     * NOTE: After exiting the exception handler function, these registers
     *       (xPSR,R0-R3,R12,LR,PC) will be automatically
     *       restored by the CPU from the stack of the next running task.
     *
     * 1. The stack of the next running task is as follows:
     *
     *                    POP: Restore the context of the next running task ===>|
     *                                                                  High addr--->|
     * ----------+-----------------------+--------------------------------------+-----
     *           |   R4-R11,   PRIMASK   |   R0-R3,   R12,   LR,   PC,   xPSR   |
     * ----------+-----------------------+--------------------------------------+-----
     *           |<---Top of the stack, restored from g_stLosTask.pstNewTask->pStackPointer
     *                      R1 to PSP--->|
     *                                   |<---      cpu auto restoring      --->|
     *                                 Stack pointer after exiting exception--->|
     *
     * 2. If the next running task is run for the first time, the stack is as follows:
     *
     *                    POP: Restore the context of the next running task ===>|
     *                                                             High addr--->|
     *                                                   Bottom of the stack--->|
     * ----------+-----------------------+--------------------------------------+
     *           |   R4-R11,   PRIMASK   |   R0-R3,   R12,   LR,   PC,   xPSR   |
     * ----------+-----------------------+--------------------------------------+
     *           |<---Top of the stack, restored from g_stLosTask.pstNewTask->pStackPointer
     *                      R1 to PSP--->|
     *                                   |<---      cpu auto restoring      --->|
     *                                 Stack pointer after exiting exception--->|
     */
    "ADDS   R1, #16\n\t"
    "LDMFD  R1!, {R3-R7}            @ restore R8-R12 to R3-R7.\n\t"
    "MOV    R8, R3                  @ copy R3-R7 to R8-R12.\n\t"
    "MOV    R9, R4\n\t"
    "MOV    R10, R5\n\t"
    "MOV    R11, R6\n\t"
    "MOV    R12, R7\n\t"
    /**
     * Set the stack pointer of the next running task to PSP.
     */
    "MSR    PSP, R1\n\t"
    "SUBS   R1, #36\n\t"
    "LDMFD  R1!, {R4-R7}            @ restore R4-R7.\n\t"

    /**
     * Restore the interruption state of the next running task.
     */
    "MSR    PRIMASK, R12\n\t"
    "BX     LR\n\t"
    :
    : "I" (OS_TASK_STATUS_RUNNING)
    );
}
