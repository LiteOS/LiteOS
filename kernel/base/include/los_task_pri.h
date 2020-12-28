/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: LiteOS Task Module Implementation Private HeadFile
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

#ifndef _LOS_TASK_PRI_H
#define _LOS_TASK_PRI_H

#include "los_task.h"
#include "los_sched_pri.h"
#include "los_sortlink_pri.h"
#include "los_spinlock.h"
#ifdef LOSCFG_DEBUG_SCHED_STATISTICS
#include "los_sched_debug_pri.h"
#endif
#include "los_stackinfo_pri.h"
#include "arch/task.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* Task signal types */
#define SIGNAL_NONE                 0U
#define SIGNAL_KILL                 (1U << 0)
#define SIGNAL_SUSPEND              (1U << 1)
#define SIGNAL_AFFI                 (1U << 2)

/* scheduler lock */
extern SPIN_LOCK_S g_taskSpin;
#define SCHEDULER_LOCK(state)       LOS_SpinLockSave(&g_taskSpin, &(state))
#define SCHEDULER_UNLOCK(state)     LOS_SpinUnlockRestore(&g_taskSpin, state)

/* default and non-running task's ownership id */
#define OS_TASK_INVALID_CPUID       0xFFFF

/* Null task ID */
#define OS_TASK_ERRORID             0xFFFFFFFF

/* Highest task priority */
#define OS_TASK_PRIORITY_HIGHEST    0

/* Lowest task priority */
#define OS_TASK_PRIORITY_LOWEST     31

/* The task control block is unused */
#define OS_TASK_STATUS_UNUSED       0x0001U

/* The task is suspended */
#define OS_TASK_STATUS_SUSPEND      0x0002U

/* The task is ready */
#define OS_TASK_STATUS_READY        0x0004U

/* The task is blocked */
#define OS_TASK_STATUS_PEND         0x0008U

/* The task is running */
#define OS_TASK_STATUS_RUNNING      0x0010U

/* The task is delayed */
#define OS_TASK_STATUS_DELAY        0x0020U

/* The time for waiting for an event to occur expires */
#define OS_TASK_STATUS_TIMEOUT      0x0040U

/* The task is pend for a period of time */
#define OS_TASK_STATUS_PEND_TIME    0x0080U

/* The task is automatically deleted */
#define OS_TASK_FLAG_DETACHED       0x0001U

/* The task is system-level task, like idle, swtmr and etc */
#define OS_TASK_FLAG_SYSTEM         0x0002U

/* Boundary on which the stack size is aligned */
#define OS_TASK_STACK_SIZE_ALIGN    16U

/* Boundary on which the stack address is aligned */
#define OS_TASK_STACK_ADDR_ALIGN    8U

/* Number of usable task priorities */
#define OS_TSK_PRINUM               (OS_TASK_PRIORITY_LOWEST - OS_TASK_PRIORITY_HIGHEST + 1)

/* This Macro is used to get task control block index */
#define OS_TSK_GET_INDEX(taskId)    (taskId)

/* This Macro is used to check task id */
#define OS_TASK_ID_CHECK_INVALID(taskId) (OS_TSK_GET_INDEX(taskId) >= g_taskMaxNum)

/* This Macro is used to obtain the pointer to a task control block using a corresponding parameter */
#define OS_TCB_FROM_PENDLIST(ptr) LOS_DL_LIST_ENTRY(ptr, LosTaskCB, pendList)

/* This Macro is used to obtain the pointer to a task control block that has a specified task ID */
#define OS_TCB_FROM_TID(taskId) (((LosTaskCB *)g_taskCBArray) + (taskId))

typedef struct {
    VOID            *stackPointer;      /* Task stack pointer */
    UINT16          taskStatus;         /* Task status */
    UINT16          priority;           /* Task priority */
    UINT32          taskFlags : 31;     /* Task extend flags: taskFlags uses 8 bits now. 23 bits left */
    UINT32          usrStack : 1;       /* Usr Stack uses the last bit */
    UINT32          stackSize;          /* Task stack size */
    UINTPTR         topOfStack;         /* Task stack top */
    UINT32          taskId;             /* Task ID */
    TSK_ENTRY_FUNC  taskEntry;          /* Task entrance function */
    VOID            *taskSem;           /* Task-held semaphore */
#ifdef LOSCFG_LAZY_STACK
    UINT32          stackFrame;         /* Stack frame: 0=Basic, 1=Extended */
#endif
#ifdef LOSCFG_COMPAT_POSIX
    VOID            *threadJoin;        /* pthread adaption */
    VOID            *threadJoinRetval;  /* pthread adaption */
#endif
    VOID            *taskMux;           /* Task-held mutex */
#ifdef LOSCFG_OBSOLETE_API
    UINTPTR         args[4];            /* Parameter, of which the maximum number is 4 */
#else
    VOID            *args;              /* Parameter, of which the type is void * */
#endif
    CHAR            *taskName;          /* Task name */
    LOS_DL_LIST     pendList;           /* Task pend node */
    SortLinkList    sortList;           /* Task sortlink node */
#ifdef LOSCFG_BASE_IPC_EVENT
    EVENT_CB_S      event;
    UINT32          eventMask;          /* Event mask */
    UINT32          eventMode;          /* Event mode */
#endif
    VOID            *msg;               /* Memory allocated to queues */
    UINT32          priBitMap;          /* BitMap for recording the change of task priority,
                                             the priority can not be greater than 31 */
    UINT32          signal;             /* Task signal */
#ifdef LOSCFG_BASE_CORE_TIMESLICE
    UINT16          timeSlice;          /* Remaining time slice */
#endif
#ifdef LOSCFG_KERNEL_SMP
    UINT16          currCpu;            /* CPU core number of this task is running on */
    UINT16          lastCpu;            /* CPU core number of this task is running on last time */
    UINT32          timerCpu;           /* CPU core number of this task is delayed or pended */
    UINT16          cpuAffiMask;        /* CPU affinity mask, support up to 16 cores */
#ifdef LOSCFG_KERNEL_SMP_TASK_SYNC
    UINT32          syncSignal;         /* Synchronization for signal handling */
#endif
#ifdef LOSCFG_KERNEL_SMP_LOCKDEP
    LockDep         lockDep;
#endif
#endif
#ifdef LOSCFG_DEBUG_SCHED_STATISTICS
    SchedStat       schedStat;          /* Schedule statistics */
#endif
#ifdef LOSCFG_KERNEL_PERF
    UINTPTR         pc;
    UINTPTR         fp;
#endif
} LosTaskCB;

/* Maximum number of tasks */
extern UINT32 g_taskMaxNum;

/* Starting address of a task */
extern LosTaskCB *g_taskCBArray;

/* Time slice structure */
typedef struct {
    LosTaskCB *task; /* Current running task */
    UINT16 time;     /* Expiration time point */
    UINT16 timeout;  /* Expiration duration */
} OsTaskRobin;

STATIC INLINE LosTaskCB *OsCurrTaskGet(VOID)
{
    return (LosTaskCB *)ArchCurrTaskGet();
}

STATIC INLINE VOID OsCurrTaskSet(LosTaskCB *task)
{
    ArchCurrTaskSet(task);
}

extern VOID OsTaskSchedule(LosTaskCB *, LosTaskCB *);
extern VOID OsStartToRun(LosTaskCB *);
extern VOID OsTaskScan(VOID);
extern VOID OsIdleTask(VOID);
extern UINT32 OsIdleTaskCreate(VOID);
extern UINT32 OsTaskInit(VOID);
#ifdef LOSCFG_BASE_CORE_TSK_MONITOR
extern VOID OsTaskMonInit(VOID);
extern VOID OsTaskSwitchCheck(const LosTaskCB *oldTask, const LosTaskCB *newTask);
#endif
extern UINT32 OsShellCmdDumpTask(INT32 argc, const CHAR **argv);

/* get task info */
#define OS_ALL_TASK_MASK  0xFFFFFFFF
extern UINT32 OsShellCmdTskInfoGet(UINT32 taskId);
extern VOID *OsGetMainTask(VOID);
extern VOID OsSetMainTask(VOID);
extern LosTaskCB *OsGetTopTask(VOID);
extern UINT32 OsGetIdleTaskId(VOID);
extern CHAR *OsCurTaskNameGet(VOID);
extern VOID OsTaskPriModify(LosTaskCB *taskCB, UINT16 priority);
extern VOID OsTaskAdd2TimerList(LosTaskCB *taskCB, UINT32 timeout);
extern VOID OsTimerListDelete(LosTaskCB *taskCB);
extern VOID OsTaskWait(LOS_DL_LIST *list, UINT16 taskStatus, UINT32 timeout);
extern VOID OsTaskWake(LosTaskCB *resumedTask, UINT16 taskStatus);
extern VOID OsTaskEntry(UINT32 taskId);
extern UINT32 OsTaskProcSignal(VOID);
#ifdef LOSCFG_DEBUG_SCHED_STATISTICS
extern VOID OsSchedStatistics(LosTaskCB *runTask, LosTaskCB *newTask);
#endif
#ifdef LOSCFG_EXC_INTERACTION
extern BOOL IsIdleTask(UINT32 taskId);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_TASK_PRI_H */
