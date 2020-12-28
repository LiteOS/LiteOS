/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: LiteOS Shell Dmesg Implementation File
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
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

/*
 *  +-------------------------------------------------------+
 *  | Info |          log_space                             |
 *  +-------------------------------------------------------+
 *  |
 *  |__buffer_space
 *
 * Case A:
 *  +-------------------------------------------------------+
 *  |           |#############################|             |
 *  +-------------------------------------------------------+
 *              |                             |
 *             Head                           Tail
 * Case B:
 *  +-------------------------------------------------------+
 *  |##########|                                    |#######|
 *  +-------------------------------------------------------+
 *             |                                    |
 *             Tail                                 Head
 */

#ifdef LOSCFG_SHELL_DMESG
#include "dmesg_pri.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdio.h"
#include "fcntl.h"
#include "securec.h"
#include "los_task.h"
#include "uart.h"
#ifdef LOSCFG_FS_VFS
#include "inode/inode.h"
#endif
#include "show.h"
#include "shcmd.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define BUF_MAX_INDEX (g_logBufSize - 1)

LITE_OS_SEC_BSS STATIC SPIN_LOCK_INIT(g_dmesgSpin);

STATIC DmesgInfo *g_dmesgInfo = NULL;
STATIC UINT32 g_logBufSize = 0;
STATIC VOID *g_mallocAddr = NULL;
STATIC UINT32 g_dmesgLogLevel = 3;
STATIC UINT32 g_consoleLock = 0;
STATIC UINT32 g_uartLock = 0;
STATIC const CHAR *g_levelString[] = {
    "EMG",
    "COMMOM",
    "ERR",
    "WARN",
    "INFO",
    "DEBUG"
};

STATIC VOID OsLockConsole(VOID)
{
    g_consoleLock = 1;
}

STATIC VOID OsUnlockConsole(VOID)
{
    g_consoleLock = 0;
}

STATIC VOID OsLockUart(VOID)
{
    g_uartLock = 1;
}

STATIC VOID OsUnlockUart(VOID)
{
    g_uartLock = 0;
}

STATIC UINT32 OsCheckError(VOID)
{
    if (g_dmesgInfo == NULL) {
        return LOS_NOK;
    }

    if (g_dmesgInfo->logSize > g_logBufSize) {
        return LOS_NOK;
    }

    if (((g_dmesgInfo->logSize == g_logBufSize) || (g_dmesgInfo->logSize == 0)) &&
        (g_dmesgInfo->logTail != g_dmesgInfo->logHead)) {
        return LOS_NOK;
    }

    return LOS_OK;
}

STATIC INT32 OsDmesgRead(CHAR *buf, UINT32 len)
{
    UINT32 readLen;
    UINT32 logSize = g_dmesgInfo->logSize;
    UINT32 head = g_dmesgInfo->logHead;
    UINT32 tail = g_dmesgInfo->logTail;
    CHAR *logBuf = g_dmesgInfo->logBuf;
    errno_t ret;

    if (OsCheckError()) {
        return -1;
    }
    if (logSize == 0) {
        return 0;
    }

    readLen = len < logSize ? len : logSize;

    if (head < tail) { /* Case A */
        ret = memcpy_s(buf, len, logBuf + head, readLen);
        if (ret != EOK) {
            return -1;
        }
        g_dmesgInfo->logHead += readLen;
        g_dmesgInfo->logSize -= readLen;
    } else { /* Case B */
        if (readLen <= (g_logBufSize - head)) {
            ret = memcpy_s(buf, len, logBuf + head, readLen);
            if (ret != EOK) {
                return -1;
            }
            g_dmesgInfo->logHead += readLen;
            g_dmesgInfo->logSize -= readLen;
        } else {
            ret = memcpy_s(buf, len, logBuf + head, g_logBufSize - head);
            if (ret != EOK) {
                return -1;
            }

            ret = memcpy_s(buf + g_logBufSize - head, len - (g_logBufSize - head),
                           logBuf, readLen - (g_logBufSize - head));
            if (ret != EOK) {
                return -1;
            }
            g_dmesgInfo->logHead = readLen - (g_logBufSize - head);
            g_dmesgInfo->logSize -= readLen;
        }
    }
    return (INT32)readLen;
}

STATIC INT32 OsCopyToNew(VOID *addr, UINT32 size)
{
    UINT32 copyStart = 0;
    UINT32 copyLen;
    CHAR *temp = NULL;
    CHAR *newBuf = (CHAR *)addr + sizeof(DmesgInfo);
    UINT32 bufSize = size - sizeof(DmesgInfo);
    INT32 ret;

    if (g_dmesgInfo->logSize == 0) {
        return 0;
    }

    temp = (CHAR *)malloc(g_dmesgInfo->logSize);
    if (temp == NULL) {
        return -1;
    }

    (VOID)memset_s(temp, g_dmesgInfo->logSize, 0, g_dmesgInfo->logSize);
    copyLen = ((bufSize < g_dmesgInfo->logSize) ? bufSize : g_dmesgInfo->logSize);
    if (bufSize < g_dmesgInfo->logSize) {
        copyStart = g_dmesgInfo->logSize - bufSize;
    }

    ret = OsDmesgRead(temp, g_dmesgInfo->logSize);
    if (ret <= 0) {
        PRINT_ERR("%s,%d failed, err:%d!\n", __FUNCTION__, __LINE__, ret);
        free(temp);
        return -1;
    }

    /* if new buf size smaller than logSize */
    ret = memcpy_s(newBuf, bufSize, temp + copyStart, copyLen);
    if (ret != EOK) {
        PRINT_ERR("%s,%d memcpy_s failed, err:%d!\n", __FUNCTION__, __LINE__, ret);
        free(temp);
        return -1;
    }
    free(temp);

    return (INT32)copyLen;
}

STATIC UINT32 OsDmesgResetMem(VOID *addr, UINT32 size)
{
    VOID *temp = NULL;
    INT32 copyLen;
    UINT32 intSave;

    if (size <= sizeof(DmesgInfo)) {
        return LOS_NOK;
    }

    LOS_SpinLockSave(&g_dmesgSpin, &intSave);
    temp = g_dmesgInfo;
    copyLen = OsCopyToNew(addr, size);
    if (copyLen < 0) {
        LOS_SpinUnlockRestore(&g_dmesgSpin, intSave);
        return LOS_NOK;
    }

    g_logBufSize = size - sizeof(DmesgInfo);
    g_dmesgInfo = (DmesgInfo *)addr;
    g_dmesgInfo->logBuf = (CHAR *)addr + sizeof(DmesgInfo);
    g_dmesgInfo->logSize = copyLen;
    g_dmesgInfo->logTail = ((copyLen == g_logBufSize) ? 0 : copyLen);
    g_dmesgInfo->logHead = 0;

    /* if old mem came from malloc */
    if (temp == g_mallocAddr) {
        free(temp);
        g_mallocAddr = NULL;
    }
    LOS_SpinUnlockRestore(&g_dmesgSpin, intSave);

    return LOS_OK;
}

STATIC UINT32 OsDmesgChangeSize(UINT32 size)
{
    VOID *temp = NULL;
    INT32 copyLen;
    CHAR *newString = NULL;
    UINT32 intSave;

    if (size == 0) {
        return LOS_NOK;
    }

    newString = (CHAR *)malloc(size + sizeof(DmesgInfo));
    if (newString == NULL) {
        return LOS_NOK;
    }

    LOS_SpinLockSave(&g_dmesgSpin, &intSave);
    temp = g_dmesgInfo;

    copyLen = OsCopyToNew(newString, size + sizeof(DmesgInfo));
    if (copyLen < 0) {
        LOS_SpinUnlockRestore(&g_dmesgSpin, intSave);
        free(newString);
        return LOS_NOK;
    }

    g_logBufSize = size;
    g_dmesgInfo = (DmesgInfo *)newString;
    g_dmesgInfo->logBuf = (CHAR *)newString + sizeof(DmesgInfo);
    g_dmesgInfo->logSize = copyLen;
    g_dmesgInfo->logTail = ((copyLen == g_logBufSize) ? 0 : copyLen);
    g_dmesgInfo->logHead = 0;

    if (temp == g_mallocAddr) {
        free(temp);
    }
    g_mallocAddr = newString;
    LOS_SpinUnlockRestore(&g_dmesgSpin, intSave);

    return LOS_OK;
}

UINT32 OsCheckConsoleLock(VOID)
{
    return g_consoleLock;
}

UINT32 OsCheckUartLock(VOID)
{
    return g_uartLock;
}

UINT32 OsDmesgInit(VOID)
{
    CHAR* buffer = NULL;

    buffer = (CHAR *)malloc(KERNEL_LOG_BUF_SIZE + sizeof(DmesgInfo));
    if (buffer == NULL) {
        return LOS_NOK;
    }
    g_mallocAddr = buffer;
    g_dmesgInfo = (DmesgInfo *)buffer;
    g_dmesgInfo->logHead = 0;
    g_dmesgInfo->logTail = 0;
    g_dmesgInfo->logSize = 0;
    g_dmesgInfo->logBuf = buffer + sizeof(DmesgInfo);
    g_logBufSize = KERNEL_LOG_BUF_SIZE;

    return LOS_OK;
}

STATIC CHAR OsLogRecordChar(CHAR c)
{
    *(g_dmesgInfo->logBuf + g_dmesgInfo->logTail++) = c;

    if (g_dmesgInfo->logTail > BUF_MAX_INDEX) {
        g_dmesgInfo->logTail = 0;
    }

    if (g_dmesgInfo->logSize < g_logBufSize) {
        (g_dmesgInfo->logSize)++;
    } else {
        g_dmesgInfo->logHead = g_dmesgInfo->logTail;
    }
    return c;
}

UINT32 OsLogRecordStr(const CHAR *str, UINT32 len)
{
    UINT32 i = 0;
    UINTPTR intSave;

    LOS_SpinLockSave(&g_dmesgSpin, &intSave);
    while (len--) {
        (VOID)OsLogRecordChar(str[i]);
        i++;
    }
    LOS_SpinUnlockRestore(&g_dmesgSpin, intSave);
    return i;
}

STATIC VOID OsBufFullWrite(const CHAR *dst, UINT32 logLen)
{
    UINT32 bufSize = g_logBufSize;
    UINT32 tail = g_dmesgInfo->logTail;
    CHAR *buf = g_dmesgInfo->logBuf;
    errno_t ret;

    if ((logLen == 0) || (dst == NULL)) {
        return;
    }
    if (logLen > bufSize) { /* full re-write */
        ret = memcpy_s(buf + tail, bufSize - tail, dst, bufSize - tail);
        if (ret != EOK) {
            PRINT_ERR("%s,%d memcpy_s failed, err:%d!\n", __FUNCTION__, __LINE__, ret);
            return;
        }
        ret = memcpy_s(buf, bufSize, dst + bufSize - tail, tail);
        if (ret != EOK) {
            PRINT_ERR("%s,%d memcpy_s failed, err:%d!\n", __FUNCTION__, __LINE__, ret);
            return;
        }

        OsBufFullWrite(dst + bufSize, logLen - bufSize);
    } else {
        if (logLen > (bufSize - tail)) { /* need cycle back to start */
            ret = memcpy_s(buf + tail, bufSize - tail, dst, bufSize - tail);
            if (ret != EOK) {
                PRINT_ERR("%s,%d memcpy_s failed, err:%d!\n", __FUNCTION__, __LINE__, ret);
                return;
            }
            ret = memcpy_s(buf, bufSize, dst + bufSize - tail, logLen - (bufSize - tail));
            if (ret != EOK) {
                PRINT_ERR("%s,%d memcpy_s failed, err:%d!\n", __FUNCTION__, __LINE__, ret);
                return;
            }

            g_dmesgInfo->logTail = logLen - (bufSize - tail);
            g_dmesgInfo->logHead = g_dmesgInfo->logTail;
        } else { /* no need cycle back to start */
            ret = memcpy_s(buf + tail, bufSize - tail, dst, logLen);
            if (ret != EOK) {
                PRINT_ERR("%s,%d memcpy_s failed, err:%d!\n", __FUNCTION__, __LINE__, ret);
                return;
            }
            g_dmesgInfo->logTail += logLen;
            if (g_dmesgInfo->logTail > BUF_MAX_INDEX) {
                g_dmesgInfo->logTail = 0;
            }
            g_dmesgInfo->logHead = g_dmesgInfo->logTail;
        }
    }
}

STATIC VOID OsWriteTailToHead(const CHAR *dst, UINT32 logLen)
{
    UINT32 writeLen;
    UINT32 bufSize = g_logBufSize;
    UINT32 logSize = g_dmesgInfo->logSize;
    UINT32 tail = g_dmesgInfo->logTail;
    CHAR *buf = g_dmesgInfo->logBuf;
    errno_t ret;

    if ((logLen == 0) || (dst == NULL)) {
        return;
    }
    if (logLen > (bufSize - logSize)) { /* space-need > space-remain */
        writeLen = bufSize - logSize;
        ret = memcpy_s(buf + tail, bufSize - tail, dst, writeLen);
        if (ret != EOK) {
            PRINT_ERR("%s,%d memcpy_s failed, err:%d!\n", __FUNCTION__, __LINE__, ret);
            return;
        }

        g_dmesgInfo->logTail = g_dmesgInfo->logHead;
        g_dmesgInfo->logSize = g_logBufSize;
        OsBufFullWrite(dst + writeLen, logLen - writeLen);
    } else {
        ret = memcpy_s(buf + tail, bufSize - tail, dst, logLen);
        if (ret != EOK) {
            PRINT_ERR("%s,%d memcpy_s failed, err:%d!\n", __FUNCTION__, __LINE__, ret);
            return;
        }

        g_dmesgInfo->logTail += logLen;
        g_dmesgInfo->logSize += logLen;
    }
}

STATIC VOID OsWriteTailToEnd(const CHAR *dst, UINT32 logLen)
{
    UINT32 writeLen;
    UINT32 bufSize = g_logBufSize;
    UINT32 tail = g_dmesgInfo->logTail;
    CHAR *buf = g_dmesgInfo->logBuf;
    errno_t ret;

    if ((logLen == 0) || (dst == NULL)) {
        return;
    }
    if (logLen >= (bufSize - tail)) { /* need cycle to start ,then became B */
        writeLen = bufSize - tail;
        ret = memcpy_s(buf + tail, writeLen, dst, writeLen);
        if (ret != EOK) {
            PRINT_ERR("%s,%d memcpy_s failed, err:%d!\n", __FUNCTION__, __LINE__, ret);
            return;
        }

        g_dmesgInfo->logSize += writeLen;
        g_dmesgInfo->logTail = 0;
        if (g_dmesgInfo->logSize == g_logBufSize) { /* Tail = Head is 0 */
            OsBufFullWrite(dst + writeLen, logLen - writeLen);
        } else {
            OsWriteTailToHead(dst + writeLen, logLen - writeLen);
        }
    } else { /* just do serial copy */
        ret = memcpy_s(buf + tail, bufSize - tail, dst, logLen);
        if (ret != EOK) {
            PRINT_ERR("%s,%d memcpy_s failed, err:%d!\n", __FUNCTION__, __LINE__, ret);
            return;
        }

        g_dmesgInfo->logTail += logLen;
        g_dmesgInfo->logSize += logLen;
    }
}

INT32 OsLogMemcpyRecord(const CHAR *buf, UINT32 logLen)
{
    UINT32 intSave;

    LOS_SpinLockSave(&g_dmesgSpin, &intSave);
    if (OsCheckError()) {
        LOS_SpinUnlockRestore(&g_dmesgSpin, intSave);
        return -1;
    }
    if (g_dmesgInfo->logSize < g_logBufSize) {
        if (g_dmesgInfo->logHead <= g_dmesgInfo->logTail) {
            OsWriteTailToEnd(buf, logLen);
        } else {
            OsWriteTailToHead(buf, logLen);
        }
    } else {
        OsBufFullWrite(buf, logLen);
    }
    LOS_SpinUnlockRestore(&g_dmesgSpin, intSave);

    return LOS_OK;
}

VOID OsLogShow(VOID)
{
    UINT32 intSave;
    UINT32 index;
    UINT32 i = 0;
    CHAR *p = NULL;

    LOS_SpinLockSave(&g_dmesgSpin, &intSave);
    index = g_dmesgInfo->logHead;
    p = (CHAR *)malloc(g_dmesgInfo->logSize + 1);
    if (p == NULL) {
        LOS_SpinUnlockRestore(&g_dmesgSpin, intSave);
        return;
    }
    (VOID)memset_s(p, g_dmesgInfo->logSize + 1, 0, g_dmesgInfo->logSize + 1);

    while (i < g_dmesgInfo->logSize) {
        *(p + i) = *(g_dmesgInfo->logBuf + index++);
        if (index > BUF_MAX_INDEX) {
            index = 0;
        }
        i++;
        if (index == g_dmesgInfo->logTail) {
            break;
        }
    }
    LOS_SpinUnlockRestore(&g_dmesgSpin, intSave);
    UartPuts(p, i, UART_WITH_LOCK);
    free(p);
}

STATIC INT32 OsDmesgLvSet(const CHAR *level)
{
    UINT32 levelNum, ret;
    CHAR *p = NULL;

    levelNum = strtoul(level, &p, 0);
    if (*p != 0) {
        PRINTK("dmesg: invalid option or parameter.\n");
        return -1;
    }

    ret = LOS_DmesgLvSet(levelNum);
    if (ret == LOS_OK) {
        PRINTK("Set current dmesg log level %s\n", g_levelString[g_dmesgLogLevel]);
        return LOS_OK;
    } else {
        PRINTK("current dmesg log level %s\n", g_levelString[g_dmesgLogLevel]);
        PRINTK("dmesg -l [num] can access as 0:EMG 1:COMMOM 2:ERROR 3:WARN 4:INFO 5:DEBUG\n");
        return -1;
    }
}

STATIC INT32 OsDmesgMemSizeSet(const CHAR *size)
{
    UINT32 sizeVal;
    CHAR *p = NULL;

    sizeVal = strtoul(size, &p, 0);
    if (!(LOS_DmesgMemSet(NULL, sizeVal))) {
        PRINTK("Set dmesg buf size %u success\n", sizeVal);
        return LOS_OK;
    } else {
        PRINTK("Set dmesg buf size %u fail\n", sizeVal);
        return LOS_NOK;
    }
}
UINT32 OsDmesgLvGet(VOID)
{
    return g_dmesgLogLevel;
}

UINT32 LOS_DmesgLvSet(UINT32 level)
{
    if (level > 5) { /* 5: count of level */
        return LOS_NOK;
    }

    g_dmesgLogLevel = level;
    return LOS_OK;
}

VOID LOS_DmesgClear(VOID)
{
    UINT32 intSave;

    LOS_SpinLockSave(&g_dmesgSpin, &intSave);
    (VOID)memset_s(g_dmesgInfo->logBuf, g_logBufSize, 0, g_logBufSize);
    g_dmesgInfo->logHead = 0;
    g_dmesgInfo->logTail = 0;
    g_dmesgInfo->logSize = 0;
    LOS_SpinUnlockRestore(&g_dmesgSpin, intSave);
}

UINT32 LOS_DmesgMemSet(VOID *addr, UINT32 size)
{
    UINT32 ret;

    if (addr == NULL) {
        ret = OsDmesgChangeSize(size);
    } else {
        ret = OsDmesgResetMem(addr, size);
    }
    return ret;
}

INT32 LOS_DmesgRead(CHAR *buf, UINT32 len)
{
    INT32 ret;
    UINT32 intSave;

    if (buf == NULL) {
        return -1;
    }
    if (len == 0) {
        return 0;
    }

    LOS_SpinLockSave(&g_dmesgSpin, &intSave);
    ret = OsDmesgRead(buf, len);
    LOS_SpinUnlockRestore(&g_dmesgSpin, intSave);
    return ret;
}

INT32 OsDmesgWrite2File(const CHAR *fullpath, const CHAR *buf, UINT32 logSize)
{
    INT32 ret;

    INT32 fd = open(fullpath, O_CREAT | O_RDWR | O_APPEND, 0644); /* 0644:file right */
    if (fd < 0) {
        return -1;
    }
    ret = write(fd, buf, logSize);
    (VOID)close(fd);
    return ret;
}

#ifdef LOSCFG_FS_VFS
INT32 LOS_DmesgToFile(CHAR *filename)
{
    CHAR *fullpath = NULL;
    CHAR *buf = NULL;
    INT32 ret;
    CHAR *shellWorkingDirectory = OsShellGetWorkingDirectory();
    UINT32 logSize, bufSize, head, tail, intSave;
    CHAR *logBuf = NULL;

    LOS_SpinLockSave(&g_dmesgSpin, &intSave);
    if (OsCheckError()) {
        LOS_SpinUnlockRestore(&g_dmesgSpin, intSave);
        return -1;
    }
    logSize = g_dmesgInfo->logSize;
    bufSize = g_logBufSize;
    head = g_dmesgInfo->logHead;
    tail = g_dmesgInfo->logTail;
    logBuf = g_dmesgInfo->logBuf;
    LOS_SpinUnlockRestore(&g_dmesgSpin, intSave);

    ret = vfs_normalize_path(shellWorkingDirectory, filename, &fullpath);
    if (ret != 0) {
        return -1;
    }

    buf = (CHAR *)malloc(logSize);
    if (buf == NULL) {
        goto ERR_OUT2;
    }

    if (head < tail) {
        ret = memcpy_s(buf, logSize, logBuf + head, logSize);
        if (ret != EOK) {
            goto ERR_OUT3;
        }
    } else {
        ret = memcpy_s(buf, logSize, logBuf + head, bufSize - head);
        if (ret != EOK) {
            goto ERR_OUT3;
        }
        ret = memcpy_s(buf + bufSize - head, logSize - (bufSize - head), logBuf, tail);
        if (ret != EOK) {
            goto ERR_OUT3;
        }
    }

    ret = OsDmesgWrite2File(fullpath, buf, logSize);
ERR_OUT3:
    free(buf);
ERR_OUT2:
    free(fullpath);
    return ret;
}
#else
INT32 LOS_DmesgToFile(CHAR *filename)
{
    (VOID)filename;
    PRINTK("File operation need VFS\n");
    return -1;
}
#endif

INT32 OsShellCmdDmesg(INT32 argc, const CHAR **argv)
{
    if (argc == 1) {
        PRINTK("\n");
        OsLogShow();
        return LOS_OK;
    } else if (argc == 2) { /* 2: count of parameters */
        if (!strcmp(argv[1], "-c")) {
            PRINTK("\n");
            OsLogShow();
            LOS_DmesgClear();
            return LOS_OK;
        } else if (!strcmp(argv[1], "-C")) {
            LOS_DmesgClear();
            return LOS_OK;
        } else if (!strcmp(argv[1], "-D")) {
            OsLockConsole();
            return LOS_OK;
        } else if (!strcmp(argv[1], "-E")) {
            OsUnlockConsole();
            return LOS_OK;
        } else if (!strcmp(argv[1], "-L")) {
            OsLockUart();
            return LOS_OK;
        } else if (!strcmp(argv[1], "-U")) {
            OsUnlockUart();
            return LOS_OK;
        }
    } else if (argc == 3) { /* 3: count of parameters */
        if (!strcmp(argv[1], ">")) {
            if (LOS_DmesgToFile((CHAR *)argv[2]) < 0) { /* 2:index of parameters */
                PRINTK("Dmesg write log to %s fail \n", argv[2]); /* 2:index of parameters */
                return -1;
            } else {
                PRINTK("Dmesg write log to %s success \n", argv[2]); /* 2:index of parameters */
                return LOS_OK;
            }
        } else if (!strcmp(argv[1], "-l")) {
            return OsDmesgLvSet(argv[2]); /* 2:index of parameters */
        } else if (!strcmp(argv[1], "-s")) {
            return OsDmesgMemSizeSet(argv[2]); /* 2:index of parameters */
        }
    }

    PRINTK("dmesg: invalid option or parameter.\n");
    return -1;
}

SHELLCMD_ENTRY(dmesg_shellcmd, CMD_TYPE_STD, "dmesg", XARGS, (CmdCallBackFunc)OsShellCmdDmesg);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif /* LOSCFG_SHELL_DMESG */
