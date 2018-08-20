/**
 * Copyright (c) 2014 - 2017, Nordic Semiconductor ASA
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

 #ifndef APP_TIMER_APPSH_H
 #define APP_TIMER_APPSH_H

#include "app_timer.h"

#ifdef __cplusplus
extern "C" {
#endif

#define APP_TIMER_SCHED_EVT_SIZE     sizeof(app_timer_event_t)  /**< Size of button events being passed through the scheduler (is to be used for computing the maximum size of scheduler events). */

/**@brief Macro for initializing the application timer module to use with app_scheduler.
 *
 * @param[in]  PRESCALER        Value of the RTC1 PRESCALER register. This will decide the
 *                              timer tick rate. Set to 0 for no prescaling.
 * @param[in]  OP_QUEUES_SIZE   Size of queues holding timer operations that are pending execution.
 * @param[in]  USE_SCHEDULER    TRUE if the application is using the app_scheduler,
 *                              FALSE otherwise.
 *
 * @note Since this macro allocates a buffer, it must only be called once (it is OK to call it
 *       several times as long as it is from the same location, e.g. to do a reinitialization).
 */
#define APP_TIMER_APPSH_INIT(PRESCALER, OP_QUEUES_SIZE, USE_SCHEDULER)                 \
    APP_TIMER_INIT(PRESCALER, OP_QUEUES_SIZE,                                          \
                                (USE_SCHEDULER) ? app_timer_evt_schedule : NULL)

typedef struct
{
    app_timer_timeout_handler_t timeout_handler;
    void *                      p_context;
} app_timer_event_t;

uint32_t app_timer_evt_schedule(app_timer_timeout_handler_t timeout_handler,
                                void *                      p_context);

#ifdef __cplusplus
}
#endif

#endif // APP_TIMER_APPSH_H

