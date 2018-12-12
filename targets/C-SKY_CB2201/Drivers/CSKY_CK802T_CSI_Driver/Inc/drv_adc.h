/*
 * Copyright (C) 2017 C-SKY Microsystems Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/******************************************************************************
 * @file     drv_adc.h
 * @brief    Header File for ADC Driver
 * @version  V1.0
 * @date     15. October 2017
 ******************************************************************************/
#ifndef _CSI_ADC_H_
#define _CSI_ADC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <drv_errno.h>
#include <drv_common.h>

/// definition for adc handle.
typedef void *adc_handle_t;

/****** ADC conversion mode *****/
typedef enum {
    ADC_SINGLE = 0,       ///< Single conversion mode --- select one channel to convert at a time.
    ADC_CONTINUOUS,       ///< Continuous conversion mode --- select a channel to convert in a specific times.
    ADC_SCAN,             ///< Scan mode --- select a group channel to convert at a time.
} adc_mode_e;

/****** ADC Event *****/
typedef enum {
    ADC_EVENT_CONVERSION_COMPLETE = 0,   ///< All data conversion completed.
    ADC_EVENT_DATA_LOST,                 ///< Data lost: adc buffer overflow / transmit underflow.
    ADC_EVENT_DATA_OVERWRITE,            ///< Converted data has not been read before the new conversion result is load to the data register.
    ADC_EVENT_DATA_COMPARE_VALID,        ///< Data compare right with adc comparator.
} adc_event_e;

/****** ADC Status *****/
typedef struct {
    uint32_t busy       : 1;              ///< adc conversion on going.
    uint32_t data_lost  : 1;              ///< adc buffer overflow.
    uint32_t complete   : 1;              ///< adc conversion completion.
    uint32_t compare_valid : 1;           ///< data valid through adc comparator.
} adc_status_t;

/**
\brief ADC Driver Capabilities.
*/
typedef struct {
    uint32_t single : 1;
    uint32_t continuous : 1;
    uint32_t scan : 1;
    uint32_t calibration : 1;
    uint32_t comparator : 1;
} adc_capabilities_t;

typedef struct {
    uint32_t    clk_prescaler;  ///< select ADC clock  prescaler.
    adc_mode_e  mode;           ///< \ref adc_mode_e
    uint32_t    trigger;  ///< 0 -- software adc start or 1 -- external event trigger to start adc.
    uint32_t    intrp_mode;  ///< specifies whether the ADC is configured is interrupt mode (1)or in polling mode (0).
    uint32_t   *channel_array;  ///< channel array base address, the ordinal of channel count from zero, like ADC_CHANNEL0 (0), ADC_CHANNEL1(1), etc.
    uint32_t    channel_nbr; ///< specifies the number of channels in channel array that will be converted.
    uint32_t    conv_cnt;  ///< conversion count times for conversion mode.
    uint32_t    sampling_time; ///< sampling time value to be set for the selected channel. Unit:ADC clock cycles.
    uint32_t    offset;  ///< reserved for future use, can be set to 0.
} adc_conf_t;

typedef struct {
    uint32_t    cmp_data;        ///< the data is used to compare with conversion result of specified channel.
    uint32_t    cmp_channel;     ///< compare channel selection.
    uint32_t    cmp_condition;   ///< compare condition: 0 = Set the compare condition as that when a A/D conversion result is less than the cmp_data. 1 = Set the compare condition as that when a A/D conversion result is greater or equal to the cmp_data.
    uint32_t    cmp_match_cnt;   ///< when the specified A/D channel analog conversion result matches the compare condition defined by cmp_condition, the internal match counter will increase 1. When the internal counter reaches the value to (cpm_match_cnt), the correspond status or flag bit will be set.
} adc_cmp_conf_t;

typedef void (*adc_event_cb_t)(int32_t idx, adc_event_e event);  ///< Pointer to \ref adc_event_cb_t : adc Event call back.

/**
  \brief       Initialize adc Interface. 1. Initializes the resources needed for the adc interface 2.registers event callback function.
  \param[in]   idx       adc index.
  \param[in]   cb_event  event call back function \ref adc_event_cb_t
  \return      return adc handle if success
*/
adc_handle_t drv_adc_initialize(int32_t idx, adc_event_cb_t cb_event);

/**
  \brief       De-initialize adc Interface. stops operation and releases the software resources used by the interface
  \param[in]   handle   adc handle to operate.
  \return      error code
*/
int32_t drv_adc_uninitialize(adc_handle_t handle);

/**
  \brief       Get driver capabilities.
  \param[in]   idx    adc index.
  \return      \ref adc_capabilities_t
*/
adc_capabilities_t drv_adc_get_capabilities(int32_t idx);

/**
  \brief       config adc mode.
  \param[in]   handle     adc handle to operate.
  \param[in]   config   adc_conf_t\ref  . pointer to adc configuration structure.
  \return      error code
*/
int32_t drv_adc_config(adc_handle_t handle, adc_conf_t *config);

/**
  \brief       config adc comparator.
  \param[in]   handle     adc handle to operate.
  \param[in]   config   adc_conf_t\ref  . pointer to adc configuration structure.
  \return      error code
*/
int32_t drv_adc_comparator_config(adc_handle_t handle, adc_cmp_conf_t *config);

/**
  \brief       start adc.
  \param[in]   handle adc handle to operate.
  \return      error code
*/
int32_t drv_adc_start(adc_handle_t handle);

/**
  \brief       stop adc.
  \param[in]   handle adc handle to operate.
  \return      error code
*/
int32_t drv_adc_stop(adc_handle_t handle);

/**
  \brief       receiving data from ADC receiver.
  \param[in]   handle ADC handle to operate.
  \param[out]  data  Pointer to buffer for data to receive from ADC receiver, the data array is correspond to channel array.
  \param[in]   num   Number of data items to receive.
  \return      error code
*/
int32_t drv_adc_read(adc_handle_t handle, uint32_t *data, uint32_t num);

/**
  \brief       Get ADC status.
  \param[in]   handle   adc handle to operate.
  \return      ADC status \ref adc_status_t
*/
adc_status_t drv_adc_get_status(adc_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif /* _CSI_ADC_H_ */
