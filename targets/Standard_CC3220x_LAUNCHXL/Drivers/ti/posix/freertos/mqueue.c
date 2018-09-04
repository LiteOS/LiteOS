/*
 * Copyright (c) 2016-2018 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== mqueue.c ========
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* FreeRTOS header file */
#include <FreeRTOS.h>
#include <portmacro.h>
#include <queue.h>
#include <semphr.h>
#include <task.h>

#include <ti/drivers/dpl/HwiP.h>

#include <mqueue.h>
#include <time.h>
#include <errno.h>

/*  Gate protection is not needed in boot thread (i.e. from main()).
 *  Check the scheduler state and only use the gate when scheduler
 *  is running. These macros improves the code presentation.
 */
#define GATE_ENTER(gate)                                        \
    if (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING) {    \
        xSemaphoreTake(gate, portMAX_DELAY);                    \
    }

#define GATE_LEAVE(gate)                                        \
    if (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING) {    \
        xSemaphoreGive(gate);                                   \
    }

/*
 *  ======== MQueueObj ========
 *  The message queue object, created the first time the message queue is
 *  opened.
 */
typedef struct MQueueObj {
    struct MQueueObj  *next;
    struct MQueueObj  *prev;
    QueueHandle_t      queue;
    struct mq_attr     attrs;
    int                refCount;
    char              *name;
} MQueueObj;

/*
 *  ======== MQueueDesc ========
 *  Threads that call mq_open() get a descriptor handle.  This allows
 *  passing of different flags to mq_open().
 */
typedef struct MQueueDesc {
    MQueueObj *msgQueue;  /* The actual message queue object */
    long       flags;
} MQueueDesc;

/*
 *  Function for obtaining a timeout in Clock ticks from the difference of
 *  an absolute time and the current time.
 */
extern int _clock_abstime2ticks(clockid_t clockId,
        const struct timespec *abstime, uint32_t *ticks);

static MQueueObj *findInList(const char *name);

static MQueueObj *mqList = NULL;

static SemaphoreHandle_t mqGate = NULL;

/*
 *  Default message queue attrs to be used if NULL attributes are
 *  passed to mq_open() on O_CREAT.  In general, applications should not
 *  rely on the default attributes, as they are implementation defined,
 *  and using them may result in non-portable code.
 *  The default value of 8 bytes for mq_msgsize in defaultAttrs was chosen
 *  to allow for a size and a pointer.  The mq_maxmsg default of 4 is a
 *  multiple of 4, and memory allocated for the default message size and
 *  number of messages would be 32 bytes.
 */
static mq_attr defaultAttrs = {
    0,      /* mq_flags */
    4,      /* mq_maxmsg - number of messages */
    8,      /* mq_msgsize - size of message */
    0       /* mq_curmsgs */
};

/*
 *  ======== mq_close ========
 */
int mq_close(mqd_t mqdes)
{
    MQueueDesc *mqd = (MQueueDesc *)mqdes;
    MQueueObj  *msgQueue = mqd->msgQueue;

    vPortFree(mqd);

    GATE_ENTER(mqGate)
    msgQueue->refCount--;
    GATE_LEAVE(mqGate)

    return (0);
}

/*
 *  ======== mq_getattr ========
 */
int mq_getattr(mqd_t mqdes, struct mq_attr *mqstat)
{
    MQueueDesc *mqd = (MQueueDesc *)mqdes;
    MQueueObj  *msgQueue = mqd->msgQueue;

    /* Get the number of messages in the queue */
    if (HwiP_inISR()) {
        msgQueue->attrs.mq_curmsgs =
                uxQueueMessagesWaitingFromISR(msgQueue->queue);
    }
    else {
        msgQueue->attrs.mq_curmsgs = uxQueueMessagesWaiting(msgQueue->queue);
    }

    *mqstat = msgQueue->attrs;
    mqstat->mq_flags = mqd->flags;

    return (0);
}

/*
 *  ======== mq_open ========
 */
mqd_t mq_open(const char *name, int oflags, ...)
{
    va_list             va;
    mode_t              mode;
    struct mq_attr     *attrs = NULL;
    MQueueObj          *msgQueue;
    MQueueDesc         *msgQueueDesc = NULL;
    bool                schedulerStarted;
    bool                rc = false;

    /* create gate, this is never deleted */
    if (mqGate == NULL) {
        schedulerStarted = (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING);

        if (schedulerStarted) {
            vTaskSuspendAll();
        }
        if (mqGate == NULL) {
            rc = ((mqGate = xSemaphoreCreateMutex()) == NULL);
        }
        if (schedulerStarted) {
            xTaskResumeAll();
        }
        if (rc) {
            errno = ENOMEM;
            return ((mqd_t)(-1));
        }
    }

    va_start(va, oflags);

    if (oflags & O_CREAT) {
        mode = va_arg(va, mode_t);
        attrs = va_arg(va, struct mq_attr *);

        if (attrs == NULL) {
            attrs = &defaultAttrs;
        }
    }

    va_end(va);

    if (name == NULL) {
        errno = EINVAL;
        return ((mqd_t)(-1));
    }

    if ((oflags & O_CREAT) && ((attrs->mq_maxmsg <= 0)
            || (attrs->mq_msgsize <= 0))) {
        errno = EINVAL;
        return ((mqd_t)(-1));
    }

    GATE_ENTER(mqGate)

    msgQueue = findInList(name);

    if ((msgQueue != NULL) && (oflags & O_CREAT) && (oflags & O_EXCL)) {
        /* Error: Message queue has already been opened and O_EXCL is set */
        errno = EEXIST;
        goto error_leave;
    }

    if (!(oflags & O_CREAT) && (msgQueue == NULL)) {
        errno = ENOENT;
        goto error_leave;
    }

    /* allocate the MQueueDesc */
    msgQueueDesc = pvPortMalloc(sizeof(MQueueDesc));

    if (msgQueueDesc == NULL) {
        errno = ENOMEM;
        goto error_leave;
    }

    if (msgQueue == NULL) {
        /* allocate the MQueueObj */
        msgQueue = pvPortMalloc(sizeof(MQueueObj));

        if (msgQueue == NULL) {
            goto error_handler;
        }

        msgQueue->refCount = 1;
        msgQueue->attrs.mq_msgsize = attrs->mq_msgsize;
        msgQueue->attrs.mq_maxmsg = attrs->mq_maxmsg;

        msgQueue->name = pvPortMalloc(strlen(name) + 1);

        if (msgQueue->name == NULL) {
            goto error_handler;
        }

        strcpy(msgQueue->name, name);

        msgQueue->queue = xQueueCreate((UBaseType_t)attrs->mq_maxmsg,
                (UBaseType_t)attrs->mq_msgsize);

        if (msgQueue->queue == NULL) {
            goto error_handler;
        }

        /* add the message queue to the list now */
        msgQueue->prev = NULL;

        if (mqList != NULL) {
            mqList->prev = msgQueue;
        }

        msgQueue->next = mqList;
        mqList = msgQueue;
    }
    else {
        msgQueue->refCount++;
    }

    GATE_LEAVE(mqGate)

    msgQueueDesc->msgQueue = msgQueue;
    msgQueueDesc->flags = ((oflags & O_NONBLOCK) ? O_NONBLOCK : 0);

    (void)mode;

    return ((mqd_t)msgQueueDesc);

error_handler:
    /*  We only get here if we attempted to allocate msgQueue (i.e., it
     *  was not already in the list), so we're ok to free it.
     */
    if (msgQueue != NULL) {
        if (msgQueue->name != NULL) {
            vPortFree(msgQueue->name);
        }
        vPortFree(msgQueue);
    }
    if (msgQueueDesc != NULL) {
        vPortFree(msgQueueDesc);
    }
    errno = ENOMEM;

error_leave:
    GATE_LEAVE(mqGate)
    return ((mqd_t)(-1));
}

/*
 *  ======== mq_receive ========
 */
ssize_t mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len,
        unsigned int *msg_prio)
{
    MQueueDesc *mqd = (MQueueDesc *)mqdes;
    MQueueObj  *msgQueue = mqd->msgQueue;
    BaseType_t  status;
    TickType_t  timeout;
    int         msgsize = msgQueue->attrs.mq_msgsize;

    /*
     *  If msg_len is less than the message size attribute of the message
     *  queue, return an error.
     */
    if (msg_len < (size_t)((msgQueue->attrs).mq_msgsize)) {
        errno = EMSGSIZE;
        return (-1);
    }

    /*
     *  If the message queue is non-blocking, return an error (-1) if
     *  no message is available.
     *  Otherwise, wait forever to receive a message.
     */
    if (mqd->flags & O_NONBLOCK) {
        timeout = 0;
    }
    else {
        timeout = portMAX_DELAY;
    }

    if (HwiP_inISR()) {
        status = xQueueReceiveFromISR(msgQueue->queue, msg_ptr, NULL);
    }
    else {
        status = xQueueReceive(msgQueue->queue, msg_ptr, timeout);
    }

    if (status != pdTRUE) {
        errno = EAGAIN;
        return (-1);
    }

    return (msgsize);
}

/*
 *  ======== mq_send ========
 */
int mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len,
        unsigned int msg_prio)
{
    MQueueDesc *mqd = (MQueueDesc *)mqdes;
    MQueueObj  *msgQueue = mqd->msgQueue;
    BaseType_t  status;
    TickType_t  timeout;

    if (msg_len > (size_t)(msgQueue->attrs.mq_msgsize)) {
        errno = EMSGSIZE;
        return (-1);
    }

    /*
     *  If O_NONBLOCK is not set, block until space is available in the
     *  queue.  Otherwise, return -1 if no space is available.
     */
    if (mqd->flags & O_NONBLOCK) {
        timeout = 0;
    }
    else {
        timeout = portMAX_DELAY;
    }

    if (HwiP_inISR()) {
        status = xQueueSendFromISR(msgQueue->queue, msg_ptr, NULL);
    }
    else {
        status = xQueueSend(msgQueue->queue, msg_ptr, timeout);
    }

    if (status != pdTRUE) {
        errno = EAGAIN;
        return (-1);
    }

    return (0);
}

/*
 *  ======== mq_setattr ========
 */
int mq_setattr(mqd_t mqdes, const struct mq_attr *newattr,
        struct mq_attr *oldattr)
{
    MQueueDesc *mqd = (MQueueDesc *)mqdes;
    MQueueObj  *msgQueue = mqd->msgQueue;

    /* mq_flags should be 0 or O_NONBLOCK */
    if ((newattr->mq_flags != 0) && (newattr->mq_flags != O_NONBLOCK)) {
        errno = EINVAL;
        return (-1);
    }

    /* save old attribute values before updating message queue description */
    if (oldattr != NULL) {
        msgQueue->attrs.mq_curmsgs = uxQueueMessagesWaiting(msgQueue->queue);
        *oldattr = msgQueue->attrs;
        oldattr->mq_flags = mqd->flags; /* overwrite with mqueue desc flag */
    }

    /*  Only mq_flags is used to set the message queue description, the
     *  remaining members of mq_attr structure are ignored.
     */
    mqd->flags = newattr->mq_flags;

    return (0);
}

/*
 *  ======== mq_timedreceive ========
 *  Receive the oldest of the highest priority messages from the message
 *  queue.
 *  If O_NONBLOCK was not specified when the message queue was opened,
 *  and no message exists on the queue, the wait will end when the
 *  timeout expires. If O_NONBLOCK is set, this function returns immediately
 *  (returning -1 if no message exists).
 *
 *  The timeout expires when the absolute time specified by abs_timeout
 *  passes, as measured by the clock on which timeouts are based (that is,
 *  when the value of that clock equals or exceeds abs_timeout), or if the
 *  absolute time specified by abs_timeout has already been passed at the
 *  time of the call.
 *
 *  If the Timers option is supported, the timeout shall be based on the
 *  CLOCK_REALTIME clock; if the Timers option is not supported, the timeout
 *  shall be based on the system clock as returned by the time() function.
 *
 *  The validity of the abs_timeout parameter need not be checked if a
 *  message can be removed from the message queue immediately.
 */
ssize_t mq_timedreceive(mqd_t mqdes, char *msg_ptr, size_t msg_len,
        unsigned int *msg_prio, const struct timespec *abstime)
{
    MQueueDesc *mqd = (MQueueDesc *)mqdes;
    MQueueObj  *msgQueue = mqd->msgQueue;
    uint32_t    timeout;
    BaseType_t  status;
    int         msgsize = msgQueue->attrs.mq_msgsize;

    /*
     *  If msg_len is less than the message size attribute of the message
     *  queue, return an error.
     */
    if (msg_len < (size_t)(msgQueue->attrs.mq_msgsize)) {
        errno = EMSGSIZE;
        return (-1);
    }

    /* should not fail if message already available (don't validate abstime) */
    if (HwiP_inISR()) {
        status = xQueueReceiveFromISR(msgQueue->queue, msg_ptr, NULL);
    }
    else {
        status = xQueueReceive(msgQueue->queue, msg_ptr, 0);
    }

    if (status == pdTRUE) {
        return (msgsize);
    }
    else {
        /* message queue is empty */
        if (HwiP_inISR() || (mqd->flags & O_NONBLOCK)) {
            errno = EAGAIN;
            return (-1);
        }
    }

    if (_clock_abstime2ticks(CLOCK_REALTIME, abstime, &timeout) != 0) {
        errno = EINVAL;
        return (-1);
    }

    /* requested abstime has already passed */
    if (timeout == 0) {
        errno = ETIMEDOUT;
        return (-1);
    }

    status = xQueueReceive(msgQueue->queue, msg_ptr, timeout);

    if (status != pdTRUE) {
        errno = ETIMEDOUT;
        return (-1);
    }

    return (msgsize);
}

/*
 *  ======== mq_timedsend ========
 *  Add a message to the message queue.
 *  If the message queue is full and O_NONBLOCK is not set, the wait for
 *  sufficient room in the queue is terminated when the specified
 *  timeout expires. If the message queue has O_NONBLOCK set,
 *  this function is equivalent to mq_send() (returns error if no space
 *  in queue).
 *
 *  The timeout shall expire when the absolute time specified by abstime
 *  passes, as measured by the clock on which timeouts are based (that is,
 *  when the value of that clock equals or exceeds abstime), or if the
 *  absolute time specified by abstime has already been passed at the time
 *  of the call.
 *
 *  The timeout shall be based on the CLOCK_REALTIME clock. The resolution
 *  of the timeout shall be the resolution of the clock on which it is based.
 *  The timespec argument is defined in the <time.h> header.
 *
 *  The validity of the abstime parameter need not be checked when there
 *  is sufficient room in the queue.
 */
int mq_timedsend(mqd_t mqdes, const char *msg_ptr, size_t msg_len,
        unsigned int msg_prio, const struct timespec *abstime)
{
    MQueueDesc *mqd = (MQueueDesc *)mqdes;
    MQueueObj  *msgQueue = mqd->msgQueue;
    uint32_t    timeout;
    BaseType_t  status;

    if (msg_len > (size_t)(msgQueue->attrs.mq_msgsize)) {
        errno = EMSGSIZE;
        return (-1);
    }

    /* should not fail if able to send message (don't validate abstime) */
    if (HwiP_inISR()) {
        status = xQueueSendFromISR(msgQueue->queue, msg_ptr, NULL);
    }
    else {
        status = xQueueSend(msgQueue->queue, msg_ptr, 0);
    }

    if (status == pdTRUE) {
        return (0);
    }
    else {
        if (HwiP_inISR() || (mqd->flags & O_NONBLOCK)) {
            errno = EAGAIN;
            return (-1);
        }
    }

    if (_clock_abstime2ticks(CLOCK_REALTIME, abstime, &timeout) != 0) {
        errno = EINVAL;
        return (-1);
    }

    /* requested abstime has already passed */
    if (timeout == 0) {
        errno = ETIMEDOUT;
        return (-1);
    }

    status = xQueueSend(msgQueue->queue, msg_ptr, timeout);

    if (status != pdTRUE) {
        errno = ETIMEDOUT;
        return (-1);
    }

    return (0);
}

/*
 *  ======== mq_unlink ========
 */
int mq_unlink(const char *name)
{
    MQueueObj  *msgQueue;
    MQueueObj  *nextMQ, *prevMQ;

    GATE_ENTER(mqGate)

    msgQueue = findInList(name);

    if (msgQueue == NULL) {
        errno = ENOENT;
        goto done_leave;
    }

    if (msgQueue->refCount == 0) {
        /* remove message queue from list */
        if (mqList == msgQueue) {
            mqList = msgQueue->next;
        }
        else {
            prevMQ = msgQueue->prev;
            nextMQ = msgQueue->next;

            if (prevMQ != NULL) {
                prevMQ->next = nextMQ;
            }
            if (nextMQ != NULL) {
                nextMQ->prev = prevMQ;
            }
        }

        msgQueue->next = msgQueue->prev = NULL;

        GATE_LEAVE(mqGate)

        if (msgQueue->queue != NULL) {
            vQueueDelete(msgQueue->queue);
        }

        if (msgQueue->name != NULL) {
            vPortFree(msgQueue->name);
        }

        vPortFree(msgQueue);

        return (0);
    }
    else {
        /* temporary fix until TIRTOS-1323 is fixed */
        errno = EBUSY;
    }

done_leave:
    GATE_LEAVE(mqGate)
    return (-1);
}

/*
 *************************************************************************
 *                      Internal functions
 *************************************************************************
 */

/*
 *  ======== findInList ========
 *  Look for the given name in the list of message queues
 *
 *  This function must be called inside a gate which protects
 *  the message queue list.
 */
static MQueueObj *findInList(const char *name)
{
    MQueueObj *mq;

    mq = mqList;

    while (mq != NULL) {
        if (strcmp(mq->name, name) == 0) {
            return (mq);
        }
        mq = mq->next;
    }

    return (NULL);
}
