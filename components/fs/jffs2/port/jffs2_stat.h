#ifndef __LINUX_STAT_H__
#define __LINUX_STAT_H__

/*-----------------------------------------------------------------------------
 * Minimum values from POSIX.1 tables 2-3, 2-7 and 2-7a.

 * These are the standard-mandated minimum values.
 * These values do not vary with the implementation - they may
 * simply be defined
 */

/* Minimum number of operations in one list I/O call.  */
#define _POSIX_AIO_LISTIO_MAX	2

/* Minimal number of outstanding asynchronous I/O operations.  */
#define _POSIX_AIO_MAX		1

/* Maximum length of arguments to `execve', including environment.  */
#define	_POSIX_ARG_MAX		4096

/* Maximum simultaneous processes per real user ID.  */
#define	_POSIX_CHILD_MAX	6

/* Minimal number of timer expiration overruns.  */
#define _POSIX_DELAYTIMER_MAX	32

/* Maximum link count of a file.  */
#define	_POSIX_LINK_MAX		8

/* Size of storage required for a login name */
#define _POSIX_LOGIN_NAME_MAX   9

/* Number of bytes in a terminal canonical input queue.  */
#define	_POSIX_MAX_CANON	255

/* Number of bytes for which space will be
   available in a terminal input queue.  */
#define	_POSIX_MAX_INPUT	255

/* Maximum number of message queues open for a process.  */
#define _POSIX_MQ_OPEN_MAX	8

/* Maximum number of supported message priorities.  */
#define _POSIX_MQ_PRIO_MAX	32

/* Number of bytes in a filename.  */
#define	_POSIX_NAME_MAX		14

/* Number of simultaneous supplementary group IDs per process.  */
#define	_POSIX_NGROUPS_MAX	0

/* Number of files one process can have open at once.  */
#define	_POSIX_OPEN_MAX		16

/* Number of bytes in a pathname.  */
#define	_POSIX_PATH_MAX		255

/* Number of bytes than can be written atomically to a pipe.  */
#define	_POSIX_PIPE_BUF		512

/* Minimal number of realtime signals reserved for the application.  */
#define _POSIX_RTSIG_MAX	8

/* Number of semaphores a process can have.  */
#define _POSIX_SEM_NSEMS_MAX	256

/* Maximal value of a semaphore.  */
#define _POSIX_SEM_VALUE_MAX	32767

/* Number of pending realtime signals.  */
#define _POSIX_SIGQUEUE_MAX	32

/* Largest value of a `ssize_t'.  */
#define	_POSIX_SSIZE_MAX	32767

/* Number of streams a process can have open at once.  */
#define	_POSIX_STREAM_MAX	8

/* Controlling the iterations of destructors for thread-specific data.  */
#define _POSIX_THREAD_DESTRUCTOR_ITERATIONS	4

/* The number of data keys per process.  */
#define _POSIX_THREAD_KEYS_MAX	128

/* The number of threads per process.  */
#define _POSIX_THREAD_THREADS_MAX	64

/* Maximum number of characters in a tty name.  */
#define	_POSIX_TTY_NAME_MAX	9

/* Number of timer for a process.  */
#define _POSIX_TIMER_MAX	32

/* Maximum length of a timezone name (element of `tzname').  */
#define	_POSIX_TZNAME_MAX	3

/* Maximum clock resolution in nanoseconds.  */
#define _POSIX_CLOCKRES_MIN	20000000

//-----------------------------------------------
//  stat.h need by fs-ecos.h
//-----------------------------------------------
#define __stat_mode_DIR    (1<<0)
#define __stat_mode_CHR    (1<<1)
#define __stat_mode_BLK    (1<<2)
#define __stat_mode_REG    (1<<3)
#define __stat_mode_FIFO   (1<<4)
#define __stat_mode_MQ     (1<<5)
#define __stat_mode_SEM    (1<<6)
#define __stat_mode_SHM    (1<<7)
#define __stat_mode_LNK    (1<<8)
#define __stat_mode_SOCK   (1<<9)

#ifdef __GNUC__
#undef S_IFDIR
#undef S_IFCHR
#undef S_IFBLK
#undef S_IFREG
#undef S_IFIFO
#undef S_IFLNK
#undef S_IFSOCK
#undef S_IFMT
#undef S_ISDIR
#undef S_ISCHR
#undef S_ISBLK
#undef S_ISREG
#undef S_ISFIFO
#undef S_ISLNK
#undef S_ISSOCK
#undef S_IRUSR
#undef S_IWUSR
#undef S_IXUSR
#undef S_IRWXU
#undef S_IRGRP
#undef S_IWGRP
#undef S_IXGRP
#undef S_IRWXG
#undef S_IROTH
#undef S_IWOTH
#undef S_IXOTH
#undef S_IRWXO
#undef S_ISUID
#undef S_ISGID
#endif

#if !defined(_POSIX_C_SOURCE) || (_POSIX_C_SOURCE >= 200112L)
#define S_IFDIR          (__stat_mode_DIR)
#define S_IFCHR          (__stat_mode_CHR)
#define S_IFBLK          (__stat_mode_BLK)
#define S_IFREG          (__stat_mode_REG)
#define S_IFIFO          (__stat_mode_FIFO)
#define S_IFLNK          (__stat_mode_LNK)
#define S_IFSOCK         (__stat_mode_SOCK)
#define S_IFMT           (S_IFDIR|S_IFCHR|S_IFBLK|S_IFREG| \
                          S_IFIFO|S_IFLNK|S_IFSOCK)
#endif

#define S_ISDIR(__mode)  ((__mode) & __stat_mode_DIR )
#define S_ISCHR(__mode)  ((__mode) & __stat_mode_CHR )
#define S_ISBLK(__mode)  ((__mode) & __stat_mode_BLK )
#define S_ISREG(__mode)  ((__mode) & __stat_mode_REG )
#define S_ISFIFO(__mode) ((__mode) & __stat_mode_FIFO )
#if !defined(_POSIX_C_SOURCE) || (_POSIX_C_SOURCE >= 200112L)
#define S_ISLNK(__mode)  ((__mode) & __stat_mode_LNK )
#define S_ISSOCK(__mode)  ((__mode) & __stat_mode_SOCK )
#endif

#define S_TYPEISMQ(__buf)   ((__buf)->st_mode & __stat_mode_MQ )
#define S_TYPEISSEM(__buf)  ((__buf)->st_mode & __stat_mode_SEM )
#define S_TYPEISSHM(__buf)  ((__buf)->st_mode & __stat_mode_SHM )


#define S_IRUSR  (1<<16)
#define S_IWUSR  (1<<17)
#define S_IXUSR  (1<<18)
#define S_IRWXU  (S_IRUSR|S_IWUSR|S_IXUSR)

#define S_IRGRP  (1<<19)
#define S_IWGRP  (1<<20)
#define S_IXGRP  (1<<21)
#define S_IRWXG  (S_IRGRP|S_IWGRP|S_IXGRP)

#define S_IROTH  (1<<22)
#define S_IWOTH  (1<<23)
#define S_IXOTH  (1<<24)
#define S_IRWXO  (S_IROTH|S_IWOTH|S_IXOTH)

#define S_ISUID  (1<<25)
#define S_ISGID  (1<<26)


#define S_IRUGO (S_IRUSR|S_IRGRP|S_IROTH)
#define S_IWUGO (S_IWUSR|S_IWGRP|S_IWOTH)
#define S_IXUGO (S_IXUSR|S_IXGRP|S_IXOTH)
#define S_IRWXUGO (S_IRWXU|S_IRWXG|S_IRWXO)

#endif /* __LINUX_STAT_H__ */
