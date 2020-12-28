/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Board
 * Author: Huawei LiteOS Team
 * Create: 2020-12-10
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
 * --------------------------------------------------------------------------- */

#if LOSCFG_DRIVERS_UART
#include "uart.h"
#include "hisoc/uart.h"
#include "linux/device.h"
#include "linux/platform_device.h"
#include "linux/module.h"
#include "virtual_serial.h"
#include "console.h"
#endif
#include "los_memory.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

VOID board_config(VOID)
{
    g_sys_mem_addr_end = 0x1000000;
}

#if LOSCFG_DRIVERS_UART
static struct uart_driver_data uart0_pl011_driver = {
    .num = 0,
    .baudrate = 115200,
    .attr.fifo_rx_en = 1,
    .attr.fifo_tx_en = 1,
    .flags = UART_FLG_RD_BLOCK,
};

static struct resource uart0_pl011_resources[] = {
    {
        .start = UART_REG_BASE,
        .end = UART_REG_BASE + 0x48,
        .flags = IORESOURCE_MEM,
    },
    {
        .start = NUM_HAL_INTERRUPT_UART,
        .end = NUM_HAL_INTERRUPT_UART,
        .flags = IORESOURCE_IRQ,
    },
};
struct platform_device uart0_pl011_device = {
    .name = "uart-pl011",
    .id = -1,
    .dev = {
        .driver_data = &uart0_pl011_driver,
    },
    .resource = uart0_pl011_resources,
    .num_resources = ARRAY_SIZE(uart0_pl011_resources),
};

int machine_init(void)
{
    PRINTK("machine_init\n");
    (void)platform_device_register(&uart0_pl011_device);

    return 0;
}

#ifdef LOSCFG_DRIVERS_BASE
arch_initcall(machine_init);
#endif

#endif