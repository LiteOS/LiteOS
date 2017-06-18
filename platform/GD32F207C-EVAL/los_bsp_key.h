#ifndef _LOS_BSP_KEY_H
#define _LOS_BSP_KEY_H

#define LOS_KEY_PRESS  0

#define USER_KEY       1

#define LOS_GPIO_ERR   0xFF

extern void LOS_EvbKeyInit(void);
extern unsigned int LOS_EvbGetKeyVal(int KeyNum);

#endif//_LOS_BSP_KEY_H
