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

#ifndef ti_net_mqtt_server_MQTTClientMgmt__include
#define ti_net_mqtt_server_MQTTClientMgmt__include

#include <ti/net/mqtt/common/mqtt_common.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t MQTTClientMgmt_bmapGet(void *usrCl);

void *MQTTClientMgmt_appHndlGet(void *usrCl);

void *MQTTClientMgmt_willHndlGet(void *usrCl);

bool MQTTClientMgmt_canSessionDelete(void *usrCl);

void MQTTClientMgmt_subCountAdd(void *usrCl);

void MQTTClientMgmt_subCountDel(void *usrCl);

bool MQTTClientMgmt_qos2PubRxUpdate(void *usrCl, uint16_t msgID);

void MQTTClientMgmt_pubDispatch(uint32_t clMap, MQTT_Packet_t *mqp);

int32_t MQTTClientMgmt_pubMsgSend(void *usrCl,
                    const MQTT_UTF8String_t *topic, const uint8_t *dataBuf,
                    uint32_t dataLen, MQTT_QOS qos, bool retain);

void MQTTClientMgmt_onNetClose(void *usrCl);

bool MQTTClientMgmt_notifyAck(void *usrCl, uint8_t msgType, uint16_t msgID);

/* uint16_t composition: MSB is CONNACK-Flags and LSB is CONNACK-RC. The place-holder
   '*usrCl' has valid value, only if CONNACK-RC in return value is 0.
*/
uint16_t MQTTClientMgmt_connectRx(void *ctxCl, bool cleanSession, char *clientID,
                  void *appCl, void *will, void **usrCl);

void MQTTClientMgmt_onConnackSend(void *usrCl, bool cleanSession);

int32_t MQTTClientMgmt_init(void);

#ifdef __cplusplus
}
#endif

#endif
