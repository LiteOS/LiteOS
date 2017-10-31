#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "los_bsp_uart.h"
#include "los_demo_debug.h"

/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
#ifdef LOS_ADuCM450LF
#include "adi_uart.h"
#endif

/*****************************************************************************
    global var
 *****************************************************************************/
#ifdef LOS_ADuCM450LF
ADI_UART_HANDLE hDevOutput = NULL;

#ifdef LOS_PACK_ALIGN_4_IAR
#pragma data_alignment=4
#endif
#ifdef LOS_PACK_ALIGN_4_KEIL
__align(4)
#endif
#ifdef LOS_PACK_ALIGN_4_GCC
__attribute__ ((aligned (4)))
#endif
uint8_t OutDeviceMem[ADI_UART_UNIDIR_MEMORY_SIZE];
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
#ifdef LOS_ADuCM450LF
    /* Set the pinmux for the UART */
    *pREG_GPIO0_CFG |= UART0_TX_PORTP0_MUX | UART0_RX_PORTP0_MUX;

    /* Open the UART device, data transfer is bidirectional with NORMAL mode by default */
    adi_uart_Open(0u, ADI_UART_DIR_TRANSMIT, OutDeviceMem, sizeof OutDeviceMem, &hDevOutput);
    return;
#endif
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
#ifdef LOS_ADuCM450LF
    uint32_t hwError;
    
    adi_uart_Write(hDevOutput,&c,/*nBufSize=*/1, /*bDMA=*/false, &hwError);
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
#ifdef LOS_ADuCM450LF
    uint32_t hwError;
    
    adi_uart_Read(hDevOutput,c,/*nBufSize=*/1, /*bDMA=*/false, &hwError);
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
void LOS_EvbUartWriteStr(char* str)
{
    while (*str)
    {
        LOS_EvbUartWriteByte(*str++);
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

