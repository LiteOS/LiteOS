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

#ifndef ti_posix_gcc_sys_types__include
#define ti_posix_gcc_sys_types__include

/* compiler vendor check */
#ifndef __GNUC__
#error Incompatible compiler: use this include path (.../ti/posix/gcc) only with a GNU compiler. You appear to be using a different compiler.
#endif

#include <stddef.h>
#include <stdint.h>
#include "_internal.h"

/* include compiler sys/types.h */
#include <../include/sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/*  In order to use POSIX types defined by TI-POSIX, instead of those
 *  defined by GCC, the user must define -std=c99 (or c++98), which will
 *  omit the GCC defines and avoid type collisions with TI-POSIX. This
 *  is new with GCC v7. In GCC v6, the compiler omitted the POSIX types
 *  by default.
 *
 *  In addition, both _POSIX_SOURCE and _POSIX_C_SOURCE must be undefined.
 *  Previous versions of SDK examples were defining these macros. If you
 *  are migrating an old example, make sure to remove these macros.
 *
 *  This check is to inform the user of these requirements.
 */
#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE)
#ifdef __cplusplus
#error "When compiling with TI-POSIX, you must define -std=c++98 (or later). You must not define _POSIX_SOURCE or _POSIX_C_SOURCE."
#else
#error "When compiling with TI-POSIX, you must define -std=c99 (or later). You must not define _POSIX_SOURCE or _POSIX_C_SOURCE."
#endif
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

#endif /* ti_posix_gcc_sys_types__include */
