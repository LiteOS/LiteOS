#ifndef __LED_H
#define __LED_H	 
#include "MM32L073PF.h"

#ifndef LED_ON
#define LED_ON      (0)
#endif

#ifndef LED_OFF
#define LED_OFF     (1)
#endif

#define LED1_ON()   GPIOB->BRR = 0x0020// PB5
#define LED1_OFF() 	GPIOB->BSRR = 0x0020// PB5
#define LED1_TOGGLE()  (GPIOB->ODR & 0x0020) ? (GPIOB->BRR = 0x0020) : (GPIOB->BSRR = 0x0020)

#define LED2_ON()  	GPIOB->BRR = 0x0010// PB4
#define LED2_OFF() 	GPIOB->BSRR = 0x0010// PB4
#define LED2_TOGGLE()  (GPIOB->ODR & 0x0010) ? (GPIOB->BRR = 0x0010) : (GPIOB->BSRR = 0x0010)

#define LED3_ON()  	GPIOB->BRR = 0x0008// PB3
#define LED3_OFF() 	GPIOB->BSRR = 0x0008;// PB3
#define LED3_TOGGLE()  (GPIOB->ODR & 0x0008) ? (GPIOB->BRR = 0x0008) : (GPIOB->BSRR = 0x0008)

#define LED4_ON()  	  GPIOA->BRR = 0x8000;// PA15
#define LED4_OFF()    GPIOA->BSRR = 0x8000// PA15
#define LED4_TOGGLE()  (GPIOA->ODR & 0x8000) ? (GPIOA->BRR = 0x8000) : (GPIOA->BSRR = 0x8000)

void LED_Init(void);//≥ı ºªØ

#endif /* __LEN_H */
