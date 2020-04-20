/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Hwi Shell Implementation
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

#include "los_config.h"
#ifdef LOSCFG_SHELL
#ifdef LOSCFG_CPUP_INCLUDE_IRQ
#include "los_cpup_pri.h"
#endif
#include "los_hwi_pri.h"
#include "shcmd.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_CPUP_INCLUDE_IRQ
STATIC CPUP_INFO_S g_hwiCpupAll[OS_HWI_MAX_NUM];
STATIC CPUP_INFO_S g_hwiCpup10s[OS_HWI_MAX_NUM];
STATIC CPUP_INFO_S g_hwiCpup1s[OS_HWI_MAX_NUM];
LITE_OS_SEC_TEXT_MINOR UINT32 OsShellCmdHwi(INT32 argc, const CHAR **argv)
{
    UINT32 i;
    UINT32 intSave;
    UINT64 cycles;
    size_t size = sizeof(CPUP_INFO_S) * OS_HWI_MAX_NUM;

    (VOID)argv;
    if (argc > 0) {
        PRINTK("\nUsage: hwi\n");
        return OS_ERROR;
    }

    (VOID)memset_s(g_hwiCpupAll, size, 0, size);
    (VOID)memset_s(g_hwiCpup10s, size, 0, size);
    (VOID)memset_s(g_hwiCpup1s, size, 0, size);

    intSave = LOS_IntLock();
    (VOID)LOS_AllCpuUsage(OS_HWI_MAX_NUM, g_hwiCpupAll, CPUP_ALL_TIME, 0);
    (VOID)LOS_AllCpuUsage(OS_HWI_MAX_NUM, g_hwiCpup10s, CPUP_LAST_TEN_SECONDS, 0);
    (VOID)LOS_AllCpuUsage(OS_HWI_MAX_NUM, g_hwiCpup1s, CPUP_LAST_ONE_SECONDS, 0);
    LOS_IntRestore(intSave);

    PRINTK(" InterruptNo     Count     Name         CYCLECOST  CPUUSE   CPUUSE10s   CPUUSE1s   mode\n");
    for (i = OS_HWI_FORM_EXC_NUM; i < OS_HWI_MAX_NUM + OS_HWI_FORM_EXC_NUM; i++) {
        if (OsGetHwiFormCnt(i)) {
            cycles = g_cpup[g_taskMaxNum + i].allTime / OsGetHwiFormCnt(i);
        } else {
            cycles = 0;
        }
        /* Different cores has different hwi form implementation */
        if (HWI_IS_REGISTED(i) && (OsGetHwiFormName(i) != NULL)) {
            PRINTK(" %8d:       %-10d%-12s  %-10llu", i, OsGetHwiFormCnt(i), OsGetHwiFormName(i), cycles);
        } else if (HWI_IS_REGISTED(i)) {
            PRINTK(" %8d:       %-10d%-12s  %-10llu", i, OsGetHwiFormCnt(i), "", cycles);
        } else {
            continue;
        }
        PRINTK("%2d.%-7d"
               "%2d.%-9d"
               "%2d.%-6d"
               "%s\n",
               g_hwiCpupAll[i].uwUsage / LOS_CPUP_PRECISION_MULT,
               g_hwiCpupAll[i].uwUsage % LOS_CPUP_PRECISION_MULT,
               g_hwiCpup10s[i].uwUsage / LOS_CPUP_PRECISION_MULT,
               g_hwiCpup10s[i].uwUsage % LOS_CPUP_PRECISION_MULT,
               g_hwiCpup1s[i].uwUsage / LOS_CPUP_PRECISION_MULT,
               g_hwiCpup1s[i].uwUsage % LOS_CPUP_PRECISION_MULT,
               g_hwiForm[i].uwParam == IRQF_SHARED ? "shared" : "normal");
    }
    return 0;
}
#else
LITE_OS_SEC_TEXT_MINOR UINT32 OsShellCmdHwi(INT32 argc, const CHAR **argv)
{
    UINT32 i;

    (VOID)argv;
    if (argc > 0) {
        PRINTK("\nUsage: hwi\n");
        return OS_ERROR;
    }

    PRINTK(" InterruptNo     Count     Name\n");
    for (i = OS_HWI_FORM_EXC_NUM; i < OS_HWI_MAX_NUM + OS_HWI_FORM_EXC_NUM; i++) {
        /* Different cores has different hwi form implementation */
        if (HWI_IS_REGISTED(i) && (OsGetHwiFormName(i) != NULL)) {
            PRINTK(" %8d:%10d:      %-s\n", i, OsGetHwiFormCnt(i), OsGetHwiFormName(i));
        } else if (HWI_IS_REGISTED(i)) {
            PRINTK(" %8d:%10d:\n", i, OsGetHwiFormCnt(i));
        }
    }
    return 0;
}
#endif

SHELLCMD_ENTRY(hwi_shellcmd, CMD_TYPE_EX, "hwi", 0, (CmdCallBackFunc)OsShellCmdHwi);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif /* LOSCFG_SHELL */
