/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Semaphore Private HeadFile
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

#ifndef _LOS_SEM_PRI_H
#define _LOS_SEM_PRI_H

#include "los_sem.h"

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
    UINT8 semStat;       /**< Semaphore state, enum LosSemState */
    UINT8 semType;       /**< Semaphore Type, enum LosSemType */
    UINT16 semCount;     /**< number of available semaphores */
    UINT32 semId;        /**< Semaphore control structure ID, COUNT(UINT16)|INDEX(UINT16) */
    LOS_DL_LIST semList; /**< List of tasks that are waiting on a semaphore */
} LosSemCB;

/**
 * @ingroup los_sem
 * Semaphore state enums.
 */
enum {
    OS_SEM_UNUSED, /**< The semaphore is not in use */
    OS_SEM_USED,   /**< The semaphore is used */
};

/**
 * @ingroup los_sem
 * Semaphore type enums.
 */
enum {
    OS_SEM_COUNTING, /**< The semaphore is a counting semaphore which max count is OS_SEM_COUNT_MAX */
    OS_SEM_BINARY,   /**< The semaphore is a binary semaphore which max count is OS_SEM_BINARY_COUNT_MAX */
};

/**
 * @ingroup los_sem
 * Max semaphore count enums.
 */
enum {
    OS_SEM_COUNT_MAX = 0xFFFE,   /**< Max count of counting semaphores */
    OS_SEM_BINARY_COUNT_MAX = 1, /**< Max count of binary semaphores */
};

/**
 *  @ingroup los_sem
 *  Semaphore information control block
 */
extern LosSemCB *g_allSem;

/**
 * @ingroup los_sem
 * Obtain the head node in a semaphore doubly linked list.
 *
 */
#define GET_SEM_LIST(ptr)                   LOS_DL_LIST_ENTRY(ptr, LosSemCB, semList)

#ifndef LOSCFG_RESOURCE_ID_NOT_USE_HIGH_BITS
/**
 * @ingroup los_sem
 * COUNT | INDEX  split bit
 */
#define SEM_SPLIT_BIT                       16

/**
 * @ingroup los_sem
 * Set the semaphore id
 */
#define SET_SEM_ID(count, semId)            (((count) << SEM_SPLIT_BIT) | (semId))

/**
 * @ingroup los_sem
 * get the semaphore index
 */
#define GET_SEM_INDEX(semId)                ((semId) & ((1U << SEM_SPLIT_BIT) - 1))

/**
 * @ingroup los_sem
 * get the semaphore count
 */
#define GET_SEM_COUNT(semId)                ((semId) >> SEM_SPLIT_BIT)

#else

/**
 * @ingroup los_sem
 * get the semaphore index
 */
#define GET_SEM_INDEX(semId)                (semId)

#endif

/**
 * @ingroup los_sem
 * Obtain a semaphore ID.
 */
#define GET_SEM(semId)                      (((LosSemCB *)g_allSem) + GET_SEM_INDEX(semId))

/**
 * @ingroup los_sem
 * @brief Initialize the semaphore module.
 *
 * @par Description:
 * This API is used to create a semaphore control structure according to the initial number of available semaphores
 * specified by count and return the ID of this semaphore control structure.
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @param count       [IN] Initial number of available semaphores. The value range is [0, OS_SEM_COUNT_MAX).
 * @param semHandle   [OUT] ID of the semaphore control structure that is initialized.
 *
 * @retval #LOS_ERRNO_SEM_PTR_NULL  The passed-in semHandle value is NULL.
 * @retval #LOS_ERRNO_SEM_OVERFLOW  The passed-in count value is greater than the maximum number of available
 * semaphores.
 * @retval #LOS_ERRNO_SEM_ALL_BUSY  No semaphore control structure is available.
 * @retval #LOS_OK   The semaphore is successfully created.
 * @par Dependency:
 * <ul><li>los_sem.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_SemDelete
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 OsSemInit(VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_SEM_PRI_H */
