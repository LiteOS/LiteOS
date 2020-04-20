/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Error Handling Private HeadFile
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

#ifndef _LOS_ERR_PRI_H
#define _LOS_ERR_PRI_H

#include "los_err.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup los_err
 * Define the error magic word.
 */
#define OS_ERR_MAGIC_WORD 0xa1b2c3f8

/**
 * @ingroup los_err
 * @brief Error handling macro capable of returning error codes.
 *
 * @par Description:
 * This API is used to call the error handling function by using an error code and return the same error code.
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @param  errNo   [IN] Error code.
 *
 * @retval errNo
 * @par Dependency:
 * <ul><li>los_err_pri.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V100R001C00
 */
#define OS_RETURN_ERROR(errNo) do {                                               \
    (VOID)LOS_ErrHandle("os_unspecific_file", OS_ERR_MAGIC_WORD, errNo, 0, NULL); \
    return errNo;                                                                 \
} while (0)

/**
 * @ingroup los_err
 * @brief Error handling macro capable of returning error codes.
 *
 * @par Description:
 * This API is used to call the error handling function by using an error code and the line number of
 * the erroneous line, and return the same error code.
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @param  errLine   [IN] Line number of the erroneous line.
 * @param  errNo   [IN] Error code.
 *
 * @retval errNo
 * @par Dependency:
 * <ul><li>los_err_pri.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V100R001C00
 */
#define OS_RETURN_ERROR_P2(errLine, errNo) do {                          \
    (VOID)LOS_ErrHandle("os_unspecific_file", errLine, errNo, 0, NULL);  \
    return errNo;                                                        \
} while (0)

/**
 * @ingroup los_err
 * @brief Macro for jumping to error handler.
 *
 * @par Description:
 * This API is used to call the error handling function by using an error code.
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @param  errorNo   [IN] Error code.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_err_pri.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V100R001C00
 */
#define OS_GOTO_ERR_HANDLER(errorNo) do { \
    errNo = errorNo;                      \
    errLine = OS_ERR_MAGIC_WORD;          \
    goto ERR_HANDLER;                     \
} while (0)

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_ERR_PRI_H */
