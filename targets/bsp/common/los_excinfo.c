/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Exception Information Module Implementation
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

#include "los_base.h"
#if defined(LOSCFG_SHELL_EXCINFO) && defined(LOSCFG_DRIVERS_MTD_SPI_NOR)
#include "linux/mtd/mtd.h"
#include "linux/module.h"
#include "linux/mtd/mtd_list.h"
#include "spinor.h"
#endif
#include "los_hwi.h"
#ifdef LOSCFG_FS_VFS
#include "fs/fs.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#ifdef LOSCFG_SHELL_EXCINFO
STATIC log_read_write_fn g_excInfoRW = NULL; /* the hook of read-writing exception information */
STATIC CHAR *g_excInfoBuf = NULL;            /* pointer to the buffer for storing the exception information */
STATIC UINT32 g_excInfoIndex = 0xFFFF;       /* the index of the buffer for storing the exception information */
STATIC UINT32 g_recordAddr = 0;              /* the address of storing the exception information */
STATIC UINT32 g_recordSpace = 0;             /* the size of storing the exception information */

VOID SetExcInfoRW(log_read_write_fn func)
{
    g_excInfoRW = func;
}

log_read_write_fn GetExcInfoRW(VOID)
{
    return g_excInfoRW;
}

VOID SetExcInfoBuf(CHAR *buf)
{
    g_excInfoBuf = buf;
}

CHAR *GetExcInfoBuf(VOID)
{
    return g_excInfoBuf;
}

VOID SetExcInfoIndex(UINT32 index)
{
    g_excInfoIndex = index;
}

UINT32 GetExcInfoIndex(VOID)
{
    return g_excInfoIndex;
}

VOID SetRecordAddr(UINT32 addr)
{
    g_recordAddr = addr;
}

UINT32 GetRecordAddr(VOID)
{
    return g_recordAddr;
}

VOID SetRecordSpace(UINT32 space)
{
    g_recordSpace = space;
}

UINT32 GetRecordSpace(VOID)
{
    return g_recordSpace;
}

VOID WriteExcBufVa(const CHAR *format, va_list arglist)
{
    errno_t ret;

    if (g_recordSpace > g_excInfoIndex) {
        ret = vsnprintf_s((g_excInfoBuf + g_excInfoIndex), (g_recordSpace - g_excInfoIndex),
                          (g_recordSpace - g_excInfoIndex - 1), format, arglist);
        if (ret == -1) {
            PRINT_ERR("exc info buffer is not enough or vsnprintf_s is error.\n");
            return;
        }
        g_excInfoIndex += ret;
    }
}

VOID WriteExcInfoToBuf(const CHAR *format, ...)
{
    va_list arglist;

    va_start(arglist, format);
    WriteExcBufVa(format, arglist);
    va_end(arglist);
}

#ifdef LOSCFG_DRIVERS_MTD_SPI_NOR

STATIC struct mtd_info *g_mtdSpinor = NULL;

STATIC VOID OsSpiflashErase(UINT32 start, size_t size)
{
    struct erase_info eraseInfo;

    (VOID)memset_s(&eraseInfo, sizeof(struct erase_info), 0, sizeof(struct erase_info));
    eraseInfo.mtd = g_mtdSpinor;
    eraseInfo.callback = NULL;
    eraseInfo.fail_addr = (UINT64)MTD_FAIL_ADDR_UNKNOWN;
    eraseInfo.addr = start;
    eraseInfo.len = size;
    eraseInfo.time = 1;
    eraseInfo.retries = 1;
    eraseInfo.dev = 0;
    eraseInfo.cell = 0;
    eraseInfo.priv = 0;
    eraseInfo.state = 0;
    eraseInfo.next = NULL;
    eraseInfo.scrub = 0;
    (VOID)g_mtdSpinor->erase(g_mtdSpinor, &eraseInfo);
}

STATIC INT32 OsWriteExcInfoToSpiFlash(UINT32 startAddr, UINT32 space, const CHAR *buf)
{
    UINT32 outLen;
    UINT32 len = (space < g_recordSpace) ? space : g_recordSpace;

    (VOID)startAddr;
    OsSpiflashErase(g_recordAddr, LOS_Align(g_recordSpace, g_mtdSpinor->erasesize));
    return g_mtdSpinor->write(g_mtdSpinor, (loff_t)g_recordAddr, len, &outLen, buf);
}

STATIC INT32 OsReadExcInfoForSpiFlash(UINT32 startAddr, UINT32 space, CHAR *buf)
{
    UINT32 outLen;
    UINT32 len = (space < g_recordSpace) ? space : g_recordSpace;

    (VOID)startAddr;
    return g_mtdSpinor->read(g_mtdSpinor, (loff_t)g_recordAddr, len, &outLen, buf);
}
#endif

VOID LOS_ExcInfoRegHook(UINT32 startAddr, UINT32 space, CHAR *buf, log_read_write_fn hook)
{
    if ((hook == NULL) || (buf == NULL)) {
        PRINT_ERR("Buf or hook is null.\n");
        return;
    }

    g_recordAddr = startAddr;
    g_recordSpace = space;
    g_excInfoBuf = buf;
    g_excInfoRW = hook;

#ifdef LOSCFG_FS_VFS
    los_vfs_init();
#endif
}

/* Be called in the exception. */
VOID OsReadWriteExceptionInfo(UINT32 startAddr, UINT32 space, UINT32 flag, CHAR *buf)
{
    if ((buf == NULL) || (space == 0)) {
        PRINT_ERR("buffer is null or space is zero\n");
        return;
    }

#ifdef LOSCFG_DRIVERS_MTD_SPI_NOR
    g_mtdSpinor = get_mtd("spinor");
    if (g_mtdSpinor == NULL) {
        (VOID)spinor_init();
        g_mtdSpinor = get_mtd("spinor");
        if (g_mtdSpinor == NULL) {
            PRINT_ERR("Init spinor is failed\n");
            return;
        }
    }
#endif

    if (flag == 0) {
#ifdef LOSCFG_DRIVERS_MTD_SPI_NOR
        if (OsWriteExcInfoToSpiFlash(startAddr, space, buf) != LOS_OK) {
            PRINT_ERR("Exception information written to flash failed\n");
        }
        free(buf);  /* Consider whether or not the "buf" is released according to actual use */
#endif
    } else if (flag == 1) {
#ifdef LOSCFG_DRIVERS_MTD_SPI_NOR
        if (OsReadExcInfoForSpiFlash(startAddr, space, buf) != LOS_OK) {
            PRINT_ERR("Exception information read from flash failed\n");
        }
#endif
    } else {
        PRINT_ERR("flag is error\n");
    }
}

VOID OsRecordExcInfoTime(VOID)
{
#ifdef LOSCFG_FS_VFS
#define NOW_TIME_LENGTH 24
    time_t t;
    struct tm *tmTime = NULL;
    CHAR nowTime[NOW_TIME_LENGTH];

    (VOID)time(&t);
    tmTime = localtime(&t);
    if (tmTime == NULL) {
        return;
    }
    (VOID)memset_s(nowTime, sizeof(nowTime), 0, sizeof(nowTime));
    (VOID)strftime(nowTime, NOW_TIME_LENGTH, "%Y-%m-%d %H:%M:%S", tmTime);
#undef NOW_TIME_LENGTH
    WriteExcInfoToBuf("%s \n", nowTime);
#endif
}

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
