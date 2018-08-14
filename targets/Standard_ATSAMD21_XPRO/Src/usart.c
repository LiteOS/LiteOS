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
#include "usart.h"
#include <peripheral_clk_config.h>
#include "hal_gpio.h"
#include "hal_usart_sync.h"
#include <hpl_gclk_base.h>
#include <hpl_pm_base.h>

/* Defines ------------------------------------------------------------------*/
#define PIN_TX_PA04 GPIO(GPIO_PORTA, 4)
#define PIN_RX_PA05 GPIO(GPIO_PORTA, 5)

/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
struct usart_sync_descriptor USART_0;

static struct io_descriptor *pio = NULL;

/* Extern variables ---------------------------------------------------------*/
/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/

void USART_0_PORT_init(void)
{
	gpio_set_pin_function(PIN_TX_PA04, PINMUX_PA04D_SERCOM0_PAD0);
	gpio_set_pin_function(PIN_RX_PA05, PINMUX_PA05D_SERCOM0_PAD1);
}

void USART_0_CLOCK_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM0);
	_gclk_enable_channel(SERCOM0_GCLK_ID_CORE, CONF_GCLK_SERCOM0_CORE_SRC);
}

void USART_0_init(void)
{
	USART_0_CLOCK_init();
	usart_sync_init(&USART_0, SERCOM0, (void *)NULL);
	USART_0_PORT_init();
}

/* Public functions ---------------------------------------------------------*/
void Debug_USART0_Init(void)
{
    USART_0_init();

	usart_sync_get_io_descriptor(&USART_0, &pio);
	usart_sync_enable(&USART_0);
}
/* Private functions --------------------------------------------------------*/
/* define fputc */
#if defined ( __CC_ARM ) || defined ( __ICCARM__ )  /* KEIL and IAR: printf will call fputc to print */
int fputc(int ch, FILE *f)
{
    if(pio != NULL)
    {
        io_write(pio, (const uint8_t *)&ch, 1);
    }
    return ch;
}
#elif defined ( __GNUC__ )  /* GCC: printf will call _write to print */
__attribute__((used)) int _write(int fd, char *ptr, int len)
{
    if(pio != NULL)
    {
        io_write(pio, (const uint8_t *)ptr, len);
    }
    return len;
}
#endif

