#ifndef _LOS_BSP_KEY_H
#define _LOS_BSP_KEY_H


#define KEY_1       1
#define KEY_2       2
#define KEY_3       3
#define KEY_4       4

#define LOS_KEY_PRESS       0
#define USER_KEY            KEY_1
#define LOS_GPIO_ERR        0xFF

extern void LOS_EvbKeyInit(void);

extern unsigned int LOS_EvbGetKeyVal(int KeyNum);

#endif//_LOS_BSP_KEY_H
