#include <stdio.h>
#include <samd21.h>
#include "los_bsp_key.h"

//PA15
void LOS_EvbKeyInit(void)
{
    PORT->Group[0].DIRCLR.reg = PORT_PA15;
    PORT->Group[0].WRCONFIG.reg = 0x50068000;
    PORT->Group[0].OUTSET.reg = PORT_PA15;
    
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
    //printf("reg:%x\n", reg);
    KeyVal = (reg & PORT_PA15) >> 15;
    
    return KeyVal;
}

