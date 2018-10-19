/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

#ifndef __JFFS2_CONFIG_H__
#define __JFFS2_CONFIG_H__

#include "los_config.h"

#define __LITEOS_JFFS2__ /* must be defined */
#define CONFIG_JFFS2_DIRECT  /* unuse mtd */

#define FILE_PATH_MAX                128  /* the longest file path */
#define CONFIG_JFFS2_ENTRY_NAME_MAX  23
#define	JFFS2_NAME_MAX   CONFIG_JFFS2_ENTRY_NAME_MAX
#define	JFFS2_PATH_MAX   FILE_PATH_MAX

#define DEVICE_PART_MAX   1  /* the max partions on a nand deivce*/

/* memory page size in kernel/asm/page.h, it is correspond with flash read/write
 * option, so this size has a great impact on reading/writing speed */
#define CONFIG_JFFS2_PAGE_SHIFT  12  /* (1<<12) 4096bytes*/

/* jffs2 support relative dir, command "ls" will get
 * +-------------------------------+
 * |   finsh>>ls("/")              |
 * |   Directory /:                |
 * |   .                   <DIR>   |
 * |   ..                  <DIR>   |
 * |   dir1                <DIR>   |
 * +-------------------------------+
 */
#define CONFIG_JFFS2_NO_RELATIVEDIR

//#define CYGPKG_FS_JFFS2_RET_DIRENT_DTYPE
#if defined(CYGPKG_FS_JFFS2_RET_DIRENT_DTYPE)
	#define CYGPKG_FILEIO_DIRENT_DTYPE
#endif

#define CONFIG_JFFS2_WRITABLE   /* if not defined, jffs2 is read only*/

/* jffs2 debug output opion */
#define CONFIG_JFFS2_FS_DEBUG 		0  /* 1 or 2 */

/* jffs2 gc thread section */
#define CONFIG_JFFS2_GC_THREAD
#define CONFIG_JFFS2_GC_THREAD_PRIORITY     (LOS_TASK_PRIORITY_LOWEST-2) /* GC thread's priority */
#define CONFIG_JFFS2_GS_THREAD_TICKS        20  /* event timeout ticks */
#define CONFIG_JFFS2_GC_THREAD_TICKS        20  /* GC thread's running ticks */
#define CONFIG_JFFS2_GC_THREAD_STACK_SIZE   (1024*4)

//#define CONFIG_JFFS2_FS_WRITEBUFFER /* should not be enabled */

/* zlib section*/
//#define CONFIG_JFFS2_PROC
//#define CONFIG_JFFS2_ZLIB
//#define CONFIG_JFFS2_RTIME
//#define CONFIG_JFFS2_RUBIN
//#define CONFIG_JFFS2_CMODE_NONE
//#define CONFIG_JFFS2_CMODE_SIZE


#define GFP_KERNEL              0


#endif /* __JFFS2_CONFIG_H__ */