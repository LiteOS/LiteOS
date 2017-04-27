#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "los_bsp_uart.h"
#include "los_demo_debug.h"


#ifdef LOS_MM32F103
#include "uart.h"
static char _buffer[128];
#endif


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
#ifdef LOS_MM32F103
    /*初始化串口1,波特率为9600,无奇偶校验,无硬件流控制,1位停止位*/
    uart_initwBaudRate(9600); //串口初始化为9600
#endif
    return;
}

/*****************************************************************************
 Function    : LOS_EvbUartWriteByte
 Description : Uart write one byte
 Input       : const char ch
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUartWriteByte(const char ch)
{
    //add you code here.
#ifdef LOS_MM32F103
    while((UART1->CSR&UART_IT_TXIEN)==0);//等待发送完毕
    UART1->TDR = (ch & (uint16_t)0x00FF);
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
#ifdef LOS_MM32F103
    UART_ReceiveData(UART1);
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
#ifdef LOS_MM32F103
    int i;
    va_list ap;
    va_start(ap, fmt);
    vsprintf(_buffer, fmt, ap);
    va_end(ap);

    for (i = 0; _buffer[i] != '\0'; i++)
    {
        LOS_EvbUartWriteByte(_buffer[i]);
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
#ifdef LOS_MM32F103 
    while (*str)
    {
        LOS_EvbUartWriteByte((uint8_t)*str);
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
