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

#include <stdio.h>
#include <string.h>
#include "fs/los_vfs.h"

#if defined (__GNUC__) || defined (__CC_ARM)
#include "fs/sys/fcntl.h"
#endif

#include <los_printf.h>

#include "jffs2_types.h"
#include <jffs2_kernel.h>
#include <jffs2_page.h>
#include <jffs2_crc32.h>
#include "jffs2_nodelist.h"
#include "jffs2_compr.h"
#include "jffs2_fileio.h"

#ifndef POINTER_ASSERT
#define POINTER_ASSERT(p) \
    if(p == NULL) \
    { \
        return -1; \
    }
#endif

/* the following should be same with os_sys_stat.h */
#define JFFS2_S_IFMT	 0x000003FF
#define JFFS2_S_IFDIR	 (1<<0)
#define JFFS2_S_IFREG	 (1<<3)

extern cyg_fileops jffs2_fileops;
extern cyg_fileops jffs2_dirops;
extern cyg_fsops jffs2_fsops;

#define jffs2_open          jffs2_fsops.open
#define jffs2_file_unlink   jffs2_fsops.unlink
#define jffs2_mkdir         jffs2_fsops.mkdir
#define jffs2_rmdir         jffs2_fsops.rmdir
#define jffs2_rename        jffs2_fsops.rename
#define jffs2_link          jffs2_fsops.link
#define jffs2_opendir       jffs2_fsops.opendir
#define jffs2_chdir         jffs2_fsops.chdir
#define jffs2_ops_stat      jffs2_fsops.stat
#define jffs2_getinfo       jffs2_fsops.getinfo
#define jffs2_setinfo       jffs2_fsops.setinfo

#define jffs2_file_read     jffs2_fileops.fo_read
#define jffs2_file_write    jffs2_fileops.fo_write
#define jffs2_file_lseek    jffs2_fileops.fo_lseek
#define jffs2_file_ioctl    jffs2_fileops.fo_ioctl
#define jffs2_file_select   jffs2_fileops.fo_select
#define jffs2_file_fsync    jffs2_fileops.fo_fsync
#define jffs2_file_close    jffs2_fileops.fo_close
#define jffs2_file_fstat    jffs2_fileops.fo_fstat
#define jffs2_file_getinfo  jffs2_fileops.fo_getinfo
#define jffs2_file_setinfo  jffs2_fileops.fo_setinfo

#define jffs2_dir_read      jffs2_dirops.fo_read
#define jffs2_dir_write     jffs2_dirops.fo_write
#define jffs2_dir_lseek     jffs2_dirops.fo_lseek
#define jffs2_dir_ioctl     jffs2_dirops.fo_ioctl
#define jffs2_dir_select    jffs2_dirops.fo_select
#define jffs2_dir_fsync     jffs2_dirops.fo_fsync
#define jffs2_dir_colse     jffs2_dirops.fo_close
#define jffs2_dir_fstat     jffs2_dirops.fo_fstat
#define jffs2_dir_getinfo   jffs2_dirops.fo_getinfo
#define jffs2_dir_setinfo   jffs2_dirops.fo_setinfo


static int jffs2_result_to_los(int result)
{
    if (result < 0) return  result;
    if (result > 0) return -result;

    return 0;
}

static int jffs2_op_open (struct file *file, const char *path_in_mp, int flags)
{
    struct super_block *sb = (struct super_block *)file->f_mp->m_data;
    cyg_file *jffs2_file;
    int ret;

    POINTER_ASSERT(sb);

    jffs2_file = (cyg_file *)malloc(sizeof(cyg_file));
    if (jffs2_file == NULL)
    {
        PRINT_ERR ("fail to malloc memory in JFFS2, <malloc.c> is needed,"
                   "make sure it is added\n");
        return -1;
    }

    ret = jffs2_open(sb, 0, path_in_mp, flags, jffs2_file);
    if(ret)
    {
        free(jffs2_file);
        return jffs2_result_to_los(ret);
    }

    file->f_data = (void *)jffs2_file;

    if (flags & O_APPEND)
    {
        off_t ofs = 0;
        jffs2_file_lseek(jffs2_file, &ofs, SEEK_END);
        file->f_offset = ofs;
    }

    return 0;
}

static int jffs2_op_close (struct file *file)
{
    int result;
    cyg_file *jffs2_file = (cyg_file *)file->f_data;

    POINTER_ASSERT(jffs2_file);

    result = jffs2_file_close(jffs2_file);
    if (result)
        return jffs2_result_to_los(result);

    /* release memory */
    free(jffs2_file);
    file->f_data = NULL;
    return 0;
}

static ssize_t jffs2_op_read (struct file *file, char *buff, size_t bytes)
{
    cyg_file * jffs2_file;
    cyg_uio uio_s;
    cyg_iovec iovec;
    int char_read;
    int result;

    if (buff == NULL || bytes == 0)
        return -1;

    jffs2_file = (cyg_file *)(file->f_data);
    POINTER_ASSERT(jffs2_file);
    uio_s.uio_iov = &iovec;
    uio_s.uio_iov->iov_base = buff;
    uio_s.uio_iov->iov_len = bytes;
    uio_s.uio_iovcnt = 1; //must be 1
    //uio_s.uio_offset //not used...
    uio_s.uio_resid = uio_s.uio_iov->iov_len; //seem no use in jffs2;

    char_read = jffs2_file->f_offset; /* the current offset */
    result = jffs2_file_read(jffs2_file, &uio_s);
    if (result)
        return jffs2_result_to_los(result);

    /* update position */
    file->f_offset = jffs2_file->f_offset;
    char_read = jffs2_file->f_offset - char_read;
    return char_read;
}

static ssize_t jffs2_op_write (struct file *file, const char *buff, size_t bytes)
{
    cyg_file * jffs2_file;
    cyg_uio uio_s;
    cyg_iovec iovec;
    int char_write;
    int result;

    if (buff == NULL || bytes == 0)
        return -1;

    jffs2_file = (cyg_file *)(file->f_data);
    POINTER_ASSERT(jffs2_file);
    uio_s.uio_iov = &iovec;
    uio_s.uio_iov->iov_base = (void *)buff;
    uio_s.uio_iov->iov_len = bytes;
    uio_s.uio_iovcnt = 1; //must be 1
    //uio_s.uio_offset //not used...
    uio_s.uio_resid = uio_s.uio_iov->iov_len; //seem no use in jffs2;

    char_write = jffs2_file->f_offset;
    result = jffs2_file_write(jffs2_file, &uio_s);
    if (result)
        return jffs2_result_to_los(result);

    /* update position */
    file->f_offset = jffs2_file->f_offset;
    char_write = jffs2_file->f_offset - char_write;
    return char_write;
}

static off_t jffs2_op_lseek (struct file *file, off_t off, int whence)
{
    cyg_file * jffs2_file;
    int result;

    jffs2_file = (cyg_file *)(file->f_data);
    POINTER_ASSERT(jffs2_file);

    /* set offset as current offset */
    result = jffs2_file_lseek(jffs2_file, &off, whence); // SEEK_SET
    if (result)
        return jffs2_result_to_los(result);

    /* update file position */
    file->f_offset = off;
    return off;
}

int jffs2_op_stat (struct mount_point *mp, const char *path_in_mp, struct stat *stat)
{
    struct super_block *sb = (struct super_block *)mp->m_data;
    cyg_stat s = {0};
    int ret = 0;

    POINTER_ASSERT(sb);

    if (path_in_mp[0] == '/')
        path_in_mp++;

    ret  = jffs2_ops_stat(sb, sb->s_root, path_in_mp, &s);
    if (ret)
    {
        return jffs2_result_to_los(ret);
    }

    stat->st_dev = s.st_dev;
    stat->st_ino = s.st_ino;
    stat->st_mode = 0;
    stat->st_nlink = s.st_nlink;
    stat->st_uid = s.st_uid;
    stat->st_gid = s.st_gid;
    stat->st_rdev = 0;
    stat->st_size = s.st_size;
    stat->st_atime = s.st_atime;
    stat->st_mtime = s.st_mtime;
    stat->st_ctime = s.st_ctime;
    stat->st_blksize = 0;
    stat->st_blocks = 0;

    switch(s.st_mode & JFFS2_S_IFMT)
    {
    case JFFS2_S_IFREG:
        stat->st_mode = S_IFREG;
        break;
    case JFFS2_S_IFDIR:
        stat->st_mode = S_IFDIR;
        break;
    default:
        break;
    }

    return 0;
}

static int jffs2_op_unlink (struct mount_point *mp, const char *path_in_mp)
{
    struct super_block *sb = (struct super_block *)mp->m_data;
    int ret;
    cyg_stat s;

    POINTER_ASSERT(sb);

    /* deal path */
    if (path_in_mp[0] == '/')
        path_in_mp++;

    /* judge file type, dir is to be delete by rmdir, others by unlink */
    ret  = jffs2_ops_stat(sb, sb->s_root, path_in_mp, &s);
    if (ret)
    {
        return jffs2_result_to_los(ret);
    }

    switch(s.st_mode & JFFS2_S_IFMT)
    {
    case JFFS2_S_IFREG:
        ret = jffs2_file_unlink(sb, sb->s_root, path_in_mp);
        break;
    case JFFS2_S_IFDIR:
        ret = jffs2_rmdir(sb, sb->s_root, path_in_mp);
        break;
    default:
        /* unknown file type */
        return -1;
    }

    return jffs2_result_to_los(ret);
}

static int jffs2_op_rename (struct mount_point *mp, const char *path_in_mp_old,
                             const char *path_in_mp_new)
{
    struct super_block *sb = (struct super_block *)mp->m_data;
    int ret;

    POINTER_ASSERT(sb);

    if (*path_in_mp_old == '/')
        path_in_mp_old += 1;
    if (*path_in_mp_new == '/')
        path_in_mp_new += 1;

    ret = jffs2_rename(sb, sb->s_root, path_in_mp_old,
                            sb->s_root, path_in_mp_new);

    return jffs2_result_to_los(ret);
}

static int jffs2_op_sync (struct file *file)
{
    return 0;
}

static int jffs2_op_opendir (struct dir *dir, const char *path)
{
    struct super_block *sb = (struct super_block *)dir->d_mp->m_data;
    cyg_file *jffs2_file;
    int ret;

    POINTER_ASSERT(sb);

    jffs2_file = (cyg_file *)malloc(sizeof(cyg_file));
    if (jffs2_file == NULL)
    {
        PRINT_ERR ("fail to malloc memory in JFFS2, <malloc.c> is needed,"
                   "make sure it is added\n");
        return -1;
    }

    ret = jffs2_opendir(sb, sb->s_root, path, jffs2_file);
    if(ret)
    {
        free(jffs2_file);
        return jffs2_result_to_los(ret);
    }

#ifdef  CONFIG_JFFS2_NO_RELATIVEDIR
    jffs2_file->f_offset = 2;
#endif

    dir->d_data = jffs2_file;

    return 0;
}

static int jffs2_op_readdir (struct dir *dir, struct dirent *dent)
{
    struct super_block *sb;
    cyg_file * jffs2_file;
    cyg_uio uio_s;
    cyg_iovec iovec;
    cyg_dirent jffs2_d;
    struct dirent * d;
#if !defined (CYGPKG_FS_JFFS2_RET_DIRENT_DTYPE)
    cyg_stat s;
    struct inode *d_inode = NULL;
#endif
    char namelen;
    off_t d_size = 0;
    int ret;

    sb = (struct super_block *)(dir->d_mp->m_data);
    jffs2_file = (cyg_file*)(dir->d_data);
    if(sb == NULL || jffs2_file == NULL)
        return -1;

    //set jffs2_d
    memset(&jffs2_d, 0, sizeof(cyg_dirent));
    //set CYG_UIO_TAG uio_s
    uio_s.uio_iov = &iovec;
    uio_s.uio_iov->iov_base = &jffs2_d;
    uio_s.uio_iov->iov_len = sizeof(cyg_dirent);;
    uio_s.uio_iovcnt = 1; //must be 1
    uio_s.uio_offset = 0;//not used...
    uio_s.uio_resid = uio_s.uio_iov->iov_len; //seem no use in jffs2;

    d = dent;
    ret = jffs2_dir_read(jffs2_file, &uio_s);
    if (ret || jffs2_d.d_name[0] == 0)
        return -1;

#if defined (CYGPKG_FS_JFFS2_RET_DIRENT_DTYPE)
    switch(jffs2_d.d_type & JFFS2_S_IFMT)
    {
    case JFFS2_S_IFDIR:
		d->type = VFS_TYPE_DIR;
		break;
    default:
		d->type = VFS_TYPE_FILE;
		break;
    }
#else
    d_inode = (struct inode *)jffs2_file->f_data;
    if(d_inode == NULL) return -1;

    ret = jffs2_ops_stat(sb, d_inode, jffs2_d.d_name, &s);
    if (ret)
        return jffs2_result_to_los(ret);

    /* convert to dfs stat structure */
    switch(s.st_mode & JFFS2_S_IFMT)
    {
    case JFFS2_S_IFDIR:
        d->type = VFS_TYPE_DIR;
        break;
    default:
        d->type = VFS_TYPE_FILE;
        break;
    }
    d_size = s.st_size;
#endif

    /* write the rest fields of struct dirent* dirp  */
    namelen = min(strlen(jffs2_d.d_name)+1, sizeof(d->name));
    strncpy(d->name, jffs2_d.d_name, namelen);
    d->size = d_size;

    return 0;
}

static int jffs2_op_closedir (struct dir *dir)
{
    cyg_file *jffs2_file = (cyg_file *)dir->d_data;
    int ret;

    POINTER_ASSERT(jffs2_file);

    ret = jffs2_dir_colse(jffs2_file);
    if (ret)
        return jffs2_result_to_los(ret);

    free(jffs2_file);
    dir->d_data = NULL;

    return 0;
}

static int jffs2_op_mkdir(struct mount_point *mp, const char *path)
{
    struct super_block *sb = (struct super_block *)mp->m_data;
    int ret;

    POINTER_ASSERT(sb);

    ret = jffs2_mkdir(sb, sb->s_root, path);
    return jffs2_result_to_los(ret);
}

static struct file_ops jffs2_ops =
{
    jffs2_op_open,
    jffs2_op_close,
    jffs2_op_read,
    jffs2_op_write,
    jffs2_op_lseek,
    jffs2_op_stat,
    jffs2_op_unlink,
    jffs2_op_rename,
    NULL,               /* ioctl not supported for now */
    jffs2_op_sync,
    jffs2_op_opendir,
    jffs2_op_readdir,
    jffs2_op_closedir,
    jffs2_op_mkdir
};

static struct file_system jffs2_fs =
{
    "jffs2",
    &jffs2_ops,
    NULL,
    0
};

int jffs2_init (void)
{
    static int jffs2_inited = FALSE;

    if (jffs2_inited)
    {
        return LOS_OK;
    }

    if (los_vfs_init () != LOS_OK)
    {
        return LOS_NOK;
    }

    if (los_fs_register (&jffs2_fs) != LOS_OK)
    {
        PRINT_ERR ("failed to register fs!\n");
        return LOS_NOK;
    }

    jffs2_inited = TRUE;

    PRINT_INFO ("register fatfs done!\n");

    return LOS_OK;
}

static struct super_block *jffs2_sb_ptr = NULL;

int jffs2_mount(const char *path, struct mtd_info *mtd)
{
    struct super_block *jffs2_sb = NULL;
    int ret = -1;

    if(path == NULL || mtd == NULL)
        return ret;

    jffs2_sb = (struct super_block *)malloc(sizeof (struct super_block));
    if(NULL == jffs2_sb)
    {
        PRINT_ERR ("fail to malloc memory in JFFS2, <malloc.c> is needed,"
                           "make sure it is added\n");
        goto err;
    }
    memset(jffs2_sb, 0, sizeof (struct super_block));

    ret = jffs2_do_mount(jffs2_sb, mtd, 0);
    if(ret)
    {
        PRINT_ERR("failed to mount jffs2, ret=%d!\n", ret);
        goto err_free;
    }

    ret = los_fs_mount ("jffs2", path, jffs2_sb);

    if (ret == LOS_OK)
    {
        jffs2_sb_ptr = jffs2_sb;
        PRINT_INFO ("jffs2 mount at %s done!\n", path);
        return LOS_OK;
    }

    PRINT_ERR ("failed to mount!\n");

err_free:
    if(jffs2_sb)
        free(jffs2_sb);
err:
    return ret;
}

int jffs2_unmount(const char *path)
{
    if (jffs2_sb_ptr)
    {
        jffs2_do_umount(jffs2_sb_ptr);
        free(jffs2_sb_ptr);
        jffs2_sb_ptr = NULL;
    }

    los_fs_unmount(path);

    return 0;
}
