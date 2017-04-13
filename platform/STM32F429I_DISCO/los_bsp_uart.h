#ifndef _LOS_BSP_UART_H
#define _LOS_BSP_UART_H

#ifdef LOS_STM32F429ZI


#include "stm32f4xx_hal.h"
#include "stm32f429i_discovery.h"

#define USARTx                              USART1
#define USARTx_CLK_ENABLE()                 __HAL_RCC_USART1_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE() 

#define USARTx_FORCE_RESET()                __HAL_RCC_USART1_FORCE_RESET()
#define USARTx_RELEASE_RESET()              __HAL_RCC_USART1_RELEASE_RESET()

/* Definition for USARTx Pins */
//#define USARTx_TX_PIN                     GPIO_PIN_9
//#define USARTx_TX_GPIO_PORT               GPIOA
//#define USARTx_TX_AF                      GPIO_AF7_USART1
//#define USARTx_RX_PIN                     GPIO_PIN_10
//#define USARTx_RX_GPIO_PORT               GPIOA
//#define USARTx_RX_AF                      GPIO_AF7_USART1

#define USARTx_TX_PIN                       GPIO_PIN_6
#define USARTx_TX_GPIO_PORT                 GPIOB  
#define USARTx_TX_AF                        GPIO_AF7_USART1
#define USARTx_RX_PIN                       GPIO_PIN_7
#define USARTx_RX_GPIO_PORT                 GPIOB
#define USARTx_RX_AF                        GPIO_AF7_USART1


/* Size of Transmission buffer */
#define TXBUFFERSIZE                        (COUNTOF(aTxBuffer) - 1)
/* Size of Reception buffer */
#define RXBUFFERSIZE                        TXBUFFERSIZE

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

/* Exported functions ------------------------------------------------------- */

#endif

void LOS_EvbUartInit(void);
void LOS_EvbUartWriteByte(char c);
void LOS_EvbUartWriteStr(const char* str);
void LOS_EvbUartReadByte(char* c);

#endif
