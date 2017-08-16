#include <stdio.h>
#include "los_bsp_led.h"
#include "los_demo_debug.h"
/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
#include "adi_gpio.h"


/*****************************************************************************
 Function    : LOS_EvbLedInit
 Description : Init LED device
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbLedInit(void)
{
#ifdef LOS_ADuCM450LF
    /* set GPIO output LED 3, 4 and 5 */
    if(ADI_GPIO_SUCCESS != adi_gpio_OutputEnable(ADI_GPIO_PORT0, ADI_GPIO_PIN_13, true))
    {
        dprintf("adi_gpio_SetDirection failed\n");
    }
    if(ADI_GPIO_SUCCESS != adi_gpio_OutputEnable(ADI_GPIO_PORT1, (ADI_GPIO_PIN_12 | ADI_GPIO_PIN_13), true))
    {
        dprintf("adi_gpio_SetDirection failed\n");
    }

    /* set GPIO output LED 3, 4 and 5 off*/
    if(ADI_GPIO_SUCCESS != adi_gpio_SetHigh(ADI_GPIO_PORT0, ADI_GPIO_PIN_13))/*led3 off */
    {
        dprintf("adi_gpio_SetLevel failed\n");
    }
    if(ADI_GPIO_SUCCESS != adi_gpio_SetHigh(ADI_GPIO_PORT1, (ADI_GPIO_PIN_12 | ADI_GPIO_PIN_13)))/*led3 off */
    {
        dprintf("adi_gpio_SetLevel failed\n");
    }
 #endif
    
    return;
}
/*****************************************************************************
 Function    : LOS_EvbLedControl
 Description : Led control function
 Input       : (1) index Led's index
               (2) cmd   Led on or off
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbLedControl(int index, int cmd)
{
#ifdef LOS_ADuCM450LF
    switch (index)
    {
        case LOS_LED1:
        {
            if (cmd == LED_ON)
            {
                (VOID)adi_gpio_SetLow(ADI_GPIO_PORT0, ADI_GPIO_PIN_13); /*LED3 on */
            }
            else
            {
                (VOID)adi_gpio_SetHigh(ADI_GPIO_PORT0, ADI_GPIO_PIN_13); /*LED3 off */
            }
            break;
        }
        case LOS_LED2:
        {
            if (cmd == LED_ON)
            {
                (VOID)adi_gpio_SetLow(ADI_GPIO_PORT1, ADI_GPIO_PIN_12); /*LED4 on */
            }
            else
            {
                (VOID)adi_gpio_SetHigh(ADI_GPIO_PORT1, ADI_GPIO_PIN_12); /*LED4 off */
            }
            break;
        }
        case LOS_LED3:
        {
            if (cmd == LED_ON)
            {
                (VOID)adi_gpio_SetLow(ADI_GPIO_PORT1, ADI_GPIO_PIN_13); /*LED5 on */
            }
            else
            {
                (VOID)adi_gpio_SetHigh(ADI_GPIO_PORT1, ADI_GPIO_PIN_13); /*LED5 off */
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


