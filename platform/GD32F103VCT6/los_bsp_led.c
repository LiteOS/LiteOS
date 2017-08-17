#include "los_bsp_led.h"

#ifdef GD32F103VCT6
#include "gd32f10x.h"
#endif

/******************************************************************************
    here include some special hearder file you need
******************************************************************************/



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
	  #ifdef GD32F103VCT6
    GPIO_InitPara GPIO_InitStructure;
    
    /* Enable GPIOC,GPIOE clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOC | RCC_APB2PERIPH_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_0 |GPIO_PIN_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_OUT_PP;
    GPIO_Init(GPIOC,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_OUT_PP;
    GPIO_Init(GPIOE,&GPIO_InitStructure);
	  #endif
    return ;
}

/*************************************************************************************************
 *  Function    £ºLOS_EvbLedControl
 *  Discription : Led control function
 *  Input       : (1) index Led's index                                                                  *
 *                (2) cmd   Led on or off  
 *  Output      : None
 *  Return      : None                                                                                *
 *                                                                                 *
**************************************************************************************************/
void LOS_EvbLedControl(int index, int cmd)
{
	 #ifdef GD32F103VCT6
    switch (index)
    {
        case LOS_LED1:
        {
            if (cmd == LED_ON)
            {
                //add you code here.
                        /*led1 on */
							  GPIOC->BOR = GPIO_PIN_0;     
            }
            else
            {
                //add you code here.
                GPIOC->BCR = GPIO_PIN_0;     
            }
            break;
        }
        case LOS_LED2:
        {
            if (cmd == LED_ON)
            {
                //add you code here.
							  GPIOC->BOR = GPIO_PIN_2;     
            }
            else
            {
                //add you code here.
                GPIOC->BCR = GPIO_PIN_2;     
            }
            break;
        }
        case LOS_LED3:
        {
            if (cmd == LED_ON)
            {    
                GPIOE->BOR = GPIO_PIN_0;
            }
            else
            {
                //add you code here.  
                GPIOE->BCR = GPIO_PIN_0;
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


