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

/* Define to prevent recursive inclusion ------------------------------------*/

#ifndef TEST_ATINY_FOTA_MANAGER_H_
#define TEST_ATINY_FOTA_MANAGER_H_

/* Includes -----------------------------------------------------------------*/
#include <cpptest.h>
#include "atiny_fota_manager.h"
#include "atiny_fota_state.h"
#include <string.h>
#include "firmware_update.h"
#include "stub.h"
/* Defines ------------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
/* Functions API ------------------------------------------------------------*/

class TestAtinyFotaManager:public Test::Suite{
public:
    TestAtinyFotaManager();
    ~TestAtinyFotaManager();
    
    void test_atiny_fota_manager_start_download();
    void test_atiny_fota_manager_execute_update();
    void test_atiny_fota_manager_finish_download();
    void test_atiny_fota_manager_repot_result();
    void test_atiny_fota_manager_set_state();
    void test_atiny_fota_manager_set_storage_device();
    void test_atiny_fota_manager_get_storage_device();
//    void test_atiny_fota_manager_update_notify();
    void test_atiny_fota_manager_destroy();
    void test_atiny_fota_manager_set_lwm2m_context();
    void test_atiny_fota_manager_get_lwm2m_context();
    void test_atiny_fota_manager_get_instance();
    void test_atiny_fota_manager_get_data_cfg();
    void test_atiny_fota_manager_get_pkg_uri();
    void test_atiny_fota_manager_get_update_result();
    void test_atiny_fota_manager_get_deliver_method();
    void test_atiny_fota_manager_update_notify();
    
    
protected:
    void tear_down();
    void setup();
};



#endif