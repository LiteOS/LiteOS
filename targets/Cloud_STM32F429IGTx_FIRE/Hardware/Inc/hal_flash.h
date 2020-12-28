/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Hal Flash HeadFile
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
 * @defgroup hal Hardware Abstract Layer
 * @defgroup hal_flash Flash Interface
 * @ingroup hal
 */

#ifndef _HAL_FLASH_H
#define _HAL_FLASH_H

#include <stdint.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup hal_flash
 * @brief read data from flash.
 *
 * @par Description:
 * This API is used to read data from flash.
 * @attention none.
 *
 * @param buf            [OUT] buffer to store the data read from flash.
 * @param len            [IN]  the length of the buffer.
 * @param location       [IN]  the address of the flash to read.
 *
 * @retval #int          0 if succeed or -1 if failed.
 * @par Dependency: none.
 * @see none.
 */
int hal_flash_read(void *buf, int32_t len, uint32_t location);

/**
 *@ingroup hal_flash
 *@brief erase data of flash.
 *
 *@par Description:
 *This API is used to erase data of flash.
 *@attention none.
 *
 *@param addr           [IN] the address of the flash to erase.
 *@param len            [IN] the number of bytes to be erased from addr.
                             Note that the whole sector will be erased
                             if addr is in this sector.
 *
 *@retval #int          0 if succeed or -1 if failed.
 *@par Dependency: none.
 *@see none.
 */
int hal_flash_erase(uint32_t addr, int32_t len);

/**
 * @ingroup hal_flash
 * @brief write data to flash.
 *
 * @par Description:
 * This API is used to write data to flash. You should call @hal_flash_erase before this.
 * location is updated by each call so that you don't need to care about write address
 * if you do sequential write.
 * @attention none.
 *
 * @param buf            [IN]     the data to be wrote to flash.
 * @param len            [IN]     the length of the buffer.
 * @param location       [IN/OUT] the address of the flash to write.
 *
 * @retval #int          0 if succeed or -1 if failed.
 * @par Dependency: none.
 * @see hal_flash_erase.
 */
int hal_flash_write(const void *buf, int32_t len, uint32_t *location);

/**
 * @ingroup hal_flash
 * @brief write data to flash.
 *
 * @par Description:
 * This API is used to write data to flash. You don't need to erase or lock flash by this interface.
 * @attention none.
 *
 * @param buf            [IN] the data to be wrote to flash.
 * @param len            [IN] the length of the buffer.
 * @param location       [IN] the address of the flash to write.
 *
 * @retval #int          0 if succeed or -1 if failed.
 * @par Dependency: none.
 * @see none.
 */
int hal_flash_erase_write(const void *buf, int32_t len, uint32_t location);

/**
 * @ingroup hal_flash
 * @brief lock flash to prevent data from being destroyed.
 *
 * @par Description:
 * This API is used to lock flash to prevent data from being destroyed.
 * @attention none.
 *
 * @param none.
 *
 * @retval none.
 * @par Dependency: none.
 * @see none.
 */
void hal_flash_lock(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _HAL_FLASH_H */
