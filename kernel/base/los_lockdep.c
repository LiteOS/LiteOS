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

#include "los_base.h"
#include "los_spinlock.h"
#include "los_task_pri.h"
#include "los_printf_pri.h"
#include "los_atomic.h"
#include "los_exc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_KERNEL_SMP_LOCKDEP

#define LOCKDEP_GET_NAME(lockDep, index)    (((SPIN_LOCK_S *)((lockDep)->heldLocks[(index)].lockPtr))->name)
#define LOCKDEP_GET_ADDR(lockDep, index)    ((lockDep)->heldLocks[(index)].lockAddr)

STATIC Atomic g_lockdepAvailable = 1;

/* atomic insurance for lockdep check */
STATIC INLINE VOID OsLockDepRequire(UINT32 *intSave)
{
    *intSave = LOS_IntLock();
    while (LOS_AtomicCmpXchg32bits(&g_lockdepAvailable, 0, 1)) {
        /* busy waiting */
    }
}

STATIC INLINE VOID OsLockDepRelease(UINT32 intSave)
{
    LOS_AtomicSet(&g_lockdepAvailable, 1);
    LOS_IntRestore(intSave);
}

STATIC INLINE UINT64 OsLockDepGetCycles(VOID)
{
    UINT32 high, low;

    LOS_GetCpuCycle(&high, &low);
    /* combine cycleHi and cycleLo into 8 bytes cycles */
    return (((UINT64)high << 32) + low);
}

STATIC INLINE CHAR *OsLockDepErrorStringGet(enum LockDepErrType type)
{
    CHAR *errorString = NULL;

    switch (type) {
        case LOCKDEP_ERR_DOUBLE_LOCK:
            errorString = "double lock";
            break;
        case LOCKDEP_ERR_DEAD_LOCK:
            errorString = "dead lock";
            break;
        case LOCKDEP_ERR_UNLOCK_WITHOUT_LOCK:
            errorString = "unlock without lock";
            break;
        case LOCKDEP_ERR_OVERFLOW:
            errorString = "lockdep overflow";
            break;
        default:
            errorString = "unknow error code";
            break;
    }

    return errorString;
}

WEAK VOID OsLockDepPanic(enum LockDepErrType errType)
{
    /* halt here */
    (VOID)errType;
    (VOID)LOS_IntLock();
    OsBackTrace();
    while (1) {}
}

STATIC VOID OsLockDepDumpLock(const LosTaskCB *task, const SPIN_LOCK_S *lock,
                              const VOID *requestAddr, enum LockDepErrType errType)
{
    INT32 i;
    const LockDep *lockDep = &task->lockDep;
    const LosTaskCB *temp = task;

    PrintExcInfo("lockdep check failed\n");
    PrintExcInfo("error type   : %s\n", OsLockDepErrorStringGet(errType));
    PrintExcInfo("request addr : 0x%x\n", requestAddr);

    while (1) {
        PrintExcInfo("task name    : %s\n", temp->taskName);
        PrintExcInfo("task id      : %u\n", temp->taskId);
        PrintExcInfo("cpu num      : %u\n", temp->currCpu);
        PrintExcInfo("start dumping lockdep information\n");
        for (i = 0; i < lockDep->lockDepth; i++) {
            if (lockDep->heldLocks[i].lockPtr == lock) {
                PrintExcInfo("[%d] %s <-- addr:0x%x\n", i, LOCKDEP_GET_NAME(lockDep, i),
                             LOCKDEP_GET_ADDR(lockDep, i));
            } else {
                PrintExcInfo("[%d] %s \n", i, LOCKDEP_GET_NAME(lockDep, i));
            }
        }
        PrintExcInfo("[%d] %s <-- now\n", i, lock->name);

        if (errType == LOCKDEP_ERR_DEAD_LOCK) {
            temp = lock->owner;
            lock = temp->lockDep.waitLock;
            lockDep = &temp->lockDep;
        }

        if (temp == task) {
            break;
        }
    }

    OsLockDepPanic(errType);
}

STATIC BOOL OsLockDepCheckDependancy(const LosTaskCB *current, const LosTaskCB *lockOwner)
{
    BOOL checkResult = TRUE;
    const SPIN_LOCK_S *lockTemp = NULL;

    do {
        if (current == lockOwner) {
            checkResult = FALSE;
            return checkResult;
        }
        if (lockOwner->lockDep.waitLock != NULL) {
            lockTemp = lockOwner->lockDep.waitLock;
            lockOwner = lockTemp->owner;
        } else {
            break;
        }
    } while (lockOwner != SPINLOCK_OWNER_INIT);

    return checkResult;
}

VOID OsLockDepCheckIn(const SPIN_LOCK_S *lock)
{
    UINT32 intSave;
    enum LockDepErrType checkResult = LOCKDEP_SUCEESS;
    VOID *requestAddr = (VOID *)__builtin_return_address(0);
    LosTaskCB *current = OsCurrTaskGet();
    LockDep *lockDep = &current->lockDep;
    LosTaskCB *lockOwner = NULL;

    if (lock == NULL) {
        return;
    }
    OsLockDepRequire(&intSave);

    if (lockDep->lockDepth >= (INT32)MAX_LOCK_DEPTH) {
        checkResult = LOCKDEP_ERR_OVERFLOW;
        goto OUT;
    }

    lockOwner = lock->owner;
    /* not owned by any tasks yet, not doing following checks */
    if (lockOwner == SPINLOCK_OWNER_INIT) {
        goto OUT;
    }

    if (current == lockOwner) {
        checkResult = LOCKDEP_ERR_DOUBLE_LOCK;
        goto OUT;
    }

    if (OsLockDepCheckDependancy(current, lockOwner) != TRUE) {
        checkResult = LOCKDEP_ERR_DEAD_LOCK;
        goto OUT;
    }

OUT:
    if (checkResult == LOCKDEP_SUCEESS) {
        /*
         * though the check may succeed, the waitLock still need to be set.
         * because the OsLockDepCheckIn and OsLockDepRecord is not strictly muti-core
         * sequential, there would be more than two tasks can pass the checking, but
         * only one task can successfully obtain the lock.
         */
        lockDep->waitLock = (SPIN_LOCK_S *)lock;
        lockDep->heldLocks[lockDep->lockDepth].lockAddr = requestAddr;
        lockDep->heldLocks[lockDep->lockDepth].waitTime = OsLockDepGetCycles(); /* start time */
    } else {
        OsLockDepDumpLock(current, lock, requestAddr, checkResult);
    }

    OsLockDepRelease(intSave);
}

VOID OsLockDepRecord(SPIN_LOCK_S *lock)
{
    UINT32 intSave;
    UINT64 cycles;
    LosTaskCB *current = OsCurrTaskGet();
    LockDep *lockDep = &current->lockDep;
    HeldLocks *heldlock = &lockDep->heldLocks[lockDep->lockDepth];

    if (lock == NULL) {
        return;
    }
    OsLockDepRequire(&intSave);

    /*
     * OsLockDepCheckIn records start time t1, after the lock is obtained, we
     * get the time t2, (t2 - t1) is the time of waiting for the lock
     */
    cycles = OsLockDepGetCycles();
    heldlock->waitTime = cycles - heldlock->waitTime;
    heldlock->holdTime = cycles;

    /* record lock info */
    lock->owner = current;
    lock->cpuid = ArchCurrCpuid();

    /* record lockdep info */
    heldlock->lockPtr = lock;
    lockDep->lockDepth++;
    lockDep->waitLock = NULL;

    OsLockDepRelease(intSave);
}

VOID OsLockDepCheckOut(SPIN_LOCK_S *lock)
{
    UINT32 intSave;
    INT32 depth;
    VOID *requestAddr = (VOID *)__builtin_return_address(0);
    LosTaskCB *current = OsCurrTaskGet();
    LosTaskCB *owner = NULL;
    LockDep *lockDep = NULL;
    HeldLocks *heldlocks = NULL;

    if (lock == NULL) {
        return;
    }

    OsLockDepRequire(&intSave);

    owner = lock->owner;
    if (owner == SPINLOCK_OWNER_INIT) {
        OsLockDepDumpLock(current, lock, requestAddr, LOCKDEP_ERR_UNLOCK_WITHOUT_LOCK);
        goto OUT;
    }

    lockDep = &owner->lockDep;
    heldlocks = &lockDep->heldLocks[0];
    depth = lockDep->lockDepth;

    /* find the lock position */
    while (depth-- >= 0) {
        if (heldlocks[depth].lockPtr == lock) {
            break;
        }
    }

    LOS_ASSERT(depth >= 0);

    /* record lock holding time */
    heldlocks[depth].holdTime = OsLockDepGetCycles() - heldlocks[depth].holdTime;

    /* if unlock a older lock, needs move heldLock records */
    while (depth < lockDep->lockDepth - 1) {
        lockDep->heldLocks[depth] = lockDep->heldLocks[depth + 1];
        depth++;
    }

    lockDep->lockDepth--;
    lock->cpuid = (UINT32)(-1);
    lock->owner = SPINLOCK_OWNER_INIT;

OUT:
    OsLockDepRelease(intSave);
}

VOID OsLockdepClearSpinlocks(VOID)
{
    LosTaskCB *task = OsCurrTaskGet();
    LockDep *lockDep = &task->lockDep;
    SPIN_LOCK_S *lock = NULL;

    /*
     * Unlock spinlocks that running task has held.
     * lockDepth will decrease after each spinlock is unlockded.
     */
    while (lockDep->lockDepth) {
        lock = lockDep->heldLocks[lockDep->lockDepth - 1].lockPtr;
        LOS_SpinUnlock(lock);
    }
}

#endif /* LOSCFG_KERNEL_SMP_LOCKDEP */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
