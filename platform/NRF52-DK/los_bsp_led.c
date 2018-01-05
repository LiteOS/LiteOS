#include "los_bsp_led.h"
#include "nrf_gpio.h"
/******************************************************************************
    here include some special hearder file you need
******************************************************************************/

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
    nrf_gpio_cfg_output(17);
		nrf_gpio_cfg_output(18);
		nrf_gpio_cfg_output(19);
		nrf_gpio_cfg_output(20);
		nrf_gpio_pin_write(17,1);
		nrf_gpio_pin_write(18,1);
		nrf_gpio_pin_write(19,1);
		nrf_gpio_pin_write(20,1);
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
    switch (index)
    {
        case LOS_LED1:
        {
            if (cmd == LED_ON)
            {
                
                //add you code here.
                nrf_gpio_pin_write(17,0);
                        /*led1 on */
            }
            else
            {
                //add you code here.
                nrf_gpio_pin_write(17,1);
                        /*led1 off */
            }
            break;
        }
        case LOS_LED2:
        {
            if (cmd == LED_ON)
            {
							nrf_gpio_pin_write(18,0);
                //add you code here.
                        /*led2 on */
            }
            else
            { 
							nrf_gpio_pin_write(18,1);
							//add you code here.
                        /*led2 off */
            }
            break;
        }
        case LOS_LED3:
        {
            if (cmd == LED_ON)
            {
							nrf_gpio_pin_write(19,0);
                //add you code here.
                        /*led3 on */
            }
            else
            {
							nrf_gpio_pin_write(19,1);
							//add you code here.
                        /*led3 off */
            }
            break;
        }
				case LOS_LED4:
        {
            if (cmd == LED_ON)
            {
							nrf_gpio_pin_write(20,0);
                //add you code here.
                        /*led3 on */
            }
            else
            {
							nrf_gpio_pin_write(20,1);
							//add you code here.
                        /*led3 off */
            }
            break;
        }
        default:
        {
            break;
        }
    }
    return;
}


