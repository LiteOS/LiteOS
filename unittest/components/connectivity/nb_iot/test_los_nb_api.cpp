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


#include "test_los_nb_api.h"
#include <sys/types.h>

extern "C" {
    #include "at_frame/at_api.h"
    #include "nb_iot/los_nb_api.h"
	extern int32_t nb_data_ioctl(void* arg,int8_t * buf, int32_t len);
	extern int32_t at_cmd(int8_t * cmd, int32_t len, const char * suffix, char * rep_buf);
	static char g_state = 0;
	static int32_t stub_at_cmd(int8_t * cmd, int32_t len, const char * suffix, char * rep_buf)
    {

        if(g_state == 1){
            return AT_FAILED;
        }
		if(NULL != rep_buf)
            memcpy(rep_buf, "SENT=6,\"0 1 2 a b\"", strlen("SENT=6,\"0 1 2 a b\""));
        return AT_OK;
    }
    
    
}

void TestNBApi::test_nb_data_ioctl()
{
    int ret = 0;
    int8_t buf[50] = {5,4,3,2};
    uint arg[50] = {1,2,3,4};
	
    ret = nb_data_ioctl(arg,NULL,0);
    TEST_ASSERT_MSG((ret != -1), "test nb_data_ioctl failed!");
    
    
    ret = nb_data_ioctl(arg,buf,50);
    TEST_ASSERT_MSG((ret != -1), "test nb_data_ioctl failed!");
    printf("exit test_nb_data_ioctl\n");
    return;
}

void TestNBApi::test_los_nb_init()
{
    int ret = 0;
    int8_t * host = (int8_t*)"localhost";
    int8_t * port = (int8_t*)"9999";
	stubInfo stub_info;

	sec_param_s sec;
    sec.pskid = "868744031131026";
    sec.psk = "d1e1be0c05ac5b8c78ce196412f0cdb0";
    //g_state = TEST_STATE_ERR;
    setStub((void *)at_cmd,(void *)stub_at_cmd,&stub_info);
	
    printf("in test_los_nb_init 89\n");
    ret = los_nb_init(host, port, &sec);
	at_free(at.linkid);
	at.linkid = NULL;
	at_free(at.userdata);
	at.userdata = NULL;
	
	at_free(at.saveddata);
	at.saveddata  = NULL;
	at_free(at.recv_buf);
	at.recv_buf = NULL;
	at_free(at.cmdresp);
	at.cmdresp = NULL;

    printf("in test_los_nb_init 91\n");
	printf("ret is %d in test_los_nb_init\n",ret);
    TEST_ASSERT_MSG((ret != -1), "test at_api_connect for connect is NULL failed!");
    
    ret = los_nb_init(host, port, &sec);
	at_free(at.linkid);
	at.linkid = NULL;
	at_free(at.userdata);
	at.userdata = NULL;
	at_free(at.saveddata);
	at.saveddata  = NULL;
	at_free(at.recv_buf);
	at.recv_buf = NULL;
	
	printf("exit test_los_nb_init,ret is %d\n",ret);
    TEST_ASSERT_MSG((ret == AT_OK), "test at_api_connect for connect is normal failed!");
	printf("exit test_los_nb_init\n");
	cleanStub(&stub_info);
    return;
}

void TestNBApi::test_los_nb_report()
{

    int ret = 0;
	char buf[6] = {'0','1','2','a','b'};
	
    printf("come in test_los_nb_report\n");
#if 1
    stubInfo stub_info;

    setStub((void *)at_cmd,(void *)stub_at_cmd,&stub_info);
	
    
	

    ret = los_nb_report(NULL, 0);
    TEST_ASSERT_MSG((ret == -1), "test los_nb_report NULL failed!");
    printf("now in test_los_nb_report\n");


    ret = los_nb_report((const char *)buf, 6);
    TEST_ASSERT_MSG((ret == 0), "test los_nb_report for normal failed!");
	cleanStub(&stub_info);
#endif

	printf("exit test_los_nb_report\n");

    return;
}
int32_t nb_data_ioctl_test(void* arg,int8_t * buf, int32_t len)
{
	return 0;
}

void TestNBApi::test_los_nb_notify()
{
    int ret = 0;
    printf("come in test_los_nb_notify\n");
    ret = los_nb_notify(NULL, 0, NULL,NULL);//last NULL added by shensheng
    TEST_ASSERT_MSG((ret == -1), "test at_api_recv for recv_func is NULL failed!");
	printf("exit test_los_nb_notify 2\n");
    
    ret = los_nb_notify("+NNMI:",strlen("+NNMI:"),nb_data_ioctl_test,NULL);//last NULL added by shensheng
    TEST_ASSERT_MSG((ret != AT_OK), "test at_api_recv for recv_func is normal failed!");
	
	printf("exit test_los_nb_notify 2\n");
    return;
}

void TestNBApi::test_los_nb_deinit()
{
    int ret = 0;
    
    ret = los_nb_deinit();
    TEST_ASSERT_MSG((ret == 0), "test at_los_nb_deinit for recv_timeout_func is NULL failed!");
    
    return;
}




TestNBApi::TestNBApi()
{
    TEST_ADD(TestNBApi::test_nb_data_ioctl);
    TEST_ADD(TestNBApi::test_los_nb_init);
    TEST_ADD(TestNBApi::test_los_nb_report);
    TEST_ADD(TestNBApi::test_los_nb_notify);
    TEST_ADD(TestNBApi::test_los_nb_deinit);
}

void TestNBApi::setup()
{
    return;
}
void TestNBApi::tear_down()
{
    return ;
}


