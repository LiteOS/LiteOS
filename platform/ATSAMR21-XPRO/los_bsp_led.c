#include <stdio.h>
#include "los_bsp_led.h"

#ifdef LOS_ATSAMR21_XPRO
#include <samr21.h>
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
#ifdef LOS_ATSAMR21_XPRO
    PORT->Group[0].DIRSET.reg |= PORT_PA19;
    PORT->Group[0].OUTSET.reg |= PORT_PA19;
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
#ifdef LOS_ATSAMR21_XPRO
    if (cmd == LED_ON) {
        PORT->Group[0].DIRSET.reg |= PORT_PA19;
        PORT->Group[0].OUTCLR.reg |= PORT_PA19;
    } 
    else {
        PORT->Group[0].DIRSET.reg |= PORT_PA19;
        PORT->Group[0].OUTSET.reg |= PORT_PA19;
    }
#endif
    return;
}


