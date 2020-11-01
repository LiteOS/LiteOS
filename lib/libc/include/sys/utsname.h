/**
 * @defgroup utsname Utsname
 * @ingroup libc
 */

#ifndef	_SYS_UTSNAME_H
#define	_SYS_UTSNAME_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>
/**
 * @ingroup utsname
 * The definition of structure utsname.
 */
struct utsname {
	char sysname[65];
	char nodename[65];
	char release[65];
	char version[65];
	char machine[65];
#ifdef _GNU_SOURCE
	char domainname[65];
#else
	char __domainname[65];
#endif
};

/**
 *@ingroup utsname
 *
 *@par Description:
 *This API is used to get the name of current system.
 *@attention
 *<ul>
 *<li> The members "machine" and "domainname" is not supported.</li>
 *</ul>
 *
 *@retval  0    Get the name of current system successfully.
 *@retval  -1   Fails to get the name of current system.
 *
 *@par Dependency:
 *<ul><li>utsname.h</li></ul>
 *@since Huawei LiteOS V100R001C00
 */
int uname (struct utsname *);

#ifdef __cplusplus
}
#endif

#endif
