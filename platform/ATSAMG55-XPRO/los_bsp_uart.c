#include "los_bsp_uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifdef LOS_ATMSAMG55xx
#include "samg55j19.h" 
#include "samg55.h"    // Device header
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
#ifdef LOS_ATMSAMG55xx
    uint32_t ul_sr;
    
    PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect */


    PMC->PMC_PCER0 = ((1UL << ID_PIOA  ) |  /* enable PIOA   clock */
                      (1UL << ID_FLEXCOM0) ); /* enable USART0 clock  */

    /* Configure UART0 Pins (PA10    = TX, PA09 = RX). */
    PIOA->PIO_IDR        =  (PIO_PA9A_RXD0 | PIO_PA10A_TXD0);
    
    ul_sr = PIOA->PIO_ABCDSR[0];
    PIOA->PIO_ABCDSR[0] &= (~(PIO_PA9A_RXD0 | PIO_PA10A_TXD0) & ul_sr);
    ul_sr = PIOA->PIO_ABCDSR[1];
    PIOA->PIO_ABCDSR[1] &= (~(PIO_PA9A_RXD0 | PIO_PA10A_TXD0) & ul_sr);

    PIOA->PIO_PDR       =  (PIO_PA9A_RXD0 | PIO_PA10A_TXD0);
    //PIOA->PIO_PUDR      =  (PIO_PA9A_RXD0 | PIO_PA10A_TXD0);

  /* Configure UART1 for 115200 baud. */
    USART0->US_CR   = (US_CR_RSTRX | US_CR_RSTTX) |
                     (US_CR_RXDIS | US_CR_TXDIS);
    USART0->US_BRGR = (SystemCoreClock / 115200) / 16;
    USART0->US_MR   =  (0x4u <<  9);        /* (UART) No Parity */
    USART0->US_MR   |=  (0x3u <<  6);        /* (UART) 8 bit */
    USART0->US_CR   = US_CR_RXEN | US_CR_TXEN;
    
    //interrupt
    //UART1->UART_IER = UART_IER_RXRDY;
    //NVIC_EnableIRQ(UART1_IRQn);

    PMC->PMC_WPMR = 0x504D4301; /* Enable write protect */
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
#ifdef LOS_ATMSAMG55xx
    while (!(USART0->US_CSR & US_CSR_TXRDY));
    USART0->US_THR = c;
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
#ifdef LOS_ATMSAMG55xx
    while(*str != '\0')
    {
        while (!(USART0->US_CSR & US_CSR_TXRDY));
        USART0->US_THR = *str;
        str++;
    }
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
#ifdef LOS_ATMSAMG55xx
    while((USART0->US_CSR & US_CSR_RXRDY) == 0);
    *c = USART0->US_RHR;
#endif
    return;
}

static char _buffer[256];
/*****************************************************************************
 Function    : LosUartPrintf
 Description : Los printf function
 Input       : char* fmt, ...
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUart1Printf(char* fmt, ...)
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
