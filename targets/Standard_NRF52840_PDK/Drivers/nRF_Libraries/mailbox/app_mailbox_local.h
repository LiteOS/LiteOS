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

/**
 * @cond (NODOX)
 * @defgroup app_mailbox_internal Auxiliary internal types declarations
 * @{
 * @ingroup app_mailbox
 * @internal
 *
 * @brief Module for internal usage inside the library only
 *
 * Some definitions must be included in the header file because
 * of the way the library is set up. In this way, the are accessible to the user.
 * However, any functions and variables defined here may change at any time
 * without a warning, so you should not access them directly.
 */

#ifndef APP_MAILBOX_LOCAL_H__
#define APP_MAILBOX_LOCAL_H__

#ifdef __cplusplus
extern "C" {
#endif

 /**
 * @brief Mailbox handle used for managing a mailbox queue.
 */
typedef struct
{
    uint8_t                      r_idx;    /**< Read index for the mailbox queue. */
    uint8_t                      w_idx;    /**< Write index for the mailbox queue. */
    uint8_t                      len;      /**< Number of elements currently in the mailbox queue. */
    app_mailbox_overflow_mode_t  mode;     /**< Mode of overflow handling. */
} app_mailbox_cb_t;

#ifdef __cplusplus
}
#endif

#endif // APP_MAILBOX_LOCAL_H__

/** @}
 * @endcond
 */
