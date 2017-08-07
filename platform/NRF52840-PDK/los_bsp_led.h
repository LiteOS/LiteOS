#ifndef _LOS_BSP_LED_H
#define _LOS_BSP_LED_H

#ifdef  LOS_NRF52840
	#include "nrf_gpio.h"
#endif

#define LED_ON      (1)
#define LED_OFF     (0)


#define LOS_LED1    (0)
#define LOS_LED2    (1)
#define LOS_LED3    (2)
#define LOS_LED4    (3)
#define LOS_LEDX    (4)

#ifdef LOS_NRF52840

// LEDs definitions for PCA10056
#define LEDS_NUMBER    4

#define LED_1          NRF_GPIO_PIN_MAP(0,13)
#define LED_2          NRF_GPIO_PIN_MAP(0,14)
#define LED_3          NRF_GPIO_PIN_MAP(0,15)
#define LED_4          NRF_GPIO_PIN_MAP(0,16)


#define LEDS_ACTIVE_STATE 0

#define LEDS_LIST { LED_1, LED_2, LED_3, LED_4 }

#endif

void LOS_EvbLedInit(void);
void LOS_EvbLedControl(int index, int cmd);
#endif
