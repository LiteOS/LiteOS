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

#ifndef _CIS_INTERFACE_NET_H_
#define _CIS_INTERFACE_NET_H_

#include "cis_config.h"
#include "cis_def.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    cisnet_event_unknow = 0,
    cisnet_event_connected,
    cisnet_event_disconnect,
	cisnet_event_timeout
}cisnet_event_t;

typedef enum
{
    cisnet_linktype_nonip = 0,
    cisnet_linktype_udp,
    cisnet_linktype_tcp,
}cisnet_linktype_t;

typedef enum
{
    cisnet_bandtype_nb = 0,
    cisnet_bandtype_gsm,
    cisnet_bandtype_lte,
    cisnet_bandtype_emtc,
}cisnet_bandtype_t;


typedef struct st_cisnet_context* cisnet_t;
typedef cis_ret_t (*cisnet_event_callback_t)(cisnet_t netctx,cisnet_event_t id,void* param,void* userData);

typedef struct st_cisnet_callback
{
    void* userData;
    cisnet_event_callback_t onEvent;
}cisnet_callback_t;


typedef struct st_cisnet_config
{
    uint16_t           mtu;
    cisnet_linktype_t  linktype;
    cisnet_bandtype_t  bandtype;

    uint8_t*      apn;
    uint16_t      apnlen;

    uint8_t*      username;
    uint16_t      usernamelen;

    uint8_t*      password;
    uint16_t      passwordlen;

    uint8_t*      data;
    uint16_t      datalen;

}cisnet_config_t;


cis_ret_t cisnet_init        (const cisnet_config_t* config,cisnet_callback_t cb);
cis_ret_t cisnet_create      (cisnet_t* ctx,const char* host);
void      cisnet_destroy     (cisnet_t ctx);
cis_ret_t cisnet_connect     (cisnet_t ctx);
cis_ret_t cisnet_disconnect  (cisnet_t ctx);
cis_ret_t cisnet_write       (cisnet_t ctx,const uint8_t* buffer,uint32_t length);
cis_ret_t cisnet_read        (cisnet_t ctx,uint8_t* buffer,uint32_t *length);
cis_ret_t cisnet_free        (cisnet_t ctx,uint8_t* buffer,uint32_t length);
uint8_t      cisnet_attached_state(void);

#ifdef __cplusplus
};
#endif

#endif//_CIS_INTERFACE_NET_H_
