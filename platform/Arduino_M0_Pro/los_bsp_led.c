#include "los_bsp_led.h"
#include "samd21.h"
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
    PORT->Group[0].DIRSET.reg=0x00020000;   // Arduino M0 Pro的L灯对应的是PA[17]
    return;
}

/*************************************************************************************************
 *  Function    ：LOS_EvbLedControl
 *  Discription : Led control function
 *  Input       : (1) index Led's index
 *                (2) cmd   Led on or off
 *  Output      : None
 *  Return      : None
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
                PORT->Group[0].OUTSET.reg=0x00020000; /*led1 on */
            }
            else
            {
                //add you code here.
                PORT->Group[0].OUTCLR.reg=0x00020000; /*led1 off */
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
    return;
}


