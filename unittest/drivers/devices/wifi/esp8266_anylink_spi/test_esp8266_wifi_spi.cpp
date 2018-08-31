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

#include "test_esp8266_wifi_spi.h"

int m8266wifi_query_certain_link_params_control = 0;
int m8266wifi_connect_control = 0;

void Test_Esp8266_Wifi_Spi::test_func_m8266_wifi_init(void)
{
	int ret;
	m8266wifi_softstart_config_control = 1;
	LOS_SemCreate_Control = 1;
	ret =  m8266_wifi_init();
	TEST_ASSERT(ret == 0 );

	m8266wifi_softstart_config_control = 1;
	LOS_SemCreate_Control = 0;
	LOS_TaskCreate_Control = 1;
	ret =  m8266_wifi_init();
	TEST_ASSERT(ret == 0 );

	m8266wifi_softstart_config_control = 1;
	LOS_SemCreate_Control = 0;
	LOS_TaskCreate_Control = 0;
	ret =  m8266_wifi_init();
    TEST_ASSERT(ret == 1 );

	m8266wifi_softstart_config_control = 0;
	LOS_SemCreate_Control = 0;
	LOS_TaskCreate_Control = 0;
	
	
}

void Test_Esp8266_Wifi_Spi::test_func_m8266wifi_query_certain_link_params(void)
{
	connection_params_struct plink;
	int ret;
	ret = m8266wifi_query_certain_link_params(M8266_WIFI_MAX_LINK_NUM , &plink);
	TEST_ASSERT(ret == 0);

	{
		plink.test_connection_type = 0;
		plink.test_connection_state = 0;
		plink.test_local_port = 8080;
		strcpy((char*)plink.test_remote_ip ,(char*)"192.168.1.1");
		plink.test_remote_port = 8081;
		plink.test_status = 0;
	}

	M8266WIFI_SPI_Query_Connection_Control = 0;
	ret = m8266wifi_query_certain_link_params(0 , &plink);
	TEST_ASSERT(ret == 0);

	M8266WIFI_SPI_Query_Connection_Control = 1;
	ret = m8266wifi_query_certain_link_params(0 , &plink);
	TEST_ASSERT(ret == 1);

	plink.test_connection_state = 1;
	ret = m8266wifi_query_certain_link_params(0 , &plink);
	TEST_ASSERT(ret == 1);

	plink.test_connection_state = 2;
	ret = m8266wifi_query_certain_link_params(0 , &plink);
	TEST_ASSERT(ret == 1);

	plink.test_connection_state = 3;
	ret = m8266wifi_query_certain_link_params(0 , &plink);
	TEST_ASSERT(ret == 1);

	plink.test_connection_state = 4;
	ret = m8266wifi_query_certain_link_params(0 , &plink);
	TEST_ASSERT(ret == 1);

	plink.test_connection_state = 5;
	ret = m8266wifi_query_certain_link_params(0 , &plink);
	TEST_ASSERT(ret == 1);

	plink.test_connection_state = 6;
	ret = m8266wifi_query_certain_link_params(0 , &plink);
	TEST_ASSERT(ret == 1);

	plink.test_connection_state = 7;
	ret = m8266wifi_query_certain_link_params(0 , &plink);
	TEST_ASSERT(ret == 1);
}


uint8_t stub_m8266wifi_query_certain_link_params(uint8_t link_no , connection_params_struct * plink)
{
	if(m8266wifi_query_certain_link_params_control == 0)
		return 0;
	else
	{
		plink->test_connection_type = 1;
		plink->test_connection_state = 0;
		plink->test_local_port = 8080;
		plink->test_remote_port = 8081;
		plink->test_status = 0;
		strcpy((char*)plink->test_remote_ip,(char*)"192.168.1.1");
	}
}


void Test_Esp8266_Wifi_Spi::test_func_m8266wifi_check_all_link_status(void)
{
	int ret;
	stubInfo si0;
	
	link_occupied_flag_arrary[0].busy_flag = 1;
	for(int i = 1 ; i < M8266_WIFI_MAX_LINK_NUM - 1 ; i++)
	{
		link_occupied_flag_arrary[i].busy_flag = 0;
	}

	m8266wifi_query_certain_link_params_control = 0;
	setStub( (void *)m8266wifi_query_certain_link_params, (void * )stub_m8266wifi_query_certain_link_params, &si0);
	ret = m8266wifi_check_all_link_status();
	TEST_ASSERT(ret == 0);

	m8266wifi_query_certain_link_params_control = 1;
	ret = m8266wifi_check_all_link_status();
	TEST_ASSERT(ret == 1);

	m8266wifi_query_certain_link_params_control = 0;
	cleanStub(&si0);
}


int32_t stubm8266wifi_connect(const char * host, const char * port, int32_t proto)
{
	if(m8266wifi_connect_control == -1)
		return -1;
	else if(m8266wifi_connect_control == 0)
		return 0;
}


void Test_Esp8266_Wifi_Spi::test_func_m8266wifi_verify_connect_func(void)
{
	int ret;
	connection_params_struct plink;
	stubInfo si0;

	m8266wifi_connect_control = -1;
	setStub((void *)m8266wifi_connect,(void *)stubm8266wifi_connect, &si0);
	ret = m8266wifi_verify_connect_func(&plink);
	TEST_ASSERT(ret == 0);

	m8266wifi_connect_control = 0;
	M8266WIFI_SPI_Query_Connection_Control = 0;
	ret = m8266wifi_verify_connect_func(&plink);
	TEST_ASSERT(ret == 0);

	m8266wifi_connect_control = 0;
	M8266WIFI_SPI_Query_Connection_Control = 1;
	{
		plink.test_connection_type = 1;
		plink.test_connection_state = 0;
		plink.test_local_port = 8080;
		plink.test_remote_port = 8181;
		plink.test_status = 0;
		strcpy((char*)plink.test_remote_ip , (char*)"192.168.1.1");
	}
	ret = m8266wifi_verify_connect_func(&plink);
	TEST_ASSERT(ret == 1);

	plink.test_connection_state = 1;	
	ret = m8266wifi_verify_connect_func(&plink);
	TEST_ASSERT(ret == 1);

	plink.test_connection_state = 2;	
	ret = m8266wifi_verify_connect_func(&plink);
	TEST_ASSERT(ret == 1);
	
	plink.test_connection_state = 3;	
	ret = m8266wifi_verify_connect_func(&plink);
	TEST_ASSERT(ret == 1);

	plink.test_connection_state = 4;	
	ret = m8266wifi_verify_connect_func(&plink);
	TEST_ASSERT(ret == 1);

	plink.test_connection_state = 5;	
	ret = m8266wifi_verify_connect_func(&plink);
	TEST_ASSERT(ret == 1);

	plink.test_connection_state = 6;	
	ret = m8266wifi_verify_connect_func(&plink);
	TEST_ASSERT(ret == 1);

	plink.test_connection_state = 7;	
	ret = m8266wifi_verify_connect_func(&plink);
	TEST_ASSERT(ret == 1);
	
	
	cleanStub(&si0);
	
}


void Test_Esp8266_Wifi_Spi::test_func_m8266wifi_connect(void)
{
	int ret;
	ret = m8266wifi_connect((const char *)"192.168.1.113" , (const char *)"0" , 0);
	TEST_ASSERT(ret == -1);

	ret = m8266wifi_connect((const char *)"192.168.1.113" , (const char *)"ABCD" , 0);
	TEST_ASSERT(ret == -1);

	for(int i = 0 ; i < M8266_WIFI_MAX_LINK_NUM ; i++ )
	{
		link_occupied_flag_arrary[i].busy_flag = 1;
	}
	ret = m8266wifi_connect((const char *)"192.168.1.113" , (const char *)"8081" , 0);
	TEST_ASSERT(ret == -1);

	for(int i = 0 ; i < M8266_WIFI_MAX_LINK_NUM ; i++ )
	{
		link_occupied_flag_arrary[i].busy_flag = 0;
	}
	test_connect_func_flag = 1;
	M8266WIFI_SPI_Setup_Connection_Control = 0;
	ret = m8266wifi_connect((const char *)"192.168.1.113" , (const char *)"8081" , 0);
	TEST_ASSERT(ret == -1);

	test_connect_func_flag = 0;
	M8266WIFI_SPI_Setup_Connection_Control = 0;
	ret = m8266wifi_connect((const char *)"192.168.1.113" , (const char *)"8081" , 0);
	TEST_ASSERT(ret == -1);

	test_connect_func_flag = 0;
	M8266WIFI_SPI_Setup_Connection_Control = 1;
	LOS_QueueCreate_Control = 1;
	ret = m8266wifi_connect((const char *)"192.168.1.113" , (const char *)"8081" , 0);
	TEST_ASSERT(ret == -1);

	test_connect_func_flag = 0;
	M8266WIFI_SPI_Setup_Connection_Control = 1;
	LOS_QueueCreate_Control = 0;
	ret = m8266wifi_connect((const char *)"192.168.1.113" , (const char *)"8081" , 0);
	TEST_ASSERT(ret == 0);
	
	
}


void Test_Esp8266_Wifi_Spi::test_func_m8266wifi_send_data(void)
{
	int ret;
	uint8_t buf[1024];
	ret =  m8266wifi_send_data(0 , buf, 0);
	TEST_ASSERT(ret == -1);

	ret =  m8266wifi_send_data(0 , buf, 65536);
	TEST_ASSERT(ret == -1);

	ret =  m8266wifi_send_data(M8266_WIFI_MAX_LINK_NUM , buf, 256);
	TEST_ASSERT(ret == -1);

	M8266WIFI_SPI_Send_Data_Control = 1;
	ret =  m8266wifi_send_data(0 , buf, 256);
	TEST_ASSERT(ret == 10);
	
}

void Test_Esp8266_Wifi_Spi::test_func_m8266wifi_spi_receive_data()
{
	int ret;
	uint8_t buf[1024];
	LOS_QueueReadCopy_Control = 0;
	ret = m8266wifi_spi_receive_data(0 , buf , 1024);
	TEST_ASSERT(ret == 18);

	LOS_QueueReadCopy_Control = 1;
	ret = m8266wifi_spi_receive_data(0 , buf , 1024);
	TEST_ASSERT(ret == -1);
}


void Test_Esp8266_Wifi_Spi::test_func_m8266wifi_spi_receive_data_timeout(void)
{
	int ret;
	uint8_t buf[1024];
	LOS_QueueReadCopy_Control = 0;
	ret = m8266wifi_spi_receive_data_timeout(0 , buf , 1024 , 1000);
	TEST_ASSERT(ret == 18);

	LOS_QueueReadCopy_Control = 1;
	ret = m8266wifi_spi_receive_data_timeout(0 , buf , 1024 , 1000);
	TEST_ASSERT(ret == -1);
}


uint8_t	stub_m8266wifi_check_all_link_status(void)
{
	return 1;
}


void Test_Esp8266_Wifi_Spi::test_func_m8266wifi_spi_close(void)
{
	int ret;
	stubInfo si0;
	ret = m8266wifi_spi_close(M8266_WIFI_MAX_LINK_NUM);
	TEST_ASSERT(ret == 0);

	LOS_QueueDelete_Control = 1;
	ret = m8266wifi_spi_close(0);
	TEST_ASSERT(ret == 0);

	setStub((void *)m8266wifi_check_all_link_status, (void *)stub_m8266wifi_check_all_link_status, &si0);
	LOS_QueueDelete_Control = 0;
	M8266WIFI_SPI_Delete_Connection_Control = 0;
	ret = m8266wifi_spi_close(0);
	TEST_ASSERT(ret == 1);

	LOS_QueueDelete_Control = 0;
	M8266WIFI_SPI_Delete_Connection_Control = 1;
	ret = m8266wifi_spi_close(0);
	TEST_ASSERT(ret == 1);

	cleanStub(&si0);
	
}



Test_Esp8266_Wifi_Spi::Test_Esp8266_Wifi_Spi()
{
	TEST_ADD(Test_Esp8266_Wifi_Spi::test_func_m8266_wifi_init);
	TEST_ADD(Test_Esp8266_Wifi_Spi::test_func_m8266wifi_query_certain_link_params);
	TEST_ADD(Test_Esp8266_Wifi_Spi::test_func_m8266wifi_check_all_link_status);
	TEST_ADD(Test_Esp8266_Wifi_Spi::test_func_m8266wifi_verify_connect_func);
	TEST_ADD(Test_Esp8266_Wifi_Spi::test_func_m8266wifi_connect);
	TEST_ADD(Test_Esp8266_Wifi_Spi::test_func_m8266wifi_send_data);
	TEST_ADD(Test_Esp8266_Wifi_Spi::test_func_m8266wifi_spi_receive_data);
	TEST_ADD(Test_Esp8266_Wifi_Spi::test_func_m8266wifi_spi_receive_data_timeout);
	TEST_ADD(Test_Esp8266_Wifi_Spi::test_func_m8266wifi_spi_close);
}

Test_Esp8266_Wifi_Spi::~Test_Esp8266_Wifi_Spi()
{
	printf("end of test\n");
}


