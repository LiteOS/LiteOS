/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: LiteOS Shell Show Implementation File
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

#include "show.h"
#include "shmsg.h"
#include "shcmd.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

STATIC BOOL g_shellSourceFlag = FALSE;
#ifndef LOSCFG_SHELL_CONSOLE
/* If not using console, then allocated shell struct will store in this global variable */
STATIC ShellCB *g_shellCB = NULL;
#endif

STATIC UINT32 OsShellSourceInit(VOID)
{
    UINT32 ret;

    if (g_shellSourceFlag) {
        return LOS_OK;
    }

    ret = OsCmdInit();
    if (ret != LOS_OK) {
        return ret;
    }

    ret = OsShellSysCmdRegister();
    if (ret != LOS_OK) {
        return ret;
    }
    g_shellSourceFlag = TRUE;
    return LOS_OK;
}

STATIC UINT32 OsShellCreateTask(ShellCB *shellCB)
{
    UINT32 ret = ShellTaskInit(shellCB);
    if (ret != LOS_OK) {
        return ret;
    }

    ret = ShellEntryInit(shellCB);
    if (ret != LOS_OK) {
        (VOID)LOS_TaskDelete(shellCB->shellTaskHandle);
    }
    return ret;
}

UINT32 OsShellInit(INT32 consoleId)
{
    UINT32 ret;
    ShellCB *shellCB = NULL;

    ret = OsShellSourceInit();
    if (ret != LOS_OK) {
        return ret;
    }

    shellCB = LOS_MemAlloc(m_aucSysMem0, sizeof(ShellCB));
    if (shellCB == NULL) {
        return LOS_NOK;
    }

    (VOID)memset_s(shellCB, sizeof(ShellCB), 0, sizeof(ShellCB));

#ifdef LOSCFG_SHELL_CONSOLE
    CONSOLE_CB *consoleCB = OsGetConsoleByID(consoleId);
    if ((consoleCB == NULL) || (consoleCB->shellHandle != NULL)) {
        (VOID)LOS_MemFree(m_aucSysMem0, shellCB);
        return LOS_NOK;
    }

    consoleCB->shellHandle = shellCB;
    shellCB->consoleID = (UINT32)consoleId;
#else
    g_shellCB = shellCB;
#endif

    (VOID)pthread_mutex_init(&shellCB->keyMutex, NULL);
    (VOID)pthread_mutex_init(&shellCB->historyMutex, NULL);

    if (strncpy_s(shellCB->shellWorkingDirectory, PATH_MAX, "/", 1) != EOK) {
        ret = LOS_NOK;
        goto ERR_OUT1;
    }

    ret = OsShellKeyInit(shellCB);
    if (ret != LOS_OK) {
        goto ERR_OUT1;
    }

    ret = OsShellCreateTask(shellCB);
    if (ret != LOS_OK) {
        goto ERR_OUT2;
    }

    return LOS_OK;
ERR_OUT2:
    OsShellKeyDeInit(shellCB);
ERR_OUT1:
    (VOID)pthread_mutex_destroy(&shellCB->historyMutex);
    (VOID)pthread_mutex_destroy(&shellCB->keyMutex);
    (VOID)LOS_MemFree((VOID *)m_aucSysMem0, shellCB);
#ifdef LOSCFG_SHELL_CONSOLE
    consoleCB->shellHandle = NULL;
#endif
    return ret;
}

INT32 OsShellDeinit(INT32 consoleId)
{
    ShellCB *shellCB = NULL;

#ifdef LOSCFG_SHELL_CONSOLE
    CONSOLE_CB *consoleCB = OsGetConsoleByID(consoleId);
    if (consoleCB == NULL) {
        PRINT_ERR("shell deinit error.\n");
        return -1;
    }

    shellCB = (ShellCB *)consoleCB->shellHandle;
    consoleCB->shellHandle = NULL;
#else
    shellCB = g_shellCB;
#endif
    ShellTaskDeinit(shellCB);

    return 0;
}

CHAR *OsShellGetWorkingDirectory(VOID)
{
    ShellCB *shellCB = NULL;

#ifdef LOSCFG_SHELL_CONSOLE
    CONSOLE_CB *consoleCB = OsGetConsoleByTaskID(OsCurrTaskGet()->taskId);
    if (consoleCB == NULL) {
        return NULL;
    }

    shellCB = (ShellCB *)consoleCB->shellHandle;
    if (shellCB == NULL) {
        return NULL;
    }
    return shellCB->shellWorkingDirectory;
#else
    shellCB = g_shellCB;
    return shellCB->shellWorkingDirectory;
#endif
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
