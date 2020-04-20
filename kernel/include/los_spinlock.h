/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
 * Description: Spinlock
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

#ifndef _LOS_SPINLOCK_H
#define _LOS_SPINLOCK_H
#include "los_typedef.h"
#include "los_config.h"
#include "los_hwi.h"
#include "los_task.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

extern VOID ArchSpinLock(size_t *lock);
extern VOID ArchSpinUnlock(size_t *lock);
extern INT32 ArchSpinTrylock(size_t *lock);

typedef struct Spinlock {
    size_t      rawLock;
} SPIN_LOCK_S;

#define LOCKDEP_CHECK_IN(lock)
#define LOCKDEP_RECORD(lock)
#define LOCKDEP_CHECK_OUT(lock)
#define LOCKDEP_CLEAR_LOCKS()
#define SPIN_LOCK_INITIALIZER(lockName) \
{                                       \
    .rawLock    = 0U,                   \
}

#define SPIN_LOCK_INIT(lock)  SPIN_LOCK_S lock = SPIN_LOCK_INITIALIZER(lock)

/*
 * For Non-SMP system, these apis does not handle with spinlocks,
 * but for unifying the code of drivers, vendors and etc.
 */
LITE_OS_SEC_ALW_INLINE STATIC INLINE VOID LOS_SpinLock(SPIN_LOCK_S *lock)
{
    (VOID)lock;
}

LITE_OS_SEC_ALW_INLINE STATIC INLINE INT32 LOS_SpinTrylock(SPIN_LOCK_S *lock)
{
    (VOID)lock;
    return LOS_OK;
}

LITE_OS_SEC_ALW_INLINE STATIC INLINE VOID LOS_SpinUnlock(SPIN_LOCK_S *lock)
{
    (VOID)lock;
}

LITE_OS_SEC_ALW_INLINE STATIC INLINE VOID LOS_SpinLockSave(SPIN_LOCK_S *lock, UINT32 *intSave)
{
    (VOID)lock;
    *intSave = LOS_IntLock();
}

LITE_OS_SEC_ALW_INLINE STATIC INLINE VOID LOS_SpinUnlockRestore(SPIN_LOCK_S *lock, UINT32 intSave)
{
    (VOID)lock;
    LOS_IntRestore(intSave);
}

LITE_OS_SEC_ALW_INLINE STATIC INLINE BOOL LOS_SpinHeld(const SPIN_LOCK_S *lock)
{
    (VOID)lock;
    return TRUE;
}

LITE_OS_SEC_ALW_INLINE STATIC INLINE VOID LOS_SpinInit(SPIN_LOCK_S *lock)
{
    (VOID)lock;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
