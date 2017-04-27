#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "los_bsp_uart.h"

#ifdef LOS_LPC54114
#include "board.h"
#include "fsl_debug_console.h"
#endif

/*****************************************************************************
 Function    : LOS_EvbUartInit
 Description : Init Debu Uart
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUartInit(void)
{
#ifdef LOS_LPC54114 
    BOARD_InitDebugConsole();
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
#ifdef LOS_LPC54114
    DbgConsole_Putchar(c);
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
#ifdef LOS_LPC54114
    *c = DbgConsole_Getchar();
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
#ifdef LOS_LPC54114
    while (*str)
    {
        DbgConsole_Putchar(* str++);
    }
#endif
    return;
}

