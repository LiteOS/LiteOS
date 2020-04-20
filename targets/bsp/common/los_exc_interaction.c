/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Exception Interaction Implementation
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

#include "los_exc_interaction_pri.h"
#ifdef LOSCFG_EXC_INTERACTION
#include "los_task_pri.h"
#ifdef LOSCFG_SHELL
#include "shell.h"
#include "shell_pri.h"
#endif
#include "console.h"
#include "hisoc/uart.h"
#include "hal_hwi.h"

/* Inter-module variable and function */
extern CONSOLE_CB *g_console[];

#define IS_UARTSHELL_ID(taskID) (((taskID) == shellCB->shellTaskHandle) || \
                                 ((taskID) == shellCB->shellEntryHandle))

STATIC BOOL IsIdleTask(UINT32 taskID)
{
    UINT32 i;

    for (i = 0; i < LOSCFG_KERNEL_CORE_NUM; i++) {
        if (taskID == g_percpu[i].idleTaskID) {
            return TRUE;
        }
    }

    return FALSE;
}

STATIC BOOL IsSwtTask(UINT32 taskID)
{
    UINT32 i;

    for (i = 0; i < LOSCFG_KERNEL_CORE_NUM; i++) {
        if (taskID == g_percpu[i].swtmrTaskID) {
            return TRUE;
        }
    }

    return FALSE;
}

UINT32 OsExcInteractionTaskCheck(const TSK_INIT_PARAM_S *initParam)
{
    if (initParam->pfnTaskEntry == (TSK_ENTRY_FUNC)OsIdleTask) {
        return LOS_OK;
    }
    if ((initParam->pfnTaskEntry == (TSK_ENTRY_FUNC)ShellTask) ||
        (initParam->pfnTaskEntry == (TSK_ENTRY_FUNC)ShellEntry)) {
        return LOS_OK;
    }
    return LOS_NOK;
}

VOID OsExcInteractionTaskKeep(VOID)
{
    LosTaskCB *taskCB = NULL;
    UINT16 tempStatus;
    UINT32 taskID;
    UINT32 curIrqNum;
    ShellCB *shellCB = NULL;
    CONSOLE_CB *consoleCB = NULL;

    consoleCB = g_console[CONSOLE_SERIAL - 1];
    if (consoleCB == NULL) {
        PRINTK("Serial Shell Uninitialized\n");
        return;
    }
    shellCB = (ShellCB *)(consoleCB->shellHandle);

    g_intCount[ArchCurrCpuid()] = 0;
    for (taskID = 0; taskID < g_taskMaxNum; taskID++) {
        if (taskID == OsCurrTaskGet()->taskID) {
            continue;
        } else if ((IsIdleTask(taskID) == TRUE) || IS_UARTSHELL_ID(taskID)) {
            continue;
        }

        taskCB = OS_TCB_FROM_TID(taskID);

        tempStatus = taskCB->taskStatus;
        if (tempStatus & OS_TASK_STATUS_UNUSED) {
            continue;
        }
        if (IsSwtTask(taskID) == TRUE) {
            taskCB->taskFlags &= (~OS_TASK_FLAG_SYSTEM);
        }

        (VOID)LOS_TaskDelete(taskID);
    }
    HalIrqInit();
    HalIrqUnmask(NUM_HAL_INTERRUPT_UART);
    curIrqNum = HalCurIrqGet();
    HalIrqClear(curIrqNum);
    (VOID)LOS_TaskDelete(OsCurrTaskGet()->taskID);
    /* unreachable */
}
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif
