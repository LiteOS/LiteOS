#ifndef _PLATFORM_CONFIG_H_
#define _PLATFORM_CONFIG_H_

#include "gd32f4xx.h"
#include "gd32f407nb-iot_eval.h"
#include "systick.h"

/* DS18B20 chip DQ pin configuration */
#define DQ_GPIO_CLK             RCU_GPIOD
#define DQ_GPIO_PORT            GPIOD
#define DQ_GPIO_PIN             GPIO_PIN_0

/* GPIO simulation I2C pin configuration */
#define GPIO_I2C_SCL_PORT           GPIOA                       /* GPIO端口 */
#define GPIO_I2C_SDA_PORT           GPIOC                       /* GPIO端口 */
#define GPIO_I2C_SCL_PORT_CLK       RCU_GPIOA                   /* GPIO端口时钟 */
#define GPIO_I2C_SDA_PORT_CLK       RCU_GPIOC                   /* GPIO端口时钟 */
#define GPIO_I2C_SCL_PIN            GPIO_PIN_8                  /* 连接到SCL时钟线的GPIO */
#define GPIO_I2C_SDA_PIN            GPIO_PIN_9                  /* 连接到SDA数据线的GPIO */

////////////////////   USART_NBxx_01  /////////////////////////////////////// 
#define NBxx_01_USART                            USART5   
#define NBxx_01_USART_CLK                        RCU_USART5
#define NBxx_01_USART_RX_GPIO_CLK                RCU_GPIOC
#define NBxx_01_USART_TX_GPIO_CLK                RCU_GPIOC

/* Definition for USART_NBxx_01 Pins */
#define NBxx_01_USART_TX_PIN                     GPIO_PIN_7      
#define NBxx_01_USART_TX_GPIO_PORT               GPIOC
#define NBxx_01_USART_TX_AF                      GPIO_AF_8 

#define NBxx_01_USART_RX_PIN                     GPIO_PIN_6  
#define NBxx_01_USART_RX_GPIO_PORT               GPIOC
#define NBxx_01_USART_RX_AF                      GPIO_AF_8  

/* Definition for USART_NBxx_01's NVIC */
#define NBxx_01_USART_IRQn                       USART5_IRQn        
#define NBxx_01_USART_IRQHandler                 USART5_IRQHandler  

#define NBxx_01_TIMEOUT_TIMER                     TIMER5
#define NBxx_01_TIMEOUT_TIMER_CLK                 RCU_TIMER5
#define NBxx_01_TIMEOUT_TIMER_IRQn                TIMER5_DAC_IRQn
#define NBxx_01_TIMEOUT_TIMER_IRQHandler          TIMER5_DAC_IRQHandler

/* Size of Trasmission buffer */
#define NBxx_BUFFER_SIZE    512 

#define BigtoLittle16(A)   (( ((uint16_t)(A) & 0xff00) >> 8)    |  (( (uint16_t)(A) & 0x00ff) << 8))  

#define BigtoLittle32(A)   ((( (uint32_t)(A) & 0xff000000) >> 24) | \
                                       (( (uint32_t)(A) & 0x00ff0000) >> 8)   | \
                                         (( (uint32_t)(A) & 0x0000ff00) << 8)   | \
                                       (( (uint32_t)(A) & 0x000000ff) << 24))  
 
#define BigtoLittle64(A)              ((( (uint64_t)(A) & 0xff00000000000000) >> 56) | \
                                       (( (uint64_t)(A) & 0x00ff000000000000) >> 40)   | \
                                       (( (uint64_t)(A) & 0x0000ff0000000000) >> 24)   | \
                                       (( (uint64_t)(A) & 0x000000ff00000000) >> 8) |  \
                                       (( (uint64_t)(A) & 0x00000000ff000000) << 8)  |           \
                                       (( (uint64_t)(A) & 0x0000000000ff0000) << 24)   |         \
                                       (( (uint64_t)(A) & 0x000000000000ff00) << 40)   |         \
                                       (( (uint64_t)(A) & 0x00000000000000ff) << 56))  
 
 
#endif /* _PLATFORM_CONFIG_H_ */
