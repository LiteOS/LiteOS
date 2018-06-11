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
#include "simple_uart.h"
#include "main.h"
#include "los_swtmr.h"


/* 为了更小的代码尺寸请定义NO_UART,并且去除所有串口输出操作 */
#define NO_UART
#if 1
#undef  NO_UART
#endif

static void HardWare_Init(void)
{
    nrf_gpio_range_cfg_output(LED_START,LED_STOP);
#ifndef NO_UART
    simple_uart_config(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER);
#endif
}

UINT16 semHandler1, semHandler2, semHandler3;

static void task1(void)
{
    while (true)
    {
#ifndef NO_UART
        simple_uart_put ('1');
#endif

        #if 0
        LOS_TaskDelay(100);
        #else
        LOS_SemPend (semHandler2, LOS_WAIT_FOREVER);
        #endif
    }
}

static void task2(void)
{
    while (true)
    {
#ifndef NO_UART
        simple_uart_put ('2');
#endif
        LOS_TaskDelay(100);
        LOS_SemPost (semHandler2);
    }
}

static void task3(void)
{
    while (true)
    {
#ifndef NO_UART
        simple_uart_put ('3');
#endif
        LOS_TaskDelay(100);
    }
}

#if (LOSCFG_STATIC_TASK == YES)
LOS_TASK_DEF (task1, "t1", task1, 1, 0, 0x100);
LOS_TASK_DEF (task2, "t2", task2, 2, 0, 0x100);
LOS_TASK_DEF (task3, "t3", task3, 3, 0, 0x100);
#endif

static void creat_tasks(void)
{
    UINT32 taskHandle;

#if (LOSCFG_STATIC_TASK == YES)
    LOS_TASK_INIT (task1, &taskHandle);
    LOS_TASK_INIT (task2, &taskHandle);
    LOS_TASK_INIT (task3, &taskHandle);
#else
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 0;
    task_init_param.pcName = "task1";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task1;
    task_init_param.uwStackSize = 0x400;

    LOS_TaskCreate(&taskHandle, &task_init_param);

    task_init_param.usTaskPrio = 1;
    task_init_param.pcName = "task2";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task2;

    LOS_TaskCreate(&taskHandle, &task_init_param);

    task_init_param.usTaskPrio = 2;
    task_init_param.pcName = "task3";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task3;

    LOS_TaskCreate(&taskHandle, &task_init_param);
#endif
}

void tmrHandler (char arg)
{
#ifndef NO_UART
    simple_uart_put (arg);
#endif
}

#if (LOSCFG_STATIC_TIMER == YES)
LOS_TIMER_DEF (tmr1, 10, LOS_SWTMR_MODE_PERIOD, tmrHandler, 'a');
LOS_TIMER_DEF (tmr2, 5,  LOS_SWTMR_MODE_PERIOD, tmrHandler, 'b');
LOS_TIMER_DEF (tmr3, 20, LOS_SWTMR_MODE_PERIOD, tmrHandler, 'c');
LOS_TIMER_DEF (tmr4, 40, LOS_SWTMR_MODE_PERIOD, tmrHandler, 'd');
LOS_TIMER_DEF (tmr5, 80, LOS_SWTMR_MODE_PERIOD, tmrHandler, 'e');
#endif

static void create_tmr (void)
{
    UINT16 tmr1, tmr2, tmr3, tmr4, tmr5;

#if (LOSCFG_STATIC_TIMER == YES)
    LOS_TIMER_INIT (tmr1, &tmr1);
    LOS_TIMER_INIT (tmr2, &tmr2);
    LOS_TIMER_INIT (tmr3, &tmr3);
    LOS_TIMER_INIT (tmr4, &tmr4);
    LOS_TIMER_INIT (tmr5, &tmr5);
#else
    LOS_SwtmrCreate (10, 1, (SWTMR_PROC_FUNC) tmrHandler, &tmr1, (UINT32) 'a');
    LOS_SwtmrCreate (5,  1, (SWTMR_PROC_FUNC) tmrHandler, &tmr2, (UINT32) 'b');
    LOS_SwtmrCreate (20, 1, (SWTMR_PROC_FUNC) tmrHandler, &tmr3, (UINT32) 'c');
    LOS_SwtmrCreate (40, 1, (SWTMR_PROC_FUNC) tmrHandler, &tmr4, (UINT32) 'd');
    LOS_SwtmrCreate (80, 1, (SWTMR_PROC_FUNC) tmrHandler, &tmr5, (UINT32) 'e');
#endif

    LOS_SwtmrStart (tmr1);
    LOS_SwtmrStart (tmr2);
    LOS_SwtmrStart (tmr3);
    LOS_SwtmrStart (tmr4);
    LOS_SwtmrStart (tmr5);
}

#if (LOSCFG_STATIC_SEM == YES)
LOS_SEM_DEF (sem1, 0);
LOS_SEM_DEF (sem2, 0);
LOS_BIN_SEM_DEF (sem3, 0);
#endif

static void create_sem (void)
{
#if (LOSCFG_STATIC_SEM == NO)
    LOS_SemCreate (0, &semHandler1);
    LOS_SemCreate (0, &semHandler2);
    LOS_SemCreate (0, &semHandler3);
#else
    LOS_SEM_INIT (sem1, &semHandler1);
    LOS_SEM_INIT (sem2, &semHandler2);
    LOS_BIN_SEM_INIT (sem3, &semHandler3);
#endif
}

/**
 * @brief Function for application main entry.
 */
int main(void)
{
    HardWare_Init();

    if (LOS_KernelInit() != LOS_OK)
    {
        return LOS_NOK;
    }

    creat_tasks();

    create_tmr();

    create_sem();

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
    (void) x;
}

int fputc(int ch, FILE *f)
{
    simple_uart_put ((uint8_t) ch);
    return ch;
}

/**
 *@}
 **/
