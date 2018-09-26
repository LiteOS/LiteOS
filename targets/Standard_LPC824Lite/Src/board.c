/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

/* Includes -----------------------------------------------------------------*/
#include "board.h"
/* Defines ------------------------------------------------------------------*/
#define LED_GPIO        GPIO
#define LED_GPIO_PORT   0
/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
static const uint8_t ledBits[BOARD_LED_CNT] = {7, 13, 16, 17, 19, 27, 28, 18};

/* Extern variables ---------------------------------------------------------*/
/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
static void Board_LED_Init(void)
{
    int i;

    GPIO_PortInit(LED_GPIO, LED_GPIO_PORT);
    gpio_pin_config_t config = { kGPIO_DigitalOutput, 0};
    
	for (i = 0; i < BOARD_LED_CNT; i++) {
		GPIO_PinInit(LED_GPIO, LED_GPIO_PORT, ledBits[i], &config);
		GPIO_PinWrite(LED_GPIO, LED_GPIO_PORT, ledBits[i], 1);
	}
}
/* Public functions ---------------------------------------------------------*/

/* Set the LED to the state of "On" */
void Board_LED_Set(uint8_t LEDNumber, bool On)
{
	if (LEDNumber < BOARD_LED_CNT) {
		GPIO_PinWrite(LED_GPIO, LED_GPIO_PORT, ledBits[LEDNumber], (bool)!On);
	}
}

/* Toggles the current state of a board LED */
void Board_LED_Toggle(uint8_t LEDNumber)
{
	if (LEDNumber < BOARD_LED_CNT) {
		GPIO_PortToggle(LED_GPIO, LED_GPIO_PORT, 1UL << ledBits[LEDNumber]);
	}
}


void Board_Init(void)
{
    /* Initialize the LEDs */
    Board_LED_Init();

}
/* Private functions --------------------------------------------------------*/

