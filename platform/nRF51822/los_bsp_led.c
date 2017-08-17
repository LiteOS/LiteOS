#include "stdint.h"
#include "los_bsp_led.h"

/******************************************************************************
	here include some special hearder file you need
******************************************************************************/
#ifdef BOARD_NRF51822
#include "nrf_drv_gpiote.h"
#include "app_error.h"
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
#ifdef BOARD_NRF51822
	ret_code_t           err_code;
	
	nrf_drv_gpiote_init();

	nrf_drv_gpiote_out_config_t config = GPIOTE_CONFIG_OUT_TASK_TOGGLE(true);
	err_code = nrf_drv_gpiote_out_init(Green_LED_PIN, &config);
	APP_ERROR_CHECK(err_code);

	err_code = nrf_drv_gpiote_out_init(Red_LED_PIN, &config);
	APP_ERROR_CHECK(err_code);

	err_code = nrf_drv_gpiote_out_init(White_LED_PIN, &config);
	APP_ERROR_CHECK(err_code);
#endif
	return ;
}

/*************************************************************************************************
 *  Function    £ºLOS_EvbLedControl
 *  Discription : Led control function
 *  Input       : (1) index Led's index                                                                  *
 *                (2) cmd   Led on or off  
 *  Output      : None
 *  Return      : None                                                                                *
 *                                                                                 *
**************************************************************************************************/
void LOS_EvbLedControl(int index, int cmd)
{
#ifdef BOARD_NRF51822
	  switch (cmd)
    {
				case LED_ON:
						switch (index)
						{
								case Green_LED_PIN:
								case Red_LED_PIN:
								case White_LED_PIN:
										nrf_drv_gpiote_out_set(index);
										break;
								default:
								{
										break;
								}
						}
						break;
        case LED_OFF:
            switch (index)
						{
								case Green_LED_PIN:
								case Red_LED_PIN:
								case White_LED_PIN:
										nrf_drv_gpiote_out_clear(index);
										break;
								default:
								{
										break;
								}
						}
						break;
        default:
        {
            break;
        }
    }
#endif
		return ;
}


