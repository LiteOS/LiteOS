/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "pin_mux.h"

#include "fsl_common.h"
#include "fsl_iocon.h"

#include <stdbool.h>


#include "app_interrupt.h"
#include "app_led.h"
#include "app_adc.h"
#include "app_key.h"
#include "app_dmic.h"
#include "app_spiflash.h"
#include "app_pct2075.h"
#include "app_wm8904.h"
#include "app_usbdmsc.h"
#include "ff.h"
#include "diskio.h"
#include "app_spisd.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
volatile float fPCTValue;
static FATFS g_fileSystem; /* File system object */
const TCHAR driverNumberBuffer[3U] = {SDSPIDISK + '0', ':', '/'};


/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
	char ch;
	uint16_t wADCValue = 0;

	uint8_t ret;
	
	/* Init board hardware. */
	/* attach 12 MHz clock to FLEXCOMM0 (debug console) */
	CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

	BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_InitDebugConsole();
	
	SystemCoreClockUpdate();
	
	SysTick_Config(SystemCoreClock/1000);
	
	PRINTF("\r\n-------------------------------\r\n\r\n");
	PRINTF("hello world.\r\n");
	PRINTF("LPC54110 Sys Clock is %dMhz.\r\n", SystemCoreClock/1000000);
	PRINTF("\r\n-------------------------------\r\n");
	
  CLOCK_EnableClock(kCLOCK_InputMux);
  CLOCK_EnableClock(kCLOCK_Iocon);
  CLOCK_EnableClock(kCLOCK_Gpio0);
  CLOCK_EnableClock(kCLOCK_Gpio1);
	
	led_init();
	key_init();
	adc_init();
	dmic_init();
	ret = spiflash_init();
	if(ret == 1)
	{
		led_on(5);
	}
	else
	{
		led_off(5);
	}
	
	pct2075_i2c_init();
	wm8904_i2s_init();
	usbdmsc_init();
	
//	if (f_mount(&g_fileSystem, driverNumberBuffer, 1))
//	{
//			PRINTF("Mount volume failed.\r\n");
//	}
//		
//#if (_FS_RPATH >= 2)
//	if (f_chdrive((char const *)&driverNumberBuffer[0]))
//	{
//		PRINTF("Change drive failed.\r\n");
//		return;
//	}
//#endif	
//	FIL fd;
//	if(f_open(&fd, "4:/readme.txt", FA_READ) )
//	{
//		PRINTF("f_open failed.\r\n");
//		led_off(6);
//	}
//	else
//	{
//		led_on(6);
//	}

	
	while (1)
	{
// ADC test schedule
		wADCValue = adc_read(ADC_CHANNEL_NUM);
		if(wADCValue != 0xFFFF)
		{
			dwLedTime = (wADCValue*2000)/0xFFFF; // ajust the led blinky freqency
		}
// Button test schedule		
		if( (key_value(0) == 0) || (key_value(1) == 0) || (key_value(2) == 0) || (key_value(3) == 0) )
		{
			led_on(2);
		}
		else
		{
			led_off(2);
		}
		ret = pct2075_i2c_read((float *)&fPCTValue);
		if(ret == 1)
		{
			led_on(4);
		}
		else
		{
			led_off(4);
		}
		if(fPCTValue>=0)
		{
			PRINTF("Temperature Value is +%d.%d oC.\r", (int)(fPCTValue*100)/100, (int)(fPCTValue*100)%100);
		}
		else
		{
			PRINTF("Temperature Value is -%d.%d oC.\r", (int)((0-fPCTValue)*100)/100, (int)((0-fPCTValue)*100)%100);
		}
		
		usbdmsc_proc();
		__WFI();
	}
}
