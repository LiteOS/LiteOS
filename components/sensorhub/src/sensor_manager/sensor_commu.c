/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Sensor commu
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
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
 * --------------------------------------------------------------------------- */

#include "sensor_commu.h"
#include "sensor_manager.h"
#include "los_typedef.h"
#include "los_printf.h"
#include "protocol.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

STATIC INT32 g_sensorItemKeep[TAG_END - 1];

LITE_OS_SEC_TEXT_MINOR STATIC INT32 SensorMessageConfigProcess(const PktParameterReq *pkt)
{
    return SensorConfigNotify(pkt, pkt->hd.resp);
}

LITE_OS_SEC_TEXT_MINOR STATIC INT32 SensorMessageSwitchProcess(PktHeader *pkt, SensorItem *item, INT32 *keep)
{
    INT32 ret;
    PktHeaderResp rpkt;

    switch (pkt->cmd) {
        case CMD_CMN_OPEN_REQ:
            *keep = 1; // receive open req
            rpkt.cmd = CMD_CMN_OPEN_RESP;
            rpkt.errNo = EN_OK;
            break;

        case CMD_CMN_CLOSE_REQ:
            *keep = 0; // receive close req
            ret = SensorItemDisable(item, &((PktCmnCloseReq *)pkt)->clsParam, pkt->tranId, pkt->resp);
            if (ret == LOS_OK) {
                return ret;
            }
            rpkt.cmd = CMD_CMN_CLOSE_RESP;
            rpkt.errNo = EN_FAIL;
            break;

        case CMD_CMN_INTERVAL_REQ:
            if (*keep != 0) { // open req has received
                ret = SensorItemEnable(item, &((PktCmnIntervalReq *)pkt)->para, pkt->tranId, pkt->resp);
                if (ret == LOS_OK) {
                    return ret;
                }
            }
            rpkt.cmd = CMD_CMN_INTERVAL_RESP;
            rpkt.errNo = EN_FAIL;
            break;

        default:
            rpkt.cmd = CMD_ERR_RESP;
            rpkt.errNo = EN_FAIL;
            break;
    }

    if (pkt->resp == RESP) {
        rpkt.tag = pkt->tag;
        rpkt.resp = NO_RESP;
        rpkt.tranId = pkt->tranId;
        rpkt.length = CMN_RESP_LENGTH;
        return SensorCommuSend((UINT8 *)&rpkt, sizeof(rpkt));
    }

    if (pkt->cmd == CMD_CMN_OPEN_REQ) {
        return LOS_OK;
    }
    return LOS_NOK;
}

LITE_OS_SEC_TEXT_MINOR INT32 SensorMessageProcess(UINT32 index, const UINT32 *buf, UINT32 len)
{
    SensorType *st = NULL;
    SensorScbTable *scbTable = NULL;

    if ((buf == NULL) || (index >= (TAG_END - 1))) {
        PRINT_ERR("senor message process parameters %u error\n", index);
        return LOS_NOK;
    }

    scbTable = SensorTableGet();
    st = scbTable[index].sensorInterface;

    if ((st == NULL) || (st->tag >= TAG_END) || (st->tag < TAG_BEGIN)) {
        return LOS_NOK;
    }

    if (((PktHeader *)buf)->cmd < CMD_PRIVATE) {
        return SensorMessageSwitchProcess((PktHeader *)buf, &st->commuItem, &g_sensorItemKeep[st->tag - TAG_BEGIN]);
    }

    if (((const PktParameterReq *)buf)->hd.length + sizeof(PktHeader) > len) {
        PRINT_ERR("senor message parameter len %d error\n", len);
        return LOS_NOK;
    }
    return SensorMessageConfigProcess((const PktParameterReq *)buf);
}

WEAK INT32 SensorCommuSend(const UINT8 *buffer, UINT32 len)
{
    (VOID)(len);

    if (buffer == NULL) {
        PRINT_ERR("sensor commu send parameters error\n");
        return LOS_NOK;
    }

    return LOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */