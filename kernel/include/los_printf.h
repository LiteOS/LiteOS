/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Los_printf HeadFile
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

/**
 * @defgroup los_printf Printf
 * @ingroup kernel
 */

#ifndef _LOS_PRINTF_H
#define _LOS_PRINTF_H

#include "stdarg.h"
#include "los_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */
#ifdef LOSCFG_SHELL_LK
extern void LOS_LkPrint(int level, const char *func, int line, const char *fmt, ...);
#endif

/**
 * @ingroup los_printf
 * log print level definition, LOS_EMG_LEVEL is set to 0, it means the log is emergency.
 */
#define LOS_EMG_LEVEL    0

/**
 * @ingroup los_printf
 * log print level definition, LOS_COMMOM_LEVEL is set to 1, it means the log is common.
 */
#define LOS_COMMOM_LEVEL (LOS_EMG_LEVEL + 1)

/**
 * @ingroup los_printf
 * log print level definition, LOS_ERR_LEVEL is set to 2, it means it is a error log.
 */
#define LOS_ERR_LEVEL    (LOS_COMMOM_LEVEL + 1)

/**
 * @ingroup los_printf
 * log print level definition, LOS_WARN_LEVEL is set to 3, it means it is a warning log.
 */
#define LOS_WARN_LEVEL   (LOS_ERR_LEVEL + 1)

/**
 * @ingroup los_printf
 * log print level definition, LOS_INFO_LEVEL is set to 4, it means the log is an information.
 */
#define LOS_INFO_LEVEL   (LOS_WARN_LEVEL + 1)

/**
 * @ingroup los_printf
 * log print level definition, LOS_DEBUG_LEVEL is set to 5, it means it is a debug log.
 */
#define LOS_DEBUG_LEVEL  (LOS_INFO_LEVEL + 1)

/**
 * @ingroup los_printf
 * The default log print level. PRINT_LEVEL is set to debug log level if
 * LOSCFG_SHELL_LK is defined, otherwise PRINT_LEVEL is set to error log level.
 * The default log print level means only print the log which its level value
 * is lower than or equal to the PRINT_LEVEL.
 */
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
 * None.
 *
 * @param fmt [IN] Type char*. It controls the ouput format as in C printf.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_printf.h: the header file that contains the API declaration.</li></ul>
 * @see printf
 * @since Huawei LiteOS V100R001C00
 */
extern void dprintf(const char *fmt, ...);

#define diag_printf      dprintf

/**
 * @ingroup los_printf
 * @brief Format and print debug log.
 *
 * @par Description:
 * Define function macros PRINT_DEBUG. The Function can print debug log according to fmt
 * when the PRINT_LEVEL is set to LOS_DEBUG_LEVEL.
 *
 * @attention
 * None.
 *
 * @param fmt  [IN] Type: const CHAR *. It controls the ouput format as in C printf.
 * @param args [IN] It point to the variable parameters.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_printf.h: the header file that contains the API declaration.</li></ul>
 * @see dprintf
 * @since Huawei LiteOS V100R001C00
 */
#ifndef PRINT_DEBUG
#if PRINT_LEVEL < LOS_DEBUG_LEVEL
#define PRINT_DEBUG(fmt, ...)
#else
#ifdef LOSCFG_SHELL_LK
#define PRINT_DEBUG(fmt, ...) LOS_LkPrint(LOS_DEBUG_LEVEL, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define PRINT_DEBUG(fmt, ...) do {           \
    (dprintf("[DEBUG] "), dprintf(fmt, ##__VA_ARGS__)); \
} while (0)
#endif
#endif
#endif

/**
 * @ingroup los_printf
 * @brief Format and print information log.
 *
 * @par Description:
 * Define function macros PRINT_INFO. The Function can print information log according to fmt
 * when the PRINT_LEVEL is greater than or equal to LOS_INFO_LEVEL.
 *
 * @attention
 * None.
 *
 * @param fmt  [IN] Type: const CHAR *. It controls the ouput format as in C printf.
 * @param args [IN] It point to the variable parameters.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_printf.h: the header file that contains the API declaration.</li></ul>
 * @see dprintf
 * @since Huawei LiteOS V100R001C00
 */
#ifndef PRINT_INFO
#if PRINT_LEVEL < LOS_INFO_LEVEL
#define PRINT_INFO(fmt, ...)
#else
#ifdef LOSCFG_SHELL_LK
#define PRINT_INFO(fmt, ...) LOS_LkPrint(LOS_INFO_LEVEL, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define PRINT_INFO(fmt, ...) do {           \
    (dprintf("[INFO] "), dprintf(fmt, ##__VA_ARGS__)); \
} while (0)
#endif
#endif
#endif

/**
 * @ingroup los_printf
 * @brief Format and print warning log.
 *
 * @par Description:
 * Define function macros PRINT_WARN. The Function can print warning log according to fmt
 * when the PRINT_LEVEL is greater than or equal to LOS_WARN_LEVEL.
 *
 * @attention
 * None.
 *
 * @param fmt  [IN] Type: const CHAR *. It controls the ouput format as in C printf.
 * @param args [IN] It point to the variable parameters.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_printf.h: the header file that contains the API declaration.</li></ul>
 * @see dprintf
 * @since Huawei LiteOS V100R001C00
 */
#ifndef PRINT_WARN
#if PRINT_LEVEL < LOS_WARN_LEVEL
#define PRINT_WARN(fmt, ...)
#else
#ifdef LOSCFG_SHELL_LK
#define PRINT_WARN(fmt, ...) LOS_LkPrint(LOS_WARN_LEVEL, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define PRINT_WARN(fmt, ...) do {           \
    (dprintf("[WARN] "), dprintf(fmt, ##__VA_ARGS__)); \
} while (0)
#endif
#endif
#endif

/**
 * @ingroup los_printf
 * @brief Format and print error log.
 *
 * @par Description:
 * Define function macros PRINT_ERR. The Function can print error log according to fmt
 * when the PRINT_LEVEL is greater than or equal to LOS_ERR_LEVEL.
 *
 * @attention
 * None.
 *
 * @param fmt  [IN] Type: const CHAR *. It controls the ouput format as in C printf.
 * @param args [IN] It point to the variable parameters.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_printf.h: the header file that contains the API declaration.</li></ul>
 * @see dprintf
 * @since Huawei LiteOS V100R001C00
 */
#ifndef PRINT_ERR
#if PRINT_LEVEL < LOS_ERR_LEVEL
#define PRINT_ERR(fmt, ...)
#else
#ifdef LOSCFG_SHELL_LK
#define PRINT_ERR(fmt, ...) LOS_LkPrint(LOS_ERR_LEVEL, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define PRINT_ERR(fmt, ...) do {           \
    (dprintf("[ERR] "), dprintf(fmt, ##__VA_ARGS__)); \
} while (0)
#endif
#endif
#endif

/**
 * @ingroup los_printf
 * @brief Format and print common log.
 *
 * @par Description:
 * Define function macros PRINTK. The Function can print common log according to fmt
 * when the PRINT_LEVEL is greater than or equal to LOS_COMMOM_LEVEL.
 *
 * @attention
 * None.
 *
 * @param fmt  [IN] Type: const CHAR *. It controls the ouput format as in C printf.
 * @param args [IN] It point to the variable parameters.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_printf.h: the header file that contains the API declaration.</li></ul>
 * @see dprintf
 * @since Huawei LiteOS V100R001C00
 */
#ifndef PRINTK
#if PRINT_LEVEL < LOS_COMMOM_LEVEL
#define PRINTK(fmt, ...)
#else
#ifdef LOSCFG_SHELL_LK
#define PRINTK(fmt, ...) LOS_LkPrint(LOS_COMMOM_LEVEL, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define PRINTK(fmt, ...) dprintf(fmt, ##__VA_ARGS__)
#endif
#endif
#endif

/**
 * @ingroup los_printf
 * @brief Format and print emergency log.
 *
 * @par Description:
 * Define function macros PRINT_EMG. The Function can print emergency log according to fmt
 * when the PRINT_LEVEL is greater than or equal to LOS_EMG_LEVEL.
 *
 * @attention
 * None.
 *
 * @param fmt  [IN] Type: const CHAR *. It controls the ouput format as in C printf.
 * @param args [IN] It point to the variable parameters.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_printf.h: the header file that contains the API declaration.</li></ul>
 * @see dprintf
 * @since Huawei LiteOS V100R001C00
 */
#ifndef PRINT_EMG
#if PRINT_LEVEL < LOS_EMG_LEVEL
#define PRINT_EMG(fmt, ...)
#else
#define PRINT_EMG(fmt, ...) do {           \
    (dprintf("[EMG] "), dprintf(fmt, ##__VA_ARGS__)); \
} while (0)
#endif
#endif

/**
 * @ingroup los_printf
 * @brief Format and print log.
 *
 * @par Description:
 * Define function macros PRINT_RELEASE. The Function can print argument(s) according to fmt.
 * It is same with dprintf function.
 *
 * @attention
 * None.
 *
 * @param fmt  [IN] Type: const CHAR *. It controls the ouput format as in C printf.
 * @param args [IN] It point to the variable parameters.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_printf.h: the header file that contains the API declaration.</li></ul>
 * @see dprintf
 * @since Huawei LiteOS V100R001C00
 */
#ifndef PRINT_RELEASE
#define PRINT_RELEASE(fmt, ...)   dprintf(fmt, ##__VA_ARGS__)
#endif

/**
 * @ingroup los_printf
 * @brief Format and print trace log.
 *
 * @par Description:
 * Define function macros PRINT_TRACE. The Function can print trace log according to fmt
 * only when the DEBUG_TRACE is set.
 *
 * @attention
 * None.
 *
 * @param fmt  [IN] Type: const CHAR *. It controls the ouput format as in C printf.
 * @param args [IN] It point to the variable parameters.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_printf.h: the header file that contains the API declaration.</li></ul>
 * @see dprintf
 * @since Huawei LiteOS V100R001C00
 */
#ifndef PRINT_TRACE
#ifdef DEBUG_TRACE
#define PRINT_TRACE(fmt, ...)   do {dprintf("[TRACE] "fmt, ##__VA_ARGS__);} while (0)
#else
#define PRINT_TRACE(fmt, ...)
#endif
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_PRINTF_H */
