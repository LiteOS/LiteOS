/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: LiteOS Mqueue Module HeadFile
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

/**
 * @defgroup mqueue Message queue
 * @ingroup posix
 */

#ifndef _HWLITEOS_POSIX_MQUEUE_H
#define _HWLITEOS_POSIX_MQUEUE_H

#include "stdarg.h"
#include "stdlib.h"
#include "limits.h"
#include "time.h"
#include "los_typedef.h"
#include "los_queue_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup mqueue
 * Maximum number of messages in a message queue
 */
#define MQ_MAX_MSG_NUM    16

/**
 * @ingroup mqueue
 * Maximum size of a single message in a message queue
 */
#define MQ_MAX_MSG_LEN    64

#define MQ_USE_MAGIC  0x89abcdef

struct mqarray {
    UINT32 mq_id;
    LosQueueCB *mqcb;
    struct mqpersonal *mq_personal;
    char mq_name[PATH_MAX];
    BOOL unlinkflag;
};

struct mqpersonal {
    struct mqarray *mq_posixdes;
    struct mqpersonal *mq_next;
    int mq_flags;
    UINT32 mq_status;
};

/**
 * @ingroup mqueue
 * Message queue attribute structure
 */
struct mq_attr {
    long mq_flags;    /**< Message queue flags */
    long mq_maxmsg;   /**< Maximum number of messages */
    long mq_msgsize;  /**< Maximum size of a message */
    long mq_curmsgs;  /**< Number of messages in the current message queue */
};

/**
 * @ingroup mqueue
 * Handle type of a message queue
 */
typedef UINTPTR   mqd_t;

/**
 * @ingroup mqueue
 *
 * @par Description:
 * This API is used to open an existed message queue that has a specified name or create a new message queue.
 * @attention
 * <ul>
 * <li>A message queue does not restrict the read and write permissions.</li>
 * <li>The length of mqueue name must less than 256.</li>
 * <li>This operation and closed mqueue scheduling must be used in coordination to release the resource.</li>
 * <li>The parameter "mode" is not supported.</li>
 * <li>The "mq_curmsgs" member of the mq_attr structure is not supported.</li>
 * </ul>
 *
 * @param mqName     [IN] Message queue name.
 * @param openFlag   [IN] Permission attributes of the message queue. The value range is
 *                        [O_RDONLY, O_WRONLY, O_RDWR, O_CREAT, O_EXCL, O_NONBLOCK].
 * @param mode       [IN] Message queue mode (variadic argument). When oflag is O_CREAT, it requires
 *                        two additional arguments: mode, which shall be of type mode_t, and attr,
 *                        which shall be a pointer to an mq_attr structure.
 * @param attr       [IN] Message queue attribute (variadic argument).
 *
 * @retval  mqd_t  The message queue is successfully opened or created.
 * @retval  (mqd_t)-1 The message queue fails to be opened or created, with any of the following error codes in errno.
 * @par Errors
 * <ul>
 * <li><b>ENOENT</b>: O_CREAT flag is not set for oflag, and the message queue specified by name does not exist.</li>
 * <li><b>EEXIST</b>: Both O_CREAT and O_EXCL are set for oflag, but the message queue
 *                    specified by name already exists.</li>
 * <li><b>EINVAL</b>: invalid parameter.</li>
 * <li><b>ENFILE</b>: The number of opened message queues exceeds the maximum limit.</li>
 * <li><b>ENOSPC</b>: insufficient memory.</li>
 * <li><b>ENAMETOOLONG</b>: The message queue name specified by name is too long.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>mqueue.h</li></ul>
 * @see mq_close
 * @since Huawei LiteOS V100R001C00
 */
extern mqd_t mq_open(const char *mqName, int openFlag, ...);

/**
 * @ingroup mqueue
 *
 * @par Description:
 * This API is used to close a message queue that has a specified descriptor.
 * @attention
 * <ul>
 * <li> If the message queue is empty, it will be reclaimed, which is similar to when mq_unlink is called.</li>
 * </ul>
 *
 * @param personal   [IN] Message queue descriptor.
 *
 * @retval  0    The message queue is successfully closed.
 * @retval -1    The message queue fails to be closed, with either of the following error codes in errno.
 *
 * @par Errors
 * <ul>
 * <li><b>EBADF</b>: Invalid message queue descriptor.</li>
 * <li><b>EAGAIN</b>: Failed to delete the message queue.</li>
 * <li><b>EFAULT</b>: Failed to free the message queue.</li>
 * <li><b>EINVAL</b>: Invalid parameter.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>mqueue.h</li></ul>
 * @see mq_open
 * @since Huawei LiteOS V100R001C00
 */
extern int mq_close(mqd_t personal);

/**
 * @ingroup mqueue
 *
 * @par Description:
 * This API is used to remove a message queue that has a specified name.
 * @attention
 * <ul>
 * <li> If the message queue is empty, it will be reclaimed, which is similar to when mq_close is called.</li>
 * <li> The length of mqueue name must less than 256.</li>
 * </ul>
 *
 * @param mqName   [IN] Message queue name.
 *
 * @retval  0    The message queue is successfully removed.
 * @retval -1    The message queue fails to be removed, with any of the following error codes in errno.
 *
 * @par Errors
 * <ul>
 * <li><b>ENOENT</b>: The message queue specified by name does not exist.</li>
 * <li><b>EAGAIN</b>: Failed to delete the message queue.</li>
 * <li><b>EBUSY</b>: The message queue to be removed is being used.</li>
 * <li><b>EINVAL</b>: Invalid parameter.</li>
 * <li><b>ENAMETOOLONG</b>: The name of mqueue is too long.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>mqueue.h</li></ul>
 * @see mq_close
 * @since Huawei LiteOS V100R001C00
 */
extern int mq_unlink(const char *mqName);

/**
 * @ingroup mqueue
 *
 * @par Description:
 * This API is used to put a message with specified message content and length into
 * a message queue that has a specified descriptor.
 * @attention
 * <ul>
 * <li> Priority-based message processing is not supported.</li>
 * <li> The msg_len should be same to the length of string which msg_ptr point to.</li>
 * </ul>
 *
 * @param personal   [IN] Message queue descriptor.
 * @param msg        [IN] Pointer to the message content to be sent.
 * @param msgLen     [IN] Length of the message to be sent.
 * @param msgPrio    [IN] Priority of the message to be sent (the value of this parameter must
 *                        be 0 because priority-based message sending is not supported. If the
 *                        value is not 0, this API will cease to work.)
 *
 * @retval  0    The message is successfully sent.
 * @retval -1    The message fails to be sent, with any of the following error codes in errno.
 *
 * @par Errors
 * <ul>
 * <li><b>EINTR</b>: An interrupt is in progress while the message is being sent.</li>
 * <li><b>EBADF</b>: The message queue is invalid or not writable.</li>
 * <li><b>EAGAIN</b>: The message queue is full.</li>
 * <li><b>EINVAL</b>: Invalid parameter.</li>
 * <li><b>ENOSPC</b>: Insufficient memory.</li>
 * <li><b>EMSGSIZE</b>: The message to be sent is too long.</li>
 * <li><b>EOPNOTSUPP</b>: The operation is not supported.</li>
 * <li><b>ETIMEDOUT</b>: The operation times out.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>mqueue.h</li></ul>
 * @see mq_receive
 * @since Huawei LiteOS V100R001C00
 */
extern int mq_send(mqd_t personal, const char *msg, size_t msgLen, unsigned int msgPrio);

/**
 * @ingroup mqueue
 *
 * @par Description:
 * This API is used to remove the oldest message from the message queue that has a specified descriptor,
 * and puts it in the buffer pointed to by msg_ptr.
 * @attention
 * <ul>
 * <li> Priority-based message processing is not supported.</li>
 * <li> The msg_len should be same to the length of string which msg_ptr point to.</li>
 * </ul>
 *
 * @param personal   [IN] Message queue descriptor.
 * @param msg        [IN] Pointer to the message content to be received.
 * @param msgLen     [IN] Length of the message to be received.
 * @param msgPrio    [OUT] Priority of the message to be received
 *                         because priority-based message processing is not supported, this parameter is useless).
 *
 * @retval  0    The message is successfully received.
 * @retval -1    The message fails to be received, with any of the following error codes in the errno.
 *
 * @par Errors
 * <ul>
 * <li><b>EINTR</b>: An interrupt is in progress while the message is being received.</li>
 * <li><b>EBADF</b>: The message queue is invalid or not readable.</li>
 * <li><b>EAGAIN</b>: The message queue is empty.</li>
 * <li><b>EINVAL</b>: invalid parameter.</li>
 * <li><b>EMSGSIZE</b>: The message to be received is too long.</li>
 * <li><b>ETIMEDOUT</b>: The operaton times out.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>mqueue.h</li></ul>
 * @see mq_send
 * @since Huawei LiteOS V100R001C00
 */
extern ssize_t mq_receive(mqd_t personal, char *msg, size_t msgLen, unsigned int *msgPrio);

/**
 * @ingroup mqueue
 *
 * @par Description:
 * This API is used to modify attributes of the message queue that has a specified descriptor.
 * @attention
 * <ul>
 * <li> The mq_maxmsg, mq_msgsize, and mq_curmsgs attributes are not modified
 *      in the message queue attribute setting.</li>
 * </ul>
 *
 * @param personal    [IN] Message queue descriptor.
 * @param mqSetAttr   [IN] New attribute of the message queue.
 * @param MqOldAttr   [OUT] Old attribute of the message queue.
 *
 * @retval   0    The message queue attributes are successfully set.
 * @retval   -1   The message queue attributes fail to be set, with either of the following error codes in the errno.
 *
 * @par Errors
 * <ul>
 * <li><b>EBADF</b>: Invalid message queue.</li>
 * <li><b>EINVAL</b>: Invalid parameter.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>mqueue.h</li></ul>
 * @see mq_getattr
 * @since Huawei LiteOS V100R001C00
 */
extern int mq_setattr(mqd_t personal, const struct mq_attr *mqSetAttr, struct mq_attr *MqOldAttr);

/**
 * @ingroup mqueue
 *
 * @par Description:
 * This API is used to obtain attributes of the message queue that has a specified descriptor.
 * @attention
 * <ul>
 * <li> None. </li>
 * </ul>
 *
 * @param personal    [IN] Message queue descriptor.
 * @param mqAttr      [OUT] Message queue attribute.
 *
 * @retval   0   The message queue attributes are successfully obtained.
 * @retval  -1   The message queue attributes fail to be obtained, with any of the following error codes in errno.
 *
 * @par Errors
 * <ul>
 * <li><b>EBADF</b>: Invalid message queue.</li>
 * <li><b>EINVAL</b>: Invalid parameter.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>mqueue.h</li></ul>
 * @see mq_setattr
 * @since Huawei LiteOS V100R001C00
 */
extern int mq_getattr(mqd_t personal, struct mq_attr *mqAttr);

/**
 * @ingroup mqueue
 *
 * @par Description:
 * This API is used to put a message with specified message content and length into
 * a message queue that has a descriptor at a scheduled time.
 * @attention
 * <ul>
 * <li> Priority-based message processing is not supported.</li>
 * <li> The expiry time must be later than the current time.</li>
 * <li> The wait time is a relative time.</li>
 * <li> The msg_len should be same to the length of string which msg_ptr point to.</li>
 * </ul>
 *
 * @param mqdes           [IN] Message queue descriptor.
 * @param msg             [IN] Pointer to the message content to be sent.
 * @param msgLen          [IN] Length of the message to be sent.
 * @param msgPrio         [IN] Priority of the message to be sent (the value of this parameter must be 0
 *                             because priority-based message processing is not supported).
 * @param absTimeout      [IN] Scheduled time at which the message will be sent. If the value is 0,
 *                             the message is an instant message.
 *
 * @retval   0   The message is successfully sent.
 * @retval  -1   The message fails to be sent, with any of the following error codes in errno.
 *
 * @par Errors
 * <ul>
 * <li><b>EINTR</b>: An interrupt is in progress while the message is being sent.</li>
 * <li><b>EBADF</b>: The message queue is invalid or not writable.</li>
 * <li><b>EAGAIN</b>: The message queue is full.</li>
 * <li><b>EINVAL</b>: Invalid parameter.</li>
 * <li><b>EMSGSIZE</b>: The message to be sent is too long.</li>
 * <li><b>EOPNOTSUPP</b>: The operation is not supported.</li>
 * <li><b>ETIMEDOUT</b>: The operation times out.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>mqueue.h</li></ul>
 * @see mq_receive
 * @since Huawei LiteOS V100R001C00
 */
extern int mq_timedsend(mqd_t personal, const char *msg, size_t msgLen,
                        unsigned int msgPrio, const struct timespec *absTimeout);

/**
 * @ingroup mqueue
 *
 * @par Description:
 * This API is used to obtain a message with specified message content and length from
 * a message queue message that has a specified descriptor.
 * @attention
 * <ul>
 * <li> Priority-based message processing is not supported.</li>
 * <li> The expiry time must be later than the current time.</li>
 * <li> The wait time is a relative time.</li>
 * <li> The msg_len should be same to the length of string which msg_ptr point to.</li>
 * </ul>
 *
 * @param personal        [IN] Message queue descriptor.
 * @param msg             [IN] Pointer to the message content to be received.
 * @param msgLen          [IN] Length of the message to be received.
 * @param msgPrio         [OUT] Priority of the message to be received (because priority-based message
 *                              processing is not supported, this parameter is useless ).
 * @param absTimeout      [IN] Scheduled time at which the messagewill be received. If the value is 0,
 *                             the message is an instant message.
 *
 * @retval  0    The message is successfully received.
 * @retval -1    The message fails to be received, with any of the following error codes in errno.
 *
 * @par Errors
 * <ul>
 * <li><b>EINTR</b>: An interrupt is in progress while the message is being received.</li>
 * <li><b>EBADF</b>: The message queue is invalid or not readable.</li>
 * <li><b>EAGAIN</b>: The message queue is empty.</li>
 * <li><b>EINVAL</b>: invalid parameter.</li>
 * <li><b>EMSGSIZE</b>: The message to be received is too long.</li>
 * <li><b>ETIMEDOUT</b>: The operation times out.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>mqueue.h</li></ul>
 * @see mq_send
 * @since Huawei LiteOS V100R001C00
 */
extern ssize_t mq_timedreceive(mqd_t personal, char *msg, size_t msgLen,
                               unsigned int *msgPrio, const struct timespec *absTimeout);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _HWLITEOS_POSIX_MQUEUE_H */
