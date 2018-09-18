/*
 * Copyright (c) 2016-2018 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== pthread.c ========
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

typedef void *(*pthread_RunFxn)(void *);

typedef struct ListElem {
    struct ListElem *next;
    struct ListElem *prev;
} ListElem, List;

/*
 *  ======== pthread_key_Obj ========
 *  Each key will have a list of threads that have called pthread_setspecific()
 *  for the given key.  In addition, each thread will have a list of the keys
 *  it has called pthread_setspecific() on.  By having the thread maintain its
 *  key list, pthread_getspecific() lookups will be faster.  Having the
 *  key maintain the list of threads will facilitate removal of the objects
 *  created by pthread_getspecific() when pthread_key_delete() is called.
 *
 *  An optional destructor function can be passed to pthread_key_create().
 *  If the destructor is non-NULL, it will be called when a thread
 *  exits and passed the thread-specific data for that key.
 */
typedef struct pthread_key_Obj {
    /* Optional destructor passed to pthread_key_create() */
    void (*destructor)(void *);

    /* List of objects containing thread specific data for the key */
    List keyDataList;
} pthread_key_Obj;

/*
 *  ======== ThreadKeyData ========
 *  Each thread maintains a list of the key specific data it has set.  Objects
 *  of this type are queued in the thread's key list.
 */
typedef struct ThreadKeyData {
    ListElem              elem;

    /* The key and data passed to pthread_setspecific() */
    pthread_key_Obj      *key;
    void                 *specData;

    /*
     *  Back pointer for easy removal of thread data from key's list when
     *  the thread exits.
     */
    struct KeyData       *keyData;
} ThreadKeyData;

/*
 *  ======== KeyData ========
 *  The pthread_key_Obj needs to maintain a list of threads that have
 *  specific data for that key.  This way, when the key is deleted, we
 *  can easily remove the key specific object from the thread's key list.
 */
typedef struct KeyData {
    ListElem         elem;
    ThreadKeyData    threadKeyData;
} KeyData;


/*
 *  ======== pthread_Obj ========
 */
typedef struct pthread_Obj {
    TaskHandle_t       freeRTOSTask;
    int                priority;

    SemaphoreHandle_t  joinSem;

    pthread_t          joinThread;

    bool               detached;
    pthread_RunFxn     fxn;
    void              *arg;
    int                cancelState;
    int                cancelPending;

    /* Thread function return value */
    void              *ret;

    /* Cleanup handlers */
    struct _pthread_cleanup_context *cleanupList;

    /* List of keys that the thread has called pthread_setspecific() on */
    List               keyList;
} pthread_Obj;

static void _pthread_runStub(void  *arg);

/*
 *  ======== defaultPthreadAttrs ========
 */
static pthread_attr_t defaultPthreadAttrs = {
    1,                          /* priority */
    NULL,                       /* stack */
    configPOSIX_STACK_SIZE      /* stacksize - must align for all kernels */
        * sizeof(portSTACK_TYPE),
    0,                          /* guardsize */
    PTHREAD_CREATE_JOINABLE     /* detachstate */
};

static void removeThreadKeys(pthread_t pthread);


static SemaphoreHandle_t mutex = NULL;
static bool isInitialized = false;

/*
 *************************************************************************
 *                      pthread_attr
 *************************************************************************
 */
/*
 *  ======== pthread_attr_destroy ========
 */
int pthread_attr_destroy(pthread_attr_t *attr)
{
    return (0);
}

/*
 *  ======== pthread_attr_getdetachstate ========
 */
int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate)
{
    *detachstate = attr->detachstate;
    return (0);
}

/*
 *  ======== pthread_attr_getguardsize ========
 */
int pthread_attr_getguardsize(const pthread_attr_t *attr, size_t *guardsize)
{
    *guardsize = attr->guardsize;
    return (0);
}

/*
 *  ======== pthread_attr_getschedparam ========
 */
int pthread_attr_getschedparam(const pthread_attr_t *attr,
        struct sched_param *schedparam)
{
    schedparam->sched_priority = attr->priority;
    return (0);
}

/*
 *  ======== pthread_attr_getstack ========
 */
int pthread_attr_getstack (const pthread_attr_t *attr, void **stackaddr,
        size_t *stacksize)
{
    // TODO: This should be removed.  Can't pass stack to xTaskCreate()
    *stackaddr = attr->stack;
    *stacksize = attr->stacksize;
    return (0);
}

/*
 *  ======== pthread_attr_getstacksize ========
 */
int pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize)
{
    *stacksize = attr->stacksize;
    return (0);
}

/*
 *  ======== pthread_attr_init ========
 */
int pthread_attr_init(pthread_attr_t *attr)
{
    *attr = defaultPthreadAttrs;

    return (0);
}

/*
 *  ======== pthread_attr_setdetachstate ========
 */
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate)
{
    if ((detachstate != PTHREAD_CREATE_JOINABLE) &&
            (detachstate != PTHREAD_CREATE_DETACHED)) {
        return (EINVAL);
    }

    attr->detachstate = detachstate;
    return (0);
}

/*
 *  ======== pthread_attr_setguardsize ========
 */
int pthread_attr_setguardsize(pthread_attr_t *attr, size_t guardsize)
{
    attr->guardsize = guardsize;
    return (0);
}

/*
 *  ======== pthread_attr_setschedparam ========
 */
int pthread_attr_setschedparam(pthread_attr_t *attr,
        const struct sched_param *schedparam)
{
    int     priority = schedparam->sched_priority;

    /*
     *  Task priorities can range from 0 to configMAX_PRIORITIES - 1,
     *  where configMAX_PRIORITIES is defined in FreeRTOSConfig.h.  The
     *  idle task has priority 0, and other tasks can share this priority
     *  (configIDLE_SHOULD_YIELD = 1).
     *  Make sure that idle task is not starved, since it is responsible
     *  for freeing up memory of deleted tasks.
     */

    if ((priority >= configMAX_PRIORITIES) || (priority <= tskIDLE_PRIORITY)) {
        /* Bad priority value */
        return (EINVAL);
    }
    attr->priority = priority;
    return (0);
}

/*
 *  ======== pthread_attr_setstacksize ========
 */
int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize)
{
    attr->stacksize = stacksize;
    return (0);
}

/*
 *************************************************************************
 *                      pthread
 *************************************************************************
 */

/*
 *  ======== pthread_cancel ========
 *  The specification of this API is that it be used as a means for one thread
 *  to termintate the execution of another thread.  There is no mention of
 *  returning an error if the argument, pthread, is the same thread as the
 *  calling thread.
 *  Only support asynchronous cancellation.  Deferred cancellation requires
 *  that certain functions have cancellation points (e.g., socket and file i/o
 *  functions).
 */
int pthread_cancel(pthread_t pthread)
{
    pthread_Obj  *thread = (pthread_Obj *)pthread;

    /* Disable the scheduler */
    vTaskSuspendAll();

    /* Indicate that cancellation is requested. */
    thread->cancelPending = 1;

    if (thread->cancelState == PTHREAD_CANCEL_ENABLE) {
        vTaskSuspend(thread->freeRTOSTask);

        /* Pop and execute the cleanup handlers */
        while (thread->cleanupList != NULL) {
            _pthread_cleanup_pop(thread->cleanupList, 1);
        }

        /* Cleanup any pthread specific data */
        removeThreadKeys(pthread);

        if (thread->detached) {
            /*  Tasks must not return (attempting to do so will result
             *  in configASSERT() if defined).  If the task must exit, it
             *  must call vTaskDelete(NULL) to ensure a clean exit.
             */
            vTaskDelete(thread->freeRTOSTask);
            vPortFree(thread);
        }
        else {
            /* The joining thread will clean up */
            thread->ret = PTHREAD_CANCELED;
            xSemaphoreGive(thread->joinSem);
        }
    }

    /* Re-enable the scheduler */
    xTaskResumeAll();

    return (0);
}

/*
 *  ======== pthread_create ========
 */
int pthread_create(pthread_t *newthread, const pthread_attr_t *attr,
        void *(*startroutine)(void *), void *arg)
{
    BaseType_t        status;
    pthread_Obj      *thread = NULL;
    pthread_attr_t   *pAttr;
    int               retVal = 0;
    bool              schedulerStarted;

    pAttr = (attr == NULL) ? &defaultPthreadAttrs : (pthread_attr_t *)attr;

    thread = pvPortMalloc(sizeof(pthread_Obj));
    if (thread == NULL) {
        return (ENOMEM);
    }

    thread->detached = (pAttr->detachstate == PTHREAD_CREATE_JOINABLE) ?
            false : true;
    thread->fxn = startroutine;
    thread->arg = arg;
    thread->joinThread = NULL;
    thread->joinSem = NULL;
    thread->cancelState = PTHREAD_CANCEL_ENABLE;
    thread->cancelPending = 0;
    thread->priority = pAttr->priority;
    thread->cleanupList = NULL;
    thread->keyList.next = thread->keyList.prev = NULL;

    if (attr->detachstate == PTHREAD_CREATE_JOINABLE) {
        /*
         *  Create the join semaphore for pthread_join().  The semaphore
         *  is created in the 'empty' state.
         */
        thread->joinSem = xSemaphoreCreateBinary();
        if (thread->joinSem == NULL) {
            vPortFree(thread);
            return (ENOMEM);
        }
    }

    schedulerStarted =
        (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) ? false : true;

    if (schedulerStarted) {
        /* Disable the scheduler */
        vTaskSuspendAll();
    }

    status = xTaskCreate((TaskFunction_t)_pthread_runStub, NULL /* name */,
            ((uint16_t)pAttr->stacksize) / sizeof(portSTACK_TYPE),
            (void *)thread, pAttr->priority, &(thread->freeRTOSTask));

    if (status == pdPASS) {
        /*  Set the task tag to the pthread object so we can use it
         *  in pthread_self().
         */
        vTaskSetApplicationTaskTag(thread->freeRTOSTask,
                (TaskHookFunction_t)thread);
    }
    else {
        if (thread->joinSem != NULL) {
            vSemaphoreDelete(thread->joinSem);
        }
        vPortFree(thread);
        thread = NULL;
        retVal = ENOMEM;
    }

    /* store thread handle before releasing the scheduler */
    *newthread = (pthread_t)thread;

    if (schedulerStarted) {
        /* Re-enable the scheduler */
        xTaskResumeAll();
    }

    return (retVal);
}

/*
 *  ======== pthread_detach ========
 */
int pthread_detach(pthread_t pthread)
{
    pthread_Obj  *thread = (pthread_Obj *)pthread;

    /* Disable the scheduler */
    vTaskSuspendAll();

    if (!thread->detached) {
        /*
         *  If a thread has already joined this thread, the joining
         *  thread must have been canceled before this thread is detached.
         */
        if (thread->joinSem) {
            vSemaphoreDelete(thread->joinSem);
        }
        thread->detached = true;
    }

    /* Re-enable the scheduler */
    xTaskResumeAll();

    return (0);
}

/*
 *  ======== pthread_equal ========
 */
int pthread_equal(pthread_t pt1, pthread_t pt2)
{
    return (pt1 == pt2);
}

/*
 *  ======== pthread_exit ========
 */
void pthread_exit(void *retval)
{
    pthread_Obj *thread = (pthread_Obj *)pthread_self();
    int oldState;

    /*
     *  This function terminates the calling thread and returns
     *  a value via retval that (if the thread is joinable) is available to
     *  another thread that calls pthread_join().
     *
     *  Any clean-up handlers that have not yet been popped, are popped
     *  (in the reverse of the order in which they were pushed) and executed.
     */
    thread->ret = retval;

    /* prevent thread cancellation while terminating myself */
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldState);

    /* Pop and execute the cleanup handlers */
    while (thread->cleanupList != NULL) {
        _pthread_cleanup_pop(thread->cleanupList, 1);
    }

    /* Cleanup any pthread specific data */
    removeThreadKeys((pthread_t)thread);

    if (!thread->detached) {
        xSemaphoreGive(thread->joinSem);

        /* Suspend ourself.  The joining thread will clean up. */
        vTaskSuspend(NULL);
    }
    else {
        /* Free memory */
        vPortFree(thread);

        /*
         *  Tasks must not return (attempting to do so will result
         *  in configASSERT() if defined).  If the task must exit, it
         *  must call vTaskDelete(NULL) to ensure a clean exit.
         */
        vTaskDelete(NULL);
    }
}

/*
 *  ======== pthread_getschedparam ========
 */
int pthread_getschedparam(pthread_t pthread, int *policy,
        struct sched_param *param)
{
    pthread_Obj      *thread = (pthread_Obj *)pthread;

    *policy = SCHED_OTHER;

    /*
     *  Note: This may not be the priority that the task is running
     *  at (eg, if it is holding a mutex that a higher priority thread
     *  is blocked on.
     */
    param->sched_priority = thread->priority;

    return (0);
}

/*
 *  ======== pthread_getspecific ========
 */
void *pthread_getspecific(pthread_key_t key)
{
    pthread_Obj        *thread = (pthread_Obj *)pthread_self();
    ThreadKeyData      *threadData;
    void               *specData = NULL;
    int                 oldState;

    /* prevent thread cancellation while holding the mutex */
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldState);

    xSemaphoreTake(mutex, portMAX_DELAY);

    threadData = (ThreadKeyData *)(thread->keyList.next);

    while (threadData != NULL) {
        if (threadData->key == (pthread_key_Obj *)key) {
            specData = threadData->specData;
            break;
        }

        threadData = (ThreadKeyData *)(threadData->elem.next);
    }

    xSemaphoreGive(mutex);

    /* restore previous cancelability state, this may never return */
    pthread_setcancelstate(oldState, &oldState);

    return (specData);
}


/*
 *  ======== pthread_join ========
 *  Wait for thread to terminate.
 *
 *  If multiple threads simultaneously try to join with the same
 *  thread, the results are undefined.  We will return an error.
 *
 *  If the thread calling pthread_join() is canceled, then the target
 *  thread will remain joinable (i.e., it will not be detached).
 */
int pthread_join(pthread_t pthread, void **thread_return)
{
    pthread_Obj  *thread = (pthread_Obj *)pthread;

    /* Disable the scheduler */
    vTaskSuspendAll();

    if ((thread->joinThread != NULL) || (thread->detached != 0)) {
        /*
         *  Error - Another thread has already called pthread_join()
         *  for this thread, or the thread is in the detached state.
         */
        /* Re-enable the scheduler */
        xTaskResumeAll();

        return (EINVAL);
    }

    if (pthread == pthread_self()) {
        /* Re-enable the scheduler */
        xTaskResumeAll();

        return (EDEADLK);
    }

    thread->joinThread = pthread_self();

    /* Re-enable the scheduler */
    xTaskResumeAll();

    xSemaphoreTake(thread->joinSem, portMAX_DELAY);

    if (thread_return) {
        *thread_return = thread->ret;
    }

    /* Clean up the joined thread */
    vSemaphoreDelete(thread->joinSem);

    vTaskDelete(thread->freeRTOSTask);

    vPortFree(thread);

    return (0);
}

/*
 *  ======== pthread_key_create ========
 */
int pthread_key_create(pthread_key_t *key, void (*destructor)(void*))
{
    pthread_key_Obj *newKey;

    *key = NULL;

    /* Disable the scheduler */
    vTaskSuspendAll();

    if (!isInitialized) {
        /* Create mutex for protecting key lists */
        mutex = xSemaphoreCreateMutex();
        if (mutex == NULL) {
            xTaskResumeAll();
            return (ENOMEM);
        }

        isInitialized = true;
    }

    /* Re-enable the scheduler */
    xTaskResumeAll();

    newKey = pvPortMalloc(sizeof(pthread_key_Obj));
    if (newKey == NULL) {
        return (ENOMEM);
    }

    newKey->destructor = destructor;
    newKey->keyDataList.next = newKey->keyDataList.prev = NULL;

    *key = (pthread_key_t)newKey;

    return (0);
}

/*
 *  ======== pthread_key_delete ========
 */
int pthread_key_delete(pthread_key_t key)
{
    pthread_key_Obj    *keyObj = (pthread_key_Obj *)key;
    KeyData            *keyData, *next;
    ThreadKeyData      *threadData; //, *tmpThreadData;
    int                 oldState;

    /* prevent thread cancellation while holding the mutex */
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldState);

    xSemaphoreTake(mutex, portMAX_DELAY);

    keyData = (KeyData *)(keyObj->keyDataList.next);

    while (keyData != NULL) {
        next = (KeyData *)(keyData->elem.next);

        /* Remove the key from thread's list */
        threadData = (ThreadKeyData *)&(keyData->threadKeyData);
        if (threadData->elem.next != NULL) {
            threadData->elem.next->prev = threadData->elem.prev;
        }
        threadData->elem.prev->next = threadData->elem.next;

        vPortFree((void *)keyData);

        keyData = next;
    }

    vPortFree((void *)keyObj);

    xSemaphoreGive(mutex);

    /* restore previous cancelability state, this may never return */
    pthread_setcancelstate(oldState, &oldState);

    return (0);
}

/*
 *  ======== pthread_once ========
 */
int pthread_once(pthread_once_t *once, void (*initFxn)(void))
{
    bool              schedulerStarted;

    schedulerStarted = (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) ?
            false : true;

    if (schedulerStarted) {
        /* Disable the scheduler */
        vTaskSuspendAll();
    }

    if (*once == PTHREAD_ONCE_INIT) {
        (*initFxn)();
        *once = ~PTHREAD_ONCE_INIT;
    }

    if (schedulerStarted) {
        /* Re-enable the scheduler */
        xTaskResumeAll();
    }

    return (0);
}

/*
 *  ======== pthread_self ========
 */
pthread_t pthread_self(void)
{
    TaskHandle_t task;
    pthread_t    thread;

    task = xTaskGetCurrentTaskHandle();
    thread = (pthread_t)xTaskGetApplicationTaskTag(task);

    return (thread);
}

/*
 *  ======== pthread_setcancelstate ========
 */
int pthread_setcancelstate(int state, int *oldState)
{
    pthread_Obj *thread = (pthread_Obj *)pthread_self();

    if ((state != PTHREAD_CANCEL_ENABLE) && (state != PTHREAD_CANCEL_DISABLE)) {
        return (EINVAL);
    }

    *oldState = thread->cancelState;
    thread->cancelState = state;

    if ((state == PTHREAD_CANCEL_ENABLE) && thread->cancelPending) {
        pthread_exit((void *)PTHREAD_CANCELED);
    }

    return (0);
}

/*
 *  ======== pthread_setschedparam ========
 */
int pthread_setschedparam(pthread_t pthread, int policy,
        const struct sched_param *param)
{
#if ( INCLUDE_vTaskPrioritySet == 1 )
    TaskHandle_t     task = ((pthread_Obj *)pthread)->freeRTOSTask;
    int              priority = param->sched_priority;

    if ((priority >= configMAX_PRIORITIES) || (priority <= tskIDLE_PRIORITY)) {
        return (EINVAL);
    }
    vTaskPrioritySet(task, (BaseType_t)priority);
    return (0);
#else
    return (EPERM);
#endif
}

/*
 *  ======== pthread_setspecific ========
 */
int pthread_setspecific(pthread_key_t key, const void *value)
{
    pthread_Obj        *thread = (pthread_Obj *)pthread_self();
    pthread_key_Obj    *keyObj = (pthread_key_Obj *)key;
    ThreadKeyData      *threadData;
    KeyData            *newKeyData;
    bool                found = false;
    int                 retVal = 0;
    int                 oldState;

    /* prevent thread cancellation while holding the mutex */
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldState);

    /*  Traverse the list of keys set by the thread, looking for key.  If
     *  the key is already in the list, re-assign its value.  Otherwise
     *  add the key to the thread's list.
     */
    xSemaphoreTake(mutex, portMAX_DELAY);

    threadData = (ThreadKeyData *)(thread->keyList.next);

    while (threadData != NULL) {
        if (threadData->key == keyObj) {
            /* Found the key */
            threadData->specData = (void *)value;
            found = true;
            break;
        }

        threadData = (ThreadKeyData *)(threadData->elem.next);
    }

    if (!found) {
        newKeyData = pvPortMalloc(sizeof(KeyData));
        if (newKeyData == NULL) {
            retVal = ENOMEM;
        }
        else {
            newKeyData->threadKeyData.key = keyObj;
            newKeyData->threadKeyData.specData = (void *)value;
            newKeyData->threadKeyData.keyData = newKeyData;

            /* Put newKeyData on the key object's list */
            newKeyData->elem.next = keyObj->keyDataList.next;
            if (keyObj->keyDataList.next) {
                keyObj->keyDataList.next->prev = (ListElem *)newKeyData;
            }
            newKeyData->elem.prev = &(keyObj->keyDataList);
            keyObj->keyDataList.next = (ListElem *)newKeyData;

            /* Put the threadKeyData part on the thread's key list */
            newKeyData->threadKeyData.elem.next = thread->keyList.next;
            newKeyData->threadKeyData.elem.prev = &(thread->keyList);
            if (thread->keyList.next) {
                thread->keyList.next->prev =
                    (ListElem *)&(newKeyData->threadKeyData);
            }
            thread->keyList.next = (ListElem *)&(newKeyData->threadKeyData);
        }
    }

    xSemaphoreGive(mutex);

    /* restore previous cancelability state, this may never return */
    pthread_setcancelstate(oldState, &oldState);

    return (retVal);
}


/*
 *************************************************************************
 *              internal functions
 *************************************************************************
 */

/*
 *  ======== _pthread_cleanup_pop ========
 */
void _pthread_cleanup_pop(struct _pthread_cleanup_context *context,
        int execute)
{
    pthread_Obj    *thread = (pthread_Obj *)context->thread;

    thread->cleanupList = context->next;

    if (execute) {
        (*(context->fxn))(context->arg);
    }
}

/*
 *  ======== _pthread_cleanup_push ========
 */
void _pthread_cleanup_push(struct _pthread_cleanup_context *context,
        void (*fxn)(void *), void *arg)
{
    pthread_Obj    *thread = (pthread_Obj *)pthread_self();

    context->fxn = fxn;
    context->arg = arg;
    context->thread = (pthread_t)thread;
    context->next = thread->cleanupList;
    thread->cleanupList = context;
}

/*
 *  ======== _pthread_runStub ========
 */
static void _pthread_runStub(void *arg)
{
    pthread_Obj *thread = (pthread_Obj *)arg;
    int oldState;

    thread->ret = thread->fxn(thread->arg);

    /* prevent thread cancellation while terminating myself */
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldState);

    /* Pop and execute the cleanup handlers */
    while (thread->cleanupList != NULL) {
        _pthread_cleanup_pop(thread->cleanupList, 1);
    }

    /* Cleanup any pthread specific data */
    removeThreadKeys((pthread_t)thread);

    if (!thread->detached) {
        xSemaphoreGive(thread->joinSem);

        /* Suspend ourself.  The joining thread will clean up. */
        vTaskSuspend(NULL);
    }
    else {
        vPortFree(thread);

        /*  Tasks must not return (attempting to do so will result
         *  in configASSERT() if defined).  If the task must exit, it
         *  must call vTaskDelete(NULL) to ensure a clean exit.
         */
        vTaskDelete(NULL);
    }
}


/*
 *  ======== removeThreadKeys ========
 *  This function is called when a thread exits.  We go through the list
 *  of keys and free the thread's pthread_keySpecObj for the key.
 */
static void removeThreadKeys(pthread_t pthread)
{
    pthread_Obj        *thread = (pthread_Obj *)pthread;
    ThreadKeyData      *threadData;
    KeyData            *keyData;

    /* Disable the scheduler */
    vTaskSuspendAll();

    while (thread->keyList.next) {
        threadData = (ThreadKeyData *)thread->keyList.next;
        thread->keyList.next = threadData->elem.next;

        keyData = threadData->keyData;

        /* Remove keyData from the pthread_key_Obj's list */
        if (keyData->elem.prev) {
            keyData->elem.prev->next = keyData->elem.next;
        }
        if (keyData->elem.next) {
            keyData->elem.next->prev = keyData->elem.prev;
        }

        vPortFree(keyData);
    }

    /* Re-enable the scheduler */
    xTaskResumeAll();
}
