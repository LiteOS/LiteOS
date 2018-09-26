/***************************************************************************//**
 * @file  norflash.h
 * @brief Definitions for the NORFLASH driver for Spansion S29GL128P90FFIR13.
 * @version 5.0.0
 *******************************************************************************
 * @section License
 * <b>Copyright 2015 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/


#ifndef __NORFLASH_H
#define __NORFLASH_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup NorFlash
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/** @brief NORFLASH status enumerator. */
typedef enum
{
  NORFLASH_STATUS_OK           = 0,             /**< No errors detected.                      */
  NORFLASH_NONUNIFORM_GEOMETRY = -1,            /**< The flash has non-uniform sector scheme. */
  NORFLASH_NOT_CFI_DEVICE      = -2,            /**< The flash is not CFI compliant.          */
  NORFLASH_WRITE_FAILURE       = -3,            /**< Flash write/erase failure.               */
  NORFLASH_WRITE_TIMEOUT       = -4,            /**< Flash write/erase timeout.               */
  NORFLASH_INVALID_ADDRESS     = -5,            /**< Invalid flash address.                   */
  NORFLASH_MISALIGNED_ADDRESS  = -6,            /**< Misaligned flash word address.           */
} NORFLASH_Status_TypeDef;

/** @brief NORFLASH device information struct. */
typedef struct
{
  uint32_t baseAddress;               /**< The device base address in cpu memory map. */
  uint32_t deviceId;                  /**< The device ID.                             */
  uint32_t deviceSize;                /**< Total device size in bytes.                */
  uint32_t sectorCount;               /**< Number of sectors in the device.           */
  uint32_t sectorSize;                /**< Sector size in bytes.                      */
  uint16_t manufacturerId;            /**< The device manufacturer ID.                */
} NORFLASH_Info_TypeDef;

bool                    NORFLASH_AddressValid(uint32_t addr);
NORFLASH_Info_TypeDef*  NORFLASH_DeviceInfo(void);
int                     NORFLASH_EraseDevice(void);
int                     NORFLASH_EraseSector(uint32_t addr);
int                     NORFLASH_Init(void);
int                     NORFLASH_ProgramByte(uint32_t addr, uint8_t data);
int                     NORFLASH_ProgramWord16(uint32_t addr, uint16_t data);
int                     NORFLASH_ProgramWord32(uint32_t addr, uint32_t data);
int                     NORFLASH_Program(uint32_t addr, uint8_t *data, uint32_t count);

#ifdef __cplusplus
}
#endif

/** @} (end group NorFlash) */
/** @} (end group Drivers) */

#endif /* __NORFLASH_H */
