#include "los_bsp_led.h"

#ifdef LOS_ATMSAMG55xx
#include "samg55.h"                     // Device header
#include "Board_LED.h"                  // ::Board Support:LED
#endif

/*****************************************************************************
 Function    : LOS_EvbLedInit
 Description : Init LED
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbLedInit(void)
{
#ifdef LOS_ATMSAMG55xx
    LED_Initialize();
#endif
    return;
}

/*****************************************************************************
 Function    : LOS_EvbLedControl
 Description : control led on or off
 Input       : index Led's index
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbLedControl(int index, int cmd)
{
#ifdef LOS_ATMSAMG55xx
    switch(cmd)
    {
        case LED_ON:
            LED_On(0);
            break;
        case LED_OFF:
            LED_Off(0);
            break;
        default:
            LED_Off(0);
            break;
    }
#endif
    return;
}


