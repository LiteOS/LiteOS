#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "los_bsp_uart.h"


#ifdef LOS_LPC824
#include "board.h"
static char _buffer[128];
#endif

/*****************************************************************************
 Function    : LOS_EvbUartInit
 Description : enable the device on the dev baord
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUartInit(void)
{
#ifdef LOS_LPC824
    Board_UART_Init();
#endif
    return;
}

/*****************************************************************************
 Function    : LOS_EvbUartWriteByte
 Description : Uart write byte
 Input       : char c
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUartWriteByte(char c)
{
#ifdef LOS_LPC824
    Board_UARTPutTextChar(c);
#endif
    return;
}

/*****************************************************************************
 Function    : LOS_EvbUartReadByte
 Description : Uart read byte
 Input       : None
 Output      : char* c
 Return      : None
 *****************************************************************************/
void LOS_EvbUartReadByte(char* c)
{
#ifdef LOS_LPC824
    *c = Board_UARTGetChar();
#endif
    return;
}

/*****************************************************************************
 Function    : LOS_EvbUartPrintf
 Description : Uart printf
 Input       : char* fmt, ...
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUartPrintf(char* fmt, ...)
{
#ifdef LOS_LPC824
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
 Description : Uart write string
 Input       : const char* str
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUartWriteStr(const char* str)
{
#ifdef LOS_LPC824
    while (*str)
    {
        Board_UARTPutTextChar(* str++);
    }
#endif
    return;
}

