/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Pthread condition file
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

#include "pprivate.h"
#include "pthread.h"
#include "stdlib.h"
#include "limits.h"
#include "errno.h"
#include "time_pri.h"
#include "los_atomic.h"
#include "los_event_pri.h"
#include "los_sys.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define BROADCAST_EVENT     1
#define COND_COUNTER_STEP   0x0004U
#define COND_FLAGS_MASK     0x0003U
#define COND_COUNTER_MASK   (~COND_FLAGS_MASK)

STATIC INLINE INT32 CondInitCheck(const pthread_cond_t *cond)
{
    if ((cond->event.stEventList.pstPrev == NULL) &&
        (cond->event.stEventList.pstNext == NULL)) {
        return 1;
    }
    return 0;
}

int pthread_condattr_getpshared(const pthread_condattr_t *attr, int *shared)
{
    if ((attr == NULL) || (shared == NULL)) {
        return EINVAL;
    }

    *shared = PTHREAD_PROCESS_PRIVATE;

    return 0;
}

int pthread_condattr_setpshared(pthread_condattr_t *attr, int shared)
{
    (VOID)attr;
    if ((shared != PTHREAD_PROCESS_PRIVATE) && (shared != PTHREAD_PROCESS_SHARED)) {
        return EINVAL;
    }

    if (shared != PTHREAD_PROCESS_PRIVATE) {
        return ENOSYS;
    }

    return 0;
}

int pthread_condattr_destroy(pthread_condattr_t *attr)
{
    if (attr == NULL) {
        return EINVAL;
    }

    attr->clock = INT_MAX;

    return 0;
}

int pthread_condattr_init(pthread_condattr_t *attr)
{
    if (attr == NULL) {
        return EINVAL;
    }

    attr->clock = CLOCK_REALTIME;

    return 0;
}

int pthread_cond_destroy(pthread_cond_t *cond)
{
    if (cond == NULL) {
        return EINVAL;
    }

    if (CondInitCheck(cond)) {
        return ENOERR;
    }

    if (LOS_EventDestroy(&cond->event) != LOS_OK) {
        return EBUSY;
    }
    if (pthread_mutex_destroy(cond->mutex) != ENOERR) {
        PRINT_ERR("%s mutex destroy fail!\n", __FUNCTION__);
        return EINVAL;
    }
    free(cond->mutex);
    cond->mutex = NULL;
    return ENOERR;
}

int pthread_condattr_getclock(const pthread_condattr_t *attr, clockid_t *clock)
{
    if ((attr == NULL) || (clock == NULL)) {
        return -1;
    }

    *clock = attr->clock;
    return 0;
}

int pthread_condattr_setclock(pthread_condattr_t *attr, clockid_t clock)
{
    if (attr == NULL) {
        return EINVAL;
    }

    if ((clock != CLOCK_MONOTONIC) && (clock != CLOCK_REALTIME)) {
        return EINVAL;
    }

    attr->clock = clock;

    return 0;
}

int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr)
{
    int ret = ENOERR;

    if (cond == NULL) {
        return EINVAL;
    }

    if (attr != NULL) {
        if ((attr->clock != CLOCK_MONOTONIC) && (attr->clock != CLOCK_REALTIME)) {
            PRINT_ERR("%s: clockAttr only supports CLOCK_MONOTONIC and CLOCK_REALTIME.\n", __FUNCTION__);
            return EINVAL;
        }
        cond->stCondAttr.clock = attr->clock;
    } else {
        cond->stCondAttr.clock = CLOCK_REALTIME;
    }

    (VOID)LOS_EventInit(&(cond->event));

    cond->mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    if (cond->mutex == NULL) {
        return ENOMEM;
    }

    (VOID)pthread_mutex_init(cond->mutex, NULL);

    cond->value = 0;
    (VOID)pthread_mutex_lock(cond->mutex);
    cond->count = 0;
    (VOID)pthread_mutex_unlock(cond->mutex);

    return ret;
}

STATIC VOID PthreadCondValueModify(pthread_cond_t *cond)
{
    UINT32 flags = ((UINT32)cond->value & COND_FLAGS_MASK);
    INT32 oldVal, newVal;

    while (true) {
        oldVal = cond->value;
        newVal = (INT32)(((UINT32)(oldVal - COND_COUNTER_STEP) & COND_COUNTER_MASK) | flags);
        if (LOS_AtomicCmpXchg32bits(&cond->value, newVal, oldVal) == 0) {
            break;
        }
    }
}

int pthread_cond_broadcast(pthread_cond_t *cond)
{
    int ret = ENOERR;

    if (cond == NULL) {
        return EINVAL;
    }

    (VOID)pthread_mutex_lock(cond->mutex);
    if (cond->count > 0) {
        cond->count = 0;
        (VOID)pthread_mutex_unlock(cond->mutex);

        PthreadCondValueModify(cond);

        (VOID)LOS_EventWrite(&(cond->event), BROADCAST_EVENT);
        return ret;
    }
    (VOID)pthread_mutex_unlock(cond->mutex);

    return ret;
}

int pthread_cond_signal(pthread_cond_t *cond)
{
    int ret = ENOERR;

    if (cond == NULL) {
        return EINVAL;
    }

    (VOID)pthread_mutex_lock(cond->mutex);
    if (cond->count > 0) {
        cond->count--;
        (VOID)pthread_mutex_unlock(cond->mutex);
        PthreadCondValueModify(cond);
        (VOID)OsEventWriteOnce(&(cond->event), 0x01);

        return ret;
    }
    (VOID)pthread_mutex_unlock(cond->mutex);

    return ret;
}

STATIC INT32 PthreadCondWaitSub(pthread_cond_t *cond, INT32 value, UINT32 ticks)
{
    EventCond eventCond = { &cond->value, value, ~0x01U };
    /*
     * When the scheduling lock is held:
     * (1) value is not equal to cond->value, clear the event message and
     * do not block the current thread, because other threads is calling pthread_cond_broadcast or
     * pthread_cond_signal to modify cond->value and wake up the current thread,
     * and others threads will block on the scheduling lock until the current thread releases
     * the scheduling lock.
     * (2) value is equal to cond->value, block the current thread
     * and wait to be awakened by other threads.
     */
    return (int)OsEventReadWithCond(&eventCond, &(cond->event), 0x0fU,
                                    LOS_WAITMODE_OR | LOS_WAITMODE_CLR, ticks);
}

STATIC VOID PthreadCountSub(pthread_cond_t *cond)
{
    (VOID)pthread_mutex_lock(cond->mutex);
    if (cond->count > 0) {
        cond->count--;
    }
    (VOID)pthread_mutex_unlock(cond->mutex);
}

STATIC INT32 ProcessReturnVal(pthread_cond_t *cond, INT32 val)
{
    INT32 ret;
    switch (val) {
        /* 0: event does not occur */
        case 0:
        case BROADCAST_EVENT:
            ret = ENOERR;
            break;
        case LOS_ERRNO_EVENT_READ_TIMEOUT:
            PthreadCountSub(cond);
            ret = ETIMEDOUT;
            break;
        default:
            PthreadCountSub(cond);
            ret = EINVAL;
            break;
    }
    return ret;
}

int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex,
                           const struct timespec *absTime)
{
    UINT32 absTicks;
    INT32 ret;
    INT32 oldValue;
    LosTaskCB *runTask = NULL;

    pthread_testcancel();
    if ((cond == NULL) || (mutex == NULL) || (absTime == NULL)) {
        return EINVAL;
    }

    runTask = (LosTaskCB*)OsCurrTaskGet();
    if ((mutex->stAttr.type == PTHREAD_MUTEX_ERRORCHECK) && (runTask != mutex->stLock.owner)) {
        return EPERM;
    }

    if (CondInitCheck(cond)) {
        ret = pthread_cond_init(cond, NULL);
        if (ret != ENOERR) {
            return ret;
        }
    }

    oldValue = cond->value;

    (VOID)pthread_mutex_lock(cond->mutex);
    cond->count++;
    (VOID)pthread_mutex_unlock(cond->mutex);

    if ((absTime->tv_sec == 0) && (absTime->tv_nsec == 0)) {
        return ETIMEDOUT;
    }

    if (!ValidTimespec(absTime)) {
        return EINVAL;
    }

    absTicks = OsTimespec2Tick(absTime);
    if (pthread_mutex_unlock(mutex) != ENOERR) {
        PRINT_ERR("%s: %d failed\n", __FUNCTION__, __LINE__);
    }

    ret = PthreadCondWaitSub(cond, oldValue, absTicks);
    if (pthread_mutex_lock(mutex) != ENOERR) {
        PRINT_ERR("%s: %d failed\n", __FUNCTION__, __LINE__);
    }

    ret = ProcessReturnVal(cond, ret);
    pthread_testcancel();
    return ret;
}

int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
    int ret;
    int oldValue;

    if ((cond == NULL) || (mutex == NULL)) {
        return EINVAL;
    }

    if (CondInitCheck(cond)) {
        ret = pthread_cond_init(cond, NULL);
        if (ret != ENOERR) {
            return ret;
        }
    }

    oldValue = cond->value;

    (VOID)pthread_mutex_lock(cond->mutex);
    cond->count++;
    (VOID)pthread_mutex_unlock(cond->mutex);

    if (pthread_mutex_unlock(mutex) != ENOERR) {
        PRINT_ERR("%s: %d failed\n", __FUNCTION__, __LINE__);
    }

    ret = PthreadCondWaitSub(cond, oldValue, LOS_WAIT_FOREVER);

    if (pthread_mutex_lock(mutex) != ENOERR) {
        PRINT_ERR("%s: %d failed\n", __FUNCTION__, __LINE__);
    }

    switch (ret) {
        /* 0: event does not occur */
        case 0:
        case BROADCAST_EVENT:
            ret = ENOERR;
            break;
        default:
            PthreadCountSub(cond);
            ret = EINVAL;
            break;
    }

    return ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
