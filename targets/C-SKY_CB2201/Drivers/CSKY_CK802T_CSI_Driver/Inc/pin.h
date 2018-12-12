/*
 * Copyright (C) 2017 C-SKY Microsystems Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/******************************************************************************
 * @file     pin.h
 * @brief    header File for pin definition
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/
#ifndef CH2201_PIN_H
#define CH2201_PIN_H

#include <stdint.h>
#include "pin_name.h"
#include "pinmux.h"

#define CONSOLE_TXD       PA10//PA2
#define CONSOLE_RXD       PA11//PA3
#define CONSOLE_TXD_FUNC    PA10_UART0_TX
#define CONSOLE_RXD_FUNC    PA11_UART0_RX
#define CONSOLE_IDX        0

#define CONSOLE_TXD1       PA17//PA2
#define CONSOLE_RXD1       PA16//PA3
#define CONSOLE_TXD1_FUNC    PA17_UART1_TX
#define CONSOLE_RXD1_FUNC    PA16_UART1_RX
#define CONSOLE_IDX1        1

#define CONSOLE_TXD2       PA24//PA2
#define CONSOLE_RXD2       PA25//PA3
#define CONSOLE_TXD2_FUNC    PA24_UART2_TX
#define CONSOLE_RXD2_FUNC    PA25_UART2_RX
#define CONSOLE_IDX2        2

/* example pin manager */
#define EXAMPLE_USART_IDX       0
#define EXAMPLE_PIN_USART_TX    PA10
#define EXAMPLE_PIN_USART_RX    PA11
#define EXAMPLE_PIN_USART_TX_FUNC   PA10_UART0_TX
#define EXAMPLE_PIN_USART_RX_FUNC   PA11_UART0_RX

#define EXAMPLE_SPI_IDX        1
#define EXAMPLE_PIN_SPI_MOSI    PA23
#define EXAMPLE_PIN_SPI_MISO    PA22
#define EXAMPLE_PIN_SPI_CS      PA18
#define EXAMPLE_PIN_SPI_SCK     PA21
#define EXAMPLE_PIN_SPI_MOSI_FUNC   PA23_SPI1_MOSI
#define EXAMPLE_PIN_SPI_MISO_FUNC   PA22_SPI1_MISO
#define EXAMPLE_PIN_SPI_CS_FUNC     PIN_FUNC_GPIO
#define EXAMPLE_PIN_SPI_SCK_FUNC    PA21_SPI1_SCK

#define EXAMPLE_IIC_IDX         1
#define EXAMPLE_PIN_IIC_SDA     PC1
#define EXAMPLE_PIN_IIC_SCL     PC0
#define EXAMPLE_PIN_IIC_SDA_FUNC    PC1_I2C1_SDA
#define EXAMPLE_PIN_IIC_SCL_FUNC    PC0_I2C1_SCL

#define EXAMPLE_GPIO_PIN    PA17
#define EXAMPLE_BOARD_GPIO_PIN_NAME "TX1"
#define EXAMPLE_GPIO_PIN_FUNC   PIN_FUNC_GPIO

#define EXAMPLE_PWM_CH      PA4
#define EXAMPLE_PWM_CH_FUNC PA4_PWM_CH0

#define EXAMPLE_ETH_SPI_IDX 0
#define EXAMPLE_ETH_SPI_MISO    PB3
#define EXAMPLE_ETH_SPI_MOSI    PA6
#define EXAMPLE_ETH_SPI_SCK     PB2
#define EXAMPLE_ETH_SPI_CS      PA7
#define EXAMPLE_ETH_PIN_INT     PA20
#define EXAMPLE_ETH_PIN_RST     PA5
#define EXAMPLE_ETH_SPI_MISO_FUNC    PB3_SPI0_MISO
#define EXAMPLE_ETH_SPI_MOSI_FUNC    PA6_SPI0_MOSI
#define EXAMPLE_ETH_SPI_SCK_FUNC     PB2_SPI0_CLK
#define EXAMPLE_ETH_SPI_CS_FUNC      PIN_FUNC_GPIO
#define EXAMPLE_ETH_PIN_RST_FUNC     PIN_FUNC_GPIO
#define EXAMPLE_ETH_PIN_INT_FUNC     PIN_FUNC_GPIO

#define EXAMPLE_I2S_IDX 0
#define EXAMPLE_I2S_MCLK        PA24
#define EXAMPLE_I2S_SCLK        PA25
#define EXAMPLE_I2S_WSCLK       PA26
#define EXAMPLE_I2S_SDA         PA27
#define EXAMPLE_I2S_MCLK_FUNC   PA24_I2S_MCLK
#define EXAMPLE_I2S_SCLK_FUNC   PA25_I2S_SCLK
#define EXAMPLE_I2S_WSCLK_FUNC  PA26_I2S_WSCLK
#define EXAMPLE_I2S_SDA_FUNC    PA27_I2S_SDA

#define EXAMPLE_ADC_CH0     PA8
#define EXAMPLE_ADC_CH0_FUNC    PA8_ADC_A0
#define EXAMPLE_ADC_CH1     PA9
#define EXAMPLE_ADC_CH1_FUNC    PA9_ADC_A1
#define EXAMPLE_ADC_CH2     PA10
#define EXAMPLE_ADC_CH2_FUNC    PA10_ADC_A2
#define EXAMPLE_ADC_CH3     PA11
#define EXAMPLE_ADC_CH3_FUNC    PA11_ADC_A3
#define EXAMPLE_ADC_CH4     PA12
#define EXAMPLE_ADC_CH4_FUNC    PA12_ADC_A4
#define EXAMPLE_ADC_CH5     PA13
#define EXAMPLE_ADC_CH5_FUNC    PA13_ADC_A5
#define EXAMPLE_ADC_CH6     PA14
#define EXAMPLE_ADC_CH6_FUNC    PA14_ADC_A6
#define EXAMPLE_ADC_CH7     PA15
#define EXAMPLE_ADC_CH7_FUNC    PA15_ADC_A7
#define EXAMPLE_ADC_CH8     PA16
#define EXAMPLE_ADC_CH8_FUNC    PA16_ADC_A8
#define EXAMPLE_ADC_CH9     PA17
#define EXAMPLE_ADC_CH9_FUNC    PA17_ADC_A9
#define EXAMPLE_ADC_CH10    PA22
#define EXAMPLE_ADC_CH10_FUNC   PA22_ADC_A10
#define EXAMPLE_ADC_CH11    PA23
#define EXAMPLE_ADC_CH11_FUNC   PA23_ADC_A11
#define EXAMPLE_ADC_CH12    PA26
#define EXAMPLE_ADC_CH12_FUNC   PA26_ADC_A12
#define EXAMPLE_ADC_CH13    PA27
#define EXAMPLE_ADC_CH13_FUNC   PA27_ADC_A13
#define EXAMPLE_ADC_CH14    PC0
#define EXAMPLE_ADC_CH14_FUNC   PC0_ADC_A14
#define EXAMPLE_ADC_CH15    PC1
#define EXAMPLE_ADC_CH15_FUNC   PC1_ADC_A15

/* tests pin manager */
#define TEST_USART_IDX       2
#define TEST_PIN_USART_TX    PA24
#define TEST_PIN_USART_RX    PA25
#define TEST_PIN_USART_TX_FUNC   PA24_UART2_TX
#define TEST_PIN_USART_RX_FUNC   PA25_UART2_RX

#define TEST_SPI_IDX         1
#define TEST_PIN_SPI_MOSI    PA23
#define TEST_PIN_SPI_MISO    PA22
#define TEST_PIN_SPI_CS      PA18
#define TEST_PIN_SPI_SCK     PA21
#define TEST_PIN_SPI_MOSI_FUNC   PA23_SPI1_MOSI
#define TEST_PIN_SPI_MISO_FUNC   PA22_SPI1_MISO
#define TEST_PIN_SPI_CS_FUNC     PIN_FUNC_GPIO
#define TEST_PIN_SPI_SCK_FUNC    PA21_SPI1_SCK

#define TEST_IIC_IDX         1
#define TEST_PIN_IIC_SDA     PC1
#define TEST_PIN_IIC_SCL     PC0
#define TEST_PIN_IIC_SDA_FUNC    PC1_I2C1_SDA
#define TEST_PIN_IIC_SCL_FUNC    PC0_I2C1_SCL

#define TEST_GPIO_PIN    PA18
#define TEST_BOARD_GPIO_PIN_NAME "PA18"
#define TEST_GPIO_PIN_FUNC   PIN_FUNC_GPIO

#define TEST_PWM_CH0      PA4
#define TEST_PWM_CH0_FUNC PA4_PWM_CH0
#define TEST_PWM_CH1      PA5
#define TEST_PWM_CH1_FUNC PA5_PWM_CH1
#define TEST_PWM_CH2      PB0
#define TEST_PWM_CH2_FUNC PB0_PWM_CH2
#define TEST_PWM_CH3      PB1
#define TEST_PWM_CH3_FUNC PB1_PWM_CH3
#define TEST_PWM_CH4      PB2
#define TEST_PWM_CH4_FUNC PB2_PWM_CH4
#define TEST_PWM_CH5      PB3
#define TEST_PWM_CH5_FUNC PB3_PWM_CH5

/* FIXME: for CDK */
#define PORTA_PIN0  PA0
#define PORTA_PIN1  PA1
#define PORTA_PIN2  PA2
#define PORTA_PIN3  PA3
#define PORTA_PIN4  PA4
#define PORTA_PIN5  PA5
#define PORTB_PIN0  PB0
#define PORTB_PIN1  PB1
#define PORTB_PIN2  PB2
#define PORTB_PIN3  PB3
#define PORTA_PIN6  PA6
#define PORTA_PIN7  PA7
#define PORTA_PIN8  PA8
#define PORTA_PIN9  PA9
#define PORTA_PIN10 PA10
#define PORTA_PIN11 PA11
#define PORTA_PIN12 PA12
#define PORTA_PIN13 PA13
#define PORTA_PIN14 PA14
#define PORTA_PIN15 PA15
#define PORTA_PIN16 PA16
#define PORTA_PIN17 PA17
#define PORTA_PIN18 PA18
#define PORTA_PIN19 PA19
#define PORTA_PIN20 PA20
#define PORTA_PIN21 PA21
#define PORTA_PIN22 PA22
#define PORTA_PIN23 PA23
#define PORTA_PIN24 PA24
#define PORTA_PIN25 PA25
#define PORTA_PIN26 PA26
#define PORTA_PIN27 PA27
#define PORTC_PIN0  PC0
#define PORTC_PIN1  PC1

#endif /* CH2201_PIN_H */
