#include <string.h>
#include <locale.h>
#include "locale_impl.h"
#include "assert.h"

/* collate only by code points */
size_t __strxfrm_l(char *restrict dest, const char *restrict src, size_t n, locale_t loc)
{
	assert(src != NULL);

	size_t l = strlen(src);
	if (n != 0) {
		assert(dest != NULL);
		strncpy(dest, src, n);
	}
	return l;
}

size_t strxfrm(char *restrict dest, const char *restrict src, size_t n)
{
	return __strxfrm_l(dest, src, n, CURRENT_LOCALE);
}

weak_alias(__strxfrm_l, strxfrm_l);
