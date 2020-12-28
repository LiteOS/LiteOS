/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Errno
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
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
 * --------------------------------------------------------------------------- */

/**
 * @defgroup los_errno Error code
 * @ingroup kernel
 */

#ifndef _LOS_ERRNO_H
#define _LOS_ERRNO_H

#include "los_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup los_errno
 * OS error code flag. It is a 24-bit unsigned integer. Its value is 0x000000U.
 */
#define LOS_ERRNO_OS_ID (0x00U << 16)

/**
 * @ingroup los_errno
 * Define the error level as informative. It is a 32-bit unsigned integer. Its value is 0x00000000U.
 */
#define LOS_ERRTYPE_NORMAL (0x00U << 24)

/**
 * @ingroup los_errno
 * Define the error level as warning. It is a 32-bit unsigned integer. Its value is 0x01000000U.
 */
#define LOS_ERRTYPE_WARN (0x01U << 24)

/**
 * @ingroup los_errno
 * Define the error level as critical. It is a 32-bit unsigned integer. Its value is 0x02000000U.
 */
#define LOS_ERRTYPE_ERROR (0x02U << 24)

/**
 * @ingroup los_errno
 * Define the error level as fatal. It is a 32-bit unsigned integer. Its value is 0x03000000U.
 */
#define LOS_ERRTYPE_FATAL (0x03U << 24)

/**
 * @ingroup los_errno
 * Define fatal OS errors. It is a 32-bit unsigned integer error code.
 * <ul>
 * <li>24-31 bits indicate the error level, here is #LOS_ERRTYPE_FATAL.</li>
 * <li>16-23 bits indicate the os error code flag, here is #LOS_ERRNO_OS_ID.</li>
 * <li>8-15 bits indicate the module which the error code belongs to. It is specified by MID.</li>
 * <li>0-7 bits indicate the error code number. It is specified by ERRNO.</li>
 * </ul>
 */
#define LOS_ERRNO_OS_FATAL(MID, ERRNO) \
    (LOS_ERRTYPE_FATAL | LOS_ERRNO_OS_ID | ((UINT32)(MID) << 8) | ((UINT32)(ERRNO)))

/**
 * @ingroup los_errno
 * Define critical OS errors. It is a 32-bit unsigned integer error code.
 * <ul>
 * <li>24-31 bits indicate the error level, here is #LOS_ERRTYPE_ERROR.</li>
 * <li>16-23 bits indicate the os error code flag, here is #LOS_ERRNO_OS_ID.</li>
 * <li>8-15 bits indicate the module which the error code belongs to. It is specified by MID.</li>
 * <li>0-7 bits indicate the error code number. It is specified by ERRNO.</li>
 * </ul>
 */
#define LOS_ERRNO_OS_ERROR(MID, ERRNO) \
    (LOS_ERRTYPE_ERROR | LOS_ERRNO_OS_ID | ((UINT32)(MID) << 8) | ((UINT32)(ERRNO)))

/**
 * @ingroup los_errno
 * Define warning OS errors. It is a 32-bit unsigned integer error code.
 * <ul>
 * <li>24-31 bits indicate the error level, here is #LOS_ERRTYPE_WARN.</li>
 * <li>16-23 bits indicate the os error code flag, here is #LOS_ERRNO_OS_ID.</li>
 * <li>8-15 bits indicate the module which the error code belongs to. It is specified by MID.</li>
 * <li>0-7 bits indicate the error code number. It is specified by ERRNO.</li>
 * </ul>
 */
#define LOS_ERRNO_OS_WARN(MID, ERRNO) \
    (LOS_ERRTYPE_WARN | LOS_ERRNO_OS_ID | ((UINT32)(MID) << 8) | ((UINT32)(ERRNO)))

/**
 * @ingroup los_errno
 * Define informative OS errors. It is a 32-bit unsigned integer error code.
 * <ul>
 * <li>24-31 bits indicate the error level, here is #LOS_ERRTYPE_NORMAL.</li>
 * <li>16-23 bits indicate the os error code flag, here is #LOS_ERRNO_OS_ID.</li>
 * <li>8-15 bits indicate the module which the error code belongs to. It is specified by MID.</li>
 * <li>0-7 bits indicate the error code number. It is specified by ERRNO.</li>
 * </ul>
 */
#define LOS_ERRNO_OS_NORMAL(MID, ERRNO) \
    (LOS_ERRTYPE_NORMAL | LOS_ERRNO_OS_ID | ((UINT32)(MID) << 8) | ((UINT32)(ERRNO)))

/**
 * @ingroup los_errno
 * Define the ID of each module in kernel. The ID is used in error code.
 */
enum LOS_MOUDLE_ID {
    LOS_MOD_SYS = 0x0,          /**< System ID. Its value is 0x0. */
    LOS_MOD_MEM = 0x1,          /**< Dynamic memory module ID. Its value is 0x1. */
    LOS_MOD_TSK = 0x2,          /**< Task module ID. Its value is 0x2. */
    LOS_MOD_SWTMR = 0x3,        /**< Software timer module ID. Its value is 0x3. */
    LOS_MOD_TICK = 0x4,         /**< Tick module ID. Its value is 0x4. */
    LOS_MOD_MSG = 0x5,          /**< Message module ID. Its value is 0x5. */
    LOS_MOD_QUE = 0x6,          /**< Queue module ID. Its value is 0x6. */
    LOS_MOD_SEM = 0x7,          /**< Semaphore module ID. Its value is 0x7. */
    LOS_MOD_MBOX = 0x8,         /**< Static memory module ID. Its value is 0x8. */
    LOS_MOD_HWI = 0x9,          /**< Hardware interrupt module ID. Its value is 0x9. */
    LOS_MOD_HWWDG = 0xa,        /**< Hardware watchdog module ID. Its value is 0xa. */
    LOS_MOD_CACHE = 0xb,        /**< Cache module ID. Its value is 0xb. */
    LOS_MOD_HWTMR = 0xc,        /**< Hardware timer module ID. Its value is 0xc. */
    LOS_MOD_MMU = 0xd,          /**< MMU module ID. Its value is 0xd. */
    LOS_MOD_LOG = 0xe,          /**< Log module ID. Its value is 0xe. */
    LOS_MOD_ERR = 0xf,          /**< Error handling module ID. Its value is 0xf. */
    LOS_MOD_EXC = 0x10,         /**< Exception handling module ID. Its value is 0x10. */
    LOS_MOD_CSTK = 0x11,        /**< This module ID is reserved. Its value is 0x11. */
    LOS_MOD_MPU = 0x12,         /**< MPU module ID. Its value is 0x12. */
    LOS_MOD_NMHWI = 0x13,       /**< NMI module ID. It is reserved. Its value is 0x13. */
    LOS_MOD_TRACE = 0x14,       /**< Trace module ID. Its value is 0x14. */
    LOS_MOD_KNLSTAT = 0x15,     /**< This module ID is reserved. Its value is 0x15. */
    LOS_MOD_EVTTIME = 0x16,     /**< This module ID is reserved. Its value is 0x16. */
    LOS_MOD_THRDCPUP = 0x17,    /**< This module ID is reserved. Its value is 0x17. */
    LOS_MOD_IPC = 0x18,         /**< This module ID is reserved. Its value is 0x18. */
    LOS_MOD_STKMON = 0x19,      /**< This module ID is reserved. Its value is 0x19. */
    LOS_MOD_TIMER = 0x1a,       /**< This module ID is reserved. Its value is 0x1a. */
    LOS_MOD_RESLEAKMON = 0x1b,  /**< This module ID is reserved. Its value is 0x1b. */
    LOS_MOD_EVENT = 0x1c,       /**< event module ID. Its value is 0x1c. */
    LOS_MOD_MUX = 0x1d,         /**< mutex module ID. Its value is 0x1d. */
    LOS_MOD_CPUP = 0x1e,        /**< CPU usage module ID. Its value is 0x1e. */
    LOS_MOD_FPB = 0x1f,         /**< FPB module ID. Its value is 0x1f. */
    LOS_MOD_PERF = 0x20,        /**< Perf module ID. Its value is 0x20. */
    LOS_MOD_SHELL = 0x31,       /**< shell module ID. Its value is 0x31. */
    LOS_MOD_DRIVER = 0x41,      /**< driver module ID. Its value is 0x41. */
    LOS_MOD_BUTT                /**< It is end flag of this enumeration. */
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_ERRNO_H */
