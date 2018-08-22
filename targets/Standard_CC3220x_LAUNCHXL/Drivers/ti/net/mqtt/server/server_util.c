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

#include "server_util.h"

/*----------------------------------------------------------------------------
 * Try to prevent fragmentation by consistently allocating a fixed memory size
 *----------------------------------------------------------------------------
 */
#ifndef CFG_SR_MAX_MQP_TX_LEN
#define MQP_SERVER_TX_LEN        1024
#else
#define MQP_SERVER_TX_LEN        CFG_SR_MAX_MQP_TX_LEN
#endif

static uint16_t MQTTServerUtil_msgID = 0xFFFF;

int32_t (*MQTTServerUtil_dbgPrn)(const char *fmt, ...) = NULL;
bool MQTTServerUtil_prnAux = false;

static void (*MQTTServerUtil_registerMutexLock)(pthread_mutex_t *)   = NULL;
static void (*MQTTServerUtil_registerMutexUnlock)(pthread_mutex_t *) = NULL;
static pthread_mutex_t *MQTTServerUtil_mutex = NULL;

//*****************************************************************************
//
//! \brief Increment the message ID
//
//*****************************************************************************
static inline uint16_t assign_new_msg_id()
{
    return MQTTServerUtil_msgID += 2;
}

static void my_pkt_free(MQTT_Packet_t *mqp)
{
    free((void *)mqp);
}

//*****************************************************************************
//
//! \brief The server_mqp_alloc function allocates memory for the message for
//! each message received by the server.
//
//*****************************************************************************
static MQTT_Packet_t *server_mqp_alloc(uint8_t msgType, uint32_t buf_sz, uint8_t offset)
{
    uint32_t mqp_sz = sizeof(MQTT_Packet_t);
    MQTT_Packet_t *mqp = NULL;

    buf_sz += offset;

    if ((mqp_sz + buf_sz) > MQP_SERVER_TX_LEN)
    {
        /* fatal, buf allocate len > MQP_SERVER_TX_LEN */
        return NULL;
    }

    /* The size of the message is the size of the MQTT packet + payload (data) */
    mqp = malloc(mqp_sz + buf_sz);
    if (NULL != mqp)
    {
        MQTT_packetInit(mqp, offset);

        mqp->msgType = msgType;
        mqp->maxlen  = buf_sz;
        mqp->buffer  = (uint8_t *)mqp + mqp_sz;

        mqp->free = my_pkt_free;

    }
    /* else - fatal, failed to allocate Server MQP */

    return mqp;
}

//*****************************************************************************
//
//! \brief Reset the Message ID counter
//
//*****************************************************************************
void MQTTServerUtil_resetMsgID(void)
{
    MQTTServerUtil_msgID = 0xFFFF;
}

//*****************************************************************************
//
//! \brief Set new message ID
//
//*****************************************************************************
uint16_t MQTTServerUtil_setMsgID(void)
{
    return assign_new_msg_id();
}

MQTT_Packet_t *MQTTServerUtil_mqpAlloc(uint8_t msgType, uint32_t buf_sz)
{
    return server_mqp_alloc(msgType, buf_sz, MQTT_MAX_FH_LEN);
}

MQTT_Packet_t *MQTTServerUtil_mqpCopy(const MQTT_Packet_t *mqp)
{
    MQTT_Packet_t *cpy = server_mqp_alloc(mqp->msgType, mqp->maxlen, 0);
    uint8_t *buffer;

    if (NULL != cpy)
    {
        buffer = cpy->buffer;

        /* Copy to overwrite everything in 'cpy' from source 'mqp' */
        MQTT_bufWrNbytes((uint8_t *)cpy, (uint8_t *)mqp, sizeof(MQTT_Packet_t));

        cpy->buffer = buffer; /* Restore buffer and copy */
        MQTT_bufWrNbytes(cpy->buffer, mqp->buffer, mqp->maxlen);
    }

    return cpy;
}

void MQTTServerUtil_mutexLock(void)
{
    if (MQTTServerUtil_registerMutexLock)
    {
        MQTTServerUtil_registerMutexLock(MQTTServerUtil_mutex);
    }
    return;
}

void MQTTServerUtil_mutexUnlock(void)
{
    if (MQTTServerUtil_registerMutexUnlock)
    {
        MQTTServerUtil_registerMutexUnlock(MQTTServerUtil_mutex);
    }
    return;
}

void MQTTServerUtil_setParams(pthread_mutex_t *mutex, void (*mutexLockin)(pthread_mutex_t *), void (*mutexUnlock)(pthread_mutex_t *))
{
    MQTTServerUtil_mutex = mutex;
    MQTTServerUtil_registerMutexLock = mutexLockin;
    MQTTServerUtil_registerMutexUnlock = mutexUnlock;

    return;
}
