/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Platform HeadFile
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

#ifndef _BOARD_DEFINES_H
#define _BOARD_DEFINES_H

#include "asm/hal_platform_ints.h"
#include "stdarg.h"

// type defines

#ifndef uint8_t
#define uint8_t unsigned char
#endif

#ifndef uint16_t
#define uint16_t unsigned short
#endif

#ifndef uint32_t
#define uint32_t unsigned int
#endif

#include "uart.h"
int do_uart_getc(void);

// ARM Realview-pbx-a9 board.

#define GIC_BASE_ADDR                   0x1F000000
#define GICD_OFFSET                     0x1000                          /* interrupt distributor offset */
#define GICC_OFFSET                     0x100                           /* CPU interface register offset */

// I/O devices
#define TIMER0_BASE_ADDR                0x10011000
#define TIMER0_LR                       (*((volatile u32 *)(UART_REG_BASE + 0x000)))
#define TIMER0_BR                       (*((volatile u32 *)(UART_REG_BASE + 0x032)))

#define PRVTIMER_BASE_ADDR              0x1F000600
#define GTIMER_BASE_ADDR                0x1F000200
#define TIMER_FREQ                      0x6000000

#define UART_REG_BASE                   0x10009000

#define KBD_BASE_ADDR                   0x10006000
#define KBD_CR                          (*((volatile u32 *)(KBD_BASE_ADDR + 0x000)))
#define KBD_DR                          (*((volatile u32 *)(KBD_BASE_ADDR + 0x008)))

#define LCD_BASE_ADDR                   0x10020000

/* status and system control registers */
#define SYS_FLAGSSET                    0x10000030

#define LINES                           4
#define N_SCAN                          64

#define BLUE                            0
#define GREEN                           1
#define RED                             2
#define CYAN                            3
#define YELLOW                          4
#define PURPLE                          5
#define WHITE                           6

extern char *tab;
extern int color;

extern void enable_scu(void);

#define TIMER0_REG_BASE                 0x10011000
#define TIMER1_REG_BASE                 0x10012000
#define TIMER2_REG_BASE                 0x10018000
#define TIMER3_REG_BASE                 0x10019000
#define HRTIMER_TIMER_REG_BASE          TIMER0_REG_BASE /* timer for hrtimer */
#define HRTIMER_TIMER_ENABLE            1u << 15

#define TIMER_LOAD                      0x0
#define TIMER_VALUE                     0x4
#define TIMER_CONTROL                   0x8
#define TIMER_INT_CLR                   0xc
#define TIMER_RIS                       0x10
#define TIMER_MIS                       0x14
#define TIMER_BGLOAD                    0x18

#endif // _BOARD_DEFINES_H
