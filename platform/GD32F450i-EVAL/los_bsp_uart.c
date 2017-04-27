#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "los_bsp_uart.h"


#ifdef GD32F4XX
#include "systick.h"
#include "gd32f4xx.h"
#include "gd32f450i_eval.h"

static char _buffer[128];


/*!
    \brief      initialize the LEDs
    \param[in]  none
    \param[out] none
    \retval     none
*/
void led_init(void)
{
    //gd_eval_led_init(LED1);
    //gd_eval_led_init(LED2);
    gd_eval_led_init(LED3);
    return;
}

/*!
    \brief      flash the LEDs for test
    \param[in]  times: times to flash the LEDs
    \param[out] none
    \retval     none
*/
void led_flash(int times)
{
    int i;
    for(i=0; i<times; i++){
        /* delay 400 ms */
        delay_1ms(400);

        /* turn on LEDs */
        //gd_eval_led_on(LED1);
        //gd_eval_led_on(LED2);
        gd_eval_led_on(LED3);

        /* delay 400 ms */
        delay_1ms(400);
        /* turn off LEDs */
        //gd_eval_led_off(LED1);
        //gd_eval_led_off(LED2);
        gd_eval_led_off(LED3);
    }
    return;
}
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
#ifdef GD32F4XX 
    /* initialize the LEDs */
    led_init();
    
    /* configure systick */
    //systick_config();
    
    /* flash the LEDs for 1 time */
    led_flash(1);
    
    /* configure EVAL_COM1 */
    gd_eval_com_init(EVAL_COM1);
    
    /* configure TAMPER key */
    gd_eval_key_init(KEY_USER, KEY_MODE_GPIO);  
    
#endif
    return;
}

#ifdef GD32F4XX
/*****************************************************************************
 Function    : LOS_EvbUartInit
 Description : enable the device on the dev baord
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUartWriteByte(char c)
{
    usart_data_transmit(EVAL_COM1, c);
    while (RESET == usart_flag_get(EVAL_COM1, USART_FLAG_TBE));
    return;
}

/*****************************************************************************
 Function    : Los_Uart1ReadByte
 Description : enable the device on the dev baord
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUartReadByte(char* c)
{
    while (RESET == usart_flag_get(EVAL_COM1,USART_FLAG_RBNE));
    *c = (usart_data_receive(EVAL_COM1));
    return;
}

/*****************************************************************************
 Function    : LosUart1Printf
 Description : enable the device on the dev baord
 Input       : None
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
        
    while(RESET == usart_flag_get(EVAL_COM1, USART_FLAG_TC)){
    }
    return;
}
#endif

/*****************************************************************************
 Function    : LOS_EvbUartInit
 Description : enable the device on the dev baord
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUartWriteStr(const char* str)
{
#ifdef GD32F4XX
    while (*str)
    {
        usart_data_transmit(EVAL_COM1, * str++);
        while (RESET == usart_flag_get(EVAL_COM1,USART_FLAG_TBE));
    }
        
    while(RESET == usart_flag_get(EVAL_COM1, USART_FLAG_TC)){
    }
#endif
    return;
}

