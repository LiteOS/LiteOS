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

/*
 * JFFS2 -- Journalling Flash File System, Version 2.
 *
 * Copyright (C) 2001-2003 Free Software Foundation, Inc.
 *
 * Created by Dominic Ostrowski <dominic.ostrowski@3glab.com>
 * Contributors: David Woodhouse, Nick Garnett, Richard Panton.
 *
 * For licensing information, see the file 'LICENCE' in this directory.
 *
 */

#include <jffs2_kernel.h>
#include <jffs2_page.h>
#include <jffs2_crc32.h>
#include "jffs2_nodelist.h"
#include "jffs2_compr.h"
#include "jffs2_stat.h"
#include <jffs2_fileio.h>
#include "jffs2_mtd.h"


int jffs2_mount_ret = ENOERR;

int jffs2_do_mount(struct super_block *jffs2_sb, struct mtd_info *mtd, int flags)
{
    return jffs2_mount_ret;
}

int jffs2_do_umount(struct super_block *jffs2_sb)
{
    return ENOERR;
}

// -------------------------------------------------------------------------
// jffs2_open()
// Open a file for reading or writing.

int jffs2_open_ret = ENOERR;

int jffs2_open(struct super_block *sb, cyg_dir *dir, const char *name,
               int mode, cyg_file *file)
{
    return jffs2_open_ret;
}

static int jffs2_ops_unlink(struct super_block *sb, cyg_dir *dir, const char *name)
{
   return ENOERR;
}

// -------------------------------------------------------------------------
// jffs2_ops_mkdir()
// Create a new directory.

static int jffs2_ops_mkdir(struct super_block *sb, cyg_dir *dir, const char *name)
{
    return ENOERR;
}

// -------------------------------------------------------------------------
// jffs2_ops_rmdir()
// Remove a directory.

static int jffs2_ops_rmdir(struct super_block *sb, cyg_dir *dir, const char *name)
{
   return ENOERR;
}

// -------------------------------------------------------------------------
// jffs2_ops_rename()
// Rename a file/dir.

static int jffs2_ops_rename(struct super_block *sb, cyg_dir *dir1,
                           const char *name1, cyg_dir *dir2, const char *name2)
{
    return ENOERR;
}

// -------------------------------------------------------------------------
// jffs2_ops_link()
// Make a new directory entry for a file.

static int jffs2_ops_link(struct super_block *sb, cyg_dir *dir1, const char *name1,
                         cyg_dir *dir2, const char *name2, int type)
{
    return ENOERR;
}

// -------------------------------------------------------------------------
// jffs2_opendir()
// Open a directory for reading.

int jffs2_opendir_ret = ENOERR;

static int jffs2_opendir(struct super_block *sb, cyg_dir *dir, const char *name,
                         cyg_file *file)
{
    return jffs2_opendir_ret;
}

static int jffs2_chdir(struct super_block *sb, cyg_dir *dir, const char *name,
                       cyg_dir **dir_out)
{
    return ENOERR;
}

int jffs2_stat_flag = 0;

static int jffs2_stat(struct super_block *sb, cyg_dir *dir, const char *name,
                      cyg_stat *buf)
{
    switch (jffs2_stat_flag)
    {
    case 0:
        buf->st_mode = (1<<3);
        break;
    case 1:
        buf->st_mode = (1<<0);
        break;
    case 2:
        buf->st_mode = 0;
        break;
    default:
        return ENOENT;
    }

    return ENOERR;
}

// -------------------------------------------------------------------------
// jffs2_getinfo()
// Getinfo. Currently only support pathconf().

static int jffs2_getinfo(struct super_block *sb, cyg_dir *dir, const char *name,
                         int key, void *buf, int len)
{
    return ENOERR;
}

static int jffs2_setinfo(struct super_block *sb, cyg_dir *dir, const char *name,
                         int key, void *buf, int len)
{
    return EINVAL;
}


int jffs2_rw_ret = ENOERR;

int jffs2_fo_read(cyg_file *fp, cyg_uio *uio)
{
    return jffs2_rw_ret;
}

static int jffs2_fo_write(cyg_file *fp, cyg_uio *uio)
{
    return jffs2_rw_ret;
}

// -------------------------------------------------------------------------
// jffs2_fo_lseek()
// Seek to a new file position.

static int jffs2_fo_lseek(cyg_file *fp, off_t *apos, int whence)
{
    return ENOERR;
}

// -------------------------------------------------------------------------
// jffs2_fo_ioctl()
// Handle ioctls. Currently none are defined.

static int jffs2_fo_ioctl(cyg_file *fp, uint32_t com, uint32_t data)
{
    return EINVAL;
}

// -------------------------------------------------------------------------
// jffs2_fo_fsync().
// Force the file out to data storage.

static int jffs2_fo_fsync(cyg_file *fp, int mode)
{
    return ENOERR;
}

// -------------------------------------------------------------------------
// jffs2_fo_close()
// Close a file. We just decrement the refcnt and let it go away if
// that is all that is keeping it here.

int jffs2_close_ret = ENOERR;

static int jffs2_fo_close(cyg_file *fp)
{
    return jffs2_close_ret;
}

// -------------------------------------------------------------------------
//jffs2_fo_fstat()
// Get file status.

static int jffs2_fo_fstat(cyg_file *fp, cyg_stat *buf)
{
    return ENOERR;
}

static int jffs2_fo_getinfo(cyg_file *fp, int key, void *buf, int len)
{
    return ENOERR;
}

// -------------------------------------------------------------------------
// jffs2_fo_setinfo()
// Set info. Nothing supported here.

static int jffs2_fo_setinfo(cyg_file *fp, int key, void *buf, int len)
{
    return ENOERR;
}

//==========================================================================
// Directory operations

static int jffs2_fo_dirread(cyg_file *fp, cyg_uio *uio)
{
    fp->f_data = fp;
    ((cyg_dirent *)uio->uio_iov->iov_base)->d_name[0] = 'a';
    return ENOERR;
}

static int jffs2_fo_dirlseek(cyg_file *fp, off_t *pos, int whence)
{
    return ENOERR;
}


//==========================================================================
// Default functions

int cyg_fileio_enosys()
{
    return ENOSYS;
}
int cyg_fileio_erofs()
{
    return EROFS;
}
int cyg_fileio_enoerr()
{
    return ENOERR;
}
int cyg_fileio_enotdir()
{
    return ENOTDIR;
}

int cyg_fileio_seltrue (cyg_file *fp, int which, uint32_t info)
{
    return 1;
}

// -------------------------------------------------------------------------
// File operations.
// This set of file operations are used for normal open files.

cyg_fileops jffs2_fileops = {
    jffs2_fo_read,
    jffs2_fo_write,
    jffs2_fo_lseek,
    jffs2_fo_ioctl,
    cyg_fileio_seltrue,
    jffs2_fo_fsync,
    jffs2_fo_close,
    jffs2_fo_fstat,
    jffs2_fo_getinfo,
    jffs2_fo_setinfo
};

// -------------------------------------------------------------------------
// Directory file operations.
// This set of operations are used for open directories. Most entries
// point to error-returning stub functions. Only the read, lseek and
// close entries are functional.

cyg_fileops jffs2_dirops = {
    jffs2_fo_dirread,
    (cyg_fileop_write *) cyg_fileio_enosys,
    jffs2_fo_dirlseek,
    (cyg_fileop_ioctl *) cyg_fileio_enosys,
    cyg_fileio_seltrue,
    (cyg_fileop_fsync *) cyg_fileio_enosys,
    jffs2_fo_close,
    (cyg_fileop_fstat *) cyg_fileio_enosys,
    (cyg_fileop_getinfo *) cyg_fileio_enosys,
    (cyg_fileop_setinfo *) cyg_fileio_enosys
};

CYG_FSOPS_DEFINE(jffs2_fsops,
        jffs2_open,
        jffs2_ops_unlink,
        jffs2_ops_mkdir,
        jffs2_ops_rmdir,
        jffs2_ops_rename,
        jffs2_ops_link,
        jffs2_opendir,
        jffs2_chdir, jffs2_stat, jffs2_getinfo, jffs2_setinfo);
