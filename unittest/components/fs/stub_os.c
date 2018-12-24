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

/* Includes -----------------------------------------------------------------*/
#include "stub_os.h"

/* Defines ------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
UINT32  g_semCrt = -1;
UINT64  g_ullTickCount = 0;

/* Extern variables ---------------------------------------------------------*/
/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/

/*###################################   LiteOS    #######################################*/

/************************************* interrupt ***********************************/
UINTPTR LOS_IntLock(VOID)
{
    g_ullTickCount++;
    return 0;
}
VOID LOS_IntRestore(UINTPTR uvIntSave)
{
    return;
}

/************************************* task ***********************************/
UINT32 LOS_TaskCreate(UINT32 *puwTaskID, TSK_INIT_PARAM_S *pstInitParam)
{
    return 0;
}

UINT32 LOS_TaskDelay(UINT32 uwTick)
{
    return 0;
}

UINT32 LOS_TaskDelete(UINT32 uwTaskID)
{
    return 0;
}

CHAR* LOS_TaskNameGet(uint32_t uwTaskID)
{
    return NULL;
}


/************************************* semaphore ***********************************/
UINT32 LOS_BinarySemCreate (UINT16 usCount, UINT32 *puwSemHandle)
{
    return g_semCrt;
}

UINT32 LOS_SemCreate (UINT16 usCount, UINT32 *puwSemHandle)
{
    return 0;
}

UINT32 LOS_SemDelete(UINT32 uwSemHandle)
{
    return 0;
}
UINT32 LOS_SemPend(UINT32 uwSemHandle, UINT32 uwTimeout)
{
    usleep(1000);
    return 0;
}
UINT32 LOS_SemPost(UINT32 uwSemHandle)
{
    return 0;
}

/************************************* memory ***********************************/
void *LOS_MemAlloc (VOID *pPool, UINT32  uwSize)
{
    return malloc(uwSize);
}
UINT32 LOS_MemFree(VOID *pPool, VOID *pMem)
{
    free(pMem);
    return 0;
}

/************************************* mutex ***********************************/
UINT32 g_mux_pend_ret = 0;

UINT32 LOS_MuxPend(UINT32 uwMuxHandle, UINT32 uwTimeout)
{
    if (g_mux_pend_ret == -1)
    {
        g_mux_pend_ret = LOS_NOK;
        return LOS_OK;
    }

    return g_mux_pend_ret;
}

UINT32 LOS_MuxPost(UINT32 uwMuxHandle)
{
    return 0;
}

UINT32 g_mux_create_ret = 0;

UINT32  LOS_MuxCreate (UINT32 *puwMuxHandle)
{
    return g_mux_create_ret;
}

UINT32 LOS_MuxDelete(UINT32 uwMuxHandle)
{
    return 0;
}

UINT32 LOS_QueueCreate(CHAR *pcQueueName, UINT16 usLen, UINT32 *puwQueueID,
                            UINT32 uwFlags,UINT16 usMaxMsgSize )
{
    return 0;
}

UINT32 LOS_QueueReadCopy(UINT32  uwQueueID,VOID *  pBufferAddr,
                            UINT32 * puwBufferSize, UINT32  uwTimeOut)
{
    return 0;
}

UINT32 LOS_QueueDelete(UINT32 uwQueueID)
{
    return 0;
}

UINT32 LOS_QueueWriteCopy( UINT32 uwQueueID,VOID * pBufferAddr,
                             UINT32 uwBufferSize, UINT32 uwTimeOut )
{
    return 0;
}

UINT64 LOS_TickCountGet (VOID)
{
    return 0;
}

UINT32 LOS_CurTaskIDGet(void)
{
    return 0;
}

/* Private functions --------------------------------------------------------*/
