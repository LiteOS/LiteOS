#include "los_bsp_key.h"

#ifdef LOS_MM32L073PF 
#include "key.h"
#endif
/******************************************************************************
    here include some special hearder file you need
******************************************************************************/




/*****************************************************************************
 Function    : LOS_EvbKeyInit
 Description : Init GIOP Key 
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbKeyInit(void)
{   
    //add you code here.
#ifdef LOS_MM32L073PF 
    KEY_Init();
#endif  
    return ;
}

/*****************************************************************************
 Function    : LOS_EvbGetKeyVal
 Description : Get GIOP Key value
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
unsigned int LOS_EvbGetKeyVal(int KeyNum)
{
    unsigned int KeyVal = LOS_GPIO_ERR;
    
    //add you code here.
#ifdef LOS_MM32L073PF
    switch (KeyNum)
    {
        case KEY_1:
        {
            KeyVal = KEY1;
            break;
        }
        case KEY_2:
        {
            KeyVal = WK_UP;
            break;
        }
        case KEY_3:
        {
            KeyVal = KEY3;
            break;
        }
        case KEY_4:
        {
            KeyVal = KEY4;
            break;
        }
        default:
        {
            break;
        }
    }
#endif
    
    return KeyVal;
}
