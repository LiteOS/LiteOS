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
 *  ======== pthread_barrier.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/Assert.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>

#include <pthread.h>
#include <errno.h>

/*
 *  ======== pthread_barrier_obj ========
 */
typedef struct {
    Semaphore_Struct sem;
    int count;
    int pendCount;
} pthread_barrier_obj;


/*
 *************************************************************************
 *                      pthread_barrierattr
 *************************************************************************
 */
/*
 *  ======== pthread_barrierattr_destroy ========
 */
int pthread_barrierattr_destroy(pthread_barrierattr_t *attr)
{
    return (0);
}

/*
 *  ======== pthread_barrierattr_init ========
 */
int pthread_barrierattr_init(pthread_barrierattr_t *attr)
{
    return (0);
}

/*
 *************************************************************************
 *                      pthread_barrier
 *************************************************************************
 */

/*
 *  ======== pthread_barrier_destroy ========
 */
int pthread_barrier_destroy(pthread_barrier_t *barrier)
{
    pthread_barrier_obj *obj = (pthread_barrier_obj *)(&barrier->sysbios);

    Semaphore_destruct(&(obj->sem));

    return (0);
}

/*
 *  ======== pthread_barrier_init ========
 */
int pthread_barrier_init(pthread_barrier_t *barrier,
        const pthread_barrierattr_t *attr, unsigned count)
{
    pthread_barrier_obj *obj = (pthread_barrier_obj *)(&barrier->sysbios);

    /* object size validation */
    Assert_isTrue(sizeof(pthread_barrier_obj)<=sizeof(pthread_barrier_t),NULL);

    if (count == 0) {
        return (EINVAL);
    }

    obj->count = count;
    obj->pendCount = 0;

    /* default semaphore mode is Semaphore_Mode_COUNTING */
    Semaphore_construct(&(obj->sem), 0, NULL);

    return (0);
}

/*
 *  ======== pthread_barrier_wait ========
 */
int pthread_barrier_wait(pthread_barrier_t *barrier)
{
    UInt key;
    int i;
    pthread_barrier_obj *obj = (pthread_barrier_obj *)(&barrier->sysbios);

    key = Task_disable();

    if (++obj->pendCount < obj->count) {
        Task_restore(key);
        Semaphore_pend(Semaphore_handle(&(obj->sem)), BIOS_WAIT_FOREVER);
        return (0);
    }
    else {
        for (i = 0; i < obj->count - 1; i++) {
            Semaphore_post(Semaphore_handle(&(obj->sem)));
        }

        /* re-initialize the barrier */
        obj->pendCount = 0;

        Task_restore(key);

        /*  pthread_barrier_wait() returns PTHREAD_BARRIER_SERIAL_THREAD
         *  for one arbitrarily chosen thread, so we'll choose the
         *  last one to wait.  The return value for all other threads
         *  is 0.
         */
        return (PTHREAD_BARRIER_SERIAL_THREAD);
    }
}
