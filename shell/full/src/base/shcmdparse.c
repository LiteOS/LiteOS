/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: LiteOS Shell Command Parse Implementation File
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

#include "los_memory.h"
#include "shcmd.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * Filter out double quote or single-quoted strings at both ends
 */
LITE_OS_SEC_TEXT_MINOR CHAR *OsCmdParseStrdup(const CHAR *str)
{
    CHAR *tempStr = NULL;
    CHAR *newStr = NULL;

    newStr = (CHAR *)LOS_MemAlloc(m_aucSysMem0, strlen(str) + 1);
    if (newStr == NULL) {
        return NULL;
    }

    tempStr = newStr;
    for (; *str != '\0'; str++) {
        if ((*str == '\"') || (*str == '\'')) {
            continue;
        }
        *newStr = *str;
        newStr++;
    }
    *newStr = '\0';
    return tempStr;
}

LITE_OS_SEC_TEXT_MINOR UINT32 OsCmdParseParaGet(CHAR **value, const CHAR *paraTokenStr)
{
    if ((paraTokenStr == NULL) || (value == NULL)) {
        return (UINT32)OS_ERROR;
    }
    *value = OsCmdParseStrdup(paraTokenStr);
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 OsCmdParseOneToken(CmdParsed *cmdParsed, UINT32 index, const CHAR *token)
{
    UINT32 ret = LOS_OK;
    UINT32 tempLen;

    if (cmdParsed == NULL) {
        return (UINT32)OS_ERROR;
    }

    if (index == 0) {
        if (cmdParsed->cmdType != CMD_TYPE_STD) {
            return ret;
        }
    }

    if ((token != NULL) && (cmdParsed->paramCnt < CMD_MAX_PARAS)) {
        tempLen = cmdParsed->paramCnt;
        ret = OsCmdParseParaGet(&(cmdParsed->paramArray[tempLen]), token);
        if (ret != LOS_OK) {
            return ret;
        }
        cmdParsed->paramCnt++;
    }
    return ret;
}

LITE_OS_SEC_TEXT_MINOR UINT32 OsCmdTokenSplit(CHAR *cmdStr, CHAR split, CmdParsed *cmdParsed)
{
    enum {
        STAT_INIT,
        STAT_TOKEN_IN,
        STAT_TOKEN_OUT
    } state = STAT_INIT;
    UINT32 count = 0;
    CHAR *p = NULL;
    CHAR *token = cmdStr;
    UINT32 ret = LOS_OK;
    BOOL quotes = FALSE;

    if (cmdStr == NULL) {
        return (UINT32)OS_ERROR;
    }

    for (p = cmdStr; (*p != '\0') && (ret == LOS_OK); p++) {
        if (*p == '\"') {
            SWITCH_QUOTES_STATUS(quotes);
        }
        switch (state) {
            case STAT_INIT:
            case STAT_TOKEN_IN:
                if ((*p == split) && QUOTES_STATUS_CLOSE(quotes)) {
                    *p = '\0';
                    ret = OsCmdParseOneToken(cmdParsed, count++, token);
                    state = STAT_TOKEN_OUT;
                }
                break;
            case STAT_TOKEN_OUT:
                if (*p != split) {
                    token = p;
                    state = STAT_TOKEN_IN;
                }
                break;
            default:
                break;
        }
    }

    if (((ret == LOS_OK) && (state == STAT_TOKEN_IN)) || (state == STAT_INIT)) {
        ret = OsCmdParseOneToken(cmdParsed, count, token);
    }

    return ret;
}

LITE_OS_SEC_TEXT_MINOR UINT32 OsCmdParse(CHAR *cmdStr, CmdParsed *cmdParsed)
{
    if ((cmdStr == NULL) || (cmdParsed == NULL) || (strlen(cmdStr) == 0)) {
        return (UINT32)OS_ERROR;
    }
    return OsCmdTokenSplit(cmdStr, ' ', cmdParsed);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif