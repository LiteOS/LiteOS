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

#include "nrf_ic_info.h"
#include "nrf.h"

#define PART_NO_NRF51 0x01
#define PART_NO_NRF52 0x06

void nrf_ic_info_get(nrf_ic_info_t * p_ic_info)
{
    uint32_t ic_data    = ((*((uint32_t volatile *)0xF0000FE8)) & 0x000000F0) >> 4;
    uint32_t ic_part_no = ((*((uint32_t volatile *)0xF0000FE0)) & 0x000000FF);

    switch (ic_part_no)
    {
#if defined(NRF51)
        case PART_NO_NRF51:
        {
            p_ic_info->ram_size   = (uint16_t) NRF_FICR->NUMRAMBLOCK;
            p_ic_info->ram_size   *= (uint16_t) (NRF_FICR->SIZERAMBLOCKS / 1024);
            p_ic_info->flash_size = (uint16_t) NRF_FICR->CODESIZE;

            switch (ic_data)
            {
                /** IC revision 1 */
                case 1:
                    p_ic_info->ic_revision = IC_REVISION_NRF51_REV1;
                    break;

                /** IC revision 2 */
                case 4:
                    p_ic_info->ic_revision = IC_REVISION_NRF51_REV2;
                    break;

                /** IC revision 3 */
                case 7:
                    /* fall through */
                case 8:
                    /* fall through */
                case 9:
                    p_ic_info->ic_revision = IC_REVISION_NRF51_REV3;
                    break;

                default:
                    p_ic_info->ic_revision = IC_REVISION_NRF51_UNKNOWN;
                    break;
            }
            break;
        }
#endif
        default:
            p_ic_info->ic_revision = IC_PART_UNKNOWN;
            p_ic_info->flash_size  = 0;
            p_ic_info->ram_size    = 0;
            break;
    }
}
