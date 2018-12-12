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
 * @file     pinmux.h
 * @brief    Header file for the pinmux
 * @version  V1.0
 * @date     23. August 2017
 ******************************************************************************/
#ifndef CH2201_PINMUX_H
#define CH2201_PINMUX_H

#include <stdint.h>
#include "pin_name.h"

void ioreuse_initial(void);
int32_t drv_pinmux_config(pin_name_e pin, pin_func_e pin_func);

#define GPIO_SET_BIT16 0x00010000
#define GPIO_SET_BIT17 0x00020000


/******************************************************************************
 * CH2201 gpio control and gpio reuse function
 * selecting regester adddress
 ******************************************************************************/

#define CH2201_GIPO0_PORTCTL_REG 0x50006008
#define CH2201_GIPO1_PORTCTL_REG 0x50009008
#define CH2201_IOMUX0L_REG       0x50006100
#define CH2201_IOMUX0H_REG       0x50006104
#define CH2201_IOMUX1L_REG       0x50006108


/*************basic gpio reuse v1.0********************************************
 * UART1(PA16,PA17) for bootrom
 * UART1(PA20,PA21) for console
 ******************************************************************************/
#define GPIO0_REUSE_DIS               (GPIO_SET_BIT16 | GPIO_SET_BIT17)

#endif /* HOBBIT_PINMUX_H */

