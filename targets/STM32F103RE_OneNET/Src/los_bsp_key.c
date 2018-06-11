#include "stdint.h"
#include "los_bsp_key.h"

/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
#ifdef LOS_STM32F103ZE
#include "stm32f10x.h"   //OneNET
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
#ifdef LOS_STM32F103ZE
    //add you code here.
    //BSP_PB_Init(BUTTON_USER,BUTTON_MODE_GPIO);
    GPIO_InitTypeDef GPIO_InitStructure;
 
		RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK,ENABLE); 

		GPIO_InitStructure.GPIO_Pin  = KEY1_GPIO_PIN;  //KEY1
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
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
    unsigned int KeyVal = 0xFFFFFFFF;
    
    //add you code here.
#ifdef LOS_STM32F103ZE
    switch (KeyNum)
    {
        case USER_KEY:
            KeyVal = (unsigned int)GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY1_GPIO_PIN);
            break;
        default:
            break;
    }
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
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
#ifdef LOS_STM32F103ZE
    //if (GPIO_Pin == GPIO_PIN_0)   //OneNET
    {
        //HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);  //onenet led3  //OneNET
    }
#endif
}

