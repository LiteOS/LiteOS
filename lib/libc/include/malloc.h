/**
 * @defgroup malloc Malloc
 * @ingroup libc
 */

#ifndef _MALLOC_H
#define _MALLOC_H

#ifdef __cplusplus
extern "C" {
#endif

#define __NEED_size_t

#include <bits/alltypes.h>

/**
 * @ingroup  malloc
 *
 * @par Description:
 * The malloc() function shall allocate unused space for an object whose size in bytes is specified by byte_count
 * and whose value is unspecified. If byte_count is 0, then malloc() returns NULL.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #void* Upon successful completion with byte_count not equal to 0, malloc() shall return a pointer to the allocated space.
 *              If byte_count is 0, it shall return a null pointer.
 *
 * @par Dependency:
 * <ul><li>malloc.h</li></ul>
 * @see calloc | free | posix_memalign | realloc
 * @since Huawei LiteOS V100R001C00
 */
void *malloc (size_t);
void *calloc (size_t, size_t);
void *realloc (void *, size_t);
void free (void *);
void *valloc (size_t);
void *memalign(size_t, size_t);

size_t malloc_usable_size(void *);

#ifdef __LITEOS__
void *zalloc(size_t);
#endif

#ifdef __cplusplus
}
#endif

#endif
