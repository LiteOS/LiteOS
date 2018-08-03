/******************************************************************************

  Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>

 ******************************************************************************
  File Name     : test_at_fota.h
  Version       : Initial Draft
  Author        : MAMINJIE
  Created       : 2018/8/3
  Last Modified :
  Description   : test_at_fota.cpp header file
  Function List :
  History       :
  1.Date        : 2018/8/3
    Author      : MAMINJIE
    Modification: Created file

******************************************************************************/
/* Define to prevent recursive inclusion ------------------------------------*/

#ifndef __TEST_AT_FOTA_H__
#define __TEST_AT_FOTA_H__

/* Includes -----------------------------------------------------------------*/
#include <cpptest.h>
#include "stub.h"
/* Defines ------------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
/* Functions API ------------------------------------------------------------*/
class TestAtFota : public Test::Suite
{
public:
    TestAtFota();
    ~TestAtFota();

    void test_ver_to_hex(void);
    void test_ota_cmd_ioctl(void);
    void test_at_ota_init(void);
    void test_ota_process_main(void);

protected:
    void setup();
    void tear_down();
};


#endif /* __TEST_AT_FOTA_H__ */
