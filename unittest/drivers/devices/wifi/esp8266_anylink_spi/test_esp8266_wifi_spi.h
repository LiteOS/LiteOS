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
#ifndef __TEST_ESP8266_WIFI_SPI_H__
#define __TEST_ESP8266_WIFI_SPI_H__



/* Includes -----------------------------------------------------------------*/
#include <cpptest.h>
#include "stub.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"

extern "C"
{
	#include "M8266_WIFI_Config.h"
	#include "M8266_WIFI_SPI_IF.h"
	
	extern int LOS_SemPost_Control;
	extern int HAL_SPI_Init_Control;
	extern int M8266HostIf_SPI_Select_Control;
	extern int M8266WIFI_SPI_Interface_Communication_OK_Control; 
	extern int M8266WIFI_SPI_Interface_Communication_Stress_Test_Control;
	extern int M8266WIFI_SPI_Set_Opmode_Control;
	extern int M8266WIFI_SPI_Get_Opmode_Control;
	extern int M8266WIFI_SPI_STA_Connect_Ap_Control;
	extern int M8266WIFI_SPI_Get_STA_Connection_Status_Control;
	extern int M8266WIFI_SPI_STA_Query_Current_SSID_And_RSSI_Control;
	extern int M8266WIFI_SPI_Get_STA_IP_Addr_Control;
	extern int M8266WIFI_SPI_Setup_Connection_Control;
	extern int M8266WIFI_SPI_Send_Data_Control;
	extern int M8266WIFI_SPI_Query_Connection_Control;
	extern int M8266WIFI_SPI_RecvData_Control;
	extern int M8266WIFI_SPI_Has_DataReceived_Control;
	extern int M8266WIFI_SPI_Delete_Connection_Control;
	extern int LOS_SemCreate_Control;
	extern int LOS_QueueCreate_Control;
	extern int LOS_QueueWriteCopy_Control;
	extern int LOS_TaskCreate_Control;
	extern int LOS_QueueReadCopy_Control;
	extern int LOS_QueueDelete_Control;
	extern int m8266wifi_softstart_config_control;

	extern link_params link_occupied_flag_arrary[M8266_WIFI_MAX_LINK_NUM];
	extern __IO uint8_t  test_connect_func_flag;
}


/* Defines ------------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
/* Functions API ------------------------------------------------------------*/
class Test_Esp8266_Wifi_Spi : public Test::Suite
{
public:
    Test_Esp8266_Wifi_Spi();
    ~Test_Esp8266_Wifi_Spi();

   void test_func_m8266_wifi_init(void);
   void test_func_m8266wifi_query_certain_link_params(void);
   void test_func_m8266wifi_check_all_link_status(void);
   void test_func_m8266wifi_verify_connect_func(void);
   void test_func_m8266wifi_connect(void);
   void test_func_m8266wifi_send_data(void);
   void test_func_m8266wifi_spi_receive_data(void);
   void test_func_m8266wifi_spi_receive_data_timeout(void);
   void test_func_m8266wifi_spi_close(void);
};



#endif /* __TEST_ESP8266_H__ */

