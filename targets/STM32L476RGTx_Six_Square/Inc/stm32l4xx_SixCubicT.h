/**
  ******************************************************************************
  * @file    stm32l0xx_nucleo.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for:
  *          - LEDs and push-button available on STM32L0XX-Nucleo Kit 
  *            from STMicroelectronics
  *          - LCD, joystick and microSD available on Adafruit 1.8" TFT LCD 
  *            shield (reference ID 802)
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32L4XX_NUCLEO_H
#define __STM32L4XX_NUCLEO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l476xx.h"
#include "stm32l4xx_hal.h"


typedef enum 
{
  LED1 = 0,

  LED_GREEN = LED1
} Led_TypeDef;

typedef enum 
{  
  BUTTON_USER = 0,
  /* Alias */
  BUTTON_KEY  = BUTTON_USER
} Button_TypeDef;

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef; 


#define LEDn                               1

#define LED1_PIN                           GPIO_PIN_11
#define LED1_GPIO_PORT                     GPIOC
#define LED1_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOC_CLK_ENABLE()  
#define LED1_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOC_CLK_DISABLE()
  
#define LEDx_GPIO_CLK_ENABLE(__INDEX__)   do { if((__INDEX__) == 0) LED1_GPIO_CLK_ENABLE();} while(0)
#define LEDx_GPIO_CLK_DISABLE(__INDEX__)  (((__INDEX__) == 0) ? LED1_GPIO_CLK_DISABLE() : 0)

#define BUTTONn                            1

/**
  * @brief User push-button
  */
#define USER_BUTTON_PIN                         GPIO_PIN_0
#define USER_BUTTON_GPIO_PORT                   GPIOA
#define USER_BUTTON_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()
#define USER_BUTTON_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()
#define USER_BUTTON_EXTI_IRQn                   EXTI0_IRQn
/* Aliases */
#define KEY_BUTTON_PIN                          USER_BUTTON_PIN
#define KEY_BUTTON_GPIO_PORT                    USER_BUTTON_GPIO_PORT
#define KEY_BUTTON_GPIO_CLK_ENABLE()            USER_BUTTON_GPIO_CLK_ENABLE()
#define KEY_BUTTON_GPIO_CLK_DISABLE()           USER_BUTTON_GPIO_CLK_DISABLE()
#define KEY_BUTTON_EXTI_IRQn                    USER_BUTTON_EXTI_IRQn

#define BUTTONx_GPIO_CLK_ENABLE(__INDEX__)    do { if((__INDEX__) == 0) USER_BUTTON_GPIO_CLK_ENABLE();} while(0)
#define BUTTONx_GPIO_CLK_DISABLE(__INDEX__)   (((__INDEX__) == 0) ? USER_BUTTON_GPIO_CLK_DISABLE() : 0)

#ifdef __cplusplus
}
#endif

#endif /* __STM32L4XX_NUCLEO_H */

/************************ (C) COPYRIGHT YFSoft *****END OF FILE****/
