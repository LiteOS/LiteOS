/******************************************************************************

  Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>

 ******************************************************************************
  File Name     : test_firmware_update.h
  Version       : Initial Draft
  Author        : MAMINJIE
  Created       : 2018/7/31
  Last Modified :
  Description   : test_firmware_update.cpp header file
  Function List :
  History       :
  1.Date        : 2018/7/31
    Author      : MAMINJIE
    Modification: Created file

******************************************************************************/
/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __TEST_FIRMWARE_UPDATE_H__
#define __TEST_FIRMWARE_UPDATE_H__

/* Includes -----------------------------------------------------------------*/
#include <cpptest.h>
#include "stub.h"
/* Defines ------------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
/* Functions API ------------------------------------------------------------*/
class TestFirmwareUpdate : public Test::Suite
{
public:
    TestFirmwareUpdate();

    void test_set_firmware_update_notify(void);
    void test_start_firmware_download(void);
    void test_parse_firmware_uri(void);

protected:
    void setup();
    void tear_down();
};


#endif /* __TEST_FIRMWARE_UPDATE_H__ */
