/**
 * @defgroup time Time
 * @ingroup libc
 */

#ifndef _SYS_TIME_H
#define _SYS_TIME_H
#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#ifdef __LITEOS__
#include <sys/types.h>
#endif
#include <sys/select.h>

/**
 * @ingroup  time
 * @par Description:
 * This function  gets the time as well as a timezone.
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
 * <ul><li>time.h</li></ul>
 *
 * @see  clock_gettime | time | ctime
 *
 * @since Huawei LiteOS V100R001C00
 */
int gettimeofday (struct timeval *__restrict, void *__restrict);

#define ITIMER_REAL    0
#define ITIMER_VIRTUAL 1
#define ITIMER_PROF    2

struct itimerval {
	struct timeval it_interval;
	struct timeval it_value;
};

int getitimer (int, struct itimerval *);
int setitimer (int, const struct itimerval *__restrict, struct itimerval *__restrict);
int utimes (const char *, const struct timeval [2]);

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
struct timezone {
	int tz_minuteswest;
	int tz_dsttime;
};
int futimes(int, const struct timeval [2]);
int futimesat(int, const char *, const struct timeval [2]);
int lutimes(const char *, const struct timeval [2]);

/**
 * @ingroup  time
 * @par Description:
 * This function  sets the time as well as a timezone.
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
 * <ul><li>time.h</li></ul>
 *
 * @see  clock_gettime | time | ctime
 *
 * @since Huawei LiteOS V100R001C00
 */
int settimeofday(const struct timeval *, const struct timezone *);

/**
 * @ingroup  time
 * @par Description:
 * This function  gradually adjusts the system clock (as returned by gettimeofday).
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
 * @see  gettimeofday | time
 *
 * @since Huawei LiteOS V100R001C00
 */
int adjtime (const struct timeval *, struct timeval *);
#define timerisset(t) ((t)->tv_sec || (t)->tv_usec)
#define timerclear(t) ((t)->tv_sec = (t)->tv_usec = 0)
#define timercmp(s,t,op) ((s)->tv_sec == (t)->tv_sec ? \
	(s)->tv_usec op (t)->tv_usec : (s)->tv_sec op (t)->tv_sec)
#define timeradd(s,t,a) (void) ( (a)->tv_sec = (s)->tv_sec + (t)->tv_sec, \
	((a)->tv_usec = (s)->tv_usec + (t)->tv_usec) >= 1000000 && \
	((a)->tv_usec -= 1000000, (a)->tv_sec++) )
#define timersub(s,t,a) (void) ( (a)->tv_sec = (s)->tv_sec - (t)->tv_sec, \
	((a)->tv_usec = (s)->tv_usec - (t)->tv_usec) < 0 && \
	((a)->tv_usec += 1000000, (a)->tv_sec--) )
#endif

#if defined(_GNU_SOURCE)
#define TIMEVAL_TO_TIMESPEC(tv, ts) ( \
	(ts)->tv_sec = (tv)->tv_sec, \
	(ts)->tv_nsec = (tv)->tv_usec * 1000, \
	(void)0 )
#define TIMESPEC_TO_TIMEVAL(tv, ts) ( \
	(tv)->tv_sec = (ts)->tv_sec, \
	(tv)->tv_usec = (ts)->tv_nsec / 1000, \
	(void)0 )
#endif

#if _REDIR_TIME64
__REDIR(gettimeofday, __gettimeofday_time64);
__REDIR(getitimer, __getitimer_time64);
__REDIR(setitimer, __setitimer_time64);
__REDIR(utimes, __utimes_time64);
#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
__REDIR(futimes, __futimes_time64);
__REDIR(futimesat, __futimesat_time64);
__REDIR(lutimes, __lutimes_time64);
__REDIR(settimeofday, __settimeofday_time64);
__REDIR(adjtime, __adjtime64);
#endif
#endif

#ifdef __cplusplus
}
#endif
#endif
