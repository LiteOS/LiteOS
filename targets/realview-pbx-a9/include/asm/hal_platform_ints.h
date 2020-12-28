/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Platform Interrupt HeadFile
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


#ifndef _PLATFORM_HAL_PLATFORM_INTS_H
#define _PLATFORM_HAL_PLATFORM_INTS_H

#include"los_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * Maximum number of supported hardware devices that generate hardware interrupts.
 * The maximum number of hardware devices that generate hardware interrupts supported by hi3516a is 96.
 */
#define OS_HWI_MAX_NUM               LOSCFG_PLATFORM_HWI_LIMIT

/**
 * Maximum interrupt number.
 */

#define OS_HWI_MAX                   ((OS_HWI_MAX_NUM) - 1)

/**
 * Minimum interrupt number.
 */

#define OS_HWI_MIN                   0

/**
 * Maximum usable interrupt number.
 */

#define OS_USER_HWI_MAX              OS_HWI_MAX

/**
 * Minimum usable interrupt number.
 */

#define OS_USER_HWI_MIN              0

#define OS_TICK_INT_NUM              PRVTIMER_INT_NUM

/* interrupts */
#define PRVTIMER_INT_NUM             29

#define NUM_HAL_INTERRUPT_TIMER0     36
#define NUM_HAL_INTERRUPT_TIMER1     37
#define NUM_HAL_INTERRUPT_TIMER2     73
#define NUM_HAL_INTERRUPT_TIMER3     34
#define UART0_INT_NUM                44
#define UART1_INT_NUM                45
#define KEYBOARD_INT_NUM             52

#define NUM_HAL_INTERRUPT_UART       UART0_INT_NUM
#define NUM_HAL_INTERRUPT_HRTIMER    NUM_HAL_INTERRUPT_TIMER0

#define IO_ADDRESS(x)                (x)

#define HAL_READ_UINT8(addr, data)   READ_UINT8(data, addr)

#define HAL_WRITE_UINT8(addr, data)  WRITE_UINT8(data, addr)

#define HAL_READ_UINT32(addr, data)  READ_UINT32(data, addr)

#define HAL_WRITE_UINT32(addr, data) WRITE_UINT32(data, addr)

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif // _PLATFORM_HAL_PLATFORM_INTS_H
