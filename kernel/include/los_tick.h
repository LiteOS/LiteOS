/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Tick
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
 * Number of milliseconds in one second.
 */
#define OS_SYS_MS_PER_SECOND   1000

/**
 * @ingroup los_tick
 * Number of microseconds in one second.
 */
#define OS_SYS_US_PER_SECOND   1000000

/**
 * @ingroup los_tick
 * Number of nanoseconds in one second.
 */
#define OS_SYS_NS_PER_SECOND   1000000000

/**
 * @ingroup los_tick
 * Number of microseconds in one milliseconds.
 */
#define OS_SYS_US_PER_MS        1000

/**
 * @ingroup los_tick
 * Number of nanoseconds in one milliseconds.
 */
#define OS_SYS_NS_PER_MS        1000000

/**
 * @ingroup los_tick
 * Number of nanoseconds in one microsecond.
 */
#define OS_SYS_NS_PER_US        1000

/**
 * @ingroup los_tick
 * Tick error code: The Tick configuration is incorrect.
 *
 * Value: 0x02000400.
 *
 * Solution: Change values of the #OS_SYS_CLOCK and #LOSCFG_BASE_CORE_TICK_PER_SECOND
 * in los_config.h.
 */
#define LOS_ERRNO_TICK_CFG_INVALID       LOS_ERRNO_OS_ERROR(LOS_MOD_TICK, 0x00)

/**
 * @ingroup los_tick
 * Tick error code: This error code is not in use temporarily.
 *
 * Value: 0x02000401
 */
#define LOS_ERRNO_TICK_NO_HWTIMER        LOS_ERRNO_OS_ERROR(LOS_MOD_TICK, 0x01)

/**
 * @ingroup los_tick
 * Tick error code: This error code is not in use temporarily.
 *
 * Value: 0x02000402
 */
#define LOS_ERRNO_TICK_PER_SEC_TOO_SMALL LOS_ERRNO_OS_ERROR(LOS_MOD_TICK, 0x02)

/**
 * @ingroup los_config
 * system clock
 */
extern UINT32 g_sysClock;

/**
 * @ingroup los_config
 * get system clock
 */
#define GET_SYS_CLOCK()        (g_sysClock)

/**
 * @ingroup los_config
 * set system clock
 */
#define SET_SYS_CLOCK(clock)   (g_sysClock = (clock))

/**
 * @ingroup los_config
 * ticks per second
 */
extern UINT32 g_tickPerSecond;

/**
 * @ingroup los_typedef
 * system time structure.
 */
typedef struct tagSysTime {
    UINT16 uwYear;   /**< year, the value range is 1970 ~ 2038 or 1970 ~ 2100 */
    UINT8 ucMonth;   /**< month, the value range is 1 - 12 */
    UINT8 ucDay;     /**< day, the value range is 1 - 31 */
    UINT8 ucHour;    /**< hour, the value range is 0 - 23 */
    UINT8 ucMinute;  /**< minute, the value range is 0 - 59 */
    UINT8 ucSecond;  /**< second, the value range is 0 - 59 */
    UINT8 ucWeek;    /**< week, the value range is 0 - 6 */
} SYS_TIME_S;

/**
 * @ingroup los_tick
 * @brief Obtain the number of Ticks.
 *
 * @par Description:
 * This API is used to obtain the number of Ticks since the system startup.
 * @attention
 * None.
 *
 * @param  None
 *
 * @retval #UINT64 The number of Ticks.
 * @par Dependency:
 * <ul><li>los_tick.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_GetCpuCycle
 * @since Huawei LiteOS V100R001C00
 */
extern UINT64 LOS_TickCountGet(VOID);

/**
 * @ingroup los_tick
 * @brief Obtain the number of cycles in one tick.
 *
 * @par Description:
 * This API is used to obtain the number of cycles in one tick.
 * @attention
 * None.
 *
 * @param  None.
 *
 * @retval #UINT32 Number of cycles in one tick.
 * @par Dependency:
 * <ul><li>los_tick.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_CyclePerTickGet(VOID);

/**
 * @ingroup los_tick
 * @brief Convert Ticks to milliseconds.
 *
 * @par Description:
 * This API is used to convert Ticks to milliseconds.
 * @attention
 * The number of milliseconds obtained through the conversion is 32-bit.
 *
 * @param  tick  [IN] Number of Ticks. The value range is (0, #OS_SYS_CLOCK).
 *
 * @retval #UINT32 Milliseconds obtained through the conversion.
 * @par  Dependency:
 * <ul><li>los_tick.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_MS2Tick
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_Tick2MS(UINT32 tick);

/**
 * @ingroup los_tick
 * @brief Convert milliseconds to Ticks.
 *
 * @par Description:
 * This API is used to convert milliseconds to Ticks.
 * @attention
 * If the parameter passed in is equal to 0xFFFFFFFF, the retval is 0xFFFFFFFF. Pay attention to
 * the value to be converted because data may overflows.
 *
 * @param  millisec  [IN] The milliseconds need to be converted to Ticks.
 *
 * @retval #UINT32 Number of Ticks obtained through the conversion.
 * @par Dependency:
 * <ul><li>los_tick.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_Tick2MS
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_MS2Tick(UINT32 millisec);

/**
 * @ingroup  los_tick
 * @brief Obtain system cycle count.
 *
 * @par Description:
 * This API is used to obtain system cycle count since the system startup.
 *
 * @attention
 * <ul>
 * <li>This count is determined by the tick source.</li>
 * <li>Parameters of this interface are pointers, it should be a correct value. Otherwise, the
 * system may be abnormal.</li>
 * </ul>
 *
 * @param  puwCntHi    [OUT] Type  #UINT32 Pointer to the higher 32bit of cycles to be obtained.
 * @param  puwCntLo    [OUT] Type  #UINT32 Pointer to the lower 32bit of cycles to be obtained.
 *
 * @retval None.
 *
 * @par Dependency:
 * <ul><li>los_tick.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_TickCountGet
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
 * @param  usecs  microsecond needs to delay.
 *
 * @retval None.
 *
 * @par Dependency:
 * <ul><li>los_tick.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_Mdelay
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
 * @param  msecs  millisecond needs to delay.
 *
 * @retval None.
 *
 * @par Dependency:
 * <ul><li>los_tick.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_Udelay
 * @since Huawei LiteOS V100R001C00
 */
extern VOID LOS_Mdelay(UINT32 msecs);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_TICK_H */
