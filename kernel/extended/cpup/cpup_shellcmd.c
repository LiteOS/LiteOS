/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: ShellCmd Cpup
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

#include "los_config.h"
#ifdef LOSCFG_SHELL
#include "stdio.h"
#include "stdlib.h"
#include "los_cpup_pri.h"
#include "los_task_pri.h"
#include "shcmd.h"
#include "shell.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

VOID OsCmdCpupOperateOneParam(UINT32 mode)
{
    UINT32 ret;

    if (mode == CPUP_LAST_TEN_SECONDS) {
        PRINTK("\nSysCpuUsage in 10s: ");
    } else if (mode == CPUP_LAST_ONE_SECONDS) {
        PRINTK("\nSysCpuUsage in 1s: ");
    } else {
        PRINTK("\nSysCpuUsage in all time: ");
    }
    ret = LOS_HistorySysCpuUsage(mode);
    PRINTK("%u.%u", ret / LOS_CPUP_PRECISION_MULT, ret % LOS_CPUP_PRECISION_MULT);
}

VOID OsCmdCpupOperateTwoParam(UINT32 mode, UINT32 taskId)
{
    UINT32 ret;

    if (mode == CPUP_LAST_TEN_SECONDS) {
        PRINTK("\nTaskId %u CpuUsage in 10s: ", taskId);
    } else if (mode == CPUP_LAST_ONE_SECONDS) {
        PRINTK("\nTaskId %u CpuUsage in 1s: ", taskId);
    } else {
        PRINTK("\nTaskId %u CpuUsage in all time: ", taskId);
    }
    ret = LOS_HistoryTaskCpuUsage(taskId, mode);
    PRINTK("%u.%u", ret / LOS_CPUP_PRECISION_MULT, ret % LOS_CPUP_PRECISION_MULT);
}

LITE_OS_SEC_TEXT_MINOR UINT32 OsShellCmdCpup(INT32 argc, const CHAR **argv)
{
    size_t mode, taskId;
    CHAR *bufMode = NULL;
    CHAR *bufId = NULL;
    LosTaskCB *taskCB = NULL;
    UINT32 ret;

    if (argc <= 0) {
        ret = LOS_HistorySysCpuUsage(CPUP_LAST_TEN_SECONDS);
        PRINTK("\nSysCpuUsage in 10s: %u.%u", ret / LOS_CPUP_PRECISION_MULT, ret % LOS_CPUP_PRECISION_MULT);
        return 0;
    }

    mode = strtoul(argv[0], &bufMode, 0);
    if ((bufMode == NULL) || (*bufMode != 0)) {
        PRINTK("\nThe input mode is invalid. Please try again.\n");
        return 0;
    }

    if (mode > CPUP_ALL_TIME) {
        mode = CPUP_ALL_TIME;
    }

    if (argc == 1) {
        OsCmdCpupOperateOneParam(mode);
        return 0;
    }

    taskId = strtoul(argv[1], &bufId, 0);
    if ((taskId >= g_taskMaxNum) || (*bufId != 0)) {
        PRINTK("\nThe input taskId is invalid. Please try again.\n");
        return 0;
    }
    taskCB = OS_TCB_FROM_TID(taskId);
    if (taskCB->taskStatus & OS_TASK_STATUS_UNUSED) {
        PRINTK("\nThe task is unused. Please try again.\n");
        return 0;
    }

    if (argc == 2) {
        OsCmdCpupOperateTwoParam(mode, taskId);
        return 0;
    }

    PRINTK("cpup [MODE] \ncpup [MODE] [TASKID] \n");
    return 0;
}

SHELLCMD_ENTRY(cpup_shellcmd, CMD_TYPE_EX, "cpup", XARGS, (CmdCallBackFunc)OsShellCmdCpup);
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif /* LOSCFG_SHELL */
