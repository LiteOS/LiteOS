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
 * @defgroup app_mailbox Mailbox library
 * @{
 * @ingroup app_common
 *
 * @brief Mailbox for safely queuing items.
 *
 */

#ifndef _APP_MAILBOX_H
#define _APP_MAILBOX_H

#include <stdint.h>
#include "sdk_errors.h"
#include "app_util.h"
#include "nordic_common.h"

/**
 * @brief Supported overflow modes.
 */
typedef enum
{
    APP_MAILBOX_MODE_NO_OVERFLOW, //!< If the mailbox is full, @ref app_mailbox_put does not add a new element.
    APP_MAILBOX_MODE_OVERFLOW     //!< If the mailbox is full, the oldest element is lost and a new one is added.
} app_mailbox_overflow_mode_t;

#include "app_mailbox_local.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Mailbox definition structure.
 */
typedef struct
{
  void                      * p_pool;    /**< Memory array for mail. */
  app_mailbox_cb_t          * p_cb;      /**< Mailbox handle. */
  uint16_t                    item_sz;  /**< Size of a single item. */
  uint8_t                     queue_sz; /**< Capacity of the queue. */
} app_mailbox_t;

/**
 * @brief Macro to statically allocate memory for a given mailbox queue.
 */
#define APP_MAILBOX_DEF(name, QUEUE_SZ, ITEM_SZ)                                            \
static uint32_t         STRING_CONCATENATE(mailbox_items_,name)[(1 + CEIL_DIV((ITEM_SZ),4)) * (QUEUE_SZ)];\
static app_mailbox_cb_t STRING_CONCATENATE(mailbox_cb_,name);                                         \
const app_mailbox_t name =                                                                  \
    {                                                                                       \
        .p_pool   = STRING_CONCATENATE(mailbox_items_,name),                                          \
        .p_cb     = STRING_CONCATENATE(&mailbox_cb_,name),                                            \
        .queue_sz = (uint8_t)(QUEUE_SZ),                                                    \
        .item_sz  = (uint16_t)(ITEM_SZ),                                                    \
    }

/**
 * @brief Function for creating a mailbox queue.
 *
 * This function creates and initializes a mailbox queue.
 *
 * @param[in]  p_mailbox  Pointer to the mailbox.
 *
 * @retval NRF_SUCCESS              If the queue was successfully created.
 */
ret_code_t app_mailbox_create(const app_mailbox_t * p_mailbox);

/**
 * @brief Function for putting an item in the mailbox queue.
 *
 * @param[in] p_mailbox   Pointer to the mailbox.
 * @param[in] p_item      Pointer to the item to be queued.
 *
 * @retval NRF_SUCCESS              If the item was enqueued.
 * @retval NRF_ERROR_NO_MEM         If the queue is full.
 */
ret_code_t app_mailbox_put (const app_mailbox_t * p_mailbox, void * p_item);

/**
 * @brief Function for putting an item with a specified size in the mailbox queue.
 *
 * @param[in] p_mailbox   Pointer to the mailbox.
 * @param[in] p_item      Pointer to the item to be queued.
 * @param[in] size        Size of the item.
 *
 * @retval NRF_SUCCESS              If the item was enqueued.
 * @retval NRF_ERROR_NO_MEM         If the queue is full.
 */
ret_code_t app_mailbox_sized_put (const app_mailbox_t * p_mailbox, void * p_item, uint16_t size);

/**
 * @brief Function for getting an item from the mailbox queue.
 *
 * @param[in]  p_mailbox   Pointer to the mailbox.
 * @param[out] p_item      Pointer to the output location for the dequeued item.
 *
 * @retval NRF_SUCCESS              If the item was retrieved successfully.
 * @retval NRF_ERROR_NO_MEM         If the queue is empty.
 */
ret_code_t app_mailbox_get (const app_mailbox_t * p_mailbox, void * p_item);

/**
 * @brief Function for getting an item and its size from the mailbox queue.
 *
 * @param[in]  p_mailbox   Pointer to the mailbox.
 * @param[out] p_item      Pointer to the output location for the dequeued item.
 * @param[out] p_size      Pointer to the item size.
 *
 * @retval NRF_SUCCESS              If the item was retrieved successfully.
 * @retval NRF_ERROR_NO_MEM         If the queue is empty.
 */
ret_code_t app_mailbox_sized_get (const app_mailbox_t * p_mailbox, void * p_item, uint16_t * p_size);

/**
 * @brief Function for getting the current length of the mailbox queue.
 *
 * @param[in]  p_mailbox   Pointer to the mailbox.
 *
 * @return Current number of elements in the queue.
 *
 */
uint32_t app_mailbox_length_get (const app_mailbox_t * p_mailbox);

/**
 * @brief Function for changing the mode of overflow handling.
 *
 * @param[in]  p_mailbox   Pointer to the mailbox.
 * @param mode             New mode to set.
 */
void app_mailbox_mode_set(const app_mailbox_t * p_mailbox, app_mailbox_overflow_mode_t mode);


#ifdef __cplusplus
}
#endif

#endif //_APP_MAILBOX_H
/** @} */
