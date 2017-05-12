#include "los_bsp_led.h"

#ifdef EFM32PG1B200F256GM48
#include "bsp.h"
#endif

void LOS_EvbLedInit(void)
{

#ifdef EFM32PG1B200F256GM48
		/* Initialize LED driver */
	BSP_LedsInit();
	BSP_LedSet(0);
	BSP_LedClear(1);
#endif

	return ;
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
#ifdef EFM32PG1B200F256GM48
    switch (index)
    {
        case LOS_LED1:
        {
            if (cmd == LED_ON)
            {
                BSP_LedSet(0); /*led1 on */
            }
            else
            {
                BSP_LedClear(0); /*led1 off */
            }
            break;
        }
        case LOS_LED2:
        {
            if (cmd == LED_ON)
            {
                BSP_LedSet(1); /*led2 on */
            }
            else
            {
                BSP_LedClear(1); /*led2 off */
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


