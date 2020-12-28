/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: LiteOS Shell lk Parse Implementation Headfile
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

#ifndef _HWLITEOS_SHELL_LK_H
#define _HWLITEOS_SHELL_LK_H

/**
 * @defgroup shell_lk lk
 * @ingroup shell
 */
#include "stdarg.h"
#include "los_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup shell_lk
 * define sys default print level
 * 0: TRACE_EMG 1: TRACE_COMMOM 2: TRACE_ERROR 3: TRACE_WARN 4: TRACE_INFO 5: TRACE_DEBUG
 */
#define TRACE_DEFAULT TRACE_ERROR

/**
 * @ingroup shell_lk
 * @brief Define an printf handling function hook.
 *
 * @par Description:
 * This API is used to define the printf handling function hook.
 * @attention None.
 *
 * @param  level    [IN] print level.
 * @param  func     [IN] means which func calls print func.
 * @param  line     [IN] means which line calls print func.
 * @param  fmt      [IN] other information by user define.
 * @param  ap       [IN] the para list.
 *
 * @retval None.
 *
 * @par Dependency:
 * shell_lk.h: the header file that contains the API declaration.
 * @see None.
 * @since Huawei LiteOS V200R001C00
 */
typedef VOID (*LK_FUNC)(INT32 level, const CHAR *func, INT32 line, const CHAR *fmt, va_list ap);

/**
 * @ingroup shell_lk
 * @brief print log.
 *
 * @par Description:
 * <ul>
 * <li>This API is used to LK print function.</li>
 * </ul>
 *
 * @param  level    [IN] print level.
 * @param  func     [IN] means which func calls print func.
 * @param  line     [IN] means which line calls print func.
 * @param  fmt      [IN] other information by user define
 *
 * @retval NONE
 * @par Dependency:
 * <ul><li>shell_lk.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V200R001C00
 */
extern VOID LOS_LkPrint(INT32 level, const CHAR *func, INT32 line, const CHAR *fmt, ...);

/**
 * @ingroup shell_lk
 * @brief register print func.
 *
 * @par Description:
 * <ul>
 * <li>This API is used to registe a hook function to LK.</li>
 * </ul>
 *
 * @param  LK_FUNC  [IN] the print func.
 *
 * @retval NONE
 * @par Dependency:
 * <ul><li>shell_lk.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V200R001C00
 */
extern VOID LOS_LkRegHook(LK_FUNC hook);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _HWLITEOS_SHELL_LK_H */