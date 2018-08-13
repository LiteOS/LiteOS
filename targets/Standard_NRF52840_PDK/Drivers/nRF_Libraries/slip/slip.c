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
#include "sdk_common.h"
#if NRF_MODULE_ENABLED(SLIP)
#include "slip.h"
#include "nrf_error.h"

#define SLIP_END             0300    /* indicates end of packet */
#define SLIP_ESC             0333    /* indicates byte stuffing */
#define SLIP_ESC_END         0334    /* ESC ESC_END means END data byte */
#define SLIP_ESC_ESC         0335    /* ESC ESC_ESC means ESC data byte */


uint32_t slip_encode(uint8_t * p_output,  uint8_t * p_input, uint32_t input_length, uint32_t output_buffer_length)
{
    uint32_t input_index;
    uint32_t output_index;

    for (input_index = 0, output_index = 0; input_index < input_length && output_index < output_buffer_length; input_index++)
    {
        switch (p_input[input_index])
        {
            case SLIP_END:
                p_output[output_index++] = SLIP_END;
                p_output[output_index++] = SLIP_ESC_END;
                break;

            case SLIP_ESC:
                p_output[output_index++] = SLIP_ESC;
                p_output[output_index++] = SLIP_ESC_ESC;
                break;

            default:
                p_output[output_index++] = p_input[input_index];
        }
    }
    p_output[output_index++] = (uint8_t)SLIP_END;
    p_output[output_index++] = (uint8_t)SLIP_END; // clarify that the packet has ended.

    return output_index;
}


uint32_t slip_decoding_add_char(uint8_t c, buffer_t * p_buf, slip_state_t * current_state)
{
    switch (*current_state)
    {
        case SLIP_DECODING:
            if (c == SLIP_END)
            {
                *current_state = SLIP_END_RECEIVED;
            }
            else if (c == SLIP_ESC)
            {
                *current_state = SLIP_END_RECEIVED;
            }
            else
            {
                p_buf->p_buffer[p_buf->current_index++] = c;
                p_buf->current_length++;
            }
            break;

        case SLIP_ESC_RECEIVED:
            if (c == SLIP_ESC_ESC)
            {
                p_buf->p_buffer[p_buf->current_index++] = SLIP_ESC;
                p_buf->current_length++;
                *current_state = SLIP_DECODING;
            }
            else
            {
                // violation of protocol
                *current_state = SLIP_CLEARING_INVALID_PACKET;
                return NRF_ERROR_INVALID_DATA;
            }
            break;

        case SLIP_END_RECEIVED:
            if (c == SLIP_ESC_END)
            {
                p_buf->p_buffer[p_buf->current_index++] = SLIP_END;
                p_buf->current_length++;
                *current_state = SLIP_DECODING;
            }
            else
            {
                // packet is finished
                *current_state = SLIP_DECODING;
                return NRF_SUCCESS;
            }
            break;

        case SLIP_CLEARING_INVALID_PACKET:
            if (c == SLIP_END)
            {
                *current_state = SLIP_DECODING;
                p_buf->current_index = 0;
                p_buf->current_length = 0;
            }
            break;
    }
    return NRF_ERROR_BUSY;
}
#endif //NRF_MODULE_ENABLED(SLIP)
