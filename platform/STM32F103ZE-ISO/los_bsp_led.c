#include "stdint.h"
#include "los_bsp_led.h"

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
#ifdef LOS_STM32F103ZE
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启GPIOB和GPIOF的外设时钟*/
		RCC_APB2PeriphClockCmd( macLED1_GPIO_CLK|macLED2_GPIO_CLK|macLED3_GPIO_CLK, ENABLE); 

		/*选择要控制的GPIOB引脚*/															   
		GPIO_InitStructure.GPIO_Pin = macLED1_GPIO_PIN;	

		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIOB0*/
		GPIO_Init(macLED1_GPIO_PORT, &GPIO_InitStructure);	
		
		/*选择要控制的GPIOF引脚*/															   
		GPIO_InitStructure.GPIO_Pin = macLED2_GPIO_PIN;

		/*调用库函数，初始化GPIOF7*/
		GPIO_Init(macLED2_GPIO_PORT, &GPIO_InitStructure);
		
		/*选择要控制的GPIOF引脚*/															   
		GPIO_InitStructure.GPIO_Pin = macLED3_GPIO_PIN;

		/*调用库函数，初始化GPIOF7*/
		GPIO_Init(macLED3_GPIO_PORT, &GPIO_InitStructure);			  

		/* 关闭所有led灯	*/
		GPIO_SetBits(macLED1_GPIO_PORT, macLED1_GPIO_PIN);
		
		/* 关闭所有led灯	*/
		GPIO_SetBits(macLED2_GPIO_PORT, macLED2_GPIO_PIN);	 
    
    /* 关闭所有led灯	*/
		GPIO_SetBits(macLED3_GPIO_PORT, macLED3_GPIO_PIN);	 
#endif

	return ;
}

/*************************************************************************************************
 *  Function    ：LOS_EvbLedControl
 *  Discription : Led control function
 *  Input       : (1) index Led's index                                                                 
 *                (2) cmd   Led on or off  
 *  Output      : None
 *  Return      : None                                                                                                                                                            
**************************************************************************************************/
void LOS_EvbLedControl(int index, int cmd)
{
#ifdef LOS_STM32F103ZE
    switch (index)
    {
        case LOS_LED1:
        {
            if (cmd == LED_ON)
            {
                LED1_ON; 
            }
            else
            {
                LED1_OFF; 
            }
            break;
        }
        case LOS_LED2:
        {
            if (cmd == LED_ON)
            {
                LED2_ON;
            }
            else
            {
                LED2_OFF;
            }
            break;
        }
        case LOS_LED3:
        {
            if (cmd == LED_ON)
            {
                LED3_ON; 
            }
            else
            {
                LED3_OFF;
            }
            break;
        }
        default:
        {
            break;
        }
    }
#endif
	return ;
}

