/*
 * Copyright (c) 2015-2018 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== pthread_mutex.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Queue.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>

#include <pthread.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>

#include "_pthread.h"
#include "pthread_util.h"

/*  Compute base address of parent structure
 *
 *  Given the address (ptr) of a member (m) of a structure (s), compute
 *  the structure base address. Useful when traversing a list of embedded
 *  Queue_Elem objects. The Queue_Elem object may be anywhere in the
 *  structure (i.e. it need *not* be the first member of the structure).
 */
#define BASE_ADDR(ptr, s, m) ((char *)(ptr) - (int)(&(((s *)0)->m)))

/*
 *  ======== mutex_prio_obj ========
 *  Mutex priority structure
 */
typedef struct mutex_prio_obj {
    /*  Each thread maintains a list of PTHREAD_PRIO_INHERIT and
     *  PTHREAD_PRIO_PROTECT mutexes it owns.
     */
    Queue_Elem qElem;

    int protocol;

    /*  The prioceiling will be used by both PTHREAD_PRIO_INHERIT and
     *  PTHREAD_PRIO_PROTECT mutexes.
     *
     *  For a PTHREAD_PRIO_PROTECT mutex, prioceiling is set at
     *  initialization to the pthread_mutexattr_t prioceiling, and
     *  can be changed at runtime with pthread_mutex_setprioceiling().
     *
     *  For a PTHREAD_PRIO_INHERIT mutex, the prioceiling will be set
     *  to the priority of the highest priority thread waiting on the
     *  mutex. If no thread is waiting on the mutex, its prioceiling
     *  will be 0.
     *
     *  A thread holding a PTHREAD_PRIO_PROTECT or PTHREAD_PRIO_INHERIT
     *  mutex must run at a priority >= the mutex prioceiling.
     */
    int prioceiling;

    /*  For a PTHREAD_PRIO_INHERIT mutex, keep a list of threads waiting
     *  on the mutex. If the timeout expires for one of the waiting threads,
     *  we may need to lower the priority of the thread that owns the mutex.
     */
    Queue_Struct waitList;
} mutex_prio_obj;

/*
 *  ======== pthread_mutex_obj ========
 *  SYS/BIOS implementation of a POSIX mutex
 *
 *  This object must align with 'struct sysbios_Mutex' (_internal.h).
 */
typedef struct pthread_mutex_obj {
    pthread_Obj        *owner;
    int                 lockCnt;
    int                 type;
    Semaphore_Struct    sem;
    mutex_prio_obj     *mpo;    /* used only with priority mutex enabled */
} pthread_mutex_obj;

static int acquireMutex(pthread_mutex_obj *mutex, UInt32 timeout);
static int acquireMutexNone(pthread_mutex_obj *mutex, UInt32 timeout);

#ifdef ti_posix_tirtos_Settings_enableMutexPriority__D
static int acquireMutexPriority(pthread_mutex_obj *mutex, UInt32 timeout);
static void adjustPri(pthread_Obj *thread);
static void removeMutex(pthread_mutex_obj *mutex);
static void resetInheritPriority(pthread_mutex_obj *mutex);
static void setInheritPriority(pthread_mutex_obj *mutex, int priority);
static int setPrioCeiling(pthread_mutex_obj *mutex);
#endif

/*
 *  Default mutex attrs.
 */
static pthread_mutexattr_t defAttrs = {
    PTHREAD_MUTEX_DEFAULT, /* type */
    PTHREAD_PRIO_NONE,     /* protocol */
    1,                     /* prioceiling */
};

/*
 *************************************************************************
 *                      pthread_mutexattr
 *************************************************************************
 */
/*
 *  ======== pthread_mutexattr_destroy ========
 */
int pthread_mutexattr_destroy(pthread_mutexattr_t *attr)
{
    return (0);
}

/*
 *  ======== pthread_mutexattr_getprioceiling ========
 */
int pthread_mutexattr_getprioceiling(const pthread_mutexattr_t *attr,
        int *prioceiling)
{
#ifdef ti_posix_tirtos_Settings_enableMutexPriority__D
    *prioceiling = attr->prioceiling;
    return (0);
#else
    return (ENOSYS);
#endif
}

/*
 *  ======== pthread_mutexattr_gettype ========
 */
int pthread_mutexattr_gettype(const pthread_mutexattr_t *attr, int *type)
{
    *type = attr->type;
    return (0);
}

/*
 *  ======== pthread_mutexattr_getprotocol ========
 */
int pthread_mutexattr_getprotocol(const pthread_mutexattr_t *attr,
        int *protocol)
{
#ifdef ti_posix_tirtos_Settings_enableMutexPriority__D
    *protocol = attr->protocol;
    return (0);
#else
    return (ENOSYS);
#endif
}

/*
 *  ======== pthread_mutexattr_init ========
 */
int pthread_mutexattr_init(pthread_mutexattr_t *attr)
{
    *attr = defAttrs;
    return (0);
}

/*
 *  ======== pthread_mutexattr_setprioceiling ========
 */
int pthread_mutexattr_setprioceiling(pthread_mutexattr_t *attr,
        const int prioceiling)
{
#ifdef ti_posix_tirtos_Settings_enableMutexPriority__D
    if ((prioceiling >= (int)Task_numPriorities) || (prioceiling < 1)) {
        /* Bad priority value */
        return (EINVAL);
    }

    attr->prioceiling = prioceiling;
    return (0);
#else
    return (ENOSYS);
#endif
}

/*
 *  ======== pthread_mutexattr_setprotocol ========
 */
int pthread_mutexattr_setprotocol(pthread_mutexattr_t *attr, int protocol)
{
#ifdef ti_posix_tirtos_Settings_enableMutexPriority__D
    if ((protocol != PTHREAD_PRIO_NONE) &&
            (protocol != PTHREAD_PRIO_INHERIT) &&
            (protocol != PTHREAD_PRIO_PROTECT)) {
        return (EINVAL);
    }

    attr->protocol = protocol;
    return (0);
#else
    return (ENOSYS);
#endif
}

/*
 *  ======== pthread_mutexattr_settype ========
 */
int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type)
{
    if ((type != PTHREAD_MUTEX_NORMAL) && (type != PTHREAD_MUTEX_RECURSIVE) &&
            (type != PTHREAD_MUTEX_ERRORCHECK)) {
        return (EINVAL);
    }

    attr->type = type;
    return (0);
}

/*
 *************************************************************************
 *                      pthread_mutex
 *************************************************************************
 */


/*
 *  ======== pthread_mutex_destroy ========
 */
int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
    pthread_mutex_obj *obj = (pthread_mutex_obj *)(&mutex->sysbios);

    if (obj->owner != NULL) {
        return (EBUSY);
    }

#ifdef ti_posix_tirtos_Settings_enableMutexPriority__D
    if (!Queue_empty(Queue_handle(&obj->mpo->waitList))) {
        return (EBUSY);
    }

    Queue_destruct(&obj->mpo->waitList);
    Memory_free(Task_Object_heap(), obj->mpo, sizeof(mutex_prio_obj));
#endif

    Semaphore_destruct(&obj->sem);

    return (0);
}

/*
 *  ======== pthread_mutex_getprioceiling ========
 *
 *  Retuns:
 *    0      - Success.
 *    EINVAL - Mutex protocol is not PTHREAD_PRIO_PROTECT
 */
int pthread_mutex_getprioceiling(const pthread_mutex_t *mutex, int *prioceiling)
{
#ifdef ti_posix_tirtos_Settings_enableMutexPriority__D
    pthread_mutex_obj *obj = (pthread_mutex_obj *)(&mutex->sysbios);


    if (obj->mpo->protocol != PTHREAD_PRIO_PROTECT) {
        return (EINVAL);
    }

    *prioceiling = obj->mpo->prioceiling;
    return (0);
#else
    return (ENOSYS);
#endif
}

/*
 *  ======== pthread_mutex_init ========
 */
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
{
    pthread_mutex_obj *obj = (pthread_mutex_obj *)(&mutex->sysbios);
    Semaphore_Params semParams;
    Error_Block eb;
    pthread_mutexattr_t *mAttrs;

    /* object size validation */
    Assert_isTrue(sizeof(pthread_mutex_obj) <= sizeof(pthread_mutex_t), NULL);

    Error_init(&eb);

    obj->owner = NULL;
    obj->lockCnt = 0;
    mAttrs = (attr == NULL) ? &defAttrs : (pthread_mutexattr_t *)attr;
    obj->type = mAttrs->type;

#ifdef ti_posix_tirtos_Settings_enableMutexPriority__D
    obj->mpo = (mutex_prio_obj *)Memory_alloc(Task_Object_heap(),
            sizeof(mutex_prio_obj), 0, &eb);

    if (obj->mpo == NULL) {
        return (ENOMEM);
    }
#else
    obj->mpo = NULL;
#endif

    Semaphore_Params_init(&semParams);
    semParams.mode = Semaphore_Mode_BINARY;
    Semaphore_construct(&obj->sem, 1, &semParams);

#ifdef ti_posix_tirtos_Settings_enableMutexPriority__D
    obj->mpo->protocol = mAttrs->protocol;
    obj->mpo->prioceiling = (mAttrs->protocol == PTHREAD_PRIO_PROTECT) ?
            mAttrs->prioceiling : 0;

    Queue_elemClear(&obj->mpo->qElem);
    Queue_construct(&obj->mpo->waitList, NULL);
#endif

    return (0);
}

/*
 *  ======== pthread_mutex_lock ========
 *
 *  Retuns:
 *    0      - Lock was successfully acquired.
 *    EINVAL - Mutex protocol is PTHREAD_PRIO_PROTECT and mutex prioceiling
 *             is lower than calling thread's priority
 *    EDEADLK - Mutex type is PTHREAD_MUTEX_ERRORCHECK and calling thread
 *              already owns the mutex.
 */
int pthread_mutex_lock(pthread_mutex_t *mutex)
{
    pthread_mutex_obj *obj = (pthread_mutex_obj *)(&mutex->sysbios);

    return (acquireMutex(obj, BIOS_WAIT_FOREVER));
}

/*
 *  ======== pthread_mutex_setprioceiling ========
 *
 *  Retuns:
 *    0      - Success.
 *    EINVAL - Mutex protocol is not PTHREAD_PRIO_PROTECT
 *           - prioceiling is out of range.
 */
int pthread_mutex_setprioceiling(pthread_mutex_t *mutex, int prioceiling,
        int *oldceiling)
{
#ifdef ti_posix_tirtos_Settings_enableMutexPriority__D
    pthread_mutex_obj *obj = (pthread_mutex_obj *)(&mutex->sysbios);

    int                ret;
    int                priority;

    if (obj->mpo->protocol != PTHREAD_PRIO_PROTECT) {
        return (EINVAL);
    }

    if ((prioceiling >= (int)Task_numPriorities) || (prioceiling < 1)) {
        /* Bad priority value */
        return (EINVAL);
    }

    /*
     *  TODO: Check if new ceiling is lower than thread's priority
     *  Scheduled or running?  The thread's priority may have been
     *  boosted because it owns a PTHREAD_PRIO_INHERIT or another
     *  PTHREAD_PRIO_PROTECT mutex with a higher ceiling.
     */

    /*
     *  Lock the mutex (may block), then change the mutex
     *  priority ceiling and unlock the mutex.  Don't need to
     *  adhere to the PTHREAD_PRIO_PROTECT protocol when locking
     *  the mutex.  If we already own the mutex, check to see if
     *  this thread's priority needs to be bumped up.
     *  This call will deadlock if the mutex type is PTHREAD_MUTEX_NORMAL
     *  and we already own it.
     *
     *  TODO: If the calling thread has priority greater than
     *  the current priority ceiling of the mutex, we don't want
     *  to return an error.
     */
    if ((ret = pthread_mutex_lock(mutex)) == 0) {
        *oldceiling = obj->mpo->prioceiling;
        obj->mpo->prioceiling = prioceiling;

        if (obj->lockCnt > 1) {
            /*
             *  Set this thread's priority to the new ceiling, if
             *  higher than the thread's priority
             */
            priority = Task_getPri(Task_self());
            if (priority < prioceiling) {
                Task_setPri(Task_self(), prioceiling);
            }
        }

        pthread_mutex_unlock(mutex);
    }

    return (ret);
#else
    return (ENOSYS);
#endif
}

/*
 *  ======== pthread_mutex_timedlock ========
 *
 *  Retuns:
 *    0      - Lock was successfully acquired.
 *    EINVAL - Mutex protocol is PTHREAD_PRIO_PROTECT and mutex prioceiling
 *             is lower than calling thread's priority
 *           - Bad nanoseconds value: < 0 or > 1000000000
 *    ETIMEDOUT - timeout expired before mutex could be acquired
 *    EDEADLK - Mutex type is PTHREAD_MUTEX_ERRORCHECK and calling thread
 *              already owns the mutex.
 */
int pthread_mutex_timedlock(pthread_mutex_t *mutex,
        const struct timespec *abstime)
{
    pthread_mutex_obj *obj = (pthread_mutex_obj *)(&mutex->sysbios);
    UInt32 timeout;
    int retc;

    /* must attempt operation before validating abstime */
    retc = acquireMutex(obj, 0);

    if (retc != EBUSY) {
        return (retc);
    }

    if (_pthread_abstime2ticks(CLOCK_REALTIME, abstime, &timeout) != 0) {
        return (EINVAL);
    }

    /* requested abstime has already passed */
    if (timeout == 0) {
        return (ETIMEDOUT);
    }

    return (acquireMutex(obj, timeout));
}

/*
 *  ======== pthread_mutex_trylock ========
 *  Retuns:
 *    0      - Lock was successfully acquired.
 *    EINVAL - Mutex protocol is PTHREAD_PRIO_PROTECT and mutex prioceiling
 *             is lower than calling thread's priority
 *    EBUSY  - The mutex is already locked and mutex is owned by another
 *             thread or not recursive.
 *    EDEADLK - Mutex type is PTHREAD_MUTEX_ERRORCHECK and calling thread
 *              already owns the mutex.
 */
int pthread_mutex_trylock(pthread_mutex_t *mutex)
{
    pthread_mutex_obj *obj = (pthread_mutex_obj *)(&mutex->sysbios);

    return (acquireMutex(obj, 0));
}

/*
 *  ======== pthread_mutex_unlock ========
 *  Retuns:
 *    0      - The mutex was successfully released.
 *    EPERM  - Calling thread does not own the mutex.
 */
int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
    pthread_mutex_obj *obj = (pthread_mutex_obj *)(&mutex->sysbios);

    UInt               key;

    key = Task_disable();

    if (obj->owner != (pthread_Obj *)pthread_self()) {
        Task_restore(key);
        return (EPERM);
    }

    obj->lockCnt--;

    if (obj->lockCnt == 0) {
        obj->owner = NULL;

#ifdef ti_posix_tirtos_Settings_enableMutexPriority__D
        if (obj->mpo->protocol == PTHREAD_PRIO_INHERIT) {
            obj->mpo->prioceiling = 0;
        }

        /* This may change the thread's priority */
        if ((obj->mpo->protocol == PTHREAD_PRIO_PROTECT) ||
                (obj->mpo->protocol == PTHREAD_PRIO_INHERIT)) {
            removeMutex(obj);
        }
#endif
        Semaphore_post(Semaphore_handle(&obj->sem));
    }

    Task_restore(key);

    return (0);
}

/*
 *************************************************************************
 *                      Internal functions
 *************************************************************************
 */

#ifdef ti_posix_tirtos_Settings_enableMutexPriority__D
/*
 *  ======== _pthread_getMaxPrioCeiling ========
 *  Return the maximum of the priority ceilings of the PTHREAD_PRIO_PROTECT
 *  and PTHREAD_PRIO_INHERIT mutexes owned by the thread.
 */
int _pthread_getMaxPrioCeiling(pthread_Obj *thread)
{
    mutex_prio_obj     *mutex_prio;
    int                 maxPri = 0;
    int                 pri;
    UInt                key;
    Queue_Handle        list;
    Queue_Elem         *elem;

    /*  If the thread is holding a PTHREAD_PRIO_PROTECT mutex and
     *  running at its ceiling, we don't want to set its priority
     *  to a lower value.  Instead, we save the new priority to set
     *  it to, once the mutexes of higher priority ceilings are
     *  released.
     */
    key = Task_disable();

    list = Queue_handle(&(thread->mutexList));
    elem = Queue_head(list);

    while (elem != (Queue_Elem *)list) {
        mutex_prio = (mutex_prio_obj *)(BASE_ADDR(elem, mutex_prio_obj, qElem));
        pri = mutex_prio->prioceiling;
        maxPri = (pri > maxPri) ? pri : maxPri;
        elem = Queue_next(elem);
    }

    Task_restore(key);

    return (maxPri);
}
#endif

/*
 *  ======== acquireMutex ========
 */
static int acquireMutex(pthread_mutex_obj *mutex, UInt32 timeout)
{
#ifdef ti_posix_tirtos_Settings_enableMutexPriority__D
    if ((mutex->mpo->protocol == PTHREAD_PRIO_PROTECT) ||
            (mutex->mpo->protocol == PTHREAD_PRIO_INHERIT)) {
        return (acquireMutexPriority(mutex, timeout));
    }
    else {
        return (acquireMutexNone(mutex, timeout));
    }
#else
    return (acquireMutexNone(mutex, timeout));
#endif
}

/*
 *  ======== acquireMutexNone ========
 */
static int acquireMutexNone(pthread_mutex_obj *mutex, UInt32 timeout)
{
    UInt               key;
    pthread_Obj       *thisThread;

    thisThread = (pthread_Obj *)pthread_self();

    /* Return if the thread already owns the mutex */
    if (mutex->owner == thisThread) {
        if (mutex->type == PTHREAD_MUTEX_ERRORCHECK) {
            return (EDEADLK);
        }
        if (mutex->type == PTHREAD_MUTEX_RECURSIVE) {
            mutex->lockCnt++;
            return (0);
        }

        /*
         *  If we get here, the mutex type is PTHREAD_MUTEX_NORMAL, so
         *  deadlock will occur (with an infinite timeout).
         */
        Assert_isTrue(mutex->type != PTHREAD_MUTEX_NORMAL, 0);
    }

    /* See if the lock is available first */
    if (!Semaphore_pend(Semaphore_handle(&(mutex->sem)), 0)) {
        if (timeout == 0) {
            return (EBUSY);
        }

        if (!Semaphore_pend(Semaphore_handle(&(mutex->sem)), timeout)) {
            return (ETIMEDOUT);
        }

        /*
         *  At this point, we have the semaphore, but a higher priority
         *  task could preempt and try to acquire the mutex.  The owner
         *  of the mutex is still NULL.
         */
    }

    /* Got the semaphore, now set the owner, etc. */
    key = Task_disable();

    Assert_isTrue(mutex->owner == NULL, 0);
    Assert_isTrue(mutex->lockCnt == 0, 0);

    mutex->owner = thisThread;
    mutex->lockCnt = 1;

    Task_restore(key);

    return (0);
}

#ifdef ti_posix_tirtos_Settings_enableMutexPriority__D
/*
 *  ======== acquireMutexPriority ========
 */
static int acquireMutexPriority(pthread_mutex_obj *mutex, UInt32 timeout)
{
    UInt               key;
    Int                thisThreadPri;
    pthread_Obj       *thisThread;
    Queue_Handle       waitList;
    Queue_Handle       mutexList;

    thisThread = (pthread_Obj *)pthread_self();

    key = Task_disable();

    thisThreadPri = Task_getPri(thisThread->task);

    if ((mutex->mpo->protocol == PTHREAD_PRIO_PROTECT) &&
            (mutex->mpo->prioceiling < thisThread->priority)) {
        Task_restore(key);
        return (EINVAL);
    }

    /* Return if the thread already owns the mutex */
    if (mutex->owner == thisThread) {
        if (mutex->type == PTHREAD_MUTEX_ERRORCHECK) {
            Task_restore(key);
            return (EDEADLK);
        }
        if (mutex->type == PTHREAD_MUTEX_RECURSIVE) {
            mutex->lockCnt++;
            Task_restore(key);

            return (0);
        }

        /*
         *  If we get here, the mutex type is PTHREAD_MUTEX_NORMAL, so
         *  deadlock will occur with an infinite timeout.
         */
        Assert_isTrue(timeout != BIOS_WAIT_FOREVER, 0);
    }

    /* See if the lock is available first */
    if (!Semaphore_pend(Semaphore_handle(&(mutex->sem)), 0)) {
        if (timeout == 0) {
            Task_restore(key);
            return (EBUSY);
        }

        /*
         *  Another thread owns the mutex (or this one does and type is
         *  NORMAL).
         *  For PTHREAD_PRIO_INHERIT mutex, bump the priority of the
         *  mutex owner, if it is lower than the calling thread's priority.
         */
        if (mutex->mpo->protocol == PTHREAD_PRIO_INHERIT) {

            /*
             *  Boost the mutex owner's priority
             *  Note: The calling thread may own a PTHREAD_PRIO_PROTECT
             *  mutex with prioceiling less than the calling thread's
             *  priority.
             */
            setInheritPriority(mutex, thisThreadPri);

            /*
             *  Put this thread at the end of the queue.  Priorty
             *  can be changed (eg, if this thread owns a mutex that
             *  a higher priority thread tries to acquire), so there
             *  is no point in sorting the queue.
             */
            waitList = Queue_handle(&(mutex->mpo->waitList));
            Queue_enqueue(waitList, (Queue_Elem *)thisThread);
            thisThread->blockedMutex = (pthread_mutex_t *)mutex;
        }

        Task_restore(key);

        if (!Semaphore_pend(Semaphore_handle(&(mutex->sem)), timeout)) {

            /* Adjust prioceiling if mutex protocol is PTHREAD_PRIO_INHERIT */
            if (mutex->mpo->protocol == PTHREAD_PRIO_INHERIT) {

                key = Task_disable();

                thisThread->blockedMutex = NULL;

                /* Remove thread from mutex wait queue */
                Queue_remove((Queue_Elem *)thisThread);

                /* Re-adjust inherited priorities, if necessary. */
                resetInheritPriority(mutex);

                Task_restore(key);
            }

            return (ETIMEDOUT);
        }

        /*
         *  At this point, we have the semaphore, but a higher priority
         *  task could preempt and try to acquire the mutex.  The owner
         *  of the mutex is still NULL.
         */

        /* Got the semaphore, now set the owner, etc. */
        key = Task_disable();

        thisThread->blockedMutex = NULL;

        if (mutex->mpo->protocol == PTHREAD_PRIO_INHERIT) {
            /* Remove thread from mutex wait queue */
            Queue_remove((Queue_Elem *)thisThread);
        }
    }

    Assert_isTrue(mutex->owner == NULL, 0);
    Assert_isTrue(mutex->lockCnt == 0, 0);

    mutex->owner = thisThread;
    mutex->lockCnt = 1;

    if (mutex->mpo->protocol == PTHREAD_PRIO_INHERIT) {
        /*
         *  Check the waitList.  The owner would have set the priority
         *  ceiling to 0 when unlocking the mutex, but there may still
         *  be threads waiting.  The waitList could also have been
         *  modified after this thread got the semaphore, but before
         *  we called Task_disable() to set the owner.
         */
        setPrioCeiling(mutex);
    }

    /* Add the mutex to this thread's list of owned mutexes */
    if ((mutex->mpo->protocol == PTHREAD_PRIO_PROTECT) ||
            (mutex->mpo->protocol == PTHREAD_PRIO_INHERIT)) {

        mutexList = Queue_handle(&(thisThread->mutexList));
        Queue_enqueue(mutexList, &(mutex->mpo->qElem));

        thisThreadPri = Task_getPri(thisThread->task);

        if (mutex->mpo->prioceiling > thisThreadPri) {
            Task_setPri(Task_self(), mutex->mpo->prioceiling);
        }
    }

    Task_restore(key);

    return (0);
}
#endif

#ifdef ti_posix_tirtos_Settings_enableMutexPriority__D
/*
 *  ======== adjustPri ========
 *  Adjust thread's priorty to the max of the priority ceilings of the
 *  mutexes it owns and its scheduled priority.  Must be called with
 *  Task scheduling disabled.
 */
static void adjustPri(pthread_Obj *thread)
{
    int                 maxCeiling;
    int                 priority = thread->priority;

    maxCeiling = _pthread_getMaxPrioCeiling(thread);

    priority = (maxCeiling > priority) ? maxCeiling : priority;
    Task_setPri(thread->task, priority);
}
#endif

#ifdef ti_posix_tirtos_Settings_enableMutexPriority__D
/*
 *  ======== removeMutex ========
 *  Remove a PTHREAD_PRIO_PROTECT or PTHREAD_PRIO_INHERIT mutex from the
 *  queue and restore priority, if necessary.
 *  Call with Task scheduling disabled.
 */
static void removeMutex(pthread_mutex_obj *mutex)
{
    pthread_Obj        *thread = (pthread_Obj *)pthread_self();

    Queue_remove(&(mutex->mpo->qElem));
    adjustPri(thread);
}
#endif

#ifdef ti_posix_tirtos_Settings_enableMutexPriority__D
/*
 *  ======== resetInheritPriority ========
 *  Called after a timeout, to recalculate inherited priorities
 */
static void resetInheritPriority(pthread_mutex_obj *mutex)
{
    pthread_Obj       *owner;
    pthread_mutex_obj *next;
    int                priority;
    int                ownerPri;

    owner = mutex->owner;
    next = mutex;

    while (owner != NULL) {
        ownerPri = _pthread_getRunningPriority(owner);

        /*
         *  Set the priority ceiling of the mutex to the priority of the
         *  highest priority thread blocked on the mutex.
         */
        priority = setPrioCeiling(next);

        /*
         *  We only need to adjust the owning thread's priority if
         *  it is higher than the mutex priority ceiling.
         */
        if (ownerPri <= priority) {
            break;
        }

        /*
         *  The owner will set its priority to the maximum of its priority
         *  and the priority ceilings of all the mutexes it owns.
         */
        adjustPri(owner);

        next = (pthread_mutex_obj *)(&owner->blockedMutex->sysbios);
        owner = (next == NULL ? NULL : next->owner);
    }
}
#endif

#ifdef ti_posix_tirtos_Settings_enableMutexPriority__D
/*
 *  ======== setInheritPriority ========
 */
static void setInheritPriority(pthread_mutex_obj *mutex, int priority)
{
    pthread_Obj       *owner;
    pthread_mutex_obj *next;
    int                ownerPri;

    owner = mutex->owner;
    next = mutex;

    while (owner != NULL) {
        ownerPri = _pthread_getRunningPriority((pthread_t)owner);

        if (ownerPri < priority) {
            /* Boost the mutex owner's priority */
            _pthread_setRunningPriority((pthread_t)owner, priority);
            next->mpo->prioceiling = priority;
        }
        else {
            /*
             *  The chain of threads will have increasing (>=) priorities,
             *  due to inheritence, so there is no reason to continue
             *  when we reach an owner with priority >= this thread's
             *  priority.  We also don't want to loop forever, in case
             *  there is a cyclic dependency on the mutexes.
             */
            break;
        }

        next = (pthread_mutex_obj *)(&owner->blockedMutex->sysbios);
        owner = (next == NULL ? NULL : next->owner);
    }
}
#endif

#ifdef ti_posix_tirtos_Settings_enableMutexPriority__D
/*
 *  ======== setPrioCeiling ========
 */
static int setPrioCeiling(pthread_mutex_obj *mutex)
{
    pthread_t          pthread;
    Queue_Handle       waitList;
    int                priority = 0;

    Assert_isTrue(mutex->mpo->protocol == PTHREAD_PRIO_INHERIT, 0);

    waitList = Queue_handle(&mutex->mpo->waitList);
    pthread = (pthread_t)Queue_head(waitList);
    mutex->mpo->prioceiling = 0;

    while (pthread != (pthread_t)waitList) {
        priority = _pthread_getRunningPriority(pthread);
        if (priority > mutex->mpo->prioceiling) {
            mutex->mpo->prioceiling = priority;
        }
        pthread = (pthread_t)Queue_next((Queue_Elem *)pthread);
    }

    return (priority);
}
#endif
