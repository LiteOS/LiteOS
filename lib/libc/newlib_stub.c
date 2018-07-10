/*----------------------------------------------------------------------------
 * Copyright (c) <2013-2018>, <Huawei Technologies Co., Ltd>
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

#include <reent.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <sys/unistd.h>
#include <sys/time.h>
#include <string.h>
#include <sys/unistd.h>
#include <los_memory.h>
#include <los_vfs.h>
#include <stdarg.h>

#include "los_config.h"

int _execve_r(struct _reent *ptr, const char *name, char *const *argv, char *const *env)
{
    /* not support */
    ptr->_errno = ENOTSUP;
    return -1;
}

int _fcntl_r(struct _reent *ptr, int fd, int cmd, int arg)
{
    /* not support */
    ptr->_errno = ENOTSUP;
    return -1;
}

int _fork_r(struct _reent *ptr)
{
    /* not support */
    ptr->_errno = ENOTSUP;
    return -1;
}

int _getpid_r(struct _reent *ptr)
{
    /* not support */
    ptr->_errno = ENOTSUP;
    return -1;
}

int _isatty_r(struct _reent *ptr, int fd)
{
    if (fd >= 0 && fd < 3)
    {
        return 1;
    }

    /* not support */
    ptr->_errno = ENOTSUP;
    return -1;
}

int _kill_r(struct _reent *ptr, int pid, int sig)
{
    /* not support */
    ptr->_errno = ENOTSUP;
    return -1;
}

int _link_r(struct _reent *ptr, const char *old, const char *new)
{
    /* not support */
    ptr->_errno = ENOTSUP;
    return -1;
}

_off_t _lseek_r(struct _reent *ptr, int fd, _off_t pos, int whence)
{
    return los_lseek (fd, pos, whence);
}

int closedir (struct dir * dir)
{
    return los_closedir (dir);
}

struct dir * opendir (const char * path)
{
    return los_opendir (path);
}

int mkdir (const char *path, int mode)
{
    return los_mkdir (path, mode);
}

int _mkdir_r(struct _reent *ptr, const char *name, int mode)
{
    ptr->_errno = ENOTSUP;
    return 0;
}

int _open_r(struct _reent *ptr, const char *file, int flags, int mode)
{
    return los_open (file, flags);  /* mode not supported */
}

int _close_r(struct _reent *ptr, int fd)
{
    return los_close (fd);
}

_ssize_t _read_r(struct _reent *ptr, int fd, void *buf, size_t nbytes)
{
    return los_read (fd, buf, nbytes);
}

_ssize_t _write_r(struct _reent *ptr, int fd, const void *buf, size_t nbytes)
{
    return los_write (fd, buf, nbytes);
}

int _fstat_r(struct _reent *ptr, int fd, struct stat *pstat)
{
    /* not support */
    ptr->_errno = ENOTSUP;
    return -1;
}

int _rename_r(struct _reent *ptr, const char *old, const char *new)
{
    return los_rename (old, new);
}

void *_sbrk_r(struct _reent *ptr, ptrdiff_t incr)
{
    /* not support */
    ptr->_errno = ENOTSUP;
    return NULL;
}

int _stat_r(struct _reent *ptr, const char *file, struct stat *pstat)
{
    return los_stat (file, pstat);;
}

_CLOCK_T_ _times_r(struct _reent *ptr, struct tms *ptms)
{
    /* not support */
    ptr->_errno = ENOTSUP;
    return -1;
}

int _unlink_r(struct _reent *ptr, const char *file)
{
    return los_unlink (file);
}

int _wait_r(struct _reent *ptr, int *status)
{
    /* not support */
    ptr->_errno = ENOTSUP;
    return -1;
}

int _gettimeofday_r(struct _reent *ptr, struct timeval *tv, void *__tzp)
{
    /* not support */
    ptr->_errno = ENOTSUP;
    return -1;
}

void *_malloc_r(struct _reent *ptr, size_t size)
{
    return malloc(size);
}

void *_realloc_r(struct _reent *ptr, void *old, size_t newlen)
{
    return realloc (old, newlen);
}

void *_calloc_r(struct _reent *ptr, size_t size, size_t len)
{
    return calloc(size, len);
}

void _free_r(struct _reent *ptr, void *addr)
{
    free(addr);
}

void _exit(int status)
{
    while (1);
}

void _system(const char *s)
{
    return;
}

void abort(void)
{
    while (1);
}

int ioctl (int fd, unsigned long func, ...)
{
    va_list       ap;
    unsigned long arg;

    va_start (ap, func);
    arg = va_arg (ap, unsigned long);
    va_end (ap);

    return los_ioctl (fd, func, arg);
}

