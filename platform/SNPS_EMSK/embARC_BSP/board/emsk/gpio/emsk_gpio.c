/* ------------------------------------------
 * Copyright (c) 2016, Synopsys, Inc. All rights reserved.

 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:

 * 1) Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.

 * 2) Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.

 * 3) Neither the name of the Synopsys, Inc., nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.

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
 *
 * \version 2016.05
 * \date 2014-07-31
 * \author Wayne Ren(Wei.Ren@synopsys.com)
--------------------------------------------- */
/**
 * \defgroup	BOARD_EMSK_DRV_GPIO	EMSK GPIO Driver
 * \ingroup	BOARD_EMSK_DRIVER
 * \brief	EMSK Onboard GPIO Peripherals Driver
 * \details
 *		Implementation of EMSK on-board button, led, dip switch driver,
 *	this is for better use, you don't have to get a gpio object before operating
 *	led,button,switch, because some basic functions for usage are provided.
 */

/**
 * \file
 * \ingroup	BOARD_EMSK_DRV_GPIO
 * \brief	all on emsk board gpio device init and operate functions
 * \details	this gpio devices are button, led, dip switch
 */

/**
 * \addtogroup	BOARD_EMSK_DRV_GPIO
 * @{
 */
#include "inc/arc/arc.h"
#include "inc/arc/arc_builtin.h"
#include "inc/embARC_toolchain.h"
#include "inc/embARC_error.h"

#include "board/emsk/gpio/emsk_gpio.h"
#include "board/emsk/emsk.h"

static DEV_GPIO *led_port;
static DEV_GPIO *button_port;
static DEV_GPIO *switch_port;

#define EMSK_GPIO_CHECK_EXP_NORTN(EXPR)		CHECK_EXP_NOERCD(EXPR, error_exit)

/** emsk on board gpio init */
void emsk_gpio_init(void)
{
	emsk_led_init();
	emsk_button_init();
	emsk_switch_init();
}

/** emsk on-board led init, led default off */
void emsk_led_init(void)
{
	led_port = gpio_get_dev(EMSK_LED_PORT);

	EMSK_GPIO_CHECK_EXP_NORTN(led_port != NULL);

	if (led_port->gpio_open(EMSK_LED_DIR) == E_OPNED) {
		led_port->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(EMSK_LED_MASK));
		led_port->gpio_control(GPIO_CMD_DIS_BIT_INT, (void *)(EMSK_LED_MASK));
	}

	led_write(0, BOARD_LED_MASK);

error_exit:
	return;
}

/** emsk on-board button init */
void emsk_button_init(void)
{
	button_port = gpio_get_dev(EMSK_BUTTON_PORT);

	EMSK_GPIO_CHECK_EXP_NORTN(button_port != NULL);

	if (button_port->gpio_open(EMSK_BUTTON_DIR) == E_OPNED) {
		button_port->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)(EMSK_BUTTON_MASK));
		button_port->gpio_control(GPIO_CMD_DIS_BIT_INT, (void *)(EMSK_BUTTON_MASK));
	}

error_exit:
	return;
}

/** emsk on-board switch init */
void emsk_switch_init(void)
{
	switch_port = gpio_get_dev(EMSK_SWITCH_PORT);

	EMSK_GPIO_CHECK_EXP_NORTN(switch_port != NULL);

	if (switch_port->gpio_open(EMSK_SWITCH_DIR) == E_OPNED) {
		switch_port->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)(EMSK_SWITCH_MASK));
		switch_port->gpio_control(GPIO_CMD_DIS_BIT_INT, (void *)(EMSK_SWITCH_MASK));
	}

error_exit:
	return;
}

/**
 * workaround for get led value
 * because gpio read return is not right
 */
static uint32_t g_led_val;
/** write 1 to light on led bit, else light off led */
void led_write(uint32_t led_val, uint32_t mask)
{
	EMSK_GPIO_CHECK_EXP_NORTN(led_port != NULL);

	led_val = (~led_val) & mask;
	led_val = led_val << EMSK_LED_OFFSET;
	mask = (mask << EMSK_LED_OFFSET) & EMSK_LED_MASK;
	led_port->gpio_write(led_val, mask);

	g_led_val = led_val;

error_exit:
	return;
}

/** read led value, on for 1, off for 0 */
/** \todo need to find why when led set to output then can't get the right value of led */
uint32_t led_read(uint32_t mask)
{
	uint32_t value;

	EMSK_GPIO_CHECK_EXP_NORTN(led_port != NULL);

	mask = (mask << EMSK_LED_OFFSET) & EMSK_LED_MASK;
	led_port->gpio_read(&value, mask);
	value = (~value) & EMSK_LED_MASK;

	return (value >> EMSK_LED_OFFSET);

error_exit:
	return 0;
}

/** Pull down switch return 1, else 0 */
uint32_t switch_read(uint32_t mask)
{
	uint32_t value;

	EMSK_GPIO_CHECK_EXP_NORTN(switch_port != NULL);

	mask = (mask << EMSK_SWITCH_OFFSET) & EMSK_SWITCH_MASK;
	switch_port->gpio_read(&value, mask);
	value = (~value) & mask;

	return (value >> EMSK_SWITCH_OFFSET);

error_exit:
	return 0;
}

/** Press down bit set to 1 else 0 */
uint32_t button_read(uint32_t mask)
{
	uint32_t value;

	EMSK_GPIO_CHECK_EXP_NORTN(button_port != NULL);

	mask = (mask << EMSK_BUTTON_OFFSET) & EMSK_BUTTON_MASK;
	button_port->gpio_read(&value, mask);
	value = (~value) & mask;

	return (value >> EMSK_BUTTON_OFFSET);

error_exit:
	return 0;
}

/** @} end of group BOARD_EMSK_DRV_GPIO */
