#include <fcntl.h>
#include <string.h>
#include <errno.h>

int __fmodeflags(const char *mode)
{
	int flags;
	if (strchr(mode, '+')) flags = O_RDWR;
	else if (*mode == 'r') flags = O_RDONLY;
	else flags = O_WRONLY;
	if (strchr(mode, 'x')) {
/* rx not support */
#ifdef __LITEOS__
		if (strchr(mode, 'r')) {
			errno = EINVAL;
			return -1;
		}
#endif
		flags |= O_EXCL;
	}
	if (strchr(mode, 'e')) flags |= O_CLOEXEC;
	if (*mode != 'r') flags |= O_CREAT;
	if (*mode == 'w') flags |= O_TRUNC;
	if (*mode == 'a') flags |= O_APPEND;
	return flags;
}