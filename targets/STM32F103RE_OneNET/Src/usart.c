#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "usart.h"
//#include "los_demo_debug.h"

/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
#ifdef LOS_STM32F103ZE
#include "stm32f10x.h" //OneNET

#endif


/*****************************************************************************
 Function    : LOS_EvbUartInit
 Description : Init uart device
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUartInit(void)
{
    //add you code here.
#ifdef LOS_STM32F103ZE
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    USART_TypeDef *uart;

    /* ??1??????GPIO??USART????????? */
    macUSART_APBxClock_FUN(macUSART_CLK, ENABLE);
    macUSART_GPIO_APBxClock_FUN(macUSART_GPIO_CLK, ENABLE);

    /* ??2??????USART Tx??GPIO????????????? */
    GPIO_InitStructure.GPIO_Pin = macUSART_TX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(macUSART_TX_PORT, &GPIO_InitStructure);

    /* ??3??????USART Rx??GPIO???????????????
        ????CPU??¦Ë??GPIO??????????????????????????????ÙM??????
        ?????????????????????????????????????????????????????¨°???
    */
    GPIO_InitStructure.GPIO_Pin = macUSART_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(macUSART_RX_PORT, &GPIO_InitStructure);
    /*  ??2??????????????????????
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    */

    /* ??4?????????????????? */
    uart = macUSARTx;   
    USART_InitStructure.USART_BaudRate = macUSART_BAUD_RATE;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(uart, &USART_InitStructure);
    USART_ITConfig(uart, USART_IT_RXNE, ENABLE);    /* ???????§Ø? */
    /* 
        USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
        ???: ??????????????§Ø?
        ?????§Ø??????SendUart()??????
    */
    USART_Cmd(uart, ENABLE);        /* ?????? */ 

    /*???????¨²????????Send?????1???????????????????????1??????????????????????? */
    USART_ClearFlag(USART1, USART_FLAG_TC);     /* ?³²?????????Transmission Complete flag */

#endif
    return;
}

/*****************************************************************************
 Function    : LOS_EvbUartWriteByte
 Description : Uart write one byte
 Input       : const char c
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUartWriteByte(const char c)
{
    //add you code here.
#ifdef LOS_STM32F103ZE
    /* ?????????USART1 */
		USART_SendData(USART1, (uint8_t) c);
		
		/* ?????? */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
#endif
    return;
}

/*****************************************************************************
 Function    : LOS_EvbUartReadByte
 Description : Uart reaad one byte
 Input       : char* c
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUartReadByte(char* c)
{
    //add you code here.
#ifdef LOS_STM32F103ZE
	  /* ????1???? */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		*c = (char)USART_ReceiveData(USART1);
#endif
    return;
}

static char _buffer[128];
/*****************************************************************************
 Function    : LosUartPrintf
 Description : Los printf function
 Input       : char* fmt, ...
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUartPrintf(char* fmt, ...)
{
    //add you code here.
    int i;
    va_list ap;
    va_start(ap, fmt);
    vsprintf(_buffer, fmt, ap);
    va_end(ap);

    for (i = 0; _buffer[i] != '\0'; i++)
    {
        LOS_EvbUartWriteByte(_buffer[i]);
    }
    return;
}

/*****************************************************************************
 Function    : LOS_EvbUartWriteStr
 Description : Uart Write String function
 Input       : const char* str
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUartWriteStr(const char* str)
{
    //add you code here.
    while (*str)
    {
        LOS_EvbUartWriteByte(* str++);
    }  
    return;
}



