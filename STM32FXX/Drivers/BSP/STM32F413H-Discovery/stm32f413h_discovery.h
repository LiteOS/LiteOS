/**
  ******************************************************************************
  * @file    stm32f413h_discovery.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    27-January-2017
  * @brief   This file contains definitions for STM32F413H_DISCOVERY's LEDs,
  *          push-buttons and COM ports hardware resources.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
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
#ifndef __STM32F413H_DISCOVERY_H
#define __STM32F413H_DISCOVERY_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
   
/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F413H_DISCOVERY
  * @{
  */
      
/** @addtogroup STM32F413H_DISCOVERY_LOW_LEVEL
  * @{
  */ 

/** @defgroup STM32F413H_DISCOVERY_LOW_LEVEL_Exported_Types STM32F413H DISCOVERY Low Level Exported Types
  * @{
  */
typedef enum 
{
LED3 = 0,
LED_GREEN = LED3,
LED4 = 1,
LED_RED = LED4,
}Led_TypeDef;


typedef enum 
{  
  BUTTON_WAKEUP = 0
}Button_TypeDef;

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
}ButtonMode_TypeDef;

typedef enum 
{
  COM1 = 0,
}COM_TypeDef;
/**
  * @}
  */ 

/** @defgroup STM32F413H_DISCOVERY_LOW_LEVEL_Exported_Constants STM32F413H DISCOVERY Low Level Exported Constants
  * @{
  */ 

/** 
  * @brief  Define for STM32F413H_DISCOVERY board
  */ 
#if !defined (USE_STM32F413H_DISCOVERY)
 #define USE_STM32F413H_DISCOVERY
#endif

/** @addtogroup STM32F413H_DISCOVERY_LOW_LEVEL_LED STM32F413H DISCOVERY Low Level Led
  * @{
  */
#define LEDn                             ((uint8_t)2)

#define LED4_GPIO_PORT                   GPIOE
#define LED4_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOE_CLK_ENABLE()
#define LED4_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOE_CLK_DISABLE()

#define LED3_GPIO_PORT                   GPIOC
#define LED3_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOC_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOC_CLK_DISABLE()

#define LED4_PIN                         GPIO_PIN_3
#define LED3_PIN                         GPIO_PIN_5

#define LEDx_GPIO_CLK_ENABLE(__INDEX__)  do{if((__INDEX__) == 0) LED3_GPIO_CLK_ENABLE(); else \
                                            if((__INDEX__) == 1) LED4_GPIO_CLK_ENABLE(); \
                                            }while(0)

#define LEDx_GPIO_CLK_DISABLE(__INDEX__)  do{if((__INDEX__) == 0) LED3_GPIO_CLK_DISABLE(); else \
                                             if((__INDEX__) == 1) LED4_GPIO_CLK_DISABLE(); \
                                             }while(0)
/**
  * @}
  */ 
  
/** @addtogroup STM32F413H_DISCOVERY_LOW_LEVEL_BUTTON STM32F413H DISCOVERY Low Level Button
  * @{
  */ 
/* Only one User/Wakeup button */
#define BUTTONn                             ((uint8_t)1)

/**
  * @brief Wakeup push-button
  */
#define WAKEUP_BUTTON_PIN                   GPIO_PIN_0
#define WAKEUP_BUTTON_GPIO_PORT             GPIOA
#define WAKEUP_BUTTON_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define WAKEUP_BUTTON_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOA_CLK_DISABLE()
#define WAKEUP_BUTTON_EXTI_IRQn             EXTI0_IRQn

/**
  * @}
  */ 

/** @addtogroup STM32F413H_DISCOVERY_LOW_LEVEL_SIGNAL
  * @{
  */
/**
  * @brief SD-detect signal
  */
#define SD_DETECT_PIN                        GPIO_PIN_11
#define SD_DETECT_GPIO_PORT                  GPIOF
#define SD_DETECT_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOF_CLK_ENABLE()
#define SD_DETECT_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOF_CLK_DISABLE()
#define SD_DETECT_EXTI_IRQn                  EXTI15_10_IRQn


/**
  * @brief TS INT pin
  */
#define TS_INT_PIN                        GPIO_PIN_1
#define TS_INT_GPIO_PORT                  GPIOC
#define TS_INT_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOC_CLK_ENABLE()
#define TS_INT_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOC_CLK_DISABLE()
#define TS_INT_EXTI_IRQn                  EXTI1_IRQn

/**
  * @brief TS RST pin
  */
#define TS_RESET_PIN                        GPIO_PIN_13
#define TS_RESET_GPIO_PORT                  GPIOB
#define TS_RESET_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOB_CLK_ENABLE()
#define TS_RESET_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOB_CLK_DISABLE()
#define TS_RESET_EXTI_IRQn                  EXTI15_10_IRQn
                                                 
/**
  * @}
  */

/** @addtogroup STM32F413H_DISCOVERY_LOW_LEVEL_COM STM32F413H DISCOVERY Low Level COM
  * @{
  */
#define COMn                              ((uint8_t)1)

/**
 * @brief Definition for COM port1, connected to USART6
 */ 
#define DISCOVERY_COM1                          USART6
#define DISCOVERY_COM1_CLK_ENABLE()             __HAL_RCC_USART6_CLK_ENABLE()
#define DISCOVERY_COM1_CLK_DISABLE()            __HAL_RCC_USART6_CLK_DISABLE()

#define DISCOVERY_COM1_TX_PIN                   GPIO_PIN_14
#define DISCOVERY_COM1_TX_GPIO_PORT             GPIOG
#define DISCOVERY_COM1_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOG_CLK_ENABLE()   
#define DISCOVERY_COM1_TX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOG_CLK_DISABLE()  
#define DISCOVERY_COM1_TX_AF                    GPIO_AF8_USART6

#define DISCOVERY_COM1_RX_PIN                   GPIO_PIN_9
#define DISCOVERY_COM1_RX_GPIO_PORT             GPIOG
#define DISCOVERY_COM1_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOG_CLK_ENABLE()   
#define DISCOVERY_COM1_RX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOG_CLK_DISABLE()  
#define DISCOVERY_COM1_RX_AF                    GPIO_AF8_USART6

#define DISCOVERY_COM1_IRQn                     USART6_IRQn

#define DISCOVERY_COMx_CLK_ENABLE(__INDEX__)            do { if((__INDEX__) == COM1) {DISCOVERY_COM1_CLK_ENABLE();} } while(0)
#define DISCOVERY_COMx_CLK_DISABLE(__INDEX__)           (((__INDEX__) == 0) ? DISCOVERY_COM1_CLK_DISABLE() : 0)

#define DISCOVERY_COMx_TX_GPIO_CLK_ENABLE(__INDEX__)    do { if((__INDEX__) == COM1) {DISCOVERY_COM1_TX_GPIO_CLK_ENABLE();} } while(0)
#define DISCOVERY_COMx_TX_GPIO_CLK_DISABLE(__INDEX__)   (((__INDEX__) == 0) ? DISCOVERY_COM1_TX_GPIO_CLK_DISABLE() : 0)

#define DISCOVERY_COMx_RX_GPIO_CLK_ENABLE(__INDEX__)    do { if((__INDEX__) == COM1) {DISCOVERY_COM1_RX_GPIO_CLK_ENABLE();} } while(0)
#define DISCOVERY_COMx_RX_GPIO_CLK_DISABLE(__INDEX__)   (((__INDEX__) == 0) ? DISCOVERY_COM1_RX_GPIO_CLK_DISABLE() : 0)

/* Exported constant IO ------------------------------------------------------*/

#define AUDIO_I2C_ADDRESS                ((uint16_t)0x34)
#define EEPROM_I2C_ADDRESS_A01           ((uint16_t)0xA0)
#define EEPROM_I2C_ADDRESS_A02           ((uint16_t)0xA6)  
#define TS_I2C_ADDRESS                   ((uint16_t)0x70)

/* User can use this section to tailor I2Cx/I2Cx instance used and associated 
   resources */
/* Definition for AUDIO I2Cx resources */
#define DISCOVERY_AUDIO_I2Cx                             FMPI2C1
#define DISCOVERY_AUDIO_I2Cx_CLK_ENABLE()                __HAL_RCC_FMPI2C1_CLK_ENABLE()
#define DISCOVERY_AUDIO_DMAx_CLK_ENABLE()                __HAL_RCC_DMA1_CLK_ENABLE()
#define DISCOVERY_AUDIO_I2Cx_SCL_SDA_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOC_CLK_ENABLE()

#define DISCOVERY_AUDIO_I2Cx_FORCE_RESET()               __HAL_RCC_FMPI2C1_FORCE_RESET()
#define DISCOVERY_AUDIO_I2Cx_RELEASE_RESET()             __HAL_RCC_FMPI2C1_RELEASE_RESET()

/* Definition for I2Cx Pins */
#define DISCOVERY_AUDIO_I2Cx_SCL_PIN                     GPIO_PIN_6
#define DISCOVERY_AUDIO_I2Cx_SCL_SDA_GPIO_PORT           GPIOC
#define DISCOVERY_AUDIO_I2Cx_SCL_SDA_AF                  GPIO_AF4_FMPI2C1
#define DISCOVERY_AUDIO_I2Cx_SDA_PIN                     GPIO_PIN_7

/* I2C interrupt requests */
#define DISCOVERY_AUDIO_I2Cx_EV_IRQn                     FMPI2C1_EV_IRQn
#define DISCOVERY_AUDIO_I2Cx_ER_IRQn                     FMPI2C1_ER_IRQn

/* I2C clock speed configuration (in Hz)
  WARNING:
   Make sure that this define is not already declared in other files.
   It can be used in parallel by other modules. */
#ifndef DISCOVERY_I2C_SPEED
 #define DISCOVERY_I2C_SPEED                             100000
#endif /* DISCOVERY_I2C_SPEED */

#ifndef DISCOVERY_I2Cx_TIMING  
#define DISCOVERY_I2Cx_TIMING                     ((uint32_t)0x00901954)
#endif /* DISCOVERY_I2Cx_TIMING */ 
/**
  * @}
  */

/**
  * @}
  */

/** @defgroup STM32F413H_DISCOVERY_LOW_LEVEL_Exported_Functions STM32F413H DISCOVERY Low Level Exported Functions
  * @{
  */
uint32_t         BSP_GetVersion(void);  
void             BSP_LED_Init(Led_TypeDef Led);
void             BSP_LED_DeInit(Led_TypeDef Led);
void             BSP_LED_On(Led_TypeDef Led);
void             BSP_LED_Off(Led_TypeDef Led);
void             BSP_LED_Toggle(Led_TypeDef Led);
void             BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);
void             BSP_PB_DeInit(Button_TypeDef Button);
uint32_t         BSP_PB_GetState(Button_TypeDef Button);
void             BSP_COM_Init(COM_TypeDef COM, UART_HandleTypeDef *husart);
void             BSP_COM_DeInit(COM_TypeDef COM, UART_HandleTypeDef *huart);

/**
  * @}
  */

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F413H_DISCOVERY_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
