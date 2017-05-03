#include "los_bsp_key.h"

/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
#ifdef LOS_STM32F411RE
#include "stm32f4xx_nucleo.h"
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
    //add you code here.

#ifdef LOS_STM32F411RE
    BSP_PB_Init(BUTTON_USER,BUTTON_MODE_GPIO);
#endif
    return;
}

/*****************************************************************************
 Function    : LOS_EvbGetKeyVal
 Description : Get GIOP Key value
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
unsigned int LOS_EvbGetKeyVal(int KeyNum)
{
    unsigned int KeyVal = LOS_GPIO_ERR;
    
    //add you code here.
#ifdef LOS_STM32F411RE
    if(KeyNum > BUTTON_KEY)
    {
        return LOS_GPIO_ERR;
    }
    
    KeyVal = BSP_PB_GetState((Button_TypeDef)KeyNum);
#endif
    
    return KeyVal;
}

/*****************************************************************************
 Function    : EXTIxxx_IRQHandler
 Description : EXT xxx IRQ Handler detail,change the function name
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void EXTIxxx_IRQHandler(void)
{
    //add you code here.
    
    return;
}



