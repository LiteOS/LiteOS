/***************************************************************************//**
 * @file
 * @brief Driver for the Si1147 Proximity sensor
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


#ifndef __SI1147_H
#define __SI1147_H

#include "em_device.h"

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup Si114x
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 *******************************   STRUCTS   ***********************************
 ******************************************************************************/
/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

typedef struct {
    I2C_TypeDef *i2c;
    uint8_t addr;
  } si114x_i2c_t;

/** @endcond */


/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/
uint32_t Si1147_Write_Register (I2C_TypeDef *i2c,uint8_t addr, uint8_t reg, uint8_t data);
uint32_t Si1147_Write_Block_Register (I2C_TypeDef *i2c,uint8_t addr, uint8_t reg, uint8_t length, uint8_t const *data);
uint32_t Si1147_Read_Block_Register (I2C_TypeDef *i2c,uint8_t addr, uint8_t reg, uint8_t length, uint8_t  *data);
uint32_t Si1147_Read_Register (I2C_TypeDef *i2c,uint8_t addr, uint8_t reg, uint8_t *data);


#ifdef __cplusplus
}
#endif

/** @} (end group Si114x) */
/** @} (end group Drivers) */

#endif /* __TEMPSENS_H */
