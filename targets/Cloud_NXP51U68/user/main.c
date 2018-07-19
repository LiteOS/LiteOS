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
/* Freescale includes. */
#include "fsl_device_registers.h"
#include "board.h"

#include "pin_mux.h"
#include <stdbool.h>
#include "los_inspect_entry.h"
#include "los_base.h" 
#include "los_task.h"
#include "los_typedef.h"

#include "osport.h"



void *fnTsk1(unsigned int args)
{
	while(1)
	{
		printf("HELLO TASK1\n\r");
	
		LOS_TaskDelay(1000);
	}
	return NULL;
}
void *fnTsk2(unsigned int args)
{
	while(1)
	{
		printf("HELLO TASK2\n\r");
	
		LOS_TaskDelay(1000);
	}
	return NULL;
}

VOID HardWare_Init(VOID)
{
    /* Init board hardware. */
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM3);
    /* enable clock for GPIO*/
    CLOCK_EnableClock(kCLOCK_Gpio0);
    CLOCK_EnableClock(kCLOCK_Gpio1);

    /* attach 12 MHz clock to SPI3 */
    //CLOCK_AttachClk(kMCLK_to_FLEXCOMM3);
    /* reset FLEXCOMM for UART */
    RESET_PeripheralReset(kFC0_RST_SHIFT_RSTn);
    /* reset FLEXCOMM for SPI */
    RESET_PeripheralReset(kFC3_RST_SHIFT_RSTn);
    BOARD_InitPins();
    //BOARD_BootClockFROHF48M();
    BOARD_BootClockFROHF96M();
   // BOARD_InitDebugConsole();
    uart_init();
}



int main(void)
{
    UINT32 uwRet = LOS_OK;
    HardWare_Init();
    uwRet = LOS_KernelInit();
    if (uwRet != LOS_OK)
    {
        return LOS_NOK;
    }
    //we want to add a new mem here
    printf("LITEOS FOR LPCEXPRESS51U68--sytem start begin\n\r");
    //do the test 
    //LOS_Inspect_Entry();  //do the system check here
//	extern void *fnLedFlash(UINT32 arg);
  //creat_maintask("fnLedFlash",fnLedFlash,0x400,0,7);
	//extern void *fnGpioFlash(UINT32 arg);
  //creat_maintask("fnGpioFlash",fnGpioFlash,0x400,0,7);
    //extern status_t uart_init(VOID);
   // uart_init();
    
    
//  creat_maintask("fnTsk1",fnTsk1,0x400,0,7);
//  creat_maintask("fnTsk2",fnTsk2,0x400,0,8);
    //make the bin size
    /* Initilialize the LwIP stack without RTOS */
    //tcpip_init(NULL, NULL);
    //agent_tiny_entry();

    //extern void * main_spi(unsigned int args);
    //creat_maintask("main_spi",main_spi,0x400,0,7);
    extern void * main_network(unsigned int args);
    creat_maintask("main_network",main_network,0x1000,0,0);
    
  //  extern VOID *main_ppp(UINT32  args);
   // creat_maintask("main_ppp",main_ppp,0x1000,0,0);

    
    //
  //extern void* eth_task( void* pvParameters );
  //creat_maintask("eth_task",eth_task,0x1000,0,0);
    printf("LITEOS FOR LPCEXPRESS51U68 --sytem start done\n\r");
    LOS_Start();

}



