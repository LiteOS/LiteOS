#ifndef _LOS_BSP_KEY_H
#define _LOS_BSP_KEY_H

#include "stdint.h"

#define LOS_GPIO_ERR    0xFF

#define LOS_KEY_PRESS   0

#define USER_KEY        1

extern void LOS_EvbKeyInit(void);

extern uint8_t LOS_EvbGetKeyVal(int KeyNum);

#endif

