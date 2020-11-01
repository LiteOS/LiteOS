#include "stdio_impl.h"
#include "pthread_impl.h"

#ifdef __LITEOS__
void __initlockattr(pthread_mutex_t *lock)
{
	lock->stAttr.protocol = PTHREAD_PRIO_INHERIT;
	lock->stAttr.prioceiling = OS_TASK_PRIORITY_LOWEST;
	lock->stAttr.type = PTHREAD_MUTEX_RECURSIVE_NP;
	lock->stAttr.reserved = 0;
	lock->stLock.muxList.pstPrev = (struct LOS_DL_LIST *)NULL;
	lock->stLock.muxList.pstNext = (struct LOS_DL_LIST *)NULL;
	lock->stLock.owner = (LosTaskCB *)NULL;
	lock->stLock.muxCount = 0;
}
#endif

int __lockfile(FILE *f)
{
#ifndef __LITEOS__
	int owner = f->lock, tid = __pthread_self()->tid;
	if ((owner & ~MAYBE_WAITERS) == tid)
		return 0;
	owner = a_cas(&f->lock, 0, tid);
	if (!owner) return 1;
	while ((owner = a_cas(&f->lock, 0, tid|MAYBE_WAITERS))) {
		if ((owner & MAYBE_WAITERS) ||
		    a_cas(&f->lock, owner, owner|MAYBE_WAITERS)==owner)
			__futexwait(&f->lock, owner|MAYBE_WAITERS, 1);
	}
	return 1;
#else
	if (f != NULL) {
		pthread_mutex_lock(&(f->_lock));
	}
	return 1;
#endif
}

void __unlockfile(FILE *f)
{
#ifndef __LITEOS__
	if (a_swap(&f->lock, 0) & MAYBE_WAITERS)
		__wake(&f->lock, 1, 1);
#else
	if (f != NULL) {
		pthread_mutex_unlock(&(f->_lock));
	}
#endif
}
