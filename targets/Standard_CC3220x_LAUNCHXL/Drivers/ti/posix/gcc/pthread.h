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
 *  ======== pthread.h ========
 */

#ifndef ti_posix_gcc_pthread__include
#define ti_posix_gcc_pthread__include

/* compiler vendor check */
#ifndef __GNUC__
#error Incompatible compiler: use this include path (.../ti/posix/gcc) only with a GNU compiler. You appear to be using a different compiler.
#endif

#include <stdint.h>

#include "sys/types.h"
#include "time.h"
#include "sched.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PTHREAD_BARRIER_SERIAL_THREAD -1

#define PTHREAD_CREATE_JOINABLE     0
#define PTHREAD_CREATE_DETACHED     1

/* PThread cancellation */
#define PTHREAD_CANCEL_ENABLE       0
#define PTHREAD_CANCEL_DISABLE      1
#define PTHREAD_CANCELED            ((void *) -1)

/*
 *  Mutex attributes - type
 *
 *  PTHREAD_MUTEX_NORMAL: Owner of mutex cannot relock it. Attempting
 *      to relock will cause deadlock.
 *  PTHREAD_MUTEX_RECURSIVE: Owner can relock the mutex.
 *  PTHREAD_MUTEX_ERRORCHECK: If owner attempts to relock the mutex, an
 *      error is returned.
 *
 */
#define PTHREAD_MUTEX_NORMAL        0
#define PTHREAD_MUTEX_RECURSIVE     1
#define PTHREAD_MUTEX_ERRORCHECK    2
#define PTHREAD_MUTEX_DEFAULT       PTHREAD_MUTEX_NORMAL

/* Passed to pthread_once() */
#define PTHREAD_ONCE_INIT 0

/*
 *  Mutex attributes - protocol
 *
 *  PTHREAD_PRIO_NONE: Ownership of mutex does not affect priority.
 *  PTHREAD_PRIO_INHERIT: Owner's priority is boosted to the priority of
 *      highest priority thread blocked on the mutex.
 *  PTHREAD_PRIO_PROTECT:  Mutex has a priority ceiling.  The owner's
 *      priority is boosted to the highest priority ceiling of all mutexes
 *      owned (regardless of whether or not other threads are blocked on
 *      any of these mutexes).
 */
#define PTHREAD_PRIO_NONE           0
#define PTHREAD_PRIO_INHERIT        1
#define PTHREAD_PRIO_PROTECT        2

#define PTHREAD_PROCESS_PRIVATE     0

/*
 *************************************************************************
 *                      pthread_attr
 *************************************************************************
 */
extern int pthread_attr_destroy(pthread_attr_t *attr);

extern int pthread_attr_getdetachstate(const pthread_attr_t *attr,
        int *detachstate);
extern int pthread_attr_getguardsize(const pthread_attr_t *attr,
        size_t *guardsize);

extern int pthread_attr_getschedparam(const pthread_attr_t *attr,
        struct sched_param *schedparam);

extern int pthread_attr_getstack(const pthread_attr_t *attr,
        void **stackaddr, size_t *stacksize);
extern int pthread_attr_getstacksize(const pthread_attr_t *attr,
        size_t *stacksize);

extern int pthread_attr_init(pthread_attr_t *attr);

extern int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachedstate);
extern int pthread_attr_setguardsize(pthread_attr_t *attr, size_t guardsize);

extern int pthread_attr_setschedparam(pthread_attr_t *attr,
        const struct sched_param *schedparam);

extern int pthread_attr_setstack(pthread_attr_t *attr, void *stackaddr,
        size_t stacksize);
extern int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);

/*
 *************************************************************************
 *                      pthread
 *************************************************************************
 */
extern int pthread_cancel(pthread_t pthread);
extern void _pthread_cleanup_pop(struct _pthread_cleanup_context *context,
                int execute);
extern void _pthread_cleanup_push(struct _pthread_cleanup_context *context,
        void (*fxn)(void *), void *arg);

#define pthread_cleanup_push(fxn, arg) \
    do { \
        struct _pthread_cleanup_context _pthread_clup_ctx; \
        _pthread_cleanup_push(&_pthread_clup_ctx, (fxn), (arg))

#define pthread_cleanup_pop(execute) \
        _pthread_cleanup_pop(&_pthread_clup_ctx, (execute)); \
    } while (0)

extern int pthread_create(pthread_t *newthread, const pthread_attr_t *attr,
            void *(*startroutine)(void *), void *arg);
extern int pthread_detach(pthread_t pthread);
extern int pthread_equal(pthread_t pt1, pthread_t pt2);
extern void pthread_exit(void *ptr);
extern int pthread_getschedparam(pthread_t thread, int *policy,
        struct sched_param *param);
extern int pthread_join(pthread_t th, void **thread_return);
extern int pthread_once(pthread_once_t *once, void (*initFxn)(void));
extern pthread_t pthread_self(void);
extern int pthread_setcancelstate(int state, int *oldstate);
extern int pthread_setschedparam(pthread_t pthread, int policy,
        const struct sched_param *param);

/*
 *************************************************************************
 *                      pthread_barrierattr
 *************************************************************************
 */
extern int pthread_barrierattr_destroy(pthread_barrierattr_t *attr);
extern int pthread_barrierattr_init(pthread_barrierattr_t *attr);

/*
 *************************************************************************
 *                      pthread_barrier
 *************************************************************************
 */
extern int pthread_barrier_destroy(pthread_barrier_t *barrier);
extern int pthread_barrier_init(pthread_barrier_t *barrier,
        const pthread_barrierattr_t *attr, unsigned count);
extern int pthread_barrier_wait(pthread_barrier_t *barrier);

/*
 *************************************************************************
 *                      pthread_condattr
 *************************************************************************
 */
extern int pthread_condattr_destroy(pthread_condattr_t *attr);
extern int pthread_condattr_getclock(const pthread_condattr_t *attr,
        clockid_t *clock_id);
extern int pthread_condattr_init(pthread_condattr_t * attr);
extern int pthread_condattr_setclock(pthread_condattr_t *attr,
        clockid_t clock_id);

/*
 *************************************************************************
 *                      pthread_cond
 *************************************************************************
 */
extern int pthread_cond_broadcast(pthread_cond_t *cond);
extern int pthread_cond_destroy(pthread_cond_t *cond);
extern int pthread_cond_init(pthread_cond_t *cond,
        const pthread_condattr_t *attr);
extern int pthread_cond_signal(pthread_cond_t *cond);
extern int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex,
        const struct timespec *abstime);
extern int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);

/*
 *************************************************************************
 *                      pthread_key
 *************************************************************************
 */
extern int pthread_key_create(pthread_key_t *key, void (*destructor)(void*));
extern int pthread_key_delete(pthread_key_t key);
extern void *pthread_getspecific(pthread_key_t key);
extern int pthread_setspecific(pthread_key_t key, const void *value);

/*
 *************************************************************************
 *                      pthread_mutexattr
 *************************************************************************
 */
extern int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);
extern int pthread_mutexattr_gettype(const pthread_mutexattr_t *attr,
        int *type);
extern int pthread_mutexattr_getprioceiling(const pthread_mutexattr_t *attr,
        int *prioceiling);
extern int pthread_mutexattr_getprotocol(const pthread_mutexattr_t *attr,
        int *protocol);
extern int pthread_mutexattr_init(pthread_mutexattr_t *attr);
extern int pthread_mutexattr_setprioceiling(pthread_mutexattr_t *attr,
        int prioceiling);
extern int pthread_mutexattr_setprotocol(pthread_mutexattr_t *attr,
        int protocol);
extern int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type);

/*
 *************************************************************************
 *                      pthread_mutex
 *************************************************************************
 */
extern int pthread_mutex_destroy(pthread_mutex_t *mutex);
extern int pthread_mutex_getprioceiling(const pthread_mutex_t *mutex,
        int *prioceiling);
extern int pthread_mutex_init(pthread_mutex_t *mutex,
            const pthread_mutexattr_t *attr);
extern int pthread_mutex_lock(pthread_mutex_t *mutex);
extern int pthread_mutex_setprioceiling(pthread_mutex_t *mutex,
        int prioceiling, int *oldceiling);
extern int pthread_mutex_timedlock(pthread_mutex_t *mutex,
            const struct timespec *abstime);
extern int pthread_mutex_trylock(pthread_mutex_t *mutex);

extern int pthread_mutex_unlock(pthread_mutex_t *mutex);

/*
 *************************************************************************
 *                      pthread_rwlockattr
 *************************************************************************
 */
extern int pthread_rwlockattr_destroy(pthread_rwlockattr_t *attr);
extern int pthread_rwlockattr_init(pthread_rwlockattr_t * attr);

/*
 *************************************************************************
 *                      pthread_rwlock
 *************************************************************************
 */
extern int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
extern int pthread_rwlock_init(pthread_rwlock_t *rwlock,
        const pthread_rwlockattr_t *attr);

extern int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
extern int pthread_rwlock_timedrdlock(pthread_rwlock_t *rwlock,
        const struct timespec *abstime);
extern int pthread_rwlock_timedwrlock(pthread_rwlock_t *rwlock,
        const struct timespec *abstime);
extern int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
extern int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
extern int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);
extern int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);

#ifdef __cplusplus
}
#endif

#endif /* ti_posix_gcc_pthread__include */
