/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: LiteOS Trace Tlv Implementation
 * Author: Huawei LiteOS Team
 * Create: 2020-03-31
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

#include "trace_tlv.h"
#include "securec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define CRC_WIDTH  8
#define CRC_POLY   0x1021
#define CRC_TOPBIT 0x8000

STATIC UINT16 CalcCrc16(const UINT8 *buf, UINT32 len)
{
    UINT32 i;
    UINT16 crc = 0;

    for (; len > 0; len--) {
        crc = crc ^ (*buf++ << CRC_WIDTH);
        for (i = 0; i < CRC_WIDTH; i++) {
            if (crc & CRC_TOPBIT) {
                crc = (crc << 1) ^ CRC_POLY;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

STATIC UINT32 OsWriteTlv(UINT8 *tlvBuf, UINT8 type, UINT8 len, UINT8 *value)
{
    TraceMsgTlvBody *body = (TraceMsgTlvBody *)tlvBuf;

    if (len == 0) {
        return 0;
    }

    body->type = type;
    body->len = len;
    (VOID)memcpy_s(body->value, len, value, len);
    return len + sizeof(body->type) + sizeof(body->len);
}

STATIC UINT32 OsTlvEncode(const TlvTable *table, UINT8 *srcBuf, UINT8 *tlvBuf, INT32 tlvBufLen)
{
    UINT32 len = 0;
    const TlvTable *tlvTableItem = table;

    while (tlvTableItem->tag != TRACE_TLV_TYPE_NULL) {
        if ((len + tlvTableItem->elemSize + sizeof(UINT8) + sizeof(UINT8)) > tlvBufLen) {
            break;
        }
        len += OsWriteTlv(tlvBuf + len, tlvTableItem->tag, tlvTableItem->elemSize, srcBuf + tlvTableItem->elemOffset);
        tlvTableItem++;
    }
    return len;
}

UINT32 OsTraceDataEncode(UINT8 type, const TlvTable *table, UINT8 *src, UINT8 *dest, INT32 destLen)
{
    UINT16 crc;
    INT32 len;
    INT32 tlvBufLen;
    UINT8 *tlvBuf = NULL;

    TraceMsgTlvHead *head = (TraceMsgTlvHead *)dest;
    tlvBufLen = destLen - sizeof(TraceMsgTlvHead);

    if ((tlvBufLen <= 0) || (table == NULL)) {
        return 0;
    }

    tlvBuf = dest + sizeof(TraceMsgTlvHead);
    len = OsTlvEncode(table, src, tlvBuf, tlvBufLen);
    crc = CalcCrc16(tlvBuf, len);

    head->magicNum = TRACE_TLV_MSG_HEAD;
    head->msgType  = type;
    head->len      = len;
    head->crc      = crc;
    return len + sizeof(TraceMsgTlvHead);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
