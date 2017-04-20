#include "los_bsp_led.h"

#ifdef LOS_LPC54114
    #include "app_led.h"
#endif

/*****************************************************************************
 Function    : LOS_EvbLedInit
 Description : Led init
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbLedInit(void)
{
#ifdef LOS_LPC54114
    led_init();
#endif

    return;
}

/******************************************************************************
 Function   £ºLOS_EvbLedControl
 Discription: Control led on or off
 Input      : index Led's index
              cmd   Led on or off
 Return     : None
*******************************************************************************/
void LOS_EvbLedControl(int index, int cmd)
{
#ifdef LOS_LPC54114
    
    if(index >= LED_NUM)
    {
        return;
    }
    switch (cmd)
    {
        case LED_ON:
        {
            led_on(index);
            break;
        }
        case LED_OFF:
        {
            led_off(index);
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


