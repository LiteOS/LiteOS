/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Mqueue file
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

#include "mqueue.h"
#include "stdio.h"
#include "fcntl.h"
#include "string.h"
#include "errno.h"
#include "limits.h"
#include "pthread.h"
#include "map_error.h"
#include "time_pri.h"
#include "los_memory.h"
#include "los_task.h"
#include "los_sys.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifndef LOSCFG_LIB_CONFIGURABLE
STATIC struct mqarray g_queueTable[LOSCFG_BASE_IPC_QUEUE_LIMIT];
#else
__attribute__((section(".libc.mqueue"))) struct mqarray g_queueTable[LOSCFG_BASE_IPC_QUEUE_LIMIT_CONFIG];
#endif
STATIC pthread_mutex_t g_mqueueMutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

STATIC INLINE INT32 MqNameCheck(const CHAR *mqName)
{
    if (mqName == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (strlen(mqName) == 0) {
        errno = EINVAL;
        return -1;
    }

    if (strlen(mqName) > (PATH_MAX - 1)) {
        errno = ENAMETOOLONG;
        return -1;
    }
    return 0;
}

STATIC INLINE UINT32 GetMqueueCBByID(UINT32 queueId, LosQueueCB **queueCB)
{
    LosQueueCB *tmpQueueCB = NULL;
    if (queueCB == NULL) {
        errno = EINVAL;
        return LOS_ERRNO_QUEUE_READ_PTR_NULL;
    }
    tmpQueueCB = GET_QUEUE_HANDLE(queueId);
    if ((GET_QUEUE_INDEX(queueId) >= LOSCFG_BASE_IPC_QUEUE_LIMIT) || (tmpQueueCB->queueId != queueId)) {
        return LOS_ERRNO_QUEUE_INVALID;
    }
    *queueCB = tmpQueueCB;

    return LOS_OK;
}

STATIC INLINE struct mqarray *GetMqueueCBByName(const CHAR *name)
{
    UINT32 index;
    struct mqarray *mqueueCB = NULL;

    for (index = 0; index < LOSCFG_BASE_IPC_QUEUE_LIMIT; index++) {
        if (strncmp(name, (const CHAR *)(g_queueTable[index].mq_name), PATH_MAX - 1) == 0) {
            mqueueCB = &(g_queueTable[index]);
            break;
        }
    }
    return mqueueCB;
}

STATIC INT32 DoMqueueDelete(struct mqarray *mqueueCB)
{
    UINT32 ret;

    mqueueCB->mq_name[0] = '\0';
    mqueueCB->mqcb = NULL;

    ret = LOS_QueueDelete(mqueueCB->mq_id);
    switch (ret) {
        case LOS_OK:
            return 0;
        case LOS_ERRNO_QUEUE_NOT_FOUND:
        case LOS_ERRNO_QUEUE_NOT_CREATE:
        case LOS_ERRNO_QUEUE_IN_TSKUSE:
        case LOS_ERRNO_QUEUE_IN_TSKWRITE:
            errno = EAGAIN;
            return -1;
        default:
            errno = EINVAL;
            return -1;
    }
}

STATIC struct mqpersonal *DoMqueueCreate(const struct mq_attr *attr, const CHAR *mqName, INT32 openFlag)
{
    struct mq_attr defaultAttr = { 0, MQ_MAX_MSG_NUM, MQ_MAX_MSG_LEN, 0 };
    struct mqarray *mqueueCB = NULL;
    UINT32 err;
    UINT32 mqueueId;

    if (attr == NULL) {
        attr = &defaultAttr;
    } else if ((attr->mq_maxmsg < 0) || (attr->mq_maxmsg > (long int)USHRT_MAX) ||
               (attr->mq_msgsize < 0) || (attr->mq_msgsize > (long int)(USHRT_MAX - sizeof(UINT32)))) {
        errno = EINVAL;
        goto ERROUT;
    }

    err = LOS_QueueCreate(NULL, attr->mq_maxmsg, &mqueueId, 0, attr->mq_msgsize);
    if (map_errno(err) != ENOERR) {
        goto ERROUT;
    }

    if (g_queueTable[GET_QUEUE_INDEX(mqueueId)].mqcb == NULL) {
        mqueueCB = &(g_queueTable[GET_QUEUE_INDEX(mqueueId)]);
        mqueueCB->mq_id = mqueueId;
    }

    if (mqueueCB == NULL) {
        errno = EINVAL;
        goto ERROUT_FREE_QUEUE;
    }

    if (GetMqueueCBByID(mqueueCB->mq_id, &(mqueueCB->mqcb)) != LOS_OK) {
        errno = ENOSPC;
        goto ERROUT_FREE_QUEUE;
    }

    mqueueCB->mq_personal = (struct mqpersonal *)LOS_MemAlloc(OS_SYS_MEM_ADDR, sizeof(struct mqpersonal));
    if (mqueueCB->mq_personal == NULL) {
        mqueueCB->mqcb->queueHandle = NULL;
        mqueueCB->mqcb = NULL;
        errno = ENOSPC;
        goto ERROUT_FREE_QUEUE;
    }

    if (strncpy_s(mqueueCB->mq_name, PATH_MAX, mqName, PATH_MAX - 1) != EOK) {
        errno = EINVAL;
        goto ERROUT_FREE_MEM;
    }
    mqueueCB->unlinkflag = FALSE;
    mqueueCB->mq_personal->mq_status = MQ_USE_MAGIC;
    mqueueCB->mq_personal->mq_next = NULL;
    mqueueCB->mq_personal->mq_posixdes = mqueueCB;
    mqueueCB->mq_personal->mq_flags = (INT32)((UINT32)openFlag | ((UINT32)attr->mq_flags & (UINT32)FNONBLOCK));

    return mqueueCB->mq_personal;

ERROUT_FREE_MEM:
    (VOID)LOS_MemFree(OS_SYS_MEM_ADDR, mqueueCB->mq_personal);
    mqueueCB->mq_personal = NULL;
ERROUT_FREE_QUEUE:
    (VOID)LOS_QueueDelete(mqueueId);
ERROUT:
    return (struct mqpersonal *)-1;
}

STATIC struct mqpersonal *DoMqueueOpen(struct mqarray *mqueueCB, INT32 openFlag)
{
    struct mqpersonal *privateMqPersonal = NULL;

    /* already have the same name of g_squeuetable */
    if (mqueueCB->unlinkflag == TRUE) {
        errno = EINVAL;
        goto ERROUT;
    }
    /* alloc mqprivate and add to mqarray */
    privateMqPersonal = (struct mqpersonal *)LOS_MemAlloc(OS_SYS_MEM_ADDR, sizeof(struct mqpersonal));
    if (privateMqPersonal == NULL) {
        errno = ENOSPC;
        goto ERROUT;
    }

    privateMqPersonal->mq_next = mqueueCB->mq_personal;
    mqueueCB->mq_personal = privateMqPersonal;

    privateMqPersonal->mq_posixdes = mqueueCB;
    privateMqPersonal->mq_flags = openFlag;
    privateMqPersonal->mq_status = MQ_USE_MAGIC;

    return privateMqPersonal;

ERROUT:
    return (struct mqpersonal *)-1;
}

mqd_t mq_open(const char *mqName, int openFlag, ...)
{
    struct mqarray *mqueueCB = NULL;
    struct mqpersonal *privateMqPersonal = (struct mqpersonal *)-1;
    struct mq_attr *attr = NULL;
    va_list ap;

    if (MqNameCheck(mqName) == -1) {
        return (mqd_t)-1;
    }

    (VOID)pthread_mutex_lock(&g_mqueueMutex);
    mqueueCB = GetMqueueCBByName(mqName);
    if ((UINT32)openFlag & (UINT32)O_CREAT) {
        if (mqueueCB != NULL) {
            if (((UINT32)openFlag & (UINT32)O_EXCL)) {
                errno = EEXIST;
                goto OUT;
            }
            privateMqPersonal = DoMqueueOpen(mqueueCB, openFlag);
        } else {
            va_start(ap, openFlag);
            (VOID)va_arg(ap, int);
            attr = va_arg(ap, struct mq_attr *);
            va_end(ap);

            privateMqPersonal = DoMqueueCreate(attr, mqName, openFlag);
        }
    } else {
        if (mqueueCB == NULL) {
            errno = ENOENT;
            goto OUT;
        }
        privateMqPersonal = DoMqueueOpen(mqueueCB, openFlag);
    }

OUT:
    (VOID)pthread_mutex_unlock(&g_mqueueMutex);
    return (mqd_t)privateMqPersonal;
}

int mq_close(mqd_t personal)
{
    INT32 ret = -1;
    struct mqarray *mqueueCB = NULL;
    struct mqpersonal *privateMqPersonal = NULL;
    struct mqpersonal *tmp = NULL;

    if ((personal == (mqd_t)NULL) || (personal == (mqd_t)-1)) {
        errno = EBADF;
        return -1;
    }

    (VOID)pthread_mutex_lock(&g_mqueueMutex);
    privateMqPersonal = (struct mqpersonal *)personal;
    if (privateMqPersonal->mq_status != MQ_USE_MAGIC) {
        errno = EBADF;
        goto OUT_UNLOCK;
    }

    mqueueCB = privateMqPersonal->mq_posixdes;
    if (mqueueCB->mq_personal == NULL) {
        errno = EBADF;
        goto OUT_UNLOCK;
    }

    /* find the personal and remove */
    if (mqueueCB->mq_personal == privateMqPersonal) {
        mqueueCB->mq_personal = privateMqPersonal->mq_next;
    } else {
        for (tmp = mqueueCB->mq_personal; tmp->mq_next != NULL; tmp = tmp->mq_next) {
            if (tmp->mq_next == privateMqPersonal) {
                break;
            }
        }
        if (tmp->mq_next == NULL) {
            errno = EBADF;
            goto OUT_UNLOCK;
        }
        tmp->mq_next = privateMqPersonal->mq_next;
    }
    /* flag no use */
    privateMqPersonal->mq_status = 0;

    /* free the personal */
    ret = LOS_MemFree(OS_SYS_MEM_ADDR, privateMqPersonal);
    if (ret != LOS_OK) {
        errno = EFAULT;
        ret = -1;
        goto OUT_UNLOCK;
    }

    if ((mqueueCB->unlinkflag == TRUE) && (mqueueCB->mq_personal == NULL)) {
        ret = DoMqueueDelete(mqueueCB);
    }

OUT_UNLOCK:
    (VOID)pthread_mutex_unlock(&g_mqueueMutex);
    return ret;
}

int mq_setattr(mqd_t personal, const struct mq_attr *mqSetAttr, struct mq_attr *mqOldAttr)
{
    struct mqpersonal *privateMqPersonal = NULL;

    if ((personal == (mqd_t)NULL) || (personal == (mqd_t)-1)) {
        errno = EBADF;
        return -1;
    }

    if (mqSetAttr == NULL) {
        errno = EINVAL;
        return -1;
    }

    (VOID)pthread_mutex_lock(&g_mqueueMutex);
    privateMqPersonal = (struct mqpersonal *)personal;
    if (privateMqPersonal->mq_status != MQ_USE_MAGIC) {
        errno = EBADF;
        (VOID)pthread_mutex_unlock(&g_mqueueMutex);
        return -1;
    }

    if (mqOldAttr != NULL) {
        (VOID)mq_getattr((mqd_t)privateMqPersonal, mqOldAttr);
    }

    privateMqPersonal->mq_flags = (INT32)((UINT32)privateMqPersonal->mq_flags & (UINT32)(~FNONBLOCK)); /* clear */
    if (((UINT32)mqSetAttr->mq_flags & (UINT32)FNONBLOCK) == (UINT32)FNONBLOCK) {
        privateMqPersonal->mq_flags = (INT32)((UINT32)privateMqPersonal->mq_flags | (UINT32)FNONBLOCK);
    }
    (VOID)pthread_mutex_unlock(&g_mqueueMutex);
    return 0;
}

int mq_getattr(mqd_t personal, struct mq_attr *mqAttr)
{
    struct mqarray *mqueueCB = NULL;
    struct mqpersonal *privateMqPersonal = NULL;

    if ((personal == (mqd_t)NULL) || (personal == (mqd_t)-1)) {
        errno = EBADF;
        return -1;
    }

    if (mqAttr == NULL) {
        errno = EINVAL;
        return -1;
    }

    (VOID)pthread_mutex_lock(&g_mqueueMutex);
    privateMqPersonal = (struct mqpersonal *)personal;
    if (privateMqPersonal->mq_status != MQ_USE_MAGIC) {
        errno = EBADF;
        (VOID)pthread_mutex_unlock(&g_mqueueMutex);
        return -1;
    }

    mqueueCB = privateMqPersonal->mq_posixdes;
    mqAttr->mq_maxmsg = mqueueCB->mqcb->queueLen;
    mqAttr->mq_msgsize = mqueueCB->mqcb->queueSize - sizeof(UINT32);
    mqAttr->mq_curmsgs = mqueueCB->mqcb->readWriteableCnt[OS_QUEUE_READ];
    mqAttr->mq_flags = privateMqPersonal->mq_flags;
    (VOID)pthread_mutex_unlock(&g_mqueueMutex);
    return 0;
}

int mq_unlink(const char *mqName)
{
    INT32 ret = 0;
    struct mqarray *mqueueCB = NULL;

    if (MqNameCheck(mqName) == -1) {
        return -1;
    }

    (VOID)pthread_mutex_lock(&g_mqueueMutex);
    mqueueCB = GetMqueueCBByName(mqName);
    if (mqueueCB == NULL) {
        errno = ENOENT;
        goto ERROUT_UNLOCK;
    }

    if (mqueueCB->mq_personal != NULL) {
        errno = EINTR;
        mqueueCB->unlinkflag = TRUE;
        goto ERROUT_UNLOCK;
    } else {
        ret = DoMqueueDelete(mqueueCB);
    }

    (VOID)pthread_mutex_unlock(&g_mqueueMutex);
    return ret;

ERROUT_UNLOCK:
    (VOID)pthread_mutex_unlock(&g_mqueueMutex);
    return -1;
}

STATIC INT32 ConvertTimeout(long flags, const struct timespec *absTimeout, UINT64 *ticks)
{
    if ((UINT32)flags & (UINT32)FNONBLOCK) {
        *ticks = LOS_NO_WAIT;
        return 0;
    }

    if (absTimeout == NULL) {
        *ticks = LOS_WAIT_FOREVER;
        return 0;
    }

    if (!ValidTimespec(absTimeout)) {
        errno = EINVAL;
        return -1;
    }

    *ticks = OsTimespec2Tick(absTimeout);
    return 0;
}

STATIC INLINE BOOL MqParamCheck(mqd_t personal, const char *msg, size_t msgLen)
{
    if ((personal == (mqd_t)NULL) || (personal == (mqd_t)-1)) {
        errno = EBADF;
        return FALSE;
    }

    if ((msg == NULL) || (msgLen == 0) || (msgLen > UINT_MAX)) {
        errno = EINVAL;
        return FALSE;
    }
    return TRUE;
}

int mq_timedsend(mqd_t personal, const char *msg, size_t msgLen,
                 unsigned int msgPrio, const struct timespec *absTimeout)
{
    UINT32 mqueueId, err;
    UINT64 absTicks;
    struct mqarray *mqueueCB = NULL;
    struct mqpersonal *privateMqPersonal = NULL;

    if (!MqParamCheck(personal, msg, msgLen)) {
        goto ERROUT;
    }

    if (msgPrio > (MQ_PRIO_MAX - 1)) {
        errno = EINVAL;
        goto ERROUT;
    }

    (VOID)pthread_mutex_lock(&g_mqueueMutex);
    privateMqPersonal = (struct mqpersonal *)personal;
    if (privateMqPersonal->mq_status != MQ_USE_MAGIC) {
        errno = EBADF;
        goto ERROUT_UNLOCK;
    }

    mqueueCB = privateMqPersonal->mq_posixdes;
    if (msgLen > (size_t)(mqueueCB->mqcb->queueSize - sizeof(UINT32))) {
        errno = EMSGSIZE;
        goto ERROUT_UNLOCK;
    }

    if ((((UINT32)privateMqPersonal->mq_flags & (UINT32)O_WRONLY) != (UINT32)O_WRONLY) &&
        (((UINT32)privateMqPersonal->mq_flags & (UINT32)O_RDWR) != (UINT32)O_RDWR)) {
        errno = EBADF;
        goto ERROUT_UNLOCK;
    }

    if (ConvertTimeout(privateMqPersonal->mq_flags, absTimeout, &absTicks) == -1) {
        goto ERROUT_UNLOCK;
    }
    mqueueId = mqueueCB->mq_id;
    (VOID)pthread_mutex_unlock(&g_mqueueMutex);

    err = LOS_QueueWriteCopy(mqueueId, (VOID *)msg, (UINT32)msgLen, (UINT32)absTicks);
    if (map_errno(err) != ENOERR) {
        goto ERROUT;
    }

    return 0;

ERROUT_UNLOCK:
    (VOID)pthread_mutex_unlock(&g_mqueueMutex);
ERROUT:
    return -1;
}

ssize_t mq_timedreceive(mqd_t personal, char *msg, size_t msgLen,
                        unsigned int *msgPrio, const struct timespec *absTimeout)
{
    UINT32 mqueueId, err;
    UINT32 receiveLen;
    UINT64 absTicks;
    struct mqarray *mqueueCB = NULL;
    struct mqpersonal *privateMqPersonal = NULL;

    if (!MqParamCheck(personal, msg, msgLen)) {
        goto ERROUT;
    }

    if (msgPrio != NULL) {
        *msgPrio = 0;
    }

    (VOID)pthread_mutex_lock(&g_mqueueMutex);
    privateMqPersonal = (struct mqpersonal *)personal;
    if (privateMqPersonal->mq_status != MQ_USE_MAGIC) {
        errno = EBADF;
        goto ERROUT_UNLOCK;
    }

    mqueueCB = privateMqPersonal->mq_posixdes;
    if (msgLen < (size_t)(mqueueCB->mqcb->queueSize - sizeof(UINT32))) {
        errno = EMSGSIZE;
        goto ERROUT_UNLOCK;
    }

    if (((UINT32)privateMqPersonal->mq_flags & (UINT32)O_WRONLY) == (UINT32)O_WRONLY) {
        errno = EBADF;
        goto ERROUT_UNLOCK;
    }

    if (ConvertTimeout(privateMqPersonal->mq_flags, absTimeout, &absTicks) == -1) {
        goto ERROUT_UNLOCK;
    }

    receiveLen = msgLen;
    mqueueId = mqueueCB->mq_id;
    (VOID)pthread_mutex_unlock(&g_mqueueMutex);

    err = LOS_QueueReadCopy(mqueueId, (VOID *)msg, &receiveLen, (UINT32)absTicks);
    if (map_errno(err) == ENOERR) {
        return (ssize_t)receiveLen;
    } else {
        goto ERROUT;
    }

ERROUT_UNLOCK:
    (VOID)pthread_mutex_unlock(&g_mqueueMutex);
ERROUT:
    return -1;
}

/* not support the prio */
int mq_send(mqd_t personal, const char *msg_ptr, size_t msg_len, unsigned int msg_prio)
{
    return mq_timedsend(personal, msg_ptr, msg_len, msg_prio, NULL);
}

ssize_t mq_receive(mqd_t personal, char *msg_ptr, size_t msg_len, unsigned int *msg_prio)
{
    return mq_timedreceive(personal, msg_ptr, msg_len, msg_prio, NULL);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
