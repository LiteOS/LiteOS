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
#include "stdio.h"
#include "test_upgrade_flag.h"
#include "upgrade_flag.h"
#include "string.h"
/*
int flag_upgrade_init(void)
{
    int ret;
    uint32_t crc;

    ret = flag_read(FLAG_BOOTLOADER, &g_flag, sizeof(upgrade_flag_s));
    if (ret != 0) return ret;

					crc = calc_crc32(0, &g_flag, sizeof(upgrade_flag_s) - sizeof(uint32_t));
	//	g_flag.crc_flag = calc_crc32(0, &g_flag, sizeof(upgrade_flag_s) - sizeof(uint32_t));
    if (crc != g_flag.crc_flag)
    {
        g_flag.upgrade_state = OTA_IDLE;
        g_flag.recover_verify = 0;
        g_flag.image_size = 0;
        g_flag.old_image_size = 0;
        return save_flag();
    }

    return 0;
}
*/
#include "flag_manager.h"
int stub_flag_read(flag_type_e flag_type, void *buf, int32_t len)
{
  return -1;
}

void TestUpgradeFlag::test_flag_upgrade_init()
{
	//无参数
	
	//ret = flag_read() 有 0和-1
	

	flag_upgrade_init();
	
	//打桩
	//1
   stubInfo stub_sem;
   setStub((void *)flag_read,(void *)stub_flag_read,&stub_sem); 
   flag_upgrade_init();
   cleanStub(&stub_sem);//---------------------------------------------------------
   
  
}

/*
int flag_set_info(upgrade_type_e upgrade_type, uint32_t image_size)
{
    g_flag.upgrade_type = upgrade_type;
    g_flag.image_size = image_size;
    g_flag.upgrade_state = OTA_NEED_UPGRADE;

    return save_flag();
}
*/	
void TestUpgradeFlag::test_flag_set_info()
{
	upgrade_type_e upgrade_type;
	uint32_t image_size;
	flag_set_info(upgrade_type, image_size);
}

/*
void flag_get_info(upgrade_type_e *upgrade_type, uint32_t *image_size, uint32_t *old_image_size, upgrade_state_e *upgrade_state)
{
    if (NULL != upgrade_type)
        *upgrade_type = g_flag.upgrade_type;
    if (NULL != image_size)
        *image_size = g_flag.image_size;
    if (NULL != old_image_size)
        *old_image_size = g_flag.old_image_size;
    if (NULL != upgrade_state)
        *upgrade_state = g_flag.upgrade_state;
}
*/
void TestUpgradeFlag::test_flag_get_info()
{
	upgrade_type_e 	upgrade_type;
	uint32_t 		image_size;
	uint32_t 		old_image_size; 
	upgrade_state_e upgrade_state;
	flag_get_info(&upgrade_type,&image_size,&old_image_size,&upgrade_state);

	upgrade_type_e  *upgrade_type1=NULL;
	uint32_t 		*image_size1=NULL;
	uint32_t 		*old_image_size1=NULL; 
	upgrade_state_e *upgrade_state1=NULL;
	flag_get_info(upgrade_type1,image_size1,old_image_size1,upgrade_state1);
	
}
/*
typedef enum
{
    OTA_IDLE = 0,
    OTA_NEED_UPGRADE,
    OTA_SUCCEED,
    OTA_FAILED,
} upgrade_state_e;

int flag_upgrade_set_result(upgrade_state_e state)
{
    g_flag.upgrade_state = state;

    return save_flag();
}
*/

void TestUpgradeFlag::test_flag_upgrade_set_result()
{
	upgrade_state_e state=OTA_SUCCEED;
	flag_upgrade_set_result(state,1);
}
/*
int flag_upgrade_get_result(upgrade_state_e *state)
{
    if (NULL != state)
        *state = g_flag.upgrade_state;
    g_flag.upgrade_state = OTA_IDLE;
    g_flag.old_image_size = g_flag.image_size;

    return save_flag();
}
*/
void TestUpgradeFlag::test_flag_upgrade_get_result()
{
	upgrade_state_e state=OTA_SUCCEED;
	flag_upgrade_get_result(NULL);
	flag_upgrade_get_result(&state);
}
/*
typedef struct
{
    upgrade_type_e upgrade_type;
    upgrade_state_e upgrade_state;
    uint32_t image_size;
    uint32_t old_image_size;
    uint32_t recover_verify;
    uint32_t verify_length;
    uint32_t crc_flag;
} upgrade_flag_s;

static upgrade_flag_s g_flag;

int flag_set_recover_verify(uint32_t recover_verify, uint32_t verify_length)
{
    g_flag.recover_verify = recover_verify;
    g_flag.verify_length = verify_length;

    return save_flag();
}
*/
void TestUpgradeFlag::test_flag_set_recover_verify()
{
	uint32_t recover_verify;
	uint32_t verify_length;
	flag_set_recover_verify(recover_verify, verify_length);
}
/*
void flag_get_recover_verify(uint32_t *recover_verify, uint32_t *verify_length)
{
    if (NULL != recover_verify)
        *recover_verify = g_flag.recover_verify;
    if (NULL != verify_length)
        *verify_length = g_flag.verify_length;
}

*/
void TestUpgradeFlag::test_flag_get_recover_verify()
{
	uint32_t recover_verify;
	uint32_t verify_length;
	flag_get_recover_verify(NULL,NULL);
	flag_get_recover_verify(&recover_verify, &verify_length);
	
}


void TestUpgradeFlag::test_flag_enable_hwpatch()
{
	
}

TestUpgradeFlag::TestUpgradeFlag()
{
	
    TEST_ADD(TestUpgradeFlag::test_flag_upgrade_init);
	TEST_ADD(TestUpgradeFlag::test_flag_set_info);
	TEST_ADD(TestUpgradeFlag::test_flag_get_info);
	TEST_ADD(TestUpgradeFlag::test_flag_upgrade_set_result);
	TEST_ADD(TestUpgradeFlag::test_flag_upgrade_get_result);
	TEST_ADD(TestUpgradeFlag::test_flag_set_recover_verify);
	TEST_ADD(TestUpgradeFlag::test_flag_get_recover_verify);
	TEST_ADD(TestUpgradeFlag::test_flag_enable_hwpatch);


}

TestUpgradeFlag::~TestUpgradeFlag()
{
}

void TestUpgradeFlag::TestUpgradeFlag::setup()
{
    std::cout << "setup\n";
}

void TestUpgradeFlag::TestUpgradeFlag::tear_down()
{
    std::cout << "test_down\n";
}


