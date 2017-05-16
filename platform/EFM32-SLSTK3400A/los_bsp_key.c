#include "los_bsp_key.h"

#ifdef EFM32HG322F64
#include "em_cmu.h"
#include "em_gpio.h"
#include "core_cm0plus.h"
#include "bspconfig.h"
#endif

/*****************************************************************************
 Function    : LOS_EvbKeyInit
 Description : Init keys
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbKeyInit(void)
{
#ifdef EFM32HG322F64
    /* Enable GPIO clock */
    CMU_ClockEnable(cmuClock_GPIO, true);

    // Configure PB0, PB1 as inputs
    GPIO_PinModeSet(BSP_GPIO_PB0_PORT, BSP_GPIO_PB0_PIN, gpioModeInput, 0);
    GPIO_PinModeSet(BSP_GPIO_PB1_PORT, BSP_GPIO_PB1_PIN, gpioModeInput, 0);
#endif

    return ;
}


/*****************************************************************************
 Function    : LOS_EvbGetKeyVal
 Description : Get key value
 Input       : KeyNum
 Output      : None
 Return      : None
 *****************************************************************************/
uint8_t LOS_EvbGetKeyVal(int KeyNum)
{
#ifdef EFM32HG322F64
    switch(KeyNum) {
        case 0:
            return GPIO_PinInGet(BSP_GPIO_PB0_PORT, BSP_GPIO_PB0_PIN);
        case 1:
            return GPIO_PinInGet(BSP_GPIO_PB1_PORT, BSP_GPIO_PB1_PIN);
        default:
            break;
    }
#endif
    return LOS_GPIO_ERR;

}

/* End of file */

