/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: LiteOS Trace Pipeline of Serial Implementation
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

#include "trace_pipeline_serial.h"
#include "trace_pipeline.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_TRACE_CONTROL_AGENT
UINT32 SerialPipelineInit(VOID)
{
    return uart_hwiCreate();
}

UINT32 SerialDataReceive(UINT8 *data, UINT32 size, UINT32 timeout)
{
    return uart_read(data, size, timeout);
}

UINT32 SerialWait(VOID)
{
    return uart_wait_adapt();
}

#else

UINT32 SerialPipelineInit(VOID)
{
    return LOS_OK;
}

UINT32 SerialDataReceive(UINT8 *data, UINT32 size, UINT32 timeout)
{
    return LOS_OK;
}

UINT32 SerialWait(VOID)
{
    return LOS_OK;
}
#endif

VOID SerialDataSend(UINT16 len, UINT8 *data)
{
    UINT32 i;

    for (i = 0; i < len; i++) {
        UART_PUTC(data[i]);
    }
}

STATIC const TracePipelineOps g_serialOps = {
    .init = SerialPipelineInit,
    .dataSend = SerialDataSend,
    .dataRecv = SerialDataReceive,
    .wait = SerialWait,
};

UINT32 OsTracePipelineInit(VOID)
{
    OsTracePipelineReg(&g_serialOps);
    return g_serialOps.init();
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
