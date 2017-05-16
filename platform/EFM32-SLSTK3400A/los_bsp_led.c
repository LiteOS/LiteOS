#include "los_bsp_led.h"

#ifdef EFM32HG322F64
#include "bsp.h"
#endif

void LOS_EvbLedInit(void)
{

#ifdef EFM32HG322F64
    /* Initialize LED driver */
    BSP_LedsInit();
    BSP_LedClear(1);
#endif

    return ;
}

void LOS_EvbLedControl(int index, int cmd)
{
#ifdef EFM32HG322F64
    switch (index)
    {
        case LOS_LED1:
        {
            if (cmd == LED_ON)
            {
                BSP_LedSet(0);   /* led1 on */
            }
            else
            {
                BSP_LedClear(0); /* led1 off */
            }
            break;
        }
        case LOS_LED2:
        {
            if (cmd == LED_ON)
            {
                BSP_LedSet(1);   /* led2 on */
            }
            else
            {
                BSP_LedClear(1); /* led2 off */
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

/* End of file */

