#include <stdio.h>
#include <samd21.h>
#include "los_bsp_led.h"

//PB30
void LOS_EvbLedInit(void)
{
    PORT->Group[1].DIRSET.reg |= PORT_PB30;
    PORT->Group[1].OUTSET.reg |= PORT_PB30;
    
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
        PORT->Group[1].OUTCLR.reg |= PORT_PB30;
    } 
    else {
        PORT->Group[1].OUTSET.reg |= PORT_PB30;
    }
    
    return;
}


