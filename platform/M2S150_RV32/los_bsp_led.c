#include "los_bsp_led.h"

#ifdef LOS_M2S150_RV32

#include "core_gpio.h"
#include "hw_platform.h"
gpio_instance_t g_gpio_out;

#endif

void LOS_EvbLedInit(void)
{

#ifdef LOS_M2S150_RV32
	GPIO_init( &g_gpio_out, COREGPIO_OUT_BASE_ADDR, GPIO_APB_32_BITS_BUS );

	GPIO_config( &g_gpio_out, GPIO_0, GPIO_OUTPUT_MODE );
	GPIO_config( &g_gpio_out, GPIO_1, GPIO_OUTPUT_MODE );
	GPIO_config( &g_gpio_out, GPIO_2, GPIO_OUTPUT_MODE );
	GPIO_config( &g_gpio_out, GPIO_3, GPIO_OUTPUT_MODE );
	GPIO_config( &g_gpio_out, GPIO_4, GPIO_OUTPUT_MODE );
	GPIO_config( &g_gpio_out, GPIO_5, GPIO_OUTPUT_MODE );
	GPIO_config( &g_gpio_out, GPIO_6, GPIO_OUTPUT_MODE );
	GPIO_config( &g_gpio_out, GPIO_7, GPIO_OUTPUT_MODE );

#endif
}

/*************************************************************************************************
 *  function��control led on off                                                                 *
 *  param (1) index Led's index                                                                  *
 *        (2) cmd   Led on or off                                                                *
 *  return : None                                                                                *
 *  discription:                                                                                 *
**************************************************************************************************/
void LOS_EvbLedControl(int index, int cmd)
{
#ifdef LOS_M2S150_RV32
	GPIO_set_output( &g_gpio_out, index, cmd );

#endif
}


