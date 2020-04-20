/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Los_printf HeadFile
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

/**
 * @defgroup los_printf Printf
 * @ingroup kernel
 */

#ifndef _LOS_PRINTF_H
#define _LOS_PRINTF_H

#include "sys_config.h"
#ifdef LOSCFG_LIB_LIBC
#include "stdarg.h"
#endif
#ifdef LOSCFG_LIB_LIBCMINI
#include "stdarg.h"
#endif
#include "los_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */
#ifdef LOSCFG_SHELL_LK
extern void LOS_LkPrint(int level, const char *func, int line, const char *fmt, ...);
#endif

#define LOS_EMG_LEVEL    0

#define LOS_COMMOM_LEVEL (LOS_EMG_LEVEL + 1)

#define LOS_ERR_LEVEL    (LOS_COMMOM_LEVEL + 1)

#define LOS_WARN_LEVEL   (LOS_ERR_LEVEL + 1)

#define LOS_INFO_LEVEL   (LOS_WARN_LEVEL + 1)

#define LOS_DEBUG_LEVEL  (LOS_INFO_LEVEL + 1)

#ifdef LOSCFG_SHELL_LK
#define PRINT_LEVEL      LOS_DEBUG_LEVEL
#else
#define PRINT_LEVEL      LOS_ERR_LEVEL
#endif

typedef VOID (*pf_OUTPUT)(const CHAR *fmt, ...);
/**
 * @ingroup los_printf
 * @brief Format and print data.
 *
 * @par Description:
 * Print argument(s) according to fmt.
 *
 * @attention
 * <ul>
 * <li>None</li>
 * </ul>
 *
 * @param fmt [IN] Type char* controls the ouput as in C printf.
 *
 * @retval None
 * @par Dependency:
 * <ul><li>los_printf.h: the header file that contains the API declaration.</li></ul>
 * @see printf
 * @since Huawei LiteOS V100R001C00
 */
extern VOID OsDprintf(const char *fmt, ...);

#define  dprintf  OsDprintf

#define diag_printf      dprintf

#ifndef PRINT_DEBUG
#if PRINT_LEVEL < LOS_DEBUG_LEVEL
#define PRINT_DEBUG(fmt, args...)
#else
#ifdef LOSCFG_SHELL_LK
#define PRINT_DEBUG(fmt, args...) LOS_LkPrint(LOS_DEBUG_LEVEL, __FUNCTION__, __LINE__, fmt, ##args)
#else
#define PRINT_DEBUG(fmt, args...) do {           \
    (dprintf("[DEBUG] "), dprintf(fmt, ##args)); \
} while (0)
#endif
#endif
#endif

#ifndef PRINT_INFO
#if PRINT_LEVEL < LOS_INFO_LEVEL
#define PRINT_INFO(fmt, args...)
#else
#ifdef LOSCFG_SHELL_LK
#define PRINT_INFO(fmt, args...) LOS_LkPrint(LOS_INFO_LEVEL, __FUNCTION__, __LINE__, fmt, ##args)
#else
#define PRINT_INFO(fmt, args...) do {           \
    (dprintf("[INFO] "), dprintf(fmt, ##args)); \
} while (0)
#endif
#endif
#endif

#ifndef PRINT_WARN
#if PRINT_LEVEL < LOS_WARN_LEVEL
#define PRINT_WARN(fmt, args...)
#else
#ifdef LOSCFG_SHELL_LK
#define PRINT_WARN(fmt, args...) LOS_LkPrint(LOS_WARN_LEVEL, __FUNCTION__, __LINE__, fmt, ##args)
#else
#define PRINT_WARN(fmt, args...) do {           \
    (dprintf("[WARN] "), dprintf(fmt, ##args)); \
} while (0)
#endif
#endif
#endif

#ifndef PRINT_ERR
#if PRINT_LEVEL < LOS_ERR_LEVEL
#define PRINT_ERR(fmt, args...)
#else
#ifdef LOSCFG_SHELL_LK
#define PRINT_ERR(fmt, args...) LOS_LkPrint(LOS_ERR_LEVEL, __FUNCTION__, __LINE__, fmt, ##args)
#else
#define PRINT_ERR(fmt, args...) do {           \
    (dprintf("[ERR] "), dprintf(fmt, ##args)); \
} while (0)
#endif
#endif
#endif

#ifndef PRINTK
#if PRINT_LEVEL < LOS_COMMOM_LEVEL
#define PRINTK(fmt, args...)
#else
#ifdef LOSCFG_SHELL_LK
#define PRINTK(fmt, args...) LOS_LkPrint(LOS_COMMOM_LEVEL, __FUNCTION__, __LINE__, fmt, ##args)
#else
#define PRINTK(fmt, args...) dprintf(fmt, ##args)
#endif
#endif
#endif

#ifndef PRINT_EMG
#if PRINT_LEVEL < LOS_EMG_LEVEL
#define PRINT_EMG(fmt, args...)
#else
#define PRINT_EMG(fmt, args...) do {           \
    (dprintf("[EMG] "), dprintf(fmt, ##args)); \
} while (0)
#endif
#endif

#ifndef PRINT_RELEASE
#define PRINT_RELEASE(fmt, args...)   dprintf(fmt, ##args)
#endif

#ifndef PRINT_TRACE
#ifdef DEBUG_TRACE
#define PRINT_TRACE(fmt, args...)   do {dprintf("[TRACE] "fmt, ##args);} while (0)
#else
#define PRINT_TRACE(fmt, args...)
#endif
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_PRINTF_H */
