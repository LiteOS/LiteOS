#ifndef _LOS_BSP_LED_H
#define _LOS_BSP_LED_H

#define LED_ON	(1)
#define LED_OFF	(0)

#define Green_LED_PIN  24
#define Red_LED_PIN    23
#define White_LED_PIN  30

#define LOS_LED1	Green_LED_PIN
#define LOS_LED2	Red_LED_PIN
#define LOS_LED3  White_LED_PIN

void LOS_EvbLedInit(void);
void LOS_EvbLedControl(int index, int cmd);
#endif
