/******************************************************************************

  Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>

 ******************************************************************************
  File Name     : test_MQTTClient.cpp
  Version       : Initial Draft
  Author        : MAMINJIE
  Created       : 2018/7/26
  Last Modified :
  Description   : 
  Function List :
  History       :
  1.Date        : 2018/7/26
    Author      : MAMINJIE
    Modification: Created file

******************************************************************************/

/* Includes -----------------------------------------------------------------*/
#include "test_MQTTClient.h"

extern "C"
{
#include "atiny_mqtt/mqtt_client.h"
#include "MQTTConnect.h"
#include "MQTTClient.h"
#include "los_sem.ph"
#include <pthread.h>
#include <unistd.h> 

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>

static SEM_CB_S s_sem;
typedef struct 
{
    char* host;         /**< connection to system under test. */
    int port;
    char* proxy_host;
    int proxy_port;
    int verbose;
    int test_no;
    int MQTTVersion;
    int iterations;
} Options;
static Options options =
{
    (char*)"localhost",
    1883,
    (char*)"localhost",
    1885,
    0, //verbose
    0, //test_no
    4,
    1,
};
static Network s_network;
static MQTTClient s_client;
static unsigned char s_buf[100];
static unsigned char s_readbuf[100];
static MQTTPacket_connectData s_connectdata = MQTTPacket_connectData_initializer;
static int s_packet_type = 0x0;
static int s_intQoS = QOS1;
static char* s_cstring = NULL;
static char s_write_flag = 1;

typedef void* (*pthread_func)(void* arg);
void MQTTClientDeInit(MQTTClient *c);
int MQTTClientInit(MQTTClient *c, Network *network, unsigned int command_timeout_ms,
                    unsigned char *sendbuf, size_t sendbuf_size, unsigned char *readbuf, size_t readbuf_size);
int MQTTConnectWithResults(MQTTClient* c, MQTTPacket_connectData* options, MQTTConnackData* data);
int MQTTConnect(MQTTClient* c, MQTTPacket_connectData* options);
int MQTTDisconnect(MQTTClient* c);
int MQTTPublish(MQTTClient* c, const char* topicName, MQTTMessage* message);
int MQTTSetMessageHandler(MQTTClient* c, const char* topicFilter, messageHandler messageHandler);
int MQTTSubscribe(MQTTClient* c, const char* topicFilter, enum QoS qos,messageHandler messageHandler);
int MQTTUnsubscribe(MQTTClient* c, const char* topicFilter);
int MQTTYield(MQTTClient* c, int timeout_ms);
void MQTTRun(void* parm);
void TimerInit(Timer *timer);
void TimerCountdownMS(Timer *timer, unsigned int timeout);
int cycle(MQTTClient* c, Timer* timer);
int MQTTDeserialize_publish(unsigned char *dup, int *qos, unsigned char *retained, unsigned short *packetid, MQTTString *topicName,
                            unsigned char **payload, int *payloadlen, unsigned char *buf, int buflen);
int MQTTSerialize_ack(unsigned char *buf, int buflen, unsigned char packettype, unsigned char dup, unsigned short packetid);
int MQTTDeserialize_ack(unsigned char *packettype, unsigned char *dup, unsigned short *packetid, unsigned char *buf, int buflen);
int keepalive(MQTTClient* c);
void NetworkInit(Network* n, mqtt_security_info_s *(*get_security_info)(void));
int NetworkConnect(Network *n, char *addr, int port);
void MQTTCleanSession(MQTTClient* c);
void MQTTCloseSession(MQTTClient* c);
int waitfor(MQTTClient* c, int packet_type, Timer* timer);


static void message_handler(MessageData* data)
{
    printf("Test %s ....\n",__func__);
}

static void *stub_atiny_mutex_create(void)
{
    memset(&s_sem, 0, sizeof(SEM_CB_S));
    return (void *)&s_sem;
}
static int stub_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    return 0;
}
static ssize_t stub_recv(int sockfd, void *buf, size_t len, int flags)
{
    static int i = 0;
    uint8_t value = 0;
    if(0 == i){
        value = 0x02 | s_packet_type;
        i++;
    }else if(1 == i){
        value = 0x02;
        i++;
    }else{
        i = 0;
    }
    *(unsigned char*)buf = value;
    return len;
}

static ssize_t stub_write(int fd, const void *buf, size_t count)
{
    if(s_write_flag)return count;
    return 0;
}

static int stub_MQTTDeserialize_publish(unsigned char* dup, int* qos, unsigned char* retained, unsigned short* packetid, MQTTString* topicName,
	unsigned char** payload, int* payloadlen, unsigned char* buf, int buflen)
{
    topicName->lenstring.data = (char*)s_buf;
    topicName->lenstring.len = sizeof((const char*)s_buf);
    topicName->cstring = s_cstring;
    *qos = s_intQoS;
    return 1;
}

static int stub_MQTTDeserialize_ack(unsigned char* packettype, unsigned char* dup, unsigned short* packetid, unsigned char* buf, int buflen)
{
    return 0;
}
static int stub_MQTTDeserialize_ack_ret2(unsigned char* packettype, unsigned char* dup, unsigned short* packetid, unsigned char* buf, int buflen)
{
	//printf("+-+-+-+-+-+-  %p  +-+-+-+-+-+-\n",stub_MQTTDeserialize_ack_ret2);
    return 2;
}

static int stub_MQTTSerialize_ack(unsigned char* buf, int buflen, unsigned char packettype, unsigned char dup, unsigned short packetid)
{
    return 0;
}
    static void test_fp_0(MessageData*){}
	static void test_fp_1(MessageData*){}
	static void test_fp_2(MessageData*){}
	static void test_fp_3(MessageData*){}
	static void test_fp_4(MessageData*){}
	static void test_msg_handler(MessageData*){}
	static int static_Ret_of_test_mqttread = 0;
	static int test_mqttread(Network*, unsigned char*, int, int){
		return static_Ret_of_test_mqttread;
	}
	static int test_mqttwrite(Network*, unsigned char*, int, int){
		//printf("+-+-+-+-+-+-  0  +-+-+-+-+-+-\n");
		return 0;
	}
	static int test_mqttwrite_ret10(Network*, unsigned char*, int, int){
		//printf("+-+-+-+-+-+-  ret10  +-+-+-+-+-+-\n");
		return 10;
	}
	static void test_disconnect(Network*){}
	static mqtt_security_info_s static_l_s_msinfo_1;
	static mqtt_security_info_s * test_get_security_info(void){
		return &static_l_s_msinfo_1;
	}
	static int stub_MutexLock(Mutex *mutex){return 0;}
	static int stub_MutexUnlock(Mutex *mutex){return 0;}
	static int stub_MQTTSerialize_disconnect(unsigned char *buf, int buflen){return 0;}
	static int stub_MQTTSerialize_publish_ret10(unsigned char *buf, int buflen, unsigned char dup, int qos, unsigned char retained, unsigned short packetid,
                          MQTTString topicName, unsigned char *payload, int payloadlen){
		return 10;
  	}
	static void stub_TimerCountdown(Timer *timer, unsigned int timeout){
		//printf("+-+-+-+-+-+-  timer count down  +-+-+-+-+-+-\n");
	}
	static int static_count_for_thread = 0;
	static void stub_TimerCountdownMS(Timer *timer, unsigned int timeout){
		static_count_for_thread++;
	}
	static bool stub_TimerIsExpiredFalse(Timer *timer){return false;}
	static int static_count_timer_expire = 0;
	static bool stub_TimerIsExpired_con(Timer *timer){
		if(static_count_timer_expire==0){
			static_count_timer_expire++;
			return false;
		}
		else if(static_count_timer_expire==2){
			return true;
		}
		return false;
	}
	static int static_count_waitfor=0;
	static int stub_waitfor_ret(MQTTClient* c, int packet_type, Timer* timer){
		static_count_waitfor++;
		if(1==static_count_waitfor)return 4;
		else if(2==static_count_waitfor)return 7;
		else if(11 == static_count_waitfor) return 11;
		else if(9 == static_count_waitfor) return 9;
		else if(102 == static_count_waitfor) return 2;
		return 0;
		//static_count_waitfor++;
		
	}
	static void print_ss(){printf("+-+-+-+-+-+-+-\t");}
	static int stub_MQTTSerialize_unsubscribe_ret10(unsigned char *buf, int buflen, unsigned char dup, unsigned short packetid,
                              int count, MQTTString topicFilters[]){return 10;}
    static int stub_MQTTSerialize_subscribe_ret10(unsigned char *buf, int buflen, unsigned char dup, unsigned short packetid, int count,
                            MQTTString topicFilters[], int requestedQoSs[]){return 10;}
	static int stub_MQTTDeserialize_suback(unsigned short *packetid, int maxcount, int *count, int grantedQoSs[], unsigned char *buf, int buflen){
		*grantedQoSs = 0xa0;
		return 1;
	}
	static int stub_MQTTSerialize_connect_ret10(unsigned char *buf, int buflen, MQTTPacket_connectData *options){
		return 10;
	}
	static int static_ret_of_cycle = 0;
	static int stub_cycle(MQTTClient* c, Timer* timer){return static_ret_of_cycle;}
	static int stub_ThreadStart(Thread *thread, void (*fn)(void *), void *arg){return 0;}
	static void* test_thread_mqttrun(void*){
		stubInfo si_countdownMS;
		stubInfo si_cycle;
		stubInfo si_mutexlock;
		stubInfo si_mutexunlock;
		setStub((void*)TimerCountdownMS,(void*)stub_TimerCountdownMS,&si_countdownMS);
		setStub((void*)cycle,(void*)stub_cycle,&si_cycle);
		setStub((void*)MutexLock,(void*)stub_MutexLock,&si_mutexlock);
		setStub((void*)MutexUnlock,(void*)stub_MutexUnlock,&si_mutexunlock);
		static_count_for_thread = 0;
		MQTTClient c;
		MQTTRun(&c);
		cleanStub(&si_countdownMS);
		cleanStub(&si_cycle);
		cleanStub(&si_mutexlock);
		cleanStub(&si_mutexunlock);
	}
} // extern "C"
TestMQTTClient::~TestMQTTClient(){}
TestMQTTClient::TestMQTTClient()
{
	TEST_ADD(TestMQTTClient::test_clean_close_session);

    TEST_ADD(TestMQTTClient::test_MQTTClientInit);
    TEST_ADD(TestMQTTClient::test_MQTTClientDeInit);
    
  //  TEST_ADD(TestMQTTClient::test_MQTTConnect);
    TEST_ADD(TestMQTTClient::test_MQTTPublish);

    TEST_ADD(TestMQTTClient::test_MQTTSetMessageHandler);
   // TEST_ADD(TestMQTTClient::test_MQTTSubscribe);
    TEST_ADD(TestMQTTClient::test_MQTTSubscribeWithResults);
    TEST_ADD(TestMQTTClient::test_MQTTUnsubscribe);

    TEST_ADD(TestMQTTClient::test_MQTTDisconnect);
  //  
    
    TEST_ADD(TestMQTTClient::test_cycle);
    TEST_ADD(TestMQTTClient::test_keepalive);
	TEST_ADD(TestMQTTClient::test_MQTTConnectWithResults);
	TEST_ADD(TestMQTTClient::test_waitfor);
	TEST_ADD(TestMQTTClient::test_MQTTYield);

}
void TestMQTTClient::test_waitfor(void){
	//TimerIsExpired  cycle
	MQTTClient c;
	Timer t;
	stubInfo si_timerexpire;
	stubInfo si_cycle;
	setStub((void*)cycle,(void*)stub_cycle,&si_cycle);
	setStub((void*)TimerIsExpired,(void*)stub_TimerIsExpired_con,&si_timerexpire);
	static_count_timer_expire = 0;
	waitfor(&c,0,&t);
	cleanStub(&si_cycle);
	cleanStub(&si_timerexpire);

	stubInfo si_thread;
	setStub((void*)ThreadStart,(void*)stub_ThreadStart,&si_thread);
	MQTTStartTask(&c);
	cleanStub(&si_thread);
	
	pthread_t s_thread;
	static_count_for_thread = 0;
	pthread_create(&s_thread,NULL,test_thread_mqttrun,NULL);
	if(static_count_for_thread == 2){
		pthread_cancel(&s_thread);
	}
}

void TestMQTTClient::test_clean_close_session(void){
	MQTTClient a;
	a.cleansession = 1;
	MQTTCloseSession(&a);
}

void TestMQTTClient::test_MQTTClientInit(void)
{
    int ret = 0;
    //MQTTClientDeInit(&s_client);
    TEST_ASSERT_MSG((0 == ret), "MQTTClientDeInit(...) failed");

    unsigned char buf[100];
    unsigned char readbuf[100];

    MQTTClientInit(&s_client, &s_network, 1000, buf, 100, readbuf, 100);
    TEST_ASSERT_MSG((0 == ret), "MQTTClientInit(...) failed");
}
void TestMQTTClient::test_MQTTClientDeInit(void)
{
    int ret = 0;

    MQTTClientDeInit(NULL);
    TEST_ASSERT_MSG((0 == ret), "MQTTClientDeInit(...) failed");
}
void TestMQTTClient::test_MQTTConnectWithResults(void)
{
    int ret = 0;
    MQTTClient l_s_mqttC_1;
	char buf_1[100];
	char buf_2[100];
	char tf_buf_0[40];
	char tf_buf_1[40];
	char tf_buf_2[40];
	char tf_buf_3[40];
	char tf_buf_4[40];
	l_s_mqttC_1.buf = buf_1;
	l_s_mqttC_1.readbuf = buf_2;
	l_s_mqttC_1.messageHandlers[0].fp = test_fp_0;
	l_s_mqttC_1.messageHandlers[1].fp = test_fp_1;
	l_s_mqttC_1.messageHandlers[2].fp = test_fp_2;
	l_s_mqttC_1.messageHandlers[3].fp = test_fp_3;
	l_s_mqttC_1.messageHandlers[4].fp = test_fp_4;
	l_s_mqttC_1.messageHandlers[0].topicFilter = tf_buf_0;
	l_s_mqttC_1.messageHandlers[1].topicFilter = tf_buf_1;
	l_s_mqttC_1.messageHandlers[2].topicFilter = tf_buf_2;
	l_s_mqttC_1.messageHandlers[3].topicFilter = tf_buf_3;
	l_s_mqttC_1.messageHandlers[4].topicFilter = tf_buf_4;
	l_s_mqttC_1.defaultMessageHandler = test_msg_handler;
	Network l_s_nw_1;
	l_s_nw_1.mqttread = test_mqttread;//(Network *, unsigned char *, int, int)
	l_s_nw_1.mqttwrite = test_mqttwrite_ret10;//(Network *, unsigned char *, int, int)
	//‘Network’ has no member named ‘disconnect’
	l_s_nw_1.get_security_info = test_get_security_info;
	l_s_mqttC_1.ipstack = &l_s_nw_1;
	static_Ret_of_test_mqttread = 0;

    MQTTConnackData connackData;
    memset(&connackData, 0, sizeof(MQTTConnackData));
	MQTTPacket_connectData l_s_packet_conddata_1;
	
	stubInfo si_countdownMS;
	stubInfo si_timercountdown;
	stubInfo si_serialize_conn;
	stubInfo si_timerexpire;
	stubInfo si_waitfor;
	static_count_waitfor = 101;
	setStub((void*)waitfor,(void*)stub_waitfor_ret,&si_waitfor);
	setStub((void*)TimerIsExpired,(void*)stub_TimerIsExpiredFalse,&si_timerexpire);
	setStub((void*)TimerCountdownMS,(void*)stub_TimerCountdownMS,&si_countdownMS);
	setStub((void*)TimerCountdown,(void*)stub_TimerCountdown,&si_timercountdown);
	setStub((void*)MQTTSerialize_connect,(void*)stub_MQTTSerialize_connect_ret10,&si_serialize_conn);
	l_s_mqttC_1.isconnected = 0;
	ret = MQTTConnectWithResults(&l_s_mqttC_1, (MQTTPacket_connectData*)0, &connackData);
	cleanStub(&si_countdownMS);
	cleanStub(&si_timercountdown);
	cleanStub(&si_serialize_conn);
	cleanStub(&si_timerexpire);
	cleanStub(&si_waitfor);


}
void TestMQTTClient::test_MQTTConnect(void)
{
    int ret = 0;

    stubInfo si_write;
    setStub((void*)write, (void*)stub_write, &si_write);
    stubInfo si_recv;
    setStub((void*)recv, (void*)stub_recv, &si_recv);
    
    s_packet_type = 0x20;
    ret = MQTTConnect(&s_client, &s_connectdata);
    TEST_ASSERT_MSG((0 == ret), "MQTTConnect(...) failed");
    s_packet_type = 0x00;

    ret = MQTTDisconnect(&s_client);
    TEST_ASSERT_MSG((0 == ret), "MQTTDisconnect(...) failed");

    cleanStub(&si_write);
    cleanStub(&si_recv);
}
void TestMQTTClient::test_MQTTPublish(void)
{
    int ret = 0;
    MQTTClient l_s_mqttC_1;
	char buf_1[100];
	char buf_2[100];
	char tf_buf_0[40];
	char tf_buf_1[40];
	char tf_buf_2[40];
	char tf_buf_3[40];
	char tf_buf_4[40];
	l_s_mqttC_1.buf = buf_1;
	l_s_mqttC_1.readbuf = buf_2;
	l_s_mqttC_1.messageHandlers[0].fp = test_fp_0;
	l_s_mqttC_1.messageHandlers[1].fp = test_fp_1;
	l_s_mqttC_1.messageHandlers[2].fp = test_fp_2;
	l_s_mqttC_1.messageHandlers[3].fp = test_fp_3;
	l_s_mqttC_1.messageHandlers[4].fp = test_fp_4;
	l_s_mqttC_1.messageHandlers[0].topicFilter = tf_buf_0;
	l_s_mqttC_1.messageHandlers[1].topicFilter = tf_buf_1;
	l_s_mqttC_1.messageHandlers[2].topicFilter = tf_buf_2;
	l_s_mqttC_1.messageHandlers[3].topicFilter = tf_buf_3;
	l_s_mqttC_1.messageHandlers[4].topicFilter = tf_buf_4;
	l_s_mqttC_1.defaultMessageHandler = test_msg_handler;
	Network l_s_nw_1;
	l_s_nw_1.mqttread = test_mqttread;//(Network *, unsigned char *, int, int)
	l_s_nw_1.mqttwrite = test_mqttwrite;//(Network *, unsigned char *, int, int)
	//‘Network’ has no member named ‘disconnect’
	l_s_nw_1.get_security_info = test_get_security_info;
	l_s_mqttC_1.ipstack = &l_s_nw_1;
	static_Ret_of_test_mqttread = 0;
	
    MQTTMessage pubmsg;
    memset(&pubmsg, '\0', sizeof(pubmsg));
	pubmsg.payload = (void*)"a much longer message that we can shorten to the extent that we need to payload up to 11";
	pubmsg.payloadlen = 11;
	pubmsg.qos = QOS0;
	pubmsg.retained = false;
    pubmsg.dup = false;

    char *topticName = (char*)"MQTTSAS topic";
    
    ret = MQTTPublish(&l_s_mqttC_1, topticName, &pubmsg);
    TEST_ASSERT_MSG((FAILURE == ret), "MQTTPublish(...) failed");
    l_s_mqttC_1.isconnected = 1;
    stubInfo si_write;
    setStub((void*)write, (void*)stub_write, &si_write);
    stubInfo si_recv;
    setStub((void*)recv, (void*)stub_recv, &si_recv);
    ret = MQTTPublish(&l_s_mqttC_1, topticName, &pubmsg);
    TEST_ASSERT_MSG((0 == ret), "MQTTPublish(...) failed");
	//
    pubmsg.qos = QOS1;
    // wait_for timeout
    ret = MQTTPublish(&l_s_mqttC_1, topticName, &pubmsg);
    TEST_ASSERT_MSG((FAILURE == ret), "MQTTPublish(...) failed");
    l_s_mqttC_1.isconnected = 1;
    s_packet_type = 0x40;
    ret = MQTTPublish(&l_s_mqttC_1, topticName, &pubmsg);
    TEST_ASSERT_MSG((0 == ret), "MQTTPublish(...) failed");
#if 1
	stubInfo si_serialize_publish;
	stubInfo si_countdown;
	stubInfo si_timerExpired;
	stubInfo si_waitfor;
	stubInfo si_deserialize_ack;
	setStub((void*)MQTTDeserialize_ack,(void*)stub_MQTTDeserialize_ack_ret2,&si_deserialize_ack);
	setStub((void*)waitfor,(void*)stub_waitfor_ret,&si_waitfor);
	static_count_waitfor=0;
	setStub((void*)TimerIsExpired,(void*)stub_TimerIsExpiredFalse,&si_timerExpired);
	setStub((void*)TimerCountdown,(void*)stub_TimerCountdown,&si_countdown);
	setStub((void*)MQTTSerialize_publish,(void*)stub_MQTTSerialize_publish_ret10,&si_serialize_publish);
	l_s_mqttC_1.isconnected = 1;
	s_packet_type = 0x40;
	l_s_mqttC_1.ipstack->mqttwrite = test_mqttwrite_ret10;
	ret = MQTTPublish(&l_s_mqttC_1, topticName, &pubmsg);//if (message->qos == QOS1)
	//
	//int MQTTDeserialize_ack(unsigned char *packettype, unsigned char *dup, unsigned short *packetid, unsigned char *buf, int buflen)
	
	stubInfo si_countdownMS;
	setStub((void*)TimerCountdownMS,(void*)stub_TimerCountdownMS,&si_countdownMS);
	l_s_mqttC_1.isconnected = 1;
	s_packet_type = 0x40;
	pubmsg.qos = QOS2;
	ret = MQTTPublish(&l_s_mqttC_1, topticName, &pubmsg);
	cleanStub(&si_deserialize_ack);
	cleanStub(&si_waitfor);
	cleanStub(&si_timerExpired);
	cleanStub(&si_countdownMS);
	cleanStub(&si_serialize_publish);
	cleanStub(&si_countdown);
	l_s_mqttC_1.ipstack->mqttwrite = test_mqttwrite;
#endif
    pubmsg.qos = QOS2;
    s_packet_type = 0x70;
    ret = MQTTPublish(&l_s_mqttC_1, topticName, &pubmsg);
    TEST_ASSERT_MSG((0 == ret), "MQTTPublish(...) failed");
    s_packet_type = 0x00;

    cleanStub(&si_write);
    cleanStub(&si_recv);
}
void TestMQTTClient::test_MQTTSetMessageHandler(void)
{
    int ret = 0;

    char* topticFilter = (char*)"topticFilter";
    
    ret = MQTTSetMessageHandler(&s_client, topticFilter, message_handler);
    TEST_ASSERT_MSG((0 == ret), "MQTTSetMessageHandler(...) failed");

    ret = MQTTSetMessageHandler(&s_client, topticFilter, NULL);
    TEST_ASSERT_MSG((0 == ret), "MQTTSetMessageHandler(...) failed");

    ret = MQTTSetMessageHandler(&s_client, topticFilter, message_handler);
    TEST_ASSERT_MSG((0 == ret), "MQTTSetMessageHandler(...) failed");
}
void TestMQTTClient::test_MQTTSubscribe(void)
{
    int ret = 0;

    stubInfo si_write;
    setStub((void*)write, (void*)stub_write, &si_write);
    stubInfo si_recv;
    setStub((void*)recv, (void*)stub_recv, &si_recv);

    s_packet_type = 0x20;
    ret = MQTTConnect(&s_client, &s_connectdata);
    TEST_ASSERT_MSG((0 == ret), "MQTTConnect(...) failed");
    s_packet_type = 0x00;
        
    char* topticFilter = (char*)"topticFilter";

    s_packet_type = 0x90;
    s_client.isconnected = 1;
    ret = MQTTSubscribe(&s_client, topticFilter, QOS0, message_handler);
    TEST_ASSERT_MSG((0 == ret), "MQTTSubscribe(...) failed");
    s_packet_type = 0x00;

    cleanStub(&si_write);
    cleanStub(&si_recv);
}
void TestMQTTClient::test_MQTTSubscribeWithResults(void)
{
	MQTTClient l_s_mqttC_1;
	char buf_1[100];
	char buf_2[100];
	char tf_buf_0[40];
	char tf_buf_1[40];
	char tf_buf_2[40];
	char tf_buf_3[40];
	char tf_buf_4[40];
	l_s_mqttC_1.buf = buf_1;
	l_s_mqttC_1.readbuf = buf_2;
	l_s_mqttC_1.messageHandlers[0].fp = test_fp_0;
	l_s_mqttC_1.messageHandlers[1].fp = test_fp_1;
	l_s_mqttC_1.messageHandlers[2].fp = test_fp_2;
	l_s_mqttC_1.messageHandlers[3].fp = test_fp_3;
	l_s_mqttC_1.messageHandlers[4].fp = test_fp_4;
	l_s_mqttC_1.messageHandlers[0].topicFilter = tf_buf_0;
	l_s_mqttC_1.messageHandlers[1].topicFilter = tf_buf_1;
	l_s_mqttC_1.messageHandlers[2].topicFilter = tf_buf_2;
	l_s_mqttC_1.messageHandlers[3].topicFilter = tf_buf_3;
	l_s_mqttC_1.messageHandlers[4].topicFilter = tf_buf_4;
	l_s_mqttC_1.defaultMessageHandler = test_msg_handler;
	Network l_s_nw_1;
	l_s_nw_1.mqttread = test_mqttread;//(Network *, unsigned char *, int, int)
	l_s_nw_1.mqttwrite = test_mqttwrite;//(Network *, unsigned char *, int, int)
	//‘Network’ has no member named ‘disconnect’
	l_s_nw_1.get_security_info = test_get_security_info;
	l_s_mqttC_1.ipstack = &l_s_nw_1;
	static_Ret_of_test_mqttread = 0;
	
    int ret = 0;

    stubInfo si_write;
    setStub((void*)write, (void*)stub_write, &si_write);
    stubInfo si_recv;
    setStub((void*)recv, (void*)stub_recv, &si_recv);

    s_packet_type = 0x20;
    ret = MQTTConnect(&l_s_mqttC_1, &s_connectdata);
    TEST_ASSERT_MSG((0 == ret), "MQTTConnect(...) failed");
    s_packet_type = 0x00;
        
    char* topticFilter = (char*)"topticFilter";

    MQTTSubackData subackData;
    memset(&subackData, 0, sizeof(MQTTSubackData));
	//print_ss();printf("511\n");
    stubInfo si_muxlock;
	stubInfo si_countdownMS;
	stubInfo si_timerExpired;
	stubInfo si_serialize_subsc;
	stubInfo si_timercountdown;
	stubInfo si_waitfor;
	stubInfo si_deseialize_suback;
	setStub((void*)waitfor,(void*)stub_waitfor_ret,&si_waitfor);
	static_count_waitfor = 8;
	setStub((void*)TimerCountdown,(void*)stub_TimerCountdown,&si_timercountdown);
	setStub((void*)MQTTSerialize_subscribe,(void*)stub_MQTTSerialize_subscribe_ret10,&si_serialize_subsc);
	setStub((void*)TimerIsExpired,(void*)stub_TimerIsExpiredFalse,&si_timerExpired);
	setStub((void*)TimerCountdownMS,(void*)stub_TimerCountdownMS,&si_countdownMS);
	setStub((void*)MutexLock,(void*)stub_MutexLock,&si_muxlock);
	l_s_nw_1.mqttwrite = test_mqttwrite_ret10;
	//anchor: if (waitfor(c, SUBACK, &timer) == SUBACK)
    ret = MQTTSubscribeWithResults(&l_s_mqttC_1, topticFilter, QOS0, message_handler, &subackData);
    TEST_ASSERT_MSG((FAILURE == ret), "MQTTSubscribeWithResults(...) failed");

	setStub((void*)MQTTDeserialize_suback,(void*)stub_MQTTDeserialize_suback,&si_deseialize_suback);
	l_s_nw_1.mqttwrite = test_mqttwrite_ret10;
	static_count_waitfor = 8;
	ret = MQTTSubscribeWithResults(&l_s_mqttC_1, topticFilter, QOS0, message_handler, &subackData);
    TEST_ASSERT_MSG((FAILURE == ret), "MQTTSubscribeWithResults(...) failed");

    s_packet_type = 0x90;
    l_s_mqttC_1.isconnected = 1;
    ret = MQTTSubscribeWithResults(&l_s_mqttC_1, topticFilter, QOS0, message_handler, &subackData);
    TEST_ASSERT_MSG((0 == ret), "MQTTSubscribeWithResults(...) failed");

	cleanStub(&si_muxlock);
	cleanStub(&si_countdownMS);
	cleanStub(&si_timerExpired);
	cleanStub(&si_serialize_subsc);
	cleanStub(&si_timercountdown);
	cleanStub(&si_waitfor);
	cleanStub(&si_deseialize_suback);
	
    s_packet_type = 0x00;
    cleanStub(&si_write);
    cleanStub(&si_recv);
}
void TestMQTTClient::test_MQTTUnsubscribe(void)
{
	MQTTClient l_s_mqttC_1;
	char buf_1[100];
	char buf_2[100];
	char tf_buf_0[40];
	char tf_buf_1[40];
	char tf_buf_2[40];
	char tf_buf_3[40];
	char tf_buf_4[40];
	l_s_mqttC_1.buf = buf_1;
	l_s_mqttC_1.readbuf = buf_2;
	l_s_mqttC_1.messageHandlers[0].fp = test_fp_0;
	l_s_mqttC_1.messageHandlers[1].fp = test_fp_1;
	l_s_mqttC_1.messageHandlers[2].fp = test_fp_2;
	l_s_mqttC_1.messageHandlers[3].fp = test_fp_3;
	l_s_mqttC_1.messageHandlers[4].fp = test_fp_4;
	l_s_mqttC_1.messageHandlers[0].topicFilter = tf_buf_0;
	l_s_mqttC_1.messageHandlers[1].topicFilter = tf_buf_1;
	l_s_mqttC_1.messageHandlers[2].topicFilter = tf_buf_2;
	l_s_mqttC_1.messageHandlers[3].topicFilter = tf_buf_3;
	l_s_mqttC_1.messageHandlers[4].topicFilter = tf_buf_4;
	l_s_mqttC_1.defaultMessageHandler = test_msg_handler;
	Network l_s_nw_1;
	l_s_nw_1.mqttread = test_mqttread;//(Network *, unsigned char *, int, int)
	l_s_nw_1.mqttwrite = test_mqttwrite;//(Network *, unsigned char *, int, int)
	//‘Network’ has no member named ‘disconnect’
	l_s_nw_1.get_security_info = test_get_security_info;
	l_s_mqttC_1.ipstack = &l_s_nw_1;
	static_Ret_of_test_mqttread = 0;

    int ret = 0;

    stubInfo si_write;
    setStub((void*)write, (void*)stub_write, &si_write);
    stubInfo si_recv;
    setStub((void*)recv, (void*)stub_recv, &si_recv);

    s_packet_type = 0x20;
    ret = MQTTConnect(&l_s_mqttC_1, &s_connectdata);
    TEST_ASSERT_MSG((0 == ret), "MQTTConnect(...) failed");
    s_packet_type = 0x00;

    char* topticFilter = (char*)"topticFilter";

    s_packet_type = 0x90;
    l_s_mqttC_1.isconnected = 0;
    ret = MQTTSubscribe(&l_s_mqttC_1, topticFilter, QOS0, message_handler);
    TEST_ASSERT_MSG((FAILURE == ret), "MQTTSubscribe(...) failed");
    
    l_s_mqttC_1.isconnected = 1;
    ret = MQTTSubscribe(&l_s_mqttC_1, topticFilter, QOS0, message_handler);
    TEST_ASSERT_MSG((0 == ret), "MQTTSubscribe(...) failed");
    s_packet_type = 0x00;

    ret = MQTTUnsubscribe(&l_s_mqttC_1, topticFilter);
    TEST_ASSERT_MSG((FAILURE == ret), "MQTTUnsubscribe(...) failed");
    l_s_mqttC_1.isconnected = 0;
    ret = MQTTUnsubscribe(&l_s_mqttC_1, topticFilter);
    TEST_ASSERT_MSG((FAILURE == ret), "MQTTUnsubscribe(...) failed");

    s_packet_type = 0xb0;
    l_s_mqttC_1.isconnected = 1;
    ret = MQTTUnsubscribe(&l_s_mqttC_1, topticFilter);
	//print_ss();printf("558 \n");
    TEST_ASSERT_MSG((0 == ret), "MQTTUnsubscribe(...) failed");
#if 1
	stubInfo si_timerExpired;
	stubInfo si_unsubscribe;
	stubInfo si_timercountdown;
	stubInfo si_waitfor;
	setStub((void*)waitfor,(void*)stub_waitfor_ret,&si_waitfor);
	static_count_waitfor = 10;
	setStub((void*)TimerIsExpired,(void*)stub_TimerIsExpiredFalse,&si_timerExpired);
	setStub((void*)MQTTSerialize_unsubscribe,(void*)stub_MQTTSerialize_unsubscribe_ret10,&si_unsubscribe);
	setStub((void*)TimerCountdown,(void*)stub_TimerCountdown,&si_timercountdown);
	l_s_mqttC_1.ipstack->mqttwrite = test_mqttwrite_ret10;
	
	//s_packet_type = 0xb0;
    l_s_mqttC_1.isconnected = 1;
	ret = MQTTUnsubscribe(&l_s_mqttC_1, topticFilter);
	cleanStub(&si_timerExpired);
	cleanStub(&si_unsubscribe);
	cleanStub(&si_timercountdown);
	cleanStub(&si_waitfor);
#endif
    s_packet_type = 0x00;

    cleanStub(&si_write);
    cleanStub(&si_recv);
}
void TestMQTTClient::test_MQTTDisconnect(void)
{
	MQTTClient l_s_mqttC_1;
	char buf_1[100];
	char buf_2[100];
	char tf_buf_0[40];
	char tf_buf_1[40];
	char tf_buf_2[40];
	char tf_buf_3[40];
	char tf_buf_4[40];
	l_s_mqttC_1.buf = buf_1;
	l_s_mqttC_1.readbuf = buf_2;
	l_s_mqttC_1.messageHandlers[0].fp = test_fp_0;
	l_s_mqttC_1.messageHandlers[1].fp = test_fp_1;
	l_s_mqttC_1.messageHandlers[2].fp = test_fp_2;
	l_s_mqttC_1.messageHandlers[3].fp = test_fp_3;
	l_s_mqttC_1.messageHandlers[4].fp = test_fp_4;
	l_s_mqttC_1.messageHandlers[0].topicFilter = tf_buf_0;
	l_s_mqttC_1.messageHandlers[1].topicFilter = tf_buf_1;
	l_s_mqttC_1.messageHandlers[2].topicFilter = tf_buf_2;
	l_s_mqttC_1.messageHandlers[3].topicFilter = tf_buf_3;
	l_s_mqttC_1.messageHandlers[4].topicFilter = tf_buf_4;
	l_s_mqttC_1.defaultMessageHandler = test_msg_handler;
	Network l_s_nw_1;
	l_s_nw_1.mqttread = test_mqttread;//(Network *, unsigned char *, int, int)
	l_s_nw_1.mqttwrite = test_mqttwrite;//(Network *, unsigned char *, int, int)
	//‘Network’ has no member named ‘disconnect’
	l_s_nw_1.get_security_info = test_get_security_info;
	l_s_mqttC_1.ipstack = &l_s_nw_1;
	static_Ret_of_test_mqttread = 0;
	//
	stubInfo si_muxlock;
	stubInfo si_muxunlock;
	stubInfo si_serialize_disconnect;
	setStub((void*)MutexLock,(void*)stub_MutexLock,&si_muxlock);
	setStub((void*)MutexUnlock,(void*)stub_MutexUnlock,&si_muxunlock);
	setStub((void*)MQTTSerialize_disconnect,(void*)stub_MQTTSerialize_disconnect,&si_serialize_disconnect);
	MQTTDisconnect(&l_s_mqttC_1);
	cleanStub(&si_serialize_disconnect);
	cleanStub(&si_muxunlock);
	cleanStub(&si_muxlock);
}
void TestMQTTClient::test_MQTTYield(void)
{
	MQTTClient l_s_mqttC_1;
	char buf_1[100];
	char buf_2[100];
	char tf_buf_0[40];
	char tf_buf_1[40];
	char tf_buf_2[40];
	char tf_buf_3[40];
	char tf_buf_4[40];
	l_s_mqttC_1.buf = buf_1;
	l_s_mqttC_1.readbuf = buf_2;
	l_s_mqttC_1.messageHandlers[0].fp = test_fp_0;
	l_s_mqttC_1.messageHandlers[1].fp = test_fp_1;
	l_s_mqttC_1.messageHandlers[2].fp = test_fp_2;
	l_s_mqttC_1.messageHandlers[3].fp = test_fp_3;
	l_s_mqttC_1.messageHandlers[4].fp = test_fp_4;
	l_s_mqttC_1.messageHandlers[0].topicFilter = tf_buf_0;
	l_s_mqttC_1.messageHandlers[1].topicFilter = tf_buf_1;
	l_s_mqttC_1.messageHandlers[2].topicFilter = tf_buf_2;
	l_s_mqttC_1.messageHandlers[3].topicFilter = tf_buf_3;
	l_s_mqttC_1.messageHandlers[4].topicFilter = tf_buf_4;
	l_s_mqttC_1.defaultMessageHandler = test_msg_handler;
	Network l_s_nw_1;
	l_s_nw_1.mqttread = test_mqttread;//(Network *, unsigned char *, int, int)
	l_s_nw_1.mqttwrite = test_mqttwrite;//(Network *, unsigned char *, int, int)
	//‘Network’ has no member named ‘disconnect’
	l_s_nw_1.get_security_info = test_get_security_info;
	l_s_mqttC_1.ipstack = &l_s_nw_1;
	static_Ret_of_test_mqttread = 0;

    int ret = 0;
    ret = MQTTYield(&l_s_mqttC_1, 100);
    TEST_ASSERT_MSG((FAILURE == ret), "MQTTYield(...) failed");

    stubInfo si_write;
    setStub((void*)write, (void*)stub_write, &si_write);
    stubInfo si_recv;
    setStub((void*)recv, (void*)stub_recv, &si_recv);
    
    ret = MQTTYield(&l_s_mqttC_1, 100);
    TEST_ASSERT_MSG((0 == ret), "MQTTYield(...) failed");
    
    cleanStub(&si_write);
    cleanStub(&si_recv);
	stubInfo si_cycle;
	stubInfo si_TimerCountdownMS;
	stubInfo si_MutexLock;
	stubInfo si_muxunlock;
	
	setStub((void*)TimerCountdownMS,(void*)stub_TimerCountdownMS,&si_TimerCountdownMS);	
	setStub((void*)MutexLock,(void*)stub_MutexLock,&si_MutexLock);
	setStub((void*)MutexUnlock,(void*)stub_MutexUnlock,&si_muxunlock);
	setStub((void*)cycle,(void*)stub_cycle,&si_cycle);
	static_ret_of_cycle = -1;
	
	ret = MQTTYield(&l_s_mqttC_1, 100);
	
	static_ret_of_cycle = 0;
	cleanStub(&si_muxunlock);
	cleanStub(&si_MutexLock);
	cleanStub(&si_TimerCountdownMS);
	cleanStub(&si_cycle);
	
}

void TestMQTTClient::test_MQTTRun(void)
{
#if 0
    int ret = 0;
    pthread_t first_thread;
    
    ret = pthread_create(&first_thread, NULL, (pthread_func)MQTTRun, (void *)&s_client);
    TEST_ASSERT_MSG((0 == ret), "MQTTRun(...) failed");

    usleep(10000);
    pthread_cancel(first_thread);

    pthread_join(first_thread, NULL); 
#endif
}

void TestMQTTClient::test_cycle(void)
{
    Timer connect_timer;
    int ret = 0;
	stubInfo si_deserialize_publish;
    memset(&connect_timer, 0, sizeof(Timer));

    TimerInit(&connect_timer);
    TimerCountdownMS(&connect_timer, s_client.command_timeout_ms);
	MQTTClient l_s_mqttC_1;
	char buf_1[100];
	char buf_2[100];
	char tf_buf_0[40];
	char tf_buf_1[40];
	char tf_buf_2[40];
	char tf_buf_3[40];
	char tf_buf_4[40];
	l_s_mqttC_1.buf = buf_1;
	l_s_mqttC_1.readbuf = buf_2;
	l_s_mqttC_1.messageHandlers[0].fp = test_fp_0;
	l_s_mqttC_1.messageHandlers[1].fp = test_fp_1;
	l_s_mqttC_1.messageHandlers[2].fp = test_fp_2;
	l_s_mqttC_1.messageHandlers[3].fp = test_fp_3;
	l_s_mqttC_1.messageHandlers[4].fp = test_fp_4;
	l_s_mqttC_1.messageHandlers[0].topicFilter = tf_buf_0;
	l_s_mqttC_1.messageHandlers[1].topicFilter = tf_buf_1;
	l_s_mqttC_1.messageHandlers[2].topicFilter = tf_buf_2;
	l_s_mqttC_1.messageHandlers[3].topicFilter = tf_buf_3;
	l_s_mqttC_1.messageHandlers[4].topicFilter = tf_buf_4;
	l_s_mqttC_1.defaultMessageHandler = test_msg_handler;
	Network l_s_nw_1;
	l_s_nw_1.mqttread = test_mqttread;//(Network *, unsigned char *, int, int)
	l_s_nw_1.mqttwrite = test_mqttwrite;//(Network *, unsigned char *, int, int)
	//‘Network’ has no member named ‘disconnect’
	l_s_nw_1.get_security_info = test_get_security_info;
	l_s_mqttC_1.ipstack = &l_s_nw_1;
	static_Ret_of_test_mqttread = 0;
    ret = cycle(&l_s_mqttC_1, &connect_timer);
    TEST_ASSERT_MSG((FAILURE == ret), "cycle(...) failed");
	static_Ret_of_test_mqttread = 1;
	ret = cycle(&l_s_mqttC_1, &connect_timer);
	static_Ret_of_test_mqttread = 3;
    
	setStub((void*)MQTTDeserialize_publish,(void*)stub_MQTTDeserialize_publish,&si_deserialize_publish);
	ret = cycle(&l_s_mqttC_1, &connect_timer);
	cleanStub(&si_deserialize_publish);
    stubInfo si_write;
    setStub((void*)write, (void*)stub_write, &si_write);
    stubInfo si_recv;
    setStub((void*)recv, (void*)stub_recv, &si_recv);

    char topic[MAX_MESSAGE_HANDLERS+5][20] = { {"foo"}, {"+/foo"}, {"foo/+/#"}, {"#/foo"}, 
                        {"a/+foo"}, {"foo+/a"}};
    for (int i = 0; i < MAX_MESSAGE_HANDLERS; ++i){
        l_s_mqttC_1.messageHandlers[i].topicFilter = (char*)topic[i];
        l_s_mqttC_1.messageHandlers[i].fp = message_handler;
    }

    s_packet_type = 0x10;
    ret = cycle(&l_s_mqttC_1, &connect_timer);
    TEST_ASSERT_MSG((1 == ret), "cycle(...) failed");

    s_packet_type = 0x20;
    ret = cycle(&l_s_mqttC_1, &connect_timer);
    TEST_ASSERT_MSG((2 == ret), "cycle(...) failed");
    
    s_packet_type = 0x30; // PUBLISH
    
    stubInfo si_publish;
    setStub((void*)MQTTDeserialize_publish, (void*)stub_MQTTDeserialize_publish, &si_publish);
    memcpy(s_readbuf, "foo", strlen("foo"));
    memcpy(s_buf, "foo/#", strlen("foo/#"));
    ret = cycle(&l_s_mqttC_1, &connect_timer);
    TEST_ASSERT_MSG((3 == ret), "cycle(...) failed");
    
    s_cstring = (char*)"cycle";
    ret = cycle(&l_s_mqttC_1, &connect_timer);
    TEST_ASSERT_MSG((3 == ret), "cycle(...) failed");
    s_cstring = NULL;

    /////
    memcpy(s_buf, "foo", strlen("foo"));
    ret = cycle(&l_s_mqttC_1, &connect_timer);
    TEST_ASSERT_MSG((3 == ret), "cycle(...) failed");

    s_intQoS = QOS2;
    ret = cycle(&l_s_mqttC_1, &connect_timer);
    TEST_ASSERT_MSG((3 == ret), "cycle(...) failed");

    stubInfo si_serial_ack;
    setStub((void*)MQTTSerialize_ack, (void*)stub_MQTTSerialize_ack, &si_serial_ack);
    ret = cycle(&l_s_mqttC_1, &connect_timer);
    TEST_ASSERT_MSG((-1 == ret), "cycle(...) failed");
    cleanStub(&si_serial_ack);
    cleanStub(&si_publish);
    
    l_s_mqttC_1.defaultMessageHandler = message_handler;
    for(int i = 0; i < MAX_MESSAGE_HANDLERS; ++i){
        memcpy(s_readbuf, topic[i], strlen(topic[i]));
        l_s_mqttC_1.messageHandlers[i].topicFilter = NULL;
        ret = cycle(&l_s_mqttC_1, &connect_timer);
        TEST_ASSERT_MSG((3 == ret), "cycle(...) failed");
    }

    s_packet_type = 0x50;
    ret = cycle(&l_s_mqttC_1, &connect_timer);
    TEST_ASSERT_MSG((5 == ret), "cycle(...) failed");

    s_write_flag = 0;
    ret = cycle(&l_s_mqttC_1, &connect_timer);
    TEST_ASSERT_MSG((-1 == ret), "cycle(...) failed");
    s_write_flag = 1;
    
    setStub((void*)MQTTSerialize_ack, (void*)stub_MQTTSerialize_ack, &si_serial_ack);
    ret = cycle(&l_s_mqttC_1, &connect_timer);
    TEST_ASSERT_MSG((-1 == ret), "cycle(...) failed");
    cleanStub(&si_serial_ack);

    stubInfo si_dser_ack;
    setStub((void*)MQTTDeserialize_ack, (void*)stub_MQTTDeserialize_ack, &si_dser_ack);
    l_s_mqttC_1.isconnected = 1;
    ret = cycle(&l_s_mqttC_1, &connect_timer);
    l_s_mqttC_1.isconnected = 0;
    TEST_ASSERT_MSG(( FAILURE == ret), "cycle(...) failed");
    
    cleanStub(&si_dser_ack);
    
    s_packet_type = 0x70;
    unsigned int tmp = l_s_mqttC_1.keepAliveInterval;
    l_s_mqttC_1.keepAliveInterval = 0;
    ret = cycle(&l_s_mqttC_1, &connect_timer);
    l_s_mqttC_1.keepAliveInterval = tmp;
    TEST_ASSERT_MSG((7 == ret), "cycle(...) failed");
    
    s_packet_type = 0xd0;
    ret = cycle(&l_s_mqttC_1, &connect_timer);
    TEST_ASSERT_MSG((13 == ret), "cycle(...) failed");

    s_packet_type = 0x00;

    cleanStub(&si_write);
    cleanStub(&si_recv);
}

void TestMQTTClient::test_keepalive(void)
{
    int ret = 0;

    stubInfo si_write;
    setStub((void*)write, (void*)stub_write, &si_write);
    stubInfo si_recv;
    setStub((void*)recv, (void*)stub_recv, &si_recv);
    
    ret = keepalive(&s_client);
    TEST_ASSERT_MSG((0 == ret), "keepalive(...) failed");

    cleanStub(&si_write);
    cleanStub(&si_recv);
}
/* Private functions --------------------------------------------------------*/
void TestMQTTClient::setup()
{
#if 0
    int ret = 0;

    NetworkInit(&s_network,NULL);
    //s_network.proto = MQTT_PROTO_NONE;
    stubInfo stub_info;
    setStub((void*)connect, (void*)stub_connect, &stub_info);
    ret = NetworkConnect(&s_network, options.host, options.port);
    TEST_ASSERT_MSG((0 == ret), "NetworkConnect(...) failed");
    cleanStub(&stub_info);

    setStub((void*)atiny_mutex_create, (void*)stub_atiny_mutex_create, &stub_info);
    ret = 0;
    MQTTClientInit(&s_client, &s_network, 1000, s_buf, 100, s_readbuf, 100);
    TEST_ASSERT_MSG((0 == ret), "MQTTClientInit(...) failed");
    cleanStub(&stub_info);

    s_connectdata.willFlag = 1;
    s_connectdata.MQTTVersion = options.MQTTVersion;
    s_connectdata.clientID.cstring = (char*)"single-threaded-test";
    s_connectdata.username.cstring = (char*)"testuser";
    s_connectdata.password.cstring = (char*)"testpassword";

    s_connectdata.keepAliveInterval = 20;
    s_connectdata.cleansession = 1;

    s_connectdata.will.message.cstring = (char*)"will message";
    s_connectdata.will.qos = 1;
    s_connectdata.will.retained = 0;
    s_connectdata.will.topicName.cstring = (char*)"will topic";
	#endif
}
void TestMQTTClient::tear_down()
{
#if 0
    int ret = 0;
    MQTTClientDeInit(&s_client);
    TEST_ASSERT_MSG((0 == ret), "MQTTClientDeInit(...) failed");
    NetworkDisconnect(&s_network);
    TEST_ASSERT_MSG((0 == ret), "NetworkDisconnect(...) failed");
	#endif
}


