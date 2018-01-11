#include "los_bsp_led.h"
#ifdef NUTINY_M2351
#include "M2351.h"
#endif
/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
extern int32_t Secure_func(void);


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
#ifdef NUTINY_M2351
	//PC->MODE = (GPIO_MODE_OUTPUT << 14 * 2);
#endif
    return ;
}

/*************************************************************************************************
 *  Function    £ºLOS_EvbLedControl
 *  Discription : Led control function
 *  Input       : (1) index Led's index                                                                  *
 *                (2) cmd   Led on or off  
 *  Output      : None
 *  Return      : None                                                                                *
 *                                                                                 *
**************************************************************************************************/
void LOS_EvbLedControl(int index, int cmd)
{
    switch (index)
    {
        case LOS_LED1:
        {
            if (cmd == LED_ON)
            {
                //add you code here.
                        /*led1 on */
            }
            else
            {
                //add you code here.
                        /*led1 off */
            }
            break;
        }
        case LOS_LED2:
        {
            if (cmd == LED_ON)
            {
                //add you code here.
                        /*led2 on */
            }
            else
            {
                //add you code here.
                        /*led2 off */
            }
            break;
        }
        case LOS_LED3:
        {
            if (cmd == LED_ON)
            {
                //add you code here.
                        /*led3 on */
            }
            else
            {
                //add you code here.
                        /*led3 off */
            }
            break;
        }
        default:
        {
            break;
        }
    }
    #ifdef NUTINY_M2351
    //PC14 ^= 1;
    Secure_func();
    #endif
    return;
}


