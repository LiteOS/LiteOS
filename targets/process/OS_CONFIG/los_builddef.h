#ifndef _LOS_BUILDDEF_H
#define _LOS_BUILDDEF_H
#include "string.h"
#include "los_compiler.h"

#define OS_LITTLE_ENDIAN 0x1234
#define OS_BIG_ENDIAN  0x3412

#ifndef OS_BYTE_ORDER
#define OS_BYTE_ORDER  OS_LITTLE_ENDIAN
#endif

#ifndef LITE_OS_SEC_ALW_INLINE
#define LITE_OS_SEC_ALW_INLINE              __attribute__((always_inline))
#endif

#ifndef LITE_OS_SEC_VEC
#define LITE_OS_SEC_VEC                     __attribute__ ((section(".data.vector")))
#endif

/**
 * @ingroup los_builddef
 * .Text section (Code section)
 */
#ifndef LITE_OS_SEC_TEXT
#define LITE_OS_SEC_TEXT                    //__attribute__((section(".sram.text")))
#endif

/**
 * @ingroup los_builddef
 * .Text.ddr section
 */
#ifndef LITE_OS_SEC_TEXT_MINOR
#define LITE_OS_SEC_TEXT_MINOR              //__attribute__((section(".dyn.text")))
#endif

/**
 * @ingroup los_builddef
 * .Text.init section
 */
#ifndef LITE_OS_SEC_TEXT_INIT
#define LITE_OS_SEC_TEXT_INIT               //__attribute__((section(".dyn.text")))
#endif

/**
 * @ingroup los_builddef
 * Redirect .Text  section (Code section)
 */
#ifndef LITE_OS_SEC_TEXT_REDIRECT
#define LITE_OS_SEC_TEXT_REDIRECT           LITE_OS_SEC_TEXT
#endif

/**
 * @ingroup los_builddef
 * Redirect .Text.ddr section
 */
#ifndef LITE_OS_SEC_TEXT_MINOR_REDIRECT
#define LITE_OS_SEC_TEXT_MINOR_REDIRECT     LITE_OS_SEC_TEXT_MINOR
#endif

/**
 * @ingroup los_builddef
 * Redirect .Text.init section
 */
#ifndef LITE_OS_SEC_TEXT_INIT_REDIRECT
#define LITE_OS_SEC_TEXT_INIT_REDIRECT      LITE_OS_SEC_TEXT_INIT
#endif

/**
 * @ingroup los_builddef
 * .Data section
 */
#ifndef LITE_OS_SEC_DATA
#define LITE_OS_SEC_DATA                    //__attribute__((section(".dyn.data")))
#endif

/**
 * @ingroup los_builddef
 * .Data.init section
 */
#ifndef LITE_OS_SEC_DATA_INIT
#define LITE_OS_SEC_DATA_INIT               //__attribute__((section(".dyn.data")))
#endif

/**
 * @ingroup los_builddef
 * Not initialized variable section
 */
#ifndef LITE_OS_SEC_BSS
#define LITE_OS_SEC_BSS                     //__attribute__((section(".sym.bss")))
#endif

/**
 * @ingroup los_builddef
 * .bss.ddr section
 */
#ifndef LITE_OS_SEC_BSS_MINOR
#define LITE_OS_SEC_BSS_MINOR
#endif

/**
 * @ingroup los_builddef
 * .bss.init sections
 */
#ifndef LITE_OS_SEC_BSS_INIT
#define LITE_OS_SEC_BSS_INIT
#endif

#ifndef LITE_OS_SEC_TEXT_DATA
#define LITE_OS_SEC_TEXT_DATA               //__attribute__((section(".dyn.data")))
#define LITE_OS_SEC_TEXT_BSS                //__attribute__((section(".dyn.bss")))
#define LITE_OS_SEC_TEXT_RODATA             //__attribute__((section(".dyn.rodata")))
#endif

#ifndef LITE_OS_SEC_SYMDATA
#define LITE_OS_SEC_SYMDATA                 //__attribute__((section(".sym.data")))
#endif

#ifndef LITE_OS_SEC_SYMBSS
#define LITE_OS_SEC_SYMBSS                  //__attribute__((section(".sym.bss")))
#endif


#ifndef LITE_OS_SEC_KEEP_DATA_DDR
#define LITE_OS_SEC_KEEP_DATA_DDR           //__attribute__((section(".keep.data.ddr")))
#endif

#ifndef LITE_OS_SEC_KEEP_TEXT_DDR
#define LITE_OS_SEC_KEEP_TEXT_DDR           //__attribute__((section(".keep.text.ddr")))
#endif

#ifndef LITE_OS_SEC_KEEP_DATA_SRAM
#define LITE_OS_SEC_KEEP_DATA_SRAM          //__attribute__((section(".keep.data.sram")))
#endif

#ifndef LITE_OS_SEC_KEEP_TEXT_SRAM
#define LITE_OS_SEC_KEEP_TEXT_SRAM          //__attribute__((section(".keep.text.sram")))
#endif

#ifndef LITE_OS_SEC_BSS_MINOR
#define LITE_OS_SEC_BSS_MINOR
#endif

/**
 * @ingroup los_builddef
 * .text.libsec section
 */
#ifndef LIB_SECURE_SEC_TEXT
#define LIB_SECURE_SEC_TEXT
#endif

/**
 * @ingroup los_builddef
 * .text.libc section
 */
#ifndef LIBC_SEC_TEXT
#define LIBC_SEC_TEXT
#endif

#endif /* _LOS_BUILDEF_H */  
