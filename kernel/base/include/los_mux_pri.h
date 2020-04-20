/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Mutex Private HeadFile
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

#ifndef _LOS_MUX_PRI_H
#define _LOS_MUX_PRI_H

#include "los_mux.h"
#include "los_task_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup los_sem
 * Configuration item for mux to use dynamic memory
 */
#if (LOSCFG_LIB_CONFIGURABLE == YES)
#define LOSCFG_BASE_IPC_MUX_DYN_MEM       YES
#else
#define LOSCFG_BASE_IPC_MUX_DYN_MEM       NO
#endif

/**
 * @ingroup los_mux
 * Mutex base object must be the same as the first three member names of LosMuxCB,
 * so that pthread_mutex_t can share the kernel mutex mechanism.
 */
typedef struct {
    LOS_DL_LIST muxList; /**< Mutex linked list */
    LosTaskCB *owner; /**< The current thread that is locking a mutex */
    UINT16 muxCount; /**< Times of locking a mutex */
} MuxBaseCB;

/**
 * @ingroup los_mux
 * Mutex object.
 */
typedef struct {
    LOS_DL_LIST muxList; /**< Mutex linked list */
    LosTaskCB *owner; /**< The current thread that is locking a mutex */
    UINT16 muxCount; /**< Times of locking a mutex */
    UINT8 muxStat; /**< State OS_MUX_UNUSED, OS_MUX_USED */
    UINT32 muxID; /**< Handle ID */
} LosMuxCB;

/**
 * @ingroup los_mux
 * Mutex state: not in use.
 */
#define OS_MUX_UNUSED 0

/**
 * @ingroup los_mux
 * Mutex state: in use.
 */
#define OS_MUX_USED   1

/**
 * @ingroup los_mux
 * Mutex global array address, which can be obtained by using a handle ID.
 */
#if (LOSCFG_BASE_IPC_MUX_DYN_MEM == YES)
extern LosMuxCB *g_allMux;
#else
extern LosMuxCB g_allMux[];
#endif

/**
 * @ingroup los_mux
 * COUNT | INDEX  split bit
 */
#define MUX_SPLIT_BIT 16
/**
 * @ingroup los_mux
 * Set the mutex id
 */
#define SET_MUX_ID(count, muxID)    (((count) << MUX_SPLIT_BIT) | (muxID))

/**
 * @ingroup los_mux
 * get the mutex index
 */
#define GET_MUX_INDEX(muxID)        ((muxID) & ((1U << MUX_SPLIT_BIT) - 1))

/**
 * @ingroup los_mux
 * get the mutex count
 */
#define GET_MUX_COUNT(muxID)        ((muxID) >> MUX_SPLIT_BIT)
/**
 * @ingroup los_mux
 * Obtain the pointer to a mutex object of the mutex that has a specified handle.
 */
#define GET_MUX(muxID)              (((LosMuxCB *)g_allMux) + GET_MUX_INDEX(muxID))

extern UINT32 OsMuxInit(VOID);

#define MUX_SCHEDULE    0x01
#define MUX_NO_SCHEDULE 0x02
extern UINT32 OsMuxPendOp(LosTaskCB *runTask, MuxBaseCB *muxPended, UINT32 timeout, UINT32 intSave);
extern UINT32 OsMuxPostOp(LosTaskCB *runTask, MuxBaseCB *muxPosted);
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_MUX_PRI_H */
