/*----------------------------------------------------------------------------
 * Copyright (c) <2019>, <Huawei Technologies Co., Ltd>
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

#ifndef _LOS_SVC_H
#define _LOS_SVC_H

#include <los_config.h>
#include <los_typedef.h>

#include <los_task.h>
#include <los_membox.h>
#include <los_memory.h>
#include <los_queue.h>
#include <los_event.h>
#include <los_mux.h>
#include <los_sem.h>
#include <los_swtmr.h>
#include <los_sys.h>

/*
 * syscall number bits:
 *
 *     8-bits        8-bits               16-bits
 * ------------------------------------------------------------
 * |           |               |                              |
 * |  module#  |  args_on_sp#  |           routine#           |
 * |           |               |                              |
 * ------------------------------------------------------------
 * 31          23              15                             0
 */

#define SVC_NUM(mod, args, rtn)                 \
    (((mod) << 24) + (((args) & 0x0f) << 16) + ((rtn) & 0xffff))

/* module ids */

#define MOD_TASK                        0
#define MOD_MEM                         1
#define MOD_QUEUE                       2
#define MOD_EVENT                       3
#define MOD_MUX                         4
#define MOD_SEM                         5
#define MOD_SWTIMER                     6
#define MOD_SYS                         7
#define MOD_CUSTOM                      15

/* syscall numbers for task module */

#define SCN_TaskCreateOnly              SVC_NUM (MOD_TASK, 2, 0)
#define SCN_TaskCreate                  SVC_NUM (MOD_TASK, 2, 1)
#define SCN_TaskDelete                  SVC_NUM (MOD_TASK, 1, 2)
#define SCN_TaskResume                  SVC_NUM (MOD_TASK, 1, 3)
#define SCN_TaskSuspend                 SVC_NUM (MOD_TASK, 1, 4)
#define SCN_TaskDelay                   SVC_NUM (MOD_TASK, 1, 5)
#define SCN_TaskYield                   SVC_NUM (MOD_TASK, 0, 6)
#define SCN_TaskLock                    SVC_NUM (MOD_TASK, 0, 7)
#define SCN_TaskUnlock                  SVC_NUM (MOD_TASK, 0, 8)
#define SCN_CurTaskPriSet               SVC_NUM (MOD_TASK, 1, 9)
#define SCN_TaskPriSet                  SVC_NUM (MOD_TASK, 2, 10)
#define SCN_TaskPriGet                  SVC_NUM (MOD_TASK, 1, 11)
#define SCN_CurTaskIDGet                SVC_NUM (MOD_TASK, 0, 12)
#define SCN_TaskInfoGet                 SVC_NUM (MOD_TASK, 2, 13)
#define SCN_TaskStatusGet               SVC_NUM (MOD_TASK, 2, 14)
#define SCN_TaskNameGet                 SVC_NUM (MOD_TASK, 0, 15)
#define SCN_TaskInfoMonitor             SVC_NUM (MOD_TASK, 0, 16)
#define SCN_NextTaskIDGet               SVC_NUM (MOD_TASK, 0, 17)

/* syscall numbers for memory module */

#define SCN_MemboxInit                  SVC_NUM (MOD_MEM, 3, 0)
#define SCN_MemboxClr                   SVC_NUM (MOD_MEM, 2, 1)
#define SCN_MemboxAlloc                 SVC_NUM (MOD_MEM, 1, 2)
#define SCN_MemboxFree                  SVC_NUM (MOD_MEM, 2, 3)
#define SCN_MemboxStatisticsGet         SVC_NUM (MOD_MEM, 4, 4)
#define SCN_MemInit                     SVC_NUM (MOD_MEM, 2, 5)
#define SCN_MemAlloc                    SVC_NUM (MOD_MEM, 2, 6)
#define SCN_MemFree                     SVC_NUM (MOD_MEM, 2, 7)
#define SCN_MemRealloc                  SVC_NUM (MOD_MEM, 3, 8)
#define SCN_MemAllocAlign               SVC_NUM (MOD_MEM, 3, 9)
//#define SCN_MemStatisticsGet            SVC_NUM (MOD_MEM, 2, 10)
//#define SCN_MemGetMaxFreeBlkSize        SVC_NUM (MOD_MEM, 1, 11)

/* syscall numbers for queue module */

#define SCN_QueueCreate                 SVC_NUM (MOD_QUEUE, 5, 0)
#define SCN_QueueRead                   SVC_NUM (MOD_QUEUE, 4, 1)
#define SCN_QueueWrite                  SVC_NUM (MOD_QUEUE, 4, 2)
#define SCN_QueueReadCopy               SVC_NUM (MOD_QUEUE, 4, 3)
#define SCN_QueueWriteCopy              SVC_NUM (MOD_QUEUE, 4, 4)
#define SCN_QueueWriteHead              SVC_NUM (MOD_QUEUE, 4, 5)
#define SCN_QueueDelete                 SVC_NUM (MOD_QUEUE, 1, 6)
#define SCN_QueueInfoGet                SVC_NUM (MOD_QUEUE, 2, 7)

/* syscall numbers for event module */

#define SCN_EventInit                   SVC_NUM (MOD_EVENT, 1, 0)
#define SCN_EventRead                   SVC_NUM (MOD_EVENT, 4, 1)
#define SCN_EventWrite                  SVC_NUM (MOD_EVENT, 2, 2)
#define SCN_EventClear                  SVC_NUM (MOD_EVENT, 2, 3)
#define SCN_EventPoll                   SVC_NUM (MOD_EVENT, 3, 4)
#define SCN_EventDestroy                SVC_NUM (MOD_EVENT, 1, 5)

/* syscall numbers for mutex module */

#define SCN_MuxCreate                   SVC_NUM (MOD_MUX, 1, 0)
#define SCN_MuxDelete                   SVC_NUM (MOD_MUX, 1, 1)
#define SCN_MuxPend                     SVC_NUM (MOD_MUX, 2, 2)
#define SCN_MuxPost                     SVC_NUM (MOD_MUX, 1, 3)

/* syscall numbers for semaphore module */

#define SCN_SemCreate                   SVC_NUM (MOD_SEM, 2, 0)
#define SCN_BinarySemCreate             SVC_NUM (MOD_SEM, 2, 1)
#define SCN_SemDelete                   SVC_NUM (MOD_SEM, 1, 2)
#define SCN_SemPend                     SVC_NUM (MOD_SEM, 2, 3)
#define SCN_SemPost                     SVC_NUM (MOD_SEM, 1, 4)

/* syscall numbers for software timer module */

#if (LOSCFG_BASE_CORE_SWTMR_ALIGN == YES)
#define SCN_SwtmrCreate                 SVC_NUM (MOD_SWTIMER, 5, 0)
#else
#define SCN_SwtmrCreate                 SVC_NUM (MOD_SWTIMER, 7, 0)
#endif
#define SCN_SwtmrDelete                 SVC_NUM (MOD_SWTIMER, 1, 1)
#define SCN_SwtmrStart                  SVC_NUM (MOD_SWTIMER, 1, 2)
#define SCN_SwtmrStop                   SVC_NUM (MOD_SWTIMER, 1, 3)
#define SCN_SwtmrTimeGet                SVC_NUM (MOD_SWTIMER, 2, 4)

/* syscall numbers for sys module (los_sys.c/h) */

#define SCN_MS2Tick                     SVC_NUM (MOD_SYS, 1, 0)
#define SCN_Tick2MS                     SVC_NUM (MOD_SYS, 1, 1)
#define SCN_CyclePerTickGet             SVC_NUM (MOD_SYS, 0, 2)
#define SCN_TickCountGet                SVC_NUM (MOD_SYS, 0, 3)
///////////////////////////////////////////////////////////////////////////////////
#define SCN_printf                      SVC_NUM (MOD_SYS, 4, 4)
///////////////////////////////////////////////////////////////////////////////////

#if defined (__CC_ARM)

#define SYSCALL_RTN(sn, rt, name, ...)  __svc_indirect_r7(0) rt __##name (int r12, ##__VA_ARGS__);

#define LOS_TaskCreateOnly(...)         __LOS_TaskCreateOnly(SCN_TaskCreateOnly, ##__VA_ARGS__)
#define LOS_UsrTaskCreate(...)          __LOS_UsrTaskCreate(SCN_TaskCreate, ##__VA_ARGS__)
#define LOS_UsrTaskDelete(...)          __LOS_UsrTaskDelete(SCN_TaskDelete, ##__VA_ARGS__)
#define LOS_UsrTaskResume(...)          __LOS_UsrTaskResume(SCN_TaskResume, ##__VA_ARGS__)
#define LOS_UsrTaskSuspend(...)         __LOS_UsrTaskSuspend(SCN_TaskSuspend, ##__VA_ARGS__)
#define LOS_UsrTaskDelay(...)           __LOS_UsrTaskDelay(SCN_TaskDelay, ##__VA_ARGS__)
#define LOS_UsrTaskYield(...)           __LOS_UsrTaskYield(SCN_TaskYield, ##__VA_ARGS__)
#define LOS_UsrTaskLock(...)            __LOS_UsrTaskLock(SCN_TaskLock, ##__VA_ARGS__)
#define LOS_UsrTaskUnlock(...)          __LOS_UsrTaskUnlock(SCN_TaskUnlock, ##__VA_ARGS__)
#define LOS_UsrCurTaskPriSet(...)       __LOS_UsrCurTaskPriSet(SCN_CurTaskPriSet, ##__VA_ARGS__)
#define LOS_UsrTaskPriSet(...)          __LOS_UsrTaskPriSet(SCN_TaskPriSet, ##__VA_ARGS__)
#define LOS_UsrTaskPriGet(...)          __LOS_UsrTaskPriGet(SCN_TaskPriGet, ##__VA_ARGS__)
#define LOS_UsrCurTaskIDGet(...)        __LOS_UsrCurTaskIDGet(SCN_CurTaskIDGet, ##__VA_ARGS__)
#define LOS_UsrTaskInfoGet(...)         __LOS_UsrTaskInfoGet(SCN_TaskInfoGet, ##__VA_ARGS__)
#define LOS_UsrTaskStatusGet(...)       __LOS_UsrTaskStatusGet(SCN_TaskStatusGet, ##__VA_ARGS__)
#define LOS_UsrTaskNameGet(...)         __LOS_UsrTaskNameGet(SCN_TaskNameGet, ##__VA_ARGS__)
#define LOS_UsrTaskInfoMonitor(...)     __LOS_UsrTaskInfoMonitor(SCN_TaskInfoMonitor, ##__VA_ARGS__)
#define LOS_UsrNextTaskIDGet(...)       __LOS_UsrNextTaskIDGet(SCN_NextTaskIDGet, ##__VA_ARGS__)

#define LOS_UsrMemboxInit(...)          __LOS_UsrMemboxInit(SCN_MemboxInit, ##__VA_ARGS__)
#define LOS_UsrMemboxClr(...)           __LOS_UsrMemboxClr(SCN_MemboxClr, ##__VA_ARGS__)
#define LOS_UsrMemboxAlloc(...)         __LOS_UsrMemboxAlloc(SCN_MemboxAlloc, ##__VA_ARGS__)
#define LOS_UsrMemboxFree(...)          __LOS_UsrMemboxFree(SCN_MemboxFree, ##__VA_ARGS__)
#define LOS_UsrMemboxStatisticsGet(...) __LOS_UsrMemboxStatisticsGet(SCN_MemboxStatisticsGet, ##__VA_ARGS__)
#define LOS_UsrMemInit(...)             __LOS_UsrMemInit(SCN_MemInit, ##__VA_ARGS__)
#define LOS_UsrMemAlloc(...)            __LOS_UsrMemAlloc(SCN_MemAlloc, ##__VA_ARGS__)
#define LOS_UsrMemFree(...)             __LOS_UsrMemFree(SCN_MemFree, ##__VA_ARGS__)
#define LOS_UsrMemRealloc(...)          __LOS_UsrMemRealloc(SCN_MemRealloc, ##__VA_ARGS__)
#define LOS_UsrMemAllocAlign(...)       __LOS_UsrMemAllocAlign(SCN_MemAllocAlign, ##__VA_ARGS__)

#define LOS_UsrQueueCreate(...)         __LOS_UsrQueueCreate(SCN_QueueCreate, ##__VA_ARGS__)
#define LOS_UsrQueueRead(...)           __LOS_UsrQueueRead(SCN_QueueRead, ##__VA_ARGS__)
#define LOS_UsrQueueWrite(...)          __LOS_UsrQueueWrite(SCN_QueueWrite, ##__VA_ARGS__)
#define LOS_UsrQueueReadCopy(...)       __LOS_UsrQueueReadCopy(SCN_QueueReadCopy, ##__VA_ARGS__)
#define LOS_UsrQueueWriteCopy(...)      __LOS_UsrQueueWriteCopy(SCN_QueueWriteCopy, ##__VA_ARGS__)
#define LOS_UsrQueueWriteHead(...)      __LOS_UsrQueueWriteHead(SCN_QueueWriteHead, ##__VA_ARGS__)
#define LOS_UsrQueueDelete(...)         __LOS_UsrQueueDelete(SCN_QueueDelete, ##__VA_ARGS__)
#define LOS_UsrQueueInfoGet(...)        __LOS_UsrQueueInfoGet(SCN_QueueInfoGet, ##__VA_ARGS__)

#define LOS_UsrEventInit(...)           __LOS_UsrEventInit(SCN_EventInit, ##__VA_ARGS__)
#define LOS_UsrEventRead(...)           __LOS_UsrEventRead(SCN_EventRead, ##__VA_ARGS__)
#define LOS_UsrEventWrite(...)          __LOS_UsrEventWrite(SCN_EventWrite, ##__VA_ARGS__)
#define LOS_UsrEventClear(...)          __LOS_UsrEventClear(SCN_EventClear, ##__VA_ARGS__)
#define LOS_UsrEventPoll(...)           __LOS_UsrEventPoll(SCN_EventPoll, ##__VA_ARGS__)
#define LOS_UsrEventDestroy(...)        __LOS_UsrEventDestroy(SCN_EventDestroy, ##__VA_ARGS__)

#define LOS_UsrMuxCreate(...)           __LOS_UsrMuxCreate(SCN_MuxCreate, ##__VA_ARGS__)
#define LOS_UsrMuxDelete(...)           __LOS_UsrMuxDelete(SCN_MuxDelete, ##__VA_ARGS__)
#define LOS_UsrMuxPend(...)             __LOS_UsrMuxPend(SCN_MuxPend, ##__VA_ARGS__)
#define LOS_UsrMuxPost(...)             __LOS_UsrMuxPost(SCN_MuxPost, ##__VA_ARGS__)

#define LOS_UsrSemCreate(...)           __LOS_UsrSemCreate(SCN_SemCreate, ##__VA_ARGS__)
#define LOS_UsrBinarySemCreate(...)     __LOS_UsrBinarySemCreate(SCN_BinarySemCreate, ##__VA_ARGS__)
#define LOS_UsrSemDelete(...)           __LOS_UsrSemDelete(SCN_SemDelete, ##__VA_ARGS__)
#define LOS_UsrSemPend(...)             __LOS_UsrSemPend(SCN_SemPend, ##__VA_ARGS__)
#define LOS_UsrSemPost(...)             __LOS_UsrSemPost(SCN_SemPost, ##__VA_ARGS__)

#define LOS_UsrSwtmrCreate(...)         __LOS_UsrSwtmrCreate(SCN_SwtmrCreate, ##__VA_ARGS__)
#define LOS_UsrSwtmrDelete(...)         __LOS_UsrSwtmrDelete(SCN_SwtmrDelete, ##__VA_ARGS__)
#define LOS_UsrSwtmrStart(...)          __LOS_UsrSwtmrStart(SCN_SwtmrStart, ##__VA_ARGS__)
#define LOS_UsrSwtmrStop(...)           __LOS_UsrSwtmrStop(SCN_SwtmrStop, ##__VA_ARGS__)
#define LOS_UsrSwtmrTimeGet(...)        __LOS_UsrSwtmrTimeGet(SCN_SwtmrTimeGet, ##__VA_ARGS__)

#define LOS_UsrMS2Tick(...)             __LOS_UsrMS2Tick(SCN_MS2Tick, ##__VA_ARGS__)
#define LOS_UsrTick2MS(...)             __LOS_UsrTick2MS(SCN_Tick2MS, ##__VA_ARGS__)
#define LOS_UsrCyclePerTickGet(...)     __LOS_UsrCyclePerTickGet(SCN_CyclePerTickGet, ##__VA_ARGS__)
#define LOS_UsrTickCountGet(...)        __LOS_UsrTickCountGet(SCN_TickCountGet, ##__VA_ARGS__)
#define LOS_UsrPrintf(...)              __LOS_UsrPrintf(SCN_printf, ##__VA_ARGS__)

#elif defined (__GNUC__)

#define _SYSCALL_RTN(sn, rt, name, ...)                     \
    _Pragma("GCC diagnostic push")                          \
    _Pragma("GCC diagnostic ignored \"-Wreturn-type\"")     \
    __attribute__((naked))                                  \
    __attribute__((unused))                                 \
    __attribute__ ((weak)) rt name (__VA_ARGS__)            \
    {                                                       \
        __asm(                                              \
            "push {r7, lr}\n\t"                             \
            "ldr  r7, ="#sn"\n\t"                           \
            "svc  #0\n\t"                                   \
            "pop  {r7, pc}\n\t"                             \
            : : : "r0", "r1", "r2", "r3", "r12", "cc"       \
        );                                                  \
    }                                                       \
    _Pragma("GCC diagnostic pop")

#define SYSCALL_RTN(sn, rt, name, ...) _SYSCALL_RTN(sn, rt, name, ##__VA_ARGS__)

#else
#error "not supported tool"
#endif

SYSCALL_RTN (SCN_TaskCreateOnly,
             UINT32, LOS_UsrTaskCreateOnly, UINT32 *puwTaskID, TSK_INIT_PARAM_S *pstInitParam)
SYSCALL_RTN (SCN_TaskCreate,
             UINT32, LOS_UsrTaskCreate, UINT32 *puwTaskID, TSK_INIT_PARAM_S *pstInitParam)
SYSCALL_RTN (SCN_TaskDelete,
             UINT32, LOS_UsrTaskDelete, UINT32 uwTaskID)
SYSCALL_RTN (SCN_TaskResume,
             UINT32, LOS_UsrTaskResume, UINT32 uwTaskID)
SYSCALL_RTN (SCN_TaskSuspend,
             UINT32, LOS_UsrTaskSuspend, UINT32 uwTaskID)
SYSCALL_RTN (SCN_TaskDelay,
             UINT32, LOS_UsrTaskDelay, UINT32 uwTick)
SYSCALL_RTN (SCN_TaskYield,
             UINT32, LOS_UsrTaskYield)
SYSCALL_RTN (SCN_TaskLock,
             VOID, LOS_UsrTaskLock)
SYSCALL_RTN (SCN_TaskUnlock,
             VOID, LOS_UsrTaskUnlock)
SYSCALL_RTN (SCN_CurTaskPriSet,
             UINT32, LOS_UsrCurTaskPriSet, UINT16 usTaskPrio)
SYSCALL_RTN (SCN_TaskPriSet,
             UINT32, LOS_UsrTaskPriSet, UINT32 uwTaskID, UINT16 usTaskPrio)
SYSCALL_RTN (SCN_TaskPriGet,
             UINT16, LOS_UsrTaskPriGet, UINT32 uwTaskID)
SYSCALL_RTN (SCN_CurTaskIDGet,
             UINT32, LOS_UsrCurTaskIDGet)
SYSCALL_RTN (SCN_TaskInfoGet,
             UINT32, LOS_UsrTaskInfoGet, UINT32 uwTaskID, TSK_INFO_S *pstTaskInfo)
SYSCALL_RTN (SCN_TaskStatusGet,
             UINT32, LOS_UsrTaskStatusGet, UINT32 uwTaskID, UINT32* puwTaskStatus)
SYSCALL_RTN (SCN_TaskNameGet,
             CHAR *, LOS_UsrTaskNameGet, UINT32 uwTaskID)
SYSCALL_RTN (SCN_TaskInfoMonitor,
             UINT32, LOS_UsrTaskInfoMonitor)
SYSCALL_RTN (SCN_NextTaskIDGet,
             UINT32, LOS_UsrNextTaskIDGet)

SYSCALL_RTN (SCN_MemboxInit,
             UINT32, LOS_UsrMemboxInit, VOID *pBoxMem, UINT32 uwBoxSize, UINT32 uwBlkSize)
SYSCALL_RTN (SCN_MemboxClr,
             VOID, LOS_UsrMemboxClr, VOID *pBoxMem, VOID *pBox)
SYSCALL_RTN (SCN_MemboxAlloc,
             VOID *, LOS_UsrMemboxAlloc, VOID *pBoxMem)
SYSCALL_RTN (SCN_MemboxFree,
             UINT32, LOS_UsrMemboxFree, VOID *pBoxMem, VOID *pBox)
SYSCALL_RTN (SCN_MemboxStatisticsGet,
             UINT32, LOS_UsrMemboxStatisticsGet, VOID *pBoxMem, UINT32 *puwMaxBlk, UINT32 *puwBlkCnt, UINT32 *puwBlkSize)
SYSCALL_RTN (SCN_MemInit,
             UINT32, LOS_UsrMemInit, VOID *pPool, UINT32 uwSize)
SYSCALL_RTN (SCN_MemAlloc,
             VOID *, LOS_UsrMemAlloc , VOID *pPool, UINT32  size)
SYSCALL_RTN (SCN_MemFree,
             UINT32, LOS_UsrMemFree, VOID *pPool, VOID *pMem)
SYSCALL_RTN (SCN_MemRealloc,
             VOID *, LOS_UsrMemRealloc, VOID *pPool, VOID *pPtr, UINT32 uwSize)
SYSCALL_RTN (SCN_MemAllocAlign,
             VOID *, LOS_UsrMemAllocAlign , VOID *pPool, UINT32 uwSize, UINT32 uwBoundary)

typedef struct
{
     CHAR * pcQueueName;
     UINT16 usLen;
     UINT32 *puwQueueID;
     UINT32 uwFlags;
     UINT16 usMaxMsgSize;
} QUEUE_INIT_PARAM_S;


SYSCALL_RTN (SCN_QueueCreate,
             UINT32, LOS_UsrQueueCreate, QUEUE_INIT_PARAM_S *pstInitParam)
SYSCALL_RTN (SCN_QueueRead,
             UINT32, LOS_UsrQueueRead, UINT32 uwQueueID, VOID *pBufferAddr, UINT32 uwBufferSize, UINT32 uwTimeOut)
SYSCALL_RTN (SCN_QueueWrite,
             UINT32, LOS_UsrQueueWrite, UINT32 uwQueueID, VOID *pBufferAddr, UINT32 uwBufferSize, UINT32 uwTimeOut)
SYSCALL_RTN (SCN_QueueReadCopy,
             UINT32, LOS_UsrQueueReadCopy, UINT32 uwQueueID, VOID *pBufferAddr, UINT32 *puwBufferSize, UINT32 uwTimeOut)
SYSCALL_RTN (SCN_QueueWriteCopy,
             UINT32, LOS_UsrQueueWriteCopy, UINT32 uwQueueID, VOID *pBufferAddr, UINT32 uwBufferSize, UINT32 uwTimeOut)
SYSCALL_RTN (SCN_QueueWriteHead,
             UINT32, LOS_UsrQueueWriteHead, UINT32 uwQueueID, VOID *pBufferAddr, UINT32 uwBufferSize, UINT32 uwTimeOut)
SYSCALL_RTN (SCN_QueueDelete,
             UINT32, LOS_UsrQueueDelete, UINT32 uwQueueID)
SYSCALL_RTN (SCN_QueueInfoGet,
             UINT32, LOS_UsrQueueInfoGet, UINT32 uwQueueID, QUEUE_INFO_S *pstQueueInfo)

SYSCALL_RTN (SCN_EventInit,
             UINT32, LOS_UsrEventInit, PEVENT_CB_S pstEventCB)
SYSCALL_RTN (SCN_EventRead,
             UINT32, LOS_UsrEventRead, PEVENT_CB_S pstEventCB, UINT32 uwEventMask, UINT32 uwMode, UINT32 uwTimeOut)
SYSCALL_RTN (SCN_EventWrite,
             UINT32, LOS_UsrEventWrite, PEVENT_CB_S pstEventCB, UINT32 uwEvents)
SYSCALL_RTN (SCN_EventClear,
             UINT32, LOS_UsrEventClear, PEVENT_CB_S pstEventCB, UINT32 uwEvents)
SYSCALL_RTN (SCN_EventPoll,
             UINT32, LOS_UsrEventPoll, UINT32 *uwEventID, UINT32 uwEventMask, UINT32 uwMode)
SYSCALL_RTN (SCN_EventDestroy,
             UINT32, LOS_UsrEventDestroy, PEVENT_CB_S pstEventCB)

SYSCALL_RTN (SCN_MuxCreate,
             UINT32, LOS_UsrMuxCreate, UINT32 *puwMuxHandle)
SYSCALL_RTN (SCN_MuxDelete,
             UINT32, LOS_UsrMuxDelete, UINT32 puwMuxHandle)
SYSCALL_RTN (SCN_MuxPend,
             UINT32, LOS_UsrMuxPend, UINT32 uwMuxHandle, UINT32 uwTimeout)
SYSCALL_RTN (SCN_MuxPost,
             UINT32, LOS_UsrMuxPost, UINT32 uwMuxHandle)

SYSCALL_RTN (SCN_SemCreate,
             UINT32, LOS_UsrSemCreate, UINT16 usCount, UINT32 *puwSemHandle)
SYSCALL_RTN (SCN_BinarySemCreate,
             UINT32, LOS_UsrBinarySemCreate , UINT16 usCount, UINT32 *puwSemHandle)
SYSCALL_RTN (SCN_SemDelete,
             UINT32, LOS_UsrSemDelete, UINT32 uwSemHandle)
SYSCALL_RTN (SCN_SemPend,
             UINT32, LOS_UsrSemPend, UINT32 uwSemHandle, UINT32 uwTimeout)
SYSCALL_RTN (SCN_SemPost,
             UINT32, LOS_UsrSemPost, UINT32 uwSemHandle)

typedef struct
{
     UINT32 uwInterval;
     UINT8 ucMode;
     SWTMR_PROC_FUNC pfnHandler;
     UINT16 *pusSwTmrID;
     UINT32 uwArg;
#if (LOSCFG_BASE_CORE_SWTMR_ALIGN == YES)
     UINT8 ucRouses;
     UINT8 ucSensitive;
#endif
} SWTMR_INIT_PARAM_S;

SYSCALL_RTN (SCN_SwtmrCreate,
             UINT32, LOS_UsrSwtmrCreate, SWTMR_INIT_PARAM_S *pstInitParam)
SYSCALL_RTN (SCN_SwtmrDelete,
             UINT32, LOS_UsrSwtmrDelete, UINT16 usSwTmrID)
SYSCALL_RTN (SCN_SwtmrStart,
             UINT32, LOS_UsrSwtmrStart, UINT16 usSwTmrID)
SYSCALL_RTN (SCN_SwtmrStop,
             UINT32, LOS_UsrSwtmrStop, UINT16 usSwTmrID)
SYSCALL_RTN (SCN_SwtmrTimeGet,
             UINT32, LOS_UsrSwtmrTimeGet, UINT16 usSwTmrID, UINT32 *uwTick)

SYSCALL_RTN (SCN_MS2Tick,
             UINT32, LOS_UsrMS2Tick, UINT32 uwMillisec)
SYSCALL_RTN (SCN_Tick2MS,
             UINT32, LOS_UsrTick2MS, UINT32 uwTick)
SYSCALL_RTN (SCN_CyclePerTickGet,
             UINT32, LOS_UsrCyclePerTickGet)
SYSCALL_RTN (SCN_TickCountGet,
             UINT64, LOS_UsrTickCountGet)
SYSCALL_RTN (SCN_printf,
             int, LOS_UsrPrintf ,const char * format, int a1, int a2, int a3)

#endif

