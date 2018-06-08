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

/*
 Copyright (c) 2017 Chinamobile

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

     * Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
     * Neither the name of Intel Corporation nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "sys_init.h"
//#include "los_inspect_entry.h"
#include "cmsis_os.h"

VOID HardWare_Init(VOID)
{
    SystemClock_Config();
    //Debug_USART1_UART_Init();  //OneNET
    LOS_EvbSetup();

    IIC_Init();
}


/* Private variables ---------------------------------------------------------*/
#if !CIS_ENABLE_BOOTSTRAP
//183.230.40.40
static const uint8_t config_hex[] = {0x13,0x00,0x33,
    0xf1,0x00,0x03,
    0xf2,0x00,0x21,0x05,0x00/*mtu*/,0x11/*Link&bind type*/,0x00,0x00/*vpn*/,0x00,0x00/*username*/,0x00,0x00/*password*/,
    0x00,0x0d/*host length*/,0x31,0x38,0x33,0x2e,0x32,0x33,0x30,0x2e,0x34,0x30,0x2e,0x34,0x30,0x00,0x04,0x4e,0x55,0x4c,0x4c,
    0xf3,0x00,0x0c,0xea,0x04,0x00,0x00,0x04,0x4e,0x55,0x4c,0x4c};
#endif 

#if CIS_ENABLE_BOOTSTRAP
//183.230.40.39 bootstrap
static const uint8_t config_hex[] = {0x13,0x00,0x33,
    0xf1,0x00,0x03,
    0xf2,0x00,0x21,0x05,0x00/*mtu*/,0x11/*Link&bind type*/,0x00,0x00/*vpn*/,0x00,0x00/*username*/,0x00,0x00/*password*/,
    0x00,0x0d/*host length*/,0x31,0x38,0x33,0x2e,0x32,0x33,0x30,0x2e,0x34,0x30,0x2e,0x33,0x39,0x00,0x04,0x4e,0x55,0x4c,0x4c,
    0xf3,0x00,0x0c,0xea,0x04,0x00,0x00,0x04,0x4e,0x55,0x4c,0x4c};
#endif

void sample_thread(void *arg)
{
extern int cis_sample_entry(void* config_bin,uint32_t config_size);
    cis_sample_entry((void*)config_hex, sizeof(config_hex));

    PRINT_EMG("Never here!\n");
}

void led_thread(void *arg)
{
    int cnt = 0;
    while (1) {

        cnt++;

        LOS_EvbLedControl(LOS_LED3, LED_ON);
        osDelayMS(100);

        LOS_EvbLedControl(LOS_LED3, LED_OFF);
        osDelayMS(100);
    }
}

int main(void)
{
    UINT32 uwRet = LOS_OK;

    HardWare_Init();

    uwRet = LOS_KernelInit();
    if (uwRet != LOS_OK)
    {
        return LOS_NOK;
    }

#if 0
    LOS_Inspect_Entry();
#else
    os_thread_new("sampleTask", sample_thread, NULL, 4096, osPriorityNormal);
    os_thread_new("ledTask", led_thread, NULL, 512, osPriorityBelowNormal);
    printf("NBSDK Starting ...\n");
#endif

    LOS_Start();
}
