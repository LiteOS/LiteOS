#include "los_bsp_key.h"

#ifdef LOS_M2S150_RV32

#include "core_gpio.h"
#include "hw_platform.h"

gpio_instance_t g_gpio_in;

#endif

void LOS_EvbKeyInit(void)
{

#ifdef LOS_M2S150_RV32

	GPIO_init( &g_gpio_in, COREGPIO_IN_BASE_ADDR, GPIO_APB_32_BITS_BUS );

	GPIO_config( &g_gpio_in, GPIO_0, GPIO_INPUT_MODE );
	GPIO_config( &g_gpio_in, GPIO_1, GPIO_INPUT_MODE );
	GPIO_config( &g_gpio_in, GPIO_2, GPIO_INPUT_MODE );
	GPIO_config( &g_gpio_in, GPIO_3, GPIO_INPUT_MODE );

#endif

    return;
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
#ifdef LOS_M2S150_RV32
    KeyVal = GPIO_get_inputs(&g_gpio_in);
    KeyVal >>= KeyNum;
    KeyVal &= 0x000000001;
#endif
    return KeyVal;
}

