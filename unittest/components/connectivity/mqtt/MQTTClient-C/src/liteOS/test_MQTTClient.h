/******************************************************************************

  Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>

 ******************************************************************************
  File Name     : test_MQTTClient.h
  Version       : Initial Draft
  Author        : MAMINJIE
  Created       : 2018/7/26
  Last Modified :
  Description   : test_MQTTClient.cpp header file
  Function List :
  History       :
  1.Date        : 2018/7/26
    Author      : MAMINJIE
    Modification: Created file

******************************************************************************/
/* Define to prevent recursive inclusion ------------------------------------*/

#ifndef __TEST_MQTTCLIENT_H__
#define __TEST_MQTTCLIENT_H__

/* Includes -----------------------------------------------------------------*/
#include <cpptest.h>
#include "stub.h"
/* Defines ------------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
/* Functions API ------------------------------------------------------------*/
class TestMQTTClient : public Test::Suite
{
public:
    TestMQTTClient();
    ~TestMQTTClient();

    void test_MQTTClientInit(void);
    void test_MQTTClientDeInit(void);
    void test_MQTTConnectWithResults(void);
    void test_MQTTConnect(void);
    void test_MQTTPublish(void);
    void test_MQTTSetMessageHandler(void);
    void test_MQTTSubscribe(void);
    void test_MQTTSubscribeWithResults(void);
    void test_MQTTUnsubscribe(void);
    void test_MQTTDisconnect(void);
    void test_MQTTYield(void);
    void test_MQTTRun(void);
    void test_cycle(void);
    void test_keepalive(void);
	void test_clean_close_session(void);

protected:
    void setup();
    void tear_down();
};

#endif /* __TEST_MQTTCLIENT_H__ */
