#include "los_bsp_led.h"
#include "Board_LED.h"

void LOS_EvbLedInit(void)
{

LED_Initialize();

	return ;
}

/*************************************************************************************************
 *  function£ºcontrol led on off                                                                 *
 *  param (1) index Led's index                                                                  *
 *        (2) cmd   Led on or off                                                                *
 *  return : None                                                                                *
 *  discription:                                                                                 *
**************************************************************************************************/
void LOS_EvbLedControl(int index, int cmd)
{
      if (cmd == LED_ON)
      {
          LED_On(0); /*led1 on */
      }
      else
      {
          LED_Off(0); /*led1 off */
      }
	return ;
}

