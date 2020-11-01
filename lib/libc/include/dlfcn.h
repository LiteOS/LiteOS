/**
 * @defgroup dlfcn Dlfcn
 * @ingroup libc
 */

#ifndef	_DLFCN_H
#define	_DLFCN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#define RTLD_LAZY   1
#define RTLD_NOW    2
#define RTLD_NOLOAD 4
#define RTLD_NODELETE 4096
#define RTLD_GLOBAL 256
#define RTLD_LOCAL  0

#define RTLD_NEXT    ((void *)-1)
#define RTLD_DEFAULT ((void *)0)

#define RTLD_DI_LINKMAP 2


/**
 * @ingroup dlfcn
 *
 * @par Description:
 * The function shall close the loading shared libraries.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int On success, this function shall return LOS_OK.
 * On failure, this function shall return LOS_NOK or -1.
 *
 * @par Dependency:
 * <ul><li>dlfcn.h</li></ul>
 *
 * @see dlopen | dlsym
 *
 * @since Huawei LiteOS V100R001C00
 */
int    dlclose(void *);
char  *dlerror(void);

/**
 * @ingroup dlfcn
 *
 * @par Description:
 * The function shall return the pointer to the handle of loading shared libraries.
 *
 * @attention
 * <ul>
 * <li>The function is not supported for the second param.</li>
 * </ul>
 *
 * @retval #void* On success, this function shall return the pointer to the handle of
 *  loading shared libraries. On failure, this function shall return NULL.
 *
 * @par Dependency:
 * <ul><li>dlfcn.h</li></ul>
 *
 * @see dlclose | dlsym
 *
 * @since Huawei LiteOS V100R001C00
 */
void  *dlopen(const char *, int);

/**
 * @ingroup dlfcn
 *
 * @par Description:
 * The function shall return the second param address in memory of loading shared libraries.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #void*  On success, this function shall return the second param address in memory
 *  of loading shared libraries. On failure, this function shall return NULL.
 *
 * @par Dependency:
 * <ul><li>dlfcn.h</li></ul>
 *
 * @see dlopen | dlclose
 *
 * @since Huawei LiteOS V100R001C00
 */
void  *dlsym(void *__restrict, const char *__restrict);

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
typedef struct {
	const char *dli_fname;
	void *dli_fbase;
	const char *dli_sname;
	void *dli_saddr;
} Dl_info;
int dladdr(const void *, Dl_info *);
int dlinfo(void *, int, void *);
#endif

#if _REDIR_TIME64
__REDIR(dlsym, __dlsym_time64);
#endif

#ifdef __cplusplus
}
#endif

#endif
