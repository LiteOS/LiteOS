#ifndef _LOS_BSP_KEY_H
#define _LOS_BSP_KEY_H

#define LOS_KEY_PRESS   0

#define USER_KEY    1

//for onenet key0
#ifdef LOS_STM32F103ZE
// KEY1
#define KEY1_GPIO_PORT    	GPIOC		                
#define KEY1_GPIO_CLK 	    RCC_APB2Periph_GPIOC
#define KEY1_GPIO_PIN				GPIO_Pin_0			        
#endif



extern void LOS_EvbKeyInit(void);

unsigned int LOS_EvbGetKeyVal(int KeyNum);

#endif

