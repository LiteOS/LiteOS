#include "los_bsp_led.h"

/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
#ifdef LOS_NUC472H
#include "NUC472_442.h"
#include "gpio.h"
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
#ifdef LOS_NUC472H
    GPIO_SetMode(PIN_LED1, BIT_LED1, GPIO_MODE_OUTPUT);
#endif
    
    return;
}

/*************************************************************************************************
 *  Function    £ºLOS_EvbLedControl
 *  Discription : Led control function
 *  Input       : (1) index Led's index
 *                (2) cmd   Led on or off
 *  Output      : None
 *  Return      : None
**************************************************************************************************/
void LOS_EvbLedControl(int index, int cmd)
{
#ifdef LOS_NUC472H
    switch (index)
    {
        case LOS_LED1:
        {
            if (cmd == LED_ON)
            {
                //add you code here.
                LED1_CONTROL = LED_ON; /* LED on */
            }
            else
            {
                //add you code here.
                LED1_CONTROL = LED_OFF; /* LED off */
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


