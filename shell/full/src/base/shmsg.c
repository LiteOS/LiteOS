/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: LiteOS Shell Message Implementation File
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

#include "shmsg.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "securec.h"
#include "los_base.h"
#include "los_task.h"
#include "los_event.h"
#include "los_list.h"
#include "los_printf.h"
#include "shell_pri.h"
#include "shcmd.h"

#ifdef LOSCFG_FS_VFS
#include "console.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define SHELL_CMD_PARSE_EVENT   0x111
#define SHELL_CMD_EXIT_EVENT    0x112
#define SHELL_CMD_EVENT_MASK    0xFFF

#ifdef LOSCFG_ARCH_ARM_CORTEX_M
#define SHELL_TASK_STACK_SIZE   0x1000
#define SHELL_ENTRY_STACK_SIZE  0x600
#else
#define SHELL_TASK_STACK_SIZE   0x3000
#define SHELL_ENTRY_STACK_SIZE  0x1000
#endif

#define SHELL_TASK_PRIORITY     9
#define SHELL_ENTRY_PRIORITY    9

CHAR *ShellGetInputBuf(ShellCB *shellCB)
{
    CmdKeyLink *cmdKey = shellCB->cmdKeyLink;
    CmdKeyLink *cmdNode = NULL;

    (VOID)pthread_mutex_lock(&shellCB->keyMutex);
    if ((cmdKey == NULL) || LOS_ListEmpty(&cmdKey->list)) {
        (VOID)pthread_mutex_unlock(&shellCB->keyMutex);
        return NULL;
    }

    cmdNode = LOS_DL_LIST_ENTRY(cmdKey->list.pstNext, CmdKeyLink, list);
    LOS_ListDelete(&(cmdNode->list));
    (VOID)pthread_mutex_unlock(&shellCB->keyMutex);

    return cmdNode->cmdString;
}

STATIC VOID ShellSaveHistoryCmd(CHAR *string, ShellCB *shellCB)
{
    CmdKeyLink *cmdHistory = shellCB->cmdHistoryKeyLink;
    CmdKeyLink *cmdKey = LOS_DL_LIST_ENTRY(string, CmdKeyLink, cmdString);
    CmdKeyLink *cmdNxt = NULL;

    if ((string == NULL) || (strlen(string) == 0)) {
        return;
    }

    (VOID)pthread_mutex_lock(&shellCB->historyMutex);
    if (cmdHistory->count != 0) {
        cmdNxt = LOS_DL_LIST_ENTRY(cmdHistory->list.pstPrev, CmdKeyLink, list);
        if (strcmp(string, cmdNxt->cmdString) == 0) {
            (VOID)LOS_MemFree(m_aucSysMem0, (VOID *)cmdKey);
            (VOID)pthread_mutex_unlock(&shellCB->historyMutex);
            return;
        }
    }

    if (cmdHistory->count == CMD_HISTORY_LEN) {
        cmdNxt = LOS_DL_LIST_ENTRY(cmdHistory->list.pstNext, CmdKeyLink, list);
        LOS_ListDelete(&cmdNxt->list);
        LOS_ListTailInsert(&cmdHistory->list, &cmdKey->list);
        (VOID)LOS_MemFree(m_aucSysMem0, (VOID *)cmdNxt);
        (VOID)pthread_mutex_unlock(&shellCB->historyMutex);
        return;
    }

    LOS_ListTailInsert(&cmdHistory->list, &cmdKey->list);
    cmdHistory->count++;

    (VOID)pthread_mutex_unlock(&shellCB->historyMutex);
    return;
}

STATIC VOID ShellNotify(ShellCB *shellCB)
{
    (VOID)LOS_EventWrite(&shellCB->shellEvent, SHELL_CMD_PARSE_EVENT);
}

enum {
    STAT_NOMAL_KEY,
    STAT_ESC_KEY,
    STAT_MULTI_KEY
};

enum {
    CMD_KEY_ESC_VALUE   = 0x1b,
    CMD_KEY_UP_VALUE    = 0x41,
    CMD_KEY_DOWN_VALUE  = 0x42,
    CMD_KEY_RIGHT_VALUE = 0x43,
    CMD_KEY_LEFT_VALUE  = 0x44,
    CMD_KEY_COMBINATION_VALUE = 0x5b
};

STATIC INT32 ShellCmdLineCheckUDRL(const CHAR ch, ShellCB *shellCB)
{
    INT32 ret = LOS_OK;
    if (ch == CMD_KEY_ESC_VALUE) {
        shellCB->shellKeyType = STAT_ESC_KEY;
        return ret;
    } else if (ch == CMD_KEY_COMBINATION_VALUE) {
        if (shellCB->shellKeyType == STAT_ESC_KEY) {
            shellCB->shellKeyType = STAT_MULTI_KEY;
            return ret;
        }
    } else if (ch == CMD_KEY_UP_VALUE) {
        if (shellCB->shellKeyType == STAT_MULTI_KEY) {
            OsShellHistoryShow(CMD_KEY_UP, shellCB);
            shellCB->shellKeyType = STAT_NOMAL_KEY;
            return ret;
        }
    } else if (ch == CMD_KEY_DOWN_VALUE) {
        if (shellCB->shellKeyType == STAT_MULTI_KEY) {
            shellCB->shellKeyType = STAT_NOMAL_KEY;
            OsShellHistoryShow(CMD_KEY_DOWN, shellCB);
            return ret;
        }
    } else if (ch == CMD_KEY_RIGHT_VALUE) {
        if (shellCB->shellKeyType == STAT_MULTI_KEY) {
            shellCB->shellKeyType = STAT_NOMAL_KEY;
            return ret;
        }
    } else if (ch == CMD_KEY_LEFT_VALUE) {
        if (shellCB->shellKeyType == STAT_MULTI_KEY) {
            shellCB->shellKeyType = STAT_NOMAL_KEY;
            return ret;
        }
    }
    return LOS_NOK;
}

LITE_OS_SEC_TEXT_MINOR VOID ShellCmdLineParse(CHAR c, pf_OUTPUT outputFunc, ShellCB *shellCB)
{
    const CHAR ch = c;
    INT32 ret;

    if ((shellCB->shellBufOffset == 0) && (ch != '\n') && (ch != '\0')) {
        (VOID)memset_s(shellCB->shellBuf, SHOW_MAX_LEN, 0, SHOW_MAX_LEN);
    }

    if (ch == '\r') {
        if (shellCB->shellBufOffset < (SHOW_MAX_LEN - 1)) {
            shellCB->shellBuf[shellCB->shellBufOffset] = '\0';
        }
        shellCB->shellBufOffset = 0;
        (VOID)pthread_mutex_lock(&shellCB->keyMutex);
        OsShellCmdPush(shellCB->shellBuf, shellCB->cmdKeyLink);
        (VOID)pthread_mutex_unlock(&shellCB->keyMutex);
        ShellNotify(shellCB);
        return;
    } else if ((ch == '\b') || (ch == 0x7F)) { /* backspace or delete(0x7F) */
        if ((shellCB->shellBufOffset > 0) && (shellCB->shellBufOffset < (SHOW_MAX_LEN - 1))) {
            shellCB->shellBuf[shellCB->shellBufOffset - 1] = '\0';
            shellCB->shellBufOffset--;
            outputFunc("\b \b");
        }
        return;
    } else if (ch == 0x09) { /* 0x09: tab */
        if ((shellCB->shellBufOffset > 0) && (shellCB->shellBufOffset < (SHOW_MAX_LEN - 1))) {
            ret = OsTabCompletion(shellCB->shellBuf, &shellCB->shellBufOffset);
            if (ret > 1) {
                outputFunc("Huawei LiteOS # %s", shellCB->shellBuf);
            }
        }
        return;
    }
    /* parse the up/down/right/left key */
    ret = ShellCmdLineCheckUDRL(ch, shellCB);
    if (ret == LOS_OK) {
        return;
    }

    if ((ch != '\n') && (ch != '\0')) {
        if (shellCB->shellBufOffset < (SHOW_MAX_LEN - 1)) {
            shellCB->shellBuf[shellCB->shellBufOffset] = ch;
        } else {
            shellCB->shellBuf[SHOW_MAX_LEN - 1] = '\0';
        }
        shellCB->shellBufOffset++;
        outputFunc("%c", ch);
    }

    shellCB->shellKeyType = STAT_NOMAL_KEY;
}

LITE_OS_SEC_TEXT_MINOR UINT32 ShellMsgTypeGet(CmdParsed *cmdParsed, const CHAR *cmdType)
{
    UINT32 minLen;
    CmdItemNode *curCmdItem = (CmdItemNode *)NULL;
    UINT32 len = strlen(cmdType);
    CmdModInfo *cmdInfo = OsCmdInfoGet();

    LOS_DL_LIST_FOR_EACH_ENTRY(curCmdItem, &cmdInfo->cmdList.list, CmdItemNode, list) {
        if ((len == strlen(curCmdItem->cmd->cmdKey)) &&
            (strncmp((CHAR *)(curCmdItem->cmd->cmdKey), cmdType, len) == 0)) {
            minLen = (len < CMD_KEY_LEN) ? len : (CMD_KEY_LEN - 1);
            if (strncpy_s((CHAR *)(cmdParsed->cmdKeyword), CMD_KEY_LEN, cmdType, minLen) != EOK) {
                PRINTK("strncpy_s failure in %s\n", __FUNCTION__);
            }
            cmdParsed->cmdType = curCmdItem->cmd->cmdType;
            return LOS_OK;
        }
    }

    return OS_INVALID;
}

STATIC UINT32 ShellMsgNameGetAndExec(CmdParsed *cmdParsed, const CHAR *output, UINT32 len)
{
    UINT32 loop;
    UINT32 ret;
    const CHAR *tmpStr = NULL;
    BOOL quotes = FALSE;
    CHAR *msgName = (CHAR *)LOS_MemAlloc(m_aucSysMem0, len + 1);
    if (msgName == NULL) {
        PRINTK("malloc failure in %s[%d]\n", __FUNCTION__, __LINE__);
        return OS_INVALID;
    }
    /* Scan the 'output' string for command */
    /* Notice: Command string must not have any special name */
    for (tmpStr = output, loop = 0; (*tmpStr != '\0') && (loop < len);) {
        /* If reach a double quotes, switch the quotes matching status */
        if (*tmpStr == '\"') {
            SWITCH_QUOTES_STATUS(quotes);
            /* Ignore the double quote CHARactor itself */
            tmpStr++;
            continue;
        }
        /* If detected a space which the quotes matching status is false */
        /* which said has detected the first space for separator, finish this scan operation */
        if ((*tmpStr == ' ') && (QUOTES_STATUS_CLOSE(quotes))) {
            break;
        }
        msgName[loop] = *tmpStr++;
        loop++;
    }
    msgName[loop] = '\0';
    /* Scan the command list to check whether the command can be found */
    ret = ShellMsgTypeGet(cmdParsed, msgName);
    PRINTK("\n");
    if (ret != LOS_OK) {
        PRINTK("%s:command not found", msgName);
    } else {
        (VOID)OsCmdExec(cmdParsed, (CHAR *)output);
    }
    (VOID)LOS_MemFree(m_aucSysMem0, msgName);
    return ret;
}

LITE_OS_SEC_TEXT_MINOR UINT32 ShellMsgParse(const VOID *msg)
{
    CHAR *output = NULL;
    UINT32 len;
    CmdParsed cmdParsed;
    UINT32 ret = OS_INVALID;

    if (msg == NULL) {
        goto END;
    }

    len = strlen(msg);
    output = (CHAR *)LOS_MemAlloc(m_aucSysMem0, len + 1);
    if (output == NULL) {
        PRINTK("malloc failure in %s[%d]\n", __FUNCTION__, __LINE__);
        goto END;
    }
    /* Call function 'OsCmdKeyShift' to squeeze and clear useless or overmuch space if string buffer */
    ret = OsCmdKeyShift((CHAR *)msg, output, len + 1);
    if ((ret != LOS_OK) || (strlen(output) == 0)) {
        ret = OS_INVALID;
        goto END_FREE_OUTPUT;
    }

    (VOID)memset_s(&cmdParsed, sizeof(CmdParsed), 0, sizeof(CmdParsed));

    ret = ShellMsgNameGetAndExec(&cmdParsed, output, len);

END_FREE_OUTPUT:
    (VOID)LOS_MemFree(m_aucSysMem0, output);
END:
    return ret;
}

LITE_OS_SEC_TEXT_MINOR UINT32 ShellEntry(VOID *param)
{
    ShellCB *shellCB = (ShellCB *)param;

    (VOID)memset_s(shellCB->shellBuf, SHOW_MAX_LEN, 0, SHOW_MAX_LEN);
    return ShellStdinLoop(shellCB);
}

STATIC VOID ShellCmdProcess(ShellCB *shellCB)
{
    CHAR *buf = NULL;
    while (1) {
        buf = ShellGetInputBuf(shellCB);
        if (buf == NULL) {
            break;
        }
        (VOID)ShellMsgParse(buf);
        ShellSaveHistoryCmd(buf, shellCB);
        shellCB->cmdMaskKeyLink = shellCB->cmdHistoryKeyLink;
    }
}

LITE_OS_SEC_TEXT_MINOR UINT32 ShellTask(VOID *param)
{
    UINT32 ret;
    ShellCB *shellCB = (ShellCB *)param;

    while (1) {
        PRINTK("\nHuawei LiteOS # ");
        ret = LOS_EventRead(&shellCB->shellEvent, SHELL_CMD_EVENT_MASK,
                            LOS_WAITMODE_OR | LOS_WAITMODE_CLR, LOS_WAIT_FOREVER);
        if (ret == SHELL_CMD_PARSE_EVENT) {
            ShellCmdProcess(shellCB);
        } else if (ret == SHELL_CMD_EXIT_EVENT) {
            break;
        }
    }
    OsShellKeyDeInit(shellCB);
    (VOID)LOS_EventDestroy(&shellCB->shellEvent);
    (VOID)LOS_MemFree((VOID *)m_aucSysMem0, shellCB);
    return 0;
}

#define SERIAL_SHELL_TASK_NAME "SerialShellTask"
#define SERIAL_ENTRY_TASK_NAME "SerialEntryTask"
#define TELNET_SHELL_TASK_NAME "TelnetShellTask"
#define TELNET_ENTRY_TASK_NAME "TelnetEntryTask"

LITE_OS_SEC_TEXT_MINOR UINT32 ShellTaskInit(ShellCB *shellCB)
{
    CHAR *name = NULL;
    TSK_INIT_PARAM_S initParam = {0};

    if (shellCB == NULL) {
        return LOS_NOK;
    }

#ifdef LOSCFG_SHELL_CONSOLE
    if (shellCB->consoleID == CONSOLE_SERIAL) {
        name = SERIAL_SHELL_TASK_NAME;
    } else if (shellCB->consoleID == CONSOLE_TELNET) {
        name = TELNET_SHELL_TASK_NAME;
    } else {
        return LOS_NOK;
    }
#else
    /* in non-console mode, shell only support one input */
    name = SERIAL_SHELL_TASK_NAME;
#endif

    initParam.pfnTaskEntry = (TSK_ENTRY_FUNC)ShellTask;
    initParam.usTaskPrio   = SHELL_TASK_PRIORITY;
    initParam.uwStackSize  = SHELL_TASK_STACK_SIZE;
    initParam.pcName       = name;
    initParam.uwResved     = LOS_TASK_STATUS_DETACHED;
    LOS_TASK_PARAM_INIT_ARG(initParam, shellCB);

    (VOID)LOS_EventInit(&shellCB->shellEvent);

    return LOS_TaskCreate(&shellCB->shellTaskHandle, &initParam);
}

LITE_OS_SEC_TEXT_MINOR VOID ShellTaskDeinit(ShellCB *shellCB)
{
    (VOID)LOS_TaskDelete(shellCB->shellEntryHandle);
    (VOID)LOS_EventWrite(&shellCB->shellEvent, SHELL_CMD_EXIT_EVENT);
}

#ifdef LOSCFG_EXC_INTERACTION
BOOL IsShellTask(UINT32 taskId)
{
    LosTaskCB *taskCB = OS_TCB_FROM_TID(taskId);

    if ((taskCB->taskEntry == (TSK_ENTRY_FUNC)ShellTask) ||
        (taskCB->taskEntry == (TSK_ENTRY_FUNC)ShellEntry)) {
        return TRUE;
    }
    return FALSE;
}
#endif

LITE_OS_SEC_TEXT_MINOR UINT32 ShellEntryInit(ShellCB *shellCB)
{
    UINT32 ret;
    CHAR *name = NULL;
    TSK_INIT_PARAM_S initParam = {0};

    if (shellCB == NULL) {
        return LOS_NOK;
    }

#ifdef LOSCFG_SHELL_CONSOLE
    if (shellCB->consoleID == CONSOLE_SERIAL) {
        name = SERIAL_ENTRY_TASK_NAME;
    } else if (shellCB->consoleID == CONSOLE_TELNET) {
        name = TELNET_ENTRY_TASK_NAME;
    } else {
        return LOS_NOK;
    }
#else
    /* in non-console mode, shell only support one input */
    name = SERIAL_ENTRY_TASK_NAME;
#endif

    initParam.pfnTaskEntry = (TSK_ENTRY_FUNC)ShellEntry;
    initParam.usTaskPrio   = SHELL_ENTRY_PRIORITY;
    initParam.uwStackSize  = SHELL_ENTRY_STACK_SIZE;
    initParam.pcName       = name;
    initParam.uwResved     = LOS_TASK_STATUS_DETACHED;
    LOS_TASK_PARAM_INIT_ARG(initParam, shellCB);

    ret = LOS_TaskCreate(&shellCB->shellEntryHandle, &initParam);

#ifdef LOSCFG_SHELL_CONSOLE
    ret = ConsoleTaskReg((INT32)shellCB->consoleID, shellCB->shellEntryHandle);
#endif

    return ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
