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

#include <stddef.h>
#include "si7013.h"
#include "i2cspm.h"

#include "stddef.h"

/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/** Si7013 Read Temperature Command */
#define SI7013_READ_TEMP       0xE0  /* Read previous T data from RH measurement
                                      * command*/
/** Si7013 Read RH Command */
#define SI7013_READ_RH         0xE5  /* Perform RH (and T) measurement. */
/** Si7013 Read RH (no hold) Command */
#define SI7013_READ_RH_NH      0xF5  /* Perform RH (and T) measurement in no hold mode. */
/** Si7013 Read Thermistor Command */
#define SI7013_READ_VIN        0xEE  /* Perform thermistor measurement. */
/** Si7013 Read ID */
#define SI7013_READ_ID1_1      0xFA
#define SI7013_READ_ID1_2      0x0F
#define SI7013_READ_ID2_1      0xFc
#define SI7013_READ_ID2_2      0xc9
/** Si7013 Read Firmware Revision */
#define SI7013_READ_FWREV_1    0x84
#define SI7013_READ_FWREV_2    0xB8

/** @endcond */

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/


/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
/**************************************************************************//**
 * @brief
 *  Reads data from the Si7013 sensor.
 * @param[in] i2c
 *   The I2C peripheral to use (not used).
 * @param[in] addr
 *   The I2C address of the sensor.
 * @param[out] data
 *   The data read from the sensor.
 * @param[in] command
 *   The command to send to device. See the \#define's for details.
 * @return
 *   Returns number of bytes read on success. Otherwise returns error codes
 *   based on the I2CDRV.
 *****************************************************************************/
static int32_t Si7013_Measure(I2C_TypeDef *i2c, uint8_t addr, uint32_t *data,
                              uint8_t command)
{
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t                    i2c_read_data[2];
  uint8_t                    i2c_write_data[1];

  seq.addr  = addr;
  seq.flags = I2C_FLAG_WRITE_READ;
  /* Select command to issue */
  i2c_write_data[0] = command;
  seq.buf[0].data   = i2c_write_data;
  seq.buf[0].len    = 1;
  /* Select location/length of data to be read */
  seq.buf[1].data = i2c_read_data;
  seq.buf[1].len  = 2;

  ret = I2CSPM_Transfer(i2c, &seq);

  if (ret != i2cTransferDone)
  {
    *data = 0;
    return((int) ret);
  }

  *data = ((uint32_t) i2c_read_data[0] << 8) + (i2c_read_data[1] & 0xfc);

  return((int) 2);
}
/** @endcond */

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
/**************************************************************************//**
 * @brief
 *  Starts a no hold measurement in the Si7013 sensor.
 * @param[in] i2c
 *   The I2C peripheral to use (not used).
 * @param[in] addr
 *   The I2C address of the sensor.
 * @param[in] command
 *   The command to send to device. See the \#define's for details.
 * @return
 *   Returns number of bytes read on success. Otherwise returns error codes
 *   based on the I2CDRV.
 *****************************************************************************/
static int32_t Si7013_StartNoHoldMeasure(I2C_TypeDef *i2c, uint8_t addr, uint8_t command)

{
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t                    i2c_read_data[2];
  uint8_t                    i2c_write_data[1];

  seq.addr  = addr;
  seq.flags = I2C_FLAG_WRITE;
  /* Select command to issue */
  i2c_write_data[0] = command;
  seq.buf[0].data   = i2c_write_data;
  seq.buf[0].len    = 1;
  /* Select location/length of data to be read */
  seq.buf[1].data = i2c_read_data;
  seq.buf[1].len  = 0;

  ret = I2CSPM_Transfer(i2c, &seq);

  if (ret != i2cTransferDone)
  {
    return((int) ret);
  }

  return((int) 0);
}
/** @endcond */


/**************************************************************************//**
 * @brief
 *  Reads Firmware Revision from a Si7013 sensor.
 * @param[in] i2c
 *   The I2C peripheral to use.
 * @param[in] addr
 *   The I2C address of the sensor.
 * @param[out] fwRev
 *   The internal firmware revision. 0xFF === 1.0
 * @return
 *   Returns zero on OK, non-zero otherwise.
 *****************************************************************************/
int32_t Si7013_GetFirmwareRevision(I2C_TypeDef *i2c, uint8_t addr, uint8_t *fwRev)
{
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t                    i2c_write_data[2];
  uint8_t                    i2c_read_data[1];

  seq.addr  = addr;
  seq.flags = I2C_FLAG_WRITE_READ;
  /* Select command to issue */
  i2c_write_data[0] = SI7013_READ_FWREV_1;
  i2c_write_data[1] = SI7013_READ_FWREV_2;
  seq.buf[0].data   = i2c_write_data;
  seq.buf[0].len    = 2;
  /* Select location/length of data to be read */
  seq.buf[1].data = i2c_read_data;
  seq.buf[1].len  = 1;

  ret = I2CSPM_Transfer(i2c, &seq);

  if (ret != i2cTransferDone)
  {
    *fwRev = 0;
    return (uint32_t) ret;
  }
  *fwRev = i2c_read_data[0];

  return (uint32_t) i2cTransferDone;
}

/**************************************************************************//**
 * @brief
 * Starts no hold measurement of relative humidity and temperature from a Si7013 sensor.
 * @param[in] i2c
 *   The I2C peripheral to use.
 * @param[in] addr
 *   The I2C address of the sensor.
 * @return
 *   Returns zero on OK, non-zero otherwise.
 *****************************************************************************/
int32_t Si7013_StartNoHoldMeasureRHAndTemp(I2C_TypeDef *i2c, uint8_t addr)
{
  int ret = Si7013_StartNoHoldMeasure(i2c, addr, SI7013_READ_RH_NH);

  return ret;
}

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
/**************************************************************************//**
 * @brief
 *  Reads data from the Si7013 sensor.
 * @param[in] i2c
 *   The I2C peripheral to use (not used).
 * @param[in] addr
 *   The I2C address of the sensor.
 * @param[out] data
 *   The data read from the sensor.
 * @return
 *   Returns number of bytes read on success. Otherwise returns error codes
 *   based on the I2CDRV.
 *****************************************************************************/
static int32_t Si7013_ReadNoHoldData(I2C_TypeDef *i2c, uint8_t addr, uint32_t *data)
{
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t                    i2c_read_data[2];

  seq.addr  = addr;
  seq.flags = I2C_FLAG_READ;
  /* Select command to issue */
  seq.buf[0].data = i2c_read_data;
  seq.buf[0].len  = 2;
  /* Select location/length of data to be read */
  seq.buf[1].data = i2c_read_data;
  seq.buf[1].len  = 2;

  ret = I2CSPM_Transfer(i2c, &seq);

  if (ret != i2cTransferDone)
  {
    *data = 0;
    return((int) ret);
  }

  *data = ((uint32_t) i2c_read_data[0] << 8) + (i2c_read_data[1] & 0xfc);

  return((int) 2);
}
/** @endcond */

/**************************************************************************//**
 * @brief
 *  Reads relative humidity and temperature from a Si7013 sensor.
 * @param[in] i2c
 *   The I2C peripheral to use.
 * @param[in] addr
 *   The I2C address of the sensor.
 * @param[out] rhData
 *   The relative humidity in percent (multiplied by 1000).
 * @param[out] tData
 *   The temperature in milli-Celsius.
 * @return
 *   Returns zero on OK, non-zero otherwise.
 *****************************************************************************/
int32_t Si7013_ReadNoHoldRHAndTemp(I2C_TypeDef *i2c, uint8_t addr, uint32_t *rhData,
                                   int32_t *tData)
{
  int ret = Si7013_ReadNoHoldData(i2c, addr, rhData);

  if (ret == 2)
  {
    /* convert to milli-percent */
    *rhData = (((*rhData) * 15625L) >> 13) - 6000;
  }
  else
  {
    return -1;
  }

  ret = Si7013_Measure(i2c, addr, (uint32_t *) tData, SI7013_READ_TEMP);

  if (ret == 2)
  {
    *tData = (((*tData) * 21965L) >> 13) - 46850; /* convert to milli-degC */
  }
  else
  {
    return -1;
  }

  return 0;
}

/**************************************************************************//**
 * @brief
 *  Reads relative humidity and temperature from a Si7013 sensor.
 * @param[in] i2c
 *   The I2C peripheral to use.
 * @param[in] addr
 *   The I2C address of the sensor.
 * @param[out] rhData
 *   The relative humidity in percent (multiplied by 1000).
 * @param[out] tData
 *   The temperature in milli-Celsius.
 * @return
 *   Returns zero on OK, non-zero otherwise.
 *****************************************************************************/
int32_t Si7013_MeasureRHAndTemp(I2C_TypeDef *i2c, uint8_t addr, uint32_t *rhData,
                                int32_t *tData)
{
  int ret = Si7013_Measure(i2c, addr, rhData, SI7013_READ_RH);

  if (ret == 2)
  {
    /* convert to milli-percent */
    *rhData = (((*rhData) * 15625L) >> 13) - 6000;
  }
  else
  {
    return -1;
  }

  ret = Si7013_Measure(i2c, addr, (uint32_t *) tData, SI7013_READ_TEMP);

  if (ret == 2)
  {
    *tData = (((*tData) * 21965L) >> 13) - 46850; /* convert to milli-degC */
  }
  else
  {
    return -1;
  }

  return 0;
}

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
static int32_t Si7013_WriteUserReg2(I2C_TypeDef *i2c, uint8_t addr, int8_t data)
{
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t                    i2c_read_data[2];
  uint8_t                    i2c_write_data[2];

  seq.addr  = addr;
  seq.flags = I2C_FLAG_WRITE;
  /* Select command to issue */
  i2c_write_data[0] = 0x50;
  i2c_write_data[1] = data;
  seq.buf[0].data   = i2c_write_data;
  seq.buf[0].len    = 2;
  /* Select location/length of data to be read */
  seq.buf[1].data = i2c_read_data;
  seq.buf[1].len  = 0;

  ret = I2CSPM_Transfer(i2c, &seq);

  if (ret != i2cTransferDone)
  {
    return((int) ret);
  }

  return((int) 0);
}
/** @endcond */


/**************************************************************************//**
 * @brief
 *  Reads relative humidity and temperature from a Si7013 sensor.
 * @param[in] i2c
 *   The I2C peripheral to use.
 * @param[in] addr
 *   The I2C address of the sensor.
 * @param[out] vData
 *   The data read from the sensor.
 * @return
 *   Returns zero on OK, non-zero otherwise.
 *****************************************************************************/
int32_t Si7013_MeasureV(I2C_TypeDef *i2c, uint8_t addr, int32_t *vData)
{
  int ret;
  Si7013_WriteUserReg2(i2c, addr, 0x0e);
  ret = Si7013_Measure(i2c, addr, (uint32_t *) vData, SI7013_READ_VIN);
  Si7013_WriteUserReg2(i2c, addr, 0x09);
  if (ret == 2)
  {
    /* convert  */
  }
  else
  {
    return -1;
  }
  return 0;
}


/**************************************************************************//**
 * @brief
 *   Checks if a Si7013 is present on the I2C bus or not.
 * @param[in] i2c
 *   The I2C peripheral to use (Not used).
 * @param[in] addr
 *   The I2C address to probe.
 * @param[out] deviceId
 *   Write device ID from SNB_3 if device reponds. Pass in NULL to discard.
 *   Should be 0x0D for Si7013, 0x14 for Si7020 or 0x15 for Si7021
 * @return
 *   True if a Si7013 is detected, false otherwise.
 *****************************************************************************/
bool Si7013_Detect(I2C_TypeDef *i2c, uint8_t addr, uint8_t *deviceId)
{
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t                    i2c_read_data[8];
  uint8_t                    i2c_write_data[2];

  seq.addr  = addr;
  seq.flags = I2C_FLAG_WRITE_READ;
  /* Select command to issue */
  i2c_write_data[0] = SI7013_READ_ID2_1;
  i2c_write_data[1] = SI7013_READ_ID2_2;
  seq.buf[0].data   = i2c_write_data;
  seq.buf[0].len    = 2;
  /* Select location/length of data to be read */
  seq.buf[1].data = i2c_read_data;
  seq.buf[1].len  = 8;

  ret = I2CSPM_Transfer(i2c, &seq);
  if (ret != i2cTransferDone)
  {
    return false;
  }
  if (NULL != deviceId)
  {
    *deviceId = i2c_read_data[0];
  }
  return true;
}
