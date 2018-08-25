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
 *  ======== sched.h ========
 */

#ifndef ti_posix_iar_sched__include
#define ti_posix_iar_sched__include

/* compiler vendor check */
#ifndef __IAR_SYSTEMS_ICC__
#error Incompatible compiler: use this include path (.../ti/posix/iar) only with an IAR compiler. You appear to be using a different compiler.
#endif

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  These defines would be in a sched.h, which TI and IAR
 *  toolchains don't have.
 */
#ifndef SCHED_FIFO
#define SCHED_FIFO 0
#endif

#ifndef SCHED_RR
#define SCHED_RR 0
#endif

#ifndef SCHED_OTHER
#define SCHED_OTHER 0
#endif

/*
 *  ======== sched_param ========
 *  This was taken from sys/sched.h
 */
struct sched_param {
  int sched_priority; /* Thread execution priority */
};

/*
 *  GNU sched.h declares sched_get_priority_min(),
 *  sched_get_priority_max(), and sched_yield() inside an
 *      #if defined(_POSIX_PRIORITY_SCHEDULING)
 *  block.
 */
extern int sched_get_priority_min(int policy);
extern int sched_get_priority_max(int policy);

extern int sched_yield(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_posix_iar_sched__include */
