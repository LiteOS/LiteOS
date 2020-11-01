/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: LiteOS Shell Command Implementation File
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

#include "shcmd.h"
#include "stdlib.h"
#include "unistd.h"
#include "dirent.h"
#include "securec.h"
#include "los_mux.h"
#include "los_memory.h"
#include "shell_pri.h"
#include "show.h"

#ifdef  __cplusplus
#if  __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define SHELL_INIT_MAGIC_FLAG 0xABABABAB
#define CTRL_C 0x03 /* 0x03: ctrl+c ASCII */
#define SPACE  0x20 /* 0x20: space ASCII */
STATIC CmdModInfo g_cmdInfo;

LOS_HAL_TABLE_BEGIN(g_shellcmd, shellcmd);
LOS_HAL_TABLE_END(g_shellcmdEnd, shellcmd);

CmdModInfo *OsCmdInfoGet(VOID)
{
    return &g_cmdInfo;
}

STATIC VOID OsFreeCmdPara(CmdParsed *cmdParsed)
{
    UINT32 i;
    for (i = 0; i < cmdParsed->paramCnt; i++) {
        if (cmdParsed->paramArray[i] != NULL) {
            (VOID)LOS_MemFree(m_aucSysMem0, cmdParsed->paramArray[i]);
            cmdParsed->paramArray[i] = NULL;
        }
    }
}

STATIC VOID OsCompleteStr(CHAR *result, const CHAR *target, CHAR *cmdKey, UINT32 *len)
{
    UINT32 size = strlen(result) - strlen(target);
    CHAR *des = cmdKey + *len;
    CHAR *src = result + strlen(target);

    while (size-- > 0) {
        PRINTK("%c", *src);
        if (*len == (SHOW_MAX_LEN - 1)) {
            *des = '\0';
            break;
        }
        *des++ = *src++;
        (*len)++;
    }
}

STATIC INT32 OsTabMatchFile(CHAR *cmdKey, UINT32 *len)
{
    return 0;
}

STATIC INT32 OsTabMatchCmd(CHAR *cmdKey, UINT32 *len)
{
    INT32 count = 0;
    INT32 ret;
    CmdItemNode *cmdItemGuard = NULL;
    CmdItemNode *curCmdItem = NULL;
    CHAR *cmdMajor = cmdKey;

    while (*cmdMajor == SPACE) { /* cut left space */
        cmdMajor++;
    }

    if (LOS_ListEmpty(&g_cmdInfo.cmdList.list)) {
        return (INT32)OS_ERROR;
    }

    LOS_DL_LIST_FOR_EACH_ENTRY(curCmdItem, &g_cmdInfo.cmdList.list, CmdItemNode, list) {
        if (strncmp(cmdMajor, curCmdItem->cmd->cmdKey, strlen(cmdMajor)) > 0) {
            continue;
        }

        if (strncmp(cmdMajor, curCmdItem->cmd->cmdKey, strlen(cmdMajor)) != 0) {
            break;
        }

        if (count == 0) {
            cmdItemGuard = curCmdItem;
        }
        ++count;
    }

    if (cmdItemGuard == NULL) {
        return 0;
    }

    if (count == 1) {
        OsCompleteStr(cmdItemGuard->cmd->cmdKey, cmdMajor, cmdKey, len);
    }

    ret = count;
    if (count > 1) {
        PRINTK("\n");
        while (count--) {
            PRINTK("%s  ", cmdItemGuard->cmd->cmdKey);
            cmdItemGuard = LOS_DL_LIST_ENTRY(cmdItemGuard->list.pstNext, CmdItemNode, list);
        }
        PRINTK("\n");
    }

    return ret;
}

/*
 * Description: Pass in the string and clear useless space, which include:
 *                1) The overmatch space which is not be marked by Quote's area
 *                   Squeeze the overmatch space into one space
 *                2) Clear all space before first valid character
 * Input:       cmdKey : Pass in the buff string, which is ready to be operated
 *              cmdOut : Pass out the buffer string ,which has already been operated
 *              size : cmdKey length
 */
LITE_OS_SEC_TEXT_MINOR UINT32 OsCmdKeyShift(const CHAR *cmdKey, CHAR *cmdOut, UINT32 size)
{
    CHAR *output = NULL;
    CHAR *outputBak = NULL;
    UINT32 len;
    INT32 ret;
    BOOL quotes = FALSE;

    if ((cmdKey == NULL) || (cmdOut == NULL)) {
        return OS_ERROR;
    }

    len = strlen(cmdKey);
    if (len >= size) {
        return OS_ERROR;
    }
    output = (CHAR*)LOS_MemAlloc(m_aucSysMem0, len + 1);
    if (output == NULL) {
        PRINTK("malloc failure in %s[%d]", __FUNCTION__, __LINE__);
        return OS_ERROR;
    }
    /* Backup the 'output' start address */
    outputBak = output;
    /* Scan each character in 'cmdKey',and squeeze the overmuch space and ignore Invalid character */
    for (; *cmdKey != '\0'; cmdKey++) {
        /* Detected a Double Quotes, switch the matching status */
        if (*cmdKey == '\"') {
            SWITCH_QUOTES_STATUS(quotes);
        }
        /* Ignore the current character in following situation */
        /* 1) Quotes matching status is FALSE (which said that the space is not been marked by double quotes) */
        /* 2) Current character is a space */
        /* 3) Next character is a space too, or the string is been seeked to the end already(\0) */
        /* 4) Invalid character, such as single quotes */
        if ((*cmdKey == ' ') && ((*(cmdKey + 1) == ' ') || (*(cmdKey + 1) == '\0')) && QUOTES_STATUS_CLOSE(quotes)) {
            continue;
        }
        if (*cmdKey == '\'') {
            continue;
        }
        *output = *cmdKey;
        output++;
    }
    *output = '\0';
    /* Restore the 'output' start address */
    output = outputBak;
    len = strlen(output);
    /* Clear the space which is located at the first character in buffer */
    if (*output == ' ') {
        output++;
        len--;
    }
    /* Copy out the buffer which is been operated already */
    ret = strncpy_s(cmdOut, size, output, len);
    if (ret != EOK) {
        PRINT_ERR("%s,%d strncpy_s failed, err:%d!\n", __FUNCTION__, __LINE__, ret);
        (VOID)LOS_MemFree(m_aucSysMem0, outputBak);
        return OS_ERROR;
    }

    (VOID)LOS_MemFree(m_aucSysMem0, outputBak);

    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR BOOL OsCmdKeyCheck(CHAR *cmdKey)
{
    CHAR *temp = cmdKey;
    enum Stat {
        STAT_NONE,
        STAT_DIGIT,
        STAT_OTHER
    } state = STAT_NONE;

    if (strlen(cmdKey) >= CMD_KEY_LEN) {
        return FALSE;
    }

    while (*temp != '\0') {
        if (!((*temp <= '9') && (*temp >= '0')) &&
            !((*temp <= 'z') && (*temp >= 'a')) &&
            !((*temp <= 'Z') && (*temp >= 'A')) &&
            (*temp != '_') && (*temp != '-')) {
            return FALSE;
        }

        if ((*temp >= '0') && (*temp <= '9')) {
            if (state == STAT_NONE) {
                state = STAT_DIGIT;
            }
        } else {
            state = STAT_OTHER;
        }

        temp++;
    }

    if (state == STAT_DIGIT) {
        return FALSE;
    }

    return TRUE;
}

LITE_OS_SEC_TEXT_MINOR INT32 OsTabCompletion(CHAR *cmdKey, UINT32 *len)
{
    INT32 count = 0;
    CHAR *space = NULL;
    CHAR *cmdMainStr = cmdKey;

    if ((cmdKey == NULL) || (len == NULL)) {
        return (INT32)OS_ERROR;
    }

    /* cut left space */
    while (*cmdMainStr == SPACE) {
        cmdMainStr++;
    }

    /* try to find space in remain */
    space = strrchr(cmdMainStr, SPACE);
    if ((space == NULL) && (*cmdMainStr != '\0')) {
        count = OsTabMatchCmd(cmdKey, len);
    }

    if (count == 0) {
        count = OsTabMatchFile(cmdKey, len);
    }

    return count;
}

LITE_OS_SEC_TEXT_MINOR VOID OsCmdAscendingInsert(CmdItemNode *cmd)
{
    CmdItemNode *cmdItem = NULL;
    CmdItemNode *cmdNext = NULL;

    for (cmdItem = LOS_DL_LIST_ENTRY((&g_cmdInfo.cmdList.list)->pstPrev, CmdItemNode, list);
         &cmdItem->list != &g_cmdInfo.cmdList.list;) {
        cmdNext = LOS_DL_LIST_ENTRY(cmdItem->list.pstPrev, CmdItemNode, list);
        if (&cmdNext->list != &g_cmdInfo.cmdList.list) {
            if ((strncmp(cmdItem->cmd->cmdKey, cmd->cmd->cmdKey, strlen(cmd->cmd->cmdKey)) >= 0) &&
                (strncmp(cmdNext->cmd->cmdKey, cmd->cmd->cmdKey, strlen(cmd->cmd->cmdKey)) < 0)) {
                LOS_ListTailInsert(&cmdItem->list, &cmd->list);
                return;
            }
            cmdItem = cmdNext;
        } else {
            if (strncmp(cmd->cmd->cmdKey, cmdItem->cmd->cmdKey, strlen(cmd->cmd->cmdKey)) > 0) {
                cmdItem = cmdNext;
            }
            break;
        }
    }

    LOS_ListTailInsert(&cmdItem->list, &cmd->list);
}

LITE_OS_SEC_TEXT_MINOR UINT32 OsShellKeyInit(ShellCB *shellCB)
{
    CmdKeyLink *cmdKeyLink = NULL;
    CmdKeyLink *cmdHistoryLink = NULL;

    cmdKeyLink = (CmdKeyLink *)LOS_MemAlloc(m_aucSysMem0, sizeof(CmdKeyLink));
    if (cmdKeyLink == NULL) {
        PRINT_ERR("Shell CmdKeyLink memory alloc error!\n");
        return OS_ERROR;
    }
    cmdHistoryLink = (CmdKeyLink *)LOS_MemAlloc(m_aucSysMem0, sizeof(CmdKeyLink));
    if (cmdHistoryLink == NULL) {
        (VOID)LOS_MemFree(m_aucSysMem0, cmdKeyLink);
        PRINT_ERR("Shell CmdHistoryLink memory alloc error!\n");
        return OS_ERROR;
    }

    cmdKeyLink->count = 0;
    LOS_ListInit(&cmdKeyLink->list);
    shellCB->cmdKeyLink = (VOID *)cmdKeyLink;

    cmdHistoryLink->count = 0;
    LOS_ListInit(&cmdHistoryLink->list);
    shellCB->cmdHistoryKeyLink = (VOID *)cmdHistoryLink;
    shellCB->cmdMaskKeyLink = (VOID *)cmdHistoryLink;
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR VOID OsShellKeyLinkDeInit(CmdKeyLink *cmdKeyLink)
{
    CmdKeyLink *cmd = NULL;

    while (!LOS_ListEmpty(&cmdKeyLink->list)) {
        cmd = LOS_DL_LIST_ENTRY(cmdKeyLink->list.pstNext, CmdKeyLink, list);
        LOS_ListDelete(&cmd->list);
        (VOID)LOS_MemFree(m_aucSysMem0, cmd);
    }

    cmdKeyLink->count = 0;
    (VOID)LOS_MemFree(m_aucSysMem0, cmdKeyLink);
}

LITE_OS_SEC_TEXT_MINOR VOID OsShellKeyDeInit(ShellCB *shellCB)
{
    OsShellKeyLinkDeInit(shellCB->cmdKeyLink);
    OsShellKeyLinkDeInit(shellCB->cmdHistoryKeyLink);
}

LITE_OS_SEC_TEXT_MINOR UINT32 OsShellSysCmdRegister(VOID)
{
    UINT32 i;
    UINT8 *cmdItemGroup = NULL;
    UINT32 index = ((UINTPTR)(&g_shellcmdEnd) - (UINTPTR)(&g_shellcmd[0])) / sizeof(CmdItem);
    CmdItemNode *cmdItem = NULL;

    cmdItemGroup = (UINT8 *)LOS_MemAlloc(m_aucSysMem0, index * sizeof(CmdItemNode));

    if (cmdItemGroup == NULL) {
        PRINT_ERR("[%s]System memory allocation failure!\n", __FUNCTION__);
        return (UINT32)OS_ERROR;
    }

    for (i = 0; i < index; ++i) {
        cmdItem = (CmdItemNode *)(cmdItemGroup + i * sizeof(CmdItemNode));
        cmdItem->cmd = &g_shellcmd[i];
        OsCmdAscendingInsert(cmdItem);
    }
    g_cmdInfo.listNum += index;
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR VOID OsShellCmdPush(const CHAR *string, CmdKeyLink *cmdKeyLink)
{
    CmdKeyLink *cmdNewNode = NULL;
    UINT32 len;

    if ((string == NULL) || (strlen(string) == 0)) {
        return;
    }

    len = strlen(string);
    cmdNewNode = (CmdKeyLink *)LOS_MemAlloc(m_aucSysMem0, sizeof(CmdKeyLink) + len + 1);
    if (cmdNewNode == NULL) {
        return;
    }

    (VOID)memset_s(cmdNewNode, sizeof(CmdKeyLink) + len + 1, 0, sizeof(CmdKeyLink) + len + 1);
    if (strncpy_s(cmdNewNode->cmdString, len + 1, string, len) != EOK) {
        (VOID)LOS_MemFree(m_aucSysMem0, cmdNewNode);
        return;
    }

    LOS_ListTailInsert(&cmdKeyLink->list, &cmdNewNode->list);

    return;
}

LITE_OS_SEC_TEXT_MINOR VOID OsShellHistoryShow(UINT32 value, ShellCB *shellCB)
{
    CmdKeyLink *cmd = NULL;
    CmdKeyLink *cmdNode = shellCB->cmdHistoryKeyLink;
    CmdKeyLink *cmdMask = shellCB->cmdMaskKeyLink;
    errno_t ret;

    (VOID)pthread_mutex_lock(&shellCB->historyMutex);
    if (value == CMD_KEY_DOWN) {
        if (cmdMask == cmdNode) {
            goto END;
        }

        cmd = LOS_DL_LIST_ENTRY(cmdMask->list.pstNext, CmdKeyLink, list);
        if (cmd != cmdNode) {
            cmdMask = cmd;
        } else {
            goto END;
        }
    } else if (value == CMD_KEY_UP) {
        cmd = LOS_DL_LIST_ENTRY(cmdMask->list.pstPrev, CmdKeyLink, list);
        if (cmd != cmdNode) {
            cmdMask = cmd;
        } else {
            goto END;
        }
    }

    while (shellCB->shellBufOffset--) {
        PRINTK("\b \b");
    }
    PRINTK("%s", cmdMask->cmdString);
    shellCB->shellBufOffset = strlen(cmdMask->cmdString);
    (VOID)memset_s(shellCB->shellBuf, SHOW_MAX_LEN, 0, SHOW_MAX_LEN);
    ret = memcpy_s(shellCB->shellBuf, SHOW_MAX_LEN, cmdMask->cmdString, shellCB->shellBufOffset);
    if (ret != EOK) {
        PRINT_ERR("%s, %d memcpy failed!\n", __FUNCTION__, __LINE__);
        goto END;
    }
    shellCB->cmdMaskKeyLink = (VOID *)cmdMask;

END:
    (VOID)pthread_mutex_unlock(&shellCB->historyMutex);
    return;
}

LITE_OS_SEC_TEXT_MINOR UINT32 OsCmdExec(CmdParsed *cmdParsed, CHAR *cmdStr)
{
    UINT32 ret;
    CmdCallBackFunc cmdHook = NULL;
    CmdItemNode *curCmdItem = NULL;
    CHAR *cmdKey = NULL;

    if ((cmdStr == NULL) || (strlen(cmdStr) == 0)) {
        return (UINT32)OS_ERROR;
    }

    ret = OsCmdParse(cmdStr, cmdParsed);
    if (ret != LOS_OK) {
        return ret;
    }

    LOS_DL_LIST_FOR_EACH_ENTRY(curCmdItem, &g_cmdInfo.cmdList.list, CmdItemNode, list) {
        cmdKey = curCmdItem->cmd->cmdKey;
        if ((cmdParsed->cmdType == curCmdItem->cmd->cmdType) &&
            (strlen(cmdKey) == strlen(cmdParsed->cmdKeyword)) &&
            (strncmp(cmdKey, (CHAR *)(cmdParsed->cmdKeyword), strlen(cmdKey)) == 0)) {
            cmdHook = curCmdItem->cmd->cmdHook;
            break;
        }
    }

    ret = OS_ERROR;
    if (cmdHook != NULL) {
        ret = (cmdHook)(cmdParsed->paramCnt, (const CHAR **)cmdParsed->paramArray);
    }
    OsFreeCmdPara(cmdParsed);

    return (UINT32)ret;
}

LITE_OS_SEC_TEXT_MINOR UINT32 OsCmdInit(VOID)
{
    UINT32 ret;
    LOS_ListInit(&(g_cmdInfo.cmdList.list));
    g_cmdInfo.listNum = 0;
    g_cmdInfo.muxLock = 0;
    g_cmdInfo.initMagicFlag = SHELL_INIT_MAGIC_FLAG;
    ret = LOS_MuxCreate(&g_cmdInfo.muxLock);
    if (ret != LOS_OK) {
        PRINT_ERR("Create mutex for shell cmd info failed\n");
        return OS_ERROR;
    }
    return LOS_OK;
}

STATIC UINT32 OsCmdItemCreate(CmdType cmdType, CHAR *cmdKey, UINT32 paraNum, CmdCallBackFunc cmdProc)
{
    CmdItem *cmdItem = NULL;
    CmdItemNode *cmdItemNode = NULL;

    cmdItem = (CmdItem *)LOS_MemAlloc(m_aucSysMem0, sizeof(CmdItem));
    if (cmdItem == NULL) {
        return OS_ERRNO_SHELL_CMDREG_MEMALLOC_ERROR;
    }
    (VOID)memset_s(cmdItem, sizeof(CmdItem), '\0', sizeof(CmdItem));

    cmdItemNode = (CmdItemNode *)LOS_MemAlloc(m_aucSysMem0, sizeof(CmdItemNode));
    if (cmdItemNode == NULL) {
        (VOID)LOS_MemFree(m_aucSysMem0, cmdItem);
        return OS_ERRNO_SHELL_CMDREG_MEMALLOC_ERROR;
    }
    (VOID)memset_s(cmdItemNode, sizeof(CmdItemNode), '\0', sizeof(CmdItemNode));
    cmdItemNode->cmd = cmdItem;
    cmdItemNode->cmd->cmdHook = cmdProc;
    cmdItemNode->cmd->paraNum = paraNum;
    cmdItemNode->cmd->cmdType = cmdType;
    cmdItemNode->cmd->cmdKey = cmdKey;

    (VOID)LOS_MuxPend(g_cmdInfo.muxLock, LOS_WAIT_FOREVER);
    OsCmdAscendingInsert(cmdItemNode);
    g_cmdInfo.listNum++;
    (VOID)LOS_MuxPost(g_cmdInfo.muxLock);

    return LOS_OK;
}

/* open API */
LITE_OS_SEC_TEXT_MINOR UINT32 osCmdReg(CmdType cmdType, CHAR *cmdKey, UINT32 paraNum, CmdCallBackFunc cmdProc)
{
    CmdItemNode *cmdItemNode = NULL;
    printf("1111111111111111111111\n");

    (VOID)LOS_MuxPend(g_cmdInfo.muxLock, LOS_WAIT_FOREVER);
    if (g_cmdInfo.initMagicFlag != SHELL_INIT_MAGIC_FLAG) {
        (VOID)LOS_MuxPost(g_cmdInfo.muxLock);
        PRINT_ERR("[%s] shell is not yet initialized!\n", __FUNCTION__);
        return OS_ERRNO_SHELL_NOT_INIT;
    }
    (VOID)LOS_MuxPost(g_cmdInfo.muxLock);

    if ((cmdProc == NULL) || (cmdKey == NULL) ||
        (cmdType >= CMD_TYPE_BUTT) || (strlen(cmdKey) >= CMD_KEY_LEN) || !strlen(cmdKey)) {
        return OS_ERRNO_SHELL_CMDREG_PARA_ERROR;
    }

    if ((paraNum > CMD_MAX_PARAS) && (paraNum != XARGS)) {
        return OS_ERRNO_SHELL_CMDREG_PARA_ERROR;
    }

    if (OsCmdKeyCheck(cmdKey) != TRUE) {
        return OS_ERRNO_SHELL_CMDREG_CMD_ERROR;
    }

    (VOID)LOS_MuxPend(g_cmdInfo.muxLock, LOS_WAIT_FOREVER);
    LOS_DL_LIST_FOR_EACH_ENTRY(cmdItemNode, &g_cmdInfo.cmdList.list, CmdItemNode, list) {
        if ((cmdType == cmdItemNode->cmd->cmdType) &&
            ((strlen(cmdKey) == strlen(cmdItemNode->cmd->cmdKey)) &&
            (strncmp((CHAR *)(cmdItemNode->cmd->cmdKey), cmdKey, strlen(cmdKey)) == 0))) {
            (VOID)LOS_MuxPost(g_cmdInfo.muxLock);
            return OS_ERRNO_SHELL_CMDREG_CMD_EXIST;
        }
    }
    (VOID)LOS_MuxPost(g_cmdInfo.muxLock);
    return OsCmdItemCreate(cmdType, cmdKey, paraNum, cmdProc);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
