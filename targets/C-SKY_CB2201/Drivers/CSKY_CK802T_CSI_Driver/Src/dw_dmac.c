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
* @file     dw_dmac.c
* @brief    CSI Source File for DMAC Driver
* @version  V1.0
* @date     02. June 2017
******************************************************************************/
#include <stdbool.h>
#include <csi_config.h>
#include "dw_dmac.h"
#include "csi_core.h"
#include "drv_dmac.h"
#include "soc.h"
#include <string.h>

#define ERR_DMA(errno) (CSI_DRV_ERRNO_DMA_BASE | errno)

typedef struct {
    uint32_t base;
    uint32_t control_base;
    uint32_t irq;
    dma_event_cb_t cb_event;
    uint8_t ch_num;
    uint8_t ch_opened[CK_DMA_MAXCHANNEL];
    dma_status_e status[CK_DMA_MAXCHANNEL];
    uint32_t src_tw;
    uint32_t dst_tw;
} dw_dma_priv_t;

static dw_dma_priv_t dma_instance[CONFIG_DMAC_NUM];

static const dma_capabilities_t dma_capabilities = {
    .unalign_addr = 1,          ///< support for unalign address transfer when memory is source
};

#define readl(addr) \
    *((volatile uint32_t *)(addr))

#define writel(b, addr) \
    *((volatile uint32_t *)(addr)) = b

static int32_t dw_dma_set_channel(uint32_t addr, uint8_t ch, uint32_t source, uint32_t dest, uint32_t size)
{
    writel(size, addr + ch * 0x58 + DMA_REG_CTRLbx);
    writel(source, addr + ch * 0x58 + DMA_REG_SARx);
    writel(dest, addr + ch * 0x58 + DMA_REG_DARx);

    return 0;
}

static int32_t dw_dma_set_transfertype(uint32_t addr, uint8_t ch, dma_trans_type_e transtype)
{
    if (transtype >= DMA_PERH2PERH) {
        return ERR_DMA(DRV_ERROR_PARAMETER);
    }

    uint32_t value = readl(addr + ch * 0x58 + DMA_REG_CTRLax);
    value &= ~(0x300000);
    value |= transtype << 20;
    writel(value, addr + ch * 0x58 + DMA_REG_CTRLax);

    return 0;
}

static int32_t dw_dma_set_addrinc(uint32_t addr, uint8_t ch, enum_addr_state_e src_addrinc, enum_addr_state_e dst_addrinc)
{
    if ((src_addrinc != DMA_ADDR_INCREMENT && src_addrinc != DMA_ADDR_DECREMENT && src_addrinc != DMA_ADDR_NOCHANGE) ||
        (dst_addrinc != DMA_ADDR_INCREMENT && dst_addrinc != DMA_ADDR_DECREMENT && dst_addrinc != DMA_ADDR_NOCHANGE)) {
        return ERR_DMA(DRV_ERROR_PARAMETER);
    }

    uint32_t value = readl(addr + ch * 0x58 + DMA_REG_CTRLax);
    value &= ~(0x780);
    value |= (src_addrinc << 9);
    value |= (dst_addrinc << 7);
    writel(value, addr + ch * 0x58 + DMA_REG_CTRLax);

    return 0;
}

static int32_t dw_dma_set_transferwidth(uint32_t addr, uint8_t ch, dma_datawidth_e src_width, dma_datawidth_e dst_width)
{
    if ((src_width != DMA_DATAWIDTH_SIZE8 && src_width != DMA_DATAWIDTH_SIZE16 && src_width != DMA_DATAWIDTH_SIZE32) ||
        (dst_width != DMA_DATAWIDTH_SIZE8 && dst_width != DMA_DATAWIDTH_SIZE16 && dst_width != DMA_DATAWIDTH_SIZE32)) {
        return ERR_DMA(DRV_ERROR_PARAMETER);
    }

    uint32_t value = readl(addr + ch * 0x58 + DMA_REG_CTRLax);
    value &= ~(0x7e);

    if (src_width == DMA_DATAWIDTH_SIZE32) {
        value |= (src_width - 2) << 4;
    } else {
        value |= (src_width - 1) << 4;
    }

    if (dst_width == DMA_DATAWIDTH_SIZE32) {
        value |= (dst_width - 2) << 1;
    } else {
        value |= (dst_width - 1) << 1;
    }

    writel(value, addr + ch * 0x58 + DMA_REG_CTRLax);

    return 0;
}

static int32_t dw_dma_set_burstlength(uint32_t addr, uint8_t ch, uint8_t burstlength)
{
    uint32_t value = readl(addr + ch * 0x58 + DMA_REG_CTRLax);
    value &= ~(0x1f800);
    value |= burstlength << 11 | burstlength << 14;
    writel(value, addr + ch * 0x58 + DMA_REG_CTRLax);

    return 0;

}

/**
  \brief       Set software or hardware handshaking.
  \param[in]   addr pointer to dma register.
  \return      error code
*/
static int32_t dw_dma_set_handshaking(uint32_t addr, uint8_t ch, dma_handshaking_select_e src_handshaking, dma_handshaking_select_e dst_handshaking)
{
    uint32_t value = readl(addr + ch * 0x58 + DMA_REG_CFGax);
    value &= ~(0xc00);
    value |= (src_handshaking << 11  | dst_handshaking << 10);
    writel(value, addr + ch * 0x58 + DMA_REG_CFGax);

    return 0;
}

static int dw_dma_assign_hdhs_interface(uint32_t addr, uint8_t ch, dwenum_dma_device_e src_device, dwenum_dma_device_e dst_device)
{
    if (src_device < 0 || src_device >= DWENUM_DMA_MEMORY || dst_device < 0 || dst_device >= DWENUM_DMA_MEMORY) {
        return ERR_DMA(DRV_ERROR_PARAMETER);
    }

    uint32_t value = readl(addr + ch * 0x58 + DMA_REG_CFGbx);
    value &= ~(0x7f80);
    value |= (src_device << 7 | dst_device << 11);
    writel(value, addr + ch * 0x58 + DMA_REG_CFGbx);

    return 0;
}


void dw_dmac_irqhandler(int32_t idx)
{
	printf("[info]进入DMA底层中断服务函数\n");
    dw_dma_priv_t *dma_priv = &dma_instance[idx];
    uint32_t addr = dma_priv->base;

    /*
     * StatusInt_temp contain the information that which types of interrupr are
     * requested.
     */
    int32_t count = 0;
    uint32_t temp = 0;

    temp = readl(addr + DMA_REG_StatusTfr);

    if (temp) {
        for (count = 0; count < dma_priv->ch_num; count++) {
            if (temp == (1 << count)) {
                break;
            }
        }

        dma_priv->status[count] = DMA_STATE_DONE;
        writel(temp, addr + DMA_REG_ClearTfr);

        if (dma_priv->cb_event) {
            dma_priv->cb_event(count, DMA_EVENT_TRANSFER_DONE);
        }

        return;
    }

    temp = readl(addr + DMA_REG_StatusErr);

    if (temp) {
        for (count = 0; count < dma_priv->ch_num; count++) {
            if (temp == (1 << count)) {
                break;
            }
        }

        dma_priv->status[count] = DMA_STATE_ERROR;
        writel(temp, addr + DMA_REG_ClearTfr);

        if (dma_priv->cb_event) {
            dma_priv->cb_event(count, DMA_EVENT_TRANSFER_ERROR);
        }

        return;
    }
}

int32_t __attribute__((weak)) target_get_dmac(uint32_t idx, uint32_t *base, uint32_t *irq)
{
    return NULL;
}

/**
  \brief       Initialize DMA Interface. 1. Initializes the resources needed for the DMA interface 2.registers event callbadw function
  \param[in]   dmac idx
  \return      pointer to dma instances
*/
dmac_handle_t csi_dma_initialize(int32_t idx)
{

    if (idx < 0 || idx >= CONFIG_DMAC_NUM) {
        return NULL;
    }

    uint32_t base = 0u;
    uint32_t irq = 0u;

    int32_t real_idx = target_get_dmac(idx, &base, &irq);

    if (real_idx != idx) {
        return NULL;
    }

    dw_dma_priv_t *dma_priv = &dma_instance[idx];

    dma_priv->base = base;
    dma_priv->irq  = irq;
    dma_priv->ch_num = CK_DMA_MAXCHANNEL;
    csi_vic_enable_irq(dma_priv->irq);

    writel(CK_DMA_MASK, base + DMA_REG_MaskTfr);
    writel(CK_DMA_MASK, base + DMA_REG_MaskErr);
    writel(CK_DMA_MASK, base + DMA_REG_MaskBlock);
    writel(CK_DMA_MASK, base + DMA_REG_MaskSrcTran);
    writel(CK_DMA_MASK, base + DMA_REG_MaskDstTran);
    writel(CK_DMA_INTC, base + DMA_REG_ClearTfr);
    writel(CK_DMA_INTC, base + DMA_REG_ClearBlock);
    writel(CK_DMA_INTC, base + DMA_REG_ClearErr);
    writel(CK_DMA_INTC, base + DMA_REG_ClearSrcTran);
    writel(CK_DMA_INTC, base + DMA_REG_ClearDstTran);

    return (dmac_handle_t)dma_priv;
}

/**
  \brief       De-initialize DMA Interface. stops operation and releases the software resources used by the interface
  \param[in]   handle damc handle to operate.
  \return      error code
*/
int32_t csi_dma_uninitialize(dmac_handle_t handle)
{
    if (handle == NULL) {
        return ERR_DMA(DRV_ERROR_PARAMETER);
    }

    dw_dma_priv_t *dma_priv = handle;
    uint32_t addr = dma_priv->base;

    writel(CK_DMA_MASK, addr + DMA_REG_MaskTfr);
    writel(CK_DMA_MASK, addr + DMA_REG_MaskErr);
    writel(CK_DMA_MASK, addr + DMA_REG_MaskBlock);
    writel(CK_DMA_MASK, addr + DMA_REG_MaskSrcTran);
    writel(CK_DMA_MASK, addr + DMA_REG_MaskDstTran);
    writel(CK_DMA_INTC, addr + DMA_REG_ClearTfr);
    writel(CK_DMA_INTC, addr + DMA_REG_ClearBlock);
    writel(CK_DMA_INTC, addr + DMA_REG_ClearErr);
    writel(CK_DMA_INTC, addr + DMA_REG_ClearSrcTran);
    writel(CK_DMA_INTC, addr + DMA_REG_ClearDstTran);

    csi_vic_disable_irq(dma_priv->irq);

    return 0;
}

/**
  \brief       Get driver capabilities.
  \param[in]   idx    dmac index.
  \return      \ref dma_capabilities_t
*/
dma_capabilities_t csi_dma_get_capabilities(int32_t idx)
{
    if (idx > (CONFIG_DMAC_NUM - 1)) {
        dma_capabilities_t ret;
        memset(&ret, 0, sizeof(dma_capabilities_t));
        return ret;
    }

    return dma_capabilities;
}

/**
  \brief     get one free dma channel
  \param[in] handle damc handle to operate.
  \param[in] ch channel num. if -1 then allocate a free channal in this dma
  \return    -1 - no channel can be used, other - channel index
 */
int32_t csi_dma_alloc_channel(dmac_handle_t handle, int32_t ch)
{
    dw_dma_priv_t *dma_priv = handle;

    if (handle == NULL || ch > dma_priv->ch_num) {
        return ERR_DMA(DRV_ERROR_PARAMETER);
    }

    uint8_t ch_num = 0;

    if (ch == -1) {     // alloc a free channal
        for (ch_num = 0; ch_num < dma_priv->ch_num; ch_num++) {
            if (dma_priv->ch_opened[ch_num] != 0x1) {
                dma_priv->ch_opened[ch_num] = 1;
                break;
            }
        }

        if (ch_num >= dma_priv->ch_num) {
            return -1;
        }
    } else {    //alloc a fixed channel
        if (dma_priv->ch_opened[ch] == 0x1) {
            return ERR_DMA(DRV_ERROR_BUSY);
        }

        dma_priv->ch_opened[ch] = 1;
        ch_num = ch;
    }

    uint32_t addr = dma_priv->base;

    writel((1 << ch_num), addr + DMA_REG_ClearTfr);
    writel((1 << ch_num), addr + DMA_REG_ClearBlock);
    writel((1 << ch_num), addr + DMA_REG_ClearErr);
    writel((1 << ch_num), addr + DMA_REG_ClearSrcTran);
    writel((1 << ch_num), addr + DMA_REG_ClearDstTran);

    uint32_t value = 1 << ch_num | (1 << (ch_num + 8));
    writel(value, addr + DMA_REG_MaskTfr);
    writel(value, addr + DMA_REG_MaskErr);

    dma_priv->status[ch_num] = DMA_STATE_READY;

    return ch_num;
}

/**
  \brief        release dma channel and related resources
  \param[in]    handle damc handle to operate.
  \param[in]    ch  channel num.
  \return       error code
 */
int32_t csi_dma_release_channel(dmac_handle_t handle, int32_t ch)
{
    dw_dma_priv_t *dma_priv = handle;

    if (handle == NULL || ch >= dma_priv->ch_num || ch < 0) {
        return ERR_DMA(DRV_ERROR_PARAMETER);
    }

    uint32_t addr = dma_priv->base;
    dma_priv->status[ch] = DMA_STATE_FREE;
    dma_priv->ch_opened[ch] = 0;

    writel((1 << ch), addr + DMA_REG_ClearTfr);
    writel((1 << ch), addr + DMA_REG_ClearBlock);
    writel((1 << ch), addr + DMA_REG_ClearErr);
    writel((1 << ch), addr + DMA_REG_ClearSrcTran);
    writel((1 << ch), addr + DMA_REG_ClearDstTran);

    uint32_t value = (1 << (ch + 8));
    writel(value, addr + DMA_REG_MaskTfr);
    writel(value, addr + DMA_REG_MaskErr);
    return 0;
}

/**
  \brief        config dma channel
  \param[in]    handle damc handle to operate.
  \param[in]    ch          channel num.
  \param[in]    config      dma channel transfer configure
  \param[in]    cb_event    Pointer to \ref dma_event_cb_t
  \return       error code
 */
int32_t csi_dma_config_channel(dmac_handle_t handle, int32_t ch,
                               dma_config_t *config, dma_event_cb_t cb_event)
{
    dw_dma_priv_t *dma_priv = handle;

    if (handle == NULL || ch >= dma_priv->ch_num || config == NULL) {
        return ERR_DMA(DRV_ERROR_PARAMETER);
    }

    if (ch == -1) { //alloc a free channel
        ch = csi_dma_alloc_channel(handle, -1);

        if (ch < 0) {
            return ERR_DMA(DRV_ERROR_BUSY);
        }
    }

    if (dma_priv->ch_opened[ch] == 0) {
        return ERR_DMA(DRV_ERROR_BUSY);
    }

    dma_priv->cb_event = cb_event;

    uint32_t addr = dma_priv->base;

    /* Initializes corresponding channel registers */


    int32_t ret = dw_dma_set_transferwidth(addr, ch, config->src_tw, config->dst_tw);

    if (ret < 0) {
        return ret;
    }

    dma_priv->dst_tw = config->dst_tw;
    dma_priv->src_tw = config->src_tw;

//    int32_t grouplen = ((length * config->src_tw / config->dst_tw) - 1) % 16;

    ret = dw_dma_set_transfertype(addr, ch, config->type);

    if (ret < 0) {
        return ret;
    }

    if (config->type == DMA_MEM2MEM) {
        dw_dma_set_handshaking(addr, ch, DMA_HANDSHAKING_SOFTWARE, DMA_HANDSHAKING_SOFTWARE);
        ret = dw_dma_set_addrinc(addr, ch, config->src_inc, config->dst_inc);

        if (ret < 0) {
            return ret;
        }

    } else if (config->type == DMA_MEM2PERH) {
        dw_dma_set_handshaking(addr, ch, DMA_HANDSHAKING_SOFTWARE, DMA_HANDSHAKING_HARDWARE);
        ret = dw_dma_set_addrinc(addr, ch, config->src_inc, config->dst_inc);

        if (ret < 0) {
            return ret;
        }

        ret = dw_dma_assign_hdhs_interface(addr, ch, config->hs_if, config->hs_if);

        if (ret < 0) {
            return ret;
        }

    } else if (config->type == DMA_PERH2MEM) {
        dw_dma_set_handshaking(addr, ch, DMA_HANDSHAKING_HARDWARE, DMA_HANDSHAKING_SOFTWARE);
        ret = dw_dma_set_addrinc(addr, ch, config->src_inc, config->dst_inc);

        if (ret < 0) {
            return ret;
        }

        ret = dw_dma_assign_hdhs_interface(addr, ch, config->hs_if, config->hs_if);

        if (ret < 0) {
            return ret;
        }

    }

    writel(0x1, addr + DMA_REG_Cfg);
    dma_priv->status[ch] = DMA_STATE_READY;

    return ch;
}

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
                      void *pdstaddr, uint32_t length)
{
    dw_dma_priv_t *dma_priv = handle;

    if (handle == NULL || ch >= dma_priv->ch_num || ch < 0 || psrcaddr == NULL || pdstaddr == NULL) {
        return ERR_DMA(DRV_ERROR_PARAMETER);
    }

    dma_priv->status[ch] = DMA_STATE_BUSY;
    uint32_t addr = dma_priv->base;

    if ((length * dma_priv->src_tw) % dma_priv->dst_tw != 0) {
        return ERR_DMA(DRV_ERROR_PARAMETER);
    }

    uint8_t i;

    for (i = 7; i > 0; i--) {
        if (!((length * dma_priv->src_tw / dma_priv->dst_tw) % (2 << (i + 1)))) {
            break;
        }
    }

    int32_t grouplen = i;

    if (i == 0) {
        grouplen = 0;
    }

    dw_dma_set_burstlength(addr, ch, grouplen);
    dw_dma_set_channel(addr, ch, (uint32_t)psrcaddr, (uint32_t)pdstaddr, length);

    // interrupt enable
    uint32_t value = readl(addr + ch * 0x58 + DMA_REG_CTRLax);
    value |= CK_DMA_INT_EN;
    writel(value, addr + ch * 0x58 + DMA_REG_CTRLax);

    value = readl(addr + DMA_REG_ChEn);
    value |= (CK_DMA_CH_EN << (8 + ch)) | (CK_DMA_CH_EN << ch);
    writel(value, addr + DMA_REG_ChEn);

    return 0;
}

/**
  \brief       Stop generate dma channel signal.
  \param[in]   handle damc handle to operate.
  \param[in]   ch  channel num.
  \return      error code
*/
int32_t csi_dma_stop(dmac_handle_t handle, int32_t ch)
{
    if (handle == NULL) {
        return ERR_DMA(DRV_ERROR_PARAMETER);
    }

    dw_dma_priv_t *dma_priv = handle;

    if (ch >= dma_priv->ch_num || ch < 0) {
        return ERR_DMA(DRV_ERROR_PARAMETER);
    }

    dma_priv->status[ch] = DMA_STATE_DONE;

    uint32_t addr = dma_priv->base;
    // interrupt disable
    uint32_t value = readl(addr + ch * 0x58 + DMA_REG_CTRLax);
    value &= ~CK_DMA_INT_EN;
    writel(value, addr + ch * 0x58 + DMA_REG_CTRLax);

    value = readl(addr + DMA_REG_ChEn);
    value |= (CK_DMA_CH_EN << (8 + ch));
    value &= ~(CK_DMA_CH_EN << ch);
    writel(value, addr + DMA_REG_ChEn);
    return 0;
}

/**
  \brief       Get DMA channel status.
  \param[in]   handle damc handle to operate.
  \param[in]   ch  channel num.
  \return      DMA status \ref dma_status_t
*/
dma_status_e csi_dma_get_status(dmac_handle_t handle, int32_t ch)
{
    if (handle == NULL) {
        return ERR_DMA(DRV_ERROR_PARAMETER);
    }

    dw_dma_priv_t *dma_priv = handle;

    if (ch >= dma_priv->ch_num || ch < 0) {
        return ERR_DMA(DRV_ERROR_PARAMETER);
    }

    return dma_priv->status[ch];
}

