/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Time file
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --------------------------------------------------------------------------- */

#include "time_pri.h"
#include "limits.h"
#include "sys/times.h"
#ifndef LOSCFG_AARCH64
#include "time64.h"
#endif
#include "errno.h"
#include "unistd.h"
#include "stdio.h"
#include "sys/time.h"
#include "stdint.h"
#include "los_sys.h"
#include "los_tick_pri.h"
#include "asm/hal_platform_ints.h"
#include "los_tick.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * Do a time package defined return. This requires the error code
 * to be placed in errno, and if it is non-zero, -1 returned as the
 * result of the function. This also gives us a place to put any
 * generic tidyup handling needed for things like signal delivery and
 * cancellation.
 */
#define TIME_RETURN(err) do { \
    INT32 retVal = 0;         \
    if ((err) != 0) {         \
        retVal = -1;          \
        errno = (err);        \
    }                         \
    return retVal;            \
} while (0)

#ifdef LOSCFG_AARCH64
/*
 * This two structures originally did't exit,
 * they added by liteos to support 64bit interfaces on 32bit platform,
 * in 64bit platform, timeval64 define to timeval which is platform adaptive.
 */
#define timeval64 timeval
#define timespec64 timespec
#endif

#define TV_SEC_MAX ((INT_MAX / 1000000L) - 2)
#define TV_SEC_MIN ((INT_MIN / 1000000L) + 2)

STATIC INLINE BOOL ValidTimeval(const struct timeval *tv)
{
    /* Fail a NULL pointer */
    if (tv == NULL) {
        return FALSE;
    }

    /* Fail illegal microseconds values */
    if ((tv->tv_usec < 0) || (tv->tv_usec >= OS_SYS_US_PER_SECOND) || (tv->tv_sec < 0)) {
        return FALSE;
    }

    return TRUE;
}

#ifndef LOSCFG_AARCH64
STATIC INLINE BOOL ValidTimeval64(const struct timeval64 *tv)
{
    /* Fail a NULL pointer */
    if (tv == NULL) {
        return FALSE;
    }

    /* Fail illegal microseconds values */
    if ((tv->tv_usec < 0) || (tv->tv_usec >= OS_SYS_US_PER_SECOND) || (tv->tv_sec < 0)) {
        return FALSE;
    }

    return TRUE;
}
#endif

STATIC SPIN_LOCK_INIT(g_timeSpin);
STATIC long long g_adjTimeLeft; /* absolute value of adjtime */
STATIC INT32 g_adjDirection;    /* 1, speed up; 0, slow down; */

/* Adjust pacement, nanoseconds per SCHED_CLOCK_INTETRVAL_TICKS ticks */
#define TIME_ADJ_PACEMENT (((LOSCFG_BASE_CORE_ADJ_PER_SECOND * SCHED_CLOCK_INTETRVAL_TICKS) / LOSCFG_BASE_CORE_TICK_PER_SECOND) * OS_SYS_NS_PER_US)

/* accumulative time delta from continuous modify, such as adjtime */
STATIC struct timespec64 g_accDeltaFromAdj;
/* accumulative time delta from discontinuous modify, such as settimeofday */
STATIC struct timespec64 g_accDeltaFromSet;

VOID OsAdjTime(VOID)
{
    UINT32 intSave;
    LOS_SpinLockSave(&g_timeSpin, &intSave);
    if (!g_adjTimeLeft) {
        LOS_SpinUnlockRestore(&g_timeSpin, intSave);
        return;
    }

    if (g_adjTimeLeft > TIME_ADJ_PACEMENT) {
        if (g_adjDirection) {
            if ((g_accDeltaFromAdj.tv_nsec + TIME_ADJ_PACEMENT) >= OS_SYS_NS_PER_SECOND) {
                g_accDeltaFromAdj.tv_sec++;
                g_accDeltaFromAdj.tv_nsec  = (g_accDeltaFromAdj.tv_nsec + TIME_ADJ_PACEMENT) % OS_SYS_NS_PER_SECOND;
            } else {
                g_accDeltaFromAdj.tv_nsec  = g_accDeltaFromAdj.tv_nsec + TIME_ADJ_PACEMENT;
            }
        } else {
            if ((g_accDeltaFromAdj.tv_nsec - TIME_ADJ_PACEMENT) < 0) {
                g_accDeltaFromAdj.tv_sec--;
                g_accDeltaFromAdj.tv_nsec  = g_accDeltaFromAdj.tv_nsec - TIME_ADJ_PACEMENT + OS_SYS_NS_PER_SECOND;
            } else {
                g_accDeltaFromAdj.tv_nsec  = g_accDeltaFromAdj.tv_nsec - TIME_ADJ_PACEMENT;
            }
        }

        g_adjTimeLeft -= TIME_ADJ_PACEMENT;
    } else {
        if (g_adjDirection) {
            if ((g_accDeltaFromAdj.tv_nsec + g_adjTimeLeft) >= OS_SYS_NS_PER_SECOND) {
                g_accDeltaFromAdj.tv_sec++;
                g_accDeltaFromAdj.tv_nsec  = (g_accDeltaFromAdj.tv_nsec + g_adjTimeLeft) % OS_SYS_NS_PER_SECOND;
            } else {
                g_accDeltaFromAdj.tv_nsec  = g_accDeltaFromAdj.tv_nsec + g_adjTimeLeft;
            }
        } else {
            if ((g_accDeltaFromAdj.tv_nsec - g_adjTimeLeft) < 0) {
                g_accDeltaFromAdj.tv_sec--;
                g_accDeltaFromAdj.tv_nsec  = g_accDeltaFromAdj.tv_nsec - g_adjTimeLeft + OS_SYS_NS_PER_SECOND;
            } else {
                g_accDeltaFromAdj.tv_nsec  = g_accDeltaFromAdj.tv_nsec - g_adjTimeLeft;
            }
        }

        g_adjTimeLeft = 0;
    }
    LOS_SpinUnlockRestore(&g_timeSpin, intSave);
    return;
}

/*
 * Function: adjtime
 * Description:  correct the time to synchronize the system clock.
 * Input:     delta - The amount of time by which the clock is to be adjusted.
 * Output: oldDelta - the amount of time remaining from any previous adjustment that has not yet been completed.
 * Return: On success, returns 0.  On failure, -1 is returned, and errno is set to indicate the error.
 */
int adjtime(const struct timeval *delta, struct timeval *oldDelta)
{
    UINT32 intSave;
    LOS_SpinLockSave(&g_timeSpin, &intSave);
    /* return the amount of time remaining from any previous adjustment that has not yet been completed. */
    if (oldDelta != NULL) {
        if (g_adjDirection == 1) {
            oldDelta->tv_sec = g_adjTimeLeft / OS_SYS_NS_PER_SECOND;
            oldDelta->tv_usec = (g_adjTimeLeft % OS_SYS_NS_PER_SECOND) / OS_SYS_NS_PER_US;
        } else {
            oldDelta->tv_sec = -(g_adjTimeLeft / OS_SYS_NS_PER_SECOND);
            oldDelta->tv_usec = -((g_adjTimeLeft % OS_SYS_NS_PER_SECOND) / OS_SYS_NS_PER_US);
        }
    }

    if ((delta == NULL) || ((delta->tv_sec == 0) && (delta->tv_usec == 0))) {
        LOS_SpinUnlockRestore(&g_timeSpin, intSave);
        return 0;
    }

    if ((delta->tv_usec > OS_SYS_US_PER_SECOND) || (delta->tv_usec < -OS_SYS_US_PER_SECOND)) {
        LOS_SpinUnlockRestore(&g_timeSpin, intSave);
        TIME_RETURN(EINVAL);
    }

    if ((delta->tv_sec < TV_SEC_MIN) ||
        (delta->tv_sec > TV_SEC_MAX)) {
        LOS_SpinUnlockRestore(&g_timeSpin, intSave);
        TIME_RETURN(EINVAL);
    }

    g_adjTimeLeft = ((INT64)delta->tv_sec * OS_SYS_NS_PER_SECOND) + (delta->tv_usec * OS_SYS_NS_PER_US);
    if (g_adjTimeLeft > 0) {
        g_adjDirection = 1;
    } else {
        g_adjDirection = 0;
        g_adjTimeLeft = -g_adjTimeLeft;
    }

    LOS_SpinUnlockRestore(&g_timeSpin, intSave);
    return 0;
}

STATIC INLINE struct timespec64 OsTimeSpecAdd(const struct timespec64 t1, const struct timespec64 t2)
{
    struct timespec64 ret = {0};

    ret.tv_sec = t1.tv_sec + t2.tv_sec;
    ret.tv_nsec = t1.tv_nsec + t2.tv_nsec;
    if (ret.tv_nsec >= OS_SYS_NS_PER_SECOND) {
        ret.tv_sec += 1;
        ret.tv_nsec -= OS_SYS_NS_PER_SECOND;
    } else if (ret.tv_nsec < 0L) {
        ret.tv_sec -= 1;
        ret.tv_nsec += OS_SYS_NS_PER_SECOND;
    }

    return ret;
}

STATIC INLINE struct timespec64 OsTimeSpecSub(const struct timespec64 t1, const struct timespec64 t2)
{
    struct timespec64 ret = {0};

    ret.tv_sec = t1.tv_sec - t2.tv_sec;
    ret.tv_nsec = t1.tv_nsec - t2.tv_nsec;
    if (ret.tv_nsec < 0) {
        ret.tv_sec -= 1;
        ret.tv_nsec += OS_SYS_NS_PER_SECOND;
    }

    return ret;
}

STATIC INT32 OsSetTimeOfDay(const struct timeval64 *tv, const struct timezone *tz)
{
    UINT64 nowNsec;
    UINT32 intSave;
    struct timespec64 setTime = {0};
    struct timespec64 hwTime = {0};
    struct timespec64 realTime = {0};
    struct timespec64 tmp = {0};

    (VOID)tz;
    nowNsec = LOS_CurrNanosec();
    setTime.tv_sec = tv->tv_sec;
    setTime.tv_nsec = tv->tv_usec * OS_SYS_NS_PER_US;
    hwTime.tv_sec = nowNsec / OS_SYS_NS_PER_SECOND;
    hwTime.tv_nsec = (INT64)(nowNsec -  hwTime.tv_sec * OS_SYS_NS_PER_SECOND);

    LOS_SpinLockSave(&g_timeSpin, &intSave);
    /* stop on-going continuous adjusement */
    if (g_adjTimeLeft) {
        g_adjTimeLeft = 0;
    }
    realTime = OsTimeSpecAdd(hwTime, g_accDeltaFromAdj);
    realTime = OsTimeSpecAdd(realTime, g_accDeltaFromSet);

    tmp = OsTimeSpecSub(setTime, realTime);
    g_accDeltaFromSet = OsTimeSpecAdd(g_accDeltaFromSet, tmp);

    LOS_SpinUnlockRestore(&g_timeSpin, intSave);

    return 0;
}

int settimeofday(const struct timeval *tv, const struct timezone *tz)
{
    struct timeval64 stTimeVal64 = {0};

    if (!ValidTimeval(tv)) {
        TIME_RETURN(EINVAL);
    }

    stTimeVal64.tv_sec = tv->tv_sec;
    stTimeVal64.tv_usec = tv->tv_usec;

    return OsSetTimeOfDay(&stTimeVal64, tz);
}

#ifndef LOSCFG_AARCH64
int settimeofday64(const struct timeval64 *tv, const struct timezone *tz)
{
    if (!ValidTimeval64(tv)) {
        TIME_RETURN(EINVAL);
    }

    return OsSetTimeOfDay(tv, tz);
}
#endif

int setlocalseconds(int seconds)
{
    struct timeval tv = {0};

    tv.tv_sec = seconds;
    tv.tv_usec = 0;

    return settimeofday(&tv, NULL);
}

STATIC INT32 OsGettimeOfDay(struct timeval64 *tv, struct timezone *tz)
{
    UINT64 nowNsec;
    UINT32 intSave;

    (VOID)tz;
    struct timespec64 hwTime = {0};
    struct timespec64 realTime = {0};

    nowNsec = LOS_CurrNanosec();
    hwTime.tv_sec = nowNsec / OS_SYS_NS_PER_SECOND;
    hwTime.tv_nsec = (INT64)(nowNsec - (hwTime.tv_sec * OS_SYS_NS_PER_SECOND));

    LOS_SpinLockSave(&g_timeSpin, &intSave);
    realTime = OsTimeSpecAdd(hwTime, g_accDeltaFromAdj);
    realTime = OsTimeSpecAdd(realTime, g_accDeltaFromSet);
    LOS_SpinUnlockRestore(&g_timeSpin, intSave);

    tv->tv_sec = realTime.tv_sec;
    tv->tv_usec = realTime.tv_nsec / OS_SYS_NS_PER_US;

    if (tv->tv_sec < 0) {
        TIME_RETURN(EINVAL);
    }
    return 0;
}

#ifndef LOSCFG_AARCH64
int gettimeofday64(struct timeval64 *tv, struct timezone *tz)
{
    if (tv == NULL) {
        TIME_RETURN(EINVAL);
    }

    return OsGettimeOfDay(tv, tz);
}
#endif

int gettimeofday(struct timeval *tv, void *tz)
{
    struct timeval64 stTimeVal64 = {0};

    if (tv == NULL) {
        TIME_RETURN(EINVAL);
    }

    if (OsGettimeOfDay(&stTimeVal64, tz) == -1) {
        return -1;
    }

#ifdef LOSCFG_AARCH64
    tv->tv_sec = stTimeVal64.tv_sec;
    tv->tv_usec = stTimeVal64.tv_usec;
#else
    if (stTimeVal64.tv_sec > (long long)LONG_MAX) {
        return -1;
    }
    tv->tv_sec = (time_t)stTimeVal64.tv_sec;
    tv->tv_usec = (suseconds_t)stTimeVal64.tv_usec;
#endif

    return 0;
}

int clock_settime(clockid_t clockId, const struct timespec *tp)
{
    struct timeval tv = {0};

    if (clockId != CLOCK_REALTIME) {
        TIME_RETURN(EINVAL);
    }

    if (!ValidTimespec(tp)) {
        TIME_RETURN(EINVAL);
    }

    tv.tv_sec = tp->tv_sec;
    tv.tv_usec = tp->tv_nsec / OS_SYS_NS_PER_US;
    return settimeofday(&tv, NULL);
}

int clock_gettime(clockid_t clockId, struct timespec *tp)
{
    UINT32 intSave;
    UINT64 nowNsec;
    struct timespec64 tmp = {0};
    struct timespec64 hwTime = {0};

    if ((clockId > MAX_CLOCKS) || (clockId < CLOCK_REALTIME)) {
        goto ERROUT;
    }

    if (tp == NULL) {
        goto ERROUT;
    }

    nowNsec = LOS_CurrNanosec();
    hwTime.tv_sec = (INT64)(nowNsec / OS_SYS_NS_PER_SECOND);
    hwTime.tv_nsec = (INT64)(nowNsec - hwTime.tv_sec * OS_SYS_NS_PER_SECOND);

    switch (clockId) {
        case CLOCK_MONOTONIC_RAW:
            tp->tv_sec = (time_t)hwTime.tv_sec;
            tp->tv_nsec = (long)hwTime.tv_nsec;
            break;
        case CLOCK_MONOTONIC:
            LOS_SpinLockSave(&g_timeSpin, &intSave);
            tmp = OsTimeSpecAdd(hwTime, g_accDeltaFromAdj);
            LOS_SpinUnlockRestore(&g_timeSpin, intSave);
            tp->tv_sec = (time_t)tmp.tv_sec;
            tp->tv_nsec = (long)tmp.tv_nsec;
            break;
        case CLOCK_REALTIME:
            LOS_SpinLockSave(&g_timeSpin, &intSave);
            tmp = OsTimeSpecAdd(hwTime, g_accDeltaFromAdj);
            tmp = OsTimeSpecAdd(tmp, g_accDeltaFromSet);
            LOS_SpinUnlockRestore(&g_timeSpin, intSave);
            tp->tv_sec = (time_t)tmp.tv_sec;
            tp->tv_nsec = (long)tmp.tv_nsec;
            break;
        default:
            goto ERROUT;
    }

    return 0;

ERROUT:
    TIME_RETURN(EINVAL);
}

int clock_getres(clockid_t clockId, struct timespec *tp)
{
    if (clockId != CLOCK_REALTIME) {
        TIME_RETURN(EINVAL);
    }

    if (tp == NULL) {
        TIME_RETURN(EINVAL);
    }

    /* the accessable rtc resolution */
    tp->tv_nsec = OS_SYS_NS_PER_US; /* the precision of clock_gettime is 1us */
    tp->tv_sec = 0;

    TIME_RETURN(0);
}

STATIC INT32 DoSleep(UINT32 mseconds)
{
    UINT32 interval;
    UINT32 ret;

    if (mseconds == 0) {
        interval = 0;
    } else {
        interval = LOS_MS2Tick(mseconds);
        if (interval == 0) {
            interval = 1;
        }
    }

    ret = LOS_TaskDelay(interval);
    if (ret == LOS_OK) {
        return 0;
    }
    return -1;
}

int usleep(unsigned useconds)
{
    UINT32 interval;

    /* the values not less than per Millisecond */
    if (useconds < OS_SYS_MS_PER_SECOND) {
        if (useconds != 0) {
            interval = OS_SYS_MS_PER_SECOND / LOSCFG_BASE_CORE_TICK_PER_SECOND;
        } else {
            interval = 0;
        }
        return DoSleep(interval);
    }

    return DoSleep(useconds / OS_SYS_US_PER_MS);
}

int nanosleep(const struct timespec *rqtp, struct timespec *rmtp)
{
    UINT64 useconds;
    INT32 ret = -1;

    (VOID)rmtp;
    /* expire time */
    if (rqtp == NULL) {
        errno = EINVAL;
        return ret;
    }

    if (!ValidTimespec(rqtp)) {
        errno = EINVAL;
        return ret;
    }

    useconds = (((UINT64)rqtp->tv_sec * OS_SYS_US_PER_SECOND) + (rqtp->tv_nsec / OS_SYS_NS_PER_US));
    if ((useconds == 0) && (rqtp->tv_nsec != 0)) {
        useconds = 1;
    } else if (useconds > UINT32_MAX) {
        useconds = UINT32_MAX;
    }

    ret = usleep((UINT32)useconds);
    return ret;
}

unsigned int sleep(unsigned int seconds)
{
    UINT64 interval;
    UINT32 ret, tickCnt;

    interval = (UINT64)seconds * LOSCFG_BASE_CORE_TICK_PER_SECOND;
    tickCnt = (interval > LOS_WAIT_FOREVER) ? LOS_WAIT_FOREVER : (UINT32)interval;
    ret = LOS_TaskDelay(tickCnt);
    if (ret == LOS_OK) {
        return 0;
    }
    return seconds;
}

double difftime(time_t time2, time_t time1)
{
    return (double)(time2 - time1);
}

clock_t clock(VOID)
{
    clock_t clockMsec;
    UINT64 nowNsec;

    nowNsec = LOS_CurrNanosec();
    clockMsec = (clock_t)(nowNsec / OS_SYS_NS_PER_MS);

    return clockMsec;
}

clock_t times(struct tms *buf)
{
    clock_t clockTick;

    clockTick = (clock_t)LOS_TickCountGet();
    if (buf != NULL) {
        buf->tms_cstime = clockTick;
        buf->tms_cutime = clockTick;
        buf->tms_stime  = clockTick;
        buf->tms_utime  = clockTick;
    }

    return clockTick;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
