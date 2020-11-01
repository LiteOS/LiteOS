/**
 * @defgroup stat Stat
 * @ingroup libc
 */

#ifndef	_SYS_STAT_H
#define	_SYS_STAT_H
#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#define __NEED_dev_t
#define __NEED_ino_t
#define __NEED_mode_t
#define __NEED_nlink_t
#define __NEED_uid_t
#define __NEED_gid_t
#define __NEED_off_t
#define __NEED_time_t
#define __NEED_blksize_t
#define __NEED_blkcnt_t
#define __NEED_struct_timespec
#ifdef __LITEOS__
#define __NEED_int64_t
#endif

#include <bits/alltypes.h>

#include <bits/stat.h>

#define st_atime st_atim.tv_sec
#define st_mtime st_mtim.tv_sec
#define st_ctime st_ctim.tv_sec

#define S_IFMT  0170000

#define S_IFDIR 0040000
#define S_IFCHR 0020000
#define S_IFBLK 0060000
#define S_IFREG 0100000
#define S_IFIFO 0010000
#define S_IFLNK 0120000
#define S_IFSOCK 0140000

#define S_TYPEISMQ(buf)  0
#define S_TYPEISSEM(buf) 0
#define S_TYPEISSHM(buf) 0
#define S_TYPEISTMO(buf) 0

#define S_ISDIR(mode)  (((mode) & S_IFMT) == S_IFDIR)
#define S_ISCHR(mode)  (((mode) & S_IFMT) == S_IFCHR)
#define S_ISBLK(mode)  (((mode) & S_IFMT) == S_IFBLK)
#define S_ISREG(mode)  (((mode) & S_IFMT) == S_IFREG)
#define S_ISFIFO(mode) (((mode) & S_IFMT) == S_IFIFO)
#define S_ISLNK(mode)  (((mode) & S_IFMT) == S_IFLNK)
#define S_ISSOCK(mode) (((mode) & S_IFMT) == S_IFSOCK)

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

#ifdef __LITEOS__
#define DEFFILEMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)
#define F_RDO 0x01    /* Read only */
#define F_HID 0x02    /* Hidden */
#define F_SYS 0x04    /* System */
#define F_ARC 0x20    /* Archive */
#endif

#define UTIME_NOW  0x3fffffff
#define UTIME_OMIT 0x3ffffffe

/**
 * @ingroup stat
 *
 * @par Description:
 * The stat() function shall obtain information about the named file and write it to the area pointed to by the buf
 * argument.
 * @param path  [IN] Points to a pathname naming a file.
 * @param buf   [IN] A pointer to a stat structure, defined in the <sys/stat.h> header,
 *                   which stores information related to files.
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
 * <li><b>EINVAL</b>: Invalid argument.</li>
 * <li><b>ENOENT</b>: A directory component in pathname does not exist or path is an empty string.</li>
 * <li><b>ENAMETOOLONG</b>: The length of a component of a pathname is longer than {NAME_MAX}.</li>
 * <li><b>ENOMEM</b>: Out of memory.</li>
 * <li><b>ENOSYS</b>: No support.</li>
 * <li><b>EBADF</b>: The fd argument is not a valid file descriptor or or the underlying file system does not support
 * this operation.</li>
 * <li><b>EAGAIN</b>: The file list is NULL.</li>
 * <li><b>EACCES</b>: Permission bits of the file mode do not permit the requested access,
 * or search permission is denied on a component of the path prefix.</li>
 * <li><b>EEXIST</b>: The file/directory object is already exist.</li>
 * <li><b>EIO</b>: An I/O error occurred while reading from or writing to a file system.</li>
 * <li><b>EROFS</b>: The physical drive is write protected.</li>
 * <li><b>ENOSPC</b>: No space left on device.</li>
 * <li><b>ENFILE</b>: The maximum allowable number of files is currently open in the system.</li>
 * <li><b>ENOTEMPTY</b>: directory not empty.</li>
 * <li><b>EISDIR</b>: The named file is a directory.</li>
 * <li><b>ENOTDIR</b>: A component of the path prefix names an existing file that is neither a directory nor a symbolic
 * link to a directory; or the path argument resolves to a non-directory file.</li>
 * <li><b>EPERM</b>: Operation not permitted or access denied due to prohibited access or directory full.</li>
 * <li><b>EBUSY</b>: The operation is rejected according to the file sharing policy.</li>
 * <li><b>ENOSYS</b>: Not support.</li>
 * <li><b>EPROTO</b>: Protocol error in nfs.</li>
 * <li><b>ENODEV</b>: No such device.</li>
 * <li><b>EFAULT</b>: Bad address.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>stat.h</li></ul>
 * @see None
 * @since Huawei LiteOS V100R001C00
 */
int stat(const char *__restrict, struct stat *__restrict);

/**
 * @ingroup stat
 *
 * @par Description:
 * The fstat() function shall obtain information about an open file associated with the file descriptor fields, and shall
 * write it to the area pointed to by buf.
 * @param fields [IN] File descriptor fields.
 * @param buf    [IN] A pointer to a stat structure, defined in the <sys/stat.h> header,
 *                    which stores information related to files.
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
 * <li><b>EBADF</b>: The fd argument is not a valid file descriptor.</li>
 * <li><b>EAGAIN</b>: The file list is NULL.</li>
 * <li><b>EINVAL</b>: Invalid argument.</li>
 * <li><b>ENOENT</b>: a directory component in pathname does not exist or path is an empty string.</li>
 * <li><b>ENAMETOOLONG</b>: The length of a component of a pathname is longer than {NAME_MAX}.</li>
 * <li><b>ENOMEM</b>: Out of memory.</li>
 * <li><b>ENOSYS</b>: no support.</li>
 * <li><b>EBADF</b>: The fd argument is not a valid file descriptor or or the underlying file system does not support
 * this operation.</li>
 * <li><b>EAGAIN</b>: The file list is NULL.</li>
 * <li><b>EACCES</b>: Permission bits of the file mode do not permit the requested access,
 * or search permission is denied on a component of the path prefix.</li>
 * <li><b>EEXIST</b>: The file/directory object is already exist.</li>
 * <li><b>EIO</b>: An I/O error occurred while reading from or writing to a file system.</li>
 * <li><b>EROFS</b>: The physical drive is write protected.</li>
 * <li><b>ENOSPC</b>: No space left on device.</li>
 * <li><b>ENFILE</b>: The maximum allowable number of files is currently open in the system.</li>
 * <li><b>ENOTEMPTY</b>: directory not empty.</li>
 * <li><b>EISDIR</b>: The named file is a directory.</li>
 * <li><b>ENOTDIR</b>: A component of the path prefix names an existing file that is neither a directory nor a symbolic
 * link to a directory; or the path argument resolves to a non-directory file.</li>
 * <li><b>EPERM</b>: operation not permitted or access denied due to prohibited access or directory full.</li>
 * <li><b>EBUSY</b>: The operation is rejected according to the file sharing policy.</li>
 * <li><b>ENOSYS</b>: not support.</li>
 * <li><b>EPROTO</b>: Protocol error in nfs.</li>
 * <li><b>ENODEV</b>: No such device.</li>
 * <li><b>EFAULT</b>: Bad address.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>stat.h</li></ul>
 * @see None
 * @since Huawei LiteOS V100R001C00
 */
int fstat(int, struct stat *);

/**
 * @ingroup stat
 *
 * @par Description:
 * The lstat() function shall be equivalent to stat(), function shall obtain information about the named file and write
 * it to the area pointed to by the buf.
 * argument.
 * @param path   [IN] Points to a pathname naming a file.
 * @param buffer [IN] A pointer to a stat structure, defined in the <sys/stat.h> header,
 *                    which stores information related to files.
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
 * <li><b>EINVAL</b>: Invalid argument.</li>
 * <li><b>ENOENT</b>: A directory component in pathname does not exist or path is an empty string.</li>
 * <li><b>ENAMETOOLONG</b>: The length of a component of a pathname is longer than {NAME_MAX}.</li>
 * <li><b>ENOMEM</b>: Out of memory.</li>
 * <li><b>ENOSYS</b>: No support.</li>
 * <li><b>EBADF</b>: The fd argument is not a valid file descriptor or or the underlying file system does not support
 * this operation.</li>
 * <li><b>EAGAIN</b>: The file list is NULL.</li>
 * <li><b>EACCES</b>: Permission bits of the file mode do not permit the requested access,
 * or search permission is denied on a component of the path prefix.</li>
 * <li><b>EEXIST</b>: The file/directory object is already exist.</li>
 * <li><b>EIO</b>: An I/O error occurred while reading from or writing to a file system.</li>
 * <li><b>EROFS</b>: The physical drive is write protected.</li>
 * <li><b>ENOSPC</b>: No space left on device.</li>
 * <li><b>ENFILE</b>: The maximum allowable number of files is currently open in the system.</li>
 * <li><b>ENOTEMPTY</b>: directory not empty.</li>
 * <li><b>EISDIR</b>: The named file is a directory.</li>
 * <li><b>ENOTDIR</b>: A component of the path prefix names an existing file that is neither a directory nor a symbolic
 * link to a directory; or the path argument resolves to a non-directory file.</li>
 * <li><b>EPERM</b>: Operation not permitted or access denied due to prohibited access or directory full.</li>
 * <li><b>EBUSY</b>: The operation is rejected according to the file sharing policy.</li>
 * <li><b>ENOSYS</b>: Not support.</li>
 * <li><b>EPROTO</b>: Protocol error in nfs.</li>
 * <li><b>ENODEV</b>: No such device.</li>
 * <li><b>EFAULT</b>: Bad address.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>stat.h</li></ul>
 * @see None
 * @since Huawei LiteOS V100R001C00
 */
int lstat(const char *__restrict, struct stat *__restrict);
int fstatat(int, const char *__restrict, struct stat *__restrict, int);
int chmod(const char *, mode_t);
int fchmod(int, mode_t);
int fchmodat(int, const char *, mode_t, int);
mode_t umask(mode_t);

/**
 * @ingroup stat
 *
 * @par Description:
 * The mkdir() function shall create a new directory with name path. The file permission bits of the new directory shall
 * be initialized from mode. These file permission bits of the mode argument shall be modified by the process' file
 * creation mask.
 * @param pathname [IN] Directory with name path.
 * @param mode     [IN] File permission bits.
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
 * <li><b>EINVAL</b>: The path is a null pointer or an empty string.</li>
 * <li><b>EACCES</b>: Permission bits of the file mode do not permit the requested access,
 * or search permission is denied on a component of the path prefix.</li>
 * <li><b>ENAMETOOLONG</b>: The length of a component of a pathname is longer than {NAME_MAX}.</li>
 * <li><b>ENOENT</b>: A directory component in pathname does not exist.</li>
 * <li><b>EEXIST</b>: The file/directory object is already exist.</li>
 * <li><b>EIO</b>: An I/O error occurred while reading from or writing to a file system.</li>
 * <li><b>EROFS</b>: The physical drive is write protected.</li>
 * <li><b>ENOSPC</b>: No space left on device.</li>
 * <li><b>ENFILE</b>: The maximum allowable number of files is currently open in the system.</li>
 * <li><b>ENOTDIR</b>: A component of the path prefix names an existing file that is neither a directory nor a symbolic
 * link to a directory; or the path argument resolves to a non-directory file.</li>
 * <li><b>EPERM</b>: Operation not permitted or access denied due to prohibited access or directory full.</li>
 * <li><b>EBUSY</b>: The operation is rejected according to the file sharing policy.</li>
 * <li><b>ENOSYS</b>: Not support.</li>
 * <li><b>EPROTO</b>: Protocol error in nfs.</li>
 * <li><b>EFAULT</b>: Bad address.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>stat.h</li></ul>
 * @see None
 * @since Huawei LiteOS V100R001C00
 */
int mkdir(const char *, mode_t);
int mkfifo(const char *, mode_t);
int mkdirat(int, const char *, mode_t);
int mkfifoat(int, const char *, mode_t);

#if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
int mknod(const char *, mode_t, dev_t);
int mknodat(int, const char *, mode_t, dev_t);
#endif

int futimens(int, const struct timespec [2]);
int utimensat(int, const char *, const struct timespec [2], int);

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
int lchmod(const char *, mode_t);
#define S_IREAD S_IRUSR
#define S_IWRITE S_IWUSR
#define S_IEXEC S_IXUSR
#endif

#if defined(_LARGEFILE64_SOURCE) || defined(_GNU_SOURCE)
#ifdef __LITEOS__
#define stat64 stat
int fstat64(int, struct stat64 *);
int lstat64(const char *, struct stat64 *);
int fstatat64(int, const char *, struct stat64 *, int);
#define blkcnt64_t blkcnt_t
#define fsblkcnt64_t fsblkcnt_t
#define fsfilcnt64_t fsfilcnt_t
#define ino64_t ino_t
#define off64_t int64_t
#else
#define stat64 stat
#define fstat64 fstat
#define lstat64 lstat
#define fstatat64 fstatat
#define blkcnt64_t blkcnt_t
#define fsblkcnt64_t fsblkcnt_t
#define fsfilcnt64_t fsfilcnt_t
#define ino64_t ino_t
#define off64_t off_t
#endif
#endif

#if _REDIR_TIME64
__REDIR(stat, __stat_time64);
__REDIR(fstat, __fstat_time64);
__REDIR(lstat, __lstat_time64);
__REDIR(fstatat, __fstatat_time64);
__REDIR(futimens, __futimens_time64);
__REDIR(utimensat, __utimensat_time64);
#endif

#ifdef __cplusplus
}
#endif
#endif


