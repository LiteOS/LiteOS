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

#include "sdk_config.h"
#if APP_MAILBOX_ENABLED
#include <stddef.h>
#include <string.h>
#include "app_mailbox.h"
#include "nrf_error.h"
#include "app_util_platform.h"
#include "app_util.h"
#include "nrf_assert.h"

ret_code_t app_mailbox_create(const app_mailbox_t * queue_def)
{
    queue_def->p_cb->r_idx    = 0;
    queue_def->p_cb->w_idx    = 0;
    queue_def->p_cb->len      = 0;
    queue_def->p_cb->mode     = APP_MAILBOX_MODE_NO_OVERFLOW;

    return NRF_SUCCESS;
}

static __INLINE void dequeue(app_mailbox_cb_t * p_cb, uint8_t queue_sz)
{
    uint8_t  r_idx = p_cb->r_idx + 1;
    p_cb->len--;

    //Handle index wrapping.
    p_cb->r_idx = (r_idx == queue_sz) ? 0 : r_idx;
}

static __INLINE void enqueue(app_mailbox_cb_t * p_cb, uint8_t queue_sz)
{
    uint8_t  w_idx = p_cb->w_idx + 1;
    p_cb->len++;

    //Handle index wrapping.
    p_cb->w_idx = (w_idx == queue_sz) ? 0 : w_idx;
}

ret_code_t app_mailbox_put(const app_mailbox_t * p_mailbox, void * p_item)
{
    ASSERT(p_mailbox);
    return app_mailbox_sized_put(p_mailbox, p_item, p_mailbox->item_sz);
}

ret_code_t app_mailbox_sized_put(const app_mailbox_t * p_mailbox, void * p_item, uint16_t size)
{
    ASSERT((uint32_t)p_item>0);
    ASSERT(p_mailbox);
    uint32_t           err_code  = NRF_ERROR_INTERNAL;
    uint32_t *         p_dst     = p_mailbox->p_pool;
    bool               do_put    = true;
    uint8_t            queue_sz  = p_mailbox->queue_sz;
    uint16_t           item_sz   = p_mailbox->item_sz;
    app_mailbox_cb_t * p_cb      = p_mailbox->p_cb;

    CRITICAL_REGION_ENTER();

    if (p_cb->len == queue_sz)
    {
        if (p_cb->mode == APP_MAILBOX_MODE_NO_OVERFLOW)
        {
            do_put = false;
        }
        else
        {
            // Remove the oldest element.
            dequeue(p_cb, queue_sz);
        }
        err_code = NRF_ERROR_NO_MEM;
    }
    else
    {
        err_code = NRF_SUCCESS;
    }

    if (do_put)
    {
        p_dst = (uint32_t *)((uint32_t)p_dst + (p_cb->w_idx * (item_sz + sizeof(uint32_t))));
        enqueue(p_cb, queue_sz);

        //Put data in mailbox.
        *p_dst = (uint32_t)size;
        p_dst++;
        memcpy(p_dst, p_item, size);
    }

    CRITICAL_REGION_EXIT();

    return err_code;
}

ret_code_t app_mailbox_get(const app_mailbox_t * p_mailbox, void * p_item)
{
    uint16_t size;
    return app_mailbox_sized_get(p_mailbox, p_item, &size);
}

ret_code_t app_mailbox_sized_get(const app_mailbox_t * p_mailbox, void * p_item, uint16_t * p_size)
{
    ASSERT(p_mailbox);
    ASSERT((uint32_t)p_item>0);
    uint32_t *         p_src     = p_mailbox->p_pool;
    ret_code_t         err_code  = NRF_SUCCESS;
    uint16_t           item_sz   = p_mailbox->item_sz;
    uint8_t            queue_sz  = p_mailbox->queue_sz;
    app_mailbox_cb_t * p_cb      = p_mailbox->p_cb;

    CRITICAL_REGION_ENTER();

    if (p_cb->len == 0)
    {
        err_code = NRF_ERROR_NO_MEM;
    }
    else
    {
        p_src = (void *)((uint32_t)p_src + (p_cb->r_idx * (item_sz + sizeof(uint32_t))));
        dequeue(p_cb, queue_sz);
    }

    if (err_code == NRF_SUCCESS)
    {
        uint16_t size = (uint16_t)*p_src;
        *p_size = size;
        p_src++;
        memcpy(p_item, p_src, size);
    }

    CRITICAL_REGION_EXIT();

    return err_code;
}

uint32_t app_mailbox_length_get (const app_mailbox_t * p_mailbox)
{
    ASSERT(p_mailbox);
    return p_mailbox->p_cb->len;
}

void app_mailbox_mode_set(const app_mailbox_t * p_mailbox, app_mailbox_overflow_mode_t mode)
{
    ASSERT(p_mailbox);
    p_mailbox->p_cb->mode = mode;
}
#endif //APP_MAILBOX_ENABLED
