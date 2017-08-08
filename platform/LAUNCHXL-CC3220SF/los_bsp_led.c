#include "los_bsp_led.h"

#ifdef LAUNCHXL_CC3220SF

#endif

void LOS_EvbLedInit(void)
{

#ifdef LAUNCHXL_CC3220SF
    *((volatile unsigned long *)(0x44025058))  = 0x01;       /* Enable clock  */
    *((volatile unsigned long *)(0x40005400)) |= 0x0E;       /* Set as output */

    *((volatile unsigned long *)(0x4402E0C4))  = 0x20;       /* Set output current as 2mA */
    *((volatile unsigned long *)(0x4402E0C8))  = 0x20;       /* Set output current as 2mA */
    *((volatile unsigned long *)(0x4402E0CC))  = 0x20;       /* Set output current as 2mA */
  
    *((volatile unsigned long *)(0x40005038))  = 0x00;       /* Turn OFF all LEDs */
#endif

    return ;
}

void LOS_EvbLedControl(int index, int cmd)
{
#ifdef LAUNCHXL_CC3220SF
    switch (index)
    {
        case LOS_LED1:
        {
            if (cmd == LED_ON)
            {
                *((volatile unsigned long *)(0x40005038)) = 0x0E;    /* Led ON  */
            }
            else
            {
                *((volatile unsigned long *)(0x40005038)) = 0x00;    /* LED OFF */
            }
            break;
        }
        case LOS_LED2:
        default:
        {
            break;
        }
    }
#endif
    return ;
}

/* End of file */

