#include "los_bsp_key.h"

#ifdef LOS_LPC824
#include "board.h"
#endif

/*****************************************************************************
 Function    : LOS_EvbKeyInit
 Description : Key init
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbKeyInit(void)
{
#ifdef LOS_LPC824   
    Chip_GPIO_Init(LPC_GPIO_PORT);
    Board_Key_Init();
#endif
    return;
}

/*****************************************************************************
 Function    : LOS_EvbGetKeyVal
 Description : Get key value
 Input       : KeyNum
 Output      : None
 Return      : None
 *****************************************************************************/
uint8_t LOS_EvbGetKeyVal(int KeyNum)
{
#ifdef LOS_LPC824
    return Board_Key_GetKeyDown(KeyNum);
#else
    return LOS_GPIO_ERR;
#endif
}

