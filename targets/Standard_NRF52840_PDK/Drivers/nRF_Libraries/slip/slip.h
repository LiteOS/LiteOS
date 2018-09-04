/**
 * Copyright (c) 2015 - 2017, Nordic Semiconductor ASA
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 * 
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 * 
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 * 
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 * 
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#ifndef SLIP_H__
#define SLIP_H__

#include <stdint.h>
#include "app_fifo.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @file
 *
 * @defgroup slip SLIP encoding decoding
 * @{
 * @ingroup app_common
 *
 * @brief  This module encodes and decodes slip packages (RFC1055).
 *
 * @details The standard is described in https://tools.ietf.org/html/rfc1055
 */

typedef enum {
    SLIP_DECODING,
    SLIP_END_RECEIVED,
    SLIP_ESC_RECEIVED,
    SLIP_CLEARING_INVALID_PACKET,
} slip_state_t;

typedef struct {
    uint8_t * p_buffer;
    uint32_t current_index;
    uint32_t current_length;
    uint32_t len;
} buffer_t;

/**@brief Encodes a slip packet.
 *
 * @details Note that the encoded output data will be longer than the input data.
 *
 * @retval The length of the encoded packet. If it is smaller than the input length, an error has occurred.
 */
uint32_t slip_encode(uint8_t * p_output,  uint8_t * p_input, uint32_t input_length, uint32_t output_buffer_length);

/**@brief Decodes a slip packet.
 *
 * @details When decoding a slip packet, a state must be preserved. Initial state must be set to SLIP_DECODING.
 *
 * @retval NRF_SUCCESS when a packet is parsed. The length of the packet can be read out from p_buf->current_index
 * @retval NRF_ERROR_BUSY when packet is not finished parsing
 * @retval NRF_ERROR_INVALID_DATA when packet is encoded wrong.
           This moves the decoding to SLIP_CLEARING_INVALID_PACKET, and will stay in this state until SLIP_END is encountered.
 */
uint32_t slip_decoding_add_char(uint8_t c, buffer_t * p_buf, slip_state_t * current_state);



#ifdef __cplusplus
}
#endif

#endif // SLIP_H__

/** @} */
