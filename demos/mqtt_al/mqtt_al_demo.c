/*----------------------------------------------------------------------------
 * Copyright (c) <2018>, <Huawei Technologies Co., Ltd>
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

#include <stdint.h>
#include <stddef.h>


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef WITH_MQTT_AL

#include "mqtt_al.h"
#include "osdepends/atiny_osdep.h"


#define cn_demo_qos          1
#define cn_demo_sub_topic    "/mqtt_al/test/download"
#define cn_demo_pub_topic    "/mqtt_al/test/upload"
#define cn_demo_clientid     "client_id"
#define cn_demo_user         "client_user"
#define cn_demo_passwd       "client_passwd"
#define cn_demo_server_addr  "47.106.88.116"
#define cn_demo_server_port   8883

static char s_mqtt_ca_crt[] =
"-----BEGIN CERTIFICATE-----\r\n"
"MIIEwTCCAqmgAwIBAgIRdi8oqJITu4uSWV2C/dUS1swwDQYJKoZIhvcNAQELBQAw\r\n"
"PDELMAkGA1UEBhMCQ04xDzANBgNVBAoTBkh1YXdlaTEcMBoGA1UEAxMTSHVhd2Vp\r\n"
"IEVxdWlwbWVudCBDQTAeFw0xNzAyMTYwNjM0MzVaFw00MTEwMDgwNjM0MzVaME0x\r\n"
"CzAJBgNVBAYTAkNOMQ8wDQYDVQQKEwZIdWF3ZWkxLTArBgNVBAMTJEh1YXdlaSBD\r\n"
"bG91ZCBDb3JlIE5ldHdvcmsgUHJvZHVjdCBDQTCCASIwDQYJKoZIhvcNAQEBBQAD\r\n"
"ggEPADCCAQoCggEBAKQQz5uvp3lmtK9uzeje7cZUF8RlRKavEQj9EQk45ly9a/Kr\r\n"
"07TlGIhaZv7j+N9ZV1jTiwA0+XWlni1anjy01qsBQ22eIzX3HQ3fTMjPfk67SFhS\r\n"
"aHdzkJwO66/Nojnaum84HfUTBuXfgiBNH4C2Bc9YBongLktwunqMGvMWEKj4YqjN\r\n"
"bjjZQ1G1Qnhk15qpEWI9YUv0I5X94oT5idEMIH+V+2hcW/6GmztoOgCekW3GPHGl\r\n"
"UJLt3cSaDkp1b5IchnGpwocZLJMd+V3emcLhbjXewIY3meUIkXMrqJ12L3ltkRVp\r\n"
"nHElgmpvp3dBjUrBiITLakrGq8P/Ta7OO5kf9JUCAwEAAaOBrDCBqTAfBgNVHSME\r\n"
"GDAWgBQq+BBZJ4A1H6d8ujufKuRKqpuS6jBGBgNVHSAEPzA9MDsGBFUdIAAwMzAx\r\n"
"BggrBgEFBQcCARYlaHR0cDovL3N1cHBvcnQuaHVhd2VpLmNvbS9zdXBwb3J0L3Br\r\n"
"aTAPBgNVHRMECDAGAQH/AgEAMA4GA1UdDwEB/wQEAwIBBjAdBgNVHQ4EFgQU9CcR\r\n"
"7wi0DatgF91OjC2HvAn0bG4wDQYJKoZIhvcNAQELBQADggIBADli3WJezyDe4qVl\r\n"
"jSpF3kxRvBPf6hPhf81WT/A3lo5f7rTOEkRqTB515i5p8Xv8QgP8WTcMu22K5oZt\r\n"
"6iV4PpWCaEVaHgAYeI8sjjWqI498nKWrJ1kaJkdOxB3omVa2Fet7xDCL6MR2jDZq\r\n"
"dtZGF2XCIiNuZhvTYOTvKTfuzN5/gi/z8GD8xP95V4vX3sB2jqgleMTirFdeA+RB\r\n"
"HDbS55MSIfA2jeXJt0IHoB5FKshcCPNLIW/s0O7lbSH62o4d+5PeVV8tbMESQ6Ve\r\n"
"NSRt22+n6Z2Z6n/ZMfM2jSziEZNIyPXQtywltkcrhRIbxWoB0IEr0Ci+7FVr9CRu\r\n"
"ZlcpliSKemrxiLo5EkoznWwxfUP11i473lUVljloJRglYWh6uo9Ci5Cu001occ4L\r\n"
"9qs13MTtpC96LouOyrqBUOlUmJvitqCrHSfqOowyi8B0pxH/+m+Q8+fP9cBztw22\r\n"
"JId8bth5j0OUbNDoY7DnjQLCI0bEZt4RSpQN1c6xf90gHutM62qoGI6NKlb2IH+r\r\n"
"Yfun6P4jYTN9vMnaDfxBH7ofz4q9nj27UBkX9ebqM8kS+RijnUUy8L2N6KsUpp2R\r\n"
"01cjcmp699kFIJ7ShCOmI95ZC9cociTnhTK6WScCweBH7eBxZwoQLi5ER3VkDs1r\r\n"
"rqnNVUgf2j9TOshCI6zuaxsA35wr\r\n"
"-----END CERTIFICATE-----\r\n";


static void mqtt_sub_dealer(void *arg,mqtt_al_msgrcv_t *msg)
{

	printf("TOPIC:%s QOS:%d DUP:%d RETAIN:%d PAYLOAD:%s\r\n",\
			msg->topic.data,msg->qos,msg->dup,msg->retain,msg->msg.data);
	return;
}


/** @brief:this is the mqtt_al demo to introduce you how to use the mqtt_al    */
static unsigned int mqtt_al_demo_main(void *arg)
{
	void *handle;
	mqtt_al_security_para_t  sec;
	mqtt_al_conpara_t  conpara;
	mqtt_al_subpara_t  subpara;
	mqtt_al_pubpara_t  pubpara;

	char  buf[64];
	int   pubtimes;
	//first,do the mqtt_init
	mqtt_al_init();
	//then do the connect
	memset(&conpara,0, sizeof(conpara));

	sec.type = en_mqtt_al_security_cas;
	sec.u.cas.ca_crt.data = s_mqtt_ca_crt;
	sec.u.cas.ca_crt.len = sizeof(s_mqtt_ca_crt);

	conpara.cleansession =1;

	conpara.clientid.data = cn_demo_clientid;
	conpara.clientid.len = strlen(cn_demo_clientid);

	conpara.conret  = 0;

	conpara.keepalivetime = 10; //unit:seconds

	conpara.passwd.data = cn_demo_passwd;
	conpara.passwd.len = strlen(cn_demo_passwd);

	conpara.security = &sec;

	conpara.serveraddr.data = cn_demo_server_addr;
	conpara.serveraddr.len = strlen(cn_demo_server_addr);
	conpara.serverport = cn_demo_server_port;

	conpara.timeout = 10000;              // unit:ms
	conpara.user.data = cn_demo_user;
	conpara.user.len = strlen(cn_demo_user);

	conpara.version = en_mqtt_al_version_3_1_1;
	conpara.willmsg = NULL;

	handle = mqtt_al_connect(&conpara);
	if(NULL == handle )
	{
		printf("connect err\r\n");
		return -1;
	}
	//do the subscribe, you also could do  in other task
	subpara.arg = NULL;
	subpara.dealer = mqtt_sub_dealer;
	subpara.qos = cn_demo_qos;
	subpara.subret = 0;
	subpara.timeout = 1000; // unit:ms
	subpara.topic.data = cn_demo_sub_topic;
	subpara.topic.len = strlen(cn_demo_sub_topic);

	if(0 != mqtt_al_subscribe(handle,&subpara))
	{
		printf("subscribe err\r\n");
		goto EXIT_SUBSCRIBE;
	}

	while(en_mqtt_al_connect_ok == mqtt_al_check_status(handle))
	{
		//evaluate the data
		snprintf(buf,64,"hello world:%d",pubtimes);
		pubpara.topic.data = cn_demo_pub_topic;
		pubpara.topic.len = strlen(cn_demo_pub_topic);
		pubpara.qos = cn_demo_qos;
		pubpara.retain = 0;
		pubpara.msg.data = buf;
		pubpara.msg.len = strlen(buf);
		pubpara.timeout = 10000;

		if(0 != mqtt_al_publish(handle,&pubpara))  //YOU COULD DO IT AS YOUR NEED
		{
			printf("publish err\r\n");
			goto EXIT_PUBLISH;
		}

		atiny_delay(10); //unit:seconds
	}

EXIT_PUBLISH:
EXIT_SUBSCRIBE:
	mqtt_al_disconnect(handle);
EXIT_CONNECT:
	mqtt_al_deinit();
	return 0;
}


int mqtt_al_demo()
{

	int ret = -1;

	TSK_INIT_PARAM_S task_init_param;

	memset (&task_init_param,0,sizeof(TSK_INIT_PARAM_S));

	task_init_param.pcName = "mqtt_al_demo";
	task_init_param.uwArg = NULL;
	task_init_param.pfnTaskEntry = mqtt_al_demo_main;
	task_init_param.usTaskPrio = 6;
	task_init_param.uwStackSize = 0x1000;

	if(LOS_OK  != LOS_TaskCreate(&ret,&task_init_param))
	{
		ret = -1;
	}

	return ret;

}

#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

