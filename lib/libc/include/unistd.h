/**
 * @defgroup unistd Unistd
 * @ingroup libc
 */

#ifndef	_UNISTD_H
#define	_UNISTD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#ifdef __LITEOS__
#undef NULL
#endif
#ifdef __cplusplus
#define NULL 0L
#else
#define NULL ((void*)0)
#endif

#define __NEED_size_t
#define __NEED_ssize_t
#define __NEED_uid_t
#define __NEED_gid_t
#define __NEED_off_t
#define __NEED_pid_t
#define __NEED_intptr_t
#define __NEED_useconds_t
#ifdef __LITEOS__
#define __NEED_int64_t
#endif

#include <bits/alltypes.h>

int pipe(int [2]);
int pipe2(int [2], int);

/**
 * @ingroup unistd
 *
 * @par Description:
 * The close() function shall close a file descriptor, so that it no longer refers to any file and may be reused.
 * Any record locks held on the file it was associated with, and owned by the process, are removed.
 * If fd is the last copy of a particular file descriptor the resources associated with it are freed;
 * if the descriptor was the last reference to a file which has been removed using unlink() the file is deleted.
 *
 * @param __fd [IN] file descriptor to close.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #0  close file successful.
 * @retval #-1 An error is encountered and close file failed.
 *
 * @par Errors
 * <ul>
 * <li><b>EBADF</b>: The __fd argument is not a open file descriptor, or value is not within valid range.</li>
 * <li><b>ENOENT</b>: Can't find volume in fat or can't find internal node in jffs.</li>
 * <li><b>EIO</b>: A hard error occurred in the low level disk I/O layer or the physical drive cannot work.</li>
 * <li><b>ENODEV</b>: The partition of disk can't find.</li>
 * <li><b>EPERM</b>: Other errors.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>unistd.h</li></ul>
 * @see dup | fclose | fopen | ioctl | open | unlink
 * @since Huawei LiteOS V100R001C00
 */
int close(int);
int posix_close(int, int);

/**
 * @ingroup unistd
 *
 * @par Description:
 * The dup() function shall duplicate an open file descriptor. It provides an alternative interface
 * to the service provided by fcntl() using the F_DUPFD command. The call dup(fildes) shall be equivalent to:
 * fcntl(fildes, F_DUPFD, 0);
 *
 * @param __oldfd [IN] file descriptor to duplicate.
 *
 * @attention
 * <ul>
 * <li>The dup() function doesn't support to duplicate a socket descriptor.</li>
 * </ul>
 *
 * @retval #int  A non-negative integer namely the file descriptor shall be returned upon successful completion.
 * @retval #-1 An error is encountered and duplicate file descriptor failed.
 *
 * @par Errors
 * <ul>
 * <li><b>EBADF</b>: __oldfd is not a valid open file descriptor, or it is a socket descriptor.</li>
 * <li><b>ENOMEM</b>: Out of memory.</li>
 * <li><b>EMFILE</b>: Too many open files and allocate a file descriptor failed.</li>
 * <li><b>ENOENT</b>: Can't find the inode by the pathname associated with __oldfd.</li>
 * <li><b>ENOSYS</b>: The current file system doesn't support the dup() function.</li>
 * <li><b>ENODEV</b>: The mount is not healthy.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>unistd.h</li></ul>
 * @see close | fcntl | open
 * @since Huawei LiteOS V100R001C00
 */
int dup(int);

/**
 * @ingroup unistd
 *
 * @par Description:
 * The dup2() function shall cause the file descriptor __newfd to refer to the same open file description
 * as the file descriptor __oldfd and to share any locks, and shall return __newfd.
 * If __newfd is already a valid open file descriptor, it shall be closed first, unless __oldfd is equal to __newfd
 * in which case dup2() shall return __newfd without closing it.
 *
 * @attention
 * <ul>
 * <li>The dup2() function doesn't support to clone a socket descriptor.</li>
 * <li>If __newfd is already a valid opend descriptor, it shall be closed first, even if the file system doesn't
 * support dup2().</li>
 * </ul>
 *
 * @retval #int  A non-negative integer namely the file descriptor shall be returned upon successful completion.
 * @retval #-1 An error is encountered and duplicate file descriptor failed.
 *
 * @par Errors
 * <ul>
 * <li><b>EBADF</b>: __oldfd is not a valid open file descriptor, or it is a socket descriptor.</li>
 * <li><b>ENOMEM</b>: Out of memory.</li>
 * <li><b>ENOENT</b>: Can't find the inode by the pathname associated with __oldfd.</li>
 * <li><b>EINTR</b>: The dup2() function was interrupted by a signal.</li>
 * <li><b>ENOSYS</b>: The current file system doesn't support the dup() function.</li>
 * <li><b>ENODEV</b>: The mount is not healthy or no such device.</li>
 * <li><b>EIO</b>: A hard error occurred in the low level disk I/O layer or the physical drive cannot work.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>unistd.h</li></ul>
 * @see close | fcntl | open
 * @since Huawei LiteOS V100R001C00
 */
int dup2(int, int);
int dup3(int, int, int);
off_t lseek(int, off_t, int);

/**
 * @ingroup unistd
 *
 * @par Description:
 * The fsync() function shall request that all data for the open file descriptor named by fildes is to
 * be transferred to the storage device associated with the file described by fildes.
 * The nature of the transfer is implementation-defined. The fsync() function shall not return until
 * the system has completed that action or until an error is detected.
 *
 * @param  __fd [IN] An open file descriptor that refers to a device.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The fsync() function shall return 0 upon successful completion.
 * Otherwise, the function shall return -1 and set errno.
 *
 * @par Errors
 * <ul>
 * <li><b>EBADF</b>: The __fd argument is not a valid open file descriptor.</li>
 * <li><b>ENOSYS</b>: The function is not supported.</li>
 * <li><b>EINVAL</b>: The __fd argument is not valid for this device.</li>
 * <li><b>ENOENT</b>: The file can't found.</li>
 * <li><b>ENODEV</b>: The device is not existed.</li>
 * <li><b>EACCES</b>: The file permission is denied.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>unistd.h</li></ul>
 * @see sync
 * @since Huawei LiteOS V100R001C00
 */
int fsync(int);
int fdatasync(int);

/**
 * @ingroup unistd
 *
 * @par Description:
 * The read() function shall attempt to read __count bytes from the file associated with the open file descriptor,
 * __fd, into the buffer pointed to by __buf.
 *
 * @param __fd    [IN]     file descriptor (or socket descriptor) to read from
 * @param __buf   [IN/OUT] user-provided to save the data
 * @param __count [IN]     The maximum size of the user-provided buffer
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #ssize_t  A positive non-zero number of bytes read on success.
 * @retval #0        End-of-file.
 * @retval #-1       On failure with errno set appropriately.
 *
 * @par Errors
 * <ul>
 * <li><b>EBADF</b>: __fd is not a valid open file descriptor, or the file system doesn't support read().</li>
 * <li><b>EFAULT</b>: __buf is outside your accessible address space.</li>
 * <li><b>EACCES</b>: The file is a write-only file.</li>
 * <li><b>EIO</b>: A hard error occurred in the low level disk I/O layer or the physical drive cannot work.</li>
 * <li><b>ENOMEM</b>: Out of memory.</li>
 * <li><b>EINVAL</b>: __count is greater than {YAFFS_MAX_FILE_SIZE} in yaffs filesystem.</li>
 * <li><b>ENODEV</b>: The mount is not healthy.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>unistd.h</li></ul>
 * @see  fcntl | ioctl | lseek | open | readv
 * @since Huawei LiteOS V100R001C00
 */
ssize_t read(int, void *, size_t);

/**
 * @ingroup unistd
 *
 * @par Description:
 * The write() function shall attempt to write __count bytes from the buffer pointed to by __buf to the file
 * associated with the open file descriptor, __fd.
 *
 * @param __fd    [IN] file descriptor (or socket descriptor) to write to
 * @param __buf   [IN] Data to write
 * @param __count [IN] Length of data to write
 *
 * @attention
 * <ul>
 * <li>On a regular file or other file capable of seeking, the actual writing of data shall proceed from the position
 * in the file indicated by the file offset associated with fildes. Before successful return from write(), the file
 * offset shall be incremented by the number of bytes actually written. On a regular file, if the position
 * of the last byte written is greater than or equal to the length of the file, the length of the file shall be
 * set to this position plus one.</li>
 * <li>If the O_APPEND flag of the file status flags is set, the file offset shall be set to the end of the file
 * prior to each write and no intervening file modification operation shall occur between changing the file offset
 * and the write operation.</li>
 * </ul>
 *
 * @retval #ssize_t  On success, the number of bytes  written are returned (zero indicates nothing was written).
 * @retval #-1       On error, -1 is returned, and errno is set appropriately.
 *
 * @par Errors
 * <ul>
 * <li><b>EBADF</b>: __fd is not a valid open file descriptor, or the file system doesn't support write().</li>
 * <li><b>EFAULT</b>: __buf is outside your accessible address space.</li>
 * <li><b>EACCES/EROFS</b>: The file is a read-only file or the file system is read-only.</li>
 * <li><b>EIO</b>: A physical I/O error has occurred.</li>
 * <li><b>ENOMEM</b>: Out of memory.</li>
 * <li><b>EINTR</b>: The write operation was terminated due to the receipt of a signal, no data was transferred.</li>
 * <li><b>ENOSPC</b>: There was no free space remaining on the device containing the file.</li>
 * <li><b>EAGAIN</b>: Non-blocking I/O has been selected using O_NONBLOCK and the write would block.</li>
 * <li><b>EFBIG</b>: An attempt was made to write a file that exceeds the implementation defined maximum file
 * size or the process' file size limit, or to write at a position past the maximum allowed offset.</li>
 * <li><b>EINVAL</b>: The current position of file is less than zero.</li>
 * <li><b>ENODEV</b>: The mount is not healthy.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>unistd.h</li></ul>
 * @see  fcntl | ioctl | lseek | open | readv
 * @since Huawei LiteOS V100R001C00
 */
ssize_t write(int, const void *, size_t);

/**
 * @ingroup unistd
 *
 * @par Description:
 * The pread() function shall be equivalent to read(), except that it shall read from a given position in the file
 * without changing the file offset. The first three arguments to pread() are the same as read() with the addition
 * of a fourth argument __offset for the desired position inside the file. An attempt to perform a pread() on a file
 * that is incapable of seeking shall result in an error.
 *
 * @param __fd     [IN]     file descriptor (or socket descriptor) to read from
 * @param __buf    [IN/OUT] user-provided to save the data
 * @param __count  [IN]     The maximum size of the user-provided buffer
 * @param __offset [IN]     The file offset
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #ssize_t  A positive non-zero number of bytes read on success.
 * @retval #0        End-of-file.
 * @retval #-1       On failure with errno set appropriately.
 *
 * @par Errors
 * <ul>
 * <li><b>EBADF</b>: __fd is not a valid open file descriptor, or the file system doesn't support read().</li>
 * <li><b>EFAULT</b>: __buf is outside your accessible address space.</li>
 * <li><b>EACCES</b>: The file is a write-only file.</li>
 * <li><b>EIO</b>: A hard error occurred in the low level disk I/O layer or the physical drive cannot work.</li>
 * <li><b>ENOMEM</b>: Out of memory.</li>
 * <li><b>EINVAL</b>: __count is greater than {YAFFS_MAX_FILE_SIZE} in yaffs filesystem.</li>
 * <li><b>ENODEV</b>: The mount is not healthy.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>unistd.h</li></ul>
 * @see  fcntl | ioctl | lseek | open | readv
 * @since Huawei LiteOS V100R001C00
 */
ssize_t pread(int, void *, size_t, off_t);

/**
 * @ingroup unistd
 *
 * @par Description:
 * The pwrite() function shall be equivalent to write(), except that it writes into a given position
 * and does not change the file offset (regardless of whether O_APPEND is set). The first three arguments to pwrite()
 * are the same as write() with the addition of a fourth argument __offset for the desired position inside the file.
 * An attempt to perform a pwrite() on a file that is incapable of seeking shall result in an error.
 *
 * @param __fd     [IN] file descriptor (or socket descriptor) to write to
 * @param __buf    [IN] Data to write
 * @param __count  [IN] Length of data to write
 * @param __offset [IN] The file offset to write from
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #ssize_t  On success, the number of bytes  written are returned (zero indicates nothing was written).
 * @retval #-1       On error, -1 is returned, and errno is set appropriately.
 *
 * @par Errors
 * <ul>
 * <li><b>EBADF</b>: __fd is not a valid open file descriptor, or the file system doesn't support write().</li>
 * <li><b>EFAULT</b>: __buf is outside your accessible address space.</li>
 * <li><b>EACCES/EROFS</b>: The file is a read-only file or the file system is read-only.</li>
 * <li><b>EIO</b>: A physical I/O error has occurred.</li>
 * <li><b>ENOMEM</b>: Out of memory.</li>
 * <li><b>EINTR</b>: The write operation was terminated due to the receipt of a signal, no data was transferred.</li>
 * <li><b>ENOSPC</b>: There was no free space remaining on the device containing the file.</li>
 * <li><b>EAGAIN</b>: Non-blocking I/O has been selected using O_NONBLOCK and the write would block.</li>
 * <li><b>EFBIG</b>: An attempt was made to write a file that exceeds the implementation defined maximum file
 * size or the process' file size limit, or to write at a position past the maximum allowed offset.</li>
 * <li><b>EINVAL</b>: The current position of file is less than zero.</li>
 * <li><b>ENODEV</b>: The mount is not healthy.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>unistd.h</li></ul>
 * @see  fcntl | ioctl | lseek | open | readv
 * @since Huawei LiteOS V100R001C00
 */
ssize_t pwrite(int, const void *, size_t, off_t);

int chown(const char *, uid_t, gid_t);
int fchown(int, uid_t, gid_t);
int lchown(const char *, uid_t, gid_t);
int fchownat(int, const char *, uid_t, gid_t, int);

int link(const char *, const char *);
int linkat(int, const char *, int, const char *, int);
int symlink(const char *, const char *);
int symlinkat(const char *, int, const char *);
ssize_t readlink(const char *__restrict, char *__restrict, size_t);
ssize_t readlinkat(int, const char *__restrict, char *__restrict, size_t);

/**
 * @ingroup unistd
 *
 * @par Description:
 * The unlink() function shall remove a link to file. When the file's link count becomes 0 and no process
 * has the file open, the space occupied by the file shall be freed and the file shall no longer be accessible.
 * If one or more processes have the file open when the last link is removed, the link shall be removed before unlink() returns,
 * but the removal of the file contents shall be postponed until all references to the file are closed.
 *
 * @param  __path [IN] path of the file which need to be removed, shall not name a directory.
 *
 * @attention
 * <ul>
 * <li>If the unlink() function is called to remove a file in /dev, it shall just call the registered unlink hook function
 * the file will still alive and won't be removed.</li>
 * </ul>
 *
 * @retval #int Upon successful completion, these functions shall return 0.
 * Otherwise, these functions shall return -1 and set errno to indicate the error.
 * If -1 is returned, the named file shall not be changed.
 *
 *
 * @par Errors
 * <ul>
 * <li><b>EACCES</b>: Search permission is denied for a component of the path prefix,
 * or write permission is denied on the directory containing the directory entry to be removed,
 * or remove a file in /dev which doesn't register the unlink function.</li>
 * <li><b>ENAMETOOLONG</b>: The length of a component of a pathname is longer than {NAME_MAX}.</li>
 * <li><b>ENOTEMPTY</b>: unlink a pseudo-file which is functioning as a directory and the directory is not empty.</li>
 * <li><b>ENOENT</b>: A component of path does not name an existing file.</li>
 * <li><b>EINVAL</b>: The path is not valid.</li>
 * <li><b>ENOMEM</b>: Out of memory.</li>
 * <li><b>EISDIR</b>: The path is a directory.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>unistd.h</li></ul>
 * @see close | remove | rename | rmdir
 * @since Huawei LiteOS V100R001C00
 */
int unlink(const char *);
int unlinkat(int, const char *, int);

/**
 * @ingroup unistd
 *
 * @par Description:
 * The rmdir() function shall remove a directory whose name is given by __path.
 * The directory shall be removed only if it is an empty directory.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #0  Remove directory successful.
 * @retval #-1 An error is encountered and remove the directory failed.
 *
 * @par Errors
 * <ul>
 * <li><b>EINVAL</b>: The path is a null pointer or an empty string.</li>
 * <li><b>ENAMETOOLONG</b>: The length of a component of a pathname is longer than {NAME_MAX}.</li>
 * <li><b>ENOENT</b>: A component of the path does not exist.</li>
 * <li><b>EPERM</b>: The path represent a mount point, or the path is neither a directory nor a file.</li>
 * <li><b>ENOTEMPTY</b>: The path argument names a directory that is not an empty directory.</li>
 * <li><b>ENOTDIR</b>: A component of path names an existing file instead of a directory.</li>
 * <li><b>ENODEV</b>: The mount is not healthy.</li>
 * <li><b>ENOMEM</b>: Out of memory.</li>
 * <li><b>EIO</b>: A hard error occurred in the low level disk I/O layer or the physical drive cannot work.</li>
 * <li><b>EROFS</b>: The physical drive is write protected with fat filesystem.</li>
 * <li><b>EACCES</b>: The directory entry to be removed resides on a read-only file system, or remove a directory
 * in proc.</li>
 * <li><b>EBUSY</b>: Remove a root of file system, or the directory is not empty of ramfs.</li>
 * <li><b>ENOSYS
</b>: The file system doesn't support to rmdir.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>unistd.h</li></ul>
 * @see mkdir | remove | rename | unlink
 * @since Huawei LiteOS V100R001C00
 */
int rmdir(const char *);
int truncate(const char *, off_t);

/**
 * @ingroup unistd
 *
 * @par Description:
 * The ftruncate() function truncates the file size to the length bytes.
 * If  the  file  previously was larger than this size, the extra data is lost.
 * If the file previously was shorter, it is extended, and the extended part reads as disk data.
 * @attention
 * <ul>
 * <li>Now only FAT32 support this function.</li>
 * </ul>
 *
 * @retval #int The ftruncate() function shall return 0 upon successful completion.
 * Otherwise, the function shall return -1 and set errno.
 *
 * @par Errors
 * <ul>
 * <li><b>EBADF</b>: The __fd argument is not a valid open file descriptor.</li>
 * <li><b>ENOSYS</b>: The function is not supported.</li>
 * <li><b>EINVAL</b>: The length argument was less than 0 or
 * The length argument was greater than the maximum file size.</li>
 * <li><b>EIO</b>: A hard error occurred in the low level disk I/O layer or the physical drive cannot work.</li>
 * <li><b>EACCES</b>: The file permission is denied.</li>
 * <li><b>ENODEV</b>: The device is not existed.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>unistd.h</li></ul>
 * @see fallocate
 * @since Huawei LiteOS V100R001C00
 */
int ftruncate(int, off_t);

#define F_OK 0
#define R_OK 4
#define W_OK 2
#define X_OK 1

/**
 * @ingroup unistd
 *
 * @par Description:
 * The access() function shall check the file named by the pathname pointed to by the __path argument
 * for accessibility according to the bit pattern contained in __mode.
 *
 * @param  __path [IN] path of the file which need to be checked.
 * @param  __mode [IN] The value of __mode is either the bitwise-inclusive OR of the access permissions to be checked
 *                     (R_OK, W_OK, X_OK) or the existence test (F_OK).
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The access() function shall return 0 upon successful completion.
 * Otherwise, the function shall return -1 and set errno.
 *
 * @par Errors
 * <ul>
 * <li><b>EACCES</b>: Permission bits of the file mode do not permit the requested access,
 * or search permission is denied on a component of the path prefix.</li>
 * <li><b>ENAMETOOLONG</b>: The length of a component of a pathname is longer than {NAME_MAX}.</li>
 * <li><b>EFAULT</b>: Bad address.</li>
 * <li><b>ENOENT</b>: A component of the path does not exist, or the path is an empty string.</li>
 * <li><b>ENOMEM</b>: Out of memory.</li>
 * <li><b>ENOTDIR</b>: A component of the path is not a directory.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>unistd.h</li></ul>
 * @see None
 * @since Huawei LiteOS V100R001C00
 */
int access(const char *, int);
int faccessat(int, const char *, int, int);

/**
 * @ingroup unistd
 *
 * @par Description:
 * The chdir() function shall cause the directory named by the pathname pointed to by the __path argument to become
 * the current working directory; that is, the starting point for path searches for pathnames not beginning with '/'.
 *
 * @param  __path [IN] path of the file which need to be set as the current working directory.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int Upon successful completion, 0 shall be returned.
 * Otherwise, -1 shall be returned, the current working directory shall remain unchanged,
 * and errno shall be set to indicate the error.
 *
 * @par Errors
 * <ul>
 * <li><b>EINVAL</b>: The path is an empty string.</li>
 * <li><b>ENAMETOOLONG</b>: The length of a component of a pathname is longer than {NAME_MAX}.</li>
 * <li><b>ENOENT</b>: A component of the path does not exist,
 * or length of __path is longer than {PATH_MAX}.</li>
 * <li><b>ENOMEM</b>: Out of memory.</li>
 * <li><b>ENOSPC</b>: Out of memory in ramfs filesystem.</li>
 * <li><b>ENOTDIR</b>: A component of the path is not a directory.</li>
 * <li><b>EMFILE</b>: Too many open file.</li>
 * <li><b>EROFS</b>: The physical drive is write protected with fat filesystem.</li>
 * <li><b>EIO</b>: A hard error occurred in the low level disk I/O layer or the physical drive cannot work.</li>
 * <li><b>ENODEV</b>: The mount is not healthy.</li>
 * <li><b>EPERM</b>: Other errors.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>unistd.h</li></ul>
 * @see getcwd
 * @since Huawei LiteOS V100R001C00
 */
int chdir(const char *);
int fchdir(int);

/**
 * @ingroup unistd
 *
 * @par Description:
 * The getcwd() function shall place an absolute pathname of the current working directory
 * in the array pointed to by __buf, and return __buf.
 *
 * @param __buf  [IN/OUT] Point to an array which place an absolute pathname of the current working directory and return.
 * @param __size [IN]     The size in bytes of the character array pointed to by the __buf argument.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #char* Return the __buf argument if upon successful completion.
 * @retval #NULL  An error is encountered and the contents of the array pointed to by __buf are undefined.
 *
 * @par Errors
 * <ul>
 * <li><b>EINVAL</b>: The __buf is a NULL pointer.</li>
 * <li><b>ERANGE</b>: The size argument is smaller than the length of the current working directory name + 1.</li>
 * <li><b>ENAMETOOLONG</b>: Use memcpy_s to copy the current working directory name to __buf failed.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>unistd.h</li></ul>
 * @see malloc
 * @since Huawei LiteOS V100R001C00
 */
char *getcwd(char *, size_t);

unsigned alarm(unsigned);

/**
 * @ingroup unistd
 *
 * @par Description:
 * The sleep() function shall cause the calling thread to be suspended from execution until either the number of
 * realtime seconds specified by the argument __seconds has elapsed or a signal is delivered to the calling thread
 * and its action is to invoke a signal-catching function or to terminate the process.
 * The suspension time may be longer than requested due to the scheduling of other activity by the system.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #0             The requested time has elapsed.
 * @retval #unsigned int  An error is encountered in LOS_TaskDelay() and the __seconds shall be returned.
 *
 * @par Dependency:
 * <ul><li>unistd.h</li></ul>
 * @see nanosleep
 * @since Huawei LiteOS V100R001C00
 */
unsigned sleep(unsigned);
int pause(void);

pid_t fork(void);
int execve(const char *, char *const [], char *const []);
int execv(const char *, char *const []);
int execle(const char *, const char *, ...);
int execl(const char *, const char *, ...);
int execvp(const char *, char *const []);
int execlp(const char *, const char *, ...);
int fexecve(int, char *const [], char *const []);
_Noreturn void _exit(int);

/**
 * @ingroup unistd
 *
 * @par Description:
 * The getpid() function get the process ID of the calling process.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The getpid() function shall return the process ID of the calling process.
 * @par Dependency:
 * <ul><li>unistd.h</li></ul>
 * @see None
 * @since Huawei LiteOS V100R001C00
 */
pid_t getpid(void);
pid_t getppid(void);
pid_t getpgrp(void);
pid_t getpgid(pid_t);
int setpgid(pid_t, pid_t);
pid_t setsid(void);
pid_t getsid(pid_t);
char *ttyname(int);
int ttyname_r(int, char *, size_t);
int isatty(int);
pid_t tcgetpgrp(int);
int tcsetpgrp(int, pid_t);

uid_t getuid(void);
uid_t geteuid(void);
gid_t getgid(void);
gid_t getegid(void);
int getgroups(int, gid_t []);
int setuid(uid_t);
int seteuid(uid_t);
int setgid(gid_t);
int setegid(gid_t);

char *getlogin(void);
int getlogin_r(char *, size_t);
int gethostname(char *, size_t);
char *ctermid(char *);

int getopt(int, char * const [], const char *);
extern char *optarg;
extern int optind, opterr, optopt;

long pathconf(const char *, int);
long fpathconf(int, int);
long sysconf(int);
size_t confstr(int, char *, size_t);

#if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#define F_ULOCK 0
#define F_LOCK  1
#define F_TLOCK 2
#define F_TEST  3
int setreuid(uid_t, uid_t);
int setregid(gid_t, gid_t);
int lockf(int, int, off_t);
long gethostid(void);
int nice(int);

/**
 * @ingroup unistd
 *
 * @par Description:
 * The sync() function shall request that all data in bcache to be transferred to the storage device for disk 0 and 1.
 *
 * @attention
 * <ul>
 * <li>The sync() function in liteos can only flush all data in bcache into storage device, but can't flush
 * the data in memory, and it shall not return until the system completed that action or until an error is detected.
 * But in POSIX, The sync() function shall cause all information in memory that updates file systems to be scheduled
 * for writing out to all file systems, and the writing, although scheduled, is not necessarily complete
 * upon return from sync().</li>
 * <li>The sync() function is only useful for fat filesystem and the bcache shall be enabled,
 * other filesystems don't need to sync.</li>
 * <li>The sync() function
 * </ul>
 *
 * @retval #void None.
 *
 * @par Dependency:
 * <ul><li>unistd.h</li></ul>
 * @see fsync
 * @since Huawei LiteOS V100R001C00
 */
void sync(void);
pid_t setpgrp(void);
char *crypt(const char *, const char *);
void encrypt(char *, int);
void swab(const void *__restrict, void *__restrict, ssize_t);
#endif

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE) \
 || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE+0 < 700)

/**
 * @ingroup unistd
 *
 * @par Description:
 * The usleep() function shall cause the calling thread to be suspended from execution until either the number of
 * microsecond specified by the argument __usec has elapsed or a signal is delivered to the calling thread
 * and its action is to invoke a signal-catching function or to terminate the process.
 * The suspension time may be longer than requested due to the scheduling of other activity by the system.
 *
 * @attention
 * <ul>
 * <li>If __usec is less than per Millisecond and is not equal to zero,
 * the suspended time shall be set to 1 millisecond.</li>
 * <li>The realtime to sleep is __usec/1000 millisecond.</li>
 * </ul>
 *
 * @retval #0   The requested time has elapsed.
 * @retval #-1  An error is encountered in LOS_TaskDelay().
 *
 * @par Dependency:
 * <ul><li>unistd.h</li></ul>
 * @see sleep
 * @since Huawei LiteOS V100R001C00
 */
int usleep(unsigned);
unsigned ualarm(unsigned, unsigned);
#endif

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#define L_SET 0
#define L_INCR 1
#define L_XTND 2
int brk(void *);
void *sbrk(intptr_t);
pid_t vfork(void);
int vhangup(void);
int chroot(const char *);
int getpagesize(void);
int getdtablesize(void);
int sethostname(const char *, size_t);
int getdomainname(char *, size_t);
int setdomainname(const char *, size_t);
int setgroups(size_t, const gid_t *);
char *getpass(const char *);
int daemon(int, int);
void setusershell(void);
void endusershell(void);
char *getusershell(void);
int acct(const char *);
long syscall(long, ...);
int execvpe(const char *, char *const [], char *const []);
int issetugid(void);
int getentropy(void *, size_t);
extern int optreset;
#endif

#ifdef _GNU_SOURCE
extern char **environ;
int setresuid(uid_t, uid_t, uid_t);
int setresgid(gid_t, gid_t, gid_t);
int getresuid(uid_t *, uid_t *, uid_t *);
int getresgid(gid_t *, gid_t *, gid_t *);
char *get_current_dir_name(void);
int syncfs(int);
int euidaccess(const char *, int);
int eaccess(const char *, int);
ssize_t copy_file_range(int, off_t *, int, off_t *, size_t, unsigned);
#endif

#if defined(_LARGEFILE64_SOURCE) || defined(_GNU_SOURCE)
#ifdef __LITEOS__
#define off64_t int64_t
off64_t lseek64(int, off64_t, int);
ssize_t pread64(int, void *, size_t, off64_t);
ssize_t pwrite64(int, const void *, size_t, off64_t);
int truncate64(const char *, off64_t);
int ftruncate64(int, off64_t);
int lockf64(int, int, off64_t);
#else
#define lseek64 lseek
#define pread64 pread
#define pwrite64 pwrite
#define truncate64 truncate
#define ftruncate64 ftruncate
#define lockf64 lockf
#define off64_t off_t
#endif
#endif

#define POSIX_CLOSE_RESTART     0

#define _XOPEN_VERSION          700
#define _XOPEN_UNIX             1
#define _XOPEN_ENH_I18N         1

#define _POSIX_VERSION          200809L
#define _POSIX2_VERSION         _POSIX_VERSION

#define _POSIX_ADVISORY_INFO    _POSIX_VERSION
#define _POSIX_CHOWN_RESTRICTED 1
#define _POSIX_IPV6             _POSIX_VERSION
#define _POSIX_JOB_CONTROL      1
#define _POSIX_MAPPED_FILES     _POSIX_VERSION
#define _POSIX_MEMLOCK          _POSIX_VERSION
#define _POSIX_MEMLOCK_RANGE    _POSIX_VERSION
#define _POSIX_MEMORY_PROTECTION _POSIX_VERSION
#define _POSIX_MESSAGE_PASSING  _POSIX_VERSION
#define _POSIX_FSYNC            _POSIX_VERSION
#define _POSIX_NO_TRUNC         1
#define _POSIX_RAW_SOCKETS      _POSIX_VERSION
#define _POSIX_REALTIME_SIGNALS _POSIX_VERSION
#define _POSIX_REGEXP           1
#define _POSIX_SAVED_IDS        1
#define _POSIX_SHELL            1
#define _POSIX_SPAWN            _POSIX_VERSION
#define _POSIX_VDISABLE         0

#define _POSIX_THREADS          _POSIX_VERSION
#define _POSIX_THREAD_PROCESS_SHARED _POSIX_VERSION
#define _POSIX_THREAD_SAFE_FUNCTIONS _POSIX_VERSION
#define _POSIX_THREAD_ATTR_STACKADDR _POSIX_VERSION
#define _POSIX_THREAD_ATTR_STACKSIZE _POSIX_VERSION
#define _POSIX_THREAD_PRIORITY_SCHEDULING _POSIX_VERSION
#define _POSIX_THREAD_CPUTIME   _POSIX_VERSION
#define _POSIX_TIMERS           _POSIX_VERSION
#define _POSIX_TIMEOUTS         _POSIX_VERSION
#define _POSIX_MONOTONIC_CLOCK  _POSIX_VERSION
#define _POSIX_CPUTIME          _POSIX_VERSION
#define _POSIX_CLOCK_SELECTION  _POSIX_VERSION
#define _POSIX_BARRIERS         _POSIX_VERSION
#define _POSIX_SPIN_LOCKS       _POSIX_VERSION
#define _POSIX_READER_WRITER_LOCKS _POSIX_VERSION
#define _POSIX_ASYNCHRONOUS_IO  _POSIX_VERSION
#define _POSIX_SEMAPHORES       _POSIX_VERSION
#define _POSIX_SHARED_MEMORY_OBJECTS _POSIX_VERSION

#define _POSIX2_C_BIND          _POSIX_VERSION

#include <bits/posix.h>



#define _PC_LINK_MAX	0
#define _PC_MAX_CANON	1
#define _PC_MAX_INPUT	2
#define _PC_NAME_MAX	3
#define _PC_PATH_MAX	4
#define _PC_PIPE_BUF	5
#define _PC_CHOWN_RESTRICTED	6
#define _PC_NO_TRUNC	7
#define _PC_VDISABLE	8
#define _PC_SYNC_IO	9
#define _PC_ASYNC_IO	10
#define _PC_PRIO_IO	11
#define _PC_SOCK_MAXBUF	12
#define _PC_FILESIZEBITS	13
#define _PC_REC_INCR_XFER_SIZE	14
#define _PC_REC_MAX_XFER_SIZE	15
#define _PC_REC_MIN_XFER_SIZE	16
#define _PC_REC_XFER_ALIGN	17
#define _PC_ALLOC_SIZE_MIN	18
#define _PC_SYMLINK_MAX	19
#define _PC_2_SYMLINKS	20

#define _SC_ARG_MAX	0
#define _SC_CHILD_MAX	1
#define _SC_CLK_TCK	2
#define _SC_NGROUPS_MAX	3
#define _SC_OPEN_MAX	4
#define _SC_STREAM_MAX	5
#define _SC_TZNAME_MAX	6
#define _SC_JOB_CONTROL	7
#define _SC_SAVED_IDS	8
#define _SC_REALTIME_SIGNALS	9
#define _SC_PRIORITY_SCHEDULING	10
#define _SC_TIMERS	11
#define _SC_ASYNCHRONOUS_IO	12
#define _SC_PRIORITIZED_IO	13
#define _SC_SYNCHRONIZED_IO	14
#define _SC_FSYNC	15
#define _SC_MAPPED_FILES	16
#define _SC_MEMLOCK	17
#define _SC_MEMLOCK_RANGE	18
#define _SC_MEMORY_PROTECTION	19
#define _SC_MESSAGE_PASSING	20
#define _SC_SEMAPHORES	21
#define _SC_SHARED_MEMORY_OBJECTS	22
#define _SC_AIO_LISTIO_MAX	23
#define _SC_AIO_MAX	24
#define _SC_AIO_PRIO_DELTA_MAX	25
#define _SC_DELAYTIMER_MAX	26
#define _SC_MQ_OPEN_MAX	27
#define _SC_MQ_PRIO_MAX	28
#define _SC_VERSION	29
#define _SC_PAGE_SIZE	30
#define _SC_PAGESIZE	30 /* !! */
#define _SC_RTSIG_MAX	31
#define _SC_SEM_NSEMS_MAX	32
#define _SC_SEM_VALUE_MAX	33
#define _SC_SIGQUEUE_MAX	34
#define _SC_TIMER_MAX	35
#define _SC_BC_BASE_MAX	36
#define _SC_BC_DIM_MAX	37
#define _SC_BC_SCALE_MAX	38
#define _SC_BC_STRING_MAX	39
#define _SC_COLL_WEIGHTS_MAX	40
#define _SC_EXPR_NEST_MAX	42
#define _SC_LINE_MAX	43
#define _SC_RE_DUP_MAX	44
#define _SC_2_VERSION	46
#define _SC_2_C_BIND	47
#define _SC_2_C_DEV	48
#define _SC_2_FORT_DEV	49
#define _SC_2_FORT_RUN	50
#define _SC_2_SW_DEV	51
#define _SC_2_LOCALEDEF	52
#define _SC_UIO_MAXIOV	60 /* !! */
#define _SC_IOV_MAX	60
#define _SC_THREADS	67
#define _SC_THREAD_SAFE_FUNCTIONS	68
#define _SC_GETGR_R_SIZE_MAX	69
#define _SC_GETPW_R_SIZE_MAX	70
#define _SC_LOGIN_NAME_MAX	71
#define _SC_TTY_NAME_MAX	72
#define _SC_THREAD_DESTRUCTOR_ITERATIONS	73
#define _SC_THREAD_KEYS_MAX	74
#define _SC_THREAD_STACK_MIN	75
#define _SC_THREAD_THREADS_MAX	76
#define _SC_THREAD_ATTR_STACKADDR	77
#define _SC_THREAD_ATTR_STACKSIZE	78
#define _SC_THREAD_PRIORITY_SCHEDULING	79
#define _SC_THREAD_PRIO_INHERIT	80
#define _SC_THREAD_PRIO_PROTECT	81
#define _SC_THREAD_PROCESS_SHARED	82
#define _SC_NPROCESSORS_CONF	83
#define _SC_NPROCESSORS_ONLN	84
#define _SC_PHYS_PAGES	85
#define _SC_AVPHYS_PAGES	86
#define _SC_ATEXIT_MAX	87
#define _SC_PASS_MAX	88
#define _SC_XOPEN_VERSION	89
#define _SC_XOPEN_XCU_VERSION	90
#define _SC_XOPEN_UNIX	91
#define _SC_XOPEN_CRYPT	92
#define _SC_XOPEN_ENH_I18N	93
#define _SC_XOPEN_SHM	94
#define _SC_2_CHAR_TERM	95
#define _SC_2_UPE	97
#define _SC_XOPEN_XPG2	98
#define _SC_XOPEN_XPG3	99
#define _SC_XOPEN_XPG4	100
#define _SC_NZERO	109
#define _SC_XBS5_ILP32_OFF32	125
#define _SC_XBS5_ILP32_OFFBIG	126
#define _SC_XBS5_LP64_OFF64	127
#define _SC_XBS5_LPBIG_OFFBIG	128
#define _SC_XOPEN_LEGACY	129
#define _SC_XOPEN_REALTIME	130
#define _SC_XOPEN_REALTIME_THREADS	131
#define _SC_ADVISORY_INFO	132
#define _SC_BARRIERS	133
#define _SC_CLOCK_SELECTION	137
#define _SC_CPUTIME	138
#define _SC_THREAD_CPUTIME	139
#define _SC_MONOTONIC_CLOCK	149
#define _SC_READER_WRITER_LOCKS	153
#define _SC_SPIN_LOCKS	154
#define _SC_REGEXP	155
#define _SC_SHELL	157
#define _SC_SPAWN	159
#define _SC_SPORADIC_SERVER	160
#define _SC_THREAD_SPORADIC_SERVER	161
#define _SC_TIMEOUTS	164
#define _SC_TYPED_MEMORY_OBJECTS	165
#define _SC_2_PBS	168
#define _SC_2_PBS_ACCOUNTING	169
#define _SC_2_PBS_LOCATE	170
#define _SC_2_PBS_MESSAGE	171
#define _SC_2_PBS_TRACK	172
#define _SC_SYMLOOP_MAX	173
#define _SC_STREAMS	174
#define _SC_2_PBS_CHECKPOINT	175
#define _SC_V6_ILP32_OFF32	176
#define _SC_V6_ILP32_OFFBIG	177
#define _SC_V6_LP64_OFF64	178
#define _SC_V6_LPBIG_OFFBIG	179
#define _SC_HOST_NAME_MAX	180
#define _SC_TRACE	181
#define _SC_TRACE_EVENT_FILTER	182
#define _SC_TRACE_INHERIT	183
#define _SC_TRACE_LOG	184

#define _SC_IPV6	235
#define _SC_RAW_SOCKETS	236
#define _SC_V7_ILP32_OFF32	237
#define _SC_V7_ILP32_OFFBIG	238
#define _SC_V7_LP64_OFF64	239
#define _SC_V7_LPBIG_OFFBIG	240
#define _SC_SS_REPL_MAX	241
#define _SC_TRACE_EVENT_NAME_MAX	242
#define _SC_TRACE_NAME_MAX	243
#define _SC_TRACE_SYS_MAX	244
#define _SC_TRACE_USER_EVENT_MAX	245
#define _SC_XOPEN_STREAMS	246
#define _SC_THREAD_ROBUST_PRIO_INHERIT	247
#define _SC_THREAD_ROBUST_PRIO_PROTECT	248

#define _CS_PATH	0
#define _CS_POSIX_V6_WIDTH_RESTRICTED_ENVS	1
#define _CS_GNU_LIBC_VERSION	2
#define _CS_GNU_LIBPTHREAD_VERSION	3
#define _CS_POSIX_V5_WIDTH_RESTRICTED_ENVS	4
#define _CS_POSIX_V7_WIDTH_RESTRICTED_ENVS	5

#define _CS_POSIX_V6_ILP32_OFF32_CFLAGS	1116
#define _CS_POSIX_V6_ILP32_OFF32_LDFLAGS	1117
#define _CS_POSIX_V6_ILP32_OFF32_LIBS	1118
#define _CS_POSIX_V6_ILP32_OFF32_LINTFLAGS	1119
#define _CS_POSIX_V6_ILP32_OFFBIG_CFLAGS	1120
#define _CS_POSIX_V6_ILP32_OFFBIG_LDFLAGS	1121
#define _CS_POSIX_V6_ILP32_OFFBIG_LIBS	1122
#define _CS_POSIX_V6_ILP32_OFFBIG_LINTFLAGS	1123
#define _CS_POSIX_V6_LP64_OFF64_CFLAGS	1124
#define _CS_POSIX_V6_LP64_OFF64_LDFLAGS	1125
#define _CS_POSIX_V6_LP64_OFF64_LIBS	1126
#define _CS_POSIX_V6_LP64_OFF64_LINTFLAGS	1127
#define _CS_POSIX_V6_LPBIG_OFFBIG_CFLAGS	1128
#define _CS_POSIX_V6_LPBIG_OFFBIG_LDFLAGS	1129
#define _CS_POSIX_V6_LPBIG_OFFBIG_LIBS	1130
#define _CS_POSIX_V6_LPBIG_OFFBIG_LINTFLAGS	1131
#define _CS_POSIX_V7_ILP32_OFF32_CFLAGS	1132
#define _CS_POSIX_V7_ILP32_OFF32_LDFLAGS	1133
#define _CS_POSIX_V7_ILP32_OFF32_LIBS	1134
#define _CS_POSIX_V7_ILP32_OFF32_LINTFLAGS	1135
#define _CS_POSIX_V7_ILP32_OFFBIG_CFLAGS	1136
#define _CS_POSIX_V7_ILP32_OFFBIG_LDFLAGS	1137
#define _CS_POSIX_V7_ILP32_OFFBIG_LIBS	1138
#define _CS_POSIX_V7_ILP32_OFFBIG_LINTFLAGS	1139
#define _CS_POSIX_V7_LP64_OFF64_CFLAGS	1140
#define _CS_POSIX_V7_LP64_OFF64_LDFLAGS	1141
#define _CS_POSIX_V7_LP64_OFF64_LIBS	1142
#define _CS_POSIX_V7_LP64_OFF64_LINTFLAGS	1143
#define _CS_POSIX_V7_LPBIG_OFFBIG_CFLAGS	1144
#define _CS_POSIX_V7_LPBIG_OFFBIG_LDFLAGS	1145
#define _CS_POSIX_V7_LPBIG_OFFBIG_LIBS	1146
#define _CS_POSIX_V7_LPBIG_OFFBIG_LINTFLAGS	1147
#define _CS_V6_ENV	1148
#define _CS_V7_ENV	1149

#ifdef __cplusplus
}
#endif

#endif
