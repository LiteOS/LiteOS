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
 * \date 2014-06-23
 * \author Huaqi Fang(Huaqi.Fang@synopsys.com)
--------------------------------------------- */

/**
 * \file
 * \brief  common io implementation
 */
#include <stdio.h>
#include "inc/embARC_toolchain.h"
#include "inc/arc/arc_builtin.h"
#include "device/device_hal/inc/dev_uart.h"
#include "common/console_io.h"
#include "common/xprintf.h"
#include "board/board.h"

#define CONSOLE_UART_ID		BOARD_CONSOLE_UART_ID	/*!< console uart id */

/*!< to indicate xprintf setup state(0 for not setup) */
static int xprintf_setup_flag = 0;
static DEV_UART *console_uart;		/*!< console uart device pointer */

/** put one char */
int console_putchar(unsigned char chr)
{
	if (console_uart == NULL) {
		return -1;
	}
	console_uart->uart_write((const void *)(&chr), 1);
	return 0;
}

/** put string */
int console_putstr(const char *str, unsigned int len)
{
	if (console_uart == NULL) {
		return -1;
	}
	return (int)console_uart->uart_write((const void *)(str), len);
}

/** get one char*/
int console_getchar(void)
{
	unsigned char data;
	if (console_uart == NULL) {
		return -1;
	}
	while (!console_uart->uart_read((void *)(&data), 1));

	return (int)data;
}

/** get string */
int console_getstr(char *str, unsigned int len)
{
	if (console_uart == NULL) {
		return -1;
	}
	return (int)console_uart->uart_read((void *)(str), len);
}

#ifndef ENABLE_BANNER
#define ENABLE_BANNER		1
#endif

#ifndef EMBARC_BANNER_TYPE
#define EMBARC_BANNER_TYPE	1
#endif

static const char *embarc_banner =
#if EMBARC_BANNER_TYPE == 1
"-----------------------------------------------------------	\r\n\
 ____                                _ ____			\r\n\
|  _ \\ _____      _____ _ __ ___  __| | __ ) _   _		\r\n\
| |_) / _ \\ \\ /\\ / / _ \\ '__/ _ \\/ _` |  _ \\| | | |	\r\n\
|  __/ (_) \\ V  V /  __/ | |  __/ (_| | |_) | |_| |		\r\n\
|_|   \\___/ \\_/\\_/ \\___|_|  \\___|\\__,_|____/ \\__, |	\r\n\
                                             |___/		\r\n\
                     _       _    ____   ____			\r\n\
       ___ _ __ ___ | |__   / \\  |  _ \\ / ___|		\r\n\
      / _ \\ '_ ` _ \\| '_ \\ / _ \\ | |_) | |			\r\n\
     |  __/ | | | | | |_) / ___ \\|  _ <| |___			\r\n\
      \\___|_| |_| |_|_.__/_/   \\_\\_| \\_\\\\____|		\r\n\
------------------------------------------------------------	\r\n\
";
#else
"-----------------------------------------------------------------------------------------------\r\n\
      _/_/_/                                                                _/ _/_/_/		\r\n\
     _/    _/   _/_/   _/      _/      _/   _/_/   _/  _/_/   _/_/     _/_/_/ _/    _/ _/    _/	\r\n\
    _/_/_/   _/    _/ _/      _/      _/ _/_/_/_/ _/_/     _/_/_/_/ _/    _/ _/_/_/   _/    _/	\r\n\
   _/       _/    _/   _/  _/  _/  _/   _/       _/       _/       _/    _/ _/    _/ _/    _/	\r\n\
  _/         _/_/       _/      _/       _/_/_/ _/         _/_/_/   _/_/_/ _/_/_/     _/_/_/	\r\n\
                                                                                         _/	\r\n\
                                                                                    _/_/	\r\n\
                                         _/            _/_/      _/_/_/     _/_/_/		\r\n\
              _/_/     _/_/_/  _/_/     _/_/_/      _/    _/   _/    _/   _/			\r\n\
           _/_/_/_/   _/    _/    _/   _/    _/    _/_/_/_/   _/_/_/     _/			\r\n\
          _/         _/    _/    _/   _/    _/    _/    _/   _/    _/   _/			\r\n\
           _/_/_/   _/    _/    _/   _/_/_/      _/    _/   _/    _/     _/_/_/			\r\n\
------------------------------------------------------------------------------------------------\r\n\
";
#endif

static void embarc_print_banner(void)
{
	xprintf("%s\r\n", embarc_banner);
}

/** xprintf need functions api setup */
void xprintf_setup(void)
{
	if (xprintf_setup_flag) {
		return;
	}
	console_uart = uart_get_dev(CONSOLE_UART_ID);
	console_uart->uart_open(BOARD_CONSOLE_UART_BAUD);

	xdev_in(console_getchar);
	xdev_out(console_putchar);

#if ENABLE_BANNER == 1
	embarc_print_banner();
#endif

	xprintf("embARC Build Time: %s, %s\r\n", __DATE__, __TIME__);
#if defined(__GNU__)
	xprintf("Compiler Version: ARC GNU, %s\r\n", __VERSION__);
#else
	xprintf("Compiler Version: Metaware, %s\r\n", __VERSION__);
#endif

	xprintf_setup_flag = 1;
}

