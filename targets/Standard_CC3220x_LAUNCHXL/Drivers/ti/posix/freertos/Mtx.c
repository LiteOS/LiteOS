/*
 * Copyright (c) 2017 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== Mtx.c ========
 *  Multithread support for IAR only
 */

/* make sure this is IAR compiler */
#ifndef __IAR_SYSTEMS_ICC__
#error Must use IAR compiler.
#endif

#include <yvals.h>
#include <DLib_Threads.h>

#include <FreeRTOS.h>   /* defines MTX_TLS_INDEX via FreeRTOSConfig.h */
#include <portmacro.h>
#include <task.h>
#include <semphr.h>

#include "Mtx.h"


/* lock struct definition */
typedef struct {
    TaskHandle_t owner;
    int count;
    SemaphoreHandle_t mutex;
} Mtx_Lock;


/*
 *  ======== Mtx_getTlsPtr ========
 */
#pragma section="__iar_tls$$DATA"
void *Mtx_getTlsPtr(void)
{
    TaskHandle_t task;
    void *buf;

    /* check if still running in main() */
    if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) {
        return (__section_begin("__iar_tls$$DATA"));
    }

    /* running in a secondary thread */
    task = xTaskGetCurrentTaskHandle();
    buf = pvTaskGetThreadLocalStoragePointer(task, MTX_TLS_INDEX);

    if (buf == NULL) {
        /*  If memory allocation fails, the error path may return to this
         *  function, resulting in an infinite call loop. To prevent this
         *  loop, temporarily assign the global TLS buffer to the current
         *  task. Once the new TLS buffer has been allocated, assign it
         *  to the current task.
         */
        buf = __section_begin("__iar_tls$$DATA");
        vTaskSetThreadLocalStoragePointer(task, MTX_TLS_INDEX, buf);

        buf = pvPortMalloc(__iar_tls_size());

        if (buf != NULL) {
            __iar_tls_init((char *)buf);
            vTaskSetThreadLocalStoragePointer(task, MTX_TLS_INDEX, buf);
        }
        else {
            /* error: out of memory */
            volatile int spin = 1;
            while (spin);
        }
    }

    return (buf);
}

/*
 *  ======== Mtx_taskDeleteHook ========
 */
void Mtx_taskDeleteHook(void *task)
{
    void *buf;
    buf = pvTaskGetThreadLocalStoragePointer(task, MTX_TLS_INDEX);

    __call_thread_dtors();

    if (buf != NULL) {
        vPortFree(buf);
    }
}

/*
 *  ======== Mtx_initLock ========
 */
void Mtx_initLock(void **ptr)
{
    Mtx_Lock *lock;

    /* allocate the lock object */
    lock = pvPortMalloc(sizeof(Mtx_Lock));

    if (lock == NULL) {
        /* error: out of memory */
        volatile int spin = 1;
        while (spin);
    }

    /* create a mutex */
    lock->mutex = xSemaphoreCreateMutex();

    if (lock->mutex == NULL) {
        vPortFree(lock);

        /* error: out of memory */
        volatile int spin = 1;
        while (spin);
    }

    /* initialize owner and count */
    lock->owner = NULL;
    lock->count = 0;

    /* store the pointer to lock */
    *ptr = lock;
}

/*
 *  ======== Mtx_destroyLock ========
 */
void Mtx_destroyLock(void **ptr)
{
    Mtx_Lock *lock;

    lock = (Mtx_Lock *)*ptr;
    vSemaphoreDelete(lock->mutex);
    vPortFree(lock);
}

/*
 *  ======== Mtx_acquireLock ========
 */
void Mtx_acquireLock(void **ptr)
{
    Mtx_Lock *lock;
    TaskHandle_t self;

    /* get the lock pointer */
    lock = (Mtx_Lock *)*ptr;

    /* check if still running in main() */
    if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) {
        self = (TaskHandle_t)(~0);
    }
    else {
        self = xTaskGetCurrentTaskHandle();
    }

    /* if nested call is detected, increment lock count */
    if (lock->owner == self) {
        (lock->count)++;
    }
    else {
        /* acquire the lock */
        xSemaphoreTake(lock->mutex, portMAX_DELAY);
        lock->count = 1;
        lock->owner = self;
    }
}

/*
 *  ======== Mtx_releaseLock ========
 */
void Mtx_releaseLock(void **ptr)
{
    Mtx_Lock  *lock;

    /* get the lock pointer */
    lock = (Mtx_Lock *)*ptr;

    /* decrement the counter */
    (lock->count)--;

    /* release the lock */
    if (lock->count == 0) {
        lock->owner = NULL;
        xSemaphoreGive(&(lock->mutex));
    }
}

/*  These are trampoline functions to avoid the need for adding the
 *  --redirect option to the linker command.
 */
void __iar_system_Mtxinit(void **ptr)
{
    Mtx_initLock(ptr);
}

void __iar_system_Mtxdst(void **ptr)
{
    Mtx_destroyLock(ptr);
}

void __iar_system_Mtxlock(void **ptr)
{
    Mtx_acquireLock(ptr);
}

void __iar_system_Mtxunlock(void **ptr)
{
    Mtx_releaseLock(ptr);
}

void __iar_file_Mtxinit(void **ptr)
{
    Mtx_initLock(ptr);
}

void __iar_file_Mtxdst(void **ptr)
{
    Mtx_destroyLock(ptr);
}

void __iar_file_Mtxlock(void **ptr)
{
    Mtx_acquireLock(ptr);
}

void __iar_file_Mtxunlock(void **ptr)
{
    Mtx_releaseLock(ptr);
}

void __iar_Initdynamiclock(void **ptr)
{
    Mtx_initLock(ptr);
}

void __iar_Dstdynamiclock(void **ptr)
{
    Mtx_destroyLock(ptr);
}

void __iar_Lockdynamiclock(void **ptr)
{
    Mtx_acquireLock(ptr);
}

void __iar_Unlockdynamiclock(void **ptr)
{
    Mtx_releaseLock(ptr);
}

void *__aeabi_read_tp(void)
{
    return(Mtx_getTlsPtr());
}
