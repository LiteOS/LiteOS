#include "unistd.h"
#include "stdio_impl.h"
#include <sys/uio.h>

size_t __stdio_read(FILE *f, unsigned char *buf, size_t len)
{
	/*
	 * Musl subtracts iov_len by '!!f->buf_size' to solve the bug in linux readv syscall.
	 * However, LiteOS does not need this operation, which reduces the fread performance.
	*/
	struct iovec iov[2] = {
		{ .iov_base = buf, .iov_len = len },
		{ .iov_base = f->buf, .iov_len = f->buf_size }
	};
	ssize_t cnt;

	cnt = iov[0].iov_len ? readv(f->fd, iov, 2)
		: read(f->fd, iov[1].iov_base, iov[1].iov_len);
	if (cnt <= 0) {
		f->flags |= cnt ? F_ERR : F_EOF;
		return 0;
	}
	if (cnt <= len) return cnt;
	cnt -= len;
	f->rpos = f->buf;
	f->rend = f->buf + cnt;
	return len;
}
