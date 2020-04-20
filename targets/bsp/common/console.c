/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: System Console Implementation
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

#include "console.h"
#include "fcntl.h"
#ifdef CONFIG_FILE_MODE
#include "stdarg.h"
#endif
#include "unistd.h"
#include "securec.h"
#include "inode/inode.h"
#ifdef LOSCFG_SHELL_DMESG
#include "dmesg_pri.h"
#endif
#ifdef LOSCFG_SHELL
#include "shcmd.h"
#include "shell_pri.h"
#endif
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#define EACH_CHAR 1
/* Inter-module variable */
extern UINT32 g_uart_fputc_en;
STATIC UINT32 ConsoleSendTask(UINTPTR param);
STATIC UINT8 g_taskConsoleIDArray[LOSCFG_BASE_CORE_TSK_CONFIG];


STATIC SPIN_LOCK_INIT(g_consoleSpin);

#define SHELL_TASK_PRIORITY       9
#define CONSOLE_CIRBUF_EVENT      0x02U
#define CONSOLE_SEND_TASK_EXIT    0x04U
#define CONSOLE_SEND_TASK_RUNNING 0x10U

CONSOLE_CB *g_console[CONSOLE_NUM];
#define MIN(a, b) ((a) < (b) ? (a) : (b))

/*
 * acquire uart driver function and filep of /dev/console,
 * then store uart driver function in *filepOps
 * and store filep of /dev/console in *privFilep.
 */
INT32 GetFilepOps(const struct file *filep, struct file **privFilep, const struct file_operations_vfs **filepOps)
{
    INT32 ret;

    if ((filep == NULL) || (filep->f_inode == NULL) || (filep->f_inode->i_private == NULL)) {
        ret = EINVAL;
        goto ERROUT;
    }

    /* to find console device's filep(now it is *privFilep) throught i_private */
    *privFilep = (struct file *)filep->f_inode->i_private;
    if (((*privFilep)->f_inode == NULL) || ((*privFilep)->f_inode->u.i_ops == NULL)) {
        ret = EINVAL;
        goto ERROUT;
    }

    /* to find uart driver operation function throutht u.i_opss */
    *filepOps = (*privFilep)->f_inode->u.i_ops;

    return ENOERR;
ERROUT:
    set_errno(ret);
    return VFS_ERROR;
}

INT32 ConsoleTcGetAttr(INT32 fd, struct termios *termios)
{
    struct file *filep = NULL;
    CONSOLE_CB *consoleCB = NULL;

    if ((fd >= STDIN_FILENO) && (fd <= STDERR_FILENO)) {
        fd = ConsoleUpdateFd();
        if (fd < STDIN_FILENO) {
            return -EBADF;
        }
    }

    filep = fs_getfilep(fd);
    if (filep == NULL) {
        return -EPERM;
    }

    consoleCB = (CONSOLE_CB *)filep->f_priv;
    if (consoleCB == NULL) {
        return -EFAULT;
    }

    termios->c_lflag = consoleCB->consoleTermios.c_lflag;
    return LOS_OK;
}

INT32 ConsoleTcSetAttr(INT32 fd, INT32 actions, const struct termios *termios)
{
    struct file *filep = NULL;
    CONSOLE_CB *consoleCB = NULL;

    (VOID)actions;
    if ((fd >= STDIN_FILENO) && (fd <= STDERR_FILENO)) {
        fd = ConsoleUpdateFd();
        if (fd < STDIN_FILENO) {
            return -EBADF;
        }
    }

    filep = fs_getfilep(fd);
    if (filep == NULL) {
        return -EPERM;
    }

    consoleCB = (CONSOLE_CB *)filep->f_priv;
    if (consoleCB == NULL) {
        return -EFAULT;
    }
    consoleCB->consoleTermios.c_lflag = termios->c_lflag;
    return LOS_OK;
}

STATIC UINT32 ConsoleRefcountGet(const CONSOLE_CB *consoleCB)
{
    return consoleCB->refCount;
}

STATIC VOID ConsoleRefcountSet(CONSOLE_CB *consoleCB, BOOL flag)
{
    if (flag == TRUE) {
        ++(consoleCB->refCount);
    } else {
        --(consoleCB->refCount);
    }
}

BOOL IsConsoleOccupied(const CONSOLE_CB *consoleCB)
{
    if (ConsoleRefcountGet(consoleCB) != FALSE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

STATIC INT32 ConsoleCtrlRightsCapture(CONSOLE_CB *consoleCB)
{
    (VOID)LOS_SemPend(consoleCB->consoleSem, LOS_WAIT_FOREVER);
    if (ConsoleRefcountGet(consoleCB) == 0) {
        (VOID)LOS_TaskSuspend(consoleCB->shellEntryId);
    }
    ConsoleRefcountSet(consoleCB, TRUE);
    (VOID)LOS_SemPost(consoleCB->consoleSem);
    return LOS_OK;
}

STATIC INT32 ConsoleCtrlRightsRelease(CONSOLE_CB *consoleCB)
{
    (VOID)LOS_SemPend(consoleCB->consoleSem, LOS_WAIT_FOREVER);

    if (ConsoleRefcountGet(consoleCB) == 0) {
        PRINT_ERR("console is free\n");
        (VOID)LOS_SemPost(consoleCB->consoleSem);
        return LOS_NOK;
    } else {
        ConsoleRefcountSet(consoleCB, FALSE);
        if (ConsoleRefcountGet(consoleCB) == 0) {
            (VOID)LOS_TaskResume(consoleCB->shellEntryId);
        }
    }
    (VOID)LOS_SemPost(consoleCB->consoleSem);
    return LOS_OK;
}

STATIC CONSOLE_CB *OsGetConsoleByDevice(const CHAR *deviceName)
{
    INT32 ret;
    CHAR *fullpath = NULL;
    struct inode *inode = NULL;

    ret = vfs_normalize_path(NULL, deviceName, &fullpath);
    if (ret < 0) {
        set_errno(EINVAL);
        return NULL;
    }

    inode = inode_find(fullpath, NULL);
    free(fullpath);
    if (inode == NULL) {
        set_errno(ENOENT);
        return NULL;
    }

    if (g_console[CONSOLE_SERIAL - 1]->devInode == inode) {
        inode_release(inode);
        return g_console[CONSOLE_SERIAL - 1];
    } else if (g_console[CONSOLE_TELNET - 1]->devInode == inode) {
        inode_release(inode);
        return g_console[CONSOLE_TELNET - 1];
    } else {
        inode_release(inode);
        set_errno(ENOENT);
        return NULL;
    }
}

STATIC INT32 OsGetConsoleID(const CHAR *deviceName)
{
    if ((deviceName != NULL) &&
        (strlen(deviceName) == strlen(SERIAL)) &&
        (!strncmp(deviceName, SERIAL, strlen(SERIAL)))) {
        return CONSOLE_SERIAL;
    }
#ifdef LOSCFG_NET_TELNET
    else if ((deviceName != NULL) &&
             (strlen(deviceName) == strlen(TELNET)) &&
             (!strncmp(deviceName, TELNET, strlen(TELNET)))) {
        return CONSOLE_TELNET;
    }
#endif
    return -1;
}

STATIC INT32 OsConsoleFullpathToID(const CHAR *fullpath)
{
#define CONSOLE_SERIAL_1 "/dev/console1"
#define CONSOLE_TELNET_2 "/dev/console2"

    size_t len;

    if (fullpath == NULL) {
        return -1;
    }

    len = strlen(fullpath);
    if ((len == strlen(CONSOLE_SERIAL_1)) &&
        (!strncmp(fullpath, CONSOLE_SERIAL_1, strlen(CONSOLE_SERIAL_1)))) {
        return CONSOLE_SERIAL;
    }
#ifdef LOSCFG_NET_TELNET
    else if ((len == strlen(CONSOLE_TELNET_2)) &&
             (!strncmp(fullpath, CONSOLE_TELNET_2, strlen(CONSOLE_TELNET_2)))) {
        return CONSOLE_TELNET;
    }
#endif
    return -1;
}

STATIC BOOL ConsoleFifoEmpty(const CONSOLE_CB *console)
{
    if (console->fifoOut == console->fifoIn) {
        return TRUE;
    }
    return FALSE;
}

STATIC VOID ConsoleFifoClearup(CONSOLE_CB *console)
{
    console->fifoOut = 0;
    console->fifoIn = 0;
    (VOID)memset_s(console->fifo, CONSOLE_FIFO_SIZE, 0, CONSOLE_FIFO_SIZE);
}

STATIC VOID ConsoleFifoLenUpdate(CONSOLE_CB *console)
{
    console->currentLen = console->fifoIn - console->fifoOut;
}

STATIC INT32 ConsoleReadFifo(CHAR *buffer, CONSOLE_CB *console, size_t bufLen)
{
    INT32 ret;
    UINT32 readNum;

    readNum = MIN(bufLen, console->currentLen);
    ret = memcpy_s(buffer, bufLen, console->fifo + console->fifoOut, readNum);
    if (ret != EOK) {
        PRINTK("%s,%d memcpy_s failed\n", __FUNCTION__, __LINE__);
        return -1;
    }
    console->fifoOut += readNum;
    if (ConsoleFifoEmpty(console)) {
        ConsoleFifoClearup(console);
    }
    ConsoleFifoLenUpdate(console);
    return (INT32)readNum;
}

INT32 FilepOpen(struct file *filep, const struct file_operations_vfs *fops)
{
    INT32 ret;
    if (fops->open == NULL) {
        return -EFAULT;
    }

    /*
     * adopt uart open function to open filep (filep is
     * corresponding to filep of /dev/console)
     */
    ret = fops->open(filep);
    if (ret < 0) {
        return -EPERM;
    }
    return ret;
}

STATIC INLINE VOID UserEndOfRead(CONSOLE_CB *consoleCB, struct file *filep,
                                 const struct file_operations_vfs *fops)
{
    CHAR ch;
    if (consoleCB->consoleTermios.c_lflag & ECHO) {
        ch = '\r';
        (VOID)fops->write(filep, &ch, 1);
    }
    consoleCB->fifo[consoleCB->fifoIn++] = '\n';
    consoleCB->fifo[consoleCB->fifoIn] = '\0';
    consoleCB->currentLen = consoleCB->fifoIn;
}

STATIC INT32 UserFilepRead(CONSOLE_CB *consoleCB, struct file *filep, const struct file_operations_vfs *fops,
                           CHAR *buffer, size_t bufLen)
{
    INT32 ret;
    CHAR ch;

    if (fops->read == NULL) {
        return -EFAULT;
    }

    /*
     * adopt uart read function to read data from filep
     * and write data to buffer (filep is
     * corresponding to filep of /dev/console)
     */
    if ((consoleCB->consoleTermios.c_lflag & ICANON) == 0) {
        ret = fops->read(filep, buffer, bufLen);
        if (ret < 0) {
            return -EPERM;
        }
        return ret;
    }
    /* store data to console buffer,  read data and stored data into console fifo */
    if (consoleCB->currentLen == 0) {
        while (1) {
            ret = fops->read(filep, &ch, EACH_CHAR);
            if (ret <= 0) {
                return ret;
            }

            if (ch == '\r') {
                ch = '\n';
            }

            if (consoleCB->consoleTermios.c_lflag & ECHO) {
                (VOID)fops->write(filep, &ch, EACH_CHAR);
            }

            /*
             * store what you input
             * 3 : fifoIn should less than (CONSOLE_FIFO_SIZE - EACH_CHAR) - 2(its space of '\n' and '\0')
             */
            if ((ret == EACH_CHAR) && (ch != '\n') && (consoleCB->fifoIn <= (CONSOLE_FIFO_SIZE - 3))) {
                consoleCB->fifo[consoleCB->fifoIn] = (UINT8)ch;
                consoleCB->fifoIn++;
            }

            /* return what you input */
            if (ch == '\n') {
                UserEndOfRead(consoleCB, filep, fops);
                ret = ConsoleReadFifo(buffer, consoleCB, bufLen);
                break;
            }
        }
    } else {
        /* if data is already in console fifo, we returen them immediately */
        ret = ConsoleReadFifo(buffer, consoleCB, bufLen);
    }

    return ret;
}

INT32 FilepRead(struct file *filep, const struct file_operations_vfs *fops, CHAR *buffer, size_t bufLen)
{
    INT32 ret;
    if (fops->read == NULL) {
        return -EFAULT;
    }
    /*
     * adopt uart read function to read data from filep
     * and write data to buffer (filep is
     * corresponding to filep of /dev/console)
     */
    ret = fops->read(filep, buffer, bufLen);
    if (ret < 0) {
        return -EPERM;
    }
    return ret;
}

INT32 FilepWrite(struct file *filep, const struct file_operations_vfs *fops, const CHAR *buffer, size_t bufLen)
{
    INT32 ret;
    if (fops->write == NULL) {
        return -EFAULT;
    }

    ret = fops->write(filep, buffer, bufLen);
    if (ret < 0) {
        return -EPERM;
    }
    return ret;
}

INT32 FilepClose(struct file *filep, const struct file_operations_vfs *fops)
{
    INT32 ret;
    if ((fops == NULL) || (fops->close == NULL)) {
        return -EFAULT;
    }

    /*
     * adopt uart close function to open filep (filep is
     * corresponding to filep of /dev/console)
     */
    ret = fops->close(filep);
    if (ret < 0) {
        return -EPERM;
    }
    return ret;
}

INT32 FilepIoctl(struct file *filep, const struct file_operations_vfs *fops, INT32 cmd, unsigned long arg)
{
    INT32 ret;
    if (fops->ioctl == NULL) {
        return -EFAULT;
    }

    ret = fops->ioctl(filep, cmd, arg);
    if (ret < 0) {
        return -EPERM;
    }
    return ret;
}

INT32 FilepPoll(struct file *filep, const struct file_operations_vfs *fops, poll_table *fds)
{
    INT32 ret;
    if (fops->poll == NULL) {
        return -EFAULT;
    }

    /*
     * adopt uart poll function to poll filep (filep is
     * corresponding to filep of /dev/serial)
     */
    ret = fops->poll(filep, fds);
    if (ret < 0) {
        return -EPERM;
    }
    return ret;
}

STATIC INT32 ConsoleOpen(struct file *filep)
{
    INT32 ret;
    UINT32 consoleID;
    struct file *privFilep = NULL;
    const struct file_operations_vfs *fileOps = NULL;

    consoleID = (UINT32)OsConsoleFullpathToID(filep->f_path);
    if (consoleID == (UINT32)-1) {
        ret = EPERM;
        goto ERROUT;
    }
    filep->f_priv = g_console[consoleID - 1];

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
    return ENOERR;

ERROUT:
    set_errno(ret);
    return VFS_ERROR;
}

STATIC INT32 ConsoleClose(struct file *filep)
{
    INT32 ret;
    struct file *privFilep = NULL;
    const struct file_operations_vfs *fileOps = NULL;

    ret = GetFilepOps(filep, &privFilep, &fileOps);
    if (ret != ENOERR) {
        ret = EINVAL;
        goto ERROUT;
    }
    ret = FilepClose(privFilep, fileOps);
    if (ret < 0) {
        ret = EPERM;
        goto ERROUT;
    }

    return ENOERR;

ERROUT:
    set_errno(ret);
    return VFS_ERROR;
}

STATIC ssize_t ConsoleRead(struct file *filep, CHAR *buffer, size_t bufLen)
{
    INT32 ret;
    struct file *privFilep = NULL;
    CONSOLE_CB *consoleCB = NULL;
    const struct file_operations_vfs *fileOps = NULL;

    ret = GetFilepOps(filep, &privFilep, &fileOps);
    if (ret != ENOERR) {
        ret = -EINVAL;
        goto ERROUT;
    }
    consoleCB = (CONSOLE_CB *)filep->f_priv;
    if (consoleCB == NULL) {
        consoleCB = OsGetConsoleByTaskID(OsCurrTaskGet()->taskID);
        if (consoleCB == NULL) {
            return -EFAULT;
        }
    }

    /*
     * shell task use FilepRead function to get data,
     * user task use UserFilepRead to get data
     */
#ifdef LOSCFG_SHELL
    if (OsCurrTaskGet()->taskEntry == (TSK_ENTRY_FUNC)ShellEntry) {
        ret = FilepRead(privFilep, fileOps, buffer, bufLen);
        if (ret < 0) {
            goto ERROUT;
        }
    } else {
#endif
        (VOID)ConsoleCtrlRightsCapture(consoleCB);
        ret = UserFilepRead(consoleCB, privFilep, fileOps, buffer, bufLen);
        (VOID)ConsoleCtrlRightsRelease(consoleCB);
        if (ret < 0) {
            goto ERROUT;
        }
#ifdef LOSCFG_SHELL
    }
#endif
    return ret;

ERROUT:
    set_errno(-ret);
    return VFS_ERROR;
}

STATIC ssize_t ConsoleWrite(struct file *filep, const CHAR *buffer, size_t bufLen)
{
    INT32 ret;
    INT32 cnt = 0;
    UINT32 intSave;
    CONSOLE_CB *consoleCB = NULL;
    CirBufSendCB *cirBufSendCB = NULL;
    struct file *privFilep = NULL;
    const struct file_operations_vfs *fileOps = NULL;

    ret = GetFilepOps(filep, &privFilep, &fileOps);
    if (ret != ENOERR) {
        ret = EINVAL;
        goto ERROUT;
    }
    if (fileOps->write == NULL) {
        ret = EFAULT;
        goto ERROUT;
    }

    consoleCB = (CONSOLE_CB *)filep->f_priv;
    if (consoleCB == NULL) {
        ret = EFAULT;
        goto ERROUT;
    }
    cirBufSendCB = consoleCB->cirBufSendCB;

    /*
     * adopt uart open function to read data from buffer
     * and write data to filep (filep is
     * corresponding to filep of /dev/console)
     */
#ifdef LOSCFG_SHELL_DMESG
    (VOID)OsLogMemcpyRecord(buffer, bufLen);
    if (!OsCheckConsoleLock()) {
#endif
    LOS_CirBufLock(&cirBufSendCB->cirBufCB, &intSave);
    while (cnt < (INT32)bufLen) {
        if ((buffer[cnt] == '\n') || (buffer[cnt] == '\r')) {
            (VOID)LOS_CirBufWrite(&cirBufSendCB->cirBufCB, "\r", 1);
            (VOID)LOS_CirBufWrite(&cirBufSendCB->cirBufCB, &buffer[cnt], 1);
            cnt++;
            continue;
        }
        (VOID)LOS_CirBufWrite(&cirBufSendCB->cirBufCB, &buffer[cnt], 1);
        cnt++;
    }
    LOS_CirBufUnlock(&cirBufSendCB->cirBufCB, intSave);
    (VOID)LOS_EventWrite(&cirBufSendCB->sendEvent, CONSOLE_CIRBUF_EVENT);
#ifdef LOSCFG_SHELL_DMESG
    }
#endif
    return cnt;

ERROUT:
    set_errno(ret);
    return VFS_ERROR;
}

STATIC INT32 ConsoleIoctl(struct file *filep, INT32 cmd, unsigned long arg)
{
    INT32 ret;
    struct file *privFilep = NULL;
    CONSOLE_CB *consoleCB = NULL;
    const struct file_operations_vfs *fileOps = NULL;

    ret = GetFilepOps(filep, &privFilep, &fileOps);
    if (ret != ENOERR) {
        ret = EINVAL;
        goto ERROUT;
    }

    if (fileOps->ioctl == NULL) {
        ret = EFAULT;
        goto ERROUT;
    }

    consoleCB = (CONSOLE_CB *)filep->f_priv;
    if (consoleCB == NULL) {
        ret = EINVAL;
        goto ERROUT;
    }

    switch (cmd) {
        case CONSOLE_CONTROL_RIGHTS_CAPTURE:
            ret = ConsoleCtrlRightsCapture(consoleCB);
            break;
        case CONSOLE_CONTROL_RIGHTS_RELEASE:
            ret = ConsoleCtrlRightsRelease(consoleCB);
            break;
        default:
            ret = fileOps->ioctl(privFilep, cmd, arg);
            break;
    }

    if (ret < 0) {
        ret = EPERM;
        goto ERROUT;
    }

    return ret;
ERROUT:
    set_errno(ret);
    return VFS_ERROR;
}

STATIC INT32 ConsolePoll(struct file *filep, poll_table *fds)
{
    INT32 ret;
    struct file *privFilep = NULL;
    const struct file_operations_vfs *fileOps = NULL;

    ret = GetFilepOps(filep, &privFilep, &fileOps);
    if (ret != ENOERR) {
        ret = EINVAL;
        goto ERROUT;
    }

    ret = FilepPoll(privFilep, fileOps, fds);
    if (ret < 0) {
        ret = EPERM;
        goto ERROUT;
    }
    return ret;

ERROUT:
    set_errno(ret);
    return VFS_ERROR;
}

/* console device driver function structure */
STATIC const struct file_operations_vfs g_consoleDevOps = {
    .open = ConsoleOpen,   /* open */
    .close = ConsoleClose, /* close */
    .read = ConsoleRead,   /* read */
    .write = ConsoleWrite, /* write */
    .seek = NULL,
    .ioctl = ConsoleIoctl,
#ifndef CONFIG_DISABLE_POLL
    .poll = ConsolePoll,
#endif
    .unlink = NULL
};

STATIC VOID OsConsoleTermiosInit(CONSOLE_CB *consoleCB, const CHAR *deviceName)
{
    struct termios consoleTermios;

    if ((deviceName != NULL) &&
        (strlen(deviceName) == strlen(SERIAL)) &&
        (!strncmp(deviceName, SERIAL, strlen(SERIAL)))) {
        consoleCB->isNonBlock = SetSerialBlock(consoleCB);

        /* set console to have a buffer for user */
        (VOID)ConsoleTcGetAttr(consoleCB->fd, &consoleTermios);
        consoleTermios.c_lflag |= ICANON | ECHO;
        (VOID)ConsoleTcSetAttr(consoleCB->fd, 0, &consoleTermios);
    }
#ifdef LOSCFG_NET_TELNET
    else if ((deviceName != NULL) &&
             (strlen(deviceName) == strlen(TELNET)) &&
             (!strncmp(deviceName, TELNET, strlen(TELNET)))) {
        consoleCB->isNonBlock = SetTelnetBlock(consoleCB);
    }
#endif
}

STATIC INT32 OsConsoleFileInit(CONSOLE_CB *consoleCB)
{
    INT32 ret;
    struct inode *inode = NULL;
    struct file *filep = NULL;
    CHAR *fullpath = NULL;

    ret = vfs_normalize_path(NULL, consoleCB->name, &fullpath);
    if (ret < 0) {
        return EINVAL;
    }

    inode = inode_find(fullpath, NULL);
    if (inode == NULL) {
        ret = ENOENT;
        goto ERROUT_WITH_FULLPATH;
    }

    consoleCB->fd = files_allocate(inode, O_RDWR, (off_t)0, consoleCB, STDERR_FILENO + 1);
    if (consoleCB->fd < 0) {
        ret = EMFILE;
        goto ERROUT_WITH_INODE;
    }

    filep = fs_getfilep(consoleCB->fd);
    if (filep == NULL) {
        ret = EPERM;
        goto ERROUT_WITH_INODE;
    }
    filep->f_path = fullpath;
    return LOS_OK;

ERROUT_WITH_INODE:
    inode_release(inode);
ERROUT_WITH_FULLPATH:
    free(fullpath);
    return ret;
}

/*
 * Initialized console control platform so that when we operate /dev/console
 * as if we are operating /dev/ttyS0 (uart0).
 */
STATIC INT32 OsConsoleDevInit(CONSOLE_CB *consoleCB, const CHAR *deviceName)
{
    INT32 ret;
    CHAR *fullpath = NULL;
    struct file *filep = NULL;
    struct inode *inode = NULL;

    /* allocate memory for filep,in order to unchange the value of filep */
    filep = (struct file *)LOS_MemAlloc(m_aucSysMem0, sizeof(struct file));
    if (filep == NULL) {
        ret = ENOMEM;
        goto ERROUT;
    }

    /* Adopt procedure of open function to allocate 'filep' to /dev/console */
    ret = vfs_normalize_path(NULL, deviceName, &fullpath);
    if (ret < 0) {
        ret = EINVAL;
        goto ERROUT_WITH_FILEP;
    }

    inode = inode_find(fullpath, NULL);
    if (inode == NULL) {
        ret = ENOENT;
        goto ERROUT_WITH_FULLPATH;
    }

    consoleCB->devInode = inode;

    /*
     * initialize the console filep which is associated with /dev/console,
     * assign the uart0 inode of /dev/ttyS0 to console inod of /dev/console,
     * then we can operate console's filep as if we operate uart0 filep of
     * /dev/ttyS0.
     */
    (VOID)memset_s(filep, sizeof(struct file), 0, sizeof(struct file));
    filep->f_oflags = O_RDWR;
    filep->f_pos = 0;
    filep->f_inode = inode;
    filep->f_path = NULL;
    filep->f_priv = NULL;

    if (inode->u.i_ops->open != NULL) {
        (VOID)inode->u.i_ops->open(filep);
    } else {
        ret = EFAULT;
        goto ERROUT_WITH_INODE;
    }

    /*
     * Use filep to connect console and uart, we can find uart driver function throught filep.
     * now we can operate /dev/console to operate /dev/ttyS0 through filep.
     */
    (VOID)register_driver(consoleCB->name, &g_consoleDevOps, DEFFILEMODE, filep);
    inode_release(inode);
    free(fullpath);
    return LOS_OK;

ERROUT_WITH_INODE:
    inode_release(inode);
ERROUT_WITH_FULLPATH:
    free(fullpath);
ERROUT_WITH_FILEP:
    (VOID)LOS_MemFree(m_aucSysMem0, filep);
ERROUT:
    set_errno(ret);
    return LOS_NOK;
}

STATIC UINT32 OsConsoleDevDeinit(const CONSOLE_CB *consoleCB)
{
    INT32 ret;
    struct file *filep = NULL;
    struct inode *inode = NULL;
    CHAR *fullpath = NULL;

    ret = vfs_normalize_path(NULL, consoleCB->name, &fullpath);
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
    if (filep != NULL) {
        (VOID)LOS_MemFree(m_aucSysMem0, filep); /* free filep what you malloc from console_init */
        inode->i_private = NULL;
    } else {
        ret = EBADF;
        goto ERROUT_WITH_INODE;
    }
    inode_release(inode);
    free(fullpath);
    (VOID)unregister_driver(consoleCB->name);
    return LOS_OK;

ERROUT_WITH_INODE:
    inode_release(inode);
ERROUT_WITH_FULLPATH:
    free(fullpath);
ERROUT:
    set_errno(ret);
    return LOS_NOK;
}

STATIC CirBufSendCB *ConsoleCirBufCreate(VOID)
{
    UINT32 ret;
    CHAR *fifo = NULL;
    CirBufSendCB *cirBufSendCB = NULL;
    CirBuf *cirBufCB = NULL;

    cirBufSendCB = (CirBufSendCB *)LOS_MemAlloc(m_aucSysMem0, sizeof(CirBufSendCB));
    if (cirBufSendCB == NULL) {
        return NULL;
    }
    (VOID)memset_s(cirBufSendCB, sizeof(CirBufSendCB), 0, sizeof(CirBufSendCB));

    fifo = (CHAR *)LOS_MemAlloc(m_aucSysMem0, TELNET_CIRBUF_SIZE);
    if (fifo == NULL) {
        goto ERROR_WITH_SENDCB;
    }
    (VOID)memset_s(fifo, TELNET_CIRBUF_SIZE, 0, TELNET_CIRBUF_SIZE);

    cirBufCB = &cirBufSendCB->cirBufCB;
    ret = LOS_CirBufInit(cirBufCB, fifo, TELNET_CIRBUF_SIZE);
    if (ret != LOS_OK) {
        goto ERROR_WITH_FIFO;
    }

    (VOID)LOS_EventInit(&cirBufSendCB->sendEvent);
    return cirBufSendCB;

ERROR_WITH_FIFO:
    (VOID)LOS_MemFree(m_aucSysMem0, cirBufCB->fifo);
ERROR_WITH_SENDCB:
    (VOID)LOS_MemFree(m_aucSysMem0, cirBufSendCB);
    return NULL;
}

STATIC VOID ConsoleCirBufDelete(CirBufSendCB *cirBufSendCB)
{
    CirBuf *cirBufCB = &cirBufSendCB->cirBufCB;

    (VOID)LOS_MemFree(m_aucSysMem0, cirBufCB->fifo);
    LOS_CirBufDeinit(cirBufCB);
    (VOID)LOS_EventDestroy(&cirBufSendCB->sendEvent);
    (VOID)LOS_MemFree(m_aucSysMem0, cirBufSendCB);
}

STATIC UINT32 OsConsoleBufInit(CONSOLE_CB *consoleCB)
{
    UINT32 ret;
    TSK_INIT_PARAM_S initParam = {0};

    consoleCB->cirBufSendCB = ConsoleCirBufCreate();
    if (consoleCB->cirBufSendCB == NULL) {
        return LOS_NOK;
    }

    initParam.pfnTaskEntry = (TSK_ENTRY_FUNC)ConsoleSendTask;
    initParam.usTaskPrio   = SHELL_TASK_PRIORITY;
    initParam.auwArgs[0]   = (UINTPTR)consoleCB;
    initParam.uwStackSize  = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    if (consoleCB->consoleID == CONSOLE_SERIAL) {
        initParam.pcName   = "SendToSer";
    } else {
        initParam.pcName   = "SendToTelnet";
    }
    initParam.uwResved     = LOS_TASK_STATUS_DETACHED;

    ret = LOS_TaskCreate(&consoleCB->sendTaskID, &initParam);
    if (ret != LOS_OK) {
        ConsoleCirBufDelete(consoleCB->cirBufSendCB);
        consoleCB->cirBufSendCB = NULL;
        return LOS_NOK;
    }
    (VOID)LOS_EventRead(&consoleCB->cirBufSendCB->sendEvent, CONSOLE_SEND_TASK_RUNNING,
                        LOS_WAITMODE_OR | LOS_WAITMODE_CLR, LOS_WAIT_FOREVER);

    return LOS_OK;
}

STATIC VOID OsConsoleBufDeinit(CONSOLE_CB *consoleCB)
{
    CirBufSendCB *cirBufSendCB = consoleCB->cirBufSendCB;

    consoleCB->cirBufSendCB = NULL;
    (VOID)LOS_EventWrite(&cirBufSendCB->sendEvent, CONSOLE_SEND_TASK_EXIT);
}

STATIC CONSOLE_CB *OsConsoleCBInit(UINT32 consoleID)
{
    CONSOLE_CB *consoleCB = (CONSOLE_CB *)LOS_MemAlloc((VOID *)m_aucSysMem0, sizeof(CONSOLE_CB));
    if (consoleCB == NULL) {
        return NULL;
    }
    (VOID)memset_s(consoleCB, sizeof(CONSOLE_CB), 0, sizeof(CONSOLE_CB));

    consoleCB->consoleID = consoleID;
    consoleCB->shellEntryId = 0xffffffff;  /* initialize shellEntryId to an invalid value */
    consoleCB->name = LOS_MemAlloc((VOID *)m_aucSysMem0, CONSOLE_NAMELEN);
    if (consoleCB->name == NULL) {
        PRINT_ERR("consoleCB->name malloc failed\n");
        (VOID)LOS_MemFree((VOID *)m_aucSysMem0, consoleCB);
        return NULL;
    }
    return consoleCB;
}

STATIC VOID OsConsoleCBDeinit(CONSOLE_CB *consoleCB)
{
    (VOID)LOS_MemFree((VOID *)m_aucSysMem0, consoleCB->name);
    consoleCB->name = NULL;
    (VOID)LOS_MemFree((VOID *)m_aucSysMem0, consoleCB);
}

STATIC CONSOLE_CB *OsConsoleCreate(UINT32 consoleID, const CHAR *deviceName)
{
    INT32 ret;
    CONSOLE_CB *consoleCB = OsConsoleCBInit(consoleID);
    if (consoleCB == NULL) {
        PRINT_ERR("console malloc error.\n");
        return NULL;
    }

    ret = snprintf_s(consoleCB->name, CONSOLE_NAMELEN, CONSOLE_NAMELEN - 1,
                     "%s%u", CONSOLE, consoleCB->consoleID);
    if (ret == -1) {
        PRINT_ERR("consoleCB->name snprintf_s failed\n");
        goto ERR_WITH_NAME;
    }

    ret = (INT32)OsConsoleBufInit(consoleCB);
    if (ret != LOS_OK) {
        goto ERR_WITH_NAME;
    }

    ret = (INT32)LOS_SemCreate(1, &consoleCB->consoleSem);
    if (ret != LOS_OK) {
        PRINT_ERR("creat sem for uart failed\n");
        goto ERR_WITH_BUF;
    }

    ret = OsConsoleDevInit(consoleCB, deviceName);
    if (ret != LOS_OK) {
        goto ERR_WITH_SEM;
    }

    ret = OsConsoleFileInit(consoleCB);
    if (ret != LOS_OK) {
        goto ERR_WITH_DEV;
    }

    OsConsoleTermiosInit(consoleCB, deviceName);
    return consoleCB;

ERR_WITH_DEV:
    ret = (INT32)OsConsoleDevDeinit(consoleCB);
    if (ret != LOS_OK) {
        PRINT_ERR("OsConsoleDevDeinit failed!\n");
    }
ERR_WITH_SEM:
    (VOID)LOS_SemDelete(consoleCB->consoleSem);
ERR_WITH_BUF:
    OsConsoleBufDeinit(consoleCB);
ERR_WITH_NAME:
    OsConsoleCBDeinit(consoleCB);
    return NULL;
}

STATIC UINT32 OsConsoleDelete(CONSOLE_CB *consoleCB)
{
    UINT32 ret;

    (VOID)files_close(consoleCB->fd);
    ret = OsConsoleDevDeinit(consoleCB);
    if (ret != LOS_OK) {
        PRINT_ERR("OsConsoleDevDeinit failed!\n");
    }
    OsConsoleBufDeinit((CONSOLE_CB *)consoleCB);
    (VOID)LOS_SemDelete(consoleCB->consoleSem);
    (VOID)LOS_MemFree(m_aucSysMem0, consoleCB->name);
    consoleCB->name = NULL;
    (VOID)LOS_MemFree(m_aucSysMem0, consoleCB);

    return ret;
}

/* Initialized system console and return stdinfd stdoutfd stderrfd */
INT32 system_console_init(const CHAR *deviceName)
{
#ifdef LOSCFG_SHELL
    UINT32 ret;
#endif
    INT32 consoleID;
    UINT32 intSave;
    CONSOLE_CB *consoleCB = NULL;

    consoleID = OsGetConsoleID(deviceName);
    if (consoleID == -1) {
        PRINT_ERR("device is full.\n");
        return VFS_ERROR;
    }

    consoleCB = OsConsoleCreate((UINT32)consoleID, deviceName);
    if (consoleCB == NULL) {
        PRINT_ERR("%s, %d\n", __FUNCTION__, __LINE__);
        return VFS_ERROR;
    }

    LOS_SpinLockSave(&g_consoleSpin, &intSave);
    g_console[consoleID - 1] = consoleCB;
    if (OsCurrTaskGet() != NULL) {
        g_taskConsoleIDArray[OsCurrTaskGet()->taskID] = (UINT8)consoleID;
    }
    LOS_SpinUnlockRestore(&g_consoleSpin, intSave);

#ifdef LOSCFG_SHELL
    ret = OsShellInit(consoleID);
    if (ret != LOS_OK) {
        PRINT_ERR("%s, %d\n", __FUNCTION__, __LINE__);
        LOS_SpinLockSave(&g_consoleSpin, &intSave);
        (VOID)OsConsoleDelete(consoleCB);
        g_console[consoleID - 1] = NULL;
        g_taskConsoleIDArray[OsCurrTaskGet()->taskID] = 0;
        LOS_SpinUnlockRestore(&g_consoleSpin, intSave);
        return VFS_ERROR;
    }
#endif

    return ENOERR;
}

INT32 system_console_deinit(const CHAR *deviceName)
{
    UINT32 ret;
    CONSOLE_CB *consoleCB = NULL;
    UINT32 taskIdx;
    LosTaskCB *taskCB = NULL;
    UINT32 intSave;

    consoleCB = OsGetConsoleByDevice(deviceName);
    if (consoleCB == NULL) {
        return VFS_ERROR;
    }

#ifdef LOSCFG_SHELL
    (VOID)OsShellDeinit((INT32)consoleCB->consoleID);
#endif

    LOS_SpinLockSave(&g_consoleSpin, &intSave);
    /* Redirect all tasks to serial as telnet was unavailable after deinitializing */
    for (taskIdx = 0; taskIdx < g_taskMaxNum; taskIdx++) {
        taskCB = ((LosTaskCB *)g_taskCBArray) + taskIdx;
        if (taskCB->taskStatus & OS_TASK_STATUS_UNUSED) {
            continue;
        } else {
            g_taskConsoleIDArray[taskCB->taskID] = CONSOLE_SERIAL;
        }
    }
    g_console[consoleCB->consoleID - 1] = NULL;
    LOS_SpinUnlockRestore(&g_consoleSpin, intSave);

    ret = OsConsoleDelete(consoleCB);
    if (ret != LOS_OK) {
        PRINT_ERR("%s, Failed to system_console_deinit\n", __FUNCTION__);
        return VFS_ERROR;
    }

    return ENOERR;
}

BOOL ConsoleEnable(VOID)
{
    INT32 consoleID;

    if (OsCurrTaskGet() != NULL) {
        consoleID = g_taskConsoleIDArray[OsCurrTaskGet()->taskID];
        if (g_uart_fputc_en == 0) {
            if ((g_console[CONSOLE_TELNET - 1] != NULL) && OsPreemptable()) {
                return TRUE;
            } else {
                return FALSE;
            }
        }

        if (consoleID == 0) {
            return FALSE;
        } else if ((consoleID == CONSOLE_TELNET) && OsPreemptable()) {
            return TRUE;
        }
#if defined (LOSCFG_DRIVERS_USB_SERIAL_GADGET) || defined (LOSCFG_DRIVERS_USB_ETH_SER_GADGET)
        else if ((SerialTypeGet() == SERIAL_TYPE_USBTTY_DEV) && (userial_mask_get() == 1)) {
            return TRUE;
        }
#endif
    }

    return FALSE;
}

VOID ConsoleTaskReg(INT32 consoleID, UINT32 taskID)
{
    g_console[consoleID - 1]->shellEntryId = taskID;
}

BOOL SetSerialNonBlock(const CONSOLE_CB *consoleCB)
{
    INT32 ret;

    if (consoleCB == NULL) {
        PRINT_ERR("%s: Input parameter is illegal\n", __FUNCTION__);
        return FALSE;
    }
    ret = ioctl(consoleCB->fd, CONSOLE_CMD_RD_BLOCK_SERIAL, CONSOLE_RD_NONBLOCK);
    if (ret != 0) {
        return FALSE;
    }

    return TRUE;
}

BOOL SetSerialBlock(const CONSOLE_CB *consoleCB)
{
    INT32 ret;

    if (consoleCB == NULL) {
        PRINT_ERR("%s: Input parameter is illegal\n", __FUNCTION__);
        return TRUE;
    }
    ret = ioctl(consoleCB->fd, CONSOLE_CMD_RD_BLOCK_SERIAL, CONSOLE_RD_BLOCK);
    if (ret != 0) {
        return TRUE;
    }

    return FALSE;
}

BOOL SetTelnetNonBlock(const CONSOLE_CB *consoleCB)
{
    INT32 ret;

    if (consoleCB == NULL) {
        PRINT_ERR("%s: Input parameter is illegal\n", __FUNCTION__);
        return FALSE;
    }
    ret = ioctl(consoleCB->fd, CONSOLE_CMD_RD_BLOCK_TELNET, CONSOLE_RD_NONBLOCK);
    if (ret != 0) {
        return FALSE;
    }
    return TRUE;
}

BOOL SetTelnetBlock(const CONSOLE_CB *consoleCB)
{
    INT32 ret;

    if (consoleCB == NULL) {
        PRINT_ERR("%s: Input parameter is illegal\n", __FUNCTION__);
        return TRUE;
    }
    ret = ioctl(consoleCB->fd, CONSOLE_CMD_RD_BLOCK_TELNET, CONSOLE_RD_BLOCK);
    if (ret != 0) {
        return TRUE;
    }
    return FALSE;
}

BOOL is_nonblock(const CONSOLE_CB *consoleCB)
{
    if (consoleCB == NULL) {
        PRINT_ERR("%s: Input parameter is illegal\n", __FUNCTION__);
        return FALSE;
    }
    return consoleCB->isNonBlock;
}

INT32 ConsoleUpdateFd(VOID)
{
    INT32 consoleID = 0;

    if (OsCurrTaskGet() != NULL) {
        consoleID = g_taskConsoleIDArray[(OsCurrTaskGet())->taskID];
    } else {
        return -1;
    }

    if (g_uart_fputc_en == 0) {
        if (g_console[CONSOLE_TELNET - 1] != NULL) {
            consoleID = CONSOLE_TELNET;
        } else {
            return -1;
        }
    } else if (consoleID == 0) {
        if (g_console[CONSOLE_SERIAL - 1] != NULL) {
            consoleID = CONSOLE_SERIAL;
        } else if (g_console[CONSOLE_TELNET - 1] != NULL) {
            consoleID = CONSOLE_TELNET;
        } else {
            PRINT_ERR("No console dev used.\n");
            return -1;
        }
    }

    if (g_console[consoleID - 1] == NULL) {
        return -1;
    }

    return g_console[consoleID - 1]->fd;
}

CONSOLE_CB *OsGetConsoleByID(INT32 consoleID)
{
    if (consoleID != CONSOLE_TELNET) {
        consoleID = CONSOLE_SERIAL;
    }
    return g_console[consoleID - 1];
}

CONSOLE_CB *OsGetConsoleByTaskID(UINT32 taskID)
{
    INT32 consoleID = g_taskConsoleIDArray[taskID];

    return OsGetConsoleByID(consoleID);
}

VOID OsSetConsoleID(UINT32 newTaskID, UINT32 curTaskID)
{
    if ((newTaskID >= LOSCFG_BASE_CORE_TSK_LIMIT) || (curTaskID >= LOSCFG_BASE_CORE_TSK_LIMIT)) {
        return;
    }

    g_taskConsoleIDArray[newTaskID] = g_taskConsoleIDArray[curTaskID];
}

STATIC ssize_t WriteToTerminal(const CONSOLE_CB *consoleCB, const CHAR *buffer, size_t bufLen)
{
    INT32 ret, fd;
    struct file *privFilep = NULL;
    struct file *filep = NULL;
    const struct file_operations_vfs *fileOps = NULL;

    fd = consoleCB->fd;
    filep = fs_getfilep(fd);
    ret = GetFilepOps(filep, &privFilep, &fileOps);

    if (fileOps->write == NULL) {
        ret = EFAULT;
        goto ERROUT;
    }
    (VOID)fileOps->write(privFilep, buffer, bufLen);

    return 0;

ERROUT:
    set_errno(ret);
    return VFS_ERROR;
}

STATIC UINT32 ConsoleSendTask(UINTPTR param)
{
    CONSOLE_CB *consoleCB = (CONSOLE_CB *)param;
    CirBufSendCB *cirBufSendCB = consoleCB->cirBufSendCB;
    CirBuf *cirBufCB = &cirBufSendCB->cirBufCB;
    UINT32 ret, size;
    UINT32 intSave;
    CHAR *buf = NULL;

    (VOID)LOS_EventWrite(&cirBufSendCB->sendEvent, CONSOLE_SEND_TASK_RUNNING);

    while (1) {
        ret = LOS_EventRead(&cirBufSendCB->sendEvent, CONSOLE_CIRBUF_EVENT | CONSOLE_SEND_TASK_EXIT,
                            LOS_WAITMODE_OR | LOS_WAITMODE_CLR, LOS_WAIT_FOREVER);
        if (ret == CONSOLE_CIRBUF_EVENT) {
            size =  LOS_CirBufUsedSize(cirBufCB);
            if (size == 0) {
                continue;
            }
            buf = (CHAR *)LOS_MemAlloc(m_aucSysMem1, size + 1);
            if (buf == NULL) {
                continue;
            }
            (VOID)memset_s(buf, size + 1, 0, size + 1);

            LOS_CirBufLock(cirBufCB, &intSave);
            (VOID)LOS_CirBufRead(cirBufCB, buf, size);
            LOS_CirBufUnlock(cirBufCB, intSave);

            (VOID)WriteToTerminal(consoleCB, buf, size);
            (VOID)LOS_MemFree(m_aucSysMem1, buf);
        } else if (ret == CONSOLE_SEND_TASK_EXIT) {
            break;
        }
    }

    ConsoleCirBufDelete(cirBufSendCB);
    return LOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
