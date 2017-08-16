#include "los_bsp_uart.h"

/******************************************************************************
	here include some special hearder file you need
******************************************************************************/


/*****************************************************************************
 Function    : LOS_EvbUartInit
 Description : enable the device on the dev baord
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUartInit(void)
{
#ifdef LOS_STM32F103ZE
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	USART_TypeDef *uart;

	/* 第1步：打开GPIO和USART部件的时钟 */
	macUSART_APBxClock_FUN(macUSART_CLK, ENABLE);
	macUSART_GPIO_APBxClock_FUN(macUSART_GPIO_CLK, ENABLE);

	/* 第2步：将USART Tx的GPIO配置为推挽复用模式 */
	GPIO_InitStructure.GPIO_Pin = macUSART_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(macUSART_TX_PORT, &GPIO_InitStructure);

	/* 第3步：将USART Rx的GPIO配置为浮空输入模式
		由于CPU复位后，GPIO缺省都是浮空输入模式，因此下面这个步骤不是必须的
		但是，我还是建议加上便于阅读，并且防止其它地方修改了这个口线的设置参数
	*/
	GPIO_InitStructure.GPIO_Pin = macUSART_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(macUSART_RX_PORT, &GPIO_InitStructure);
	/*  第2步已经做了，因此这步可以不做
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	*/
	
	/* 第4步：设置串口硬件参数 */
	uart = macUSARTx;	
	USART_InitStructure.USART_BaudRate = macUSART_BAUD_RATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(uart, &USART_InitStructure);
	USART_ITConfig(uart, USART_IT_RXNE, ENABLE);	/* 使能接收中断 */
	/* 
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		注意: 不要在此处打开发送中断
		发送中断使能在SendUart()函数打开
	*/
	USART_Cmd(uart, ENABLE);		/* 使能串口 */ 

	/*串口配置好，如果直接Send，则第1个字节发送不出去如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART1, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */
#endif
	return ;
}


/*****************************************************************************
 Function    : LOS_EvbUartInit
 Description : enable the device on the dev baord
 Input       : ch
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUartWriteByte(char ch)
{
#ifdef LOS_STM32F103ZE
    /* 发送一个字节数据到USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
#endif
		return;
}

/*****************************************************************************
 Function    : Los_Uart1ReadByte
 Description : enable the device on the dev baord
 Input       : None
 Output      : ch
 Return      : None
 *****************************************************************************/
void LOS_EvbUartReadByte(char* ch)
{
#ifdef LOS_STM32F103ZE
	  /* 等待串口1输入数据 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		*ch = (char)USART_ReceiveData(USART1);
#endif
}

/*****************************************************************************
 Function    : LosUartPrintf
 Description : enable the device on the dev baord
 Input       : char* fmt
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUartPrintf(char* fmt, ...)
{
#ifdef LOS_STM32F103ZE
    int i;
	  static char _buffer[128];
    va_list ap;
    va_start(ap, fmt);
    vsprintf(_buffer, fmt, ap);
    va_end(ap);

    for (i = 0; _buffer[i] != '\0'; i++)
    {
        LOS_EvbUartWriteByte(_buffer[i]);
    }
#endif
}

/*****************************************************************************
 Function    : LOS_EvbUartInit
 Description : enable the device on the dev baord
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUartWriteStr(const char* str)
{
#ifdef LOS_STM32F103ZE
    while (*str)
    {  
			  LOS_EvbUartWriteByte(*str++);  
    }
#endif
	return;
}

#ifndef LOS_KERNEL_TEST_KEIL_SWSIMU

//重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
#ifdef LOS_STM32F103ZE
		/* 发送一个字节数据到USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
#endif
		return (ch);
}

///重定向c库函数scanf到USART1
int fgetc(FILE *f)
{
#ifdef LOS_STM32F103ZE
		/* 等待串口1输入数据 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
#else
	  return LOS_ERR;
#endif
}
#endif
