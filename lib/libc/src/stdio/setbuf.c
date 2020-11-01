#include <stdio.h>
#if defined(__LITEOS__) && defined(LOSCFG_LLTSER)
#include "gcov_ser.h"
#endif

void setbuf(FILE *restrict f, char *restrict buf)
{
#if defined(__LITEOS__) && defined(LOSCFG_LLTSER)
	GCOV_SETBUF(f);
#endif
	setvbuf(f, buf, buf ? _IOFBF : _IONBF, BUFSIZ);
}
