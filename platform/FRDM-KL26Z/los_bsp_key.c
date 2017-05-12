#include "los_bsp_key.h"

#ifdef FRDM_KL25Z
#include "MKL25Z4.h"
#endif

#ifdef FRDM_KL26Z
#include "MKL26Z4.h"
#endif

#if (defined FRDM_KL25Z) || (defined FRDM_KL26Z)
void BSP_KEY_Init(void)
{
    SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
    
    PORTC_PCR3  &= ~(0x07<<8);
    PORTC_PCR3  |=   0x01<<8;                  //GPIO mode
    PORTC_PCR3  |=   0x03;                     //pullup
    GPIOC_PDDR  &= ~(1U << 3);                 //input
}

uint16_t BSP_KEY_Get(void)
{
    uint16_t KeyVal=LOS_GPIO_ERR;
    
    if(!((GPIOC_PDIR >> 3) & 1))
    {
        KeyVal = 0;
    }
    return KeyVal;
}
#endif

void LOS_EvbKeyInit(void)
{

#if (defined FRDM_KL25Z) || (defined FRDM_KL26Z)
    BSP_KEY_Init();
#endif
}


/*****************************************************************************
 Function    : LOS_EvbGetKeyVal
 Description : Get GIOP Key value
 Input       : int KeyNum
 Output      : None
 Return      : KeyVal
 *****************************************************************************/
unsigned int LOS_EvbGetKeyVal(int KeyNum)
{
    unsigned int KeyVal = LOS_GPIO_ERR; 

#if (defined FRDM_KL25Z) || (defined FRDM_KL26Z)
    if(KeyNum == USER_KEY)
    {
        KeyVal = BSP_KEY_Get();
    }
#endif
    
    return KeyVal;
}

