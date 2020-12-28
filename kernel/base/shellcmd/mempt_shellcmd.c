/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: ShellCmd Mem
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

#include "stdlib.h"
#include "stdio.h"
#include "los_memory_pri.h"
#ifdef LOSCFG_MEM_RECORDINFO
#include "los_memrecord_pri.h"
#endif
#ifdef LOSCFG_KERNEL_SCATTER
#include "los_scatter_pri.h"
#endif
#ifdef LOSCFG_SHELL
#include "shcmd.h"
#include "shell.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define MEM_SIZE_1K 0x400
#define MEM_SIZE_1M 0x100000

LITE_OS_SEC_TEXT_MINOR UINT32 OsShellCmdMemCheck(INT32 argc, const CHAR *argv[])
{
    if (argc > 0) {
        PRINTK("\nUsage: memcheck\n");
        return OS_ERROR;
    }

#ifdef LOSCFG_KERNEL_MEM_BESTFIT
    OsMemIntegrityMultiCheck();
#endif
    return 0;
}

#ifdef LOSCFG_SHELL
LITE_OS_SEC_TEXT_MINOR UINT32 OsShellCmdMemRead(INT32 argc, const CHAR *argv[])
{
    size_t tempAddr;
    size_t length;
    CHAR *ptrlen = NULL;
    CHAR *ptrAddr = NULL;

    if ((argc == 0) || (argc > 2)) { /* argc is num of parameters */
        PRINTK("\nUsage: readreg [ADDRESS] [LENGTH]\n");
        return OS_ERROR;
    }

    if (argc == 1) {
        length = 0;
    } else {
        length = strtoul(argv[1], &ptrlen, 0);
        if ((ptrlen == NULL) || (*ptrlen != 0)) {
            PRINTK("readreg invalid length %s\n", argv[1]);
            return OS_ERROR;
        }
    }
    tempAddr = strtoul(argv[0], &ptrAddr, 0);
    if ((ptrAddr == NULL) || (*ptrAddr != 0) || (tempAddr > g_sys_mem_addr_end) ||
        ((tempAddr + length) > g_sys_mem_addr_end) || (tempAddr > (OS_NULL_INT - length))) {
        PRINTK("readreg invalid address %s\n", argv[0]);
        return OS_ERROR;
    }

    OsDumpMemByte(length, tempAddr);

    return 0;
}

LITE_OS_SEC_TEXT_MINOR STATIC VOID OsShellCmdSectionInfo(INT32 argc, const CHAR *argv[])
{
    size_t textLen = &__text_end - &__text_start;
    size_t dataLen = &__ram_data_end - &__ram_data_start;
    size_t rodataLen = &__rodata_end - &__rodata_start;
    size_t bssLen = &__bss_end - &__bss_start;

#ifdef LOSCFG_KERNEL_SCATTER
    textLen += &__fast_text_end - &__fast_text_start;
    dataLen += &__fast_data_end - &__fast_data_start;
    rodataLen += &__fast_rodata_end - &__fast_rodata_start;
#endif

    PRINTK("\r\n        text         data          rodata        bss\n");
    if ((argc == 1) && (strcmp(argv[0], "-k") == 0)) {
        PRINTK("Mem:    %-9lu    %-10lu    %-10lu    %-10lu\n", textLen / MEM_SIZE_1K, dataLen / MEM_SIZE_1K,
               rodataLen / MEM_SIZE_1K, bssLen / MEM_SIZE_1K);
    } else if ((argc == 1) && (strcmp(argv[0], "-m") == 0)) {
        PRINTK("Mem:    %-9lu    %-10lu    %-10lu    %-10lu\n", textLen / MEM_SIZE_1M, dataLen / MEM_SIZE_1M,
               rodataLen / MEM_SIZE_1M, bssLen / MEM_SIZE_1M);
    } else {
        PRINTK("Mem:    %-9lu    %-10lu    %-10lu    %-10lu\n", textLen, dataLen, rodataLen, bssLen);
    }
}

LITE_OS_SEC_TEXT_MINOR STATIC UINT32 OsShellCmdFreeInfo(INT32 argc, const CHAR *argv[])
{
#ifdef LOSCFG_EXC_INTERACTION
    UINT32 memUsed0 = LOS_MemTotalUsedGet(m_aucSysMem0);
    UINT32 totalMem0 = LOS_MemPoolSizeGet(m_aucSysMem0);
    UINT32 freeMem0 = totalMem0 - memUsed0;
#endif

    UINT32 memUsed = LOS_MemTotalUsedGet(m_aucSysMem1);
    UINT32 totalMem = LOS_MemPoolSizeGet(m_aucSysMem1);
    UINT32 freeMem = totalMem - memUsed;

    if ((argc == 0) ||
        ((argc == 1) && (strcmp(argv[0], "-k") == 0)) ||
        ((argc == 1) && (strcmp(argv[0], "-m") == 0))) {
#ifdef LOSCFG_EXC_INTERACTION
        PRINTK("\r\n***** Mem:system mem      Mem1:exception interaction mem *****\n");
#endif
        PRINTK("\r\n        total        used          free\n");
    }

    if ((argc == 1) && (strcmp(argv[0], "-k") == 0)) {
        PRINTK("Mem:    %-9u    %-10u    %-10u\n", totalMem / MEM_SIZE_1K, memUsed / MEM_SIZE_1K,
               freeMem / MEM_SIZE_1K);
#ifdef LOSCFG_EXC_INTERACTION
        PRINTK("Mem1:   %-9u    %-10u    %-10u\n", totalMem0 / MEM_SIZE_1K, memUsed0 / MEM_SIZE_1K,
               freeMem0 / MEM_SIZE_1K);
#endif
    } else if ((argc == 1) && (strcmp(argv[0], "-m") == 0)) {
        PRINTK("Mem:    %-9u    %-10u    %-10u\n", totalMem / MEM_SIZE_1M, memUsed / MEM_SIZE_1M,
               freeMem / MEM_SIZE_1M);
#ifdef LOSCFG_EXC_INTERACTION
        PRINTK("Mem1:   %-9u    %-10u    %-10u\n", totalMem0 / MEM_SIZE_1M, memUsed0 / MEM_SIZE_1M,
               freeMem0 / MEM_SIZE_1M);
#endif
    } else if (argc == 0) {
        PRINTK("Mem:    %-9u    %-10u    %-10u\n", totalMem, memUsed, freeMem);
#ifdef LOSCFG_EXC_INTERACTION
        PRINTK("Mem1:   %-9u    %-10u    %-10u\n", totalMem0, memUsed0, freeMem0);
#endif
    } else {
        PRINTK("\nUsage: free or free [-k/-m]\n");
        return OS_ERROR;
    }
    return 0;
}

LITE_OS_SEC_TEXT_MINOR UINT32 OsShellCmdFree(INT32 argc, const CHAR *argv[])
{
    if (argc > 1) {
        PRINTK("\nUsage: free or free [-k/-m]\n");
        return OS_ERROR;
    }
    if (OsShellCmdFreeInfo(argc, argv) != 0) {
        return OS_ERROR;
    }
    OsShellCmdSectionInfo(argc, argv);
    return 0;
}

LITE_OS_SEC_TEXT_MINOR UINT32 OsShellCmdUname(INT32 argc, const CHAR *argv[])
{
    if (argc == 0) {
        PRINTK("Huawei LiteOS\n");
        return 0;
    }

    if (argc == 1) {
        if (strcmp(argv[0], "-a") == 0) {
            PRINTK("%s %s %s %s %s\n", HW_LITEOS_VER, HW_LITEOS_SYSNAME, HW_LITEOS_KERNEL_VERSION_STRING,
                   __DATE__, __TIME__);
            return 0;
        } else if (strcmp(argv[0], "-s") == 0) {
            PRINTK("Huawei LiteOS\n");
            return 0;
        } else if (strcmp(argv[0], "-t") == 0) {
            PRINTK("build date : %s %s", __DATE__, __TIME__);
            return 0;
        } else if (strcmp(argv[0], "-v") == 0) {
            PRINTK("%s %s %s %s\n", HW_LITEOS_SYSNAME, HW_LITEOS_KERNEL_VERSION_STRING,
                   __DATE__, __TIME__);
            return 0;
        } else if (strcmp(argv[0], "--help") == 0) {
            PRINTK("-a,            print all information\n"
                   "-s,            print the kernel name\n"
                   "-t,            print the build date\n"
                   "-v,            print the kernel version\n");
            return 0;
        }
    }

    PRINTK("uname: invalid option %s\n"
           "Try 'uname --help' for more information.\n",
           argv[0]);
    return OS_ERROR;
}
#ifdef LOSCFG_MEM_LEAKCHECK
LITE_OS_SEC_TEXT_MINOR UINT32 OsShellCmdMemUsed(INT32 argc, const CHAR *argv[])
{
    if (argc > 0) {
        PRINTK("\nUsage: memused\n");
        return OS_ERROR;
    }

    OsMemUsedNodeShow(m_aucSysMem1);

#ifdef LOSCFG_EXC_INTERACTION
    PRINTK("\n exc interaction memory\n");
    OsMemUsedNodeShow(m_aucSysMem0);
#endif
    return 0;
}
#endif
#ifdef LOSCFG_MEM_RECORDINFO
LITE_OS_SEC_TEXT_MINOR UINT32 OsShellCmdMemRecordEnable(INT32 argc, const CHAR *argv[])
{
    OsMemRecordShowSet(TRUE);
    return 0;
}

LITE_OS_SEC_TEXT_MINOR UINT32 OsShellCmdMemRecordDisable(INT32 argc, const CHAR *argv[])
{
    OsMemRecordShowSet(FALSE);
    return 0;
}

SHELLCMD_ENTRY(memshowenable_shellcmd, CMD_TYPE_EX, "memshowenable", 0,
               (CmdCallBackFunc)OsShellCmdMemRecordEnable);
SHELLCMD_ENTRY(memshowdisable_shellcmd, CMD_TYPE_EX, "memshowdisable", 0,
               (CmdCallBackFunc)OsShellCmdMemRecordDisable);
#endif

#ifdef LOSCFG_MEM_LEAKCHECK
SHELLCMD_ENTRY(memused_shellcmd, CMD_TYPE_EX, "memused", 0, (CmdCallBackFunc)OsShellCmdMemUsed);
#endif

SHELLCMD_ENTRY(memcheck_shellcmd, CMD_TYPE_EX, "memcheck", 0, (CmdCallBackFunc)OsShellCmdMemCheck);
SHELLCMD_ENTRY(uname_shellcmd, CMD_TYPE_EX, "uname", XARGS, (CmdCallBackFunc)OsShellCmdUname);
SHELLCMD_ENTRY(free_shellcmd, CMD_TYPE_EX, "free", XARGS, (CmdCallBackFunc)OsShellCmdFree);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
