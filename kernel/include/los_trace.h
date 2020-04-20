/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: LiteOS Trace Module Implementation HeadFile
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

#ifndef RECORD_TRACE_H
#define RECORD_TRACE_H

#include "los_base.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup los_trace
 * Task error code: Insufficient memory for trace struct.
 *
 * Value: 0x02001400
 *
 * Solution: Decrease the maximum number of tasks.
 */
#define LOS_ERRNO_TRACE_NO_MEMORY              LOS_ERRNO_OS_ERROR(LOS_MOD_TRACE, 0x00)

/**
 * @ingroup los_trace
 * Task error code: User type is invalid when register new trace.
 *
 * Value: 0x02001401
 *
 * Solution: Use valid type to regeister the new trace.
 */
#define LOS_ERRNO_TRACE_TYPE_INVALID           LOS_ERRNO_OS_ERROR(LOS_MOD_TRACE, 0x01)

/**
 * @ingroup los_trace
 * Task error code: The callback function is null when register new trace.
 *
 * Value: 0x02001402
 *
 * Solution: Use valid callback function to regeister the new trace.
 */
#define LOS_ERRNO_TRACE_FUNCTION_NULL          LOS_ERRNO_OS_ERROR(LOS_MOD_TRACE, 0x02)

/**
 * @ingroup los_trace
 * Task error code: The filled size is 0 when register new trace.
 *
 * Value: 0x02001403
 *
 * Solution: Use valid filled size to regeister the new trace.
 */
#define LOS_ERRNO_TRACE_MAX_SIZE_INVALID       LOS_ERRNO_OS_ERROR(LOS_MOD_TRACE, 0x03)

/**
 * @ingroup los_trace
 * Interrupt direction: enter the new interrupt
 */
#define IRQ_DIRECT_IN                          1
/**
 * @ingroup los_trace
 * Interrupt direction: leave the old interrupt
 */
#define IRQ_DIRECT_OUT                         0

/**
 * @ingroup los_trace
 * It's the length of tag, filled by los_trace system
 */
#define LOS_TRACE_TAG_LENGTH                   sizeof(UINTPTR)

/**
* @ingroup  los_trace
* @brief Define the type of a function used to record trace.
*
* @par Description:
* This API is used to define the type of a recording trace function and call it after task or interrupt switch.
* @attention None.
*
* @param  inBuf  [IN]  Type #UINT8 * The buffer saved trace information.
* @param  newID  [IN]  Type #UINT32  The destination.
* @param  oldID  [IN]  Type #UINT32  The source ID for task or the direction for interrupt.
*
* @retval None.
* @par Dependency:
* <ul><li>los_trace.h: the header file that contains the API declaration.</li></ul>
* @see
* @since Huawei LiteOS V200R003C00
*/
typedef UINT16 (*WriteHook)(UINT8 *inBuf, UINT32 newID, UINT32 oldID);

/**
 * @ingroup los_trace
 * Stands for the trace type can be registered.
 */
typedef enum {
    LOS_TRACE_SWITCH    = 0,                /**< trace for task switch, 0 is reserved for taskswitch */
    LOS_TRACE_INTERRUPT = 1,                /**< trace for Interrrupt, 1 is reserved for interrupt */
    LOS_TRACE_TYPE_NUM  = 5,                /**< num for the register type, user can use 2~ LOS_TRACE_TYPE_NUM-1 */
} TraceType;

/**
 * @ingroup los_trace
 * struct to store the trace infomation
 */
typedef struct {
    UINTPTR tracePos;                       /**< Data buffer current index position */
    UINTPTR traceWrapPos;                   /**< Data buffer last loop end position */
    UINT8   dataBuf[LOS_TRACE_BUFFER_SIZE]; /**< Data buffer */
} TraceBuffer;

/**
 * @ingroup los_trace
 * struct to store the task switch infomation
 */
typedef struct {
    UINT32  srcTaskId;                      /**< source taskid */
    UINT32  destTaskId;                     /**< destination taskid */
    UINT64  currentTick;                    /**< Time at which the task switch happens */
} TaskTraceFrame;

/**
 * @ingroup los_trace
 * struct to store the interrupt infomation
 */
typedef struct {
    UINT32  irqDirection;                   /**< IRQ_DIRECT_IN  stands for entering the irq handler */
                                            /**< IRQ_DIRECT_OUT stands for leaving the previous irq handler */
    UINT32  irqNum;                         /**< IRQ number which trigger the interrupt */
    UINT64  currentTick;                    /**< Time at which the the trace is called */
} IntTraceFrame;

/**
 * @ingroup los_trace
 * @brief main trace function is called by user to logger the information.
 *
 * @par Description:
 * This API is used to trace the infomation.
 * @attention
 * <ul>
 * <li>This API can be called only after trace type is intialized. Otherwise, the trace will be failed.</li>
 * </ul>
 *
 * @param  traceType     [IN] TraceType. Type of trace information.
 * @param  newID         [IN] UINT32. It stands for the new direction of trace
 * @param  oldID         [IN] UINT32. It stands for the previous source of trace
 *
 * @retval NONE.
 *
 * @par Dependency:
 * <ul><li>los_trace.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_Trace
 * @since Huawei LiteOS V200R003C00
 */
VOID LOS_Trace(TraceType traceType, UINT32 newID, UINT32 oldID);

/**
 * @ingroup los_trace
 * @brief intialize the trace when the system startup.
 *
 * @par Description:
 * This API is used to intilize the trace for system level.
 * @attention
 * <ul>
 * <li>This API can be called only after the memory is initialized. Otherwise, the CPU usage fails to be obtained.</li>
 * </ul>
 *
 * @param None.
 *
 * @retval #LOS_ERRNO_TRACE_NO_MEMORY         0x02001400: The memory is not enough for initilize.
 * @retval #LOS_OK                            0x00000000: The intialization is successful.
 * @par Dependency:
 * <ul><li>los_trace.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_TraceInit
 * @since Huawei LiteOS V200R003C00
 */
UINT32 LOS_TraceInit(VOID);

/**
 * @ingroup los_trace
 * @brief register the hook for specific trace type.
 *
 * @par Description:
 * This API is used to register the hook for specific trace type.
 * @attention
 * <ul>
 * <li>This API can be called only after that trace type, input hookfnc and trace's data struct are established.</li>
 * Otherwise, the trace will be failed.</li>
 * </ul>
 *
 * @param  traceType     [IN] TraceType. Type of trace information.
 * @param  inhook        [IN] WriteHook. It's a callback function to store the useful trace
 *                            information
 * @param  useSize       [IN] UINT16. The maximum size the trace will use for the specific trace type.
 *
 * @retval #LOS_ERRNO_TRACE_NO_MEMORY          0x02001400: The memory is not enough for initilize.
 * @retval #LOS_ERRNO_TRACE_TYPE_INVALID       0x02001401: The trace type is invalid. Valid type is from
 *                                                         LOS_TRACE_TYPE_NUM-1
 * @retval #LOS_ERRNO_TRACE_FUNCTION_NULL      0x02001402: The input callback function is NULL
 * @retval #LOS_ERRNO_TRACE_MAX_SIZE_INVALID   0x02001403: The information maxmum size is 0 to store.
 * @retval #LOS_OK                             0x00000000: The registeration is successful.
 *
 * @par Dependency:
 * <ul><li>los_trace.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_TraceUserReg
 * @since Huawei LiteOS V200R003C00
 */
UINT32 LOS_TraceUserReg(TraceType traceType, WriteHook inHook, UINT16 useSize);

/**
 * @ingroup los_trace
 * @brief the function to get certain type trace information frame size.
 *
 * @par Description:
 * This API is used to get certain type trace information frame size.
 * @attention
 * <ul>
 * <li>This API can be called to get frame size.</li>
 * Otherwise, the trace will be failed.</li>
 * </ul>
 *
 * @param  traceType     [IN] TraceType. Type of trace information, it must belong to the defined enumeration type.
 *
 * @retval #UINT16       The certain type trace information frame size.
 *
 * @par Dependency:
 * <ul><li>los_trace.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_TraceFrameSizeGet
 * @since Huawei LiteOS V200R003C00
 */
INT32 LOS_TraceFrameSizeGet(TraceType traceType);

/**
 * @ingroup los_trace
 * @brief copy trace buffer of certain cpu to user-defined buffer.
 *
 * @par Description:
 * This API is used to copy system trace buffer of certain cpu to user-defined buffer.
 * @attention
 * <ul>
 * <li>This API can be called only after that trace buffer has been established.</li>
 * Otherwise, the trace will be failed.</li>
 * </ul>
 *
 * @param  cpuID         [IN] UINT32. Current cpu ID.
 * @param  outputBuf     [IN] TraceBuffer. The type of user-defined buffer must be TraceBuffer structure type.
 *
 * @retval #TraceBuffer  The point to trace buffer of certain cpuID.
 *
 * @par Dependency:
 * <ul><li>los_trace.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_TraceBufGet
 * @since Huawei LiteOS V200R003C00
 */
UINT32 LOS_TraceBufGet(TraceBuffer *outputBuf, UINT32 cpuID);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
