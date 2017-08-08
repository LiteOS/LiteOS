#include "los_bsp_led.h"

/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
#include "Nano100Series.h"
#include "gpio.h"


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
	
	  GPIO_SetMode(PA, BIT0, GPIO_PMD_OUTPUT);
	  PA->DOUT =0;

    
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
                        /*led1 on */
							//µãµÆ
						//	printf("light is on ^-^\n");
								PA->DOUT = 0xFFFF;
						//	PA->DOUT = 0x0;
							
            }
            else
            {
                //add you code here.
                        /*led1 off */
						//	printf("light is off ^-^\n");
						
						PA->DOUT = 0x0;
					//		PA->DOUT = 0xFFFF;
							
            }
            break;
        }
        case LOS_LED2:
        {
            if (cmd == LED_ON)
            {
                //add you code here.
                        /*led2 on */
            }
            else
            {
                //add you code here.
                        /*led2 off */
            }
            break;
        }
        case LOS_LED3:
        {
            if (cmd == LED_ON)
            {
                //add you code here.
                        /*led3 on */
            }
            else
            {
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


