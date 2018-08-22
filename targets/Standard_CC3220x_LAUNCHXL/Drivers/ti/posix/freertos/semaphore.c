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
 *  ======== semaphore.c ========
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* FreeRTOS header files */
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <portmacro.h>

#include <ti/drivers/dpl/HwiP.h>

#include <errno.h>
#include <semaphore.h>
#include <time.h>

/* temporary until limits.h is restored, TIRTOS-1315 */
#define SEM_VALUE_MAX 65535

typedef struct {
    SemaphoreHandle_t sem;
} sem_obj;

/*
 *  Must have configUSE_COUNTING_SEMAPHORES == 1 in FreeRTOSConfig.h
 *
 *  Note:  Posix semaphore does not support binary mode.  If you want a
 *  binary semaphore, you can implement it with a condition variable and
 *  a mutex.
 */

/*
 *  Function for obtaining a timeout in Clock ticks from the difference of
 *  an absolute time and the current time.
 */
extern int _clock_abstime2ticks(clockid_t clockId,
        const struct timespec *abstime, uint32_t *ticks);

static int semaphorePend(SemaphoreHandle_t sem, TickType_t ticks);

/*
 *  ======== sem_destroy ========
 */
int sem_destroy(sem_t *semaphore)
{
    sem_obj *obj = (sem_obj*)(&semaphore->freertos);

    vSemaphoreDelete(obj->sem);
    return (0);
}

/*
 *  ======== sem_getvalue ========
 */
int sem_getvalue(sem_t *semaphore, int *value)
{
    UBaseType_t count;
    sem_obj *obj = (sem_obj*)(&semaphore->freertos);

    count = uxSemaphoreGetCount(obj->sem);
    *value = (int)count;
    return (0);
}

/*
 *  ======== sem_init ========
 */
int sem_init(sem_t *semaphore, int pshared, unsigned value)
{
    sem_obj *obj = (sem_obj*)(&semaphore->freertos);

    /* TODO object size validation */
//  assert(sizeof(sem_obj) <= sizeof(sem_t));

    if (value > SEM_VALUE_MAX) {
        errno = EINVAL;
        return (-1);
    }

    /*  FreeRTOS xSemaphoreCreateCounting() creates a queue of
     *  length maxCount, where maxCount is the maximum count that
     *  the semaphore should ever reach.  It looks like this should be
     *  ok since the item length for a semaphore queue is 0, so the
     *  memory allocated is not dependent on maxCount.
     */
    obj->sem = xSemaphoreCreateCounting((UBaseType_t)SEM_VALUE_MAX,
            (UBaseType_t)value);

    if (obj->sem == NULL) {
        errno = ENOSPC;
        return (-1);
    }

    return (0);
}

/*
 *  ======== sem_post ========
 */
int sem_post(sem_t *semaphore)
{
    BaseType_t xHigherPriorityTaskWoken;
    BaseType_t result;
    sem_obj *obj = (sem_obj*)(&semaphore->freertos);

    if (HwiP_inISR()) {
        result = xSemaphoreGiveFromISR(obj->sem, &xHigherPriorityTaskWoken);

        if (result != pdTRUE) {
            /* queue is full */
            return (-1);
        }
    }
    else {
        xSemaphoreGive(obj->sem);
    }

    return (0);
}

/*
 *  ======== sem_timedwait ========
 */
int sem_timedwait(sem_t *semaphore, const struct timespec *abstime)
{
    int retc = 0;
    uint32_t timeout;
    sem_obj *obj = (sem_obj*)(&semaphore->freertos);

    /* don't bother checking the time if the semaphore is available */
    if (semaphorePend(obj->sem, 0) == 0) {
        return (0);
    }

    if (_clock_abstime2ticks(CLOCK_REALTIME, abstime, &timeout) != 0) {
        errno = EINVAL;
        return (-1);
    }

    retc = semaphorePend(obj->sem, (TickType_t)timeout);

    if (retc == -1) {
        errno = ETIMEDOUT;
    }

    return (retc);
}

/*
 *  ======== sem_trywait ========
 */
int sem_trywait(sem_t *semaphore)
{
    int retc;
    sem_obj *obj = (sem_obj*)(&semaphore->freertos);

    retc = semaphorePend(obj->sem, 0);

    if (retc == -1) {
        errno = EAGAIN;
    }

    return (retc);
}

/*
 *  ======== sem_wait ========
 */
int sem_wait(sem_t *semaphore)
{
    sem_obj *obj = (sem_obj*)(&semaphore->freertos);

    semaphorePend(obj->sem, portMAX_DELAY);
    return (0);
}

/*
 *************************************************************************
 *                      Internal functions
 *************************************************************************
 */

/*
 *  ======== semaphorePend ========
 */
static int semaphorePend(SemaphoreHandle_t sem, TickType_t ticks)
{
    BaseType_t status = xSemaphoreTake(sem, ticks);
    return (status == pdTRUE ? 0 : -1);
}
