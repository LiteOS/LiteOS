#include "stdio_impl.h"
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "libc.h"
#include "lock.h"

FILE __sF[3];
struct glue __sglue = { NULL, 3, __sF };
static struct glue* lastglue = &__sglue;
static pthread_mutex_t lock_glue = PTHREAD_MUTEX_INITIALIZER;
static bool init_first = false;

FILE *__fdopen(int fd, const char *mode)
{
	FILE *f, *tf;
	char *data;
	struct glue *g;
	int n;

	if (init_first == false) {
		__sF[0] = __stdin_FILE;
		__sF[1] = __stdout_FILE;
		__sF[2] = __stderr_FILE;
		init_first = true;
	}

#ifndef __LITEOS__
	struct winsize wsz;
#endif
	/* Check for valid initial mode character */
	if (!strchr("rwa", *mode)) {
		errno = EINVAL;
		return 0;
	}
	LOCK(lock_glue);
	for (g = &__sglue; g != NULL; g = g->next) {
		for (f = g->iobs, n = g->niobs; --n >= 0;) {
			if ((f->flags & F_CLS)) {
				goto found;
			}
			if (g == &__sglue) {
				f++;
			} else {
				f = (FILE *)((char *)f + sizeof *f + UNGET + BUFSIZ);
			}
		}
	}
	UNLOCK(lock_glue);

	/* Allocate FILE+buffer or fail */
	if (!(data=(char *)malloc(sizeof(struct glue) + NDYNAMIC *(sizeof *f + UNGET + BUFSIZ)))) return 0;
	g = (struct glue *)(data);
	f = (FILE *)(data + sizeof(*g));
	tf = f;
	for (int i = 0; i < NDYNAMIC; i++) {
		tf->flags |= F_CLS;
		tf = (FILE *)((char *)tf + sizeof *f + UNGET + BUFSIZ);
	}
	g->next = (struct glue *)NULL;
	g->niobs = NDYNAMIC;
	g->iobs = f;
	LOCK(lock_glue);
	lastglue->next = g;
	lastglue = g;
	f = g->iobs;
found:
	/* Zero-fill only the struct, not the buffer */
	memset(f, 0, sizeof *f);
	UNLOCK(lock_glue);

	/* Impose mode restrictions */
	if (!strchr(mode, '+')) f->flags = (*mode == 'r') ? F_NOWR : F_NORD;

	/* Apply close-on-exec flag */
	if (strchr(mode, 'e')) fcntl(fd, F_SETFD, FD_CLOEXEC);

#ifdef __LITEOS__
	int fd_flags = fcntl(fd, F_GETFL, 0);
	if (fd_flags == -1) return (FILE *)NULL;
#endif
	/* Set append mode on fd if opened for append */
	if (*mode == 'a') {
		int flags = fcntl(fd, F_GETFL);
		if (!(flags & O_APPEND))
			fcntl(fd, F_SETFL, flags | O_APPEND);
		f->flags |= F_APP;
	}

	f->fd = fd;
	f->buf = (unsigned char *)f + sizeof *f + UNGET;
	f->buf_size = BUFSIZ;

	/* Activate line buffered mode for terminals */
	f->lbf = EOF;
#ifndef __LITEOS__
	if (!(f->flags & F_NOWR) && !ioctl(fd, TIOCGWINSZ, &wsz))
		f->lbf = '\n';
#endif
	/* Initialize op ptrs. No problem if some are unneeded. */
	f->read = __stdio_read;
	f->write = __stdio_write;
	f->seek = __stdio_seek;
	f->close = __stdio_close;
#ifdef __LITEOS__
	f->seek64 = __stdio_seek64;
#endif

#ifndef __LITEOS__
	if (!libc.threaded) f->lock = -1;
#else
	/* f->lock is 0, FLOCK() always check */
	__INIT_LOCK_ATTR(f->_lock);
#endif
	/* Add new FILE to open file list */
	return __ofl_add(f);
}

weak_alias(__fdopen, fdopen);
