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

#include "app_key.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define KEY_GPIO_CFG IOCON_MODE_PULLUP | IOCON_FUNC0 | IOCON_GPIO_MODE | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
const uint8_t  KEY_GPIO_PORT[KEY_NUM] = { 1,  1,  1,  1 };
const uint8_t  KEY_GPIO_PIN [KEY_NUM] = { 8,  9, 10, 11 };

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
uint8_t key_init(void)
{	
	uint8_t ret = 0;
	uint8_t i = 0;
	
// Init output LED GPIO
	for(i=0; i<KEY_NUM; i++)
	{
		GPIO->DIR[KEY_GPIO_PORT[i]] &= ~(1U << KEY_GPIO_PIN[i]);
		GPIO->B[KEY_GPIO_PORT[i]][KEY_GPIO_PORT[i]] = KEY_GPIO_PIN[i];
		IOCON_PinMuxSet(IOCON, KEY_GPIO_PORT[i], KEY_GPIO_PIN[i], KEY_GPIO_CFG);
	}
	
	ret = 1;
	
	return ret;
}

uint8_t key_value(uint8_t num)
{
	return GPIO_ReadPinInput(GPIO, KEY_GPIO_PORT[num], KEY_GPIO_PIN[num]);
}

// end file
