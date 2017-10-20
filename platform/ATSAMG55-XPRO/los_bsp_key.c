#include "los_bsp_key.h"

#ifdef LOS_ATMSAMG55xx
#include "samg55.h"                     // Device header
#include "Board_Buttons.h"              // ::Board Support:Buttons
#endif

/*****************************************************************************
 Function    : LOS_EvbKeyInit
 Description : Init GIOP Key 
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbKeyInit(void)
{
#ifdef LOS_ATMSAMG55xx
    Buttons_Initialize();
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
    
    //add you code here.
#ifdef LOS_ATMSAMG55xx
    KeyVal = Buttons_GetState();
#endif
    return KeyVal;
}

