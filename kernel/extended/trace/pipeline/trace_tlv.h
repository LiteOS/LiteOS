/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: LiteOS Trace Tlv Implementation HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2020-03-16
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

#ifndef _TRACE_TLV_H
#define _TRACE_TLV_H

#include "los_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define TRACE_TLV_MSG_HEAD  0xFF
#define TRACE_TLV_TYPE_NULL 0xFF

typedef struct {
    UINT8 magicNum;
    UINT8 msgType;
    UINT16 len;
    UINT16 crc;
} TraceMsgTlvHead;

typedef struct {
    UINT8 type;
    UINT8 len;
    UINT8 value[];
} TraceMsgTlvBody;

typedef struct {
    UINT8 tag;
    UINT8 elemOffset;
    UINT8 elemSize;
} TlvTable;

/**
 * @ingroup los_trace
 * @brief Encode trace raw data.
 *
 * @par Description:
 * This API is used to encode trace raw data to tlv data.
 * @attention
 * <ul>
 * <li>Encade trace data</li>
 * </ul>
 *
 * @param  type     [IN] Type #UINT8. The type stands for different struct of src data.
 * @param  src      [IN] Type #UINT8 *. The raw trace data.
 * @param  table    [IN] Type #const TlvTable *. The tlv table descript elemOffset and elemSize.
 * @param  dest     [OUT] Type #UINT8 *. The tlv data.
 * @param  destLen  [IN] Type #UINT8 *. The tlv buf max len.

 * @retval #0                                  convert failed.
 * @retval #UINT32                             convert success bytes.
 *
 * @par Dependency:
 * <ul><li>trace_tlv.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_TraceDataEncode
 * @since Huawei LiteOS V200R005C00
 */
extern UINT32 OsTraceDataEncode(UINT8 type, const TlvTable *table, UINT8 *src, UINT8 *dest, INT32 destLen);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _TRACE_TLV_H */