#include "stddef.h"
#include "errno.h"
#include "sys/types.h"
#ifdef LOSCFG_FS_VFS
#include "fs/fs.h"
#endif
#if defined(LOSCFG_HW_RANDOM_ENABLE) || defined(LOSCFG_DRIVERS_RANDOM)
#include "randomdev.h"
#endif
#include "sys/random.h"
#include "sys/param.h"

#if defined(LOSCFG_HW_RANDOM_ENABLE) || defined(LOSCFG_DRIVERS_RANDOM)
#define RANDOM_BUF_MAX_LEN 300
#define RANDOM_BLOCKSIZE 16
#endif

static int getentropy_urandom(void *buf, size_t len) {
#if defined(LOSCFG_HW_RANDOM_ENABLE) || defined(LOSCFG_DRIVERS_RANDOM)
	int ret;
	u_int read_len;
	uint8_t local_buf[RANDOM_BUF_MAX_LEN];
	random_alg_context.ra_pre_read();
	read_len = roundup(len, RANDOM_BLOCKSIZE);
	random_alg_context.ra_read(local_buf, read_len);
	ret = memcpy_s(buf, len, local_buf, len);
	if (ret != EOK) {
		errno = EIO;
		return -1;
	}
	(void)memset_s(local_buf, sizeof(local_buf), 0 , sizeof(local_buf));

	return 0;
#else
	(void)buf;
	(void)len;
	errno = EIO;
	return -1;
#endif
}

int getentropy(void *buf, size_t len) {
	int ret;

	if (len > 256) {
		errno = EIO;
		return -1;
	}

	ret = getentropy_urandom(buf, len);
	if (ret == -1) {
		errno = EIO;
	}
	return ret;
}

