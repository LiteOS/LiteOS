#include "los_bsp_led.h"

/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
#ifdef LOS_NRF52840
#include "nrf_gpio.h"
#endif

/*****************************************************************************
    here implements some special bsp code you need
******************************************************************************/
#ifdef LOS_NRF52840

#if LEDS_NUMBER > 0
static const uint8_t m_board_led_list[LEDS_NUMBER] = LEDS_LIST;
#endif

void bsp_board_led_on(uint32_t led_idx)
{
	  nrf_gpio_pin_write(m_board_led_list[led_idx], LEDS_ACTIVE_STATE ? 1 : 0);
}

void bsp_board_led_off(uint32_t led_idx)
{
    nrf_gpio_pin_write(m_board_led_list[led_idx],LEDS_ACTIVE_STATE ? 0 : 1);
}

void bsp_board_leds_off(void)
{
    uint32_t i;
    for(i = 0; i < LEDS_NUMBER; ++i)
    {
        bsp_board_led_off(i);
    }
}

void bsp_board_leds_on(void)
{
    uint32_t i;
    for(i = 0; i < LEDS_NUMBER; ++i)
    {
        bsp_board_led_on(i);
    }
}

void bsp_board_leds_init(void)
{
    uint32_t i;
    for(i = 0; i < LEDS_NUMBER; ++i)
    {
        nrf_gpio_cfg_output(m_board_led_list[i]);
    }
    bsp_board_leds_off();
}
#endif


/*****************************************************************************
 Function    : LOS_EvbLedInit
 Description : Init LED device
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbLedInit(void)
{
    //add you code here.
#ifdef LOS_NRF52840
    bsp_board_leds_init();
	  //nrf_gpio_cfg_output(13);
		//nrf_gpio_cfg_output(14);
		//nrf_gpio_cfg_output(15);
		//nrf_gpio_cfg_output(16);
#endif
    
    return;
}

/*************************************************************************************************
 *  Function    ¡êoLOS_EvbLedControl
 *  Discription : Led control function
 *  Input       : (1) index Led's index
 *                (2) cmd   Led on or off
 *  Output      : None
 *  Return      : None
**************************************************************************************************/
void LOS_EvbLedControl(int index, int cmd)
{
#ifdef LOS_NRF52840

	if (cmd == LED_ON)
	{
		//add you code here.
		bsp_board_led_on(index);   /* LED on */
	}
	else
	{
		//add you code here.
		bsp_board_led_off(index);  /* LED off */
	}
		
#endif
    return;
}


