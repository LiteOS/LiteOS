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
 *  ======== pthread_util.c ========
 *  Utility functions shared accross files.
 */

#include <stdint.h>

#include <ti/sysbios/knl/Clock.h>

#include <time.h>

#include "pthread_util.h"

/* mqueue.c */
int _pthread_abstime2ticks(clockid_t clockId, const struct timespec *abstime,
        uint32_t *ticks)
{
    struct timespec     curtime;
    uint32_t            timeout;
    long                usecs = 0;
    long                nsecs;
    time_t              secs = 0;

    if ((abstime->tv_nsec < 0) || (1000000000 <= abstime->tv_nsec)) {
        /* EINVAL */
        return (-1);
    }

    clock_gettime(clockId, &curtime);
    secs = abstime->tv_sec - curtime.tv_sec;

    if ((abstime->tv_sec < curtime.tv_sec) ||
            ((secs == 0) && (abstime->tv_nsec <= curtime.tv_nsec))) {
        timeout = 0;
    }
    else {
        nsecs = abstime->tv_nsec - curtime.tv_nsec;
        if (nsecs < 0) {
            nsecs += 1000000000;
            secs--;
        }

        /* Take the ceiling: 1 to 1000 nsecs -> 1 usec */
        usecs = (nsecs + 999) / 1000;
        timeout = secs * (1000000 / Clock_tickPeriod);

        /* Take the ceiling: 1 to Clock_tickPeriod usecs -> 1 tick */
        timeout += (usecs + Clock_tickPeriod - 1) / Clock_tickPeriod;

        /*
         *  Add one tick to ensure the timeout is not less than the
         *  amount of time requested.  The Clock may be about to tick,
         *  and that counts as one tick even though the amount of time
         *  until this tick is less than Clock_tickPeriod.
         */
        timeout++;
    }

    *ticks = timeout;
    return (0);
}
