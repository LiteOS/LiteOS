/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Time64 Head File
 * Author: Huawei LiteOS Team
 * Create: 2020-05-15
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


#ifndef _HW_TIME64_H_
#define _HW_TIME64_H_

#if defined(__LP64__)
#error Your time_t is already 64-bit.
#else
#include <sys/time.h>
#include <time.h>
#include <stdint.h>

#if defined(__LITEOS__) && !defined(__LP64___)
#include <bits/alltypes.h>
#endif /* __LITEOS__ && !__LP64___ */

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if defined(__LITEOS__) && !defined(__LP64___)
#ifndef __NEED_int64_t
#define __NEED_int64_t
#endif

struct timeval64 {
    int64_t tv_sec;
    int64_t tv_usec;
};

struct timespec64 {
    int64_t tv_sec;
    int64_t tv_nsec;
};

/**
 * @ingroup  time
 * @par Description:
 * This function  sets the time as well as a timezone in 32-bit system.
 *
 * @attention
 * <ul>
 * <li>The function is not supported to set timezone,So the second parameter is unused</li>
 * </ul>
 *
 * @retval #0  The function is executed successfully.
 * @retval #-1 The function failed to execute, and corresponding error code is set.
 *
 * @par Errors
 * <ul>
 * <li><b>EINVAL</b>: An invalid Input.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>time64.h</li></ul>
 *
 * @see  clock_gettime | time | ctime
 *
 * @since Huawei LiteOS V100R001C00
 */
int settimeofday64(const struct timeval64 *, const struct timezone *);

/**
 * @ingroup  time
 * @par Description:
 * This function  gets the time as well as a timezone in 32-bit system.
 *
 * @attention
 * <ul>
 * <li>The function is not supported to get timezone,So the second parameter is unused</li>
 * </ul>
 *
 * @retval #0  The function is executed successfully.
 * @retval #-1 The function failed to execute, and corresponding error code is set.
 *
 * @par Errors
 * <ul>
 * <li><b>EINVAL</b>: An invalid Input.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>time64.h</li></ul>
 *
 * @see  clock_gettime | time | ctime
 *
 * @since Huawei LiteOS V100R001C00
 */
int gettimeofday64(struct timeval64 *, struct timezone *);
#endif

typedef int64_t time64_t;

char *asctime64(const struct tm *);
char *asctime64_r(const struct tm *, char *);
char *ctime64(const time64_t *);
char *ctime64_r(const time64_t *, char *);
struct tm *gmtime64(const time64_t *);
struct tm *gmtime64_r(const time64_t *, struct tm *);
struct tm *localtime64(const time64_t *);
struct tm *localtime64_r(const time64_t *, struct tm *);
time64_t mktime64(struct tm *);
time64_t timegm64(const struct tm *);
time64_t timelocal64(const struct tm *);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __LP64__ */
#endif /* _HW_TIME64_H_ */
