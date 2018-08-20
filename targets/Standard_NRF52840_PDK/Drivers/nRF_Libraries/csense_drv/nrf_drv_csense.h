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

#ifndef NRF_DRV_CSENSE_H__
#define NRF_DRV_CSENSE_H__

#include <stdint.h>
#include "sdk_errors.h"
#include "app_timer.h"
#include "nrf_drv_common.h"

/** @file
 *
 * @defgroup nrf_drv_csense Capacitive sensor support
 * @{
 * @ingroup app_common
 *
 * @brief Module for using a capacitive sensor on low-energy level.
 */

/** @brief Maximum number of analog inputs. */
#define MAX_ANALOG_INPUTS 8

/**
 * @brief Module initializing structure.
 */
typedef struct
{
    uint8_t output_pin;                         /**< Pin where to generate a voltage for charging capacitors. */
}nrf_drv_csense_config_t;

/**
 * @brief Structure holding event parameters.
 */
typedef struct
{
    uint16_t                    read_value;             /**< Value which was read on analog channel. It is voltage in millivolts for NRF51 or time in ticks of 10 periods of relaxation
                                                        oscillator for NRF52. Voltage corresponds to capacitance of pad attached to analog channel and gets bigger once it
                                                        is touched. Period of relaxation also corresponds to pad capacitance and increases its value when capacitance getting
                                                        bigger. */
    uint8_t                     analog_channel;         /**< Index of analog channel from which value was read. */
}nrf_drv_csense_evt_t;

/**
 * @brief Csense events handler. Called from conversion handler.
 *
 * @param[in] event_struct                Structure holding event parameters.
 */
typedef void (* nrf_drv_csense_event_handler_t) (nrf_drv_csense_evt_t * p_event_struct);

/**
 * @brief Function for initializing the module.
 *
 * After calling function module is in initialized state and all channels are disabled, @ref nrf_drv_csense_channels_enable
 * function needs to be called. Initializes all modules needed by capacitive sensor library: ADC for NRF51 or TIMERs, PPIs and COMP for NRF52.
 *
 * @param[in] p_config                           Structure for initializing module.
 * @param[in] event_handler                      Event handler for capacitor sensor events.
 *
 * @retval    NRF_ERROR_INVALID_PARAM            Invalid parameter.
 * @retval    NRF_ERROR_NO_MEM                   Timer operations queue was full.
 * @retval    NRF_ERROR_INTERNAL                 Error occurred during timers, PPI's or COMP initialization.
 * @retval    NRF_SUCCESS                        Module was initialized successfully.
 *
 * @sa nrf_drv_csense_channels_enable
 */
ret_code_t nrf_drv_csense_init(nrf_drv_csense_config_t const * p_config, nrf_drv_csense_event_handler_t event_handler);

/**
 * @brief Function for unintializing the capacitive sensor. Clears mask of enabled channels.
 *
 * @return Values returned by @ref nrf_drv_ppi_channel_free.
 */
ret_code_t nrf_drv_csense_uninit(void);

/**
 * @brief Function for enabling analog channels for the capacitive sensor.
 *
 * @param[in] channels_mask                     Mask of analog channels to be enabled.
 */
void nrf_drv_csense_channels_enable(uint8_t channels_mask);

/**
 * @brief Function for disabling analog channels of the capacitive sensor.
 *
 * @param[in] channels_mask                      Mask of analog channels to be enabled.
 */
void nrf_drv_csense_channels_disable(uint8_t channels_mask);

/**
 * @brief Function for getting last read value from an analog channel.
 *
 * @param[in] csense_channel                    Number of the channel to get value from.
 *
 * @return Analog value measured on the channel.
 */
uint16_t nrf_drv_csense_channel_read(uint8_t csense_channel);

/**
 * @brief Function for triggering a measurement on all enabled analog channels. Handler will be called on every completed measurement.
 *
 * @retval      NRF_ERROR_BUSY                  If module was busy or ADC module is in use and was busy.
 * @retval      NRF_SUCCESS                     If measurement was triggered successfully.
 */
ret_code_t nrf_drv_csense_sample(void);

/**
 * @brief Function for checking if module is busy.
 *
 * @return True if busy or false if not busy.
 */
bool nrf_drv_csense_is_busy(void);

/** @} */

#endif //NRF_DRV_CSENSE_H__
