/*!
    \file  gd32f1x0_can.h
    \brief definitions for the can
*/

/*
    Copyright (C) 2016 GigaDevice

    2014-12-26, V1.0.0, firmware for GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, firmware for GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#ifdef GD32F170_190
#ifndef GD32F1X0_CAN_H
#define GD32F1X0_CAN_H

#include "gd32f1x0.h"

/* CAN definitions */
#define CAN0                               CAN_BASE                     /*!< CAN0 base address */
#define CAN1                               (CAN0 + 0x00000400)          /*!< CAN1 base address */

/* registers definitions */
#define CAN_CTL(canx)                      REG32((canx) + 0x00)         /*!< CAN control register */
#define CAN_STAT(canx)                     REG32((canx) + 0x04)         /*!< CAN status register */
#define CAN_TSTAT(canx)                    REG32((canx) + 0x08)         /*!< CAN transmit status register*/
#define CAN_RFIFO0(canx)                   REG32((canx) + 0x0C)         /*!< CAN receive FIFO0 register */
#define CAN_RFIFO1(canx)                   REG32((canx) + 0x10)         /*!< CAN receive FIFO1 register */
#define CAN_INTEN(canx)                    REG32((canx) + 0x14)         /*!< CAN interrupt enable register */
#define CAN_ERR(canx)                      REG32((canx) + 0x18)         /*!< CAN error register */
#define CAN_BT(canx)                       REG32((canx) + 0x1C)         /*!< CAN bit timing register */
#define CAN_TMI0(canx)                     REG32((canx) + 0x180)        /*!< CAN transmit mailbox0 identifier register */
#define CAN_TMP0(canx)                     REG32((canx) + 0x184)        /*!< CAN transmit mailbox0 property register */
#define CAN_TMDATA00(canx)                 REG32((canx) + 0x188)        /*!< CAN transmit mailbox0 data0 register */
#define CAN_TMDATA10(canx)                 REG32((canx) + 0x18C)        /*!< CAN transmit mailbox0 data1 register */
#define CAN_TMI1(canx)                     REG32((canx) + 0x190)        /*!< CAN transmit mailbox1 identifier register */
#define CAN_TMP1(canx)                     REG32((canx) + 0x194)        /*!< CAN transmit mailbox1 property register */
#define CAN_TMDATA01(canx)                 REG32((canx) + 0x198)        /*!< CAN transmit mailbox1 data0 register */
#define CAN_TMDATA11(canx)                 REG32((canx) + 0x19C)        /*!< CAN transmit mailbox1 data1 register */
#define CAN_TMI2(canx)                     REG32((canx) + 0x1A0)        /*!< CAN transmit mailbox2 identifier register */
#define CAN_TMP2(canx)                     REG32((canx) + 0x1A4)        /*!< CAN transmit mailbox2 property register */
#define CAN_TMDATA02(canx)                 REG32((canx) + 0x1A8)        /*!< CAN transmit mailbox2 data0 register */
#define CAN_TMDATA12(canx)                 REG32((canx) + 0x1AC)        /*!< CAN transmit mailbox2 data1 register */
#define CAN_RFIFOMI0(canx)                 REG32((canx) + 0x1B0)        /*!< CAN receive FIFO0 mailbox identifier register */
#define CAN_RFIFOMP0(canx)                 REG32((canx) + 0x1B4)        /*!< CAN receive FIFO0 mailbox property register */
#define CAN_RFIFOMDATA00(canx)             REG32((canx) + 0x1B8)        /*!< CAN receive FIFO0 mailbox data0 register */
#define CAN_RFIFOMDATA10(canx)             REG32((canx) + 0x1CC)        /*!< CAN receive FIFO0 mailbox data1 register */
#define CAN_RFIFOMI1(canx)                 REG32((canx) + 0x1C0)        /*!< CAN receive FIFO1 mailbox identifier register */
#define CAN_RFIFOMP1(canx)                 REG32((canx) + 0x1C4)        /*!< CAN receive FIFO1 mailbox property register */
#define CAN_RFIFOMDATA01(canx)             REG32((canx) + 0x1C8)        /*!< CAN receive FIFO1 mailbox data0 register */
#define CAN_RFIFOMDATA11(canx)             REG32((canx) + 0x1CC)        /*!< CAN receive FIFO1 mailbox data1 register */
#define CAN_FCTL(canx)                     REG32((canx) + 0x200)        /*!< CAN filter control register */
#define CAN_FMCFG(canx)                    REG32((canx) + 0x204)        /*!< CAN filter mode register */
#define CAN_FSCFG(canx)                    REG32((canx) + 0x20C)        /*!< CAN filter scale register */
#define CAN_FAFIFO(canx)                   REG32((canx) + 0x214)        /*!< CAN filter associated FIFO register */
#define CAN_FW(canx)                       REG32((canx) + 0x21C)        /*!< CAN filter working register */
#define CAN_F0DATA0(canx)                  REG32((canx) + 0x240)        /*!< CAN filter 0 data 0 register */
#define CAN_F1DATA0(canx)                  REG32((canx) + 0x248)        /*!< CAN filter 1 data 0 register */
#define CAN_F2DATA0(canx)                  REG32((canx) + 0x250)        /*!< CAN filter 2 data 0 register */
#define CAN_F3DATA0(canx)                  REG32((canx) + 0x258)        /*!< CAN filter 3 data 0 register */
#define CAN_F4DATA0(canx)                  REG32((canx) + 0x260)        /*!< CAN filter 4 data 0 register */
#define CAN_F5DATA0(canx)                  REG32((canx) + 0x268)        /*!< CAN filter 5 data 0 register */
#define CAN_F6DATA0(canx)                  REG32((canx) + 0x270)        /*!< CAN filter 6 data 0 register */
#define CAN_F7DATA0(canx)                  REG32((canx) + 0x278)        /*!< CAN filter 7 data 0 register */
#define CAN_F8DATA0(canx)                  REG32((canx) + 0x280)        /*!< CAN filter 8 data 0 register */
#define CAN_F9DATA0(canx)                  REG32((canx) + 0x288)        /*!< CAN filter 9 data 0 register */
#define CAN_F10DATA0(canx)                 REG32((canx) + 0x290)        /*!< CAN filter 10 data 0 register */
#define CAN_F11DATA0(canx)                 REG32((canx) + 0x298)        /*!< CAN filter 11 data 0 register */
#define CAN_F12DATA0(canx)                 REG32((canx) + 0x2A0)        /*!< CAN filter 12 data 0 register */
#define CAN_F13DATA0(canx)                 REG32((canx) + 0x2A8)        /*!< CAN filter 13 data 0 register */
#define CAN_F14DATA0(canx)                 REG32((canx) + 0x2B0)        /*!< CAN filter 14 data 0 register */
#define CAN_F15DATA0(canx)                 REG32((canx) + 0x2B8)        /*!< CAN filter 15 data 0 register */
#define CAN_F16DATA0(canx)                 REG32((canx) + 0x2C0)        /*!< CAN filter 16 data 0 register */
#define CAN_F17DATA0(canx)                 REG32((canx) + 0x2C8)        /*!< CAN filter 17 data 0 register */
#define CAN_F18DATA0(canx)                 REG32((canx) + 0x2D0)        /*!< CAN filter 18 data 0 register */
#define CAN_F19DATA0(canx)                 REG32((canx) + 0x2D8)        /*!< CAN filter 19 data 0 register */
#define CAN_F20DATA0(canx)                 REG32((canx) + 0x2E0)        /*!< CAN filter 20 data 0 register */
#define CAN_F21DATA0(canx)                 REG32((canx) + 0x2E8)        /*!< CAN filter 21 data 0 register */
#define CAN_F22DATA0(canx)                 REG32((canx) + 0x2F0)        /*!< CAN filter 22 data 0 register */
#define CAN_F23DATA0(canx)                 REG32((canx) + 0x3F8)        /*!< CAN filter 23 data 0 register */
#define CAN_F24DATA0(canx)                 REG32((canx) + 0x300)        /*!< CAN filter 24 data 0 register */
#define CAN_F25DATA0(canx)                 REG32((canx) + 0x308)        /*!< CAN filter 25 data 0 register */
#define CAN_F26DATA0(canx)                 REG32((canx) + 0x310)        /*!< CAN filter 26 data 0 register */
#define CAN_F27DATA0(canx)                 REG32((canx) + 0x318)        /*!< CAN filter 27 data 0 register */
#define CAN_F0DATA1(canx)                  REG32((canx) + 0x244)        /*!< CAN filter 0 data 1 register */
#define CAN_F1DATA1(canx)                  REG32((canx) + 0x24C)        /*!< CAN filter 1 data 1 register */
#define CAN_F2DATA1(canx)                  REG32((canx) + 0x254)        /*!< CAN filter 2 data 1 register */
#define CAN_F3DATA1(canx)                  REG32((canx) + 0x25C)        /*!< CAN filter 3 data 1 register */
#define CAN_F4DATA1(canx)                  REG32((canx) + 0x264)        /*!< CAN filter 4 data 1 register */
#define CAN_F5DATA1(canx)                  REG32((canx) + 0x26C)        /*!< CAN filter 5 data 1 register */
#define CAN_F6DATA1(canx)                  REG32((canx) + 0x274)        /*!< CAN filter 6 data 1 register */
#define CAN_F7DATA1(canx)                  REG32((canx) + 0x27C)        /*!< CAN filter 7 data 1 register */
#define CAN_F8DATA1(canx)                  REG32((canx) + 0x284)        /*!< CAN filter 8 data 1 register */
#define CAN_F9DATA1(canx)                  REG32((canx) + 0x28C)        /*!< CAN filter 9 data 1 register */
#define CAN_F10DATA1(canx)                 REG32((canx) + 0x294)        /*!< CAN filter 10 data 1 register */
#define CAN_F11DATA1(canx)                 REG32((canx) + 0x29C)        /*!< CAN filter 11 data 1 register */
#define CAN_F12DATA1(canx)                 REG32((canx) + 0x2A4)        /*!< CAN filter 12 data 1 register */
#define CAN_F13DATA1(canx)                 REG32((canx) + 0x2AC)        /*!< CAN filter 13 data 1 register */
#define CAN_F14DATA1(canx)                 REG32((canx) + 0x2B4)        /*!< CAN filter 14 data 1 register */
#define CAN_F15DATA1(canx)                 REG32((canx) + 0x2BC)        /*!< CAN filter 15 data 1 register */
#define CAN_F16DATA1(canx)                 REG32((canx) + 0x2C4)        /*!< CAN filter 16 data 1 register */
#define CAN_F17DATA1(canx)                 REG32((canx) + 0x24C)        /*!< CAN filter 17 data 1 register */
#define CAN_F18DATA1(canx)                 REG32((canx) + 0x2D4)        /*!< CAN filter 18 data 1 register */
#define CAN_F19DATA1(canx)                 REG32((canx) + 0x2DC)        /*!< CAN filter 19 data 1 register */
#define CAN_F20DATA1(canx)                 REG32((canx) + 0x2E4)        /*!< CAN filter 20 data 1 register */
#define CAN_F21DATA1(canx)                 REG32((canx) + 0x2EC)        /*!< CAN filter 21 data 1 register */
#define CAN_F22DATA1(canx)                 REG32((canx) + 0x2F4)        /*!< CAN filter 22 data 1 register */
#define CAN_F23DATA1(canx)                 REG32((canx) + 0x2FC)        /*!< CAN filter 23 data 1 register */
#define CAN_F24DATA1(canx)                 REG32((canx) + 0x304)        /*!< CAN filter 24 data 1 register */
#define CAN_F25DATA1(canx)                 REG32((canx) + 0x30C)        /*!< CAN filter 25 data 1 register */
#define CAN_F26DATA1(canx)                 REG32((canx) + 0x314)        /*!< CAN filter 26 data 1 register */
#define CAN_F27DATA1(canx)                 REG32((canx) + 0x31C)        /*!< CAN filter 27 data 1 register */
#define CAN_PHYCTL(canx)                   REG32((canx) + 0x3FC)        /*!< CAN PHY control register */

/* CAN transmit mailbox bank */
#define CAN_TMI(canx, bank)                REG32((canx) + 0x180 + ((bank) * 0x10))      /*!< CAN transmit mailbox identifier register */
#define CAN_TMP(canx, bank)                REG32((canx) + 0x184 + ((bank) * 0x10))      /*!< CAN transmit mailbox property register */
#define CAN_TMDATA0(canx, bank)            REG32((canx) + 0x188 + ((bank) * 0x10))      /*!< CAN transmit mailbox data0 register */
#define CAN_TMDATA1(canx, bank)            REG32((canx) + 0x18C + ((bank) * 0x10))      /*!< CAN transmit mailbox data1 register */

/* CAN filter bank */
#define CAN_FDATA0(canx, bank)             REG32((canx) + 0x240 + ((bank) * 0x8) + 0x0) /*!< CAN filter data 0 register */
#define CAN_FDATA1(canx, bank)             REG32((canx) + 0x240 + ((bank) * 0x8) + 0x4) /*!< CAN filter data 1 register */

/* CAN receive fifo mailbox bank */
#define CAN_RFIFOMI(canx, bank)            REG32((canx) + 0x1B0 + ((bank) * 0x10))      /*!< CAN receive FIFO mailbox identifier register */
#define CAN_RFIFOMP(canx, bank)            REG32((canx) + 0x1B4 + ((bank) * 0x10))      /*!< CAN receive FIFO mailbox property register */
#define CAN_RFIFOMDATA0(canx, bank)        REG32((canx) + 0x1B8 + ((bank) * 0x10))      /*!< CAN receive FIFO mailbox data0 register */
#define CAN_RFIFOMDATA1(canx, bank)        REG32((canx) + 0x1BC + ((bank) * 0x10))      /*!< CAN receive FIFO mailbox data1 register */

/* bits definitions */
/* CAN_CTL */
#define CAN_CTL_IWMOD                      BIT(0)                       /*!< initial working mode */
#define CAN_CTL_SLPWMOD                    BIT(1)                       /*!< sleep working mode */
#define CAN_CTL_TFO                        BIT(2)                       /*!< transmit FIFO order */
#define CAN_CTL_RFOD                       BIT(3)                       /*!< receive FIFO overwrite disable */
#define CAN_CTL_ARD                        BIT(4)                       /*!< automatic retransmission disable */
#define CAN_CTL_AWU                        BIT(5)                       /*!< automatic wakeup */
#define CAN_CTL_ABOR                       BIT(6)                       /*!< automatic bus-off recovery */
#define CAN_CTL_TTC                        BIT(7)                       /*!< time triggered communication */
#define CAN_CTL_SWRST                      BIT(15)                      /*!< CAN software reset */
#define CAN_CTL_DFZ                        BIT(16)                      /*!< CAN debug freeze */

/* CAN_STAT */
#define CAN_STAT_IWS                       BIT(0)                       /*!< initial working state */
#define CAN_STAT_SLPWS                     BIT(1)                       /*!< sleep working state */
#define CAN_STAT_ERRIF                     BIT(2)                       /*!< error interrupt flag*/
#define CAN_STAT_WUIF                      BIT(3)                       /*!< status change interrupt flag of wakeup from sleep working mode */
#define CAN_STAT_SLPIF                     BIT(4)                       /*!< status change interrupt flag of sleep working mode entering */
#define CAN_STAT_TS                        BIT(8)                       /*!< transmitting state */
#define CAN_STAT_RS                        BIT(9)                       /*!< receiving state */
#define CAN_STAT_LASTRX                    BIT(10)                      /*!< last sample value of rx pin */
#define CAN_STAT_RXL                       BIT(11)                      /*!< CAN rx signal */

/* CAN_TSTAT */
#define CAN_TSTAT_MTF0                     BIT(0)                       /*!< mailbox0 transmit finished */
#define CAN_TSTAT_MTFNERR0                 BIT(1)                       /*!< mailbox0 transmit finished and no error */
#define CAN_TSTAT_MAL0                     BIT(2)                       /*!< mailbox0 arbitration lost */
#define CAN_TSTAT_MTE0                     BIT(3)                       /*!< mailbox0 transmit error */
#define CAN_TSTAT_MST0                     BIT(7)                       /*!< mailbox0 stop transmitting */
#define CAN_TSTAT_MTF1                     BIT(8)                       /*!< mailbox1 transmit finished */
#define CAN_TSTAT_MTFNERR1                 BIT(9)                       /*!< mailbox1 transmit finished and no error */
#define CAN_TSTAT_MAL1                     BIT(10)                      /*!< mailbox1 arbitration lost */
#define CAN_TSTAT_MTE1                     BIT(11)                      /*!< mailbox1 transmit error */
#define CAN_TSTAT_MST1                     BIT(15)                      /*!< mailbox1 stop transmitting */
#define CAN_TSTAT_MTF2                     BIT(16)                      /*!< mailbox2 transmit finished */
#define CAN_TSTAT_MTFNERR2                 BIT(17)                      /*!< mailbox2 transmit finished and no error */
#define CAN_TSTAT_MAL2                     BIT(18)                      /*!< mailbox2 arbitration lost */
#define CAN_TSTAT_MTE2                     BIT(19)                      /*!< mailbox2 transmit error */
#define CAN_TSTAT_MST2                     BIT(23)                      /*!< mailbox2 stop transmitting */
#define CAN_TSTAT_NUM                      BITS(24,25)                  /*!< mailbox number */
#define CAN_TSTAT_TME0                     BIT(26)                      /*!< transmit mailbox0 empty */
#define CAN_TSTAT_TME1                     BIT(27)                      /*!< transmit mailbox1 empty */
#define CAN_TSTAT_TME2                     BIT(28)                      /*!< transmit mailbox2 empty */
#define CAN_TSTAT_TMLS0                    BIT(29)                      /*!< last sending priority flag for mailbox0 */
#define CAN_TSTAT_TMLS1                    BIT(30)                      /*!< last sending priority flag for mailbox1 */
#define CAN_TSTAT_TMLS2                    BIT(31)                      /*!< last sending priority flag for mailbox2 */

/* CAN_RFIFO0 */
#define CAN_RFIFO0_RFL0                    BITS(0,1)                    /*!< receive FIFO0 length */
#define CAN_RFIFO0_RFF0                    BIT(3)                       /*!< receive FIFO0 full */
#define CAN_RFIFO0_RFO0                    BIT(4)                       /*!< receive FIFO0 overfull */
#define CAN_RFIFO0_RFD0                    BIT(5)                       /*!< receive FIFO0 dequeue */

/* CAN_RFIFO1 */
#define CAN_RFIFO1_RFL1                    BITS(0,1)                    /*!< receive FIFO1 length */
#define CAN_RFIFO1_RFF1                    BIT(3)                       /*!< receive FIFO1 full */
#define CAN_RFIFO1_RFO1                    BIT(4)                       /*!< receive FIFO1 overfull */
#define CAN_RFIFO1_RFD1                    BIT(5)                       /*!< receive FIFO1 dequeue */

/* CAN_INTEN */
#define CAN_INTEN_TMEIE                    BIT(0)                       /*!< transmit mailbox empty interrupt enable */
#define CAN_INTEN_RFNEIE0                  BIT(1)                       /*!< receive FIFO0 not empty interrupt enable */
#define CAN_INTEN_RFFIE0                   BIT(2)                       /*!< receive FIFO0 full interrupt enable */
#define CAN_INTEN_RFOIE0                   BIT(3)                       /*!< receive FIFO0 overfull interrupt enable */
#define CAN_INTEN_RFNEIE1                  BIT(4)                       /*!< receive FIFO1 not empty interrupt enable */
#define CAN_INTEN_RFFIE1                   BIT(5)                       /*!< receive FIFO1 full interrupt enable */
#define CAN_INTEN_RFOIE1                   BIT(6)                       /*!< receive FIFO1 overfull interrupt enable */
#define CAN_INTEN_WERRIE                   BIT(8)                       /*!< warning error interrupt enable */
#define CAN_INTEN_PERRIE                   BIT(9)                       /*!< passive error interrupt enable */
#define CAN_INTEN_BOIE                     BIT(10)                      /*!< bus-off interrupt enable */
#define CAN_INTEN_ERRNIE                   BIT(11)                      /*!< error number interrupt enable */
#define CAN_INTEN_ERRIE                    BIT(15)                      /*!< error interrupt enable */
#define CAN_INTEN_WUIE                     BIT(16)                      /*!< wakeup interrupt enable */
#define CAN_INTEN_SLPWIE                   BIT(17)                      /*!< sleep working interrupt enable */

/* CAN_ERR */
#define CAN_ERR_WERR                       BIT(0)                       /*!< warning error */
#define CAN_ERR_PERR                       BIT(1)                       /*!< passive error */
#define CAN_ERR_BOERR                      BIT(2)                       /*!< bus-off error */
#define CAN_ERR_ERRN                       BITS(4,6)                    /*!< error number */
#define CAN_ERR_TECNT                      BITS(16,23)                  /*!< transmit error count */
#define CAN_ERR_RECNT                      BITS(24,31)                  /*!< receive error count */

/* CAN_BT */
#define CAN_BT_BAUDPSC                     BITS(0,9)                    /*!< baudrate prescaler */
#define CAN_BT_BS1                         BITS(16,19)                  /*!< bit segment 1 */
#define CAN_BT_BS2                         BITS(20,22)                  /*!< bit segment 2 */
#define CAN_BT_SJW                         BITS(24,25)                  /*!< resynchronization jump width */
#define CAN_BT_LCMOD                       BIT(30)                      /*!< loopback communication mode */
#define CAN_BT_SCMOD                       BIT(31)                      /*!< silent communication mode */

/* CAN_TMIx */
#define CAN_TMI_TEN                        BIT(0)                       /*!< transmit enable */
#define CAN_TMI_FT                         BIT(1)                       /*!< frame type */
#define CAN_TMI_FF                         BIT(2)                       /*!< frame format */
#define CAN_TMI_EFID                       BITS(3,31)                   /*!< the frame identifier */
#define CAN_TMI_SFID                       BITS(21,31)                  /*!< the frame identifier */

/* CAN_TMPx */
#define CAN_TMP_DLENC                      BITS(0,3)                    /*!< data length code */
#define CAN_TMP_TSEN                       BIT(8)                       /*!< time stamp enable */
#define CAN_TMP_TS                         BITS(16,31)                  /*!< time stamp */

/* CAN_TMDATA0x */
#define CAN_TMDATA0_DB0                    BITS(0,7)                    /*!< transmit data byte 0 */
#define CAN_TMDATA0_DB1                    BITS(8,15)                   /*!< transmit data byte 1 */
#define CAN_TMDATA0_DB2                    BITS(16,23)                  /*!< transmit data byte 2 */
#define CAN_TMDATA0_DB3                    BITS(24,31)                  /*!< transmit data byte 3 */

/* CAN_TMDATA1x */
#define CAN_TMDATA1_DB4                    BITS(0,7)                    /*!< transmit data byte 4 */
#define CAN_TMDATA1_DB5                    BITS(8,15)                   /*!< transmit data byte 5 */
#define CAN_TMDATA1_DB6                    BITS(16,23)                  /*!< transmit data byte 6 */
#define CAN_TMDATA1_DB7                    BITS(24,31)                  /*!< transmit data byte 7 */

/* CAN_RFIFOMIx */
#define CAN_RFIFOMI_FT                     BIT(1)                       /*!< frame type */
#define CAN_RFIFOMI_FF                     BIT(2)                       /*!< frame format */
#define CAN_RFIFOMI_EFID                   BITS(3,31)                   /*!< the frame identifier */
#define CAN_RFIFOMI_SFID                   BITS(21,31)                  /*!< the frame identifier */

/* CAN_RFIFOMPx */
#define CAN_RFIFOMP_DLENC                  BITS(0,3)                    /*!< receive data length code */
#define CAN_RFIFOMP_FI                     BITS(8,15)                   /*!< filter index */
#define CAN_RFIFOMP_TS                     BITS(16,31)                  /*!< time stamp */

/* CAN_RFIFOMDATA0x */
#define CAN_RFIFOMDATA0_DB0                BITS(0,7)                    /*!< receive data byte 0 */
#define CAN_RFIFOMDATA0_DB1                BITS(8,15)                   /*!< receive data byte 1 */
#define CAN_RFIFOMDATA0_DB2                BITS(16,23)                  /*!< receive data byte 2 */
#define CAN_RFIFOMDATA0_DB3                BITS(24,31)                  /*!< receive data byte 3 */

/* CAN_RFIFOMDATA1x */
#define CAN_RFIFOMDATA1_DB4                BITS(0,7)                    /*!< receive data byte 4 */
#define CAN_RFIFOMDATA1_DB5                BITS(8,15)                   /*!< receive data byte 5 */
#define CAN_RFIFOMDATA1_DB6                BITS(16,23)                  /*!< receive data byte 6 */
#define CAN_RFIFOMDATA1_DB7                BITS(24,31)                  /*!< receive data byte 7 */

/* CAN_FCTL */
#define CAN_FCTL_FLD                       BIT(0)                       /*!< filter lock disable */
#define CAN_FCTL_HBC1F                     BITS(8,13)                   /*!< header bank of CAN1 filter */

/* CAN_FMCFG */
#define CAN_FMCFG_FMOD(regval)             BIT(regval)                  /*!< filter mode, list or mask*/

/* CAN_FSCFG */
#define CAN_FSCFG_FS(regval)               BIT(regval)                  /*!< filter scale, 32 bits or 16 bits*/

/* CAN_FAFIFO */
#define CAN_FAFIFOR_FAF(regval)            BIT(regval)                  /*!< filter associated with FIFO */

/* CAN_FW */
#define CAN_FW_FW(regval)                  BIT(regval)                  /*!< filter working */

/* CAN_PHYCTL */
#define CAN_PHYCTL_PHYEN                   BIT(0)                       /*!< PHY enable */
#define CAN_PHYCTL_POMOD                   BITS(8,9)                    /*!< PHY mode */

/* consts definitions */
/* CAN initiliaze parameters struct */
typedef struct
{
    uint8_t can_mode;                                                   /*!< CAN working mode */ 
    uint8_t can_sjw;                                                    /*!< CAN resynchronization jump width */
    uint8_t can_bs1;                                                    /*!< time segment 1 */
    uint8_t can_bs2;                                                    /*!< time segment 2 */
    ControlStatus can_ttc;                                              /*!< time triggered communication mode */
    ControlStatus can_abor;                                             /*!< automatic bus-off recovery */
    ControlStatus can_awu;                                              /*!< automatic wake-up mode */
    ControlStatus can_ard;                                              /*!< automatic retransmission mode */
    ControlStatus can_rfod;                                             /*!< receive FIFO overwrite mode */
    ControlStatus can_tfo;                                              /*!< transmit FIFO order */
    uint16_t can_psc;                                                   /*!< baudrate prescaler */
}can_parameter_struct;

/* CAN transmit message struct */
typedef struct
{
    uint32_t can_tx_sfid;                                               /*!< standard format frame identifier */
    uint32_t can_tx_efid;                                               /*!< extended format frame identifier */
    uint8_t can_tx_ff;                                                  /*!< format of frame, standard or extended format */
    uint8_t can_tx_ft;                                                  /*!< type of frame, data or remote */
    uint8_t can_tx_dlen;                                                /*!< data length */
    uint8_t can_tx_data[8];                                             /*!< transmit data */
}can_trasnmit_message_struct;

/* CAN receive message struct */
typedef struct
{
    uint32_t can_rx_sfid;                                               /*!< standard format frame identifier */
    uint32_t can_rx_efid;                                               /*!< extended format frame identifier */
    uint8_t can_rx_ff;                                                  /*!< format of frame, standard or extended format */
    uint8_t can_rx_ft;                                                  /*!< type of frame, data or remote */
    uint8_t can_rx_dlen;                                                /*!< data length */
    uint8_t can_rx_data[8];                                             /*!< receive data */
    uint8_t can_rx_fi;                                                  /*!< filtering index */
} can_receive_message_struct;

/* CAN filter parameters struct */
typedef struct
{
    uint16_t can_filter_list_high;                                      /*!< filter list number high bits*/
    uint16_t can_filter_list_low;                                       /*!< filter list number low bits */
    uint16_t can_filter_mask_high;                                      /*!< filter mask number high bits */
    uint16_t can_filter_mask_low;                                       /*!< filter mask number low bits */
    uint16_t can_filter_fifo_number;                                    /*!< receive FIFO associated with the filter */
    uint16_t can_filter_number;                                         /*!< filter number */
    uint16_t can_filter_mode;                                           /*!< filter mode, list or mask */
    uint16_t can_filter_bits;                                           /*!< filter scale */
    ControlStatus can_filter_enable;                                    /*!< filter work or not */
}can_filter_parameter_struct;

/* CAN errors */
typedef enum
{
    CAN_ERROR_NONE = 0,                                                 /*!< no error */
    CAN_ERROR_FILL,                                                     /*!< fill error */
    CAN_ERROR_FORMATE,                                                  /*!< format error */
    CAN_ERROR_ACK,                                                      /*!< ACK error */
    CAN_ERROR_BITRECESSIVE,                                             /*!< bit recessive error */
    CAN_ERROR_BITDOMINANTER,                                            /*!< bit dominant error */
    CAN_ERROR_CRC,                                                      /*!< CRC error */
    CAN_ERROR_SOFTWARECFG                                               /*!< software error */
}can_error_enum;

/* transmit states */
typedef enum
{
    CAN_TRANSMIT_FAILED = 0,                                            /*!< CAN transmitted failure */
    CAN_TRANSMIT_OK = 1,                                                /*!< CAN transmitted success */
    CAN_TRANSMIT_PENDING = 2,                                           /*!< CAN transmitted pending */
    CAN_TRANSMIT_NOMAILBOX = 4,                                         /*!< no empty mailbox to be used for CAN */
}can_transmit_state_enum;

/* CAN baudrate prescaler*/
#define BT_BAUDPSC(regval)                 (BITS(0,9) & ((regval) << 0))

/* CAN bit segment 1*/
#define BT_BS1(regval)                     (BITS(16,19) & ((regval) << 16))

/* CAN bit segment 2*/
#define BT_BS2(regval)                     (BITS(20,22) & ((regval) << 20))

/* CAN resynchronization jump width*/
#define BT_SJW(regval)                     (BITS(24,25) & ((regval) << 24))

/* CAN communication mode*/
#define BT_MODE(regval)                    (BITS(30,31) & ((regval) << 30))

/* CAN FDATA high 16 bits */
#define FDATA_MASK_HIGH(regval)            (BITS(16,31) & ((regval) << 16))

/* CAN FDATA low 16 bits */
#define FDATA_MASK_LOW(regval)             (BITS(0,15) & ((regval) << 0))

/* CAN1 filter start bank_number*/
#define FCTL_HBC1F(regval)                 (BITS(8,13) & ((regval) << 8))

/* CAN transmit mailbox extended identifier*/
#define TMI_EFID(regval)                   (BITS(3,31) & ((regval) << 3))

/* CAN transmit mailbox standard identifier*/
#define TMI_SFID(regval)                   (BITS(21,31) & ((regval) << 21))

/* transmit data byte 0 */                 
#define TMDATA0_DB0(regval)                (BITS(0,7) & ((regval) << 0))

/* transmit data byte 1 */                
#define TMDATA0_DB1(regval)                (BITS(8,15) & ((regval) << 8))

/* transmit data byte 2 */                 
#define TMDATA0_DB2(regval)                (BITS(16,23) & ((regval) << 16))

/* transmit data byte 3 */                 
#define TMDATA0_DB3(regval)                (BITS(24,31) & ((regval) << 24))

/* transmit data byte 4 */                 
#define TMDATA1_DB4(regval)                (BITS(0,7) & ((regval) << 0))

/* transmit data byte 5 */                 
#define TMDATA1_DB5(regval)                (BITS(8,15) & ((regval) << 8))

/* transmit data byte 6 */                 
#define TMDATA1_DB6(regval)                (BITS(16,23) & ((regval) << 16))

/* transmit data byte 7 */                 
#define TMDATA1_DB7(regval)                (BITS(24,31) & ((regval) << 24))

/* receive mailbox extended identifier*/
#define RFIFOMI_EFID(regval)               GET_BITS((regval), 3, 31)

/* receive mailbox standrad identifier*/
#define RFIFOMI_SFID(regval)               GET_BITS((regval), 21, 31)

/* receive data length */
#define RFIFOMP_DLENC(regval)              GET_BITS((regval), 0, 3)

#define RFIFOMP_FI(regval)                 GET_BITS((regval), 8, 15)

/* receive data byte 0 */
#define RFIFOMDATA0_DB0(regval)            GET_BITS((regval), 0, 7)

/* receive data byte 1 */
#define RFIFOMDATA0_DB1(regval)            GET_BITS((regval), 8, 15)

/* receive data byte 2 */
#define RFIFOMDATA0_DB2(regval)            GET_BITS((regval), 16, 23)

/* receive data byte 3 */
#define RFIFOMDATA0_DB3(regval)            GET_BITS((regval), 24, 31)

/* receive data byte 4 */
#define RFIFOMDATA1_DB4(regval)            GET_BITS((regval), 0, 7)

/* receive data byte 5 */
#define RFIFOMDATA1_DB5(regval)            GET_BITS((regval), 8, 15)

/* receive data byte 6 */
#define RFIFOMDATA1_DB6(regval)            GET_BITS((regval), 16, 23)

/* receive data byte 7 */
#define RFIFOMDATA1_DB7(regval)            GET_BITS((regval), 24, 31)

/* CAN error */
#define ERR_ERRN(regval)                   (BITS(4,6) & ((regval) << 4))
#define CAN_ERRN_0                         ERR_ERRN(0)                  /* no error */
#define CAN_ERRN_1                         ERR_ERRN(1)                  /*!< fill error */
#define CAN_ERRN_2                         ERR_ERRN(2)                  /*!< format error */
#define CAN_ERRN_3                         ERR_ERRN(3)                  /*!< ACK error */
#define CAN_ERRN_4                         ERR_ERRN(4)                  /*!< bit recessive error */
#define CAN_ERRN_5                         ERR_ERRN(5)                  /*!< bit dominant error */
#define CAN_ERRN_6                         ERR_ERRN(6)                  /*!< CRC error */
#define CAN_ERRN_7                         ERR_ERRN(7)                  /*!< software error */

/* CAN phy mode bits */
#define PHYCTL_POMOD(regval)               (BITS(8,9) & ((regval) << 8))

#define CAN_PHYCTL_POMODE_0                PHYCTL_POMOD(0)              /*!< low slope mode */
#define CAN_PHYCTL_POMODE_1                PHYCTL_POMOD(1)              /*!< middle slope mode */
#define CAN_PHYCTL_POMODE_2                PHYCTL_POMOD(2)              /*!< high slope mode */
#define CAN_PHYCTL_POMODE_3                PHYCTL_POMOD(3)              /*!< high speed mode */

#define CAN_STATE_PENDING                  ((uint32_t)0x00)             /*!< CAN pending */

/* CAN communication mode */
#define CAN_NORMAL_MODE                    ((uint8_t)0x00)              /*!< normal communication mode */
#define CAN_LOOPBACK_MODE                  ((uint8_t)0x01)              /*!< loopback communication mode */
#define CAN_SILENT_MODE                    ((uint8_t)0x02)              /*!< silent communication mode */
#define CAN_SILENT_LOOPBACK_MODE           ((uint8_t)0x03)              /*!< loopback and silent communication mode */

/* CAN resynchronisation jump width */
#define CAN_BT_SJW_1TQ                     ((uint8_t)0x00)              /*!< 1 time quanta */
#define CAN_BT_SJW_2TQ                     ((uint8_t)0x01)              /*!< 2 time quanta */
#define CAN_BT_SJW_3TQ                     ((uint8_t)0x02)              /*!< 3 time quanta */
#define CAN_BT_SJW_4TQ                     ((uint8_t)0x03)              /*!< 4 time quanta */

/* CAN time segment 1 */
#define CAN_BT_BS1_1TQ                     ((uint8_t)0x00)               /*!< 1 time quanta */
#define CAN_BT_BS1_2TQ                     ((uint8_t)0x01)               /*!< 2 time quanta */
#define CAN_BT_BS1_3TQ                     ((uint8_t)0x02)               /*!< 3 time quanta */
#define CAN_BT_BS1_4TQ                     ((uint8_t)0x03)               /*!< 4 time quanta */
#define CAN_BT_BS1_5TQ                     ((uint8_t)0x04)               /*!< 5 time quanta */
#define CAN_BT_BS1_6TQ                     ((uint8_t)0x05)               /*!< 6 time quanta */
#define CAN_BT_BS1_7TQ                     ((uint8_t)0x06)               /*!< 7 time quanta */
#define CAN_BT_BS1_8TQ                     ((uint8_t)0x07)               /*!< 8 time quanta */
#define CAN_BT_BS1_9TQ                     ((uint8_t)0x08)               /*!< 9 time quanta */
#define CAN_BT_BS1_10TQ                    ((uint8_t)0x09)               /*!< 10 time quanta */
#define CAN_BT_BS1_11TQ                    ((uint8_t)0x0A)               /*!< 11 time quanta */
#define CAN_BT_BS1_12TQ                    ((uint8_t)0x0B)               /*!< 12 time quanta */
#define CAN_BT_BS1_13TQ                    ((uint8_t)0x0C)               /*!< 13 time quanta */
#define CAN_BT_BS1_14TQ                    ((uint8_t)0x0D)               /*!< 14 time quanta */
#define CAN_BT_BS1_15TQ                    ((uint8_t)0x0E)               /*!< 15 time quanta */
#define CAN_BT_BS1_16TQ                    ((uint8_t)0x0F)               /*!< 16 time quanta */

/* CAN time segment 2 */
#define CAN_BT_BS2_1TQ                     ((uint8_t)0x00)               /*!< 1 time quanta */
#define CAN_BT_BS2_2TQ                     ((uint8_t)0x01)               /*!< 2 time quanta */
#define CAN_BT_BS2_3TQ                     ((uint8_t)0x02)               /*!< 3 time quanta */
#define CAN_BT_BS2_4TQ                     ((uint8_t)0x03)               /*!< 4 time quanta */
#define CAN_BT_BS2_5TQ                     ((uint8_t)0x04)               /*!< 5 time quanta */
#define CAN_BT_BS2_6TQ                     ((uint8_t)0x05)               /*!< 6 time quanta */
#define CAN_BT_BS2_7TQ                     ((uint8_t)0x06)               /*!< 7 time quanta */
#define CAN_BT_BS2_8TQ                     ((uint8_t)0x07)               /*!< 8 time quanta */

/* CAN mailbox number */
#define CAN_MAILBOX0                       ((uint8_t)0x00)              /*!< mailbox0 */
#define CAN_MAILBOX1                       ((uint8_t)0x01)              /*!< mailbox1 */
#define CAN_MAILBOX2                       ((uint8_t)0x02)              /*!< mailbox2 */
#define CAN_NOMAILBOX                      ((uint8_t)0x03)              /*!< no mailbox empty */

/* CAN frame format */
#define CAN_FF_STANDARD                    ((uint32_t)0x00000000)       /*!< standard frame */
#define CAN_FF_EXTENDED                    ((uint32_t)0x00000004)       /*!< extended frame */

/* CAN receive fifo */
#define CAN_FIFO0                          ((uint8_t)0x00)              /*!< receive FIFO0 */
#define CAN_FIFO1                          ((uint8_t)0x01)              /*!< receive FIFO1 */

/* frame number of receive fifo */
#define CAN_RFIFO_RFL0_MASK                ((uint32_t)0x00000003)       /*!< mask for frame number in receive FIFO0 */

#define CAN_SFID_MASK                      ((uint32_t)0x000007FF)       /*!< mask of standard identifier */
#define CAN_EFID_MASK                      ((uint32_t)0x1FFFFFFF)       /*!< mask of extended identifier */

/* CAN working mode */
#define CAN_MODE_INITIALIZE                ((uint8_t)0x01)              /*!< CAN initialize mode */
#define CAN_MODE_NORMAL                    ((uint8_t)0x02)              /*!< CAN normal mode */
#define CAN_MODE_SLEEP                     ((uint8_t)0x04)              /*!< CAN sleep mode */

/* filter bits */
#define CAN_FILTERBITS_16BIT               ((uint8_t)0x00)              /*!< CAN filter 16 bits */
#define CAN_FILTERBITS_32BIT               ((uint8_t)0x01)              /*!< CAN filter 32 bits */

/* filter mode */
#define CAN_FILTERMODE_MASK                ((uint8_t)0x00)              /*!< mask mode */
#define CAN_FILTERMODE_LIST                ((uint8_t)0x01)              /*!< list mode */

/* filter 16 bits mask */
#define CAN_FILTER_MASK_16BITS             ((uint32_t)0x0000FFFF) 

/* frame type */
#define CAN_FT_DATA                        ((uint32_t)0x00000000)       /*!< data frame */
#define CAN_FT_REMOTE                      ((uint32_t)0x00000002)       /*!< remote frame */

/* CAN timeout */
#define CAN_TIMEOUT                        ((uint32_t)0x0000FFFF)

/* transmit flags in TSTAT */
#define CAN_FLAG_MTF0                      ((uint32_t)0x08000001)       /*!< mailbox0 transmit finished flag */
#define CAN_FLAG_MTF1                      ((uint32_t)0x08000100)       /*!< mailbox1 transmit finished flag */
#define CAN_FLAG_MTF2                      ((uint32_t)0x08010000)       /*!< mailbox2 transmit finished flag */

/* receive flags in REFIFO0 */
#define CAN_FLAG_RFL0                      ((uint32_t)0x02000003)       /*!< the length of the receive FIFO0 flag */
#define CAN_FLAG_RFF0                      ((uint32_t)0x02000008)       /*!< receive FIFO0 full flag */
#define CAN_FLAG_RFO0                      ((uint32_t)0x02000010)       /*!< receive FIFO0 overfull flag */

/* receive flags in REFIFO1 */
#define CAN_FLAG_RFL1                      ((uint32_t)0x04000003)       /*!< the length of the receive FIFO1 flag */
#define CAN_FLAG_RFF1                      ((uint32_t)0x04000008)       /*!< receive FIFO1 full flag */
#define CAN_FLAG_RFO1                      ((uint32_t)0x04000010)       /*!< receive FIFO1 overfull flag */

/* working mode flags in STAT */
#define CAN_FLAG_WUIF                      ((uint32_t)0x01000008)       /*!< wake up flag */
#define CAN_FLAG_SLP                       ((uint32_t)0x01000012)       /*!< sleep working state flag */

/* error flags in ERR */
#define CAN_FLAG_WERR                      ((uint32_t)0x00F00001)       /*!< warning error flag */
#define CAN_FLAG_PERR                      ((uint32_t)0x00F00002)       /*!< passive error flag */
#define CAN_FLAG_BOERR                     ((uint32_t)0x00F00004)       /*!< bus-off error flag */
#define CAN_FLAG_ERRN                      ((uint32_t)0x00F00070)       /*!< error number flag  */

#define CAN_FLAG_MASK                      ((uint32_t)0x000FFFFF)       /*!< CAN flag mask */
#define CAN_FLAGS_TSTAT                    ((uint32_t)0x08000000)       /*!< flags in TSTAT register */
#define CAN_FLAGS_RFIFO1                   ((uint32_t)0x04000000)       /*!< flags in RFIFO1 register */
#define CAN_FLAGS_RFIFO0                   ((uint32_t)0x02000000)       /*!< flags in RFIFO0 register */
#define CAN_FLAGS_STAT                     ((uint32_t)0x01000000)       /*!< flags in STAT register */
#define CAN_FLAGS_ERR                      ((uint32_t)0x00F00000)       /*!< flags in ERR register */

/* CAN interrupt flags, just identifiers without any meaning */
#define CAN_INT_TME                        ((uint32_t)0x00000001)       /*!< transmit mailbox empty interrupt*/
#define CAN_INT_RFNE0                      ((uint32_t)0x00000002)       /*!< FIFO0 not empty interrupt */
#define CAN_INT_RFF0                       ((uint32_t)0x00000004)       /*!< FIFO0 full interrupt */
#define CAN_INT_RFO0                       ((uint32_t)0x00000008)       /*!< FIFO0 overrun interrupt */
#define CAN_INT_RFNE1                      ((uint32_t)0x00000010)       /*!< FIFO1 not empty interrupt */
#define CAN_INT_RFF1                       ((uint32_t)0x00000020)       /*!< FIFO1 full interrupt */
#define CAN_INT_RFO1                       ((uint32_t)0x00000040)       /*!< FIFO1 overfull interrupt */
#define CAN_INT_WERR                       ((uint32_t)0x00000080)       /*!< warning error interrupt */
#define CAN_INT_PERR                       ((uint32_t)0x00000100)       /*!< passive error interrupt */
#define CAN_INT_BO                         ((uint32_t)0x00000200)       /*!< bus-off error interrupt */
#define CAN_INT_ERRN                       ((uint32_t)0x00000400)       /*!< error number interrupt */
#define CAN_INT_ERR                        ((uint32_t)0x00000800)       /*!< error interrupt */
#define CAN_INT_WU                         ((uint32_t)0x00001000)       /*!< wake-up interrupt */
#define CAN_INT_SLPW                       ((uint32_t)0x00002000)       /*!< sleep working interrupt interrupt */

/* PHY mode */
#define CAN_PHYMODE_LOW_SLOPE              CAN_PHYCTL_POMODE_0          /*!< CAN PHY low slope mode */
#define CAN_PHYMODE_MIDDLE_SLOPE           CAN_PHYCTL_POMODE_1          /*!< CAN PHY middle slope mode */
#define CAN_PHYMODE_HIGH_SLOPE             CAN_PHYCTL_POMODE_2          /*!< CAN PHY high slope mode */
#define CAN_PHYMODE_HIGH_SPEED             CAN_PHYCTL_POMODE_3          /*!< CAN PHY high speed mode */

/* function declarations */

/* initialization functions */
/* CAN deinit */
void can_deinit(uint32_t can_periph);
/* CAN init */
ErrStatus can_init(uint32_t can_periph, can_parameter_struct* can_parameter_init);

/* transmit functions */
/* CAN transmit message */
uint8_t can_transmit_message(uint32_t can_periph, can_trasnmit_message_struct* transmit_message);
/* CAN transmit state */
can_transmit_state_enum can_transmit_states(uint32_t can_periph, uint8_t mailbox_number);
/* CAN stop transmission */
void can_stop_transmission(uint32_t can_periph, uint8_t mailbox_number);
/* CAN transmit error number */
uint8_t can_transmit_error_number(uint32_t can_periph);

/* filter functions */
/* CAN filter init */
void can_filter_init(can_filter_parameter_struct* can_filter_parameter_init);
/* set can1 fliter start bank number */
void can1_filter_start_bank(uint8_t can1_start_bank_number);

/* enable functions */
/* CAN debug freeze enable */
void can_debug_freeze_enable(uint32_t can_periph);
/* CAN debug freeze disable */
void can_debug_freeze_disable(uint32_t can_periph);
/* CAN time triggle mode enable */
void can_time_trigger_mode_enable(uint32_t can_periph);
/* CAN time triggle mode disable */
void can_time_trigger_mode_disable(uint32_t can_periph);
/* CAN interrupt enable */
void can_interrupt_enable(uint32_t can_periph, uint32_t can_interrupt);
/* CAN interrupt disable */
void can_interrupt_disable(uint32_t can_periph, uint32_t can_interrupt);
/* CAN phy enable */
void can_phy_enable(uint32_t can_periph);
/* CAN phy disable */
void can_phy_disable(uint32_t can_periph);

/* receive functions */
/* CAN receive message */
void can_receive_message(uint32_t can_periph, uint8_t fifo_number, can_receive_message_struct* receive_message);
/* CAN release fifo */
void can_release_fifo(uint32_t can_periph, uint8_t fifo_number);
/* CAN receive message length */
uint8_t can_receive_message_length(uint32_t can_periph, uint8_t fifo_number);
/* CAN receive error number */
uint8_t can_receive_error_number(uint32_t can_periph);

/* mode functions */
/* CAN working mode */
ErrStatus can_working_mode(uint32_t can_periph, uint8_t can_working_mode);
/* CAN set sleep mode */
ErrStatus can_set_sleep_mode(uint32_t can_periph);
/* CAN set sleep mode */
ErrStatus can_wakeup(uint32_t can_periph);
/* set CAN PHY mode */
void can_phy_mode(uint32_t can_periph, uint32_t can_phy_mode);

/* flag functions */
/* CAN get error */
can_error_enum can_get_error(uint32_t can_periph);
/* CAN get flag state */
FlagStatus can_flag_get(uint32_t can_periph, uint32_t can_flag);
/* CAN clear flag state */
void can_flag_clear(uint32_t can_periph, uint32_t can_flag);
/* CAN get interrupt flag */
FlagStatus can_interrupt_flag_get(uint32_t can_periph, uint32_t can_interrupt_flag);
/* CAN clear interrupt flag */
void can_interrupt_flag_clear(uint32_t can_periph, uint32_t can_interrupt_flag);

#endif /* GD32F1X0_CAN_H */

#endif /* GD32F170_190 */
