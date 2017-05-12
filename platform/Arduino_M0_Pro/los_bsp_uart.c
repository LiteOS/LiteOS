#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "los_bsp_uart.h"
#include "los_demo_debug.h"
#include "los_hwi.h"
#include "los_hw.h"

#include "samd21.h"
#define BaudRate 9600
/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
static char _buffer[128];


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
    PM->APBCMASK.reg |= 0x01<<7;
    GCLK->CLKCTRL.reg|= (0x1<<14) |0x19;
    REG_SERCOM5_USART_CTRLA=(0x1<<30) |(0x3<<20) |(0x1<<16) |(0x1<<13) |(0x1<<2);
    //  REG_SERCOM5_USART_INTENSET=0x4;
    REG_SERCOM5_USART_BAUD =SystemCoreClock/(16*BaudRate);
    REG_SERCOM5_USART_CTRLB|=0x3<<16;
    PORT->Group[1].PINCFG[22].reg = 0x1;
    PORT->Group[1].PINCFG[23].reg = 0x1;
    PORT->Group[1].PMUX[11].reg = 0x33;
    REG_SERCOM5_USART_CTRLA|=0x02;
    
//  (*(RwReg  *)0xE000E100) = 0x4000;
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

    REG_SERCOM5_USART_DATA=c;
    while(!(REG_SERCOM5_USART_INTFLAG&0x01));

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
    while(!(REG_SERCOM5_USART_INTFLAG&0x04));
    *c = REG_SERCOM5_USART_DATA;
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
    int i;

    va_list ap;
    va_start(ap, fmt);
    vsprintf(_buffer, fmt, ap);
    va_end(ap);

    for (i = 0; _buffer[i] != '\0'; i++)
    {
        LOS_EvbUartWriteByte(_buffer[i]);
    }

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
    while (*str)
    {
        LOS_EvbUartWriteByte(* str++);
    }  
}

#ifndef LOS_KERNEL_TEST_KEIL_SWSIMU
//重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
    /* 发送一个字节数据到串口USART */
    LOS_EvbUartWriteByte((char)ch);

    return (ch);
}
#endif

