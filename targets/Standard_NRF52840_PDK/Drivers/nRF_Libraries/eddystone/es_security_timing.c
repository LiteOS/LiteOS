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

#include <stdint.h>
#include <stdbool.h>
#include "app_timer.h"
#include "es_app_config.h"

#define TICKS_1_SECOND APP_TIMER_TICKS(1000, APP_TIMER_PRESCALER)

static uint32_t m_ticks_last_returned_second = 0; //!< Tick count at last second returned from 'es_security_timing_seconds_passed'.


uint32_t es_security_timing_seconds_passed(void)
{
    uint32_t ticks_current = app_timer_cnt_get();
    uint32_t ticks_diff;
    uint32_t err_code;
    
    err_code = app_timer_cnt_diff_compute(ticks_current, m_ticks_last_returned_second, &ticks_diff);
    APP_ERROR_CHECK(err_code);
    
    if(ticks_diff > TICKS_1_SECOND)
    {
        // Update 'm_ticks_last_returned_second' so it points to the last 'whole second' as it is this point we are referring to when
        // returning the number of seconds passed since last invocation. 
        // I.E. if this function is called after 1.5 seconds and 2.0 seconds, 'm_ticks_last_returned_second' must point to 1.0 seconds
        // after the first invocation in order to return '1' upon the second invocation.
        uint32_t current_second_ms = (ticks_current / TICKS_1_SECOND) * 1000;
        
        m_ticks_last_returned_second = APP_TIMER_TICKS(current_second_ms, APP_TIMER_PRESCALER);
        
        return ticks_diff / TICKS_1_SECOND;
    }
    
    return 0;
}


void es_security_timing_init(void)
{
    m_ticks_last_returned_second = 0;
}
