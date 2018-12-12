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
 * @file     dw_iic.c
 * @brief    CSI Source File for IIC Driver
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/

#include <csi_config.h>
#include "drv_iic.h"
#include "dw_iic.h"
#include "soc.h"
#include "csi_core.h"
#include "string.h"
#include "stdio.h"

#define ERR_IIC(errno) (CSI_DRV_ERRNO_IIC_BASE | errno)

#define IIC_NULL_PARAM_CHK(para)                  \
    do {                                        \
        if (para == NULL) {                     \
            return ERR_IIC(DRV_ERROR_PARAMETER);   \
        }                                       \
    } while (0)

typedef struct  {
    uint32_t base;
    uint32_t irq;
    iic_event_cb_t cb_event;
    uint32_t rx_total_num;
    uint32_t tx_total_num;
    uint8_t *rx_buf;
    uint8_t *tx_buf;
    volatile uint32_t rx_cnt;
    volatile uint32_t tx_cnt;
    uint32_t status;             ///< status of iic transfer
} dw_iic_priv_t;

extern int32_t target_iic_init(int32_t idx, uint32_t *base, uint32_t *irq);

static dw_iic_priv_t iic_instance[CONFIG_IIC_NUM];

static const iic_capabilities_t iic_capabilities = {
    .address_10_bit = 0  /* supports 10-bit addressing */
};

static inline void dw_iic_disable(dw_iic_reg_t *addr)
{
    /* First clear ACTIVITY, then Disable IIC */
    addr->IC_CLR_ACTIVITY;
    addr->IC_ENABLE = DW_IIC_DISABLE;

}

static inline void dw_iic_enable(dw_iic_reg_t *addr)
{
    addr->IC_ENABLE = DW_IIC_ENABLE;

}

static inline void dw_iic_set_transfer_speed(dw_iic_reg_t *addr, DWENUM_IIC_SPEED speed)
{
    uint16_t temp = addr->IC_CON;
    temp &= ~((1 << 1) + (1 << 2));
    temp |= speed << 1;
    addr->IC_CON = temp;
}

static inline void dw_iic_set_target_address(dw_iic_reg_t *addr, uint16_t address)
{
    uint16_t temp = addr->IC_TAR;
    temp &= 0xfc00;
    temp |= address;
    addr->IC_TAR = temp;
}
static inline void dw_iic_set_addr_mode(dw_iic_reg_t *addr, iic_address_mode_e addr_mode)
{
    uint16_t temp = addr->IC_TAR;
    temp &= 0xefff;
    temp |= addr_mode << 12;
    addr->IC_TAR = temp;
}

static uint32_t dw_iic_read_clear_intrbits(dw_iic_reg_t *addr)
{
    uint32_t  stat = 0;

    stat = addr->IC_INTR_STAT;

    if (stat & DW_IIC_INTR_RX_UNDER) {
        addr->IC_CLR_RX_UNDER;
    }

    if (stat & DW_IIC_INTR_RX_OVER) {
        addr->IC_CLR_RX_OVER;
    }

    if (stat & DW_IIC_INTR_TX_OVER) {
        addr->IC_CLR_TX_OVER;
    }

    if (stat & DW_IIC_INTR_RD_REQ) {
        addr->IC_CLR_RD_REQ;
    }

    if (stat & DW_IIC_INTR_TX_ABRT) {
        addr->IC_TX_ABRT_SOURCE;
    }

    if (stat & DW_IIC_INTR_RX_DONE) {
        addr->IC_CLR_RX_DONE;
    }

    if (stat & DW_IIC_INTR_ACTIVITY) {
        addr->IC_CLR_ACTIVITY;
    }

    if (stat & DW_IIC_INTR_STOP_DET) {
        addr->IC_CLR_STOP_DET;
    }

    if (stat & DW_IIC_INTR_START_DET) {
        addr->IC_CLR_START_DET;
    }

    if (stat & DW_IIC_INTR_GEN_CALL) {
        addr->IC_CLR_GEN_CALL;
    }

    return stat;
}

/**
  \brief       interrupt service function for transmit FIFO empty interrupt.
  \param[in]   iic_priv pointer to iic private.
*/
static void dw_iic_intr_tx_empty(int32_t idx, dw_iic_priv_t *iic_priv, uint32_t intr_stat)
{
    dw_iic_reg_t *addr = (dw_iic_reg_t *)(iic_priv->base);

    if (intr_stat & DW_IIC_INTR_TX_EMPTY) {

        uint32_t remain_txfifo = iic_priv->tx_total_num - iic_priv->tx_cnt;
        uint8_t emptyfifo = (remain_txfifo > (DW_IIC_FIFO_MAX_LV - addr->IC_TXFLR)) ? DW_IIC_FIFO_MAX_LV - addr->IC_TXFLR : remain_txfifo;
        uint32_t i = 0u;

        for (i = 0; i < emptyfifo; i++) {
            addr->IC_DATA_CMD = *iic_priv->tx_buf++;
        }


        iic_priv->tx_cnt += emptyfifo;

        if (iic_priv->tx_cnt == iic_priv->tx_total_num) {
            addr->IC_INTR_MASK &= ~DW_IIC_INTR_TX_EMPTY;
        }
    }

    if (intr_stat & DW_IIC_INTR_TX_OVER) {
        iic_priv->status = IIC_STATE_ERROR;
        dw_iic_disable(addr);
        addr->IC_FIFO_RST_EN = DW_IIC_FIFO_RST_EN;

        if (iic_priv->cb_event) {
            iic_priv->cb_event(idx, IIC_EVENT_BUS_ERROR);
        }
    }

    if (intr_stat & DW_IIC_INTR_STOP_DET) {
        iic_priv->status  = IIC_STATE_SEND_DONE;

        if (iic_priv->tx_cnt != iic_priv->tx_total_num) {
            addr->IC_INTR_MASK &= ~DW_IIC_INTR_TX_EMPTY;
        }

        dw_iic_disable(addr);
        addr->IC_FIFO_RST_EN = DW_IIC_FIFO_RST_EN;

        if (iic_priv->cb_event) {
            iic_priv->cb_event(idx, IIC_EVENT_TRANSFER_DONE);
        }
    }

}
/**
  \brief       interrupt service function for receive FIFO full interrupt .
  \param[in]   iic_priv pointer to iic private.
*/
static void dw_iic_intr_rx_full(int32_t idx, dw_iic_priv_t *iic_priv, uint32_t intr_stat)
{
    dw_iic_reg_t *addr = (dw_iic_reg_t *)(iic_priv->base);

    if (intr_stat & DW_IIC_INTR_RX_FULL) {
        *iic_priv->rx_buf++ = ((addr->IC_DATA_CMD) & 0xff);

        iic_priv->rx_cnt++;;

        if (iic_priv->rx_cnt != iic_priv->rx_total_num) {
            addr->IC_DATA_CMD = 1 << 8;

        }
    }

    if (intr_stat & DW_IIC_INTR_RX_OVER) {
        iic_priv->status = IIC_STATE_ERROR;
        addr->IC_DATA_CMD = 0;
        dw_iic_disable(addr);

        if (iic_priv->cb_event) {
            iic_priv->cb_event(idx, IIC_EVENT_BUS_ERROR);
        }
    }

    if (intr_stat & DW_IIC_INTR_STOP_DET) {
        if (iic_priv->rx_cnt == iic_priv->rx_total_num) {
            iic_priv->status  = IIC_STATE_RECV_DONE;
            addr->IC_INTR_MASK = 0;
            dw_iic_disable(addr);

            if (iic_priv->cb_event) {
                iic_priv->cb_event(idx, IIC_EVENT_TRANSFER_DONE);
            }
        }
    }

}
void dw_iic_irqhandler(int32_t idx)
{
    dw_iic_priv_t *iic_priv = &iic_instance[idx];
    dw_iic_reg_t *addr = (dw_iic_reg_t *)(iic_priv->base);

    uint32_t  intr_stat = dw_iic_read_clear_intrbits(addr);
	printf("[info]进入iic底层中断\n");
    if (intr_stat & DW_IIC_INTR_TX_ABRT) {
        /* If arbitration fault, it indicates either a slave device not
        * responding as expected, or other master which is not supported
        * by this SW.
        */
        iic_priv->status    = IIC_STATE_DONE;
        dw_iic_disable(addr);
        addr->IC_INTR_MASK = 0;
        addr->IC_FIFO_RST_EN = DW_IIC_FIFO_RST_EN;

        if (iic_priv->cb_event) {
            iic_priv->cb_event(idx, IIC_EVENT_BUS_ERROR);
            return;
        }
    }

    switch (iic_priv->status) {
            /* send data to slave */
        case IIC_STATE_DATASEND: {
            dw_iic_intr_tx_empty(idx, iic_priv, intr_stat);
            break;
        }

        /* wait for data from slave */
        case IIC_STATE_WFDATA: {
            dw_iic_intr_rx_full(idx, iic_priv, intr_stat);
            break;
        }

        /* unexpected state,SW fault */
        default: {
            addr->IC_FIFO_RST_EN = DW_IIC_FIFO_RST_EN;
            addr->IC_INTR_MASK = 0;
            dw_iic_disable(addr);

            if (iic_priv->cb_event) {
                iic_priv->cb_event(idx, IIC_EVENT_ARBITRATION_LOST);
            }
        }
    }
}

/**
  \brief       Initialize IIC Interface specified by pins. \n
               1. Initializes the resources needed for the IIC interface 2.registers event callback function
  \param[in]   idx iic index
  \param[in]   cb_event  Pointer to \ref iic_event_cb_t
  \return      0 for success, negative for error code
*/
iic_handle_t csi_iic_initialize(int32_t idx, iic_event_cb_t cb_event)
{
    uint32_t base = 0u;
    uint32_t irq = 0u;

    int32_t ret = target_iic_init(idx, &base, &irq);

    if (ret < 0 || ret >= CONFIG_IIC_NUM) {
        return NULL;
    }

    dw_iic_priv_t *iic_priv = &iic_instance[idx];
    iic_priv->base = base;
    iic_priv->irq  = irq;


    iic_priv->cb_event = cb_event;
    iic_priv->rx_total_num = 0;
    iic_priv->tx_total_num = 0;
    iic_priv->rx_buf = NULL;
    iic_priv->tx_buf = NULL;
    iic_priv->rx_cnt = 0;
    iic_priv->tx_cnt = 0;
    iic_priv->status = 0;

    dw_iic_reg_t *addr = (dw_iic_reg_t *)(iic_priv->base);

    /* mask all interrupts */
    addr->IC_INTR_MASK  = 0x00;
    addr->IC_CON        = DW_IIC_CON_DEFAUL;

    csi_vic_enable_irq(iic_priv->irq);

    return iic_priv;
}

/**
  \brief       De-initialize IIC Interface. stops operation and releases the software resources used by the interface
  \param[in]   handle  iic handle to operate.
  \return      error code
*/
int32_t csi_iic_uninitialize(iic_handle_t handle)
{
    IIC_NULL_PARAM_CHK(handle);

    /* First clear ACTIVITY, then Disable IIC */
    dw_iic_priv_t *iic_priv = handle;
    dw_iic_reg_t *addr = (dw_iic_reg_t *)(iic_priv->base);

    addr->IC_CLR_ACTIVITY;
    addr->IC_INTR_MASK  = 0x00;
    addr->IC_ENABLE = DW_IIC_DISABLE;

    iic_priv->cb_event = NULL;
    iic_priv->rx_total_num = 0;
    iic_priv->tx_total_num = 0;
    iic_priv->rx_buf = NULL;
    iic_priv->tx_buf = NULL;
    iic_priv->rx_cnt = 0;
    iic_priv->tx_cnt = 0;
    iic_priv->status = 0;

    csi_vic_disable_irq(iic_priv->irq);

    return 0;
}

/**
  \brief       Get driver capabilities.
  \param[in]   idx iic index.
  \return      \ref iic_capabilities_t
*/
iic_capabilities_t csi_iic_get_capabilities(int32_t idx)
{
    if (idx < 0 || idx >= CONFIG_SPI_NUM) {
        iic_capabilities_t ret;
        memset(&ret, 0, sizeof(iic_capabilities_t));
        return ret;
    }

    return iic_capabilities;
}

/**
  \brief       config iic.
  \param[in]   handle  iic handle to operate.
  \param[in]   mode      \ref iic_mode_e.if negative, then this attribute not changed
  \param[in]   speed     \ref iic_speed_e.if negative, then this attribute not changed
  \param[in]   addr_mode \ref iic_address_mode_e.if negative, then this attribute not changed
  \param[in]   slave_addr slave address.if negative, then this attribute not changed
  \return      error code
*/
int32_t csi_iic_config(iic_handle_t handle,
                       iic_mode_e mode,
                       iic_speed_e speed,
                       iic_address_mode_e addr_mode,
                       int32_t slave_addr)
{
    int32_t ret;

    ret = csi_iic_config_mode(handle, mode);

    if (ret < 0) {
        return ret;
    }

    ret = csi_iic_config_speed(handle, speed);

    if (ret < 0) {
        return ret;
    }

    ret = csi_iic_config_addr_mode(handle, addr_mode);

    if (ret < 0) {
        return ret;
    }

    ret = csi_iic_config_slave_addr(handle, slave_addr);

    if (ret < 0) {
        return ret;
    }

    return 0;
}


/**
  \brief       config iic mode.
  \param[in]   handle  iic handle to operate.
  \param[in]   mode      \ref iic_mode_e.if negative, then this attribute not changed
  \return      error code
*/
int32_t csi_iic_config_mode(iic_handle_t handle, iic_mode_e mode)
{
    IIC_NULL_PARAM_CHK(handle);

    if ((int32_t)mode < 0) {
        return 0;
    }

    switch (mode) {
        case IIC_MODE_MASTER:
            break;

        case IIC_MODE_SLAVE:
            return ERR_IIC(DRV_ERROR_UNSUPPORTED);
            break;

        default:
            return ERR_IIC(DRV_ERROR_PARAMETER);
    }

    return 0;
}

/**
  \brief       config iic speed.
  \param[in]   handle  iic handle to operate.
  \param[in]   speed     \ref iic_speed_e.if negative, then this attribute not changed
  \return      error code
*/
int32_t csi_iic_config_speed(iic_handle_t handle, iic_speed_e speed)
{
    IIC_NULL_PARAM_CHK(handle);

    if ((int32_t)speed < 0) {
        return 0;
    }

    dw_iic_reg_t *addr = (dw_iic_reg_t *)(((dw_iic_priv_t *)handle)->base);

    switch (speed) {
        case IIC_BUS_SPEED_STANDARD:
            dw_iic_set_transfer_speed(addr, DW_IIC_STANDARDSPEED);
            break;

        case IIC_BUS_SPEED_FAST:
            dw_iic_set_transfer_speed(addr, DW_IIC_FASTSPEED);
            break;

        case IIC_BUS_SPEED_FAST_PLUS:
            return ERR_IIC(DRV_ERROR_UNSUPPORTED);

        case IIC_BUS_SPEED_HIGH:
            dw_iic_set_transfer_speed(addr, DW_IIC_HIGHSPEED);
            break;

        default:
            return ERR_IIC(DRV_ERROR_PARAMETER);
    }

    return 0;
}

/**
  \brief       config iic address mode.
  \param[in]   handle  iic handle to operate.
  \param[in]   addr_mode \ref iic_address_mode_e.if negative, then this attribute not changed
  \return      error code
*/
int32_t csi_iic_config_addr_mode(iic_handle_t handle, iic_address_mode_e addr_mode)
{
    IIC_NULL_PARAM_CHK(handle);

    if ((int32_t)addr_mode < 0) {
        return 0;
    }

    dw_iic_reg_t *addr = (dw_iic_reg_t *)(((dw_iic_priv_t *)handle)->base);

    switch (addr_mode) {
        case IIC_ADDRESS_10BIT:
        case IIC_ADDRESS_7BIT:
            dw_iic_set_addr_mode(addr, addr_mode);
            break;

        default:
            return ERR_IIC(DRV_ERROR_PARAMETER);
    }

    return 0;
}

/**
  \brief       config iic slave address.
  \param[in]   handle  iic handle to operate.
  \param[in]   slave_addr slave address.if negative, then this attribute not changed
  \return      error code
*/
int32_t csi_iic_config_slave_addr(iic_handle_t handle, int32_t slave_addr)
{
    IIC_NULL_PARAM_CHK(handle);

    if (slave_addr < 0) {
        return 0;
    }

    dw_iic_reg_t *addr = (dw_iic_reg_t *)(((dw_iic_priv_t *)handle)->base);

    dw_iic_set_target_address(addr, slave_addr);
    return 0;
}

/**
  \brief       Start transmitting data as I2C Master.
               This function is non-blocking,\ref iic_event_e is signaled when transfer completes or error happens.
               \ref csi_iic_get_status can indicates transmission status.
  \param[in]   handle         iic handle to operate.
  \param[in]   devaddr        iic addrress of slave device.
  \param[in]   data           data to send to I2C Slave
  \param[in]   num            Number of data items to send
  \param[in]   xfer_pending   Transfer operation is pending - Stop condition will not be generated
  \return      0 for success, negative for error code
*/
int32_t csi_iic_master_send(iic_handle_t handle, uint32_t devaddr, const void *data, uint32_t num, bool xfer_pending)
{
    IIC_NULL_PARAM_CHK(handle);

    if (data == NULL || num == 0) {
        return ERR_IIC(DRV_ERROR_PARAMETER);
    }

    dw_iic_priv_t *iic_priv = handle;
    dw_iic_reg_t *addr = (dw_iic_reg_t *)(iic_priv->base);

    iic_priv->tx_buf          = (uint8_t *)data;
    iic_priv->tx_total_num    = num;
    iic_priv->tx_cnt          = 0;
    iic_priv->status          = IIC_STATE_DATASEND;

    dw_iic_disable(addr);

    addr->IC_CLR_INTR;
    addr->IC_TX_TL = DW_IIC_TXFIFO_LV;

    /* open corresponding interrupts */
    addr->IC_INTR_MASK = DW_IIC_INTR_DEFAULT_MASK;
    dw_iic_enable(addr);

    return 0;

}

/**
  \brief       Start receiving data as I2C Master.
               This function is non-blocking,\ref iic_event_e is signaled when transfer completes or error happens.
               \ref csi_iic_get_status can indicates transmission status.
  \param[in]   handle  iic handle to operate.
  \param[in]   devaddr        iic addrress of slave device.
  \param[out]  data    Pointer to buffer for data to receive from IIC receiver
  \param[in]   num     Number of data items to receive
  \param[in]   xfer_pending   Transfer operation is pending - Stop condition will not be generated
  \return      0 for success, negative for error code
*/
int32_t csi_iic_master_receive(iic_handle_t handle, uint32_t devaddr, void *data, uint32_t num, bool xfer_pending)
{
    IIC_NULL_PARAM_CHK(handle);

    if (data == NULL || num == 0) {
        return ERR_IIC(DRV_ERROR_PARAMETER);
    }

    dw_iic_priv_t *iic_priv = handle;

    iic_priv->rx_buf          = (uint8_t *)data;
    iic_priv->rx_total_num      = num;
    iic_priv->rx_cnt          = 0;
    iic_priv->status          = IIC_STATE_WFDATA;

    dw_iic_reg_t *addr = (dw_iic_reg_t *)(iic_priv->base);
    dw_iic_disable(addr);

    addr->IC_CLR_INTR;
    addr->IC_RX_TL        = DW_IIC_RXFIFO_LV;    /* Sets receive FIFO threshold */
    dw_iic_enable(addr);
    addr->IC_DATA_CMD = 1 << 8;

    return 0;

}

/**
  \brief       Start transmitting data as I2C Slave.
  \param[in]   handle  iic handle to operate.
  \param[in]   data  Pointer to buffer with data to transmit to I2C Master
  \param[in]   num   Number of data items to send
  \return      error code
*/
int32_t csi_iic_slave_send(iic_handle_t handle, const void *data, uint32_t num)
{
    return ERR_IIC(DRV_ERROR_UNSUPPORTED);
}

/**
  \fn          int32_t  csi_iic_slave_receive (iic_handle_t handle, const void *data, uint32_t num)
  \brief       Start receiving data as I2C Slave.
  \param[in]   handle  iic handle to operate.
  \param[out]  data  Pointer to buffer for data to receive from I2C Master
  \param[in]   num   Number of data items to receive
  \return      error code
*/
int32_t csi_iic_slave_receive(iic_handle_t handle, void *data, uint32_t num)
{
    return ERR_IIC(DRV_ERROR_UNSUPPORTED);
}

/**
  \brief       abort transfer.
  \param[in]   handle  iic handle to operate.
  \return      error code
*/
int32_t csi_iic_abort_transfer(iic_handle_t handle)
{
    IIC_NULL_PARAM_CHK(handle);

    dw_iic_priv_t *iic_priv = handle;
    dw_iic_reg_t *addr = (dw_iic_reg_t *)(iic_priv->base);

    dw_iic_disable(addr);

    if (iic_priv->status == IIC_STATE_DATASEND) {
        addr->IC_FIFO_RST_EN = DW_IIC_FIFO_RST_EN;
    }

    iic_priv->rx_cnt          = 0;
    iic_priv->tx_cnt          = 0;
    iic_priv->rx_buf          = NULL;
    iic_priv->tx_buf          = NULL;
    return 0;
}


/**
  \brief       Get IIC status.
  \param[in]   handle  iic handle to operate.
  \return      IIC status \ref iic_status_t
*/
iic_status_t csi_iic_get_status(iic_handle_t handle)
{
    iic_status_t iic_status = {0};

    if (handle == NULL) {
        return iic_status;
    }

    dw_iic_priv_t *iic_priv = handle;
    dw_iic_reg_t *addr = (dw_iic_reg_t *)(iic_priv->base);


    if ((iic_priv->status == IIC_STATE_DATASEND) || (iic_priv->status == IIC_STATE_WFDATA)) {
        iic_status.busy = 1;
    }

    if (iic_priv->status == IIC_STATE_WFDATA) {
        iic_status.direction = 1;
    }

    if (addr->IC_RAW_INTR_STAT & 0x800) {
        iic_status.general_call = 1;
    }

    if (iic_priv->status == IIC_STATE_ERROR) {
        iic_status.bus_error = 1;
    }

    return iic_status;
}

/**
  \brief       Get IIC transferred data count.
  \param[in]   handle  iic handle to operate.
  \return      number of data bytes transferred
*/
uint32_t csi_iic_get_data_count(iic_handle_t handle)
{
    uint32_t cnt = 0;

    if (handle == NULL) {
        return 0;
    }

    dw_iic_priv_t *iic_priv = handle;

    if ((iic_priv->status == IIC_STATE_WFDATA) || (iic_priv->status == IIC_STATE_RECV_DONE)) {
        cnt = iic_priv->rx_cnt;
    } else if ((iic_priv->status == IIC_STATE_DATASEND) || (iic_priv->status == IIC_STATE_SEND_DONE)) {
        cnt = iic_priv->tx_cnt;
    }

    return cnt;
}

/**
  \brief       control IIC power.
  \param[in]   handle  iic handle to operate.
  \param[in]   state   power state.\ref csi_power_stat_e.
  \return      error code
*/
int32_t csi_iic_power_control(iic_handle_t handle, csi_power_stat_e state)
{
    /* return ERR_IIC(DRV_ERROR_UNSUPPORTED); */
    return 0; //FIXME: for xxxxx validation
}
/**
  \brief       Send START command.
  \param[in]   handle  iic handle to operate.
  \return      error code
*/
int32_t csi_iic_send_start(iic_handle_t handle)
{
    return ERR_IIC(DRV_ERROR_UNSUPPORTED);
}

/**
  \brief       Send STOP command.
  \param[in]   handle  iic handle to operate.
  \return      error code
*/
int32_t csi_iic_send_stop(iic_handle_t handle)
{
    return ERR_IIC(DRV_ERROR_UNSUPPORTED);
}

/**
  \brief       Reset I2C peripheral.
  \param[in]   handle  iic handle to operate.
  \return      error code
  \note The action here. Most of the implementation sends stop.
*/
int32_t csi_iic_reset(iic_handle_t handle)
{
    return csi_iic_send_stop(handle);
}

