/***************************************************************************//**
 * @file
 * @brief Driver for the Si7013 Temperature / Humidity sensor
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

#ifndef __SI7013_H
#define __SI7013_H

#include "em_device.h"
#include <stdbool.h>

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup Si7013
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/

/** I2C device address for Si7013 */
#define SI7013_ADDR      0x82
/** I2C device address for Si7021 */
#define SI7021_ADDR      0x80


/** Device ID value for Si7013 */
#define SI7013_DEVICE_ID 0x0D
/** Device ID value for Si7020 */
#define SI7020_DEVICE_ID 0x14
/** Device ID value for Si7021 */
#define SI7021_DEVICE_ID 0x21

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

int32_t Si7013_MeasureRHAndTemp(I2C_TypeDef *i2c, uint8_t addr,
                                 uint32_t *rhData, int32_t *tData);

int32_t Si7013_GetFirmwareRevision(I2C_TypeDef *i2c, uint8_t addr, uint8_t *fwRev);

bool Si7013_Detect(I2C_TypeDef *i2c, uint8_t addr, uint8_t *deviceId);
int32_t Si7013_ReadNoHoldRHAndTemp(I2C_TypeDef *i2c, uint8_t addr, uint32_t *rhData,
                                int32_t *tData);
int32_t Si7013_StartNoHoldMeasureRHAndTemp(I2C_TypeDef *i2c, uint8_t addr);
int32_t Si7013_MeasureV(I2C_TypeDef *i2c, uint8_t addr, int32_t *vData);
#ifdef __cplusplus
}
#endif

/** @} (end group Si7013) */
/** @} (end group Drivers) */
#endif /* __SI7013_H */
