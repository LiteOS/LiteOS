/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Hal Rng HeadFile
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
 * @defgroup hal_spi_flash SPI Flash Interface
 * @ingroup hal
 */

#ifndef _HAL_SPI_FLASH_H
#define _HAL_SPI_FLASH_H

#include <stdint.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 *@ingroup hal_spi_flash
 *@brief config the spi flash.
 *
 *@par Description:
 *This API is used to config the spi flash.
 *@attention none.
 *
 *@param none.
 *
 *@retval none.
 *@par Dependency: none.
 *@see none.
 */
void hal_spi_flash_config(void);

/**
 *@ingroup hal_spi_flash
 *@brief erase data of spi flash.
 *
 *@par Description:
 *This API is used to erase data of spi flash.
 *@attention none.
 *
 *@param addr           [IN] the address of the spi flash to erase.
 *@param len            [IN] the number of bytes to be erased from addr.
                             Note that at lease 4K byte will be erased by once.
 *
 *@retval #int          0 if succeed or -1 if failed.
 *@par Dependency: none.
 *@see none.
 */
int hal_spi_flash_erase(uint32_t addr, int32_t len);

/**
 *@ingroup hal_spi_flash
 *@brief write data to spi flash.
 *
 *@par Description:
 *This API is used to write data to spi flash. You should call @hal_spi_flash_erase before this.
 *location is updated by each call so that you don't need to care about write address if you do
 *sequential write.
 *@attention none.
 *
 *@param buf            [IN]     the data to be wrote to spi flash.
 *@param len            [IN]     the length of the buffer.
 *@param location       [IN/OUT] the address of the spi flash to write.
 *
 *@retval #int          0 if succeed or -1 if failed.
 *@par Dependency: none.
 *@see hal_spi_flash_erase.
 */
int hal_spi_flash_write(const void* buf, int32_t len, uint32_t* location);

/**
 *@ingroup hal_spi_flash
 *@brief write data to spi flash.
 *
 *@par Description:
 *This API is used to write data to spi flash. You don't need to erase flash by this interface.
 *@attention none.
 *
 *@param buf            [IN] the data to be wrote to spi flash.
 *@param len            [IN] the length of the buffer.
 *@param location       [IN] the address of the spi flash to write.
 *
 *@retval #int          0 if succeed or -1 if failed.
 *@par Dependency: none.
 *@see none.
 */
int hal_spi_flash_erase_write(const void* buf, int32_t len, uint32_t location);

/**
 *@ingroup hal_spi_flash
 *@brief read data from spi flash.
 *
 *@par Description:
 *This API is used to read data from spi flash.
 *@attention none.
 *
 *@param buf            [OUT] buffer to store the data read from spi flash.
 *@param len            [IN]  the length of the buffer.
 *@param location       [IN]  the address of the spi flash to read.
 *
 *@retval #int          0 if succeed or -1 if failed.
 *@par Dependency: none.
 *@see none.
 */
int hal_spi_flash_read(void* buf, int32_t len, uint32_t location);

/**
 *@ingroup hal_spi_flash
 *@brief get ID of the target spi flash.
 *
 *@par Description:
 *This API is used to get ID of the target spi flash.
 *@attention none.
 *
 *@param none.
 *
 *@retval #int          ID of the spi flash.
 *@par Dependency: none.
 *@see none.
 */
int hal_spi_flash_get_id(void);

/**
 *@ingroup hal_spi_flash
 *@brief power down the spi flash.
 *
 *@par Description:
 *This API is used to power down the spi flash.
 *@attention none.
 *
 *@param none.
 *
 *@retval none.
 *@par Dependency: none.
 *@see none.
 */
void hal_spi_flash_power_down(void);

/**
 *@ingroup hal_spi_flash
 *@brief wake up the spi flash.
 *
 *@par Description:
 *This API is used to wake up the spi flash.
 *@attention none.
 *
 *@param none.
 *
 *@retval none.
 *@par Dependency: none.
 *@see none.
 */
void hal_spi_flash_wake_up(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif  /* _HAL_SPI_FLASH_H */
