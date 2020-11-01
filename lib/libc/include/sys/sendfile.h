#ifndef _SYS_SENDFILE_H
#define _SYS_SENDFILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>
#include <unistd.h>

#ifdef __LITEOS__
#define __NEED_off_t
#define __NEED_int64_t
#include <bits/alltypes.h>
#endif

ssize_t sendfile(int, int, off_t *, size_t);

#if defined(_LARGEFILE64_SOURCE) || defined(_GNU_SOURCE)
#define sendfile64 sendfile
#ifdef __LITEOS__
#define off64_t int64_t
#else
#define off64_t off_t
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
