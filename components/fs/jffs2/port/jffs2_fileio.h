/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

#ifndef __JFFS2_FILEIO_H__
#define __JFFS2_FILEIO_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "jffs2_types.h"
#include "jffs2_fs.h"
#include "jffs2_stat.h"
#include "jffs2_fs_sb.h"

#if defined (__GNUC__) || defined (__CC_ARM)
#include "fs/sys/fcntl.h"
#endif

/* Variable names for pathconf()			*/
#define _PC_ASYNC_IO                 1
#define	_PC_CHOWN_RESTRICTED	     2
#define	_PC_LINK_MAX		         3
#define	_PC_MAX_CANON		         4
#define	_PC_MAX_INPUT		         5
#define	_PC_NAME_MAX		         6
#define	_PC_NO_TRUNC		         7
#define	_PC_PATH_MAX		         8
#define	_PC_PIPE_BUF		         9
#define _PC_PRIO_IO                 10
#define _PC_SYNC_IO                 11
#define	_PC_VDISABLE		        12

#define LINK_MAX                    _POSIX_LINK_MAX

typedef struct CYG_STAT_TAG {
    mode_t  st_mode;     /* File mode */
    ino_t   st_ino;      /* File serial number */
    dev_t   st_dev;      /* ID of device containing file */
    nlink_t st_nlink;    /* Number of hard links */
    uid_t   st_uid;      /* User ID of the file owner */
    gid_t   st_gid;      /* Group ID of the file's group */
    off_t   st_size;     /* File size (regular files only) */
    time_t  st_atime;    /* Last access time */
    time_t  st_mtime;    /* Last data modification time */
    time_t  st_ctime;    /* Last file status change time */
}cyg_stat;

//-----------------------------------------------------------------------------
// Keys for getinfo() and setinfo()

#define FS_INFO_CONF            1       /* pathconf() */
#define FS_INFO_ACCESS          2       /* access() */
#define FS_INFO_GETCWD          3       /* getcwd() */
#define FS_INFO_SYNC            4       /* cyg_fs_fssync() */
#define FS_INFO_ATTRIB          5       /* cyg_fs_(get|set)_attrib() */
#ifdef  CYGSEM_FILEIO_INFO_DISK_USAGE
#define FS_INFO_DISK_USAGE      6       /* get_disk_usage()    */
#endif
#define FS_INFO_CHMOD           7       /* chmod() */

//-----------------------------------------------------------------------------
// Types for link()

#define CYG_FSLINK_HARD         1       /* form a hard link */
#define CYG_FSLINK_SOFT         2       /* form a soft link */

//-----------------------------------------------------------------------------
// getinfo() and setinfo() buffers structures.

struct cyg_getcwd_info
{
    char        *buf;           /* buffer for cwd string */
    size_t      size;           /* size of buffer */
};

struct cyg_fs_disk_usage{
  uint64_t total_blocks;
  uint64_t free_blocks;
  uint32_t block_size;
};

//=============================================================================
// IO vector descriptors

typedef struct CYG_IOVEC_TAG
{
    void           *iov_base;           /* Base address. */
    ssize_t        iov_len;             /* Length. */
}cyg_iovec;

enum	cyg_uio_rw { UIO_READ, UIO_WRITE };

/* Segment flag values. */
enum cyg_uio_seg
{
    UIO_USERSPACE,		        /* from user data space */
    UIO_SYSSPACE		        /* from system space */
};

typedef struct CYG_UIO_TAG
{
    cyg_iovec               *uio_iov;	/* pointer to array of iovecs */
    int	                    uio_iovcnt;	/* number of iovecs in array */
    off_t                   uio_offset;	/* offset into file this uio corresponds to */
    ssize_t                 uio_resid;	/* residual i/o count */
    enum cyg_uio_seg        uio_segflg;    /* see above */
    enum cyg_uio_rw         uio_rw;        /* see above */
}cyg_uio;

// Limits
#define UIO_SMALLIOV	8		/* 8 on stack, else malloc */

#ifndef SEEK_SET
#define SEEK_SET        0       /* Seek from beginning of file.  */
#define SEEK_CUR        1       /* Seek from current position.  */
#define SEEK_END        2       /* Set file pointer to EOF plus "offset" */
#endif

//-----------------------------------------------------------------------------
// File flags

// Allocation here is that bits 0..15 are copies of bits from the open
// flags, bits 16..23 are extra bits that are visible to filesystems but
// are not derived from the open call, and bits 24..31 are reserved for
// the fileio infrastructure.
#define CYG_FREAD       O_RDONLY
#define CYG_FWRITE      O_WRONLY
#define CYG_FNONBLOCK   O_NONBLOCK
#define CYG_FAPPEND     O_APPEND
#define CYG_FASYNC      0x00010000
#define CYG_FDIR        0x00020000

#define CYG_FLOCKED     0x01000000  // Set if file is locked
#define CYG_FLOCK       0x02000000  // Lock during file ops
#define CYG_FALLOC      0x80000000  // File is "busy", i.e. allocated

// Mask for open mode bits stored in file object
#define CYG_FILE_MODE_MASK (CYG_FREAD|CYG_FWRITE|CYG_FNONBLOCK|CYG_FAPPEND)

//-----------------------------------------------------------------------------
// Type of file

#define	CYG_FILE_TYPE_FILE      1	/* file */
#define	CYG_FILE_TYPE_SOCKET	2	/* communications endpoint */
#define	CYG_FILE_TYPE_DEVICE	3	/* device */

//-----------------------------------------------------------------------------
// Keys for getinfo() and setinfo()

#define FILE_INFO_CONF          1       /* fpathconf() */

//-----------------------------------------------------------------------------
// Modes for fsync()

#define CYG_FSYNC              1
#define CYG_FDATASYNC          2

//-----------------------------------------------------------------------------
// Get/set info buffer structures

// This is used for pathconf() and fpathconf()
struct cyg_pathconf_info
{
    int         name;           // POSIX defined variable name
    long        value;          // Returned variable value
};

typedef struct CYG_FILE_TAG
{
    uint32_t            f_flag;		/* file state                   */
    uint16_t            f_ucount;   /* use count                    */
    uint16_t            f_type;		/* descriptor type              */
    uint32_t            f_syncmode; /* synchronization protocol     */
    off_t               f_offset;   /* current offset               */
    void	            *f_data;	/* file or socket               */
    uint32_t            f_xops;     /* extra type specific ops      */
}cyg_file;

typedef struct inode cyg_dir;

typedef struct CYG_DIRENT_TAG
{
#ifdef CYGPKG_FILEIO_DIRENT_DTYPE

	mode_t      d_type; // Only supported with FATFS, RAMFS, ROMFS,
	// and JFFS2.
	// d_type is not part of POSIX so
	// should be used with caution.
#endif
	char        d_name[JFFS2_NAME_MAX+1];
}cyg_dirent;


typedef int cyg_fileop_readwrite (cyg_file *fp, cyg_uio *uio);
typedef cyg_fileop_readwrite cyg_fileop_read;
typedef cyg_fileop_readwrite cyg_fileop_write;
typedef int cyg_fileop_lseek   (cyg_file *fp, off_t *pos, int whence );
typedef int cyg_fileop_ioctl   (cyg_file *fp, uint32_t com, uint32_t data);
typedef int cyg_fileop_select  (cyg_file *fp, int which, uint32_t info);
typedef int cyg_fileop_fsync   (cyg_file *fp, int mode );
typedef int cyg_fileop_close   (cyg_file *fp);
typedef int cyg_fileop_fstat   (cyg_file *fp, cyg_stat *buf );
typedef int cyg_fileop_getinfo (cyg_file *fp, int key, void *buf, int len );
typedef int cyg_fileop_setinfo (cyg_file *fp, int key, void *buf, int len );

typedef struct CYG_FILEOPS_TAG
{
    cyg_fileop_read     *fo_read;
    cyg_fileop_write    *fo_write;
    cyg_fileop_lseek    *fo_lseek;
    cyg_fileop_ioctl    *fo_ioctl;
    cyg_fileop_select   *fo_select;
    cyg_fileop_fsync    *fo_fsync;
    cyg_fileop_close    *fo_close;
    cyg_fileop_fstat    *fo_fstat;
    cyg_fileop_getinfo  *fo_getinfo;
    cyg_fileop_setinfo  *fo_setinfo;
}cyg_fileops;

typedef int cyg_fsop_open     ( struct super_block *sb, cyg_dir *dir, const char *name,
                                    int mode,  cyg_file *fte );
typedef int cyg_fsop_unlink( struct super_block *sb, cyg_dir *dir, const char *name );
typedef int cyg_fsop_mkdir    ( struct super_block *sb, cyg_dir *dir, const char *name );
typedef int cyg_fsop_rmdir    ( struct super_block *sb, cyg_dir *dir, const char *name );
typedef int cyg_fsop_rename( struct super_block *sb, cyg_dir *dir1, const char *name1,
                                    cyg_dir *dir2, const char *name2 );
typedef int cyg_fsop_link     ( struct super_block *sb, cyg_dir *dir1, const char *name1,
                                    cyg_dir *dir2, const char *name2, int type );
typedef int cyg_fsop_opendir( struct super_block *sb, cyg_dir *dir, const char *name,
                                    cyg_file *fte );
typedef int cyg_fsop_chdir    ( struct super_block *sb, cyg_dir *dir, const char *name,
                                    cyg_dir **dir_out );
typedef int cyg_fsop_stat     ( struct super_block *sb, cyg_dir *dir, const char *name,
                                    cyg_stat *buf);
typedef int cyg_fsop_getinfo  ( struct super_block *sb, cyg_dir *dir, const char *name,
                                    int key, void *buf, int len );
typedef int cyg_fsop_setinfo  ( struct super_block *sb, cyg_dir *dir, const char *name,
                                    int key, void *buf, int len );

typedef struct CYG_FSOPS_TAG
{
    cyg_fsop_open       *open;
    cyg_fsop_unlink     *unlink;
    cyg_fsop_mkdir      *mkdir;
    cyg_fsop_rmdir      *rmdir;
    cyg_fsop_rename     *rename;
    cyg_fsop_link       *link;
    cyg_fsop_opendir    *opendir;
    cyg_fsop_chdir      *chdir;
    cyg_fsop_stat       *stat;
    cyg_fsop_getinfo    *getinfo;
    cyg_fsop_setinfo    *setinfo;
}cyg_fsops;

//-----------------------------------------------------------------------------
// Macro to define an initialized fstab entry

#define CYG_FSOPS_DEFINE( _l, _open, _unlink,  _mkdir, _rmdir, _rename, _link,   \
                     _opendir, _chdir, _stat, _getinfo, _setinfo)       \
cyg_fsops _l =                  \
{                                                                       \
    _open,                                                              \
    _unlink,                                                            \
    _mkdir,                                                             \
    _rmdir,                                                             \
    _rename,                                                            \
    _link,                                                              \
    _opendir,                                                           \
    _chdir,                                                             \
    _stat,                                                              \
    _getinfo,                                                           \
    _setinfo                                                            \
};


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __JFFS2_FILEIO_H__ */

