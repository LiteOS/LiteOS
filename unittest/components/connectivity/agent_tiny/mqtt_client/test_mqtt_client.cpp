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
#include "test_mqtt_client.h"
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> 

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>


//#include <cstring>
/* Defines ------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
extern "C"
{
#include "los_base.h"
#include "los_task.ph"
#include "los_typedef.h"
#include "los_sys.h"
#include "mqtt_client.h"
//#include "atiny_adatper.h"
//#include "mqtt_client.h"
#define MQTT_VERSION_3_1 (3)
#define MQTT_VERSION_3_1_1 (4)
#define MQTT_CLEAN_SESSION_TRUE (1)
#define MQTT_CLEAN_SESSION_FALSE (0)
#define MQTT_TOPIC_SUBSCRIBED_TRUE (1)
#define MQTT_TOPIC_SUBSCRIBED_FALSE (0)

static int g_random = 0;

static int i = 0;

static int funcno = 0;

stubInfo si_atiny_strdup;

typedef struct
{
    atiny_device_info_t device_info;
    MQTTClient client;
    atiny_param_t atiny_params;
    char atiny_quit;
    char bind_quit;
} handle_data_t;


void message_test(cloud_msg_t *msg)
{
    ATINY_LOG(LOG_DEBUG, "%.*s : %.*s", msg->uri_len, msg->uri, msg->payload_len,  (char *)msg->payload);
}


atiny_interest_uri_t g_interest_uris_test[ATINY_INTEREST_URI_MAX_NUM] =
{
    {
        .uri = "/helloworldtest",
        .qos = CLOUD_QOS_MOST_ONCE,
        .cb = message_test
    },
};


extern void atiny_param_member_free(atiny_param_t *param);
extern int atiny_param_dup(atiny_param_t *dest, atiny_param_t *src);
extern int atiny_init(atiny_param_t* atiny_params, void** phandle);
extern int mqtt_add_interest_topic(char *topic, cloud_qos_level_e qos, atiny_rsp_cb cb, char **topic_dup);
extern void atiny_free(void* ptr);
extern int mqtt_is_topic_subscribed(const char *topic);
extern int mqtt_del_interest_topic(const char *topic);
extern int mqtt_is_topic_subscribed_same(char *topic, cloud_qos_level_e qos, atiny_rsp_cb cb);
extern int mqtt_topic_subscribe(MQTTClient *client, char *topic, cloud_qos_level_e qos, atiny_rsp_cb cb);
extern int mqtt_topic_unsubscribe(MQTTClient *client, const char *topic);
extern void mqtt_message_arrived(MessageData *md);
extern void device_info_member_free(atiny_device_info_t* info);
extern void will_options_member_free(cloud_will_options_t *will_options);
extern void device_info_member_free(atiny_device_info_t *info);
extern int device_info_dup(atiny_device_info_t* dest, atiny_device_info_t* src);
extern void device_info_member_free(atiny_device_info_t* info);
extern int device_info_dup(atiny_device_info_t *dest, atiny_device_info_t *src);
extern int atiny_isconnceted(void *phandle);
extern int atiny_data_send(void *pandle, cloud_msg_t *send_data, atiny_rsp_cb cb);
extern int mqtt_message_publish(MQTTClient *client, cloud_msg_t *send_data);
extern int mqtt_subscribe_interest_topics(MQTTClient *client, atiny_interest_uri_t interest_uris[ATINY_INTEREST_URI_MAX_NUM]);
extern int mqtt_message_publish(MQTTClient *client, cloud_msg_t *send_data);
struct atiny_bind_para{
	atiny_device_info_t* device_info;
	void* phandle;
};
void * pthread_test(void *para)
{
    struct atiny_bind_para *bind_para;
    bind_para = (struct atiny_bind_para *)para;
	handle_data_t *tt = (handle_data_t *)bind_para->phandle;
	
	atiny_bind(bind_para->device_info,bind_para->phandle);
	printf("exit from pthread_test,security_type is %d,quit is %d\n",
		tt->atiny_params.security_type,tt->atiny_quit);
	
    return para;
}


int stub_NetworkConnect(Network* n, char* addr, int port)
{
    return 0;
}
int stub_NetworkConnect_fail(Network *n, char *addr, int port)
{
	return 1;
}


int stub_MQTTConnect(MQTTClient* c, MQTTPacket_connectData* options)
{
    return 0;
}

int stub_device_info_dup_fail(atiny_device_info_t* dest, atiny_device_info_t* src)
{
    return -1;
}
int stub_MQTTYield(MQTTClient* c, int timeout_ms)
{
    return 0;
}

int stub_mqtt_add_interest_topic(char *topic, cloud_qos_level_e qos, atiny_rsp_cb cb, char **topic_dup)
{
    return -1;
}


int sub_scr = 0;
int stub_MQTTSubscribe(MQTTClient* c, const char* topicFilter, enum QoS qos,
       messageHandler messageHandler)
{
    return sub_scr;
}

int unsub_scr = 0;

int stub_MQTTUnsubscribe(MQTTClient* c, const char* topicFilter)
{
    return unsub_scr;
}




}
/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
TestMQTT_Client::TestMQTT_Client()
{
    TEST_ADD(TestMQTT_Client::test_atiny_param_member_free);
    TEST_ADD(TestMQTT_Client::test_atiny_param_dup);
    TEST_ADD(TestMQTT_Client::test_atiny_init);

    TEST_ADD(TestMQTT_Client::test_atiny_deinit);
    TEST_ADD(TestMQTT_Client::test_mqtt_add_interest_topic);
    TEST_ADD(TestMQTT_Client::test_mqtt_is_topic_subscribed);
    TEST_ADD(TestMQTT_Client::test_mqtt_del_interest_topic);
    TEST_ADD(TestMQTT_Client::test_mqtt_is_topic_subscribed_same);
    TEST_ADD(TestMQTT_Client::test_mqtt_topic_subscribe);
    TEST_ADD(TestMQTT_Client::test_mqtt_topic_unsubscribe);
    TEST_ADD(TestMQTT_Client::test_mqtt_message_publish);
    TEST_ADD(TestMQTT_Client::test_mqtt_message_arrived);
    TEST_ADD(TestMQTT_Client::test_mqtt_subscribe_interest_topics);
	TEST_ADD(TestMQTT_Client::test_will_options_member_free);
	TEST_ADD(TestMQTT_Client::test_device_info_member_free);
	TEST_ADD(TestMQTT_Client::test_device_info_dup);
	TEST_ADD(TestMQTT_Client::test_atiny_isconnected);
	TEST_ADD(TestMQTT_Client::test_atiny_bind);
	TEST_ADD(TestMQTT_Client::test_atiny_data_send);

}
TestMQTT_Client::~TestMQTT_Client()
{

}

void fun_atiny_rsp_cb(cloud_msg_t *msg)
{
	return;
}

char *stub_atiny_strdup_fail_random(const char *ch)
{
    char * p;
    
    if(i++ < g_random)
    {
        cleanStub(&si_atiny_strdup);
        p = atiny_strdup(ch);
		
		setStub((void *)atiny_strdup, (void *)stub_atiny_strdup_fail_random, &si_atiny_strdup);

		return p;
    }
	
    
	return NULL;
}

void* stub_atiny_malloc_fail(size_t size)
{
    printf("call stub_atiny_malloc_fail\n");
    return NULL;
}


void TestMQTT_Client::test_atiny_param_member_free(void)
{
	int result = 0;
	atiny_param_member_free(NULL);
	TEST_ASSERT_MSG((result == 0), "atiny_param_member_free(...) failed");
	
	atiny_param_t *para_dest, test_dest;
	atiny_param_t *para_src, test_src;
	para_dest = &test_dest;
	para_src = &test_src;
	test_src.server_ip = (char *)"192.168.1.100";
	test_src.server_port = (char *)"6656";
	test_src.security_type = CLOUD_SECURITY_TYPE_PSK;
	test_src.u.psk.psk_id = (unsigned char *)("110");
	test_src.u.psk.psk_len = 10;
	test_src.u.psk.psk = (unsigned char *)("psk");
	atiny_param_dup(para_dest, para_src);
	test_dest.security_type = CLOUD_SECURITY_TYPE_PSK;
	atiny_param_member_free(para_dest);
	TEST_ASSERT_MSG((result == 0), "atiny_param_member_free(...) failed");
	
	test_src.security_type = CLOUD_SECURITY_TYPE_CA;
	test_src.u.ca.ca_crt = (char *)"ca_crt";
	test_src.u.ca.server_crt = (char *)"server_crt";
	test_src.u.ca.server_key = (char *)"server_key";
	atiny_param_dup(para_dest, para_src);
	test_dest.security_type = CLOUD_SECURITY_TYPE_CA;
	atiny_param_member_free(para_dest);
	TEST_ASSERT_MSG((result == 0), "atiny_param_member_free(...) failed");
	
}

void TestMQTT_Client::test_atiny_param_dup(void)
{
	int result;
	result = atiny_param_dup(NULL, NULL);
	TEST_ASSERT_MSG((result == -1), "atiny_param_dup(...) failed");
	
	atiny_param_t *para_dest, test_dest;
	atiny_param_t *para_src, test_src;
	para_dest = &test_dest;
	para_src = &test_src;
	test_src.server_ip = (char *)"192.168.1.100";
	test_src.server_port = (char *)"6656";
	test_src.security_type = CLOUD_SECURITY_TYPE_PSK;
	test_src.u.psk.psk_id = (unsigned char *)"110";
	test_src.u.psk.psk_len = 10;
	test_src.u.psk.psk = (unsigned char *)"psk";
	
    
	
    setStub((void *)atiny_strdup, (void *)stub_atiny_strdup_fail_random, &si_atiny_strdup);
    i = 0;
	g_random = 0;
	memset((uint8_t *)para_dest,0,sizeof(atiny_param_t));
	result = atiny_param_dup(para_dest, para_src);
	TEST_ASSERT_MSG((result == -1), "atiny_param_dup(...) failed");

    i = 0;
	g_random = 1;
	memset((uint8_t *)para_dest,0,sizeof(atiny_param_t));
	result = atiny_param_dup(para_dest, para_src);
	TEST_ASSERT_MSG((result == -1), "atiny_param_dup(...) failed");


	i = 0;
	g_random = 2;
	memset((uint8_t *)para_dest,0,sizeof(atiny_param_t));
	result = atiny_param_dup(para_dest, para_src);
	TEST_ASSERT_MSG((result == -1), "atiny_param_dup(...) failed");
    cleanStub(&si_atiny_strdup);

	
    

	
    memset((uint8_t *)para_dest,0,sizeof(atiny_param_t));
	result = atiny_param_dup(para_dest, para_src);
	TEST_ASSERT_MSG((result == 0), "atiny_param_dup(...) failed");
	test_dest.security_type = CLOUD_SECURITY_TYPE_PSK;
	atiny_param_member_free(para_dest);

    
	
	test_src.security_type = CLOUD_SECURITY_TYPE_CA;
	test_src.u.ca.ca_crt = (char *)"ca_crt";
	test_src.u.ca.server_crt = (char *)"server_crt";
	test_src.u.ca.server_key = (char *)"server_key";

	setStub((void *)atiny_strdup, (void *)stub_atiny_strdup_fail_random, &si_atiny_strdup);
    i = 0;
	g_random = 2;
	memset((uint8_t *)para_dest,0,sizeof(atiny_param_t));
	result = atiny_param_dup(para_dest, para_src);
	TEST_ASSERT_MSG((result == -1), "atiny_param_dup(...) failed");
    
	i = 0;
	g_random = 3;
	memset((uint8_t *)para_dest,0,sizeof(atiny_param_t));
	result = atiny_param_dup(para_dest, para_src);
	TEST_ASSERT_MSG((result == -1), "atiny_param_dup(...) failed");
    

	i = 0;
	g_random = 4;
	memset((uint8_t *)para_dest,0,sizeof(atiny_param_t));
	result = atiny_param_dup(para_dest, para_src);
	TEST_ASSERT_MSG((result == -1), "atiny_param_dup(...) failed");
    cleanStub(&si_atiny_strdup);
	
	memset((uint8_t *)para_dest,0,sizeof(atiny_param_t));
	result = atiny_param_dup(para_dest, para_src);
	TEST_ASSERT_MSG((result == 0), "atiny_param_dup(...) failed");
	test_dest.security_type = CLOUD_SECURITY_TYPE_CA;
	atiny_param_member_free(para_dest);


	test_src.security_type = CLOUD_SECURITY_TYPE_MAX;
	test_src.u.ca.ca_crt = (char *)"ca_crt";
	test_src.u.ca.server_crt = (char *)"server_crt";
	test_src.u.ca.server_key = (char *)"server_key";

	memset((uint8_t *)para_dest,0,sizeof(atiny_param_t));
	result = atiny_param_dup(para_dest, para_src);
	TEST_ASSERT_MSG((result == 0), "atiny_param_dup(...) failed");
    atiny_param_member_free(para_dest);


	
}


void TestMQTT_Client::test_atiny_init(void)
{
	int result;
	result = atiny_init(NULL, NULL);
	TEST_ASSERT_MSG((ATINY_ARG_INVALID == result), "atiny_init(...) failed");

	atiny_param_t test_atiny_params;
    memset(&test_atiny_params,0,sizeof(atiny_param_t));
	
	handle_data_t *test_g_p;
	test_atiny_params.server_ip = (char *)"192.168.1.100";
	test_atiny_params.server_port = (char *)"6556";
	test_atiny_params.security_type = CLOUD_SECURITY_TYPE_PSK;
	test_atiny_params.u.psk.psk_id = (unsigned char *)"1234";
	test_atiny_params.u.psk.psk_len = 5;
	test_atiny_params.u.psk.psk = (unsigned char *)"abcde";

	result = atiny_init(&test_atiny_params, (void **)&test_g_p);
	TEST_ASSERT_MSG((result == ATINY_OK), "atiny_init(...) failed");

    test_g_p->atiny_quit = 1;
	atiny_deinit(test_g_p);
	
	
	memset(&test_atiny_params,0,sizeof(atiny_param_t));
    test_atiny_params.server_ip = (char *)"192.168.1.100";
	test_atiny_params.server_port = (char *)"6556";
	
	test_atiny_params.security_type = CLOUD_SECURITY_TYPE_CA;
	test_atiny_params.u.ca.ca_crt = (char *)"asdf";
	test_atiny_params.u.ca.server_crt = (char *)"zxcv";
	test_atiny_params.u.ca.server_key = (char *)"qwer";
	printf("start init...server_ip is %s,port is %s\n",
		test_atiny_params.server_ip,test_atiny_params.server_port);
	result = atiny_init(&test_atiny_params, (void **)&test_g_p);
	TEST_ASSERT_MSG((result == ATINY_OK), "atiny_init(...) failed");
	printf("start deinit...,client_id is %p\n",test_g_p->device_info.client_id);
    test_g_p->atiny_quit = 1;
	atiny_deinit(test_g_p);


    setStub((void *)atiny_strdup, (void *)stub_atiny_strdup_fail_random, &si_atiny_strdup);
    i = 0;
	g_random = 0;

	result = atiny_init(&test_atiny_params, (void **)&test_g_p);
    TEST_ASSERT_MSG((result == ATINY_MALLOC_FAILED), "atiny_init(...) failed");
	
    cleanStub(&si_atiny_strdup);
	
	printf("exit from test_atiny_init\n");
	
}

void TestMQTT_Client::test_atiny_deinit(void)
{
	
	
	/*test in test_atiny_init and test_atiny_bind*/


}



void TestMQTT_Client::test_mqtt_add_interest_topic(void)
{
	int result;
	cloud_qos_level_e test_qos = CLOUD_QOS_LEAST_ONCE;
	result = mqtt_add_interest_topic(NULL, test_qos, NULL, NULL);
	TEST_ASSERT_MSG((result == -1), "mqtt_add_interest_topic(...) failed");
	
 
	char *para_topic_dup = NULL;

	result = mqtt_add_interest_topic((char *)"www.baidu.com", test_qos, message_test, &para_topic_dup);
	TEST_ASSERT_MSG((result == 0), "mqtt_add_interest_topic(...) failed");
	
	result = mqtt_add_interest_topic((char *)"www.baidu.com", test_qos, message_test, &para_topic_dup);
	TEST_ASSERT_MSG((result == 0), "mqtt_add_interest_topic(...) failed");
	mqtt_del_interest_topic("www.baidu.com");


    setStub((void*)atiny_strdup, (void*)stub_atiny_strdup_fail_random, &si_atiny_strdup);
	i=0;
	g_random=0;

	result = mqtt_add_interest_topic((char *)"www.baidu.com", test_qos, message_test, &para_topic_dup);
	TEST_ASSERT_MSG((result == ATINY_MALLOC_FAILED), "mqtt_add_interest_topic(...) failed");
	mqtt_del_interest_topic("www.baidu.com");

	cleanStub(&si_atiny_strdup);


	
}

void TestMQTT_Client::test_mqtt_is_topic_subscribed()
 {
	int result;
	result = mqtt_is_topic_subscribed(NULL);
	TEST_ASSERT_MSG((result == -1), "mqtt_is_topic_subscribed(...) failed");
	
	atiny_rsp_cb test_atiny_rsp_cb;
	cloud_qos_level_e test_qos = CLOUD_QOS_LEAST_ONCE;
	char *para_topic_dup = NULL;
	test_atiny_rsp_cb = fun_atiny_rsp_cb;
	result = mqtt_add_interest_topic((char *)"www.baidu.com", test_qos, test_atiny_rsp_cb, &para_topic_dup);
	result = mqtt_is_topic_subscribed((const char *)"www.baidu.com");
	TEST_ASSERT_MSG((result == MQTT_TOPIC_SUBSCRIBED_TRUE), "mqtt_is_topic_subscribed(...) failed");
	mqtt_del_interest_topic("www.baidu.com");
}

void TestMQTT_Client::test_mqtt_del_interest_topic()
{
	int result;
	result = mqtt_del_interest_topic(NULL);
	TEST_ASSERT_MSG((result == -1), "mqtt_is_topic_subscribed(...) failed");
	
	cloud_qos_level_e test_qos = CLOUD_QOS_LEAST_ONCE;
	atiny_rsp_cb test_atiny_rsp_cb;
	char *para_topic_dup = NULL;
	test_atiny_rsp_cb = fun_atiny_rsp_cb;
	mqtt_add_interest_topic((char *)"www.baidu.com", test_qos, test_atiny_rsp_cb, &para_topic_dup);
	result = mqtt_del_interest_topic((char *)"www.baidu.com");
	TEST_ASSERT_MSG((result == 0), "mqtt_is_topic_subscribed(...) failed");
}

void TestMQTT_Client::test_mqtt_is_topic_subscribed_same()
{
		int result;
		result = mqtt_is_topic_subscribed_same(NULL, CLOUD_QOS_LEAST_ONCE, NULL);
		TEST_ASSERT_MSG((result == -1), "mqtt_is_topic_subscribed_same(...) failed");
		
		cloud_qos_level_e test_qos = CLOUD_QOS_LEAST_ONCE;
		atiny_rsp_cb test_atiny_rsp_cb;
		char *para_topic_dup = NULL;
		test_atiny_rsp_cb = fun_atiny_rsp_cb;
		mqtt_add_interest_topic((char *)"www.baidu.com", test_qos, test_atiny_rsp_cb, &para_topic_dup);
		
		result = mqtt_is_topic_subscribed_same((char *)"www.baidu.com", test_qos, test_atiny_rsp_cb);
		TEST_ASSERT_MSG((result == MQTT_TOPIC_SUBSCRIBED_TRUE), "mqtt_is_topic_subscribed_same(...) failed");
		
		mqtt_del_interest_topic((char *)"www.baidu.com");
}

void TestMQTT_Client::test_mqtt_topic_subscribe()
{
	int result;
	uint8_t buf[512] = {0};
	result = mqtt_topic_subscribe(NULL, NULL, CLOUD_QOS_MOST_ONCE, NULL);
	TEST_ASSERT_MSG((result == -1), "mqtt_topic_subscribe(...) failed");
	
	MQTTClient test_client;
	memset(&test_client, 0, sizeof(MQTTClient));
	cloud_qos_level_e test_qos = CLOUD_QOS_LEAST_ONCE;
	atiny_rsp_cb test_atiny_rsp_cb;
	char *para_topic_dup = NULL;
	
	test_atiny_rsp_cb = fun_atiny_rsp_cb;
	mqtt_add_interest_topic((char *)"www.baidu.com", test_qos, test_atiny_rsp_cb, &para_topic_dup);
	result = mqtt_topic_subscribe(&test_client, (char *)"www.baidu.com", test_qos, test_atiny_rsp_cb);
	TEST_ASSERT_MSG((result == 0), "mqtt_topic_subscribe(...) failed");

	test_client.isconnected = 0;
	test_client.mutex = NULL;
	result = mqtt_topic_subscribe(&test_client, (char *)"www.yunzhijia.com", test_qos, test_atiny_rsp_cb);
	TEST_ASSERT_MSG((result == FAILURE), "mqtt_topic_subsrcibe(...) failed");
       
    stubInfo si_mqtt_add_interest_topic;
    setStub((void*)mqtt_add_interest_topic, (void*)stub_mqtt_add_interest_topic, &si_mqtt_add_interest_topic);

	
	test_client.isconnected = 0;
	test_client.mutex = (void *)buf;
    sub_scr = 0;
	result = mqtt_topic_subscribe(&test_client, (char *)"www.yunzhijia.com", test_qos, test_atiny_rsp_cb);
	TEST_ASSERT_MSG((result == FAILURE), "mqtt_topic_subsrcibe(...) failed");
	cleanStub(&si_mqtt_add_interest_topic);
	
	printf("start....,%s\n",__FUNCTION__);
	stubInfo si_MQTTSubscribe;
    setStub((void*)MQTTSubscribe, (void*)stub_MQTTSubscribe, &si_MQTTSubscribe);

	stubInfo si_MQTTUnsubscribe;
    setStub((void*)MQTTUnsubscribe, (void*)stub_MQTTUnsubscribe, &si_MQTTUnsubscribe);
	
	sub_scr = -1;
	result = mqtt_topic_subscribe(&test_client, (char *)"www.yunzhijia.com", test_qos, test_atiny_rsp_cb);
	TEST_ASSERT_MSG((result == sub_scr), "mqtt_topic_subsrcibe(...) failed");

	sub_scr = 0;
	result = mqtt_topic_subscribe(&test_client, (char *)"www.yunzhijia.com", test_qos, test_atiny_rsp_cb);
	TEST_ASSERT_MSG((result == sub_scr), "mqtt_topic_subsrcibe(...) failed");


	
    result = mqtt_topic_unsubscribe(NULL,NULL);
	TEST_ASSERT_MSG((result == -1), "mqtt_topic_unsubscribe(...) failed");
    unsub_scr = -1;
	result = mqtt_topic_unsubscribe(&test_client,(char *)"www.yunzhijia.com");
	TEST_ASSERT_MSG((result == unsub_scr), "mqtt_topic_unsubscribe(...) failed");

	unsub_scr = 0;
	result = mqtt_topic_unsubscribe(&test_client,(char *)"www.bbbbbb.com");
	TEST_ASSERT_MSG((result == MQTT_TOPIC_SUBSCRIBED_FALSE), "mqtt_topic_unsubscribe(...) failed");
	 
	unsub_scr = 0;
	result = mqtt_topic_unsubscribe(&test_client,(char *)"www.yunzhijia.com");
	TEST_ASSERT_MSG((result == unsub_scr), "mqtt_topic_unsubscribe(...) failed");
    
	
	
    cleanStub(&si_MQTTSubscribe);
	cleanStub(&si_MQTTUnsubscribe);
	
	
	
	
	mqtt_del_interest_topic((char *)"www.baidu.com");
	

}

void TestMQTT_Client::test_mqtt_topic_unsubscribe()
{
	/*test in test_mqtt_topic_subscribe*/
}

void TestMQTT_Client::test_mqtt_message_publish()
{
	int result;
	result = mqtt_message_publish(NULL, NULL);
	TEST_ASSERT_MSG((result == -1), "mqtt_message_public(...) failed");
	
	MQTTClient *para_client = NULL, test_client;
	cloud_msg_t *para_senddata = NULL, test_senddata;
	memset(&test_client, 0, sizeof(MQTTClient));
	memset(&test_senddata, 0, sizeof(cloud_msg_t));
	para_client = &test_client;
	para_senddata = &test_senddata;
	test_senddata.qos = CLOUD_QOS_LEAST_ONCE;
	test_senddata.payload = (void *)"hello_world";
	test_senddata.payload_len = 15;
	test_client.isconnected = 0;
	test_client.cleansession = 0;
	test_client.mutex = 0;
	result = mqtt_message_publish(para_client, para_senddata);
	TEST_ASSERT_MSG((result == FAILURE), "mqtt_message_public(...) failed");
	
	//stubInfo si_write;
	//setStub((void *)linux_write,(void *)stub_linux_write,&si_write);
	//cleanStub(&si_write);
	
	
}

void TestMQTT_Client::test_mqtt_message_arrived()
{
	int result = 0;
	mqtt_message_arrived(NULL);
	TEST_ASSERT_MSG((result == 0), "mqtt_message_public(...) failed");
	
	MessageData *para_messagedata = NULL, test_messagedata;
	memset(&test_messagedata, 0, sizeof(MessageData));
	para_messagedata = &test_messagedata;
	test_messagedata.topic_sub = NULL;
	mqtt_message_arrived(para_messagedata);
	TEST_ASSERT_MSG((result == 0), "mqtt_message_public(...) failed");
	
	atiny_rsp_cb test_atiny_rsp_cb = NULL;
	cloud_qos_level_e test_qos = CLOUD_QOS_LEAST_ONCE;
	char **para_topic_dup, *test_topic;
	para_topic_dup = &test_topic;
	test_atiny_rsp_cb = fun_atiny_rsp_cb;
	mqtt_add_interest_topic((char *)"www.baidu.com", test_qos, test_atiny_rsp_cb, para_topic_dup);
	MQTTMessage test_mqttmessage;
	MQTTString test_mqttstring;
	memset(&test_mqttmessage, 0, sizeof(MQTTMessage));
	memset(&test_mqttstring, 0, sizeof(MQTTString));
	test_messagedata.message = &test_mqttmessage;
	test_messagedata.topicName = &test_mqttstring;
	test_messagedata.topic_sub = (char *)"www.baidu.com";
	test_messagedata.topicName->cstring = (char *)"csing";
	test_mqttstring.lenstring.data = (char *)"hello_world";
	test_mqttmessage.payload = (char *)"payload";
	mqtt_message_arrived(para_messagedata);
	TEST_ASSERT_MSG((result == 0), "mqtt_message_public(...) failed");
	
	test_messagedata.topicName->cstring = NULL;
	mqtt_message_arrived(para_messagedata);
	TEST_ASSERT_MSG((result == 0), "mqtt_message_public(...) failed");
	mqtt_del_interest_topic((char *)"www.baidu.com");
}

void TestMQTT_Client::test_mqtt_subscribe_interest_topics()
{
	int result;
	int i = 0;
	result = mqtt_subscribe_interest_topics(NULL, NULL);
	TEST_ASSERT_MSG((result == ATINY_ARG_INVALID), "mqtt_subscribe_interest_topics(...) failed");
	
	atiny_interest_uri_t test_interest[ATINY_INTEREST_URI_MAX_NUM];
	for(i=0; i< ATINY_INTEREST_URI_MAX_NUM; i++)
		memset(&test_interest[i], 0, sizeof(atiny_interest_uri_t));
	MQTTClient *para_client, test_client;
	memset(&test_client, 0, sizeof(MQTTClient));
	para_client = &test_client;
	result = mqtt_subscribe_interest_topics(para_client, test_interest);
	TEST_ASSERT_MSG((result == ATINY_ARG_INVALID), "mqtt_subscribe_interest_topics(...) failed");
	
	test_interest[0].uri = (char *)"www.baidu.com";
	test_interest[0].cb = fun_atiny_rsp_cb;
	test_interest[0].qos = CLOUD_QOS_ONLY_ONCE;
	test_client.isconnected = 0;
	test_client.cleansession = 0;
	test_client.ping_outstanding = 0;
	test_client.mutex = 0;
	result = mqtt_subscribe_interest_topics(para_client, test_interest);
	TEST_ASSERT_MSG((result = ATINY_SOCKET_ERROR), "mqtt_subsrcibe_interest_topic(...) failed");
}

void TestMQTT_Client::test_will_options_member_free()
{
	int result = 0;
	will_options_member_free(NULL);
	TEST_ASSERT_MSG((result == 0), "will_options_member_free(...) failed");
	
	cloud_will_options_t *para_willoptions, test_willoptions;
	memset(&test_willoptions, 0, sizeof(cloud_will_options_t));
	para_willoptions = &test_willoptions;
	test_willoptions.topic_name = NULL;
	test_willoptions.topic_msg = NULL;
	will_options_member_free(para_willoptions);
	TEST_ASSERT_MSG((result == 0), "will_options_member_free(...) failed");
	
	char test_topicname[20] = "hello_world";
	char test_topicmsg[20] = "ddddddd";
	test_willoptions.topic_name = atiny_strdup(test_topicname);
	test_willoptions.topic_msg = atiny_strdup(test_topicmsg);
	will_options_member_free(para_willoptions);
	TEST_ASSERT_MSG((result == 0), "will_options_member_free(...) failed");
	
}

void TestMQTT_Client::test_device_info_member_free()
{
	int result = 0;
	int i = 0;
	device_info_member_free(NULL);
	TEST_ASSERT_MSG((result == 0), "device_info_member_free(...) failed");
	
	atiny_device_info_t *para_dest, test_dest;
	atiny_device_info_t *para_src, test_src;
	test_src.client_id = NULL;
	test_src.user_name = NULL;
	test_src.password = NULL;
	test_src.will_flag = MQTT_WILL_FLAG_FALSE;
	test_src.will_options = NULL; 
	for(i=0; i<ATINY_INTEREST_URI_MAX_NUM; i++)
	test_src.interest_uris[i].uri = NULL;
	device_info_member_free(&test_src);
	TEST_ASSERT_MSG((result == 0), "device_info_member_free(...) failed");
	
	memset(&test_src, 0, sizeof(atiny_device_info_t));
	memset(&test_dest, 0, sizeof(atiny_device_info_t));
	cloud_will_options_t test_willoptions;
	test_willoptions.topic_name = (char *)"topic_name";
	test_willoptions.topic_msg = (char *)"topic_msg";
	test_willoptions.retained = 'q';
	test_willoptions.qos = CLOUD_QOS_LEAST_ONCE;
	test_src.client_id = (char *)"123";
	test_src.user_name = (char *)"name";
	test_src.password = (char *)"password";
	test_src.will_flag = MQTT_WILL_FLAG_TRUE;
	test_src.will_options = &test_willoptions;
	
	printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
	
	atiny_rsp_cb test_atiny_rsp_cb;
	cloud_qos_level_e test_qos = CLOUD_QOS_LEAST_ONCE;
	char **para_topic_dup, *para_topic;
	para_topic_dup = &para_topic;
	test_atiny_rsp_cb = fun_atiny_rsp_cb;
	mqtt_add_interest_topic((char *)"www.baidu.com", test_qos, test_atiny_rsp_cb, para_topic_dup);
	
	para_src = &test_src;
	para_dest = &test_dest;
	para_src->interest_uris[0].qos = test_qos;
	para_src->interest_uris[0].uri = *para_topic_dup;
	para_src->interest_uris[0].cb = fun_atiny_rsp_cb;
	
	for(int i=0; i<ATINY_INTEREST_URI_MAX_NUM; i++)
    {
		//printf("*********** %p\n",test_src.interest_uris[i].uri);
    }
	#if 1
	result = device_info_dup(para_dest, para_src);
	
	#endif
	
	device_info_member_free(para_dest);
	TEST_ASSERT_MSG((result == 0), "device_info_member_free(...) failed");
	mqtt_del_interest_topic((char *)"www.baidu.com");

}

void TestMQTT_Client::test_device_info_dup()
{

	int result = 0;
	result = device_info_dup(NULL, NULL);
	TEST_ASSERT_MSG((result == -1), "device_info_dup(...) failed");
	
	atiny_device_info_t test_src,test_dest;
	memset(&test_src, 0, sizeof(atiny_device_info_t));
	memset(&test_dest, 0, sizeof(atiny_device_info_t));
	
	cloud_will_options_t test_willoptions;
	memset(&test_willoptions, 0, sizeof(cloud_will_options_t));
	test_willoptions.topic_name = (char *)"topic_name";
	test_willoptions.topic_msg = (char *)"topic_msg";
	test_willoptions.retained = 'q';
	test_willoptions.qos = CLOUD_QOS_LEAST_ONCE;
	test_src.client_id = (char *)"123";
	test_src.user_name = (char *)"name";
	test_src.password = (char *)"password";
	test_src.will_flag = MQTT_WILL_FLAG_TRUE;
	test_src.will_options = &test_willoptions;
	
	 
	
	cloud_qos_level_e test_qos = CLOUD_QOS_LEAST_ONCE;



	test_src.interest_uris[0].qos = CLOUD_QOS_LEAST_ONCE;
	test_src.interest_uris[0].uri = "www.baidu.com";
	test_src.interest_uris[0].cb = fun_atiny_rsp_cb;
	
 
	result = device_info_dup(&test_dest, &test_src);
	 

	 
	TEST_ASSERT_MSG((result == 0), "device_info_dup(...) failed");
	
	device_info_member_free(&test_dest);
	

}

void TestMQTT_Client::test_atiny_isconnected()
{
	int result;
	result = atiny_isconnected(NULL);
	TEST_ASSERT_MSG((result == ATINY_ARG_INVALID), "atiny_isconnected(...) failed");
	
	handle_data_t *para_phandle, test_phandle;
	memset(&test_phandle, 0, sizeof(test_phandle));
	test_phandle.client.isconnected = 1;
	para_phandle = &test_phandle;
	result = atiny_isconnected(para_phandle);
	TEST_ASSERT_MSG((result == 1), "atiny_isconnected(...) failed");
}


void TestMQTT_Client::test_atiny_bind()
{


	int result;
    atiny_param_t test_atiny_params;
	
	
	handle_data_t *test_g_p;
	test_atiny_params.server_ip = (char *)"192.168.1.100";
	test_atiny_params.server_port = (char *)"6556";
	test_atiny_params.security_type = CLOUD_SECURITY_TYPE_PSK;
	test_atiny_params.u.psk.psk_id = (unsigned char *)"1234";
	test_atiny_params.u.psk.psk_len = 5;
	test_atiny_params.u.psk.psk = (unsigned char *)"abcde";
	result = atiny_init(&test_atiny_params, (void **)&test_g_p);
	
	printf("security_type  is %d in here test_atiny_bind\n",test_g_p->atiny_params.security_type);

	printf("test_g_p:::%d,%s,%s\n",
		test_g_p->atiny_params.security_type,
		test_g_p->atiny_params.server_ip,
		test_g_p->atiny_params.server_port);
	
	result = atiny_bind(NULL, NULL);
	TEST_ASSERT_MSG((result == ATINY_ARG_INVALID), "atiny_bind(...) failed");
	
	atiny_device_info_t test_device_info;
	memset(&test_device_info, 0, sizeof(atiny_device_info_t));
	test_device_info.client_id = NULL;
	result = atiny_bind(&test_device_info, test_g_p);
	TEST_ASSERT_MSG((result == ATINY_ARG_INVALID), "atiny_bind(...) failed");
	
	test_device_info.client_id = (char *)"123";
	test_device_info.will_flag = MQTT_WILL_FLAG_TRUE;
	test_device_info.will_options = NULL;
	result = atiny_bind(&test_device_info, test_g_p);
	TEST_ASSERT_MSG((result == ATINY_ARG_INVALID), "atiny_bind(...) failed");
    stubInfo si_device_info_dup;
    setStub((void*)device_info_dup, (void*)stub_device_info_dup_fail, &si_device_info_dup);
	
	test_device_info.will_flag = MQTT_WILL_FLAG_FALSE;
	result = atiny_bind(&test_device_info, test_g_p);
	TEST_ASSERT_MSG((result == ATINY_MALLOC_FAILED), "atiny_bind(...) failed");
	cleanStub(&si_device_info_dup);
    
#if 1
    test_g_p->bind_quit = 0;
    result = atiny_init(&test_atiny_params, (void **)&test_g_p);

	cloud_will_options_t test_cloud;
	memset(&test_cloud, 0, sizeof(cloud_will_options_t));
	
	
	test_cloud.topic_name = (char *)"www.baidu.com";
	test_cloud.topic_msg = (char *)"hello_world";
	test_cloud.qos = CLOUD_QOS_LEAST_ONCE;
	test_device_info.will_options = &test_cloud;
	test_device_info.client_id = (char *)"123";
	test_device_info.user_name = (char *)"name";
	test_device_info.password = (char *)"password";
	test_device_info.will_flag = MQTT_WILL_FLAG_FALSE;
	for(int i=0; i<ATINY_INTEREST_URI_MAX_NUM; i++)
		memset(&test_device_info.interest_uris[i], 0, sizeof(atiny_interest_uri_t));

	memcpy(test_device_info.interest_uris, g_interest_uris_test, sizeof(g_interest_uris_test));

	
	
	int ret = 0;
    pthread_t first_thread;
	
    struct atiny_bind_para bind_para;
	bind_para.device_info = &test_device_info;
	bind_para.phandle     = test_g_p;
	printf("test_g_p->atiny_params:%d,%s,%s\n",
		test_g_p->atiny_params.security_type,
		test_g_p->atiny_params.server_ip,
		test_g_p->atiny_params.server_port);
	
	test_g_p->atiny_quit  = 0;
	stubInfo si_NetworkConnect;
    setStub((void*)NetworkConnect, (void*)stub_NetworkConnect, &si_NetworkConnect);
    stubInfo si_MQTTConnect;
    setStub((void*)MQTTConnect, (void*)stub_MQTTConnect, &si_MQTTConnect);
	
	stubInfo si_MQTTYield;
    setStub((void*)MQTTYield, (void*)stub_MQTTYield, &si_MQTTYield);
	
    ret = pthread_create(&first_thread, NULL, pthread_test, (void *)&bind_para);
    TEST_ASSERT_MSG((0 == ret), "atiny_bind(...) failed");

    usleep(10000);
	test_g_p->atiny_quit = 1;
	usleep(10000);
    pthread_cancel(first_thread);

    pthread_join(first_thread, NULL); 

	atiny_deinit(test_g_p);

	cleanStub(&si_MQTTYield);
    cleanStub(&si_MQTTConnect);
	cleanStub(&si_NetworkConnect);
#endif
}
void TestMQTT_Client::test_atiny_data_send()
{
	int result;
	unsigned char test_buf[170];
	atiny_rsp_cb test_cb = fun_atiny_rsp_cb;
	result = atiny_data_send(NULL, NULL, test_cb);
	TEST_ASSERT_MSG((result == ATINY_ARG_INVALID), "atiny_data_send(...) failed");
	
	atiny_interest_uri_t *para_atiny_interest_uri, test_atiny_interest_uri;
	memset(&test_atiny_interest_uri, 0, sizeof(atiny_interest_uri_t));
	handle_data_t *para_handle, test_handle;
	cloud_msg_t test_send_data;
	memset(&test_handle, 0, sizeof(handle_data_t));
	memset(&test_send_data, 0, sizeof(cloud_msg_t));
	para_handle = &test_handle;
	
	test_send_data.method = CLOUD_METHOD_GET;
	test_send_data.uri = (char *)"www.baidu.com";
	test_send_data.qos = CLOUD_QOS_LEAST_ONCE;
	MQTTClient test_client;
	memset(&test_client,0,sizeof(MQTTClient));
	
	test_client.isconnected = 1;
	test_handle.client = test_client;
	
	//test_handle.client.isconnected = 1;
	cloud_qos_level_e test_qos = CLOUD_QOS_LEAST_ONCE;
	
	char *para_topic = NULL;
	
	mqtt_add_interest_topic((char *)"www.baidu.com", test_qos, fun_atiny_rsp_cb, &para_topic);
	printf("add successfully\n");
	result = atiny_data_send(&test_handle, &test_send_data, fun_atiny_rsp_cb);
	TEST_ASSERT_MSG((result == 0), "atiny_data_send(...) failed");
	mqtt_del_interest_topic((char *)"www.baidu.com");
	
	
	test_send_data.method = CLOUD_METHOD_POST;
	//test_send_data.qos = CLOUD_QOS_MOST_ONCE;
	memset(&test_client, 0, sizeof(MQTTClient));
	test_send_data.qos = CLOUD_QOS_LEAST_ONCE;
	test_send_data.payload = (void *)"hello_world";
	test_send_data.payload_len = 10;
	test_client.isconnected = 1;
	test_client.cleansession = 0;
	test_client.mutex = 0;
	test_handle.client = test_client;
	result = atiny_data_send(para_handle, &test_send_data, fun_atiny_rsp_cb);
	TEST_ASSERT_MSG((result == -1), "mqtt_message_public(...) failed");
	
	test_send_data.payload_len = 128;
	test_handle.client = test_client;
	printf("test_handle.client.isconnected is %d\n", test_handle.client.isconnected);
	printf("test_handle.client.cleansession is %d\n", test_handle.client.cleansession);
	result = atiny_data_send(para_handle, &test_send_data, fun_atiny_rsp_cb);
	TEST_ASSERT_MSG((result == -1), "mqtt_message_public(...) failed");

	
	test_send_data.payload_len = 16384;
	test_handle.client = test_client;
	result = atiny_data_send(para_handle, &test_send_data, fun_atiny_rsp_cb);
	TEST_ASSERT_MSG((result == -1), "mqtt_message_public(...) failed");
	
	test_send_data.payload_len = 2097151;
	test_handle.client = test_client;
	result = atiny_data_send(para_handle, &test_send_data, fun_atiny_rsp_cb);
	TEST_ASSERT_MSG((result == -1), "mqtt_message_public(...) failed");
	
	
	test_send_data.method = CLOUD_METHOD_DEL;
	test_client.mutex = NULL;
	mqtt_add_interest_topic((char *)"www.baidu.com", test_qos, fun_atiny_rsp_cb, &para_topic);
	test_client.buf = test_buf;
	test_client.buf_size = 5;
	test_client.next_packetid == 170;
	test_client.cleansession = 0;
	test_handle.client = test_client;
	//stubInfo si_write;
	//setStub((void *)los_write,(void *)stub_los_write,&si_write);
	result = atiny_data_send(para_handle, &test_send_data, fun_atiny_rsp_cb);
	TEST_ASSERT_MSG((result == -1), "atiny_data_send(...) failed");
	mqtt_del_interest_topic((char *)"www.baidu.com");
}
void TestMQTT_Client::setup()
{
    printf("come into %d func, in file %s\n", ++funcno, __FILE__);
}


void TestMQTT_Client::tear_down()
{
    printf("exit %d func, in file %s\n", funcno, __FILE__);
}

