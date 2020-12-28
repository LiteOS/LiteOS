/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: LiteOS Shell Command Parse Implementation Headfile
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

#ifndef _HWLITEOS_SHELL_SHCMDPARSE_H
#define _HWLITEOS_SHELL_SHCMDPARSE_H

#include "string.h"
#include "show.h"
#include "los_base.h"

#ifdef  __cplusplus
#if  __cplusplus
extern "C" {
#endif
#endif

#define CMD_PARSED_RETCODE_BASE            LOS_OK
#define CMD_PARSED_RETCODE_TYPE_INVALID    (CMD_PARSED_RETCODE_BASE + 1)
#define CMD_PARSED_RETCODE_PARAM_OVERTOP   (CMD_PARSED_RETCODE_BASE + 3)
#define CMD_PARSED_RETCODE_CMDKEY_NOTFOUND (CMD_PARSED_RETCODE_BASE + 4)

typedef UINT32 (*FUNC_ONE_TOKEN)(VOID *ctx, UINT32 index, CHAR *token);

/*
 * Description: the info struct after cmd parser
 */
typedef struct {
    UINT32 paramCnt;                /* count of para */
    CmdType cmdType;                /* cmd type, judge cmd keyword */
    CHAR cmdKeyword[CMD_KEY_LEN];   /* cmd keyword str */
    CHAR *paramArray[CMD_MAX_PARAS];
} CmdParsed;

extern UINT32 OsCmdParse(CHAR *cmdStr, CmdParsed *cmdParsed);
extern CHAR *OsCmdParseStrdup(const CHAR *str);
extern UINT32 OsCmdParseOneToken(CmdParsed *cmdParsed, UINT32 index, const CHAR *token);
extern UINT32 OsCmdTokenSplit(CHAR *cmdStr, CHAR split, CmdParsed *cmdParsed);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* _HWLITEOS_SHELL_SHCMDPARSE_H */
