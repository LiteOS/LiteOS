#ifndef _LOS_PRINTF_H
#define _LOS_PRINTF_H
//#ifdef LOSCFG_LIB_LIBC
#include "stdarg.h"
#include <stdio.h>
//#endif
#ifdef LOSCFG_LIB_LIBCMINI
#include "libcmini.h"
#endif
#include "los_typedef.h"
#include "los_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define LOS_EMG_LEVEL   0

#define LOS_COMMOM_LEVEL   (LOS_EMG_LEVEL + 1)

#define LOS_ERR_LEVEL   (LOS_COMMOM_LEVEL + 1)

#define LOS_WARN_LEVEL  (LOS_ERR_LEVEL + 1)

#define LOS_INFO_LEVEL  (LOS_WARN_LEVEL + 1)

#define LOS_DEBUG_LEVEL (LOS_INFO_LEVEL + 1)

#define PRINT_LEVEL LOS_ERR_LEVEL

#if PRINT_LEVEL < LOS_DEBUG_LEVEL
#define PRINT_DEBUG(fmt, args...)
#else
#define PRINT_DEBUG(fmt, args...)   do{(printf("[DEBUG] "), printf(fmt, ##args));}while(0)
#endif

#if PRINT_LEVEL < LOS_INFO_LEVEL
#define PRINT_INFO(fmt, args...)
#else
#define PRINT_INFO(fmt, args...)    do{(printf("[INFO] "), printf(fmt, ##args));}while(0)
#endif

#if PRINT_LEVEL < LOS_WARN_LEVEL
#define PRINT_WARN(fmt, args...)
#else
#define PRINT_WARN(fmt, args...)    do{(printf("[WARN] "), printf(fmt, ##args));}while(0)
#endif

#if PRINT_LEVEL < LOS_ERR_LEVEL
#define PRINT_ERR(fmt, args...)
#else
#define PRINT_ERR(fmt, args...)     do{(printf("[ERR] "), printf(fmt, ##args));}while(0)
#endif

#if PRINT_LEVEL < LOS_COMMOM_LEVEL
#define PRINTK(fmt, args...)
#else
#define PRINTK(fmt, args...)     printf(fmt, ##args)
#endif

#if PRINT_LEVEL < LOS_EMG_LEVEL
#define PRINT_EMG(fmt, args...)
#else
#define PRINT_EMG(fmt, args...)     do{(printf("[EMG] "), printf(fmt, ##args));}while(0)
#endif

#define PRINT_RELEASE(fmt, args...)   printf(fmt, ##args)


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_PRINTF_H */

