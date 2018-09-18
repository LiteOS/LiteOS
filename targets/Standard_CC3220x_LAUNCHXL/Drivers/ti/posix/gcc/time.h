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
 *  ======== time.h ========
 */

#ifndef ti_posix_gcc_time__include
#define ti_posix_gcc_time__include

/* compiler vendor check */
#ifndef __GNUC__
#error Incompatible compiler: use this include path (.../ti/posix/gcc) only with a GNU compiler. You appear to be using a different compiler.
#endif

#include <stddef.h>
#include <stdint.h>

/* include compiler time.h */
#include <../include/time.h>

#include "signal.h"
#include "sys/types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  Timer support.
 */
/*
 *  CLOCK_REALTIME represents the realtime clock for the system.  For this
 *  clock, clock_gettime() returns the time since the beginning of the
 *  Epoch.  SYS/BIOS implements clock_gettime() and clock_settime() with
 *  the ti.sysbios.hal.Seconds module for CLOCK_REALTIME.
 */
#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME (clockid_t)1
#endif

/*
 *  CLOCK_MONOTONIC represents a system clock that cannot be set via
 *  clock_settime().  For SYS/BIOS, CLOCK_MONOTONIC represents the
 *  ti.sysbios.knl.Clock system clock.
 */
#ifndef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC (clockid_t)2
#endif

#ifndef TIMER_ABSTIME
#define TIMER_ABSTIME 4
#endif

/*
 *  For clockId = CLOCK_REALTIME, clock_gettime() and clock_settime() use
 *  the BIOS Seconds module to get/set the time.  Before using clock_gettime()
 *  with clockId = CLOCK_REALTIME, the Seconds module must be initialized.
 *  This can be done by either calling clock_settime(CLOCK_REALTIME,...),
 *  or Seconds_set().
 *  For clockId = CLOCK_MONOTONIC, clock_gettime() returns a time based on
 *  ti.sysbios.knl.Clock ticks.
 */
extern int clock_gettime(clockid_t clockId, struct timespec *ts);

extern int clock_nanosleep(clockid_t clock_id, int flags,
        const struct timespec *rqtp, struct timespec *rmtp);

/*
 *  Only clockId = CLOCK_REALTIME is supported for clock_settime(). Only
 *  the value of ts->tv_sec is used is used in clock_settime().
 *  Returns EINVAL if clockId = CLOCK_MONOTONIC.
 */
extern int clock_settime(clockid_t clockId, const struct timespec *ts);

/*
 *  Create a timer based on the BIOS Clock module.  To reduce code size,
 *  the clockId parameter is ignored.
 */
extern int timer_create(clockid_t clockId, struct sigevent *evp,
       timer_t *timerid);
extern int timer_delete(timer_t timerid);
extern int timer_gettime(timer_t timerid, struct itimerspec *its);
extern int timer_settime(timer_t timerid, int flags,
        const struct itimerspec *value, struct itimerspec *ovalue);

#ifdef __cplusplus
}
#endif

#endif /* ti_posix_gcc_time__include */
