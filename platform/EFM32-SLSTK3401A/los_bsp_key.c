#include "los_bsp_key.h"

#ifdef EFM32PG1B200F256GM48
#include "em_cmu.h"
#include "em_gpio.h"
#include "core_cm4.h"
#include "bspconfig.h"
#endif

void LOS_EvbKeyInit(void)
{

#ifdef EFM32PG1B200F256GM48
	/* Enable GPIO clock */
	CMU_ClockEnable(cmuClock_GPIO, true);

#if __ENABLE_INTERRUPT__
	/* Configure PB0 as input and enable interrupt	*/
	GPIO_PinModeSet(BSP_GPIO_PB0_PORT, BSP_GPIO_PB0_PIN, gpioModeInputPull, 1);
	GPIO_IntConfig(BSP_GPIO_PB0_PORT, BSP_GPIO_PB0_PIN, false, true, true);
	
	/* Configure PB1 as input and enable interrupt */
	GPIO_PinModeSet(BSP_GPIO_PB1_PORT, BSP_GPIO_PB1_PIN, gpioModeInputPull, 1);
	GPIO_IntConfig(BSP_GPIO_PB1_PORT, BSP_GPIO_PB1_PIN, false, true, true);
	
	NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
	NVIC_EnableIRQ(GPIO_EVEN_IRQn);
	
	NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
	NVIC_EnableIRQ(GPIO_ODD_IRQn);
#else
	// Configure PB0, PB1 as inputs
	GPIO_PinModeSet(BSP_GPIO_PB0_PORT, BSP_GPIO_PB0_PIN, gpioModeInput, 0);
	GPIO_PinModeSet(BSP_GPIO_PB1_PORT, BSP_GPIO_PB1_PIN, gpioModeInput, 0);
#endif

#endif

	return ;
}

/*****************************************************************************
 Function    : LOS_EvbGetKeyVal
 Description : Get key value
 Input       : KeyNum
 Output      : None
 Return      : None
 *****************************************************************************/
uint8_t LOS_EvbGetKeyVal(int KeyNum)
{
#ifdef EFM32PG1B200F256GM48
		switch(KeyNum) {
			case 0:
				return GPIO_PinInGet(BSP_GPIO_PB0_PORT, BSP_GPIO_PB0_PIN);
			case 1:
				return GPIO_PinInGet(BSP_GPIO_PB1_PORT, BSP_GPIO_PB1_PIN);
			default:
				break;
		}

		return LOS_GPIO_ERR;
#else
		return LOS_GPIO_ERR;
#endif
}

