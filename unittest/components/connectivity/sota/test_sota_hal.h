/******************************************************************************

  Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>

 ******************************************************************************
  File Name     : test_at_fota_hal.h
  Version       : Initial Draft
  Author        : MAMINJIE
  Created       : 2018/8/3
  Last Modified :
  Description   : test_at_fota_hal.cpp header file
  Function List :
  History       :
  1.Date        : 2018/8/3
    Author      : MAMINJIE
    Modification: Created file

******************************************************************************/
/* Define to prevent recursive inclusion ------------------------------------*/

#ifndef __TEST_SOTA_HAL_H__
#define __TEST_SOTA_HAL_H__

/* Includes -----------------------------------------------------------------*/
#include <cpptest.h>
#include "stub.h"
/* Defines ------------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
/* Functions API ------------------------------------------------------------*/
class TestSotaHal : public Test::Suite
{
public:
    TestSotaHal();
    ~TestSotaHal();

    void test_at_fota_write(void);
    void test_crc_check(void);
    void test_sota_str_to_hex(void);
    void test_HexStrToByte(void);
    void test_do_crc(void);

protected:
    void setup();
    void tear_down();
};



#endif /* __TEST_SOTA_HAL_H__ */
