#ifndef _LOS_BSP_KEY_H
#define _LOS_BSP_KEY_H

#define APP_TIMER_PRESCALER         0x00 /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_OP_QUEUE_SIZE     0x04 /**< Size of timer operation queues. */

#define KEY1  12
#define KEY2  13
#define KEY3  14
#define KEY4  15
#define KEY5  16

#define LOS_KEY_PRESS  0
#define USER_KEY  KEY1
#define BUTTONS_NUMBER 5
#define BUTTON_PULL  NRF_GPIO_PIN_PULLUP
#define BUTTONS_LIST {KEY1, KEY2, KEY3, KEY4, KEY5}


extern void LOS_EvbKeyInit(void);
extern unsigned int LOS_EvbGetKeyVal(int KeyNum);
#endif

