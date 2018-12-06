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

//#include "sys_init.h"
#ifdef CONFIG_FEATURE_FOTA
#include "ota_port.h"
#endif
#include "at_frame/at_api.h"
#include "at_device/bc95.h"
#include "usart.h"

static UINT32 g_atiny_tskHandle;


uint8_t aTxStartMessages[] = "\r\n******UART commucition using IT******\r\nPlease enter characters:\r\n";
uint8_t aRxBuffer[512];
uint16_t wBuffer_Index = 0;
extern UART_HandleTypeDef huart1;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	//UNUSED(huart);
	if(USART1 == huart->Instance)
    {   
	    uint16_t wp;
        wBuffer_Index++;
        if(wBuffer_Index >= 512)
            wBuffer_Index = 0;
        HAL_UART_Receive_IT(&huart1,(uint8_t*)&aRxBuffer[wBuffer_Index],1);
        #if 0
        if(wBuffer_Index %8 == 0)
        {
            if(wBuffer_Index >= 8)
            {
                wp = wBuffer_Index - 8;

            }
            else
            {
                wp = 512 - 8;
                
            }
            HAL_UART_Transmit(&huart1,(uint8_t*)&aRxBuffer[wp],8,0xFFFF);
            
        }
        #endif
    }
    
}

void atiny_usart1_rx_entry(void)
{
    HAL_UART_Transmit_IT(&huart1 ,(uint8_t*)aTxStartMessages,sizeof(aTxStartMessages)); 
	HAL_UART_Receive_IT(&huart1,(uint8_t*)&aRxBuffer[wBuffer_Index],1);
}

void atiny_task_entry(void)
{
    atiny_usart1_rx_entry();
	extern void demo_nbiot_only();
    //demo_nbiot_only();
}




UINT32 creat_main_task(VOID)
{
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 2;
    task_init_param.pcName = "agenttiny_task";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)atiny_task_entry;

#if defined(CONFIG_FEATURE_FOTA) || defined(WITH_MQTT)
    task_init_param.uwStackSize = 0x2000; /* fota use mbedtls bignum to verify signature  consuming more stack  */
#else
    task_init_param.uwStackSize = 0x1000;
#endif

    uwRet = LOS_TaskCreate(&g_atiny_tskHandle, &task_init_param);
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;
}





