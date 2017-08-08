#ifndef _LOS_BSP_KEY_H
#define _LOS_BSP_KEY_H

#define LOS_KEY_PRESS   1

#define USER_KEY        0

#define LOS_GPIO_ERR    0xFF;

#ifdef LOS_NRF52840

#define BUTTONS_NUMBER 4

#define BUTTON_1       11
#define BUTTON_2       12
#define BUTTON_3       24
#define BUTTON_4       25
#define BUTTON_PULL    NRF_GPIO_PIN_PULLUP

#define BUTTONS_ACTIVE_STATE 0

#define BUTTONS_LIST { BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4 }

#define BSP_BUTTON_0   BUTTON_1
#define BSP_BUTTON_1   BUTTON_2
#define BSP_BUTTON_2   BUTTON_3
#define BSP_BUTTON_3   BUTTON_4

#endif


extern void LOS_EvbKeyInit(void);

unsigned int LOS_EvbGetKeyVal(int KeyNum);

#endif

