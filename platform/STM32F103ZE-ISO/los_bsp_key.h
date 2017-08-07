#ifndef _LOS_BSP_KEY_H
#define _LOS_BSP_KEY_H

#define USER_KEY    		0

#define LOS_KEY_PRESS   1

#ifdef LOS_STM32F103ZE
// KEY1
#define KEY1_GPIO_PORT    	GPIOA		                
#define KEY1_GPIO_CLK 	    RCC_APB2Periph_GPIOA		
#define KEY1_GPIO_PIN				GPIO_Pin_0			        
#endif

void LOS_EvbKeyInit(void);

unsigned int LOS_EvbGetKeyVal(int KeyNum);

#endif
