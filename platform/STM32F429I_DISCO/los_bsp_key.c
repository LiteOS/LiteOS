#include "los_bsp_key.h"

#ifdef LOS_STM32F429ZI
#include "stm32f4xx_hal.h"
#include "stm32f429i_discovery.h"
#endif

void LOS_EvbKeyInit(void)
{

#ifdef LOS_STM32F429ZI
    GPIO_InitTypeDef   GPIO_InitStructure;

    BSP_LED_Init(LED3);

    /* Enable GPIOA clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* Configure PA0 pin as input floating */
    GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Pin = GPIO_PIN_0;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Enable and set EXTI Line0 Interrupt to the lowest priority */
    HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
#endif

    return;
}

#ifdef LOS_STM32F429ZI
/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == KEY_BUTTON_PIN)
    {
        BSP_LED_Toggle(LED3);
    }
}
#endif

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

    return KeyVal;
}

