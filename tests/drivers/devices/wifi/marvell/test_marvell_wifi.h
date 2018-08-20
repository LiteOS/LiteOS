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
#ifndef __TEST_MARVELL_WIFI_H__
#define __TEST_MARVELL_WIFI_H__

/* Includes -----------------------------------------------------------------*/
#include <cpptest.h>
#include "stub.h"
/* Defines ------------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
/* Functions API ------------------------------------------------------------*/

class TestMarvellWifi : public Test::Suite
{
public:
    TestMarvellWifi();
    ~TestMarvellWifi();

    void test_WiFi_Associate(void);
    void test_WiFi_AssociateEx(void);
    void test_WiFi_CheckTimeout(void);
    void test_WiFi_Deauthenticate(void);
    void test_WiFi_DiscardData(void);
    void test_WiFi_GetBSSID(void);
    void test_WiFi_GetDataLength(void);
    void test_WiFi_GetMACAddress(void);
    void test_WiFi_GetPacketBuffer(void);
    void test_WiFi_GetReceivedPacket(void);
    void test_WiFi_GetSecurityType(void);
    void test_WiFi_Init(void);
    void test_WiFi_Input(void);
    void test_WiFi_IsCommandBusy(void);
    void test_WiFi_JoinADHOC(void);
    void test_WiFi_JoinADHOCEx(void);
    void test_WiFi_KeyMaterial(void);
    void test_WiFi_MACAddr(void);
    void test_WiFi_MACControl(void);
    void test_WiFi_Scan(void);
    void test_WiFi_ScanSSID(void);
    void test_WiFi_SendCommand(void);
    void test_WiFi_SendPacket(void);
    void test_WiFi_SetWEP(void);
    void test_WiFi_SetWPA(void);
    void test_WiFi_ShowCIS(void);
    void test_WiFi_StartADHOC(void);
    void test_WiFi_StartADHOCEx(void);
    void test_WiFi_StopADHOC(void);
    void test_WiFi_TranslateTLV(void);
    void test_WiFi_Wait(void);
    void test_WiFi_WaitForLastTask(void);
    void test_WiFi_EAPOL(void);
    void test_WiFi_DumpData(void);
    void test_WiFi_OutputBuffer(void);

protected:
    void setup();
    void tear_down();
};


#endif /* __TEST_MARVELL_WIFI_H__ */
