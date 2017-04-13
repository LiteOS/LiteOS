#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "los_bsp_uart.h"


#ifdef GD32F190R8
#include "systick.h"
#include "gd32f1x0.h"
#include "gd32f1x0_eval.h"

static char _buffer[128];


/*!
    \brief      initialize the LEDs
    \param[in]  none
    \param[out] none
    \retval     none
*/
void led_init(void)
{
    //gd_eval_ledinit(LED1);
    //gd_eval_ledinit(LED2);
    gd_eval_ledinit(LED3);
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
        gd_eval_ledon(LED3);

        /* delay 400 ms */
        delay_1ms(400);
        /* turn off LEDs */
        //gd_eval_led_off(LED1);
        //gd_eval_led_off(LED2);
        gd_eval_ledoff(LED3);
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
#ifdef GD32F190R8   
    /* initialize the LEDs */
    led_init();
    
    /* configure systick */
    //systick_config();
    
    /* flash the LEDs for 1 time */
    led_flash(1);
    
    /* configure EVAL_COM2 */
    gd_eval_COMinit(EVAL_COM2);
    
    /* configure TAMPER key */
    gd_eval_keyinit(KEY_WAKEUP, KEY_MODE_GPIO);  
    
#endif
    return;
}

#ifdef GD32F190R8
/*****************************************************************************
 Function    : LOS_EvbUartInit
 Description : enable the device on the dev baord
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUartWriteByte(char c)
{
    usart_data_transmit(EVAL_COM2, c);
    while (RESET == usart_flag_get(EVAL_COM2, USART_STAT_TBE));
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
    while (RESET == usart_flag_get(EVAL_COM2,USART_STAT_RBNE));
    *c = (usart_data_receive(EVAL_COM2));
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
        
    while(RESET == usart_flag_get(EVAL_COM2, USART_STAT_TC)){
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
#ifdef GD32F190R8
    while (*str)
    {
        usart_data_transmit(EVAL_COM2, * str++);
        while (RESET == usart_flag_get(EVAL_COM2,USART_STAT_TBE));
    }
    
    while(RESET == usart_flag_get(EVAL_COM2, USART_STAT_TC)){
    }
#endif
    return;
}

