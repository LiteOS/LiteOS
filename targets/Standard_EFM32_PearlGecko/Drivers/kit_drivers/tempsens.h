/***************************************************************************//**
 * @file
 * @brief Temperature sensor driver for DS75 temperature sensor compatible
 *   device on the DK.
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


#ifndef __TEMPSENS_H
#define __TEMPSENS_H

#include "em_device.h"

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup TempSensor
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/

/** I2C device address for temperature sensor on DK */
#define TEMPSENS_DK_ADDR    0x90


/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/

/** Available registers in DS75 sensor device */
typedef enum
{
  tempsensRegTemp       = 0,   /**< Temperature register (read-only) */
  tempsensRegConfig     = 1,   /**< Configuration register */
  tempsensRegHysteresis = 2,   /**< Hysteresis register */
  tempsensRegShutdown   = 3    /**< Overtemperature shutdown register */
} TEMPSENS_Register_TypeDef;


/*******************************************************************************
 *******************************   STRUCTS   ***********************************
 ******************************************************************************/

/** Structure used to fetch temperature using integer numbers. */
typedef struct
{
  /** Integer part of temperature, including sign */
  int16_t i;

  /**
   * Fractional part of temperature in 1/10000 parts, including sign. Ie 0.5
   * will be stored as 5000 and -0.5 as -5000.
   */
  int16_t f;
} TEMPSENS_Temp_TypeDef;


/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

void TEMPSENS_Celsius2Fahrenheit(TEMPSENS_Temp_TypeDef *temp);
int TEMPSENS_RegisterGet(I2C_TypeDef *i2c,
                         uint8_t addr,
                         TEMPSENS_Register_TypeDef reg,
                         uint16_t *val);
int TEMPSENS_RegisterSet(I2C_TypeDef *i2c,
                         uint8_t addr,
                         TEMPSENS_Register_TypeDef reg,
                         uint16_t val);
int TEMPSENS_TemperatureGet(I2C_TypeDef *i2c,
                            uint8_t addr,
                            TEMPSENS_Temp_TypeDef *temp);

#ifdef __cplusplus
}
#endif

/** @} (end group TempSensor) */
/** @} (end group Drivers) */

#endif /* __TEMPSENS_H */
