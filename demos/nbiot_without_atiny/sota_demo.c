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

#ifdef WITH_SOTA
#include <stdio.h>
#include "osdepends/atiny_osdep.h"
#include "sota/sota.h"
#include "../../components/ota/sota/sota_hal.h"
#include "ota_port.h"
#include "board.h"
#include "hal_spi_flash.h"
#include "at_frame/at_main.h"

extern int nb_send_str(const char* buf, int len);
#define DEVICE_VER "V0.0"
int read_ver(char* buf, uint32_t len)
{
    memcpy(buf,DEVICE_VER,strlen(DEVICE_VER));
    return 0;
}

int notify_new_ver(char* buf, uint32_t len)
{
    return 0;
}
int set_ver(const char* buf, uint32_t len)
{
    return 0;
}

int32_t sota_cmd_match(const char *buf, char* featurestr,int len)
{
    
    if(strstr(buf,featurestr) != NULL)
        return 0;
    else
        return -1;
}

int32_t sota_callback(void *arg, int8_t* buf, int32_t buflen)
{
    int ret;
    ret = sota_process_main(arg, buf, buflen);
    if (ret == SOTA_NEEDREBOOT)
    {
        atiny_reboot();
    }
    return 0;
}

void nb_sota_demo(void)
{
    sota_opt_t flash_op =
    {
    .get_ver = read_ver,
    .set_ver = set_ver,
    .sota_send = nb_send_str,
    .sota_malloc = at_malloc,
    .sota_free = at_free,
    };
    hal_get_ota_opt(&flash_op.ota_info);
    flash_op.ota_info.key.rsa_N = "C94BECB7BCBFF459B9A71F12C3CC0603B11F0D3A366A226FD3E73D453F96EFBBCD4DFED6D9F77FD78C3AB1805E1BD3858131ACB5303F61AF524F43971B4D429CB847905E68935C1748D0096C1A09DD539CE74857F9FDF0B0EA61574C5D76BD9A67681AC6A9DB1BB22F17120B1DBF3E32633DCE34F5446F52DD7335671AC3A1F21DC557FA4CE9A4E0E3E99FED33A0BAA1C6F6EE53EDD742284D6582B51E4BF019787B8C33C2F2A095BEED11D6FE68611BD00825AF97DB985C62C3AE0DC69BD7D0118E6D620B52AFD514AD5BFA8BAB998332213D7DBF5C98DC86CB8D4F98A416802B892B8D6BEE5D55B7E688334B281E4BEDDB11BD7B374355C5919BA5A9A1C91F";
    flash_op.ota_info.key.rsa_E = "10001";
    flash_op.frame_buf_len = 1224;
    flash_op.run_mode = APP_MODE;
    hal_init_ota();

    sota_init(&flash_op);
    (void)at.oob_register("+NNMI:", strlen("+NNMI:"), sota_callback,sota_cmd_match);
}



#endif

 

