#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "los_bsp_uart.h"
#include "los_demo_debug.h"

/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
#ifdef GD32F407
#include "gd32f4xx.h"
#include "gd32f407nb-iot_eval.h"
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
#ifdef GD32F407
    //add you code here.
    gd_eval_com_init(EVAL_COM1);
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
void LOS_EvbUartWriteByte(char c)
{
#ifdef GD32F407
    usart_data_transmit(EVAL_COM1, c);
    while (RESET == usart_flag_get(EVAL_COM1, USART_FLAG_TBE));
#endif
    return;
}

/*****************************************************************************
 Function    : Los_Uart1ReadByte
 Description : enable the device on the dev baord
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUartReadByte(char* c)
{
#ifdef GD32F407
    while (RESET == usart_flag_get(EVAL_COM1,USART_FLAG_RBNE));
    *c = (usart_data_receive(EVAL_COM1));
#endif
    return;
}

/*****************************************************************************
 Function    : LOS_EvbUartWriteStr
 Description : Uart write string
 Input       : const char* str
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUartWriteStr(const char* str)
{
#ifdef GD32F4XX
    while (*str)
    {
        usart_data_transmit(EVAL_COM1, * str++);
        while (RESET == usart_flag_get(EVAL_COM1,USART_FLAG_TBE));
    }
        
        while(RESET == usart_flag_get(EVAL_COM1, USART_FLAG_TC)){
        }
#endif
    return;
}

/*****************************************************************************
 Function    : LosUart1Printf
 Description : enable the device on the dev baord
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUartPrintf(char* fmt, ...)
{
    int i;
    static char _buffer[128];
    va_list ap;
    va_start(ap, fmt);
    vsprintf(_buffer, fmt, ap);
    va_end(ap);

#ifdef GD32F407
    for (i = 0; _buffer[i] != '\0'; i++)
    {
        LOS_EvbUartWriteByte(_buffer[i]);
    }
        
        while(RESET == usart_flag_get(EVAL_COM1, USART_FLAG_TC)){
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
