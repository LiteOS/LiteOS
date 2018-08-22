/*
 * Copyright (c) 2015-2017 Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== pthread_rwlock.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/Assert.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>

#include <pthread.h>
#include <errno.h>

#include "pthread_util.h"

/*
 *  ======== pthread_rwlock_obj ========
 */
typedef struct {
    /*  This semaphore must be acquired to obtain a write lock.
     *  A readlock can be obtained if there is already a read lock
     *  acquired, or by acquiring this semaphore.
     */
    ti_sysbios_knl_Semaphore_Struct sem;

    /*  This semaphore is used to block readers when sem is in use
     *  by a write lock.
     */
    ti_sysbios_knl_Semaphore_Struct readSem;

    int activeReaderCnt;   /* number of read locks acquired */
    int blockedReaderCnt;  /* number of readers blocked on readSem */

    /*  The 'owner' is the writer holding the lock, or the first reader
     *  that acquired the lock.
     */
    pthread_t owner;
} pthread_rwlock_obj;

static int rdlockAcquire(pthread_rwlock_obj *lock, UInt timeout);


/*
 *************************************************************************
 *                      pthread_rwlockattr
 *************************************************************************
 */
/*
 *  ======== pthread_rwlockattr_destroy ========
 */
int pthread_rwlockattr_destroy(pthread_rwlockattr_t *attr)
{
    return (0);
}

/*
 *  ======== pthread_rwlockattr_init ========
 */
int pthread_rwlockattr_init(pthread_rwlockattr_t *attr)
{
    return (0);
}

/*
 *************************************************************************
 *                      pthread_rwlock
 *************************************************************************
 */

/*
 *  ======== pthread_rwlock_destroy ========
 */
int pthread_rwlock_destroy(pthread_rwlock_t *rwlock)
{
    pthread_rwlock_obj *obj = (pthread_rwlock_obj *)(&rwlock->sysbios);

    /* return EBUSY if the lock is in use */
    if (obj->owner != NULL) {
        return (EBUSY);
    }

    Semaphore_destruct(&(obj->sem));
    Semaphore_destruct(&(obj->readSem));
    return (0);
}

/*
 *  ======== pthread_rwlock_init ========
 */
int pthread_rwlock_init(pthread_rwlock_t *rwlock,
        const pthread_rwlockattr_t *attr)
{
    pthread_rwlock_obj *obj = (pthread_rwlock_obj *)(&rwlock->sysbios);

    /* object size validation */
    Assert_isTrue(sizeof(pthread_rwlock_obj) <= sizeof(pthread_rwlock_t), NULL);

    /* default semaphore mode is Semaphore_Mode_COUNTING */
    Semaphore_construct(&(obj->sem), 1, NULL);
    Semaphore_construct(&(obj->readSem), 0, NULL);

    obj->activeReaderCnt = 0;
    obj->blockedReaderCnt = 0;
    obj->owner = NULL;
    return (0);
}

/*
 *  ======== pthread_rwlock_rdlock ========
 */
int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock)
{
    pthread_rwlock_obj *obj = (pthread_rwlock_obj *)(&rwlock->sysbios);

    return (rdlockAcquire(obj, BIOS_WAIT_FOREVER));
}

/*
 *  ======== pthread_rwlock_timedrdlock ========
 */
int pthread_rwlock_timedrdlock(pthread_rwlock_t *rwlock,
        const struct timespec *abstime)
{
    UInt32 timeout;
    pthread_rwlock_obj *obj = (pthread_rwlock_obj *)(&rwlock->sysbios);

    if (_pthread_abstime2ticks(CLOCK_REALTIME, abstime, &timeout) != 0) {
        return (EINVAL);
    }

    return (rdlockAcquire(obj, timeout));
}

/*
 *  ======== pthread_rwlock_timedwrlock ========
 */
int pthread_rwlock_timedwrlock(pthread_rwlock_t *rwlock,
        const struct timespec *abstime)
{
    UInt32 timeout;
    pthread_rwlock_obj *obj = (pthread_rwlock_obj *)(&rwlock->sysbios);

    if (_pthread_abstime2ticks(CLOCK_REALTIME, abstime, &timeout) != 0) {
        return (EINVAL);
    }

    if (Semaphore_pend(Semaphore_handle(&(obj->sem)), timeout)) {
        Assert_isTrue(obj->owner == NULL, 0);
        obj->owner = pthread_self();
        Assert_isTrue(obj->activeReaderCnt == 0, 0);
        return (0);
    }

    return (ETIMEDOUT);
}

/*
 *  ======== pthread_rwlock_tryrdlock ========
 */
int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock)
{
    pthread_rwlock_obj *obj = (pthread_rwlock_obj *)(&rwlock->sysbios);

    return (rdlockAcquire(obj, 0));
}

/*
 *  ======== pthread_rwlock_trywrlock ========
 */
int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock)
{
    pthread_rwlock_obj *obj = (pthread_rwlock_obj *)(&rwlock->sysbios);

    if (Semaphore_pend(Semaphore_handle(&(obj->sem)), 0)) {
        Assert_isTrue(obj->owner == NULL, 0);
        obj->owner = pthread_self();
        Assert_isTrue(obj->activeReaderCnt == 0, 0);
        return (0);
    }

    return (EBUSY);
}

/*
 *  ======== pthread_rwlock_unlock ========
 */
int pthread_rwlock_unlock(pthread_rwlock_t *rwlock)
{
    UInt key;
    int i;
    pthread_rwlock_obj *obj = (pthread_rwlock_obj *)(&rwlock->sysbios);

    key = Task_disable();

    if (obj->activeReaderCnt) {
        Assert_isTrue(obj->blockedReaderCnt == 0, 0);
        /*  Lock is held by a reader.  The last active reader
         *  releases the semaphore.
         */
        if (--obj->activeReaderCnt == 0) {
            obj->owner = NULL;
            Semaphore_post(Semaphore_handle(&(obj->sem)));
        }
    }
    else {
        /*  Lock is held by a writer.  Release the semaphore and
         *  if there are any blocked readers, unblock all of them.
         *  By unblocking all readers, we ensure that the highest
         *  priority reader is unblocked.
         */
        Semaphore_post(Semaphore_handle(&(obj->sem)));

        /* unblock all readers */
        for (i = 0; i < obj->blockedReaderCnt; i++) {
            Semaphore_post(Semaphore_handle(&(obj->readSem)));
        }

        Assert_isTrue(obj->owner == pthread_self(), 0);
        obj->owner = NULL;
        Assert_isTrue(obj->activeReaderCnt == 0, 0);
    }

    Task_restore(key);

    return (0);
}

/*
 *  ======== pthread_rwlock_wrlock ========
 */
int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock)
{
    pthread_rwlock_obj *obj = (pthread_rwlock_obj *)(&rwlock->sysbios);

    Semaphore_pend(Semaphore_handle(&(obj->sem)), BIOS_WAIT_FOREVER);

    Assert_isTrue(obj->owner == NULL, 0);
    obj->owner = pthread_self();
    Assert_isTrue(obj->activeReaderCnt == 0, 0);
    return (0);
}


/*
 *************************************************************************
 *                      internal functions
 *************************************************************************
 */

/*
 *  ======== rdlockAcquire ========
 */
static int rdlockAcquire(pthread_rwlock_obj *rwlock, UInt timeout)
{
    UInt32 curTicks;
    UInt32 prevTicks;
    UInt32 deltaTicks;
    UInt key;

    key = Task_disable();

    if (rwlock->activeReaderCnt > 0) {
        /* The semaphore is owned by a reader, no need to pend. */
        rwlock->activeReaderCnt++;
        Task_restore(key);
        return (0);
    }

    if (Semaphore_pend(Semaphore_handle(&(rwlock->sem)), 0)) {
        /* got the semaphore */
        rwlock->activeReaderCnt++;
        Assert_isTrue(rwlock->activeReaderCnt == 1, 0);

        Assert_isTrue(rwlock->owner == NULL, 0);
        rwlock->owner = pthread_self();

        /*  Either there are no blocked readers, or a writer has just
         *  unlocked rwlock->sem and posted all the blocked readers.
         */
        Task_restore(key);
        return (0);
    }

    if (timeout == 0) {
        Task_restore(key);
        return (EBUSY);
    }

    rwlock->blockedReaderCnt++;
    curTicks = Clock_getTicks();

    Task_restore(key);

    for (;;) {
        if (!Semaphore_pend(Semaphore_handle(&(rwlock->readSem)), timeout)) {
            key = Task_disable();
            rwlock->blockedReaderCnt--;
            Task_restore(key);
            return (ETIMEDOUT);
        }

        key = Task_disable();

        /*  If another reader is active, the rwlock->sem is owned
         *  by a reader, so just increment the active reader count.
         */
        if (rwlock->activeReaderCnt > 0) {
            rwlock->blockedReaderCnt--;
            rwlock->activeReaderCnt++;
            Task_restore(key);
            return (0);
        }

        /*  We have been unblocked by a writer.  Try again to take the
         *  rwlock->sem, since another writer or reader may have taken
         *  it in the meantime.
         */
        if (Semaphore_pend(Semaphore_handle(&(rwlock->sem)), 0)) {
            /* got it */
            rwlock->blockedReaderCnt--;
            rwlock->activeReaderCnt++;

            Assert_isTrue(rwlock->activeReaderCnt == 1, 0);
            Assert_isTrue(rwlock->owner == NULL, 0);
            rwlock->owner = pthread_self();

            Task_restore(key);
            return (0);
        }

        if (timeout != BIOS_WAIT_FOREVER) {
            prevTicks = curTicks;
            curTicks = Clock_getTicks();
            deltaTicks = curTicks - prevTicks;

            if (deltaTicks >= timeout) {
                /* timeout without acquiring the lock */
                rwlock->blockedReaderCnt--;
                Task_restore(key);
                break;
            }
            timeout -= deltaTicks;
        }

        Task_restore(key);
    }

    return (ETIMEDOUT);
}
