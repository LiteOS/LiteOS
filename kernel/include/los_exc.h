/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Exception handling HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2020-06-24
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
 * @defgroup los_exc Exception handling
 * @ingroup kernel
 */
#ifndef _LOS_EXC_H
#define _LOS_EXC_H

#include "arch/exception.h"
#include "los_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup los_exc
 * @brief Kernel panic function.
 *
 * @par Description:
 * Stack function that prints kernel panics.
 * @attention
 * <ul>
 * <li>After this function is called and stack information is printed, the system will fail to respond.</li>
 * <li>The input parameter can be NULL.</li>
 * </ul>
 * @param fmt [IN] Type #CHAR*. It is a variadic argument.
 *
 * @retval None.
 *
 * @par Dependency:
 * los_exc.h: the header file that contains the API declaration.
 * @since Huawei LiteOS V100R001C00
 */
VOID LOS_Panic(const CHAR *fmt, ...);

/**
 * @ingroup los_exc
 * @brief Kernel backtrace function.
 *
 * @par Description:
 * Backtrace function that prints task call stack information traced from the running task.
 * @attention None.
 *
 * @param None.
 *
 * @retval None.
 *
 * @par Dependency:
 * los_exc.h: the header file that contains the API declaration.
 * @since Huawei LiteOS V200R005C10
 */
extern VOID LOS_BackTrace(VOID);
#define OsBackTrace LOS_BackTrace

/**
 * @ingroup los_exc
 * @brief Kernel task backtrace function.
 *
 * @par Description:
 * Backtrace function that prints task call stack information traced from the input task.
 * @attention
 * The input taskID should be valid.
 *
 * @param  taskID [IN] Type #UINT32. Task ID.
 *
 * @retval None.
 *
 * @par Dependency:
 * los_exc.h: the header file that contains the API declaration.
 * @since Huawei LiteOS V100R001C00
 */
extern VOID LOS_TaskBackTrace(UINT32 taskID);
#define OsTaskBackTrace LOS_TaskBackTrace

#ifdef LOSCFG_SHELL_EXCINFO_DUMP
/**
 * @ingroup los_exc
 * @brief  Define the type of functions for reading or writing exception information.
 *
 * @par Description:
 * This definition is used to declare the type of functions for reading or writing exception information.
 * @attention
 * The first parameter "startAddr" must be left to save the exception information.
 *
 * @param startAddr    [IN] The storage space address, it uses to save exception information.
 * @param space        [IN] The storage space size, it is also the size of the last parameter "buf".
 * @param rwFlag       [IN] The write-read flag, 0 for writing,1 for reading, other number is to do nothing.
 * @param buf          [IN] The buffer of storing data.
 *
 * @retval none.
 * @par Dependency:
 * <ul><li>los_exc.h: the header file that contains the type definition.</li></ul>
 * @since Huawei LiteOS V200R005C10
 */
typedef VOID (*LogReadWriteFunc)(UINTPTR startAddr, UINT32 space, UINT32 rwFlag, CHAR *buf);
#define log_read_write_fn LogReadWriteFunc /* old API since V200R002C00, please avoid use of it */

 /**
 * @ingroup los_exc
 * @brief Register the function of recording exception information .
 *
 * @par Description:
 * This API is used to register the function of recording exception information, and specify the
 * location, size and buffer of the exception information recording.
 * @attention
 * The first parameter "startAddr" must be left to save the exception information.
 *
 * @param startAddr    [IN] The storage space address, it uses to save exception information.
 * @param space        [IN] The storage space size, it is also the size of the third parameter "buf".
 * @param buf          [IN] The buffer of storing exception information. The buffer is allocated or
 *                          free in user's code.
 * @param hook         [IN] the function for reading or writing exception information.
 *
 * @retval none.
 * @par Dependency:
 * <ul><li>los_exc.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V200R002C00
 */
VOID LOS_ExcInfoRegHook(UINTPTR startAddr, UINT32 space, CHAR *buf, LogReadWriteFunc hook);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_EXC_H */
