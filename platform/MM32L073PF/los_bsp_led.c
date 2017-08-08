#include "los_bsp_led.h"

#ifdef LOS_MM32L073PF
#include "led.h"
#endif

/******************************************************************************
    here include some special hearder file you need
******************************************************************************/



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
#ifdef LOS_MM32L073PF
    LED_Init();
#endif
    return;
}

/******************************************************************************
 Function   ¡êoLOS_EvbLedControl
 Discription: Control led on or off
 Input      : index Led's index
              cmd   Led on or off
 Return     : None
*******************************************************************************/
void LOS_EvbLedControl(unsigned int index, unsigned int cmd)
{
    switch (index)
    {
        case LOS_LED1:
        {
            //add you code here.
            #ifdef LOS_MM32L073PF
			if(cmd == LED_ON) {
				LED1_ON();
			} else {
				LED1_OFF();
			}
            #endif
            break;
        }
        case LOS_LED2:
        {
            //add you code here.
            #ifdef LOS_MM32L073PF
			if(cmd == LED_ON) {
				LED2_ON();
			} else {
				LED2_OFF();
			}
            #endif
            break;
        }
        case LOS_LED3:
        {
            //add you code here.
            #ifdef LOS_MM32L073PF
			if(cmd == LED_ON) {
				LED3_ON();
			} else {
				LED3_OFF();
			}
            #endif
            break;
        }
        case LOS_LED4:
        {
            //add you code here.
            #ifdef LOS_MM32L073PF
			if(cmd == LED_ON) {
				LED4_ON();
			} else {
				LED4_OFF();
			}
            #endif
            break;
        }
        default:
        {
            break;
        }
    }
    return;
}


