#ifndef _LOS_BSP_UART_H
#define _LOS_BSP_UART_H

#ifdef LOS_STM32F429ZI

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f429i_discovery.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor I2Cx/I2Cx instance used and associated
   resources */
/* Definition for I2Cx clock resources */
#define I2Cx                             I2C3
#define I2Cx_CLK_ENABLE()                __HAL_RCC_I2C3_CLK_ENABLE()
#define I2Cx_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()
#define I2Cx_SCL_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()

#define I2Cx_FORCE_RESET()               __HAL_RCC_I2C3_FORCE_RESET()
#define I2Cx_RELEASE_RESET()             __HAL_RCC_I2C3_RELEASE_RESET()

/* Definition for I2Cx Pins */
#define I2Cx_SCL_PIN                    GPIO_PIN_8
#define I2Cx_SCL_GPIO_PORT              GPIOA
#define I2Cx_SCL_AF                     GPIO_AF4_I2C3
#define I2Cx_SDA_PIN                    GPIO_PIN_9
#define I2Cx_SDA_GPIO_PORT              GPIOC
#define I2Cx_SDA_AF                     GPIO_AF4_I2C3

/* Definition for I2Cx's NVIC */
#define I2Cx_EV_IRQn                    I2C3_EV_IRQn
#define I2Cx_EV_IRQHandler              I2C3_EV_IRQHandler
#define I2Cx_ER_IRQn                    I2C3_ER_IRQn
#define I2Cx_ER_IRQHandler              I2C3_ER_IRQHandler

/* Size of Transmission buffer */
#define TXBUFFERSIZE                      (COUNTOF(aTxBuffer) - 1)
/* Size of Reception buffer */
#define RXBUFFERSIZE                      TXBUFFERSIZE

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* Exported functions ------------------------------------------------------- */

#endif



#endif
