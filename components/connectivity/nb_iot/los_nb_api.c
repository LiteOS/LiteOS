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

#if defined(WITH_AT_FRAMEWORK) && defined(USE_NB_NEUL95)
#include "los_nb_api.h"
#include "at_api_interface.h"
//#include "atiny_socket.h"
#include "bc95.h"

int los_nb_init(const int8_t* host, const int8_t* port, sec_param_s* psk)
{
    int ret;
    int timecnt = 0;
    if(host == NULL || port == NULL)
        return -1;
    nb_setip((char *)host, (char *)port);
    nb_init();

    if(psk != NULL && 0)//encryption v1.9
    {
        LOS_TaskDelay(2000);
        nb_setpskbuf(psk->psk,psk->pskid);
        if(psk->setpsk)
            at.cmd(NB_SETPSK);
        else
            at.cmd(NB_CLEANPSK);
    }
    at.cmd(NB_REBOOT);
    ret = at.cmd(NB_INIT);
    return ret;
}

int los_nb_report(const char* buf, int len)
{
    if(buf == NULL || len <= 0)
        return -1;
    nb_setsnddata(buf, len);
    return at.cmd(NB_SND);
}

int los_nb_notify(char* featurestr,int cmdlen, oob_callback callback)
{
    if(featurestr == NULL ||cmdlen <= 0 || cmdlen >= OOB_CMD_LEN - 1)
        return -1;
    return at.oob_register(featurestr,cmdlen, callback);
}

int los_nb_deinit(void)
{
    nb_reboot();
	at.deinit();
	return 0;
}

#endif
