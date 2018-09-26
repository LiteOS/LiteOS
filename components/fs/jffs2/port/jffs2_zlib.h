#ifndef __JFFS2_ZLIB_H__
#define __JFFS2_ZLIB_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include <cyg/compress/zlib.h>

#define zlib_deflateInit(x,y) deflateInit(x,y)
#define zlib_deflate(x,y) deflate(x,y)
#define zlib_deflateEnd(x) deflateEnd(x)
#define zlib_inflateInit(x) inflateInit(x)
#define zlib_inflateInit2(x,y) inflateInit2(x,y)
#define zlib_inflate(x,y) inflate(x,y)
#define zlib_inflateEnd(x) inflateEnd(x)

#define PRESET_DICT 0x20 /* preset dictionary flag in zlib header */


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __JFFS2_ZLIB_H__ */

