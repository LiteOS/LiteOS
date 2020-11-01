/**
 * @defgroup fcntl Fcntl
 * @ingroup libc
 */

#ifndef	_FCNTL_H
#define	_FCNTL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#define __NEED_off_t
#define __NEED_pid_t
#define __NEED_mode_t

#ifdef _GNU_SOURCE
#define __NEED_size_t
#define __NEED_ssize_t
#define __NEED_struct_iovec
#endif
#ifdef __LITEOS__
#define __NEED_int64_t
#endif

#include <bits/alltypes.h>

#include <bits/fcntl.h>

struct flock {
	short l_type;
	short l_whence;
	off_t l_start;
	off_t l_len;
	pid_t l_pid;
};


/**
 * @ingroup fcntl
 *
 * @par Description:
 * Create a new file or rewrite an existing one.
 * @param  path  [IN] Path of the file which need to be created.
 * @param  mode  [IN] Describes the permissions of the file, should be an integer type.
 * (For example: 0666, 0777)
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #0  Upon successful completion.
 * @retval #-1 Failed and set errno to indicate the error.
 *
 * @par Errors
 * <ul>
 * <li>Refer to open.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>fcntl.h</li></ul>
 * @see None
 * @since Huawei LiteOS V100R001C00
 */
int creat(const char *, mode_t);
int fcntl(int, int, ...);

#ifdef __LITEOS__
#define fcntl64 fcntl
#endif

/**
 * @ingroup fcntl
 *
 * @par Description:
 * The open() function shall establish the connection between a file and a file descriptor.
 * It shall create an open file description that refers to a file and a file descriptor that refers to that open file
 * description. The file descriptor is used by other I/O functions to refer to that file. The file status flags
 * The file offset used to mark the current position within the file shall be set to the beginning of the file.
 * and file access modes of the open file description shall be set according to the value of oflags.
 * @param  path   [IN] Points to a pathname naming the file.
 * @param  oflags [IN] Values for oflags are constructed by a bitwise-inclusive OR of flags from the following list,
 * defined in <libc/kernel/asm-generic/fcntl.h>. Applications should specify one of the median values
 * (file access patterns) in the value of oflags in the attention description.
 * @param  "..."  [IN] If the file is opened for creation, description file mode_t mode.
 * @attention
 * <ul>
 * <li><b>O_RDONLY</b>: Open for reading only.</li>
 * <li><b>O_RDWR</b>: Open for reading and writing. The result is undefined if this flag is applied to a FIFO.</li>
 * <li><b>O_WRONLY</b>: Open for writing only.</li>
 * <li><b>O_CREAT</b>: If the file exists, this flag has no effect except as noted under O_EXCL below. Otherwise,
 * if O_DIRECTORY is not set the file shall be created as a regular file; the access permission bits of the file mode
 * shall be set to the value of the argument following the oflag argument taken as type mode_t.</li>
 * <li><b>O_EXCL</b>: If O_CREAT and O_EXCL are set, open() shall fail if the file exists.
 * The check for the existence of the file and the creation of the file if it does not exist shall be atomic with
 * respect to other threads executing open() naming the same filename in the same directory with O_EXCL and O_CREAT set.
 * If O_EXCL and O_CREAT are set, and path names a symbolic link, open() shall fail and set errno to [EEXIST],
 * regardless of the contents of the symbolic link. If O_EXCL is set and O_CREAT is not set, the result is undefined
 * .</li>
 * <li><b>O_NOCTTY</b>: Not support.</li>
 * <li><b>O_APPEND</b>: If set, the file offset shall be set to the end of the file prior to each write.</li>
 * <li><b>O_NONBLOCK</b>: When opening a FIFO with O_RDONLY or O_WRONLY set:
 * If O_NONBLOCK is set, an open() for reading-only shall return without delay.
 * An open() for writing-only shall return an error if no process currently has the file open for reading.
 * If O_NONBLOCK is clear, an open() for reading-only shall block the calling thread until a thread opens the file
 * for writing. An open() for writing-only shall block the calling thread until a thread opens the file for reading.</li>
 * <li><b>O_TRUNC</b>: If the file exists and is a regular file, and the file is successfully opened O_RDWR or O_WRONLY,
 * its length shall be truncated to 0, and the mode and owner shall be unchanged. It shall have no effect on FIFO special
 * files or terminal device files. Its effect on other file types is implementation-defined.
 * The result of using O_TRUNC without either O_RDWR or O_WRONLY is undefined.</li>
 * <li><b>O_DIRECTORY</b>: No support, if set this mode returns error code [EACCES].</li>
 * <li><b>O_NOFOLLOW</b>: Not support.</li>
 * <li><b>O_DIRECT</b>: Not support.</li>
 * </ul>
 *
 * @retval #0  Upon successful completion.
 * @retval #-1 Failed and set errno to indicate the error.
 *
 * @par Errors
 * <ul>
 * <li><b>EACCES</b>: Permission bits of the file mode do not permit the requested access,
 * or search permission is denied on a component of the path prefix, or set oflags with O_DIRECTORY shall return it.</li>
 * <li><b>EINVAL</b>: The path name format is invalid.</li>
 * <li><b>ENAMETOOLONG</b>: The length of a component of a pathname is longer than {NAME_MAX}.</li>
 * <li><b>ENOENT</b>: O_CREAT is not set and the named file does not exist. Or a directory component in pathname does not
 * exist or is a dangling symbolic link. This error code is also returned in the fat file system: no valid FAT volume,
 * not find the file, not find the path</li>
 * <li><b>ENOMEM</b>: Out of memory.</li>
 * <li><b>ENXIO</b>: Inode is invalid or not a "normal" character driver or not a mountpoint.</li>
 * <li><b>EMFILE</b>: All file descriptors available to the process are currently open.</li>
 * <li><b>EPERM</b>: Get the file structure corresponding to the file descriptor failed.
 * In fat file system: This error code is also returned when operation not permitted or access denied due to prohibited
 * access or directory full.</li>
 * <li><b>EEXIST</b>: O_CREAT and O_EXCL are set, and the named file exists.
 * <li><b>EIO</b>: A hard error occurred in the low level disk I/O layer ro the physical drive cannot work or Assertion
 * failed.</li>
 * <li><b>EROFS</b>: The named file resides on a read-only file system and either O_WRONLY, O_RDWR, O_CREAT
 * (if the file does not exist), or O_TRUNC is set in the oflags argument.</li>
 * <li><b>ENOSPC</b>: The directory or file system that would contain the new file cannot be expanded,
 * the file does not exist, and O_CREAT is specified.</li>
 * <li><b>ENFILE</b>: The maximum allowable number of files is currently open in the system.</li>
 * <li><b>EISDIR</b>: The named file is a directory and oflag includes O_WRONLY or O_RDWR, or includes O_CREAT without
 * O_DIRECTORY.</li>
 * <li><b>ENOTDIR</b>: A component of the path prefix names an existing file that is neither a directory nor a symbolic
 * link to a directory; or O_CREAT and O_EXCL are not specified, the path argument contains at least one non-<slash>
 * character and ends with one or more trailing <slash> characters, and the last pathname component names an existing
 * file that is neither a directory nor a symbolic link to a directory; or the path
 * argument resolves to a non-directory file.</li>
 * <li><b>EBUSY</b>: The operation is rejected according to the file sharing policy.</li>
 * <li><b>ENODEV</b>: No such device.</li>
 * <li><b>ENOTEMPTY</b>: Directory not empty.</li>
 * <li><b>ELOOP</b>: Too many symbolic links encountered.</li>
 * <li><b>EFAULT</b>: Bad address.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>fcntl.h</li></ul>
 * @see None
 * @since Huawei LiteOS V100R001C00
 */
int open(const char *, int, ...);
int openat(int, const char *, int, ...);
int posix_fadvise(int, off_t, off_t, int);
int posix_fallocate(int, off_t, off_t);

#define O_SEARCH   O_PATH
#define O_EXEC     O_PATH
#define O_TTY_INIT 0

#define O_ACCMODE (03|O_SEARCH)
#define O_RDONLY  00
#define O_WRONLY  01
#define O_RDWR    02

#define F_OFD_GETLK 36
#define F_OFD_SETLK 37
#define F_OFD_SETLKW 38

#define F_DUPFD_CLOEXEC 1030

#define F_RDLCK 0
#define F_WRLCK 1
#define F_UNLCK 2

#define FD_CLOEXEC 1

#define AT_FDCWD (-100)
#define AT_SYMLINK_NOFOLLOW 0x100
#define AT_REMOVEDIR 0x200
#define AT_SYMLINK_FOLLOW 0x400
#define AT_EACCESS 0x200

#define POSIX_FADV_NORMAL     0
#define POSIX_FADV_RANDOM     1
#define POSIX_FADV_SEQUENTIAL 2
#define POSIX_FADV_WILLNEED   3
#ifndef POSIX_FADV_DONTNEED
#define POSIX_FADV_DONTNEED   4
#define POSIX_FADV_NOREUSE    5
#endif

#undef SEEK_SET
#undef SEEK_CUR
#undef SEEK_END
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#ifndef S_IRUSR
#define S_ISUID 04000
#define S_ISGID 02000
#define S_ISVTX 01000
#define S_IRUSR 0400
#define S_IWUSR 0200
#define S_IXUSR 0100
#define S_IRWXU 0700
#define S_IRGRP 0040
#define S_IWGRP 0020
#define S_IXGRP 0010
#define S_IRWXG 0070
#define S_IROTH 0004
#define S_IWOTH 0002
#define S_IXOTH 0001
#define S_IRWXO 0007
#endif

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#define AT_NO_AUTOMOUNT 0x800
#define AT_EMPTY_PATH 0x1000
#define AT_STATX_SYNC_TYPE 0x6000
#define AT_STATX_SYNC_AS_STAT 0x0000
#define AT_STATX_FORCE_SYNC 0x2000
#define AT_STATX_DONT_SYNC 0x4000
#define AT_RECURSIVE 0x8000

#define FAPPEND O_APPEND
#define FFSYNC O_SYNC
#define FASYNC O_ASYNC
#define FNONBLOCK O_NONBLOCK
#define FNDELAY O_NDELAY

#define F_OK 0
#define R_OK 4
#define W_OK 2
#define X_OK 1
#define F_ULOCK 0
#define F_LOCK  1
#define F_TLOCK 2
#define F_TEST  3

#define F_SETLEASE	1024
#define F_GETLEASE	1025
#define F_NOTIFY	1026
#define F_CANCELLK	1029
#define F_SETPIPE_SZ	1031
#define F_GETPIPE_SZ	1032
#define F_ADD_SEALS	1033
#define F_GET_SEALS	1034

#define F_SEAL_SEAL	0x0001
#define F_SEAL_SHRINK	0x0002
#define F_SEAL_GROW	0x0004
#define F_SEAL_WRITE	0x0008
#define F_SEAL_FUTURE_WRITE	0x0010

#define F_GET_RW_HINT		1035
#define F_SET_RW_HINT		1036
#define F_GET_FILE_RW_HINT	1037
#define F_SET_FILE_RW_HINT	1038

#define RWF_WRITE_LIFE_NOT_SET	0
#define RWH_WRITE_LIFE_NONE	1
#define RWH_WRITE_LIFE_SHORT	2
#define RWH_WRITE_LIFE_MEDIUM	3
#define RWH_WRITE_LIFE_LONG	4
#define RWH_WRITE_LIFE_EXTREME	5

#define DN_ACCESS	0x00000001
#define DN_MODIFY	0x00000002
#define DN_CREATE	0x00000004
#define DN_DELETE	0x00000008
#define DN_RENAME	0x00000010
#define DN_ATTRIB	0x00000020
#define DN_MULTISHOT	0x80000000

int lockf(int, int, off_t);
#endif

#if defined(_GNU_SOURCE)
#define F_OWNER_TID 0
#define F_OWNER_PID 1
#define F_OWNER_PGRP 2
#define F_OWNER_GID 2
#ifndef __LITEOS__
struct file_handle {
	unsigned handle_bytes;
	int handle_type;
	unsigned char f_handle[];
};
#endif
struct f_owner_ex {
	int type;
	pid_t pid;
};
#define FALLOC_FL_KEEP_SIZE 1
#define FALLOC_FL_PUNCH_HOLE 2
#define MAX_HANDLE_SZ 128
#define SYNC_FILE_RANGE_WAIT_BEFORE 1
#define SYNC_FILE_RANGE_WRITE 2
#define SYNC_FILE_RANGE_WAIT_AFTER 4
#define SPLICE_F_MOVE 1
#define SPLICE_F_NONBLOCK 2
#define SPLICE_F_MORE 4
#define SPLICE_F_GIFT 8

/**
 * @ingroup fcntl
 *
 * @par Description:
 * The fallocate() function shall ensure that any required storage for regular file data starting at offset and
 * continuing for len bytes is allocated on the file system storage media.
 * @param fd     [IN] File descriptor.
 * @param mode   [IN] Operation mode. Only support FALLOC_FL_KEEP_SIZE.
 * @param offset [IN] Offset of the file to allocated.
 * @param len    [IN] The size to allocate for the file.
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #0  Upon successful completion.
 * @retval #-1 Failed and set errno to indicate the error.
 *
 * @par Errors
 * <ul>
 * <li><b>EINVAL</b>: The len argument or the offset argument is less than zero or greater than INT_MAX,
 * or operation mode is not equal to FALLOC_FL_KEEP_SIZE.</li>
 * <li><b>EBADF</b>: The fd argument is not a valid file descriptor or or the underlying file system does not support
 * this operation.</li>
 * <li><b>EAGAIN</b>: The file list is NULL.</li>
 * <li><b>EACCES</b>: Permission bits of the file mode do not permit the requested access,
 * or search permission is denied on a component of the path prefix.</li>
 * <li><b>ENOENT</b>: A directory component in pathname does not exist or is a dangling symbolic link.</li>
 * <li><b>EEXIST</b>: The file/directory object is already exist.</li>
 * <li><b>EIO</b>: An I/O error occurred while reading from or writing to a file system.</li>
 * <li><b>EROFS</b>: The physical drive is write protected.</li>
 * <li><b>ENOSPC</b>: The directory or file system that would contain the new file cannot be expanded,
 * the file does not exist, and O_CREAT is specified.</li>
 * <li><b>ENFILE</b>: The maximum allowable number of files is currently open in the system.</li>
 * <li><b>ENOTEMPTY</b>: Directory not empty.</li>
 * <li><b>EISDIR</b>: The named file is a directory.</li>
 * <li><b>ENOTDIR</b>: A component of the path prefix names an existing file that is neither a directory nor a symbolic
 * link to a directory; or the path argument resolves to a non-directory file.</li>
 * <li><b>EPERM</b>: Operation not permitted or access denied due to prohibited access or directory full.</li>
 * <li><b>EBUSY</b>: The operation is rejected according to the file sharing policy.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>fcntl.h</li></ul>
 * @see None
 * @since Huawei LiteOS V100R001C00
 */
int fallocate(int, int, off_t, off_t);
#ifndef __LITEOS__
#define fallocate64 fallocate
int name_to_handle_at(int, const char *, struct file_handle *, int *, int);
int open_by_handle_at(int, struct file_handle *, int);
#endif
ssize_t readahead(int, off_t, size_t);
int sync_file_range(int, off_t, off_t, unsigned);
ssize_t vmsplice(int, const struct iovec *, size_t, unsigned);
ssize_t splice(int, off_t *, int, off_t *, size_t, unsigned);
ssize_t tee(int, int, size_t, unsigned);
#if defined(__LITEOS__) && !defined(__LP64__)
typedef long long _loff_t;
#define loff_t _loff_t
#else
#define loff_t off_t
#endif
#endif

#if defined(_LARGEFILE64_SOURCE) || defined(_GNU_SOURCE)
#ifdef __LITEOS__
#define F_GETLK64 F_GETLK
#define F_SETLK64 F_SETLK
#define F_SETLKW64 F_SETLKW
#define flock64 flock
#define off64_t int64_t
int open64 (const char *, int, ...);
int openat64(int, const char *, int, ...);
int creat64(const char *, mode_t);
int lockf64(int, int, off64_t);
int posix_fadvise64(int, off64_t, off64_t, int);
int posix_fallocate64(int, off64_t, off64_t);
int fallocate64(int, int, off64_t, off64_t);
#else
#define F_GETLK64 F_GETLK
#define F_SETLK64 F_SETLK
#define F_SETLKW64 F_SETLKW
#define flock64 flock
#define open64 open
#define openat64 openat
#define creat64 creat
#define lockf64 lockf
#define posix_fadvise64 posix_fadvise
#define posix_fallocate64 posix_fallocate
#define off64_t off_t
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
