/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Tick
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

/**
 * @defgroup los_tick Tick
 * @ingroup kernel
 */

#ifndef _LOS_TICK_H
#define _LOS_TICK_H

#include "los_err.h"
#include "los_errno.h"
#include "hal_timer.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup los_tick
 * Tick error code: The Tick configuration is incorrect.
 *
 * Value: 0x02000400
 *
 * Solution: Change values of the OS_SYS_CLOCK and LOSCFG_BASE_CORE_TICK_PER_SECOND system time configuration modules
 * in Los_config.h.
 */
#define LOS_ERRNO_TICK_CFG_INVALID       LOS_ERRNO_OS_ERROR(LOS_MOD_TICK, 0x00)

/**
 * @ingroup los_tick
 * Tick error code: This error code is not in use temporarily.
 *
 * Value: 0x02000401
 *
 * Solution: None.
 */
#define LOS_ERRNO_TICK_NO_HWTIMER        LOS_ERRNO_OS_ERROR(LOS_MOD_TICK, 0x01)

/**
 * @ingroup los_tick
 * Tick error code: The number of Ticks is too small.
 *
 * Value: 0x02000402
 *
 * Solution: Change values of the OS_SYS_CLOCK and LOSCFG_BASE_CORE_TICK_PER_SECOND system time configuration modules
 * according to the SysTick_Config function.
 */
#define LOS_ERRNO_TICK_PER_SEC_TOO_SMALL LOS_ERRNO_OS_ERROR(LOS_MOD_TICK, 0x02)

/**
 * @ingroup los_config
 * system clock
 */
extern UINT32 g_sysClock;

/**
 * @ingroup los_config
 * ticks per second
 */
extern UINT32 g_tickPerSecond;

/**
 * @ingroup  los_tick
 * @brief Obtain system cycle count.
 *
 * @par Description:
 * This API is used to obtain system cycle count.
 *
 * @attention
 * <ul>
 * <li> This count is determined by the tick source.</li>
 * </ul>
 *
 * @param  puwCntHi    [OUT] Type  #UINT32 Pointer to the higher 32bit of cycles to be obtained.
 * @param  puwCntLo    [OUT] Type  #UINT32 Pointer to the lower 32bit of cycles to be obtained.
 *
 * @retval None.
 *
 * @par Dependency:
 * <ul><li>los_tick.h: the header file that contains the API declaration.</li></ul>
 * @see
 * @since Huawei LiteOS V100R001C00
 */
extern VOID LOS_GetCpuCycle(UINT32 *puwCntHi, UINT32 *puwCntLo);

/**
 * @ingroup  los_tick
 * @brief Obtain system time in nanoseconds.
 *
 * @par Description:
 * This API is used to obtain system time in nanoseconds.
 *
 * @attention None.
 *
 * @param  None.
 *
 * @retval #UINT64  system time in nanoseconds.
 *
 * @par Dependency:
 * <ul><li>los_tick.h: the header file that contains the API declaration.</li></ul>
 * @see
 * @since Huawei LiteOS V100R001C00
 */
extern UINT64 LOS_CurrNanosec(VOID);

/**
 * @ingroup  los_tick
 * @brief spinning-delay in microsecond (us).
 *
 * @par Description:
 * This API is used to delay in microsecond.
 *
 * @attention None.
 *
 * @param  #UINT32  microsecond needs to delay.
 *
 * @retval None.
 *
 * @par Dependency:
 * <ul><li>los_tick.h: the header file that contains the API declaration.</li></ul>
 * @see
 * @since Huawei LiteOS V100R001C00
 */
extern VOID LOS_Udelay(UINT32 usecs);

/**
 * @ingroup  los_tick
 * @brief spinning-delay in millisecond (ms).
 *
 * @par Description:
 * This API is used to delay in millisecond.
 *
 * @attention None.
 *
 * @param  #UINT32  millisecond needs to delay.
 *
 * @retval None.
 *
 * @par Dependency:
 * <ul><li>los_tick.h: the header file that contains the API declaration.</li></ul>
 * @see
 * @since Huawei LiteOS V100R001C00
 */
extern VOID LOS_Mdelay(UINT32 usecs);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_TICK_H */
