/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: LiteOS Date command Implementation File
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

#include "stdlib.h"
#include "sys/stat.h"
#include "securec.h"
#include "los_typedef.h"
#ifdef LOSCFG_FS_VFS
#include "inode/inode.h"
#endif
#include "shcmd.h"
#include "shell.h"

#if defined(__LP64__)
#define  timeval64      timeval
#define  settimeofday64 settimeofday
#define  gettimeofday64 gettimeofday
#define  localtime64    localtime
#define  ctime64        ctime
#define  mktime64       mktime
#include "sys/time.h"
#else
#include "time64.h"
#endif

#define  DATE_ERR_INFO      1
#define  DATE_HELP_INFO     0
#define  DATE_ERR           (-1)
#define  DATE_OK            0
#define  DATE_BASE_YEAR     1900
#define  LEAPYEAR(y) (((y) % 4) == 0 && (((y) % 100) != 0 || ((y) % 400) == 0))

STATIC const INT32 g_monLengths[2][12] = { /* 2: 2 Column,Contains leap year; 12: 12 months */
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

STATIC VOID OsCopyTm(struct tm *destTm, const struct tm *srcTm)
{
    if (srcTm == NULL) {
        (VOID)memset_s(destTm, sizeof(struct tm), 0, sizeof(struct tm));
    } else {
        destTm->tm_sec = srcTm->tm_sec;
        destTm->tm_min = srcTm->tm_min;
        destTm->tm_hour = srcTm->tm_hour;
        destTm->tm_mday = srcTm->tm_mday;
        destTm->tm_mon = srcTm->tm_mon;
        destTm->tm_year = srcTm->tm_year;
        destTm->tm_wday = srcTm->tm_wday;
        destTm->tm_yday = srcTm->tm_yday;
        destTm->tm_isdst = srcTm->tm_isdst;
        destTm->tm_gmtoff = srcTm->tm_gmtoff;
        destTm->tm_zone = srcTm->tm_zone;
    }
}

STATIC VOID OsCmdUsageDate(INT32 order)
{
    if (order) {
        PRINTK("date: invalid option or parameter.\n");
        PRINTK("Try 'date --help' for more information.\n");
        return;
    }
    PRINTK("\nUsage: date [+FORMAT]\n");
    PRINTK("   or: date [-s] [YY/MM/DD] [hh:mm:ss]\n");
    PRINTK("Display the current time in the given FORMAT, or set the system date.\n");
    PRINTK("FORMAT controls the output.  Interpreted sequences are:\n");
    PRINTK("  %%b     The abbreviated month name according to the current locale.\n");
    PRINTK("  %%B     The full month name according to the current locale.\n");
    PRINTK("  %%C     The century number (year/100) as a 2-digit integer. (SU)\n");
    PRINTK("  %%d     The day of the month as a decimal number (range 01 to 31).\n");
    PRINTK("  %%e     Like %%d, the day of the month as a decimal number, \n");
    PRINTK("         but a leading zero is replaced by a space.\n");
    PRINTK("  %%h     Equivalent to %%b.  (SU)\n");
    PRINTK("  %%H     The hour as a decimal number using a 24-hour clock (range 00 to 23).\n");
    PRINTK("  %%I     The hour as a decimal number using a 12-hour clock (range 01 to 12).\n");
    PRINTK("  %%j     The day of the year as a decimal number (range 001 to 366).\n");
    PRINTK("  %%k     The hour (24-hour clock) as a decimal number (range  0  to  23); \n");
    PRINTK("         single digits are preceded by a blank.  (See also %%H.)  (TZ)\n");
    PRINTK("  %%l     The hour (12-hour  clock) as a decimal number (range 1 to 12); \n");
    PRINTK("         single digits are preceded by a blank.  (See also %%I.)  (TZ)\n");
    PRINTK("  %%m     The month as a decimal number (range 01 to 12).\n");
    PRINTK("  %%M     The minute as a decimal number (range 00 to 59).\n");
    PRINTK("  %%n     A newline character. (SU)\n");
    PRINTK("  %%p     Either \"AM\" or \"PM\" according to the given time value, \n");
    PRINTK("         or the corresponding strings for the current locale.\n");
    PRINTK("         Noon is treated as \"PM\" and midnight as \"AM\".\n");
    PRINTK("  %%P     Like %%p but in lowercase: \"am\" or \"pm\" \n");
    PRINTK("         or a corresponding string for the current locale. (GNU)\n");
    PRINTK("  %%s     The number of seconds since the Epoch, that is,\n");
    PRINTK("         since 1970-01-01 00:00:00 UTC. (TZ)\n");
    PRINTK("  %%S     The second as a decimal number (range 00 to 60).\n");
    PRINTK("         (The range is up to 60 to allow for occasional leap seconds.)\n");
    PRINTK("  %%t     A tab character. (SU)\n");
    PRINTK("  %%y     The year as a decimal number without a century (range 00 to 99).\n");
    PRINTK("  %%Y     The year as a decimal number including the century.\n");
    PRINTK("  %%%%     A literal '%%' character.\n");
    PRINTK("\nExamples:\n");
    PRINTK("Set system date (2017-01-01)\n");
    PRINTK("$ date -s 20170101\n");
    PRINTK("Set system time (12:00:00)\n");
    PRINTK("$ date -s 12:00:00\n");
    PRINTK("Show the time with format Year-Month-Day\n");
    PRINTK("$ date +%%Y-%%m-%%d\n");
}

STATIC INT32 OsStrToTm(const CHAR *str, struct tm *tm)
{
    CHAR *ret = NULL;
    UINT32 strLen = strlen(str);
    if (strLen == 8) { /* 8:Time format string length, such as hh:mm:ss or yyyymmdd */
        if (str[2] == ':') { /* 2:Index of Eigenvalues */
            ret = strptime(str, "%H:%M:%S", tm);
        } else {
            ret = strptime(str, "%Y%m%d", tm);
        }
    } else if (strLen == 10) { /* 10:Time format string length,such as yyyy/mm/dd  */
        ret = strptime(str, "%Y/%m/%d", tm);
    } else if (strLen == 5) { /* 5:Time format string length,such as hh:mm or mm/dd */
        if (str[2] == ':') { /* 2:Index of Eigenvalues */
            ret = strptime(str, "%H:%M", tm);
        } else if (str[2] == '/') { /* 2:Index of Eigenvalues */
            ret = strptime(str, "%m/%d", tm);
        }
    } else if (strLen == 7) { /* 7:Time format string length,such as yyyy/mm */
        if (str[4] == '/') { /* 4:Index of Eigenvalues */
            ret = strptime(str, "%Y/%m", tm);
        }
    }

    if (tm->tm_year < 70) { /* 70:the year is starting in 1970,tm_year must be greater than 70 */
        PRINTK("\nUsage: date -s set system time range from 1970.\n");
        return DATE_ERR;
    }

    if (tm->tm_mday > g_monLengths[(INT32)LEAPYEAR(tm->tm_year + DATE_BASE_YEAR)][tm->tm_mon]) {
        return DATE_ERR;
    }

    if ((tm->tm_sec < 0) || (tm->tm_sec > 59)) { /* Seconds (0-59), leap seconds shall not be used when set time. */
        return DATE_ERR;
    }
    return (ret == NULL) ? DATE_ERR : DATE_OK;
}

STATIC INT32 OsFormatPrintTime(const CHAR *formatStr)
{
    CHAR timebuf[SHOW_MAX_LEN] = {0};
    struct tm *tm = NULL;
    struct timeval64 nowTime = {0};

    if (strlen(formatStr) < 2) { /* 2:check format string length */
        OsCmdUsageDate(DATE_ERR_INFO);
        return DATE_ERR;
    }

    if (gettimeofday64(&nowTime, NULL)) {
        return DATE_ERR;
    }
    tm = localtime64(&nowTime.tv_sec);
    if (tm == NULL) {
        return DATE_ERR;
    }

    if (strftime(timebuf, SHOW_MAX_LEN - 1, formatStr + 1, tm)) {
        PRINTK("%s\n", timebuf);
    } else {
        OsCmdUsageDate(DATE_ERR_INFO);
        return DATE_ERR;
    }
    return DATE_OK;
}

STATIC INT32 OsDateSetTime(const CHAR *timeStr)
{
    struct tm tm = {0};
    struct timeval64 nowTime = {0};
    struct timeval64 setTime = {0};

    if (gettimeofday64(&nowTime, NULL)) {
        PRINTK("settime failed...\n");
        return DATE_ERR;
    }

    setTime.tv_usec = nowTime.tv_usec;
    OsCopyTm(&tm, localtime64(&nowTime.tv_sec));

    if (OsStrToTm(timeStr, &tm)) {
        OsCmdUsageDate(DATE_ERR_INFO);
        return DATE_ERR;
    }

    setTime.tv_sec = mktime64(&tm);
    if (setTime.tv_sec == -1) {
        PRINTK("mktime failed...\n");
        return DATE_ERR;
    }

    if (settimeofday64(&setTime, NULL)) {
        PRINTK("settime failed...\n");
        return DATE_ERR;
    }

    return DATE_OK;
}

#ifdef  LOSCFG_FS_VFS
STATIC INT32 OsViewFileTime(const CHAR *filename)
{
#define BUFFER_SIZE 26 /* The buffer size is equal to the size used by the asctime_r interface */
    struct stat statBuf = {0};
    CHAR *fullpath = NULL;
    INT32 ret;
    CHAR buf[BUFFER_SIZE];
    CHAR *shellWorkingDirectory = OsShellGetWorkingDirectory();

    ret = vfs_normalize_path(shellWorkingDirectory, filename, &fullpath);
    if (ret < 0) {
        set_errno(-ret);
        perror("date error");
        return DATE_ERR;
    }

    if (stat(fullpath, &statBuf) != 0) {
        OsCmdUsageDate(DATE_ERR_INFO);
        free(fullpath);
        return DATE_ERR;
    }
    PRINTK("%s\n", ctime_r(&statBuf.st_mtim.tv_sec, buf));
    free(fullpath);
    return DATE_OK;
}
#endif

INT32 OsShellCmdDate(INT32 argc, const CHAR **argv)
{
    struct timeval64 nowTime = {0};

    if (argc == 1) { /* 1:count of parameters */
        if (gettimeofday64(&nowTime, NULL)) {
            return DATE_ERR;
        }
        PRINTK("%s\n", ctime64(&nowTime.tv_sec));
        return DATE_OK;
    }

    if (argc == 2) { /* 2:count of parameters */
        if (!(strcmp(argv[1], "--help"))) {
            OsCmdUsageDate(DATE_HELP_INFO);
            return DATE_OK;
        }
        if (!(strncmp(argv[1], "+", 1))) {
            return OsFormatPrintTime(argv[1]);
        }
    }

    if (argc > 2) { /* 2:count of parameters */
        if (!(strcmp(argv[1], "-s"))) {
            return OsDateSetTime(argv[2]); /* 2:index of parameters */
        }
#ifdef  LOSCFG_FS_VFS
        else if (!(strcmp(argv[1], "-r"))) {
            return OsViewFileTime(argv[2]); /* 2:index of parameters */
        }
#endif
    }

    OsCmdUsageDate(DATE_ERR_INFO);
    return DATE_OK;
}

SHELLCMD_ENTRY(date_shellcmd, CMD_TYPE_STD, "date", XARGS, (CmdCallBackFunc)OsShellCmdDate);
