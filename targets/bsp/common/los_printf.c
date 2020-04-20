/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: LiteOS Printf Implementation
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
#ifdef LOSCFG_LIB_LIBC
#include "stdlib.h"
#include "unistd.h"
#endif
#ifdef LOSCFG_LIB_LIBCMINI
#include "stdarg.h"
#endif
#include "los_hwi.h"
#include "los_memory_pri.h"
#include "uart.h"
#ifdef LOSCFG_FS_VFS
#include "console.h"
#endif
#ifdef LOSCFG_SHELL_DMESG
#include "dmesg_pri.h"
#endif
#ifdef LOSCFG_SHELL_EXCINFO
#include "los_excinfo_pri.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define USE_STACK_BUFFER
#define SIZEBUF  256

typedef enum {
    NO_OUTPUT = 0,
    UART_OUTPUT = 1,
    CONSOLE_OUTPUT = 2,
    EXC_OUTPUT = 3
} OutputType;

STATIC VOID ErrorMsg(VOID)
{
    const CHAR *p = "Output illegal string! vsnprintf_s failed!\n";
    (VOID)UartPuts(p, (INT32)strlen(p), UART_WITH_LOCK);
}

STATIC VOID UartOutput(const CHAR *str, UINT32 len, BOOL isLock)
{
#ifdef LOSCFG_SHELL_DMESG
    if (!OsCheckUartLock()) {
        UartPuts(str, len, isLock);
    }
    if (isLock != UART_WITHOUT_LOCK) {
        (VOID)OsLogMemcpyRecord(str, len);
    }
#else
    (VOID)UartPuts(str, (INT32)len, isLock);
#endif
}

STATIC VOID OutputControl(const CHAR *str, UINT32 len, OutputType type)
{
    switch (type) {
        case CONSOLE_OUTPUT:
#ifdef LOSCFG_PLATFORM_CONSOLE
            if (ConsoleEnable() == TRUE) {
                (VOID)write(STDOUT_FILENO, str, (size_t)len);
                break;
            }
#endif
            /* fall-through */
        case UART_OUTPUT:
            UartOutput(str, len, UART_WITH_LOCK);
            break;
        case EXC_OUTPUT:
            UartOutput(str, len, UART_WITHOUT_LOCK);
            break;
        default:
            break;
    }
    return;
}

STATIC VOID OsVprintfFree(CHAR *buf, UINT32 bufLen)
{
#ifdef USE_STACK_BUFFER
    if (bufLen != SIZEBUF) {
        (VOID)LOS_MemFree(m_aucSysMem0, buf);
    }
#else
    (VOID)LOS_MemFree(m_aucSysMem0, buf);
#endif
}

STATIC VOID OsVprintf(const CHAR *fmt, va_list ap, OutputType type)
{
    INT32 len;
    UINT32 bufLen = SIZEBUF;
    CHAR *bBuf = NULL;
#ifdef USE_STACK_BUFFER
    CHAR aBuf[SIZEBUF];
    bBuf = aBuf;
#else
    bBuf = (CHAR *)LOS_MemAlloc(m_aucSysMem0, SIZEBUF);
    if (bBuf == NULL) {
        PRINT_ERR("%s, %d, malloc failed!\n", __FUNCTION__, __LINE__);
        return;
    }
#endif
    len = vsnprintf_s(bBuf, bufLen, bufLen - 1, fmt, ap);
    if ((len == -1) && (*bBuf == '\0')) {
        /* parameter is illegal or some features in fmt dont support */
        ErrorMsg();
        goto EXIT;
    }

    while (len == -1) {
        /* bBuf is not enough */
        OsVprintfFree(bBuf, bufLen);

        bufLen = bufLen << 1;
        if ((INT32)bufLen <= 0) {
            PRINT_ERR("%s, %d, length overflow!\n", __FUNCTION__, __LINE__);
            return;
        }
        bBuf = (CHAR *)LOS_MemAlloc(m_aucSysMem0, bufLen);
        if (bBuf == NULL) {
            PRINT_ERR("%s, %d, malloc failed!\n", __FUNCTION__, __LINE__);
            return;
        }
        len = vsnprintf_s(bBuf, bufLen, bufLen - 1, fmt, ap);
        if (*bBuf == '\0') {
            /* parameter is illegal or some features in fmt dont support */
            ErrorMsg();
            goto EXIT;
        }
    }
    *(bBuf + len) = '\0';
    OutputControl(bBuf, len, type);
EXIT:
    OsVprintfFree(bBuf, bufLen);
}

VOID UartVprintf(const CHAR *fmt, va_list ap)
{
    OsVprintf(fmt, ap, UART_OUTPUT);
}

__attribute__((noinline)) VOID UartPrintf(const CHAR *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    OsVprintf(fmt, ap, UART_OUTPUT);
    va_end(ap);
}

__attribute__ ((noinline)) VOID OsDprintf(const CHAR *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    OsVprintf(fmt, ap, CONSOLE_OUTPUT);
    va_end(ap);
}

VOID LkDprintf(const CHAR *fmt, va_list ap)
{
    OsVprintf(fmt, ap, CONSOLE_OUTPUT);
}

#ifdef LOSCFG_SHELL_DMESG
VOID DmesgPrintf(const CHAR *fmt, va_list ap)
{
    OsVprintf(fmt, ap, CONSOLE_OUTPUT);
}
#endif

#ifdef LOSCFG_PLATFORM_UART_WITHOUT_VFS
__attribute__ ((noinline)) INT32 printf(const CHAR *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    OsVprintf(fmt, ap, UART_OUTPUT);
    va_end(ap);
}
#else
#ifdef LOSCFG_PLATFORM_NO_UART
__attribute__ ((noinline)) int printf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    OsVprintf(fmt, ap, CONSOLE_OUTPUT);
    va_end(ap);
    return LOS_OK;
}
#endif
#endif

__attribute__((noinline)) VOID syslog(INT32 level, const CHAR *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    OsVprintf(fmt, ap, CONSOLE_OUTPUT);
    va_end(ap);
    (VOID)level;
}

__attribute__((noinline)) VOID ExcPrintf(const CHAR *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    /* uart output without print-spinlock */
    OsVprintf(fmt, ap, EXC_OUTPUT);
    va_end(ap);
}

VOID PrintExcInfo(const CHAR *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    /* uart output without print-spinlock */
    OsVprintf(fmt, ap, EXC_OUTPUT);
#ifdef LOSCFG_SHELL_EXCINFO
    WriteExcBufVa(fmt, ap);
#endif
    va_end(ap);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
