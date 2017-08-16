#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "los_bsp_uart.h"
#include "los_demo_debug.h"

#ifdef GD32F103VCT6
#include "gd32f10x.h"
#endif

static char _buffer[128];
/******************************************************************************
    here include some special hearder file you need
******************************************************************************/



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
	  #ifdef GD32F103VCT6
		    /* Enable GPIOA clock */
    RCC_APB2PeriphClock_Enable( RCC_APB2PERIPH_GPIOA  , ENABLE );
    
    /* Enable USART2 APB clock */
    RCC_APB1PeriphClock_Enable( RCC_APB1PERIPH_USART2  , ENABLE );
      
    /* USART2 Pins configuration **************************************************/
    {
        /* Configure the GPIO ports */
        GPIO_InitPara  GPIO_InitStructure;
       
        GPIO_InitStructure.GPIO_Pin     = GPIO_PIN_2 ;
        GPIO_InitStructure.GPIO_Mode    = GPIO_MODE_AF_PP;
        GPIO_InitStructure.GPIO_Speed   = GPIO_SPEED_50MHZ;
        GPIO_Init( GPIOA , &GPIO_InitStructure); 
        GPIO_InitStructure.GPIO_Pin     = GPIO_PIN_3;
        GPIO_InitStructure.GPIO_Mode    = GPIO_MODE_IN_FLOATING;;
        GPIO_Init( GPIOA , &GPIO_InitStructure); 
    }
    { 
        USART_InitPara  USART_InitStructure;
        
        USART_DeInit( USART2 );        
        USART_InitStructure.USART_BRR                 = 115200;
        USART_InitStructure.USART_WL                  = USART_WL_8B;
        USART_InitStructure.USART_STBits              = USART_STBITS_1;
        USART_InitStructure.USART_Parity              = USART_PARITY_RESET;
        USART_InitStructure.USART_HardwareFlowControl = USART_HARDWAREFLOWCONTROL_NONE;
        USART_InitStructure.USART_RxorTx              = USART_RXORTX_RX | USART_RXORTX_TX;
        USART_Init(USART2, &USART_InitStructure);
    }

    /* USART enable */
    USART_Enable(USART2, ENABLE);

    /* Output a message on Hyperterminal using printf function */
    printf("\n\r USART2 Printf Example: Please prees the Wakeup Key \n\r");
    
    /* Loop until the end of transmission */
    /* The software must wait until TC=1. The TC flag remains cleared during all data
    transfers and it is set by hardware when the one frame transmission complete */
    while ( USART_GetBitState( USART2 , USART_FLAG_TC  ) == RESET )
    {}
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
	  #ifdef GD32F103VCT6
        /* e.g. write a character to the USART */
    USART_DataSend( USART2 , (uint8_t) c );

    /* Loop until transmit data register is empty */
    while ( USART_GetBitState( USART2 , USART_FLAG_TBE ) == RESET)
    {}
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
	  #ifdef GD32F103VCT6
     while (RESET == USART_GetBitState(USART2,USART_FLAG_RBNE));
    *c = (USART_DataReceive(USART2));
	  #endif
    return;
}

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
	  #ifdef GD32F103VCT6
    int i;
    va_list ap;
    va_start(ap, fmt);
    vsprintf(_buffer, fmt, ap);
    va_end(ap);
    
    for (i = 0; _buffer[i] != '\0'; i++)
    {
        LOS_EvbUartWriteByte(_buffer[i]);
    }
        
    while(RESET == USART_GetBitState(USART2, USART_FLAG_TC))
		{ }
		#endif
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
		while(*str)
		{
			LOS_EvbUartWriteByte(*str++);
		}
    return;
}

#ifndef LOS_KERNEL_TEST_KEIL_SWSIMU
///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
    /* 发送一个字节数据到串口USART */
    LOS_EvbUartWriteByte((char)ch);
    			
    return (ch);
}
#endif
