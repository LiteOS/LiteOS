#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "los_bsp_uart.h"
#include "gd32f20x_gpio.h"

int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, ch);
    while (usart_flag_get(USART0, USART_FLAG_TC) == RESET);
    return ch;
}

/*****************************************************************************
 Function    : LOS_EvbUartInit
 Description : usart init
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUartInit(void)
{
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);

    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);

    /* connect port to USARTx_Tx */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);

    /* connect port to USARTx_Rx */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    /* USART configure */
    usart_deinit(USART0);
    usart_baudrate_set(USART0, 115200U);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);
}

/*****************************************************************************
 Function    : LOS_EvbUartWriteStr
 Description : send str to usart
 Input       : str      --- string need to send
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUartWriteStr(const char* str)
{
    while (*str)
    {
        usart_data_transmit(USART0, *str);
        while (usart_flag_get(USART0, USART_FLAG_TC) == RESET);
        
        str++;
    }
    
    return;
}
