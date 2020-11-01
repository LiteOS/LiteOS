/**
 * @defgroup dirent Dirent
 * @ingroup libc
 */

#ifndef	_DIRENT_H
#define	_DIRENT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#define __NEED_ino_t
#define __NEED_off_t
#if defined(_BSD_SOURCE) || defined(_GNU_SOURCE)
#define __NEED_size_t
#endif
#ifdef __LITEOS__
#define __NEED_int64_t
#endif

#include <bits/alltypes.h>

#include <bits/dirent.h>

#ifndef __LITEOS__
typedef struct __dirstream DIR;
#else
typedef struct DIR DIR;
#endif

#define d_fileno d_ino

/**
 * @ingroup dirent
 *
 * @par Description:
 * The closedir() function shall close the directory stream referred to by the argument dirp.
 *
 * @param  dirp [IN] An instance of type DIR created by a previous call to opendir().
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #0  Close directory successful.
 * @retval #-1 An error is encountered and close the directory failed.
 *
 * @par Errors
 * <ul>
 * <li><b>EBADF</b>: The dirp argument does not refer to an open directory stream.</li>
 * <li><b>EEXIST</b> : The 1st member in the FIL/DIR object is invalid of fat.</li>
 * <li><b>EIO</b>: A hard error occurred in the low level disk I/O layer or the physical drive cannot work.</li>
 * <li><b>ENOENT</b>: The internal node pointed to by DIR doesn't exist in jffs2 and romfs.</li>
 * <li><b>EPERM</b>: Other errors.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>dirent.h</li></ul>
 * @see None
 * @since Huawei LiteOS V100R001C00
 */
int            closedir(DIR *);
DIR           *fdopendir(int);

/**
 * @ingroup dirent
 *
 * @par Description:
 * The opendir() function shall open a directory stream corresponding to the directory named by the path argument.
 * The directory stream is positioned at the first entry. If the type DIR is implemented using a file descriptor,
 * applications shall only be able to open up to a total of {OPEN_MAX} files and directories.
 *
 * @param  path [IN] path of the diretory which need to open.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #DIR* Upon successful completion, these functions shall return a pointer to an object of type DIR.
 * @retval #NULL An error is encountered and open the directory faild.
 *
 * @par Errors
 * <ul>
 * <li><b>EINVAL</b>: The path is an empty string.</li>
 * <li><b>ENAMETOOLONG</b>: The length of a component of a pathname is longer than {NAME_MAX}.</li>
 * <li><b>ENOENT</b>: A component of the path does not exist.</li>
 * <li><b>ENOMEM</b>: Out of memory.</li>
 * <li><b>ENOTDIR</b>: A component of the path is not a directory.</li>
 * <li><b>EMFILE</b>: Too many open file.</li>
 * <li><b>EROFS</b>: The physical drive is write protected with fat filesystem.</li>
 * <li><b>EIO</b>: A hard error occurred in the low level disk I/O layer or the physical drive cannot work.</li>
 * <li><b>ENODEV</b>: The mount is not healthy.</li>
 * <li><b>EPERM</b>: Other errors.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>dirent.h</li></ul>
 * @see open | readdir | rewinddir | closedir
 * @since Huawei LiteOS V100R001C00
 */
DIR           *opendir(const char *);

/**
 * @ingroup dirent
 *
 * @par Description:
 * The readdir() function shall return a pointer to a structure representing the directory entry
 * at the current position in the directory stream specified by the argument dirp,
 * and position the directory stream at the next entry.
 * It shall return a null pointer upon reaching the end of the directory stream.
 *
 * @param  dirp [IN] a directory stream of current directory.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval "struct dirent*"  Upon successful completion, readdir() shall return a pointer to
 *                           an object of type struct dirent.
 * @retval NULL              An error is encountered or the end of the directory is encountered.
 *
 * @par Errors
 * <ul>
 * <li><b>EINVAL</b>: dirp argument is invalid.</li>
 * <li><b>ENAMETOOLONG</b>: The length of the pathname of the current directory entry is longer than {NAME_MAX}.</li>
 * <li><b>ENOENT</b>: The current position of the directory stream is invalid.</li>
 * <li><b>ENOMEM</b>: Out of memory.</li>
 * <li><b>ENOSPC</b>: No free cluster of fat filesystem.</li>
 * <li><b>EBADF</b>: The dirp argument does not refer to an open directory stream.</li>
 * <li><b>EIO</b>: A hard error occurred in the low level disk I/O layer or the physical drive cannot work.</li>
 * <li><b>ENODEV</b>: The mount is not healthy.</li>
 * <li><b>EACCES</b>: The current file system doesn't support readdir.</li>
 * <li><b>EPERM</b>: Other errors.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>dirent.h</li></ul>
 * @see closedir | rewinddir
 * @since Huawei LiteOS V100R001C00
 */
struct dirent *readdir(DIR *);
int            readdir_r(DIR *__restrict, struct dirent *__restrict, struct dirent **__restrict);

/**
 * @ingroup dirent
 *
 * @par Description:
 * The rewinddir() function shall reset the position of the directory stream to
 * which dirp refers to the beginning of the directory.
 *
 * @param  dirp [IN] An instance of type DIR created by a previous call to opendir().
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #void None.
 *
 * @par Errors
 * <ul>
 * <li><b>EBADF</b>: The dirp argument does not refer to an open directory stream.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>dirent.h</li></ul>
 * @see  readdir | closedir
 * @since Huawei LiteOS V100R001C00
 */
void           rewinddir(DIR *);
int            dirfd(DIR *);

/**
 * @ingroup dirent
 *
 * @par Description:
 * The alphasort() function can be used as the comparison function for the scandir() function
 * to sort the directory entries, d1 and d2, into alphabetical order.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int Upon successful completion, the alphasort() function shall return an integer greater than,
 * equal to, or less than 0, according to whether the name of the directory entry pointed to by d1
 * is lexically greater than, equal to, or less than the directory pointed to by d2 when both are interpreted
 * as appropriate to the current locale. There is no return value reserved to indicate an error.
 *
 * @par Dependency:
 * <ul><li>dirent.h</li></ul>
 * @see  strcoll
 * @since Huawei LiteOS V100R001C00
 */
int alphasort(const struct dirent **, const struct dirent **);

/**
 * @ingroup dirent
 *
 * @par Description:
 * The scandir() function shall scan the directory dir, calling the function referenced by filter on each directory
 * entry. Entries for which the function referenced by filter returns non-zero shall be stored in strings allocated
 * as if by a call to malloc(), and sorted as if by a call to qsort() with the comparison function compar,
 * except that compar need not provide total ordering. The strings are collected in array namelist
 * which shall be allocated as if by a call to malloc(). If filter is a null pointer, all entries shall be selected.
 * If the comparison function compar does not provide total ordering, the order in which the directory entries
 * are stored is unspecified.
 *
 * @param  dir      [IN]  The path of the directory need to scan.
 * @param  namelist [OUT] Return the pointer of type struct dirent* to store the dirent scanned in the directory.
 * @param  filter   [IN]  Refer to the function to select the entries.
 * @param  compar   [IN]  The comparison function used in qsort() to sort the entries.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int the number of entries in the array and a pointer to the array through the parameter namelist.
 * @retval #-1 An error is encountered.
 *
 * @par Errors
 * <ul>
 * <li><b>EINVAL</b>: The path is an empty string.</li>
 * <li><b>ENAMETOOLONG</b>: The length of a component of a pathname is longer than {NAME_MAX}.</li>
 * <li><b>ENOENT</b>: A component of the path does not exist.</li>
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
 * <ul><li>dirent.h</li></ul>
 * @see  qsort
 * @since Huawei LiteOS V100R001C00
 */
int scandir(const char *, struct dirent ***, int (*)(const struct dirent *), int (*)(const struct dirent **, const struct dirent **));

#if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
/**
 * @ingroup dirent
 *
 * @par Description:
 * The seekdir() function shall set the position of the next readdir() operation on the directory stream specified
 * by dirp to the position specified by offset. The value of offset should have been returned from
 * an earlier call to telldir() using the same directory stream.
 *
 * @param  dirp   [IN] An instance of type DIR created by a previous call to opendir().
 * @param  offset [IN] offset to seek to
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #void None.
 *
 * @par Errors
 * <ul>
 * <li><b>EBADF</b>: The dirp argument does not refer to an open directory stream.</li>
 * <li><b>EINVAL</b>: offset is less than zero.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>dirent.h</li></ul>
 * @see  readdir | telldir
 * @since Huawei LiteOS V100R001C00
 */
void           seekdir(DIR *, long);

/**
 * @ingroup dirent
 *
 * @par Description:
 * The telldir() function shall obtain the current location associated with the directory stream specified by dirp.
 *
 * @param  dirp [IN] An instance of type DIR created by a previous call to opendir().
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #long Upon successful completion, telldir() shall return the current location
 * of the specified directory stream.
 *
 * @par Errors
 * <ul>
 * <li><b>EBADF</b>: The dirp argument does not refer to an open directory stream.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>dirent.h</li></ul>
 * @see  readdir | seekdir
 * @since Huawei LiteOS V100R001C00
 */
long           telldir(DIR *);
#endif

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#define DT_UNKNOWN 0
#define DT_FIFO 1
#define DT_CHR 2
#define DT_DIR 4
#define DT_BLK 6
#define DT_REG 8
#define DT_LNK 10
#define DT_SOCK 12
#define DT_WHT 14
#define IFTODT(x) ((x)>>12 & 017)
#define DTTOIF(x) ((x)<<12)
int getdents(int, struct dirent *, size_t);
#endif

#ifdef _GNU_SOURCE
int versionsort(const struct dirent **, const struct dirent **);
#endif

#if defined(_LARGEFILE64_SOURCE) || defined(_GNU_SOURCE)
#define dirent64 dirent
#define readdir64 readdir
#define readdir64_r readdir_r
#define scandir64 scandir
#define alphasort64 alphasort
#define versionsort64 versionsort
#ifdef __LITEOS__
#define off64_t int64_t
#else
#define off64_t off_t
#endif
#define ino64_t ino_t
#define getdents64 getdents
#endif

#ifdef __cplusplus
}
#endif

#endif
