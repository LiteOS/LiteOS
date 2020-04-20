/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Runstop HeadFile
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
 * @defgroup los_runstop Wifi wake-up
 * @ingroup kernel
 */

#ifndef _LOS_RUNSTOP_H
#define _LOS_RUNSTOP_H

#include "los_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup  los_runstop
 * @brief Define the type of a runstop callback function.
 *
 * @par Description:
 * This API is used to define the type of a runstop callback function,
 * so that it can be called in the phase of wifi wake-up.
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
 * <ul><li>los_runstop.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V100R001C00
 */
typedef VOID (*RUNSTOP_CALLBACK_FUNC)(VOID);

/**
 * @ingroup  los_runstop
 * @brief Define the type of a flash-reading function.
 *
 * @par Description:
 * This API is used to define the type of a flash-reading function,
 * so that it can be called to read data from a certain flash.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @param  memAddr [IN] The memory starting address where to write the data reading from a certain flash.
 * @param  start   [IN] The starting address to read data from a certain flash.
 * @param  size    [IN] The size of data reading from a certain flash.
 *
 * @retval #INT32         The size of data read from flash.
 * @par Dependency:
 * <ul><li>los_runstop.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V100R001C00
 */
typedef INT32 (*FLASH_READ_FUNC)(VOID *memAddr, size_t start, size_t size);

/**
 * @ingroup  los_runstop
 * @brief Define the type of a flash-writing function.
 *
 * @par Description:
 * This API is used to define the type of a flash-writing function,
 * so that it can be called to write data from a certain flash.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @param  memAddr [IN] The memory starting address where to read data and be wrote to a certain flash.
 * @param  start   [IN] The starting address of a certain flash to write data.
 * @param  size    [IN] The size of data writing to a certain flash.
 *
 * @retval #INT32         The size of data write to flash.
 * @par Dependency:
 * <ul><li>los_runstop.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V100R001C00
 */
typedef INT32 (*FLASH_WRITE_FUNC)(VOID *memAddr, size_t start, size_t size);

/**
 * @ingroup los_runstop
 * Define the structure of the parameters used for runstop.
 *
 * Information of specified parameters passed in during runstop.
 */
typedef struct tagRunstopParam {
    RUNSTOP_CALLBACK_FUNC pfIdleWakeupCallback; /**< Wake up callback function, called in idle task just after wake up
                                                     from runstop wifi memory mirroring */
    RUNSTOP_CALLBACK_FUNC pfWakeupCallback;     /**< Wake up callback function, called just after wake up from runstop
                                                     wifi memory mirroring */
    RUNSTOP_CALLBACK_FUNC pfImageDoneCallback;  /**< Image done callback function, called just after wifi memory
                                                     mirroring is stored */
    FLASH_READ_FUNC pfFlashReadFunc;            /**< An abstract function for reading data from a certain flash */
    FLASH_WRITE_FUNC pfFlashWriteFunc;          /**< An abstract function for writing data to a certain flash */
    size_t uwFlashReadAlignSize;                /**< Block align size when reading data from a certain flash */
    size_t uwFlashEraseAlignSize;               /**< Block align size when erasing data in a certain flash */
    size_t uwFlashWriteAlignSize;               /**< Block align size when writing data to a certain flash */
    UINTPTR uwWowFlashAddr;                     /**< The flash starting address to which the wifi memory mirroring
                                                     is to be stored, the address should be flash write-alignment
                                                     length aligned */
    UINTPTR uwImageFlashAddr;                  /**< The flash starting address where the whole application image
                                                     is burned */
} RUNSTOP_PARAM_S;

/**
 * @ingroup los_runstop
 * @brief Store the wifi memory mirroring on a flash.
 *
 * @par Description:
 * This API is used to store the wifi memory mirroring to the flash starting address passed in by a user.
 * @attention
 * <ul>
 * <li>uwWowFlashAddr in runstopParam specifies the flash starting address to which the wifi memory mirroring
 * is to be stored.It needs to be ensured that the flash starting address and the flash of the wifi memory
 * mirroring size after the starting address are not occupied.</li>
 * </ul>
 *
 * @param runstopParam [IN] Parameter which contains key information for runstop to use, including flash starting
 *                          address to which the wifi memory mirroring is to be stored, etc.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_runstop.h: the header file that contains the API declaration.</li></ul>
 * @see
 * @since Huawei LiteOS V100R001C00
 */
extern VOID LOS_MakeImage(RUNSTOP_PARAM_S *runstopParam);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_RUNSTOP_H */
