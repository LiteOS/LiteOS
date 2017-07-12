#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "los_bsp_uart.h"

/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
#include "board/board.h"
#include "common/console_io.h"


/*****************************************************************************
 Function    : LOS_EvbUartInit
 Description : Init Debu Uart
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUartInit(void)
{

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
    console_putchar(c);
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
    *c = (char)console_getchar();
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
    console_putstr(str, strlen(str));
}

