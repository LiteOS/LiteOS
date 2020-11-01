#include "stdio_impl.h"
#include <string.h>
#if defined(__LITEOS__) && defined(LOSCFG_LLTSER)
#include "gcov_ser.h"
#endif

size_t __fwritex(const unsigned char *restrict s, size_t l, FILE *restrict f)
{
	size_t i=0;

	if (!f->wend && __towrite(f)) return 0;

	if (l > f->wend - f->wpos) return f->write(f, s, l);

	if (f->lbf >= 0) {
		/* Match /^(.*\n|)/ */
		for (i=l; i && s[i-1] != '\n'; i--);
		if (i) {
			size_t n = f->write(f, s, i);
			if (n < i) return n;
			s += i;
			l -= i;
		}
	}

	memcpy(f->wpos, s, l);
	f->wpos += l;
	return l+i;
}


/* bionic compile use, update to musl compile and delete */
size_t __fwrite_chk(const void* buf, size_t size, size_t count, FILE* stream, size_t buf_size) {
	return 0;
}

/* bionic compile use, update to musl compile and delete */
int __sprintf_chk(char* dst, int flags, size_t dst_len_from_compiler,
                             const char* format, ...) {
	return 0;
}

/* bionic compile use, update to musl compile and delete */
size_t __strlen_chk(const char* s, size_t s_len) {
	return 0;
}
size_t fwrite(const void *restrict src, size_t size, size_t nmemb, FILE *restrict f)
{
	size_t k, l = size*nmemb;
#if defined(__LITEOS__) && defined(LOSCFG_LLTSER)
	GCOV_FWRITE(f, src, size, nmemb);
#endif
#ifdef __LITEOS__
	if (nmemb == 0 || size == 0) {
		errno = EINVAL;
		return 0;
	}

	if (f == NULL) {
		errno = EBADF;
		return 0;
	}
	if (((nmemb | size) > 0xFFFF) &&
		(nmemb >= SIZE_MAX / size)) {
		errno = EINVAL;
		f->flags |= F_ERR;
		return 0;
	}
#endif
	if (!size) nmemb = 0;
	FLOCK(f);
	k = __fwritex(src, l, f);
	FUNLOCK(f);
	return k==l ? nmemb : k/size;
}

weak_alias(fwrite, fwrite_unlocked);
