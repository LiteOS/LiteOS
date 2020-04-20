/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Scatter HeadFile
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
 * @defgroup los_scatter Scatter
 *  @ingroup kernel
 */

#ifndef _LOS_SCATTER_H
#define _LOS_SCATTER_H

#include "los_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup los_scatter
 * Configuration item for scatter-loading time printing
 *
 * The scatter-loading time printing is disabled by default.
 */
#ifndef SCATTER_LOADTIME_PRINT
#define SCATTER_LOADTIME_PRINT 0
#endif

/**
 * @ingroup  los_scatter
 * @brief Define the type of a flash-reading function.
 *
 * @par Description:
 * This API is used to define the type of a flash-reading function, so that it can be called to read data from a
 * certain flash.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @param  memAddr   [IN] The memory starting address where to write the data reading from a certain flash.
 * @param  start     [IN] The starting address to read data from a certain flash.
 * @param  size      [IN] The size of data reading from a certain flash.
 *
 * @retval #INT32          The size of data read from flash.
 * @par Dependency:
 * <ul><li>los_scatter.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V100R001C00
 */
typedef INT32 (*SCATTER_FLASH_READ_FUNC)(VOID *memAddr, size_t start, size_t size);

/**
 * @ingroup  los_scatter
 * @brief Define the type of a scatter-loading hook function.
 *
 * @par Description:
 * This API is used to define the type of a scatter-loading hook function, so that it can be called in the loading phase
 * of scatter-loading.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @param  None.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_scatter.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V100R001C00
 */
typedef VOID (*SCATTER_ENTRY_FUNC)(VOID);

/**
 * @ingroup los_scatter
 * Scatter-loading hook function
 *
 */
extern SCATTER_ENTRY_FUNC g_SCATTER_AFTER_FUNC;

/**
 * @ingroup los_scatter
 * @brief Scatter-loading function
 *
 * @par Description:
 * This API is used to copy image data of specified size from a specified type of storage medium.
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @param  imageFlashAddr   [IN] The flash starting address where the whole application image is burned.
 * @param  flashReadFunc    [IN] An abstract function for reading data from a certain flash.
 * @param  readAlignSize    [IN] Block align size when reading data from a certain flash.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_scatter.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V100R001C00
 */
extern VOID LOS_ScatterLoad(UINTPTR imageFlashAddr, SCATTER_FLASH_READ_FUNC flashReadFunc, size_t readAlignSize);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_SCATTER_H */
