/**************************************************************************//**
 * @file
 * @brief Swipe algorithm for Si114x
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


#ifndef __SI114X_ALGORITHM_H
#define __SI114X_ALGORITHM_H

#include "em_device.h"
#include "si114x_functions.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup Si114x
 * @{
 ******************************************************************************/

/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/
/** I2C device address for Si1147 on weather station board. */
#define SI1147_ADDR    0xc0

/*******************************************************************************
 *******************************   STRUCTS   ***********************************
 ******************************************************************************/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/** Interrupt Sample */
typedef struct
{
  u32 timestamp;         /* Timestamp to record */
  u16 vis;               /* VIS */
  u16 ir;                /* IR */
  u16 ps1;               /* PS1 */
  u16 ps2;               /* PS2 */
  u16 ps3;               /* PS3 */
  u16 aux;               /* AUX */
} Si114x_Sample_TypeDef;

/** @endcond */

/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/
/** Si114x gestures */
typedef enum
{
  NONE,
  UP,
  DOWN,
  LEFT,
  RIGHT,
  TAP
} gesture_t;

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/
gesture_t Si1147_NewSample(I2C_TypeDef *i2c, uint8_t addr, uint32_t timestamp);
int Si1147_ConfigureDetection(I2C_TypeDef *i2c, uint8_t addr, int slow);
int Si1147_SetInterruptOutputEnable(I2C_TypeDef *i2c, uint8_t addr, int enable);
int Si1147_GetInterruptOutputEnable(I2C_TypeDef *i2c, uint8_t addr, int *enable);
int Si1147_Detect_Device(I2C_TypeDef *i2c, uint8_t addr);
int Si1147_MeasureUVAndObjectPresent(I2C_TypeDef *i2c, uint8_t addr, uint16_t *uvIndex, int* objectDetect);

/** @} (end addtogroup Drivers) */
/** @} (end addtogroup Si114x) */

#ifdef __cplusplus
}
#endif

#endif /* #define SI114X_ALGORITHM_H */
