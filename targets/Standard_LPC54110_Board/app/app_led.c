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
#include "board.h"

#include "pin_mux.h"

#include "fsl_common.h"
#include "fsl_iocon.h"

#include <stdbool.h>

#include "app_led.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define LED_GPIO_CFG IOCON_MODE_PULLUP | IOCON_FUNC0 | IOCON_GPIO_MODE | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
const uint8_t  LED_GPIO_PORT[LED_NUM] = { 0,  0,  0,  0,  0,  0,  0,  0};
const uint8_t  LED_GPIO_PIN [LED_NUM] = {15, 19, 21, 22, 25, 26, 29, 30};
const uint8_t  LED_GPIO_ON  [LED_NUM] = { 1,  1,  1,  1,  0,  0,  0,  0};
const uint8_t  LED_GPIO_OFF [LED_NUM] = { 0,  0,  0,  0,  1,  1,  1,  1};

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
uint8_t led_init(void)
{	
	uint8_t ret = 0;
	uint8_t i = 0;
	
// Init output LED GPIO
	for(i=0; i<LED_NUM; i++)
	{
		GPIO->DIR[LED_GPIO_PORT[i]] |= 1U << LED_GPIO_PIN[i];
		GPIO->B[LED_GPIO_PORT[i]][LED_GPIO_PIN[i]] = LED_GPIO_OFF[i];
		IOCON_PinMuxSet(IOCON, LED_GPIO_PORT[i], LED_GPIO_PIN[i], LED_GPIO_CFG);
	}
	
	ret = 1;
	
	return ret;
}

void led_on(uint8_t num)
{
	GPIO->B[LED_GPIO_PORT[num]][LED_GPIO_PIN[num]] = LED_GPIO_ON[num];
}

void led_off(uint8_t num)
{
	GPIO->B[LED_GPIO_PORT[num]][LED_GPIO_PIN[num]] = LED_GPIO_OFF[num];
}

void led_toggle(uint8_t num)
{
	GPIO->NOT[LED_GPIO_PORT[num]] |= (1 << LED_GPIO_PIN[num]);
}

// end file
