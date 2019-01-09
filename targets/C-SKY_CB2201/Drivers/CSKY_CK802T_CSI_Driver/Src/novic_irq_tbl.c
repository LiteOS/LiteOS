/*
 * Copyright (C) 2016 YunOS Project. All rights reserved.
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

#include <csi_config.h>

extern void NOVIC_IRQ_Default_Handler(void);
extern void CORET_IRQHandler(void);

extern void TIMA0_IRQHandler(void);
extern void TIMA1_IRQHandler(void);
extern void TIMB0_IRQHandler(void);
extern void TIMB1_IRQHandler(void);
extern void WDT_IRQHandler(void);
extern void USART0_IRQHandler(void);
extern void USART1_IRQHandler(void);
extern void USART2_IRQHandler(void);
extern void USART3_IRQHandler(void);
extern void I2C0_IRQHandler(void);
extern void I2C1_IRQHandler(void);
extern void SPI0_IRQHandler(void);
extern void SPI1_IRQHandler(void);
extern void GPIO0_IRQHandler(void);
extern void GPIO1_IRQHandler(void);
extern void RTC_IRQHandler(void);
extern void RTC1_IRQHandler(void);
extern void AES_IRQHandler(void);
extern void SHA_IRQHandler(void);
extern void RSA_IRQHandler(void);
extern void ADC_IRQHandler(void);
extern void I2S_IRQHandler(void);
extern void DMAC0_IRQHandler(void);

void (*g_irqvector[])(void) = {
    GPIO0_IRQHandler, /* 0, default interrupt entry */
    CORET_IRQHandler,  /* 1, default interrupt entry */
    TIMA0_IRQHandler, /* 2, default interrupt entry */
    TIMA1_IRQHandler, /* 3, default interrupt entry */
    I2S_IRQHandler , /* 4, default interrupt entry */
    WDT_IRQHandler, /* 5, default interrupt entry */
    USART0_IRQHandler, /* 6, default interrupt entry */
    USART1_IRQHandler, /* 7, default interrupt entry */
    USART2_IRQHandler, /* 8, default interrupt entry */
    I2C0_IRQHandler, /* 9, default interrupt entry */
    I2C1_IRQHandler, /* 10, default interrupt entry */
    SPI1_IRQHandler, /* 11, default interrupt entry */
    SPI0_IRQHandler, /* 12, default interrupt entry */
    RTC_IRQHandler, /* 13, default interrupt entry */
    NOVIC_IRQ_Default_Handler, /* 14, default interrupt entry */
    ADC_IRQHandler,            /* 15, ADC */
    NOVIC_IRQ_Default_Handler, /* 16, default interrupt entry */
    DMAC0_IRQHandler, /* 17, default interrupt entry */
    NOVIC_IRQ_Default_Handler, /* 18, default interrupt entry */
    NOVIC_IRQ_Default_Handler, /* 19, default interrupt entry */
    NOVIC_IRQ_Default_Handler, /* 20, default interrupt entry */
    NOVIC_IRQ_Default_Handler, /* 21, default interrupt entry */
    NOVIC_IRQ_Default_Handler, /* 22, default interrupt entry */
    TIMB0_IRQHandler, /* 23, default interrupt entry */
    TIMB1_IRQHandler, /* 24, default interrupt entry */
    NOVIC_IRQ_Default_Handler, /* 25, default interrupt entry */
    AES_IRQHandler, /* 26, default interrupt entry */
    GPIO1_IRQHandler, /* 27, default interrupt entry */
    RSA_IRQHandler, /* 28, default interrupt entry */
    SHA_IRQHandler, /* 29, default interrupt entry */
};
