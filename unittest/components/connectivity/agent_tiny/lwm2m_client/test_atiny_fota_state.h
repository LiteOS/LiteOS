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
#ifndef _TEST_ATINY_FOTA_STATE_H
#define _TEST_ATINY_FOTA_STATE_H
/* Includes -----------------------------------------------------------------*/
#include <cpptest.h>
#include "stub.h"


/* Defines ------------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/

/* Functions API ------------------------------------------------------------*/


class TestAtinyFotaState:public Test::Suite {
 protected:
  void tear_down();
  void setup();


 public:
    void test_atiny_fota_state_default_handle();
    void test_atiny_fota_state_init();
    void test_atiny_fota_start_download();
    void test_atiny_fota_idle_state_report_result();
    void test_atiny_fota_manager_update_notify();
    void test_atiny_fota_downloading_state_finish_download();
    void test_atiny_fota_downloading_state_init();
    void test_atiny_fota_downloaded_state_init();
    void test_atiny_fota_updating_state_init();
    void test_atiny_fota_downloaded_state_execute_update();
    void test_atiny_fota_idle_state_init();
    void test_atiny_fota_idle_state_recv_notify_ack();
    void test_atiny_fota_idle_state_int_report_result();
    void atiny_fota_downloading_state_recv_notify_ack();
    void test_atiny_fota_downloaded_state_recv_notify_ack();

  TestAtinyFotaState();
};




#endif

