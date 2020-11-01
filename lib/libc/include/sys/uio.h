/**
 * @defgroup uio Uio
 * @ingroup libc
 */

#ifndef _SYS_UIO_H
#define _SYS_UIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#define __NEED_size_t
#define __NEED_ssize_t
#define __NEED_struct_iovec

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#define __NEED_off_t
#endif

#ifdef _GNU_SOURCE
#define __NEED_pid_t
#endif
#ifdef __LITEOS__
#define __NEED_int64_t
#endif

#include <bits/alltypes.h>

#define UIO_MAXIOV 1024

/**
 * @ingroup uio
 *
 * @par Description:
 * The readv() function shall be equivalent to read(), except as described below.
 * The readv() function shall place the input data into the iovcnt buffers specified by the members
 * of the iov array: iov[0], iov[1], ..., iov[iovcnt-1].
 * Each iovec entry specifies the base address and length of an area in memory where data should be placed.
 * The readv() function shall always fill an area completely before proceeding to the next.
 *
 * @param fd      [IN]     File descriptor (or socket descriptor) to read from
 * @param iov     [OUT]    The array that user-provided to save the data
 * @param iovcnt  [IN]     The number of members in the array
 *
 * @attention
 * <ul>
 * <li>If the sum of the iov_len values is greater than {SSIZE_MAX},
 * the operation shall fail and no data shall be transferred.</li>
 * </ul>
 *
 * @retval #ssize_t  A positive non-zero number of bytes read on success.
 * @retval #0        End-of-file or failed in read().
 * @retval #-1       The iov is a null pointer or out of memory.
 *
 * @par Errors
 * <ul>
 * <li><b>EBADF</b>: fd is not a valid open file descriptor, or the file system doesn't support read().</li>
 * <li><b>EACCES</b>: The file is a write-only file.</li>
 * <li><b>EIO</b>: A hard error occurred in the low level disk I/O layer or the physical drive cannot work.</li>
 * <li><b>ENOMEM</b>: Out of memory.</li>
 * <li><b>EINVAL</b>: The sum of the iov_len values is greater than {SSIZE_MAX}</li>
 * <li><b>ENODEV</b>: The mount is not healthy.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>uio.h</li></ul>
 * @see  writev
 * @since Huawei LiteOS V100R001C00
 */
ssize_t readv (int, const struct iovec *, int);

/**
 * @ingroup uio
 *
 * @par Description:
 * The writev() function shall be equivalent to write(), except as described below.
 * The writev() function shall gather output data from the iovcnt buffers specified by the members
 * of the iov array: iov[0], iov[1], ..., iov[iovcnt-1].
 * Each iovec entry specifies the base address and length of an area in memory from which data should be written.
 * The writev() function shall always write a complete area before proceeding to the next.
 *
 * @param fd      [IN] File descriptor (or socket descriptor) to read from
 * @param iov     [IN] The array of data that user-provided to write
 * @param iovcnt  [IN] The number of members in the array
 *
 * @attention
 * <ul>
 * <li>If all of the iov_len members in the array pointed to by iov are 0, it shall return -1.</li>
 * <li>If the sum of the iov_len values is greater than {SSIZE_MAX},
 * the operation shall fail and no data shall be transferred.</li>
 * </ul>
 *
 * @retval #ssize_t  On success, the number of bytes written are returned (zero indicates nothing was written).
 * @retval #-1       On error, -1 is returned, and errno is set appropriately.
 *
 * @par Errors
 * <ul>
 * <li><b>EBADF</b>: fd is not a valid open file descriptor, or the file system doesn't support write().</li>
 * <li><b>EACCES/EROFS</b>: The file is a read-only file or the file system is read-only.</li>
 * <li><b>EIO</b>: A physical I/O error has occurred.</li>
 * <li><b>ENOMEM</b>: Out of memory.</li>
 * <li><b>EINTR</b>: The write operation was terminated due to the receipt of a signal, no data was transferred.</li>
 * <li><b>ENOSPC</b>: There was no free space remaining on the device containing the file.</li>
 * <li><b>EAGAIN</b>: Non-blocking I/O has been selected using O_NONBLOCK and the write would block.</li>
 * <li><b>EFBIG</b>: An attempt was made to write a file that exceeds the implementation defined maximum file
 * size or the process' file size limit, or to write at a position past the maximum allowed offset.</li>
 * <li><b>EINVAL</b>: The current position of file is less than zero,
 * or the sum of the iov_len values is greater than {SSIZE_MAX}.</li>
 * <li><b>ENODEV</b>: The mount is not healthy.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>uio.h</li></ul>
 * @see  readv
 * @since Huawei LiteOS V100R001C00
 */
ssize_t writev (int, const struct iovec *, int);

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
ssize_t preadv (int, const struct iovec *, int, off_t);
ssize_t pwritev (int, const struct iovec *, int, off_t);
#if defined(_LARGEFILE64_SOURCE) || defined(_GNU_SOURCE)
#define preadv64 preadv
#define pwritev64 pwritev
#ifdef __LITEOS__
#define off64_t int64_t
#else
#define off64_t off_t
#endif
#endif
#endif

#ifdef _GNU_SOURCE
ssize_t process_vm_writev(pid_t, const struct iovec *, unsigned long, const struct iovec *, unsigned long, unsigned long);
ssize_t process_vm_readv(pid_t, const struct iovec *, unsigned long, const struct iovec *, unsigned long, unsigned long);
#endif

#ifdef __cplusplus
}
#endif

#endif
