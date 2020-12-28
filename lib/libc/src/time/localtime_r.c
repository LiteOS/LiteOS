#include "time_impl.h"
#include <errno.h>
#include <limits.h>
#include "los_typedef.h"
#include "tzdst_pri.h"
#include "lock.h"

struct tm *__localtime_r(const time_t *restrict t, struct tm *restrict tm)
{
#ifndef __LITEOS__
	/* Reject time_t values whose year would overflow int because
	 * __secs_to_zone cannot safely handle them. */
	if (*t < INT_MIN * 31622400LL || *t > INT_MAX * 31622400LL) {
		errno = EOVERFLOW;
		return 0;
	}
	__secs_to_zone(*t, 0, &tm->tm_isdst, &tm->__tm_gmtoff, 0, &tm->__tm_zone);
	if (__secs_to_tm((long long)*t + tm->__tm_gmtoff, tm) < 0) {
		errno = EOVERFLOW;
		return 0;
	}
	return tm;
#else
	LOCK(lock);
	time_t timeoff;
	int dstsec = 0;
	if (CheckWithinDstPeriod(NULL, *t) == TRUE)
	 	dstsec = DstForwardSecondGet();
	timeoff = *t + timezone + dstsec;
	UNLOCK(lock);

	/* Reject time_t values whose year would overflow int because
	 * __secs_to_zone cannot safely handle them. */
	if (timeoff < INT_MIN * 31622400LL || timeoff > INT_MAX * 31622400LL) {
		errno = EOVERFLOW;
		return 0;
	}

	if (__secs_to_tm((long long)timeoff, tm) < 0) {
		errno = EOVERFLOW;
		return 0;
	}
	return tm;
#endif
}

weak_alias(__localtime_r, localtime_r);
