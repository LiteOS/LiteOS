#include <stdio.h>
#include <samr21.h>
#include "los_bsp_key.h"

/*****************************************************************************
 Function    : LOS_EvbKeyInit
 Description : Init user key
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbKeyInit(void)
{
    PORT->Group[0].DIRCLR.reg |= PORT_PA28;
    PORT->Group[0].WRCONFIG.reg |= 0xD0071000;
    PORT->Group[0].OUTSET.reg |= PORT_PA28;
    
    return;
}

/*****************************************************************************
 Function    : LOS_EvbGetKeyVal
 Description : Get GIOP Key value
 Input       : int KeyNum
 Output      : None
 Return      : KeyVal
 *****************************************************************************/
unsigned int LOS_EvbGetKeyVal(int KeyNum)
{
    uint32_t reg;
    unsigned int KeyVal = LOS_GPIO_ERR; 
    
    //add you code here.
    reg = PORT->Group[0].IN.reg;
    
    KeyVal = (reg & PORT_PA28) >> 28;
    
    PORT->Group[0].OUTSET.reg |= PORT_PA28;
    
    return KeyVal;
}

