/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: MagicKey Implementation
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

#include "los_magickey.h"
#include "los_task_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#ifdef LOSCFG_ENABLE_MAGICKEY

#define MAGIC_KEY_NUM 5

STATIC VOID OsMagicHelp(VOID);
STATIC VOID OsMagicTaskShow(VOID);
STATIC VOID OsMagicPanic(VOID);
STATIC VOID OsMagicMemCheck(VOID);

STATIC MagicKeyOp g_magicMemCheckOp = {
    .opHandler = OsMagicMemCheck,
    .helpMsg = "Check system memory(ctrl+e) ",
    .magicKey = 0x05 /* ctrl + e */
};

STATIC MagicKeyOp g_magicPanicOp = {
    .opHandler = OsMagicPanic,
    .helpMsg = "System panic(ctrl+p) ",
    .magicKey = 0x10 /* ctrl + p */
};

STATIC MagicKeyOp g_magicTaskShowOp = {
    .opHandler = OsMagicTaskShow,
    .helpMsg = "Show task information(ctrl+t) ",
    .magicKey = 0x14 /* ctrl + t */
};

STATIC MagicKeyOp g_magicHelpOp = {
    .opHandler = OsMagicHelp,
    .helpMsg = "Show all magic op key(ctrl+z) ",
    .magicKey = 0x1a /* ctrl + z */
};

/*
 * NOTICE:Suggest don't use
 * ctrl+h/backspace=0x8,
 * ctrl+i/tab=0x9,
 * ctrl+m/enter=0xd,
 * ctrl+n/shift out=0xe,
 * ctrl+o/shift in=0xf,
 * ctrl+[/esc=0x1b,
 * ctrl+] used for telnet commond mode;
 */
STATIC MagicKeyOp *g_magicOpTable[MAGIC_KEY_NUM] = {
    &g_magicMemCheckOp, /* ctrl + e */
    &g_magicPanicOp,    /* ctrl + p */
    &g_magicTaskShowOp, /* ctrl + t */
    &g_magicHelpOp,     /* ctrl + z */
    NULL                /* rserved */
};

STATIC VOID OsMagicHelp(VOID)
{
    INT32 i;
    PRINTK("HELP: ");
    for (i = 0; g_magicOpTable[i] != NULL; ++i) {
        PRINTK("%s ", g_magicOpTable[i]->helpMsg);
    }
    PRINTK("\n");
    return;
}

STATIC VOID OsMagicTaskShow(VOID)
{
    (VOID)OsShellCmdDumpTask(0, NULL);
    return;
}

STATIC VOID OsMagicPanic(VOID)
{
    LOS_Panic("Magic key :\n");
    return;
}

STATIC VOID OsMagicMemCheck(VOID)
{
    if (LOS_MemIntegrityCheck(m_aucSysMem1) == LOS_OK) {
        PRINTK("system memcheck over, all passed!\n");
    }
    return;
}
#endif

INT32 CheckMagicKey(CHAR key)
{
#ifdef LOSCFG_ENABLE_MAGICKEY
    INT32 i;
    STATIC UINT32 magicKeySwitch = 0;
    if (key == 0x12) { /* ctrl + r */
        magicKeySwitch = ~magicKeySwitch;
        if (magicKeySwitch != 0) {
            PRINTK("Magic key on\n");
        } else {
            PRINTK("Magic key off\n");
        }
        return 1;
    }
    if (magicKeySwitch != 0) {
        for (i = 0; g_magicOpTable[i] != NULL; ++i) {
            if (key == g_magicOpTable[i]->magicKey) {
                (g_magicOpTable[i])->opHandler();
                return 1;
            }
        }
    }
#else
    (VOID)key;
#endif
    return 0;
}
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
