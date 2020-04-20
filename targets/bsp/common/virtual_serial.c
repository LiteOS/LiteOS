/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Virtual Serial Implementation
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
 * --------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 * --------------------------------------------------------------------------- */

#include "virtual_serial.h"
#include "fcntl.h"
#ifdef CONFIG_FILE_MODE
#include "stdarg.h"
#endif
#ifdef LOSCFG_FS_VFS
#include "inode/inode.h"
#include "console.h"
#endif
#include "uart.h"
#include "local.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

STATIC volatile UINT32 g_serialType = 0;
STATIC struct file g_serialFilep;

UINT32 SerialTypeGet(VOID)
{
    return g_serialType;
}

STATIC VOID SerialTypeSet(const CHAR *deviceName)
{
    if (!strncmp(deviceName, SERIAL_UARTDEV, strlen(SERIAL_UARTDEV))) {
        g_serialType = SERIAL_TYPE_UART_DEV;
    } else if (!strncmp(deviceName, SERIAL_TTYGS0, strlen(SERIAL_TTYGS0))) {
        g_serialType = SERIAL_TYPE_USBTTY_DEV;
    }
}

STATIC INT32 SerialOpen(struct file *filep)
{
    INT32 ret;
    struct file *privFilep = NULL;
    const struct file_operations_vfs *fileOps = NULL;

    ret = GetFilepOps(filep, &privFilep, &fileOps);
    if (ret != ENOERR) {
        ret = EINVAL;
        goto ERROUT;
    }

    ret = FilepOpen(privFilep, fileOps);
    if (ret < 0) {
        ret = EPERM;
        goto ERROUT;
    }

    if (g_serialType == SERIAL_TYPE_UART_DEV) {
        HalIrqUnmask(NUM_HAL_INTERRUPT_UART);
    }
    return ENOERR;

ERROUT:
    set_errno(ret);
    return VFS_ERROR;
}

STATIC INT32 SerialClose(struct file *filep)
{
    (VOID)filep;

    if (g_serialType == SERIAL_TYPE_UART_DEV) {
        HalIrqMask(NUM_HAL_INTERRUPT_UART);
    }
#if defined(LOSCFG_DRIVERS_USB_SERIAL_GADGET) || defined(LOSCFG_DRIVERS_USB_ETH_SER_GADGET)
    else if (g_serialType == SERIAL_TYPE_USBTTY_DEV) {
        userial_mask_set(0);
    }
#endif

    return ENOERR;
}

STATIC ssize_t SerialRead(struct file *filep, CHAR *buffer, size_t bufLen)
{
    INT32 ret;
    struct file *privFilep = NULL;
    const struct file_operations_vfs *fileOps = NULL;

    ret = GetFilepOps(filep, &privFilep, &fileOps);
    if (ret != ENOERR) {
        ret = -EINVAL;
        goto ERROUT;
    }

    ret = FilepRead(privFilep, fileOps, buffer, bufLen);
    if (ret < 0) {
        goto ERROUT;
    }
    return ret;

ERROUT:
    set_errno(-ret);
    return VFS_ERROR;
}

/* Note: do not add print function in this module! */
STATIC ssize_t SerialWrite(FAR struct file *filep, FAR const CHAR *buffer, size_t bufLen)
{
    INT32 ret;
    struct file *privFilep = NULL;
    const struct file_operations_vfs *fileOps = NULL;

    ret = GetFilepOps(filep, &privFilep, &fileOps);
    if (ret != ENOERR) {
        ret = -EINVAL;
        goto ERROUT;
    }

    ret = FilepWrite(privFilep, fileOps, buffer, bufLen);
    if (ret < 0) {
        goto ERROUT;
    }
    return ret;

ERROUT:
    set_errno(-ret);
    return VFS_ERROR;
}

STATIC INT32 SerialIoctl(struct file *filep, INT32 cmd, unsigned long arg)
{
    INT32 ret;
    struct file *privFilep = NULL;
    const struct file_operations_vfs *fileOps = NULL;

    ret = GetFilepOps(filep, &privFilep, &fileOps);
    if (ret != ENOERR) {
        ret = -EINVAL;
        goto ERROUT;
    }

    ret = FilepIoctl(privFilep, fileOps, cmd, arg);
    if (ret < 0) {
        goto ERROUT;
    }
    return ret;

ERROUT:
    set_errno(-ret);
    return VFS_ERROR;
}

STATIC INT32 SerialPoll(struct file *filep, poll_table *fds)
{
    INT32 ret;
    struct file *privFilep = NULL;
    const struct file_operations_vfs *fileOps = NULL;

    ret = GetFilepOps(filep, &privFilep, &fileOps);
    if (ret != ENOERR) {
        ret = -EINVAL;
        goto ERROUT;
    }
    ret = FilepPoll(privFilep, fileOps, fds);
    if (ret < 0) {
        goto ERROUT;
    }
    return ret;

ERROUT:
    set_errno(-ret);
    return VFS_ERROR;
}

STATIC const struct file_operations_vfs g_serialDevOps = {
    SerialOpen,  /* open */
    SerialClose, /* close */
    SerialRead,  /* read */
    SerialWrite,
    NULL,
    SerialIoctl,
#ifndef CONFIG_DISABLE_POLL
    SerialPoll,
#endif
    NULL
};

INT32 virtual_serial_init(const CHAR *deviceName)
{
    INT32 ret;
    CHAR *fullpath = NULL;
    struct inode *inode = NULL;

    if (deviceName == NULL) {
        ret = EINVAL;
        goto ERROUT;
    }

    SerialTypeSet(deviceName);

    ret = vfs_normalize_path(NULL, deviceName, &fullpath);
    if (ret < 0) {
        ret = EINVAL;
        goto ERROUT;
    }

    inode = inode_find(fullpath, NULL);
    if (inode == NULL) {
        ret = ENOENT;
        goto ERROUT_WITH_FULLPATH;
    }

    (VOID)memset_s(&g_serialFilep, sizeof(struct file), 0, sizeof(struct file));
    g_serialFilep.f_oflags = O_RDWR;
    g_serialFilep.f_inode = inode;

    if (inode->u.i_ops->open != NULL) {
        (VOID)inode->u.i_ops->open(&g_serialFilep);
    } else {
        ret = EFAULT;
        inode_release(inode);
        goto ERROUT_WITH_FULLPATH;
    }

    (VOID)register_driver(SERIAL, &g_serialDevOps, DEFFILEMODE, &g_serialFilep);
    inode_release(inode);
    free(fullpath);
    return ENOERR;

ERROUT_WITH_FULLPATH:
    free(fullpath);
ERROUT:
    set_errno(ret);
    return VFS_ERROR;
}

INT32 virtual_serial_deinit(VOID)
{
    INT32 ret;
    struct file *filep = NULL;
    struct inode *inode = NULL;
    CHAR *fullpath = NULL;

    /* It's a process opposite virtual_serial_init */
    ret = vfs_normalize_path(NULL, SERIAL, &fullpath);
    if (ret < 0) {
        ret = EINVAL;
        goto ERROUT;
    }

    inode = inode_find(fullpath, NULL);
    if (inode == NULL) {
        ret = ENOENT;
        goto ERROUT_WITH_FULLPATH;
    }

    filep = inode->i_private;
    if ((filep != NULL) && (inode->u.i_ops != NULL)) {
        (VOID)inode->u.i_ops->close(filep);     /* close filep */
        inode->i_private = NULL;
    } else {
        ret = EBADF;
        goto ERROUT_WITH_INODE;
    }
    inode_release(inode);
    free(fullpath);
    (VOID)unregister_driver(SERIAL);

    return ENOERR;

ERROUT_WITH_INODE:
    inode_release(inode);
ERROUT_WITH_FULLPATH:
    free(fullpath);
ERROUT:
    set_errno(ret);
    return VFS_ERROR;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif