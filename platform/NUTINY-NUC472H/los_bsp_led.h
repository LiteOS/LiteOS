#ifndef _LOS_BSP_LED_H
#define _LOS_BSP_LED_H

#ifdef LOS_NUC472H
    #define PIN_LED1     (PB)
    #define BIT_LED1     (BIT10)
    #define LED1_CONTROL (PB10)
#endif


#define LED_ON      (0)
#define LED_OFF     (1)
#define LOS_LED1    (0)
#define LOS_LED2    (1)
#define LOS_LED3    (2)
#define LOS_LED4    (3)
#define LOS_LEDX    (4)


void LOS_EvbLedInit(void);
void LOS_EvbLedControl(int index, int cmd);
#endif
