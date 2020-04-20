/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Queue Private HeadFile
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

#ifndef _LOS_QUEUE_PRI_H
#define _LOS_QUEUE_PRI_H

#include "los_queue.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup los_sem
 * Configuration item for queue to use dynamic memory
 */
#if (LOSCFG_LIB_CONFIGURABLE == YES)
#define LOSCFG_BASE_IPC_QUEUE_DYN_MEM       YES
#else
#define LOSCFG_BASE_IPC_QUEUE_DYN_MEM       NO
#endif

typedef enum {
    OS_QUEUE_READ = 0,
    OS_QUEUE_WRITE = 1,
    OS_QUEUE_N_RW = 2
} QueueReadWrite;

typedef enum {
    OS_QUEUE_HEAD = 0,
    OS_QUEUE_TAIL = 1
} QueueHeadTail;

#define OS_QUEUE_OPERATE_TYPE(ReadOrWrite, HeadOrTail) (((UINT32)(HeadOrTail) << 1) | (ReadOrWrite))
#define OS_QUEUE_READ_WRITE_GET(type) ((type) & 0x01U)
#define OS_QUEUE_READ_HEAD     (OS_QUEUE_READ | (OS_QUEUE_HEAD << 1))
#define OS_QUEUE_READ_TAIL     (OS_QUEUE_READ | (OS_QUEUE_TAIL << 1))
#define OS_QUEUE_WRITE_HEAD    (OS_QUEUE_WRITE | (OS_QUEUE_HEAD << 1))
#define OS_QUEUE_WRITE_TAIL    (OS_QUEUE_WRITE | (OS_QUEUE_TAIL << 1))
#define OS_QUEUE_OPERATE_GET(type) ((type) & 0x03U)
#define OS_QUEUE_IS_READ(type) (OS_QUEUE_READ_WRITE_GET(type) == OS_QUEUE_READ)
#define OS_QUEUE_IS_WRITE(type) (OS_QUEUE_READ_WRITE_GET(type) == OS_QUEUE_WRITE)

/**
 * @ingroup los_queue
 * Queue information block structure
 */
typedef struct {
    UINT8 *queueHandle; /**< Pointer to a queue handle */
    UINT16 queueState; /**< Queue state */
    UINT16 queueLen; /**< Queue length */
    UINT16 queueSize; /**< Node size */
    UINT32 queueID; /**< queueID */
    UINT16 queueHead; /**< Node head */
    UINT16 queueTail; /**< Node tail */
    UINT16 readWriteableCnt[OS_QUEUE_N_RW]; /**< Count of readable or writable resources, 0:readable, 1:writable */
    LOS_DL_LIST readWriteList[OS_QUEUE_N_RW]; /**< the linked list to be read or written, 0:readlist, 1:writelist */
    LOS_DL_LIST memList; /**< Pointer to the memory linked list */
} LosQueueCB;

/* queue state */
/**
 *  @ingroup los_queue
 *  Message queue state: not in use.
 */
#define OS_QUEUE_UNUSED        0

/**
 *  @ingroup los_queue
 *  Message queue state: used.
 */
#define OS_QUEUE_INUSED        1

/**
 *  @ingroup los_queue
 *  Not in use.
 */
#define OS_QUEUE_WAIT_FOR_POOL 1

/**
 *  @ingroup los_queue
 *  Normal message queue.
 */
#define OS_QUEUE_NORMAL        0

/**
 *  @ingroup los_queue
 *  Queue information control block
 */
#if (LOSCFG_BASE_IPC_QUEUE_DYN_MEM == YES)
extern LosQueueCB *g_allQueue;
#else
extern LosQueueCB g_allQueue[];
#endif

/**
 * @ingroup los_queue
 * COUNT | INDEX  split bit
 */
#define QUEUE_SPLIT_BIT        16
/**
 * @ingroup los_queue
 * Set the queue id
 */
#define SET_QUEUE_ID(count, queueID)    (((count) << QUEUE_SPLIT_BIT) | (queueID))

/**
 * @ingroup los_queue
 * get the queue index
 */
#define GET_QUEUE_INDEX(queueID)        ((queueID) & ((1U << QUEUE_SPLIT_BIT) - 1))

/**
 * @ingroup los_queue
 * get the queue count
 */
#define GET_QUEUE_COUNT(queueID)        ((queueID) >> QUEUE_SPLIT_BIT)

/**
 * @ingroup los_queue
 * Obtain a handle of the queue that has a specified ID.
 *
 */
#define GET_QUEUE_HANDLE(queueID)       (((LosQueueCB *)g_allQueue) + GET_QUEUE_INDEX(queueID))

/**
 * @ingroup los_queue
 * Obtain the head node in a queue doubly linked list.
 */
#define GET_QUEUE_LIST(ptr) LOS_DL_LIST_ENTRY(ptr, LosQueueCB, readWriteList[OS_QUEUE_WRITE])

/**
 * @ingroup los_queue
 * @brief Alloc a stationary memory for a mail.
 *
 * @par Description:
 * This API is used to alloc a stationary memory for a mail according to queueID.
 * @attention
 * <ul>
 * <li>Do not alloc memory in unblocking modes such as interrupt.</li>
 * <li>This API cannot be called before the Huawei LiteOS is initialized.</li>
 * <li>The argument timeout is a relative time.</li>
 * </ul>
 *
 * @param queueID        [IN]        Queue ID. The value range is [1,LOSCFG_BASE_IPC_QUEUE_LIMIT].
 * @param mailPool        [IN]        The memory poll that stores the mail.
 * @param timeout        [IN]        Expiry time. The value range is [0,LOS_WAIT_FOREVER].
 *
 * @retval   #NULL                     The memory allocation is failed.
 * @retval   #pMem                     The address of alloc memory.
 * @par Dependency:
 * <ul><li>los_queue_pri.h: the header file that contains the API declaration.</li></ul>
 * @see OsQueueMailFree
 * @since Huawei LiteOS V100R001C00
 */
extern VOID *OsQueueMailAlloc(UINT32 queueID, VOID *mailPool, UINT32 timeout);

/**
 * @ingroup los_queue
 * @brief Free a stationary memory of a mail.
 *
 * @par Description:
 * This API is used to free a stationary memory for a mail according to queueID.
 * @attention
 * <ul>
 * <li>This API cannot be called before the Huawei LiteOS is initialized.</li>
 * </ul>
 *
 * @param queueID        [IN]        Queue ID. The value range is [1,LOSCFG_BASE_IPC_QUEUE_LIMIT].
 * @param mailPool        [IN]        The mail memory poll address.
 * @param mailMem         [IN]        The mail memory block address.
 *
 * @retval   #LOS_OK                                 0x00000000: The memory free successfully.
 * @retval   #OS_ERRNO_QUEUE_MAIL_HANDLE_INVALID     0x02000619: The handle of the queue passed-in when the memory
 *                                                               for the queue is being freed is invalid.
 * @retval   #OS_ERRNO_QUEUE_MAIL_PTR_INVALID        0x0200061a: The pointer to the memory to be freed is null.
 * @retval   #OS_ERRNO_QUEUE_MAIL_FREE_ERROR         0x0200061b: The memory for the queue fails to be freed.
 * @par Dependency:
 * <ul><li>los_queue_pri.h: the header file that contains the API declaration.</li></ul>
 * @see OsQueueMailAlloc
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 OsQueueMailFree(UINT32 queueID, VOID *mailPool, VOID *mailMem);

extern UINT32 OsQueueInit(VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_QUEUE_PRI_H */
