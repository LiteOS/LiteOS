/*
 * Copyright (C) 2016-2018, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "server_plug.h"
#include "server_core.h"
#include "server_util.h"

#define MAX_PLUGINS            PG_MAP_MAX_ELEMS
#define PG_NAME_LEN            32

typedef struct _MQTTServerPlug_desc_t_
{

    char *name;
    uint8_t index;
    uint8_t inuse;

    MQTTServerCore_AppCBs_t app_cbs;

} MQTTServerPlug_desc_t;

static MQTTServerPlug_desc_t MQTTServerPlug_plugins[MAX_PLUGINS];
static MQTTServerPlug_desc_t *MQTTServerPlug_aclPg = NULL;

static MQTTServerPlug_CBs_t MQTTServerPlug_msgCBacks = { NULL, NULL, NULL };
static MQTTServerPlug_CBs_t *MQTTServerPlug_msgCBs;

static inline bool is_inuse(MQTTServerPlug_desc_t *plugin)
{
    return (plugin->inuse ? true : false);
}

static inline void inuse_set(MQTTServerPlug_desc_t *plugin, bool inuse)
{
    plugin->inuse = inuse ? 0x01 : 0x00;
}

static inline MQTTServerPlug_desc_t *plugin_find(int32_t idx)
{
    return (MQTTServerPlug_plugins + idx);
}

static void plugin_reset(MQTTServerPlug_desc_t *plugin)
{
    inuse_set(plugin, false);

    return;
}

static MQTTServerPlug_desc_t *plugin_alloc(void)
{
    MQTTServerPlug_desc_t *plugin = NULL;
    int32_t idx = 0;

    for (idx = 0; idx < MAX_PLUGINS; idx++)
    {
        plugin = MQTTServerPlug_plugins + idx;
        if (false == is_inuse(plugin))
        {
            inuse_set(plugin, true);
            break;
        }
    }
    return ((MAX_PLUGINS != idx) ? plugin : NULL);
}

static void *server_app_register(const MQTTServerCore_AppCBs_t *cbs, const char *name)
{
    MQTTServerPlug_desc_t *plugin = plugin_alloc();

    if (NULL != plugin)
    {
        plugin->name = (char *)name;
        memcpy(&plugin->app_cbs, cbs, sizeof(MQTTServerCore_AppCBs_t));

        if ((NULL == MQTTServerPlug_aclPg) && cbs->connect)
        {
            MQTTServerPlug_aclPg = plugin;
        }
    }
    return plugin;
}


uint16_t MQTTServerPlug_connect(const MQTT_UTF8String_t *clientId, const MQTT_UTF8String_t *username,
                        const MQTT_UTF8String_t *password, void **appUsr)
{
    uint16_t rv = MQTT_CONNACK_RC_REQ_ACCEPT; /* Accept everything from MQTT network */

    *appUsr = NULL;
    if (MQTTServerPlug_aclPg)
    {
        rv = MQTTServerPlug_aclPg->app_cbs.connect(clientId, username, password, appUsr);
    }
    return rv;
}

int32_t MQTTServerPlug_publish( uint8_t pg_map, const MQTT_UTF8String_t *topic, const uint8_t *payload,
                        uint32_t payLen, bool dup, uint8_t qos, bool retain)
{
    int32_t i = 0;

    for (i = 0; i < MAX_PLUGINS; i++)
    {
        if (PG_MAP_HAS_VALUE(pg_map, i))
        {
            MQTTServerPlug_desc_t *plugin = plugin_find(i);

            if (false == is_inuse(plugin))
            {
                continue; /* Must not happen */
            }
            plugin->app_cbs.publish(topic, payload, payLen, dup, qos, retain);
        }
    }

    return payLen;
}

int32_t MQTTServerPlug_disconn(const void *appUsr, bool due2err)
{
    if (MQTTServerPlug_aclPg)
    {
        MQTTServerPlug_aclPg->app_cbs.disconn(appUsr, due2err);
    }
    return 0;
}

int32_t MQTTServerCore_topicEnroll(const void *app_hnd, const MQTT_UTF8String_t *topic, MQTT_QOS qos)
{
    return (app_hnd ? MQTTServerPlug_msgCBs->topicEnroll(((MQTTServerPlug_desc_t *)(app_hnd))->index, topic, qos) : -1);
}

int32_t MQTTServerCore_topicDisenroll(const void *app_hnd, const MQTT_UTF8String_t *topic)
{
    return (app_hnd ? MQTTServerPlug_msgCBs->topicCancel(((MQTTServerPlug_desc_t *)(app_hnd))->index, topic) : -1);
}

int32_t MQTTServerCore_pubSend(const MQTT_UTF8String_t *topic, const uint8_t *dataBuf, uint32_t dataLen, MQTT_QOS qos, bool retain)
{
    return (MQTTServerPlug_msgCBs->publish(topic, dataBuf, dataLen, qos, retain));
}

void *MQTTServerCore_appRegister(const MQTTServerCore_AppCBs_t *cbs, const char *name)
{
    if ((NULL == cbs) || ((!!cbs->connect) ^ (!!cbs->disconn)) || (MQTTServerPlug_aclPg && cbs->connect))
    {
        return NULL;
    }
    return server_app_register(cbs, name);
}

int32_t MQTTServerPlug_init(const MQTTServerPlug_CBs_t *cbs)
{
    int32_t idx = 0;
    MQTTServerPlug_desc_t *plugin;

    MQTTServerPlug_aclPg = NULL;

    if (NULL == cbs)
    {
        return -2;
    }
    if (!(cbs->topicEnroll && cbs->topicCancel && cbs->publish))
    {
        return -3;
    }
    for (idx = 0; idx < MAX_PLUGINS; idx++)
    {
        plugin = MQTTServerPlug_plugins + idx;
        plugin->index = idx;

        plugin_reset(plugin);
    }

    MQTTServerPlug_msgCBs = &MQTTServerPlug_msgCBacks;
    memcpy(MQTTServerPlug_msgCBs, cbs, sizeof(MQTTServerPlug_CBs_t));

    /* Plugin module has been initialized */
    return 0;
}
