/***************************************************************************//**
 * @file
 * @brief i2c driver for the Si1147
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
#include "si114x_functions.h"
#include "rtcdriver.h"
#include "si1147_i2c.h"

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/



/**************************************************************************//**
 * @brief
  *  Reads register from the Si1147 sensor.
 * @param[in] i2c
 *   The I2C peripheral to use (not used).
 * @param[in] addr
 *   The I2C address of the sensor.
 * @param[out] data
 *   The data read from the sensor.
 * @param[in] reg
 *   The register address to read from in the sensor.
 * @return
 *   Returns number of bytes read on success. Otherwise returns error codes
 *   based on the I2CDRV.
 *****************************************************************************/
uint32_t Si1147_Read_Register(I2C_TypeDef *i2c,uint8_t addr, uint8_t reg, uint8_t *data)
{
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t i2c_write_data[1];

  seq.addr  = addr;
  seq.flags = I2C_FLAG_WRITE_READ;
  /* Select register to start reading from */
  i2c_write_data[0] = reg;
  seq.buf[0].data = i2c_write_data;
  seq.buf[0].len  = 1;
  /* Select length of data to be read */
  seq.buf[1].data = data;
  seq.buf[1].len  = 1;

  ret = I2CSPM_Transfer(i2c, &seq);
  if (ret != i2cTransferDone)
  {
    *data = 0xff;
    return (uint32_t)ret;
  }
  return (uint32_t)0;
}

/**************************************************************************//**
 * @brief
  *  Writes register in the Si1147 sensor.
 * @param[in] i2c
 *   The I2C peripheral to use (not used).
 * @param[in] addr
 *   The I2C address of the sensor.
 * @param[in] data
 *   The data to write to the sensor.
 * @param[in] reg
 *   The register address to write to in the sensor.
 * @return
 *   Returns zero on success. Otherwise returns error codes
 *   based on the I2CDRV.
 *****************************************************************************/
uint32_t Si1147_Write_Register(I2C_TypeDef *i2c,uint8_t addr, uint8_t reg, uint8_t data)
{
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t i2c_write_data[2];
  uint8_t i2c_read_data[1];

  seq.addr  = addr;
  seq.flags = I2C_FLAG_WRITE;
  /* Select register and data to write */
  i2c_write_data[0] = reg;
  i2c_write_data[1] = data;
  seq.buf[0].data = i2c_write_data;
  seq.buf[0].len  = 2;
  seq.buf[1].data = i2c_read_data;
  seq.buf[1].len  = 0;

  ret = I2CSPM_Transfer(i2c, &seq);
  if (ret != i2cTransferDone)
  {
    return (uint32_t)ret;
  }
  return (uint32_t)0;
}

/**************************************************************************//**
 * @brief
  *  Writes a block of data to the Si1147 sensor.
 * @param[in] i2c
 *   The I2C peripheral to use (not used).
 * @param[in] addr
 *   The I2C address of the sensor.
 * @param[in] data
 *   The data to write to the sensor.
 * @param[in] length
 *   The number of bytes to write to the sensor.
 * @param[in] reg
 *   The first register to begin writing to.
 * @return
 *   Returns zero on success. Otherwise returns error codes
 *   based on the I2CDRV.
 *****************************************************************************/
uint32_t Si1147_Write_Block_Register(I2C_TypeDef *i2c,uint8_t addr, uint8_t reg, uint8_t length, uint8_t const *data)
{
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t i2c_write_data[10];
  uint8_t i2c_read_data[1];
  int i;

  seq.addr  = addr;
  seq.flags = I2C_FLAG_WRITE;
  /* Select register to start writing to*/
  i2c_write_data[0] = reg;
  for (i=0; i<length;i++)
  {
    i2c_write_data[i+1] = data[i];
  }
  seq.buf[0].data = i2c_write_data;
  seq.buf[0].len  = 1+length;
  seq.buf[1].data = i2c_read_data;
  seq.buf[1].len  = 0;

  ret = I2CSPM_Transfer(i2c, &seq);
  if (ret != i2cTransferDone)
  {
    return (uint32_t)ret;
  }
  return (uint32_t)0;
}

/**************************************************************************//**
 * @brief
  *  Reads a block of data from the Si1147 sensor.
 * @param[in] i2c
 *   The I2C peripheral to use (not used).
 * @param[in] addr
 *   The I2C address of the sensor.
 * @param[out] data
 *   The data read from the sensor.
 * @param[in] length
 *   The number of bytes to write to the sensor.
 * @param[in] reg
 *   The first register to begin reading from.
 * @return
 *   Returns number of bytes read on success. Otherwise returns error codes
 *   based on the I2CDRV.
 *****************************************************************************/
uint32_t Si1147_Read_Block_Register(I2C_TypeDef *i2c,uint8_t addr, uint8_t reg, uint8_t length, uint8_t *data)
{
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t i2c_write_data[1];

  seq.addr  = addr;
  seq.flags = I2C_FLAG_WRITE_READ;
  /* Select register to start reading from */
  i2c_write_data[0] = reg;
  seq.buf[0].data = i2c_write_data;
  seq.buf[0].len  = 1;
  /* Select length of data to be read */
  seq.buf[1].data = data;
  seq.buf[1].len  = length;

  ret = I2CSPM_Transfer(i2c, &seq);
  if (ret != i2cTransferDone)
  {
    return (uint32_t)ret;
  }
  return (uint32_t)0;
}

/**************************************************************************//**
 * @brief  Writes to Si114x Register
 * @param[in] si114x_handle
 *   The programmer toolkit handle.
 * @param[in] address
 *   The register address to write to.
 * @param[in] data
 *   The data to write to the sensor.
 * @return
 *   Returns Error status
 *****************************************************************************/
int16_t Si114xWriteToRegister(HANDLE si114x_handle, uint8_t address, uint8_t data)
{
  return Si1147_Write_Register(((si114x_i2c_t*)si114x_handle)->i2c,((si114x_i2c_t*)si114x_handle)->addr, address, data);
}

/**************************************************************************//**
 * @brief  Reads from Si114x register
 * @param[in] si114x_handle
 *   The programmer toolkit handle.
 * @param[in] address
 *   The register address to read from.
 * @return
 *   Returns Value read
 *****************************************************************************/
int16_t Si114xReadFromRegister(HANDLE si114x_handle, uint8_t address)
{
  uint8_t data;
  Si1147_Read_Register(((si114x_i2c_t*)si114x_handle)->i2c,((si114x_i2c_t*)si114x_handle)->addr, address, &data);
  return data;
}

/**************************************************************************//**
 * @brief  Writes block of Si114x registers
 * @param[in] si114x_handle
 *   The programmer toolkit handle.
 * @param[in] address
 *   The register address to write to.
 * @param[in] length
 *   The number of bytes to write.
 * @param[in] values
 *   The data to write to the sensor.
 * @return
 *   Returns Error status
 *****************************************************************************/
int16_t Si114xBlockWrite(HANDLE si114x_handle,
                     uint8_t address, uint8_t length, uint8_t *values)
{
  return Si1147_Write_Block_Register(((si114x_i2c_t*)si114x_handle)->i2c,((si114x_i2c_t*)si114x_handle)->addr, address, length, values);
}

/**************************************************************************//**
 * @brief  Reads block of Si114x registers
 * @param[in] si114x_handle
 *   The programmer toolkit handle.
 * @param[in] address
 *   The register address to read from.
 * @param[in] length
 *   The number of bytes to read.
 * @param[in] values
 *   The data read from the sensor.
 * @return
 *   Returns Error status
 *****************************************************************************/
int16_t Si114xBlockRead(HANDLE si114x_handle,
                    uint8_t address, uint8_t length, uint8_t *values)
{
  return Si1147_Read_Block_Register(((si114x_i2c_t*)si114x_handle)->i2c,((si114x_i2c_t*)si114x_handle)->addr, address, length, values);
}

/**************************************************************************//**
 * @brief  Implements 10ms delay
 *****************************************************************************/
void delay_10ms()
{
  // This is needed immediately after a reset command to the Si114x
  // In the PGM_Toolkit, there is sufficient latency, so none is added
  // here. This is a reminder that when porting code, that this must
  // be implemented.
  RTCDRV_Delay(10);
}

/**************************************************************************//**
 * @brief  Implements 1ms delay
 *****************************************************************************/
void delay_1ms()
{
  // This can be used to save power while polling Si114x status
  // registers by using a sleep delay instead of busy wait.
  RTCDRV_Delay(1);
}
