/*
 * Copyright 2008 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

 #include "sota_test.h"
extern unsigned char* rabuf;
 
static void It_Los_sota_init_001(void **state)       //flash_op = NULL
{
    int ret;
    g_flash_op_t = NULL;
    hal_init_ota();
    ret = sota_init(g_flash_op_t);
    at_free(rabuf);
    rabuf= NULL;
    assert_int_equal(ret, SOTA_FAILED);
    
}

static void It_Los_sota_init_002(void **state)     //data_len = 0
{
    sota_opt_t flash_op =
    {
    .get_ver = read_ver,
    .set_ver = set_ver,
    .sota_send = nb_send_str,
    };
    hal_get_ota_opt(&flash_op.ota_info);
    flash_op.ota_info.key.rsa_N = "C94BECB7BCBFF459B9A71F12C3CC0603B11F0D3A366A226FD3E73D453F96EFBBCD4DFED6D9F77FD78C3AB1805E1BD3858131ACB5303F61AF524F43971B4D429CB847905E68935C1748D0096C1A09DD539CE74857F9FDF0B0EA61574C5D76BD9A67681AC6A9DB1BB22F17120B1DBF3E32633DCE34F5446F52DD7335671AC3A1F21DC557FA4CE9A4E0E3E99FED33A0BAA1C6F6EE53EDD742284D6582B51E4BF019787B8C33C2F2A095BEED11D6FE68611BD00825AF97DB985C62C3AE0DC69BD7D0118E6D620B52AFD514AD5BFA8BAB998332213D7DBF5C98DC86CB8D4F98A416802B892B8D6BEE5D55B7E688334B281E4BEDDB11BD7B374355C5919BA5A9A1C91F";
    flash_op.ota_info.key.rsa_E = "10002";
    flash_op.frame_buf_len = 0;
    hal_init_ota();
    int ret;

    ret = sota_init(&flash_op);
    at_free(rabuf);
    rabuf= NULL;
    assert_int_equal(ret, SOTA_FAILED);
    
}

static void It_Los_sota_init_003(void **state)     //ota_info not init
{
    sota_opt_t flash_op =
    {
    .get_ver = read_ver,
    .set_ver = set_ver,
    .sota_send = nb_send_str,
    };
    hal_get_ota_opt(&flash_op.ota_info);
    flash_op.frame_buf_len = 10;

    hal_init_ota();
    int ret;

    ret = sota_init(&flash_op);
    at_free(rabuf);
    rabuf= NULL;
    assert_int_equal(ret, SOTA_OK);
    
}

static void It_Los_sota_init_004(void **state)       //normal
{
	int ret;
	ret = sota_init_normal();
	at_free(rabuf);
	rabuf= NULL;
	assert_int_equal(ret, SOTA_OK);
}

static void It_Los_ota_process_main_001(void **state)  // buf=NULL
{
    int8_t* buf = NULL;
	int32_t buflen = 0;
	if(rabuf == NULL)
	{
		sota_init_normal();
	}
    int32_t ret;
    ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_FAILED);
}

static void It_Los_ota_process_main_002(void **state)  //prefix is wrong
{
    int8_t buf[] = "NNMM";
	int32_t buflen = strlen((const char *)buf);
	if(rabuf == NULL)
	{
		sota_init_normal();
	}
    int32_t ret;
    ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_FAILED);
}

static void It_Los_ota_process_main_003(void **state)  //buflen = 0
{
    int8_t buf[] = "+NNMI:8,FFFE01134c9a0000";
	int32_t buflen = 0;
	if(rabuf == NULL)
	{
		sota_init_normal();
	}
    int32_t ret;

    ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_FAILED);
}

static void It_Los_ota_process_main_004(void **state)  //MSG_GET_VER
{
    int8_t buf[] = "+NNMI:8,FFFE01134c9a0000";
	int32_t buflen = strlen((const char *)buf);
	if(rabuf == NULL)
	{
		sota_init_normal();
	}
    int32_t ret;
    
    ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_OK);
}

static void It_Los_ota_process_main_005(void **state)   //MSG_NOTIFY_NEW_VER
{
    int8_t buf[] = "+NNMI:8,FFFE0114e51f0000";
	int32_t buflen = strlen((const char *)buf);
	if(rabuf == NULL)
	{
		sota_init_normal();
	}
    int32_t ret;
    ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_DOWNLOADING);
}

static void It_Los_ota_process_main_006(void **state)   //crc_code is error
{
    int8_t buf[] = "+NNMI:8,FFFE0114abcd0000";
	int32_t buflen = strlen((const char *)buf);
	if(rabuf == NULL)
	{
		sota_init_normal();
	}
    int32_t ret;
    ret = sota_process_main(arg,buf,buflen);

	assert_int_equal(ret, SOTA_FAILED);
}

static void It_Los_ota_process_main_007(void **state)  //format wrong
{
    int8_t buf[] = "+NNMI,8,FFFE01151c840000";
	int32_t buflen = strlen((const char *)buf);
	if(rabuf == NULL)
	{
		sota_init_normal();
	}
    int32_t ret;
    ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, AT_FAILED);
}

static void It_Los_ota_process_main_008(void **state)   //msg_code = MSG_EXC_UPDATE
{
    int8_t buf[] = "+NNMI:10,FFFE0117dbb700020001";
	int32_t buflen = strlen((const char *)buf);
	if(rabuf == NULL)
	{
		sota_init_normal();
	}
    int32_t ret;
    ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_OK);
}

static void It_Los_ota_process_main_009(void **state)   // msg_code = MSG_UPDATE_STATE
{
    int8_t buf[] = "+NNMI:9,FFFE0116850E000100";
	int32_t buflen = strlen((const char *)buf);
	if(rabuf == NULL)
	{
		sota_init_normal();
	}
    int32_t ret;
    ret = sota_process_main(arg,buf,buflen);

    assert_int_equal( ret, SOTA_OK);
}

static void It_Los_ota_process_main_010(void **state)     // msg_code = MSG_NOTIFY_STATE
{
    int8_t buf[] = "+NNMI:9,FFFE0118afa1000100";
	int32_t buflen = strlen((const char *)buf);
	if(rabuf == NULL)
	{
		sota_init_normal();
	}
    int32_t ret;
    ret = sota_process_main(arg,buf,buflen);

    assert_int_equal( ret, SOTA_OK);
}
static void It_Los_ota_process_main_011(void **state)  //  msg_code =  MSG_GET_VER and datalen = 1
{
    int8_t buf[] = "+NNMI:9,FFFE011316ec000100";
	int32_t buflen = strlen((const char *)buf);
	if(rabuf == NULL)
	{
		sota_init_normal();
	}
    int32_t ret;
    
    ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_OK);
}

static void It_Los_ota_process_main_012(void **state)   //  msg_code =  MSG_NOTIFY_NEW_VER and datalen = 2
{
    int8_t buf[] = "+NNMI:10,FFFE011442be00020001";
	int32_t buflen = strlen((const char *)buf);
	if(rabuf == NULL)
	{
		sota_init_normal();
	}
    int32_t ret;
    ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_DOWNLOADING);
}

static void It_Los_ota_process_main_013(void **state)   //  msg_code < 19
{
    int8_t buf[] = "+NNMI:8,FFFE0112b5010000";
	int32_t buflen = strlen((const char *)buf);
	if(rabuf == NULL)
	{
		sota_init_normal();
	}
    int32_t ret;
    ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_FAILED);
}

static void It_Los_ota_process_main_014(void **state)   //  msg_code > 24
{
    int8_t buf[] = "+NNMI:8,FFFE0119d34e0000";
	int32_t buflen = strlen((const char *)buf);
	if(rabuf == NULL)
	{
		sota_init_normal();
	}
    int32_t ret;
    ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_FAILED);
}

static void It_Los_ota_process_main_015(void **state)  //ver_num != 1
{
    int8_t buf[] = "+NNMI:8,FFFE02134c9a0000";
	int32_t buflen = strlen((const char *)buf);
	if(rabuf == NULL)
	{
		sota_init_normal();
	}
    int32_t ret;
    
    ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_FAILED);
}

static void It_Los_ota_process_main_016(void **state)  //ver_num =null
{
    int8_t buf[] = "+NNMI:8,FFFE134c9a0000";
	int32_t buflen = strlen((const char *)buf);
	if(rabuf == NULL)
	{
		sota_init_normal();
	}
    int32_t ret;
    
    ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_FAILED);
}

static void It_Los_ota_process_main_017(void **state)  //crc_code = null
{
    int8_t buf[] = "+NNMI:8,FFFE0113";
	int32_t buflen = strlen((const char *)buf);
	if(rabuf == NULL)
	{
		sota_init_normal();
	}
    int32_t ret;
    
    ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_FAILED);
}

static void It_Los_ota_process_main_018(void **state)  //data_length = null
{
    int8_t buf[] = "+NNMI:8,FFFE02134c9a";
	int32_t buflen = strlen((const char *)buf);
	if(rabuf == NULL)
	{
		sota_init_normal();
	}
    int32_t ret;
    
    ret = sota_process_main(arg,buf,buflen);
    at_free(rabuf);
    rabuf= NULL;
    assert_int_equal(ret, SOTA_FAILED);
}

int sota_test_main(void)
{
    const UnitTest tests[] =
    {
        unit_test(It_Los_sota_init_001),
        unit_test(It_Los_sota_init_002),
        unit_test(It_Los_sota_init_003),
        unit_test(It_Los_sota_init_004),
        unit_test(It_Los_ota_process_main_001),
        unit_test(It_Los_ota_process_main_002),
        unit_test(It_Los_ota_process_main_003),
        unit_test(It_Los_ota_process_main_004),
        unit_test(It_Los_ota_process_main_005),
		unit_test(It_Los_ota_process_main_006),
        unit_test(It_Los_ota_process_main_007),
        unit_test(It_Los_ota_process_main_008),
        unit_test(It_Los_ota_process_main_009),
        unit_test(It_Los_ota_process_main_010),
        unit_test(It_Los_ota_process_main_011),
		unit_test(It_Los_ota_process_main_012),
		unit_test(It_Los_ota_process_main_013),
		unit_test(It_Los_ota_process_main_014),
		unit_test(It_Los_ota_process_main_015),
		unit_test(It_Los_ota_process_main_016),
		unit_test(It_Los_ota_process_main_017),
		unit_test(It_Los_ota_process_main_018),
    };

    printf("Huawei LiteOS Sota update Test\n");

    run_tests(tests);

    return 0;
}
