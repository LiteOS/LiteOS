#include <stdio.h>
#include <stdint.h>
#include "los_bsp_key.h"

#ifdef LOS_ATSAMR21_XPRO
#include <samr21.h>
#endif

/*****************************************************************************
 Function    : LOS_EvbKeyInit
 Description : Init user key
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbKeyInit(void)
{
#ifdef LOS_ATSAMR21_XPRO
    PORT->Group[0].DIRCLR.reg |= PORT_PA28;
    PORT->Group[0].WRCONFIG.reg |= 0xD0071000;
    PORT->Group[0].OUTSET.reg |= PORT_PA28;
#endif
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
    unsigned int KeyVal = LOS_GPIO_ERR;
#ifdef LOS_ATSAMR21_XPRO  
    //add you code here.
	uint32_t reg;
    reg = PORT->Group[0].IN.reg;
    
    KeyVal = (reg & PORT_PA28) >> 28;
    
    PORT->Group[0].OUTSET.reg |= PORT_PA28;
#endif
    return KeyVal;
}

