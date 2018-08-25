/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
 /*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

#ifndef __CC32xx_H
#define __CC32xx_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

/** @addtogroup Configuration_section_for_CMSIS
  * @{
  */

/**
  * @brief Configuration of the Cortex-M4 Processor and Core Peripherals
  */
#define __CM4_REV                 0x0001U  /*!< Core revision r0p1                            */
#define __MPU_PRESENT             1U       /*!< CC32xx provides an MPU                        */
#define __NVIC_PRIO_BITS          4U       /*!< CC32xx uses 4 Bits for the Priority Levels    */
#define __Vendor_SysTickConfig    0U       /*!< Set to 1 if different SysTick Config is used  */
#define __FPU_PRESENT             1U       /*!< FPU present                                   */

typedef enum
{
/******  Cortex-M4 Processor Exceptions Numbers ****************************************************************/
  NonMaskableInt_IRQn           = -14,    /*!< 2 Non Maskable Interrupt                                         */
  MemoryManagement_IRQn         = -12,    /*!< 4 Cortex-M4 Memory Management Interrupt                          */
  BusFault_IRQn                 = -11,    /*!< 5 Cortex-M4 Bus Fault Interrupt                                  */
  UsageFault_IRQn               = -10,    /*!< 6 Cortex-M4 Usage Fault Interrupt                                */
  SVCall_IRQn                   = -5,     /*!< 11 Cortex-M4 SV Call Interrupt                                   */
  DebugMonitor_IRQn             = -4,     /*!< 12 Cortex-M4 Debug Monitor Interrupt                             */
  PendSV_IRQn                   = -2,     /*!< 14 Cortex-M4 Pend SV Interrupt                                   */
  SysTick_IRQn                  = -1,     /*!< 15 Cortex-M4 System Tick Interrupt                               */
/******  CC32xx specific Interrupt Numbers **********************************************************************/
  GPIOA0_IRQn                   = 0,      /*!< GPIO Port S0 Interrupt                                           */
  GPIOA1_IRQn                   = 1,      /*!< GPIO Port S1 Interrupt                                           */
  GPIOA2_IRQn                   = 2,      /*!< GPIO Port S2 interrupt                                           */
  GPIOA3_IRQn                   = 3,      /*!< GPIO Port S3 interrupt                                           */
  UART0_IRQn                    = 5,      /*!< UART0 Rx and Tx Interrupt                                        */
  UART1_IRQn                    = 6,      /*!< UART1 Rx and Tx Interrupt                                        */
  I2C_IRQn                      = 8,      /*!< I2C controller Interrupt                                         */
  ADCCH0_IRQn                   = 14,     /*!< ADC Sequence 0 Interrupt                                         */
  ADCCH1_IRQn                   = 15,     /*!< ADC Sequence 1 Interrupt                                         */
  ADCCH2_IRQn                   = 16,     /*!< ADC Sequence 2 Interrupt                                         */
  ADCCH3_IRQn                   = 17,     /*!< ADC Sequence 3 Interrupt                                         */
  WDT_IRQn                      = 18,     /*!< Watchdog Timer0 Interrupt                                        */
  TIMERA0A_IRQn                 = 19,     /*!< Timer 0 subtimer A Interrupt                                     */
  TIMERA0B_IRQn                 = 20,     /*!< Timer 0 subtimer B Interrupt                                     */
  TIMERA1A_IRQn                 = 21,     /*!< Timer 1 subtimer A Interrupt                                     */
  TIMERA1B_IRQn                 = 22,     /*!< Timer 1 subtimer B Interrupt                                     */
  TIMERA2A_IRQn                 = 23,     /*!< Timer 2 subtimer A Interrupt                                     */
  TIMERA2B_IRQn                 = 24,     /*!< Timer 2 subtimer B Interrupt                                     */
  FLASH_IRQn                    = 29,     /*!< FLASH Control Interrupts                                         */
  TIMERA3A_IRQn                 = 35,     /*!< Timer 3 subtimer A Interrupt                                     */
  TIMERA3B_IRQn                 = 36,     /*!< Timer 3 subtimer B Interrupt                                     */
  UDMA_IRQn                     = 46,     /*!< uDMA controller Interrupt                                        */
  UDMAERR_IRQn                  = 47,     /*!< uDMA Error Interrupt                                             */
  SHA_IRQn                      = 148,    /*!< SHA Interrupts                                                   */
  AES_IRQn                      = 151,    /*!< AES interrupt                                                    */
  DES_IRQn                      = 153,    /*!< DES interrupt                                                    */
  MMCHS_IRQn                    = 159,    /*!< SDIO interrupt                                                   */
  I2S_IRQn                      = 161,    /*!< McAPS Interrupt                                                  */
  CAMERA_IRQn                   = 163,    /*!< Camera Interrupt                                                 */
  NWPIC_IRQn                    = 171,    /*!< Interprocessor communication Interrupt                           */
  PRCM_IRQn                     = 172,    /*!< Power, Reset and Clock Module Interrupt                          */
  SSPI_IRQn                     = 175,    /*!< Shared SPI Interrupt                                             */
  GSPI_IRQn                     = 176,    /*!< Generic SPI Interrupt                                            */
  LSPI_IRQn                     = 177     /*!< Link SPI Interrupt                                               */
} IRQn_Type;

/**
  * @}
  */

#include "core_cm4.h"             /* Cortex-M4 processor and core peripherals */
#include "system_cc32xx.h"
#include <stdint.h>


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CC32xx_H */

