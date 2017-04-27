#ifndef _LOS_BSP_UART_H
#define _LOS_BSP_UART_H

#ifdef LOS_STM32L476xx


#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor USARTx/UARTx instance used and associated 
   resources */
/* Definition for USARTx clock resources */
#define USARTx                          UART4
#define USARTx_CLK_ENABLE()             __HAL_RCC_UART4_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()  // __HAL_RCC_GPIOC_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()  // __HAL_RCC_GPIOC_CLK_ENABLE()

#define USARTx_FORCE_RESET()            __HAL_RCC_UART4_FORCE_RESET()
#define USARTx_RELEASE_RESET()          __HAL_RCC_UART4_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                   GPIO_PIN_0  // GPIO_PIN_10
#define USARTx_TX_GPIO_PORT             GPIOA  // GPIOC
#define USARTx_TX_AF                    GPIO_AF8_UART4
#define USARTx_RX_PIN                   GPIO_PIN_1  // GPIO_PIN_11
#define USARTx_RX_GPIO_PORT             GPIOA  // GPIOC
#define USARTx_RX_AF                    GPIO_AF8_UART4

/* Size of Trasmission buffer */
#define TXBUFFERSIZE                    (COUNTOF(aTxBuffer) - 1)
/* Size of Reception buffer */
#define RXBUFFERSIZE                    TXBUFFERSIZE
  
/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* Exported functions ------------------------------------------------------- */

#endif

void LOS_EvbUartInit(void);
void LOS_EvbUartWriteByte(char c);
void LOS_EvbUartWriteStr(const char* str);
void LOS_EvbUartReadByte(char* c);

#endif
