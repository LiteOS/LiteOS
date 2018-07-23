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

#include "sys_init.h"
#include "los_inspect_entry.h"
#include "los_demo_debug.h"
#include "los_swtmr.h"
#include "los_queue.h"


/* task ID */
static UINT32 g_uwDemoTxTaskID;
static UINT32 g_uwDemoRxTaskID;

/* task priority */
#define TASK_PRIO_DEMOTUTO     8
#define TASK_PRIO_DEMO_TXTSK   (TASK_PRIO_DEMOTUTO)
#define TASK_PRIO_DEMO_RXTSK   (TASK_PRIO_DEMOTUTO + 1)


/* software timer counter */
static UINT32 g_demotimercount = 0;

/* message queue */
static UINT32 g_uwDemoQueue;
static UINT32 g_uwDemoQ_NUM;

/* function declaration */
VOID HardWare_Init(VOID);
VOID Demo_Tutorial_Entry(VOID);
UINT32 Demo_SoftwareTimer_Entry(VOID);
static VOID Demo_SoftwareTimer_Callback(UINT32 arg);
UINT32 Demo_MsgQueue_Entry(VOID);
static UINT32 Demo_TransmitData_Entry(VOID);
static UINT32 Demo_ReceiveData_Entry(VOID);
static VOID TransmitData_Task(VOID);
static VOID ReceiveData_Task(VOID);


/*****************************************************************************/

int main(void)
{
    UINT32 uwRet = LOS_OK;

    HardWare_Init();

    uwRet = LOS_KernelInit();
    if (uwRet != LOS_OK)
    {
        return LOS_NOK;
    }

    /* This demo is a getting started tutorial */
    Demo_Tutorial_Entry();
    
    /* This entry function is for testing kernel components */
    //LOS_Inspect_Entry();  
    
    LOS_Start();
}

/*****************************************************************************
 Function    : HardWare_Init
 Description : 
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
VOID HardWare_Init(VOID)
{
    SystemClock_Config();
    Debug_USART1_UART_Init();
    Debug_LED_Init();
}

/*****************************************************************************
 Function    : Demo_Tutorial_Entry
 Description : Getting started tutorial which demonstrate kernel functions: 
               tasks, soft-timers, queues etc.
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
VOID Demo_Tutorial_Entry(VOID)
{
    /* now create a software timer */
    Demo_SoftwareTimer_Entry();
    
    /* now create a message queue */
    Demo_MsgQueue_Entry();    

    /* now create a task for transmit data */
    Demo_TransmitData_Entry();

    /* now create another task for recerive data */
    Demo_ReceiveData_Entry();
}


/*****************************************************************************
 Function    : Demo_SoftwareTimer_Entry
 Description : 
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
UINT32 Demo_SoftwareTimer_Entry(VOID)
{
    UINT16 id1;
    UINT32 uwRet = LOS_OK;

#if (LOSCFG_BASE_CORE_SWTMR_ALIGN == YES)
    uwRet = LOS_SwtmrCreate(1000, LOS_SWTMR_MODE_PERIOD, Demo_SoftwareTimer_Callback, &id1, 1, OS_SWTMR_ROUSES_ALLOW, OS_SWTMR_ALIGN_SENSITIVE);
#else
    uwRet = LOS_SwtmrCreate(1000, LOS_SWTMR_MODE_PERIOD, Demo_SoftwareTimer_Callback, &id1, 1);
#endif
    if (LOS_OK != uwRet)
    {
        dprintf("create Software Timer failed\n");
    }
    else
    {
        dprintf("create Software Timer success\n");
    }

    uwRet = LOS_SwtmrStart(id1);
    if (LOS_OK != uwRet)
    {
        dprintf("start Software Timer failed\n");
    }
    else
    {
        dprintf("start Software Timer sucess\n");
    }

    return LOS_OK;
}

/*****************************************************************************
 Function    : Demo_SoftwareTimer_Callback
 Description : 
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
static VOID Demo_SoftwareTimer_Callback(UINT32 arg)
{
    g_demotimercount ++;
    dprintf("[SW] software timer num = %d \n", g_demotimercount);

    /*increase number */
    g_uwDemoQ_NUM = g_demotimercount;

    /*reset software timer number */
    if(g_demotimercount % 10 == 0)
    {
        g_demotimercount = 0;
    }

}

/*****************************************************************************
 Function    : Demo_MsgQueue_Entry
 Description : 
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
UINT32 Demo_MsgQueue_Entry(VOID)
{
    UINT32 uwRet = 0;
    
    /* create queue */
    uwRet = LOS_QueueCreate("demo_queue", 1, &g_uwDemoQueue, 0, sizeof(UINT32));
    if (uwRet != LOS_OK)
    {
        dprintf("create queue failure!,error:%x\n",uwRet);
    }

    dprintf("create the queue success!\n");

    return LOS_OK;
}


/*****************************************************************************
 Function    : Demo_TransmitData_Entry
 Description : 
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
static UINT32 Demo_TransmitData_Entry(VOID)
{
    UINT32 uwRet;
    TSK_INIT_PARAM_S stTaskInitParam;
    
    (VOID)memset((VOID *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
    stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)TransmitData_Task;
    stTaskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE;
    stTaskInitParam.pcName = "TxdataTSK";
    stTaskInitParam.usTaskPrio = TASK_PRIO_DEMO_TXTSK;
    uwRet = LOS_TaskCreate(&g_uwDemoTxTaskID, &stTaskInitParam);
    if (uwRet != LOS_OK)
    {
        dprintf("create tx_task failed\n");
        return LOS_NOK;
    }
    
    return LOS_OK;
}

/*****************************************************************************
 Function    : Demo_ReceiveData_Entry
 Description : 
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
static UINT32 Demo_ReceiveData_Entry(VOID)
{
    UINT32 uwRet;
    TSK_INIT_PARAM_S stTaskInitParam;
    
    (VOID)memset((VOID *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
    stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)ReceiveData_Task;
    stTaskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE;
    stTaskInitParam.pcName = "RxdataTSK";
    stTaskInitParam.usTaskPrio = TASK_PRIO_DEMO_RXTSK;
    uwRet = LOS_TaskCreate(&g_uwDemoRxTaskID, &stTaskInitParam);
    if (uwRet != LOS_OK)
    {
        dprintf("create rx_task failed\n");
        return LOS_NOK;
    }

    return LOS_OK;
}


/*****************************************************************************
 Function    : TransmitData_Task
 Description : 
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
static VOID TransmitData_Task(VOID)
{
    UINT32 uwRet = 0;
    UINT32 uwlen = sizeof(UINT32);

    while (1)
    {
        /* write number to queue */
        uwRet = LOS_QueueWrite(g_uwDemoQueue, &g_uwDemoQ_NUM, uwlen, LOS_WAIT_FOREVER);
        if(uwRet != LOS_OK)
        {
            dprintf("send message failure,error:%x\n",uwRet);
        }
        else
        {
            dprintf("[TX] send message:%d\n", g_uwDemoQ_NUM);
        }

        LOS_TaskDelay(1500);
    }

}

/*****************************************************************************
 Function    : ReceiveData_Task
 Description : 
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
static VOID ReceiveData_Task(VOID)
{
    UINT32 **uwReadbuf;
    UINT32 uwRet = LOS_OK;
    UINT32 uwlen = sizeof(UINT32);
    
    while (1)
    {
        /* read data from queue to uwReadbuf */
        uwRet = LOS_QueueRead(g_uwDemoQueue, uwReadbuf, uwlen, LOS_WAIT_FOREVER);
        if (uwRet != LOS_OK)
        {
            dprintf("recv message failure,error:%x\n",uwRet);
            break;
        }
        else
        {
            dprintf("[RX] recv message:%d\n", *(*uwReadbuf));
        }

        /* if number ==10 or 5, light the LED */
        if( (**uwReadbuf == 10)||(**uwReadbuf == 5))
        {
            /*LED0 ON*/
            dprintf("[RX] LED ON\n");
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET); 
        }
        else
        {
            /*LED OFF*/ 
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET); 
        }
    
        (VOID)LOS_TaskDelay(1500);
    }

}






