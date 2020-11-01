/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: LiteOS Trace Module Implementation HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2019-08-30
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

#ifndef _LOS_TRACE_H
#define _LOS_TRACE_H

#include "los_base.h"
#include "los_task_pri.h"

#if (LOSCFG_KERNEL_SMP == YES)
#include "los_mp_pri.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#if (LOSCFG_TRACE_CONTROL_AGENT == YES)
#define LOSCFG_TRACE_TASK_PRIORITY                              2
#endif

#define LOSCFG_TRACE_FRAME_CORE_MSG                             NO
#define LOSCFG_TRACE_FRAME_EVENT_COUNT                          NO
#define LOSCFG_TRACE_FRAME_MAX_PARAMS                           0

#define LOSCFG_TRACE_OBJ_MAX_NAME_SIZE                          LOS_TASK_NAMELEN
#define LOSCFG_TRACE_OBJ_MAX_NUM                                0 // LOSCFG_BASE_CORE_TSK_LIMIT

#define LOSCFG_TRACE_TLV_BUF_SIZE                               100

#define LOSCFG_TRACE_TICK_HWI                                   OS_TICK_INT_NUM
#define LOSCFG_TRACE_UART_HWI                                   NUM_HAL_INTERRUPT_UART

#if (LOSCFG_KERNEL_SMP == YES)
#define LOSCFG_TRACE_IPI_SCHEDULE_HWI                           LOS_MP_IPI_SCHEDULE
#endif

/**
 * @ingroup los_trace
 * Trace error code: init trace failed.
 *
 * Value: 0x02001400
 *
 * Solution: Follow the trace State Machine.
 */
#define LOS_ERRNO_TRACE_ERROR_STATUS               LOS_ERRNO_OS_ERROR(LOS_MOD_TRACE, 0x00)

/**
 * @ingroup los_trace
 * Trace error code: Insufficient memory for trace buf init.
 *
 * Value: 0x02001401
 *
 * Solution: Decrease the maximum number of tasks.
 */
#define LOS_ERRNO_TRACE_NO_MEMORY                  LOS_ERRNO_OS_ERROR(LOS_MOD_TRACE, 0x01)

/**
 * @ingroup los_trace
 * Trace error code: Insufficient memory for trace struct.
 *
 * Value: 0x02001402
 *
 * Solution: Increase trace buffer's size.
 */
#define LOS_ERRNO_TRACE_BUF_TOO_SMALL              LOS_ERRNO_OS_ERROR(LOS_MOD_TRACE, 0x02)

/**
 * @ingroup los_trace
 * Trace mask
 */
typedef enum {
    TRACE_SYS_FLAG      = 0x10,
    TRACE_HWI_FLAG      = 0x20,
    TRACE_TASK_FLAG     = 0x40,
    TRACE_SWTMR_FLAG    = 0x80,
    TRACE_MEM_FLAG      = 0x100,
    TRACE_QUE_FLAG      = 0x200,
    TRACE_EVENT_FLAG    = 0x400,
    TRACE_SEM_FLAG      = 0x800,
    TRACE_MUX_FLAG      = 0x1000,

    TRACE_MAX_FLAG      = 0x80000000,
} LOS_TRACE_MASK;

/**
 * @ingroup los_trace
 * Trace event type
 */
typedef enum {
    /* 0x1000~0x1FFF */
    TRACE_TASK_CREATE           = TRACE_TASK_FLAG | 0,
    TRACE_TASK_PRIOSET          = TRACE_TASK_FLAG | 1,
    TRACE_TASK_DELETE           = TRACE_TASK_FLAG | 2,
    TRACE_TASK_SUSPEND          = TRACE_TASK_FLAG | 3,
    TRACE_TASK_RESUME           = TRACE_TASK_FLAG | 4,
    TRACE_TASK_DELAY            = TRACE_TASK_FLAG | 5,
    TRACE_TASK_SWITCH           = TRACE_TASK_FLAG | 6,
    TRACE_TASK_READY            = TRACE_TASK_FLAG | 7,
    TRACE_TASK_UNREADY          = TRACE_TASK_FLAG | 8,

    /* 0x800~0xFFF */
    TRACE_SWTMR_CREATE          = TRACE_SWTMR_FLAG | 0,
    TRACE_SWTMR_DELETE          = TRACE_SWTMR_FLAG | 1,
    TRACE_SWTMR_START           = TRACE_SWTMR_FLAG | 2,
    TRACE_SWTMR_STOP            = TRACE_SWTMR_FLAG | 3,
    TRACE_SWTMR_EXPIRED         = TRACE_SWTMR_FLAG | 4,
    TRACE_SWTMR_FUNCIN          = TRACE_SWTMR_FLAG | 5,

    /* 0x400~0x7FF */
    TRACE_HWI_CREATE            = TRACE_HWI_FLAG | 0,
    TRACE_HWI_DELETE            = TRACE_HWI_FLAG | 1,
    TRACE_HWI_RESPONSE_IN       = TRACE_HWI_FLAG | 2,
    TRACE_HWI_RESPONSE_OUT      = TRACE_HWI_FLAG | 3,

    /* 0x200~0x3FF */
    TRACE_MEM_ALLOC             = TRACE_MEM_FLAG | 0,
    TRACE_MEM_FREE              = TRACE_MEM_FLAG | 1,
    TRACE_MEM_INFO_REQ          = TRACE_MEM_FLAG | 2,
    TRACE_MEM_INFO              = TRACE_MEM_FLAG | 3,

    /* 0x100~0x1FF */
    TRACE_QUEUE_CREATE          = TRACE_QUE_FLAG | 0,
    TRACE_QUEUE_DELETE          = TRACE_QUE_FLAG | 1,
    TRACE_QUEUE_READ            = TRACE_QUE_FLAG | 2,
    TRACE_QUEUE_WRITE           = TRACE_QUE_FLAG | 3,
    TRACE_QUEUE_READ_BLOCK      = TRACE_QUE_FLAG | 4,
    TRACE_QUEUE_WRITE_BLOCK     = TRACE_QUE_FLAG | 5,
    TRACE_QUEUE_MAIL_BLOCK      = TRACE_QUE_FLAG | 6,

    /* 0x80~0xFF */
    TRACE_EVENT_CREATE          = TRACE_EVENT_FLAG | 0,
    TRACE_EVENT_DELETE          = TRACE_EVENT_FLAG | 1,
    TRACE_EVENT_READ            = TRACE_EVENT_FLAG | 2,
    TRACE_EVENT_WRITE           = TRACE_EVENT_FLAG | 3,
    TRACE_EVENT_CLEAR           = TRACE_EVENT_FLAG | 4,
    TRACE_EVENT_POLL            = TRACE_EVENT_FLAG | 5,
    TRACE_EVENT_READ_BLOCK      = TRACE_EVENT_FLAG | 6,

    /* 0x40~0x7F */
    TRACE_SEM_CREATE            = TRACE_SEM_FLAG | 0,
    TRACE_SEM_DELETE            = TRACE_SEM_FLAG | 1,
    TRACE_SEM_PEND              = TRACE_SEM_FLAG | 2,
    TRACE_SEM_POST              = TRACE_SEM_FLAG | 3,
    TRACE_SEM_PEND_BLOCK        = TRACE_SEM_FLAG | 4,

    /* 0x20~0x3F */
    TRACE_MUX_CREATE            = TRACE_MUX_FLAG | 0,
    TRACE_MUX_DELETE            = TRACE_MUX_FLAG | 1,
    TRACE_MUX_PEND              = TRACE_MUX_FLAG | 2,
    TRACE_MUX_POST              = TRACE_MUX_FLAG | 3,
    TRACE_MUX_PEND_BLOCK        = TRACE_MUX_FLAG | 4,

    /* 0x10~0x1F */
    TRACE_SYS_ERROR             = TRACE_SYS_FLAG | 0,
    TRACE_SYS_START             = TRACE_SYS_FLAG | 1,
    TRACE_SYS_STOP              = TRACE_SYS_FLAG | 2,
} LOS_TRACE_TYPE;

/**
 * @ingroup los_trace
 * struct to store the trace config information.
 */
typedef struct {
    UINT32 bigLittleEndian;     /**< big little endian flag */
    UINT32 clockFreq;           /**< system clock frequency */
    UINT32 version;             /**< trace version */
} TraceBaseHeaderInfo;

/**
 * @ingroup los_trace
 * struct to store the event infomation
 */
typedef struct {
    UINT32  eventType;
    UINT32  curTask;
    UINT64  curTime;
    UINTPTR identity;
#if (LOSCFG_TRACE_FRAME_CORE_MSG == YES)
    struct CoreStatus {
        UINT32 cpuId      : 8,
               hwiActive  : 4,
               isTaskLock : 4,
               paramCount : 4,
               reserves   : 12;
    } core;
#endif

#if (LOSCFG_TRACE_FRAME_EVENT_COUNT == YES)
    UINT32  eventCount;
#endif
    UINTPTR params[LOSCFG_TRACE_FRAME_MAX_PARAMS];
} TraceEventFrame;

/**
 * @ingroup los_trace
 * struct to store the kernel obj information
 */
typedef struct {
    UINT32      id;                                     /**< kernel obj's id */
    UINT32      prio;                                   /**< kernel obj's prio */
    CHAR        name[LOSCFG_TRACE_OBJ_MAX_NAME_SIZE];   /**< kernel obj's name */
} ObjData;

/**
 * @ingroup los_trace
 * struct to store the trace data.
 */
typedef struct {
    TraceBaseHeaderInfo baseInfo;
    UINT16 totalLen;
    UINT16 objSize;
    UINT16 frameSize;
    UINT16 objOffset;
    UINT16 frameOffset;
} OfflineHead;

typedef BOOL (*TRACE_HWI_FILTER_HOOK)(UINT32 hwiNum);

extern VOID OsTraceHook(UINT32 eventType, UINTPTR identity, const UINTPTR *params, UINT16 paramCount);

/**
 * @ingroup los_trace
 * Trace event params:
 1. Configure the macro without parameters so as not to record events of this type;
 2. Configure the macro at least with one parameter to record this type of event;
 3. User can add parameters according to the structure type of the variable;
 * @attention
 * <ul>
 * <li>Cause the first param was Casted to UINTPTR, do not enclose it with parentheses.</li>
 * <li>The structure member variables of the first param were treated as UINTPTR too.</li>
 * </ul>
 eg. TASK_CREATE_PARAMS(taskCB)         taskCB, taskCB->taskID, taskCB->stackSize, taskCB->priority
 eg. SWTMR_CREATE_PARAMS(swtmr)         swtmr, swtmr->ucMode, swtmr->uwInterval, (UINTPTR)(swtmr->pfnHandler)
 eg. EVENT_CREATE_PARAMS(eventCB)       eventCB, eventCB->uwEventID
 eg. MEM_INFO_PARAMS(pool, status)      pool, (status)->uwTotalUsedSize, (status)->uwTotalFreeSize
 */
#define TASK_SWITCH_PARAMS(taskCB)                      taskCB
#define TASK_READY_PARAMS(taskCB)                       taskCB
#define TASK_PRIOSET_PARAMS(taskCB)                     taskCB, taskCB->priority
#define TASK_CREATE_PARAMS(taskCB)                      taskCB, taskCB->priority
#define TASK_RESUME_PARAMS(taskCB)                      taskCB
#define TASK_SUSPEND_PARAMS(taskCB)                     taskCB
#define TASK_DELETE_PARAMS(taskCB)                      taskCB
#define TASK_DELAY_PARAMS(taskCB, tick)                 taskCB, tick

#define SWTMR_FUNCIN_PARAMS(handler, tick)              handler, tick
#define SWTMR_START_PARAMS(swtmr)                       swtmr, swtmr->uwInterval
#define SWTMR_DELETE_PARAMS(swtmr)                      swtmr
#define SWTMR_EXPIRED_PARAMS(swtmr)                     swtmr, (UINTPTR)(swtmr->pfnHandler)
#define SWTMR_STOP_PARAMS(swtmr)                        swtmr
#define SWTMR_CREATE_PARAMS(swtmr)                      swtmr

#define HWI_RESPONSE_IN_PARAMS(intNum)                  intNum
#define HWI_RESPONSE_OUT_PARAMS(intNum)                 intNum
#define HWI_CREATE_PARAMS(intNum, prio, handler, irqDevId)      intNum, prio, (UINTPTR)handler, (UINTPTR)irqDevId
#define HWI_DELETE_PARAMS(intNum, irqDevId)             intNum, irqDevId


#define EVENT_CREATE_PARAMS(eventCB)                    eventCB
#define EVENT_READ_BLOCK_PARAMS(eventCB, events, timeout)   eventCB, events, timeout
#define EVENT_READ_PARAMS(eventCB, events)              eventCB, events
#define EVENT_WRITE_PARAMS(eventCB, events)             eventCB, events
#define EVENT_POLL_PARAMS(eventID, events)              eventID, events
#define EVENT_DELETE_PARAMS(eventCB)                    eventCB
#define EVENT_CLEAR_PARAMS(eventCB, events)             eventCB, events

#define QUEUE_CREATE_PARAMS(queueCB)                    queueCB, queueCB->queueLen, queueCB->queueSize
#define QUEUE_READ_BLOCK_PARAMS(queueCB, timeout)       queueCB, timeout
#define QUEUE_WRITE_BLOCK_PARAMS(queueCB, timeout)      queueCB, timeout
#define QUEUE_READ_PARAMS(queueCB)                      queueCB
#define QUEUE_WRITE_PARAMS(queueCB)                     queueCB
#define QUEUE_DELETE_PARAMS(queueCB)                    queueCB

#define SEM_CREATE_PARAMS(semCB)                        semCB, semCB->semCount, semCB->maxSemCount
#define SEM_DELETE_PARAMS(semCB)                        semCB
#define SEM_PEND_BLOCK_PARAMS(semCB, timeout)           semCB, timeout
#define SEM_PEND_PARAMS(semCB)                          semCB
#define SEM_POST_PARAMS(semCB)                          semCB

#define MUX_CREATE_PARAMS(muxCB)                        muxCB
#define MUX_DELETE_PARAMS(muxCB)                        muxCB
#define MUX_PEND_BLOCK_PARAMS(muxCB, timeout)           muxCB, timeout
#define MUX_PEND_PARAMS(muxCB)                          muxCB
#define MUX_POST_PARAMS(muxCB)                          muxCB

#define MEM_ALLOC_PARAMS(ptr, size)                     ptr, size
#define MEM_FREE_PARAMS(ptr, size)                      ptr
#define MEM_INFO_REQ_PARAMS(pool)                       pool
#define MEM_INFO_PARAMS(pool, status)                   pool, (status)->uwTotalUsedSize, (status)->uwTotalFreeSize

#define SYS_ERROR_PARAMS(errno)                         errno

#if (LOSCFG_KERNEL_TRACE == YES)
#define LOS_TRACE(MOD, OPS, IDENTITY, ...)                                                \
    do {                                                                                  \
        UINTPTR _inner[] = {0, MOD##_##OPS##_PARAMS((UINTPTR)IDENTITY, ##__VA_ARGS__)};   \
        UINTPTR _n = sizeof(_inner) / sizeof(UINTPTR);                                    \
        if (_n > 1) {                                                                     \
            OsTraceHook(TRACE_##MOD##_##OPS, (UINTPTR)IDENTITY, &_inner[1], _n - 1);      \
        }                                                                                 \
    } while (0)
#else
#define LOS_TRACE(MOD, OPS, ...)
#endif

/**
 * @ingroup los_trace
 * @brief Intialize the trace when the system startup.
 *
 * @par Description:
 * This API is used to intilize the trace for system level.
 * @attention
 * <ul>
 * <li>This API can be called only after the memory is initialized. Otherwise, the Trace Init will be fail.</li>
 * </ul>
 *
 * @param buf        [IN] Type #VOID *. The ptr is trace buffer address, if ptr is NULL, system will malloc a new one in
 *                                  trace offline mode.
 * @param size       [IN] Type #UINT32. The trace buffer's size.
 *
 * @retval #LOS_ERRNO_TRACE_ERROR_STATUS        0x02001400: The trace status is not TRACE_UNINIT.
 * @retval #LOS_ERRNO_TRACE_NO_MEMORY           0x02001401: The memory is not enough for initilize.
 * @retval #LOS_ERRNO_TRACE_BUF_TOO_SMALL       0x02001402: Trace buf size not enough.
 * @retval #LOS_ERRNO_TSK_TCB_UNAVAILABLE       0x02000211: No free task control block is available.
 * @retval #LOS_ERRNO_TSK_MP_SYNC_RESOURCE      0x02000225: Mp sync resource create failed
 * @retval #LOS_OK                              0x00000000: The intialization is successful.
 * @par Dependency:
 * <ul><li>los_trace.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_TraceInit
 * @since Huawei LiteOS V200R005C00
 */
extern UINT32 LOS_TraceInit(VOID *buf, UINT32 size);

/**
 * @ingroup los_trace
 * @brief Start trace.
 *
 * @par Description:
 * This API is used to start trace.
 * @attention
 * <ul>
 * <li>Start trace</li>
 * </ul>
 *
 * @param  None.
 * @retval #LOS_ERRNO_TRACE_ERROR_STATUS        0x02001400: Trace start failed.
 * @retval #LOS_OK                              0x00000000: Trace start success.
 *
 * @par Dependency:
 * <ul><li>los_trace.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_TraceStart
 * @since Huawei LiteOS V200R005C00
 */
extern UINT32 LOS_TraceStart(VOID);

/**
 * @ingroup los_trace
 * @brief Stop trace sample.
 *
 * @par Description:
 * This API is used to start trace sample.
 * @attention
 * <ul>
 * <li>Stop trace sample</li>
 * </ul>
 *
 * @param  None.
 * @retval #None.
 *
 * @par Dependency:
 * <ul><li>los_trace.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_TraceStop
 * @since Huawei LiteOS V200R005C00
 */
extern VOID LOS_TraceStop(VOID);

/**
 * @ingroup los_trace
 * @brief Clear the trace buf.
 *
 * @par Description:
 * Clear the event frames in trace buf only at offline mode.
 * @attention
 * <ul>
 * <li>This API can be called only after that trace buffer has been established.</li>
 * Otherwise, the trace will be failed.</li>
 * </ul>
 *
 * @param  None.
 * @retval #NA
 * @par Dependency:
 * <ul><li>los_trace.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_TraceReset
 * @since Huawei LiteOS V200R005C00
 */
extern VOID LOS_TraceReset(VOID);

/**
 * @ingroup los_trace
 * @brief Set trace event mask.
 *
 * @par Description:
 * Set trace event mask.
 * @attention
 * <ul>
 * <li>Set trace event filter mask.</li>
 * <li>The Default mask is (TRACE_HWI_FLAG | TRACE_TASK_FLAG), stands for switch on task and hwi events.</li>
 * <li>Customize mask according to the type defined in enum LOS_TRACE_MASK to switch on corresponding module's
 * trace.</li>
 * <li>The system's trace mask will be overrode by the input parameter.</li>
 * </ul>
 *
 * @param  mask [IN] Type #UINT32. The mask used to filter events of LOS_TRACE_MASK.
 * @retval #NA.
 * @par Dependency:
 * <ul><li>los_trace.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_TraceEventMaskSet
 * @since Huawei LiteOS V200R005C00
 */
extern VOID LOS_TraceEventMaskSet(UINT32 mask);

/**
 * @ingroup los_trace
 * @brief Offline trace buffer export.
 *
 * @par Description:
 * Return the trace buf only at offline mode.
 * @attention
 * <ul>
 * <li>This API can be called only after that trace buffer has been established.</li>
 * Otherwise, the trace will be failed.</li>
 * <li>Trace data will be send to pipeline when user set toClient = TRUE.</li>
 * <li>The return buffer's address is a critical resource, user can only ready.</li>
 * </ul>
 *
 * @param toClient           [IN] Type #BOOL. Whether send trace data to Client through pipeline.
 * @retval #VOID*            The trace buffer's address, analyze this buffer according to the structure of OfflineHead.
 *
 * @par Dependency:
 * <ul><li>los_trace.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_TraceRecordDump
 * @since Huawei LiteOS V200R005C00
 */
extern VOID *LOS_TraceRecordDump(BOOL toClient);

/**
 * @ingroup los_trace
 * @brief Hwi num fliter hook.
 *
 * @par Description:
 * Hwi fliter function.
 * @attention
 * <ul>
 * <li>Filter the hwi events by hwi num</li>
 * </ul>
 *
 * @param  hook [IN] Type #TRACE_HWI_FILTER_HOOK. The user defined hook for hwi num filter,
 *                             the hook should return true if you don't want trace this hwi num.
 * @retval #None
 * @par Dependency:
 * <ul><li>los_trace.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_TraceHwiFilterHookReg
 * @since Huawei LiteOS V200R005C00
 */
extern VOID LOS_TraceHwiFilterHookReg(TRACE_HWI_FILTER_HOOK hook);
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_TRACE_H */
