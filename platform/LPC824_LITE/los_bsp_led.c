#include "los_bsp_led.h"

#ifdef LOS_LPC824
#include "board.h"
#endif

/*****************************************************************************
 Function    : LOS_EvbLedInit
 Description : Led init
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbLedInit(void)
{
#ifdef LOS_LPC824
    Chip_GPIO_Init(LPC_GPIO_PORT);
    Board_LED_Init();
#endif
    return;
}

/******************************************************************************
 Function   : LOS_EvbLedControl
 Discription: Control led on or off
 Input      : index Led's index
              cmd   Led on or off
 Return     : None
*******************************************************************************/
void LOS_EvbLedControl(int index, int cmd)
{
#ifdef LOS_LPC824
    Board_LED_Set(index,cmd);
#endif
    return;
}




