#include <stdio.h>
#include <samr21.h>
#include "los_bsp_led.h"

/*****************************************************************************
 Function    : LOS_EvbLedInit
 Description : Init LED
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbLedInit(void)
{
    PORT->Group[0].DIRSET.reg |= PORT_PA19;
    PORT->Group[0].OUTSET.reg |= PORT_PA19;
    
    return;
}

/*************************************************************************************************
 *  function£ºcontrol led on off                                                                 *
 *  param (1) index Led's index                                                                  *
 *        (2) cmd   Led on or off                                                                *
 *  return : None                                                                                *
 *  discription:                                                                                 *
**************************************************************************************************/
void LOS_EvbLedControl(int index, int cmd)
{
    if (cmd == LED_ON) {
        PORT->Group[0].DIRSET.reg |= PORT_PA19;
        PORT->Group[0].OUTCLR.reg |= PORT_PA19;
    } 
    else {
        PORT->Group[0].DIRSET.reg |= PORT_PA19;
        PORT->Group[0].OUTSET.reg |= PORT_PA19;
    }
    
    return;
}


