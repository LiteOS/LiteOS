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
 *  ======== timer.c ========
 */

#include <stdint.h>
#include <stdbool.h>

/* FreeRTOS header file */
#include <FreeRTOS.h>
#include <timers.h>
#include <task.h>
#include <projdefs.h>
#include <portmacro.h>

#include <ti/drivers/dpl/HwiP.h>

#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>

/*
 *  ======== TimerObj ========
 */
typedef struct TimerObj {
    void               (*sigev_notify_function)(union sigval val);
    union sigval       val;
    struct timespec    interval;   /* Reload value */
    TickType_t         timeout;    /* Expiration time (absolute) in ticks */
    TickType_t         reload;     /* Reload value in ticks */
    TimerHandle_t      xTimer;
    bool               isActive;   /* TRUE if timer is armed */
    clockid_t          clockId;
} TimerObj;


extern int _clock_abstime2ticks(clockid_t clockId,
        const struct timespec *abstime, TickType_t *ticks);

static void callbackFxn(TimerHandle_t xTimer);
static TickType_t timespecToTicks(const struct timespec *ts);

/*
 *  ======== timer_create ========
 */
int timer_create(clockid_t clockId, struct sigevent *evp, timer_t *timerid)
{
    TimerObj *timer;

    if ((clockId != CLOCK_MONOTONIC) && (clockId != CLOCK_REALTIME)) {
        errno = EINVAL;
        return (-1);
    }

    /*
     *  evp and evp->sigev_notify_function cannot be null.
     *  FreeRTOS uses a thread for timer services, so we will
     *  ignore the pthread_attr_t field of evp.
     */

    *timerid = (timer_t)NULL;

    timer = pvPortMalloc(sizeof(TimerObj));
    if (timer == NULL) {
        errno = ENOMEM;
        return (-1);
    }

    timer->sigev_notify_function = evp->sigev_notify_function;
    timer->val = evp->sigev_value;  /* Passed to user's callback function */
    timer->interval.tv_sec = timer->interval.tv_nsec = 0;
    timer->timeout = timer->reload = 0;
    timer->isActive = false;
    timer->clockId = clockId;

    /*
     *  Create timer with autoReload = pdTRUE, period = -1, since for posix
     *  these values are specified in args to timer_settime().
     *  FreeRTOS timers are created in the dormant state and are activated
     *  with xTimerStart().
     */
    timer->xTimer = xTimerCreate(NULL /* name */,
            (TickType_t)-1,           /* Timer period (ticks) */
            pdTRUE,                   /* autoReload*/
            (void *)timer,            /* pvTimerID */
            callbackFxn);

    if (timer->xTimer == NULL) {
        vPortFree(timer);
        errno = ENOMEM;
        return (-1);
    }

    *timerid = (timer_t)timer;

    return (0);
}

/*
 *  ======== timer_delete ========
 */
int timer_delete(timer_t timerid)
{
    TimerObj *timer = (TimerObj *)timerid;

    if (timer->xTimer) {
        /*
         *  Block until delete command has been sent to timer command
         *  queue.
         */
        xTimerDelete(timer->xTimer, (TickType_t)(-1));
    }

    vPortFree((void *)timer);

    return (0);
}

/*
 *  ======== timer_gettime ========
 */
int timer_gettime(timer_t timerid, struct itimerspec *its)
{
    TimerObj *timer = (TimerObj *)timerid;
    TickType_t xTicksRemaining, xLeftOverTicks;
    uint64_t   nsecs, secs;

    if (timer->isActive) {
        /*
         *  Calculate the time that remains before the timer expires.
         *  TickType_t is unsigned, so subtraction results in the
         *  correct answer even if the timer will not expire until
         *  after the tick count has wrapped.
         */
        xTicksRemaining = xTimerGetExpiryTime(timer->xTimer) -
                xTaskGetTickCount();
        secs = xTicksRemaining / (TickType_t)configTICK_RATE_HZ;

        /* Left over ticks when we subtract off the seconds part */
        xLeftOverTicks = xTicksRemaining -
                (secs * (TickType_t)configTICK_RATE_HZ);
        nsecs = xLeftOverTicks * (1000000000 / configTICK_RATE_HZ);

        /*
         *  Store the amount of time remaining until the timer expires
         *  (regardless of whether or not the timer was armed with an
         *  absolute time).
         */
        its->it_value.tv_sec = (time_t)secs;
        its->it_value.tv_nsec = nsecs;
    }
    else {
        /* Timer is disarmed, store 0. */
        its->it_value.tv_sec = its->it_value.tv_nsec = 0;
    }

    /* The interval last set by timer_settime()  */
    its->it_interval = timer->interval;

    return (0);
}

/*
 *  ======== timer_settime ========
 *  This is a blocking call.
 */
int timer_settime(timer_t timerid, int flags,
        const struct itimerspec *value, struct itimerspec *ovalue)
{
    TimerObj  *timer = (TimerObj *)timerid;
    TickType_t timeoutTicks;
    BaseType_t xHigherPriorityTaskWoken;
    BaseType_t status;

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
     *  value->it_value = 0  ==> disarm the timer
     *  otherwise arm the timer with value->it_value
     *
     *  value->it_interval is the reload value (0 ==> one-shot,
     *  non-zero ==> periodic)
     */

    /* Stop the timer if the value is 0 */
    if ((value->it_value.tv_sec == 0) && (value->it_value.tv_nsec == 0)) {
        if (HwiP_inISR()) {
            status = xTimerStopFromISR(timer->xTimer,
                    &xHigherPriorityTaskWoken);
        }
        else {
            /* Block until stop command is sent to timer command queue */
            status = xTimerStop(timer->xTimer, (TickType_t)-1);
        }
        if (status == pdPASS) {
            timer->isActive = false;
            return (0);
        }
        else  if (status == errQUEUE_FULL) {
            errno = EAGAIN; /* timer queue is full, try again */
            return (-1);
        }
        else {
            errno = ENOMEM; /* timer initialization failed */
            return (-1);
        }
    }

    /*
     *  If the timer is already armed, we need to change the expiration
     *  to the new value.  FreeRTOS timers only support period, and not
     *  a timeout, so if it_interval is non-zero, we'll ignore the it_value.
     */

    if ((value->it_interval.tv_sec != 0) || (value->it_interval.tv_nsec != 0)) {
        /* Non-zero reload value, so change period */
        timeoutTicks = timespecToTicks(&(value->it_interval));
        /*
         *  Change the timer period.  FreeRTOS timers only have a
         *  period, so we'll ignore value->it_value.
         *  xTimerChangePeriod() can be called on an active or dormant
         *  timer, but does not start a dormant timer.
         *  When xTimerStart() is called on an active timer, the timer
         *  will be restarted with the new period.
         */
        timer->reload = timeoutTicks;

        /* Save the new interval for timer_gettime() */
        timer->interval.tv_sec = value->it_interval.tv_sec;
        timer->interval.tv_nsec = value->it_interval.tv_nsec;
    }
    else {
        if (flags & TIMER_ABSTIME) {
            _clock_abstime2ticks(timer->clockId, &(value->it_value),
                    &timeoutTicks);

            if (timeoutTicks <= 0) {
                /* Timeout has already expired */
                (timer->sigev_notify_function)(timer->val);
                return (0);
            }
        }
        else {
            timeoutTicks = timespecToTicks(&(value->it_value));
        }
    }

    if (HwiP_inISR()) {
        status = xTimerChangePeriodFromISR(timer->xTimer, timeoutTicks,
                &xHigherPriorityTaskWoken);
        if (status == pdPASS) {
            xTimerStartFromISR(timer->xTimer, &xHigherPriorityTaskWoken);
        }
    }
    else {
        status = xTimerChangePeriod(timer->xTimer, timeoutTicks,
                (TickType_t)-1);
        if (status == pdPASS) {
            status = xTimerStart(timer->xTimer, (TickType_t)-1);
        }
    }

    if (status == pdPASS) {
        timer->isActive = true;
        return (0);
    }
    else  if (status == errQUEUE_FULL) {
        errno = EAGAIN; /* timer queue is full, try again */
        return (-1);
    }
    else {
        errno = ENOMEM; /* timer initialization failed */
        return (-1);
    }
}

/*
 *  ======== callbackFxn ========
 */
static void callbackFxn(TimerHandle_t xTimer)
{
    TimerObj *timer;

    timer = (TimerObj *)pvTimerGetTimerID(xTimer);

    if (timer->reload == 0) {
        xTimerStop(timer->xTimer, (TickType_t)-1);
        timer->isActive = false;
    }

    (timer->sigev_notify_function)(timer->val);
}

/*
 *  ======== timespecToTicks ========
 */
static TickType_t timespecToTicks(const struct timespec *ts)
{
    uint64_t ticks;

    ticks = ts->tv_sec * configTICK_RATE_HZ +
            ts->tv_nsec * (uint64_t)configTICK_RATE_HZ / 1000000000L;
    return ((TickType_t)ticks);
}
