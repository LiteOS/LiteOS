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
 * @file     drv_dmac.h
 * @brief    header file for dmac driver
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/
#ifndef _CSI_DMA_H_
#define _CSI_DMA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <drv_common.h>

/// definition for dmac handle.
typedef void *dmac_handle_t;

/**
\brief DMA Driver Capabilities.
*/
typedef struct {
    uint32_t unalign_addr : 1;          ///< support for unalign address transfer when memory is source
} dma_capabilities_t;

typedef enum {
    DMA_STATE_FREE = 0,       ///< DMA channel not yet initialized or disabled
    DMA_STATE_READY,          ///< DMA channel process success and ready for use, but not start yet
    DMA_STATE_BUSY,           ///< DMA channel process is ongoing
    DMA_STATE_DONE,           ///< DMA channel transfer done
    DMA_STATE_ERROR,          ///< DMA channel transfer error
} dma_status_e;

/****** DMA specific error codes *****/
typedef enum {
    EDRV_DMA_MODE  = (DRV_ERROR_SPECIFIC + 1),     ///< Specified Mode not supported
} dma_error_e;

/****** DMA Event *****/
typedef enum {
    DMA_EVENT_TRANSFER_DONE        = 0,  ///< transfer complete
    DMA_EVENT_TRANSFER_HALF_DONE   = 1,  ///< transfer half done
    DMA_EVENT_TRANSFER_MODE_DONE   = 2,  ///< transfer complete in a certain dma trigger mode.
    DMA_EVENT_CAHNNEL_PEND         = 3,  ///< it happens when there is a low priority channel was preempted by a high priority channel
    DMA_EVENT_TRANSFER_ERROR       = 4,  ///< transfer error
} dma_event_e;

typedef enum {
    DMA_ADDR_INC    = 0,
    DMA_ADDR_DEC,
    DMA_ADDR_CONSTANT
} dma_addr_inc_e;

typedef enum {
    DMA_MEM2MEM     = 0,
    DMA_MEM2PERH,
    DMA_PERH2MEM,
    DMA_PERH2PERH,
} dma_trans_type_e;

typedef enum {
    DMA_SINGLE_TRIGGER     = 0,
    DMA_GROUP_TRIGGER,
    DMA_BLOCK_TRIGGER
} dma_trig_trans_mode_e;

typedef enum {
    DMA_DIR_DEST = 0,
    DMA_DIR_SOURCE
} dma_single_dir_e;

typedef enum {
    DMA_ADDR_LITTLE = 0,
    DMA_ADDR_BIG
} dma_addr_endian_e;

typedef enum {
    DMA_MODE_HARDWARE = 0,
    DMA_MODE_SOFTWARE
} dma_channel_req_mode_e;

typedef struct {
    dma_addr_inc_e         src_inc;        ///< source address increment
    dma_addr_inc_e         dst_inc;        ///< destination address increment
    dma_addr_endian_e      src_endian;     ///< source read data little-big endian change control.
    dma_addr_endian_e      dst_endian;     ///< destination write data little-big endian change control.
    uint8_t                src_tw;         ///< source transfer width in byte
    uint8_t                dst_tw;         ///< destination transfer width in byte
    uint8_t                hs_if;          ///< a hardware handshaking interface (optional).
    uint8_t                preemption;     ///< determine whether if a channel can be preempted by a higher priority channel, 0 -- not allow preempt, 1 -- allow preempt.
    dma_trans_type_e       type;           ///< transfer type
    dma_trig_trans_mode_e  mode;           ///< channel trigger mode
    dma_channel_req_mode_e ch_mode;        ///< software or hardware to tigger dma channel work.
    dma_single_dir_e       single_dir;     ///< after select single mode control for source(read) or destination(write) transfer.
    uint32_t               group_len;      ///< group transaction length (unit: bytes) when use DMA_GROUP_TRIGGER mode.
} dma_config_t;

typedef void (*dma_event_cb_t)(int32_t ch, dma_event_e event);   ///< Pointer to \ref dma_event_cb_t : dmac event call back.

/**
  \brief       Initialize DMA Interface. 1. Initializes the resources needed for the DMA interface 2.registers event callback function
  \param[in]   dmac idx
  \return      pointer to dma instances
*/
dmac_handle_t csi_dma_initialize(int32_t idx);

/**
  \brief       De-initialize DMA Interface. stops operation and releases the software resources used by the interface
  \param[in]   handle damc handle to operate.
  \return      error code
*/
int32_t csi_dma_uninitialize(dmac_handle_t handle);

/**
  \brief       Get driver capabilities.
  \param[in]   idx    dmac index.
  \return      \ref dma_capabilities_t
*/
dma_capabilities_t csi_dma_get_capabilities(int32_t idx);

/**
  \brief     get one free dma channel
  \param[in] handle damc handle to operate.
  \param[in] ch channel num. if -1 then allocate a free channal in this dma
  \return    -1 - no channel can be used, other - channel index
 */
int32_t csi_dma_alloc_channel(dmac_handle_t handle, int32_t ch);

/**
  \brief        release dma channel and related resources
  \param[in]    handle damc handle to operate.
  \param[in]    ch  channel num.
  \return       error code
 */
int32_t csi_dma_release_channel(dmac_handle_t handle, int32_t ch);

/**
  \brief        config dma channel
  \param[in]    handle damc handle to operate.
  \param[in]    ch          channel num.
  \param[in]    config      dma channel transfer configure
  \param[in]    cb_event    Pointer to \ref dma_event_cb_t
  \return       error code if negative, otherwise return the channel num if success.
 */
int32_t csi_dma_config_channel(dmac_handle_t handle, int32_t ch,
                               dma_config_t *config, dma_event_cb_t cb_event);

/**
  \brief       start generate dma channel signal.
  \param[in]   handle damc handle to operate.
  \param[in]   ch  channel num.
  \param[in]   psrcaddr    dma transfer source address
  \param[in]   pdstaddr    dma transfer destination address
  \param[in]   length      dma transfer length (unit: bytes).
  \return      error code
*/
int32_t csi_dma_start(dmac_handle_t handle, int32_t ch, void *psrcaddr,
                      void *pdstaddr, uint32_t length);

/**
  \brief       Stop generate dma channel signal.
  \param[in]   handle damc handle to operate.
  \param[in]   ch  channel num.
  \return      error code
*/
int32_t csi_dma_stop(dmac_handle_t handle, int32_t ch);

/**
  \brief       Get DMA channel status.
  \param[in]   handle damc handle to operate.
  \param[in]   ch  channel num.
  \return      DMA status \ref dma_status_e
*/
dma_status_e csi_dma_get_status(dmac_handle_t handle, int32_t ch);

#ifdef __cplusplus
}
#endif

#endif /* _CSI_DMA_H_ */

