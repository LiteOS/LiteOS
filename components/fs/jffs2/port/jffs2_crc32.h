#ifndef __JFFS2_CRC32_H__
#define __JFFS2_CRC32_H__

#include <cyg/crc/crc.h>

#define crc32(val, s, len) cyg_crc32_accumulate(val, (unsigned char *)s, len)

#endif /* __JFFS2_CRC32_H__ */