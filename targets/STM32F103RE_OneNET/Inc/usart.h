#ifndef _USART_H
#define _USART_H

#include <stdio.h>
#include <string.h>

#include "stm32f10x.h"

#define             macUSART_BAUD_RATE                       115200
#define             macUSARTx                                USART1
#define             macUSART_APBxClock_FUN                   RCC_APB2PeriphClockCmd
#define             macUSART_CLK                             RCC_APB2Periph_USART1
#define             macUSART_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             macUSART_GPIO_CLK                        (RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO)     
#define             macUSART_TX_PORT                         GPIOA   
#define             macUSART_TX_PIN                          GPIO_Pin_9
#define             macUSART_RX_PORT                         GPIOA   
#define             macUSART_RX_PIN                          GPIO_Pin_10

extern void LOS_EvbUartInit(void);
extern void LOS_EvbUartReadByte(char* c);
extern void LOS_EvbUartWriteByte(const char c);
extern void LOS_EvbUartWriteStr(const char* str);


#endif

