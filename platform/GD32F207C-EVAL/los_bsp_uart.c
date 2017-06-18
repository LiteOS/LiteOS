#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "los_bsp_uart.h"
#include "gd32f20x_gpio.h"

int fputc(int ch, FILE *f)
{
    USART_DataSend(USART1,ch);
    while (USART_GetBitState(USART1, USART_FLAG_TC) == RESET);
    return ch;
}

/*****************************************************************************
 Function    : LOS_EvbUartInit
 Description : usart init
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUartInit(void)
{
    /* Configure the GPIO ports */
    GPIO_InitPara  GPIO_InitStructure;
    USART_InitPara  USART_InitStructure;
    //NVIC_InitPara NVIC_InitStructure;

    /* Enable GPIOA clock */
    RCC_APB2PeriphClock_Enable( RCC_APB2PERIPH_GPIOA, ENABLE );
    /* Enable USART1 APB clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_USART1, ENABLE );
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_AF, ENABLE);
    GPIO_InitStructure.GPIO_Pin     = GPIO_PIN_9 ;
    GPIO_InitStructure.GPIO_Mode    = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Speed   = GPIO_SPEED_50MHZ;
    GPIO_Init( GPIOA , &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin     = GPIO_PIN_10;
    GPIO_InitStructure.GPIO_Mode    = GPIO_MODE_IN_FLOATING;;
    GPIO_Init( GPIOA , &GPIO_InitStructure);

    USART_DeInit(USART1 );
    USART_InitStructure.USART_BRR                 = 115200;
    USART_InitStructure.USART_WL                  = USART_WL_8B;
    USART_InitStructure.USART_STBits              = USART_STBITS_1;
    USART_InitStructure.USART_Parity              = USART_PARITY_RESET;
    USART_InitStructure.USART_HardwareFlowControl = USART_HARDWAREFLOWCONTROL_NONE;
    USART_InitStructure.USART_RxorTx              = USART_RXORTX_RX | USART_RXORTX_TX;
    USART_Init(USART1, &USART_InitStructure);

    /* USART enable */
    USART_Enable(USART1, ENABLE);	    
}

/*****************************************************************************
 Function    : LOS_EvbUartWriteStr
 Description : send str to usart
 Input       : str      --- string need to send
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUartWriteStr(const char* str)
{
    while (*str)
    {
        USART_DataSend(USART1,*str);
        while (USART_GetBitState(USART1, USART_FLAG_TC) == RESET);
        
        str++;
    }
    
    return;
}
