/*----------------------------------------------------------------------------
 * Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
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

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */


/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
#include <string.h>
#include "los_inspect_entry.h"
#include "los_task.h"
#include "los_demo_debug.h"

#include "los_bsp_adapter.h"
#include "los_bsp_led.h"
#include "los_bsp_key.h"
#include "los_bsp_uart.h"

/* task schedule */
#include "los_api_task.h"
/* dynamic memory */
#include "los_api_dynamic_mem.h"
/* static memory */
#include "los_api_static_mem.h"
/* interrupt */
#include "los_api_interrupt.h"
/* message queue */
#include "los_api_msgqueue.h"
/* event  */
#include "los_api_event.h"
/* mutex */
#include "los_api_mutex.h"
/* semphore */
#include "los_api_sem.h"
/* sw timer */
#include "los_api_timer.h"
/* system tick */
#include "los_api_systick.h"
/* dlist */
#include "los_api_list.h"


/*****************************************************************************
    global var
 *****************************************************************************/
static UINT32 gInspectErrCnt = 0;

static UINT32 g_uwDemoTaskID;

#ifndef RAM_SIZE_LEVEL_0

/*巡检项目表*/
static osInspect_Def gInspect[LOS_INSPECT_BUFF] = {

    {LOS_INSPECT_TASK,LOS_INSPECT_STU_START,Example_TskCaseEntry,"TASK"},

    {LOS_INSPECT_EVENT,LOS_INSPECT_STU_START,Example_SndRcvEvent,"EVENT"},
    
    {LOS_INSPECT_MSG,LOS_INSPECT_STU_START,Example_MsgQueue,"MSG"},
    
    {LOS_INSPECT_SEM,LOS_INSPECT_STU_START,Example_Semphore,"SEM"},
    
    {LOS_INSPECT_MUTEX,LOS_INSPECT_STU_START,Example_MutexLock,"MUTEX"},
    
    {LOS_INSPECT_SYSTIC,LOS_INSPECT_STU_START,Example_GetTick,"SYSTIC"},
    
    {LOS_INSPECT_TIMER,LOS_INSPECT_STU_START,Example_swTimer,"TIMER"},
    
    {LOS_INSPECT_LIST,LOS_INSPECT_STU_START,Example_list,"LIST"},
    
    {LOS_INSPECT_SMEM,LOS_INSPECT_STU_START,Example_StaticMem,"S_MEM"},
    
    {LOS_INSPECT_DMEM,LOS_INSPECT_STU_START,Example_Dyn_Mem,"D_MEM"},
    
    //{LOS_INSPECT_INTERRUPT,LOS_INSPECT_STU_START,Example_Interrupt},//目前中断测试未加入
    
};

#endif
/*****************************************************************************
    LOS function private
 *****************************************************************************/
static void LOS_Inspect_KeyAndLed(UINT32 KeyID,UINT32 LedID);


/*****************************************************************************
 Function    : LOS_InspectStatusSetByID
 Description : Set Inspect Status ByID
 Input       : enInspectID  InspectID
               enInspectStu InspectStu
 Output      : None
 Return      : None
 *****************************************************************************/
UINT32 LOS_InspectStatusSetByID(enInspectID InspectID,enInspectStu InspectStu)
{
#ifndef RAM_SIZE_LEVEL_0
    UINT32 ulIndex = 0;

    if(InspectID >= LOS_INSPECT_BUFF)
    {
        dprintf("\r\n InspectID = [%d] Err.\r\n",InspectID);
        return LOS_NOK;
    }

    for(ulIndex = 0;ulIndex < LOS_INSPECT_BUFF;ulIndex++)
    {   
        if(InspectID == gInspect[ulIndex].InspectID)
        {
            gInspect[ulIndex].Status = InspectStu;
        }
     
    }

    if(LOS_INSPECT_BUFF == ulIndex)
    {
        return LOS_NOK;
    }
    else
    {
        return LOS_OK;
    }
#else
    return LOS_OK;
#endif

}

/*****************************************************************************
 Function    : LOS_InspectByID
 Description : Inspect function By ID
 Input       : enInspectID  InspectID
 Output      : None
 Return      : LOS_NOK/LOS_OK
 *****************************************************************************/
UINT32 LOS_InspectByID(enInspectID InspectID)
{
#ifndef RAM_SIZE_LEVEL_0
    UINT32 ulIndex,ulRet = LOS_OK;
    enInspectStu  enCurStatus = LOS_INSPECT_STU_START;
    
    if(InspectID >= LOS_INSPECT_BUFF)
    {
        dprintf("\r\n InspectID = [%d] Err.\r\n",InspectID);
        return LOS_NOK;
    }
    
    for(ulIndex = 0;ulIndex < LOS_INSPECT_BUFF;ulIndex++)
    {
        if(InspectID == gInspect[ulIndex].InspectID)
        {
            if(NULL == gInspect[ulIndex].Inspectfunc)
            {
                dprintf("InspectID = [%d] Err,Inspectfunc is NULL.\r\n\r\n",InspectID);
                return LOS_NOK;
            }
            
            ulRet = gInspect[ulIndex].Inspectfunc();
            
            do{
                if((LOS_INSPECT_STU_SUCCESS == gInspect[ulIndex].Status) && (ulRet == LOS_OK))
                {
                    printf("Inspect %s success\r\n\r\n",gInspect[ulIndex].name);
                    enCurStatus = LOS_INSPECT_STU_SUCCESS;
                }
                else if(LOS_INSPECT_STU_ERROR == gInspect[ulIndex].Status)
                {
                    enCurStatus = LOS_INSPECT_STU_ERROR;
                    printf("Inspect %s error,gInspectErrCnt = [%d]\r\n\r\n",gInspect[ulIndex].name,gInspectErrCnt);
                }
                
                LOS_TaskDelay(100);
                    
            }while((LOS_INSPECT_STU_START == gInspect[ulIndex].Status)||(enCurStatus != gInspect[ulIndex].Status));
            
            break;
        }
    }
    
    if(LOS_INSPECT_BUFF == ulIndex)
    {
        return LOS_NOK;
    }
    else
    {
        return enCurStatus ? LOS_NOK:LOS_OK;
    }
#else
    return LOS_OK;
#endif

}

/*****************************************************************************
 Function    : LOS_Inspect_KeyAndLed
 Description : Inspect Task Deal
 Input       : UINT32 KeyID
               UINT32 LedID
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_Inspect_KeyAndLed(UINT32 KeyID,UINT32 LedID)
{
     /* check if the user key is pressed */
    if(LOS_KEY_PRESS == LOS_EvbGetKeyVal(KeyID)){
                 
        LOS_TaskDelay(50);
            
        if(LOS_KEY_PRESS == LOS_EvbGetKeyVal(KeyID)){
                    
            /* turn on LED1 */
            LOS_EvbLedControl(LedID,LED_ON);

            /* output a message on hyperterminal using dprintf function */
            dprintf("\r\n Key test example \r\n");

        }else{
            /* turn off LED1 */
            LOS_EvbLedControl(LedID,LED_OFF);
        }
    }else{
        /* turn off LED1 */
        LOS_EvbLedControl(LedID,LED_OFF);
    }

    return;
}

/*****************************************************************************
 Function    : LOS_Inspect_TskDeal
 Description : Inspect Task Deal
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_Inspect_TskDeal(void)
{
    UINT32 ulRet = LOS_OK;
     
    gInspectErrCnt = 0;
      
     /* output a message on hyperterminal using printf function */
    dprintf("\r\n Los Inspect start.\r\n");
      
    for(int index = 0;index < LOS_INSPECT_BUFF;index++)
    {
        ulRet = LOS_InspectByID((enInspectID)index);
        if(LOS_OK != ulRet)
        {
            gInspectErrCnt++;
            
            /* turn on LED2 */
            LOS_EvbLedControl(LOS_LED2,LED_ON);
        }
    }
     
     printf("Inspect completed,gInspectErrCnt = [%d]\r\n\r\n",gInspectErrCnt);
     
     /* output a message on hyperterminal using printf function */
    dprintf("\r\n Los Key example: please press the UserKey key \r\n");
    while(1)
    {
        LOS_Inspect_KeyAndLed(USER_KEY,LOS_LED1);
    }
}

/*****************************************************************************
 Function    : LOS_Inspect_Entry
 Description : Create Inspect task
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_Inspect_Entry(void)
{
    UINT32 uwRet;
    TSK_INIT_PARAM_S stTaskInitParam;

    (VOID)memset((void *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
    stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)LOS_Inspect_TskDeal;
    stTaskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE;
    stTaskInitParam.pcName = "InspectTsk";
    stTaskInitParam.usTaskPrio = 9;
    uwRet = LOS_TaskCreate(&g_uwDemoTaskID, &stTaskInitParam);
    if (uwRet != LOS_OK)
    {
        return;
    }
    
    return;
}




#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

