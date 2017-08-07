#include "los_bsp_key.h"
#include "Board_Buttons.h"              // ::Board Support:Buttons

void LOS_EvbKeyInit(void)
{
	Buttons_Initialize();
	return ;
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
	KeyVal = Buttons_GetState();
	
	return KeyVal;
}

