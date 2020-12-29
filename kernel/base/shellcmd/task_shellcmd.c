/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: ShellCmd Task
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

#include "stdio.h"
#include "stdlib.h"
#include "los_config.h"
#include "los_exc.h"
#include "los_memstat_pri.h"
#include "los_sem_pri.h"
#include "los_task_pri.h"
#ifdef LOSCFG_SHELL
#include "shcmd.h"
#include "shell.h"
#endif
#ifdef LOSCFG_KERNEL_CPUP
#include "los_cpup_pri.h"
#endif
#ifdef LOSCFG_SHELL_EXCINFO_DUMP
#include "los_exc_pri.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define OS_INVALID_SEM_ID  0xFFFFFFFF

#ifdef LOSCFG_KERNEL_CPUP
LITE_OS_SEC_BSS STATIC CPUP_INFO_S g_taskCpupAll[LOSCFG_BASE_CORE_TSK_LIMIT];
LITE_OS_SEC_BSS STATIC CPUP_INFO_S g_taskCpup10s[LOSCFG_BASE_CORE_TSK_LIMIT];
LITE_OS_SEC_BSS STATIC CPUP_INFO_S g_taskCpup1s[LOSCFG_BASE_CORE_TSK_LIMIT];
#endif

LITE_OS_SEC_BSS STATIC UINT32 g_taskWaterLine[LOSCFG_BASE_CORE_TSK_LIMIT];

LITE_OS_SEC_TEXT_MINOR UINT8 *OsShellCmdConvertTskStatus(UINT16 taskStatus)
{
    if (taskStatus & OS_TASK_STATUS_RUNNING) {
        return (UINT8 *)"Running";
    } else if (taskStatus & OS_TASK_STATUS_READY) {
        return (UINT8 *)"Ready";
    } else {
        if (taskStatus & OS_TASK_STATUS_DELAY) {
            return (UINT8 *)"Delay";
        } else if (taskStatus & OS_TASK_STATUS_PEND_TIME) {
            if (taskStatus & OS_TASK_STATUS_SUSPEND) {
                return (UINT8 *)"SuspendTime";
            } else if (taskStatus & OS_TASK_STATUS_PEND) {
                return (UINT8 *)"PendTime";
            }
        } else if (taskStatus & OS_TASK_STATUS_PEND) {
            return (UINT8 *)"Pend";
        } else if (taskStatus & OS_TASK_STATUS_SUSPEND) {
            return (UINT8 *)"Suspend";
        }
    }

    return (UINT8 *)"Invalid";
}

STATIC VOID OsShellCmdTaskWaterLineGet(const LosTaskCB *allTaskArray)
{
    const LosTaskCB *taskCB = NULL;
    UINT32 loop;

    for (loop = 0; loop < g_taskMaxNum; ++loop) {
        taskCB = allTaskArray + loop;
        if (taskCB->taskStatus & OS_TASK_STATUS_UNUSED) {
            continue;
        }

        (VOID)OsStackWaterLineGet((const UINTPTR *)((UINTPTR)taskCB->topOfStack + taskCB->stackSize),
                                  (const UINTPTR *)taskCB->topOfStack, &g_taskWaterLine[taskCB->taskId]);
    }
}

#ifdef LOSCFG_SHELL_EXCINFO_DUMP
LITE_OS_SEC_TEXT_MINOR STATIC VOID OsShellCmdTskInfoTitleExc(VOID)
{
    WriteExcInfoToBuf("\r\nName                   TaskEntryAddr       TID    ");
#ifdef LOSCFG_KERNEL_SMP
    WriteExcInfoToBuf("Affi    CPU    ");
#endif
    WriteExcInfoToBuf("Priority   Status       "
           "StackSize    WaterLine      StackPoint   TopOfStack   SemID        EventMask");

#ifdef LOSCFG_KERNEL_CPUP
    WriteExcInfoToBuf("  CPUUSE  CPUUSE10s  CPUUSE1s  ");
#endif /* LOSCFG_KERNEL_CPUP */
#ifdef LOSCFG_MEM_TASK_STAT
    WriteExcInfoToBuf("   MEMUSE");
#endif
    WriteExcInfoToBuf("\n");
    WriteExcInfoToBuf("----                   -------------       ---    ");
#ifdef LOSCFG_KERNEL_SMP
    WriteExcInfoToBuf("-----   ----   ");
#endif
    WriteExcInfoToBuf("--------   --------     "
           "---------    ----------     ----------   ----------   ----------   ---------");
#ifdef LOSCFG_KERNEL_CPUP
    WriteExcInfoToBuf("  ------  ---------  --------  ");
#endif /* LOSCFG_KERNEL_CPUP */
#ifdef LOSCFG_MEM_TASK_STAT
	WriteExcInfoToBuf("   ------");
#endif
    WriteExcInfoToBuf("\n");
}
#endif

LITE_OS_SEC_TEXT_MINOR STATIC VOID OsShellCmdTskInfoTitle(VOID)
{
    PRINTK("\r\nName                   TaskEntryAddr       TID    ");
#ifdef LOSCFG_KERNEL_SMP
    PRINTK("Affi    CPU    ");
#endif
    PRINTK("Priority   Status       "
           "StackSize    WaterLine      StackPoint   TopOfStack   SemID        EventMask");

#ifdef LOSCFG_KERNEL_CPUP
    PRINTK("  CPUUSE  CPUUSE10s  CPUUSE1s  ");
#endif /* LOSCFG_KERNEL_CPUP */
#ifdef LOSCFG_MEM_TASK_STAT
    PRINTK("   MEMUSE");
#endif
    PRINTK("\n");
    PRINTK("----                   -------------       ---    ");
#ifdef LOSCFG_KERNEL_SMP
    PRINTK("-----   ----   ");
#endif
    PRINTK("--------   --------     "
           "---------    ----------     ----------   ----------   ----------   ---------");
#ifdef LOSCFG_KERNEL_CPUP
    PRINTK("  ------  ---------  --------  ");
#endif /* LOSCFG_KERNEL_CPUP */
#ifdef LOSCFG_MEM_TASK_STAT
    PRINTK("   ------");
#endif
    PRINTK("\n");

#ifdef LOSCFG_SHELL_EXCINFO_DUMP
    OsShellCmdTskInfoTitleExc();
#endif
}

LITE_OS_SEC_TEXT_MINOR STATIC INLINE UINT32 OsGetSemID(const LosTaskCB *taskCB)
{
    UINT32 semId = OS_INVALID_SEM_ID;

    if (taskCB->taskSem != NULL) {
        semId = ((LosSemCB *)taskCB->taskSem)->semId;
    }

    return semId;
}

#ifdef LOSCFG_SHELL_EXCINFO_DUMP
LITE_OS_SEC_TEXT_MINOR STATIC VOID OsShellCmdTskInfoDataExc(const LosTaskCB *allTaskArray)
{
    const LosTaskCB *taskCB = NULL;
    UINT32 loop;
    UINT32 semId;

    for (loop = 0; loop < g_taskMaxNum; ++loop) {
        taskCB = allTaskArray + loop;
        if (taskCB->taskStatus & OS_TASK_STATUS_UNUSED) {
            continue;
        }

        semId = OsGetSemID(taskCB);

        WriteExcInfoToBuf("%-23s%-20p0x%-5x", taskCB->taskName, taskCB->taskEntry, taskCB->taskId);

#ifdef LOSCFG_KERNEL_SMP
        WriteExcInfoToBuf("0x%04x  %4d   ", taskCB->cpuAffiMask, (INT16)(taskCB->currCpu));
#endif

        WriteExcInfoToBuf("%-11u%-13s0x%-11x  0x%-11x%p   0x%-8x   0x%-10x   ", taskCB->priority,
               OsShellCmdConvertTskStatus(taskCB->taskStatus), taskCB->stackSize,
               g_taskWaterLine[taskCB->taskId],
               taskCB->stackPointer, taskCB->topOfStack, semId);

#ifdef LOSCFG_BASE_IPC_EVENT
        WriteExcInfoToBuf("0x%-3x", taskCB->eventMask);
#endif
#ifdef LOSCFG_KERNEL_CPUP
        WriteExcInfoToBuf(" %4u.%1u%9u.%1u%8u.%1u   ",
               g_taskCpupAll[taskCB->taskId].uwUsage / LOS_CPUP_PRECISION_MULT,
               g_taskCpupAll[taskCB->taskId].uwUsage % LOS_CPUP_PRECISION_MULT,
               g_taskCpup10s[taskCB->taskId].uwUsage / LOS_CPUP_PRECISION_MULT,
               g_taskCpup10s[taskCB->taskId].uwUsage % LOS_CPUP_PRECISION_MULT,
               g_taskCpup1s[taskCB->taskId].uwUsage / LOS_CPUP_PRECISION_MULT,
               g_taskCpup1s[taskCB->taskId].uwUsage % LOS_CPUP_PRECISION_MULT);
#endif /* LOSCFG_KERNEL_CPUP */
#ifdef LOSCFG_MEM_TASK_STAT
        WriteExcInfoToBuf("     %-11u", OsMemTaskUsage(taskCB->taskId));
#endif
        WriteExcInfoToBuf("\n");
    }
}
#endif

LITE_OS_SEC_TEXT_MINOR STATIC VOID OsShellCmdTskInfoData(const LosTaskCB *allTaskArray)
{
    const LosTaskCB *taskCB = NULL;
    UINT32 loop;
    UINT32 semId;

    for (loop = 0; loop < g_taskMaxNum; ++loop) {
        taskCB = allTaskArray + loop;
        if (taskCB->taskStatus & OS_TASK_STATUS_UNUSED) {
            continue;
        }

        semId = OsGetSemID(taskCB);

        PRINTK("%-23s%-20p0x%-5x", taskCB->taskName, taskCB->taskEntry, taskCB->taskId);

#ifdef LOSCFG_KERNEL_SMP
        PRINTK("0x%04x  %4d   ", taskCB->cpuAffiMask, (INT16)(taskCB->currCpu));
#endif
        PRINTK("%-11u%-13s0x%-11x  0x%-11x%p   0x%-8x   0x%-10x   ", taskCB->priority,
               OsShellCmdConvertTskStatus(taskCB->taskStatus), taskCB->stackSize,
               g_taskWaterLine[taskCB->taskId],
               taskCB->stackPointer, taskCB->topOfStack, semId);
#ifdef LOSCFG_BASE_IPC_EVENT
        PRINTK("0x%-3x", taskCB->eventMask);
#endif
#ifdef LOSCFG_KERNEL_CPUP
        PRINTK(" %4u.%1u%9u.%1u%8u.%1u   ",
               g_taskCpupAll[taskCB->taskId].uwUsage / LOS_CPUP_PRECISION_MULT,
               g_taskCpupAll[taskCB->taskId].uwUsage % LOS_CPUP_PRECISION_MULT,
               g_taskCpup10s[taskCB->taskId].uwUsage / LOS_CPUP_PRECISION_MULT,
               g_taskCpup10s[taskCB->taskId].uwUsage % LOS_CPUP_PRECISION_MULT,
               g_taskCpup1s[taskCB->taskId].uwUsage / LOS_CPUP_PRECISION_MULT,
               g_taskCpup1s[taskCB->taskId].uwUsage % LOS_CPUP_PRECISION_MULT);
#endif /* LOSCFG_KERNEL_CPUP */
#ifdef LOSCFG_MEM_TASK_STAT
        PRINTK("     %-11u", OsMemTaskUsage(taskCB->taskId));
#endif
        PRINTK("\n");
    }

#ifdef LOSCFG_SHELL_EXCINFO_DUMP
    OsShellCmdTskInfoDataExc(allTaskArray);
#endif
}

LITE_OS_SEC_TEXT_MINOR UINT32 OsShellCmdTskInfoGet(UINT32 taskId)
{
    BOOL backupFlag = TRUE;
    BOOL lockFlag = FALSE;
    UINT32 size, intSave;
    LosTaskCB *tcbArray = NULL;
    INT32 ret;

    if (taskId == OS_ALL_TASK_MASK) {
        size = g_taskMaxNum * sizeof(LosTaskCB);
        tcbArray = (LosTaskCB *)LOS_MemAlloc(m_aucSysMem1, size);
        if (tcbArray == NULL) {
            PRINTK("Memory is not enough to save task info!\n");
            tcbArray = g_taskCBArray;
            backupFlag = FALSE;
        }
#ifdef LOSCFG_KERNEL_CPUP
        (VOID)memset_s((VOID *)g_taskCpupAll, sizeof(g_taskCpupAll), 0, sizeof(g_taskCpupAll));
        (VOID)memset_s((VOID *)g_taskCpup10s, sizeof(g_taskCpup10s), 0, sizeof(g_taskCpup10s));
        (VOID)memset_s((VOID *)g_taskCpup1s, sizeof(g_taskCpup1s), 0, sizeof(g_taskCpup1s));
#endif
        (VOID)memset_s((VOID *)g_taskWaterLine, sizeof(g_taskWaterLine), 0, sizeof(g_taskWaterLine));

        if (LOS_SpinHeld(&g_taskSpin) == FALSE) {
            SCHEDULER_LOCK(intSave);
            lockFlag = TRUE;
        }
        if (backupFlag == TRUE) {
            ret = memcpy_s(tcbArray, size, g_taskCBArray, size);
            if (ret != EOK) {
                return LOS_NOK;
            }
        }
#ifdef LOSCFG_KERNEL_CPUP
        (VOID)LOS_AllCpuUsage(LOSCFG_BASE_CORE_TSK_LIMIT, g_taskCpupAll, CPUP_ALL_TIME, 1);
        (VOID)LOS_AllCpuUsage(LOSCFG_BASE_CORE_TSK_LIMIT, g_taskCpup10s, CPUP_LAST_TEN_SECONDS, 1);
        (VOID)LOS_AllCpuUsage(LOSCFG_BASE_CORE_TSK_LIMIT, g_taskCpup1s, CPUP_LAST_ONE_SECONDS, 1);
#endif
        OsShellCmdTaskWaterLineGet(tcbArray);

        if (lockFlag == TRUE) {
            SCHEDULER_UNLOCK(intSave);
        }

        OsShellCmdTskInfoTitle();
        OsShellCmdTskInfoData(tcbArray);

        if (backupFlag == TRUE) {
            (VOID)LOS_MemFree(m_aucSysMem1, tcbArray);
        }
    } else {
        OsTaskBackTrace(taskId);
    }

    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 OsShellCmdDumpTask(INT32 argc, const CHAR **argv)
{
    size_t taskId;
    CHAR *endPtr = NULL;

    if (argc < 2) { /* 2:Just as number of parameters */
        if (argc == 0) {
            taskId = OS_ALL_TASK_MASK;
        } else {
            taskId = strtoul(argv[0], &endPtr, 0);
            if ((*endPtr != 0) || (taskId >= g_taskMaxNum)) {
                PRINTK("\ntask ID can't access %s.\n", argv[0]);
                return OS_ERROR;
            }
        }

        return OsShellCmdTskInfoGet((UINT32)taskId);
    } else {
        PRINTK("\nUsage: task or task ID\n");
        return OS_ERROR;
    }
}

#ifdef LOSCFG_SHELL
SHELLCMD_ENTRY(task_shellcmd, CMD_TYPE_EX, "task", 1, (CmdCallBackFunc)OsShellCmdDumpTask);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
