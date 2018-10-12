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
 *  ======== semaphore.h ========
 */

#ifndef ti_posix_gcc_semaphore__include
#define ti_posix_gcc_semaphore__include

/* compiler vendor check */
#ifndef __GNUC__
#error Incompatible compiler: use this include path (.../ti/posix/gcc) only with a GNU compiler. You appear to be using a different compiler.
#endif

#include <stdint.h>
#include "sys/_internal.h"

/*
 *  Include definitions of timespec and clockid_t that would
 *  be in sys/types.h.  TI and IAR tool chains do not have a
 *  sys/types.h header file, while GNU toolchain does.  For
 *  GNU, sys/types.h is included in time.h.
 */
#include "time.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef union {
    struct sysbios_Semaphore sysbios;
    struct freertos_Semaphore freertos;
} sem_t;

int    sem_destroy(sem_t *sem);
int    sem_getvalue(sem_t *sem, int *value);
int    sem_init(sem_t *sem, int pshared, unsigned value);
int    sem_post(sem_t *sem);
int    sem_timedwait(sem_t *sem, const struct timespec *abstime);
int    sem_trywait(sem_t *sem);
int    sem_wait(sem_t *sem);

#ifdef __cplusplus
}
#endif

#endif /* ti_posix_gcc_semaphore__include */
