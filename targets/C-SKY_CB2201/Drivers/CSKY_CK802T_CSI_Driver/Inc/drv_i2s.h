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
 * @file     drv_i2s.h
 * @brief    header file for i2s driver
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/

#ifndef _DRV_I2S_H_
#define _DRV_I2S_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <drv_common.h>

/// definition for i2s handle.
typedef void *i2s_handle_t;

/****** i2s specific error codes *****/
typedef enum {
    I2S_ERROR_SYNCHRONIZATION   = (DRV_ERROR_SPECIFIC + 1),  ///< Specified Synchronization not supported
    I2S_ERROR_PROTOCOL,                                      ///< Specified Protocol not supported
    I2S_ERROR_DATA_SIZE,                                     ///< Specified Data size not supported
    I2S_ERROR_BIT_ORDER,                                     ///< Specified Bit order not supported
    I2S_ERROR_MONO_MODE,                                     ///< Specified Mono mode not supported
    I2S_ERROR_COMPANDING,                                    ///< Specified Companding not supported
    I2S_ERROR_CLOCK_POLARITY,                                ///< Specified Clock polarity not supported
    I2S_ERROR_AUDIO_FREQ,                                    ///< Specified Audio frequency not supported
    I2S_ERROR_MCLK_PIN,                                      ///< Specified MCLK Pin setting not supported
    I2S_ERROR_MCLK_PRESCALER,                                ///< Specified MCLK Prescaler not supported
    I2S_ERROR_FRAME_LENGHT,                                  ///< Specified Frame length not supported
    I2S_ERROR_FRAME_SYNC_WIDTH,                              ///< Specified Frame Sync width not supported
    I2S_ERROR_FRAME_SYNC_POLARITY,                           ///< Specified Frame Sync polarity not supported
    I2S_ERROR_FRAME_SYNC_EARLY,                              ///< Specified Frame Sync early not supported
    I2S_ERROR_SLOT_COUNT,                                    ///< Specified Slot count not supported
    I2S_ERROR_SLOT_SIZE,                                     ///< Specified Slot size not supported
    I2S_ERROR_SLOT_OFFESET                                   ///< Specified Slot offset not supported
} i2s_error_e;

typedef enum {
    I2S_MODE_TX_MASTER,  ///< i2s transmitter master mode
    I2S_MODE_TX_SLAVE,   ///< i2s transmitter slave mode
    I2S_MODE_RX_MASTER,  ///< i2s receiver master mode
    I2S_MODE_RX_SLAVE,   ///< i2s receiver slave mode
} i2s_mode_e;

typedef enum {
    I2S_PROTOCOL_USER,          ///< User defined
    I2S_PROTOCOL_I2S,           ///< i2s bus
    I2S_PROTOCOL_MSB_JUSTIFIED, ///< MSB (left) justified
    I2S_PROTOCOL_LSB_JUSTIFIED, ///< LSB (right) justified
} i2s_protocol_e;

/**
\brief I2S Status
*/
typedef struct {
    uint32_t tx_busy          : 1;        ///< Transmitter busy flag
    uint32_t rx_busy          : 1;        ///< Receiver busy flag
    uint32_t tx_underflow     : 1;        ///< Transmit data underflow detected (cleared on start of next send operation)
    uint32_t rx_overflow      : 1;        ///< Receive data overflow detected (cleared on start of next receive operation)
    uint32_t frame_error      : 1;        ///< Sync Frame error detected (cleared on start of next send/receive operation)
} i2s_status_t;

/****** IIC Event *****/
typedef enum {
    I2S_EVENT_SEND_COMPLETE        = 0,  ///< Send completed
    I2S_EVENT_RECEIVE_COMPLETE     = 1,  ///< Receive completed
    I2S_EVENT_TX_UNDERFLOW         = 2,  ///< Transmit data not available
    I2S_EVENT_RX_OVERFLOW          = 3,  ///< Receive data overflow
    I2S_EVENT_FRAME_ERROR          = 4,  ///< Sync Frame error in Slave mode (optional)
} i2s_event_e;

/**
\brief i2s Driver Capabilities.
*/
typedef struct {
    uint32_t    asynchronous        : 1;    ///< supports asynchronous Transmit/Receive
    uint32_t    synchronous         : 1;    ///< supports synchronous Transmit/Receive
    uint32_t    protocol_user       : 1;    ///< supports user defined Protocol
    uint32_t    protocol_i2s        : 1;    ///< supports I2S Protocol
    uint32_t    protocol_justified  : 1;    ///< supports MSB/LSB justified Protocol
    uint32_t    mono_mode           : 1;    ///< supports Mono mode
    uint32_t    companding          : 1;    ///< supports Companding
    uint32_t    mclk_pin            : 1;    ///< supports MCLK (Master Clock) pin
    uint32_t    event_frame_error   : 1;    ///< supports Frame error event: ARM_SAI_EVENT_FRAME_ERROR
} i2s_capabilities_t;

typedef void (*i2s_event_cb_t)(int32_t idx, i2s_event_e event);  ///< Pointer to \ref i2s_event_cb_t : i2s Event call back.

/**
  \brief       Initialize I2S Interface. 1. Initializes the resources needed for the I2S index 2.registers event callback function
  \param[in]   idx  i2s index
  \param[in]   cb_event  Pointer to \ref usart_event_cb_t
  \return      pointer to i2s instances
*/
i2s_handle_t csi_i2s_initialize(int32_t idx, i2s_event_cb_t cb_event);

/**
  \brief       De-initialize I2S Interface. stops operation and releases the software resources used by the interface
  \param[in]   handle i2s handle to operate.
  \return      error code
*/
int32_t csi_i2s_uninitialize(i2s_handle_t handle);

/**
  \brief       Get driver capabilities.
  \param[in]   idx i2s index.
  \return      \ref i2s_capabilities_t
*/
i2s_capabilities_t csi_i2s_get_capabilities(int32_t idx);

/**
  \brief       config i2s attributes.
  \param[in]   handle       i2s handle to operate.
  \param[in]   protocol     i2s protocol \ref i2s_protocol_e.
  \param[in]   samplewidth  size of sample in bits
  \param[in]   mode         work mode of i2s \ref Work mode of i2s.
  \param[in]   rate         audio sample rate.
  \return      0 for success, negative for error code
*/
int32_t csi_i2s_config(i2s_handle_t handle, i2s_protocol_e protocol, uint32_t samplewidth, i2s_mode_e mode, uint32_t rate);

/**
  \brief       config i2s protocol.
  \param[in]   handle       i2s handle to operate.
  \param[in]   protocol     i2s protocol \ref i2s_protocol_e.
  \return      0 for success, negative for error code
*/
int32_t csi_i2s_config_protocol(i2s_handle_t handle, i2s_protocol_e protocol);

/**
  \brief       config i2s sample width.
  \param[in]   handle       i2s handle to operate.
  \param[in]   samplewidth  size of sample in bits
  \return      0 for success, negative for error code
*/
int32_t csi_i2s_config_sample_width(i2s_handle_t handle, uint32_t samplewidth);

/**
  \brief       config i2s work mode.
  \param[in]   mode         work mode of i2s \ref Work mode of i2s.
  \return      0 for success, negative for error code
*/
int32_t csi_i2s_config_work_mode(i2s_handle_t handle, i2s_mode_e mode);

/**
  \brief       config i2s sample rate.
  \param[in]   handle       i2s handle to operate.
  \param[in]   rate         audio sample rate.
  \return      0 for success, negative for error code
*/
int32_t csi_i2s_config_sample_rate(i2s_handle_t handle, uint32_t rate);

/**
  \brief       sending data to i2s transmitter.
  \param[in]   handle       i2s handle to operate.
  \param[in]   data         Pointer to buffer for data to send
  \param[in]   data_size    size of tranmitter data
  \return      0 for success, negative for error code
*/
int32_t csi_i2s_send(i2s_handle_t handle, const void *data, uint32_t data_size);

/**
  \brief       abort send data to I2S transmmitter.
  \param[in]   handle       i2s handle to operate.
  \return      0 for success, negative for error code
*/
int32_t csi_i2s_abort_send(i2s_handle_t handle);

/**
  \brief       using dma send data to i2s transmitter.
  \param[in]   handle       i2s handle to operate.
  \param[in]   data         Pointer to buffer for data to send
  \param[in]   data_size    size of tranmitter data
  \return      0 for success, negative for error code
*/
int32_t csi_i2s_dma_send(i2s_handle_t handle, const void *data, uint32_t data_size);

/**
  \brief       receiving data from i2s receiver.
  \param[in]   handle       i2s handle to operate.
  \param[in]   data         Pointer to buffer for data to receive from i2s receiver
  \param[in]   data_size    size of receiver data
  \return      0 for success, negative for error code
*/
int32_t csi_i2s_receive(i2s_handle_t handle, void *data, uint32_t data_size);

/**
  \brief       abort receive data from I2S receiver.
  \param[in]   handle       i2s handle to operate.
  \return      0 for success, negative for error code
*/
int32_t csi_i2s_abort_receive(i2s_handle_t handle);

/**
  \brief       using dma receive data from i2s receiver.
  \param[in]   handle       i2s handle to operate.
  \param[in]   data         Pointer to buffer for data to receive from i2s receiver
  \param[in]   data_size    size of receiver data
  \return      0 for success, negative for error code
*/
int32_t csi_i2s_dma_receive(i2s_handle_t handle, void *data, uint32_t data_size);

/**
  \brief       Get i2s status.
  \param[in]   handle i2s handle to operate.
  \return      i2s status \ref i2s_status_e
*/
i2s_status_t csi_i2s_get_status(i2s_handle_t handle);

/**
  \brief       control I2S power.
  \param[in]   handle  i2s handle to operate.
  \param[in]   state   power state.\ref csi_power_stat_e.
  \return      error code
*/
int32_t csi_i2s_power_control(i2s_handle_t handle, csi_power_stat_e state);

#ifdef __cplusplus
}
#endif

#endif /* _DRV_I2S_H_ */

