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
 *  ======== sleep.c ========
 */

#include <errno.h>
#include <unistd.h>

#include <FreeRTOS.h>
#include <task.h>

/* number of microseconds per tick */
#define TICK_PERIOD_USECS (1000000 / configTICK_RATE_HZ)

/*
 *  ======== sleep ========
 */
unsigned sleep(unsigned seconds)
{
    TickType_t xDelay;

    /* maximum sleep is ~50 days at 1000 tics per second */
    xDelay = seconds * configTICK_RATE_HZ;

    vTaskDelay(xDelay);

    return (0);
}

/*
 *  ======== usleep ========
 */
int usleep(useconds_t usec)
{
    TickType_t xDelay;

    /* usec must be less than 1000000 */
    if (usec >= 1000000) {
        errno = EINVAL;
        return (-1);
    }

    /* take the ceiling */
    xDelay = (usec + TICK_PERIOD_USECS - 1) / TICK_PERIOD_USECS;

    vTaskDelay(xDelay);

    return (0);
}
