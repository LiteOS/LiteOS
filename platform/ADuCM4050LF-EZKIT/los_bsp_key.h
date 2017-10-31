#ifndef _LOS_BSP_KEY_H
#define _LOS_BSP_KEY_H

#define LOS_KEY_PRESS   0

#define PB1             1
#define PB2             2
#define USER_KEY        PB1


extern void LOS_EvbKeyInit(void);

unsigned int LOS_EvbGetKeyVal(int KeyNum);

#endif

