/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: hal platform header
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
/* ----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 * --------------------------------------------------------------------------- */

#ifndef PLATFORM_HAL_PLATFORM_INTS_H
#define PLATFORM_HAL_PLATFORM_INTS_H

#include "stm32f429xx.h"
#include"los_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * Maximum number of supported hardware devices that generate hardware interrupts.
 * The maximum number of hardware devices that generate hardware interrupts is 128.
 */
#define OS_HWI_MAX_NUM                  240

/**
 * Maximum interrupt number.
 */
#define OS_HWI_MAX                      ((OS_HWI_MAX_NUM) - 1)

/**
 * Minimum interrupt number.
 */
#define OS_HWI_MIN                      0

/**
 * Maximum usable interrupt number.
 */
#define OS_USER_HWI_MAX                 OS_HWI_MAX

/**
 * Minimum usable interrupt number.
 */
#define OS_USER_HWI_MIN                 OS_HWI_MIN

#define OS_TICK_INT_NUM     (SysTick_IRQn + OS_SYS_VECTOR_CNT)

#define IO_ADDRESS(x)       (x)

#define HAL_READ_UINT8(addr, data)  READ_UINT8(data, addr)

#define HAL_WRITE_UINT8(addr, data) WRITE_UINT8(data, addr)

#define HAL_READ_UINT32(addr, data) READ_UINT32(data, addr)

#define HAL_WRITE_UINT32(addr, data) WRITE_UINT32(data, addr)

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif // PLATFORM_HAL_PLATFORM_INTS_H
