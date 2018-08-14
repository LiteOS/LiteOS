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

/** @file
 *
 * @defgroup nrf_ic_info IC information
 * @{
 * @ingroup app_common
 *
 * @brief Library for checking IC information (IC revision, RAM size, FLASH size).
 *
 */

#ifndef NRF51_IC_INFO_H__
#define NRF51_IC_INFO_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**@brief Enum identifying the IC revision as described in the Compatibility Matrix. */
typedef enum
{
    IC_PART_UNKNOWN = 0,        /**< IC Revision unknown. */
    IC_REVISION_NRF51_REV1,     /**< IC Revision 1. */
    IC_REVISION_NRF51_REV2,     /**< IC Revision 2. */
    IC_REVISION_NRF51_REV3,     /**< IC Revision 3. */
    IC_REVISION_NRF51_UNKNOWN   /**< IC Revision unknown. */
} nrf_ic_revision_t;

 /**@brief IC information struct containing the IC revision, RAM size, and FLASH size. */
typedef struct
{
    nrf_ic_revision_t ic_revision;    /**< IC revision. */
    uint16_t          ram_size;       /**< RAM size in kB (16 = 16 kB RAM). */
    uint16_t          flash_size;     /**< FLASH size in kB (256 = 256 kB FLASH). */
} nrf_ic_info_t;


/**@brief  Function for returning information about the IC revision, the RAM size, and the FLASH size.
 *
 * @param[out]  p_ic_info  Struct containing IC revision, RAM size, and FLASH size.
 *
 */
void nrf_ic_info_get(nrf_ic_info_t*  p_ic_info);



#ifdef __cplusplus
}
#endif

#endif // NRF51_IC_INFO_H__

/** @} */
