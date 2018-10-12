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

#include "i2cspm.h"
#include "tempsens.h"

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   Convert temperature from Celsius to Fahrenheit.
 *
 * @param[in,out] temp
 *   Reference to structure to holding temperature in Celsius to convert. Upon
 *   return, it has been changed to Fahrenheit.
 ******************************************************************************/
void TEMPSENS_Celsius2Fahrenheit(TEMPSENS_Temp_TypeDef *temp)
{
  int32_t convert;

  /* Use integer arithmetic. Values are sufficiently low, so int32 will */
  /* not overflow. */
  convert  = (int32_t)(temp->i) * 10000;
  convert += (int32_t)(temp->f);
  convert  = ((convert * 9) / 5) + (32 * 10000);

  temp->i = (int16_t)(convert / 10000);
  temp->f = (int16_t)(convert % 10000);
}


/***************************************************************************//**
 * @brief
 *   Read sensor register content.
 *
 * @details
 *   If reading the temperature register, when a measurement is completed inside
 *   the sensor device, the new measurement may not be stored. For this reason,
 *   the temperature should not be polled with a higher frequency than the
 *   measurement conversion time for a given resolution configuration. Please
 *   refer to sensor device datasheet.
 *
 * @param[in] i2c
 *   Pointer to I2C peripheral register block.
 *
 * @param[in] addr
 *   I2C address for temperature sensor, in 8 bit format, where LSB is reserved
 *   for R/W bit.
 *
 * @param[in] reg
 *   Register to read.
 *
 * @param[out] val
 *   Reference to place register read.
 *
 * @return
 *   Returns 0 if register read, <0 if unable to read register.
 ******************************************************************************/
int TEMPSENS_RegisterGet(I2C_TypeDef *i2c,
                         uint8_t addr,
                         TEMPSENS_Register_TypeDef reg,
                         uint16_t *val)
{
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t                    regid[1];
  uint8_t                    data[2];

  seq.addr  = addr;
  seq.flags = I2C_FLAG_WRITE_READ;
  /* Select register to be read */
  regid[0]        = ((uint8_t) reg) & 0x3;
  seq.buf[0].data = regid;
  seq.buf[0].len  = 1;
  /* Select location/length to place register */
  if (reg == tempsensRegConfig)
  {
    /* Only 1 byte reg, clear upper 8 bits */
    data[0]         = 0;
    seq.buf[1].data = data + 1;
    seq.buf[1].len  = 1;
  }
  else
  {
    seq.buf[1].data = data;
    seq.buf[1].len  = 2;
  }

  ret = I2CSPM_Transfer(i2c, &seq);
  if (ret != i2cTransferDone)
  {
    return((int) ret);
  }

  *val = (((uint16_t)(data[0])) << 8) | data[1];

  return(0);
}


/***************************************************************************//**
 * @brief
 *   Write to sensor register.
 *
 * @param[in] i2c
 *   Pointer to I2C peripheral register block.
 *
 * @param[in] addr
 *   I2C address for temperature sensor, in 8 bit format, where LSB is reserved
 *   for R/W bit.
 *
 * @param[in] reg
 *   Register to write (temperature register cannot be written).
 *
 * @param[in] val
 *   Value used when writing to register.
 *
 * @return
 *   Returns 0 if register written, <0 if unable to write to register.
 ******************************************************************************/
int TEMPSENS_RegisterSet(I2C_TypeDef *i2c,
                         uint8_t addr,
                         TEMPSENS_Register_TypeDef reg,
                         uint16_t val)
{
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t                    data[3];

  if (reg == tempsensRegTemp)
  {
    return(-1);
  }

  seq.addr  = addr;
  seq.flags = I2C_FLAG_WRITE;
  /* Select register to be written */
  data[0]         = ((uint8_t) reg) & 0x3;
  seq.buf[0].data = data;
  if (reg == tempsensRegConfig)
  {
    /* Only 1 byte reg */
    data[1]        = (uint8_t) val;
    seq.buf[0].len = 2;
  }
  else
  {
    data[1]        = (uint8_t)(val >> 8);
    data[2]        = (uint8_t) val;
    seq.buf[0].len = 3;
  }

  ret = I2CSPM_Transfer(i2c, &seq);
  if (ret != i2cTransferDone)
  {
    return((int) ret);
  }

  return(ret);
}


/***************************************************************************//**
 * @brief
 *   Fetch current temperature from temperature sensor (in Celsius).
 *
 * @details
 *   If reading the temperature, when a measurement is completed inside the
 *   sensor device, the new measurement may not be stored. For this reason, the
 *   temperature should not be polled with a higher frequency than the
 *   measurement conversion time for a given resolution configuration. Please
 *   refer to sensor device datasheet.
 *
 * @note
 *   The sensor on the the DK, STDS75, does not seem to have any check on max
 *   temperature limit, ie when exceeding specified max 125 C limit, it will keep
 *   increasing 9 bit counter to max representable 9 bit value of 127.5 C and
 *   then flip over to negative numbers. There is no limit check in this
 *   implementation on such issues.
 *
 * @param[in] i2c
 *   Pointer to I2C peripheral register block.
 *
 * @param[in] addr
 *   I2C address for temperature sensor, in 8 bit format, where LSB is reserved
 *   for R/W bit.
 *
 * @param[out] temp
 *   Reference to structure to place temperature (in Celsius).
 *
 * @return
 *   Returns 0 if temperature read, <0 if unable to read temperature.
 ******************************************************************************/
int TEMPSENS_TemperatureGet(I2C_TypeDef *i2c,
                            uint8_t addr,
                            TEMPSENS_Temp_TypeDef *temp)
{
  int      ret;
  uint32_t tmp;
  uint16_t val = 0;

  ret = TEMPSENS_RegisterGet(i2c, addr, tempsensRegTemp, &val);
  if (ret < 0)
  {
    return(ret);
  }

  /* Get all 12 bits potentially used */
  tmp = (uint32_t)(val >> 4);

  /* If negative number, convert using 2s complement */
  if (tmp & 0x800)
  {
    tmp     = (~tmp + 1) & 0xfff;
    temp->i = -(int16_t)(tmp >> 4);
    temp->f = -(int16_t)((tmp & 0xf) * 625);
  }
  else
  {
    temp->i = (int16_t)(tmp >> 4);
    temp->f = (int16_t)((tmp & 0xf) * 625);
  }

  return(0);
}
