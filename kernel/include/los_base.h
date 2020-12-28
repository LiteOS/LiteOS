/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Basic definitions
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

/**
 * @defgroup kernel Kernel
 * @defgroup los_base Basic definitions
 * @ingroup kernel
 */

#ifndef _LOS_BASE_H
#define _LOS_BASE_H

#include "los_builddef.h"
#include "los_typedef.h"
#include "los_config.h"
#include "los_printf.h"
#include "los_list.h"
#include "los_err.h"
#include "los_errno.h"
#include "los_hw.h"
#include "securec.h"
#ifdef LOSCFG_LIB_LIBCMINI
#include "stdarg.h"
#endif
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


#define SIZE(a) (a)

/**
 * @ingroup los_base
 * @brief Assertion.
 *
 * @par Description:
 * When the program runs to the assertion position, the corresponding expression should
 * be true. If the expression is not true, the program stops running and gives an error
 * message. The LOS_ASSERT_COND function is same with #LOS_ASSERT function.
 *
 * @attention
 * The function is effective only LOSCFG_DEBUG_VERSION is defined.
 *
 * @param expression  [IN] The judgement expression of the assertion.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_base.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_ASSERT
 * @since Huawei LiteOS V100R001C00
 */
#define LOS_ASSERT_COND(expression) LOS_ASSERT(expression)

/**
 * @ingroup los_base
 * Define the timeout interval as LOS_NO_WAIT.
 */
#define LOS_NO_WAIT                                 0

/**
 * @ingroup los_base
 * Define the timeout interval as LOS_WAIT_FOREVER.
 */
#define LOS_WAIT_FOREVER                            0xFFFFFFFF

/**
 * @ingroup los_base
 * @brief Align the value (addr) by some bytes (boundary).
 *
 * @par Description:
 * Align the beginning of the object with the base address, with boundary bytes being
 * the smallest unit of alignment. The ALIGN function is same with #LOS_Align function.
 *
 * @attention
 * <ul>
 * <li>the value of boundary usually is 4,8,16,32.</li>
 * <li>users should avoid overflows due to alignment.</li>
 * </ul>
 *
 * @param addr     [IN] Type #UINTPTR The address you want to align.
 * @param boundary [IN] Type #UINT32 The alignment size.
 *
 * @retval #UINTPTR The address which have been aligned.
 * @par Dependency:
 * <ul><li>los_base.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_Align | TRUNCATE
 * @since Huawei LiteOS V100R001C00
 */
#ifndef ALIGN
#define ALIGN(addr, boundary)                       LOS_Align(addr, boundary)
#endif

/**
 * @ingroup los_base
 * @brief Align the value (addr) by some bytes (size).
 *
 * @par Description:
 * Align the tail of the object with the base address, with size bytes being the
 * smallest unit of alignment.
 *
 * @attention
 * <ul>
 * <li>the value of size usually is 4,8,16,32.</li>
 * <li>users should avoid overflows due to alignment.</li>
 * </ul>
 *
 * @param addr     [IN] Type #UINTPTR The address you want to align.
 * @param size     [IN] Type #UINT32  The alignment size.
 *
 * @retval #UINTPTR The address which have been aligned.
 * @par Dependency:
 * <ul><li>los_base.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_Align | ALIGN
 * @since Huawei LiteOS V100R001C00
 */
#define TRUNCATE(addr, size)                        ((UINTPTR)(addr) & ~((size) - 1))

/**
 * Read a UINT8 value from addr and stroed in value.
 */
#define READ_UINT8(value, addr)                     ({ (value) = *((volatile UINT8 *)((UINTPTR)(addr))); dsb(); })

/**
 * Read a UINT16 value from addr and stroed in addr.
 */
#define READ_UINT16(value, addr)                    ({ (value) = *((volatile UINT16 *)((UINTPTR)(addr))); dsb(); })

/**
 * Read a UINT32 value from addr and stroed in value.
 */
#define READ_UINT32(value, addr)                    ({ (value) = *((volatile UINT32 *)((UINTPTR)(addr))); dsb(); })

#ifdef __LP64__
/**
 * Read a UINT64 value from addr and stroed in value.
 */
#define READ_UINT64(value, addr)                    ({ (value) = *((volatile UINT64 *)((UINTPTR)(addr))); dsb(); })
#endif

/**
 * Write a UINT8 value to addr.
 */
#define WRITE_UINT8(value, addr)                    ({ dsb(); *((volatile UINT8 *)((UINTPTR)(addr))) = (value); })

/**
 * Write a UINT16 value to addr.
 */
#define WRITE_UINT16(value, addr)                   ({ dsb(); *((volatile UINT16 *)((UINTPTR)(addr))) = (value); })

/**
 * Write a UINT32 value to addr.
 */
#define WRITE_UINT32(value, addr)                   ({ dsb(); *((volatile UINT32 *)((UINTPTR)(addr))) = (value); })

#ifdef __LP64__
/**
 * Write a UINT64 addr to addr.
 */
#define WRITE_UINT64(value, addr)                   ({ dsb(); *((volatile UINT64 *)((UINTPTR)(addr))) = (value); })
#endif

/**
 * Get a UINT8 value from addr.
 */
#define GET_UINT8(addr)                             ({ UINT8 r = *((volatile UINT8 *)((UINTPTR)(addr))); dsb(); r; })

/**
 * Get a UINT16 value from addr.
 */
#define GET_UINT16(addr)                            ({ UINT16 r = *((volatile UINT16 *)((UINTPTR)(addr))); dsb(); r; })

/**
 * Get a UINT32 value from addr.
 */
#define GET_UINT32(addr)                            ({ UINT32 r = *((volatile UINT32 *)((UINTPTR)(addr))); dsb(); r; })

#ifdef __LP64__
/**
 * Get a UINT64 value from addr.
 */
#define GET_UINT64(addr)                            ({ UINT64 r = *((volatile UINT64 *)((UINTPTR)(addr))); dsb(); r; })
#endif

/**
 * @ingroup los_base
 * @brief Assertion.
 *
 * @par Description:
 * When the program runs to the assertion position, the corresponding expression should
 * be true. If the expression is not true, the program stops running and gives an error
 * message. The function is same with #LOS_ASSERT_COND function.
 *
 * @attention
 * The function is effective only LOSCFG_DEBUG_VERSION is defined.
 *
 * @param judge [IN] The judgement expression of the assertion.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_base.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_ASSERT_COND
 * @since Huawei LiteOS V100R001C00
 */
#ifdef LOSCFG_DEBUG_VERSION
#define LOS_ASSERT(judge) do {                                                     \
    if ((UINT32)(judge) == 0) {                                                    \
        (VOID)LOS_IntLock();                                                       \
        PRINT_ERR("ASSERT ERROR! %s, %d, %s\n", __FILE__, __LINE__, __FUNCTION__); \
        while (1) {}                                                               \
    }                                                                              \
} while (0)
#else
#define LOS_ASSERT(judge)
#endif

#define STATIC_ASSERT _Static_assert

/**
 * @ingroup los_base
 * @brief Align the address (addr) by some bytes (boundary) you specify.
 *
 * @par Description:
 * This API is used to align the address (addr) by some bytes (boundary) you specify.
 *
 * @attention
 * <ul>
 * <li>the value of boundary usually is 4,8,16,32.</li>
 * <li>users should avoid overflows due to alignment.</li>
 * </ul>
 *
 * @param addr     [IN] Type #UINTPTR The address you want to align.
 * @param boundary [IN] Type #UINT32  The alignment size.
 *
 * @retval #UINTPTR The address which have been aligned.
 * @par Dependency:
 * <ul><li>los_base.h: the header file that contains the API declaration.</li></ul>
 * @see ALIGN | TRUNCATE
 * @since Huawei LiteOS V100R001C00
 */
extern UINTPTR LOS_Align(UINTPTR addr, UINT32 boundary);

/**
 * @ingroup los_base
 * @brief Sleep the current task.
 *
 * @par Description:
 * This API is used to delay the execution of the current task. The task is able to be scheduled after it is delayed
 * for a specified milliseconds.
 *
 * @attention
 * <ul>
 * <li>The task fails to be delayed if it is being delayed during interrupt processing or it is locked.</li>
 * <li>If 0 is passed in and the task scheduling is not locked, execute the next task in the queue of tasks with the
 * priority of the current task.
 * If no ready task with the priority of the current task is available, the task scheduling will not occur, and the
 * current task continues to be executed.</li>
 * <li>The parameter passed in can not be equal to LOS_WAIT_FOREVER(0xFFFFFFFF).
 * If that happens, the task will not sleep 0xFFFFFFFF milliseconds or sleep forever but sleep 0xFFFFFFFF Ticks.</li>
 * </ul>
 *
 * @param msecs [IN] Type #UINT32 Milliseconds for which the task is delayed.
 *
 * @retval None
 * @par Dependency:
 * <ul><li>los_base.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_Mdelay | LOS_TaskDelay
 * @since Huawei LiteOS V100R001C00
 */
extern VOID LOS_Msleep(UINT32 msecs);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_BASE_H */
