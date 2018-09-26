/**
 * Copyright (c) 2016 - 2017, Nordic Semiconductor ASA
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

#include "sdk_errors.h"
#include "nordic_common.h"

#if defined(NRF_LOG_ENABLED) && NRF_LOG_ENABLED
/**
 * @defgroup err_strings Arrays with error codes as strings.
 * @{
 */
const char * m_sdk_errors_name[ERR_NAMES_SIZE] = 
{
    "NRF_SUCCESS",
    "NRF_ERROR_SVC_HANDLER_MISSING",
    "NRF_ERROR_SOFTDEVICE_NOT_ENABLED",
    "NRF_ERROR_INTERNAL",
    "NRF_ERROR_NO_MEM",
    "NRF_ERROR_NOT_FOUND",
    "NRF_ERROR_NOT_SUPPORTED",
    "NRF_ERROR_INVALID_PARAM",
    "NRF_ERROR_INVALID_STATE",
    "NRF_ERROR_INVALID_LENGTH",
    "NRF_ERROR_INVALID_FLAGS",
    "NRF_ERROR_INVALID_DATA",
    "NRF_ERROR_DATA_SIZE",
    "NRF_ERROR_TIMEOUT",
    "NRF_ERROR_NULL",
    "NRF_ERROR_FORBIDDEN",
    "NRF_ERROR_INVALID_ADDR",
    "NRF_ERROR_BUSY"
};

const char * m_sdk_errors_name_common[ERR_NAMES_COMMON_SIZE] =
{
    "NRF_ERROR_MODULE_NOT_INITIALZED",
    "NRF_ERROR_MUTEX_INIT_FAILED",
    "NRF_ERROR_MUTEX_LOCK_FAILED",
    "NRF_ERROR_MUTEX_UNLOCK_FAILED",
    "NRF_ERROR_MUTEX_COND_INIT_FAILED",
    "NRF_ERROR_MODULE_ALREADY_INITIALIZED",
    "NRF_ERROR_STORAGE_FULL",
    "NRF_ERROR_API_NOT_IMPLEMENTED",
    "NRF_ERROR_FEATURE_NOT_ENABLED"
};

const char * m_sdk_errors_name_twi[ERR_NAMES_TWI_SIZE] =
{
    "NRF_ERROR_DRV_TWI_ERR_OVERRUN",
    "NRF_ERROR_DRV_TWI_ERR_ANACK",
    "NRF_ERROR_DRV_TWI_ERR_DNACK"
};
/* @} */
#endif // NRF_LOG_ENABLED
