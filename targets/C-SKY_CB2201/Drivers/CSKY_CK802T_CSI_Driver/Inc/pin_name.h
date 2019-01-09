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
 * @file     pin_name.h
 * @brief    header file for the pin_name
 * @version  V1.0
 * @date     23. August 2017
 ******************************************************************************/
#ifndef _PINNAMES_H
#define _PINNAMES_H


#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PA0 = 0,
    PA1,
    PA2,
    PA3,
    PA4,
    PA5,

    PB0,
    PB1,
    PB2,
    PB3,

    PA6,
    PA7,
    PA8,
    PA9,
    PA10,
    PA11,
    PA12,
    PA13,
    PA14,
    PA15,
    PA16,
    PA17,
    PA18,
    PA19,
    PA20,
    PA21,
    PA22,
    PA23,
    PA24,
    PA25,
    PA26,
    PA27,

    PC0,
    PC1
}
pin_name_e;

typedef enum {
    PA0_ETB_TRIG0    = 0,
    PA0_ACPM_P1      = 1,
    PA0_JTAG_TCK     = 2,
    PA0_ACPM_N1      = 1,
    PA1_ETB_TRIG1    = 0,
    PA1_JTAG_TMS     = 2,
    PA2_UART0_TX     = 0,
    PA2_SPI0_MISO    = 2,
    PA2_UART0_SIROUT = 3,
    PA3_UART0_RX     = 0,
    PA3_SPI0_MOSI    = 2,
    PA3_UART0_SIRIN  = 3,
    PA4_UART0_CTS    = 0,
    PA4_PWM_CH0      = 1,
    PA4_SPI0_CLK     = 2,
    PA4_ETB_TRIG0    = 3,
    PA5_UART0_RTS    = 0,
    PA5_PWM_CH1      = 1,	//20181130原先叫PA5_PWMCH1
    PA5_SPI0_CS      = 2,
    PA5_ETB_TRIG1    = 3,
    PB0_I2C0_SCL     = 0,
    PB0_PWM_CH2      = 1,
    PB0_I2S_MCLK     = 2,
    PB1_I2C0_SCL     = 0,
    PB1_PWM_CH3      = 1,	//20181130//PB1_PWM_CH2
    PB1_I2S_CLK      = 2,
    PB2_SPI0_CLK     = 0,
    PB2_PWM_CH4      = 1,
    PB2_I2S_WSCLK    = 2,
    PB3_SPI0_MISO    = 0,
    PB3_PWM_CH5      = 1,
    PB3_I2S_SDA      = 2,
    PA6_SPI0_MOSI    = 0,
    PA6_PWM_CH6      = 1,
    PA6_I2C0_SCL     = 2,
    PA7_SPI0_CS      = 0,
    PA7_PWM_CH7      = 1,
    PA7_I2C0_SDA     = 2,
    PA8_SYS_WKUP     = 0,
    PA8_ADC_A0       = 1,
    PA8_ACMP_P0      = 2,
    PA9_BOOT         = 0,
    PA9_ADC_A1       = 1,
    PA9_PWM_FAULT    = 2,
    PA10_ADC_A2      = 1,
    PA10_UART0_TX    = 2,
    PA10_UART0_SIROUT = 3,
    PA11_ACMP_N0      = 0,
    PA11_ADC_A3       = 1,
    PA11_UART0_RX     = 2,
    PA11_UART0_SIRIN  = 3,
    PA12_PWM_CH8      = 0,
    PA12_JTAG_TCK     = 1,
    PA12_ADC_A4       = 2,
    PA13_PWM_CH9      = 0,
    PA13_JTAG_TMS     = 1,
    PA13_ADC_A5       = 2,
    PA14_PWM_CH10     = 0,
    PA14_ADC_A6       = 1,
    PA15_PWM_CH11     = 0,
    PA15_ADC_A7       = 1,
    PA16_UART1_RX     = 0,
    PA16_ADC_A8       = 1,
    PA16_UART1_SIRIN  = 3,
    PA17_UART1_TX     = 0,
    PA17_ADC_A9       = 1,
    PA17_UART1_SIROUT = 3,
    PA18_SPI1_CS0     = 0,
    PA18_ACMP_O0      = 1,
    PA19_SPI1_CS1     = 0,
    PA19_ACMP_O1      = 1,
    PA20_SPI1_CS2     = 0,
    PA20_ETB_TRIG0    = 1,
    PA20_UART1_RX     = 2,
    PA20_UART1_SIRIN  = 3,
    PA21_SPI1_SCK     = 0,
    PA21_ETB_TRIG1    = 1,
    PA21_UART1_TX     = 2,
    PA21_UART1_SIROUT = 3,
    PA22_SPI1_MISO    = 0,
    PA22_PWM_CH0      = 1,
    PA22_ADC_A10      = 2,
    PA23_SPI1_MOSI    = 0,
    PA23_PWM_CH1      = 1,
    PA23_ADC_A11      = 2,
    PA24_UART2_TX     = 0,
    PA24_I2S_MCLK     = 1,
    PA24_SPI1_CS0     = 2,
    PA24_UART2_SIROUT = 3,
    PA25_UART2_RX     = 0,
    PA25_I2S_SCLK     = 1,
    PA25_SPI1_CS1     = 2,
    PA25_UART2_SIRIN  = 3,
    PA26_UART2_CTS    = 0,
    PA26_I2S_WSCLK    = 1,
    PA26_ADC_A12      = 2,
    PA27_UART2_RTS    = 0,
    PA27_I2S_SDA      = 1,
    PA27_ADC_A13      = 2,
    PC0_I2C1_SCL      = 0,
    PC0_UART1_CTS     = 1,
    PC0_PWM_CH10      = 2,
    PC0_ADC_A14       = 3,
    PC1_I2C1_SDA      = 0,
    PC1_UART1_RTS     = 1,
    PC1_PWM_CH11      = 2,
    PC1_ADC_A15       = 3,
    PIN_FUNC_GPIO     = 4
} pin_func_e;

typedef enum {
    PORTA = 0,
    PORTB = 1,
} port_name_e;

#ifdef __cplusplus
}
#endif

#endif
