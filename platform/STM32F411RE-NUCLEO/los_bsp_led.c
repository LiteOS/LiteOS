#include "los_bsp_led.h"

/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
#ifdef LOS_STM32F411RE
#include "stm32f4xx_nucleo.h"
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
    //add you code here.
#ifdef LOS_STM32F411RE
    BSP_LED_Init(LED2);
#endif
    
    return;
}

/*************************************************************************************************
 *  Function    £ºLOS_EvbLedControl
 *  Discription : Led control function
 *  Input       : (1) index Led's index
 *                (2) cmd   Led on or off
 *  Output      : None
 *  Return      : None
**************************************************************************************************/
void LOS_EvbLedControl(int index, int cmd)
{
#ifdef LOS_STM32L476xx
    switch (index)
    {
        case LOS_LED1:
        {
            if (cmd == LED_ON)
            {
                //add you code here.
                BSP_LED_On((Led_TypeDef)index); /* LD2 on */
            }
            else
            {
                //add you code here.
                BSP_LED_Off((Led_TypeDef)index); /* LD2 off */
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


