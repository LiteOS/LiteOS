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

#ifndef ti_net_mqtt_server_MQTTServerUtil__include
#define ti_net_mqtt_server_MQTTServerUtil__include

#include <ti/net/mqtt/common/mqtt_common.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif


#define MQTT_SERVER_VERSTR "1.0.4"

#define MUTEX_LOCKIN()    MQTTServerUtil_mutexLock()
#define MUTEX_UNLOCK()    MQTTServerUtil_mutexUnlock()

#define USR_INFO(FMT, ...) if(MQTTServerUtil_dbgPrn) MQTTServerUtil_dbgPrn(FMT, ##__VA_ARGS__)

#define DBG_INFO(FMT, ...)                              \
        if(MQTTServerUtil_prnAux && MQTTServerUtil_dbgPrn)                \
                MQTTServerUtil_dbgPrn(FMT, ##__VA_ARGS__)

extern int32_t (*MQTTServerUtil_dbgPrn)(const char *fmt, ...);
extern bool  MQTTServerUtil_prnAux;

void MQTTServerUtil_mutexLock(void);
void MQTTServerUtil_mutexUnlock(void);

uint16_t MQTTServerUtil_setMsgID(void);

void MQTTServerUtil_resetMsgID(void);

MQTT_Packet_t *MQTTServerUtil_mqpAlloc(uint8_t msgType, uint32_t buf_sz);

MQTT_Packet_t *MQTTServerUtil_mqpCopy(const MQTT_Packet_t *mqp);

void MQTTServerUtil_setParams(pthread_mutex_t *mutex,
                     void (*mutexLockin)(pthread_mutex_t *),
                     void (*mutexUnlock)(pthread_mutex_t *));

#ifdef __cplusplus
}
#endif

#endif
