#include "stdint.h"
#include "los_bsp_key.h"
#include "los_bsp_led.h"

/******************************************************************************
	here include some special hearder file you need
******************************************************************************/
#ifdef BOARD_NRF51822
#include "nrf_drv_gpiote.h"
#include "app_error.h"
#include "app_timer.h"
#include "bsp.h"
#endif

/*****************************************************************************
 Function    : LOS_EvbKeyHandler
 Description : Function for handling key events 
 Input       : bsp_event_t event
 Output      : None
 Return      : None
 *****************************************************************************/
#ifdef BOARD_NRF51822
void LOS_EvbKeyHandler(bsp_event_t event)
{
    switch (event)
    {
        case KEY1:
            nrf_drv_gpiote_out_toggle(Green_LED_PIN);
            break;

        case KEY2:
            nrf_drv_gpiote_out_toggle(Red_LED_PIN);
            break;

        case KEY3:
            nrf_drv_gpiote_out_toggle(White_LED_PIN);
            break;
				case KEY4:
            nrf_drv_gpiote_out_toggle(White_LED_PIN);
            break;
				case KEY5:
            nrf_drv_gpiote_out_toggle(White_LED_PIN);
            break;
        default:
            break;
    }
		return;
}
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
#ifdef BOARD_NRF51822
	uint32_t err_code;

	APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, NULL);

	err_code = bsp_init(BSP_INIT_BUTTONS,
											APP_TIMER_TICKS(100, APP_TIMER_PRESCALER),
											LOS_EvbKeyHandler);
	APP_ERROR_CHECK(err_code);
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
	unsigned int KeyVal = 0xFFFFFFFF; 
	
	//add you code here.
	
	return KeyVal;
}

