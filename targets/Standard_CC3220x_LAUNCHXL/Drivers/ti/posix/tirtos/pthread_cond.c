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
 *  ======== pthread_cond.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/Assert.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Queue.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>

#include <pthread.h>
#include <time.h>
#include <errno.h>

#include "pthread_util.h"

typedef struct CondElem {
    Queue_Elem qElem;
    Semaphore_Struct sem;
} CondElem;

/*
 *  ======== pthread_cond_obj ========
 */
typedef struct {
    Queue_Struct waitList;
    clockid_t clockId;
} pthread_cond_obj;

static int condWait(pthread_cond_obj *cond, pthread_mutex_t *mutex,
        UInt32 timeout);

/*
 *************************************************************************
 *                      pthread_condattr
 *************************************************************************
 */
/*
 *  ======== pthread_condattr_destroy ========
 */
int pthread_condattr_destroy(pthread_condattr_t *attr)
{
    return (0);
}

/*
 *  ======== pthread_condattr_getclock ========
 */
int pthread_condattr_getclock(const pthread_condattr_t *attr,
        clockid_t *clock_id)
{
    *clock_id = (clockid_t)(*attr);
    return (0);
}

/*
 *  ======== pthread_condattr_init ========
 */
int pthread_condattr_init(pthread_condattr_t * attr)
{
    *attr = (pthread_condattr_t)CLOCK_REALTIME;
    return (0);
}

/*
 *  ======== pthread_condattr_setclock ========
 */
int pthread_condattr_setclock(pthread_condattr_t *attr,
        clockid_t clock_id)
{
    if ((clock_id != CLOCK_REALTIME) && (clock_id != CLOCK_MONOTONIC)) {
        return (EINVAL);
    }

    *attr = (pthread_condattr_t)clock_id;
    return (0);
}

/*
 *************************************************************************
 *                      pthread_cond
 *************************************************************************
 */
/*
 *  ======== pthread_cond_broadcast ========
 */
int pthread_cond_broadcast(pthread_cond_t *cond)
{
    CondElem *condElem;
    UInt key;
    pthread_cond_obj *obj = (pthread_cond_obj *)(&cond->sysbios);

    /*  The calling thread is not required to hold the mutex when
     *  calling pthread_cond_broadcast() or pthread_cond_signal().
     */
    key = Task_disable();

    while (!Queue_empty(Queue_handle(&(obj->waitList)))) {
        condElem = (CondElem *)Queue_dequeue(Queue_handle(&(obj->waitList)));
        Semaphore_post(Semaphore_handle(&condElem->sem));
    }

    Task_restore(key);
    return (0);
}

/*
 *  ======== pthread_cond_destroy ========
 */
int pthread_cond_destroy(pthread_cond_t *cond)
{
    pthread_cond_obj *obj = (pthread_cond_obj *)(&cond->sysbios);

    if (!Queue_empty(Queue_handle(&(obj->waitList)))) {
        return (EBUSY);
    }

    Queue_destruct(&obj->waitList);
    return (0);
}

/*
 *  ======== pthread_cond_init ========
 */
int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr)
{
    pthread_cond_obj *obj = (pthread_cond_obj *)(&cond->sysbios);

    /* object size validation */
    Assert_isTrue(sizeof(pthread_cond_obj) <= sizeof(pthread_cond_t), NULL);

    Queue_construct(&obj->waitList, NULL);

    if (attr != NULL) {
        obj->clockId = (clockid_t)(*attr);
    }
    else {
        obj->clockId = CLOCK_REALTIME;
    }

    return (0);
}

/*
 *  ======== pthread_cond_signal ========
 */
int pthread_cond_signal(pthread_cond_t *cond)
{
    CondElem *elem;
    UInt key;
    pthread_cond_obj *obj = (pthread_cond_obj *)(&cond->sysbios);

    /*  The calling thread is not required to hold the mutex when
     *  calling pthread_cond_broadcast() or pthread_cond_signal().
     */
    key = Task_disable();

    if (!Queue_empty(Queue_handle(&obj->waitList))) {
        /* the calling thread is holding the mutex */
        elem = (CondElem *)Queue_dequeue(Queue_handle(&obj->waitList));
        Queue_elemClear(&elem->qElem);
        Semaphore_post(Semaphore_handle(&elem->sem));
    }

    Task_restore(key);
    return (0);
}

/*
 *  ======== pthread_cond_timedwait ========
 */
int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex,
        const struct timespec *abstime)
{
    UInt32 timeout;
    pthread_cond_obj *obj = (pthread_cond_obj *)(&cond->sysbios);

    /* must validate abstime before modifying mutex state */
    if (_pthread_abstime2ticks(obj->clockId, abstime, &timeout) != 0) {
        return (EINVAL);
    }

    return (condWait(obj, mutex, timeout));
}

/*
 *  ======== pthread_cond_wait ========
 */
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
    pthread_cond_obj *obj = (pthread_cond_obj *)(&cond->sysbios);

    return (condWait(obj, mutex, BIOS_WAIT_FOREVER));
}

/*
 *  ======== condWait ========
 */
static int condWait(pthread_cond_obj *cond, pthread_mutex_t *mutex,
        UInt32 timeout)
{
    CondElem condElem;
    Semaphore_Params semParams;
    int ret = 0;
    UInt key;

    /*  The calling thread is holding mutex but threads signalling
     *  the condition variable are not required to hold the mutex.
     *  Therefore, we need to call Task_disable() to protect the
     *  condition variable's waitList.
     */

    Queue_elemClear(&(condElem.qElem));

    Semaphore_Params_init(&semParams);
    semParams.mode = Semaphore_Mode_BINARY;
    Semaphore_construct(&(condElem.sem), 0, &semParams);

    key = Task_disable();
    Queue_enqueue(Queue_handle(&(cond->waitList)), (Queue_Elem *)&condElem);
    Task_restore(key);

    pthread_mutex_unlock(mutex);

    if (!Semaphore_pend(Semaphore_handle(&condElem.sem), timeout)) {
        key = Task_disable();
        Queue_remove((Queue_Elem *)&condElem);
        Task_restore(key);

        ret = ETIMEDOUT;
    }

    pthread_mutex_lock(mutex);

    return (ret);
}
