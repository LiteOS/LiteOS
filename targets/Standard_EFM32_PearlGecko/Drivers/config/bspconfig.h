/***************************************************************************//**
 * @file
 * @brief Provide BSP (board support package) configuration parameters.
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

#ifndef __BSPCONFIG_H
#define __BSPCONFIG_H

#define BSP_STK
#define BSP_STK_BRD2001

#define BSP_BCC_USART       UART0
#define BSP_BCC_CLK         cmuClock_UART0
#define BSP_BCC_LOCATION    UART_ROUTE_LOCATION_LOC1
#define BSP_BCC_TXPORT      gpioPortE
#define BSP_BCC_TXPIN       0
#define BSP_BCC_RXPORT      gpioPortE
#define BSP_BCC_RXPIN       1
#define BSP_BCC_ENABLE_PORT gpioPortD
#define BSP_BCC_ENABLE_PIN  13

#define BSP_GPIO_LEDS
#define BSP_NO_OF_LEDS  4
#define BSP_GPIO_LEDARRAY_INIT {{gpioPortC,0},{gpioPortC,1}, \
                                {gpioPortC,2},{gpioPortC,3}}

#define BSP_INIT_DEFAULT  0

#define BSP_BCP_VERSION 1
#include "bsp_bcp.h"

#endif
