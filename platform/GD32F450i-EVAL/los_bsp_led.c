#include "los_bsp_led.h"

#ifdef GD32F4XX
//#include "gd32f4xx.h"
#include "gd32f450i_eval.h"
#endif

void LOS_EvbLedInit(void)
{
#ifdef GD32F4XX
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED2);
    gd_eval_led_init(LED3);
#endif

    return;
}

/*************************************************************************************************
 *  function£ºcontrol led on off                                                                 *
 *  param (1) index Led's index                                                                  *
 *        (2) cmd   Led on or off                                                                *
 *  return : None                                                                                *
 *  discription:                                                                                 *
**************************************************************************************************/
void LOS_EvbLedControl(int index, int cmd)
{
#ifdef GD32F4XX
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
        case LOS_LED3:
        {
            if (cmd == LED_ON)
            {
                gd_eval_led_on(LED3); /*led3 on */
            }
            else
            {
                gd_eval_led_off(LED3); /*led3 off */
            }
            break;
        }
        default:
        {
            break;
        }
    }
#endif
    return ;
}


