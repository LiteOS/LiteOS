/**************************************************************************//**
 * @file
 * @brief EFM32GG_DK3750, SPI controller implementation for SSD2119 display
 *        interface when using Generic/Direct Drive mode
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


#include "em_device.h"
#include "em_usart.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "tftspi.h"

/** Set default values for tft synchronous mode init structure */
static const USART_InitSync_TypeDef inittft =
{ usartEnable,     /* Enable RX/TX when init completed. */
  48000000,        /* Use 48MHz reference clock */
  1000000,         /* 7 Mbits/s. */
  usartDatabits9,  /* 9 databits. */
  true,            /* Master mode. */
  true,            /* Send most significant bit first. */
  usartClockMode3, /* Clock idle low, sample on rising edge. */
  false,
  usartPrsRxCh0,
  false };


/**************************************************************************//**
 * @brief SPI_TFT_Init
 *    Initialize SPI interface to TFT-LCD SSD2119 controller
 * @note To enable access, be sure to call the functions
 *            BSP_PeripheralAccess(BSP_TFT, enable);
 *    before using this interface.
 *****************************************************************************/
void SPI_TFT_Init(void)
{
  /* Enabling clock to USART */
  CMU_ClockEnable(cmuClock_HFPER, true);
  CMU_ClockEnable(cmuClock_USART1, true);
  CMU_ClockEnable(cmuClock_GPIO, true);

  /* IO configuration (USART 1, Location #1) */
  GPIO_PinModeSet(gpioPortD, 0, gpioModePushPull, 0); /* TX - MOSI */
  GPIO_PinModeSet(gpioPortD, 1, gpioModeInput, 0);    /* RX - MISO */
  GPIO_PinModeSet(gpioPortD, 2, gpioModePushPull, 0); /* CLK */
  GPIO_PinModeSet(gpioPortD, 3, gpioModePushPull, 1); /* CS  */

  /* Ensure out of reset configuration */
  USART_Reset(USART1);

  /* Initialize USART1, in SPI master mode. */
  USART_InitSync(USART1, &inittft);

  USART1->ROUTE =
    USART_ROUTE_TXPEN |
    USART_ROUTE_RXPEN |
    USART_ROUTE_CLKPEN |
    USART_ROUTE_LOCATION_LOC1;
}


/**************************************************************************//**
 * @brief SPI_TFT_Write Write registers/data to SSD2119 controller
 * @param reg
 *      Register to write to
 * @param data
 *      16-bit data to write into register
 * @note
 *      It's not possible to read back register value through SSD2119 SPI
 *      interface, so no SPI_TFT_ReadRegister function is implemented
 *****************************************************************************/
void SPI_TFT_WriteRegister(uint8_t reg, uint16_t data)
{
  /* Enable chip select */
  GPIO_PinOutClear(gpioPortD, 3);

  /* Select register first */
  USART1->CTRL = USART1->CTRL & ~USART_CTRL_BIT8DV;

  USART_Tx(USART1, reg & 0xFF);
  USART_Rx(USART1);

  /* Write data */
  USART1->CTRL = USART1->CTRL | USART_CTRL_BIT8DV;
  USART_Tx(USART1, (data & 0xff00) >> 8);
  USART_Rx(USART1);
  USART_Tx(USART1, (data & 0x00ff));
  USART_Rx(USART1);

  /* Clear chip select */
  GPIO_PinOutSet(gpioPortD, 3);
}
