#include "los_bsp_key.h"

/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
#ifdef GD32F407
#include "gd32f4xx.h"
#include "gd32f407nb-iot_eval.h"
#endif

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
#ifdef GD32F407 
    /* configure User key */
    gd_eval_key_init(KEY_USER1, KEY_MODE_GPIO);
    
    /* configure TAMPER key */
    gd_eval_key_init(KEY_USER2, KEY_MODE_EXTI);
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
uint8_t LOS_EvbGetKeyVal(int KeyNum)
{
#ifdef GD32F407
      if(KeyNum > KEY_USER2)
        {
            return LOS_GPIO_ERR;
        }
        return gd_eval_key_state_get(KeyNum);
#else
        return LOS_GPIO_ERR;
#endif
}


/*****************************************************************************
 Function    : EXTIxxx_IRQHandler
 Description : EXT xxx IRQ Handler detail,change the function name
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void EXTI10_15_IRQHandler(void)
{
#ifdef GD32F407
    if(RESET != exti_interrupt_flag_get(EXTI_13)){
        gd_eval_led_toggle(LED2);
    }
    exti_interrupt_flag_clear(EXTI_13);
#endif
}







