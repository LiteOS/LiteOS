#include "los_bsp_led.h"

/******************************************************************************
	here include some special hearder file you need
******************************************************************************/
#include "board/board.h"
/*****************************************************************************
 Function    : LOS_EvbLedInit
 Description : Init LED device
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbLedInit(void)
{
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
    if (cmd == LED_ON) {
       led_write((1 << index), (1 << index));
    } else {
        led_write(0, (1 << index)); 
    }
}


