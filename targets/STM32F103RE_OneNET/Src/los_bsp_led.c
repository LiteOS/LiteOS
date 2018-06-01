#include "los_bsp_led.h"


/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
#ifdef LOS_STM32F103ZE
#include "stm32f10x.h"  //suzhen
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
#ifdef LOS_STM32F103ZE
		/*?¡§¨°?¨°???GPIO_InitTypeDef¨¤¨¤D¨ª¦Ì??¨¢11¨¬?*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*?a??GPIOBo¨ªGPIOF¦Ì?¨ªa¨¦¨¨¨º¡À?¨®*/
		RCC_APB2PeriphClockCmd( macLED1_GPIO_CLK|macLED2_GPIO_CLK|macLED3_GPIO_CLK, ENABLE); 

		/*????¨°a????¦Ì?GPIOB¨°y??*/															   
		GPIO_InitStructure.GPIO_Pin = macLED1_GPIO_PIN;	

		/*¨¦¨¨??¨°y???¡ê¨º??a¨ª¡§¨®?¨ª?¨ª¨¬¨º?3?*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*¨¦¨¨??¨°y???¨´?¨º?a50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*¦Ì¡Â¨®??ao¡¥¨ºy¡ê?3?¨º??¡¥GPIOB0*/
		GPIO_Init(macLED1_GPIO_PORT, &GPIO_InitStructure);	
		
		/*????¨°a????¦Ì?GPIOF¨°y??*/															   
		GPIO_InitStructure.GPIO_Pin = macLED2_GPIO_PIN;

		/*¦Ì¡Â¨®??ao¡¥¨ºy¡ê?3?¨º??¡¥GPIOF7*/
		GPIO_Init(macLED2_GPIO_PORT, &GPIO_InitStructure);
		
		/*????¨°a????¦Ì?GPIOF¨°y??*/															   
		GPIO_InitStructure.GPIO_Pin = macLED3_GPIO_PIN;

		/*¦Ì¡Â¨®??ao¡¥¨ºy¡ê?3?¨º??¡¥GPIOF7*/
		GPIO_Init(macLED3_GPIO_PORT, &GPIO_InitStructure);			  

		/* 1?¡À??¨´¨®Dled¦Ì?	*/
		GPIO_SetBits(macLED1_GPIO_PORT, macLED1_GPIO_PIN);
		
		/* 1?¡À??¨´¨®Dled¦Ì?	*/
		GPIO_SetBits(macLED2_GPIO_PORT, macLED2_GPIO_PIN);	 
    
    /* 1?¡À??¨´¨®Dled¦Ì?	*/
		GPIO_SetBits(macLED3_GPIO_PORT, macLED3_GPIO_PIN);	 
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
#ifdef LOS_STM32F103ZE
    switch (index)
    {
        case LOS_LED1:
        {
            if (cmd == LED_ON)
            {
                //add you code here.
                LED1_ON;         /*led1 on */
            }
            else
            {
                //add you code here.
                LED1_OFF;         /*led1 off */
            }
            break;
        }
        case LOS_LED2:
        {
            if (cmd == LED_ON)
            {
                //add you code here.
                //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);        /*led3 on */
           		LED2_ON;
		    }
            else
            {
                //add you code here.
                //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);      /*led3 off */
				LED2_OFF;           
		    }
            break;
        }
        case LOS_LED3:
        {
            if (cmd == LED_ON)
            {
                //add you code here.
                 LED3_ON;       /*led3 on */
            }
            else
            {
                //add you code here.
                LED3_OFF;        /*led3 off */
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


