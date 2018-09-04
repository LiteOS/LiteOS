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

#ifndef ti_net_mqtt_server_MQTTServerPlug__include
#define ti_net_mqtt_server_MQTTServerPlug__include

#include "server_pkts.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Used by Server Core Logic */
#define PG_MAP_BITS_SIZE     2
#define PG_MAP_BITS_MASK     ((1 << PG_MAP_BITS_SIZE) - 1)
#define PG_MAP_MAX_ELEMS     4  /* should be able accomodate in 1 byte */
#define PG_MAP_ALL_DFLTS     ((1 << (PG_MAP_BITS_SIZE * PG_MAP_MAX_ELEMS)) - 1)

#define PG_MAP_HAS_VALUE(pg_map, index)                 \
        (((~pg_map) >> (index * PG_MAP_BITS_SIZE)) & PG_MAP_BITS_MASK)

#define PG_MAP_VAL_SETUP(pg_map, qid, index)                            \
        {                                                               \
                uint32_t ofst = index * PG_MAP_BITS_SIZE;                    \
                pg_map  &= ~(PG_MAP_BITS_MASK << ofst);                 \
                pg_map  |= qid << ofst;                                 \
        }

#define PG_MAP_VAL_RESET(pg_map, index)                                 \
        pg_map |= PG_MAP_BITS_MASK << (index * PG_MAP_BITS_SIZE);

#if (PG_MAP_BITS_MASK != QID_VMASK)
#error "PG_MAP_BITS_MASK must be same as 2bit QOS_VMASK"
#endif

#if ((PG_MAP_MAX_ELEMS * PG_MAP_BITS_SIZE) > 8)
#error "Make size-of pg_map greate than 1 byte"
#endif

typedef struct _MQTTServerPlug_CBs_t_
{
        int32_t (*topicEnroll)(uint8_t pgID, const MQTT_UTF8String_t *topic,
                            MQTT_QOS qos);
        int32_t (*topicCancel)(uint8_t pgID, const MQTT_UTF8String_t *topic);
        int32_t (*publish)(const MQTT_UTF8String_t *topic,    const uint8_t *dataBuf,
                       uint32_t dataLen, MQTT_QOS qos, bool retain);
}MQTTServerPlug_CBs_t;

int32_t MQTTServerPlug_init(const MQTTServerPlug_CBs_t *cbs);

/* uint16_t composition: MSB is CONNACK-Flags = 0, LSB is CONNACK-RC */
uint16_t MQTTServerPlug_connect(const MQTT_UTF8String_t *clientId,
                   const MQTT_UTF8String_t *username,
                   const MQTT_UTF8String_t *password,
                   void **appUsr);

int32_t MQTTServerPlug_publish(uint8_t pg_map, const MQTT_UTF8String_t *topic,
                   const uint8_t *payload, uint32_t payLen,
                   bool dup, uint8_t qos, bool retain);

int32_t MQTTServerPlug_disconn(const void *appUsr, bool due2err);

#ifdef __cplusplus
}
#endif

#endif
