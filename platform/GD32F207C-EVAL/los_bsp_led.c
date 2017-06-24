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
    /* enable the led(PC0 PC2 PE0 PE1) clock */
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOE);

    /* configure led GPIO port */ 
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_2);
    gpio_init(GPIOE, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1);

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
    void (*gpioCtrl)(uint32_t , uint32_t );

    /* param check */
    if(index > LOS_LED4 | cmd != LED_ON && cmd != LED_OFF)
        return;
    
    if(cmd == LED_ON)
        gpioCtrl = gpio_bit_set;

    if(cmd == LED_OFF)
        gpioCtrl = gpio_bit_reset;

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
