#include "los_bsp_key.h"

#ifdef GD32F190R8
#include "gd32f1x0_eval.h"
#endif


void LOS_EvbKeyInit(void)
{
#ifdef GD32F190R8
    /* configure User key */
    gd_eval_keyinit(KEY_TAMPER, KEY_MODE_EXTI);
    
    gd_eval_ledinit(LED2);
#endif

    return;
}

#ifdef GD32F190R8
/*!
    \brief      this function handles external lines 10 to 15 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EXTI4_15_IRQHandler(void)
{
    if(RESET != exti_interrupt_flag_get(EXTI_13)){
        gd_eval_ledtoggle(LED2);
    }
    exti_interrupt_flag_clear(EXTI_13);
}


#endif

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
    
    return KeyVal;
}
