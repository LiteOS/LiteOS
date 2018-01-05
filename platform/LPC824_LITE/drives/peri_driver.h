#ifndef _PERI_DRIVER_H_
#define _PERI_DRIVER_H_

#include "chip.h"
#include "romapi_8xx.h"
#include "acmp_8xx.h"

#if defined(CHIP_LPC82X)
	#include "adc_8xx.h"
#endif

#include "crc_8xx.h"

#if defined(CHIP_LPC82X)
	#include "dma_8xx.h"
#endif

#include "gpio_8xx.h"
#include "i2c_8xx.h"
#include "iap.h"
#include "crc_8xx.h"

#if defined(CHIP_LPC82X)
	#include "inmux_8xx.h"
#endif

#include "mrt_8xx.h"
#include "stopwatch.h"
#include "pinint_8xx.h"
#include "pmu_8xx.h"

#include "sct_8xx.h"
#include "sct_pwm_8xx.h"
#include "spi_8xx.h"
#include "uart_8xx.h"
#include "wkt_8xx.h"
#include "wwdt_8xx.h"
#include "iap.h"
#endif
