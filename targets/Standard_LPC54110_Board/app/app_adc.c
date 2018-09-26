
#include "board.h"

#include "pin_mux.h"

#include "fsl_common.h"
#include "fsl_iocon.h"

#include <stdbool.h>

#include "fsl_adc.h"
#include "fsl_clock.h"
#include "fsl_power.h"

#include "app_interrupt.h"
#include "app_adc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define ADC_GPIO_CFG IOCON_MODE_PULLUP | IOCON_FUNC0 | IOCON_GPIO_MODE | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
const uint8_t  ADC_GPIO_PORT[ADC_NUM] = { 1 };
const uint8_t  ADC_GPIO_PIN [ADC_NUM] = { 0 };

volatile uint8_t bADCSampleFlag = 0;

static adc_result_info_t gAdcResultInfoStruct;
adc_result_info_t *volatile gAdcResultInfoPtr = &gAdcResultInfoStruct;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
uint8_t adc_init(void)
{	
	uint8_t ret = 0;

	
	adc_config_t adcConfigStruct;
	adc_conv_seq_config_t adcConvSeqConfigStruct;
	
	
// Configure the power and clock for ADC.
  POWER_DisablePD(kPDRUNCFG_PD_ADC0);     /* Power on the ADC converter. */
  POWER_DisablePD(kPDRUNCFG_PD_VD7_ENA);  /* Power on the analog power supply. */
  POWER_DisablePD(kPDRUNCFG_PD_VREFP_SW); /* Power on the reference voltage source. */
  POWER_DisablePD(kPDRUNCFG_PD_TEMPS);    /* Power on the temperature sensor. */

// Enable the clock.
//  CLOCK_AttachClk(kFRO12M_to_MAIN_CLK);
  CLOCK_EnableClock(kCLOCK_Adc0); /* SYSCON->AHBCLKCTRL[0] |= SYSCON_AHBCLKCTRL_ADC0_MASK; */
	
	IOCON_PinMuxSet(IOCON, 1, 0, IOCON_MODE_INACT | IOCON_FUNC0 | IOCON_ANALOG_EN | IOCON_INPFILT_OFF);
	
// Calibration
	if (ADC_DoSelfCalibration(ADC0))
	{
    ;
	}
	else
	{
    return 0;
	}

	/* Configure the converter. */
	adcConfigStruct.clockMode = kADC_ClockSynchronousMode; /* Using sync clock source. */
	adcConfigStruct.clockDividerNumber = 1;                /* The divider for sync clock is 2. */
	adcConfigStruct.resolution = kADC_Resolution12bit;
	adcConfigStruct.enableBypassCalibration = false;
	adcConfigStruct.sampleTimeNumber = 0U;
	ADC_Init(ADC0, &adcConfigStruct);

	/* Use the sensor input to channel 0. */
	ADC_EnableTemperatureSensor(ADC0, true);

	/* Enable channel 0's conversion in Sequence A. */
	adcConvSeqConfigStruct.channelMask = (1U << ADC_CHANNEL_NUM); /* Includes channel 0. */
	adcConvSeqConfigStruct.triggerMask = 0U;
	adcConvSeqConfigStruct.triggerPolarity = kADC_TriggerPolarityNegativeEdge;
	adcConvSeqConfigStruct.enableSingleStep = false;
	adcConvSeqConfigStruct.enableSyncBypass = false;
	adcConvSeqConfigStruct.interruptMode = kADC_InterruptForEachSequence;
	ADC_SetConvSeqAConfig(ADC0, &adcConvSeqConfigStruct);
	ADC_EnableConvSeqA(ADC0, true); /* Enable the conversion sequence A. */
#if 0		
  ADC_EnableInterrupts(ADC0, kADC_ConvSeqAInterruptEnable);
  NVIC_EnableIRQ(ADC0_SEQA_IRQn);
	
	ADC_EnableConvSeqABurstMode(ADC0, true);
#endif	
  bADCSampleFlag = 0;
	
	ret = 1;
	return ret;
}

uint16_t adc_read(uint16_t num)
{
#if 0
	if(bADCSampleFlag == 0)
	{
		return 0xFFFF;
	}
	if(bADCSampleFlag == 1) // set 1 in ADC interrupt
	{
		bADCSampleFlag = 0;
		if(gAdcResultInfoStruct.channelNumber == num)
		{
			return (gAdcResultInfoStruct.result&0x0FFF);
		}
		
		return 0xFFFF;
	}
#endif
	ADC_DoSoftwareTriggerConvSeqA(ADC0);
	/* Wait for the converter to be done. */
	while (!ADC_GetChannelConversionResult(ADC0, num, &gAdcResultInfoStruct))
	{
	}	
	return (gAdcResultInfoStruct.result&0x0FFF);
}
// end file
