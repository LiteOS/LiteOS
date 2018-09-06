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
#if defined(WITH_AT_FRAMEWORK) && defined(USE_ESP8266)

#include "agent_tiny_demo.h"
#ifdef CONFIG_FEATURE_FOTA
#include "fota_port.h"
#endif
//#define DEFAULT_SERVER_IPV4 "139.159.209.89"/*Huawei */
//#define DEFAULT_SERVER_IPV4 "192.168.0.116"/*Huawei */
//#define DEFAULT_SERVER_IPV4 "192.168.1.111"/*sjn */
#define DEFAULT_SERVER_IPV4 "180.101.147.115"/*dianxin */

#define LWM2M_LIFE_TIME     50000

char * g_endpoint_name = "201808070034";
#ifdef WITH_DTLS
char* g_endpoint_name_s = "11110001";
char* g_endpoint_name_iots = "66667777";
char* g_endpoint_name_bs = "22224444";
unsigned char g_psk_iot_value[16] = {0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33};
unsigned char g_psk_bs_value[12] = {0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33};
//unsigned char g_psk_value[16] = {0xef,0xe8,0x18,0x45,0xa3,0x53,0xc1,0x3c,0x0c,0x89,0x92,0xb3,0x1d,0x6b,0x6a,0x33};
#endif

static void* g_phandle = NULL;
static atiny_device_info_t g_device_info;
static atiny_param_t g_atiny_params;


msg_for_Wifi Wifi_send;
DHT11_Data_TypeDef  DHT11_Data;

void ack_callback(atiny_report_type_e type, int cookie, data_send_status_e status)
{
    printf("type:%d cookie:%d status:%d\n", type,cookie, status);
}

void app_data_report(void)
{
		UINT32 uwRet = LOS_OK;
    data_report_t report_data;
    int ret;
    int cnt = 0;
//    sprintf(Wifi_send.buf, "31312e3131312e31");
    report_data.buf = (uint8_t *)(Wifi_send.buf);
    report_data.callback = ack_callback;
    report_data.cookie = 0;
    report_data.len = sizeof(Wifi_send.buf);
    report_data.type = APP_DATA;
    while(1)
    {
        report_data.cookie = cnt;
        cnt++;
        ret = atiny_data_report(g_phandle, &report_data);
        printf("report ret:%d\n",ret);
        uwRet=LOS_TaskDelay(250*8);
				if(uwRet !=LOS_OK)
				return;
    }
}

UINT32 creat_report_task()
{
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;
    UINT32 TskHandle;

    task_init_param.usTaskPrio = 1;
    task_init_param.pcName = "app_data_report";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)app_data_report;
    task_init_param.uwStackSize = 0x400;

    uwRet = LOS_TaskCreate(&TskHandle, &task_init_param);
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;

}
#ifdef CONFIG_FEATURE_FOTA
void agent_tiny_fota_init(void)
{
    atiny_fota_storage_device_s *storage_device = NULL ;
    fota_hardware_s *hardware = NULL;
    fota_pack_device_info_s device_info;

    (void)hal_init_fota();

    (void)hal_get_fota_device(&storage_device, &hardware);

    device_info.hardware = hardware;
    device_info.storage_device = storage_device;
    device_info.head_info_notify  = NULL;
    (void)fota_set_pack_device(fota_get_pack_device(), &device_info);
}
#endif
VOID data_collection_task(VOID)
{
	UINT32 uwRet = LOS_OK;
	short int Lux;
	DHT11_Init();
	Init_BH1750();
	while (1)
  {

	  /****************temperature and humidity*****************/
	if(DHT11_Read_TempAndHumidity(&DHT11_Data)==SUCCESS)
	{
		printf("Read DHT11 success!-->Hum is %.1f £¥RH £¬Temp is %.1f¡æ \n",DHT11_Data.humidity,DHT11_Data.temperature);
	}
	else
	{
		printf("read DHT11 fialed\n");
		DHT11_Init();      
	}
				/****************BH1750******************/
		Lux=(int)Convert_BH1750();
		printf("\r\n******************************BH1750 Value is  %d\r\n",Lux);
		
		sprintf((char*)Wifi_send.buf, "%.1f%.1f%5d", DHT11_Data.temperature,DHT11_Data.humidity,Lux);
		
		uwRet=LOS_TaskDelay(1500);
		if(uwRet !=LOS_OK)
		return;
	
  }
}
UINT32 creat_data_collection_task()
{
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;
    UINT32 TskHandle;

    task_init_param.usTaskPrio = 0;
    task_init_param.pcName = "data_collection_task";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)data_collection_task;
    task_init_param.uwStackSize = 0x1000;

    uwRet = LOS_TaskCreate(&TskHandle, &task_init_param);
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;
}
void agent_tiny_entry(void)
{
    UINT32 uwRet = LOS_OK;
    atiny_param_t* atiny_params;
    atiny_security_param_t  *iot_security_param = NULL;
    atiny_security_param_t  *bs_security_param = NULL;

    atiny_device_info_t *device_info = &g_device_info;
#ifdef CONFIG_FEATURE_FOTA
    agent_tiny_fota_init();
#endif

#ifdef WITH_DTLS
    device_info->endpoint_name = g_endpoint_name_s;
#else
    device_info->endpoint_name = g_endpoint_name;
#endif
    device_info->manufacturer = "Agent_Tiny";

    atiny_params = &g_atiny_params;
    atiny_params->server_params.binding = "UQ";
    //atiny_params->server_params.life_time = LWM2M_LIFE_TIME;
    atiny_params->server_params.life_time = 20;
    atiny_params->server_params.storing_cnt = 0;

    atiny_params->bootstrap_mode = BOOTSTRAP_FACTORY;

    //pay attention: index 0 for iot server, index 1 for bootstrap server.
    iot_security_param = &(atiny_params->security_params[0]);
    bs_security_param = &(atiny_params->security_params[1]);


    iot_security_param->server_ip = DEFAULT_SERVER_IPV4;
    bs_security_param->server_ip = DEFAULT_SERVER_IPV4;

#ifdef WITH_DTLS
    iot_security_param->server_port = "5684";
    bs_security_param->server_port = "5684";

    iot_security_param->psk_Id = g_endpoint_name_iots;
    iot_security_param->psk = (char*)g_psk_iot_value;
    iot_security_param->psk_len = 16;

    bs_security_param->psk_Id = g_endpoint_name_bs;
    bs_security_param->psk = (char*)g_psk_bs_value;
    bs_security_param->psk_len = 16;
#else
    iot_security_param->server_port = "5683";
    bs_security_param->server_port = "5683";

    iot_security_param->psk_Id = NULL;
    iot_security_param->psk = NULL;
    iot_security_param->psk_len = 0;

    bs_security_param->psk_Id = NULL;
    bs_security_param->psk = NULL;
    bs_security_param->psk_len = 0;
#endif

    if(ATINY_OK != atiny_init(atiny_params, &g_phandle))
    {
        return;
    }

    uwRet = creat_report_task();
    if(LOS_OK != uwRet)
    {
        return;
    }
	uwRet = creat_data_collection_task();
    if(LOS_OK != uwRet)
    {
        return ;
    }
    (void)atiny_bind(device_info, g_phandle);
}
#endif