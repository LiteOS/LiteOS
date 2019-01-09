
#ifndef __TEST_HMAC_H__
#define __TEST_HMAC_H__

/* Includes -----------------------------------------------------------------*/
#include <cpptest.h>
#include "stub.h"
#include "MQTTClient.h"
#include "MQTTliteos.h"
#include "atiny_mqtt/mqtt_client.h"
#include "adapter_layer.h"
#include "hmac.h"
/* Defines ------------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
/* Functions API ------------------------------------------------------------*/
class TestHmac : public Test::Suite
{
public:
    TestHmac();
    ~TestHmac();

	void test_mbedtls_hmac_calc(void);
  #if 0
    void test_atiny_param_member_free(void);
	void test_atiny_param_dup(void);
	void test_atiny_init(void);
	void test_atiny_deinit(void);
	void test_mqtt_add_interest_topic(void);
	void test_mqtt_is_topic_subscribed(void);
	void test_mqtt_del_interest_topic(void);
	void test_mqtt_is_topic_subscribed_same(void);
	void test_mqtt_topic_subscribe(void);
	void test_mqtt_topic_unsubscribe(void);
	void test_mqtt_message_publish(void);
	void test_mqtt_message_arrived(void);
	void test_mqtt_subscribe_interest_topics(void);
	void test_will_options_member_free(void);
	void test_device_info_member_free(void);
	void test_device_info_dup(void);
	void test_atiny_isconnected(void);
	void test_atiny_bind(void);
	void test_atiny_data_send(void);
  #endif


protected:
    void setup();
    void tear_down();
};



#endif /* __TEST_HMAC_H__ */

