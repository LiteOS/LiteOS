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

/**
 * @defgroup los_trace Trace
 * @ingroup kernel
 */

#ifndef _LOS_TRACE_H
#define _LOS_TRACE_H

#include "los_base.h"
#include "los_task.h"

#include "los_perf.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_TRACE_CONTROL_AGENT

/**
 * @ingroup los_trace
 * Trace Control agent task's priority.
 */
#define LOSCFG_TRACE_TASK_PRIORITY                              2
#endif

/**
 * @ingroup los_trace
 * Whether trace records key informations, including cpuid, hardware interrupt status, task lock status.
 */
#define LOSCFG_TRACE_FRAME_CORE_MSG                             NO

/**
 * @ingroup los_trace
 * Whether trace records event count, which can indicate the sequence of happend events and the event loss count.
 */
#define LOSCFG_TRACE_FRAME_EVENT_COUNT                          NO

/**
 * @ingroup los_trace
 * Trace records the max params number in struct TraceEventFrame of events. if set to 0, trace will just record basic
 * event's type, not record event's any params.
 */
#define LOSCFG_TRACE_FRAME_MAX_PARAMS                           3

#define LOSCFG_TRACE_OBJ_MAX_NAME_SIZE                          LOS_TASK_NAMELEN

/**
 * @ingroup los_trace
 * Trace records the max number of objects(kernel object, like tasks). if set to 0, trace will not record any object.
 */
#define LOSCFG_TRACE_OBJ_MAX_NUM                                0 // LOSCFG_BASE_CORE_TSK_LIMIT

/**
 * @ingroup los_trace
 * Trace tlv encode buffer size, the buffer is used to encode one piece raw frame to tlv message in online mode.
 */
#define LOSCFG_TRACE_TLV_BUF_SIZE                               100

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
 * Solution: Expand the configured system memory or decrease the value defined by LOS_TRACE_BUFFER_SIZE.
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
 * Trace state.
 */
enum TraceState {
    TRACE_UNINIT = 0,  /**< trace isn't inited */
    TRACE_INITED,      /**< trace is inited but not started yet */
    TRACE_STARTED,     /**< trace is started and system is tracing */
    TRACE_STOPED,      /**< trace is stopped */
};

/**
 * @ingroup los_trace
 * Trace mask is used to filter events in runtime. Each mask keep only one unique bit to 1, and user can define own
 * module's trace mask.
 */
typedef enum {
    TRACE_SYS_FLAG          = 0x10,
    TRACE_HWI_FLAG          = 0x20,
    TRACE_TASK_FLAG         = 0x40,
    TRACE_SWTMR_FLAG        = 0x80,
    TRACE_MEM_FLAG          = 0x100,
    TRACE_QUE_FLAG          = 0x200,
    TRACE_EVENT_FLAG        = 0x400,
    TRACE_SEM_FLAG          = 0x800,
    TRACE_MUX_FLAG          = 0x1000,

    TRACE_MAX_FLAG          = 0x80000000,
    TRACE_USER_DEFAULT_FLAG = 0xFFFFFFF0,
} LOS_TRACE_MASK;

/**
 * @ingroup los_trace
 * Trace event type which indicate the exactly happend events, user can define own module's event type like
 * TRACE_#MODULE#_FLAG | NUMBER.
 */
typedef enum {
    /* 0x10~0x1F */
    SYS_ERROR             = TRACE_SYS_FLAG | 0,
    SYS_START             = TRACE_SYS_FLAG | 1,
    SYS_STOP              = TRACE_SYS_FLAG | 2,

    /* 0x20~0x3F */
    HWI_CREATE              = TRACE_HWI_FLAG | 0,
    HWI_CREATE_SHARE        = TRACE_HWI_FLAG | 1,
    HWI_DELETE              = TRACE_HWI_FLAG | 2,
    HWI_DELETE_SHARE        = TRACE_HWI_FLAG | 3,
    HWI_RESPONSE_IN         = TRACE_HWI_FLAG | 4,
    HWI_RESPONSE_OUT        = TRACE_HWI_FLAG | 5,
    HWI_ENABLE              = TRACE_HWI_FLAG | 6,
    HWI_DISABLE             = TRACE_HWI_FLAG | 7,
    HWI_TRIGGER             = TRACE_HWI_FLAG | 8,
    HWI_SETPRI              = TRACE_HWI_FLAG | 9,
    HWI_CLEAR               = TRACE_HWI_FLAG | 10,
    HWI_SETAFFINITY         = TRACE_HWI_FLAG | 11,
    HWI_SENDIPI             = TRACE_HWI_FLAG | 12,

    /* 0x40~0x7F */
    TASK_CREATE           = TRACE_TASK_FLAG | 0,
    TASK_PRIOSET          = TRACE_TASK_FLAG | 1,
    TASK_DELETE           = TRACE_TASK_FLAG | 2,
    TASK_SUSPEND          = TRACE_TASK_FLAG | 3,
    TASK_RESUME           = TRACE_TASK_FLAG | 4,
    TASK_SWITCH           = TRACE_TASK_FLAG | 5,
    TASK_SIGNAL           = TRACE_TASK_FLAG | 6,

     /* 0x80~0xFF */
    SWTMR_CREATE          = TRACE_SWTMR_FLAG | 0,
    SWTMR_DELETE          = TRACE_SWTMR_FLAG | 1,
    SWTMR_START           = TRACE_SWTMR_FLAG | 2,
    SWTMR_STOP            = TRACE_SWTMR_FLAG | 3,
    SWTMR_EXPIRED         = TRACE_SWTMR_FLAG | 4,

     /* 0x100~0x1FF */
    MEM_ALLOC             = TRACE_MEM_FLAG | 0,
    MEM_ALLOC_ALIGN       = TRACE_MEM_FLAG | 1,
    MEM_REALLOC           = TRACE_MEM_FLAG | 2,
    MEM_FREE              = TRACE_MEM_FLAG | 3,
    MEM_INFO_REQ          = TRACE_MEM_FLAG | 4,
    MEM_INFO              = TRACE_MEM_FLAG | 5,

     /* 0x200~0x3FF */
    QUEUE_CREATE          = TRACE_QUE_FLAG | 0,
    QUEUE_DELETE          = TRACE_QUE_FLAG | 1,
    QUEUE_RW              = TRACE_QUE_FLAG | 2,

     /* 0x400~0x7FF */
    EVENT_CREATE          = TRACE_EVENT_FLAG | 0,
    EVENT_DELETE          = TRACE_EVENT_FLAG | 1,
    EVENT_READ            = TRACE_EVENT_FLAG | 2,
    EVENT_WRITE           = TRACE_EVENT_FLAG | 3,
    EVENT_CLEAR           = TRACE_EVENT_FLAG | 4,

     /* 0x800~0xFFF */
    SEM_CREATE            = TRACE_SEM_FLAG | 0,
    SEM_DELETE            = TRACE_SEM_FLAG | 1,
    SEM_PEND              = TRACE_SEM_FLAG | 2,
    SEM_POST              = TRACE_SEM_FLAG | 3,

     /* 0x1000~0x1FFF */
    MUX_CREATE            = TRACE_MUX_FLAG | 0,
    MUX_DELETE            = TRACE_MUX_FLAG | 1,
    MUX_PEND              = TRACE_MUX_FLAG | 2,
    MUX_POST              = TRACE_MUX_FLAG | 3,
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
    UINT32  eventType;                               /**< event type */
    UINT32  curTask;                                 /**< current running task */
    UINT64  curTime;                                 /**< current timestamp */
    UINTPTR identity;                                /**< subject of the event description */
#if (LOSCFG_TRACE_FRAME_CORE_MSG == YES)
    struct CoreStatus {
        UINT32 cpuId      : 8,                       /**< cpuid */
               hwiActive  : 4,                       /**< whether is in hwi response */
               taskLockCnt : 4,                      /**< task lock count */
               paramCount : 4,                       /**< event frame params' number */
               reserves   : 12;                      /**< reserves */
    } core;
#endif

#if (LOSCFG_TRACE_FRAME_EVENT_COUNT == YES)
    UINT32  eventCount;                               /**< the sequence of happend events */
#endif
    UINTPTR params[LOSCFG_TRACE_FRAME_MAX_PARAMS];    /**< event frame's params */
} TraceEventFrame;

/**
 * @ingroup los_trace
 * struct to store the kernel obj information, we defined task as kernel obj in this system.
 */
typedef struct {
    UINT32      id;                                     /**< kernel obj's id */
    UINT32      prio;                                   /**< kernel obj's priority */
    CHAR        name[LOSCFG_TRACE_OBJ_MAX_NAME_SIZE];   /**< kernel obj's name */
} ObjData;

/**
 * @ingroup los_trace
 * struct to store the trace data.
 */
typedef struct {
    TraceBaseHeaderInfo baseInfo;          /**< basic info, include bigLittleEndian flag, system clock freq */
    UINT16 totalLen;                       /**< trace data's total length */
    UINT16 objSize;                        /**< sizeof #ObjData */
    UINT16 frameSize;                      /**< sizeof #TraceEventFrame */
    UINT16 objOffset;                      /**< the offset of the first obj data to record beginning */
    UINT16 frameOffset;                    /**< the offset of the first event frame data to record beginning */
} OfflineHead;

/**
 * @ingroup  los_trace
 * @brief Define the type of trace hardware interrupt filter hook function.
 *
 * @par Description:
 * User can register fliter function by LOS_TraceHwiFilterHookReg to filter hardware interrupt events. Return true if
 * user don't need trace the certain number.
 *
 * @attention
 * None.
 *
 * @param hwiNum        [IN] Type #UINT32. The hardware interrupt number.
 * @retval #TRUE        0x00000001: Not record the certain number.
 * @retval #FALSE       0x00000000: Need record the certain number.
 *
 * @par Dependency:
 * <ul><li>los_trace.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V200R005C00
 */
typedef BOOL (*TRACE_HWI_FILTER_HOOK)(UINT32 hwiNum);

extern VOID OsTraceHook(UINT32 eventType, UINTPTR identity, const UINTPTR *params, UINT16 paramCount);

/**
 * @ingroup los_trace
 * Trace event params:
 1. Configure the macro without parameters so as not to record events of this type;
 2. Configure the macro at least with one parameter to record this type of event;
 3. User can delete unnecessary parameters which defined in corresponding marco;
 * @attention
 * <ul>
 * <li>The first param is treat as key, keep at least this param if you want trace this event.</li>
 * <li>All parameters were treated as UINTPTR.</li>
 * </ul>
 * eg. Trace a event as:
 * #define TASK_PRIOSET_PARAMS(taskId, taskStatus, oldPrio, newPrio) taskId, taskStatus, oldPrio, newPrio
 * eg. Not Trace a event as:
 * #define TASK_PRIOSET_PARAMS(taskId, taskStatus, oldPrio, newPrio)
 * eg. Trace only you need parmas as:
 * #define TASK_PRIOSET_PARAMS(taskId, taskStatus, oldPrio, newPrio) taskId
 */
#define TASK_SWITCH_PARAMS(taskId, oldPriority, oldTaskStatus, newPriority, newTaskStatus) \
    taskId, oldPriority, oldTaskStatus, newPriority, newTaskStatus
#define TASK_PRIOSET_PARAMS(taskId, taskStatus, oldPrio, newPrio) taskId, taskStatus, oldPrio, newPrio
#define TASK_CREATE_PARAMS(taskId, taskStatus, prio)     taskId, taskStatus, prio
#define TASK_DELETE_PARAMS(taskId, taskStatus, usrStack) taskId, taskStatus, usrStack
#define TASK_SUSPEND_PARAMS(taskId, taskStatus, runTaskId) taskId, taskStatus, runTaskId
#define TASK_RESUME_PARAMS(taskId, taskStatus, prio)     taskId, taskStatus, prio
#define TASK_SIGNAL_PARAMS(taskId, signal, schedFlag)    // taskId, signal, schedFlag

#define SWTMR_START_PARAMS(swtmrId, mode, overrun, interval, expiry)  swtmrId, mode, overrun, interval, expiry
#define SWTMR_DELETE_PARAMS(swtmrId)                                  swtmrId
#define SWTMR_EXPIRED_PARAMS(swtmrId)                                 swtmrId
#define SWTMR_STOP_PARAMS(swtmrId)                                    swtmrId
#define SWTMR_CREATE_PARAMS(swtmrId)                                  swtmrId

#define HWI_CREATE_PARAMS(hwiNum, hwiPrio, hwiMode, hwiHandler) hwiNum, hwiPrio, hwiMode, hwiHandler
#define HWI_CREATE_SHARE_PARAMS(hwiNum, pDevId, ret)    hwiNum, pDevId, ret
#define HWI_DELETE_PARAMS(hwiNum)                       hwiNum
#define HWI_DELETE_SHARE_PARAMS(hwiNum, pDevId, ret)    hwiNum, pDevId, ret
#define HWI_RESPONSE_IN_PARAMS(hwiNum)                  hwiNum
#define HWI_RESPONSE_OUT_PARAMS(hwiNum)                 hwiNum
#define HWI_ENABLE_PARAMS(hwiNum)                       hwiNum
#define HWI_DISABLE_PARAMS(hwiNum)                      hwiNum
#define HWI_TRIGGER_PARAMS(hwiNum)                      hwiNum
#define HWI_SETPRI_PARAMS(hwiNum, priority)             hwiNum, priority
#define HWI_CLEAR_PARAMS(hwiNum)                        hwiNum
#define HWI_SETAFFINITY_PARAMS(hwiNum, cpuMask)         hwiNum, cpuMask
#define HWI_SENDIPI_PARAMS(hwiNum, cpuMask)             hwiNum, cpuMask

#define EVENT_CREATE_PARAMS(eventCB)                    eventCB
#define EVENT_DELETE_PARAMS(eventCB, delRetCode)        eventCB, delRetCode
#define EVENT_READ_PARAMS(eventCB, eventId, mask, mode, timeout) \
    eventCB, eventId, mask, mode, timeout
#define EVENT_WRITE_PARAMS(eventCB, eventId, events)    eventCB, eventId, events
#define EVENT_CLEAR_PARAMS(eventCB, eventId, events)    eventCB, eventId, events

#define QUEUE_CREATE_PARAMS(queueId, queueSz, itemSz, queueAddr, memType) \
    queueId, queueSz, itemSz, queueAddr, memType
#define QUEUE_DELETE_PARAMS(queueId, state, readable)   queueId, state, readable
#define QUEUE_RW_PARAMS(queueId, queueSize, bufSize, operateType, readable, writeable, timeout) \
    queueId, queueSize, bufSize, operateType, readable, writeable, timeout

#define SEM_CREATE_PARAMS(semId, type, count)           semId, type, count
#define SEM_DELETE_PARAMS(semId, delRetCode)            semId, delRetCode
#define SEM_PEND_PARAMS(semId, count, timeout)          semId, count, timeout
#define SEM_POST_PARAMS(semId, type, count)             semId, type, count

#define MUX_CREATE_PARAMS(muxId)                        muxId
#define MUX_DELETE_PARAMS(muxId, state, count, owner)   muxId, state, count, owner
#define MUX_PEND_PARAMS(muxId, count, owner, timeout)   muxId, count, owner, timeout
#define MUX_POST_PARAMS(muxId, count, owner)            muxId, count, owner

#define MEM_ALLOC_PARAMS(pool, ptr, size)                   pool, ptr, size
#define MEM_ALLOC_ALIGN_PARAMS(pool, ptr, size, boundary)   pool, ptr, size, boundary
#define MEM_REALLOC_PARAMS(pool, ptr, size)                 pool, ptr, size
#define MEM_FREE_PARAMS(pool, ptr)                          pool, ptr
#define MEM_INFO_REQ_PARAMS(pool)                           pool
#define MEM_INFO_PARAMS(pool, usedSize, freeSize)           pool, usedSize, freeSize

#define SYS_ERROR_PARAMS(errno)                         errno

#ifdef LOSCFG_KERNEL_TRACE

/**
 * @ingroup los_trace
 * @brief Trace static code stub.
 *
 * @par Description:
 * This API is used to instrument trace code stub in source code, to track events.
 * @attention
 * None.
 *
 * @param TYPE           [IN] Type #LOS_TRACE_TYPE. The event type.
 * @param IDENTITY       [IN] Type #UINTPTR. The subject of this event description.
 * @param ...            [IN] Type #UINTPTR. This piece of event's params.
 * @retval None.
 *
 * @par Dependency:
 * <ul><li>los_trace.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V200R005C00
 */
#define LOS_TRACE(TYPE, IDENTITY, ...)                                             \
    do {                                                                           \
        LOS_PERF(TYPE);                                                            \
        UINTPTR _inner[] = {0, TYPE##_PARAMS((UINTPTR)IDENTITY, ##__VA_ARGS__)};   \
        UINTPTR _n = sizeof(_inner) / sizeof(UINTPTR);                             \
        if (_n > 1) {                                                              \
            OsTraceHook(TYPE, _inner[1], _n > 2 ? &_inner[2] : NULL, _n - 2);      \
        }                                                                          \
    } while (0)
#else
#define LOS_TRACE(TYPE, ...)   LOS_PERF(TYPE)
#endif

#ifdef LOSCFG_KERNEL_TRACE

/**
 * @ingroup los_trace
 * @brief Trace static easier user-defined code stub.
 *
 * @par Description:
 * This API is used to instrument user-defined trace code stub in source code, to track events simply.
 * @attention
 * None.
 *
 * @param TYPE           [IN] Type #UINT32. The event type, only low 4 bits take effect.
 * @param IDENTITY       [IN] Type #UINTPTR. The subject of this event description.
 * @param ...            [IN] Type #UINTPTR. This piece of event's params.
 * @retval None.
 *
 * @par Dependency:
 * <ul><li>los_trace.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V200R005C00
 */
#define LOS_TRACE_EASY(TYPE, IDENTITY, ...)                                                                 \
    do {                                                                                                    \
        UINTPTR _inner[] = {0, ##__VA_ARGS__};                                                              \
        UINTPTR _n = sizeof(_inner) / sizeof(UINTPTR);                                                      \
        OsTraceHook(TRACE_USER_DEFAULT_FLAG | TYPE, (UINTPTR)IDENTITY, _n > 1 ? &_inner[1] : NULL, _n - 1); \
    } while (0)
#else
#define LOS_TRACE_EASY(...)
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
 * @brief Offline trace buffer display.
 *
 * @par Description:
 * Display trace buf data only at offline mode.
 * @attention
 * <ul>
 * <li>This API can be called only after that trace stopped. Otherwise the trace dump will be failed.</li>
 * <li>Trace data will be send to pipeline when user set toClient = TRUE. Otherwise it will be formatted and printed
 * out.</li>
 * </ul>
 *
 * @param toClient           [IN] Type #BOOL. Whether send trace data to Client through pipeline.
 * @retval #NA
 *
 * @par Dependency:
 * <ul><li>los_trace.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_TraceRecordDump
 * @since Huawei LiteOS V200R005C00
 */
extern VOID LOS_TraceRecordDump(BOOL toClient);

/**
 * @ingroup los_trace
 * @brief Offline trace buffer export.
 *
 * @par Description:
 * Return the trace buf only at offline mode.
 * @attention
 * <ul>
 * <li>This API can be called only after that trace buffer has been established. </li>
 * <li>The return buffer's address is a critical resource, user can only ready.</li>
 * </ul>
 *
 * @param NA
 * @retval #OfflineHead*   The trace buffer's address, analyze this buffer according to the structure of
 * OfflineHead.
 *
 * @par Dependency:
 * <ul><li>los_trace.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_TraceRecordGet
 * @since Huawei LiteOS V200R005C00
 */
extern OfflineHead *LOS_TraceRecordGet(VOID);

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
