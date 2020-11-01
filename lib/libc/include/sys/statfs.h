/**
 * @defgroup statfs Statfs
 * @ingroup libc
 */

#ifndef	_SYS_STATFS_H
#define	_SYS_STATFS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#include <sys/statvfs.h>

#ifdef __LITEOS__
#define ROMFS_MAGIC 0x7275
#define MSDOS_SUPER_MAGIC 0x4d44
#define RAMFS_MAGIC 0x858458f6
#define YAFFS_MAGIC 0x5941ff53
#define NFS_SUPER_MAGIC 0x6969
#define PROCFS_MAGIC 0x434f5250
#define JFFS2_SUPER_MAGIC 0x72b6
#endif

typedef struct __fsid_t {
	int __val[2];
} fsid_t;

#include <bits/statfs.h>

/**
 * @ingroup vfs
 *
 * @par Description:
 * The statfs() function shall get the information of the file system pointed to by the path and return the
 * information in buf.
 *
 * @attention
 * <ul>
 * <li>Now only fat filesystem support this function.</li>
 * </ul>
 *
 * @retval #0  On success.
 * @retval #-1 On failure with errno set.
 *
 * @par Errors
 * <ul>
 * <li><b>EFAULT</b>: The path is a null pointer or the buf is a null pointer.</li>
 * <li><b>ENAMETOOLONG</b>: The length of a component of a pathname is longer than {NAME_MAX}.</li>
 * <li><b>ENOENT</b>: A component of the path does not exist.</li>
 * <li><b>ENODEV</b>: The mount is not healthy.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>statfs.h</li></ul>
 * @see None
 * @since Huawei LiteOS V100R001C00
 */
int statfs (const char *, struct statfs *);
int fstatfs (int, struct statfs *);

#if defined(_LARGEFILE64_SOURCE) || defined(_GNU_SOURCE)
#define statfs64 statfs
#define fstatfs64 fstatfs
#define fsblkcnt64_t fsblkcnt_t
#define fsfilcnt64_t fsfilcnt_t
#endif

#ifdef __cplusplus
}
#endif

#endif
