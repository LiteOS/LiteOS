/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Uart Module Implementation
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
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

#include <ctype.h>
#include <los_hwi.h>
#include <los_sem.h>
#include "usart.h"
#include "los_queue.h"

#define UART_QUEUE_SIZE        64
#define UART_QUEUE_BUF_MAX_LEN 1
#define UART_QUEUE_REC_DELAY   5

STATIC UINT32 g_uartQueue;

INT32 uart_putc(CHAR c)
{
    return uart_write(&c, 1, 0);
}

UINT8 uart_getc(VOID)
{
    UINT8 ch = 0;
    HAL_UART_Receive(&huart1, &ch, sizeof(UINT8), 0);
    (VOID)LOS_QueueWriteCopy(g_uartQueue, &ch, sizeof(UINT8), 0);
    return ch;
}

STATIC VOID UartHandler(VOID)
{
    (VOID)uart_getc();
}

INT32 ShellQueueCreat(VOID)
{
    return LOS_QueueCreate("uartQueue", UART_QUEUE_SIZE, &g_uartQueue, 0, UART_QUEUE_BUF_MAX_LEN);;
}

INT32 uart_hwiCreate(VOID)
{
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    __HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_TC);
    (VOID)LOS_HwiCreate(NUM_HAL_INTERRUPT_UART, 0, 0, UartHandler, NULL);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
    return LOS_OK;
}

UINT8 uart_read(VOID)
{
    UINT8 rec = 0;
    UINT32 ret;
    UINT32 len;
    len = UART_QUEUE_BUF_MAX_LEN;
    ret = LOS_QueueReadCopy(g_uartQueue, &rec, &len, LOS_WAIT_FOREVER);
    if (ret == LOS_OK) {
        LOS_TaskDelay(UART_QUEUE_REC_DELAY);
        return rec;
    }
    return rec;
}

INT32 uart_write(const CHAR *buf, INT32 len, INT32 timeout)
{
#ifdef LOSCFG_PLATFORM_STM32F072_Nucleo
    (VOID)HAL_UART_Transmit(&huart2, (UINT8 *)buf, len, DEFAULT_TIMEOUT);
#else
    (VOID)HAL_UART_Transmit(&huart1, (UINT8 *)buf, len, DEFAULT_TIMEOUT);
#endif
    return len;
}

VOID UartPuts(const CHAR *s, UINT32 len, BOOL isLock)
{
    UINT32 i;
    for (i = 0; i < len; i++) {
        if (*(s + i) == '\n') {
            uart_putc('\r');
        }
        uart_putc(*(s + i));
    }
}

VOID uart_init(VOID) {}
