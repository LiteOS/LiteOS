#include "los_bsp_key.h"
#include "los_bsp_led.h"
#include "gd32f20x_gpio.h"
#include "gd32f20x_exti.h"

/*****************************************************************************
 Function    : LOS_EvbKeyInit
 Description : key init
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbKeyInit(void)
{
    /* configure User key */
    GPIO_InitPara GPIO_InitStructure;
    EXTI_InitPara EXTI_InitStructure;
    NVIC_InitPara NVIC_InitStructure;
    
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOB ,ENABLE);

    /* Configure PB14 pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    /* Connect EXTI Line14 to PC14 pin */
    GPIO_EXTILineConfig(GPIO_PORT_SOURCE_GPIOB, GPIO_PINSOURCE14);

    /* Configure EXTI Line14 */
    EXTI_InitStructure.EXTI_LINE = EXTI_LINE14;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LINEEnable = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    /* Configure EXTI Line15-10 priority */
    NVIC_InitStructure.NVIC_IRQ = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQPreemptPriority = 0;
    NVIC_InitStructure.NVIC_IRQSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQEnable = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    return;
}

/*****************************************************************************
 Function    : LosAdapIrqDisable
 Description : external interrupt disable
               this function is called by LOS_HwiDelete(), so use bsp func
               to inplemente it
               Note : if don't use LOS_HwiDelete(), leave it empty
 Input       : irqnum: external interrupt number
 Output      : None
 Return      : None
 *****************************************************************************/
void EXTI15_10_IRQHandler(void)
{
    if (EXTI_GetIntBitState(EXTI_LINE14) != RESET)
    {
        EXTI_ClearIntBitState(EXTI_LINE14);

        //LOS_EvbLedControl(LOS_LED2, LED_ON);
    }
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

    if(GPIO_ReadInputBit(GPIOB, GPIO_PIN_14) == Bit_SET)
        KeyVal = LOS_KEY_PRESS;

    return KeyVal;
}
