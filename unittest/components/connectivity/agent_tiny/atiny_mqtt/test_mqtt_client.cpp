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
	#include "atiny_mqtt/mqtt_client.h"
	#include "flash_manager.h"
	#include "cJSON.h"
	#include "log/atiny_log.h"
	#include "MQTTClient.h"
	#include "MQTTPacket.h"
	#include "MQTTliteos.h"
	#include "hmac.h"
	
	#include "mqtt_client.h"
	
	#define MQTT_VERSION_3_1 (3)
	#define MQTT_VERSION_3_1_1 (4)

	#define VARIABLE_SIZE (4 + 1)
	#define CMD_TOPIC_FMT "/huawei/v1/devices/%s/command/%s"
	#define DATA_TOPIC_FMT "/huawei/v1/devices/%s/data/%s"
	#define SECRET_NOTIFY_TOPIC_FMT "/huawei/v1/products/%s/sn/%s/secretNotify"
	#define SECRET_ACK_TOPIC_FMT "/huawei/v1/products/%s/sn/%s/secretACK"
	

	typedef struct
	{
	    void (*init)(void);
	    int (*write_flash_info)(const void *buffer, uint32_t len);
	    int (*read_flash_info)(void *buffer, uint32_t len);
	}demo_param_s;

	static demo_param_s g_demo_param;
	
	static int g_random = 0;

	static int g_malloc_random = 0;

	static int i = 0;
	
	static int j = 0;
	

	static int funcno = 0;

	static mqtt_client_s *test_phandle = NULL;

	static mqtt_client_s *test_gphandle = NULL;

	static unsigned char *test_readbuf=NULL;

	static cJSON *jsp1=NULL;
	static cJSON *jsp2=NULL;
	static cJSON *jsp3=NULL;
	
	
	static cJSON js;
	stubInfo si_atiny_malloc;

	stubInfo si_atiny_strdup;

	stubInfo si_flash_manager_write;

	
	typedef enum
	{
    	MQTT_CONNECT_WITH_PRODUCT_ID,
    	MQTT_CONNECT_WITH_DEVICE_ID
	}MQTT_DYNAMIC_CONNECT_STATE;

	typedef struct
	{
	    mqtt_static_connection_info_s save_info;
	    char *  got_passward;
	    MQTT_DYNAMIC_CONNECT_STATE state;
	    uint8_t connection_update_flag;
	    uint8_t has_device_id; //
	    uint8_t reserve[2];
	}mqtt_dynamic_info_s;



	struct mqtt_client_tag_s
	{
	    mqtt_device_info_s device_info;
	    MQTTClient client;
	    mqtt_param_s params;
	    mqtt_dynamic_info_s dynamic_info;
	    char *sub_topic;
	    //uint8_t atiny_quit;
	  //  uint8_t bind_quit;
		uint8_t init_flag;
	    uint8_t reserve[3];
	};

	typedef struct mqtt_client_tag_s mqtt_client_s;
	static mqtt_client_s g_mqtthandle;

	struct atiny_mqtt_bind_t{
	mqtt_device_info_s* device_info;
	mqtt_client_s* handle;
	};

	extern int atiny_mqtt_init(const mqtt_param_s *params, mqtt_client_s **phandle);
	//extern void atiny_deinit(void *phandle);
	extern void atiny_mqtt_deinit(mqtt_client_s *handle);
	extern void atiny_free(void* ptr);
	extern int atiny_mqtt_bind(const mqtt_device_info_s* device_info, mqtt_client_s* handle);
	extern int atiny_mqtt_isconnected(mqtt_client_s* phandle);
	extern int atiny_mqtt_data_send(mqtt_client_s *phandle, const char *msg,  uint32_t msg_len, mqtt_qos_e qos);



	extern int flash_adaptor_write_mqtt_info(const void *buffer, uint32_t len);
	extern int flash_adaptor_read_mqtt_info(void *buffer, uint32_t len);
	extern int mbedtls_hmac_calc(mbedtls_hmac_t *hmac_info);


	extern void hieth_hw_init(void);
	extern void net_init(void);


	extern void flash_manager_init(int (*cmd_ioctl)(mqtt_cmd_e cmd, void *arg, int32_t len));
	
	extern int flash_manager_write(const flash_info_s *flash_info);

	extern void flash_adaptor_init(void);

	extern int MQTTConnect(MQTTClient *c, MQTTPacket_connectData *options);
	extern int MQTTSubscribe(MQTTClient *c, const char *topicFilter, enum QoS qos,
                  messageHandler messageHandler);
	extern int MQTTYield(MQTTClient *c, int timeout_ms);
	extern int stub_MQTTPublishWithoutMutex(MQTTClient *c, const char *topicName, MQTTMessage *message);

	extern void NetworkDisconnect(Network *n);
	extern int MQTTSetMessageHandler(MQTTClient* c, const char* topicFilter, messageHandler messageHandler);
	
	int cmd_ioctl_exam(mqtt_cmd_e cmd, void *arg, int32_t len)
	{
		return 0;
	}

	static int demo_get_time(char *time, int32_t len)
	{
	    const int32_t min_time_len = 11;
	    if ((time == NULL) || (len < min_time_len))
	    {
	        ATINY_LOG(LOG_ERR,"invalid param len %d", len);
	        return ATINY_ERR;
	    }
	    (void)strncpy(time, "2018111517", len);
	    return ATINY_OK;
	}

	static int demo_rcv_msg(const void *msg, int32_t len)
	{
	    char* print_msg;
	    if ((msg == NULL) && len > 0)
	    {
	        ATINY_LOG(LOG_ERR, "invalid param len %ld", len);
	        return ATINY_ERR;
	    }
	    if (msg == NULL)
	    {
	        msg = (const uint8_t *)"";
	    }

	    print_msg = (char *)atiny_malloc(len + 1);
	    if (print_msg == NULL)
	    {
	        ATINY_LOG(LOG_ERR, "atiny_malloc null %ld", len);
	        return ATINY_ERR;
	    }
	    if (len > 0)
	    {
	        memcpy(print_msg, msg, len);
	    }
	    print_msg[len] = '\0';
	    ATINY_LOG(LOG_INFO, "rcv msg: len = %ld, msg:%s", len, (char *)msg);
	    return ATINY_OK;
	}



	
	static int test_cmd_ioctl(mqtt_cmd_e cmd, void *arg, int32_t len)
	{
    	int result = ATINY_ERR;
		//TODO:
    	switch(cmd)
        {
	        case MQTT_GET_TIME:
	            result = demo_get_time((char *)arg, len);
	            break;
	        case MQTT_RCV_MSG:
	            result = demo_rcv_msg(arg, len);
	            break;
	        case MQTT_SAVE_SECRET_INFO:
	            if (g_demo_param.write_flash_info == NULL)
	            {
	                result = ATINY_ERR;
	                ATINY_LOG(LOG_ERR, "write_flash_info null");
	            }
	            else
	            {
	                result = g_demo_param.write_flash_info(arg, len);
	            }
	            break;
	        case MQTT_READ_SECRET_INFO:
	            if (g_demo_param.read_flash_info == NULL)
	            {

	                result = ATINY_ERR;
	                ATINY_LOG(LOG_ERR, "read_flash_info null");
	            }
	            else
	            {
	                result = g_demo_param.read_flash_info(arg, len);
	            }
	            break;

	        default:
	            break;
        }
    return result;
	}
#if 0
	void mqtt_free_dynamic_info(mqtt_client_s* handle)
{
    if (handle->sub_topic)
    {
        (void)MQTTSetMessageHandler(&handle->client, handle->sub_topic, NULL);
        atiny_free(handle->sub_topic);
        handle->sub_topic = NULL;
    }
    TRY_FREE_MEM(handle->dynamic_info.save_info.deviceid);
    TRY_FREE_MEM(handle->dynamic_info.got_passward);
}
#endif
	void agent_tiny_demo_init(const demo_param_s *param)
	{
    	g_demo_param = *param;
	}

	static void init_mqtt(void)
	{
	    hieth_hw_init();
	    net_init();
	    flash_adaptor_init();
	}


	void * pthread_mqtt_test(void *para)
	{
	  struct atiny_mqtt_bind_t *bind_para;
	  bind_para=(struct atiny_mqtt_bind_t *)para;
	  printf("+++++++++++++++++++++into pthread_mqtt_test1129++++++++++++++++++++++++\n");
	  atiny_mqtt_bind(bind_para->device_info,bind_para->handle);
	  
	}

	void * pthread_mqtt_test1(void *para)
	{
	  struct atiny_mqtt_bind_t *bind_para;
	  bind_para=(struct atiny_mqtt_bind_t *)para;
	  
	  atiny_mqtt_bind(bind_para->device_info,bind_para->handle);
	}


	void * pthread_mqtt_test3(void *para)
	{
	  struct atiny_mqtt_bind_t *bind_para;
	  bind_para=(struct atiny_mqtt_bind_t *)para;
		  
	  atiny_mqtt_bind(bind_para->device_info,bind_para->handle);
	}

}
/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
TestMQTT_Client::TestMQTT_Client()
{
	
    TEST_ADD(TestMQTT_Client::test_atiny_mqtt_init);
	
	TEST_ADD(TestMQTT_Client::test_atiny_mqtt_subscribe_topic);
	
	TEST_ADD(TestMQTT_Client::test_atiny_mqtt_json_fail);
	
	TEST_ADD(TestMQTT_Client::test_atiny_mqtt_json_fail2);
	
	TEST_ADD(TestMQTT_Client::test_mqtt_dup_devinfo_fail);
	
	TEST_ADD(TestMQTT_Client::test_mqtt_dup_info_fail);
	

	TEST_ADD(TestMQTT_Client::test_mqtt_dup_devin_fail);
	
	TEST_ADD(TestMQTT_Client::test_atiny_mqtt_isconnected);
	
	TEST_ADD(TestMQTT_Client::test_atiny_mqttconnect);
	
	TEST_ADD(TestMQTT_Client::test_atiny_mqtt_recv_cmd_topic);
	
	TEST_ADD(TestMQTT_Client::test_atiny_cjsparse);
	
	TEST_ADD(TestMQTT_Client::test_atiny_mqtt_dup_devi_info);
	TEST_ADD(TestMQTT_Client::test_atiny_mqtt_dup_dev_check_info);
	
	
	//TEST_ADD(TestMQTT_Client::test_atiny_mqtt_bind);
	
	TEST_ADD(TestMQTT_Client::test_atiny_mqtt_data_send);
	
	TEST_ADD(TestMQTT_Client::test_atiny_mqtt_dup_dev_info);
	
	//TEST_ADD(TestMQTT_Client::test_atiny_mqtt_get_send_pw);
	
	TEST_ADD(TestMQTT_Client::test_mqtt_proc_connect_nack);
	
	TEST_ADD(TestMQTT_Client::test_mqtt_proc_connect_nack1);
	

	TEST_ADD(TestMQTT_Client::test_mqtt_clientInit_fail);
	
	TEST_ADD(TestMQTT_Client::test_mqtt_networkconnect_fail);
	
	TEST_ADD(TestMQTT_Client::test_atiny_mqtt_read_flash);

	
	//TEST_ADD(TestMQTT_Client::test_mqtt_cmd_ioctrlfail);
	
	
	//TEST_ADD(TestMQTT_Client::test_atiny_mqtt_bind_add);
	
}

TestMQTT_Client::~TestMQTT_Client()
{

}



char *stub_atiny_strdup_fail_time(const char *ch)
{
    char * p;
    
    if(i++ < g_random)
    {
        cleanStub(&si_atiny_strdup);
        p = atiny_strdup(ch);
		printf("time %d",i);
		setStub((void *)atiny_strdup, (void *)stub_atiny_strdup_fail_time, &si_atiny_strdup);

		return p;
    }
	
    
	return NULL;
}

CJSON_PUBLIC(cJSON *) stub_cJSON_Parse(char *value)
{

	cJSON *jsp=(cJSON *)atiny_malloc(sizeof(cJSON));
	
	return jsp;
}

CJSON_PUBLIC(cJSON *) stub_cJSON_Parse_fail(char *value)
{
	return NULL;
}

CJSON_PUBLIC(cJSON *) stub_cJSON_GetObjectItem(cJSON * const object, char * string)
{
	if(!strcmp(string,"msgType"))
	{  jsp1=(cJSON *)atiny_malloc(sizeof(cJSON));
	   jsp1->valuestring="cloudSendSecret";
	   return jsp1;
	}

	if(!strcmp(string,"deviceid"))
	{  jsp2=(cJSON *)atiny_malloc(sizeof(cJSON));
	   jsp2->valuestring="cloudSendSecret";
	   return jsp2;
	}

	if(!strcmp(string,"secret"))
	{  jsp3=(cJSON *)atiny_malloc(sizeof(cJSON));
	   jsp3->valuestring="cloudSendSecret";
	   return jsp3;
	}
	
}

CJSON_PUBLIC(cJSON *) stub_cJSON_GetObjectItem_fail(cJSON * const object, char * string)
{
	if(!strcmp(string,"msgType"))
	{  
	   return NULL;
	}

	if(!strcmp(string,"deviceid"))
	{  jsp2=(cJSON *)atiny_malloc(sizeof(cJSON));
	   jsp2->valuestring="cloudSendSecret";
	   return jsp2;
	}

	if(!strcmp(string,"secret"))
	{  jsp3=(cJSON *)atiny_malloc(sizeof(cJSON));
	   jsp3->valuestring="cloudSendSecret";
	   return jsp3;
	}
	
}

CJSON_PUBLIC(cJSON *) stub_cJSON_GetObjectItem_fail2(cJSON * const object, char * string)
{
	if(!strcmp(string,"msgType"))
	{  jsp1=(cJSON *)atiny_malloc(sizeof(cJSON));
	   jsp1->valuestring="cloudSendSecret";
	   return jsp1;
	}

	if(!strcmp(string,"deviceid"))
	{  
	   return NULL;
	}

	if(!strcmp(string,"secret"))
	{  jsp3=(cJSON *)atiny_malloc(sizeof(cJSON));
	   jsp3->valuestring="cloudSendSecret";
	   return jsp3;
	}
	
}



CJSON_PUBLIC(void) stub_cJSON_Delete(cJSON *c)
{
	atiny_free(c);
}



int stub_mbedtls_hmac_calc(mbedtls_hmac_t *hmac_info)
{
	return 0;
}

int stub_mbedtls_hmac_calc_fail(mbedtls_hmac_t *hmac_info)
{
	return 1;
}





void *stub_atiny_malloc_fail_time(size_t size)
{
    void * p;
    
    if(j++ < g_malloc_random)
    {
        cleanStub(&si_atiny_malloc);
        p = atiny_malloc(size);
		
		setStub((void *)atiny_malloc, (void *)stub_atiny_malloc_fail_time, &si_atiny_malloc);

		return p;
    }
	
    
	return NULL;
}


char *stub_atiny_strdup(const char *ch)
{
    return NULL;
}

void* stub_atiny_malloc_fail(size_t size)
{
    printf("call stub_atiny_malloc_fail\n");
    return NULL;
}

void stub_flash_manager_init(int (*cmd_ioctl)(mqtt_cmd_e cmd, void *arg, int32_t len))
{

}

int stub_flash_manager_write(const flash_info_s *flash_info)
{
	return ATINY_OK;
}

int stub_flash_manager_read(flash_info_s *flash_info)
{   

    int i=0;
	char exam0[8]="lgko";
	char exam1[8]="nodeid"; 
	char exam2[8]="hdell";
	char exam3[8]="abc";

	for(i=0;i<4;i++)
		flash_info->items[i]=(char *)atiny_malloc(8);
	
	strcpy(flash_info->items[0],exam0);
	strcpy(flash_info->items[1],exam1);
	strcpy(flash_info->items[2],exam2);
	strcpy(flash_info->items[3],exam3);
	
	return 0;
	
}

int stub_flash_manager_read2(flash_info_s *flash_info)
{   

    int i=0;
	char exam0[8]="lgk";
	char exam1[8]="node"; 
	char exam2[8]="hdell";
	char exam3[8]="abc";

	for(i=0;i<4;i++)
		flash_info->items[i]=(char *)atiny_malloc(8);
	
	strcpy(flash_info->items[0],exam0);
	strcpy(flash_info->items[1],exam1);
	strcpy(flash_info->items[2],exam2);
	strcpy(flash_info->items[3],exam3);
	
	return 0;
	
}


int stub_MQTTPublish(MQTTClient *c, const char *topicName, MQTTMessage *message)
{
	return 0;
}

int stub_MQTTPublish1(MQTTClient *c, const char *topicName, MQTTMessage *message)
{
	return -1;
}

void *stub_atiny_malloc(size_t size)
{
	return NULL;
}
mqtt_security_info_s *stub_mqtt_get_security_info(void)
{
	mqtt_client_s *handle=&g_mqtthandle;
	
	return &handle->params.info;
}

int stub_MQTTPublishWithoutMutex(MQTTClient *c, const char *topicName, MQTTMessage *message)
{
	return 0;
}

int  stub_los_write(struct Network* network, unsigned char* m, int i , int j)
{
	return 1;
}

void stub_NetworkInit(Network *n, mqtt_security_info_s *(*get_security_info)(void))
{
    if((NULL == n) ||
       (NULL == get_security_info))
    {
        ATINY_LOG(LOG_FATAL, "invalid params.");
        return;
    }
	   
	mqtt_security_info_s mqseinfo;
    memset(n, 0x0, sizeof(Network));
    get_security_info();
    n->mqttwrite = stub_los_write;
	n->get_security_info = stub_mqtt_get_security_info;

    return;
}





int stub_NetworkConnect(Network *n, char *addr, int port)
{
	return 0;
}

int stub_NetworkConnect_fail(Network *n, char *addr, int port)
{
	return 1;
}

int stub_MQTTClientInit(MQTTClient *c, Network *network, unsigned int command_timeout_ms,
                    unsigned char *sendbuf, size_t sendbuf_size, unsigned char *readbuf, size_t readbuf_size)
{
    
    int i;
    c->ipstack = network;
	int offset;
	int p_handle;
	mqtt_client_s *phandle;

    for (i = 0; i < MAX_MESSAGE_HANDLERS; ++i)
        c->messageHandlers[i].topicFilter = 0;
    c->command_timeout_ms = command_timeout_ms;
    c->buf = sendbuf;
    c->buf_size = sendbuf_size;
    c->readbuf = readbuf;
    c->readbuf_size = readbuf_size;
    c->isconnected = 0;
    c->cleansession = 0;
    c->ping_outstanding = 0;
    c->defaultMessageHandler = NULL;
    c->next_packetid = 1;
	test_readbuf=readbuf;
	
	offset=(int)&(((mqtt_client_s *)0)->client);
	p_handle=(int)c-offset;
	phandle=(mqtt_client_s *)p_handle;
	phandle->dynamic_info.state=MQTT_CONNECT_WITH_PRODUCT_ID;
	
    TimerInit(&c->last_sent);
    TimerInit(&c->last_received);
#if defined(MQTT_TASK)
		if (MutexInit(&c->mutex) != 0)
		{
			return FAILURE;
		}
#endif
    return MQTT_SUCCESS;

}

int stub_MQTTClientInit_fail(MQTTClient *c, Network *network, unsigned int command_timeout_ms,
                    unsigned char *sendbuf, size_t sendbuf_size, unsigned char *readbuf, size_t readbuf_size)
{
	return 1;
}
					

int stub_MQTTConnect(MQTTClient *c, MQTTPacket_connectData *options)
{	
	c->isconnected=1;
	
	return 0;
}


int stub_MQTTConnect_fail(MQTTClient *c, MQTTPacket_connectData *options)
{	
		return 1;
}

int stub_MQTTSubscribe(MQTTClient *c, const char *topicFilter, enum QoS qos,
                  messageHandler messageHandler)
{	
	memcpy(test_readbuf,"abc",3);
	MessageData md;
	MQTTMessage mqm;
	
	mqm.payload=test_readbuf;
	mqm.payloadlen=3;
	md.message=&mqm;
	MessageData mdn;
	MQTTMessage mqmn;
	mqmn.payload=NULL;
	mqmn.payloadlen=3;
	mdn.message=&mqmn;
	messageHandler(&mdn);
	messageHandler(&md);

	return 0;
}

int stub_MQTTSubscribe_dir(MQTTClient *c, const char *topicFilter, enum QoS qos,
									messageHandler messageHandler)
{   
	return 0;
}
			  
int stub_MQTTSubscribe2(MQTTClient *c, const char *topicFilter, enum QoS qos,
									messageHandler messageHandler)
{  
	test_gphandle->params.cmd_ioctl=NULL;
	memcpy(test_readbuf,"abc",3);
	MessageData md;
	MQTTMessage mqm;
	mqm.payload=test_readbuf;
	mqm.payloadlen=3;
	md.message=&mqm;
	messageHandler(&md);		  
	return 0;
}

 int stub_MQTTSubscribe_fail2(MQTTClient *c, const char *topicFilter, enum QoS qos,
									messageHandler messageHandler)
{   			  		
	messageHandler(NULL);			  
	return 0;
 }

				  
int stub_MQTTSubscribe_fail(MQTTClient *c, const char *topicFilter, enum QoS qos,
									messageHandler messageHandler)
{
	return 1;
}

int stub_MQTTSetMessageHandler(MQTTClient* c, const char* topicFilter, messageHandler messageHandler)			 
{
	return 0;
}

int stub_MQTTYield(MQTTClient *c, int timeout_ms)
{
	int offset;
	int p_handle;
	mqtt_client_s *phandle;
	offset=(int)&(((mqtt_client_s *)0)->client);
	p_handle=(int)c-offset;
	phandle=(mqtt_client_s *)p_handle;
	phandle->dynamic_info.connection_update_flag=1;
	return 0;
}

int stub_MQTTYield2(MQTTClient *c, int timeout_ms)
{	
	return 0;
}

int stub_atiny_task_mutex_lock(atiny_task_mutex_s *mutex)
{
	return 0;
}

int stub_atiny_task_mutex_unlock(atiny_task_mutex_s *mutex)
{
	return 0;
}


char stub_TimerIsExpired(Timer *timer)
{
	return 1;
}

void stub_NetworkDisconnect(Network *n)
{
	if(NULL == n)
		return;
	 n->ctx = NULL;

    return;
}

void TestMQTT_Client::test_atiny_mqtt_init(void)
{
	
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++begin0\n");
	

	demo_param_s demo_param;
	demo_param.init=init_mqtt;
	demo_param.write_flash_info=flash_adaptor_write_mqtt_info;
	demo_param.read_flash_info=flash_adaptor_read_mqtt_info;
	
    agent_tiny_demo_init(&demo_param);

	int result;

	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++begin1\n");
	result = atiny_mqtt_init(NULL, NULL);
	TEST_ASSERT_MSG((ATINY_ARG_INVALID == result), "atiny_init(...) failed");

	mqtt_param_s test_mqtt_params;
	
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++begin2\n");
	test_mqtt_params.server_ip = NULL;
	result = atiny_mqtt_init(&test_mqtt_params,&test_phandle);
	TEST_ASSERT_MSG((ATINY_ARG_INVALID == result), "atiny_init(...) failed");
	
	//mqtt_check_param psk
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++begin3/n");
	test_mqtt_params.server_ip = (char *)"192.168.1.100";
	test_mqtt_params.server_port = (char *)"6556";
	test_mqtt_params.info.security_type = MQTT_SECURITY_TYPE_PSK;
	test_mqtt_params.cmd_ioctl = test_cmd_ioctl;
	test_mqtt_params.info.u.psk.psk = NULL;
	result = atiny_mqtt_init(&test_mqtt_params,&test_phandle);
	TEST_ASSERT_MSG((ATINY_ARG_INVALID == result), "atiny_init(...) failed");


	//mqtt_check_param ca
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++begin4/n");
	test_mqtt_params.server_ip = (char *)"192.168.1.100";
	test_mqtt_params.server_port = (char *)"6556";
	test_mqtt_params.info.security_type = MQTT_SECURITY_TYPE_CA;
	test_mqtt_params.cmd_ioctl = test_cmd_ioctl;
	test_mqtt_params.info.u.ca.ca_crt = NULL;
	result = atiny_mqtt_init(&test_mqtt_params,&test_phandle);
	TEST_ASSERT_MSG((ATINY_ARG_INVALID == result), "atiny_init(...) failed");

	

	//mqtt_dup_param server_ip=NULL
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++begin5/n");
	int test_psk_len = 3;
	unsigned char test_psk[3] = {0xab, 0xcd, 0xef};
	test_mqtt_params.info.u.psk.psk = test_psk;
	test_mqtt_params.info.u.psk.psk_len = test_psk_len;
	unsigned char test_psk_id[20] = "testID";
	test_mqtt_params.info.u.psk.psk_id = test_psk_id;
	test_mqtt_params.info.u.psk.psk_id_len = strlen((char *)test_psk_id)+1;

	i =  0;
	g_random = 0;
	setStub((void *)atiny_strdup, (void *)stub_atiny_strdup_fail_time, &si_atiny_strdup);
	result = atiny_mqtt_init(&test_mqtt_params,&test_phandle);
	TEST_ASSERT_MSG((ATINY_MALLOC_FAILED == result), "atiny_init(...) failed");
	cleanStub(&si_atiny_strdup);
	

	
	//mqtt_dup_param server_port=NULL
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++begin6/n");
	i =  0;
	g_random = 1;
	setStub((void *)atiny_strdup, (void *)stub_atiny_strdup_fail_time, &si_atiny_strdup);
	result = atiny_mqtt_init(&test_mqtt_params,&test_phandle);
	TEST_ASSERT_MSG((ATINY_MALLOC_FAILED == result), "atiny_init(...) failed");
	cleanStub(&si_atiny_strdup);


	

	//mqtt_dup_param MQTT_SECURITY_TYPE_PSK
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++begin7/n");
	test_mqtt_params.info.security_type = MQTT_SECURITY_TYPE_PSK;
	j = 0;
	g_malloc_random = 2;
	setStub((void *)atiny_malloc, (void *)stub_atiny_malloc_fail_time, &si_atiny_malloc);
	result = atiny_mqtt_init(&test_mqtt_params,&test_phandle);
	TEST_ASSERT_MSG((ATINY_MALLOC_FAILED == result), "atiny_init(...) failed");
	cleanStub(&si_atiny_malloc);

	

	//mqtt_dup_param MQTT_SECURITY_TYPE_PSK
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++begin8/n");
	test_mqtt_params.info.security_type = MQTT_SECURITY_TYPE_PSK;
	i =  0;
	g_random = 2;
	j = 0;
	g_malloc_random = 3;
	setStub((void *)atiny_strdup, (void *)stub_atiny_strdup_fail_time, &si_atiny_strdup);
	setStub((void *)atiny_malloc, (void *)stub_atiny_malloc_fail_time, &si_atiny_malloc);
	result = atiny_mqtt_init(&test_mqtt_params,&test_phandle);
	TEST_ASSERT_MSG((ATINY_MALLOC_FAILED == result), "atiny_init(...) failed");
	cleanStub(&si_atiny_strdup);
	cleanStub(&si_atiny_malloc);

	

	//mqtt_dup_param MQTT_SECURITY_TYPE_CA
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++begin9/n");
	test_mqtt_params.info.security_type = MQTT_SECURITY_TYPE_CA;
	char test_mqtt_ca_crt[] =
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
	test_mqtt_params.info.u.ca.ca_crt = test_mqtt_ca_crt;
    test_mqtt_params.info.u.ca.ca_len = sizeof(test_mqtt_ca_crt);
	i =  0;
	g_random = 2;
	j = 0;
	g_malloc_random = 2;
	setStub((void *)atiny_strdup, (void *)stub_atiny_strdup_fail_time, &si_atiny_strdup);
	setStub((void *)atiny_malloc, (void *)stub_atiny_malloc_fail_time, &si_atiny_malloc);
	result = atiny_mqtt_init(&test_mqtt_params,&test_phandle);
	TEST_ASSERT_MSG((0 != result), "atiny_init(...) failed");
	cleanStub(&si_atiny_strdup);
	cleanStub(&si_atiny_malloc);



	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++begin10/n");
	//mqtt_dup_param MQTT_SECURITY_TYPE_NONE
	//stubInfo si_flash_manager_write;
	test_mqtt_params.info.security_type = MQTT_SECURITY_TYPE_NONE;
	i =  0;
	g_random = 2;
	setStub((void *)atiny_strdup, (void *)stub_atiny_strdup_fail_time, &si_atiny_strdup);
	setStub((void *)flash_manager_write, (void *)stub_flash_manager_write, &si_flash_manager_write);
	//setStub((void *)flash_manager_init, (void *)stub_flash_manager_init, &si_flash_manager_init);
	result = atiny_mqtt_init(&test_mqtt_params,&test_phandle);
	TEST_ASSERT_MSG((ATINY_OK == result), "atiny_init(...) failed");
	cleanStub(&si_atiny_strdup);
	cleanStub(&si_flash_manager_write);
	atiny_free((*test_phandle).params.server_ip);	
	atiny_free((*test_phandle).params.server_port);

	result = atiny_mqtt_init(&test_mqtt_params,&test_phandle);
	TEST_ASSERT_MSG((ATINY_OK == result), "atiny_init(...) failed");

	
	//mqtt_dup_param MQTT_SECURITY_TYPE_PSK
	(*test_phandle).init_flag=0;
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++begin8\n");
	test_mqtt_params.info.security_type = MQTT_SECURITY_TYPE_PSK;
	i =  0;
	g_random = 2;
	j = 0;
	g_malloc_random = 4;
	setStub((void *)atiny_strdup, (void *)stub_atiny_strdup_fail_time, &si_atiny_strdup);
	setStub((void *)atiny_malloc, (void *)stub_atiny_malloc_fail_time, &si_atiny_malloc);
	setStub((void *)flash_manager_write, (void *)stub_flash_manager_write, &si_flash_manager_write);
	result = atiny_mqtt_init(&test_mqtt_params,&test_phandle);
	TEST_ASSERT_MSG((ATINY_OK == result), "atiny_init(...) failed");
	cleanStub(&si_atiny_strdup);
	cleanStub(&si_atiny_malloc);
	cleanStub(&si_flash_manager_write);
	atiny_free((*test_phandle).params.server_ip);	
	atiny_free((*test_phandle).params.server_port);
	atiny_free((*test_phandle).params.info.u.psk.psk_id);
	atiny_free((*test_phandle).params.info.u.psk.psk);

	
	//mqtt_dup_param MQTT_SECURITY_TYPE_CA
	(*test_phandle).init_flag=0;
	test_mqtt_params.info.security_type = MQTT_SECURITY_TYPE_CA;
	i =  0;
	g_random = 2;
	j = 0;
	g_malloc_random = 3;
	setStub((void *)atiny_strdup, (void *)stub_atiny_strdup_fail_time, &si_atiny_strdup);
	setStub((void *)atiny_malloc, (void *)stub_atiny_malloc_fail_time, &si_atiny_malloc);
	setStub((void *)flash_manager_write, (void *)stub_flash_manager_write, &si_flash_manager_write);
	result = atiny_mqtt_init(&test_mqtt_params,&test_phandle);
	TEST_ASSERT_MSG((ATINY_OK == result), "atiny_init(...) failed");
	cleanStub(&si_atiny_strdup);
	cleanStub(&si_atiny_malloc);
	cleanStub(&si_flash_manager_write);
	atiny_free((*test_phandle).params.server_ip);	
	atiny_free((*test_phandle).params.server_port);
	atiny_free((*test_phandle).params.info.u.ca.ca_crt);
}

void TestMQTT_Client::test_atiny_mqtt_isconnected()
{
	int result;
	result = atiny_mqtt_isconnected(NULL);
	TEST_ASSERT_MSG((result == 0), "atiny_isconnected(...) failed");

	mqtt_client_s test_mqtt_params;
	test_mqtt_params.device_info.connection_type = MQTT_DYNAMIC_CONNECT;
	test_mqtt_params.dynamic_info.state = MQTT_CONNECT_WITH_DEVICE_ID;
	test_mqtt_params.client.isconnected = 1;
	result = atiny_mqtt_isconnected(&test_mqtt_params);
	TEST_ASSERT_MSG((result == 1), "atiny_isconnected(...) failed");
}

void TestMQTT_Client::test_atiny_mqtt_data_send()
{
	int result;
	mqtt_qos_e test_mqtt_qos_e = MQTT_QOS_MOST_ONCE;
	result = atiny_mqtt_data_send(NULL, NULL, 0, test_mqtt_qos_e);
	TEST_ASSERT_MSG((result == ATINY_ARG_INVALID), "atiny_data_send(...) failed");

	mqtt_client_s test_mqtt_params;
	char test_msg[] = "test";
	char *test_p_msg = test_msg;
	uint32_t test_msg_len = strlen(test_msg)+1;
	test_mqtt_params.device_info.connection_type = MQTT_DYNAMIC_CONNECT;
	test_mqtt_params.dynamic_info.state = MQTT_CONNECT_WITH_DEVICE_ID;
	test_mqtt_params.client.isconnected = 0;
	result = atiny_mqtt_data_send(&test_mqtt_params, test_p_msg, test_msg_len, test_mqtt_qos_e);
	TEST_ASSERT_MSG((result == ATINY_ERR), "atiny_data_send(...) failed");
	
	test_mqtt_params.client.isconnected = 1;
    test_mqtt_params.dynamic_info.save_info.deviceid="hdell";
    test_mqtt_params.device_info.codec_mode=MQTT_CODEC_MODE_JASON;
	
	stubInfo stub_info;
	setStub((void *)MQTTPublish, (void *)stub_MQTTPublish, &stub_info);
	result = atiny_mqtt_data_send(&test_mqtt_params, test_p_msg, test_msg_len, test_mqtt_qos_e);
	TEST_ASSERT_MSG((result == 0), "atiny_data_send(...) failed");
    cleanStub(&stub_info);

	stubInfo stub_info1;
	setStub((void *)MQTTPublish, (void *)stub_MQTTPublish1, &stub_info1);
	result = atiny_mqtt_data_send(&test_mqtt_params, test_p_msg, test_msg_len, test_mqtt_qos_e);
	TEST_ASSERT_MSG((result == ATINY_ERR), "atiny_data_send(...) failed");
    cleanStub(&stub_info1);

    stubInfo stub_info2;
	setStub((void *)atiny_malloc, (void *)stub_atiny_malloc, &stub_info2);
	result = atiny_mqtt_data_send(&test_mqtt_params, test_p_msg, test_msg_len, test_mqtt_qos_e);
	TEST_ASSERT_MSG((result == ATINY_MALLOC_FAILED), "atiny_data_send(...) failed");
    cleanStub(&stub_info2);
	
}

void TestMQTT_Client::test_atiny_mqtt_dup_dev_info()
{
    int ret;
	mqtt_device_info_s device_info; 
	mqtt_client_s handle;
	
	device_info.connection_type=MQTT_STATIC_CONNECT;
	device_info.codec_mode=MQTT_CODEC_MODE_JASON;
	device_info.sign_type=MQTT_SIGN_TYPE_HMACSHA256_CHECK_TIME;
	device_info.password=(char *)atiny_malloc(8);
	strcpy(device_info.password,"abc");
	device_info.u.s_info.deviceid="hdell";
	device_info.connection_type=MQTT_DYNAMIC_CONNECT;
	device_info.u.d_info.productid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.productid,"lgko");
	device_info.u.d_info.nodeid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.nodeid,"nodeid");
	handle.sub_topic=NULL;
	handle.dynamic_info.save_info.deviceid=(char *)atiny_malloc(8);
	strcpy(handle.dynamic_info.save_info.deviceid,"devid");
	handle.dynamic_info.got_passward=(char *)atiny_malloc(8);
	strcpy(handle.dynamic_info.got_passward,"abc");
		
	handle.params.server_ip=(char *)atiny_malloc(16);
	strcpy(handle.params.server_ip,"192.168.1.102");
	handle.params.server_port=(char *)atiny_malloc(8);
	strcpy(handle.params.server_port,"5683");
	g_mqtthandle.params.info.security_type=MQTT_SECURITY_TYPE_NONE;
	handle.dynamic_info.connection_update_flag=1;

    pthread_t first_thread;

    struct atiny_mqtt_bind_t mqtt_bind_para;
	mqtt_bind_para.device_info=&device_info;
	mqtt_bind_para.handle=&handle;
		
    stubInfo stub_info;
	setStub((void *)atiny_strdup, (void *)stub_atiny_strdup, &stub_info);

	ret = pthread_create(&first_thread, NULL, pthread_mqtt_test, (void *)&mqtt_bind_para);
	TEST_ASSERT_MSG((0 == ret), "atiny_bind(...) failed");
	usleep(10000);
	usleep(10000);
    pthread_cancel(first_thread);
    pthread_join(first_thread, NULL);

	cleanStub(&stub_info);

	atiny_free(device_info.password);
	atiny_free(device_info.u.d_info.productid);
	atiny_free(device_info.u.d_info.nodeid);
}

void TestMQTT_Client::test_atiny_mqtt_dup_devi_info()
{
    int ret;
	mqtt_device_info_s device_info; 
	mqtt_client_s handle;
	
	device_info.connection_type=MQTT_STATIC_CONNECT;
	device_info.codec_mode=MQTT_CODEC_MODE_JASON;
	device_info.sign_type=MQTT_SIGN_TYPE_HMACSHA256_CHECK_TIME;
	device_info.connection_type=MQTT_STATIC_CONNECT;
	device_info.password=(char *)atiny_malloc(8);
	strcpy(device_info.password,"abc");
	device_info.u.s_info.deviceid="hdell";
	device_info.u.d_info.productid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.productid,"lgko");
	device_info.u.d_info.nodeid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.nodeid,"nodeid");
	handle.sub_topic=NULL;
	handle.dynamic_info.save_info.deviceid=(char *)atiny_malloc(8);
	strcpy(handle.dynamic_info.save_info.deviceid,"devid");
	handle.dynamic_info.got_passward=(char *)atiny_malloc(8);
	strcpy(handle.dynamic_info.got_passward,"abc");
		
	handle.params.server_ip=(char *)atiny_malloc(16);
	strcpy(handle.params.server_ip,"192.168.1.102");
	handle.params.server_port=(char *)atiny_malloc(8);
	strcpy(handle.params.server_port,"5683");
	g_mqtthandle.params.info.security_type=MQTT_SECURITY_TYPE_NONE;
	
 	handle.sub_topic=(char *)atiny_malloc(8);
    strcpy(handle.sub_topic,"abc");
	handle.dynamic_info.connection_update_flag=1;

	pthread_t first_thread;
	struct atiny_mqtt_bind_t mqtt_bind_para;
	mqtt_bind_para.device_info=&device_info;
	mqtt_bind_para.handle=&handle;
		
	stubInfo stub_info2;
	stubInfo stub_info1;
	stubInfo stub_info_netwcon;
	stubInfo stub_mqttcon;
	stubInfo stub_mqttsub;
	stubInfo stub_mqttyie;
		
	setStub((void *)flash_manager_read, (void *)stub_flash_manager_read, &stub_info2);
	setStub((void *)NetworkInit, (void *)stub_NetworkInit, &stub_info1);
	setStub((void *)NetworkConnect, (void *)stub_NetworkConnect, &stub_info_netwcon);
	setStub((void *)MQTTConnect, (void *)stub_MQTTConnect, &stub_mqttcon);
	setStub((void *)MQTTSubscribe, (void *)stub_MQTTSubscribe, &stub_mqttsub);
	setStub((void *)MQTTYield, (void *)stub_MQTTYield, &stub_mqttyie);

	ret = pthread_create(&first_thread, NULL, pthread_mqtt_test, (void *)&mqtt_bind_para);
	TEST_ASSERT_MSG((0 == ret), "atiny_bind(...) failed");
	usleep(10000);
	usleep(10000);
	pthread_cancel(first_thread);
	pthread_join(first_thread, NULL); 
	
	cleanStub(&stub_mqttyie);
   	cleanStub(&stub_mqttsub);
    cleanStub(&stub_mqttcon);
   	cleanStub(&stub_info_netwcon);
	cleanStub(&stub_info1);
	cleanStub(&stub_info2);
		
	atiny_free(device_info.password);
	atiny_free(device_info.u.d_info.productid);
	atiny_free(device_info.u.d_info.nodeid);	
}


void TestMQTT_Client::test_atiny_mqtt_dup_dev_check_info()
{
	int ret;
	mqtt_device_info_s device_info; 
    mqtt_client_s handle;
	   
	device_info.codec_mode=MQTT_CODEC_MODE_JASON;
	device_info.sign_type=MQTT_SIGN_TYPE_HMACSHA256_CHECK_TIME;
    device_info.connection_type=MQTT_MAX_CONNECTION_TYPE;
	device_info.password=(char *)atiny_malloc(8);
	strcpy(device_info.password,"abc");
	device_info.u.s_info.deviceid="hdell";
	device_info.u.d_info.productid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.productid,"lgko");
	device_info.u.d_info.nodeid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.nodeid,"nodeid");
 
	   handle.sub_topic=NULL;
	   handle.dynamic_info.save_info.deviceid=(char *)atiny_malloc(8);
	   strcpy(handle.dynamic_info.save_info.deviceid,"devid");
	   handle.dynamic_info.got_passward=(char *)atiny_malloc(8);
	   strcpy(handle.dynamic_info.got_passward,"abc");
		   
	   handle.params.server_ip=(char *)atiny_malloc(16);
	   strcpy(handle.params.server_ip,"192.168.1.102");
	   handle.params.server_port=(char *)atiny_malloc(8);
	   strcpy(handle.params.server_port,"5683");
	   g_mqtthandle.params.info.security_type=MQTT_SECURITY_TYPE_NONE;
	   
	
	   handle.dynamic_info.connection_update_flag=1;


	   stubInfo stub_mulock;
	   stubInfo stub_muunlock;
		   
	   setStub((void *)atiny_task_mutex_lock, (void *)stub_atiny_task_mutex_lock, &stub_mulock);
	   setStub((void *)atiny_task_mutex_unlock, (void *)stub_atiny_task_mutex_unlock, &stub_muunlock);
	   
	   atiny_mqtt_bind(&device_info, &handle);

	   device_info.connection_type=MQTT_STATIC_CONNECT;
	   device_info.u.s_info.deviceid=NULL;

	   atiny_mqtt_bind(&device_info, &handle);

	   device_info.connection_type=MQTT_DYNAMIC_CONNECT;
	   device_info.u.d_info.productid=NULL;

	   atiny_mqtt_bind(&device_info, &handle);

	   cleanStub(&stub_muunlock);
	   cleanStub(&stub_mulock);

}


void TestMQTT_Client::test_atiny_mqtt_get_send_pw()
{
    int ret;
	mqtt_device_info_s device_info; 
	mqtt_client_s handle;
	
	device_info.connection_type=MQTT_STATIC_CONNECT;
	device_info.codec_mode=MQTT_CODEC_MODE_JASON;
	device_info.sign_type=MQTT_SIGN_TYPE_HMACSHA256_CHECK_TIME;
	device_info.password=(char *)atiny_malloc(8);
	strcpy(device_info.password,"abc");
	device_info.u.s_info.deviceid="hdell";
	device_info.connection_type=MQTT_DYNAMIC_CONNECT;
	device_info.u.d_info.productid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.productid,"lgko");
	device_info.u.d_info.nodeid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.nodeid,"nodeid");
	handle.sub_topic=NULL;
	handle.params.server_ip=(char *)atiny_malloc(16);
	strcpy(handle.params.server_ip,"192.168.1.102");
	handle.params.server_port=(char *)atiny_malloc(8);
	strcpy(handle.params.server_port,"5683");
	g_mqtthandle.params.info.security_type=MQTT_SECURITY_TYPE_NONE;
	
	handle.sub_topic=(char *)atiny_malloc(8);
	strcpy(handle.sub_topic,"abc");
	handle.dynamic_info.connection_update_flag=1;

	pthread_t first_thread;
	struct atiny_mqtt_bind_t mqtt_bind_para;
	mqtt_bind_para.device_info=&device_info;
	mqtt_bind_para.handle=&handle;

	stubInfo stub_hcf;
	stubInfo stub_info2;
	stubInfo stub_info1;
	stubInfo stub_info_netwcon;
	stubInfo stub_mqttcon;
	stubInfo stub_mqttsub;
	stubInfo stub_mqttyie;
	
    setStub((void *)mbedtls_hmac_calc, (void *)stub_mbedtls_hmac_calc_fail, &stub_hcf);
	setStub((void *)flash_manager_read, (void *)stub_flash_manager_read, &stub_info2);
	setStub((void *)NetworkInit, (void *)stub_NetworkInit, &stub_info1);
	setStub((void *)NetworkConnect, (void *)stub_NetworkConnect, &stub_info_netwcon);
	setStub((void *)MQTTConnect, (void *)stub_MQTTConnect, &stub_mqttcon);
	setStub((void *)MQTTSubscribe, (void *)stub_MQTTSubscribe, &stub_mqttsub);
	setStub((void *)MQTTYield, (void *)stub_MQTTYield, &stub_mqttyie);

	ret = pthread_create(&first_thread, NULL, pthread_mqtt_test, (void *)&mqtt_bind_para);
	TEST_ASSERT_MSG((0 == ret), "atiny_bind(...) failed");
	usleep(10000);
	usleep(10000);
	pthread_cancel(first_thread);
	pthread_join(first_thread, NULL); 
	
	cleanStub(&stub_hcf);
	cleanStub(&stub_mqttyie);
   	cleanStub(&stub_mqttsub);
    cleanStub(&stub_mqttcon);
   	cleanStub(&stub_info_netwcon);
	cleanStub(&stub_info1);
	cleanStub(&stub_info2);

	atiny_free(device_info.password);
	atiny_free(device_info.u.d_info.productid);
	atiny_free(device_info.u.d_info.nodeid);
}

void TestMQTT_Client::test_atiny_mqttconnect()
{
    int ret;
	mqtt_device_info_s device_info; 
	mqtt_client_s handle;
	
	device_info.connection_type=MQTT_STATIC_CONNECT;
	device_info.codec_mode=MQTT_CODEC_MODE_JASON;
	device_info.sign_type=MQTT_SIGN_TYPE_HMACSHA256_CHECK_TIME;
    device_info.password=(char *)atiny_malloc(8);
	strcpy(device_info.password,"abc");
	device_info.u.s_info.deviceid="hdell";
	device_info.connection_type=MQTT_DYNAMIC_CONNECT;
	device_info.u.d_info.productid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.productid,"lgko");
	device_info.u.d_info.nodeid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.nodeid,"nodeid");
	handle.sub_topic=NULL;
	handle.params.server_ip=(char *)atiny_malloc(16);
	strcpy(handle.params.server_ip,"192.168.1.102");
	handle.params.server_port=(char *)atiny_malloc(8);
	strcpy(handle.params.server_port,"5683");
	g_mqtthandle.params.info.security_type=MQTT_SECURITY_TYPE_NONE;
	handle.sub_topic=(char *)atiny_malloc(8);
	strcpy(handle.sub_topic,"abc");
	handle.dynamic_info.connection_update_flag=1;

	pthread_t first_thread;
	struct atiny_mqtt_bind_t mqtt_bind_para;
	mqtt_bind_para.device_info=&device_info;
	mqtt_bind_para.handle=&handle;
	
	stubInfo stub_info2;
	stubInfo stub_info1;
	stubInfo stub_info_netwcon;
	stubInfo stub_mqttcon;
	stubInfo stub_mqttsub;
	stubInfo stub_mqttyie;
	
	setStub((void *)flash_manager_read, (void *)stub_flash_manager_read, &stub_info2);
	setStub((void *)NetworkInit, (void *)stub_NetworkInit, &stub_info1);
	setStub((void *)NetworkConnect, (void *)stub_NetworkConnect, &stub_info_netwcon);
	setStub((void *)MQTTConnect, (void *)stub_MQTTConnect_fail, &stub_mqttcon);
	setStub((void *)MQTTSubscribe, (void *)stub_MQTTSubscribe, &stub_mqttsub);
	setStub((void *)MQTTYield, (void *)stub_MQTTYield, &stub_mqttyie);

	ret = pthread_create(&first_thread, NULL, pthread_mqtt_test, (void *)&mqtt_bind_para);
	TEST_ASSERT_MSG((0 == ret), "atiny_bind(...) failed");
	usleep(10000);
	usleep(10000);
	pthread_cancel(first_thread);
	pthread_join(first_thread, NULL); 

	cleanStub(&stub_mqttyie);
   	cleanStub(&stub_mqttsub);
    cleanStub(&stub_mqttcon);
   	cleanStub(&stub_info_netwcon);
	cleanStub(&stub_info1);
	cleanStub(&stub_info2);

	atiny_free(handle.params.server_ip);
	atiny_free(handle.sub_topic);
	atiny_free(handle.params.server_port);
	atiny_free(handle.device_info.u.d_info.nodeid);
	atiny_free(handle.device_info.u.d_info.productid);
	atiny_free(handle.device_info.password);
	
	atiny_free(device_info.password);
	atiny_free(device_info.u.d_info.productid);
	atiny_free(device_info.u.d_info.nodeid);
}


void TestMQTT_Client::test_atiny_cjsparse()
{
	int ret;
	mqtt_device_info_s device_info; 
	mqtt_client_s handle;

	device_info.connection_type=MQTT_STATIC_CONNECT;
	device_info.codec_mode=MQTT_CODEC_MODE_JASON;
	device_info.sign_type=MQTT_SIGN_TYPE_HMACSHA256_CHECK_TIME;
	device_info.password=(char *)atiny_malloc(8);
	strcpy(device_info.password,"abc");
    device_info.u.s_info.deviceid="hdell";
	device_info.connection_type=MQTT_DYNAMIC_CONNECT;
	device_info.u.d_info.productid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.productid,"lgko");
	
	pthread_t first_thread;
	device_info.u.d_info.nodeid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.nodeid,"nodeid");
	handle.params.server_ip=(char *)atiny_malloc(16);
	strcpy(handle.params.server_ip,"192.168.1.102");
	handle.params.server_port=(char *)atiny_malloc(8);
	strcpy(handle.params.server_port,"5683");
	g_mqtthandle.params.info.security_type=MQTT_SECURITY_TYPE_NONE;
	
	struct atiny_mqtt_bind_t mqtt_bind_para;
	mqtt_bind_para.device_info=&device_info;
	mqtt_bind_para.handle=&handle;
	handle.sub_topic=(char *)atiny_malloc(8);
	strcpy(handle.sub_topic,"abc");
	handle.dynamic_info.connection_update_flag=1;
	
	stubInfo stub_info;
	stubInfo stub_info1;
	stubInfo stub_info_netcon;
	stubInfo stub_mqttcon;
	stubInfo stub_mqttsub;
	stubInfo stub_mqttyie;
	
	setStub((void *)flash_manager_read, (void *)stub_flash_manager_read, &stub_info);
	setStub((void *)NetworkInit, (void *)stub_NetworkInit, &stub_info1);
	setStub((void *)NetworkConnect, (void *)stub_NetworkConnect, &stub_info_netcon);
	setStub((void *)MQTTConnect, (void *)stub_MQTTConnect, &stub_mqttcon);
	setStub((void *)MQTTSubscribe, (void *)stub_MQTTSubscribe_fail, &stub_mqttsub);
	setStub((void *)MQTTYield, (void *)stub_MQTTYield, &stub_mqttyie);


	ret = pthread_create(&first_thread, NULL, pthread_mqtt_test, (void *)&mqtt_bind_para);
	TEST_ASSERT_MSG((0 == ret), "atiny_bind(...) failed");
	usleep(10000);
	usleep(10000);
    pthread_cancel(first_thread);
    pthread_join(first_thread, NULL);
	
    cleanStub(&stub_mqttyie);
    cleanStub(&stub_mqttsub);
    cleanStub(&stub_mqttcon);
    cleanStub(&stub_info_netcon);
	cleanStub(&stub_info1);
	cleanStub(&stub_info);
	
	atiny_free(device_info.password);
	atiny_free(device_info.u.d_info.productid);
	atiny_free(device_info.u.d_info.nodeid);
}

void TestMQTT_Client::test_atiny_mqtt_recv_cmd_topic()
{
    int ret;
	mqtt_device_info_s device_info; 
	mqtt_client_s handle;
	
	device_info.connection_type=MQTT_STATIC_CONNECT;
	device_info.codec_mode=MQTT_CODEC_MODE_JASON;
	device_info.sign_type=MQTT_SIGN_TYPE_HMACSHA256_CHECK_TIME;
	device_info.password=(char *)atiny_malloc(8);
	strcpy(device_info.password,"abc");
	device_info.u.s_info.deviceid="hdell";
	device_info.u.d_info.productid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.productid,"lgko");
	   
	pthread_t first_thread;
	device_info.u.d_info.nodeid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.nodeid,"nodeid");
	handle.params.server_ip=(char *)atiny_malloc(16);
	strcpy(handle.params.server_ip,"192.168.1.102");
	handle.params.server_port=(char *)atiny_malloc(8);
	strcpy(handle.params.server_port,"5683");
    g_mqtthandle.params.info.security_type=MQTT_SECURITY_TYPE_NONE;

	struct atiny_mqtt_bind_t mqtt_bind_para;
	mqtt_bind_para.device_info=&device_info;
	mqtt_bind_para.handle=&handle;
	handle.sub_topic=(char *)atiny_malloc(8);
	strcpy(handle.sub_topic,"abc");
	handle.dynamic_info.connection_update_flag=1;
	handle.dynamic_info.save_info.deviceid=(char *)atiny_malloc(8);
	strcpy(handle.dynamic_info.save_info.deviceid,"abc");
	handle.dynamic_info.got_passward=(char *)atiny_malloc(8);
	strcpy(handle.dynamic_info.got_passward,"abc");
	   
	stubInfo stub_info;
	stubInfo stub_info1;
	stubInfo stub_info_netcon;
	stubInfo stub_mqttcon;
	stubInfo stub_mqttsub;
	stubInfo stub_mqttyie;
	setStub((void *)flash_manager_read, (void *)stub_flash_manager_read, &stub_info);
	setStub((void *)NetworkInit, (void *)stub_NetworkInit, &stub_info1);
	setStub((void *)NetworkConnect, (void *)stub_NetworkConnect, &stub_info_netcon);
	setStub((void *)MQTTConnect, (void *)stub_MQTTConnect, &stub_mqttcon);
	setStub((void *)MQTTSubscribe, (void *)stub_MQTTSubscribe_fail2, &stub_mqttsub);
	setStub((void *)MQTTYield, (void *)stub_MQTTYield, &stub_mqttyie);
	
    ret = pthread_create(&first_thread, NULL, pthread_mqtt_test, (void *)&mqtt_bind_para);
	TEST_ASSERT_MSG((0 == ret), "atiny_bind(...) failed");
	usleep(10000);
	usleep(10000);
    pthread_cancel(first_thread);
    pthread_join(first_thread, NULL); 

	cleanStub(&stub_mqttyie);
	cleanStub(&stub_mqttsub);
	cleanStub(&stub_mqttcon);
	cleanStub(&stub_info_netcon);
	cleanStub(&stub_info1);
	cleanStub(&stub_info);

	atiny_free(handle.params.server_ip);
	atiny_free(handle.dynamic_info.got_passward);
	atiny_free(handle.dynamic_info.save_info.deviceid);
	atiny_free(handle.params.server_port);


	atiny_free(device_info.password);
	atiny_free(device_info.u.d_info.productid);
	atiny_free(device_info.u.d_info.nodeid);
}

void TestMQTT_Client::test_atiny_mqtt_subscribe_topic()
{
    int ret;
	mqtt_device_info_s device_info; 
	mqtt_client_s handle;
	mqtt_client_s *phandle=&handle;

	pthread_t first_thread;
	handle.params.server_ip=(char *)atiny_malloc(16);
	strcpy(handle.params.server_ip,"192.168.1.102");
	handle.params.server_port=(char *)atiny_malloc(8);
	strcpy(handle.params.server_port,"5683");
	handle.params.info.security_type=MQTT_SECURITY_TYPE_NONE;
	handle.params.cmd_ioctl=cmd_ioctl_exam;
	g_mqtthandle.params.info.security_type=MQTT_SECURITY_TYPE_NONE;
	handle.sub_topic=(char *)atiny_malloc(8);
	strcpy(handle.sub_topic,"abc");
	handle.dynamic_info.connection_update_flag=1;
	handle.dynamic_info.state=MQTT_CONNECT_WITH_PRODUCT_ID;
	(*test_phandle).init_flag=0;
	stubInfo stub_flashwr;
	setStub((void *)flash_manager_write, (void *)stub_flash_manager_write, &stub_flashwr);
	atiny_mqtt_init(&(handle.params),&test_phandle);
	device_info.connection_type=MQTT_STATIC_CONNECT;
	device_info.codec_mode=MQTT_CODEC_MODE_JASON;
	device_info.sign_type=MQTT_SIGN_TYPE_HMACSHA256_CHECK_TIME;
	device_info.password=(char *)atiny_malloc(8);
	strcpy(device_info.password,"abc");
    device_info.u.s_info.deviceid="hdell";
	device_info.connection_type=MQTT_DYNAMIC_CONNECT;
	device_info.u.d_info.productid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.productid,"lgko");
	device_info.u.d_info.nodeid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.nodeid,"nodeid");
	struct atiny_mqtt_bind_t mqtt_bind_para;
	mqtt_bind_para.device_info=&device_info;
	mqtt_bind_para.handle=test_phandle;

	stubInfo stub_info;
	stubInfo stub_info1;
	stubInfo stub_mci;
	stubInfo stub_info_netcon;
	stubInfo stub_mqttcon;
	stubInfo stub_mqttsub;
	stubInfo stub_mqttyie;
	stubInfo stub_jsonp;
	stubInfo stub_jsonitm;
	stubInfo stub_jsondel;
	stubInfo stub_mbhc;
	stubInfo stub_mpwm;
	stubInfo stub_netdis;
	
	setStub((void *)flash_manager_read, (void *)stub_flash_manager_read, &stub_info);
	setStub((void *)NetworkInit, (void *)stub_NetworkInit, &stub_info1);
	setStub((void *)MQTTClientInit, (void *)stub_MQTTClientInit, &stub_mci);
	setStub((void *)NetworkConnect, (void *)stub_NetworkConnect, &stub_info_netcon);
	setStub((void *)MQTTConnect, (void *)stub_MQTTConnect, &stub_mqttcon);
	setStub((void *)MQTTSubscribe, (void *)stub_MQTTSubscribe, &stub_mqttsub);
	setStub((void *)MQTTYield, (void *)stub_MQTTYield, &stub_mqttyie);
	setStub((void *)cJSON_Parse, (void *)stub_cJSON_Parse, &stub_jsonp);
	setStub((void *)cJSON_GetObjectItem, (void *)stub_cJSON_GetObjectItem, &stub_jsonitm);
	setStub((void *)cJSON_Delete, (void *)stub_cJSON_Delete, &stub_jsondel);
	setStub((void *)mbedtls_hmac_calc, (void *)stub_mbedtls_hmac_calc, &stub_mbhc);
	setStub((void *)MQTTPublish, (void *)stub_MQTTPublish, &stub_mpwm);
	setStub((void *)NetworkDisconnect,(void *)stub_NetworkDisconnect, &stub_netdis);
	
	ret = pthread_create(&first_thread, NULL, pthread_mqtt_test1, (void *)&mqtt_bind_para);
	TEST_ASSERT_MSG((0 == ret), "atiny_bind(...) failed");
	usleep(10000);
	//mqtt_bind_para.device_info=NULL;
	//usleep(10000);
	pthread_cancel(first_thread);
	pthread_join(first_thread, NULL); 
//    mqtt_free_dynamic_info(test_phandle);
	cleanStub(&stub_netdis);
   	cleanStub(&stub_mpwm);
    cleanStub(&stub_mbhc);
    cleanStub(&stub_flashwr);
    cleanStub(&stub_jsondel);
    cleanStub(&stub_jsonitm);
    cleanStub(&stub_jsonp);
    cleanStub(&stub_mqttyie);
    cleanStub(&stub_mqttsub);
    cleanStub(&stub_mqttcon);
    cleanStub(&stub_info_netcon);
	cleanStub(&stub_mci);
	cleanStub(&stub_info1);
	cleanStub(&stub_info);

	atiny_free(device_info.password);
	atiny_free(device_info.u.d_info.productid);
	atiny_free(device_info.u.d_info.nodeid);
	atiny_free(handle.params.server_port);
	atiny_free(handle.sub_topic);
	atiny_free(handle.params.server_ip);
	
	atiny_free(test_phandle->sub_topic);
	
	#if 0
	atiny_free(test_phandle->params.server_ip);
	atiny_free(test_phandle->params.server_port);
	atiny_free(test_phandle->dynamic_info.got_passward);
	atiny_free(test_phandle->dynamic_info.save_info.deviceid);
    atiny_free(test_phandle->device_info.u.d_info.nodeid);
	atiny_free(test_phandle->device_info.u.d_info.productid);
	
	atiny_free(test_phandle->device_info.password);
	#endif
	atiny_free(jsp1);
	atiny_free(jsp2);
	atiny_free(jsp3);
}

void TestMQTT_Client::test_atiny_mqtt_json_fail()
{
    int ret;
	mqtt_device_info_s device_info; 
	mqtt_client_s handle;
	mqtt_client_s *phandle=&handle;

	pthread_t first_thread;
	handle.params.server_ip=(char *)atiny_malloc(16);
	strcpy(handle.params.server_ip,"192.168.1.102");
	handle.params.server_port=(char *)atiny_malloc(8);
	strcpy(handle.params.server_port,"5683");
	handle.params.info.security_type=MQTT_SECURITY_TYPE_NONE;
	handle.params.cmd_ioctl=cmd_ioctl_exam;
	g_mqtthandle.params.info.security_type=MQTT_SECURITY_TYPE_NONE;
	handle.sub_topic=(char *)atiny_malloc(8);
	strcpy(handle.sub_topic,"abc");
	handle.dynamic_info.connection_update_flag=1;
	handle.dynamic_info.state=MQTT_CONNECT_WITH_PRODUCT_ID;
	
	(*test_phandle).init_flag=0;
	
	stubInfo stub_flashwr;
	setStub((void *)flash_manager_write, (void *)stub_flash_manager_write, &stub_flashwr);
	atiny_mqtt_init(&(handle.params),&test_phandle);
	device_info.connection_type=MQTT_STATIC_CONNECT;
	device_info.codec_mode=MQTT_CODEC_MODE_JASON;
	device_info.sign_type=MQTT_SIGN_TYPE_HMACSHA256_CHECK_TIME;
	device_info.password=(char *)atiny_malloc(8);
	strcpy(device_info.password,"abc");
    device_info.u.s_info.deviceid="hdell";
	device_info.connection_type=MQTT_DYNAMIC_CONNECT;
	device_info.u.d_info.productid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.productid,"lgko");
	device_info.u.d_info.nodeid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.nodeid,"nodeid");
	struct atiny_mqtt_bind_t mqtt_bind_para;
	mqtt_bind_para.device_info=&device_info;
	mqtt_bind_para.handle=test_phandle;

	stubInfo stub_info;
	stubInfo stub_info1;
	stubInfo stub_mci;
	stubInfo stub_info_netcon;
	stubInfo stub_mqttcon;
	stubInfo stub_mqttsub;
	stubInfo stub_mqttyie;
	stubInfo stub_jsonp;
	stubInfo stub_jsonitm;
	stubInfo stub_jsondel;
	stubInfo stub_mbhc;
	stubInfo stub_mpwm;
	stubInfo stub_netdis;
	
	setStub((void *)flash_manager_read, (void *)stub_flash_manager_read, &stub_info);
	setStub((void *)NetworkInit, (void *)stub_NetworkInit, &stub_info1);
	setStub((void *)MQTTClientInit, (void *)stub_MQTTClientInit, &stub_mci);
	setStub((void *)NetworkConnect, (void *)stub_NetworkConnect, &stub_info_netcon);
	setStub((void *)MQTTConnect, (void *)stub_MQTTConnect, &stub_mqttcon);
	setStub((void *)MQTTSubscribe, (void *)stub_MQTTSubscribe, &stub_mqttsub);
	setStub((void *)MQTTYield, (void *)stub_MQTTYield, &stub_mqttyie);
	setStub((void *)cJSON_Parse, (void *)stub_cJSON_Parse, &stub_jsonp);
	setStub((void *)cJSON_GetObjectItem, (void *)stub_cJSON_GetObjectItem_fail, &stub_jsonitm);
	setStub((void *)cJSON_Delete, (void *)stub_cJSON_Delete, &stub_jsondel);
	setStub((void *)mbedtls_hmac_calc, (void *)stub_mbedtls_hmac_calc, &stub_mbhc);
	setStub((void *)MQTTPublish, (void *)stub_MQTTPublish, &stub_mpwm);
	setStub((void *)NetworkDisconnect,(void *)stub_NetworkDisconnect, &stub_netdis);
	
	ret = pthread_create(&first_thread, NULL, pthread_mqtt_test1, (void *)&mqtt_bind_para);
	TEST_ASSERT_MSG((0 == ret), "atiny_bind(...) failed");
	usleep(10000);
	usleep(10000);
	pthread_cancel(first_thread);
	pthread_join(first_thread, NULL); 
	
	cleanStub(&stub_netdis);
   	cleanStub(&stub_mpwm);
    cleanStub(&stub_mbhc);
    cleanStub(&stub_flashwr);
    cleanStub(&stub_jsondel);
    cleanStub(&stub_jsonitm);
    cleanStub(&stub_jsonp);
    cleanStub(&stub_mqttyie);
    cleanStub(&stub_mqttsub);
    cleanStub(&stub_mqttcon);
    cleanStub(&stub_info_netcon);
	cleanStub(&stub_mci);
	cleanStub(&stub_info1);
	cleanStub(&stub_info);

	atiny_free(device_info.password);
	atiny_free(device_info.u.d_info.productid);
	atiny_free(device_info.u.d_info.nodeid);
	atiny_free(handle.params.server_port);
	atiny_free(handle.sub_topic);
	atiny_free(handle.params.server_ip);
	#if 0
	atiny_free(test_phandle->sub_topic);
	atiny_free(test_phandle->params.server_ip);
	atiny_free(test_phandle->params.server_port);
	atiny_free(test_phandle->dynamic_info.got_passward);
	atiny_free(test_phandle->dynamic_info.save_info.deviceid);
    atiny_free(test_phandle->device_info.u.d_info.nodeid);
	atiny_free(test_phandle->device_info.u.d_info.productid);
	
	atiny_free(test_phandle->device_info.password);
	#endif
	//atiny_free(jsp1);
	//atiny_free(jsp2);
	//atiny_free(jsp3);
}

void TestMQTT_Client::test_atiny_mqtt_json_fail2()
{
    int ret;
	mqtt_device_info_s device_info; 
	mqtt_client_s handle;
	mqtt_client_s *phandle=&handle;

	pthread_t first_thread;
	handle.params.server_ip=(char *)atiny_malloc(16);
	strcpy(handle.params.server_ip,"192.168.1.102");
	handle.params.server_port=(char *)atiny_malloc(8);
	strcpy(handle.params.server_port,"5683");
	handle.params.info.security_type=MQTT_SECURITY_TYPE_NONE;
	handle.params.cmd_ioctl=cmd_ioctl_exam;
	g_mqtthandle.params.info.security_type=MQTT_SECURITY_TYPE_NONE;
	handle.sub_topic=(char *)atiny_malloc(8);
	strcpy(handle.sub_topic,"abc");
	handle.dynamic_info.connection_update_flag=1;
	handle.dynamic_info.state=MQTT_CONNECT_WITH_PRODUCT_ID;
	
	(*test_phandle).init_flag=0;
	
	stubInfo stub_flashwr;
	setStub((void *)flash_manager_write, (void *)stub_flash_manager_write, &stub_flashwr);
	atiny_mqtt_init(&(handle.params),&test_phandle);
	device_info.connection_type=MQTT_STATIC_CONNECT;
	device_info.codec_mode=MQTT_CODEC_MODE_JASON;
	device_info.sign_type=MQTT_SIGN_TYPE_HMACSHA256_CHECK_TIME;
	device_info.password=(char *)atiny_malloc(8);
	strcpy(device_info.password,"abc");
    device_info.u.s_info.deviceid="hdell";
	device_info.connection_type=MQTT_DYNAMIC_CONNECT;
	device_info.u.d_info.productid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.productid,"lgko");
	device_info.u.d_info.nodeid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.nodeid,"nodeid");
	struct atiny_mqtt_bind_t mqtt_bind_para;
	mqtt_bind_para.device_info=&device_info;
	mqtt_bind_para.handle=test_phandle;

	stubInfo stub_info;
	stubInfo stub_info1;
	stubInfo stub_mci;
	stubInfo stub_info_netcon;
	stubInfo stub_mqttcon;
	stubInfo stub_mqttsub;
	stubInfo stub_mqttyie;
	stubInfo stub_jsonp;
	stubInfo stub_jsonitm;
	stubInfo stub_jsondel;
	stubInfo stub_mbhc;
	stubInfo stub_mpwm;
	stubInfo stub_netdis;
	
	setStub((void *)flash_manager_read, (void *)stub_flash_manager_read, &stub_info);
	setStub((void *)NetworkInit, (void *)stub_NetworkInit, &stub_info1);
	setStub((void *)MQTTClientInit, (void *)stub_MQTTClientInit, &stub_mci);
	setStub((void *)NetworkConnect, (void *)stub_NetworkConnect, &stub_info_netcon);
	setStub((void *)MQTTConnect, (void *)stub_MQTTConnect, &stub_mqttcon);
	setStub((void *)MQTTSubscribe, (void *)stub_MQTTSubscribe, &stub_mqttsub);
	setStub((void *)MQTTYield, (void *)stub_MQTTYield, &stub_mqttyie);
	setStub((void *)cJSON_Parse, (void *)stub_cJSON_Parse, &stub_jsonp);
	setStub((void *)cJSON_GetObjectItem, (void *)stub_cJSON_GetObjectItem_fail2, &stub_jsonitm);
	setStub((void *)cJSON_Delete, (void *)stub_cJSON_Delete, &stub_jsondel);
	setStub((void *)mbedtls_hmac_calc, (void *)stub_mbedtls_hmac_calc, &stub_mbhc);
	setStub((void *)MQTTPublish, (void *)stub_MQTTPublish, &stub_mpwm);
	setStub((void *)NetworkDisconnect,(void *)stub_NetworkDisconnect, &stub_netdis);
	
	ret = pthread_create(&first_thread, NULL, pthread_mqtt_test1, (void *)&mqtt_bind_para);
	TEST_ASSERT_MSG((0 == ret), "atiny_bind(...) failed");
	usleep(10000);
	usleep(10000);
	pthread_cancel(first_thread);
	pthread_join(first_thread, NULL); 
	
	cleanStub(&stub_netdis);
   	cleanStub(&stub_mpwm);
    cleanStub(&stub_mbhc);
    cleanStub(&stub_flashwr);
    cleanStub(&stub_jsondel);
    cleanStub(&stub_jsonitm);
    cleanStub(&stub_jsonp);
    cleanStub(&stub_mqttyie);
    cleanStub(&stub_mqttsub);
    cleanStub(&stub_mqttcon);
    cleanStub(&stub_info_netcon);
	cleanStub(&stub_mci);
	cleanStub(&stub_info1);
	cleanStub(&stub_info);

	atiny_free(device_info.password);
	atiny_free(device_info.u.d_info.productid);
	atiny_free(device_info.u.d_info.nodeid);
	atiny_free(handle.params.server_port);
	atiny_free(handle.sub_topic);
	atiny_free(handle.params.server_ip);
	
	atiny_free(test_phandle->sub_topic);
	#if 0
	atiny_free(test_phandle->params.server_ip);
	atiny_free(test_phandle->params.server_port);
	atiny_free(test_phandle->dynamic_info.got_passward);
	atiny_free(test_phandle->dynamic_info.save_info.deviceid);
    atiny_free(test_phandle->device_info.u.d_info.nodeid);
	atiny_free(test_phandle->device_info.u.d_info.productid);
	
	atiny_free(test_phandle->device_info.password);
	#endif
	atiny_free(jsp1);
	//atiny_free(jsp2);
	//atiny_free(jsp3);
}




void TestMQTT_Client::test_atiny_mqtt_bind_add()
{
    int ret;
	mqtt_device_info_s device_info; 
	mqtt_client_s handle;
	mqtt_client_s *phandle=&handle;

	pthread_t first_thread;

	handle.params.server_ip=(char *)atiny_malloc(16);
	strcpy(handle.params.server_ip,"192.168.1.102");
	handle.params.server_port=(char *)atiny_malloc(8);
	strcpy(handle.params.server_port,"5683");
	handle.params.info.security_type=MQTT_SECURITY_TYPE_NONE;
	handle.params.cmd_ioctl=cmd_ioctl_exam;
	g_mqtthandle.params.info.security_type=MQTT_SECURITY_TYPE_NONE;
	handle.sub_topic=(char *)atiny_malloc(8);
	strcpy(handle.sub_topic,"abc");
	handle.dynamic_info.connection_update_flag=1;
	handle.dynamic_info.state=MQTT_CONNECT_WITH_PRODUCT_ID;
	
	

	
	//stubInfo stub_flashwr;
	//setStub((void *)flash_manager_write, (void *)stub_flash_manager_write, &stub_flashwr);
	//atiny_mqtt_init(&(handle.params),&test_phandle);
	device_info.connection_type=MQTT_STATIC_CONNECT;
	device_info.codec_mode=MQTT_CODEC_MODE_JASON;
	device_info.sign_type=MQTT_SIGN_TYPE_HMACSHA256_CHECK_TIME;
	device_info.password=(char *)atiny_malloc(8);
	strcpy(device_info.password,"abc");
    device_info.u.s_info.deviceid="hdell";
	device_info.connection_type=MQTT_DYNAMIC_CONNECT;
	device_info.u.d_info.productid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.productid,"lgko");
	device_info.u.d_info.nodeid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.nodeid,"nodeid");

	struct atiny_mqtt_bind_t mqtt_bind_para;
	mqtt_bind_para.device_info=&device_info;
	mqtt_bind_para.handle=&handle;

	stubInfo stub_info;
	stubInfo stub_info1;
	stubInfo stub_mci;
	stubInfo stub_info_netcon;
	stubInfo stub_mqttcon;
	stubInfo stub_mqttsub;
	stubInfo stub_mqttyie;
	//stubInfo stub_jsonp;
	//stubInfo stub_jsonitm;
	//stubInfo stub_jsondel;
	stubInfo stub_mbhc;
	stubInfo stub_mpwm;
	stubInfo stub_time;
	stubInfo stub_netdis;
	
	setStub((void *)flash_manager_read, (void *)stub_flash_manager_read, &stub_info);
	setStub((void *)NetworkInit, (void *)stub_NetworkInit, &stub_info1);
	setStub((void *)MQTTClientInit, (void *)stub_MQTTClientInit, &stub_mci);
	setStub((void *)NetworkConnect, (void *)stub_NetworkConnect, &stub_info_netcon);
	setStub((void *)MQTTConnect, (void *)stub_MQTTConnect, &stub_mqttcon);
	setStub((void *)MQTTSubscribe, (void *)stub_MQTTSubscribe_dir, &stub_mqttsub);
	setStub((void *)MQTTYield, (void *)stub_MQTTYield2, &stub_mqttyie);
	//setStub((void *)cJSON_Parse, (void *)stub_cJSON_Parse, &stub_jsonp);
	//setStub((void *)cJSON_GetObjectItem, (void *)stub_cJSON_GetObjectItem, &stub_jsonitm);
	//setStub((void *)cJSON_Delete, (void *)stub_cJSON_Delete, &stub_jsondel);
	setStub((void *)mbedtls_hmac_calc, (void *)stub_mbedtls_hmac_calc, &stub_mbhc);
	setStub((void *)MQTTPublish, (void *)stub_MQTTPublish, &stub_mpwm);
	setStub((void *)TimerIsExpired, (void *)stub_TimerIsExpired, &stub_time);
	//char TimerIsExpired(Timer *timer)
	setStub((void *)NetworkDisconnect,(void *)stub_NetworkDisconnect, &stub_netdis);
	
	ret = pthread_create(&first_thread, NULL, pthread_mqtt_test3, (void *)&mqtt_bind_para);
	TEST_ASSERT_MSG((0 == ret), "atiny_bind(...) failed");
	usleep(10000);
	//usleep(10000);
	pthread_cancel(first_thread);
	pthread_join(first_thread, NULL); 
	
	cleanStub(&stub_netdis);
	cleanStub(&stub_time);
   	cleanStub(&stub_mpwm);
    cleanStub(&stub_mbhc);
   // cleanStub(&stub_flashwr);
   // cleanStub(&stub_jsondel);
    //cleanStub(&stub_jsonitm);
   // cleanStub(&stub_jsonp);
    cleanStub(&stub_mqttyie);
    cleanStub(&stub_mqttsub);
    cleanStub(&stub_mqttcon);
    cleanStub(&stub_info_netcon);
	cleanStub(&stub_mci);
	cleanStub(&stub_info1);
	cleanStub(&stub_info);

	atiny_free(device_info.password);
	atiny_free(device_info.u.d_info.productid);
	atiny_free(device_info.u.d_info.nodeid);
	atiny_free(handle.params.server_port);
	atiny_free(handle.sub_topic);
	atiny_free(handle.params.server_ip);
	#if 0
	atiny_free(test_phandle->sub_topic);
	atiny_free(test_phandle->params.server_ip);
	atiny_free(test_phandle->params.server_port);
	atiny_free(test_phandle->dynamic_info.got_passward);
	atiny_free(test_phandle->dynamic_info.save_info.deviceid);
    atiny_free(test_phandle->device_info.u.d_info.nodeid);
	atiny_free(test_phandle->device_info.u.d_info.productid);
	atiny_free(test_phandle->device_info.password);
	#endif
	//atiny_free(jsp1);
	//atiny_free(jsp2);
	//atiny_free(jsp3);
}

void TestMQTT_Client::test_mqtt_proc_connect_nack()
{

    int ret;
	mqtt_device_info_s device_info; 
	mqtt_client_s handle;

	device_info.connection_type=MQTT_DYNAMIC_CONNECT;
	device_info.codec_mode=MQTT_CODEC_MODE_JASON;
	device_info.sign_type=MQTT_SIGN_TYPE_HMACSHA256_CHECK_TIME;
	device_info.password=(char *)atiny_malloc(8);
	strcpy(device_info.password,"abc");
	
    device_info.u.s_info.deviceid="hdell";
	device_info.connection_type=MQTT_DYNAMIC_CONNECT;
	device_info.u.d_info.productid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.productid,"lgko");
	pthread_t first_thread;
	device_info.u.d_info.nodeid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.nodeid,"nodeid");
	handle.params.server_ip=(char *)atiny_malloc(16);
	strcpy(handle.params.server_ip,"192.168.1.102");
	handle.params.server_port=(char *)atiny_malloc(8);
	strcpy(handle.params.server_port,"5683");
	g_mqtthandle.params.info.security_type=MQTT_SECURITY_TYPE_NONE;
	struct atiny_mqtt_bind_t mqtt_bind_para;
	mqtt_bind_para.device_info=&device_info;
	mqtt_bind_para.handle=&handle;
	handle.sub_topic=(char *)atiny_malloc(8);
	strcpy(handle.sub_topic,"abc");
	handle.dynamic_info.state=MQTT_CONNECT_WITH_DEVICE_ID;
	
	stubInfo stub_info;
	stubInfo stub_info1;
	stubInfo stub_info_netcon;
	stubInfo stub_mbhc;
	stubInfo stub_mqttcon;
	stubInfo stub_mqttsub;
	stubInfo stub_mqttyie;
	
	setStub((void *)flash_manager_read, (void *)stub_flash_manager_read, &stub_info);
	setStub((void *)NetworkInit, (void *)stub_NetworkInit, &stub_info1);
	setStub((void *)NetworkConnect, (void *)stub_NetworkConnect, &stub_info_netcon);
	setStub((void *)mbedtls_hmac_calc, (void *)stub_mbedtls_hmac_calc, &stub_mbhc);
	setStub((void *)MQTTConnect, (void *)stub_MQTTConnect_fail, &stub_mqttcon);
	setStub((void *)MQTTSubscribe, (void *)stub_MQTTSubscribe, &stub_mqttsub);
	setStub((void *)MQTTYield, (void *)stub_MQTTYield, &stub_mqttyie);
	
	ret = pthread_create(&first_thread, NULL, pthread_mqtt_test, (void *)&mqtt_bind_para);
	TEST_ASSERT_MSG((0 == ret), "atiny_bind(...) failed");
	usleep(10000);
	usleep(10000);
    pthread_cancel(first_thread);
    pthread_join(first_thread, NULL); 
	
    cleanStub(&stub_mqttyie);
    cleanStub(&stub_mqttsub);
    cleanStub(&stub_mqttcon);
	cleanStub(&stub_mbhc);
    cleanStub(&stub_info_netcon);
	cleanStub(&stub_info1);
	cleanStub(&stub_info);
	
	atiny_free(device_info.password);
	atiny_free(device_info.u.d_info.productid);
	atiny_free(device_info.u.d_info.nodeid);
}


void TestMQTT_Client::test_mqtt_proc_connect_nack1()
{

    int ret;
	mqtt_device_info_s device_info; 
	mqtt_client_s handle;
	device_info.connection_type=MQTT_DYNAMIC_CONNECT;
	device_info.codec_mode=MQTT_CODEC_MODE_JASON;
	device_info.sign_type=MQTT_SIGN_TYPE_HMACSHA256_CHECK_TIME;
	device_info.password=(char *)atiny_malloc(8);
	strcpy(device_info.password,"abc");
    device_info.u.s_info.deviceid="hdell";
	device_info.connection_type=MQTT_DYNAMIC_CONNECT;
	device_info.u.d_info.productid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.productid,"lgko");
	
	pthread_t first_thread;
	device_info.u.d_info.nodeid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.nodeid,"nodeid");
	handle.params.server_ip=(char *)atiny_malloc(16);
	strcpy(handle.params.server_ip,"192.168.1.102");
	handle.params.server_port=(char *)atiny_malloc(8);
	strcpy(handle.params.server_port,"5683");
	g_mqtthandle.params.info.security_type=MQTT_SECURITY_TYPE_NONE;
	
	struct atiny_mqtt_bind_t mqtt_bind_para;
	mqtt_bind_para.device_info=&device_info;
	mqtt_bind_para.handle=&handle;
	handle.sub_topic=(char *)atiny_malloc(8);
	strcpy(handle.sub_topic,"abc");
	handle.dynamic_info.state=MQTT_CONNECT_WITH_PRODUCT_ID;
	handle.dynamic_info.has_device_id=1;

    stubInfo stub_info;
	stubInfo stub_info1;
	stubInfo stub_mci;
	stubInfo stub_info_netcon;
	stubInfo stub_mbhc;
	stubInfo stub_mqttcon;
	stubInfo stub_mqttsub;
	stubInfo stub_mqttyie;
	
	setStub((void *)flash_manager_read, (void *)stub_flash_manager_read, &stub_info);
	setStub((void *)NetworkInit, (void *)stub_NetworkInit, &stub_info1);
	setStub((void *)MQTTClientInit, (void *)stub_MQTTClientInit, &stub_mci);
	setStub((void *)NetworkConnect, (void *)stub_NetworkConnect, &stub_info_netcon);
	setStub((void *)mbedtls_hmac_calc, (void *)stub_mbedtls_hmac_calc, &stub_mbhc);
	setStub((void *)MQTTConnect, (void *)stub_MQTTConnect_fail, &stub_mqttcon);
	setStub((void *)MQTTSubscribe, (void *)stub_MQTTSubscribe, &stub_mqttsub);
	setStub((void *)MQTTYield, (void *)stub_MQTTYield, &stub_mqttyie);
	
	ret = pthread_create(&first_thread, NULL, pthread_mqtt_test, (void *)&mqtt_bind_para);
	TEST_ASSERT_MSG((0 == ret), "atiny_bind(...) failed");
	usleep(10000);
	usleep(10000);
    pthread_cancel(first_thread);
    pthread_join(first_thread, NULL); 
	
    cleanStub(&stub_mqttyie);
    cleanStub(&stub_mqttsub);
    cleanStub(&stub_mqttcon);
	cleanStub(&stub_mbhc);
    cleanStub(&stub_info_netcon);
	cleanStub(&stub_mci);
	cleanStub(&stub_info1);
	cleanStub(&stub_info);
	
	atiny_free(device_info.password);
	atiny_free(device_info.u.d_info.productid);
	atiny_free(device_info.u.d_info.nodeid);
}


void TestMQTT_Client::test_mqtt_clientInit_fail()
{
    int ret;
	mqtt_device_info_s device_info; 
	mqtt_client_s handle;

	device_info.connection_type=MQTT_STATIC_CONNECT;
	device_info.codec_mode=MQTT_CODEC_MODE_JASON;
	device_info.sign_type=MQTT_SIGN_TYPE_HMACSHA256_CHECK_TIME;
	device_info.password=(char *)atiny_malloc(8);
	strcpy(device_info.password,"abc");
    device_info.u.s_info.deviceid="hdell";
	device_info.connection_type=MQTT_DYNAMIC_CONNECT;
	device_info.u.d_info.productid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.productid,"lgko");
	
	pthread_t first_thread;
	device_info.u.d_info.nodeid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.nodeid,"nodeid");
	handle.params.server_ip=(char *)atiny_malloc(16);
	strcpy(handle.params.server_ip,"192.168.1.102");
	handle.params.server_port=(char *)atiny_malloc(8);
	strcpy(handle.params.server_port,"5683");
	g_mqtthandle.params.info.security_type=MQTT_SECURITY_TYPE_NONE;
	
	struct atiny_mqtt_bind_t mqtt_bind_para;
	mqtt_bind_para.device_info=&device_info;
	mqtt_bind_para.handle=&handle;
	handle.sub_topic=(char *)atiny_malloc(8);
	strcpy(handle.sub_topic,"abc");
	handle.dynamic_info.connection_update_flag=1;

	stubInfo stub_info;
	stubInfo stub_info1;
	stubInfo stub_mci;
	stubInfo stub_info_netcon;
	stubInfo stub_mqttcon;
	stubInfo stub_mqttsub;
	stubInfo stub_mqttyie;
	
	setStub((void *)flash_manager_read, (void *)stub_flash_manager_read, &stub_info);
	setStub((void *)NetworkInit, (void *)stub_NetworkInit, &stub_info1);
	setStub((void *)MQTTClientInit, (void *)stub_MQTTClientInit_fail, &stub_mci);
	setStub((void *)NetworkConnect, (void *)stub_NetworkConnect, &stub_info_netcon);
	setStub((void *)MQTTConnect, (void *)stub_MQTTConnect, &stub_mqttcon);
	setStub((void *)MQTTSubscribe, (void *)stub_MQTTSubscribe, &stub_mqttsub);
	setStub((void *)MQTTYield, (void *)stub_MQTTYield, &stub_mqttyie);

	ret = pthread_create(&first_thread, NULL, pthread_mqtt_test, (void *)&mqtt_bind_para);
	TEST_ASSERT_MSG((0 == ret), "atiny_bind(...) failed");
	usleep(10000);
	usleep(10000);
    pthread_cancel(first_thread);
    pthread_join(first_thread, NULL); 
	
    cleanStub(&stub_mqttyie);
    cleanStub(&stub_mqttsub);
    cleanStub(&stub_mqttcon);
    cleanStub(&stub_info_netcon);
	cleanStub(&stub_mci);
	cleanStub(&stub_info1);
	cleanStub(&stub_info);
	
	atiny_free(device_info.password);
	atiny_free(device_info.u.d_info.productid);
	atiny_free(device_info.u.d_info.nodeid);
}



void TestMQTT_Client::test_mqtt_networkconnect_fail()
{
    int ret;
	mqtt_device_info_s device_info; 
	mqtt_client_s handle;
	
	device_info.connection_type=MQTT_STATIC_CONNECT;
	device_info.codec_mode=MQTT_CODEC_MODE_JASON;
	device_info.sign_type=MQTT_SIGN_TYPE_HMACSHA256_CHECK_TIME;
	device_info.password=(char *)atiny_malloc(8);
	strcpy(device_info.password,"abc");
	device_info.u.s_info.deviceid="hdell";
	device_info.connection_type=MQTT_DYNAMIC_CONNECT;
	device_info.u.d_info.productid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.productid,"lgko");
	pthread_t first_thread;
	device_info.u.d_info.nodeid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.nodeid,"nodeid");
		
	handle.params.server_ip=(char *)atiny_malloc(16);
	strcpy(handle.params.server_ip,"192.168.1.102");
	handle.params.server_port=(char *)atiny_malloc(8);
	strcpy(handle.params.server_port,"5683");
	g_mqtthandle.params.info.security_type=MQTT_SECURITY_TYPE_NONE;
		
	struct atiny_mqtt_bind_t mqtt_bind_para;
	mqtt_bind_para.device_info=&device_info;
	mqtt_bind_para.handle=&handle;
	handle.sub_topic=(char *)atiny_malloc(8);
	strcpy(handle.sub_topic,"abc");
	handle.dynamic_info.connection_update_flag=1;
		
	stubInfo stub_info;
	stubInfo stub_info1;
	stubInfo stub_info_netcon;
	stubInfo stub_mqttcon;
	stubInfo stub_mqttsub;
	stubInfo stub_mqttyie;
		
	setStub((void *)flash_manager_read, (void *)stub_flash_manager_read, &stub_info);
	setStub((void *)NetworkInit, (void *)stub_NetworkInit, &stub_info1);
	setStub((void *)NetworkConnect, (void *)stub_NetworkConnect_fail, &stub_info_netcon);
	setStub((void *)MQTTConnect, (void *)stub_MQTTConnect, &stub_mqttcon);
	setStub((void *)MQTTSubscribe, (void *)stub_MQTTSubscribe, &stub_mqttsub);
	setStub((void *)MQTTYield, (void *)stub_MQTTYield, &stub_mqttyie);
		
	ret = pthread_create(&first_thread, NULL, pthread_mqtt_test, (void *)&mqtt_bind_para);
	TEST_ASSERT_MSG((0 == ret), "atiny_bind(...) failed");
	usleep(10000);
	usleep(10000);
	pthread_cancel(first_thread);
    pthread_join(first_thread, NULL); 
		
	cleanStub(&stub_mqttyie);
	cleanStub(&stub_mqttsub);
	cleanStub(&stub_mqttcon);
	cleanStub(&stub_info_netcon);
	cleanStub(&stub_info1);
	cleanStub(&stub_info);
	
	atiny_free(device_info.password);
	atiny_free(device_info.u.d_info.productid);
	atiny_free(device_info.u.d_info.nodeid);
}

void TestMQTT_Client::test_mqtt_cmd_ioctrlfail()
{
    int ret;
	mqtt_device_info_s device_info; 
	mqtt_client_s handle;
	mqtt_client_s *phandle=&handle;
	
	pthread_t first_thread;
	handle.params.server_ip=(char *)atiny_malloc(16);
	strcpy(handle.params.server_ip,"192.168.1.102");
	handle.params.server_port=(char *)atiny_malloc(8);
	strcpy(handle.params.server_port,"5683");
	handle.params.info.security_type=MQTT_SECURITY_TYPE_NONE;
	handle.params.cmd_ioctl=cmd_ioctl_exam;
	g_mqtthandle.params.info.security_type=MQTT_SECURITY_TYPE_NONE;
	
	handle.sub_topic=(char *)atiny_malloc(8);
	strcpy(handle.sub_topic,"abc");
	handle.dynamic_info.connection_update_flag=1;
	handle.dynamic_info.state=MQTT_CONNECT_WITH_DEVICE_ID;
	handle.device_info.connection_type=MQTT_STATIC_CONNECT;
	
	stubInfo stub_flashwr;
	stubInfo stub_info;
	stubInfo stub_info1;
	stubInfo stub_mci;
	stubInfo stub_info_netcon;
	stubInfo stub_mqttcon;
	stubInfo stub_mqttsub;
	stubInfo stub_mqttyie;
	stubInfo stub_mbhc;
	stubInfo stub_mpwm;
	stubInfo stub_netdis;
	
	setStub((void *)flash_manager_write, (void *)stub_flash_manager_write, &stub_flashwr);

	atiny_mqtt_init(&(handle.params),&test_gphandle);
	
	device_info.connection_type=MQTT_STATIC_CONNECT;
	device_info.codec_mode=MQTT_CODEC_MODE_JASON;
	device_info.sign_type=MQTT_SIGN_TYPE_HMACSHA256_CHECK_TIME;
	device_info.password=(char *)atiny_malloc(8);
	strcpy(device_info.password,"abc");
    device_info.u.s_info.deviceid="hdell";
	device_info.connection_type=MQTT_STATIC_CONNECT;
	device_info.u.d_info.productid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.productid,"lgko");
	device_info.u.d_info.nodeid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.nodeid,"nodeid");

	struct atiny_mqtt_bind_t mqtt_bind_para;
	mqtt_bind_para.device_info=&device_info;
	mqtt_bind_para.handle=test_gphandle;

	setStub((void *)flash_manager_read, (void *)stub_flash_manager_read, &stub_info);
	setStub((void *)NetworkInit, (void *)stub_NetworkInit, &stub_info1);
	setStub((void *)MQTTClientInit, (void *)stub_MQTTClientInit, &stub_mci);
	setStub((void *)NetworkConnect, (void *)stub_NetworkConnect, &stub_info_netcon);
	setStub((void *)MQTTConnect, (void *)stub_MQTTConnect, &stub_mqttcon);
	setStub((void *)MQTTSubscribe, (void *)stub_MQTTSubscribe2, &stub_mqttsub);
	setStub((void *)MQTTYield, (void *)stub_MQTTYield, &stub_mqttyie);
	setStub((void *)mbedtls_hmac_calc, (void *)stub_mbedtls_hmac_calc, &stub_mbhc);
	setStub((void *)MQTTPublish, (void *)stub_MQTTPublish, &stub_mpwm);
	setStub((void *)NetworkDisconnect,(void *)stub_NetworkDisconnect, &stub_netdis);

	ret = pthread_create(&first_thread, NULL, pthread_mqtt_test3, (void *)&mqtt_bind_para);
	TEST_ASSERT_MSG((0 == ret), "atiny_bind(...) failed");
	usleep(10000);
	usleep(10000);
	pthread_cancel(first_thread);
	pthread_join(first_thread, NULL); 

	cleanStub(&stub_netdis);
   	cleanStub(&stub_mpwm);
    cleanStub(&stub_mbhc);
    cleanStub(&stub_mqttyie);
    cleanStub(&stub_mqttsub);
    cleanStub(&stub_mqttcon);
    cleanStub(&stub_info_netcon);
	cleanStub(&stub_mci);
	cleanStub(&stub_info1);
	cleanStub(&stub_info);
	cleanStub(&stub_flashwr);

	atiny_free(device_info.password);
	atiny_free(device_info.u.d_info.productid);
	atiny_free(device_info.u.d_info.nodeid);
	atiny_free(handle.params.server_port);
	atiny_free(handle.sub_topic);
	atiny_free(handle.params.server_ip);
	
	atiny_free(test_gphandle->sub_topic);
	atiny_free(test_gphandle->params.server_ip);
	atiny_free(test_gphandle->params.server_port);
	atiny_free(test_gphandle->dynamic_info.got_passward);
	atiny_free(test_gphandle->dynamic_info.save_info.deviceid);
	atiny_free(test_gphandle->device_info.u.d_info.nodeid);
	atiny_free(test_gphandle->device_info.u.d_info.productid);
	atiny_free(test_gphandle->device_info.password);
}

void TestMQTT_Client::test_mqtt_dup_devinfo_fail()
{
    int ret;
	mqtt_device_info_s device_info; 
	mqtt_client_s handle;

	device_info.codec_mode=MQTT_CODEC_MODE_JASON;
	device_info.sign_type=MQTT_SIGN_TYPE_HMACSHA256_CHECK_TIME;
	device_info.password=(char *)atiny_malloc(8);
	strcpy(device_info.password,"abc");
    device_info.u.s_info.deviceid="hdell";
	device_info.connection_type=MQTT_DYNAMIC_CONNECT;
	device_info.u.d_info.productid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.productid,"lgko");
	device_info.u.d_info.nodeid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.nodeid,"nodeid");
	handle.params.server_ip=(char *)atiny_malloc(16);
	strcpy(handle.params.server_ip,"192.168.1.102");
	handle.params.server_port=(char *)atiny_malloc(8);
	strcpy(handle.params.server_port,"5683");
	g_mqtthandle.params.info.security_type=MQTT_SECURITY_TYPE_NONE;
	handle.sub_topic=NULL;
	handle.dynamic_info.connection_update_flag=1;
	handle.dynamic_info.got_passward=(char *)atiny_malloc(8);
	strcpy(handle.dynamic_info.got_passward,"abc");
	handle.dynamic_info.save_info.deviceid=(char *)atiny_malloc(8);
	strcpy(handle.dynamic_info.save_info.deviceid,"abc");
	
	stubInfo stub_msmh;
	
	i =  0;
	g_random = 1;

	setStub((void *)atiny_strdup, (void *)stub_atiny_strdup_fail_time, &si_atiny_strdup);
	setStub((void *)MQTTSetMessageHandler, (void *)stub_MQTTSetMessageHandler, &stub_msmh);

	ret=atiny_mqtt_bind(&device_info,&handle);
	TEST_ASSERT_MSG((0 != ret), "atiny_bind(...) failed");
	
	cleanStub(&stub_msmh);
	cleanStub(&si_atiny_strdup);
	
	atiny_free(device_info.password);
	atiny_free(device_info.u.d_info.productid);
	atiny_free(device_info.u.d_info.nodeid);
}


void TestMQTT_Client::test_mqtt_dup_info_fail()
{
    int ret;
    mqtt_device_info_s device_info; 
    mqtt_client_s handle;
 
    device_info.codec_mode=MQTT_CODEC_MODE_JASON;
    device_info.sign_type=MQTT_SIGN_TYPE_HMACSHA256_CHECK_TIME;
    device_info.password=(char *)atiny_malloc(8);
    strcpy(device_info.password,"abc");
    device_info.u.s_info.deviceid="hdell";
    device_info.connection_type=MQTT_DYNAMIC_CONNECT;
    device_info.u.d_info.productid=(char *)atiny_malloc(8);
    strcpy(device_info.u.d_info.productid,"lgko");
  
    device_info.u.d_info.nodeid=(char *)atiny_malloc(8);
    strcpy(device_info.u.d_info.nodeid,"nodeid");
    handle.params.server_ip=(char *)atiny_malloc(16);
    strcpy(handle.params.server_ip,"192.168.1.102");
    handle.params.server_port=(char *)atiny_malloc(8);
    strcpy(handle.params.server_port,"5683");
    g_mqtthandle.params.info.security_type=MQTT_SECURITY_TYPE_NONE;
   
    handle.sub_topic=NULL;
    handle.dynamic_info.connection_update_flag=1;
    handle.dynamic_info.got_passward=(char *)atiny_malloc(8);
    strcpy(handle.dynamic_info.got_passward,"abc");
    handle.dynamic_info.save_info.deviceid=(char *)atiny_malloc(8);
    strcpy(handle.dynamic_info.save_info.deviceid,"abc");
   
    stubInfo stub_msmh;
	
    i =	0;
    g_random = 2;

    setStub((void *)atiny_strdup, (void *)stub_atiny_strdup_fail_time, &si_atiny_strdup);
    setStub((void *)MQTTSetMessageHandler, (void *)stub_MQTTSetMessageHandler, &stub_msmh);

    ret=atiny_mqtt_bind(&device_info,&handle);
	TEST_ASSERT_MSG((0 != ret), "atiny_bind(...) failed");
   
    cleanStub(&stub_msmh);
    cleanStub(&si_atiny_strdup);
   
    atiny_free(device_info.password);
    atiny_free(device_info.u.d_info.productid);
    atiny_free(device_info.u.d_info.nodeid);
}


void TestMQTT_Client::test_mqtt_dup_devin_fail()
{
    int ret;
	mqtt_device_info_s device_info; 
	mqtt_client_s handle;

	device_info.codec_mode=MQTT_CODEC_MODE_JASON;
	device_info.sign_type=MQTT_SIGN_TYPE_HMACSHA256_CHECK_TIME;
	device_info.password=(char *)atiny_malloc(8);
	strcpy(device_info.password,"abc");
	device_info.u.s_info.deviceid="hdell";
	device_info.connection_type=MQTT_STATIC_CONNECT;
	device_info.u.d_info.productid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.productid,"lgko");
	   
	device_info.u.d_info.nodeid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.nodeid,"nodeid");
	   
	handle.params.server_ip=(char *)atiny_malloc(16);
	strcpy(handle.params.server_ip,"192.168.1.102");
	handle.params.server_port=(char *)atiny_malloc(8);
	strcpy(handle.params.server_port,"5683");
	g_mqtthandle.params.info.security_type=MQTT_SECURITY_TYPE_NONE;
	   
	handle.sub_topic=NULL;
	handle.dynamic_info.connection_update_flag=1;
    handle.dynamic_info.got_passward=(char *)atiny_malloc(8);
	strcpy(handle.dynamic_info.got_passward,"abc");
	handle.dynamic_info.save_info.deviceid=(char *)atiny_malloc(8);
	strcpy(handle.dynamic_info.save_info.deviceid,"abc");
	   
	stubInfo stub_msmh;
	   
	i =	0;
	g_random = 1;
	
	setStub((void *)atiny_strdup, (void *)stub_atiny_strdup_fail_time, &si_atiny_strdup);
	setStub((void *)MQTTSetMessageHandler, (void *)stub_MQTTSetMessageHandler, &stub_msmh);
	
	ret=atiny_mqtt_bind(&device_info,&handle);
	TEST_ASSERT_MSG((0 != ret), "atiny_bind(...) failed");
	   
	cleanStub(&stub_msmh);
	cleanStub(&si_atiny_strdup);
	   
	atiny_free(device_info.password);
	atiny_free(device_info.u.d_info.productid);
	atiny_free(device_info.u.d_info.nodeid);
}

void TestMQTT_Client::test_atiny_mqtt_bind()
{
    int ret;
	mqtt_device_info_s device_info; 
	mqtt_client_s handle;

	handle.sub_topic=(char *)atiny_malloc(8);
	strcpy(handle.sub_topic,"abc");
	stubInfo stub_msmh;
	setStub((void *)MQTTSetMessageHandler, (void *)stub_MQTTSetMessageHandler, &stub_msmh);

	ret=atiny_mqtt_bind(NULL,&handle);
	TEST_ASSERT_MSG((ret == ATINY_ARG_INVALID), "atiny_mqtt_bind(...) failed");
	cleanStub(&stub_msmh);

	device_info.connection_type=MQTT_STATIC_CONNECT;
	device_info.codec_mode=MQTT_CODEC_MODE_JASON;
	device_info.sign_type=MQTT_SIGN_TYPE_HMACSHA256_CHECK_TIME;
	device_info.password=NULL;
	atiny_mqtt_bind(&device_info,&handle);

	device_info.password=(char *)atiny_malloc(8);
	strcpy(device_info.password,"abc");
	device_info.u.s_info.deviceid=NULL;
	atiny_mqtt_bind(&device_info,&handle);
	
    device_info.u.s_info.deviceid="hdell";
	device_info.connection_type=MQTT_DYNAMIC_CONNECT;
	device_info.u.d_info.productid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.productid,"lgko");
	device_info.u.d_info.nodeid=NULL;
	atiny_mqtt_bind(&device_info,&handle);

	pthread_t first_thread;

	device_info.u.d_info.nodeid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.nodeid,"nodeid");
	handle.params.server_ip=(char *)atiny_malloc(16);
	strcpy(handle.params.server_ip,"192.168.1.102");
	handle.params.server_port=(char *)atiny_malloc(8);
	strcpy(handle.params.server_port,"5683");
	g_mqtthandle.params.info.security_type=MQTT_SECURITY_TYPE_NONE;

    struct atiny_mqtt_bind_t mqtt_bind_para;
	mqtt_bind_para.device_info=&device_info;
	mqtt_bind_para.handle=&handle;
	
	handle.dynamic_info.connection_update_flag=1;
	
	stubInfo stub_info;
	stubInfo stub_info1;
	stubInfo stub_info_netcon;
	stubInfo stub_mqttcon;
	stubInfo stub_mqttsub;
	stubInfo stub_mqttyie;
	
	setStub((void *)flash_manager_read, (void *)stub_flash_manager_read, &stub_info);
	setStub((void *)NetworkInit, (void *)stub_NetworkInit, &stub_info1);
	setStub((void *)NetworkConnect, (void *)stub_NetworkConnect, &stub_info_netcon);
	setStub((void *)MQTTConnect, (void *)stub_MQTTConnect, &stub_mqttcon);
	setStub((void *)MQTTSubscribe, (void *)stub_MQTTSubscribe, &stub_mqttsub);
	setStub((void *)MQTTYield, (void *)stub_MQTTYield, &stub_mqttyie);

	ret = pthread_create(&first_thread, NULL, pthread_mqtt_test, (void *)&mqtt_bind_para);
	TEST_ASSERT_MSG((0 == ret), "atiny_bind(...) failed");
	usleep(10000);
	usleep(10000);
    pthread_cancel(first_thread);
    pthread_join(first_thread, NULL); 
	
    cleanStub(&stub_mqttyie);
    cleanStub(&stub_mqttsub);
    cleanStub(&stub_mqttcon);
    cleanStub(&stub_info_netcon);
	cleanStub(&stub_info1);
	cleanStub(&stub_info);

	atiny_free(device_info.password);
	atiny_free(device_info.u.d_info.productid);
	atiny_free(device_info.u.d_info.nodeid);
}

void TestMQTT_Client::test_atiny_mqtt_read_flash()
{
    int ret;
	mqtt_device_info_s device_info; 
	mqtt_client_s handle;

	handle.sub_topic=(char *)atiny_malloc(8);
	strcpy(handle.sub_topic,"abc");
	
	device_info.connection_type=MQTT_STATIC_CONNECT;
	device_info.codec_mode=MQTT_CODEC_MODE_JASON;
	device_info.sign_type=MQTT_SIGN_TYPE_HMACSHA256_CHECK_TIME;
	device_info.password=(char *)atiny_malloc(8);
	strcpy(device_info.password,"abc");
    device_info.u.s_info.deviceid="hdell";
	device_info.connection_type=MQTT_DYNAMIC_CONNECT;
	device_info.u.d_info.productid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.productid,"lgko");

	pthread_t first_thread;

	device_info.u.d_info.nodeid=(char *)atiny_malloc(8);
	strcpy(device_info.u.d_info.nodeid,"nodeid");
	handle.params.server_ip=(char *)atiny_malloc(16);
	strcpy(handle.params.server_ip,"192.168.1.102");
	handle.params.server_port=(char *)atiny_malloc(8);
	strcpy(handle.params.server_port,"5683");
	g_mqtthandle.params.info.security_type=MQTT_SECURITY_TYPE_NONE;

    struct atiny_mqtt_bind_t mqtt_bind_para;
	mqtt_bind_para.device_info=&device_info;
	mqtt_bind_para.handle=&handle;
	
	handle.dynamic_info.connection_update_flag=1;
	
	stubInfo stub_info;
	stubInfo stub_info1;
	stubInfo stub_mclf;
	stubInfo stub_info_netcon;
	stubInfo stub_mqttcon;
	stubInfo stub_mqttsub;
	stubInfo stub_mqttyie;
	
	setStub((void *)flash_manager_read, (void *)stub_flash_manager_read2, &stub_info);
	setStub((void *)NetworkInit, (void *)stub_NetworkInit, &stub_info1);
	setStub((void *)MQTTClientInit, (void *)stub_MQTTClientInit_fail, &stub_mclf);
	setStub((void *)NetworkConnect, (void *)stub_NetworkConnect, &stub_info_netcon);
	setStub((void *)MQTTConnect, (void *)stub_MQTTConnect, &stub_mqttcon);
	setStub((void *)MQTTSubscribe, (void *)stub_MQTTSubscribe, &stub_mqttsub);
	setStub((void *)MQTTYield, (void *)stub_MQTTYield, &stub_mqttyie);

	ret = pthread_create(&first_thread, NULL, pthread_mqtt_test, (void *)&mqtt_bind_para);
	TEST_ASSERT_MSG((0 == ret), "atiny_bind(...) failed");
	usleep(10000);
	usleep(10000);
    pthread_cancel(first_thread);
    pthread_join(first_thread, NULL); 
	
    cleanStub(&stub_mqttyie);
    cleanStub(&stub_mqttsub);
    cleanStub(&stub_mqttcon);
    cleanStub(&stub_info_netcon);
	cleanStub(&stub_mclf);
	cleanStub(&stub_info1);
	cleanStub(&stub_info);

	atiny_free(device_info.password);
	atiny_free(device_info.u.d_info.productid);
	atiny_free(device_info.u.d_info.nodeid);
}

void TestMQTT_Client::setup()
{
    printf("come into %d func, in file %s\n", ++funcno, __FILE__);
}

void TestMQTT_Client::tear_down()
{
    printf("exit %d func, in file %s\n", funcno, __FILE__);
}


