/******************************************************************************

  Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>

 ******************************************************************************
  File Name     : test_at_fota.cpp
  Version       : Initial Draft
  Author        : MAMINJIE
  Created       : 2018/8/3
  Last Modified :
  Description   : 
  Function List :
  History       :
  1.Date        : 2018/8/3
    Author      : MAMINJIE
    Modification: Created file

******************************************************************************/

/* Includes -----------------------------------------------------------------*/
#include "test_at_fota.h"

extern "C"
{
    #include <string.h>
    #include "at_fota.h"
    #include "atadapter.h"


    extern int32_t nb_send_str(const char* buf, int len);
    
    static int32_t stub_nb_send_str(const char* buf, int len)
    {
        return 0;
    }
}
/* Defines ------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
TestAtFota::TestAtFota()
{
    TEST_ADD(TestAtFota::test_ver_to_hex);
    TEST_ADD(TestAtFota::test_ota_cmd_ioctl);
    TEST_ADD(TestAtFota::test_at_ota_init);
    TEST_ADD(TestAtFota::test_ota_process_main);
}
TestAtFota::~TestAtFota()
{
    
}

void TestAtFota::test_ver_to_hex(void)
{
    int ret = 0;
    char *bufin = (char*)"123abc";
    int len = strlen(bufin);
    char bufout[1024] = {0};
    
    ret = ver_to_hex(bufin, len, bufout);
    TEST_ASSERT_MSG((0 == ret), "ver_to_hex(...) failed");
    printf("\"%s\" ver_to_hex() result is: \n", bufin);
    for(int i = 0; i < len; i++){
        printf("0x%02x ", bufout[i]);
        if(i+1 % 10 == 0) printf("\n");
    }
    printf("\n");
}
void TestAtFota::test_ota_cmd_ioctl(void)
{
    int ret = 0;
    OTA_CMD_E cmd = OTA_GET_VER;
    char arg[20] = {0};
    int len = sizeof(arg);
    
    ret = ota_cmd_ioctl(cmd, arg, len);
    TEST_ASSERT_MSG((0 == ret), "ota_cmd_ioctl(...) failed");

    cmd = OTA_NOTIFY_NEW_VER;
    ret = ota_cmd_ioctl(cmd, arg, len);
    TEST_ASSERT_MSG((0 == ret), "ota_cmd_ioctl(...) failed");

    cmd = OTA_WRITE_BLOCK;
    ret = ota_cmd_ioctl(cmd, arg, len);
    TEST_ASSERT_MSG((0 == ret), "ota_cmd_ioctl(...) failed");
    
    cmd = OTA_UPDATE_EXC;
    ret = ota_cmd_ioctl(cmd, arg, len);
    TEST_ASSERT_MSG((0 == ret), "ota_cmd_ioctl(...) failed");
}
void TestAtFota::test_at_ota_init(void)
{
    int ret = 0;
    char* featurestr = (char*)"Send OK";
    int cmdlen = strlen(featurestr);
    
    ret = at_ota_init( NULL, 0);
    TEST_ASSERT_MSG((-1 == ret), "at_ota_init(...) failed");
    
    ret = at_ota_init( featurestr, cmdlen);
    TEST_ASSERT_MSG((0 == ret), "at_ota_init(...) failed");
}
void TestAtFota::test_ota_process_main(void)
{
    int32_t ret = 0;
    char *arg = (char*)"123abc";
    int8_t buf[512] = "123456789abcdefghijk";
    int32_t len = strlen((const char*)buf);

    stubInfo stub_info;
    setStub((void*)nb_send_str, (void*)stub_nb_send_str, &stub_info);

    ret = ota_process_main(arg, NULL, 0);
    TEST_ASSERT_MSG((-1 == ret), "ota_cmd_ioctl(...) failed");
    
    ret = ota_process_main(arg, buf, len);
    TEST_ASSERT_MSG((-1 == ret), "ota_cmd_ioctl(...) failed");

    char *msg = (char*)"FFFE0120";
    len = strlen(msg);
    memcpy(buf, msg, len);
    ret = ota_process_main(arg, buf, len);
    TEST_ASSERT_MSG((0 == ret), "ota_cmd_ioctl(...) failed");


    msg = (char*)"FFFE0113";
    len = strlen(msg);
    memcpy(buf, msg, len);
    ret = ota_process_main(arg, buf, len);
    TEST_ASSERT_MSG((0 == ret), "ota_cmd_ioctl(...) failed");
    

    msg = (char*)"FFFE01142BB20020V201812345678901201812345678901d001000120002";
    len = strlen(msg);
    memcpy(buf, msg, len);
    ret = ota_process_main(arg, buf, len);
    TEST_ASSERT_MSG((0 == ret), "ota_cmd_ioctl(...) failed");

    for(int i = 0; i < 5; i++){
        msg = (char*)"FFFE0113";
        len = strlen(msg);
        memcpy(buf, msg, len);
        ret = ota_process_main(arg, buf, len);
        TEST_ASSERT_MSG((0 == ret), "ota_cmd_ioctl(...) failed");
    }

    
    msg = (char*)"FFFE01132BB2";
    len = strlen(msg);
    memcpy(buf, msg, len);
    ret = ota_process_main(arg, buf, len);
    TEST_ASSERT_MSG((0 == ret), "ota_cmd_ioctl(...) failed");
//    printf("[%s:%d] ******* ret=%d\n", __func__, __LINE__, ret);

    cleanStub(&stub_info);
}

/* Private functions --------------------------------------------------------*/
void TestAtFota::setup()
{
    
}
void TestAtFota::tear_down()
{
    
}

