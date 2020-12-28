/**
 * @defgroup time Time
 * @ingroup libc
 */

#ifndef	_TIME_H
#define _TIME_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#ifdef __LITEOS__
#undef NULL
#endif
#ifdef __cplusplus
#define NULL 0L
#else
#define NULL ((void*)0)
#endif


#define __NEED_size_t
#define __NEED_time_t
#define __NEED_clock_t
#define __NEED_struct_timespec

#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) \
 || defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) \
 || defined(_BSD_SOURCE)
#define __NEED_clockid_t
#define __NEED_timer_t
#define __NEED_pid_t
#define __NEED_locale_t
#endif

#include <bits/alltypes.h>

#if defined(_BSD_SOURCE) || defined(_GNU_SOURCE)
#define __tm_gmtoff tm_gmtoff
#define __tm_zone tm_zone
#endif

struct tm {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
	long __tm_gmtoff;
	const char *__tm_zone;
};

/**
 * @ingroup  time
 * @par Description:
 * This function gets the current time as an integer number of microseconds.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval
 * #clock_t This function returns the current time as an integer number of microseconds.
 *
 * @par Dependency:
 * <ul><li>time.h</li></ul>
 *
 * @see  asctime | clock_getres | ctime | difftime | gmtime | localtime | mktime | strftime |
 * strptime | time | times | utime
 *
 * @since Huawei LiteOS V100R001C00
 */
clock_t clock (void);

/**
 * @ingroup  time
 * @par Description:
 * This function gets the value of time in seconds since the Epoch(1970-01-01 00:00:00 UTC).
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval
 * #time_t On success, this function shall return the value of time in seconds since the Epoch.
 * the argument points to an area where the return value is also stored. If the argument is a null pointer,
 * no value is stored.On failure, this function shall return (time_t)-1.
 *
 * @par Dependency:
 * <ul><li>time.h</li></ul>
 *
 * @see  asctime | clock | clock_getres | ctime | difftime | gettimeofday | gmtime | localtime | mktime | strftime | strptime | times | utime
 *
 * @since Huawei LiteOS V100R001C00
 */
time_t time (time_t *);

/**
 * @ingroup  time
 * @par Description:
 * This function shall compute the difference between two calendar times (as returned by time()).
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval
 * #double This function return the difference expressed in seconds as a type double.
 *
 * @par Dependency:
 * <ul><li>time.h</li></ul>
 *
 * @see  clock | ctime | asctime | gmtime | localtime | mktime | strftime | strptime | times | utime
 *
 * @since Huawei LiteOS V100R001C00
 */
double difftime (time_t, time_t);

/**
 * @ingroup  time
 * @par Description:
 * This function shall convert the broken-down time, expressed as local time, in the structure pointed
 * to by the argument,into a time since the Epoch value with the same encoding as that of the values
 * returned by time(). The original values of the tm_wday and tm_yday components of the structure shall
 * be ignored, and the original values of the other components shall not be restricted to the ranges
 * described in <time.h>.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval
 * #time_t On success, shall return the specified time since the Epoch encoded as a value of type time_t.
 * On failure, If the time since the Epoch cannot be represented, the function shall return the value (time_t)-1.
 *
 * @par Errors
 * <ul>
 * <li><b>EINVAL</b>: An invalid input.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>time.h</li></ul>
 *
 * @see  asctime | clock | ctime | difftime | gmtime | localtime | strftime | strptime | time | tzset | utime
 *
 * @since Huawei LiteOS V100R001C00
 */
time_t mktime (struct tm *);

/**
 * @ingroup  time
 * @par Description:
 * This function formats the broken-down time tm according to the format specification format and places the result in the character
 * array s of size the second.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval
 * #size_t Provided  that  the  result  string, including the terminating null byte, does not exceed max bytes,
 * it returns the number of bytes (excluding the terminating null byte) placed in the array s.
 * If the length of the result string  (including  the  terminating  null  byte) would exceed max bytes,
 * then it returns 0, and the contents of the array are undefined.
 *
 * @par Dependency:
 * <ul><li>time.h</li></ul>
 *
 * @see  asctime | clock | clock_getres | ctime | difftime | gettimeofday | gmtime | localtime | mktime | strptime | times | utime | tzset
 *
 * @since Huawei LiteOS V100R001C00
 */
size_t strftime (char *__restrict, size_t, const char *__restrict, const struct tm *__restrict);

/**
 * @ingroup  time
 * @par Description:
 * This function convert the time in seconds since the Epoch pointed to by the argument into
 * a broken-down time, expressed as Coordinated Universal Time (UTC).
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval
 * "struct tm*" On success, this function returns a pointer to a struct tm.
 * On failure, it shall return a null pointer.
 *
 * @par Dependency:
 * <ul><li>time.h</li></ul>
 *
 * @see  asctime | clock | ctime | difftime | localtime | mktime | strftime | strptime | time | utime
 *
 * @since Huawei LiteOS V100R001C00
 */
struct tm *gmtime (const time_t *);

/**
 * @ingroup  time
 * @par Description:
 * This function shall convert the time in seconds since the Epoch pointed to by the argument
 * into a broken-down time, expressed as a local time.
 *
 * @attention
 * <ul>
 * <li>Failure compared with standard, the error code is not set.</li>
 * </ul>
 *
 * @retval
 * "struct tm*" On success, the function shall return a pointer to the broken-down time structure.
 * On failure, this shall return a null pointer.
 *
 * @par Dependency:
 * <ul><li>time.h</li></ul>
 *
 * @see  asctime | clock | ctime | difftime | gmtime | mktime | strftime | strptime | time | tzset | utime
 *
 * @since Huawei LiteOS V100R001C00
 */
struct tm *localtime (const time_t *);

/**
 * @ingroup  time
 * @par Description:
 * This function shall convert the broken-down time in the structure pointed to by the argument into a string in the form:\n
 * Sun Sep 16 01:03:52 1973\n\0.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval
 * #char* On success, this function shall return a pointer to the string. On failure, it shall return NULL.
 *
 * @par Dependency:
 * <ul><li>time.h</li></ul>
 *
 * @see  clock | ctime | difftime | gmtime | localtime | mktime | strftime | strptime | times | utime
 *
 * @since Huawei LiteOS V100R001C00
 */
char *asctime (const struct tm *);

/**
 * @ingroup  time
 * @par Description:
 * This function shall convert the time pointed to by the first argument, representing time in seconds since the Epoch,
 * to local time in the form of a string.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval
 * #char* The function returns the pointer returned by asctime() with that broken-down time as an argument.
 *
 * @par Dependency:
 * <ul><li>time.h</li></ul>
 *
 * @see  asctime | clock | difftime | gmtime | localtime | mktime | strftime | strptime | time | utime
 *
 * @since Huawei LiteOS V100R001C00
 */
char *ctime (const time_t *);
int timespec_get(struct timespec *, int);

#define CLOCKS_PER_SEC 1000000L

#define TIME_UTC 1

#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) \
 || defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) \
 || defined(_BSD_SOURCE)

size_t strftime_l (char *  __restrict, size_t, const char *  __restrict, const struct tm *  __restrict, locale_t);

/**
 * @ingroup  time
 * @par Description:
 * This function convert the time in seconds since the Epoch pointed to by the first argument into
 * a broken-down time expressed as Coordinated Universal Time (UTC). The broken-down time is stored
 * in the structure referred to by the second argument.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval
 * "struct tm*" On success, this function returns the address of the structure pointed to by the second argument.
 * On failure, it shall return a null pointer.
 *
 * @par Dependency:
 * <ul><li>time.h</li></ul>
 *
 * @see  asctime | clock | ctime | difftime | localtime | mktime | strftime | strptime | time | utime
 *
 * @since Huawei LiteOS V100R001C00
 */
struct tm *gmtime_r (const time_t *__restrict, struct tm *__restrict);

/**
 * @ingroup  time
 * @par Description:
 * This function shall convert the time in seconds since the Epoch pointed to by timer into
 * a broken-down time stored in the structure to which the second argument points.
 *
 * @attention
 * <ul>
 * <li>Failure compared with standard, the error code is not set. </li>
 * </ul>
 *
 * @retval
 * "struct tm*" On success, the function shall return a pointer to the structure pointed to by the second argument.
 * On failure, this shall return a null pointer.
 *
 * @par Dependency:
 * <ul><li>time.h</li></ul>
 *
 * @see  asctime | clock | ctime | difftime | gmtime | mktime | strftime | strptime | time | tzset | utime
 *
 * @since Huawei LiteOS V100R001C00
 */
struct tm *localtime_r (const time_t *__restrict, struct tm *__restrict);

/**
 * @ingroup  time
 * @par Description:
 * This function shall convert the broken-down time in the structure pointed to by first argument into a string.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval
 * #char* On success, this function shall return a pointer to a character string containing the date and time.
 * This string is pointed to by the second argument. On failure, it shall return NULL.
 *
 * @par Dependency:
 * <ul><li>time.h</li></ul>
 *
 * @see  clock | ctime | difftime | gmtime | localtime | mktime | strftime | strptime | times | utime
 *
 * @since Huawei LiteOS V100R001C00
 */
char *asctime_r (const struct tm *__restrict, char *__restrict);

/**
 * @ingroup  time
 * @par Description:
 * This function shall convert the calendar time pointed to by the first argument to local time in
 * exactly the same form as ctime() and put the string into the array pointed to by the second argument
 * (which shall be at least 26 bytes in size).
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval
 * #char* On success, the function returns a pointer to the string pointed to by second argument.
 * On failure, a null pointer shall be returned.
 *
 * @par Dependency:
 * <ul><li>time.h</li></ul>
 *
 * @see  asctime | clock | difftime | gmtime | localtime | mktime | strftime | strptime | time | utime
 *
 * @since Huawei LiteOS V100R001C00
 */
char *ctime_r (const time_t *, char *);

/**
 * @ingroup  time
 * @par Description:
 * This function uses the value of the environment variable TZ to set time conversion information used
 * by ctime, localtime, mktime, and strftime. If TZ is absent from the environment, implementation-defined default
 * timezone information shall be used..
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval
 * None
 *
 * @par Dependency:
 * <ul><li>time.h</li></ul>
 *
 * @see  ctime | localtime | mktime | strftime
 *
 * @since Huawei LiteOS V100R001C00
 */
void tzset (void);

struct itimerspec {
	struct timespec it_interval;
	struct timespec it_value;
};

#ifdef __LITEOS__
#define MAX_CLOCKS              16
#endif
#define CLOCK_REALTIME           0
#define CLOCK_MONOTONIC          1
#define CLOCK_PROCESS_CPUTIME_ID 2
#define CLOCK_THREAD_CPUTIME_ID  3
#define CLOCK_MONOTONIC_RAW      4
#define CLOCK_REALTIME_COARSE    5
#define CLOCK_MONOTONIC_COARSE   6
#define CLOCK_BOOTTIME           7
#define CLOCK_REALTIME_ALARM     8
#define CLOCK_BOOTTIME_ALARM     9
#define CLOCK_SGI_CYCLE         10
#define CLOCK_TAI               11

#define TIMER_ABSTIME 1

/**
 * @ingroup  time
 * @par Description:
 * This function shall cause the current thread to be suspended from execution
 * until either the time interval specified by the first argument has elapsed.
 *
 * @attention
 * <ul>
 * <li>The second argument is unused.</li>
 * <li>Tasks will not be awakened, if the requested time does not has elapsed </li>
 * </ul>
 *
 * @retval #0  The function is executed successfully.
 * @retval #-1 The function failed to execute, and corresponding error code is set.
 *
 * @par Errors
 * <ul>
 * <li><b>EINVAL</b>: An invalid input.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>time.h</li></ul>
 *
 * @see  sleep
 *
 * @since Huawei LiteOS V100R001C00
 */
int nanosleep (const struct timespec *, struct timespec *);

/**
 * @ingroup  time
 * @par Description:
 * This function finds  the  resolution  (precision) of the specified clock, and, if the second argument is non-NULL, stores it in the
 * struct timespec pointed to by the second argument.
 *
 * @attention
 * <ul>
 * <li>The first argument of value only supports CLOCK_REALTIME.</li>
 * </ul>
 *
 * @retval #0  The function is executed successfully.
 * @retval #-1 The function failed to execute, and corresponding error code is set.
 *
 * @par Errors
 * <ul>
 * <li><b>EINVAL</b>: An invalid input.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>time.h</li></ul>
 *
 * @see  gettimeofday | settimeofday | time | adjtime | ctime
 *
 * @since Huawei LiteOS V100R001C00
 */
int clock_getres (clockid_t, struct timespec *);

/**
 * @ingroup  time
 * @par Description:
 * This function retrieves the time of the specified clock.
 *
 * @attention
 * <ul>
 * <li>The first argument of value only supports CLOCK_REALTIME.</li>
 * </ul>
 *
 * @retval #0  The function is executed successfully.
 * @retval #-1 The function failed to execute, and corresponding error code is set.
 *
 * @par Errors
 * <ul>
 * <li><b>EINVAL</b>: An invalid input.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>time.h</li></ul>
 *
 * @see  gettimeofday | settimeofday | time | adjtime | ctime
 *
 * @since Huawei LiteOS V100R001C00
 */
int clock_gettime (clockid_t, struct timespec *);

/**
 * @ingroup  time
 * @par Description:
 * This function sets the time of the specified clock clk_id.
 *
 * @attention
 * <ul>
 * <li>The first argument of value only supports CLOCK_REALTIME.</li>
 * </ul>
 *
 * @retval #0  The function is executed successfully.
 * @retval #-1 The function failed to execute, and corresponding error code is set.
 *
 * @par Errors
 * <ul>
 * <li><b>EINVAL</b>: An invalid input.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>time.h</li></ul>
 *
 * @see  gettimeofday | settimeofday | time | adjtime | ctime
 *
 * @since Huawei LiteOS V100R001C00
 */
int clock_settime (clockid_t, const struct timespec *);
int clock_nanosleep (clockid_t, int, const struct timespec *, struct timespec *);
int clock_getcpuclockid (pid_t, clockid_t *);

struct sigevent;


/**
 * @ingroup  time
 * @par Description:
 * This function creates a per-process timer using the specified clock, as the timing base.
 * The function shall return, in the location referenced by the third argument, a timer ID
 * of type timer_t used to identify the timer in timer requests.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #0  The function is executed successfully.
 * @retval #-1 The function failed to execute, and corresponding error code is set.
 *
 * @par Errors
 * <ul>
 * <li><b>EINVAL</b>: An invalid input.</li>
 * <li><b>EAGAIN</b>: Duplicate creation.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>time.h</li></ul>
 *
 * @see  clock_getres | timer_delete | timer_getoverrun
 *
 * @since Huawei LiteOS V100R001C00
 */
int timer_create (clockid_t, struct sigevent *__restrict, timer_t *__restrict);

/**
 * @ingroup  time
 * @par Description:
 * This function deletes the specified timer,previously created by the timer_create() function.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #0  The function is executed successfully.
 * @retval #-1 The function failed to execute, and corresponding error code is set.
 *
 * @par Errors
 * <ul>
 * <li><b>EINVAL</b>: An invalid input.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>time.h</li></ul>
 *
 * @see  timer_create
 *
 * @since Huawei LiteOS V100R001C00
 */
int timer_delete (timer_t);

/**
 * @ingroup  time
 * @par Description:
 * This function set the time until the next expiration of the timer specified by timerid from
 * the it_value member of the third argument and arm the timer if the it_value member of value is non-zero.
 *
 * @attention
 * <ul>
 * <li>the second parameter is unused</li>
 * </ul>
 *
 * @retval #0  The function is executed successfully.
 * @retval #-1 The function failed to execute, and corresponding error code is set.
 *
 * @par Errors
 * <ul>
 * <li><b>EINVAL</b>: An invalid input.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>time.h</li></ul>
 *
 * @see  clock_getres | timer_create
 *
 * @since Huawei LiteOS V100R001C00
 */
int timer_settime (timer_t, int, const struct itimerspec *__restrict, struct itimerspec *__restrict);

/**
 * @ingroup  time
 * @par Description:
 * This function shall store the amount of time until the specified timer,
 * expires and the reload value of the timer into the space pointed to by the second argument.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #0  The function is executed successfully.
 * @retval #-1 The function failed to execute, and corresponding error code is set.
 *
 * @par Errors
 * <ul>
 * <li><b>EINVAL</b>: An invalid input.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>time.h</li></ul>
 *
 * @see  clock_getres | timer_create
 *
 * @since Huawei LiteOS V100R001C00
 */
int timer_gettime (timer_t, struct itimerspec *);

/**
 * @ingroup  time
 * @par Description:
 * This function shall get the timer expiration overrun count for the specified timer.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval
 * #int On success, this function returns the timer expiration overrun count as explained above.
 * On failure, -1 is returned.
 *
 * @par Errors
 * <ul>
 * <li><b>EINVAL</b>: An invalid input.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>time.h</li></ul>
 *
 * @see  clock_getres | timer_create
 *
 * @since Huawei LiteOS V100R001C00
 */
int timer_getoverrun (timer_t);

extern char *tzname[2];

#endif


#if defined(_XOPEN_SOURCE) || defined(_BSD_SOURCE) || defined(_GNU_SOURCE)

/**
 * @ingroup  time
 * @par Description:
 * This function converts the character string pointed to by the first argument to values which are stored in
 * the tm structure pointed to by the third argument, using the format specified by the second argument.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval
 * #char* On success, this function returns a pointer to the character following the last character parsed.
 * On failure, a null pointer shall be returned.
 *
 * @par Dependency:
 * <ul><li>time.h</li></ul>
 *
 * @see  fprintf | fscanf | strftime | time
 *
 * @since Huawei LiteOS V100R001C00
 */
char *strptime (const char *__restrict, const char *__restrict, struct tm *__restrict);
extern int daylight;
extern long timezone;
extern int getdate_err;
struct tm *getdate (const char *);
#endif


#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)

/**
 * @ingroup  time
 * @par Description:
 * This function  sets  the  system's  idea  of  the  time  and date.  The time, pointed to by t, is measured in seconds since the Epoch.
 *
 * @attention
 * <ul>
 * <li>There are no restrictions on the use of superusers.</li>
 * </ul>
 *
 * @retval #0  The function is executed successfully.
 * @retval #-1 The function failed to execute, and corresponding error code is set.
 *
 * @par Errors
 * <ul>
 * <li><b>EINVAL</b>: An invalid input.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>time.h</li></ul>
 *
 * @see  settimeofday
 *
 * @since Huawei LiteOS V100R001C00
 */
int stime(const time_t *);
time_t timegm(struct tm *);
#endif

#if _REDIR_TIME64
__REDIR(time, __time64);
__REDIR(difftime, __difftime64);
__REDIR(mktime, __mktime64);
__REDIR(gmtime, __gmtime64);
__REDIR(localtime, __localtime64);
__REDIR(ctime, __ctime64);
__REDIR(timespec_get, __timespec_get_time64);
#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) \
 || defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) \
 || defined(_BSD_SOURCE)
__REDIR(gmtime_r, __gmtime64_r);
__REDIR(localtime_r, __localtime64_r);
__REDIR(ctime_r, __ctime64_r);
__REDIR(nanosleep, __nanosleep_time64);
__REDIR(clock_getres, __clock_getres_time64);
__REDIR(clock_gettime, __clock_gettime64);
__REDIR(clock_settime, __clock_settime64);
__REDIR(clock_nanosleep, __clock_nanosleep_time64);
__REDIR(timer_settime, __timer_settime64);
__REDIR(timer_gettime, __timer_gettime64);
#endif
#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
__REDIR(stime, __stime64);
__REDIR(timegm, __timegm_time64);
#endif
#endif

#ifdef __cplusplus
}
#endif


#endif
