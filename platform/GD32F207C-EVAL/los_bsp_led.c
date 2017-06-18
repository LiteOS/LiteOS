#include "los_bsp_led.h"
#include "gd32f20x_gpio.h"

/*****************************************************************************
 Function    : LOS_EvbLedInit
 Description : init led gpio
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbLedInit(void)
{
    GPIO_InitPara GPIO_InitStructure;
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOC,ENABLE);
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOE,ENABLE);

    /* Configure PC0 PC2 pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_0 | GPIO_PIN_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOC,&GPIO_InitStructure);

    /* Configure PE0 PE1 pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_Init(GPIOE,&GPIO_InitStructure);

    return ;
}

/*****************************************************************************
 Function    : LOS_EvbLedControl
 Description : control leds on/off
 Input       : index        --- which led to control
             : cmd          --- LED_ON/LED_OFF
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbLedControl(int index, int cmd)
{
    void (*gpioCtrl)(GPIO_TypeDef *, uint16_t );

    /* param check */
    if(index > LOS_LED4 | cmd != LED_ON && cmd != LED_OFF)
        return;
    
    if(cmd == LED_ON)
        gpioCtrl = GPIO_SetBits;

    if(cmd == LED_OFF)
        gpioCtrl = GPIO_ResetBits;

    switch (index)
    {
        case LOS_LED1:
            gpioCtrl(GPIOC, GPIO_PIN_0);
            break;
        case LOS_LED2:
            gpioCtrl(GPIOC, GPIO_PIN_2);
            break;
        case LOS_LED3:
            gpioCtrl(GPIOE, GPIO_PIN_0);
            break;
        case LOS_LED4:
            gpioCtrl(GPIOE, GPIO_PIN_1);
            break;
        default:
            break;
    }

    return ;
}