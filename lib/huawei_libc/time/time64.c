/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Time64 file
 * Author: Huawei LiteOS Team
 * Create: 2020-08-28
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

#ifndef __LP64__
#include "time64.h"
#include "time_impl.h"
#include "errno.h"
#include "limits.h"
#include "tzdst_pri.h"
#include "lock.h"

#define TM tm
#define RETURN_STRING_LEN 35
#define MAX_SEC_PER_YEAR (366LL * 24LL * 60LL * 60LL)
#define TM_YEAR_BASE 1900
#define TM_YEAR_MAX  9999
typedef time64_t Time64_T;
static struct TM g_returnDate;
static char g_returnString[RETURN_STRING_LEN];

static int valid_tm_wday(const struct TM *date)
{
    if ((date->tm_wday >= 0) && (date->tm_wday <= 6)) {
        return 1;
    } else {
        return 0;
    }
}

static int valid_tm_mon(const struct TM *date)
{
    if ((date->tm_mon >= 0) && (date->tm_mon <= 11)) {
        return 1;
    } else {
        return 0;
    }
}

struct TM *localtime64_r(const Time64_T *time, struct TM *local_tm)
{
    Time64_T timeoff;
    int dstsec = 0;

    LOCK(lock);
    if (CheckWithinDstPeriod(NULL, *time) == TRUE) {
        dstsec = DstForwardSecondGet();
    }
    timeoff = *time + timezone + dstsec;
    UNLOCK(lock);

    /* Reject time_t values whose year would overflow int because
     * __secs_to_zone cannot safely handle them. */
    if ((timeoff < (INT_MIN * MAX_SEC_PER_YEAR)) || (timeoff > (INT_MAX * MAX_SEC_PER_YEAR))) {
        errno = EOVERFLOW;
        return 0;
    }

    if (__secs_to_tm(timeoff, local_tm) < 0) {
        errno = EOVERFLOW;
        return 0;
    }
    return local_tm;
}

struct TM *localtime64(const Time64_T *time)
{
    return localtime64_r(time, &g_returnDate);
}

char *asctime64_r(const struct TM *date, char *result)
{
    if (!valid_tm_wday(date) || !valid_tm_mon(date)) {
        return NULL;
    }

    if ((TM_YEAR_BASE + date->tm_year) > TM_YEAR_MAX) {
        return NULL;
    }

    return asctime_r(date, result);
}

char *asctime64(const struct TM *date)
{
    return asctime64_r(date, g_returnString);
}

char *ctime64(const Time64_T *time)
{
    struct TM *ptm = localtime64(time);
    if (ptm != NULL) {
        return asctime64(ptm);
    }
    return NULL;
}

Time64_T mktime64(struct TM *tm)
{
    long long t = __tm_to_secs(tm);
    int dstsec = 0;

    LOCK(lock);
    if (tm->tm_isdst != 0) {
        if (CheckWithinDstPeriod(tm, 0) == TRUE) {
            dstsec = DstForwardSecondGet();
        }
        tm->tm_isdst = 0;
    }
    t = t - timezone - dstsec;
    UNLOCK(lock);

    if ((Time64_T)t != t) {
        goto error;
    }

    return t;

error:
    UNLOCK(lock);
    errno = EOVERFLOW;
    return -1;
}

struct TM *gmtime64_r(const Time64_T *in_time, struct TM *tm)
{
    if (__secs_to_tm(*in_time, tm) < 0) {
        errno = EOVERFLOW;
        return 0;
    }
    tm->tm_isdst = 0;
    tm->__tm_gmtoff = 0;
    tm->__tm_zone = __utc;
    return tm;
}

struct TM *gmtime64(const Time64_T *time)
{
    return gmtime64_r(time, &g_returnDate);
}

char *ctime64_r(const Time64_T *time, char *result)
{
    struct TM date;

    (void)localtime64_r(time, &date);
    return asctime64_r(&date, result);
}
#endif
