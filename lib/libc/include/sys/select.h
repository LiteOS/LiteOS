/**
 * @defgroup select Select
 * @ingroup libc
 */

#ifndef _SYS_SELECT_H
#define _SYS_SELECT_H
#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#define __NEED_size_t
#define __NEED_time_t
#define __NEED_suseconds_t
#define __NEED_struct_timeval
#define __NEED_struct_timespec
#define __NEED_sigset_t

#include <bits/alltypes.h>

#if defined(__LITEOS__) && defined(LOSCFG_FS_VFS)
#include "vfs_config.h"
#else
#ifndef FD_SETSIZE
#define FD_SETSIZE 1024
#endif
#endif

typedef unsigned long fd_mask;

typedef struct {
	unsigned long fds_bits[FD_SETSIZE / 8 / sizeof(long)];
} fd_set;

#define FD_ZERO(s) do { int __i; unsigned long *__b=(s)->fds_bits; for(__i=sizeof (fd_set)/sizeof (long); __i; __i--) *__b++=0; } while(0)
#define FD_SET(d, s)   ((s)->fds_bits[(d)/(8*sizeof(long))] |= (1UL<<((d)%(8*sizeof(long)))))
#define FD_CLR(d, s)   ((s)->fds_bits[(d)/(8*sizeof(long))] &= ~(1UL<<((d)%(8*sizeof(long)))))
#define FD_ISSET(d, s) !!((s)->fds_bits[(d)/(8*sizeof(long))] & (1UL<<((d)%(8*sizeof(long)))))

/**
 * @ingroup  select
 * @par Description:
 *
 * The select function examines the file descriptor sets whose addresses are passed in the readfds, writefds, and errorfds parameters to see whether
 * some of their descriptors are ready for reading, are ready for writing, or have an exceptional condition pending, respectively. \n\n
 *
 * Upon successful completion, the select function would modify the objects pointed to by the readfds, writefds, and errorfds arguments to indicate
 * which file descriptors are ready for reading, ready for writing, or have an error condition pending, respectively, and the total number of ready descriptors
 * in all the output sets would be returned. For each file descriptor less than nfds, the corresponding bit shall be set upon successful completion if it was
 * set on input and the associated condition is true for that file descriptor. If none of the selected descriptors are ready for the requested operation, the
 * select function shall block until at least one of the requested operations becomes ready, until the timeout occurs. The timeout parameter controls how
 * long the select function shall take before timing out. If the timeout parameter is not a null pointer, it specifies a maximum interval to wait for the selection
 * to complete. If the specified time interval expires without any requested operation becoming ready, the function would return. If the timeout parameter is
 * a null pointer, then the call to select would block indefinitely until at least one descriptor meets the specified criteria.\n\n
 *
 * A descriptor shall be considered ready for reading when a call to an input function with O_NONBLOCK clear would not block, whether or not the function
 * would transfer data successfully. (The function might return data, an end-of-file indication, or an error other than one indicating that it is blocked, and
 * in each of these cases the descriptor shall be considered ready for reading). If the descriptor refers to a socket, the implied input function is the recvmsg
 * function with parameters requesting normal and ancillary data, such that the presence of either type shall cause the socket to be marked as readable.
 * The presence of out-of-band data shall be checked if the socket option SO_OOBINLINE has been enabled, as out-of-band data is enqueued with normal data.
 * If the socket is currently listening, then it shall be marked as readable if an incoming connection request has been received, and a call to the accept function
 * shall complete without blocking.\n\n
 *
 * A descriptor shall be considered ready for writing when a call to an output function with O_NONBLOCK clear would not block, whether or not the function
 * would transfer data successfully. If the descriptor refers to a socket, the implied output function is the sendmsg function supplying an amount of normal
 * data equal to the current value of the SO_SNDLOWAT option for the socket. If a non-blocking call to the connect function has been made for a socket, and
 * the connection attempt has either succeeded or failed leaving a pending error, the socket shall be marked as writable.\n\n
 *
 * If a socket has a pending error, it shall be considered to have an exceptional condition pending. Otherwise, what constitutes an exceptional condition is file
 * type-specific. For a file descriptor for use with a socket, it is protocol-specific except as noted below. For other file types it is implementation-defined. If the
 * operation is meaningless for a particular file type, the select shall indicate that the descriptor is ready for read or write operations, and shall indicate that the
 * descriptor has no exceptional condition pending. A socket shall be considered to have an exceptional condition pending if a receive operation with O_NONBLOCK
 * clear for the open file description and with the MSG_OOB flag set would return out-of-band data without blocking. (It is protocol-specific whether the MSG_OOB
 * flag would be used to read out-of-band data.) A socket shall also be considered to have an exceptional condition pending if an out-of-band data mark is present
 * in the receive queue. Other circumstances under which a socket may be considered to have an exceptional condition pending are implementation-defined. \n\n
 *
 * File descriptors associated with regular files shall always select true for ready to read, ready to write, and error conditions.\n\n
 *
 * If the readfds, writefds, and errorfds arguments are all null pointers and the timeout argument is not a null pointer, the select() function would block for the
 * time specified. If the readfds, writefds, and errorfds arguments are all null pointers and the timeout argument is a null pointer, the select() function returns
 * failure immediately.\n\n
 *
 * File descriptor masks of type fd_set can be initialized and tested with FD_CLR, FD_ISSET, FD_SET, and FD_ZERO. It is unspecified whether each of these
 * is a macro or a function. If a macro definition is suppressed in order to access an actual function, or a program defines an external identifier with any of these
 * names, the behavior is undefined.\n\n
 *
 * FD_CLR(fd, set) shall remove the file descriptor fd from the set pointed to by set. If fd is not a member of this set, there is no effect on the set.
 * FD_ISSET(fd, set) shall evaluate to non-zero if the file descriptor fd is a member of the set pointed to by set, and shall evaluate to zero otherwise.
 * FD_SET(fd, set) shall add the file descriptor fd to the set pointed to by set. If the file descriptor fd is already in this set, there is no effect on the set.
 * FD_ZERO(set) shall initialize the descriptor set pointed to by set to the null set. No error is returned if the set is not empty at the time FD_ZERO() is invoked.\n\n
 *
 * The behavior of these macros is undefined if the fd argument is less than 0 or greater than or equal to FD_SETSIZE, or if fd is not a valid file descriptor,
 * or if any of the arguments are expressions with side-effects.\n\n
 *
 * @param    nfds       [IN]   the range of descriptors to be tested, all descriptors from zero to nfds-1 in the descriptor sets shall be examined.
 * @param    readfds   [IN/OUT]  on input specifies the file descriptors to be checked for being ready to read, and on output indicates which file descriptors are ready to read.
 * @param    writefds  [IN/OUT]  on input specifies the file descriptors to be checked for being ready to write, and on output indicates which file descriptors are ready to write.
 * @param    errorfds  [IN/OUT]  on input specifies the file descriptors to be checked for error conditions pending, and on output indicates which file descriptors have error conditions pending.
 * @param    timeout  [IN]  a maximum interval to wait for the selection to complete.
 *
 * @attention
 * <ul>
 * <li>Unlike Linux or other operation system, LiteOS does not support signal mechanism, so the select function can't be interrupted by signal.</li>
 * <li>Unlike Linux, LiteOS does not modify the timeout argument to reflect the amount of time not slept on successful completion.</li>
 * <li>The select functions support regular files, terminal, device files and sockets. The behavior of select on file descriptors that refer to other types of file is unspecified.</li>
 * <li>If the readfds, writefds, errorfds and timeout are all null, the select function returns immediately. But in Linux or other system, the select function
 * would block until interrupted by a signal.</li>
 * <li>LiteOS don't support TCP urgent data(also used as the out-of-band data), so the caller can not use the select function with errorfds to examine the TCP urgent data.</li>
 * <li>It is recommended that the poll function is preferred than the select as the implementation of select is based on the pool, so the poll function is more efficient.</li>
 * <li>If a file descriptor being monitored by select is closed in another thread, the result is unspecified. On some UNIX systems, select unblocks and returns
 * with  an  indication  that  the  file descriptor is ready. On LiteOS, closing the file descriptor in another thread has no effect on select. In summary,
 * any application that relies on a particular behavior in this scenario must be considered buggy.</li>
 * <li>The timeout will be rounded up to the system clock granularity, on LiteOS, it's the system scheduling tick. So the minimal poll timeout is one
 * scheduling tick on LiteOS.</li>
 * </ul>
 *
 * @retval    #>=0   On success, the total number of bits set in the bit masks is retuened. If timeout expires before anything interesting happens, 0 is returned.
 * @retval    #-1   On failure, -1 is returned, and errno is set to indicate the error, the file descriptor sets and timeout are unmodified.
 * FD_CLR, FD_SET, and FD_ZERO do not return a value. FD_ISSET returns a non-zero value if the bit for the file descriptor fd is set in the file descriptor
 * set pointed to by fdset, and 0 otherwise.\n\n
 *
 * @par Errors
 * <ul>
 * <li><b>EBADF</b>: One or more of the file descriptor sets specified a file descriptor that is not a valid open file descriptor.</li>
 * <li><b>EINVAL</b>: The nfds argument is less than 0 or greater than FD_SETSIZE.</li>
 * <li><b>EINVAL</b>: The readfds, writefds, errorfds and timeout are all null.</li>
 * <li><b>ENOMEM</b>: Unable to allocate memory for internal tables.</li>
 * <li><b>ENOSYS</b>: One or more of the file descriptor sets specified a file descriptor don't support poll operation</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>sys/select.h</li></ul>
 *
 * @see  accept | connect | poll | read | recv | send | write
 *
 * @since Huawei LiteOS V100R001C00
 */
int select (int, fd_set *__restrict, fd_set *__restrict, fd_set *__restrict, struct timeval *__restrict);
int pselect (int, fd_set *__restrict, fd_set *__restrict, fd_set *__restrict, const struct timespec *__restrict, const sigset_t *__restrict);

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#define NFDBITS (8*(int)sizeof(long))
#endif

#if _REDIR_TIME64
__REDIR(select, __select_time64);
__REDIR(pselect, __pselect_time64);
#endif

#ifdef __cplusplus
}
#endif
#endif
