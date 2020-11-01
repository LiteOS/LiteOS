#include <stdlib.h>

int mkstemp(char *template)
{
	return __mkostemps(template, 0, 0);
}

#ifndef __LITEOS__
weak_alias(mkstemp, mkstemp64);
#endif
