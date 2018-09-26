/**************************************************************************//**
 * @file
 * @brief SPI interface API for KSZ8851SNL Ethernet controller
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
#include <stdio.h>
#include <stdint.h>

#include "ksz8851snl_spi.h"
#include "em_assert.h"
#include "em_common.h"
#include "em_gpio.h"
#include "spidrv.h"

#define BOGUS_BYTE           0xFF               /**< Bogus byte used for receiving via SPI */
#define REG_MASK             0x03               /**< Register mask */
#define OPCODE_REG_READ      0x00               /**< Opcode for reading a register */
#define OPCODE_REG_WRITE     0x40               /**< Opcode for writing a register */
#define OPCODE_FIFO_READ     0x80               /**< Opcode for reading from FIFO */
#define OPCODE_FIFO_WRITE    0xC0               /**< Opcode for writing to FIFO */
#define ADDRESS_MS2B_POS     0x06               /**< Most significant 2 Bytes of the address position */
#define ADDRESS_SHIFT        0x02               /**< Number of positions to shift the register address */
#define BYTE_ENABLE          0x03               /**< Enable 2 byte register read/write */
#define BYTE_ENABLE_SHIFT    0x02               /**< Number of positions to shitf the byte enable bits */

/* GPIO pins used for SPI communication. */
/*(USART 1, Location #1) */
#define ETH_CS_PIN       3                      /**< SPI CHIP SELECT Pin */
#define ETH_CS_PORT      gpioPortD              /**< SPI CHIP SELECT Port */


/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
static SPIDRV_HandleData_t spiHandleData;
static SPIDRV_Handle_t spiHandle = &spiHandleData;

/** @endcond */


/**************************************************************************//**
 * @brief KSZ8851SNL_SPI_Init
 *   Initialize SPI interface to Ethernet controller.
 *
 * @note
 *   To enable access, be sure to call the functions
 *     BSP_PeripheralAccess(BSP_ETH, true);
 *   before using this interface.
 *****************************************************************************/
void KSZ8851SNL_SPI_Init(void)
{
  Ecode_t result;
  SPIDRV_Init_t initData = SPIDRV_MASTER_USART1;
  initData.bitRate = 8000000; // 8 MHz max safe speed
  initData.frameLength = 8;
  initData.dummyTxValue = BOGUS_BYTE;
  initData.bitOrder = spidrvBitOrderMsbFirst;
  initData.clockMode = spidrvClockMode0;
  initData.csControl = spidrvCsControlApplication;

  result = SPIDRV_Init(spiHandle, &initData);
  EFM_ASSERT(result == ECODE_EMDRV_SPIDRV_OK);

  GPIO_PinModeSet(ETH_CS_PORT, ETH_CS_PIN, gpioModePushPull, 1);
}


/**************************************************************************//**
 * @brief
 *   Transmits a series of bytes over the spi link
 *
 * @param[in] numBytes
 *   the number of data bytes to be transmitted
 *
 * @param[in] data
 *   actual data to be transmitted
 *****************************************************************************/
static void KSZ8851SNL_SPI_Transmit(int numBytes, const uint8_t * data)
{
  Ecode_t result;
  int remaining = numBytes;

  /* The spidrv is using dma to transfer bytes to the USART (spi)
   * this is why we must transmit bytes in chunks of at most
   * DMADRV_MAX_XFER_COUNT (1024 on EFM32GG990F1024)
   */
  do {
    int count  = SL_MIN(remaining, DMADRV_MAX_XFER_COUNT);
    result     = SPIDRV_MTransmitB(spiHandle, data, count);
    remaining -= count;
    data      += count;
  } while (result == ECODE_EMDRV_SPIDRV_OK && remaining > 0);

  EFM_ASSERT(result == ECODE_EMDRV_SPIDRV_OK);
}


/**************************************************************************//**
 * @brief
 *   Receive a series of bytes over the spi link
 *
 * @param[in] numBytes
 *   the number of data bytes to be received
 *
 * @param[in] buffer
 *   the destination buffer
 *****************************************************************************/
static void KSZ8851SNL_SPI_Receive(int numBytes, uint8_t * buffer)
{
  Ecode_t result;
  int remaining = numBytes;

  /* The spidrv is using dma to receive bytes from the USART (spi)
   * this is why we must receive bytes in chunks of at most
   * DMADRV_MAX_XFER_COUNT (1024 on EFM32GG990F1024)
   */
  do {
    int count  = SL_MIN(remaining, DMADRV_MAX_XFER_COUNT);
    result     = SPIDRV_MReceiveB(spiHandle, buffer, count);
    remaining -= count;
    buffer    += count;
  } while (result == ECODE_EMDRV_SPIDRV_OK && remaining > 0);

  EFM_ASSERT(result == ECODE_EMDRV_SPIDRV_OK);
}


/**************************************************************************//**
 * @brief
 *   Select/deselect the ksz8851snl chip. This will clear/set the chip select
 *   GPIO pin connected to the ksz8851snl. The chip has active low chip select.
 *
 * @param[in] enable
 *   true will select the ksz8851snl chip
 *   false will deselect the ksz8851snl chip.
 *****************************************************************************/
static void KSZ8851SNL_SPI_SetChipSelect(bool enable)
{
  if (enable)
  {
    GPIO_PinOutClear(ETH_CS_PORT, ETH_CS_PIN);
  }
  else
  {
    GPIO_PinOutSet(ETH_CS_PORT, ETH_CS_PIN);
  }
}


/**************************************************************************//**
 * @brief
 *   Read ethernet controller register
 *
 * @param[in] reg
 *   Register to read
 *
 * @return
 *   value of the register
 *****************************************************************************/
uint16_t KSZ8851SNL_SPI_ReadRegister(uint8_t reg)
{
  uint16_t value = 0x0000;
  uint8_t txBuffer[4];
  uint8_t rxBuffer[4];

  /**
   * SPI Command Format
   *
   * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   * |op | byte  | reg[7:2] |  ... |
   * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   *
   * op=00 "Register Read"
   * byte=0011 "Read register at reg"
   * byte=1100 "Read register at reg+2"
   * reg=6 most significant bits of the register address
   */
  txBuffer[0]  = OPCODE_REG_READ;
  txBuffer[0] |= BYTE_ENABLE << (BYTE_ENABLE_SHIFT + (reg & REG_MASK));
  txBuffer[0] |= (reg >> ADDRESS_MS2B_POS);
  txBuffer[1]  = (reg << ADDRESS_SHIFT) & 0xf0;

  KSZ8851SNL_SPI_SetChipSelect(true);
  SPIDRV_MTransferB(spiHandle, txBuffer, rxBuffer, 4);
  KSZ8851SNL_SPI_SetChipSelect(false);

  value = (rxBuffer[3] << 8) | rxBuffer[2];
  return value;
}


/**************************************************************************//**
 * @brief
 *   Write ethernet controller register
 *
 * @param[in] reg
 *   Register to write
 *
 * @param[in] value
 *   The value to be written into the register
 *****************************************************************************/
void KSZ8851SNL_SPI_WriteRegister(uint8_t reg, uint16_t value)
{
  uint8_t txBuffer[4];

  /**
   * SPI Command Format
   *
   * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   * |op | byte  | reg[7:2]  |  ...  |        16 bit value           |
   * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   *
   * op=01 "Register Write"
   * byte=0011 "Write register at reg"
   * byte=1100 "Write register at reg+2"
   * reg=6 most significant bits of the register address
   */
  txBuffer[0]  = OPCODE_REG_WRITE | (BYTE_ENABLE << (BYTE_ENABLE_SHIFT + (reg & REG_MASK)));
  txBuffer[0] |= reg >> ADDRESS_MS2B_POS;
  txBuffer[1]  = (reg << ADDRESS_SHIFT) & 0xf0;
  txBuffer[2]  = value & 0xff;
  txBuffer[3]  = (value >> 8) & 0xff;

  KSZ8851SNL_SPI_SetChipSelect(true);
  KSZ8851SNL_SPI_Transmit(4, txBuffer);
  KSZ8851SNL_SPI_SetChipSelect(false);
}


/*************************************************************************//**
* @brief
*   Read data from the ethernet controller RX FIFO
*
* @details
*   Make sure that the ethernet controller is in DMA mode before calling this
*   function. This is configured by setting bit 3 in the RXQCR register. This
*   bit should also be cleared whenever you want to access the control
*   registers.
*
* @details
*   When reading data from the controller the first 4 bytes is always dummy
*   data and is skipped by this function.
*
* @param[in] numBytes
*   Number of bytes to read, 1-12K
*
* @param[out] data
*   Buffer where the bytes are inserted.
*****************************************************************************/
void KSZ8851SNL_SPI_ReadFifo(int numBytes, uint8_t *data)
{
  EFM_ASSERT(numBytes >= 0 && numBytes < 12000);
  EFM_ASSERT(data != NULL);

  uint8_t cmd = OPCODE_FIFO_READ;
  uint8_t dummy[4];

  KSZ8851SNL_SPI_SetChipSelect(true);
  KSZ8851SNL_SPI_Transmit(1, &cmd);
  KSZ8851SNL_SPI_Receive(4, dummy);
  KSZ8851SNL_SPI_Receive(numBytes, data);
  KSZ8851SNL_SPI_SetChipSelect(false);
}

/*************************************************************************//**
 * @brief Start writing to the ethernet controller FIFO
 *****************************************************************************/
void KSZ8851SNL_SPI_WriteFifoBegin(void)
{
  uint8_t cmd = OPCODE_FIFO_WRITE;
  /* Enable chip select */
  KSZ8851SNL_SPI_SetChipSelect(true);
  /* Send Write command */
  KSZ8851SNL_SPI_Transmit(1, &cmd);
}

/*************************************************************************//**
 * @brief Continue writing ethernet controller FIFO
 * @param[in] numBytes Number of bytes to write, 1-12K
 * @param[in] data Actual bytes to write
 *****************************************************************************/
void KSZ8851SNL_SPI_WriteFifo(int numBytes, const uint8_t *data)
{
  EFM_ASSERT(numBytes >= 0 && numBytes < 12000);
  EFM_ASSERT(data != NULL);

  KSZ8851SNL_SPI_Transmit(numBytes, data);
}


/*************************************************************************//**
 * @brief Stop read/write the ethernet controller FIFO
 *****************************************************************************/
void KSZ8851SNL_SPI_WriteFifoEnd(void)
{
  KSZ8851SNL_SPI_SetChipSelect(false);
}
