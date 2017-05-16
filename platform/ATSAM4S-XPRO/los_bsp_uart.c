#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "los_bsp_uart.h"

#ifdef ATSAM4S_XPRO
#include "sam4s.h"
#endif

void LOS_EvbUartInit(void)
{
#ifdef ATSAM4S_XPRO
    uint32_t ul_sr;
    
    PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect */


    PMC->PMC_PCER0 = ((1UL << ID_PIOB  ) |  /* enable PIOB   clock */
                      (1UL << ID_UART1) ); /* enable USART1 clock  */  

    /* Configure UART1 Pins (PB3 = TX, PB2 = RX). */
    PIOB->PIO_IDR        =  (PIO_PB2A_URXD1 | PIO_PB3A_UTXD1);
    
    ul_sr = PIOB->PIO_ABCDSR[0];
    PIOB->PIO_ABCDSR[0] &= (~(PIO_PB2A_URXD1 | PIO_PB3A_UTXD1) & ul_sr);
    ul_sr = PIOB->PIO_ABCDSR[1];
    PIOB->PIO_ABCDSR[1] &= (~(PIO_PB2A_URXD1 | PIO_PB3A_UTXD1) & ul_sr);

    PIOB->PIO_PDR        =  (PIO_PB2A_URXD1 | PIO_PB3A_UTXD1);
    PIOB->PIO_PUDR       =  (PIO_PB2A_URXD1 | PIO_PB3A_UTXD1);


  /* Configure UART1 for 115200 baud. */
    UART1->UART_CR   = (UART_CR_RSTRX | UART_CR_RSTTX) |
                     (UART_CR_RXDIS | UART_CR_TXDIS);
    UART1->UART_BRGR = (SystemCoreClock / 115200) / 16;
    UART1->UART_MR   =  (0x4u <<  9);        /* (UART) No Parity */
    UART1->UART_CR   = UART_CR_RXEN | UART_CR_TXEN;
    
    //interrupt
    //UART1->UART_IER = UART_IER_RXRDY;
    //NVIC_EnableIRQ(UART1_IRQn);

    PMC->PMC_WPMR = 0x504D4301; /* Enable write protect */
#endif
    return;
}


void LOS_EvbUartWriteByte(char c)
{
#ifdef ATSAM4S_XPRO
    while (!(UART1->UART_SR & UART_SR_TXRDY));
    UART1->UART_THR = c;
#endif
}

void LOS_EvbUartReadByte(char* c)
{
#ifdef ATSAM4S_XPRO
    while((UART1->UART_SR & UART_SR_RXRDY) == 0);
    *c = UART1->UART_RHR;
#endif
}

static char _buffer[256];
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

void LOS_EvbUartWriteStr(const char* str)
{
#ifdef ATSAM4S_XPRO
    while (*str)
    {
        LOS_EvbUartWriteByte(* str++);   
    }
#endif
}

int fputc(int ch, FILE *f)
{
    LOS_EvbUartWriteByte(ch);
    return (ch);
}
