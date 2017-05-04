#include "los_bsp_led.h"


#ifdef LOS_STM32F746ZG

#include "stm32f7xx_hal.h"
#include "stm32f7xx_nucleo_144.h"

#endif

void LOS_EvbLedInit(void)
{

#ifdef LOS_STM32F746ZG
    BSP_LED_Init(LED1);
    BSP_LED_Init(LED2);
    BSP_LED_Init(LED3);
#endif

    return ;
}

/*************************************************************************************************
 *  function：control led on off                                                                 *
 *  param (1) index Led's index                                                                  *
 *        (2) cmd   Led on or off                                                                *
 *  return : None                                                                                *
 *  discription:                                                                                 *
**************************************************************************************************/
void LOS_EvbLedControl(int index, int cmd)
{
#ifdef LOS_STM32F746ZG
    switch (index)
    {
        case LOS_LED1:
        {
            if (cmd == LED_ON)
            {
                BSP_LED_On(LED1); /*led1 on */
            }
            else
            {
                BSP_LED_Off(LED1); /*led1 off */
            }
            break;
        }
        case LOS_LED2:
        {
            if (cmd == LED_ON)
            {
                BSP_LED_On(LED2); /*led2 on */
            }
            else
            {
                BSP_LED_Off(LED2); /*led2 off */
            }
            break;
        }
        case LOS_LED3:
        {
            if (cmd == LED_ON)
            {
                BSP_LED_On(LED3); /*led2 on */
            }
            else
            {
                BSP_LED_Off(LED3); /*led2 off */
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

