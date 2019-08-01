/*----------------------------------------------------------------------------
 * Copyright (c) <2013-2018>, <Huawei Technologies Co., Ltd>
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

#ifndef _LOS_RWLOCK_H
#define _LOS_RWLOCK_H

#include "los_base.h"
#include "los_sys.h"
#include "los_list.h"
#include "los_task.ph"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup los_rwlock
 * RWLock error code: The memory request fails.
 *
 * Value: 0x02001f00
 *
 * Solution: Decrease the number of rwlockes defined by LOSCFG_BASE_IPC_RWLOCK_LIMIT.
 */
#define LOS_ERRNO_RWLOCK_NO_MEMORY         LOS_ERRNO_OS_ERROR(LOS_MOD_RWLOCK, 0x00)

/**
 * @ingroup los_rwlock
 * RWLock error code: The rwlock is not usable.
 *
 * Value: 0x02001f01
 *
 * Solution: Check whether the rwlock ID and the rwlock state are applicable for the current operation.
 */
#define LOS_ERRNO_RWLOCK_INVALID            LOS_ERRNO_OS_ERROR(LOS_MOD_RWLOCK, 0x01)

/**
* @ingroup los_rwlock
* RWLock error code: Null pointer.
*
* Value: 0x02001f02
*
* Solution: Check whether the input parameter is usable.
*/
#define LOS_ERRNO_RWLOCK_PTR_NULL           LOS_ERRNO_OS_ERROR(LOS_MOD_RWLOCK, 0x02)

/**
* @ingroup los_rwlock
* RWLock error code: No rwlock is available and the rwlock request fails.
*
* Value: 0x02001f03
*
* Solution: Increase the number of rwlockes defined by LOSCFG_BASE_IPC_RWLOCK_LIMIT.
*/
#define LOS_ERRNO_RWLOCK_ALL_BUSY           LOS_ERRNO_OS_ERROR(LOS_MOD_RWLOCK, 0x03)

/**
* @ingroup los_rwlock
* RWLock error code: The rwlock fails to be locked in non-blocking mode because it is locked by another thread.
*
* Value: 0x02001f04
*
* Solution: Lock the rwlock after it is unlocked by the thread that owns it, or set a waiting time.
*/
#define LOS_ERRNO_RWLOCK_UNAVAILABLE       LOS_ERRNO_OS_ERROR(LOS_MOD_RWLOCK, 0x04)

/**
* @ingroup los_rwlock
* RWLock error code: The rwlock is being locked during an interrupt.
*
* Value: 0x02001f05
*
* Solution: Check whether the rwlock is being locked during an interrupt.
*/
#define LOS_ERRNO_RWLOCK_PEND_INTERR       LOS_ERRNO_OS_ERROR(LOS_MOD_RWLOCK, 0x05)

/**
* @ingroup los_rwlock
* RWLock error code: A thread locks a rwlock after waiting for the rwlock to be unlocked by another thread when the task scheduling is disabled.
*
* Value: 0x02001f06
*
* Solution: Check whether the task scheduling is disabled, or set uwtimeout to 0, which means that the thread will not wait for the rwlock to become available.
*/
#define LOS_ERRNO_RWLOCK_PEND_IN_LOCK      LOS_ERRNO_OS_ERROR(LOS_MOD_RWLOCK, 0x06)

/**
* @ingroup los_rwlock
* RWLock error code: The rwlock locking times out.
*
* Value: 0x02001f07
*
* Solution: Increase the waiting time or set the waiting time to LOS_WAIT_FOREVER (forever-blocking mode).
*/
#define LOS_ERRNO_RWLOCK_TIMEOUT           LOS_ERRNO_OS_ERROR(LOS_MOD_RWLOCK, 0x07)

/**
 * @ingroup los_rwlock
 *
 * Value: 0x02001f08
 * Not in use temporarily.
 */
#define LOS_ERRNO_RWLOCK_OVERFLOW          LOS_ERRNO_OS_ERROR(LOS_MOD_RWLOCK, 0x08)

/**
* @ingroup los_rwlock
* RWLock error code: The rwlock to be deleted is being locked.
*
* Value: 0x02001f09
*
* Solution: Delete the rwlock after it is unlocked.
*/
#define LOS_ERRNO_RWLOCK_PENDED            LOS_ERRNO_OS_ERROR(LOS_MOD_RWLOCK, 0x09)

/**
 * @ingroup los_rwlock
 *
 * RWLock error code: LOSCFG_BASE_IPC_RWLOCK_LIMIT is zero.
 * Value: 0x02001f0A
 *
 * Solution: LOSCFG_BASE_IPC_RWLOCK_LIMIT should not be zero.
 */
#define LOS_ERRNO_RWLOCK_MAXNUM_ZERO       LOS_ERRNO_OS_ERROR(LOS_MOD_RWLOCK, 0x0A)

/**
 * @ingroup los_rwlock
 * RWLock object.
 */
typedef struct
{
    UINT8           ucRWLockStat;       /**< State OS_RWLOCK_UNUSED,OS_RWLOCK_USED  */
    UINT8           ucRCount;           /**< Times of locking a RWLock by readers*/
    UINT8           ucWCount;           /**< Times of locking a RWLock by writers*/
    UINT32          ucRWLockID;         /**< Handle ID*/
    LOS_DL_LIST     stRPendList;        /**< RLock pended linked list*/
    LOS_DL_LIST     stWPendList;        /**< WLock pended linked list*/
    LOS_TASK_CB     *pstWOwner;         /**< The current write thread that is locking a RWLock*/
} RWLOCK_CB_S;

/**
 * @ingroup los_rwlock
 * RWLock state: not in use.
 */
#define OS_RWLOCK_UNUSED                   0

/**
 * @ingroup los_rwlock
 * RWLock state: in use.
 */
#define OS_RWLOCK_USED                     1

extern RWLOCK_CB_S                         *g_pstAllRWLock;

/**
 * @ingroup los_rwlock
 * Obtain the pointer to a rwlock object of the rwlock that has a specified handle.
 */
#define GET_RWLOCK(rwlockid)                  &g_pstAllRWLock[rwlockid]

#define GET_RWLOCK_LIST(ptr)                  LOS_DL_LIST_ENTRY(ptr, RWLOCK_CB_S, stRPendList)
/**
 *@ingroup los_rwlock
 *@brief Initializes the rwlock.
 *
 *@par Description:
 *This API is used to initializes the rwlock.
 *@attention
 *<ul>
 *<li>None.</li>
 *</ul>
 *
 *@param None.
 *
 *@retval UINT32     Initialization result.
 *@par Dependency:
 *<ul><li>los_rwlock.h: the header file that contains the API declaration.</li></ul>
 *@since Huawei LiteOS V100R001C00
 */
UINT32 osRWLockInit(VOID);

/**
 *@ingroup los_rwlock
 *@brief Create a rwlock.
 *
 *@par Description:
 *This API is used to create a rwlock. A rwlock handle is assigned to puwRWLockHandle when the rwlock is created successfully. Return LOS_OK on creating successful, return specific error code otherwise.
 *@attention
 *<ul>
 *<li>The total number of rwlock is pre-configured. If there are no available rwlock, the rwlock creation fails.</li>
 *</ul>
 *
 *@param puwRWLockHandle   [OUT] Handle pointer of the successfully created rwlock. The value of handle should be in [0, LOSCFG_BASE_IPC_RWLOCK_LIMIT - 1].
 *
 *@retval #LOS_ERRNO_RWLOCK_PTR_NULL           The puwRWLockHandle pointer is NULL.
 *@retval #LOS_ERRNO_RWLOCK_ALL_BUSY           No available rwlock.
 *@retval #LOS_OK                              The rwlock is successfully created.
 *@par Dependency:
 *<ul><li>los_rwlock.h: the header file that contains the API declaration.</li></ul>
 *@see LOS_RWLockDelete
 *@since Huawei LiteOS
 */
UINT32  LOS_RWLockCreate (UINT32 *puwRWLockHandle);

/**
 *@ingroup los_rwlock
 *@brief Delete a rwlock.
 *
 *@par Description:
 *This API is used to delete a specified rwlock. Return LOS_OK on deleting successfully, return specific error code otherwise.
 *@attention
 *<ul>
 *<li>The specific rwlock should be created firstly.</li>
 *<li>The rwlock can be deleted successfully only if no other tasks pend on it.</li>
 *</ul>
 *
 *@param uwRWLockHandle   [IN] Handle of the rwlock to be deleted. The value of handle should be in [0, LOSCFG_BASE_IPC_RWLOCK_LIMIT - 1].
 *
 *@retval #LOS_ERRNO_RWLOCK_INVALID            Invalid handle or rwlock in use.
 *@retval #LOS_ERRNO_RWLOCK_PENDED             Tasks pended on this rwlock.
 *@retval #LOS_OK                              The rwlock is successfully deleted.
 *@par Dependency:
 *<ul><li>los_rwlock.h: the header file that contains the API declaration.</li></ul>
 *@see LOS_RWLockCreate
 *@since Huawei LiteOS
 */
UINT32 LOS_RWLockDelete(UINT32 uwRWLockHandle);

/**
 *@ingroup los_rwlock
 *@brief Wait reader to lock a rwlock.
 *
 *@par Description:
 *This API is used to wait for a specified period of time to lock a rwlock.
 *@attention
 *<ul>
 *<li>The specific rwlock should be created firstly.</li>
 *<li>The function fails if the rwlock that is waited on is already locked by another thread when the task scheduling is disabled.</li>
 *<li>Do not wait on a rwlock during an interrupt.</li>
 *<li>The priority inheritance protocol is not supported.</li>
 *<li>A recursive rwlock can be locked more than once by the same thread.</li>
 *</ul>
 *
 *@param uwRWLockHandle    [IN] Handle of the rwlock to be waited on.  The value of handle should be in [0, LOSCFG_BASE_IPC_RWLOCK_LIMIT - 1].
 *@param uwTimeout         [IN] Waiting time. The value range is [0, LOS_WAIT_FOREVER](unit: Tick).
 *
 *@retval #LOS_ERRNO_RWLOCK_INVALID            The rwlock state (for example, the rwlock does not exist or is not in use) is not applicable for the current operation.
 *@retval #LOS_ERRNO_RWLOCK_UNAVAILABLE        The rwlock fails to be locked because it is locked by another thread and a period of time is not set for waiting for the rwlock to become available.
 *@retval #LOS_ERRNO_RWLOCK_PEND_INTERR        The rwlock is being locked during an interrupt.
 *@retval #LOS_ERRNO_RWLOCK_PEND_IN_LOCK       The rwlock is waited on when the task scheduling is disabled.
 *@retval #LOS_ERRNO_RWLOCK_TIMEOUT            The rwlock waiting times out.
 *@retval #LOS_OK                              The rwlock is successfully locked.
 *@par Dependency:
 *<ul><li>los_rwlock.h: the header file that contains the API declaration.</li></ul>
 *@see LOS_RWLockCreate | LOS_RWReadUnLock
 *@since Huawei LiteOS
 */
UINT32 LOS_RWReadLock(UINT32 uwRWLockHandle, UINT32 uwTimeout);

/**
 *@ingroup los_rwlock
 *@brief Wait writer to lock a rwlock.
 *
 *@par Description:
 *This API is used to wait for a specified period of time to lock a rwlock.
 *@attention
 *<ul>
 *<li>The specific rwlock should be created firstly.</li>
 *<li>The function fails if the rwlock that is waited on is already locked by another thread when the task scheduling is disabled.</li>
 *<li>Do not wait on a rwlock during an interrupt.</li>
 *<li>The priority inheritance protocol is not supported.</li>
 *<li>A recursive rwlock can be locked more than once by the same thread.</li>
 *</ul>
 *
 *@param uwRWLockHandle    [IN] Handle of the rwlock to be waited on.  The value of handle should be in [0, LOSCFG_BASE_IPC_RWLOCK_LIMIT - 1].
 *@param uwTimeout         [IN] Waiting time. The value range is [0, LOS_WAIT_FOREVER](unit: Tick).
 *
 *@retval #LOS_ERRNO_RWLOCK_INVALID            The rwlock state (for example, the rwlock does not exist or is not in use) is not applicable for the current operation.
 *@retval #LOS_ERRNO_RWLOCK_UNAVAILABLE        The rwlock fails to be locked because it is locked by another thread and a period of time is not set for waiting for the rwlock to become available.
 *@retval #LOS_ERRNO_RWLOCK_PEND_INTERR        The rwlock is being locked during an interrupt.
 *@retval #LOS_ERRNO_RWLOCK_PEND_IN_LOCK       The rwlock is waited on when the task scheduling is disabled.
 *@retval #LOS_ERRNO_RWLOCK_TIMEOUT            The rwlock waiting times out.
 *@retval #LOS_OK                              The rwlock is successfully locked.
 *@par Dependency:
 *<ul><li>los_rwlock.h: the header file that contains the API declaration.</li></ul>
 *@see LOS_RWLockCreate | LOS_RWWriteUnLock
 *@since Huawei LiteOS
 */
UINT32 LOS_RWWriteLock(UINT32 uwRWLockHandle, UINT32 uwTimeout);

/**
 *@ingroup los_rwlock
 *@brief Reader Release a rwlock.
 *
 *@par Description:
 *This API is used to unlock a specified rwlock.
 *@attention
 *<ul>
 *<li>The specific rwlock should be created firstly.</li>
 *<li>Do not unlock a rwlock during an interrupt.</li>
 *<li>If a recursive rwlock is locked for many times, it must be unlocked for the same times to be released.</li>
 *</ul>
 *
 *@param uwRWLockHandle    [IN] Handle of the rwlock to be unlock. The value of handle should be in [0, LOSCFG_BASE_IPC_RWLOCK_LIMIT - 1].
 *
 *@retval #LOS_ERRNO_RWLOCK_INVALID            The rwlock state (for example, the rwlock does not exist or is not in use or owned by other thread) is not applicable for the current operation.
 *@retval #LOS_OK                              The rwlock is successfully unlock.
 *@par Dependency:
 *<ul><li>los_rwlock.h: the header file that contains the API declaration.</li></ul>
 *@see LOS_RWLockCreate | LOS_RWReadLock
 *@since Huawei LiteOS
 */
UINT32 LOS_RWReadUnLock(UINT32 uwRWLockHandle);

/**
 *@ingroup los_rwlock
 *@brief Writer Release a rwlock.
 *
 *@par Description:
 *This API is used to unlock a specified rwlock.
 *@attention
 *<ul>
 *<li>The specific rwlock should be created firstly.</li>
 *<li>Do not unlock a rwlock during an interrupt.</li>
 *<li>If a recursive rwlock is locked for many times, it must be unlocked for the same times to be released.</li>
 *</ul>
 *
 *@param uwRWLockHandle    [IN] Handle of the rwlock to be unlock. The value of handle should be in [0, LOSCFG_BASE_IPC_RWLOCK_LIMIT - 1].
 *
 *@retval #LOS_ERRNO_RWLOCK_INVALID            The rwlock state (for example, the rwlock does not exist or is not in use or owned by other thread) is not applicable for the current operation.
 *@retval #LOS_OK                              The rwlock is successfully unlock.
 *@par Dependency:
 *<ul><li>los_rwlock.h: the header file that contains the API declaration.</li></ul>
 *@see LOS_RWLockCreate | LOS_RWWriteLock
 *@since Huawei LiteOS
 */
UINT32 LOS_RWWriteUnLock(UINT32 uwRWLockHandle);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_RWLOCK_H */
