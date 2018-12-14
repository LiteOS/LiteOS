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

#ifndef _DAL_USART_H
#define _DAL_USART_H

#include <stdint.h>
#include "uds/uds_usart.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
  * @brief  Initializes the USART mode according to the specified parameters
  * @param  cfg: the USART port configuration
  * @retval 0 on success, or -1 on error
  */
int32_t dal_usart_init(uds_usart_cfg *cfg);

/**
  * @brief  DeInitializes the USART peripheral.
  * @param  port: the USART port
  * @retval none
  */
int32_t dal_usart_deinit(uint32_t port);

/**
  * @brief  Sends an amount of data in non blocking mode.
  * @param  port: the USART port
  * @param  buf: Pointer to data buffer
  * @param  len: Amount of data to be sent
  * @param  timeout: Timeout duration, (uint32_t)-1 for WAIT FOREVER
  * @retval 0 on success, or -1 on error
  */
int32_t dal_usart_send(uint32_t port, uint8_t *buf, uint32_t len, uint32_t timeout);

/**
  * @brief  Receives an amount of data in blocking mode.
  * @param  port: the USART port
  * @param  buf: Pointer to data buffer
  * @param  len: Amount of data to be received
  * @param  timeout: Timeout duration, (uint32_t)-1 for WAIT FOREVER
  * @retval 0 on the specified number of bytes received, or -1 on timeout/error
  */
int32_t dal_usart_recv(uint32_t port, uint8_t *buf, uint32_t len, uint32_t timeout);

/**
  * @brief  Set recv callback.
  * @param  port: the USART port
  * @param  cb: Callback for receiving data, NULL to disable it
  * @param  break_type: Break type
  * @param  break_cond:
  *             specify the number of bytes to report when break_type is 1
  *             specify the terminator char when break_type is 2
  * @retval 0 on success, or -1 on error
  */
int32_t dal_set_usart_recv_callback(uint32_t port, uds_usart_recv_callback cb,
                                                int32_t break_type, int32_t break_cond);


#ifdef __cplusplus
}
#endif
#endif /* _DAL_USART_H */

