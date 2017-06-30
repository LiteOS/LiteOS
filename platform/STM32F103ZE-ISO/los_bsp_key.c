#include "los_bsp_key.h"
#include "los_typedef.h"

/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
#ifdef LOS_STM32F103ZE
#include "stm32f10x.h"
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
#ifdef LOS_STM32F103ZE
    //add you code here.
    //BSP_PB_Init(BUTTON_USER,BUTTON_MODE_GPIO);
    GPIO_InitTypeDef GPIO_InitStructure;
 
		RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK,ENABLE);  //使能PORTA时钟

		GPIO_InitStructure.GPIO_Pin  = KEY1_GPIO_PIN;  //KEY1
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //设置成上拉输入
		GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);//初始化GPIO
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
    UINT32 KeyVal = 0xFFFFFFFF;
#ifdef LOS_STM32F103ZE 
    //add you code here.
    if(KeyNum > 1)
    {
        return 0xFF;
    }
    KeyVal = (UINT32)GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY1_GPIO_PIN);
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



