#include "stdio_impl.h"
#if defined(__LITEOS__) && defined(LOSCFG_LLTSER)
#include "gcov_ser.h"
#endif

int __fseeko_unlocked(FILE *f, off_t off, int whence)
{
	/* Adjust relative offset for unread data in buffer, if any. */
	if (whence == SEEK_CUR && f->rend) off -= f->rend - f->rpos;

	/* Flush write buffer, and report error on failure. */
	if (f->wpos != f->wbase) {
		f->write(f, 0, 0);
		if (!f->wpos) return -1;
	}

	/* Leave writing mode */
	f->wpos = f->wbase = f->wend = 0;

	/* Perform the underlying seek. */
	if (f->seek(f, off, whence) < 0) return -1;

	/* If seek succeeded, file is seekable and we discard read buffer. */
	f->rpos = f->rend = 0;
	f->flags &= ~F_EOF;

	return 0;
}

int __fseeko(FILE *f, off_t off, int whence)
{
	int result;
	FLOCK(f);
	result = __fseeko_unlocked(f, off, whence);
	FUNLOCK(f);
	return result;
}

int fseek(FILE *f, long off, int whence)
{
#if defined(__LITEOS__) && defined(LOSCFG_LLTSER)
	GCOV_FSEEK(f, off, whence);
#endif
	return __fseeko(f, off, whence);
}

#ifdef __LITEOS__
int __fseeko64_unlocked(FILE *f, off64_t off, int whence)
{
	/* Adjust relative offset for unread data in buffer, if any. */
	if (whence == SEEK_CUR && f->rend) off -= f->rend - f->rpos;

	/* Flush write buffer, and report error on failure. */
	if (f->wpos != f->wbase) {
		f->write(f, 0, 0);
		if (!f->wpos) return -1;
	}

	/* Leave writing mode */
	f->wpos = f->wbase = f->wend = 0;

	/* Perform the underlying seek. */
	if (f->seek64(f, off, whence) < 0) return -1;

	/* If seek succeeded, file is seekable and we discard read buffer. */
	f->rpos = f->rend = 0;
	f->flags &= ~F_EOF;

	return 0;
}

int __fseeko64(FILE *f, off64_t off, int whence)
{
	int result;
	FLOCK(f);
	result = __fseeko64_unlocked(f, off, whence);
	FUNLOCK(f);
	return result;
}

int fseeko64(FILE *f, off64_t off , int whence)
{
	return __fseeko64(f, off, whence);
}
#endif

weak_alias(__fseeko, fseeko);

#ifndef __LITEOS__
weak_alias(fseeko, fseeko64);
#endif
