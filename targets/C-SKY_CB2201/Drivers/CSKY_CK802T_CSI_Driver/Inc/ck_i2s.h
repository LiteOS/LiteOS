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
 * @file     ck_i2s.h
 * @brief    header File for I2S Driver
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/
#ifndef _CK_I2S_H_
#define _CK_I2S_H_
#include "soc.h"

/* Bitfields in IIS_AUDIOEN */
#define AUDIOEN_IIS_EN      (1 << 0)    /* IIS enable */
#define AUDIOEN_SPDIF_EN    (1 << 1)    /* SPDIF enable */

/* Bitfields in IIS_FUNCMODE */
#define FUNCMODE_MODE_RX    (0 << 0)
#define FUNCMODE_MODE_TX    (1 << 0)
#define FUNCMODE_MODE_WEN   (1 << 1)    /* MODE write enable */

/* Bitfields in IIS_IISCNF_IN */
#define I2SCNF_IN_AUDFMT_I2S            (0 << 0)
#define I2SCNF_IN_AUDFMT_RIGHT_J        (1 << 0)
#define I2SCNF_IN_AUDFMT_LEFT_J         (2 << 0)
#define I2SCNF_IN_WS_POLARITY_NORMAL    (0 << 2)
#define I2SCNF_IN_WS_POLARITY_INVERTED  (1 << 2)
#define I2SCNF_IN_SAMPLE_SOURCE_VOICE   (1 << 4)
#define I2SCNF_IN_SLAVE         (0 << 8)
#define I2SCNF_IN_MASTER        (1 << 8)

/* Bitfields in I2S_IISCNF_OUT */
#define I2SCNF_OUT_SLAVE         (0 << 4)
#define I2SCNF_OUT_MASTER        (1 << 4)
#define I2SCNF_OUT_AUDFMT_I2S           (0 << 0)
#define I2SCNF_OUT_AUDFMT_RIGHT_J       (1 << 0)
#define I2SCNF_OUT_AUDFMT_LEFT_J        (2 << 0)

/* Bitfields in I2S_FSSTA */
#define FSSTA_RATE_SET_BY_USER  (0 << 0)    /* input rate is set by user */
#define FSSTA_RATE_AUTO_DETECT  (1 << 0)    /* input rate auto detected */
#define FSSTA_RES16_FIFO16      (0 << 1)
#define FSSTA_RES16_FIFO24      (1 << 1)
#define FSSTA_RES24_FIFO16      (2 << 1)
#define FSSTA_RES24_FIFO24      (3 << 1)
#define FSSTA_RES_MASK          0x3
#define FSSTA_RES_SHIFT         1
#define FSSTA_AFR(x)            ((x) << 4)    /* Audio Fundamental Rate */
#define FSSTA_AFR_MASK           0x3
#define FSSTA_ARS(x)            ((x) << 6)    /* Audio Rate Scale(I2S only) */
#define FSSTA_ARS_MASK          0x3s

/* Bitfields in IIS_IMR */
#define I2SIMR_RXFIM    (1 << 4)    /* Receive FIFO Full Interrupt Mask */
#define I2SIMR_RXIOM    (1 << 3)    /* Receive FIFO Overflow Interrupt Mask */
#define I2SIMR_RXUIM    (1 << 2)    /* Receive FIFO Underflow Interrupt Mask */
#define I2SIMR_TXOIM    (1 << 1)    /* Transmit FIFO Overflow Interrupt Mask */
#define I2SIMR_TXEM     (1 << 0)    /* ransmit FIFO Empty Interrupt Mask */

//#define I2SIMR_DEFAULT_MASK  (I2SIMR_RXIOM | I2SIMR_RXUIM | I2SIMR_TXOIM)
#define I2SIMR_DEFAULT_MASK  (I2SIMR_RXUIM | I2SIMR_TXOIM)
/* dmac */
#define  I2SDMACR_TX_ENABLE     (1 << 1)
#define  I2SDMACR_RX_ENABLE     (1 << 0)
/* Bitfields in IIS_FICR */
#define I2SFICR_RXFIC    (1 << 4)    /* clear Receive FIFO Full Interrupt status */
#define I2SFICR_RXIOC    (1 << 3)    /* clear Receive FIFO Overflow Interrupt status */
#define I2SFICR_RXUIC    (1 << 2)    /* clear Receive FIFO Underflow Interrupt status */
#define I2SFICR_TXOIC    (1 << 1)    /* clear Transmit FIFO Overflow Interrupt status */
#define I2SFICR_TXEC     (1 << 0)    /* clear ransmit FIFO Empty Interrupt status */

#define CK_I2S_FIFO_MAX_LV      0x18
/* Bitfields in IIS_SR */
#define I2SSR_TX_BUSY   (1 << 1)    /* IIS Tx Busy Flag. */
#define I2SSR_RX_BUSY   (1 << 0)    /* IIS Rx Busy Flag */

#define I2S_SRC_CLK LSP_DEFAULT_FREQ
typedef struct {

    __IOM uint32_t I2S_AUDIOEN;          /* Offset: 0x000h (R/W)  AUDIO_IO Enable Register */
    __IOM uint32_t I2S_FUNCMODE;         /* Offset: 0x004h (R/W)  AUDIO_IO function mode */
    __IOM uint32_t I2S_IISCNF_IN;        /* Offset: 0x008h (R/W)  IIS interface configuration in (on ARX side) */
    __IOM uint32_t I2S_FSSTA;            /* Offset: 0x00ch (R/W)  IIS ATX audio input control/state register */
    __IOM uint32_t I2S_IISCNF_OUT;       /* Offset: 0x010h (R/W)  IIS interface configuration in (on ATX side) */
    __IOM uint32_t I2S_FADTLR;           /* Offset: 0x014h (R/W)  IIS Fs auto detected Threshold level register */
    __IOM uint32_t I2S_SCCR;             /* Offset: 0x018h (R/W)  Sample compress control register */
    __IOM uint32_t I2S_TXFTLR;           /* Offset: 0x01ch (R/W)  Transmit FIFO Threshold Level */
    __IOM uint32_t I2S_RXFTLR;           /* Offset: 0x020h (R/W)  Receive FIFO Threshold Level */
    __IOM uint32_t I2S_TXFLR;            /* Offset: 0x024h (R/W)  Transmit FIFO Level Register */
    __IOM uint32_t I2S_RXFLR;            /* Offset: 0x028h (R/W)  Receive FIFO Level Register */
    __IOM uint32_t I2S_SR;               /* Offset: 0x02ch (R/W)  Status Register */
    __IOM uint32_t I2S_IMR;              /* Offset: 0x030h (R/W)  Interrupt Mask Register */
    __IOM uint32_t I2S_ISR;              /* Offset: 0x034h (R/W)  Interrupt Status Register */
    __IOM uint32_t I2S_RISR;             /* Offset: 0x038h (R/W)  Raw Interrupt Status Register */
    __IOM uint32_t I2S_FICR;             /* Offset: 0x03ch (R/W)  FIFO Interrupt Clear Register */
    uint32_t RESERVED1[3];
    __IOM uint32_t I2S_DMACR;            /* Offset: 0x04ch (R/W)  DMA Control Register */
    __IOM uint32_t I2S_DMATDLR;          /* Offset: 0x050h (R/W)  DMA Transmit Data Level */
    __IOM uint32_t I2S_DMARDLR;          /* Offset: 0x054h (R/W)  DMA Receive Data Level */
    uint32_t RESERVED2[2];
    __IOM uint32_t I2S_DR;               /* Offset: 0x060h (R/W)  Data Register */
    uint32_t RESERVED3[3];
    __IOM uint32_t I2S_SRCR;             /* Offset: 0x070h (R/W)  SPDIF Receiver Configuration register */
    __IOM uint32_t I2S_SRSSR;            /* Offset: 0x074h (R/W)  SPDIF Receiver Signal Status Register */
    __IOM uint32_t I2S_STSSR;            /* Offset: 0x078h (R/W)  SPDIF Transmitter Signal Status Register */
    __IOM uint32_t I2S_SCSR;             /* Offset: 0x07ch (R/W)  SPDIF Channel status Bits Register */
    __IOM uint32_t I2S_MIMR;             /* Offset: 0x080h (R/W)  Mode Interrupt Mask Register */
    __IOM uint32_t I2S_MISR;             /* Offset: 0x084h (R/W)  Raw Mode Interrupt Status Register */
    __IOM uint32_t I2S_RMISR;            /* Offset: 0x088h (R/W)  Mode Interrupt Status Register */
    __IOM uint32_t I2S_CMIR;             /* Offset: 0x08ch (R/W)  Clear Mode Interrupt Register */
    __IOM uint32_t I2S_DIV0_LEVEL;       /* Offset: 0x090h (R/W)  Divide i2s_clkgen source clock, get mclk or sclk */
    __IOM uint32_t I2S_DIV1_LEVEL;       /* Offset: 0x094h (R/W)  Divide i2s_clkgen source clock, get spdif_clk */
    __IOM uint32_t I2S_DIV2_LEVEL;       /* Offset: 0x098h (R/W)  Divide i2s_clkgen sclk, get wsclk */
    __IOM uint32_t I2S_DIV3_LEVEL;       /* Offset: 0x09ch (R/W)  Divide i2s_clkgen source clock, get reference clock */
} ck_i2s_reg_t;

#endif /* _CK_I2S_H_ */
