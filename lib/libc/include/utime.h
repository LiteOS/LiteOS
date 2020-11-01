/**
 * @defgroup utime Utime
 * @ingroup libc
 */

#ifndef	_UTIME_H
#define	_UTIME_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#define __NEED_time_t

#include <bits/alltypes.h>

struct utimbuf {
	time_t actime;
	time_t modtime;
};

/**
 * @ingroup utime
 *
 * @par Description:
 * The utime() function shall set the modification times of the file named by the path argument.
 * If ptimes is a null pointer, the modification times of the file shall be set to the current time.
 * If ptimes is not a null pointer, times shall be interpreted as a pointer to a utimbuf structure and
 * modification times shall be set to the values contained in the designated structure.
 *
 * @attention
 * <ul>
 * <li>The utime() doesn't support to set access time.</li>
 * <li>Now only fat filesystem support this function.</li>
 * </ul>
 *
 * @retval #0  On success.
 * @retval #-1 On failure with errno set.
 *
 * @par Errors
 * <ul>
 * <li><b>EINVAL</b>: The path is a null pointer or the ptimes is a invalid value.</li>
 * <li><b>ENAMETOOLONG</b>: The length of a component of a pathname is longer than {NAME_MAX}.</li>
 * <li><b>ENOENT</b>: A component of the path does not exist.</li>
 * <li><b>ENOSYS</b>: The file system doesn't support this function.</li>
 * <li><b>EACCES</b>: It is a read-only file system.</li>
 * <li><b>ENOMEM</b>: Out of memory.</li>
 * <li><b>EIO</b>: A hard error occurred in the low level disk I/O layer or the physical drive cannot work.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>utime.h</li></ul>
 * @see fstat
 * @since Huawei LiteOS V100R001C00
 */
int utime (const char *, const struct utimbuf *);

#if _REDIR_TIME64
__REDIR(utime, __utime64);
#endif

#ifdef __cplusplus
}
#endif

#endif
