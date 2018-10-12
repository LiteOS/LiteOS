/**************************************************************************//**
 * @file
 * @brief Driver for retrieving capsense button and slider inputs from CPT112S
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

#ifndef CPT112S_I2C_H
#define CPT112S_I2C_H

#include <stdint.h>

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup CPT112S_I2C
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/

#define CPT112S_I2C_ADDRESS         0xE0
#define CPT112S_I2C_RXBUFFER_SIZE   3

#define CPT112S_I2C_TOUCH_EVENT     0
#define CPT112S_I2C_RELEASE_EVENT   1
#define CPT112S_I2C_SLIDER_ACTIVITY 2

/*******************************************************************************
 *******************************  FUNCTIONS  ***********************************
 ******************************************************************************/

/******************************************************************************
 * @brief Return a bitmask containing the current state for all capsense
 * buttons
 *****************************************************************************/
uint16_t CPT112S_getCapsenseCurrent(void);

/******************************************************************************
 * @brief Return a bitmask containing the previous state for all capsense
 * buttons
 *****************************************************************************/
uint16_t CPT112S_getCapsensePrevious(void);

/******************************************************************************
 * @brief Return the current slider value
 *****************************************************************************/
uint16_t CPT112S_getSliderCurrent(void);

/******************************************************************************
 * @brief Return the previous slider value
 *****************************************************************************/
uint16_t CPT112S_getSliderPrevious(void);

/******************************************************************************
 * SMBUS data structure initialization
 *****************************************************************************/
void CPT112S_init(void);

/******************************************************************************
 * update SMBUS data structure from current packet of I2C
 *****************************************************************************/
void CPT112S_update(void);

#ifdef __cplusplus
}
#endif

/** @} (end group CPT112S_I2C) */
/** @} (end group Drivers) */

#endif /* CPT112S_I2C_H */
