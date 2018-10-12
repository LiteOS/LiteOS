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
 *  ======== sleep.c ========
 */

#include <errno.h>
#include <unistd.h>

#include <xdc/std.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>

/*
 *  ======== sleep ========
 */
unsigned sleep(unsigned seconds)
{
    unsigned long timeout;

    timeout = ((unsigned long)seconds * 1000000L) / Clock_tickPeriod;

    Task_sleep((UInt32)timeout);

    return (0);
}

/*
 *  ======== usleep ========
 */
int usleep(useconds_t usec)
{
    UInt32 timeout;

    /* usec must be less than 1000000 */
    if (usec >= 1000000) {
        errno = EINVAL;
        return (-1);
    }

    /*  Implementations may place limitations on the granularity of timer
     *  values. For each interval timer, if the requested timer value requires
     *  a finer granularity than the implementation supports, the actual timer
     *  value shall be rounded up to the next supported value.
     */
    /* Clock_tickPeriod is the Clock period in microseconds */
    timeout = (UInt32)((usec + Clock_tickPeriod - 1) / Clock_tickPeriod);

    Task_sleep(timeout);

    return (0);
}
