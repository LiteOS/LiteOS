#include "los_bsp_uart.h"
#include <stdarg.h>

#ifdef EFM32HG322F64
#include "efm32hg322f64.h"
#include "retargetVirtual.h"

void Error_Handler(void)
{
    while(1);
}
#endif

void LOS_EvbUartInit(void)
{
#ifdef EFM32HG322F64
    /* Initialize LEUART/USART and map LF to CRLF */
    RETARGET_SerialInit();
    RETARGET_SerialCrLf(1);
#endif
    
    return ;
}

void LOS_EvbUartWriteByte(char c)
{
#ifdef EFM32HG322F64
    RETARGET_WriteChar(c);//USART_RETARGET_WriteChar(c);
#endif
    return ;
}

void LOS_EvbUartWriteStr(const char* str)
{
#ifdef EFM32HG322F64
    while (*str)
    {
        RETARGET_WriteChar(*str);//USART_RETARGET_WriteChar(*str);
        str++;
    }
#endif
    return ;
}


void LOS_EvbUartReadByte(char* c)
{
#ifdef EFM32HG322F64
    *c = USART_RETARGET_ReadChar();
#endif
    return ;
}

#ifdef EFM32HG322F64
static char _buffer[128];
#endif
void LOS_EvbUartPrintf(char* fmt, ...)
{
#ifdef EFM32HG322F64
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
    return ;
}

/* End of file */

