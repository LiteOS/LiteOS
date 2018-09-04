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
 *  ======== pthread_barrier.c ========
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <FreeRTOS.h>
#include <portmacro.h>
#include <task.h>
#include <semphr.h>

#include <pthread.h>
#include <errno.h>

struct WaitElem {
    TaskHandle_t task;
    struct WaitElem *next;
};

/*
 *  ======== pthread_barrier_obj ========
 */
typedef struct {
    int               count;
    int               pendCount;
    struct WaitElem  *waitList;
    struct WaitElem  *last;
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
/*  pthread_barrier_obj *obj = (pthread_barrier_obj *)(&barrier->freertos); */

    return (0);
}

/*
 *  ======== pthread_barrier_init ========
 */
int pthread_barrier_init(pthread_barrier_t *barrier,
        const pthread_barrierattr_t *attr, unsigned count)
{
    pthread_barrier_obj *obj = (pthread_barrier_obj *)(&barrier->freertos);

    /* TODO object size validation */
/*  assert(sizeof(pthread_barrier_obj) <= sizeof(pthread_barrier_t)); */

    if (count == 0) {
        return (EINVAL);
    }

    obj->count = count;
    obj->pendCount = 0;
    obj->waitList = NULL;

    return (0);
}

/*
 *  ======== pthread_barrier_wait ========
 */
int pthread_barrier_wait(pthread_barrier_t *barrier)
{
    pthread_barrier_obj *obj = (pthread_barrier_obj *)(&barrier->freertos);
    UBaseType_t priority;
    struct WaitElem myself;
    struct WaitElem *other;

    /*  Raise task priority to eliminate race conditions below and to
     *  effectively disable scheduling (i.e. guarantee execution of the
     *  current task until it blocks).
     */
    priority = uxTaskPriorityGet(NULL);
    vTaskPrioritySet(NULL, configMAX_PRIORITIES - 1);

    if (++obj->pendCount < obj->count) {

        /* add myself to the wait list */
        myself.task = xTaskGetCurrentTaskHandle();
        myself.next = NULL;

        if (obj->waitList == NULL) {
            obj->waitList = &myself;
        }
        else {
            obj->last->next = &myself;
        }
        obj->last = &myself;

        /* wait for other tasks, last one will wake me */
        vTaskSuspend(NULL);

        /* restore my priority */
        vTaskPrioritySet(NULL, priority);
        return (0);
    }
    else {
        /* release everyone on the wait list */
        for (other = obj->waitList; other != NULL; other = other->next) {
            vTaskResume(other->task);
        }

        /* re-initialize the barrier */
        obj->pendCount = 0;
        obj->waitList = NULL;

        /* restore my priority */
        vTaskPrioritySet(NULL, priority);

        /*  pthread_barrier_wait() returns PTHREAD_BARRIER_SERIAL_THREAD
         *  for one arbitrarily chosen thread, so we'll choose the
         *  last one to wait.  The return value for all other threads
         *  is 0.
         */
        return (PTHREAD_BARRIER_SERIAL_THREAD);
    }
}
