#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "los_bsp_uart.h"
#include "los_demo_debug.h"

/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
#ifdef LOS_NRF52840
#include "nrf_gpio.h"
#endif

/*****************************************************************************
    here implements some special bsp code you need
******************************************************************************/
#ifdef LOS_NRF52840

static char _buffer[128];

void uart_init(void)
{
	nrf_gpio_cfg_output(TX_PIN_NUMBER);   
	nrf_gpio_cfg_input(RX_PIN_NUMBER, NRF_GPIO_PIN_NOPULL);

	NRF_UART0->PSELTXD =TX_PIN_NUMBER;
	NRF_UART0->PSELRXD =RX_PIN_NUMBER;

	NRF_UART0->BAUDRATE         = (UART_BAUDRATE_BAUDRATE_Baud115200 << UART_BAUDRATE_BAUDRATE_Pos);   
	NRF_UART0->ENABLE           = (UART_ENABLE_ENABLE_Enabled << UART_ENABLE_ENABLE_Pos);           
	NRF_UART0->TASKS_STARTTX    = 1;
	NRF_UART0->TASKS_STARTRX    = 1;
	NRF_UART0->EVENTS_RXDRDY    = 0;
}
   
void uart_send(unsigned char  tx)  
{
	NRF_UART0->TXD = tx;

	while (NRF_UART0->EVENTS_TXDRDY!=1)
	{
	// Wait for TXD data to be sent
	}

	NRF_UART0->EVENTS_TXDRDY=0;
}

unsigned char uart_get(void)  
{
	while (NRF_UART0->EVENTS_RXDRDY != 1)
	{
	// Wait for RXD data to be received
	}
	NRF_UART0->EVENTS_RXDRDY = 0;
	return (uint8_t)NRF_UART0->RXD;
}
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
#ifdef LOS_NRF52840
	uart_init();
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
#ifdef LOS_NRF52840
	uart_send(c);
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
#ifdef LOS_NRF52840
		*c =uart_get();	
#endif
    return ;
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
#ifdef LOS_NRF52840
    int i;
    va_list ap;
    va_start(ap, fmt);
    vsprintf(_buffer, fmt, ap);
    va_end(ap);

    for (i = 0; _buffer[i] != '\0'; i++)
    {
        uart_send(_buffer[i]);
    }
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
#ifdef LOS_NRF52840
    while (*str)
    {				
		uart_send(*str);
        str++;
    }
#endif
		    
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
