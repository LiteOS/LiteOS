#ifndef _LOS_BSP_LED_H
#define _LOS_BSP_LED_H

#ifdef LOS_M2S150_RV32

#include "core_gpio.h"

#endif

#define LED_ON      (0)
#define LED_OFF     (1)

#ifdef LOS_M2S150_RV32

#define LOS_LED1    (GPIO_0)
#define LOS_LED2    (GPIO_1)
#define LOS_LED3    (GPIO_2)
#define LOS_LED4    (GPIO_3)
#define LOS_LED5    (GPIO_4)
#define LOS_LED6    (GPIO_5)
#define LOS_LED7    (GPIO_6)
#define LOS_LED8    (GPIO_7)

#endif

void LOS_EvbLedInit(void);
void LOS_EvbLedControl(int index, int cmd);

#endif
