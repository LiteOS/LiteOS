#include "stdint.h"
#include "los_bsp_key.h"

/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
#ifdef LOS_STM32L053XX
#include "stm32l0xx_hal.h"
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
#ifdef LOS_STM32L053XX
    GPIO_InitTypeDef GPIO_InitStruct;

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /*Configure GPIO pin : PC13 */
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    
    GPIO_InitTypeDef   GPIO_InitStructure;
    
    /* Enable GPIOC clock */
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /* Configure PC.13 pin as input floating */
    GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Pin = GPIO_PIN_13;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Enable and set EXTI line 15_10 Interrupt to the lowest priority */
    HAL_NVIC_SetPriority(EXTI4_15_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
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
#ifdef LOS_STM32L053XX
    switch (KeyNum)
    {
        case USER_KEY:
            KeyVal = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
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
#ifdef LOS_STM32L053XX
    if (GPIO_Pin == GPIO_PIN_13)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    }
#endif
}

