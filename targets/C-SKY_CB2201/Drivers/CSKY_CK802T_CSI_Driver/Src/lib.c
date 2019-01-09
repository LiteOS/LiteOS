/*
 * Copyright (C) 2017 C-SKY Microsystems Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/******************************************************************************
 * @file     lib.c
 * @brief    source file for the lib
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/
#include <csi_config.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include "soc.h"
#include "csi_core.h"     //for test
#include "drv_usart.h"
#include "drv_timer.h"

extern uint32_t csi_coret_get_load(void);
extern uint32_t csi_coret_get_value(void);

extern int32_t csi_usart_putchar(usart_handle_t handle, uint8_t ch);
extern int32_t csi_usart_getchar(usart_handle_t handle, uint8_t *ch);

static void _mdelay(void)
{
    uint32_t load = csi_coret_get_load();
    uint32_t start = csi_coret_get_value();
    uint32_t cur;
    uint32_t cnt = (SYSTEM_CLOCK / 1000);

    while (1) {
        cur = csi_coret_get_value();

        if (start > cur) {
            if (start - cur >= cnt) {
                return;
            }
        } else {
            if (cur + load - start > cnt) {
                return;
            }
        }
    }
}

void mdelay(uint32_t ms)
{
    if (ms == 0) {
        return;
    }

    while (ms--) {
        _mdelay();
    }
}

static void _10udelay(void)
{
    uint32_t load = csi_coret_get_load();
    uint32_t start = csi_coret_get_value();
    uint32_t cur;
    uint32_t cnt = (SYSTEM_CLOCK / 1000 / 100);

    while (1) {
        cur = csi_coret_get_value();

        if (start > cur) {
            if (start - cur >= cnt) {
                return;
            }
        } else {
            if (cur + load - start > cnt) {
                return;
            }
        }
    }
}

void udelay(uint32_t us)
{
    us = (us / 10) - 1;

    if (us == 0) {
        return;
    }

    while (us--) {
        _10udelay();
    }
}
