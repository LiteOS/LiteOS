#include "stdio_impl.h"
#include <stdlib.h>
#if defined(__LITEOS__) && defined(LOSCFG_LLTSER)
#include "gcov_ser.h"
#endif

static void dummy(FILE *f) { }
weak_alias(dummy, __unlist_locked_file);

int fclose(FILE *f)
{
	int r;
#if defined(__LITEOS__) && defined(LOSCFG_LLTSER)
	GCOV_FCLOSE(f);
#endif
	if (f->flags & F_CLS) {
		errno = EBADF;
		return EOF;
	}
	FLOCK(f);
	r = fflush(f);
	r |= f->close(f);
	FUNLOCK(f);

	/* Past this point, f is closed and any further explicit access
	 * to it is undefined. However, it still exists as an entry in
	 * the open file list and possibly in the thread's locked files
	 * list, if it was closed while explicitly locked. Functions
	 * which process these lists must tolerate dead FILE objects
	 * (which necessarily have inactive buffer pointers) without
	 * producing any side effects. */

	if (f->flags & F_PERM) return r;

	__unlist_locked_file(f);

	FILE **head = __ofl_lock();
	if (f->prev) f->prev->next = f->next;
	if (f->next) f->next->prev = f->prev;
	if (*head == f) *head = f->next;
	__ofl_unlock();

	free(f->getln_buf);
	f->flags |= F_CLS;
	return r;
}
