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

#include <xdc/std.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/IHeap.h>
#include <xdc/runtime/Memory.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/gates/GateMutex.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Mailbox.h>
#include <ti/sysbios/knl/Task.h>

#include <pthread.h>
#include <mqueue.h>
#include <errno.h>

#include "pthread_util.h"

/*  Gate protection is not needed in boot thread (i.e. from main()).
 *  Check current thread type and only use the gate when running in
 *  task context. These macros improves the code presentation.
 */
#define GATE_ENTER(gate, key, use_gate) \
    if (use_gate) {                     \
        key = GateMutex_enter(gate);    \
    }

#define GATE_LEAVE(gate, key, use_gate) \
    if (use_gate) {                     \
        GateMutex_leave(gate, key);     \
    }

#define GATE_INIT(use_gate) \
    use_gate = (BIOS_getThreadType() == BIOS_ThreadType_Task);


/* this instance was created in Settings.xs */
extern const ti_sysbios_gates_GateMutex_Handle tiposix_mqGate;


/*
 *  ======== MQueueObj ========
 *  The message queue object, created the first time the message queue is
 *  opened.
 */
typedef struct MQueueObj {
    struct MQueueObj  *next;
    struct MQueueObj  *prev;
    Mailbox_Handle     mailbox;
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

static MQueueObj *findInList(const char *name);

static MQueueObj *mqList = NULL;

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
    IArg key;
    bool use_gate;

    GATE_INIT(use_gate)
    Memory_free(Task_Object_heap(), mqd, sizeof(MQueueDesc));

    GATE_ENTER(tiposix_mqGate, key, use_gate)
    msgQueue->refCount--;
    GATE_LEAVE(tiposix_mqGate, key, use_gate)

    return (0);
}

/*
 *  ======== mq_getattr ========
 */
int mq_getattr(mqd_t mqdes, struct mq_attr *mqstat)
{
    MQueueDesc *mqd = (MQueueDesc *)mqdes;
    MQueueObj  *msgQueue = mqd->msgQueue;

    msgQueue->attrs.mq_curmsgs = Mailbox_getNumPendingMsgs(msgQueue->mailbox);

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
    Error_Block         eb;
    IArg                key;
    IHeap_Handle        heap = Task_Object_heap();
    size_t              nameLen;
    bool use_gate;

    GATE_INIT(use_gate)

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
    nameLen = strlen(name);

    if ((oflags & O_CREAT) && ((attrs->mq_maxmsg <= 0)
            || (attrs->mq_msgsize <= 0))) {
        errno = EINVAL;
        return ((mqd_t)(-1));
    }

    Error_init(&eb);

    GATE_ENTER(tiposix_mqGate, key, use_gate)

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
    msgQueueDesc = (MQueueDesc *)Memory_alloc(heap, sizeof(MQueueDesc), 0, &eb);

    if (msgQueueDesc == NULL) {
        errno = ENOMEM;
        goto error_leave;
    }

    if (msgQueue == NULL) {
        /* allocate the MQueueObj */
        msgQueue = (MQueueObj *)Memory_alloc(heap, sizeof(MQueueObj), 0, &eb);

        if (msgQueue == NULL) {
            goto error_handler;
        }

        msgQueue->refCount = 1;
        msgQueue->attrs.mq_msgsize = attrs->mq_msgsize;
        msgQueue->attrs.mq_maxmsg = attrs->mq_maxmsg;

        msgQueue->name = (char *)Memory_alloc(heap, nameLen + 1, 0, &eb);

        if (msgQueue->name == NULL) {
            goto error_handler;
        }

        strcpy(msgQueue->name, name);

        msgQueue->mailbox = Mailbox_create(attrs->mq_msgsize,
                attrs->mq_maxmsg, NULL, &eb);

        if (msgQueue->mailbox == NULL) {
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

    GATE_LEAVE(tiposix_mqGate, key, use_gate)

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
            Memory_free(heap, msgQueue->name, nameLen + 1);
        }
        Memory_free(heap, msgQueue, sizeof(MQueueObj));
    }
    if (msgQueueDesc != NULL) {
        Memory_free(heap, msgQueueDesc, sizeof(MQueueDesc));
    }
    errno = ENOMEM;

error_leave:
    GATE_LEAVE(tiposix_mqGate, key, use_gate)
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
    UInt32      timeout;
    int         retVal = -1;

    /*
     *  If msg_len is less than the message size attribute of the message
     *  queue, return an error.
     */
    if (msg_len < (size_t)((msgQueue->attrs).mq_msgsize)) {
        errno = EMSGSIZE;
        return (-1);
    }

    if (mqd->flags & O_NONBLOCK) {
        timeout = BIOS_NO_WAIT;
    }
    else {
        timeout = BIOS_WAIT_FOREVER;
    }

    /* Receive a message */
    if (Mailbox_pend(msgQueue->mailbox, (Ptr)msg_ptr, timeout)) {
        retVal = (msgQueue->attrs).mq_msgsize;
    }
    else {
        errno = EAGAIN;
    }

    return (retVal);
}

/*
 *  ======== mq_send ========
 */
int mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len,
        unsigned int msg_prio)
{
    MQueueDesc *mqd = (MQueueDesc *)mqdes;
    MQueueObj  *msgQueue = mqd->msgQueue;
    UInt32 timeout;

    if (msg_len > (size_t)(msgQueue->attrs.mq_msgsize)) {
        errno = EMSGSIZE;
        return (-1);
    }

    if (mqd->flags & O_NONBLOCK) {
        timeout = BIOS_NO_WAIT;
    }
    else {
        timeout = BIOS_WAIT_FOREVER;
    }

    /* Send a message */
    if (!Mailbox_post(msgQueue->mailbox, (Ptr)msg_ptr, timeout)) {
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
        msgQueue->attrs.mq_curmsgs = Mailbox_getNumPendingMsgs(
                msgQueue->mailbox);
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
 */
ssize_t mq_timedreceive(mqd_t mqdes, char *msg_ptr, size_t msg_len,
        unsigned int *msg_prio, const struct timespec *abstime)
{
    MQueueDesc *mqd = (MQueueDesc *)mqdes;
    MQueueObj  *msgQueue = mqd->msgQueue;
    UInt32      timeout;
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
    if (Mailbox_pend(msgQueue->mailbox, (Ptr)msg_ptr, BIOS_NO_WAIT)) {
        return (msgsize);
    }
    else {
        /* message queue is empty */
        if (mqd->flags & O_NONBLOCK) {
            errno = EAGAIN;
            return (-1);
        }
    }

    if (_pthread_abstime2ticks(CLOCK_REALTIME, abstime, &timeout) != 0) {
        errno = EINVAL;
        return (-1);
    }

    /* requested abstime has already passed */
    if (timeout == 0) {
        errno = ETIMEDOUT;
        return (-1);
    }

    if (!Mailbox_pend(msgQueue->mailbox, (Ptr)msg_ptr, timeout)) {
        errno = ETIMEDOUT;
        return (-1);
    }

    return (msgsize);
}

/*
 *  ======== mq_timedsend ========
 */
int mq_timedsend(mqd_t mqdes, const char *msg_ptr, size_t msg_len,
        unsigned int msg_prio, const struct timespec *abstime)
{
    MQueueDesc *mqd = (MQueueDesc *)mqdes;
    MQueueObj  *msgQueue = mqd->msgQueue;
    UInt32      timeout;

    if (msg_len > (size_t)(msgQueue->attrs.mq_msgsize)) {
        errno = EMSGSIZE;
        return (-1);
    }

    /* should not fail if able to send message (don't validate abstime) */
    if (Mailbox_post(msgQueue->mailbox, (Ptr)msg_ptr, BIOS_NO_WAIT)) {
        return (0);
    }
    else {
        /* message queue is full */
        if (mqd->flags & O_NONBLOCK) {
            errno = EAGAIN;
            return (-1);
        }
    }

    if (_pthread_abstime2ticks(CLOCK_REALTIME, abstime, &timeout) != 0) {
        errno = EINVAL;
        return (-1);
    }

    /* requested abstime has already passed */
    if (timeout == 0) {
        errno = ETIMEDOUT;
        return (-1);
    }

    if (!Mailbox_post(msgQueue->mailbox, (Ptr)msg_ptr, timeout)) {
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
    IArg        key;
    bool        use_gate;

    GATE_INIT(use_gate)
    GATE_ENTER(tiposix_mqGate, key, use_gate)

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

        GATE_LEAVE(tiposix_mqGate, key, use_gate)

        if (msgQueue->mailbox != NULL) {
            Mailbox_delete(&msgQueue->mailbox);
        }

        if (msgQueue->name != NULL) {
            Memory_free(Task_Object_heap(), msgQueue->name,
                    strlen(msgQueue->name) + 1);
        }

        Memory_free(Task_Object_heap(), msgQueue, sizeof(MQueueObj));

        return (0);
    }
    else {
        /* temporary fix until TIRTOS-1323 is fixed */
        errno = EBUSY;
    }

done_leave:
    GATE_LEAVE(tiposix_mqGate, key, use_gate)
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
