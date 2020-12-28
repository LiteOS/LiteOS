#include "stdio_impl.h"
#include <limits.h>
#include <errno.h>
#if defined(__LITEOS__) && defined(LOSCFG_LLTSER)
#include "gcov_ser.h"
#endif

off_t __ftello_unlocked(FILE *f)
{
	off_t pos = f->seek(f, 0,
		(f->flags & F_APP) && f->wpos != f->wbase
		? SEEK_END : SEEK_CUR);
	if (pos < 0) return pos;

	/* Adjust for data in buffer. */
	if (f->rend)
		pos += f->rpos - f->rend;
	else if (f->wbase)
		pos += f->wpos - f->wbase;
	return pos;
}

off_t __ftello(FILE *f)
{
	off_t pos;
	FLOCK(f);
	pos = __ftello_unlocked(f);
	FUNLOCK(f);
	return pos;
}

long ftell(FILE *f)
{
#if defined(__LITEOS__) && defined(LOSCFG_LLTSER)
	GCOV_FTELL(f);
#endif
	off_t pos = __ftello(f);
	if (pos > LONG_MAX) {
		errno = EOVERFLOW;
		return -1;
	}
	return pos;
}

#ifdef __LITEOS__
off64_t __ftello64_unlocked(FILE *f)
{
	off64_t pos = f->seek64(f, 0,
		(f->flags & F_APP) && f->wpos != f->wbase
		? SEEK_END : SEEK_CUR);
	if (pos < 0) return pos;

	/* Adjust for data in buffer. */
	if (f->rend)
		pos += f->rpos - f->rend;
	else if (f->wbase)
		pos += f->wpos - f->wbase;
	return pos;
}

off64_t __ftello64(FILE *f)
{
	off64_t pos;
	FLOCK(f);
	pos = __ftello64_unlocked(f);
	FUNLOCK(f);
	return pos;
}

off64_t ftello64(FILE *f)
{
	off64_t pos = __ftello64(f);
	return pos;
}
#endif

weak_alias(__ftello, ftello);

#ifndef __LITEOS__
weak_alias(ftello, ftello64);
#endif
