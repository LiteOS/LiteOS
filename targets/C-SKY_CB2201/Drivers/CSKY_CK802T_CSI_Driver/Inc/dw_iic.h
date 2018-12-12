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
 * @file     dw_iic.h
 * @brief    header File for IIC Driver
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/
#ifndef __DW_IIC_H
#define __DW_IIC_H
#include "soc.h"

/*
 * Define the speed of I2C
 */
typedef enum {
    DW_IIC_STANDARDSPEED = 1,
    DW_IIC_FASTSPEED     = 2,
    DW_IIC_HIGHSPEED     = 3
} DWENUM_IIC_SPEED;

enum i2c_state_e {
    IIC_STATE_NONE = 0,          /* Send start + (first part of) address. */
    IIC_STATE_DATASEND,          /* Send data. */
    IIC_STATE_WFDATA,            /* Wait for data. */
    IIC_STATE_WFSTOPSENT,        /* Wait for STOP to have been transmitted. */
    IIC_STATE_DONE,              /* Transfer completed successfully. */
    IIC_STATE_SEND_DONE,         /* send completed successfully. */
    IIC_STATE_RECV_DONE,         /* receive completed successfully. */
    IIC_STATE_ERROR              /* Transfer error. */
};
/*
 * Define the interrupt type of I2C
 */
#define DW_IIC_INTR_RX_UNDER     0x001
#define DW_IIC_INTR_RX_OVER      0x002
#define DW_IIC_INTR_RX_FULL      0x004
#define DW_IIC_INTR_TX_OVER      0x008
#define DW_IIC_INTR_TX_EMPTY     0x010
#define DW_IIC_INTR_RD_REQ       0x020
#define DW_IIC_INTR_TX_ABRT      0x040
#define DW_IIC_INTR_RX_DONE      0x080
#define DW_IIC_INTR_ACTIVITY     0x100
#define DW_IIC_INTR_STOP_DET     0x200
#define DW_IIC_INTR_START_DET    0x400
#define DW_IIC_INTR_GEN_CALL     0x800

#define DW_IIC_INTR_DEFAULT_MASK     (DW_IIC_INTR_RX_FULL | \
                                      DW_IIC_INTR_TX_EMPTY | \
                                      DW_IIC_INTR_TX_ABRT | \
                                      DW_IIC_INTR_STOP_DET)

/*
 * I2C register bit definitions
 */
#define DW_IIC_DISABLE      0
#define DW_IIC_ENABLE       1
#define DW_IIC_FIFO_MAX_LV  0x8
#define DW_IIC_TXFIFO_LV    0x0
#define DW_IIC_RXFIFO_LV    0x0

#define DW_IIC_RXFIFO_FULL       (0x1 << 4)
#define DW_IIC_RXFIFO_NOT_EMPTY  (0x1 << 3)
#define DW_IIC_TXFIFO_EMPTY      (0x1 << 2)
#define DW_IIC_TXFIFO_NOT_FULL   (0x1 << 1)
#define DW_IIC_STATUS_ACTIVITY    0x1
#define DW_IIC_FIFO_RST_EN        0x01
#define DW_IIC_CON_DEFAUL         0x23

typedef struct {
    __IOM uint32_t IC_CON;                    /* Offset: 0x000 (R/W)  I2C Control */
    __IOM uint32_t IC_TAR;                    /* Offset: 0x004 (R/W)  I2C target address */
    __IOM uint32_t IC_SAR;                    /* Offset: 0x008 (R/W)  I2C slave address  */
    __IOM uint32_t IC_HS_MADDR;               /* Offset: 0x00c (R/W)  I2C HS Master Mode Code Address */
    __IOM uint32_t IC_DATA_CMD;               /* Offset: 0x010 (R/W)  I2C RX/TX Data Buffer and Command */
    __IOM uint32_t IC_SS_SCL_HCNT;            /* Offset: 0x014 (R/W)  Standard speed I2C Clock SCL High Count */
    __IOM uint32_t IC_SS_SCL_LCNT;            /* Offset: 0x018 (R/W)  Standard speed I2C Clock SCL Low Count */
    __IOM uint32_t IC_FS_SCL_HCNT;            /* Offset: 0x01c (R/W)  Fast speed I2C Clock SCL High Count */
    __IOM uint32_t IC_FS_SCL_LCNT;            /* Offset: 0x020 (R/W)  Fast speed I2C Clock SCL Low Count */
    __IOM uint32_t IC_HS_SCL_HCNT;            /* Offset: 0x024 (R/W)  High speed I2C Clock SCL High Count*/
    __IOM uint32_t IC_HS_SCL_LCNT;            /* Offset: 0x028 (R/W)  High speed I2C Clock SCL Low Count */
    __IM  uint32_t IC_INTR_STAT;              /* Offset: 0x02c (R)    I2C Interrupt Status */
    __IOM uint32_t IC_INTR_MASK;              /* Offset: 0x030 (R/W)  I2C Interrupt Mask */
    __IM  uint32_t IC_RAW_INTR_STAT;          /* Offset: 0x034 (R)    I2C Raw Interrupt Status */
    __IOM uint32_t IC_RX_TL;                  /* Offset: 0x038 (R/W)  I2C Receive FIFO Threshold */
    __IOM uint32_t IC_TX_TL;                  /* Offset: 0x03c (R/W)  I2C Transmit FIFO Threshold */
    __IM  uint32_t IC_CLR_INTR;               /* Offset: 0x040 (R)    Clear combined and individual interrupts*/
    __IM  uint32_t IC_CLR_RX_UNDER;           /* Offset: 0x044 (R)    I2C Clear RX_UNDER interrupt  */
    __IM  uint32_t IC_CLR_RX_OVER;            /* Offset: 0x048 (R)    I2C Clear RX_OVER interrupt  */
    __IM  uint32_t IC_CLR_TX_OVER;            /* Offset: 0x04c (R)    I2C Clear TX_OVER interrupt  */
    __IM  uint32_t IC_CLR_RD_REQ;             /* Offset: 0x050 (R)    I2C Clear RD_REQ interrupt  */
    __IM  uint32_t IC_CLR_TX_ABRT;            /* Offset: 0x054 (R)    I2C Clear TX_ABRT interrupt  */
    __IM  uint32_t IC_CLR_RX_DONE;            /* Offset: 0x058 (R)    I2C Clear RX_DONE interrupt  */
    __IM  uint32_t IC_CLR_ACTIVITY;           /* Offset: 0x05c (R)    I2C Clear ACTIVITY interrupt  */
    __IM  uint32_t IC_CLR_STOP_DET;           /* Offset: 0x060 (R)    I2C Clear STOP_DET interrupt  */
    __IM  uint32_t IC_CLR_START_DET;          /* Offset: 0x064 (R)    I2C Clear START_DET interrupt  */
    __IM  uint32_t IC_CLR_GEN_CALL;           /* Offset: 0x068 (R)    I2C Clear GEN_CAL interrupt  */
    __IOM uint32_t IC_ENABLE;                 /* Offset: 0x06c (R/W)  I2C enable */
    __IM  uint32_t IC_STATUS;                 /* Offset: 0x070 (R)    I2C status register */
    __IM  uint32_t IC_TXFLR;                  /* Offset: 0x074 (R)    Transmit FIFO Level register */
    __IM  uint32_t IC_RXFLR;                  /* Offset: 0x078 (R)    Receive FIFO Level Register */
    uint32_t RESERVED;                  /* Offset: 0x07c (R/ )   */
    __IOM uint32_t IC_TX_ABRT_SOURCE;         /* Offset: 0x080 (R/W)  I2C Transmit Abort Status Register */
    __IOM uint32_t IC_SAR1;                   /* Offset: 0x084 (R/W)  I2C Slave Address1 */
    __IOM uint32_t IC_DMA_CR;                 /* Offset: 0x088 (R/W)  DMA Control Register for transmit and receive handshaking interface  */
    __IOM uint32_t IC_DMA_TDLR;               /* Offset: 0x08c (R/W)  DMA Transmit Data Level */
    __IOM uint32_t IC_DMA_RDLR;               /* Offset: 0x090 (R/W)  DMA Receive Data Level */
    __IOM uint32_t IC_SAR2;                   /* Offset: 0x094 (R/W)  I2C Slave Address2 */
    __IOM uint32_t IC_SAR3;                   /* Offset: 0x098 (R/W)  I2C Slave Address3 */
    __IOM uint32_t IC_MULTI_SLAVE;            /* Offset: 0x09c (R/W)  I2C address number in slave mode */
    __IOM uint32_t IC_GEN_CALL_EN;            /* Offset: 0x0a0 (R/W)  I2C general call mask register when I2C is in the slave mode */
    __IOM uint32_t IC_FIFO_RST_EN;            /* Offset: 0x0a4 (R/W)  I2C FIFO flush register when I2C is in the slave transfer mode*/
} dw_iic_reg_t;

#endif /* __DW_IIC_H */
