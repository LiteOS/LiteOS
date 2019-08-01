/*----------------------------------------------------------------------------
 * Copyright (c) <2018>, <Huawei Technologies Co., Ltd>
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
/**
 *  DATE                AUTHOR      INSTRUCTION
 *  2019-05-21 16:25  zhangqianfu  The first version  
 *
 */

#include <osal.h>
#include <string.h>
#include <oc_lwm2m_al.h>
#include <agent_lwm2m.h>

typedef struct
{
    oc_config_param_t para;
}oc_lwm2m_impdemo_t;

static oc_lwm2m_impdemo_t  s_oc_lwm2m_impdemo;

static void *__demo_config(oc_config_param_t *param)
{
    void *ret;

    s_oc_lwm2m_impdemo.para = *param;

    ret = &s_oc_lwm2m_impdemo;

    return ret;
}

static int __demo_deconfig(void *handle)
{
    memset(&s_oc_lwm2m_impdemo,0,sizeof(s_oc_lwm2m_impdemo));
    return 0;
}

static int __demo_report(void *handle, char *msg, int len, int timeout)
{
    printf("%s:report:len:%d  timeout:%d\n\r",__FUNCTION__,len,timeout);

    return 0;
}

const oc_lwm2m_opt_t  g_oc_demo_opt = \
{
    .config = __demo_config,
    .deconfig = __demo_deconfig,
    .report = __demo_report,
};

int oc_lwm2m_install_impdemo()
{
    int ret = -1;

    ret = oc_lwm2m_register("impdemo",&g_oc_demo_opt);

    return ret;
}
