/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Error Handling
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
 * @defgroup los_err Error handling
 * @ingroup kernel
 */

#ifndef _LOS_ERR_H
#define _LOS_ERR_H

#include "los_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup los_err
 * @brief Define the pointer to the error handling function.
 *
 * @par Description:
 * This API is used to define the pointer to the error handling function.
 * @attention
 * None.
 *
 * @param  fileName  [IN] Log file that stores error information.
 * @param  lineNo    [IN] Line number of the erroneous line.
 * @param  errorNo   [IN] Error code.
 * @param  paraLen   [IN] Length of the input parameter pPara.
 * @param  para      [IN] User label of the error.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_err.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V100R001C00
 */
typedef VOID (*LOS_ERRORHANDLE_FUNC)(CHAR *fileName,
                                     UINT32 lineNo,
                                     UINT32 errorNo,
                                     UINT32 paraLen,
                                     VOID *para);

/**
 * @ingroup los_err
 * @brief Error handling function.
 *
 * @par Description:
 * This API is used to perform different operations according to error types.
 * @attention
 * None
 *
 * @param  fileName  [IN] Log file that stores error information.
 * @param  lineNo    [IN] Line number of the erroneous line which should not be OS_ERR_MAGIC_WORD.
 * @param  errorNo   [IN] Error code.
 * @param  paraLen   [IN] Length of the input parameter pPara.
 * @param  para      [IN] User label of the error.
 *
 * @retval #LOS_OK The error is successfully processed.
 * @par Dependency:
 * <ul><li>los_err.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_ErrHandle(CHAR *fileName, UINT32 lineNo,
                            UINT32 errorNo, UINT32 paraLen,
                            VOID *para);

/**
 * @ingroup los_err
 * @brief User registration error handling hook function.
 *
 * @par Description:
 * This API is used to register user error handling hook function,
 * Support repeated registration.
 * @attention
 * None
 *
 * @param  func  [IN] error handling hook function.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_err.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V100R001C00
 */
extern VOID LOS_RegErrHandle(LOS_ERRORHANDLE_FUNC func);

/**
 * @ingroup los_err
 * Error handling function structure.
 */
typedef struct tagUserErrFunc_S {
    LOS_ERRORHANDLE_FUNC pfnHook; /**< Hook function for error handling. */
} USER_ERR_FUNC_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_ERR_H */
