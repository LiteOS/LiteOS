#ifndef _LOS_BSP_KEY_H
#define _LOS_BSP_KEY_H

#include "stdint.h"

#define LOS_GPIO_ERR              (0xFF)  /* Error return value for GPIO */

#define LOS_KEY_PRESS             (0)     /* State for key reset         */

#define USER_KEY                  (1)     /* Number for user key         */


extern void LOS_EvbKeyInit(void);

extern uint8_t LOS_EvbGetKeyVal(int KeyNum);

#endif

