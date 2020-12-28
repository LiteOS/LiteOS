/**
 * @defgroup poll Poll
 * @ingroup libc
 */

#ifndef	_POLL_H
#define	_POLL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#include <bits/poll.h>

#define POLLIN     0x001
#define POLLPRI    0x002
#define POLLOUT    0x004
#define POLLERR    0x008
#define POLLHUP    0x010
#define POLLNVAL   0x020
#define POLLRDNORM 0x040
#define POLLRDBAND 0x080
#ifndef POLLWRNORM
#define POLLWRNORM 0x100
#define POLLWRBAND 0x200
#endif
#ifndef POLLMSG
#define POLLMSG    0x400
#define POLLRDHUP  0x2000
#endif

typedef unsigned long nfds_t;

#ifndef __LITEOS__
struct pollfd {
	int fd;
	short events;
	short revents;
};
#else
typedef unsigned int pollevent_t;

#include "semaphore.h"
#include "linux/wait.h"

struct pollfd {
	int fd;               /* The descriptor being polled */
	sem_t *sem;           /* Pointer to semaphore used to post output event */
	pollevent_t events;   /* The input event flags */
	pollevent_t revents;  /* The output event flags */
	void *priv;           /* For use by drivers */
};

struct tag_poll_wait_entry;

typedef struct tag_poll_wait_entry* poll_wait_head;

typedef struct tag_poll_table {
	poll_wait_head wait;
	pollevent_t key;
} poll_table;

extern void notify_poll(wait_queue_head_t *);
extern void notify_poll_with_key(wait_queue_head_t *, pollevent_t);
#endif

/**
 * @ingroup  poll
 * @par Description:
 *
 * The poll function provides applications with a mechanism for multiplexing input/output over a set of file descriptors. For each member of the
 * array pointed to by fds, the poll examines the given file descriptor for the event(s) specified in events. The poll identifies those file descriptors
 * on which an application can read or write data, or on which certain events have occurred.\n\n
 *
 * The fds argument specifies the file descriptors to be examined and the events of interest for each file descriptor. It is a pointer to an array with
 * one member for each open file descriptor of interest. The array's members are pollfd structures within which fd specifies an open file descriptor
 * and events and revents are bitmasks constructed by OR'ing a combination of the following event flags:\n
 * POLLIN -- There is data to read.\n
 * POLLOUT -- Writing is now possible, though a write larger that the available space in a socket or pipe will still block (unless O_NONBLOCK is set).\n
 * POLLERR -- Error condition (only returned in revents; ignored in events).\n
 * POLLRDHUP -- Stream socket peer closed connection, or shut down writing half of connection.\n
 * POLLHUP -- Hang up (only returned in revents; ignored in events). Note that when reading from a channel such as a stream socket, this event
 * merely indicates that the peer closed its end of the channel. Subsequent reads from the channel will return 0 (end of file) only after all outstanding
 * data in the channel has been consumed.\n\n
 *
 * @param    fds  [IN/OUT]  The field fd contains a file descriptor for an open file. The field events is an input parameter, a bit mask specifying
 * the events the application is interested in for the file descriptor fd. This field can be specified as zero, in which case the only events that can be
 * returned in revents are POLLHUP and POLLERR. The field revents is an output parameter, filled by LiteOS kernel with the events that actually occurred.
 * The bits returned in revents can include any of those specified in events, or one of the values POLLERR or POLLHUP.
 * @param    nfds   [IN]  the number of items in the fds.
 * @param    timeout  [IN]  the number of milliseconds that the poll should block waiting for a file descriptor to become ready. The call will block until
 * either a file descriptor becomes ready or the timeout expires. If the value of timeout is 0, poll just check the file descriptors and return immediately,
 * even if no file descriptors are ready. If the value of timeout is -1, poll blocks until a requested event occurs.
 *
 * @attention
 * <ul>
 * <li>Unlike Linux or other operation system, LiteOS does not support signal mechanism, so the poll function can't be interrupted by signal.</li>
 * <li>Other events which supported on Linux, such as POLLRDNORM, POLLWRNORM, POLLRDBAND, POLLWRBAND and POLLNVAL, are not supported on LiteOS.</li>
 * <li>On LiteOS, the user must make sure all the file descriptors supplied in fds are valid, else the poll function returns failure immediately. On Linux or
 * other platform, if the file descriptor is negative or invalid, the corresponding events field is ignored and the revents field returns zero.</li>
 * <li>It is recommended that the poll function is preferred than the select as the implementation of select is based on the pool, so the poll function is more efficient.</li>
 * <li>If a file descriptor being monitored by poll is closed in another thread, the result is unspecified. On some UNIX systems, poll unblocks and returns
 * with an indication that the file descriptor is ready. On LiteOS, closing the file descriptor in another thread has no effect on poll.  In summary,
 * any application that relies on a particular behavior in this scenario must be considered buggy.</li>
 * <li>The timeout will be rounded up to the system clock granularity, on LiteOS, it's the system scheduling tick. So the minimal poll timeout is one
 * scheduling tick on LiteOS.</li>
 * <li>LiteOS does not support TCP urgent data(also used as the out-of-band data), so user cannot use the poll function with POLLPRI to examine the TCP urgent data.</li>
 * </ul>
 *
 * @retval    #>=0   On success, a positive number is returned; this is the number of structures which have nonzero revents fields (in other words,
 * those descriptors with events or errors reported).  A value of 0 indicates that the call timed out and no file descriptors were ready.
 * @retval    #-1   On failure, -1 is returned, and errno is set to indicate the error.
 *
 * @par Errors
 * <ul>
 * <li><b>EBADF</b>: One of the file descriptor supplied in fds is invalid.</li>
 * <li><b>EINVAL</b>: The fds is a null pointer.</li>
 * <li><b>ENOMEM</b>: There was no space to allocate file descriptor tables.</li>
 * <li><b>ENOSYS</b>: One or more of the file descriptor sets specified a file descriptor don't support poll operation</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>poll.h</li></ul>
 *
 * @see  accept | connect | select | read | recv | send | write
 *
 * @since Huawei LiteOS V100R001C00
 */
int poll (struct pollfd *, nfds_t, int);

#ifdef _GNU_SOURCE
#define __NEED_time_t
#define __NEED_struct_timespec
#define __NEED_sigset_t
#include <bits/alltypes.h>
int ppoll(struct pollfd *, nfds_t, const struct timespec *, const sigset_t *);
#endif

#if _REDIR_TIME64
#ifdef _GNU_SOURCE
__REDIR(ppoll, __ppoll_time64);
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
