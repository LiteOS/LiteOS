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
 *  ======== _pthread.h ========
 */

#ifndef ti_posix_tirtos__pthread__include
#define ti_posix_tirtos__pthread__include

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Queue.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>

#include "pthread.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void *(*pthread_RunFxn)(void *);

/*
 *  ======== pthread_Obj ========
 */
typedef struct pthread_Obj {
    /*
     *  Queue_Elem for mutex.  This is also used to put the thread
     *  on a list of terminated threads.
     *  Each PTHREAD_PRIO_INHERIT mutex maintains a list of threads
     *  waiting on the mutex, so it can adjust its priority ceiling
     *  when pthread_mutex_timedlock() times out.
     */
    ti_sysbios_knl_Queue_Elem        qElem;

#ifdef ti_posix_tirtos_Settings_enableMutexPriority__D
    /*
     *  When a thread acquires a PTHREAD_PRIO_PROTECT mutex, the thread's
     *  priority will be boosted to the priority ceiling of the mutex, if
     *  that is higher than the thread's priority.  A thread's priority
     *  may also be boosted, if it owns a PTHREAD_PRIO_INHERIT mutex and a
     *  higher priority thread tries to acquire the mutex.
     *  When the thread releases the mutex, its priority will be set to the
     *  max of the priority ceilings of the mutexes it still owns, and
     *  its original priority.  So we need to keep track of its acquired
     *  mutexes and original priority before acquiring any mutexes.
     */
    ti_sysbios_knl_Queue_Struct      mutexList;

    /* PTHREAD_PRIO_INHERIT mutex the thread is blocked on */
    pthread_mutex_t  *blockedMutex;
#endif
    int               priority;

    ti_sysbios_knl_Task_Handle       task;
    ti_sysbios_knl_Semaphore_Struct  joinSem;

    pthread_t         joinThread;

    int               detached;
    pthread_RunFxn    fxn;
    int               cancelState;
    int               cancelPending;

    /* Thread function return value */
    void             *ret;

    /* Cleanup handlers */
    struct _pthread_cleanup_context *cleanupList;

    /* List of keys that the thread has called pthread_setspecific() on */
    ti_sysbios_knl_Queue_Struct      keyList;
} pthread_Obj;

#define _pthread_getRunningPriority(pthread) \
    (ti_sysbios_knl_Task_getPri(((pthread_Obj *)(pthread))->task))

#define _pthread_getTaskHandle(pthread) \
    (((pthread_Obj *)(pthread))->task)

#define _pthread_setRunningPriority(pthread, pri) \
    (ti_sysbios_knl_Task_setPri(((pthread_Obj *)(pthread))->task, (pri)))

#ifdef ti_posix_tirtos_Settings_enableMutexPriority__D
extern int _pthread_getMaxPrioCeiling(pthread_Obj *thread);
#endif

extern void _pthread_removeThreadKeys(pthread_t pthread);

#ifdef __cplusplus
}
#endif

#endif /* ti_posix_tirtos__pthread__include */
