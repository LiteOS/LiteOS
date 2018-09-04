/**************************************************************************//**
 * @file
 * @brief Micro SD card driver for the EFM32GG_DK3750 development kit.
 *        This file provides the interface layer between the DK and the
 *        fat filesystem provided.
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


#include "diskio.h"
#include "microsd.h"
#include "em_cmu.h"
#include "em_usart.h"

/**************************************************************************//**
 * @addtogroup MicroSd
 * @{ This module implements the SPI layer needed to control a micro SD card.
 ** @} ***********************************************************************/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
static uint32_t timeOut, xfersPrMsec;

/**************************************************************************//**
 * @brief Wait for micro SD card ready.
 * @return 0xff: micro SD card ready, other value: micro SD card not ready.
 *****************************************************************************/
static uint8_t WaitReady(void)
{
  uint8_t res;
  uint32_t retryCount;

  /* Wait for ready in timeout of 500ms */
  retryCount = 500 * xfersPrMsec;
  do
    res = MICROSD_XferSpi(0xff);
  while ((res != 0xFF) && --retryCount);

  return res;
}
/** @endcond */

/**************************************************************************//**
 * @brief
 *  Initialize the SPI peripheral for microSD card usage.
 *  SPI pins and speed etc. is defined in microsdconfig.h.
 *****************************************************************************/
void MICROSD_Init(void)
{
  USART_InitSync_TypeDef init = USART_INITSYNC_DEFAULT;

  /* Enabling clock to USART 0 */
  CMU_ClockEnable(MICROSD_CMUCLOCK, true);
  CMU_ClockEnable(cmuClock_GPIO, true);

  /* Initialize USART in SPI master mode. */
  xfersPrMsec   = MICROSD_LO_SPI_FREQ / 8000;
  init.baudrate = MICROSD_LO_SPI_FREQ;
  init.msbf     = true;
  USART_InitSync(MICROSD_USART, &init);

  /* Enabling pins and setting location, SPI CS not enable */
  MICROSD_USART->ROUTE = USART_ROUTE_TXPEN | USART_ROUTE_RXPEN |
                         USART_ROUTE_CLKPEN | MICROSD_LOC;

#if defined( USART_CTRL_SMSDELAY )
  /* This will allow us to use higher baudrate. */
  MICROSD_USART->CTRL |= USART_CTRL_SMSDELAY;
#endif

  /* IO configuration */
  GPIO_PinModeSet(MICROSD_GPIOPORT, MICROSD_MOSIPIN, gpioModePushPull, 0);  /* MOSI */
  GPIO_PinModeSet(MICROSD_GPIOPORT, MICROSD_MISOPIN, gpioModeInputPull, 1); /* MISO */
  GPIO_PinModeSet(MICROSD_GPIOPORT, MICROSD_CSPIN,   gpioModePushPull, 1);  /* CS */
  GPIO_PinModeSet(MICROSD_GPIOPORT, MICROSD_CLKPIN,  gpioModePushPull, 0);  /* CLK */
}

/**************************************************************************//**
 * @brief
 *  Deinitialize SPI peripheral.
 *  Turn off the SPI peripheral and disable SPI GPIO pins.
 *****************************************************************************/
void MICROSD_Deinit(void)
{
  USART_Reset(MICROSD_USART);

  /* IO configuration (USART 0, Location #0) */
  GPIO_PinModeSet(MICROSD_GPIOPORT, MICROSD_MOSIPIN, gpioModeDisabled, 0);  /* MOSI */
  GPIO_PinModeSet(MICROSD_GPIOPORT, MICROSD_MISOPIN, gpioModeDisabled, 0);  /* MISO */
  GPIO_PinModeSet(MICROSD_GPIOPORT, MICROSD_CSPIN,   gpioModeDisabled, 0);  /* CS */
  GPIO_PinModeSet(MICROSD_GPIOPORT, MICROSD_CLKPIN,  gpioModeDisabled, 0);  /* Clock */
}

/**************************************************************************//**
 * @brief
 *  Do one SPI transfer.
 *
 * @param data
 *  Byte to transmit.
 *
 * @return
 *  Byte received.
 *****************************************************************************/
uint8_t MICROSD_XferSpi(uint8_t data)
{
  if ( timeOut )
  {
    timeOut--;
  }

  return USART_SpiTransfer(MICROSD_USART, data);
}

/**************************************************************************//**
 * @brief Deselect the micro SD card and release the SPI bus.
 *****************************************************************************/
void MICROSD_Deselect(void)
{
  GPIO->P[ MICROSD_GPIOPORT ].DOUTSET = 1 << MICROSD_CSPIN; /* CS pin high. */
  MICROSD_XferSpi(0xff);
}

/**************************************************************************//**
 * @brief Select the micro SD card and wait for the card to become ready.
 * @return 1:Successful, 0:Timeout.
 *****************************************************************************/
int MICROSD_Select(void)
{
  GPIO->P[ MICROSD_GPIOPORT ].DOUTCLR = 1 << MICROSD_CSPIN; /* CS pin low. */
  if (WaitReady() != 0xFF)
  {
    MICROSD_Deselect();
    return 0;
  }
  return 1;
}

/**************************************************************************//**
 * @brief Turn on micro SD card power.
 *        DK doesn't support socket power control, only enable the SPI clock.
 *****************************************************************************/
void MICROSD_PowerOn(void)
{
  /* Enable SPI clock */
  CMU_ClockEnable(MICROSD_CMUCLOCK, true);
}

/**************************************************************************//**
 * @brief Turn off micro SD card power.
 *        DK doesn't support socket power control, only disable the SPI clock.
 *****************************************************************************/
void MICROSD_PowerOff(void)
{
  /* Wait for micro SD card ready */
  MICROSD_Select();
  MICROSD_Deselect();
  /* Disable SPI clock */
  CMU_ClockEnable(MICROSD_CMUCLOCK, false);
}

/**************************************************************************//**
 * @brief Receive a data block from micro SD card.
 * @param[out] buff
 *  Data buffer to store received data.
 * @param btr
 *  Byte count (must be multiple of 4).
 * @return
 *  1:OK, 0:Failed.
 *****************************************************************************/
int MICROSD_BlockRx(uint8_t *buff, uint32_t btr)
{
  uint8_t token;
  uint16_t val;
  uint32_t retryCount, framectrl, ctrl;

  /* Wait for data packet in timeout of 100ms */
  retryCount = 100 * xfersPrMsec;
  do
  {
    token = MICROSD_XferSpi(0xff);
  } while ((token == 0xFF) && --retryCount);

  if (token != 0xFE)
  {
    /* Invalid data token */
    return 0;
  }

  /* Save current configuration. */
  framectrl = MICROSD_USART->FRAME;
  ctrl      = MICROSD_USART->CTRL;

  /* Set frame length to 16 bit. This will increase the effective data rate. */
  MICROSD_USART->FRAME = (MICROSD_USART->FRAME & (~_USART_FRAME_DATABITS_MASK))
                         | USART_FRAME_DATABITS_SIXTEEN;
  MICROSD_USART->CTRL |= USART_CTRL_BYTESWAP;

  /* Clear send and receive buffers. */
  MICROSD_USART->CMD = USART_CMD_CLEARRX | USART_CMD_CLEARTX;

  if ( timeOut >= btr + 2 )
  {
    timeOut -= btr + 2;
  }
  else
  {
    timeOut = 0;
  }

  /* Pipelining - The USART has two buffers of 16 bit in both
   * directions. Make sure that at least one is in the pipe at all
   * times to maximize throughput. */
  MICROSD_USART->TXDOUBLE = 0xffff;
  do
  {
    MICROSD_USART->TXDOUBLE = 0xffff;

    while (!(MICROSD_USART->STATUS & USART_STATUS_RXDATAV));

    val = MICROSD_USART->RXDOUBLE;
    *buff++ = val;
    *buff++ = val >> 8;

    btr -= 2;
  } while (btr);

  /* Next two bytes is the CRC which we discard. */
  while (!(MICROSD_USART->STATUS & USART_STATUS_RXDATAV));
  MICROSD_USART->RXDOUBLE;

  /* Restore old settings. */
  MICROSD_USART->FRAME = framectrl;
  MICROSD_USART->CTRL  = ctrl;

  return 1;     /* Return with success */
}

/**************************************************************************//**
 * @brief Send a data block to micro SD card.
 * @param[in] buff 512 bytes data block to be transmitted.
 * @param token Data token.
 * @return 1:OK, 0:Failed.
 *****************************************************************************/
#if _READONLY == 0
int MICROSD_BlockTx(const uint8_t *buff, uint8_t token)
{
  uint8_t resp;
  uint16_t val;
  uint32_t bc = 512;
  uint32_t framectrl, ctrl;

  if (WaitReady() != 0xFF)
  {
    return 0;
  }

  MICROSD_XferSpi(token);         /* Xmit a token */

  if (token == 0xFD)
  {
    /* StopTran token */
    return 1;
  }

  /* Save current configuration. */
  framectrl = MICROSD_USART->FRAME;
  ctrl      = MICROSD_USART->CTRL;

  /* Set frame length to 16 bit. This will increase the effective data rate. */
  MICROSD_USART->FRAME = (MICROSD_USART->FRAME & (~_USART_FRAME_DATABITS_MASK))
                         | USART_FRAME_DATABITS_SIXTEEN;
  MICROSD_USART->CTRL |= USART_CTRL_BYTESWAP;

  /* Clear send and receive buffers. */
  MICROSD_USART->CMD = USART_CMD_CLEARRX | USART_CMD_CLEARTX;

  if ( timeOut >= bc + 2 )
  {
    timeOut -= bc + 2;
  }
  else
  {
    timeOut = 0;
  }

  do
  {
    /* Transmit a 512 byte data block to the SD-Card. */

    val  = *buff++;
    val |= *buff++ << 8;
    bc  -= 2;

    while (!(MICROSD_USART->STATUS & USART_STATUS_TXBL));

    MICROSD_USART->TXDOUBLE = val;
  } while (bc);

  while (!(MICROSD_USART->STATUS & USART_STATUS_TXBL));

  /* Transmit two dummy CRC bytes. */
  MICROSD_USART->TXDOUBLE = 0xFFFF;

  while (!(MICROSD_USART->STATUS & USART_STATUS_TXC));

  /* Clear send and receive buffers. */
  MICROSD_USART->CMD = USART_CMD_CLEARRX | USART_CMD_CLEARTX;

  /* Restore old settings. */
  MICROSD_USART->FRAME = framectrl;
  MICROSD_USART->CTRL  = ctrl;

  resp = MICROSD_XferSpi(0xff); /* Receive a data response */

  if ((resp & 0x1F) != 0x05)    /* If not accepted, return with error */
  {
    return 0;
  }

  return 1;
}
#endif  /* _READONLY */

/**************************************************************************//**
 * @brief
 *  Send a command packet to micro SD card.
 * @param[in] cmd
 *  Command byte.
 * @param[in] arg
 *  Argument.
 * @return
 *  Response value.
 *****************************************************************************/
uint8_t MICROSD_SendCmd(uint8_t cmd, DWORD arg)
{
  uint8_t  n, res;
  uint32_t retryCount;

  if (cmd & 0x80)
  { /* ACMD<n> is the command sequense of CMD55-CMD<n> */
    cmd &= 0x7F;
    res  = MICROSD_SendCmd(CMD55, 0);
    if (res > 1)
    {
      return res;
    }
  }

  /* Select the card and wait for ready */
  MICROSD_Deselect();
  if (!MICROSD_Select())
  {
    return 0xFF;
  }

  /* Send command packet */
  MICROSD_XferSpi(0x40 | cmd);            /* Start + Command index */
  MICROSD_XferSpi((uint8_t)(arg >> 24));  /* Argument[31..24] */
  MICROSD_XferSpi((uint8_t)(arg >> 16));  /* Argument[23..16] */
  MICROSD_XferSpi((uint8_t)(arg >> 8));   /* Argument[15..8] */
  MICROSD_XferSpi((uint8_t) arg);         /* Argument[7..0] */
  n = 0x01;                               /* Dummy CRC + Stop */
  if (cmd == CMD0)
  {
    n = 0x95;                             /* Valid CRC for CMD0(0) */
  }
  if (cmd == CMD8)
  {
    n = 0x87;                             /* Valid CRC for CMD8(0x1AA) */
  }
  MICROSD_XferSpi(n);

  /* Receive command response */
  if (cmd == CMD12)
  {
    MICROSD_XferSpi(0xff);                /* Skip a stuff byte when stop reading */
  }
  retryCount = 10;                        /* Wait for a valid response in timeout of 10 attempts */
  do
  {
    res = MICROSD_XferSpi(0xff);
  } while ((res & 0x80) && --retryCount);

  return res;             /* Return with the response value */
}

/**************************************************************************//**
 * @brief Set SPI clock to a low frequency suitable for initial
 *        card initialization.
 *****************************************************************************/
void MICROSD_SpiClkSlow(void)
{
  USART_BaudrateSyncSet(MICROSD_USART, 0, MICROSD_LO_SPI_FREQ);
  xfersPrMsec = MICROSD_LO_SPI_FREQ / 8000;
}

/**************************************************************************//**
 * @brief Set SPI clock to maximum frequency.
 *****************************************************************************/
void MICROSD_SpiClkFast(void)
{
  USART_BaudrateSyncSet(MICROSD_USART, 0, MICROSD_HI_SPI_FREQ);
  xfersPrMsec = MICROSD_HI_SPI_FREQ / 8000;
}

/**************************************************************************//**
 * @brief
 *  Set a timeout value. The timeout value will be decremented towards zero
 *  when SPI traffic to/from the micro SD card takes place. Use @ref
 *  MICROSD_TimeOutElapsed() to check if timeout has elapsed.
 * @param[in] msec
 *  Millisecond timeout value (very approximate).
 *****************************************************************************/
void MICROSD_TimeOutSet(uint32_t msec)
{
  timeOut = xfersPrMsec * msec;
}

/**************************************************************************//**
 * @brief
 *  Check if timeout value set with @ref MICROSD_TimeOutSet() has elapsed.
 * @return
 *  True if timeout has elapsed.
 *****************************************************************************/
bool MICROSD_TimeOutElapsed(void)
{
  return timeOut == 0;
}
