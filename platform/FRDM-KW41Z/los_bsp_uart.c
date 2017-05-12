#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "los_bsp_uart.h"
#include "los_demo_debug.h"

/******************************************************************************
	here include some special hearder file you need
******************************************************************************/
#ifdef LOS_FRDM_KW41Z
#include "fsl_port.h"
#include "fsl_clock.h"
#include "fsl_lpuart.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
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
#ifdef LOS_FRDM_KW41Z  
    /* Initialize LPUART0 pins below */
    /* Ungate the port clock */
    CLOCK_EnableClock(kCLOCK_PortC);

    PORT_SetPinMux(LPUART_DEBUG_PORT, LPUART_DEBUG_RX_PIN, kPORT_MuxAlt4);
    PORT_SetPinMux(LPUART_DEBUG_PORT, LPUART_DEBUG_TX_PIN, kPORT_MuxAlt4);

    BOARD_BootClockRUN();

    /* SIM_SOPT2[27:26]:
     *  00: Clock Disabled
     *  01: MCGFLLCLK
     *  10: OSCERCLK
     *  11: MCGIRCCLK
     */
    CLOCK_SetLpuartClock(2);
    DbgConsole_Init((uint32_t)LPUART0_BASE, 115200, DEBUG_CONSOLE_DEVICE_TYPE_LPUART, CLOCK_GetOsc0ErClkFreq());
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
 #ifdef LOS_FRDM_KW41Z 
    LPUART_WriteBlocking(LPUART0, (const uint8_t *)&c, 1);
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
 #ifdef LOS_FRDM_KW41Z 
    LPUART_ReadBlocking(LPUART0, (uint8_t *)c, 1);
#endif   
    return;
}

static char _buffer[128];

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
    while(*str) {
        LOS_EvbUartWriteByte(*str);
        str++;
    }

    return;
}

#ifndef LOS_KERNEL_TEST_KEIL_SWSIMU
///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch/*, FILE *f*/)
{
    /* 发送一个字节数据到串口USART */
    LOS_EvbUartWriteByte((char)ch);
    
    return (ch);
}
#endif
