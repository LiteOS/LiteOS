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
 * @file     pinmux.c
 * @brief    source file for the pinmux
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/

#include <csi_config.h>
#include <stdint.h>
#include "pinmux.h"
#include "pin_name.h"
#include <drv_gpio.h>

#define readl(addr) \
    ({ unsigned int __v = (*(volatile unsigned int *) (addr)); __v; })

#define writel(b,addr) (void)((*(volatile unsigned int *) (addr)) = (b))

/*******************************************************************************
 * function: ioreuse_initial
 *
 * description:
 *   initial hobbit_pinmux
 *******************************************************************************/

void ioreuse_initial(void)
{
    unsigned int value;

    value = readl(CH2201_GIPO0_PORTCTL_REG);
    value &= ~(GPIO0_REUSE_DIS);
    writel(value, CH2201_GIPO0_PORTCTL_REG);

}

int32_t drv_pinmux_config(pin_name_e pin, pin_func_e pin_func)
{
    unsigned int val = 0;
    unsigned int reg_val = 0;

    uint8_t offset;

    if (pin_func > 3) {
        if (pin <= PB3) {
            if (pin <= PA5) {
                offset = pin;
                /* gpio data source select */
                val = readl(CH2201_GIPO0_PORTCTL_REG);
                val &= ~(1 << offset);
                writel(val, CH2201_GIPO0_PORTCTL_REG);
                return 0;
            } else if (pin >= PB0) {
                offset = pin - 6;
                /* gpio data source select */
                val = readl(CH2201_GIPO1_PORTCTL_REG);
                val &= ~(1 << offset);
                writel(val, CH2201_GIPO1_PORTCTL_REG);
                return 0;
            }
        }

        if ((pin >= PA6) && (pin <= PA27)) {
            offset = pin - 4;
            /* gpio data source select */
            val = readl(CH2201_GIPO0_PORTCTL_REG);
            val &= ~(1 << offset);
            writel(val, CH2201_GIPO0_PORTCTL_REG);
            return 0;
        }

        return -1;
    }

    if ((pin >= PA6) && (pin <= PA27)) {
        offset = pin - 4;

        /* gpio data source select */
        val = readl(CH2201_GIPO0_PORTCTL_REG);
        val |= (1 << offset);
        writel(val, CH2201_GIPO0_PORTCTL_REG);

        if (pin <= PA11) {
            offset = pin;
            reg_val = (0x3 << (offset * 2));
            /* reuse function select */
            val = readl(CH2201_IOMUX0L_REG);
            val &= ~(reg_val);
            val |= (pin_func << (2 * offset));
            writel(val, CH2201_IOMUX0L_REG);
            return 0;
        } else {
            offset = pin - 16;
            reg_val = (0x3 << (offset * 2));
            /* reuse function select */
            val = readl(CH2201_IOMUX0H_REG);
            val &= ~(reg_val);
            val |= (pin_func << (2 * offset));
            writel(val, CH2201_IOMUX0H_REG);
            return 0;
        }
    }

    if ((pin >= PA0) && (pin <= PB3)) {
        if (pin >= PB0) {
            offset = pin - 6;
            val = readl(CH2201_GIPO1_PORTCTL_REG);
            val |= (1 << offset);
            writel(val, CH2201_GIPO1_PORTCTL_REG);

            offset = pin;
            reg_val = (0x3 << (offset * 2));
            /* reuse function select */
            val = readl(CH2201_IOMUX0L_REG);
            val &= ~(reg_val);
            val |= (pin_func << (2 * offset));
            writel(val, CH2201_IOMUX0L_REG);
            return 0;
        }

        if (pin <= PA5) {
            offset = pin;
            /* gpio data source select */
            val = readl(CH2201_GIPO0_PORTCTL_REG);
            val |= (1 << offset);
            writel(val, CH2201_GIPO0_PORTCTL_REG);

            reg_val = (0x3 << (offset * 2));
            /* reuse function select */
            val = readl(CH2201_IOMUX0L_REG);
            val &= ~(reg_val);
            val |= (pin_func << (2 * offset));
            writel(val, CH2201_IOMUX0L_REG);
            return 0;
        }
    }

    if (pin > PA27) {
        offset = pin - PC0;
        reg_val = (0x3 << (offset * 2));
        val = readl(CH2201_IOMUX1L_REG);
        val &= ~(reg_val);
        val |= (pin_func << (2 * offset));
        writel(val, CH2201_IOMUX1L_REG);
        return 0;
    }

    return -1;
}
