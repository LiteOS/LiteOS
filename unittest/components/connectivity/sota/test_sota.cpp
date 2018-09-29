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
#include "test_sota.h"


/* Defines ------------------------------------------------------------------*/

#define htons_ota(x) ((((x) & 0x00ff) << 8) | (((x) & 0xff00) >> 8))
//#define __DEBUG__

/* Extern variables ---------------------------------------------------------*/

extern "C"
{
    #include <stdio.h>
    #include <string.h>
    #include "sota.h"
    #include "atadapter.h"
    #include "los_swtmr.h"

    extern int ota_report_result(void);
    extern int32_t nb_send_str(const char* buf, int len);

    SWTMR_PROC_FUNC s_function_handler = NULL;
    UINT32 s_arg = 0;
    
    static int32_t stub_nb_send_str(const char* buf, int len)
    {
#ifdef __DEBUG__
        printf("nb_send_str is: \n\t");
        for(int i = 0; i < len/2; i++){
            printf("%c", buf[i]);
            if((i+1)%40 == 0) printf("\n\t");
        }
        printf("\n");
#endif
        return 0;
    }

    int hex_to_str(const char *bufin, int len, char *bufout)
    {
        int i = 0;
        if (NULL == bufin || len <= 0 || NULL == bufout)
        {
            return -1;
        }
        for(i = 0; i < len; i++)
        {
            sprintf(bufout+i*2, "%02X", (unsigned char)bufin[i]);
        }
        return 0;
    }

    static void print_hex_to_string(const char *bufin, int len, char *bufout)
    {
#ifdef __DEBUG__
        printf("Hex 0x");
        for(int i = 0; i < len; i++){
            printf("%02x", (unsigned char)bufin[i]);
        }
        printf(" hex_to_string() result is: \n\tString \"");
        for(int i = 0; i < len*2; i++){
            printf("%c", (unsigned char)bufout[i]);
        }
        printf("\"\n");
#endif
    }

    extern UINT32 LOS_SwtmrCreate(UINT32 uwInterval, UINT8 ucMode, SWTMR_PROC_FUNC pfnHandler, UINT16 *pusSwTmrID, UINT32 uwArg
#if (LOSCFG_BASE_CORE_SWTMR_ALIGN == YES)
                        , UINT8 ucRouses, UINT8 ucSensitive
#endif
                        );

    static UINT32 stub_LOS_SwtmrCreate(UINT32 uwInterval, UINT8 ucMode, SWTMR_PROC_FUNC pfnHandler, UINT16 *pusSwTmrID, UINT32 uwArg
#if (LOSCFG_BASE_CORE_SWTMR_ALIGN == YES)
                    , UINT8 ucRouses, UINT8 ucSensitive
#endif
                    )
    {
        s_function_handler = pfnHandler;
        s_arg = uwArg;
        return 1;
    }
}

/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
TestSota::TestSota()
{
    TEST_ADD(TestSota::test_ver_to_hex);
//    TEST_ADD(TestSota::test_ota_cmd_ioctl);
    TEST_ADD(TestSota::test_at_ota_init);
    TEST_ADD(TestSota::test_ota_report_result);
    TEST_ADD(TestSota::test_ota_process_main);
}
TestSota::~TestSota()
{
    
}

void TestSota::test_ver_to_hex(void)
{
    int ret = 0;
    char bufin[] = {0x30, 0x31, 0x37, 0x38};
    int len = sizeof(bufin);
    char bufout[50] = {0};
    
    ret = ver_to_hex(bufin, len, bufout);
    TEST_ASSERT_MSG((0 == ret), "ver_to_hex(...) failed");
    print_hex_to_string(bufin, len, bufout);
}
void TestSota::test_ota_cmd_ioctl(void)
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
void TestSota::test_at_ota_init(void)
{
    int ret = 0;
    char* featurestr = (char*)"Send OK";
    int cmdlen = strlen(featurestr);
    
    ret = at_ota_init( NULL, 0);
    TEST_ASSERT_MSG((-1 == ret), "at_ota_init(...) failed");

    stubInfo si;
    setStub((void*)LOS_SwtmrCreate, (void*)stub_LOS_SwtmrCreate, &si);
    ret = at_ota_init( featurestr, cmdlen);
    TEST_ASSERT_MSG((-1 == ret), "at_ota_init(...) failed");
    cleanStub(&si);
    
    ret = at_ota_init( featurestr, cmdlen);
    TEST_ASSERT_MSG((0 == ret), "at_ota_init(...) failed");
}

void TestSota::test_ota_report_result(void)
{
    int ret = 0;
    ret = ota_report_result();
    TEST_ASSERT_MSG((-1 == ret), "ota_report_result() failed");
}

void TestSota::test_ota_process_main(void)
{
    int32_t ret = 0;
    char *arg = (char*)"unuse";
    int8_t buf[512] = {0};
    int8_t tx_buf[1064] = "invalid header";
    int32_t len = strlen((const char*)tx_buf);

    stubInfo stub_info;
    setStub((void*)nb_send_str, (void*)stub_nb_send_str, &stub_info);

    ret = ota_process_main(arg, NULL, 0);
    TEST_ASSERT_MSG((-1 == ret), "ota_process_main(...) failed");
    
    ret = ota_process_main(arg, tx_buf, len);
    TEST_ASSERT_MSG((-1 == ret), "ota_process_main(...) failed");

    ////////////////////////
    ota_pcp_head_s* phead = (ota_pcp_head_s*)buf;
    uint8_t *pbuf = (uint8_t*)(buf + 8);
    ota_ver_notify_t* notify = (ota_ver_notify_t*)(buf+8);
    
    phead->ori_id = htons_ota(0xFFFE);
    phead->ver_num = 1;
    phead->chk_code = htons_ota(0x2BB2);
    phead->data_len = htons_ota(0x0010);

    // test get version
    phead->msg_code = MSG_GET_VER;
    len = sizeof(ota_pcp_head_s);
    hex_to_str((const char*)buf, len, (char*)tx_buf);
    print_hex_to_string((const char*)buf, len, (char*)tx_buf);
    len *= 2;
    ret = ota_process_main(arg, tx_buf, len);
    TEST_ASSERT_MSG((0 == ret), "ota_process_main(...) failed");

    phead->msg_code = MSG_NOTIFY_STATE;
    len = sizeof(ota_pcp_head_s);
    hex_to_str((const char*)buf, len, (char*)tx_buf);
    print_hex_to_string((const char*)buf, len, (char*)tx_buf);
    len *= 2;
    ret = ota_process_main(arg, tx_buf, len);
    TEST_ASSERT_MSG((0 == ret), "ota_process_main(...) failed");

    // test notify new version
    phead->msg_code = MSG_NOTIFY_NEW_VER;
    notify->ver_chk_code = htons_ota(0x2BB2);
    len = sizeof(ota_pcp_head_s)+sizeof(ota_ver_notify_t);
    hex_to_str((const char*)buf, len, (char*)tx_buf);
    print_hex_to_string((const char*)buf, len, (char*)tx_buf);
    len *= 2;
    ret = ota_process_main(arg, tx_buf, len);
    TEST_ASSERT_MSG((0 == ret), "ota_process_main(...) failed");

    // downloading...
    // 1. error
    *pbuf = OTA_ERR;
    *(pbuf+1) = 0x00;
    *(pbuf+2) = 0x00;
    len = sizeof(ota_pcp_head_s)+3;
    hex_to_str((const char*)buf, len, (char*)tx_buf);
    print_hex_to_string((const char*)buf, len, (char*)tx_buf);
    len *= 2;
    ret = ota_process_main(arg, tx_buf, len);
    TEST_ASSERT_MSG((-1 == ret), "ota_process_main(...) failed");

    // test notify new version
    phead->msg_code = MSG_NOTIFY_NEW_VER;
    notify->ver_chk_code = htons_ota(0x3DD3);
    len = sizeof(ota_pcp_head_s)+sizeof(ota_ver_notify_t);
    hex_to_str((const char*)buf, len, (char*)tx_buf);
    print_hex_to_string((const char*)buf, len, (char*)tx_buf);
    len *= 2;
    ret = ota_process_main(arg, tx_buf, len);
    TEST_ASSERT_MSG((0 == ret), "ota_process_main(...) failed");

    if(s_function_handler){
        s_function_handler(s_arg);
    }

    // 2. OK
    *pbuf = OTA_OK;
    *(pbuf+1) = 0x00;
    *(pbuf+2) = 0x00;
    len = sizeof(ota_pcp_head_s)+3;
    hex_to_str((const char*)buf, len, (char*)tx_buf);
    print_hex_to_string((const char*)buf, len, (char*)tx_buf);
    len *= 2;
    ret = ota_process_main(arg, tx_buf, len);
    TEST_ASSERT_MSG((0 == ret), "ota_process_main(...) failed");

    // downloaded.
    // update state
    phead->msg_code = MSG_UPDATE_STATE;
    len = sizeof(ota_pcp_head_s);
    hex_to_str((const char*)buf, len, (char*)tx_buf);
    print_hex_to_string((const char*)buf, len, (char*)tx_buf);
    len *= 2;
    ret = ota_process_main(arg, tx_buf, len);
    TEST_ASSERT_MSG((0 == ret), "ota_process_main(...) failed");

    phead->msg_code = MSG_NOTIFY_NEW_VER;
    notify->block_size = 10;
    notify->block_totalnum = 2;
    notify->ver_chk_code = htons_ota(0x4CC4);
    len = sizeof(ota_pcp_head_s)+sizeof(ota_ver_notify_t);
    hex_to_str((const char*)buf, len, (char*)tx_buf);
    print_hex_to_string((const char*)buf, len, (char*)tx_buf);
    len *= 2;
    ret = ota_process_main(arg, tx_buf, len);
    TEST_ASSERT_MSG((0 == ret), "ota_process_main(...) failed");

    // OK
    *pbuf = OTA_OK;
    *(pbuf+1) = 0x00;
    *(pbuf+2) = 0x00;
    len = sizeof(ota_pcp_head_s)+3;
    hex_to_str((const char*)buf, len, (char*)tx_buf);
    print_hex_to_string((const char*)buf, len, (char*)tx_buf);
    len *= 2;
    ret = ota_process_main(arg, tx_buf, len);
    TEST_ASSERT_MSG((0 == ret), "ota_process_main(...) failed");


    phead->msg_code = MSG_GET_BLOCK;
    len = sizeof(ota_pcp_head_s);
    hex_to_str((const char*)buf, len, (char*)tx_buf);
    print_hex_to_string((const char*)buf, len, (char*)tx_buf);
    len *= 2;
    ret = ota_process_main(arg, tx_buf, len);
    TEST_ASSERT_MSG((-1 == ret), "ota_process_main(...) failed");
    
    // head wrong
    phead->msg_code = MSG_GET_VER-1;
    len = sizeof(ota_pcp_head_s);
    hex_to_str((const char*)buf, len, (char*)tx_buf);
    print_hex_to_string((const char*)buf, len, (char*)tx_buf);
    len *= 2;
    ret = ota_process_main(arg, tx_buf, len);
    TEST_ASSERT_MSG((-1 == ret), "ota_process_main(...) failed");

    cleanStub(&stub_info);
}

/* Private functions --------------------------------------------------------*/
void TestSota::setup()
{
    
}
void TestSota::tear_down()
{
    
}

