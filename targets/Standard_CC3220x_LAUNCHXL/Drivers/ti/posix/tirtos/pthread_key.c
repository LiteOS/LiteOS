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
 *  ======== pthread_key.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Queue.h>

#include <pthread.h>
#include <errno.h>

#include "_pthread.h"


/*
 *  ======== pthread_key_Obj ========
 *  Each key will have a list of threads that have called pthread_setspecific()
 *  for the given key.  In addition, each thread will have a list of the keys
 *  it has called pthread_setspecific() on.  By having the thread maintain its
 *  key list, lookups for pthread_getspecific() will be faster.  Having the
 *  key maintain the list of threads will facilitate removal of the objects
 *  created by pthread_getspecific() when pthread_key_delete() is called.
 *
 *  An optional destructor function can be passed to pthread_key_create().
 *  If the destructor is non-NULL, it will be called when a thread
 *  exits and passed the thread-specific data for that key (if non-NULL).
 */
typedef struct pthread_key_Obj {
    void (*destructor)(void *);
    Queue_Struct threadList;    /* Queue of pthread_KeyData objects */
} pthread_key_Obj;

/*
 *  ======== pthread_ThreadData ========
 *  Each thread maintains a list of the key specific data it has set.  Objects
 *  of this type are queued in the thread's key list.
 */
typedef struct pthread_ThreadData {
    Queue_Elem               qElem;
    pthread_key_Obj         *key;
    void                    *specData;  /* Pointer to thread specific data */
    struct pthread_KeyData  *keyData;   /* Back pointer for easy removal of
                                         * thread data from key's thread list */
} pthread_ThreadData;

/*
 *  ======== pthread_KeyData ========
 *  The pthread_key_Obj needs to maintain a list of threads that have
 *  specific data for that key.  This way, when the key is deleted, we
 *  can easily remove the key specific object from the thread's key list.
 */
typedef struct pthread_KeyData {
    Queue_Elem            qElem;
    pthread_ThreadData    threadData;
} pthread_KeyData;

static Semaphore_Struct  sem;         /* Semaphore to protect key lists */
static Bool isInitialized = FALSE;    /* Set to true when sem is constructed */

/*
 *************************************************************************
 *                      pthread_key
 *************************************************************************
 */

/*
 *  ======== pthread_key_create ========
 */
int pthread_key_create(pthread_key_t *key, void (*destructor)(void*))
{
    pthread_key_Obj *newKey;
    Error_Block      eb;
    UInt             taskKey;

    taskKey = Task_disable();
    if (!isInitialized) {
        Semaphore_construct(&sem, 1, NULL);
        isInitialized = TRUE;
    }
    Task_restore(taskKey);

    Error_init(&eb);
    newKey = (pthread_key_Obj *)Memory_alloc(Task_Object_heap(),
            sizeof(pthread_key_Obj), 0, &eb);

    if (newKey == NULL) {
        *key = NULL;
        return (ENOMEM);
    }

    Queue_construct(&(newKey->threadList), NULL);

    newKey->destructor = destructor;

    *key = (void *)newKey;

    return (0);
}

/*
 *  ======== pthread_key_delete ========
 */
int pthread_key_delete(pthread_key_t key)
{
    pthread_key_Obj    *keyObj = (pthread_key_Obj *)key;
    pthread_KeyData    *keyData;
    int                 oldState;

    /* prevent thread cancellation while holding the mutex */
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldState);

    Semaphore_pend(Semaphore_handle(&sem), BIOS_WAIT_FOREVER);

    /* Delete thread-specific elements associated with the key */
    while (!Queue_empty(Queue_handle(&(keyObj->threadList)))) {
        keyData = Queue_get(Queue_handle(&keyObj->threadList));

        /* Remove the object from the thread's list */
        Queue_remove((Queue_Elem *)&(keyData->threadData));

        Memory_free(Task_Object_heap(), keyData, sizeof(pthread_KeyData));
    }

    Memory_free(Task_Object_heap(), keyObj, sizeof(pthread_key_Obj));

    Semaphore_post(Semaphore_handle(&sem));

    /* restore previous cancelability state, this may never return */
    pthread_setcancelstate(oldState, &oldState);

    return (0);
}

/*
 *  ======== pthread_getspecific ========
 */
void *pthread_getspecific(pthread_key_t key)
{
    Queue_Elem         *elem;
    pthread_ThreadData *threadSpec;
    pthread_Obj        *thread = (pthread_Obj *)pthread_self();
    void               *threadData = NULL;
    int                 oldState;

    /* prevent thread cancellation while holding the mutex */
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldState);

    /*  Traverse the list of keys set by the thread, looking for key.
     *  This list could be modified while we are traversing it, if
     *  another thread calls pthread_key_delete() for a different key and
     *  this thread has specific data for the key being deleted.
     *  Therefore we acquire the semaphore to prevent pthread_key_delete()
     *  from modifying the list.
     */
    Semaphore_pend(Semaphore_handle(&sem), BIOS_WAIT_FOREVER);

    elem = Queue_head(Queue_handle(&(thread->keyList)));
    while (elem != (Queue_Elem *)Queue_handle(&thread->keyList)) {
        threadSpec = (pthread_ThreadData *)elem;
        if (threadSpec->key == (pthread_key_Obj *)key) {
            /* Found the key */
            threadData = threadSpec->specData;
            break;
        }
        elem = Queue_next(elem);
    }

    Semaphore_post(Semaphore_handle(&sem));

    /* restore previous cancelability state, this may never return */
    pthread_setcancelstate(oldState, &oldState);

    return (threadData);
}

/*
 *  ======== pthread_setspecific ========
 */
int pthread_setspecific(pthread_key_t key, const void *value)
{
    Queue_Elem          *elem;
    pthread_key_Obj     *keyObj;
    pthread_KeyData     *keyData;
    pthread_ThreadData  *threadSpec;
    Error_Block          eb;
    Bool                 found = FALSE;
    int                  retVal = 0;
    pthread_Obj         *thread = (pthread_Obj *)pthread_self();
    int                 oldState;

    Error_init(&eb);

    /* prevent thread cancellation while holding the mutex */
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldState);

    /*  Traverse the list of keys set by the thread, looking for key.  If
     *  the key is already in the list, re-assign its value.  Otherwise
     *  add the key to the thread's list.
     */
    Semaphore_pend(Semaphore_handle(&sem), BIOS_WAIT_FOREVER);

    elem = Queue_head(Queue_handle(&(thread->keyList)));
    while (elem != (Queue_Elem *)Queue_handle(&thread->keyList)) {
        threadSpec = (pthread_ThreadData *)elem;
        if (threadSpec->key == (pthread_key_Obj *)key) {
            /* Found the key */
            threadSpec->specData = (void *)value;
            found = TRUE;
            break;
        }
        elem = Queue_next(elem);
    }

    if (!found) {
        /*
         *  Allocate object for thread data and add to the key's list.
         *  The threadData part will be added to the thread's list.
         */
        keyData = (pthread_KeyData *)Memory_alloc(
            Task_Object_heap(), sizeof(pthread_KeyData), 0, &eb);
        if (keyData == NULL) {
            retVal = ENOMEM;
        }
        else {
            keyData->threadData.key = key;
            keyData->threadData.specData = (void *)value;
            keyData->threadData.keyData = keyData;
            Queue_put(Queue_handle(&(thread->keyList)),
                    (Queue_Elem *)(&(keyData->threadData)));

            keyObj = (pthread_key_Obj *)key;
            Queue_put(Queue_handle(&(keyObj->threadList)),
                            (Queue_Elem *)keyData);
        }
    }

    Semaphore_post(Semaphore_handle(&sem));

    /* restore previous cancelability state, this may never return */
    pthread_setcancelstate(oldState, &oldState);

    return (retVal);
}

/*
 *  ======== _pthread_removeThreadKeys ========
 *  This function is called when a thread exits.  We go through the list
 *  of keys and free the thread's pthread_keySpecObj for the key.
 */
void _pthread_removeThreadKeys(pthread_t pthread)
{
    pthread_Obj            *thread = (pthread_Obj *)pthread;
    Queue_Elem             *elem;
    pthread_key_Obj        *keyObj;
    pthread_KeyData        *keyData;
    pthread_ThreadData     *threadData;
    UInt                    taskKey;

    taskKey = Task_disable();

    if (Queue_empty(Queue_handle(&(thread->keyList)))) {
        Task_restore(taskKey);
        return;
    }
    Task_restore(taskKey);

    Semaphore_pend(Semaphore_handle(&sem), BIOS_WAIT_FOREVER);

    elem = Queue_head(Queue_handle(&(thread->keyList)));
    while (elem != (Queue_Elem *)Queue_handle(&thread->keyList)) {
        threadData = (pthread_ThreadData *)elem;
        keyData = threadData->keyData;
        keyObj = threadData->key;

        /* Get the next in the queue before we free this one */
        elem = Queue_next(elem);

        /* Remove from the thread's queue */
        Queue_remove((Queue_Elem *)threadData);

        /* Remove from the key's queue */
        Queue_remove((Queue_Elem *)keyData);

        if ((keyObj->destructor != NULL) && (threadData->specData != NULL)) {
            (keyObj->destructor)(threadData->specData);
        }
        Memory_free(Task_Object_heap(), keyData, sizeof(pthread_KeyData));
    }

    Semaphore_post(Semaphore_handle(&sem));
}
