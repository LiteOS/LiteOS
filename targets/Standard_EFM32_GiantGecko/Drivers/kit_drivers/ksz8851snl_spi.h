/**************************************************************************//**
 * @file
 * @brief SPI interface API for KSZ8851SNL Ethernet controller
 * @version 5.0.0
 ******************************************************************************
 * @section License
 * <b>Copyright 2015 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#ifndef KSZ8851SNL_SPI_H__
#define KSZ8851SNL_SPI_H__
#include <stdint.h>

/**************************************************************************//**
* @addtogroup kitdrv
* @{
******************************************************************************/

/**************************************************************************//**
* @addtogroup ksz8851snl
* @{
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void KSZ8851SNL_SPI_Init(void);
uint16_t KSZ8851SNL_SPI_ReadRegister(uint8_t reg);
void KSZ8851SNL_SPI_WriteRegister(uint8_t reg, uint16_t value);
void KSZ8851SNL_SPI_ReadFifo(int numBytes, uint8_t *data);
void KSZ8851SNL_SPI_WriteFifoBegin(void);
void KSZ8851SNL_SPI_WriteFifo(int numBytes, const uint8_t *data);
void KSZ8851SNL_SPI_WriteFifoEnd(void);

#ifdef __cplusplus
}
#endif

/** @} (end group EthSpi) */
/** @} (end group Drivers) */

#endif
