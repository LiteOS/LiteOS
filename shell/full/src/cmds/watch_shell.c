/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: LiteOS Shell Watch Command Implementation File
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

#include "unistd.h"
#include "stdio.h"
#include "securec.h"
#include "los_event.h"
#if defined(__LP64__)
#define  timeval64      timeval
#define  gettimeofday64 gettimeofday
#define  ctime64        ctime
#include "sys/time.h"
#else
#include "time64.h"
#endif
#include "shcmd.h"
#include "shmsg.h"

typedef struct {
    BOOL title; /* whether to hide the timestamps */
    UINT32 count; /* the total number of command executions */
    UINT32 interval; /* running cycle of the command */
    EVENT_CB_S watchEvent; /* event handle of the watch structure */
    CHAR cmdBuf[CMD_MAX_LEN]; /* the command to watch */
} WatchCB;

STATIC WatchCB *g_watchCmd;

#define WATCH_COUNT_MAX 0xFFFFFF
#define WATCH_INTETVAL_MAX 0xFFFFFF
#define WATCH_EVENT_MASK 0x01U

STATIC VOID PrintTime(VOID)
{
    struct timeval64 nowTime = {0};

    if (gettimeofday64(&nowTime, NULL) == 0) {
        PRINTK("%s", ctime64(&nowTime.tv_sec));
    }
}

STATIC VOID OsShellCmdDoWatch(VOID *arg1)
{
    WatchCB *watchItem = (WatchCB *)arg1;
    UINT32 ret;
    g_watchCmd = watchItem;

    while (watchItem->count--) {
        PRINTK("\033[2J\n");
        if (watchItem->title) {
            PrintTime();
        }
        (VOID)ShellMsgParse(watchItem->cmdBuf);
        ret = LOS_EventRead(&watchItem->watchEvent, WATCH_EVENT_MASK,
                            LOS_WAITMODE_OR | LOS_WAITMODE_CLR, watchItem->interval);
        if (ret == WATCH_EVENT_MASK) {
            break;
        }
    }

    (VOID)LOS_EventDestroy(&watchItem->watchEvent);
    free(g_watchCmd);
    g_watchCmd = NULL;
    PRINTK("\nHuawei LiteOS # ");
}

STATIC INLINE VOID OsWatchCmdUsage(VOID)
{
    PRINTK("\nUsage: watch\n");
    PRINTK("watch [options] command\n");
}

STATIC UINT32 OsWatchOverFunc(VOID)
{
    UINT32 ret;
    if (g_watchCmd != NULL) {
        ret = LOS_EventWrite(&g_watchCmd->watchEvent, WATCH_EVENT_MASK);
        if (ret != LOS_OK) {
            PRINT_ERR("Write event failed in %s,%d\n", __FUNCTION__, __LINE__);
            return OS_ERROR;
        }
        return LOS_OK;
    } else {
        PRINTK("No watch task to turn off.\n");
        return OS_ERROR;
    }
}

STATIC INT32 OsWatchOptionParsed(UINT32 argc, UINT32 *argoff, const CHAR **argv, WatchCB *watchItem)
{
    long tmpVal;
    CHAR *strPtr = NULL;
    UINT32 argCount = argc;

    while (argv[*argoff][0] == '-') {
        if (argCount <= 1) {
            OsWatchCmdUsage();
            return -1;
        }

        if ((strcmp(argv[*argoff], "-n") == 0) || (strcmp(argv[*argoff], "--interval") == 0)) {
            if (argCount <= 2) { /* 2:count of parameter */
                OsWatchCmdUsage();
                return -1;
            }
            tmpVal = (long)strtoul(argv[*argoff + 1], &strPtr, 0);
            if ((*strPtr != 0) || (tmpVal <= 0) || (tmpVal > WATCH_INTETVAL_MAX)) {
                PRINTK("\ninterval time is invalid\n");
                OsWatchCmdUsage();
                return -1;
            }
            watchItem->interval = g_tickPerSecond * (UINT32)tmpVal;
            argCount -= 2; /* 2:offset of argv */
            (*argoff) += 2; /* 2:offset of argv */
        } else if ((strcmp(argv[*argoff], "-t") == 0) || (strcmp(argv[*argoff], "-no-title") == 0)) {
            watchItem->title = FALSE;
            argCount--;
            (*argoff)++;
        } else if ((strcmp(argv[*argoff], "-c") == 0) || (strcmp(argv[*argoff], "--count") == 0)) {
            if (argCount <= 2) { /* 2:count of parameter */
                OsWatchCmdUsage();
                return -1;
            }
            tmpVal = (long)strtoul(argv[*argoff + 1], &strPtr, 0);
            if ((*strPtr != 0) || (tmpVal <= 0) || (tmpVal > WATCH_COUNT_MAX)) {
                PRINTK("\ncount is invalid\n");
                OsWatchCmdUsage();
                return -1;
            }
            watchItem->count = (UINT32)tmpVal;
            argCount -= 2; /* 2:offset of argv */
            (*argoff) += 2; /* 2:offset of argv */
        } else {
            PRINTK("Unknown option.\n");
            return -1;
        }
    }
    return 0;
}

STATIC INT32 OsWatchCmdSplice(UINT32 argc, UINT32 argoff, const CHAR **argv, WatchCB *watchItem)
{
    INT32 err = 0;
    if ((argc - argoff) == 0) {
        PRINT_ERR("no watch command!\n");
        return -1;
    }
    while (argc - argoff) {
        err = strcat_s(watchItem->cmdBuf, sizeof(watchItem->cmdBuf), argv[argoff]);
        if (err != EOK) {
            PRINT_ERR("%s, %d strcat_s failed!\n", __FUNCTION__, __LINE__);
            return -1;
        }
        err = strcat_s(watchItem->cmdBuf, sizeof(watchItem->cmdBuf), " ");
        if (err != EOK) {
            PRINT_ERR("%s, %d strcat_s failed!\n", __FUNCTION__, __LINE__);
            return -1;
        }
        argoff++;
    }
    return err;
}

STATIC UINT32 OsWatchTaskCreate(WatchCB *watchItem)
{
    TSK_INIT_PARAM_S initParam = {0};
    UINT32 watchTaskId = 0;
    UINT32 ret;

    ret = LOS_EventInit(&watchItem->watchEvent);
    if (ret != 0) {
        PRINT_ERR("Watch event init failed in %s, %d\n", __FUNCTION__, __LINE__);
        return ret;
    }

    initParam.pfnTaskEntry = (TSK_ENTRY_FUNC)OsShellCmdDoWatch;
    initParam.usTaskPrio   = 10; /* 10:shellcmd_watch task priority */
    initParam.uwStackSize  = 0x3000; /* 0x3000:stack size of shellcmd_watch task */
    initParam.pcName       = "shellcmd_watch";
    initParam.uwResved     = LOS_TASK_STATUS_DETACHED;
    LOS_TASK_PARAM_INIT_ARG(initParam, watchItem);

    ret = LOS_TaskCreate(&watchTaskId, &initParam);
    if (ret != 0) {
        PRINT_ERR("Watch task init failed in %s, %d\n", __FUNCTION__, __LINE__);
        return ret;
    }
    return ret;
}

UINT32 OsShellCmdWatch(UINT32 argc, const CHAR **argv)
{
    WatchCB *watchItem = NULL;
    UINT32 argoff = 0;
    UINT32 ret;
    INT32 err;

    if (argc == 0) {
        OsWatchCmdUsage();
        return OS_ERROR;
    }

    if ((argc == 1) && (strcmp(argv[0], "--over") == 0)) {
        ret = OsWatchOverFunc();
        return ret;
    }

    if (g_watchCmd != NULL) {
        PRINTK("Please turn off previous watch before to start a new watch.\n");
        return OS_ERROR;
    }

    watchItem = (WatchCB *)malloc(sizeof(WatchCB));
    if (watchItem == NULL) {
        PRINTK("Malloc error!\n");
        return OS_ERROR;
    }
    (VOID)memset_s(watchItem, sizeof(WatchCB), 0, sizeof(WatchCB));
    watchItem->title = TRUE;
    watchItem->count = WATCH_COUNT_MAX;
    watchItem->interval = g_tickPerSecond;

    err = OsWatchOptionParsed(argc, &argoff, argv, watchItem);
    if (err != 0) {
        goto WATCH_ERROR;
    }

    err = OsWatchCmdSplice(argc, argoff, argv, watchItem);
    if (err != 0) {
        goto WATCH_ERROR;
    }

    ret = OsWatchTaskCreate(watchItem);
    if (ret != 0) {
        goto WATCH_ERROR;
    }

    return LOS_OK;

WATCH_ERROR:
    free(watchItem);
    return OS_ERROR;
}

SHELLCMD_ENTRY(watch_shellcmd, CMD_TYPE_EX, "watch", XARGS, (CmdCallBackFunc)OsShellCmdWatch);
