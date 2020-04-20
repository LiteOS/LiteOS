/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Semaphore Private HeadFile
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

#ifndef _LOS_SEM_PRI_H
#define _LOS_SEM_PRI_H

#include "los_sem.h"
#include "los_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup los_sem
 * Semaphore control structure.
 */
typedef struct {
    UINT8 semStat; /**< Semaphore state */
    UINT16 semCount; /**< Number of available semaphores */
    UINT16 maxSemCount;  /**< Max number of available semaphores */
    UINT32 semID; /**< Semaphore control structure ID */
    LOS_DL_LIST semList; /**< Queue of tasks that are waiting on a semaphore */
} LosSemCB;

/**
 * @ingroup los_sem
 * The semaphore is not in use.
 *
 */
#define OS_SEM_UNUSED        0
/**
 * @ingroup los_sem
 * The semaphore is used.
 *
 */
#define OS_SEM_USED          1
/**
 * @ingroup los_sem
 * Obtain the head node in a semaphore doubly linked list.
 *
 */
#define GET_SEM_LIST(ptr) LOS_DL_LIST_ENTRY(ptr, LosSemCB, semList)

/**
 * @ingroup los_sem
 * Configuration item for IPC to use dynamic memory
 */
#if (LOSCFG_LIB_CONFIGURABLE == YES)
#define LOSCFG_BASE_IPC_DYNAMIC_MEM       YES
#else
#define LOSCFG_BASE_IPC_DYNAMIC_MEM       NO
#endif

/**
 *  @ingroup los_sem
 *  Semaphore information control block
 */
#if (LOSCFG_BASE_IPC_DYNAMIC_MEM == YES)
extern LosSemCB *g_allSem;
#else
extern LosSemCB g_allSem[];
#endif

/**
 * @ingroup los_sem
 * COUNT | INDEX  split bit
 */
#define SEM_SPLIT_BIT        16
/**
 * @ingroup los_sem
 * Set the semaphore id
 */
#define SET_SEM_ID(count, semID)    (((count) << SEM_SPLIT_BIT) | (semID))

/**
 * @ingroup los_sem
 * get the semaphore index
 */
#define GET_SEM_INDEX(semID)        ((semID) & ((1U << SEM_SPLIT_BIT) - 1))

/**
 * @ingroup los_sem
 * get the semaphore count
 */
#define GET_SEM_COUNT(semID)        ((semID) >> SEM_SPLIT_BIT)

/**
 * @ingroup los_sem
 * Obtain a semaphore ID.
 *
 */
#define GET_SEM(semID)              (((LosSemCB *)g_allSem) + GET_SEM_INDEX(semID))

extern UINT32 OsSemInit(VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_SEM_PRI_H */
