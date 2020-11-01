#include "unistd.h"
#include "stdio_impl.h"
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#if defined(__LITEOS__) && defined(LOSCFG_LLTSER)
#include "gcov_ser.h"
#endif

FILE *fopen(const char *restrict filename, const char *restrict mode)
{
	FILE *f;
	int fd;
	int flags;
#if defined(__LITEOS__) && defined(LOSCFG_LLTSER)
	GCOV_FOPEN(filename);
#endif

	/* Check for valid initial mode character */
	if (!strchr("rwa", *mode)) {
		errno = EINVAL;
		return 0;
	}

	/* Compute the flags to pass to open() */
	flags = __fmodeflags(mode);
#ifdef __LITEOS__
	if (flags == -1) return (FILE *)NULL;
#endif

	fd = open(filename, flags, 0666);
	if (fd < 0) return 0;
	if (flags & O_CLOEXEC)
		fcntl(fd, F_SETFD, FD_CLOEXEC);

	f = __fdopen(fd, mode);
	if (f) {
#ifdef __LITEOS__
		f->flags &= ~F_CLS;
#endif
		return f;
	}

	close(fd);
	return 0;
}

weak_alias(fopen, fopen64);
