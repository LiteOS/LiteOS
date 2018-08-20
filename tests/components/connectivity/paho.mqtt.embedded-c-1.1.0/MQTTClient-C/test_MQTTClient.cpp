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
#include "MQTTClient.h"
#include "MQTTConnect.h"
#include "los_sem.ph"
#include <pthread.h>
#include <unistd.h> 

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>


static SEM_CB_S s_sem;

struct Options
{
    char* host;         /**< connection to system under test. */
    int port;
    char* proxy_host;
    int proxy_port;
    int verbose;
    int test_no;
    int MQTTVersion;
    int iterations;
} options =
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

extern void MQTTRun(void* parm);
extern int cycle(MQTTClient* c, Timer* timer);
extern int keepalive(MQTTClient* c);
extern int MQTTDeserialize_publish(unsigned char* dup, int* qos, unsigned char* retained, unsigned short* packetid, MQTTString* topicName,
	unsigned char** payload, int* payloadlen, unsigned char* buf, int buflen);
extern int MQTTDeserialize_ack(unsigned char* packettype, unsigned char* dup, unsigned short* packetid, unsigned char* buf, int buflen);
extern int MQTTSerialize_ack(unsigned char* buf, int buflen, unsigned char packettype, unsigned char dup, unsigned short packetid);

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
    if(s_write_flag)
        return count;
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

static int stub_MQTTSerialize_ack(unsigned char* buf, int buflen, unsigned char packettype, unsigned char dup, unsigned short packetid)
{
    return 0;
}
    
} // extern "C"
/* Defines ------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
TestMQTTClient::TestMQTTClient()
{
    TEST_ADD(TestMQTTClient::test_MQTTClientInit);
    TEST_ADD(TestMQTTClient::test_MQTTClientDeInit);
    TEST_ADD(TestMQTTClient::test_MQTTConnectWithResults);
    TEST_ADD(TestMQTTClient::test_MQTTConnect);
    TEST_ADD(TestMQTTClient::test_MQTTPublish);

    TEST_ADD(TestMQTTClient::test_MQTTSetMessageHandler);
    TEST_ADD(TestMQTTClient::test_MQTTSubscribe);
    TEST_ADD(TestMQTTClient::test_MQTTSubscribeWithResults);
    TEST_ADD(TestMQTTClient::test_MQTTUnsubscribe);

    TEST_ADD(TestMQTTClient::test_MQTTDisconnect);
    TEST_ADD(TestMQTTClient::test_MQTTYield);
    TEST_ADD(TestMQTTClient::test_MQTTRun);
    
    TEST_ADD(TestMQTTClient::test_cycle);
    TEST_ADD(TestMQTTClient::test_keepalive);
}
TestMQTTClient::~TestMQTTClient()
{
    
}

void TestMQTTClient::test_MQTTClientInit(void)
{
    int ret = 0;

    MQTTClientDeInit(&s_client);
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
    
    MQTTConnackData connackData;
    memset(&connackData, 0, sizeof(MQTTConnackData));

    stubInfo si_write;
    setStub((void*)write, (void*)stub_write, &si_write);

    ret = MQTTConnectWithResults(&s_client, NULL, &connackData);
    TEST_ASSERT_MSG((FAILURE == ret), "MQTTConnectWithResults(...) failed");

    s_client.isconnected = 1;
    ret = MQTTConnectWithResults(&s_client, &s_connectdata, &connackData);
    TEST_ASSERT_MSG((FAILURE == ret), "MQTTConnectWithResults(...) failed");

    s_client.isconnected = 0;
    ret = MQTTConnectWithResults(&s_client, &s_connectdata, &connackData);
    TEST_ASSERT_MSG((FAILURE == ret), "MQTTConnectWithResults(...) failed");

    if(NULL != s_client.ipstack){
        s_client.ipstack->proto = MQTT_PROTO_NONE;
    }
    ret = MQTTConnectWithResults(&s_client, &s_connectdata, &connackData);
    TEST_ASSERT_MSG((FAILURE == ret), "MQTTConnectWithResults(...) failed");

    stubInfo si_recv;
    setStub((void*)recv, (void*)stub_recv, &si_recv);
    s_packet_type = 0x20;
    ret = MQTTConnectWithResults(&s_client, &s_connectdata, &connackData);
    TEST_ASSERT_MSG((0 == ret), "MQTTConnectWithResults(...) failed");
    s_packet_type = 0x00;
    
    cleanStub(&si_write);
    cleanStub(&si_recv);
//    printf("******** [%s:%d] ret=%d\n", __func__, __LINE__, ret);
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
    
    MQTTMessage pubmsg;
    memset(&pubmsg, '\0', sizeof(pubmsg));
	pubmsg.payload = (void*)"a much longer message that we can shorten to the extent that we need to payload up to 11";
	pubmsg.payloadlen = 11;
	pubmsg.qos = QOS0;
	pubmsg.retained = false;
    pubmsg.dup = false;

    char *topticName = (char*)"MQTTSAS topic";
    
    ret = MQTTPublish(&s_client, topticName, &pubmsg);
    TEST_ASSERT_MSG((FAILURE == ret), "MQTTPublish(...) failed");

    s_client.isconnected = 1;
    stubInfo si_write;
    setStub((void*)write, (void*)stub_write, &si_write);
    stubInfo si_recv;
    setStub((void*)recv, (void*)stub_recv, &si_recv);
    ret = MQTTPublish(&s_client, topticName, &pubmsg);
    TEST_ASSERT_MSG((0 == ret), "MQTTPublish(...) failed");

    pubmsg.qos = QOS1;
    // wait_for timeout
    ret = MQTTPublish(&s_client, topticName, &pubmsg);
    TEST_ASSERT_MSG((FAILURE == ret), "MQTTPublish(...) failed");

    s_client.isconnected = 1;
    s_packet_type = 0x40;
    ret = MQTTPublish(&s_client, topticName, &pubmsg);
    TEST_ASSERT_MSG((0 == ret), "MQTTPublish(...) failed");
    s_packet_type = 0x00;

    pubmsg.qos = QOS2;
    s_packet_type = 0x70;
    ret = MQTTPublish(&s_client, topticName, &pubmsg);
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

    MQTTSubackData subackData;
    memset(&subackData, 0, sizeof(MQTTSubackData));

    // timeout
    ret = MQTTSubscribeWithResults(&s_client, topticFilter, QOS0, message_handler, &subackData);
    TEST_ASSERT_MSG((FAILURE == ret), "MQTTSubscribeWithResults(...) failed");
    
    s_packet_type = 0x90;
    s_client.isconnected = 1;
    ret = MQTTSubscribeWithResults(&s_client, topticFilter, QOS0, message_handler, &subackData);
    TEST_ASSERT_MSG((0 == ret), "MQTTSubscribeWithResults(...) failed");
    s_packet_type = 0x00;

    cleanStub(&si_write);
    cleanStub(&si_recv);
}
void TestMQTTClient::test_MQTTUnsubscribe(void)
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
    s_client.isconnected = 0;
    ret = MQTTSubscribe(&s_client, topticFilter, QOS0, message_handler);
    TEST_ASSERT_MSG((FAILURE == ret), "MQTTSubscribe(...) failed");
    
    s_client.isconnected = 1;
    ret = MQTTSubscribe(&s_client, topticFilter, QOS0, message_handler);
    TEST_ASSERT_MSG((0 == ret), "MQTTSubscribe(...) failed");
    s_packet_type = 0x00;

    ret = MQTTUnsubscribe(&s_client, topticFilter);
    TEST_ASSERT_MSG((FAILURE == ret), "MQTTUnsubscribe(...) failed");
    s_client.isconnected = 0;
    ret = MQTTUnsubscribe(&s_client, topticFilter);
    TEST_ASSERT_MSG((FAILURE == ret), "MQTTUnsubscribe(...) failed");

    s_packet_type = 0xb0;
    s_client.isconnected = 1;
    ret = MQTTUnsubscribe(&s_client, topticFilter);
    TEST_ASSERT_MSG((0 == ret), "MQTTUnsubscribe(...) failed");
    s_packet_type = 0x00;

    cleanStub(&si_write);
    cleanStub(&si_recv);
}
void TestMQTTClient::test_MQTTDisconnect(void)
{    
    printf("Test %s link test_MQTTConnect...\n", __func__);
}
void TestMQTTClient::test_MQTTYield(void)
{
    int ret = 0;
    ret = MQTTYield(&s_client, 100);
    TEST_ASSERT_MSG((FAILURE == ret), "MQTTYield(...) failed");

    stubInfo si_write;
    setStub((void*)write, (void*)stub_write, &si_write);
    stubInfo si_recv;
    setStub((void*)recv, (void*)stub_recv, &si_recv);
    
    ret = MQTTYield(&s_client, 100);
    TEST_ASSERT_MSG((0 == ret), "MQTTYield(...) failed");
    
    cleanStub(&si_write);
    cleanStub(&si_recv);
}

void TestMQTTClient::test_MQTTRun(void)
{
    int ret = 0;
    pthread_t first_thread;
    
    ret = pthread_create(&first_thread, NULL, (pthread_func)MQTTRun, (void *)&s_client);
    TEST_ASSERT_MSG((0 == ret), "MQTTRun(...) failed");

    usleep(10000);
    pthread_cancel(first_thread);

    pthread_join(first_thread, NULL); 
}

void TestMQTTClient::test_cycle(void)
{
    Timer connect_timer;
    int ret = 0;

    memset(&connect_timer, 0, sizeof(Timer));

    TimerInit(&connect_timer);
    TimerCountdownMS(&connect_timer, s_client.command_timeout_ms);

    ret = cycle(&s_client, &connect_timer);
    TEST_ASSERT_MSG((FAILURE == ret), "cycle(...) failed");

    stubInfo si_write;
    setStub((void*)write, (void*)stub_write, &si_write);
    stubInfo si_recv;
    setStub((void*)recv, (void*)stub_recv, &si_recv);

    char topic[MAX_MESSAGE_HANDLERS+5][20] = { {"foo"}, {"+/foo"}, {"foo/+/#"}, {"#/foo"}, 
                        {"a/+foo"}, {"foo+/a"}};
    for (int i = 0; i < MAX_MESSAGE_HANDLERS; ++i){
        s_client.messageHandlers[i].topicFilter = (char*)topic[i];
        s_client.messageHandlers[i].fp = message_handler;
    }

    s_packet_type = 0x10;
    ret = cycle(&s_client, &connect_timer);
    TEST_ASSERT_MSG((1 == ret), "cycle(...) failed");

    s_packet_type = 0x20;
    ret = cycle(&s_client, &connect_timer);
    TEST_ASSERT_MSG((2 == ret), "cycle(...) failed");
    
    s_packet_type = 0x30; // PUBLISH
    
    stubInfo si_publish;
    setStub((void*)MQTTDeserialize_publish, (void*)stub_MQTTDeserialize_publish, &si_publish);
    memcpy(s_readbuf, "foo", strlen("foo"));
    memcpy(s_buf, "foo/#", strlen("foo/#"));
    ret = cycle(&s_client, &connect_timer);
    TEST_ASSERT_MSG((3 == ret), "cycle(...) failed");
    
    s_cstring = (char*)"cycle";
    ret = cycle(&s_client, &connect_timer);
    TEST_ASSERT_MSG((3 == ret), "cycle(...) failed");
    s_cstring = NULL;

    /////
    memcpy(s_buf, "foo", strlen("foo"));
    ret = cycle(&s_client, &connect_timer);
    TEST_ASSERT_MSG((3 == ret), "cycle(...) failed");

    s_intQoS = QOS2;
    ret = cycle(&s_client, &connect_timer);
    TEST_ASSERT_MSG((3 == ret), "cycle(...) failed");

    stubInfo si_serial_ack;
    setStub((void*)MQTTSerialize_ack, (void*)stub_MQTTSerialize_ack, &si_serial_ack);
    ret = cycle(&s_client, &connect_timer);
    TEST_ASSERT_MSG((-1 == ret), "cycle(...) failed");
    cleanStub(&si_serial_ack);
    cleanStub(&si_publish);
    
    s_client.defaultMessageHandler = message_handler;
    for(int i = 0; i < MAX_MESSAGE_HANDLERS; ++i){
        memcpy(s_readbuf, topic[i], strlen(topic[i]));
        s_client.messageHandlers[i].topicFilter = NULL;
        ret = cycle(&s_client, &connect_timer);
        TEST_ASSERT_MSG((3 == ret), "cycle(...) failed");
    }

    s_packet_type = 0x50;
    ret = cycle(&s_client, &connect_timer);
    TEST_ASSERT_MSG((5 == ret), "cycle(...) failed");

    s_write_flag = 0;
    ret = cycle(&s_client, &connect_timer);
    TEST_ASSERT_MSG((-1 == ret), "cycle(...) failed");
    s_write_flag = 1;
    
    setStub((void*)MQTTSerialize_ack, (void*)stub_MQTTSerialize_ack, &si_serial_ack);
    ret = cycle(&s_client, &connect_timer);
    TEST_ASSERT_MSG((-1 == ret), "cycle(...) failed");
    cleanStub(&si_serial_ack);

    stubInfo si_dser_ack;
    setStub((void*)MQTTDeserialize_ack, (void*)stub_MQTTDeserialize_ack, &si_dser_ack);
    s_client.isconnected = 1;
    ret = cycle(&s_client, &connect_timer);
    s_client.isconnected = 0;
    TEST_ASSERT_MSG(( FAILURE == ret), "cycle(...) failed");
    
    cleanStub(&si_dser_ack);
    
    s_packet_type = 0x70;
    unsigned int tmp = s_client.keepAliveInterval;
    s_client.keepAliveInterval = 0;
    ret = cycle(&s_client, &connect_timer);
    s_client.keepAliveInterval = tmp;
    TEST_ASSERT_MSG((7 == ret), "cycle(...) failed");
    
    s_packet_type = 0xd0;
    ret = cycle(&s_client, &connect_timer);
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
    int ret = 0;

    NetworkInit(&s_network);
    s_network.proto = MQTT_PROTO_NONE;
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
}
void TestMQTTClient::tear_down()
{
    int ret = 0;
    MQTTClientDeInit(&s_client);
    TEST_ASSERT_MSG((0 == ret), "MQTTClientDeInit(...) failed");
    NetworkDisconnect(&s_network);
    TEST_ASSERT_MSG((0 == ret), "NetworkDisconnect(...) failed");
}

