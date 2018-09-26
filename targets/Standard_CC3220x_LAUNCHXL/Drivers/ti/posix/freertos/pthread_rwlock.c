/*
 * Copyright (c) 2016-2017 Texas Instruments Incorporated - http://www.ti.com
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


#include <stdint.h>

#include <FreeRTOS.h>
#include <portmacro.h>
#include <semphr.h>
#include <task.h>

#include <pthread.h>
#include <time.h>
#include <errno.h>

#define MAXCOUNT 65535

/*
 *  ======== pthread_rwlock_obj ========
 */
typedef struct {
    /*
     *  This semaphore must be acquired to obtain a write lock.
     *  A readlock can be obtained if there is already a read lock
     *  acquired, or by acquiring this semaphore.
     */
    SemaphoreHandle_t  sem;

    /*
     *  This semaphore is used to block readers when sem is in use
     *  by a write lock.
     */
    SemaphoreHandle_t  readSem;

    int       activeReaderCnt;   /* Number of read locks acquired */
    int       blockedReaderCnt;  /* Number of readers blocked on readSem */

    /*
     *  The 'owner' is the writer holding the lock, or the first reader
     *  that acquired the lock.
     */
    pthread_t owner;
} pthread_rwlock_obj;

/*
 *  Function for obtaining a timeout in Clock ticks from the difference of
 *  an absolute time and the current time.
 */
extern int _clock_abstime2ticks(clockid_t clockId,
        const struct timespec *abstime, uint32_t *ticks);

static int rdlockAcquire(pthread_rwlock_obj *lock, uint32_t timeout);

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
    pthread_rwlock_obj *obj = (pthread_rwlock_obj *)(&rwlock->freertos);

    /* return EBUSY if the lock is in use */
    if (obj->owner != NULL) {
        return (EBUSY);
    }

    vSemaphoreDelete(obj->sem);
    vSemaphoreDelete(obj->readSem);
    return (0);
}

/*
 *  ======== pthread_rwlock_init ========
 */
int pthread_rwlock_init(pthread_rwlock_t *rwlock,
        const pthread_rwlockattr_t *attr)
{
    pthread_rwlock_obj *obj = (pthread_rwlock_obj *)(&rwlock->freertos);

    /* TODO object size validation */
//  assert(sizeof(pthread_rwlock_obj) <= sizeof(pthread_rwlock_t));

    /*  FreeRTOS xSemaphoreCreateCounting() creates a queue of
     *  length maxCount, where maxCount is the maximum count that
     *  the semaphore should ever reach.  It looks like this should be
     *  ok since the item length for a semaphore queue is 0, so the
     *  memory allocated is not dependent on maxCount.
     */
    /* create semaphore with count of 1 */
    obj->sem = xSemaphoreCreateCounting(MAXCOUNT, 1);
    if (obj->sem == NULL) {
        return (ENOMEM);
    }

    /* create semaphore with count of 0 */
    obj->readSem = xSemaphoreCreateCounting(MAXCOUNT, 0);
    if (obj->readSem == NULL) {
        vPortFree(obj->sem);
        obj->sem = NULL;
        return (ENOMEM);
    }

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
    pthread_rwlock_obj *obj = (pthread_rwlock_obj *)(&rwlock->freertos);

    return (rdlockAcquire(obj, portMAX_DELAY));
}

/*
 *  ======== pthread_rwlock_timedrdlock ========
 */
int pthread_rwlock_timedrdlock(pthread_rwlock_t *rwlock,
        const struct timespec *abstime)
{
    uint32_t timeout;
    pthread_rwlock_obj *obj = (pthread_rwlock_obj *)(&rwlock->freertos);

    if (_clock_abstime2ticks(CLOCK_REALTIME, abstime, &timeout) != 0) {
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
    uint32_t timeout;
    pthread_rwlock_obj *obj = (pthread_rwlock_obj *)(&rwlock->freertos);

    if (_clock_abstime2ticks(CLOCK_REALTIME, abstime, &timeout) != 0) {
        return (EINVAL);
    }

    if (xSemaphoreTake(obj->sem, (TickType_t)timeout) == pdTRUE) {
        obj->owner = pthread_self();
        return (0);
    }

    return (ETIMEDOUT);
}

/*
 *  ======== pthread_rwlock_tryrdlock ========
 */
int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock)
{
    pthread_rwlock_obj *obj = (pthread_rwlock_obj *)(&rwlock->freertos);

    return (rdlockAcquire(obj, 0));
}

/*
 *  ======== pthread_rwlock_trywrlock ========
 */
int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock)
{
    pthread_rwlock_obj *obj = (pthread_rwlock_obj *)(&rwlock->freertos);

    if (xSemaphoreTake(obj->sem, 0) == pdTRUE) {
        obj->owner = pthread_self();
        return (0);
    }

    return (EBUSY);
}

/*
 *  ======== pthread_rwlock_unlock ========
 */
int pthread_rwlock_unlock(pthread_rwlock_t *rwlock)
{
    int i;
    pthread_rwlock_obj *obj = (pthread_rwlock_obj *)(&rwlock->freertos);

    /* disable the scheduler */
    vTaskSuspendAll();

    if (obj->activeReaderCnt) {
        /*  Lock is held by a reader.  The last active reader
         *  releases the semaphore.
         */
        if (--obj->activeReaderCnt == 0) {
            obj->owner = NULL;
            xSemaphoreGive(obj->sem);
        }
    }
    else {
        /*  Lock is held by a writer.  Release the semaphore and
         *  if there are any blocked readers, unblock all of them.
         *  By unblocking all readers, we ensure that the highest
         *  priority reader is unblocked.
         */
        xSemaphoreGive(obj->sem);

        /* unblock all readers */
        for (i = 0; i < obj->blockedReaderCnt; i++) {
            xSemaphoreGive(obj->readSem);
        }

        obj->owner = NULL;
    }

    /* re-enable the scheduler */
    xTaskResumeAll();

    return (0);
}

/*
 *  ======== pthread_rwlock_wrlock ========
 */
int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock)
{
    pthread_rwlock_obj *obj = (pthread_rwlock_obj *)(&rwlock->freertos);

    xSemaphoreTake(obj->sem, portMAX_DELAY);

    obj->owner = pthread_self();
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
static int rdlockAcquire(pthread_rwlock_obj *rwlock, uint32_t timeout)
{
    TickType_t  curTicks;
    TickType_t  prevTicks;
    TickType_t  deltaTicks;

    /* disable the scheduler */
    vTaskSuspendAll();

    if (rwlock->activeReaderCnt > 0) {
        /* The semaphore is owned by a reader, no need to pend. */
        rwlock->activeReaderCnt++;
        xTaskResumeAll();
        return (0);
    }

    if (xSemaphoreTake(rwlock->sem, 0) == pdTRUE) {
        /* got the semaphore */
        rwlock->activeReaderCnt++;
        rwlock->owner = pthread_self();

        /*  Either there are no blocked readers, or a writer has just
         *  unlocked rwlock->sem and posted all the blocked readers.
         */
        xTaskResumeAll();
        return (0);
    }

    if (timeout == 0) {
        xTaskResumeAll();
        return (EBUSY);
    }

    rwlock->blockedReaderCnt++;
    curTicks = xTaskGetTickCount();

    xTaskResumeAll();

    for (;;) {
        if (xSemaphoreTake(rwlock->readSem, (TickType_t)timeout) != pdTRUE) {
            vTaskSuspendAll();
            rwlock->blockedReaderCnt--;
            xTaskResumeAll();
            return (ETIMEDOUT);
        }

        vTaskSuspendAll();

        /*  If another reader is active, the rwlock->sem is owned
         *  by a reader, so just increment the active reader count.
         */
        if (rwlock->activeReaderCnt > 0) {
            rwlock->blockedReaderCnt--;
            rwlock->activeReaderCnt++;
            xTaskResumeAll();
            return (0);
        }

        /*  We have been unblocked by a writer.  Try again to take the
         *  rwlock->sem, since another writer or reader may have taken
         *  it in the meantime.
         */
        if (xSemaphoreTake(rwlock->sem, 0) == pdTRUE) {
            /* got it */
            rwlock->blockedReaderCnt--;
            rwlock->activeReaderCnt++;
            rwlock->owner = pthread_self();
            xTaskResumeAll();
            return (0);
        }

        if (timeout != portMAX_DELAY) {
            prevTicks = curTicks;
            curTicks = xTaskGetTickCount();
            deltaTicks = curTicks - prevTicks;

            if (deltaTicks >= timeout) {
                /* timed out without acquiring the lock */
                rwlock->blockedReaderCnt--;
                xTaskResumeAll();
                break;
            }
            timeout -= deltaTicks;
        }

        xTaskResumeAll();
    }

    return (ETIMEDOUT);
}
