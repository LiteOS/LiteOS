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
 *  ======== signal.h ========
 */

#ifndef ti_posix_gcc_signal__include
#define ti_posix_gcc_signal__include

/* compiler vendor check */
#ifndef __GNUC__
#error Incompatible compiler: use this include path (.../ti/posix/gcc) only with a GNU compiler. You appear to be using a different compiler.
#endif

#include "sys/types.h"

/* include toolchain's header file */
#include <../include/signal.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SIGEV_NONE
#define SIGEV_NONE 1
#endif

#ifndef SIGEV_SIGNAL
#define SIGEV_SIGNAL 2
#endif

#ifndef SIGEV_THREAD
#define SIGEV_THREAD 3
#endif


/*
 *************************************************************************
 *                      signal types
 *************************************************************************
 */

/*
 *  ======== sigval ========
 */
union sigval {
    int     sival_int;      /* integer signal value */
    void   *sival_ptr;      /* pointer signal value */
};

/*  Deprecated. This typedef is for compatibility with old SDKs. It is
 *  not part of the POSIX standard. It will be removed in a future
 *  release. TIRTOS-1317
 */
typedef union sigval sigval;

/*
 *  ======== sigevent ========
 */
struct sigevent {
    int             sigev_notify;       /* notification type */
    int             sigev_signo;        /* signal number */
    union sigval    sigev_value;        /* signal value */

    void (*sigev_notify_function)(union sigval val);    /* notify function */
    pthread_attr_t *sigev_notify_attributes;            /* notify attributes */
};

/*  Deprecated. This typedef is for compatibility with old SDKs. It is
 *  not part of the POSIX standard. It will be removed in a future
 *  release. TIRTOS-1317
 */
typedef struct sigevent sigevent;


#ifdef __cplusplus
}
#endif

#endif /* ti_posix_gcc_signal__include */
