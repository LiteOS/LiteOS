#include "los_bsp_led.h"

#ifdef LOS_STM32F429ZI

#include "stm32f4xx_hal.h"
#include "stm32f429i_discovery.h"

#endif

void LOS_EvbLedInit(void)
{

#ifdef LOS_STM32F429ZI
    BSP_LED_Init(LED3);
    BSP_LED_Init(LED4);
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
#ifdef LOS_STM32F429ZI
    switch (index)
    {
        case LOS_LED1:
        {
            if (cmd == LED_ON)
            {
                BSP_LED_On(LED3); /*led1 on */
            }
            else
            {
                BSP_LED_Off(LED3); /*led1 off */
            }
            break;
        }
        case LOS_LED2:
        {
            if (cmd == LED_ON)
            {
                BSP_LED_On(LED4); /*led2 on */
            }
            else
            {
                BSP_LED_Off(LED4); /*led2 off */
            }
            break;
        }
        default:
        {
            break;
        }
    }
#endif
    return;
}


