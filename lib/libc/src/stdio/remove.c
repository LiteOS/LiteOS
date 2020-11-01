#include "unistd.h"
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include "syscall.h"
#include "sys/stat.h"

int remove(const char *path)
{
	struct stat sb;

	if (lstat(path, &sb) < 0)
		return -1;
	if (S_ISDIR(sb.st_mode))
		return rmdir(path);
	return unlink(path);
}
