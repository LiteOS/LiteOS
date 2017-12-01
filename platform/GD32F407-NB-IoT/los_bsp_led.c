#include "los_bsp_led.h"

/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
#ifdef GD32F407
#include "gd32f4xx.h"
#include "gd32f407nb-iot_eval.h"
#endif


/*****************************************************************************
 Function    : LOS_EvbLedInit
 Description : Init LED device
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbLedInit(void)
{
    //add you code here.
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED2);
    return ;
}

/*************************************************************************************************
 Function    £ºLOS_EvbLedControl
 Discription : Led control function
 Input       : (1) index Led's index
               (2) cmd   Led on or off
 Output      : None
 Return      : None
**************************************************************************************************/
void LOS_EvbLedControl(int index, int cmd)
{
    switch (index)
    {
        case LOS_LED1:
        {
            if (cmd == LED_ON)
            {
                gd_eval_led_on(LED1); /*led1 on */
            }
            else
            {
                gd_eval_led_off(LED1); /*led1 off */
            }
            break;
        }
        case LOS_LED2:
        {
            if (cmd == LED_ON)
            {
                gd_eval_led_on(LED2); /*led2 on */
            }
            else
            {
                gd_eval_led_off(LED2); /*led2 off */
            }
            break;
        }
        default:
        {
            break;
        }
    }
    return;
}


