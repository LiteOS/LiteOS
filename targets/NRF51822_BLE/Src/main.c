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

#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_rtc.h"
#include "simple_uart.h"
#include "main.h"

static UINT32 s_TaskHandle;

static void HardWare_Init(void)
{
    nrf_gpio_range_cfg_output(LED_START,LED_STOP);
    simple_uart_config(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER, HWFC);
}

static void task1(void)
{
    int count = 0;
    
    while (true)
    {
        nrf_gpio_pin_set(LED_0);
        nrf_gpio_pin_set(LED_1);
        nrf_delay_ms(300);
        nrf_gpio_pin_clear(LED_0);
        nrf_gpio_pin_clear(LED_1);
        nrf_delay_ms(300);
        printf("[%d] This is Task1++++++++++\n", ++count);
        LOS_TaskDelay(100);
    }
}

static void task2(void)
{
    int count = 0;

    while (true)
    {
        nrf_gpio_pin_set(LED_2);
        nrf_gpio_pin_set(LED_3);
        nrf_gpio_pin_set(LED_4);
        nrf_delay_ms(300);
        nrf_gpio_pin_clear(LED_2);
        nrf_gpio_pin_clear(LED_3);
        nrf_gpio_pin_clear(LED_4);
        nrf_delay_ms(300);
        printf("[%d] This is Task2----------\n", ++count);
        LOS_TaskDelay(100);
    }
}

static UINT32 creat_tasks(void)
{
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 0;
    task_init_param.pcName = "task1";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task1;
    task_init_param.uwStackSize = 0x400;

    uwRet = LOS_TaskCreate(&s_TaskHandle, &task_init_param);
    if (LOS_OK != uwRet)
    {
        return uwRet;
    }

    task_init_param.usTaskPrio = 1;
    task_init_param.pcName = "task2";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task2;

    return LOS_TaskCreate(&s_TaskHandle, &task_init_param);
}

/**
 * @brief Function for application main entry.
 */
int main(void)
{
    HardWare_Init();

    printf ("Wellcome to Huawei LiteOS!\n");

    if (LOS_KernelInit() != LOS_OK)
    {
        return LOS_NOK;
    }

    if (creat_tasks() != LOS_OK)
    {
        return LOS_NOK;
    }

    nrf_rtc_init();         /* 51822 do not have SysTick, use RTC as tick timer */
    LOS_Start();
}

/* for printf */

#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
};

FILE __stdout;

void _sys_exit(int x)
{
    x = x;
}

int fputc(int ch, FILE *f)
{
    simple_uart_put ((uint8_t) ch);
    return ch;
}

/**
 *@}
 **/
