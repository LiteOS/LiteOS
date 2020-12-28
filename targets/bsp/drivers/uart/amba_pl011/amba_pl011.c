/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: LiteOS amba_pl011 uart driver
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

#include "amba_pl011.h"
#include "asm/platform.h"
#include "los_hwi.h"
#include "los_spinlock.h"
#include "los_event.h"
#include "los_task_pri.h"
#include "los_queue.h"

#ifdef LOSCFG_SERIAL_OUTPUT_ENABLE
UINT32 g_uart_fputc_en = 1;
#else
UINT32 g_uart_fputc_en = 0;
#endif

#define REG32(addr)             ((volatile UINT32 *)(UINTPTR)(addr))
#define UARTREG(base, reg)      (*REG32((base) + (reg)))
#define UART_FR_TXFF            (0x1U << 5)
#define UART_QUEUE_SIZE         64
#define UART_QUEUE_BUF_MAX_LEN  1
#define UART_QUEUE_REC_DELAY    5

STATIC UINT32 g_uartQueue;

STATIC VOID UartPutcReg(UINTPTR base, CHAR c)
{
    /* Spin while fifo is full */
    while (UARTREG(base, UART_FR) & UART_FR_TXFF) {}
    UARTREG(base, UART_DR) = c;
}

INT32 uart_putc(CHAR c)
{
    UartPutcReg(UART_REG_BASE, c);
    return 1;
}

STATIC CHAR UartFputc(CHAR c, VOID *f)
{
    (VOID)f;
    if (g_uart_fputc_en == 1) {
        if (c == '\n') {
            (VOID)uart_putc('\r');
        }
        return (uart_putc(c));
    } else {
        return 0;
    }
}

LITE_OS_SEC_BSS STATIC SPIN_LOCK_INIT(g_uartOutputSpin);

STATIC VOID UartPutStr(UINTPTR base, const CHAR *s, UINT32 len)
{
    UINT32 i;

    for (i = 0; i < len; i++) {
        /*
         * Only system uart output needs to add extra '\r' to improve
         * the compatibility on win.
         */
        if ((*(s + i) == '\n') && (base == UART_REG_BASE)) {
            UartPutcReg(base, '\r');
        }
        UartPutcReg(base, *(s + i));
    }
}

STATIC UINT32 UartPutsReg(UINTPTR base, const CHAR *s, UINT32 len, BOOL isLock)
{
    UINT32 intSave;

    if (g_uart_fputc_en == 0) {
        return 0;
    }

    if (isLock) {
        LOS_SpinLockSave(&g_uartOutputSpin, &intSave);
        UartPutStr(base, s, len);
        LOS_SpinUnlockRestore(&g_uartOutputSpin, intSave);
    } else {
        UartPutStr(base, s, len);
    }

    return len;
}

VOID UartPuts(const CHAR *s, UINT32 len, BOOL isLock)
{
    (VOID)UartPutsReg(UART_REG_BASE, s, len, isLock);
}

UINT8 uart_getc(void)
{
    UINT8 ch = 0;
    UINTPTR base = UART_REG_BASE;

    UINT32 status;
    READ_UINT32(status, base + UART_FR);
    /* Wait until there is data in the FIFO */
    while (status & 0x10) {
        READ_UINT32(status, base + UART_FR);
        (VOID)LOS_TaskDelay(1);
    }

    READ_UINT8(ch, base + UART_DR);

    return ch;
}

STATIC INT32 UartFputs(const CHAR *s, UINTPTR len, VOID *state)
{
    (VOID)state;
    UINTPTR i;

    for (i = 0; i < len; i++) {
        if (*(s + i) != '\0') {
            if (*(s + i) == '\n') {
                (VOID)UartFputc('\r', NULL);
            }

            (VOID)UartFputc(*(s + i), NULL);
        }
    }

    return (INT32)len;
}

UINT8 uart_read(VOID)
{
    UINT8 rec = 0;
    UINT32 ret;
    UINT32 len;
    len = UART_QUEUE_BUF_MAX_LEN;
    ret = LOS_QueueReadCopy(g_uartQueue, &rec, &len, LOS_WAIT_FOREVER);
    if (ret == LOS_OK) {
        return rec;
    }
    return rec;
}

INT32 uart_write(const CHAR *buf, INT32 len, INT32 timeout)
{
    UartFputs(buf, len, &timeout);
    return len;
}

STATIC VOID UartHandler(VOID)
{
    UINT8 ch = UARTREG(UART_REG_BASE, UART_DR);
    (VOID)LOS_QueueWriteCopy(g_uartQueue, &ch, sizeof(UINT8), 0);
}

VOID uart_early_init(VOID)
{
    /* enable uart transmit */
    UARTREG(UART_REG_BASE, UART_CR) = (1 << 8) | (1 << 0);
}

INT32 ShellQueueCreat(VOID)
{
    return LOS_QueueCreate("uartQueue", UART_QUEUE_SIZE, &g_uartQueue, 0, UART_QUEUE_BUF_MAX_LEN);
}

INT32 uart_hwiCreate(VOID)
{
    UINT32 ret;
    /* uart interrupt priority should be the highest in interrupt preemption mode */
    ret = LOS_HwiCreate(NUM_HAL_INTERRUPT_UART, 0, 0, (HWI_PROC_FUNC)UartHandler, NULL);
    if (ret != LOS_OK) {
        PRINT_ERR("%s,%d, uart interrupt created error:%x\n", __FUNCTION__, __LINE__, ret);
    } else {
        /* clear all irqs */
        UARTREG(UART_REG_BASE, UART_ICR) = 0x3ff;

        /* set fifo trigger level */
        UARTREG(UART_REG_BASE, UART_IFLS) = 0;

        /* enable rx interrupt */
        UARTREG(UART_REG_BASE, UART_IMSC) = (1 << 4 | 1 << 6);

        /* enable receive */
        UARTREG(UART_REG_BASE, UART_CR) |= (1 << 9);

        HalIrqUnmask(NUM_HAL_INTERRUPT_UART);
    }
    return ret;
}

VOID uart_init(VOID) {}
