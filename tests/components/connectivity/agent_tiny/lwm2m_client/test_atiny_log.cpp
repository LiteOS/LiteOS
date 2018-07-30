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

/* Includes -----------------------------------------------------------------*/
#include "test_atiny_log.h"
/* Defines ------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
extern "C"
{
#include "atiny_log.h"

    extern void atiny_set_log_level(atiny_log_e level);
    extern atiny_log_e atiny_get_log_level(void);
    extern const char *atiny_get_log_level_name(atiny_log_e log_level);
}
/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
TestAtinyLog::TestAtinyLog()
{
    TEST_ADD(TestAtinyLog::test_atiny_set_log_level);
    TEST_ADD(TestAtinyLog::test_atiny_get_log_level);
    TEST_ADD(TestAtinyLog::test_atiny_get_log_level_name);
}
TestAtinyLog::~TestAtinyLog()
{

}

void TestAtinyLog::test_atiny_set_log_level(void)
{
    // like test_atiny_get_log_level
}
void TestAtinyLog::test_atiny_get_log_level(void)
{
    atiny_log_e logLevel = LOG_INFO;
    atiny_log_e ori_logLevel;
    int ret = 0;

    ori_logLevel = logLevel = atiny_get_log_level();
    TEST_ASSERT_MSG((LOG_INFO == logLevel), "atiny_get_log_level() failed");

    logLevel = LOG_DEBUG;
    atiny_set_log_level(logLevel);
    TEST_ASSERT_MSG((0 == ret), "atiny_set_log_level(...) failed");

    logLevel = atiny_get_log_level();
    TEST_ASSERT_MSG((LOG_DEBUG == logLevel), "atiny_get_log_level() failed");

    atiny_set_log_level(ori_logLevel);
    TEST_ASSERT_MSG((0 == ret), "atiny_set_log_level(...) failed");
}

void TestAtinyLog::test_atiny_get_log_level_name(void)
{
    atiny_log_e logLevel = LOG_INFO;
    char *name = NULL;
    int ret = 0;

    name = (char *)atiny_get_log_level_name(logLevel);
    ret = strcmp((const char *)name, "INFO");
    TEST_ASSERT_MSG((0 == ret), "atiny_get_log_level_name(...) failed");

    logLevel = LOG_MAX;
    name = (char *)atiny_get_log_level_name(logLevel);
    ret = strcmp((const char *)name, "UNKOWN");
    TEST_ASSERT_MSG((0 == ret), "atiny_get_log_level_name(...) failed");
}

/* Private functions --------------------------------------------------------*/

