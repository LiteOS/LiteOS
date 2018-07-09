#ifndef _LOS_BSP_LED_H
#define _LOS_BSP_LED_H

#ifdef LOS_STM32F103ZE

#include "stm32f10x.h"


#define macLED1_GPIO_PORT    	GPIOA
#define macLED1_GPIO_CLK 	    RCC_APB2Periph_GPIOA
#define macLED1_GPIO_PIN		  GPIO_Pin_4

#define macLED2_GPIO_PORT    	GPIOA
#define macLED2_GPIO_CLK 	    RCC_APB2Periph_GPIOA
#define macLED2_GPIO_PIN		  GPIO_Pin_5

#define macLED3_GPIO_PORT    	GPIOA
#define macLED3_GPIO_CLK 	    RCC_APB2Periph_GPIOA
#define macLED3_GPIO_PIN		  GPIO_Pin_6

#define macLED4_GPIO_PORT    	GPIOA
#define macLED4_GPIO_CLK 	    RCC_APB2Periph_GPIOA
#define macLED4_GPIO_PIN		  GPIO_Pin_7


#define LED1(a)	if (a)	\
					GPIO_SetBits(macLED1_GPIO_PORT,macLED1_GPIO_PIN);\
					else		\
					GPIO_ResetBits(macLED1_GPIO_PORT,macLED1_GPIO_PIN)

#define LED2(a)	if (a)	\
					GPIO_SetBits(macLED2_GPIO_PORT,macLED2_GPIO_PIN);\
					else		\
					GPIO_ResetBits(macLED2_GPIO_PORT,macLED2_GPIO_PIN)

#define LED3(a)	if (a)	\
					GPIO_SetBits(macLED2_GPIO_PORT,macLED3_GPIO_PIN);\
					else		\
					GPIO_ResetBits(macLED2_GPIO_PORT,macLED3_GPIO_PIN)


#define	digitalHi(p,i)			{p->BSRR=i;}
#define digitalLo(p,i)			{p->BRR=i;}
#define digitalToggle(p,i)		{p->ODR ^=i;}


#define LED1_TOGGLE		digitalToggle(macLED1_GPIO_PORT,macLED1_GPIO_PIN)
#define LED1_OFF		digitalLo(macLED1_GPIO_PORT,macLED1_GPIO_PIN)
#define LED1_ON			digitalHi(macLED1_GPIO_PORT,macLED1_GPIO_PIN)

#define LED2_TOGGLE		digitalToggle(macLED2_GPIO_PORT,macLED2_GPIO_PIN)
#define LED2_OFF		digitalLo(macLED2_GPIO_PORT,macLED2_GPIO_PIN)
#define LED2_ON			digitalHi(macLED2_GPIO_PORT,macLED2_GPIO_PIN)

#define LED3_TOGGLE		digitalToggle(macLED2_GPIO_PORT,macLED3_GPIO_PIN)
#define LED3_OFF		digitalLo(macLED2_GPIO_PORT,macLED3_GPIO_PIN)
#define LED3_ON			digitalHi(macLED2_GPIO_PORT,macLED3_GPIO_PIN)

void LED_GPIO_Config(void);

#endif


#define LED_ON	  (1)
#define LED_OFF	  (0)
#define LOS_LED1	(0)
#define LOS_LED2	(1)
#define LOS_LED3	(2)
#define LOS_LED4	(3)
#define LOS_LEDX	(4)


void LOS_EvbLedInit(void);
void LOS_EvbLedControl(int index, int cmd);

#endif
