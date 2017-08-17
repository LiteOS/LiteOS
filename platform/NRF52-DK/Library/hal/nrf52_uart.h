/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
*
* The information contained herein is property of Nordic Semiconductor ASA.
* Terms and conditions of usage are described in detail in NORDIC
* SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
*
* Licensees are granted free, non-transferable use of the information. NO
* WARRANTY of ANY KIND is provided. This heading must NOT be removed from
* the file.
*
*/

#ifndef NRF52_UART_H
#define NRF52_UART_H

/*lint ++flb "Enter library region" */

#include <stdbool.h>
#include <stdint.h>


/** @file
* @brief  UART driver
*
*
* @defgroup nrf_drivers_uart  UART driver
* @{
* @ingroup nrf_drivers
* @brief  UART driver
*/

/** @brief Function for reading a character from UART.
Execution is blocked until UART peripheral detects character has been received.
\return cr Received character.
*/
uint8_t uart_get(void);

/** @brief Function for sending a character to UART.
Execution is blocked until UART peripheral reports character to have been send.
@param cr Character to send.
*/
void uart_put(uint8_t cr);


/** @brief Function for configuring UART to use 38400 baud rate.
@param rts_pin_number Chip pin number to be used for UART RTS
@param txd_pin_number Chip pin number to be used for UART TXD
@param cts_pin_number Chip pin number to be used for UART CTS
@param rxd_pin_number Chip pin number to be used for UART RXD
@param hwfc Enable hardware flow control
*/
void uart_config(uint8_t rts_pin_number, uint8_t txd_pin_number, uint8_t cts_pin_number, uint8_t rxd_pin_number, bool FlowControl);

/**
 *@}
 **/

#endif
