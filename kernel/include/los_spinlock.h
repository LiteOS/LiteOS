/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
 * Description: Spinlock
 * Author: Huawei LiteOS Team
 * Create: 2018-07-11
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
#include "los_lockdep.h"

#include "arch/spinlock.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_KERNEL_SMP_LOCKDEP
#define SPINLOCK_OWNER_INIT     NULL

#define LOCKDEP_CHECK_IN(lock)  OsLockDepCheckIn(lock)
#define LOCKDEP_RECORD(lock)    OsLockDepRecord(lock)
#define LOCKDEP_CHECK_OUT(lock) OsLockDepCheckOut(lock)
#define LOCKDEP_CLEAR_LOCKS()   OsLockdepClearSpinlocks()

#define SPIN_LOCK_INITIALIZER(lockName) \
{                                       \
    .rawLock    = 0U,                   \
    .cpuid      = (UINT32)(-1),         \
    .owner      = SPINLOCK_OWNER_INIT,  \
    .name       = #lockName,            \
}
#else
#define LOCKDEP_CHECK_IN(lock)
#define LOCKDEP_RECORD(lock)
#define LOCKDEP_CHECK_OUT(lock)
#define LOCKDEP_CLEAR_LOCKS()
#define SPIN_LOCK_INITIALIZER(lockName) \
{                                       \
    .rawLock    = 0U,                   \
}
#endif

#define SPIN_LOCK_INIT(lock)  SPIN_LOCK_S lock = SPIN_LOCK_INITIALIZER(lock)

#if (LOSCFG_KERNEL_SMP == YES)
/**
 * @ingroup  los_spinlock
 * @brief Lock the spinlock.
 *
 * @par Description:
 * This API is used to lock the spin lock.
 *
 * @attention None.
 *
 * @param  lock [IN] Type #SPIN_LOCK_S spinlock pointer.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_spinlock.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_SpinUnlock
 * @since Huawei LiteOS V200R003C00
 */
LITE_OS_SEC_ALW_INLINE STATIC INLINE VOID LOS_SpinLock(SPIN_LOCK_S *lock)
{
    /*
     * disable the scheduler, so it won't do schedule until
     * scheduler is reenabled. The LOS_TaskUnlock should not
     * be directly called along this critic area.
     */
    LOS_TaskLock();

    LOCKDEP_CHECK_IN(lock);
    ArchSpinLock(&lock->rawLock);
    LOCKDEP_RECORD(lock);
}

/**
 * @ingroup  los_spinlock
 * @brief Trying lock the spinlock.
 *
 * @par Description:
 * This API is used to trying lock the spin lock.
 *
 * @attention None.
 *
 * @param  lock [IN] Type #SPIN_LOCK_S spinlock pointer.
 *
 * @retval LOS_OK   Got the spinlock.
 * @retval LOS_NOK  Not getting the spinlock.
 * @par Dependency:
 * <ul><li>los_spinlock.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_SpinLock
 * @since Huawei LiteOS V200R003C00
 */
LITE_OS_SEC_ALW_INLINE STATIC INLINE INT32 LOS_SpinTrylock(SPIN_LOCK_S *lock)
{
    LOS_TaskLock();

    LOCKDEP_CHECK_IN(lock);
    INT32 ret = ArchSpinTrylock(&lock->rawLock);
    if (ret == LOS_OK) {
        LOCKDEP_RECORD(lock);
    }

    return ret;
}

/**
 * @ingroup  los_spinlock
 * @brief Unlock the spinlock.
 *
 * @par Description:
 * This API is used to unlock the spin lock.
 *
 * @attention None.
 *
 * @param  lock [IN] Type #SPIN_LOCK_S spinlock pointer.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_spinlock.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_SpinLock
 * @since Huawei LiteOS V200R003C00
 */
LITE_OS_SEC_ALW_INLINE STATIC INLINE VOID LOS_SpinUnlock(SPIN_LOCK_S *lock)
{
    LOCKDEP_CHECK_OUT(lock);
    ArchSpinUnlock(&lock->rawLock);

    /* restore the scheduler flag */
    LOS_TaskUnlock();
}

/**
 * @ingroup  los_spinlock
 * @brief Lock the spinlock and disable all interrupts.
 *
 * @par Description:
 * This API is used to lock the spin lock and disable all interrupts.
 *
 * @attention None.
 *
 * @param  lock     [IN]    Type #SPIN_LOCK_S spinlock pointer.
 * @param  intSave  [OUT]   Type #UINT32 Saved interrupt flag for latter restored.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_spinlock.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_SpinLock
 * @since Huawei LiteOS V200R003C00
 */
LITE_OS_SEC_ALW_INLINE STATIC INLINE VOID LOS_SpinLockSave(SPIN_LOCK_S *lock, UINT32 *intSave)
{
    *intSave = LOS_IntLock();
    LOS_SpinLock(lock);
}

/**
 * @ingroup  los_spinlock
 * @brief Unlock the spinlock and restore interrupt flag.
 *
 * @par Description:
 * This API is used to unlock the spin lock and restore interrupt flag.
 *
 * @attention None.
 *
 * @param  lock     [IN]    Type #SPIN_LOCK_S spinlock pointer.
 * @param  intSave  [IN]    Type #UINT32 Interrupt flag to be restored.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_spinlock.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_SpinUnlock
 * @since Huawei LiteOS V200R003C00
 */
LITE_OS_SEC_ALW_INLINE STATIC INLINE VOID LOS_SpinUnlockRestore(SPIN_LOCK_S *lock, UINT32 intSave)
{
    LOS_SpinUnlock(lock);
    LOS_IntRestore(intSave);
}

/**
 * @ingroup  los_spinlock
 * @brief Check if holding the spinlock.
 *
 * @par Description:
 * This API is used to check if holding the spinlock.
 *
 * @attention None.
 *
 * @param  lock     [IN]    Type #SPIN_LOCK_S spinlock pointer.
 *
 * @retval TRUE   Holding the spinlock.
 * @retval FALSE  Not Holding the spinlock.
 * @par Dependency:
 * <ul><li>los_spinlock.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V200R003C00
 */
LITE_OS_SEC_ALW_INLINE STATIC INLINE BOOL LOS_SpinHeld(const SPIN_LOCK_S *lock)
{
    return (lock->rawLock != 0);
}

/**
 * @ingroup  los_spinlock
 * @brief Spinlock initialization.
 *
 * @par Description:
 * This API is used to initialize a spinlock.
 *
 * @attention None.
 *
 * @param  lock     [IN]    Type #SPIN_LOCK_S spinlock pointer.
 *
 * @retval None.
 *
 * @par Dependency:
 * <ul><li>los_spinlock.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V200R003C00
 */
LITE_OS_SEC_ALW_INLINE STATIC INLINE VOID LOS_SpinInit(SPIN_LOCK_S *lock)
{
    lock->rawLock     = 0;
#ifdef LOSCFG_KERNEL_SMP_LOCKDEP
    lock->cpuid       = (UINT32)-1;
    lock->owner       = SPINLOCK_OWNER_INIT;
    lock->name        = "spinlock";
#endif
}

#else

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

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
