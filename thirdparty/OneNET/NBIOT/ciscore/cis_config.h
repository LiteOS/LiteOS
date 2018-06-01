/*******************************************************************************
 *
 * Copyright (c) 2017 China Mobile and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Bai Jie & Long Rong, China Mobile - initial API and implementation
 *
 *******************************************************************************/

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


 Bai Jie <baijie@chinamobile.com>
 Long Rong <longrong@chinamobile.com>

*/
/************************************************************************/
/* nb-iot middle software of china mobile api                           */
/************************************************************************/
#ifndef _CIS_CONFIG_H_
#define _CIS_CONFIG_H_

#include "cis_def.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef enum enum_cis_cfgid
{
    cis_cfgid_init   = 0x01,
    cis_cfgid_net    = 0x02,
    cis_cfgid_sys    = 0x03
}cis_cfgid_t;

typedef struct st_cis_cfg_init  cis_cfg_init_t;
typedef struct st_cis_cfg_net   cis_cfg_net_t;
typedef struct st_cis_cfg_sys   cis_cfg_sys_t;
typedef struct st_cis_cfg_vdata cis_cfgdata_t;
typedef struct st_cis_cfg_ret   cis_cfgret_t;

struct st_cis_cfg_vdata
{
    uint16_t len;
    uint8_t* data;
};


struct st_cis_cfg_init
{
    uint8_t conf;
};


struct st_cis_cfg_net
{
    uint16_t      mtu;
    uint8_t       linktype;
    uint8_t       bandtype;
    uint8_t       bs_enabled;

    cis_cfgdata_t apn;
    cis_cfgdata_t username;
    cis_cfgdata_t password;
    cis_cfgdata_t host;
    cis_cfgdata_t user_data;
};


struct st_cis_cfg_sys
{
    bool log_enabled;
    uint8_t log_ext_output;
    uint8_t log_output_type;
    uint8_t log_output_level;
    uint16_t log_buffer_size;
    cis_cfgdata_t user_data;
};


struct st_cis_cfg_ret
{
    union
    {
       cis_cfg_init_t* cfg_init;
       cis_cfg_net_t* cfg_net;
       cis_cfg_sys_t* cfg_sys;
    }data;
};


cis_ret_t cis_config_init(void* config,uint16_t config_len);
cis_ret_t cis_config_get(cis_cfgid_t cfgid,cis_cfgret_t* ret);

#ifdef __cplusplus
};
#endif


#endif //_CIS_CONFIG_H_



