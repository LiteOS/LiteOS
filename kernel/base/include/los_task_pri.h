/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: LiteOS Task Module Implementation Private HeadFile
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

#ifndef _LOS_TASK_PRI_H
#define _LOS_TASK_PRI_H

#include "los_task.h"
#include "los_sched_pri.h"
#include "los_sortlink_pri.h"
#include "los_spinlock.h"
#include "los_stackinfo_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup los_task
 * Define task siginal types.
 *
 * Task siginal types.
 */
#define SIGNAL_NONE                                 0U
#define SIGNAL_KILL                                 (1U << 0)
#define SIGNAL_SUSPEND                              (1U << 1)
#define SIGNAL_AFFI                                 (1U << 2)

/* scheduler lock */
extern SPIN_LOCK_S g_taskSpin;
#define SCHEDULER_LOCK(state)                       LOS_SpinLockSave(&g_taskSpin, &(state))
#define SCHEDULER_UNLOCK(state)                     LOS_SpinUnlockRestore(&g_taskSpin, state)

/* default and non-running task's ownership id */
#define OS_TASK_INVALID_CPUID                       0xFFFF

/**
 * @ingroup los_task
 * Null task ID
 *
 */
#define OS_TASK_ERRORID           0xFFFFFFFF

/**
 * @ingroup los_task
 * Define a usable task priority.
 *
 * Highest task priority.
 */
#define OS_TASK_PRIORITY_HIGHEST  0

/**
 * @ingroup los_task
 * Define a usable task priority.
 *
 * Lowest task priority.
 */
#define OS_TASK_PRIORITY_LOWEST   31

/**
 * @ingroup los_task
 * Flag that indicates the task or task control block status.
 *
 * The task control block is unused.
 */
#define OS_TASK_STATUS_UNUSED     0x0001U

/**
 * @ingroup los_task
 * Flag that indicates the task or task control block status.
 *
 * The task is suspended.
 */
#define OS_TASK_STATUS_SUSPEND    0x0002U

/**
 * @ingroup los_task
 * Flag that indicates the task or task control block status.
 *
 * The task is ready.
 */
#define OS_TASK_STATUS_READY      0x0004U

/**
 * @ingroup los_task
 * Flag that indicates the task or task control block status.
 *
 * The task is blocked.
 */
#define OS_TASK_STATUS_PEND       0x0008U

/**
 * @ingroup los_task
 * Flag that indicates the task or task control block status.
 *
 * The task is running.
 */
#define OS_TASK_STATUS_RUNNING    0x0010U

/**
 * @ingroup los_task
 * Flag that indicates the task or task control block status.
 *
 * The task is delayed.
 */
#define OS_TASK_STATUS_DELAY      0x0020U

/**
 * @ingroup los_task
 * Flag that indicates the task or task control block status.
 *
 * The time for waiting for an event to occur expires.
 */
#define OS_TASK_STATUS_TIMEOUT    0x0040U

/**
 * @ingroup los_task
 * Flag that indicates the task or task control block status.
 *
 * The task is pend for a period of time.
 */
#define OS_TASK_STATUS_PEND_TIME  0x0080U

/**
 * @ingroup los_task
 * Flag that indicates the task property.
 *
 * The task is automatically deleted.
 */
#define OS_TASK_FLAG_DETACHED                       0x0001

/**
 * @ingroup los_task
 * Flag that indicates the task property.
 *
 * The task is system-level task, like idle, swtmr and etc.
 */
#define OS_TASK_FLAG_SYSTEM                         0x0002

/**
 * @ingroup los_task
 * Boundary on which the stack size is aligned.
 *
 */
#define OS_TASK_STACK_SIZE_ALIGN  16U

/**
 * @ingroup los_task
 * Boundary on which the stack address is aligned.
 *
 */
#define OS_TASK_STACK_ADDR_ALIGN  8U

/**
 * @ingroup los_task
 * Number of usable task priorities.
 */
#define OS_TSK_PRINUM             (OS_TASK_PRIORITY_LOWEST - OS_TASK_PRIORITY_HIGHEST + 1)

/**
 * @ingroup los_task
 * @brief the max task count for switch.
 */
#define OS_TASK_SWITCH_INFO_COUNT                   0xA

/**
 * @ingroup  los_task
* @brief Check whether a task ID is valid.
*
* @par Description:
* This API is used to check whether a task ID, excluding the idle task ID, is valid.
* @attention None.
*
* @param  taskID [IN] Task ID.
*
* @retval 0 or 1. One indicates that the task ID is invalid, whereas zero indicates that the task ID is valid.
* @par Dependency:
* <ul><li>los_task_pri.h: the header file that contains the API declaration.</li></ul>
* @see
* @since Huawei LiteOS V100R001C00
*/
#define OS_TSK_GET_INDEX(taskID) (taskID)

/**
* @ingroup  los_task
* @brief Obtain the pointer to a task control block.
*
* @par Description:
* This API is used to obtain the pointer to a task control block using a corresponding parameter.
* @attention None.
*
* @param  ptr [IN] Parameter used for obtaining the task control block.
*
* @retval Pointer to the task control block.
* @par Dependency:
* <ul><li>los_task_pri.h: the header file that contains the API declaration.</li></ul>
* @see
* @since Huawei LiteOS V100R001C00
*/
#define OS_TCB_FROM_PENDLIST(ptr) LOS_DL_LIST_ENTRY(ptr, LosTaskCB, pendList)

/**
* @ingroup  los_task
* @brief Obtain the pointer to a task control block.
*
* @par Description:
* This API is used to obtain the pointer to a task control block that has a specified task ID.
* @attention None.
*
* @param  TaskID [IN] Task ID.
*
* @retval Pointer to the task control block.
* @par Dependency:
* <ul><li>los_task_pri.h: the header file that contains the API declaration.</li></ul>
* @see
* @since Huawei LiteOS V100R001C00
*/
#define OS_TCB_FROM_TID(taskID) (((LosTaskCB *)g_taskCBArray) + (taskID))

#ifndef LOSCFG_STACK_POINT_ALIGN_SIZE
#define LOSCFG_STACK_POINT_ALIGN_SIZE                       (sizeof(UINTPTR) * 2)
#endif

/**
 * @ingroup los_task
 * @brief Dynamic memory init switch
 */
#if (LOSCFG_LIB_CONFIGURABLE == YES)
#define LOSCFG_BASE_CORE_TASK_DYN_MEM       YES
#else
#define LOSCFG_BASE_CORE_TASK_DYN_MEM       NO
#endif


typedef struct {
    VOID            *stackPointer;      /**< Task stack pointer */
    UINT16          taskStatus;         /**< Task status */
    UINT16          priority;           /**< Task priority */
    UINT32          taskFlags;          /**< Task extend flags */
    UINT32          stackSize;          /**< Task stack size */
    UINTPTR         topOfStack;         /**< Task stack top */
    UINT32          taskID;             /**< Task ID */
    TSK_ENTRY_FUNC  taskEntry;          /**< Task entrance function */
    VOID            *taskSem;           /**< Task-held semaphore */
    VOID            *threadJoin;        /**< pthread adaption */
    VOID            *threadJoinRetval;  /**< pthread adaption */
    VOID            *taskMux;           /**< Task-held mutex */
    UINTPTR         args[4];            /**< Parameter, of which the maximum number is 4 */
    CHAR            *taskName;          /**< Task name */
    LOS_DL_LIST     pendList;           /**< Task pend node */
    SortLinkList    sortList;           /**< Task sortlink node */
    EVENT_CB_S      event;
    UINT32          eventMask;          /**< Event mask */
    UINT32          eventMode;          /**< Event mode */
    VOID            *msg;               /**< Memory allocated to queues */
    UINT32          priBitMap;          /**< BitMap for recording the change of task priority,
                                             the priority can not be greater than 31 */
    UINT32          signal;             /**< Task signal */
#if (LOSCFG_BASE_CORE_TIMESLICE == YES)
    UINT16          timeSlice;          /**< Remaining time slice */
#endif
} LosTaskCB;

typedef struct {
    LosTaskCB *runTask;
    LosTaskCB *newTask;
} LosTask;


typedef struct {
    UINT8 maxCnt : 7;                /* bits [6:0] store count of task switch info */
    UINT8 isFull : 1;                /* bit [7] store isfull status */
} TaskCountInfo;

/**
 * @ingroup los_task
 * Task switch information structure.
 *
 */
typedef struct {
    UINT8 idx;
    TaskCountInfo cntInfo;
    UINT16 pid[OS_TASK_SWITCH_INFO_COUNT];
    CHAR name[OS_TASK_SWITCH_INFO_COUNT][LOS_TASK_NAMELEN];
} TaskSwitchInfo;
/**
 * @ingroup los_task
 * Maximum number of tasks.
 *
 */
extern UINT32 g_taskMaxNum;


/**
 * @ingroup los_task
 * Starting address of a task.
 *
 */
#if (LOSCFG_BASE_CORE_TASK_DYN_MEM == YES)
extern LosTaskCB *g_taskCBArray;
#else
extern LosTaskCB g_taskCBArray[];
#endif

/**
 * @ingroup los_task
 * Time slice structure.
 */
typedef struct {
    LosTaskCB *task; /**< Current running task */
    UINT16 time;     /**< Expiration time point */
    UINT16 timeout;  /**< Expiration duration */
} OsTaskRobin;

STATIC INLINE LosTaskCB *OsCurrTaskGet(VOID)
{
    return (LosTaskCB *)ArchCurrTaskGet();
}

STATIC INLINE VOID OsCurrTaskSet(LosTaskCB *task)
{
    ArchCurrTaskSet((VOID *)task);
}

extern VOID OsTaskSchedule(LosTaskCB *, LosTaskCB *);
extern VOID OsStartToRun(LosTaskCB *);
extern VOID OsTaskScan(VOID);
extern VOID OsIdleTask(VOID);
extern UINT32 OsIdleTaskCreate(VOID);
extern UINT32 OsTaskInit(VOID);
#if (LOSCFG_BASE_CORE_TSK_MONITOR == YES)
extern VOID OsTaskMonInit(VOID);
#endif
extern UINT32 OsShellCmdDumpTask(INT32 argc, const CHAR **argv);

/* get task info */
#define OS_ALL_TASK_MASK  0xFFFFFFFF
extern UINT32 OsShellCmdTskInfoGet(UINT32 taskID);

extern VOID* OsGetMainTask(VOID);
extern VOID OsSetMainTask(VOID);
extern LosTaskCB* OsGetTopTask(VOID);
extern UINT32 OsGetIdleTaskId(VOID);

/**
 * @ingroup  los_task
 * @brief Modify the priority of task.
 *
 * @par Description:
 * This API is used to modify the priority of task.
 *
 * @attention
 * <ul>
 * <li>The taskCB should be a correct pointer to task control block structure.</li>
 * <li>the priority should be in [0, OS_TASK_PRIORITY_LOWEST].</li>
 * </ul>
 *
 * @param  taskCB [IN] Type #LosTaskCB * pointer to task control block structure.
 * @param  priority  [IN] Type #UINT16 the priority of task.
 *
 * @retval  None.
 * @par Dependency:
 * <ul><li>los_task_pri.h: the header file that contains the API declaration.</li></ul>
 * @see
 * @since Huawei LiteOS V100R001C00
 */
extern VOID OsTaskPriModify(LosTaskCB *taskCB, UINT16 priority);

/**
 * @ingroup  los_task
 * @brief Add task to sorted delay list.
 *
 * @par Description:
 * This API is used to add task to sorted delay list.
 *
 * @attention
 * <ul>
 * <li>The taskCB should be a correct pointer to task control block structure.</li>
 * </ul>
 *
 * @param  taskCB [IN] Type #LosTaskCB * pointer to task control block structure.
 * @param  timeout  [IN] Type #UINT32 wait time, ticks.
 *
 * @retval  None.
 * @par Dependency:
 * <ul><li>los_task_pri.h: the header file that contains the API declaration.</li></ul>
 * @see OsTimerListDelete
 * @since Huawei LiteOS V100R001C00
 */
extern VOID OsTaskAdd2TimerList(LosTaskCB *taskCB, UINT32 timeout);

/**
 * @ingroup  los_task
 * @brief delete task from sorted delay list.
 *
 * @par Description:
 * This API is used to delete task from sorted delay list.
 *
 * @attention
 * <ul>
 * <li>The taskCB should be a correct pointer to task control block structure.</li>
 * </ul>
 *
 * @param  taskCB [IN] Type #LosTaskCB * pointer to task control block structure.
 *
 * @retval  None.
 * @par Dependency:
 * <ul><li>los_task_pri.h: the header file that contains the API declaration.</li></ul>
 * @see OsTaskAdd2TimerList
 * @since Huawei LiteOS V100R001C00
 */
extern VOID OsTimerListDelete(LosTaskCB *taskCB);

/**
 * @ingroup  los_task
 * @brief pend running task to pendlist
 *
 * @par Description:
 * This API is used to pend task to pendlist and add to sorted delay list.
 *
 * @attention
 * <ul>
 * <li>The list should be a vaild pointer to pendlist.</li>
 * </ul>
 *
 * @param  list       [IN] Type #LOS_DL_LIST * pointer to list which running task will be pended.
 * @param  taskStatus [IN] Type #UINT16  Task Status.
 * @param  timeout    [IN] Type #UINT32  Expiry time. The value range is [0,LOS_WAIT_FOREVER].
 *
 * @retval  LOS_OK       wait success
 * @retval  LOS_NOK      pend out
 * @par Dependency:
 * <ul><li>los_task_pri.h: the header file that contains the API declaration.</li></ul>
 * @see OsTaskWake
 * @since Huawei LiteOS V100R001C00
 */
extern VOID OsTaskWait(LOS_DL_LIST *list, UINT16 taskStatus, UINT32 timeout);

/**
 * @ingroup  los_task
 * @brief delete task from pendlist.
 *
 * @par Description:
 * This API is used to delete task from pendlist and also add to the priqueue.
 *
 * @attention
 * <ul>
 * <li>The resumedTask should be the task which will be add to priqueue.</li>
 * </ul>
 *
 * @param  resumedTask [IN] Type #LosTaskCB * pointer to the task which will be add to priqueue.
 * @param  taskStatus  [IN] Type #UINT16  Task Status.
 *
 * @retval  None.
 * @par Dependency:
 * <ul><li>los_task_pri.h: the header file that contains the API declaration.</li></ul>
 * @see OsTaskWait
 * @since Huawei LiteOS V100R001C00
 */
extern VOID OsTaskWake(LosTaskCB *resumedTask, UINT16 taskStatus);

extern VOID OsTaskEntry(UINT32 taskID);
extern SortLinkAttribute *OsTaskSortLinkGet(VOID);
extern UINT32 OsTaskSwitchCheck(LosTaskCB *oldTask, LosTaskCB *newTask);
extern UINT32 OsTaskProcSignal(VOID);
extern VOID OsSchedStatistics(LosTaskCB *runTask, LosTaskCB *newTask);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_TASK_PRI_H */
