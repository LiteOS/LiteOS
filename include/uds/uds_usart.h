/*----------------------------------------------------------------------------
 * Copyright (c) <2013-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
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
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

#ifndef _UDS_USART_H
#define _UDS_USART_H

#include <stdint.h>
#include "osdepends/osport.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
    uint32_t port;          // the USART port
    uint32_t baudrate;      // the USART communication baud rate
    uint32_t word_length;   // the number of data bits transmitted or received in a frame
    uint32_t stop_bits;     // the number of stop bits transmitted
    uint32_t parity;        // the parity mode
    uint32_t mode;          // whether the Receive or Transmit mode is enabled or disabled
    uint32_t flow_ctrl;     // whether the hardware flow control mode is enabled or disabled
    uint32_t over_sampling; // whether the Over sampling 8 is enabled or disabled
} uds_usart_cfg;

/**
  * @brief  the recv callback for interrupt mode
  * @param  port: the USART port
  * @param  buf: Pointer to data buffer
  * @param  len: Amount of data to be received
  * @retval none
  */
typedef void (*uds_usart_recv_callback)(uint32_t port, uint8_t *buf, uint32_t len);

typedef struct
{
    uds_usart_recv_callback cb;
    int32_t break_type;
    int32_t break_cond;
} uds_usart_cb_cfg;

typedef enum
{
    USART_SET_RECV_CALLBACK,    // Set the recv callback for interrupt mode
    USART_CLR_RECV_CALLBACK     // Clear the recv callback
} uds_usart_cmd;

s32_t uds_usart_dev_install(const char *name, void *para);


#ifdef __cplusplus
}
#endif
#endif /* _UDS_USART_H */

