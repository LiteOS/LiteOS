#include "los_bsp_led.h"


/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
#ifdef LOS_STM32L053XX
#include "stm32l0xx_hal.h"
#endif

/*****************************************************************************
 Function    : LOS_EvbLedInit
 Description : Init LED device
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbLedInit(void)
{
#ifdef LOS_STM32L053XX
    //add you code here.
    GPIO_InitTypeDef GPIO_InitStruct;
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

    /*Configure GPIO pin : PA5 */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
#endif
    return;
}

/*****************************************************************************
 Function    £ºLOS_EvbLedControl
 Discription : Led control function
 Input       : (1) index Led's index
               (2) cmd   Led on or off
 Output      : None
 Return      : None
******************************************************************************/
void LOS_EvbLedControl(int index, int cmd)
{
#ifdef LOS_STM32L053XX
    switch (index)
    {
        case LOS_LED1:
        {
            if (cmd == LED_ON)
            {
                //add you code here.
                        /*led1 on */
            }
            else
            {
                //add you code here.
                        /*led1 off */
            }
            break;
        }
        case LOS_LED2:
        {
            if (cmd == LED_ON)
            {
                //add you code here.
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);        /*led2 on */
            }
            else
            {
                //add you code here.
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);      /*led2 off */
            }
            break;
        }
        case LOS_LED3:
        {
            if (cmd == LED_ON)
            {
                //add you code here.
                        /*led3 on */
            }
            else
            {
                //add you code here.
                        /*led3 off */
            }
            break;
        }
        default:
        {
            break;
        }
    }
#endif
    return;
}


