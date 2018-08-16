
#ifndef _APP_ADC_H_
#define _APP_ADC_H_

#include "stdint.h"

#include "fsl_adc.h"

#define ADC_NUM            1

#define ADC_CHANNEL_NUM    3

extern volatile uint8_t bADCSampleFlag;
extern adc_result_info_t *volatile gAdcResultInfoPtr;

extern uint8_t  adc_init(void);
extern uint16_t adc_read(uint16_t num);

#endif
