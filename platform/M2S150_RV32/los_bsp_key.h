#ifndef _LOS_BSP_KEY_H
#define _LOS_BSP_KEY_H

#define LOS_KEY_PRESS   0

#ifdef LOS_M2S150_RV32

#define SWITCH1			GPIO_0
#define SWITCH2			GPIO_2
#define SWITCH3			GPIO_1
#define SWITCH4			GPIO_3

#define USER_KEY        SWITCH2		//this used by los_inspect_entry.c

#endif

#define LOS_GPIO_ERR    0xFF

void LOS_EvbKeyInit(void);

unsigned int LOS_EvbGetKeyVal(int KeyNum);

#endif
