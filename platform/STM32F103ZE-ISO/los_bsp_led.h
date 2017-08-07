#ifndef _LOS_BSP_LED_H
#define _LOS_BSP_LED_H

#ifdef LOS_STM32F103ZE

#include "stm32f10x.h"


/* 定义LED连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的LED引脚 */
#define macLED1_GPIO_PORT    	GPIOB			              /* GPIO端口 */
#define macLED1_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define macLED1_GPIO_PIN		  GPIO_Pin_0			          /* LED引脚号 */

#define macLED2_GPIO_PORT    	GPIOB			              /* GPIO端口 */
#define macLED2_GPIO_CLK 	    RCC_APB2Periph_GPIOF		/* GPIO端口时钟 */
#define macLED2_GPIO_PIN		  GPIO_Pin_1			         /* LED引脚号 */

#define macLED3_GPIO_PORT    	GPIOB			              /* GPIO端口 */
#define macLED3_GPIO_CLK 	    RCC_APB2Periph_GPIOF		/* GPIO端口时钟 */
#define macLED3_GPIO_PIN		  GPIO_Pin_5			          /* LED引脚号 */


/* 带参宏，可以像内联函数一样使用 */
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


/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)			{p->BSRR=i;}			//设置为高电平		
#define digitalLo(p,i)			{p->BRR=i;}				//输出低电平
#define digitalToggle(p,i)		{p->ODR ^=i;}			//输出反转状态


/* 定义控制IO的宏 */
#define LED1_TOGGLE		digitalToggle(macLED1_GPIO_PORT,macLED1_GPIO_PIN)
#define LED1_OFF		digitalHi(macLED1_GPIO_PORT,macLED1_GPIO_PIN)
#define LED1_ON			digitalLo(macLED1_GPIO_PORT,macLED1_GPIO_PIN)

#define LED2_TOGGLE		digitalToggle(macLED2_GPIO_PORT,macLED2_GPIO_PIN)
#define LED2_OFF		digitalHi(macLED2_GPIO_PORT,macLED2_GPIO_PIN)
#define LED2_ON			digitalLo(macLED2_GPIO_PORT,macLED2_GPIO_PIN)

#define LED3_TOGGLE		digitalToggle(macLED2_GPIO_PORT,macLED3_GPIO_PIN)
#define LED3_OFF		digitalHi(macLED2_GPIO_PORT,macLED3_GPIO_PIN)
#define LED3_ON			digitalLo(macLED2_GPIO_PORT,macLED3_GPIO_PIN)

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
