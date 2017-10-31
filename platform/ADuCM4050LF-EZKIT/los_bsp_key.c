#include <stdio.h>
#include <stdint.h>
#include "los_bsp_key.h"
#include "los_demo_debug.h"
/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
#ifdef LOS_ADuCM450LF
#include "adi_gpio.h"
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
#ifdef LOS_ADuCM450LF
    static uint8_t gpioMemory[ADI_GPIO_MEMORY_SIZE];
    if(ADI_GPIO_SUCCESS != adi_gpio_Init(gpioMemory, ADI_GPIO_MEMORY_SIZE))
    {
        dprintf("adi_gpio_Init failed\n");
    }

    /* set GPIO input PB1 and PB2 */
    if(ADI_GPIO_SUCCESS != adi_gpio_InputEnable(ADI_GPIO_PORT2, (ADI_GPIO_PIN_9 | ADI_GPIO_PIN_6), true))
    {
        dprintf("adi_gpio_SetDirection failed\n");
    }
#endif
    return;
}

/*****************************************************************************
 Function    : LOS_EvbGetKeyVal
 Description : Get GIOP Key value
 Input       : int KeyNum
 Output      : None
 Return      : None
 *****************************************************************************/
unsigned int LOS_EvbGetKeyVal(int KeyNum)
{
    uint16_t KeyVal = 0xFFFF;
 
#ifdef LOS_ADuCM450LF
    switch (KeyNum)
    {
        case PB1:
        {
            adi_gpio_GetData(ADI_GPIO_PORT2,ADI_GPIO_PIN_9,&KeyVal);
            break;
        }
        case PB2:
        {
            adi_gpio_GetData(ADI_GPIO_PORT2,ADI_GPIO_PIN_6,&KeyVal);
            break;
        }
        default:
            return KeyVal;
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
void EXTIxxx_IRQHandler(void)
{
    //add you code here.
    
    return;
}



