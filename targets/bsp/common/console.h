/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: System Console HeadFile
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

#ifndef _CONSOLE_H
#define _CONSOLE_H

#include "los_config.h"
#ifdef LOSCFG_FS_VFS
#include "asm/termbits.h"
#ifdef LOSCFG_NET_TELNET
#include "telnet_dev.h"
#endif
#include "virtual_serial.h"
#include "los_cir_buf_pri.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_FS_VFS

/* Define two fixed console id for Console ID. */
#define CONSOLE_SERIAL 1
#define CONSOLE_TELNET 2

#define LOSCFG_PLATFORM_CONSOLE
#define STDIN  0
#define STDOUT 1
#define STDERR 2

#define CONSOLE  "/dev/console"
#define CONSOLE_NAMELEN 16

#define CONSOLE_CMD_RD_BLOCK_SERIAL    0x104
#define CONSOLE_CMD_RD_BLOCK_TELNET    101
#define CONSOLE_RD_BLOCK               1
#define CONSOLE_RD_NONBLOCK            0
#define CONSOLE_SHELL_KEY_EVENT        0x112
#define CONSOLE_SHELL_EXITED           0x400
#define CONSOLE_CONTROL_RIGHTS_CAPTURE 201
#define CONSOLE_CONTROL_RIGHTS_RELEASE 202
#define CONSOLE_FIFO_SIZE              1024
#define CONSOLE_NUM                    2


#define TELNET_CIRBUF_SIZE 0x1000

typedef struct {
    CirBuf cirBufCB;        /* Circular buffer CB */
    EVENT_CB_S sendEvent;   /* Inform telnet send task */
} CirBufSendCB;

typedef struct {
    UINT32 consoleID;
    UINT32 consoleType;
    UINT32 consoleSem;
    UINT32 shellEntryId;
    UINT32 consoleMask;
    struct inode *devInode;
    CHAR *name;
    INT32 fd;
    UINT32 refCount;
    BOOL isNonBlock;
#ifdef LOSCFG_SHELL
    VOID *shellHandle;
#endif
    UINT32 sendTaskID;
    CirBufSendCB *cirBufSendCB;
    UINT8 fifo[CONSOLE_FIFO_SIZE];
    UINT32 fifoOut;
    UINT32 fifoIn;
    UINT32 currentLen;
    struct termios consoleTermios;
} CONSOLE_CB;

extern INT32 system_console_init(const CHAR *deviceName);
extern INT32 system_console_deinit(const CHAR *deviceName);
extern BOOL SetSerialNonBlock(const CONSOLE_CB *consoleCB);
extern BOOL SetSerialBlock(const CONSOLE_CB *consoleCB);
extern BOOL SetTelnetNonBlock(const CONSOLE_CB *consoleCB);
extern BOOL SetTelnetBlock(const CONSOLE_CB *consoleCB);
extern CONSOLE_CB *OsGetConsoleByID(INT32 consoleID);
extern CONSOLE_CB *OsGetConsoleByTaskID(UINT32 taskID);
extern VOID ConsoleTaskReg(INT32 consoleID, UINT32 taskID);
extern INT32 ConsoleUpdateFd(VOID);
extern BOOL ConsoleEnable(VOID);
extern BOOL is_nonblock(const CONSOLE_CB *consoleCB);
extern BOOL IsConsoleOccupied(const CONSOLE_CB *consoleCB);
extern INT32 FilepOpen(struct file *filep, const struct file_operations_vfs *fops);
extern INT32 FilepClose(struct file *filep, const struct file_operations_vfs *fops);
extern INT32 FilepRead(struct file *filep, const struct file_operations_vfs *fops, CHAR *buffer, size_t bufLen);
extern INT32 FilepWrite(struct file *filep, const struct file_operations_vfs *fops, const CHAR *buffer, size_t bufLen);
extern INT32 FilepPoll(struct file *filep, const struct file_operations_vfs *fops, poll_table *fds);
extern INT32 FilepIoctl(struct file *filep, const struct file_operations_vfs *fops, INT32 cmd, unsigned long arg);
extern INT32 GetFilepOps(const struct file *filep, struct file **privFilep, const struct file_operations_vfs **fops);

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _CONSOLE_H */
