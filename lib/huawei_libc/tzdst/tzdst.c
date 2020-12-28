/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: LiteOS Time Zone Implementation
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
#include "tzdst.h"
#include "tzdst_pri.h"
#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "time.h"
#include "pthread.h"
#include "lock.h"
#ifndef LOSCFG_AARCH64
#include "time64.h"
#endif
#include "securec.h"
#include "los_printf.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define SECSPERMIN 60
#define MINSPERHOUR 60
#define HOURSPERDAY 24
#define DAYSPERWEEK 7
#define SECSPERHOUR (SECSPERMIN * MINSPERHOUR)
#define SECSPERDAY ((int_fast32_t) SECSPERHOUR * HOURSPERDAY)
#define MONSPERYEAR 12

#define TM_YEAR_BASE 1900
#define isleap(y) (((y) % 4) == 0 && (((y) % 100) != 0 || ((y) % 400) == 0))

/* 2: leap year or normal year */
STATIC const INT32 g_monLengths[2][MONSPERYEAR] = {
    { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
    { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

/* Time Zone functions */
#define IS_NUM(x) (((x) >= '0') && ((x) <= '9'))

/* used in localtime_r.c */
long timezone;

STATIC VOID BufferInsert(CHAR *buf, size_t bufLen, size_t positions, CHAR data)
{
    if (bufLen <= positions) {
        return;
    }
    if (memmove_s(&buf[positions + 1], bufLen - positions - 1, &buf[positions], bufLen - positions - 1) != EOK) {
        dprintf("%s falied \n", __FUNCTION__);
        return;
    }

    buf[positions] = data;
}

#define OPERATE_OFF     3
#define HOUR_HIGH_OFF   4
#define HOUR_LOW_OFF    5
#define MIN_HIGH_OFF    7
#define MIN_LOW_OFF     8
#define SEC_HIGH_OFF    10
#define SEC_LOW_OFF     11

/*
 * tzn[+/-]hh[:mm[:ss]][dzn]
 * tzn  +  11 :30 : 7   dzn
 * tzn  -  9  : 7 :11   dzn
 */
STATIC BOOL TimezoneFormat(CHAR *standardString, size_t bufLen)
{
    if ((standardString[OPERATE_OFF] == '-') || (standardString[OPERATE_OFF] == '+')) {
        if (!IS_NUM(standardString[OPERATE_OFF + 1])) {
            return FALSE;
        }
    } else if (IS_NUM(standardString[OPERATE_OFF])) {
        BufferInsert(standardString, bufLen, OPERATE_OFF, '+'); /* no operate is default to add */
    } else {
        return FALSE;
    }

    if (!IS_NUM(standardString[HOUR_LOW_OFF])) {
        BufferInsert(standardString, bufLen, HOUR_HIGH_OFF, '0'); /* hour only one bit, padding 0 to high bit */
    }

    if (standardString[HOUR_LOW_OFF + 1] == ':') {
        if (!IS_NUM(standardString[MIN_HIGH_OFF])) {
            return FALSE;
        } else if (!IS_NUM(standardString[MIN_LOW_OFF])) {
            BufferInsert(standardString, bufLen, MIN_HIGH_OFF, '0'); /* minute only one bit, padding 0 to high bit */
        }
    } else {
        /* no minute bits, default is 0 */
        BufferInsert(standardString, bufLen, HOUR_LOW_OFF + 1, ':');
        BufferInsert(standardString, bufLen, MIN_HIGH_OFF, '0');
        BufferInsert(standardString, bufLen, MIN_LOW_OFF, '0');
    }

    if (standardString[MIN_LOW_OFF + 1] == ':') {
        if (!IS_NUM(standardString[SEC_HIGH_OFF])) {
            return FALSE;
        } else if (!IS_NUM(standardString[SEC_LOW_OFF])) {
            BufferInsert(standardString, bufLen, SEC_HIGH_OFF, '0'); /* second only one bit, padding 0 to high bit */
        }
    } else {
        /* no second bits, default is 0 */
        BufferInsert(standardString, bufLen, MIN_LOW_OFF + 1, ':');
        BufferInsert(standardString, bufLen, SEC_HIGH_OFF, '0');
        BufferInsert(standardString, bufLen, SEC_LOW_OFF, '0');
    }
    return TRUE;
}

STATIC INLINE INT32 StringToDigital(CHAR high, CHAR low)
{
    /* 10: decimal base number */
    return ((high - '0') * 10) + (low - '0');
}

/*
 * tzn[+/-]hh[:mm[:ss]][dzn]
 * tzn  +  11 :30 : 7   dzn
 * tzn  -  9  : 7 :11   dzn
 */
void settimezone(const char *buff)
{
#define STANDARD_TZ_LEN 15
#define MIN_BUF_LEN     (OPERATE_OFF + 1)
    INT32 hour;
    INT32 minute;
    INT32 second;
    size_t buffLen;
    CHAR standardString[STANDARD_TZ_LEN] = {0};

    if (buff == NULL) {
        goto ERROR;
    }

    buffLen = strlen(buff);
    if (buffLen < MIN_BUF_LEN) {
        goto ERROR;
    }

    (VOID)memset_s(standardString, STANDARD_TZ_LEN, '#', STANDARD_TZ_LEN);
    if (memcpy_s(standardString, STANDARD_TZ_LEN, buff, buffLen) != EOK) {
        goto ERROR;
    }

    if (!TimezoneFormat(standardString, STANDARD_TZ_LEN)) {
        goto ERROR;
    }

    hour = StringToDigital(standardString[HOUR_HIGH_OFF], standardString[HOUR_LOW_OFF]);
    minute = StringToDigital(standardString[MIN_HIGH_OFF], standardString[MIN_LOW_OFF]);
    second = StringToDigital(standardString[SEC_HIGH_OFF], standardString[SEC_LOW_OFF]);
    /* [-12:00:00, +14:00:00] limits */
    if (((minute > 59) || (second > 59)) ||
        ((standardString[OPERATE_OFF] == '-') && ((hour > 12) || ((hour == 12) && ((minute != 0) || (second != 0))))) ||
        ((standardString[OPERATE_OFF] == '+') && ((hour > 14) || ((hour == 14) && ((minute != 0) || (second != 0)))))) {
        goto ERROR;
    }

    if (LOCK(lock)) {
        goto ERROR;
    }

    /* 1h: 3600s, 1min: 60s */
    timezone = hour * 3600 + minute * 60 + second;
    if (standardString[OPERATE_OFF] == '-') {
        timezone = -timezone;
    }

    (void)UNLOCK(lock);

    return;

ERROR:
    PRINT_ERR("TZ file data error\n");
}

/* DST functions */
#define DST_STR_LEN_FORMAT_MDAY 15 /* for example "Feb-03 03:00:00" */
#define DST_STR_LEN_FORMAT_WDAY 20 /* for example "Oct-1st-Fri 02:59:59" */
#define DST_SET_LENGTH_MAX (DST_STR_LEN_FORMAT_WDAY + 1)

#define MONTH_NAME_LEN 3
STATIC const CHAR *g_strMonth[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
                                    "Aug", "Sep", "Oct", "Nov", "Dec" };
STATIC const CHAR *g_strMonthWeek[] = { "1st", "2nd", "3rd", "4th", "5th" };
STATIC const CHAR *g_strWeekDay[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

STATIC BOOL g_isDstWork = FALSE;
INT32 g_dstForwardSeconds = 0;
STATIC CHAR g_strDstStart[DST_SET_LENGTH_MAX] = {0};
STATIC CHAR g_strDstEnd[DST_SET_LENGTH_MAX] = {0};

INT32 DstForwardSecondGet(VOID)
{
    return g_dstForwardSeconds;
}

STATIC INT32 GetMonthFromString(const CHAR *strMonth)
{
    UINT32 index;
    for (index = 0; index < sizeof(g_strMonth) / sizeof(CHAR *); index++) {
        if (strncmp(strMonth, g_strMonth[index], MONTH_NAME_LEN) == 0) {
            /* A legal month is from 0 to 11. */
            return (int)index;
        }
    }

    return -1;
}

STATIC INT32 GetWeekDayFromString(const CHAR *strWeekDay)
{
    UINT32 index;
    for (index = 0; index < sizeof(g_strWeekDay) / sizeof(CHAR *); index++) {
        if (strncmp(strWeekDay, g_strWeekDay[index], MONTH_NAME_LEN) == 0) {
            /* Day of the week (0-6, Sunday = 0) */
            return (INT32)index;
        }
    }

    return -1;
}

STATIC INT32 GetMonthWeekFromString(const CHAR *strMonthWeek)
{
    UINT32 index;
    for (index = 0; index < sizeof(g_strMonthWeek) / sizeof(CHAR *); index++) {
        if (strncmp(strMonthWeek, g_strMonthWeek[index], MONTH_NAME_LEN) == 0) {
            /* Month of the week (1-5) */
            return (INT32)(index + 1);
        }
    }

    return -1;
}

STATIC INLINE INT32 CheckTimeVaild(const CHAR *strTimeString)
{
    if (((strTimeString[0] < '0') || (strTimeString[0] > '9')) ||
        ((strTimeString[1] < '0') || (strTimeString[1] > '9'))) {
        return -1;
    }
    return 0;
}

/* Day of the month 1 ~ 31 */
STATIC INT32 GetMonthDayFromString(INT32 month, const CHAR *strMonDay)
{
    INT32 monDay;

    if (CheckTimeVaild(strMonDay)) {
        return -1;
    }

    monDay = StringToDigital(strMonDay[0], strMonDay[1]);
    if (monDay > 31) {
        return -1;
    }

    /* Not every year have 29 days in Feb when set DST. */
    if ((monDay == 29) && ((month + 1) == 2)) {
        return -1;
    }

    if (monDay > g_monLengths[0][month]) {
        return -1;
    }

    /* Day of the month (1-31) */
    return monDay;
}

/*
 * time format HH:MM:SS
 * index       01234567
 * 0~23 for hours per day
 * 0~59 for minutes per hour
 * 0~59 for seconds per minute
 */
STATIC INT32 GetDaySecondsFromString(const CHAR *strTimeString)
{
    INT32 hour, min, sec;

    if (CheckTimeVaild(strTimeString)) {
        return -1;
    }

    if (CheckTimeVaild(strTimeString + 3)) {
        return -1;
    }

    if (CheckTimeVaild(strTimeString + 6)) {
        return -1;
    }

    if ((strTimeString[2] != ':') || (strTimeString[5] != ':')) {
        return -1;
    }

    hour = StringToDigital(strTimeString[0], strTimeString[1]);
    min = StringToDigital(strTimeString[3], strTimeString[4]);
    sec = StringToDigital(strTimeString[6], strTimeString[7]);
    /* Hours (0-23) */
    if ((hour < 0) || (hour > 23)) {
        return -1;
    }

    /* Minutes (0-59) */
    if ((min < 0) || (min > 59)) {
        return -1;
    }

    /* Seconds (0-59), not consider of the leap seconds in DST. */
    if ((sec < 0) || (sec > 59)) {
        return -1;
    }

    return hour * 3600 + min * 60 + sec;
}

STATIC INT32 DstGetDayOfMonth(INT32 year, INT32 month, INT32 mweek, INT32 wday)
{
#define FIRST_DAY 4   /* the first day of 1970.1.1 is Thursday. */
    INT32 firstWeekDay; /* First week day in this month of the specified year. */
    INT32 firstMdayOfTargetWday; /* First target month day in this month of the specified year. */
    INT32 targetMdayOfTargetWday; /* The target month day specified by user. */
    struct tm time = {0};
    INT64 seconds, days;

    time.tm_year = year;
    time.tm_mon = month;
    time.tm_mday = 1;
    /* 14: Hour-value range is [0,23] */
    time.tm_hour = 14;
    time.tm_isdst = 0;
#ifdef LOSCFG_AARCH64
    seconds = mktime(&time);
#else
    seconds = mktime64(&time);
#endif
    if (seconds == -1) {
        return -1;
    }
    days = seconds / SECSPERDAY;
    if (days < 0) {
        days = -days;
        firstWeekDay = DAYSPERWEEK - (days - (DAYSPERWEEK - FIRST_DAY)) % DAYSPERWEEK;
    } else {
        if (days > FIRST_DAY) {
            firstWeekDay = 1 + (days - FIRST_DAY) % DAYSPERWEEK;
        } else {
            firstWeekDay = FIRST_DAY;
        }
    }

    firstMdayOfTargetWday = 1 + (DAYSPERWEEK + wday - firstWeekDay) % DAYSPERWEEK;
    /*
     * Users may use 5th weekday to represent the last week of this month
     * for example "Oct-5th-Fri", but there does not exist the 5th Friday in October, so the last monweek is 4th.
     */
    targetMdayOfTargetWday = firstMdayOfTargetWday + (mweek - 1) * DAYSPERWEEK;
    if (targetMdayOfTargetWday > g_monLengths[(INT32)isleap(year + TM_YEAR_BASE)][month]) {
        targetMdayOfTargetWday -= 7;
    }

    return targetMdayOfTargetWday;
}

/*
 * time format decode
 * 1.  Feb-03 03:00:00
 * idx 012345678901234
 * 2.  Oct-1st-Fri 02:59:59
 * idx 12345678901234567890
 */
STATIC INT32 DateDecode(INT32 year, const CHAR *dstString, INT32 *month, INT32 *monDay, INT32 *sec)
{
    INT32 monWeek, weekDay;
    /* For example "Feb-03 03:00:00" */
    if (strlen(dstString) == DST_STR_LEN_FORMAT_MDAY) {
        if ((dstString[3] != '-') || (dstString[6] != ' ')) {
            return -1;
        }

        *month = GetMonthFromString(&dstString[0]);
        if (*month == -1) {
            return -1;
        }

        *monDay = GetMonthDayFromString(*month, &dstString[4]);
        if (*monDay == -1) {
            return -1;
        }

        *sec = GetDaySecondsFromString(&dstString[7]);
        if (*sec == -1) {
            return -1;
        }
    } else if (strlen(dstString) == DST_STR_LEN_FORMAT_WDAY) {
        /* For example "Oct-1st-Fri 02:59:59" */
        if ((dstString[3] != '-') || (dstString[7] != '-') || (dstString[11] != ' ')) {
            return -1;
        }

        *month = GetMonthFromString(&dstString[0]);
        if (*month == -1) {
            return -1;
        }

        monWeek = GetMonthWeekFromString(&dstString[4]);
        if (monWeek == -1) {
            return -1;
        }

        weekDay = GetWeekDayFromString(&dstString[8]);
        if (weekDay == -1) {
            return -1;
        }

        *sec = GetDaySecondsFromString(&dstString[12]);
        if (*sec == -1) {
            return -1;
        }

        *monDay = DstGetDayOfMonth(year, *month, monWeek, weekDay);
        if (*monDay  == -1) {
            return -1;
        }
    } else {
        return -1;
    }

    return 0;
}

STATIC INT64 DstConfigDecode(INT32 year, const CHAR *dstString)
{
    INT32 month, monDay, sec;
    INT32 ret;
    struct tm time = {0};
    INT64 dstSeconds;

    ret = DateDecode(year, dstString, &month, &monDay, &sec);
    if (ret == -1) {
        return -1;
    }
    /* get the DST period */
    time.tm_year = year;
    time.tm_mon = month;
    time.tm_mday = monDay;
    time.tm_isdst = 0;
#ifdef LOSCFG_AARCH64
    dstSeconds = mktime(&time);
#else
    dstSeconds = mktime64(&time);
#endif
    if (dstSeconds == -1) {
        return -1;
    }

    return dstSeconds + sec;
}

STATIC BOOL DstConfigCheck(const CHAR *strDstStart, const CHAR *strDstEnd)
{
    INT64 dstStart, dstEnd;
    const INT32 year = 70; /* 70 stands for epoch time */

    if ((strDstStart == NULL) || (strDstEnd == NULL)) {
        return FALSE;
    }

    dstStart = DstConfigDecode(year, strDstStart);
    dstEnd = DstConfigDecode(year, strDstEnd);
    if ((dstStart < 0) || (dstEnd < 0)) {
        return FALSE;
    }

    if (dstStart >= dstEnd) {
        return FALSE;
    }

    return TRUE;
}

STATIC BOOL CheckDstPeriodInner(const struct tm * const tm, INT64 seconds, INT64 dstStart, INT64 dstEnd)
{
    if (tm != NULL) {
        if (tm->tm_isdst < 0) {
            /* it must to be. */
            if ((seconds >= dstStart) && (seconds < dstStart + g_dstForwardSeconds)) {
                return FALSE;
            }

            /* determine the time period of the second pass, out of the DST period. */
            if ((seconds > dstEnd) && (seconds <= dstEnd + g_dstForwardSeconds)) {
                return TRUE;
            }
        } else if (tm->tm_isdst > 0) {
            /* the logical judgment here is the opposite of common sense */
            return TRUE;
        } else {
            /* tm->tm_isdst is zero */
            return FALSE;
        }
    }

    if ((seconds < dstStart) || (seconds >= dstEnd)) {
        return FALSE; /* daylight saving time is not effect. */
    }

    return TRUE;
}

BOOL CheckWithinDstPeriod(const struct tm * const tm, INT64 seconds)
{
    INT64 dstStart, dstEnd;
    struct tm time = {0};

    if (LOCK(lock)) {
        return FALSE;
    }
    if (g_isDstWork == FALSE) {
        (void)UNLOCK(lock);
        return FALSE;
    }

    (void)UNLOCK(lock);
    /* represent a local time. */
    if (tm != NULL) {
        (void)memcpy_s(&time, sizeof(struct tm), tm, sizeof(struct tm));
        time.tm_isdst = 0;
        /* the input-param of seconds is unused in this case. */
#ifdef LOSCFG_AARCH64
        seconds = mktime(&time);
#else
        seconds = mktime64(&time);
#endif
        if (seconds == -1) {
            return FALSE;
        }
    } else {
        /* represent a standard time, not care TZ. */
#ifdef LOSCFG_AARCH64
        if (gmtime_r(&seconds, &time) == NULL) {
#else
        if (gmtime64_r(&seconds, &time) == NULL) {
#endif
            return FALSE;
        }
    }

    dstStart = DstConfigDecode(time.tm_year, g_strDstStart);
    dstEnd = DstConfigDecode(time.tm_year, g_strDstEnd);
    if ((dstStart == -1) || (dstEnd == -1)) {
        return FALSE;
    }

    return CheckDstPeriodInner(tm, seconds, dstStart, dstEnd);
}

int dst_disable(VOID)
{
    if (LOCK(lock)) {
        return -1;
    }

    g_isDstWork = FALSE;

    (void)UNLOCK(lock);

    return 0;
}

int dst_enable(const char *strDstStartTime, const char *strDstEndTime, int swForwardSeconds)
{
    if (LOCK(lock)) {
        return -1;
    }

    /* Check if the format of dst config is correct. */
    if (DstConfigCheck(strDstStartTime, strDstEndTime) != TRUE) {
        (void)UNLOCK(lock);
        return -1;
    }

    if ((swForwardSeconds < 0) || (swForwardSeconds >= 24 * 3600)) { /* seconds per day 24 * 3600 */
        (void)UNLOCK(lock);
        return -1;
    }

    g_isDstWork = FALSE;
    if (strncpy_s(g_strDstStart, DST_SET_LENGTH_MAX, strDstStartTime, strlen(strDstStartTime)) != EOK) {
        (void)UNLOCK(lock);
        return -1;
    }
    if (strncpy_s(g_strDstEnd, DST_SET_LENGTH_MAX, strDstEndTime, strlen(strDstEndTime)) != EOK) {
        (void)UNLOCK(lock);
        return -1;
    }

    g_dstForwardSeconds = swForwardSeconds;
    g_isDstWork = TRUE;

    (void)UNLOCK(lock);

    return 0;
}

int dst_inquire(int year, struct tm *start, struct tm *end)
{
    INT64 dstStart, dstEnd;

    if (LOCK(lock)) {
        return -1;
    }

    if (!g_isDstWork) {
        (void)UNLOCK(lock);
        return -1;
    }

    if ((start == NULL) || (end == NULL)) {
        (void)UNLOCK(lock);
        return -1;
    }

    dstStart = DstConfigDecode(year, g_strDstStart);
    dstEnd = DstConfigDecode(year, g_strDstEnd);
    if ((dstStart == -1) || (dstEnd == -1)) {
        (void)UNLOCK(lock);
        return -1;
    }

    dstStart += timezone;
    dstEnd += timezone;
#ifdef LOSCFG_AARCH64
    if ((gmtime_r(&dstStart, start) == NULL) || (gmtime_r(&dstEnd, end) == NULL)) {
#else
    if ((gmtime64_r(&dstStart, start) == NULL) || (gmtime64_r(&dstEnd, end) == NULL)) {
#endif
        (void)UNLOCK(lock);
        return -1;
    }

    (void)UNLOCK(lock);
    return 0;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
