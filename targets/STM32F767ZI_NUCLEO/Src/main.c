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
#include "cmsis_os2.h"
#include "hal_rng.h"
#include "dwt.h"
#include "M8266_WIFI_SPI_IF.h"
#include "M8266_WIFI_Config.h"
#include "agent_tiny_demo.h"

#if (LOSCFG_TEST == YES)
	#include "los_inspect_entry.h"
#else

#endif

/* variables */
uint32_t global_TaskHandle = 0;

const uint8_t * test_buf = (const uint8_t *)"hello tcp_ip";
uint8_t test_data[1024];

/* function prototype */
uint32_t creat_task_test();
void HardWare_Init(VOID);
void task_test();


void HardWare_Init(VOID)
{
    SystemClock_Config();
    Debug_USART3_UART_Init();
		hal_rng_config();
    dwt_delay_init(SystemCoreClock);
}


/**
  *@brief  main body of task_test
  *@params none
*/
void task_test()
{
	uint32_t uwcyclePerTick;
	uint64_t uwTickCount1,uwTickCount2;

	for( ; ; )
	{
		#ifdef BASIC_TEST_KERNEL
			/* this block of code is used to test whether LOS kernel works appropriately */
			{
				uwcyclePerTick  = LOS_CyclePerTickGet();
				if(0 != uwcyclePerTick)
				{
					printf("LOS_CyclePerTickGet = %d \n", uwcyclePerTick);
				}

				uwTickCount1 = LOS_TickCountGet();
				if(0 != uwTickCount1)
				{
					printf("LOS_TickCountGet = %d \n", (UINT32)uwTickCount1);
				}
				
				LOS_TaskDelay(2000);
				uwTickCount2 = LOS_TickCountGet();
				if(0 != uwTickCount2)
				{
					printf("LOS_TickCountGet after delay = %d \n", (UINT32)uwTickCount2);
				}
			}
		#endif
			
			/* agentiny code is here */
		#if 0
			{
				wifi_spi_if_register(&wifi_spi_if_instance);
		
				wifi_spi_if_init();
		
				for(int i = 0 ; i < M8266_WIFI_MAX_LINK_NUM ; i++)
				{
					wifi_spi_if_connect((const char *)"192.168.1.113" , (const char *)"8080" , ATINY_PROTO_TCP);
				}
				
				for(int i = 0 ; i < M8266_WIFI_MAX_LINK_NUM ; i++)
				{
					wifi_spi_if_send( i , test_buf , strlen((const char *)test_buf));
				}
				
				wifi_spi_if_check_link_params();
		
				while(1)
				{
					wifi_spi_if_recv(0 , test_data , 1024);
					wifi_spi_if_close(0);
					
					wifi_spi_if_recv(1 , test_data , 1024);
					wifi_spi_if_close(1);
					
					wifi_spi_if_recv(2 , test_data , 1024);
					wifi_spi_if_close(2);
					
					wifi_spi_if_recv(3 , test_data , 1024);
					wifi_spi_if_close(3);
					while(1);
				}
			}
		#endif
		
		printf("hello_world\n");
		wifi_spi_if_register(&wifi_spi_if_instance);
		wifi_spi_if_init();
		agent_tiny_entry();
		
	}
}


/**
  * @brief create a task to test kernel
  * @param none
  * @ret status--->the status of task creation
*/
uint32_t creat_task_test()
{
	uint32_t uwRet = LOS_OK;
  TSK_INIT_PARAM_S task_init_param;

  task_init_param.usTaskPrio = 0;
  task_init_param.pcName = "task_test";
  task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task_test;
  task_init_param.uwStackSize = 4096;   //minumun stack size is 304

  uwRet = LOS_TaskCreate(&global_TaskHandle, &task_init_param);
  if(LOS_OK != uwRet)
  {
		return uwRet;
  }
	
  return uwRet;
}


int main(void)
{
    uint32_t uwRet = LOS_OK;

    HardWare_Init();

    uwRet = LOS_KernelInit();
    if (uwRet != LOS_OK)
    {
        return LOS_NOK;
    }

#if (LOSCFG_TEST == YES)
	LOS_Inspect_Entry();
#else
    
#endif
		
		uwRet = creat_task_test();

	  if(uwRet != LOS_OK)
	  {
		  printf("create test task failed\n\r");
		  return uwRet;
	  }

    LOS_Start();
}




