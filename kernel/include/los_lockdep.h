/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
 * Description: Lock Dependency Check.
 * Author: Huawei LiteOS Team
 * Create: 2018-10-18
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

#ifndef _LOS_LOCKDEP_H
#define _LOS_LOCKDEP_H
#include "los_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef struct Spinlock SPIN_LOCK_S;

#define MAX_LOCK_DEPTH  16U

enum LockDepErrType {
    LOCKDEP_SUCEESS = 0,
    LOCKDEP_ERR_DOUBLE_LOCK,
    LOCKDEP_ERR_DEAD_LOCK,
    LOCKDEP_ERR_UNLOCK_WITHOUT_LOCK,
    /* overflow, needs expand */
    LOCKDEP_ERR_OVERFLOW,
};

typedef struct {
    VOID *lockPtr;
    VOID *lockAddr;
    UINT64 waitTime;
    UINT64 holdTime;
} HeldLocks;

typedef struct {
    VOID *waitLock;
    INT32 lockDepth;
    HeldLocks heldLocks[MAX_LOCK_DEPTH];
} LockDep;

/**
 * @ingroup los_lockdep
 *
 * @par Description:
 * This API is used to check dead lock in spinlock.
 * @attention
 * <ul>
 * <li>The parameter passed in should be ensured to be a legal pointer.</li>
 * </ul>
 *
 * @param lock    [IN] point to a SPIN_LOCK_S.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_lockdep.h: the header file that contains the API declaration.</li></ul>
 * @see
 * @since Huawei LiteOS V200R003C00
 */
extern VOID OsLockDepCheckIn(const SPIN_LOCK_S *lock);

/**
 * @ingroup los_lockdep
 *
 * @par Description:
 * This API is used to trace when a spinlock locked.
 * @attention
 * <ul>
 * <li>The parameter passed in should be ensured to be a legal pointer.</li>
 * </ul>
 *
 * @param lock    [IN] point to a SPIN_LOCK_S.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_lockdep.h: the header file that contains the API declaration.</li></ul>
 * @see
 * @since Huawei LiteOS V200R003C00
 */
extern VOID OsLockDepRecord(SPIN_LOCK_S *lock);

/**
 * @ingroup los_lockdep
 *
 * @par Description:
 * This API is used to trace when a spinlock unlocked.
 * @attention
 * <ul>
 * <li>The parameter passed in should be ensured to be a legal pointer.</li>
 * </ul>
 *
 * @param lock  [IN] point to a SPIN_LOCK_S.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_lockdep.h: the header file that contains the API declaration.</li></ul>
 * @see
 * @since Huawei LiteOS V200R003C00
 */
extern VOID OsLockDepCheckOut(SPIN_LOCK_S *lock);

/**
 * @ingroup los_lockdep
 *
 * @par Description:
 * This API is used to clear lockdep record of current task.
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @param None
 * @retval None.
 * @par Dependency:
 * <ul><li>los_lockdep.h: the header file that contains the API declaration.</li></ul>
 * @see
 * @since Huawei LiteOS V200R003C00
 */
extern VOID OsLockdepClearSpinlocks(VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /* _LOS_LOCKDEP_H */
