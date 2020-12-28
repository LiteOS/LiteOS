/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description : LiteOS Cpu Usage Calculation Module Headfile For User
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
 * @defgroup los_cpup CPU usage
 * @ingroup kernel
 */

#ifndef _LOS_CPUP_H
#define _LOS_CPUP_H

#include "los_hwi.h"
#include "los_base.h"
#include "los_sys.h"
#include "los_task.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup los_cpup
 * CPU usage error code: The request for memory fails.
 *
 * Value: 0x02001e00.
 *
 * Solution: Decrease the maximum number of tasks.
 */
#define LOS_ERRNO_CPUP_NO_MEMORY             LOS_ERRNO_OS_ERROR(LOS_MOD_CPUP, 0x00)

/**
 * @ingroup los_cpup
 * CPU usage error code: The pointer to an input parameter is NULL.
 *
 * Value: 0x02001e01.
 *
 * Solution: Check whether the pointer to the input parameter is usable.
 */
#define LOS_ERRNO_CPUP_TASK_PTR_NULL         LOS_ERRNO_OS_ERROR(LOS_MOD_CPUP, 0x01)

/**
 * @ingroup los_cpup
 * CPU usage error code: The CPU usage is not initialized.
 *
 * Value: 0x02001e02.
 *
 * Solution: Check whether the CPU usage is initialized.
 */
#define LOS_ERRNO_CPUP_NO_INIT               LOS_ERRNO_OS_ERROR(LOS_MOD_CPUP, 0x02)

/**
 * @ingroup los_cpup
 * CPU usage error code: The number of threads is invalid.
 *
 * Value: 0x02001e03.
 *
 * Solution: Check whether the number of threads is applicable for the current operation.
 */
#define LOS_ERRNO_CPUP_MAXNUM_INVALID        LOS_ERRNO_OS_ERROR(LOS_MOD_CPUP, 0x03)

/**
 * @ingroup los_cpup
 * CPU usage error code: The target thread is not created.
 *
 * Value: 0x02001e04.
 *
 * Solution: Check whether the target thread is created.
 */
#define LOS_ERRNO_CPUP_THREAD_NO_CREATED     LOS_ERRNO_OS_ERROR(LOS_MOD_CPUP, 0x04)

/**
 * @ingroup los_cpup
 * CPU usage error code: The target task ID is invalid.
 *
 * Value: 0x02001e05.
 *
 * Solution: Check whether the target task ID is applicable for the current operation.
 */
#define LOS_ERRNO_CPUP_TSK_ID_INVALID        LOS_ERRNO_OS_ERROR(LOS_MOD_CPUP, 0x05)

/**
 * @ingroup los_cpup
 * The structure of the CPU usage information of all tasks.
 */
typedef struct tagCpupInfo {
    UINT16 usStatus; /**< The task status.   */
    UINT32 uwUsage;  /**< CPU usage of all tasks. It is a permillage. And the value range is [0,1000]. */
} CPUP_INFO_S;

/**
 * @ingroup los_cpup
 * The time period which the CPU usage collects in.
 */
enum {
    CPUP_LAST_TEN_SECONDS = 0, /**< Display CPU usage in the last ten seconds, the value is 0.         */
    CPUP_LAST_ONE_SECONDS = 1, /**< Display CPU usage in the last one second, the value is 1.          */
    CPUP_ALL_TIME = 0xffff     /**< Display CPU usage from system startup to now, the value is 0xffff. */
};


/**
 * @ingroup los_cpup
 * @brief Obtain the historical CPU usage.
 *
 * @par Description:
 * This API is used to obtain the historical CPU usage of the system.
 * @attention
 * This API can be called only the CPU usage has been initialized. Otherwise, the obtainment of
 * the CPU usage will be failed.
 *
 * @param  mode     [IN] Type #UINT32. Task mode. The parameter value 0 indicates that the CPU
 *                                     usage in the last ten seconds will be obtained, and the
 *                                     parameter value 1 indicates that the CPU usage in the
 *                                     former one second will be obtained. Other values indicate
 *                                     that the CPU usage in all time will be obtained.
 *
 * @retval #LOS_ERRNO_CPUP_NO_INIT     The CPU usage is not initialized.
 * @retval #UINT32                     The historical CPU usage of the system. It is a permillage.
 *                                     And the value range is [0,1000].
 * @par Dependency:
 * <ul><li>los_cpup.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_HistoryTaskCpuUsage | LOS_AllCpuUsage
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_HistorySysCpuUsage(UINT32 mode);

/**
 * @ingroup los_cpup
 * @brief  Obtain the historical CPU usage of a specified task.
 *
 * @par Description:
 * This API is used to obtain the historical CPU usage of a task specified by a passed-in task ID.
 * @attention
 * <ul>
 * <li>This API can be called only the CPU usage has been initialized. Otherwise, the obtainment of
 * the CPU usage will be failed.</li>
 * <li>The passed-in task ID must be valid and the task specified by the task ID must has been created.
 * Otherwise, the obtainment of the CPU usage will be failed.</li>
 * </ul>
 *
 * @param taskId   [IN] Type #UINT32. Task ID.
 * @param  mode     [IN] Type #UINT32. Task mode. The parameter value 0 indicates that the CPU
 *                                     usage in the last ten seconds will be obtained, and the
 *                                     parameter value 1 indicates that the CPU usage in the
 *                                     former one second will be obtained. Other values indicate
 *                                     that the CPU usage in all time will be obtained.
 *
 * @retval #LOS_ERRNO_CPUP_NO_INIT                  The CPU usage has not been initialized.
 * @retval #LOS_ERRNO_CPUP_TSK_ID_INVALID           The passed-in task ID is invalid.
 * @retval #LOS_ERRNO_CPUP_THREAD_NO_CREATED        The task specified by the task ID has not been created.
 * @retval #UINT32                                  The historical CPU usage of a specified task. It is a
 *                                                  permillage. And the value range is [0,1000].
 * @par Dependency:
 * <ul><li>los_cpup.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_HistorySysCpuUsage
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_HistoryTaskCpuUsage(UINT32 taskId, UINT32 mode);

/**
 * @ingroup los_cpup
 * @brief Obtain the CPU usage of tasks and hardware interrupts.
 *
 * @par Description:
 * This API is used to obtain the CPU usage of specified number of tasks or hardware interrupts.
 * The first parameter maxNum means the maximum number of tasks or hardware interrupts. And the
 * last parameter flag determines to obtain the CPU usage of tasks or hardware interrupts.
 * @attention
 * <ul>
 * <li>This API can be called only the CPU usage has been initialized. Otherwise, the obtainment
 * of the CPU usage will be failed.</li>
 * <li>The input parameter pointer (cpupInfo) must not be NULL, and the maximum number (maxNum)
 * must be usable. Otherwise, the obtainment of the CPU usage will be failed.</li>
 * <li>The input parameter pointer (cpupInfo) point to the structure array whose size should
 * be greater than (maxNum * sizeof(CPUP_INFO_S)).</li>
 * </ul>
 *
 * @param maxNum    [IN]  Type #UINT16.       The Maximum number of tasks or hardware interrupts
 *                                            to be obtained.
 * @param cpupInfo  [OUT] Type #CPUP_INFO_S*. The pointer used to save the CPU usage information.
 * @param  mode     [IN] Type #UINT32.        Task mode. The parameter value 0 indicates that the
 *                                            CPU usage in the last ten seconds will be obtained,
 *                                            and the parameter value 1 indicates that the CPU
 *                                            usage in the former one second will be obtained.
 *                                            Other values indicate that the CPU usage in all time
 *                                            will be obtained.
 * @param flag      [IN]  Type #UINT16.       The parameter value 0 indicates that the API only
 *                                            obtains hardware interrupt CPU usage. Other values
 *                                            indicate that the API only obtains the task CPU usage.
 *
 * @retval #LOS_ERRNO_CPUP_NO_INIT            The CPU usage is not initialized.
 * @retval #LOS_ERRNO_CPUP_TASK_PTR_NULL      The input parameter pointer is NULL.
 * @retval #LOS_ERRNO_CPUP_MAXNUM_INVALID     The maximum number of tasks or hardware interrupts is
 *                                            invalid.
 * @retval #LOS_OK                            The CPU usage is successfully obtained.
 * @par Dependency:
 * <ul><li>los_cpup.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_AllCpuUsage(UINT16 maxNum, CPUP_INFO_S *cpupInfo, UINT32 mode, UINT16 flag);

/**
 * @ingroup los_cpup
 * @brief Reset the data of CPU usage.
 *
 * @par Description:
 * This API is used to reset the CPU usage. It will clear the historical CPU usage. If this API
 * is called, the CPU usage in all time will become the time period from the API calling to now.
 * @attention
 * None.
 *
 * @param None.
 *
 * @retval None.
 *
 * @par Dependency:
 * <ul><li>los_cpup.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V100R001C00
 */
extern VOID LOS_CpupReset(VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_CPUP_H */
