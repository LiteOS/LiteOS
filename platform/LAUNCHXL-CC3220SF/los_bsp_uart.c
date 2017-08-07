#include "los_bsp_uart.h"
#include <stdarg.h>

#ifdef LAUNCHXL_CC3220SF

void Error_Handler(void)
{
    while(1);
}
#endif

void LOS_EvbUartInit(void)
{
#ifdef LAUNCHXL_CC3220SF
    *((volatile unsigned long *)(0x44025080))  =  0x01;          /* Enable UART clock        */
    *((volatile unsigned long *)(0x4000C030)) &=  0xFFFFFFFE;    /* Disable Uart             */ 
    *((volatile unsigned long *)(0x4000C02C)) &= ~0x10;          /* Flush FIFO               */	
    *((volatile unsigned long *)(0x4402E0A4))  =  0x63;          /* PIN as UART0_TX          */ 
    *((volatile unsigned long *)(0x4402E0A8))  =  0x63;          /* PIN as UART0_RX          */ 		
    *((volatile unsigned long *)(0x4000C024))  =  0x2B;          /* Set 115200               */ 		 
    *((volatile unsigned long *)(0x4000C028))  =  0x1A;          /* Set 115200               */ 		 		 
    *((volatile unsigned long *)(0x4000C02C))  =  0x00000060;    /* 8-bit ,1-stop ,no parity */ 		 		 
    *((volatile unsigned long *)(0x4000C030))  =  0x00000301;    /* Enable Uart              */
#endif
    
    return ;
}

void LOS_EvbUartWriteByte(char c)
{
#ifdef LAUNCHXL_CC3220SF
    while(*((volatile unsigned long *)(0x4000C018)) &0x20); /* Wait for empty TX buffer */
    *((volatile unsigned long *)(0x4000C000)) = c;          /*Send data                 */
#endif
    return ;
}

void LOS_EvbUartWriteStr(const char* str)
{
#ifdef LAUNCHXL_CC3220SF
    while (*str)
    {
        while(*((volatile unsigned long *)(0x4000C018)) &0x20); /* Wait for empty TX buffer */
        *((volatile unsigned long *)(0x4000C000)) = *str;       /*Send data                 */
        str++;
    }
#endif
    return ;
}


void LOS_EvbUartReadByte(char* c)
{
#ifdef LAUNCHXL_CC3220SF
    *c = *((volatile unsigned long *)(0x4000C000));  /* Get data */
#endif
    return ;
}

#ifdef LAUNCHXL_CC3220SF
static char _buffer[128];
#endif
void LOS_EvbUartPrintf(char* fmt, ...)
{
#ifdef LAUNCHXL_CC3220SF
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

int fputc(int ch, FILE *f) 
{ 
    LOS_EvbUartWriteByte(ch);
    return(ch);
}

/* End of file */

