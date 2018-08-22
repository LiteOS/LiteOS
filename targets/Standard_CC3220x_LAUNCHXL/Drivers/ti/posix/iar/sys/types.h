/*
 * Copyright (c) 2017-2018 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== sys/types.h ========
 */

#ifndef ti_posix_iar_sys_types__include
#define ti_posix_iar_sys_types__include

/* compiler vendor check */
#ifndef __IAR_SYSTEMS_ICC__
#error Incompatible compiler: use this include path (.../ti/posix/iar) only with an IAR compiler. You appear to be using a different compiler.
#endif

#include <stddef.h>
#include <stdint.h>
#include "_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

/*  IAR compiler does not define type ssize_t. However, the compiler does
 *  define size_t as __SIZE_T_TYPE__ for all ISAs. We define ssize_t by
 *  changing 'unsigned' to 'signed' for this one definition to ensure both
 *  types have the same bit-width. Set declaration flag using same name
 *  as GNU compiler, which defines both types.
 */
#ifdef __SIZE_T_TYPE__
#define unsigned signed
typedef __SIZE_T_TYPE__ ssize_t;
#undef unsigned
#define _SSIZE_T_DECLARED
#else
#error __SIZE_T_TYPE__ not defined
#endif

typedef uint32_t clockid_t;
typedef unsigned long useconds_t;
typedef unsigned long timer_t;
typedef long suseconds_t;
typedef unsigned short uid_t;

/*  IAR compiler defines time_t in time[32,64].h (should be in sys/types.h).
 *  Pull in time.h (which includes time[32,64].h) to get time_t definition.
 */
#if defined(__430_CORE__) || defined(__430X_CORE__)
#include <../inc/dlib/c/time.h>
#else
#include <../inc/c/time.h>
#endif


/*
 *************************************************************************
 *                      posix types
 *************************************************************************
 */

/*
 *  ======== pthread_attr_t ========
 */
typedef struct pthread_attr_t {
    int priority;
    void *stack;
    size_t stacksize;
    size_t guardsize;
    int  detachstate;
} pthread_attr_t;

typedef uint32_t pthread_barrierattr_t;
typedef uint32_t pthread_condattr_t;

typedef void *pthread_key_t;

typedef struct pthread_mutexattr_t {
    int type;
    int protocol;
    int prioceiling;
} pthread_mutexattr_t;

typedef uint32_t pthread_rwlockattr_t;

typedef void *pthread_t;

typedef union {
    struct sysbios_Barrier sysbios;
    struct freertos_Barrier freertos;
} pthread_barrier_t;

typedef union {
    struct sysbios_Cond sysbios;
    struct freertos_Cond freertos;
} pthread_cond_t;

typedef union {
    struct sysbios_Mutex sysbios;
    struct freertos_Mutex freertos;
} pthread_mutex_t;

typedef uint32_t pthread_once_t;

typedef union {
    struct sysbios_RWLock sysbios;
    struct freertos_RWLock freertos;
} pthread_rwlock_t;

struct _pthread_cleanup_context {
    pthread_t  thread;
    void       (*fxn)(void *);
    void      *arg;
    int        cancelType;
    struct _pthread_cleanup_context *next;
};

#ifdef __cplusplus
}
#endif

#endif /* ti_posix_iar_sys_types__include */
