/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: LiteOS Shell Help Implementation File
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

#include "shcmd.h"
#include "shell_pri.h"

UINT32 OsShellCmdHelp(UINT32 argc, const CHAR **argv)
{
    UINT32 loop = 0;
    CmdItemNode *curCmdItem = NULL;
    CmdModInfo *cmdInfo = OsCmdInfoGet();

    (VOID)argv;
    if (argc > 0) {
        PRINTK("\nUsage: help\n");
        return OS_ERROR;
    }

    PRINTK("*******************shell commands:*************************\n");
    LOS_DL_LIST_FOR_EACH_ENTRY(curCmdItem, &cmdInfo->cmdList.list, CmdItemNode, list) {
        if ((loop & (8 - 1)) == 0) { /* 8 - 1:just align print */
            PRINTK("\n");
        }
        PRINTK("%-12s  ", curCmdItem->cmd->cmdKey);

        loop++;
    }

    PRINTK("\n");
    return 0;
}

SHELLCMD_ENTRY(help_shellcmd, CMD_TYPE_EX, "help", 0, (CmdCallBackFunc)OsShellCmdHelp);
