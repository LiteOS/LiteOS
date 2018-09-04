/***************************************************************************//**
 * @file
 * @brief EEPROM driver for 24AA024 (2Kbit) EEPROM device on the DK.
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
#include "i2cspm.h"
#include "eeprom.h"

/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/

/** Number of bytes in EEPROM */
#define EEPROM_DVK_LEN         0x100

/** Pagesize in EEPROM */
#define EEPROM_DVK_PAGESIZE    16


/*******************************************************************************
 ***************************   LOCAL FUNCTIONS   *******************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   Do acknowledge polling on EEPROM device.
 *
 * @details
 *   When writing to an EEPROM, the EEPROM device will be busy for some time
 *   after issuing a (page) write. During this time, the EEPROM is not
 *   accessible, and will therefore not ACK any requests. This feature can
 *   be used to determine when the write is actually completed, and is denoted
 *   acknowledgement polling.
 *
 * @note
 *   This function will not return until the EEPROM device acknowledges (or some
 *   sort of I2C failure occurs). If trying to acknowledge poll a non-existing
 *   device, NACK will always result and this function will never return. Thus,
 *   it should not be used unless the EEPROM device is actually present.
 *
 * @param[in] i2c
 *   Pointer to I2C peripheral register block.
 *
 * @param[in] addr
 *   I2C address for EEPROM, in 8 bit format, where LSB is reserved
 *   for R/W bit.
 *
 * @return
 *   Returns 0 when EEPROM acknowledges. Negative value is returned
 *   is some sort of error occurred during acknowledgement polling.
 ******************************************************************************/
static int EEPROM_AckPoll(I2C_TypeDef *i2c, uint8_t addr)
{
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;

  /* Do acknowledge polling waiting for write process to finish in EEPROM */
  seq.addr  = addr;
  seq.flags = I2C_FLAG_WRITE;
  /* Just access device with write operation */
  seq.buf[0].data = NULL;
  seq.buf[0].len  = 0;

  /* Wait for ACK from device */
  while (1)
  {
    ret = I2CSPM_Transfer(i2c, &seq);
    if (ret == i2cTransferDone)
    {
      break;
    }
    else if (ret == i2cTransferNack)
    {
      continue;
    }
    else
    {
      return((int) ret);
    }
  }

  return(0);
}


/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   Read data from EEPROM.
 *
 * @param[in] i2c
 *   Pointer to I2C peripheral register block.
 *
 * @param[in] addr
 *   I2C address for EEPROM, in 8 bit format, where LSB is reserved
 *   for R/W bit.
 *
 * @param[in] offset
 *   Offset in EEPROM to start reading from.
 *
 * @param[out] data
 *   Location to place read data, must be at least @p len long.
 *
 * @param[in] len
 *   Number of bytes to read.
 *
 * @return
 *   Returns number of bytes read. Less than specified number of bytes is
 *   returned if reading beyond end of EEPROM. Negative value is returned
 *   is some sort of error occurred during read.
 ******************************************************************************/
int EEPROM_Read(I2C_TypeDef *i2c,
                uint8_t addr,
                unsigned int offset,
                uint8_t *data,
                unsigned int len)
{
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t                    offsetLoc[1];

  if (offset >= EEPROM_DVK_LEN)
  {
    return(0);
  }

  if ((offset + len) > EEPROM_DVK_LEN)
  {
    len = EEPROM_DVK_LEN - offset;
  }

  seq.addr  = addr;
  seq.flags = I2C_FLAG_WRITE_READ;
  /* Select offset to start reading from */
  offsetLoc[0]    = (uint8_t) offset;
  seq.buf[0].data = offsetLoc;
  seq.buf[0].len  = 1;
  /* Select location/length of data to be read */
  seq.buf[1].data = data;
  seq.buf[1].len  = len;

  ret = I2CSPM_Transfer(i2c, &seq);
  if (ret != i2cTransferDone)
  {
    return((int) ret);
  }

  return((int) len);
}


/***************************************************************************//**
 * @brief
 *   Write data to EEPROM.
 *
 * @param[in] i2c
 *   Pointer to I2C peripheral register block.
 *
 * @param[in] addr
 *   I2C address for EEPROM, in 8 bit format, where LSB is reserved
 *   for R/W bit.
 *
 * @param[in] offset
 *   Offset in EEPROM to start writing to.
 *
 * @param[out] data
 *   Location holding data to write, must be at least @p len long.
 *
 * @param[in] len
 *   Number of bytes to write.
 *
 * @return
 *   Returns number of bytes written. Less than specified number of bytes is
 *   returned if writing beyond end of EEPROM. Negative value is returned
 *   is some sort of error occurred during write.
 ******************************************************************************/
int EEPROM_Write(I2C_TypeDef *i2c,
                 uint8_t addr,
                 unsigned int offset,
                 uint8_t *data,
                 unsigned int len)
{
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  int                        tmp;
  unsigned int               chunk;
  unsigned int               max;
  uint8_t                    offsetLoc[1];

  if (offset >= EEPROM_DVK_LEN)
  {
    return(0);
  }

  if ((offset + len) > EEPROM_DVK_LEN)
  {
    len = EEPROM_DVK_LEN - offset;
  }

  /* Write max one page at a time */
  while (len)
  {
    max = EEPROM_DVK_PAGESIZE - (offset % EEPROM_DVK_PAGESIZE);

    if (len > max)
    {
      chunk = max;
    }
    else
    {
      chunk = len;
    }

    seq.addr  = addr;
    seq.flags = I2C_FLAG_WRITE_WRITE;
    /* Select offset to start writing to */
    offsetLoc[0]    = (uint8_t) offset;
    seq.buf[0].data = offsetLoc;
    seq.buf[0].len  = 1;
    /* Select location/length of data to be written */
    seq.buf[1].data = data;
    seq.buf[1].len  = chunk;

    ret = I2CSPM_Transfer(i2c, &seq);
    if (ret != i2cTransferDone)
    {
      return((int) ret);
    }

    /* Update counters etc */
    data   += chunk;
    offset += chunk;
    len    -= chunk;

    /* Do acknowledge polling waiting for write process to finish in EEPROM */
    tmp = EEPROM_AckPoll(i2c, addr);
    if (tmp)
    {
      return(tmp);
    }
  }

  return((int) len);
}
