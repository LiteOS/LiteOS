/******************************************************************************

  Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>

 ******************************************************************************
  File Name     : test_at_fota_hal.cpp
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
#include "test_at_fota_hal.h"

extern "C"
{
#include <string.h>    
#include "at_fota_hal.h"

}
/* Defines ------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
TestAtFotaHal::TestAtFotaHal()
{
    TEST_ADD(TestAtFotaHal::test_at_fota_write);
    TEST_ADD(TestAtFotaHal::test_crc_check);
    TEST_ADD(TestAtFotaHal::test_sota_str_to_hex);
    TEST_ADD(TestAtFotaHal::test_HexStrToByte);
    TEST_ADD(TestAtFotaHal::test_do_crc);
}
TestAtFotaHal::~TestAtFotaHal()
{
    
}

void TestAtFotaHal::test_at_fota_write(void)
{
    int ret = 0;
    int offset = 0;
    char *buffer = (char*)"123456abcdefg";
    int len = strlen(buffer);

    for(int i = 0; i < 0x1000/len+10; i++){
        ret = at_fota_write(offset, buffer, len);
        TEST_ASSERT_MSG((0 == ret), "at_fota_write(...) failed");
        offset += len;
    }
}
void TestAtFotaHal::test_crc_check(void)
{
    int ret = 0;
    unsigned char *message = (unsigned char*)"123";
    int len = strlen((const char*)message);
    
    ret = crc_check(message, len);
    TEST_ASSERT_MSG((56084 == ret), "crc_check(...) failed");
//    printf("[%s:%d] ******* ret=%d\n", __func__, __LINE__, ret);
}
void TestAtFotaHal::test_sota_str_to_hex(void)
{
    int ret = 0;
    char *bufin = (char*)"123abc";
    int len = strlen(bufin);
    unsigned char bufout[1024] = {0};
    
    ret = sota_str_to_hex(bufin, len, bufout);
    TEST_ASSERT_MSG((0 == ret), "sota_str_to_hex(...) failed");
    printf("\"%s\" sota_str_to_hex() result is: \n", bufin);
    for(int i = 0; i < len/2; i++){
        printf("0x%02x ", bufout[i]);
        if(i+1 % 10 == 0) printf("\n");
    }
    printf("\n");
}
void TestAtFotaHal::test_HexStrToByte(void)
{
    int ret = 0;
    unsigned char *source = (unsigned char*)"abc123";
    unsigned char dest[50] = {0};
    int source_len = strlen((const char*)source);
    
    HexStrToByte(source, dest, source_len);
    TEST_ASSERT_MSG((0 == ret), "HexStrToByte(...) failed");
    printf("\"%s\" HexStrToByte() result is: \n", source);
    for(int i = 0; i < source_len/2; i++){
        printf("0x%02x ", dest[i]);
        if(i+1 % 10 == 0) printf("\n");
    }
    printf("\n");
}
void TestAtFotaHal::test_do_crc(void)
{
    printf("test do_crc() like test_crc_check()...\n");
}


/* Private functions --------------------------------------------------------*/

void TestAtFotaHal::setup()
{
    
}
void TestAtFotaHal::tear_down()
{
    
}

