#include "los_bsp_led.h"

/******************************************************************************
	here include some special hearder file you need
******************************************************************************/
#ifdef LOS_FRDM_KW41Z
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"
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
#ifdef LOS_FRDM_KW41Z
    /* Port C clock is ungated at LOS_EvbUartInit, so here only enable A/B. */
    CLOCK_EnableClock(kCLOCK_PortA);
    CLOCK_EnableClock(kCLOCK_PortB);

    PORT_SetPinMux(LED_RED_PORT, LED_RED_PIN, kPORT_MuxAsGpio);
    PORT_SetPinMux(LED_GREEN_PORT, LED_GREEN_PIN, kPORT_MuxAsGpio);
    PORT_SetPinMux(LED_BLUE_PORT, LED_BLUE_PIN, kPORT_MuxAsGpio);
    PORT_SetPinMux(LED3_PORT, LED3_PIN, kPORT_MuxAsGpio);

    gpio_pin_config_t config =
    {
        kGPIO_DigitalOutput,
        0,
    };
    GPIO_PinInit(LED_RED_GPIO, LED_RED_PIN, &config);
    GPIO_PinInit(LED_GREEN_GPIO, LED_GREEN_PIN, &config);		
    GPIO_PinInit(LED_BLUE_GPIO, LED_BLUE_PIN, &config);		
    GPIO_PinInit(LED3_GPIO, LED3_PIN, &config);

    /* turn off all leds */
    GPIO_WritePinOutput(LED_RED_GPIO, LED_RED_PIN, 1);
    GPIO_WritePinOutput(LED_GREEN_GPIO, LED_GREEN_PIN, 1);
    GPIO_WritePinOutput(LED_BLUE_GPIO, LED_BLUE_PIN, 1);
    GPIO_WritePinOutput(LED3_GPIO, LED3_PIN, 1);
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
 #ifdef LOS_FRDM_KW41Z
  switch (index)
    {
        case LOS_LED1:
        {
            if (cmd == LED_ON)
            {
                GPIO_WritePinOutput(LED_RED_GPIO, LED_RED_PIN, 0);
            }
            else
            {
                GPIO_WritePinOutput(LED_RED_GPIO, LED_RED_PIN, 1);
            }
            break;
        }
        case LOS_LED2:
        {
            if (cmd == LED_ON)
            {
                GPIO_WritePinOutput(LED_GREEN_GPIO, LED_GREEN_PIN, 0);
            }
            else
            {
                GPIO_WritePinOutput(LED_GREEN_GPIO, LED_GREEN_PIN, 1);
            }
            break;
        }
        case LOS_LED3:
        {
            if (cmd == LED_ON)
            {
                GPIO_WritePinOutput(LED_GREEN_GPIO, LED_BLUE_PIN, 0);
            }
            else
            {
                GPIO_WritePinOutput(LED_GREEN_GPIO, LED_BLUE_PIN, 1);
            }
            break;
        }
        default:
        {
            break;
        }
    }
#endif 
	return ;
}


