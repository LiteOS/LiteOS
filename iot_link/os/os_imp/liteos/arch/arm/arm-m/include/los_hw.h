/*----------------------------------------------------------------------------
 * Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
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
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

 /**@defgroup los_hw hardware
   *@ingroup kernel
 */

#ifndef _LOS_HW_H
#define _LOS_HW_H

#include "los_base.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#include "los_task.ph"

#if (LOSCFG_STATIC_TASK == YES) && (LOSCFG_ENABLE_MPU == YES)
#include <heap.h>
#endif

#if (LOSCFG_ENABLE_MPU == YES)
#include "los_mpu.h"
#endif

#if (LOSCFG_STATIC_TASK == YES)
#include "cmsis_compiler.h"
#endif

/**
 * @ingroup los_hw
 * The initialization value of stack space.
 */
#define EMPTY_STACK                 0xCACA

/**
 * @ingroup los_hw
 * Trigger a task.
 */
#define osTaskTrap()                __asm("   TRAP    #31")

/**
 * @ingroup los_hw
 * Check task schedule.
 */
#define LOS_CHECK_SCHEDULE          ((!g_usLosTaskLock))

/**
 * @ingroup los_hw
 * FPU exist macro: If it exist, save EXC_RETURN value in the context of the task to check
 * the size of the stack frame(BIT_4: 1 ---> 8 words, 0 ---> 26 wrods) used by the task.
 */
#define FPU_EXIST  (  (defined ( __CC_ARM ) && defined ( __TARGET_FPU_VFP )) \
                   || (defined ( __ICCARM__ ) && defined ( __ARMVFP__ )) \
                   || (defined ( __GNUC__ ) && defined ( __VFP_FP__ ) && !defined( __SOFTFP__ )) )


/**
 * @ingroup los_hw
 * Define the type of a task context control block.
 */
typedef struct tagTskContext
{
#if (LOSCFG_ENABLE_MPU == YES)
    UINT32 uwControl;
#endif
    UINT32 uwR4;
    UINT32 uwR5;
    UINT32 uwR6;
    UINT32 uwR7;
    UINT32 uwR8;
    UINT32 uwR9;
    UINT32 uwR10;
    UINT32 uwR11;
    UINT32 uwPriMask;
#if FPU_EXIST
    UINT32 uwExcReturn;
#endif
    UINT32 uwR0;
    UINT32 uwR1;
    UINT32 uwR2;
    UINT32 uwR3;
    UINT32 uwR12;
    UINT32 uwLR;
    UINT32 uwPC;
    UINT32 uwxPSR;
} TSK_CONTEXT_S;

#if (LOSCFG_STATIC_TASK == YES)

#if (LOSCFG_ENABLE_MPU == YES)

#define ROUND_UP(n, a)          ((n + (a - 1)) & (a - 1))

/**
 * @ingroup los_task
 * Define a task statically.
 */
#define LOS_TASK_DEF(name, taskName, entry, taskPrio, arg, stackSize, heapSize,\
                     regions)                                                  \
LOS_STATIC_ASSERT (taskPrio <= OS_TASK_PRIORITY_LOWEST);                       \
LOS_STATIC_ASSERT (taskPrio >= 0);                                             \
LOS_STATIC_ASSERT ((stackSize & 7) == 0);                                      \
LOS_STATIC_ASSERT ((heapSize & 7) == 0);                                       \
enum {                                                                         \
    s_##name##_hpsz = heapSize == 0 ? 0 : heapSize + sizeof (heap_t),          \
    s_##name##_ss_0 = stackSize + sizeof (TSK_CONTEXT_S) +                     \
                      sizeof (LOS_MPU_ENTRY) * (MPU_NR_USR_ENTRIES + 1),       \
    s_##name##_as_0 = s_##name##_ss_0 + s_##name##_hpsz - 1,                   \
    s_##name##_as_1 = s_##name##_as_0 | (s_##name##_as_0 >> 1),                \
    s_##name##_as_2 = s_##name##_as_1 | (s_##name##_as_1 >> 2),                \
    s_##name##_as_3 = s_##name##_as_2 | (s_##name##_as_2 >> 4),                \
    s_##name##_as_4 = s_##name##_as_3 | (s_##name##_as_3 >> 8),                \
    s_##name##_as_5 = s_##name##_as_4 | (s_##name##_as_4 >> 16),               \
    s_##name##_as_6 = s_##name##_as_5 + 1,                                     \
    s_##name##_allocSize = s_##name##_as_6 < 256 ? 256 : s_##name##_as_6,      \
    s_##name##_stackSize = s_##name##_allocSize - s_##name##_hpsz,             \
    };                                                                         \
__ALIGNED (s_##name##_allocSize) static char                                   \
    s_##sp_##name [s_##name##_allocSize] = {0};                                \
static LOS_TASK_CB s_##name##CB =                                              \
{                                                                              \
    s_##sp_##name,                                                             \
    OS_TASK_STATUS_SUSPEND,                                                    \
    taskPrio,                                                                  \
    (VOID *) regions,                                                          \
    s_##name##_hpsz,                                                           \
    s_##name##_hpsz ? s_##sp_##name + s_##name##_stackSize : NULL,             \
    s_##name##_stackSize,                                                      \
    NULL,                                                                      \
    0,  /* uwTaskID init later? */                                             \
    (TSK_ENTRY_FUNC) entry,                                                    \
    NULL,                                                                      \
    NULL,                                                                      \
    arg,                                                                       \
    taskName,                                                                  \
    {0},    /* stPendList init later */                                        \
    {0},    /* stTimerList */                                                  \
}

#else

/**
 * @ingroup los_task
 * Define a task statically.
 */
#define LOS_TASK_DEF(name, taskName, entry, taskPrio, arg, stackSize, ...)     \
LOS_STATIC_ASSERT (taskPrio <= OS_TASK_PRIORITY_LOWEST);                       \
LOS_STATIC_ASSERT (taskPrio >= 0);                                             \
LOS_STATIC_ASSERT (stackSize > sizeof (TSK_CONTEXT_S));                        \
LOS_STATIC_ASSERT ((stackSize & 7) == 0);                                      \
static INT64 s_##sp_##name [stackSize / sizeof (INT64)] = {0};                 \
static LOS_TASK_CB s_##name##CB =                                              \
{                                                                              \
    s_##sp_##name,                                                             \
    OS_TASK_STATUS_SUSPEND,                                                    \
    taskPrio,                                                                  \
    stackSize,                                                                 \
    NULL,                                                                      \
    0,  /* uwTaskID init later? */                                             \
    (TSK_ENTRY_FUNC) entry,                                                    \
    NULL,                                                                      \
    NULL,                                                                      \
    arg,                                                                       \
    taskName,                                                                  \
    {0},    /* stPendList init later */                                        \
    {0},    /* stTimerList */                                                  \
}

#endif

extern UINT32 LOS_StaticTaskInit(void *pvTaskCB, UINT32 *puwTaskID);

/**
 * @ingroup los_task
 * Initialize statically created tasks.
 */

#define LOS_TASK_INIT(name, pid)                                               \
    LOS_StaticTaskInit (((void *) &s_##name##CB), pid)
#endif


/**
 * @ingroup  los_hw
 * @brief: Task stack initialization.
 *
 * @par Description:
 * This API is used to initialize the task stack.
 *
 * @attention:
 * <ul><li>None.</li></ul>
 *
 * @param  pstTaskCB    [IN] Type#LOS_TASK_CB: TCB.
 * @param  pstInitParam [IN] Type  #TSK_INIT_PARAM_S * Parameter for task creation.
 *
 * @retval: pstContext Type#TSK_CONTEXT_S *.
 * @par Dependency:
 * <ul><li>los_hw.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V100R001C00
 */
extern VOID osTskStackInit(LOS_TASK_CB *pstTaskCB, TSK_INIT_PARAM_S *pstInitParam);

/**
 * @ingroup  los_hw
 * @brief: Task scheduling Function.
 *
 * @par Description:
 * This API is used to scheduling task.
 *
 * @attention:
 * <ul><li>None.</li></ul>
 *
 * @param  None.
 *
 * @retval: None.
 * @par Dependency:
 * <ul><li>los_hw.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V100R001C00
 */
extern VOID osSchedule(VOID);


/**
 * @ingroup  los_hw
 * @brief: Function to determine whether task scheduling is required.
 *
 * @par Description:
 * This API is used to Judge and entry task scheduling.
 *
 * @attention:
 * <ul><li>None.</li></ul>
 *
 * @param  None.
 *
 * @retval: None.
 * @par Dependency:
 * <ul><li>los_hw.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V100R001C00
 */
extern VOID LOS_Schedule(VOID);

LITE_OS_SEC_TEXT_MINOR VOID osTaskExit(VOID);

/**
 * @ingroup  los_hw
 * @brief: The M3 wait interrupt instruction.
 *
 * @par Description:
 * This API is used to make CPU enter to power-save mode.
 *
 * @attention:
 * <ul><li>None.</li></ul>
 *
 * @param  None.
 *
 * @retval: None.
 * @par Dependency:
 * <ul><li>los_hw.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V100R001C00
 */

extern VOID osEnterSleep(VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


#endif /* _LOS_HW_H */

