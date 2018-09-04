/**************************************************************************//**
 * @brief Implementation specific functions for HRM code
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
#ifndef SI114X_SYS_OUT
#define SI114X_SYS_OUT

#include "em_i2c.h"

typedef struct Si114xPortConfig
{
  I2C_TypeDef       *i2cPort;   /**< I2C port Si114x is connected to */
  uint8_t           i2cAddress; /**< I2C address of Si114x */
  GPIO_Port_TypeDef irqPort;    /**< Port for Si114x INT pin */
  int               irqPin;     /**< Pin for Si114x INT pin */
} Si114xPortConfig_t;

int16_t Si114xProcessIrq(void* si114x_handle, uint16_t timestamp);

#endif
