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
    /* enable the key(PB14) clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    /* configure led GPIO port */ 
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_14);

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

    if(gpio_input_bit_get(GPIOB, GPIO_PIN_14) == SET)
        KeyVal = LOS_KEY_PRESS;

    return KeyVal;
}
