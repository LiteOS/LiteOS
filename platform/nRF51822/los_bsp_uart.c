#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "los_bsp_uart.h"

/******************************************************************************
	here include some special hearder file you need
******************************************************************************/
#ifdef BOARD_NRF51822
#include "app_trace.h"
#include "app_uart.h"
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
#ifdef BOARD_NRF51822
  app_trace_init();
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
#ifdef BOARD_NRF51822
	app_uart_put((uint8_t)c);
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
#ifdef BOARD_NRF51822
  app_uart_get((uint8_t*)c);
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
	static char _buffer[128];
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

#ifndef LOS_KERNEL_TEST_KEIL_SWSIMU
///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口USART */
		LOS_EvbUartWriteByte((char)ch);	
	
		return (ch);
}
#endif
