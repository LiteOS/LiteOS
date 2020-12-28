/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: LiteOS Shell Command Module Implementation Headfile
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

#ifndef _HWLITEOS_SHELL_SHCMD_H
#define _HWLITEOS_SHELL_SHCMD_H

#include "string.h"
#include "stdlib.h"
#include "los_base.h"
#include "los_list.h"
#include "los_tables.h"
#include "console.h"
#include "shcmdparse.h"
#include "show.h"

#ifdef  __cplusplus
#if  __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef BOOL (*CmdVerifyTransID)(UINT32 transId);

typedef struct {
    CmdType cmdType;
    CHAR *cmdKey;
    UINT32 paraNum;
    CmdCallBackFunc cmdHook;
} CmdItem;

typedef struct {
    LOS_DL_LIST list;
    CmdItem *cmd;
} CmdItemNode;

/* global info for shell module */
typedef struct {
    CmdItemNode cmdList;
    UINT32 listNum;
    UINT32 initMagicFlag;
    UINT32 muxLock;
    CmdVerifyTransID transIdHook;
} CmdModInfo;

typedef struct {
    UINT32 count;
    LOS_DL_LIST list;
    CHAR cmdString[0];
} CmdKeyLink;

#define SHELLCMD_ENTRY(l, cmdType, cmdKey, paraNum, cmdHook)    \
    CmdItem l LOS_HAL_TABLE_ENTRY(shellcmd) = {                 \
        cmdType,                                                \
        cmdKey,                                                 \
        paraNum,                                                \
        cmdHook                                                 \
    }

#define NEED_NEW_LINE(timesPrint, lineCap) ((timesPrint) % (lineCap) == 0)
#define SCREEN_IS_FULL(timesPrint, lineCap) ((timesPrint) >= ((lineCap) * DEFAULT_SCREEN_HEIGHT))

extern UINT32 OsCmdInit(VOID);
extern CmdModInfo *OsCmdInfoGet(VOID);
extern UINT32 OsCmdExec(CmdParsed *cmdParsed, CHAR *cmdStr);
extern UINT32 OsCmdKeyShift(const CHAR *cmdKey, CHAR *cmdOut, UINT32 size);
extern INT32 OsTabCompletion(CHAR *cmdKey, UINT32 *len);
extern VOID OsShellCmdPush(const CHAR *string, CmdKeyLink *cmdKeyLink);
extern VOID OsShellHistoryShow(UINT32 value, ShellCB *shellCB);
extern UINT32 OsShellKeyInit(ShellCB *shellCB);
extern VOID OsShellKeyDeInit(const ShellCB *shellCB);
extern VOID OsShellKeyLinkDeInit(CmdKeyLink *cmdKeyLink);
extern UINT32 OsShellSysCmdRegister(VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* _HWLITEOS_SHELL_SHCMD_H */
