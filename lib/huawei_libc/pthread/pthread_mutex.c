/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Pthread mutex file
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

#include "pthread.h"
#include "stdlib.h"
#include "errno.h"
#include "map_error.h"
#include "los_bitmap.h"
#include "los_mux_pri.h"
#include "los_mux_debug_pri.h"
#include "los_task_pri.h"
#include "los_mp_pri.h"
#if (LOSCFG_BASE_CORE_SWTMR == YES)
#include "los_exc.h"
#include "los_swtmr_pri.h"
#endif
#include "time_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define MUTEXATTR_TYPE_MASK 0x0fU

STATIC UINT32 OsMuxPendForPosix(pthread_mutex_t *mutex, UINT32 timeout);
STATIC INT32 OsMuxPostForPosix(pthread_mutex_t *mutex);

int pthread_mutexattr_init(pthread_mutexattr_t *attr)
{
    if (attr == NULL) {
        return EINVAL;
    }

#if defined POSIX_MUTEX_DEFAULT_INHERIT
    attr->protocol    = PTHREAD_PRIO_INHERIT;
#elif defined POSIX_MUTEX_DEFAULT_PROTECT
    attr->protocol    = PTHREAD_PRIO_PROTECT;
#else
    attr->protocol    = PTHREAD_PRIO_NONE;
#endif
    attr->prioceiling = OS_TASK_PRIORITY_LOWEST;
    attr->type        = PTHREAD_MUTEX_DEFAULT;
    return ENOERR;
}

int pthread_mutexattr_destroy(pthread_mutexattr_t *attr)
{
    if (attr == NULL) {
        return EINVAL;
    }

    return ENOERR;
}

int pthread_mutexattr_setprotocol(pthread_mutexattr_t *attr, int protocol)
{
    if (attr == NULL) {
        return EINVAL;
    }

    switch (protocol) {
        case PTHREAD_PRIO_NONE:
        case PTHREAD_PRIO_INHERIT:
        case PTHREAD_PRIO_PROTECT:
            attr->protocol = (UINT8)protocol;
            return ENOERR;
        default:
            return EINVAL;
    }
}

int pthread_mutexattr_getprotocol(const pthread_mutexattr_t *attr, int *protocol)
{
    if ((attr != NULL) && (protocol != NULL)) {
        *protocol = attr->protocol;
    } else {
        return EINVAL;
    }

    return ENOERR;
}

int pthread_mutexattr_setprioceiling(pthread_mutexattr_t *attr, int prioceiling)
{
    if ((attr == NULL) ||
        (prioceiling < OS_TASK_PRIORITY_HIGHEST) ||
        (prioceiling > OS_TASK_PRIORITY_LOWEST)) {
        return EINVAL;
    }

    attr->prioceiling = (UINT8)prioceiling;

    return ENOERR;
}

int pthread_mutexattr_getprioceiling(const pthread_mutexattr_t *attr, int *prioceiling)
{
    if (attr == NULL) {
        return EINVAL;
    }

    if (prioceiling != NULL) {
        *prioceiling = attr->prioceiling;
    }

    return ENOERR;
}

int pthread_mutex_setprioceiling(pthread_mutex_t *mutex, int prioceiling, int *oldPrioceiling)
{
    INT32 ret;
    INT32 retLock;
    if ((mutex == NULL) ||
        (prioceiling < OS_TASK_PRIORITY_HIGHEST) ||
        (prioceiling > OS_TASK_PRIORITY_LOWEST)) {
        return EINVAL;
    }

    retLock = pthread_mutex_lock(mutex);
    if (retLock != ENOERR) {
        return retLock;
    }

    if (oldPrioceiling != NULL) {
        *oldPrioceiling = mutex->stAttr.prioceiling;
    }

    ret = pthread_mutexattr_setprioceiling(&mutex->stAttr, prioceiling);

    retLock = pthread_mutex_unlock(mutex);
    if ((ret == ENOERR) && (retLock != ENOERR)) {
        return retLock;
    }

    return ret;
}

int pthread_mutex_getprioceiling(const pthread_mutex_t *mutex, int *prioceiling)
{
    if ((mutex != NULL) && (prioceiling != NULL)) {
        *prioceiling = mutex->stAttr.prioceiling;
        return ENOERR;
    }
    return EINVAL;
}

int pthread_mutexattr_gettype(const pthread_mutexattr_t *attr, int *outType)
{
    INT32 type;

    if ((attr == NULL) || (outType == NULL)) {
        return EINVAL;
    }

    type = (INT32)(attr->type & MUTEXATTR_TYPE_MASK);
    if ((type < PTHREAD_MUTEX_NORMAL) || (type > PTHREAD_MUTEX_ERRORCHECK)) {
        return EINVAL;
    }

    *outType = type;
    return ENOERR;
}

int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type)
{
    if ((attr == NULL) ||
        (type < PTHREAD_MUTEX_NORMAL) ||
        (type > PTHREAD_MUTEX_ERRORCHECK)) {
        return EINVAL;
    }

    attr->type = (UINT8)((attr->type & ~MUTEXATTR_TYPE_MASK) | (UINT32)type);
    return ENOERR;
}

STATIC UINT32 CheckMutexAttr(const pthread_mutexattr_t *attr)
{
    if (((INT8)(attr->type) < PTHREAD_MUTEX_NORMAL) ||
        (attr->type > PTHREAD_MUTEX_ERRORCHECK)) {
        return LOS_NOK;
    }
    if (((INT8)(attr->prioceiling) < OS_TASK_PRIORITY_HIGHEST) ||
        (attr->prioceiling > OS_TASK_PRIORITY_LOWEST)) {
        return LOS_NOK;
    }
    if (((INT8)(attr->protocol) < PTHREAD_PRIO_NONE) ||
        (attr->protocol > PTHREAD_PRIO_PROTECT)) {
        return LOS_NOK;
    }
    return LOS_OK;
}

/* Initialize mutex. If mutexAttr is NULL, use default attributes. */
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexAttr)
{
    pthread_mutexattr_t useAttr;
    MuxBaseCB *muxCreated = NULL;
    UINT32 intSave;

    if (mutex == NULL) {
        return EINVAL;
    }

    /* Set up the attributes we are going to use. */
    if (mutexAttr == NULL) {
        (VOID)pthread_mutexattr_init(&useAttr);
    } else {
        useAttr = *mutexAttr;
    }

    if (CheckMutexAttr(&useAttr) != LOS_OK) {
        return EINVAL;
    }

    mutex->stAttr = useAttr;
    muxCreated = &mutex->stLock;

    SCHEDULER_LOCK(intSave);
    muxCreated->muxCount = 0;
    muxCreated->owner = NULL;
    LOS_ListInit(&muxCreated->muxList);
    SCHEDULER_UNLOCK(intSave);

    return ENOERR;
}

int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
    MuxBaseCB *muxDeleted = NULL;
    UINT32 intSave;

    if (mutex == NULL) {
        return EINVAL;
    }

    muxDeleted = &mutex->stLock;

    SCHEDULER_LOCK(intSave);
    if (muxDeleted->muxCount != 0) {
        SCHEDULER_UNLOCK(intSave);
        return EBUSY;
    }
    (VOID)memset_s(mutex, sizeof(pthread_mutex_t), 0, sizeof(pthread_mutex_t));

    SCHEDULER_UNLOCK(intSave);
    return ENOERR;
}

UINT32 OsMuxPreCheck(pthread_mutex_t *mutex, LosTaskCB *runTask)
{
    if (mutex == NULL) {
        return EINVAL;
    }

    if (OS_INT_ACTIVE) {
        return EPERM;
    }

    /* DO NOT recommend to use blocking API in system tasks */
    if (runTask->taskFlags & OS_TASK_FLAG_SYSTEM) {
        PRINT_DEBUG("Warning: DO NOT recommend to use %s in system tasks.\n", __FUNCTION__);
    }

    if (CheckMutexAttr(&mutex->stAttr) != LOS_OK) {
        return EINVAL;
    }

    return ENOERR;
}

/* Lock mutex, waiting for it if necessary. */
int pthread_mutex_lock(pthread_mutex_t *mutex)
{
    MuxBaseCB *muxPended = NULL;
    LosTaskCB *runTask = NULL;
    UINT32 ret;

    runTask = (LosTaskCB*)OsCurrTaskGet();
    ret = OsMuxPreCheck(mutex, runTask);
    if (ret != ENOERR) {
        return (INT32)ret;
    }

    muxPended = &mutex->stLock;
    if ((mutex->stAttr.type == PTHREAD_MUTEX_ERRORCHECK) &&
        (muxPended->muxCount != 0) &&
        (muxPended->owner == runTask)) {
        return EDEADLK;
    }

    ret = OsMuxPendForPosix(mutex, LOS_WAIT_FOREVER);
    return map_errno(ret);
}

int pthread_mutex_trylock(pthread_mutex_t *mutex)
{
    MuxBaseCB *muxPended = NULL;
    LosTaskCB *runTask = NULL;
    UINT32 ret;

    runTask = (LosTaskCB*)OsCurrTaskGet();
    ret = OsMuxPreCheck(mutex, runTask);
    if (ret != ENOERR) {
        return (INT32)ret;
    }

    muxPended = &mutex->stLock;

    if ((mutex->stLock.owner != NULL) && (mutex->stLock.owner != runTask)) {
        return EBUSY;
    }
    if ((mutex->stAttr.type != PTHREAD_MUTEX_RECURSIVE) && (muxPended->muxCount != 0)) {
        return EBUSY;
    }

    ret = OsMuxPendForPosix(mutex, 0);
    return map_errno(ret);
}

int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
    LosTaskCB *runTask = NULL;
    UINT32 ret;

    runTask = (LosTaskCB*)OsCurrTaskGet();
    ret = OsMuxPreCheck(mutex, runTask);
    if (ret != ENOERR) {
        return (INT32)ret;
    }

    return OsMuxPostForPosix(mutex);
}

STATIC VOID OsMuxBitmapSet(const pthread_mutex_t *mutex, const LosTaskCB *runTask, const MuxBaseCB *muxPended)
{
    if ((muxPended->owner->priority > runTask->priority) &&
        (mutex->stAttr.protocol == PTHREAD_PRIO_INHERIT)) {
        LOS_BitmapSet(&(muxPended->owner->priBitMap), muxPended->owner->priority);
        OsTaskPriModify(muxPended->owner, runTask->priority);
    }
}

STATIC UINT32 OsMuxPendForPosix(pthread_mutex_t *mutex, UINT32 timeout)
{
    UINT32 retErr;
    UINT32 intSave;
    MuxBaseCB *muxPended = &mutex->stLock;
    LosTaskCB *runTask = NULL;

    SCHEDULER_LOCK(intSave);
    if ((muxPended->muxList.pstPrev == NULL) ||
        (muxPended->muxList.pstNext == NULL)) {
        /* This is for mutex macro initialization. */
        muxPended->muxCount = 0;
        muxPended->owner = NULL;
        LOS_ListInit(&muxPended->muxList);
    }
    runTask = OsCurrTaskGet();
    if (muxPended->muxCount == 0) {
        muxPended->muxCount++;
        muxPended->owner = runTask;
        OsMuxDlockNodeInsertHook(runTask->taskId, muxPended);

        if ((runTask->priority > mutex->stAttr.prioceiling) &&
            (mutex->stAttr.protocol == PTHREAD_PRIO_PROTECT)) {
            LOS_BitmapSet(&runTask->priBitMap, runTask->priority);
            OsTaskPriModify(runTask, mutex->stAttr.prioceiling);
        }

        SCHEDULER_UNLOCK(intSave);
        return LOS_OK;
    }

    if ((muxPended->owner == runTask) &&
        (mutex->stAttr.type == PTHREAD_MUTEX_RECURSIVE)) {
        muxPended->muxCount++;
        SCHEDULER_UNLOCK(intSave);
        return LOS_OK;
    }

    if (!OsPreemptableInSched()) {
        retErr = LOS_ERRNO_MUX_PEND_IN_LOCK;
        goto LOS_ERREND;
    }

    OsMuxBitmapSet(mutex, runTask, muxPended);
    retErr = OsMuxPendOp(runTask, muxPended, timeout, &intSave);
    if (retErr != LOS_OK) {
        goto LOS_ERREND;
    }

    SCHEDULER_UNLOCK(intSave);
    return LOS_OK;

LOS_ERREND:
    SCHEDULER_UNLOCK(intSave);
    return retErr;
}

STATIC INT32 OsMuxPostForPosix(pthread_mutex_t *mutex)
{
    UINT32 intSave;
    UINT32 ret;
    UINT16 bitMapPri;
    MuxBaseCB *muxPosted = &mutex->stLock;
    LosTaskCB *runTask = NULL;

    SCHEDULER_LOCK(intSave);

    if (muxPosted->muxCount == 0) {
        SCHEDULER_UNLOCK(intSave);
        return EPERM;
    }

    runTask = OsCurrTaskGet();
    if (muxPosted->owner != runTask) {
        SCHEDULER_UNLOCK(intSave);
        return EPERM;
    }

    if ((--muxPosted->muxCount != 0) &&
        (mutex->stAttr.type == PTHREAD_MUTEX_RECURSIVE)) {
        SCHEDULER_UNLOCK(intSave);
        return ENOERR;
    }

    if (mutex->stAttr.protocol == PTHREAD_PRIO_PROTECT) {
        bitMapPri = LOS_HighBitGet(runTask->priBitMap);
        if (bitMapPri != LOS_INVALID_BIT_INDEX) {
            LOS_BitmapClr(&runTask->priBitMap, bitMapPri);
            OsTaskPriModify(runTask, bitMapPri);
        }
    }

    /* Whether a task block the mutex lock. */
    ret = OsMuxPostOp(runTask, (MuxBaseCB *)muxPosted);
    SCHEDULER_UNLOCK(intSave);
    if (ret == MUX_SCHEDULE) {
        LOS_MpSchedule(OS_MP_CPU_ALL);
        LOS_Schedule();
    }

    return ENOERR;
}

int pthread_mutex_timedlock(pthread_mutex_t *mutex, const struct timespec *absTime)
{
    MuxBaseCB *muxPended = NULL;
    LosTaskCB *runTask = NULL;
    UINT32 ret;
    UINT32 absTicks;
    struct timespec to = {0};

    runTask = (LosTaskCB*)OsCurrTaskGet();
    ret = OsMuxPreCheck(mutex, runTask);
    if (ret != ENOERR) {
        return (INT32)ret;
    }

    muxPended = &mutex->stLock;

    if ((mutex->stAttr.type == PTHREAD_MUTEX_ERRORCHECK) &&
        (muxPended->muxCount != 0) &&
        (muxPended->owner == runTask)) {
        return EDEADLK;
    }

    if ((absTime == NULL) || (!ValidTimespec(absTime)) || clock_gettime(CLOCK_REALTIME, &to)) {
        return EINVAL;
    }

    /* calculate abs time */
    to.tv_sec = absTime->tv_sec - to.tv_sec;
    to.tv_nsec = absTime->tv_nsec - to.tv_nsec;
    if (to.tv_nsec < 0) {
        to.tv_sec--;
        to.tv_nsec += OS_SYS_NS_PER_SECOND;
    }
    if (to.tv_sec < 0) {
        return ETIMEDOUT;
    }
    /* convert to tick */
    absTicks = OsTimespec2Tick(&to);

    ret = OsMuxPendForPosix(mutex, absTicks);
    return map_errno(ret);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
