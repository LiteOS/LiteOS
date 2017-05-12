#include "los_bsp_key.h"

/******************************************************************************
	here include some special hearder file you need
******************************************************************************/
#ifdef LOS_FRDM_KW41Z
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"
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
    /* Port C clock is ungated at LOS_EvbUartInit, so here do nothing. */
#ifdef LOS_FRDM_KW41Z
    /* Set pin config, includes pull up & pinmux */
    port_pin_config_t pinConfig = {0};
    pinConfig.pullSelect = kPORT_PullUp; /* it must be set as Pull Up and enabled */
    pinConfig.slewRate = kPORT_SlowSlewRate;
    pinConfig.passiveFilterEnable = kPORT_PassiveFilterDisable;
    pinConfig.driveStrength = kPORT_LowDriveStrength;
    pinConfig.mux = kPORT_MuxAsGpio;/* set pinmux as GPIO */
    PORT_SetPinConfig(SW3_PORT, SW3_PIN, &pinConfig);
    PORT_SetPinConfig(SW4_PORT, SW4_PIN, &pinConfig);

    gpio_pin_config_t gpioConfig =
    {
        kGPIO_DigitalInput,
        0,
    };
    GPIO_PinInit(SW3_GPIO, SW3_PIN, &gpioConfig);
    GPIO_PinInit(SW4_GPIO, SW4_PIN, &gpioConfig);
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
#ifdef LOS_FRDM_KW41Z
    KeyVal = GPIO_ReadPinInput(GPIOC, KeyNum);
#endif    
    return KeyVal;
}

/*****************************************************************************
 Function    : EXTIxxx_IRQHandler
 Description : EXT xxx IRQ Handler detail,change the function name 
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void EXTIxxx_IRQHandler(void)
{
    return;
}



