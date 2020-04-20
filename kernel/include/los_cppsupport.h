/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Cpp Support HeadFile
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
 * @defgroup los_cppsupport c++
 * @ingroup kernel
 */

#ifndef _LOS_CPPSUPPORT_H
#define _LOS_CPPSUPPORT_H

#include "los_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup los_cppsupport
 * If LOS_CppSystemInit() is called in the first stage of scatter load,
 * this flag should be passed as the third parameter
 */
#define BEFORE_SCATTER 0

/**
 * @ingroup los_cppsupport
 * If LOS_CppSystemInit() is called in the second stage of scatter load,
 * this flag should be passed as the third parameter
 */
#define AFTER_SCATTER 1

/**
 * @ingroup los_cppsupport
 * If scatter load is disabled, this flag should be passed as the third parameter when LOS_CppSystemInit() is called.
 */
#define NO_SCATTER 2

/**
 * @ingroup los_cppsupport
 * @brief System cppsupport initialization.
 *
 * @par Description:
 * This API is used to initialize the cppsupport .
 * @attention
 * <ul>
 * <li>initArrayStart is the start address of .init_array section,
 * initArrayEnd is the end address of .init_array section.</li>
 * <li>initArrayStart must be smaller than initArrayEnd,
 * initArrayStart and initArrayEnd should be 4(32 bits platform) or 8(64 bits platform) bytes alignment.</li>
 * </ul>
 *
 * @param  initArrayStart  [IN] Start address of init_array section.
 * @param  initArrayEnd    [IN] End address of init_array section.
 * @param  flag            [IN] Under what circumstances when LOS_CppSystemInit() is called,
 *                              BEFORE_SCATTER, AFTER_SCATTER or NO_SCATTER.
 *
 * @retval 0 always return 0.
 * @par Dependency:
 * <ul><li>los_cppsupport.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V100R001C00
 */
extern INT32 LOS_CppSystemInit(UINTPTR initArrayStart, UINTPTR initArrayEnd, INT32 flag);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_CPPSUPPORT_H */
