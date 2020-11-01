/**
 * @defgroup errno Errno
 * @ingroup libc
 */

#ifndef	_ERRNO_H
#define _ERRNO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#include <bits/errno.h>

#ifdef __LITEOS__
/**
 * @ingroup  errno
 *
 * @par Description:
 * The set_errno() function sets the error code.
 *
 * @attention
 * <ul>
 * <li>The set_errno() function is not a standard C Library Function.</li>
 * </ul>
 *
 * @retval #void None.
 *
 * @par Dependency:
 * <ul><li>errno.h</li></ul>
 * @see get_errno
 * @since Huawei LiteOS V100R001C00
 */
void set_errno(int errcode);

/**
 * @ingroup  errno
 *
 * @par Description:
 * The get_errno() function gets the error code.
 *
 * @attention
 * <ul>
 * <li>The get_errno() function is not a standard C Library Function.</li>
 * </ul>
 *
 * @retval #int The get_errno() returns the error code.
 *
 * @par Dependency:
 * <ul><li>errno.h</li></ul>
 * @see set_errno
 * @since Huawei LiteOS V100R001C00
 */
int get_errno(void);
#endif

#ifdef __GNUC__
__attribute__((const))
#endif
int *__errno_location(void);
#define errno (*__errno_location())

#ifdef _GNU_SOURCE
extern char *program_invocation_short_name, *program_invocation_name;
#endif

#ifdef __cplusplus
}
#endif

#endif

