#ifndef _LOS_BSP_KEY_H
#define _LOS_BSP_KEY_H

#ifdef LOS_NUC472H
    #define PIN_KEY1     (PH)
    #define BIT_KEY1     (BIT12)
    #define KEY1_STATE   (PH12)
#endif

#define LOS_KEY_PRESS   0

#define USER_KEY        0

#define LOS_GPIO_ERR    0xFF;

extern void LOS_EvbKeyInit(void);

unsigned int LOS_EvbGetKeyVal(int KeyNum);

#endif

