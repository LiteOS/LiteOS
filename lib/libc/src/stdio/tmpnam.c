#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include "syscall.h"
#include "kstat.h"

#define MAXTRIES 100

char *tmpnam(char *buf)
{
	static char internal[L_tmpnam];
	char s[] = "/tmp/tmpnam_XXXXXX";
	int try;

	for (try=0; try<MAXTRIES; try++) {
		__randname(s+12);
		(void)lstat(s, &(struct stat){0});
		if (errno == ENOENT) return strcpy(buf ? buf : internal, s);
	}
	return 0;
}
