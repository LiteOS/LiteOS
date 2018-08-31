/**************************************************************************//**
 * @file
 * @brief EFM32GG_DK3750, TFT Initialization and setup for Adress Mapped mode
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


#ifndef __TFTAMAPPED_H
#define __TFTAMAPPED_H

#include <stdbool.h>

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup Tft
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

bool TFT_AddressMappedInit(void);

#ifdef __cplusplus
}
#endif

/** @} (end group Tft) */
/** @} (end group Drivers) */

#endif
