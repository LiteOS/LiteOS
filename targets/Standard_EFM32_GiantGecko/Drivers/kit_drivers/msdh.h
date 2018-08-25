/***************************************************************************//**
 * @file  msdh.h
 * @brief Host side implementation of Mass Storage class Device (MSD) interface.
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


#ifndef __MSDH_H
#define __MSDH_H

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup Msd
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*** MSDH Function prototypes ***/

bool MSDH_Init(uint8_t *usbDeviceInfo, int usbDeviceInfoSize);
bool MSDH_GetSectorCount(uint32_t *sectorCount);
bool MSDH_GetSectorSize(uint16_t *sectorSize);
bool MSDH_GetBlockSize(uint32_t *blockSize);
bool MSDH_ReadSectors(uint32_t lba, uint16_t sectors, void *data);
bool MSDH_WriteSectors(uint32_t lba, uint16_t sectors, const void *data);

#ifdef __cplusplus
}
#endif

/** @} (end group Msd) */
/** @} (end group Drivers) */

#endif /* __MSDH_H */
