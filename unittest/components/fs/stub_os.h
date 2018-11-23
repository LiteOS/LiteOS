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

/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __STUB_OS_H__
#define __STUB_OS_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/* Includes -----------------------------------------------------------------*/
#include <stdlib.h>
#include "los_typedef.h"
#include "los_task.h"

/* Defines ------------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
/* Functions API ------------------------------------------------------------*/

/*###################################   LiteOS    #######################################*/
/* interrupt */
extern UINTPTR LOS_IntLock(VOID);
extern VOID LOS_IntRestore(UINTPTR uvIntSave);
/* task */
extern UINT32 LOS_TaskCreate(UINT32 *puwTaskID, TSK_INIT_PARAM_S *pstInitParam);
extern UINT32 LOS_TaskDelay(UINT32 uwTick);
extern UINT32 LOS_TaskDelete(UINT32 uwTaskID);
extern CHAR* LOS_TaskNameGet(uint32_t uwTaskID);

/* semaphore */
extern UINT32 LOS_BinarySemCreate (UINT16 usCount, UINT32 *puwSemHandle);
extern UINT32 LOS_SemCreate (UINT16 usCount, UINT32 *puwSemHandle);
extern UINT32 LOS_SemDelete(UINT32 uwSemHandle);
extern UINT32 LOS_SemPend(UINT32 uwSemHandle, UINT32 uwTimeout);
extern UINT32 LOS_SemPost(UINT32 uwSemHandle);
/* memory */
extern void  *LOS_MemAlloc (VOID *pPool, UINT32  uwSize);
extern UINT32 LOS_MemFree(VOID *pPool, VOID *pMem);
/* mutex */
extern UINT32 LOS_MuxCreate (UINT32 *puwMuxHandle);
extern UINT32 LOS_MuxDelete(UINT32 uwMuxHandle);
extern UINT32 LOS_MuxPend(UINT32 uwMuxHandle, UINT32 uwTimeout);
extern UINT32 LOS_MuxPost(UINT32 uwMuxHandle);
/* queue */
extern UINT32 LOS_QueueCreate(CHAR *pcQueueName, UINT16 usLen, UINT32 *puwQueueID,
                                    UINT32 uwFlags,UINT16 usMaxMsgSize );
extern UINT32 LOS_QueueDelete(UINT32 uwQueueID);
extern UINT32 LOS_QueueReadCopy(UINT32  uwQueueID, VOID *  pBufferAddr,
                                    UINT32 * puwBufferSize, UINT32  uwTimeOut);
extern UINT32 LOS_QueueWriteCopy( UINT32 uwQueueID, VOID * pBufferAddr,
                                    UINT32 uwBufferSize, UINT32 uwTimeOut );
/* tick */
extern UINT64 LOS_TickCountGet(VOID);

extern UINT32 LOS_CurTaskIDGet(void);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __STUB_OS_H__ */
