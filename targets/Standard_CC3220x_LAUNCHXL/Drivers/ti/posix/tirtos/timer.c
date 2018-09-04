/*
 * Copyright (c) 2015-2018 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== timer.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>

#include <pthread.h>
#include <time.h>
#include <errno.h>

#include "_pthread.h"

static Void timerFxn(UArg arg);
static Void *timerThreadFxn(Void *arg);


/*
 *  ======== TimerObj ========
 */
typedef struct TimerObj {
    Clock_Struct      clock;
    void             (*sigev_notify_function)(union sigval val);
    union sigval     val;
    pthread_t        thread;
    Semaphore_Handle sem;
    Int              notifyType;    /* e.g., SIGEV_SIGNAL */
    clockid_t        clockId;
} TimerObj;

/*
 *  ======== timer_create ========
 *  sigevent {
 *      int sigev_notify;
 *          SIGEV_NONE - No notification when the event occurs
 *          SIGEV_SIGNAL - A queued signal will be generated.
 *          SIGEV_THREAD - A notification function will be called.
 *      int sigev_signo;  Signal number (don't care about)
 *      sigval sigev_value;
 *          Passed to notification function
 *      void (*sigev_notify_function)(sigval val);
 *      pthread_attr_t *sigev_notify_attributes;  - Don't care about.
 *
 */
int timer_create(clockid_t clockid, struct sigevent *evp, timer_t *timerid)
{
    Semaphore_Params   semParams;
    Clock_Params       params;
    Error_Block        eb;
    TimerObj          *timer;
    pthread_attr_t     pDefAttrs;
    pthread_attr_t    *pAttrs;
    Int                retc;

    Assert_isTrue(evp != NULL, 0);
    Assert_isTrue(evp->sigev_notify_function != NULL, 0);
    Assert_isTrue((evp->sigev_notify == SIGEV_THREAD) ||
            (evp->sigev_notify == SIGEV_SIGNAL), 0);

    if ((clockid != CLOCK_MONOTONIC) && (clockid != CLOCK_REALTIME)) {
        errno = EINVAL;
        return (-1);
    }

    pthread_attr_init(&pDefAttrs);

    pAttrs = (evp->sigev_notify_attributes != NULL) ?
        evp->sigev_notify_attributes : &pDefAttrs;

    Error_init(&eb);

    *timerid = (timer_t)NULL;

    timer = (TimerObj *)Memory_alloc(Task_Object_heap(),
            sizeof(TimerObj), 0, &eb);

    if (timer == NULL) {
        errno = ENOMEM;
        return (-1);
    }

    Clock_Params_init(&params);

    params.arg = (UArg)timer;
    Clock_construct(&(timer->clock), timerFxn, 0, &params);

    timer->sigev_notify_function = evp->sigev_notify_function;
    timer->val = evp->sigev_value;
    timer->notifyType = evp->sigev_notify;
    timer->thread = NULL;
    timer->sem = NULL;
    timer->clockId = clockid;

    if (evp->sigev_notify == SIGEV_THREAD) {
        /* Create semaphore that will be posted when timer expires */
        Semaphore_Params_init(&semParams);
        semParams.mode = Semaphore_Mode_BINARY;
        timer->sem = Semaphore_create(0, &semParams, NULL);

        if (timer->sem == NULL) {
            timer_delete((timer_t)timer);
            errno = ENOMEM;
            return (-1);
        }

        /* Timer notification threads must be detached */
        pthread_attr_setdetachstate(pAttrs, PTHREAD_CREATE_DETACHED);

        retc = pthread_create(&(timer->thread), pAttrs, timerThreadFxn,
                (void *)timer);

        if (retc != 0) {
            timer_delete((timer_t)timer);
            errno = ENOMEM;
            return (-1);
        }

        /* help ROV display the correct thread function */
        Task_setArg0(((pthread_Obj *)(timer->thread))->task,
                (UArg)timer->sigev_notify_function);
    }

    *timerid = (timer_t)timer;
    return (0);
}

/*
 *  ======== timer_delete ========
 */
int timer_delete(timer_t timerid)
{
    TimerObj    *timer = (TimerObj *)timerid;

    Clock_destruct(&(timer->clock));

    if (timer->thread) {
        pthread_cancel(timer->thread);
    }

    if (timer->sem) {
        Semaphore_delete(&(timer->sem));
    }

    Memory_free(Task_Object_heap(), timer, sizeof(TimerObj));

    return (0);
}

/*
 *  ======== timer_gettime ========
 *  Returns the time until timer expires (if armed), and the
 *  timer period.
 */
int timer_gettime(timer_t timerid, struct itimerspec *its)
{
    TimerObj    *timer = (TimerObj *)timerid;
    UInt32       timeout;
    long         timeoutUs;
    long         periodUs;

    /*
     *  Clock_getTimeout() returns number of clock ticks until timer expires.
     */
    timeout = Clock_getTimeout(Clock_handle(&(timer->clock)));

    timeoutUs = timeout * Clock_tickPeriod;
    periodUs = Clock_getPeriod(Clock_handle(&timer->clock)) * Clock_tickPeriod;

    /* Store the time until timer expires */
    its->it_value.tv_sec = timeoutUs / 1000000;
    its->it_value.tv_nsec = (timeoutUs - (timeoutUs / 1000000) * 1000000) *
            1000;

    /* Store the timer period */
    its->it_interval.tv_sec = periodUs / 1000000;
    its->it_interval.tv_nsec = (periodUs - (periodUs / 1000000) * 1000000) *
            1000;

    return (0);
}

/*
 *  ======== timer_settime ========
 */
int timer_settime(timer_t timerid, int flags,
        const struct itimerspec *value, struct itimerspec *ovalue)
{
    TimerObj       *timer = (TimerObj *)timerid;
    struct timespec reltime;
    struct timespec curtime;
    long            timeoutUs;
    long            periodUs;
    UInt32          timeout;
    UInt32          period;

    if ((value->it_interval.tv_nsec < 0) ||
            (value->it_interval.tv_nsec >= 1000000000)) {
        errno = EINVAL;
        return (-1);
    }

    if ((value->it_value.tv_nsec < 0) ||
            (value->it_value.tv_nsec >= 1000000000)) {
        errno = EINVAL;
        return (-1);
    }

    /*
     *  If ovalue is non-NULL, save the time before the timer
     *  would have expired, and the timer's old reload value.
     */
    if (ovalue) {
        timer_gettime(timerid, ovalue);
    }

    /*
     *  Set the time until next expiration if value->it_value is
     *  non-zero.  If the timer was already armed, this will reset
     *  it.  If value->it_value is 0, the timer will be disarmed.
     *
     *  If the TIMER_ABSTIME bit is set in flags, value->it_value will
     *  be treated as absolute time, and we need to use the difference
     *  between value->it_value and the current time as the timeout.
     *  If the expiration time has already passed, call the notification
     *  function and return success.
     */
    if ((value->it_value.tv_sec == 0) && (value->it_value.tv_nsec == 0)) {
        Clock_stop(Clock_handle(&(timer->clock)));
        return (0);
    }

    if (flags & TIMER_ABSTIME) {
        clock_gettime(timer->clockId, &curtime);

        if ((curtime.tv_sec > value->it_value.tv_sec) ||
                ((curtime.tv_sec == value->it_value.tv_sec) &&
                        (curtime.tv_nsec >= value->it_value.tv_nsec))) {
            /* The time has already passed */
            (timer->sigev_notify_function)(timer->val);
            return (0);
        }

        reltime.tv_sec = value->it_value.tv_sec - curtime.tv_sec;
        reltime.tv_nsec = value->it_value.tv_nsec - curtime.tv_nsec;

        if (reltime.tv_nsec < 0) {
            reltime.tv_nsec += 1000000000;
            reltime.tv_sec--;
        }
    }
    else {
        reltime.tv_sec = value->it_value.tv_sec;
        reltime.tv_nsec = value->it_value.tv_nsec;
    }

    timeoutUs = reltime.tv_nsec / 1000 + reltime.tv_sec * 1000000;
    periodUs = value->it_interval.tv_sec * 1000000 +
        value->it_interval.tv_nsec / 1000;

    /*
     *  Calculate period and timeout in Clock ticks.  Take the
     *  ceiling if the value in microseconds is not a multiple of
     *  the Clock tick period.
     */
    period = (periodUs + Clock_tickPeriod - 1) / Clock_tickPeriod;
    timeout = (timeoutUs + Clock_tickPeriod - 1) / Clock_tickPeriod;

    Clock_setPeriod((Clock_handle)(&(timer->clock)), period);
    Clock_setTimeout((Clock_handle)(&(timer->clock)), timeout);
    Clock_start((Clock_handle)(&(timer->clock)));

    return (0);
}

/*
 *  ======== timerFxn ========
 */
static Void timerFxn(UArg arg)
{
    TimerObj *timer = (TimerObj *)arg;

    if (timer->thread) {
        Semaphore_post(timer->sem);
    }
    else {
        (timer->sigev_notify_function)(timer->val);
    }
}

/*
 *  ======== timerThreadFxn ========
 */
static Void *timerThreadFxn(Void *arg)
{
    TimerObj *timer = (TimerObj *)arg;

    for (;;) {
        Semaphore_pend(timer->sem, BIOS_WAIT_FOREVER);
        (timer->sigev_notify_function)(timer->val);
    }
}
