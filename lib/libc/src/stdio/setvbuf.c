#include "stdio_impl.h"

/* The behavior of this function is undefined except when it is the first
 * operation on the stream, so the presence or absence of locking is not
 * observable in a program whose behavior is defined. Thus no locking is
 * performed here. No allocation of buffers is performed, but a buffer
 * provided by the caller is used as long as it is suitably sized. */

int setvbuf(FILE *restrict f, char *restrict buf, int type, size_t size)
{
	f->lbf = EOF;

#ifdef __LITEOS__
	/* size cannot be less than 0 */
	if ((int)size < 0) {
		errno = EINVAL;
		return -1;
	}
#endif

	if (type == _IONBF) {
		f->buf_size = 0;
	} else if (type == _IOLBF || type == _IOFBF) {
#ifdef __LITEOS__
		/* when buf is NULL, malloc default size for buf */
		if (buf == NULL) {
			size = BUFSIZ;
			buf = malloc(size);
			f->flags |= F_MBF;
			if (buf == NULL) {
				errno = EINVAL;
			}
		}
#endif
		if (buf && size >= UNGET) {
			f->buf = (void *)(buf + UNGET);
			f->buf_size = size - UNGET;
		}
		if (type == _IOLBF && f->buf_size)
			f->lbf = '\n';
	} else {
		return -1;
	}

	f->flags |= F_SVB;

	return 0;
}
