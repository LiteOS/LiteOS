/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2017-2020. All rights reserved.
 * Description: LiteOS Performance Monitor Module Implementation
 * Author: Huawei LiteOS Team
 * Create: 2017-01-01
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
 * @defgroup los_perf Perf
 * @ingroup kernel
 */

#ifndef _LOS_PERF_H
#define _LOS_PERF_H

#include "los_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup los_perf
 * Perf max sample filter task number.
 */
#define PERF_MAX_FILTER_TSKS                32

/**
 * @ingroup los_perf
 * Perf max sample event counter's number.
 */
#define PERF_MAX_EVENT                      7

/**
 * @ingroup los_perf
 * Perf max backtrace depth.
 */
#define PERF_MAX_CALLCHAIN_DEPTH            10

/**
 * @ingroup los_perf
 * Perf sample data buffer's water mark 1/N.
 */
#define PERF_BUFFER_WATERMARK_ONE_N         2

/**
 * @ingroup los_perf
 * Perf state.
 */
enum PerfStatus {
    PERF_UNINIT,   /**< perf isn't inited */
    PERF_STARTED,  /**< perf is started */
    PERF_STOPED,   /**< perf is stopped */
};

/**
 * @ingroup los_perf
 * Define the type of the perf sample data buffer water mark hook function.
 *
 */
typedef VOID (*PERF_BUF_NOTIFY_HOOK)(VOID);

/**
 * @ingroup los_perf
 * Define the type of the perf sample data buffer flush hook function.
 *
 */
typedef VOID (*PERF_BUF_FLUSH_HOOK)(VOID *addr, UINT32 size);

/**
 * @ingroup los_perf
 * Perf error code: Bad status.
 *
 * Value: 0x02002000
 *
 * Solution: Follow the perf state machine.
 */
#define LOS_ERRNO_PERF_STATUS_INVALID        LOS_ERRNO_OS_ERROR(LOS_MOD_PERF, 0x00)

/**
 * @ingroup los_perf
 * Perf error code: Hardware pmu init failed.
 *
 * Value: 0x02002001
 *
 * Solution: Check the pmu hwi irq.
 */
#define LOS_ERRNO_PERF_HW_INIT_ERROR         LOS_ERRNO_OS_ERROR(LOS_MOD_PERF, 0x01)

/**
 * @ingroup los_perf
 * Perf error code: Hrtimer init failed for hrtimer timed pmu init.
 *
 * Value: 0x02002002
 *
 * Solution: Check the Hrtimer init.
 */
#define LOS_ERRNO_PERF_TIMED_INIT_ERROR      LOS_ERRNO_OS_ERROR(LOS_MOD_PERF, 0x02)

/**
 * @ingroup los_perf
 * Perf error code: Software pmu init failed.
 *
 * Value: 0x02002003
 *
 * Solution: Check the Perf software events init.
 */
#define LOS_ERRNO_PERF_SW_INIT_ERROR         LOS_ERRNO_OS_ERROR(LOS_MOD_PERF, 0x03)

/**
 * @ingroup los_perf
 * Perf error code: Perf buffer init failed.
 *
 * Value: 0x02002004
 *
 * Solution: Check the buffer init size.
 */
#define LOS_ERRNO_PERF_BUF_ERROR             LOS_ERRNO_OS_ERROR(LOS_MOD_PERF, 0x04)

/**
 * @ingroup los_perf
 * Perf error code: Perf pmu type error.
 *
 * Value: 0x02002005
 *
 * Solution: Check whether the corresponding pmu is enabled in the menuconfig.
 */
#define LOS_ERRNO_PERF_INVALID_PMU           LOS_ERRNO_OS_ERROR(LOS_MOD_PERF, 0x05)

/**
 * @ingroup los_perf
 * Perf error code: Perf pmu config error.
 *
 * Value: 0x02002006
 *
 * Solution: Check the config attr of event id and event period.
 */
#define LOS_ERRNO_PERF_PMU_CONFIG_ERROR      LOS_ERRNO_OS_ERROR(LOS_MOD_PERF, 0x06)

/**
 * @ingroup los_perf
 * Perf error code: Perf pmu config attr is null.
 *
 * Value: 0x02002007
 *
 * Solution: Check if the input params of attr is null.
 */
#define LOS_ERRNO_PERF_CONFIG_NULL      LOS_ERRNO_OS_ERROR(LOS_MOD_PERF, 0x07)

/**
 * @ingroup los_perf
 * Perf types
 */
enum PerfEventType {
    PERF_EVENT_TYPE_HW,      /**< boards common hw events */
    PERF_EVENT_TYPE_TIMED,   /**< hrtimer timed events */
    PERF_EVENT_TYPE_SW,      /**< software trace events */
    PERF_EVENT_TYPE_RAW,     /**< boards special hw events, see enum PmuEventType in corresponding arch headfile */

    PERF_EVENT_TYPE_MAX
};

/**
 * @ingroup los_perf
 * Common hardware pmu events
 */
enum PmuHwId {
    PERF_COUNT_HW_CPU_CYCLES = 0,      /**< cpu cycle event */
    PERF_COUNT_HW_INSTRUCTIONS,        /**< instruction event */
    PERF_COUNT_HW_DCACHE_REFERENCES,   /**< dcache access event */
    PERF_COUNT_HW_DCACHE_MISSES,       /**< dcache miss event */
    PERF_COUNT_HW_ICACHE_REFERENCES,   /**< icache access event */
    PERF_COUNT_HW_ICACHE_MISSES,       /**< icache miss event */
    PERF_COUNT_HW_BRANCH_INSTRUCTIONS, /**< software change of pc event */
    PERF_COUNT_HW_BRANCH_MISSES,       /**< branch miss event */

    PERF_COUNT_HW_MAX,
};

/**
 * @ingroup los_perf
 * Common hrtimer timed events
 */
enum PmuTimedId {
    PERF_COUNT_CPU_CLOCK = 0,      /**< hrtimer timed event */
};

/**
 * @ingroup los_perf
 * Common software pmu events
 */
enum PmuSwId {
    PERF_COUNT_SW_TASK_SWITCH = 1, /**< task switch event */
    PERF_COUNT_SW_IRQ_RESPONSE,    /**< irq response event */
    PERF_COUNT_SW_MEM_ALLOC,       /**< memory alloc event */
    PERF_COUNT_SW_MUX_PEND,        /**< mutex pend event */

    PERF_COUNT_SW_MAX,
};

/**
 * @ingroup los_perf
 * perf sample data types
 * Config it through PerfConfigAttr->sampleType.
 */
enum PerfSampleType {
    PERF_RECORD_CPU       = 1U << 0, /**< record current cpuid */
    PERF_RECORD_TID       = 1U << 1, /**< record current task id */
    PERF_RECORD_TYPE      = 1U << 2, /**< record event type */
    PERF_RECORD_PERIOD    = 1U << 3, /**< record event period */
    PERF_RECORD_TIMESTAMP = 1U << 4, /**< record timestamp */
    PERF_RECORD_IP        = 1U << 5, /**< record instruction pointer */
    PERF_RECORD_CALLCHAIN = 1U << 6, /**< record backtrace */
};

/**
 * @ingroup los_perf
 * perf configuration sub event information
 *
 * This structure is used to config specific events attributes.
 */
typedef struct {
    UINT32 type;              /**< enum PerfEventType */
    struct {
        UINT32 eventId;       /**< the specific event corresponds to the PerfEventType */
        UINT32 period;        /**< event period, for every "period"th occurrence of the event a
                                   sample will be recorded */
    } events[PERF_MAX_EVENT]; /**< perf event list */
    UINT32 eventsNr;          /**< total perf event number */
    BOOL predivided;         /**< whether to prescaler (once every 64 counts),
                                  which only take effect on cpu cycle hardware event */
} PerfEventConfig;

/**
 * @ingroup los_perf
 * perf configuration main information
 *
 * This structure is used to set perf sampling attributes, including events, tasks and other information.
 */
typedef struct {
    PerfEventConfig         eventsCfg;                      /**< perf event config */
    UINT32                  taskIds[PERF_MAX_FILTER_TSKS];  /**< perf task filter list (whitelist) */
    UINT32                  taskIdsNr;                      /**< task numbers of task filter whiltelist,
                                                                 if set 0 perf will sample all tasks */
    UINT32                  sampleType;                     /**< type of data to sample defined in PerfSampleType */
    BOOL                    needSample;                     /**< whether to sample data */
    BOOL                    taskFilterEnable;               /**< whether to filter tasks */
} PerfConfigAttr;

extern VOID OsPerfHook(UINT32 event);

#if defined(LOSCFG_KERNEL_PERF) && defined(LOSCFG_PERF_SW_PMU)
#define LOS_PERF(EVENT) do {      \
        OsPerfHook(EVENT);        \
    } while (0)
#else
#define LOS_PERF(EVENT)
#endif

/**
 * @ingroup los_perf
 * @brief Init perf.
 *
 * @par Description:
 * <ul>
 * <li>Used to initialize the perf module, including initializing the PMU, allocating memory,
 * etc.,which is called during the phase of system initialization.</li>
 * </ul>
 * @attention
 * <ul>
 * <li>If buf is not null, user must ensure size is not bigger than buf's length.</li>
 * </ul>
 *
 * @param  buf     [IN] Pointer of sample data buffer;Use the dynamically allocated memory if the pointer is NULL.
 * @param  size    [IN] Length of sample data buffer;
 *
 * @retval #LOS_ERRNO_PERF_STATUS_INVALID              Perf in a wrong status.
 * @retval #LOS_ERRNO_PERF_HW_INIT_ERROR               Perf hardware pmu init fail.
 * @retval #LOS_ERRNO_PERF_TIMED_INIT_ERROR            Perf timed pmu init fail.
 * @retval #LOS_ERRNO_PERF_SW_INIT_ERROR               Perf software pmu init fail.
 * @retval #LOS_ERRNO_PERF_BUF_ERROR                   Perf buffer init fail.
 * @retval #LOS_OK                                     Perf init success.
 * @par Dependency:
 * <ul>
 * <li>los_perf.h: the header file that contains the API declaration.</li>
 * </ul>
 * @since Huawei LiteOS V200R005C00
 */
UINT32 LOS_PerfInit(VOID *buf, UINT32 size);

/**
 * @ingroup los_perf
 * @brief Start perf sampling.
 *
 * @par Description
 * Start perf sampling.
 * @attention
 * None.
 *
 * @param  sectionId          [IN] Set the section id for marking this piece of data in the perf sample data buffer.
 * @retval None.
 * @par Dependency:
 * <ul>
 * <li>los_perf.h: the header file that contains the API declaration.</li>
 * </ul>
 * @since Huawei LiteOS V200R005C00
 */
VOID LOS_PerfStart(UINT32 sectionId);

/**
 * @ingroup los_perf
 * @brief Stop perf sampling.
 *
 * @par Description
 * Stop perf sampling.
 * @attention
 * None.
 *
 * @param  None.
 *
 * @retval None.
 * @par Dependency:
 * <ul>
 * <li>los_perf.h: the header file that contains the API declaration.</li>
 * </ul>
 * @since Huawei LiteOS V200R005C00
 */
VOID LOS_PerfStop(VOID);

/**
 * @ingroup los_perf
 * @brief Config perf parameters.
 *
 * @par Description
 * Config perf parameters before sample, for example, sample event, sample task, etc. it need to be called
 * before LOS_PerfStart.
 * @attention
 * None.
 *
 * @param  attr                      [IN] Address of a perf event attr struct.
 *
 * @retval #LOS_ERRNO_PERF_STATUS_INVALID          Perf in a wrong status.
 * @retval #LOS_ERRNO_PERF_CONFIG_NULL             Attr is null.
 * @retval #LOS_ERRNO_PERF_INVALID_PMU             Config perf pmu with error type.
 * @retval #LOS_ERRNO_PERF_PMU_CONFIG_ERROR        Config perf events fail with invaild event id or event period.
 * @retval #LOS_OK                                 Config success.
 * @par Dependency:
 * <ul>
 * <li>los_perf.h: the header file that contains the API declaration.</li>
 * </ul>
 * @since Huawei LiteOS V200R005C00
 */
UINT32 LOS_PerfConfig(PerfConfigAttr *attr);

/**
 * @ingroup los_perf
 * @brief Read data from perf sample data buffer.
 *
 * @par Description
 * Because perf sample data buffer is a ringbuffer, the data may be covered after user read ringbuffer.
 * @attention
 * None.
 *
 * @param  dest                      [IN] The destionation address.
 * @param  size                      [IN] Read size.
 * @retval #UINT32                   The really read bytes.
 * @par Dependency:
 * <ul>
 * <li>los_perf.h: the header file that contains the API declaration.</li>
 * </ul>
 * @since Huawei LiteOS V200R005C00
 */
UINT32 LOS_PerfDataRead(CHAR *dest, UINT32 size);

/**
 * @ingroup los_perf
 * @brief Register perf sample data buffer water mark hook function.
 *
 * @par Description
 * <ul>
 * <li> Register perf sample data buffer water mark hook function.</li>
 * <li> The registered hook will be called when buffer reaches the water mark./li>
 * </ul>
 * @attention
 * None.
 *
 * @param  func                      [IN] Buffer water mark hook function.
 *
 * @retval None.
 * @par Dependency:
 * <ul>
 * <li>los_perf.h: the header file that contains the API declaration.</li>
 * </ul>
 * @since Huawei LiteOS V200R005C00
 */
VOID LOS_PerfNotifyHookReg(const PERF_BUF_NOTIFY_HOOK func);

/**
 * @ingroup los_perf
 * @brief Register perf sample data buffer flush hook function.
 *
 * @par Description
 * <ul>
 * <li> Register perf sample data buffer flush hook function.</li>
 * <li> The flush hook will be called when the buffer be read or written.</li>
 * </ul>
 * @attention
 * None.
 *
 * @param  func                      [IN] Buffer flush hook function.
 *
 * @retval None.
 * @par Dependency:
 * <ul>
 * <li>los_perf.h: the header file that contains the API declaration.</li>
 * </ul>
 * @since Huawei LiteOS V200R005C00
 */
VOID LOS_PerfFlushHookReg(const PERF_BUF_FLUSH_HOOK func);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_PERF_H */
