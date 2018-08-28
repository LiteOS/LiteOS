/**************************************************************************//**
 * @file
 * @brief Capacitive sense driver
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



#ifndef __CAPLESENSE_H_
#define __CAPLESENSE_H_

#include <stdint.h>
#include <stdbool.h>

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup CapSense
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

uint8_t  CAPLESENSE_getSegmentChannel(uint8_t capSegment);
uint32_t CAPLESENSE_getVal(uint8_t channel);
uint32_t CAPLESENSE_getNormalizedVal(uint8_t channel);
int32_t CAPLESENSE_getSliderPosition(void);
void CAPLESENSE_Init(bool sleep);
void CAPLESENSE_setupLESENSE(bool sleep);
void CAPLESENSE_setupCallbacks(void (*scanCb)(void), void (*chCb)(void));
void CAPLESENSE_Sleep(void);

#ifdef __cplusplus
}
#endif

/** @} (end group CapSense) */
/** @} (end group Drivers) */

#endif /* __CAPSENSE_H_ */
