#include "test_agenttiny.h"

void test_atiny_bind_000 (void **state)
{
	void *phandle;
	atiny_param_t uwAtiny_params;
	memset(&uwAtiny_params, 0, sizeof(atiny_param_t));
    atiny_device_info_t uwDevice_info;
    memset(&uwDevice_info, 0, sizeof(atiny_device_info_t));
	int ret = 0;

	uwAtiny_params.server_params.binding = (char *)"UQ";
	uwAtiny_params.server_params.life_time = 20;
	uwAtiny_params.server_params.storing_cnt = 0;

	uwAtiny_params.server_params.bootstrap_mode = BOOTSTRAP_FACTORY;
	uwAtiny_params.server_params.hold_off_time = 10;
#ifdef WITH_DTLS
    uwAtiny_params.security_params[0].server_ip = TEST_LWM2M_SERVER_DTLS_IP;
	uwAtiny_params.security_params[0].server_port = TEST_LWM2M_SERVER_DTLS_PORT;
	uwAtiny_params.security_params[0].psk_Id = TEST_LWM2M_SERVER_PSK_ID;
	uwAtiny_params.security_params[0].psk = TEST_LWM2M_SERVER_PSK;
	uwAtiny_params.security_params[0].psk_len = sizeof(TEST_LWM2M_SERVER_PSK);
    
    uwAtiny_params.security_params[1].server_ip = TEST_LWM2M_SERVER_DTLS_IP;
	uwAtiny_params.security_params[1].server_port = TEST_LWM2M_SERVER_DTLS_PORT;
	uwAtiny_params.security_params[1].psk_Id = TEST_LWM2M_SERVER_PSK_ID;
	uwAtiny_params.security_params[1].psk = TEST_LWM2M_SERVER_PSK;
	uwAtiny_params.security_params[1].psk_len = sizeof(TEST_LWM2M_SERVER_PSK);
    
#else
	uwAtiny_params.security_params[0].server_ip = TEST_LWM2M_SERVER_IP;
	uwAtiny_params.security_params[0].server_port = TEST_LWM2M_SERVER_PORT;
    
    uwAtiny_params.security_params[1].server_ip = TEST_LWM2M_SERVER_IP;
	uwAtiny_params.security_params[1].server_port = TEST_LWM2M_SERVER_PORT;
#endif

    uwDevice_info.endpoint_name = "99990009";
    uwDevice_info.manufacturer = "LiteOS";
    uwDevice_info.dev_type = "IOT";
    
    ret = atiny_init(&uwAtiny_params, &phandle);
    assert_int_equal(ret, ATINY_OK);
    
    ret = atiny_bind(NULL,NULL);
    assert_int_equal(ret, ATINY_ARG_INVALID);
    
    ret = atiny_bind(&uwDevice_info,NULL);
    assert_int_equal(ret, ATINY_ARG_INVALID);
    
	ret = atiny_bind(NULL,phandle);
	assert_int_equal(ret, ATINY_ARG_INVALID);
    
    uwDevice_info.endpoint_name = NULL;
    ret = atiny_bind(&uwDevice_info,&phandle);
    assert_int_equal(ret, ATINY_ARG_INVALID);
    
    uwDevice_info.endpoint_name = "99990009";
    uwDevice_info.manufacturer = NULL;
    assert_int_equal(ret, ATINY_ARG_INVALID);

}

