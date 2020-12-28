/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Get Char From UART
 * Author: Huawei LiteOS Team
 * Create: 2020-08-06
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

#include "string.h"
#include "los_hwi.h"
#include "uart.h"

#ifdef LOSCFG_DRIVERS_UART
/* Keyboard input buffer size */
#define INPUT_BUF_SIZE 64
extern int uart_putc(char c);

int do_uart_getc(void)
{
    int ret;
    static char *buf = NULL;
    static int num = 0;
    static int pos = 0;
    char ch;
    HalIrqMask(NUM_HAL_INTERRUPT_UART);
    if (buf == NULL) {
        buf = (char *)malloc(INPUT_BUF_SIZE);
        if (buf == NULL) {
            HalIrqUnmask(NUM_HAL_INTERRUPT_UART);
            return EOF;
        }
        pos = 0;
    }

    /* refill buf */
    if (num == 0) {
        do {
            ch = (char)uart_getc();
            if (ch == '\r' || ch == '\n') {
                (void)uart_putc('\r');
                (void)uart_putc('\n');
                ch = '\n';
            } else {
                (void)uart_putc(ch);
            }
            if (num <= INPUT_BUF_SIZE - 1) {
                buf[num] = ch;
                num++;
            }
        } while (ch != '\n');
        pos = 0;
    }

    num--;
    ret = buf[pos++];
    HalIrqUnmask(NUM_HAL_INTERRUPT_UART);
    return ret;
}
#endif