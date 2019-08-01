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

#include <stdio.h>

#include <los_config.h>

#include <los_task.h>
#include <los_membox.h>
#include <los_memory.h>
#include <los_queue.h>
#include <los_event.h>
#include <los_mux.h>
#include <los_sem.h>
#include <los_swtmr.h>
#include <los_sys.h>

#include "los_svc.h"

typedef uint32_t (* syscall_rtn_t) (uint32_t);

const syscall_rtn_t svc_tbl_task [] =
{
#if (LOSCFG_STATIC_TASK == NO)
    (syscall_rtn_t) LOS_TaskCreateOnly,
    (syscall_rtn_t) LOS_TaskCreate,
#else
    (syscall_rtn_t) NULL,
    (syscall_rtn_t) NULL,
#endif
    (syscall_rtn_t) LOS_TaskDelete,
    (syscall_rtn_t) LOS_TaskResume,
    (syscall_rtn_t) LOS_TaskSuspend,
    (syscall_rtn_t) LOS_TaskDelay,
    (syscall_rtn_t) LOS_TaskYield,
    (syscall_rtn_t) LOS_TaskLock,
    (syscall_rtn_t) LOS_TaskUnlock,
    (syscall_rtn_t) LOS_CurTaskPriSet,
    (syscall_rtn_t) LOS_TaskPriSet,
    (syscall_rtn_t) LOS_TaskPriGet,
    (syscall_rtn_t) LOS_CurTaskIDGet,
    (syscall_rtn_t) LOS_TaskInfoGet,
    (syscall_rtn_t) LOS_TaskStatusGet,
    (syscall_rtn_t) LOS_TaskNameGet,
    (syscall_rtn_t) LOS_TaskInfoMonitor,
    (syscall_rtn_t) LOS_NextTaskIDGet,
};

const syscall_rtn_t svc_tbl_mem [] =
{
    (syscall_rtn_t) LOS_MemboxInit,
    (syscall_rtn_t) LOS_MemboxClr,
    (syscall_rtn_t) LOS_MemboxAlloc,
    (syscall_rtn_t) LOS_MemboxFree,
    (syscall_rtn_t) LOS_MemboxStatisticsGet,
    (syscall_rtn_t) LOS_MemInit,
    (syscall_rtn_t) LOS_MemAlloc,
    (syscall_rtn_t) LOS_MemFree,
    (syscall_rtn_t) LOS_MemRealloc,
    (syscall_rtn_t) LOS_MemAllocAlign,
};

#if (LOSCFG_BASE_IPC_QUEUE == YES)
    
static UINT32 LOS_QueueCreateSvc (QUEUE_INIT_PARAM_S *pstInitParam)
{
    return LOS_QueueCreate(pstInitParam->pcQueueName, pstInitParam->usLen, 
                           pstInitParam->puwQueueID, pstInitParam->uwFlags,
                           pstInitParam->usMaxMsgSize);
}

const syscall_rtn_t svc_tbl_queue [] =
{
    (syscall_rtn_t) LOS_QueueCreateSvc,
    (syscall_rtn_t) LOS_QueueRead,
    (syscall_rtn_t) LOS_QueueWrite,
    (syscall_rtn_t) LOS_QueueReadCopy,
    (syscall_rtn_t) LOS_QueueWriteCopy,
    (syscall_rtn_t) LOS_QueueWriteHead,
    (syscall_rtn_t) LOS_QueueDelete,
    (syscall_rtn_t) LOS_QueueInfoGet,
};
#endif

const syscall_rtn_t svc_tbl_event [] =
{
    (syscall_rtn_t) LOS_EventInit,
    (syscall_rtn_t) LOS_EventRead,
    (syscall_rtn_t) LOS_EventWrite,
    (syscall_rtn_t) LOS_EventClear,
    (syscall_rtn_t) LOS_EventPoll,
    (syscall_rtn_t) LOS_EventDestroy,
};

#if (LOSCFG_BASE_IPC_MUX == YES)
const syscall_rtn_t svc_tbl_mux [] =
{
    (syscall_rtn_t) LOS_MuxCreate,
    (syscall_rtn_t) LOS_MuxDelete,
    (syscall_rtn_t) LOS_MuxPend,
    (syscall_rtn_t) LOS_MuxPost,
};
#endif

#if (LOSCFG_BASE_IPC_SEM == YES)
const syscall_rtn_t svc_tbl_sem [] =
{
    (syscall_rtn_t) LOS_SemCreate,
    (syscall_rtn_t) LOS_BinarySemCreate,
    (syscall_rtn_t) LOS_SemDelete,
    (syscall_rtn_t) LOS_SemPend,
    (syscall_rtn_t) LOS_SemPost,
};
#endif

#if (LOSCFG_BASE_CORE_SWTMR == YES)
    
static UINT32 LOS_SwtmrCreateSvc (SWTMR_INIT_PARAM_S *pstInitParam)
{
    return LOS_SwtmrCreate(pstInitParam->uwInterval, pstInitParam->ucMode, 
                           pstInitParam->pfnHandler, pstInitParam->pusSwTmrID,
                           pstInitParam->uwArg
#if (LOSCFG_BASE_CORE_SWTMR_ALIGN == YES)
                           , pstInitParam->ucRouses, pstInitParam->ucSensitive
#endif
                           );
}
    
const syscall_rtn_t svc_tbl_swtimer [] =
{
    (syscall_rtn_t) LOS_SwtmrCreateSvc,
    (syscall_rtn_t) LOS_SwtmrDelete,
    (syscall_rtn_t) LOS_SwtmrStart,
    (syscall_rtn_t) LOS_SwtmrStop,
    (syscall_rtn_t) LOS_SwtmrTimeGet,
};
#endif

const syscall_rtn_t svc_tbl_sys [] =
{
    (syscall_rtn_t) LOS_MS2Tick,
    (syscall_rtn_t) LOS_Tick2MS,
    (syscall_rtn_t) LOS_CyclePerTickGet,
    (syscall_rtn_t) LOS_TickCountGet,
    (syscall_rtn_t) printf,
};

WEAK const syscall_rtn_t svc_tbl_custom [] =
{
    (syscall_rtn_t) NULL,
    (syscall_rtn_t) NULL,
    (syscall_rtn_t) NULL,
    (syscall_rtn_t) NULL,
    (syscall_rtn_t) NULL,
};

const syscall_rtn_t * svc_tables [] =
{
    svc_tbl_task,
    svc_tbl_mem,
#if (LOSCFG_BASE_IPC_QUEUE == YES)
    svc_tbl_queue,
#else
    NULL,
#endif
    svc_tbl_event,
#if (LOSCFG_BASE_IPC_MUX == YES)
    svc_tbl_mux,
#else
    NULL,
#endif
#if (LOSCFG_BASE_IPC_SEM == YES)
    svc_tbl_sem,
#else
    NULL
#endif
#if (LOSCFG_BASE_CORE_SWTMR == YES)
    svc_tbl_swtimer,
#else
    NULL,
#endif
    svc_tbl_sys,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    svc_tbl_custom,
    NULL,
};

