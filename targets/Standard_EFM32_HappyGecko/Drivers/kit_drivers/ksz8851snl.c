/**************************************************************************//**
 * @file
 * @brief Driver for Micrel KSZ8851SNL Ethernet controller
 * @version 5.0.0
 ******************************************************************************
 * @section License
 * <b>Copyright 2015 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/
#include <stdio.h>

#include "ksz8851snl.h"
#include "ksz8851snl_spi.h"
#include "em_assert.h"

#include "lwip/sys.h"
#include "lwipopts.h"
#include "debug.h"

#ifndef LWIP4EFM32
#error "lwiopts.h for EFM32 are not included"
#endif

/* Register definitions */
#define MARL      0x10 /**< MAC Address Low */
#define MARM      0x12 /**< MAC Address Middle */
#define MARH      0x14 /**< MAC Address High */
#define OBCR      0x20 /**< On-Chip Bus Control Register */
#define GRR       0x26 /**< Global Reset Register */
#define TXCR      0x70 /**< Transmit Flow Control Register */
#define RXCR1     0x74 /**< Receive Flow Control Register 1 */
#define RXCR2     0x76 /**< Receive Flow Control Register 2 */
#define TXMIR     0x78 /**< TXQ Memory Information Register */
#define RXFHSR    0x7C /**< Receive Frame Header Status Register */
#define RXFHBCR   0x7E /**< Receive Frame Header Bytecount Register */
#define TXQCR     0x80 /**< TXQ Command Register */
#define RXQCR     0x82 /**< RXQ Command Register */
#define TXFDPR    0x84 /**< TX Frame Data Pointer Register */
#define RXFDPR    0x86 /**< RX Frame Data Pointer Register */
#define IER       0x90 /**< Interrupt Enable Register */
#define ISR       0x92 /**< Interrupt Status Register */
#define RXFCTR    0x9C /**< RX Frame Count & Threshold Register */
#define TXNTFSR   0x9E /**< TX Next Frame size register */
#define FCLWR     0xB0 /**< Configure Low Watermark to 6KByte */
#define FCHWR     0xB2 /**< Configure High Watermark to 4KByte */
#define CIDER     0xC0 /**< Chip ID and Enable Register */
#define IACR      0xC8 /**< Indirect access control Register */
#define IADLR     0xD0 /**< Indirect access data low Register */
#define IADHR     0xD2 /**< Indirect access data high Register */
#define PMECR     0xD4 /**< Power Managment Event Control Register */
#define PHYRR     0xD8 /**< PHY Reset Register  */
#define P1MBCR    0xE4 /**< PHY1 MII-Register Basic Control Register */
#define P1CR      0xF6 /**< Port 1 Control Register */
#define P1SR      0xF8 /**< Port 1 Status Register */

#define KSZ8851SNL_CHIP_ID           0x8870 /**< Default Chip ID for KSZ8851SNL */
#define CHIP_ID_MASK                 0xFFF0 /**< Used to mask the revision ID */
#define ONE_FRAME_THRES              0x0001 /**< RX INT after one frame */
#define FD_PTR_AUTO_INC              0x4000 /**< Used to reset the FD pointer */
#define CLEAR_INT                    0xFFFF /**< Used to clear INT_STATUS_REG */
#define NO_INT                       0x0000 /**< Used to disable the interupts */
#define TX_MEM_AVAIL_MASK            0x1FFF /**< Used to mask the reserved bits */
#define FRAME_ID_MASK                0x003F /**< Used to mask the reserved bits */
#define CHECKSUM_VALID_FRAME_MASK    0x3C17 /**< CRC OK for ICMP, IP, TCP, UDP
                                             *   MII error
                                             *   Frame too long error */

#define VALID_FRAME_MASK             0x8000 /**< RXFV Receive Frame Valid of
                                             *   the @ref RXFHSR register */

#define RX_BYTE_CNT_MASK             0x0FFF /**< Used to mask the reserved bits */
#define LSB_MASK                     0x00FF /**< Used to mask the LSB */
#define MSB_POS                      0x0008 /**< Used to mark the MSB pos */
#define TX_INT_on_COMPLETION         0x8000 /**< TX INT on completion */
#define WORD_SIZE                    0x0004 /**< Word size in # of bytes */
#define EXTRA_SIZE                   0x0008 /**< Needed for the frame header */
#define BLOCKING_RECEIVE             0      /**< Determines if receive will block */
#define WATERMARK_6KB                0x0600 /**< 6KByte Watermark */
#define WATERMARK_4KB                0x0400 /**< 4KByte Watermark */
/* Silicon Laboratories MAC address space */
#define HIGH_QMU_MAC_H               0x00   /**< 1st segment of the MAC address */
#define HIGH_QMU_MAC_L               0x0B   /**< 2nd segment of the MAC address */
#define MID_QMU_MAC_H                0x57   /**< 3rd segment of the MAC address */
#define BYTE_MASK                    0x00FF /**< Used to mask the LSB */
#define BYTE_SIZE                    0x0008 /**< Used to mark the MSB pos */

/* TX Flow Control Register Options */

/** Enable Transmit Checksum Generation for ICMP */
#define   TX_FLOW_CTRL_ICMP_CHECKSUM    0x0100
/** Enable Transmit Checksum Generation for TCP */
#define   TX_FLOW_CTRL_TCP_CHECKSUM     0x0040
/** Enable Transmit Checksum Generation for IP */
#define   TX_FLOW_CTRL_IP_CHECKSUM      0x0020
/** Flush Transmit Queue */
#define   TX_FLOW_CTRL_FLUSH_QUEUE      0x0010
/** Transmit flow control enable*/
#define   TX_FLOW_CTRL_FLOW_ENABLE      0x0008
/** Transmit Padding enable */
#define   TX_FLOW_CTRL_PAD_ENABLE       0x0004
/** Transmit CRC Enable */
#define   TX_FLOW_CTRL_CRC_ENABLE       0x0002
/** Enable tranmsit */
#define   TX_FLOW_CTRL_ENABLE           0x0001

/** TX FLOW CONTROL Initialization collection */
#define   TX_FLOW_CTRL_CONFIG           (TX_FLOW_CTRL_ICMP_CHECKSUM | \
                                         TX_FLOW_CTRL_TCP_CHECKSUM |  \
                                         TX_FLOW_CTRL_IP_CHECKSUM |   \
                                         TX_FLOW_CTRL_FLOW_ENABLE |   \
                                         TX_FLOW_CTRL_PAD_ENABLE |    \
                                         TX_FLOW_CTRL_CRC_ENABLE)

/* TXQ Command Register Options */
/** Enable Auto-Enqueue TXQ Frame */
#define   TXQ_AUTO_ENQUEUE         0x0004
/** Enable INT generation when TXQ Memory Available */
#define   TXQ_MEM_AVAILABLE_INT    0x0002
/** Enable Manual Engueue TXQ Frame */
#define   TXQ_ENQUEUE              0x0001


/* RX Flow Control Register 1 Options */
/** Flush Receive Queue */
#define   RX_FLOW_CTRL_FLUSH_QUEUE       0x8000
/** Enable Receive UDP Frame Checksum Check */
#define   RX_FLOW_CTRL_UDP_CHECKSUM      0x4000
/** Enable Receive TCP Frame Checksum Check */
#define   RX_FLOW_CTRL_TCP_CHECKSUM      0x2000
/** Enable Receive IP Frame Checksum Check */
#define   RX_FLOW_CTRL_IP_CHECKSUM       0x1000
/** Receive Physical Address Filtering with MAC Address Enable */
#define   RX_FLOW_CTRL_MAC_FILTER        0x0800
/** Enable Receive Flow Control */
#define   RX_FLOW_CTRL_FLOW_ENENABLE     0x0400
/** Enable Receive Error Frames */
#define   RX_FLOW_CTRL_BAD_PACKET        0x0200
/** Enable Receive Broadcast frames */
#define   RX_FLOW_CTRL_BROADCAST_ENABLE  0x0080
/** Enable Receive Multicast frames */
#define   RX_FLOW_CTRL_MULTICAST_ENABLE  0x0040
/** Enable Receive Unicast frames */
#define   RX_FLOW_CTRL_UNICAST_ENABLE    0x0020
/** Receive all incoming frames */
#define   RX_FLOW_CTRL_PROMISCUOUS_MODE  0x0012
/** Receive all frames */
#define   RX_FLOW_CTRL_RX_ALL            0x0010
/** Receive Inverse Filtering */
#define   RX_FLOW_CTRL_INVERSE_FILTER    0x0002
/** Enable receive */
#define   RX_FLOW_CTRL_RX_ENABLE         0x0001

/** RX FLOW CONTROL1 Initialization collection */
#define   RX_FLOW_CTRL1_CONFIG           (RX_FLOW_CTRL_UDP_CHECKSUM     |  \
                                          RX_FLOW_CTRL_TCP_CHECKSUM     |  \
                                          RX_FLOW_CTRL_IP_CHECKSUM      |  \
                                          RX_FLOW_CTRL_MAC_FILTER       |  \
                                          RX_FLOW_CTRL_FLOW_ENENABLE    |  \
                                          RX_FLOW_CTRL_BROADCAST_ENABLE |  \
                                          RX_FLOW_CTRL_UNICAST_ENABLE)

/* RX Flow Control Register 2 Options */
/* SPI Receive Data Burst Length */
/** Receive Flow Control Burst Length mask */
#define   RX_FLOW_CTRL_BURST_LEN_MASK        0x00E0
/** 4 bytes length */
#define   RX_FLOW_CTRL_BURST_LEN_4           0x0000
/** 8 Bytes length */
#define   RX_FLOW_CTRL_BURST_LEN_8           0x0020
/** 16 Bytes length */
#define   RX_FLOW_CTRL_BURST_LEN_16          0x0040
/** 32 Bytes length */
#define   RX_FLOW_CTRL_BURST_LEN_32          0x0060
/** Full frame length */
#define   RX_FLOW_CTRL_BURST_LEN_FRAME       0x0080
/** IPV4/IPV6/UDP Fragment Frame Pass */
#define   RX_FLOW_CTRL_IPV6_UDP_FRAG_PASS    0x0010
/** IPV4/IPV6/UDP Frame Checksum Equal Zero */
#define   RX_FLOW_CTRL_IPV6_UDP_ZERO_PASS    0x0008
/** Enable UDP Lite frame */
#define   RX_FLOW_CTRL_UDP_LITE_CHECKSUM     0x0004
/** Enable ICMP frame */
#define   RX_FLOW_CTRL_ICMP_CHECKSUM         0x0002
/** Receive Source Address Filtering */
#define   RX_FLOW_CTRL_BLOCK_MAC             0x0001

/** RX FLOW CONTROL2 Initialization collection */
#define   RX_FLOW_CTRL2_CONFIG               (RX_FLOW_CTRL_IPV6_UDP_FRAG_PASS | \
                                              RX_FLOW_CTRL_UDP_LITE_CHECKSUM |  \
                                              RX_FLOW_CTRL_ICMP_CHECKSUM |      \
                                              RX_FLOW_CTRL_BURST_LEN_FRAME)

/* RXQ Command Register Options */
/** RX interrupt is occured on timer duration */
#define   RXQ_ON_TIME_INT            0x1000
/** RX interrupt is occured on byte count threshold */
#define   RXQ_ON_BYTE_CNT_INT        0x0800
/** RX interrupt is occured on frame count threshold */
#define   RXQ_ON_FRAME_CNT_INT       0x0400
/** Enable adding 2-bytes offset before IP frame header */
#define   RXQ_TWOBYTE_OFFSET         0x0200
/** Enable RX interrupt on timer duration */
#define   RXQ_EN_ON_TIME_INT         0x0080
/** Enable RX interrupt on byte count threshold */
#define   RXQ_EN_ON_BYTE_CNT_INT     0x0040
/** Enable RX interrupt on frame count threshold */
#define   RXQ_EN_ON_FRAME_CNT_INT    0x0020
/** Enable Auto Dequeue RXQ Frame */
#define   RXQ_AUTO_DEQUEUE           0x0010
/** Start QMU transfer operation */
#define   RXQ_START_DMA              0x0008
/** Release RX Error Frame */
#define   RXQ_RELEASE_ERROR_FRAME    0x0001

/** RX COMMAND Initialization collection */
#define   RXQ_CMD_CONFIG             (RXQ_EN_ON_FRAME_CNT_INT | \
                                      RXQ_AUTO_DEQUEUE)

/* Port 1 Status Register */
/** Auto-neg done */
#define PORT1_AN_DONE                 0x0040
/** Link good */
#define PORT1_LINK_GOOD               0x0020

/* Port 1 Control Register Options */
/** Turn off port LEDs */
#define   PORT1_LED_OFF               0x8000
/** Disable port transmit */
#define   PORT1_TX_DISABLE            0x4000
/** Restart auto-negotiation */
#define   PORT1_AUTO_NEG_RESTART      0x2000
/** Set port power-down */
#define   PORT1_POWER_DOWN            0x0800
/** Disable auto MDI/MDI-X */
#define   PORT1_AUTO_MDIX_DISABLE     0x0400
/** Force MDI-X */
#define   PORT1_FORCE_MDIX            0x0200
/** Enable auto-negotiation */
#define   PORT1_AUTO_NEG_ENABLE       0x0080
/** Force PHY 100Mbps */
#define   PORT1_FORCE_100_MBIT        0x0040
/** Force PHY in full duplex mode */
#define   PORT1_FORCE_FULL_DUPLEX     0x0020
/** Advertise flow control capability */
#define   PORT1_AUTO_NEG_FLOW_CTRL    0x0010
/** Advertise 100BT full-duplex capability */
#define   PORT1_AUTO_NEG_100BTX_FD    0x0008
/** Advertise 100BT half-duplex capability */
#define   PORT1_AUTO_NEG_100BTX       0x0004
/** Advertise 10BT full-duplex capability  */
#define   PORT1_AUTO_NEG_10BT_FD      0x0002
/** Advertise 10BT half-duplex capability  */
#define   PORT1_AUTO_NEG_10BT         0x0001

/** PHY Port default config @ref P1CR */
#define   PORT1_CONFIG          (PORT1_AUTO_NEG_ENABLE    | \
                                 PORT1_FORCE_100_MBIT     | \
                                 PORT1_FORCE_FULL_DUPLEX  | \
                                 PORT1_AUTO_NEG_FLOW_CTRL | \
                                 PORT1_AUTO_NEG_100BTX_FD | \
                                 PORT1_AUTO_NEG_100BTX    | \
                                 PORT1_AUTO_NEG_10BT_FD   | \
                                 PORT1_AUTO_NEG_10BT      )

/* Global Reset Register Options */
/** QMU Reset */
#define QMU_MODULE_SOFT_RESET    0x0002
/** Global reset */
#define GLOBAL_SOFT_RESET        0x0001

/** PHY Reset Register Options */
#define PHY_RESET                0x0001

/* P1MBCR register */
/** Enable Digital loopback mode */
#define DIGITAL_LOOPBACK     0x4000
/** Force the speed to 100MBps */
#define FORCE_100            0x2000
/** Force auto negotiation */
#define AUTO_NEG             0x1000
/** Restart auto negotiation */
#define RESTART_AUTO_NEG     0x0200
/** Force full duplex */
#define FORCE_FULL_DUPLEX    0x0100

/** wait time in ms for TX memory to be available */
#define TX_MEMORY_WAIT_MS    500

/** Receive Frame Count Threshold to use in RXFCTR */
#define FRAME_COUNT_THRESHOLD 1

/** Management information base registers */
#define MIB_MASK                    0x1C00       /**< MIB Mask */
#define MIB_RxByte                  0x00         /**< # of received bytes */
#define MIB_XXX                     0x01         /**< MIB Reserved byte */
#define MIB_RxUndersizePkt          0x02         /**< # of received undersized packets */
#define MIB_RxFragments             0x03         /**< # of received fragments */
#define MIB_RxOversize              0x04         /**< # of received oversized packets */
#define MIB_RxJabbers               0x05         /**< # of received jabbers */
#define MIB_RxSymbolError           0x06         /**< # of received error symbols */
#define MIB_RxCRCError              0x07         /**< # of received packets with CRC error */
#define MIB_RxAlignmentError        0x08         /**< # of received missaligned packets */
#define MIB_RxControl8808Pkts       0x09         /**< # of received control packets */
#define MIB_RxPausePkts             0x0A         /**< # of received pause packets */
#define MIB_RxBroadcast             0x0B         /**< # of received broadcast packets */
#define MIB_RxMulticast             0x0C         /**< # of received multicast packets */
#define MIB_RxUnicast               0x0D         /**< # of received unicast packets */
#define MIB_Rx64Octets              0x0E         /**< # of received packets with size of 64 bytes */
#define MIB_Rx65to127Octets         0x0F         /**< # of received packets with size between 65 and 127 bytes */
#define MIB_Rx128to255Octets        0x10         /**< # of received packets with size between 128 and 255 bytes */
#define MIB_Rx256to511Octets        0x11         /**< # of received packets with size between 256 and 511 bytes */
#define MIB_Rx512to1023Octets       0x12         /**< # of received packets with size between 512 and 1023 bytes */
#define MIB_Rx1024to1521Octets      0x13         /**< # of received packets with size between 1024 and 1521 bytes */
#define MIB_Rx1522to2000Octets      0x14         /**< # of received packets with size between 1522 and 2000 bytes */
#define MIB_TxByte                  0x15         /**< # of transmitted bytes */
#define MIB_TxLateCollision         0x16         /**< # of transmitted late collision packets */
#define MIB_TxPausePkts             0x17         /**< # of transmitted pause packets */
#define MIB_TxBroadcastPkts         0x18         /**< # of transmitted broadcast packets */
#define MIB_TxMulticastPkts         0x19         /**< # of transmitted multicast packets */
#define MIB_TxUnicastPkts           0x1A         /**< # of transmitted unicast packets */
#define MIB_TxDeferred              0x1B         /**< # of transmitted deferred packets */
#define MIB_TxTotalCollision        0x1C         /**< # of transmitted total collisions */
#define MIB_TxExcessiveCollision    0x1D         /**< # of transmitted excessive collisions */
#define MIB_TxSingleCollision       0x1E         /**< # of transmitted single collisions */
#define MIB_TxMultipleCollision     0x1F         /**< # of transmitted multiple collisions */

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
#define READ_UNSAFE_REGISTERS       0

static uint16_t frameId = 0;        /**< Raw ethernet frame sent over the network */
static uint8_t  macAddress[6];      /**< Unique MAC address */
static uint16_t rxFrameCount;
static uint32_t interruptCnt = 0;
/** @endcond */

/**
 * @brief
 *   Copy of the current MIB counters values from the ksz8851snl.
 *   This is updated by calling @ref KSZ8851SNL_MIBCountersUpdate.
 *
 * @details
 *   The datasheet recommends that the MIB counter values are read
 *   at least every 30 seconds because of counter overflow possibility.
 */
static KSZ8851SLN_mib_t mibCounters;

/***************************************************************************//**
 * @brief
 *   helper function for KSZ8851SNL_UpdateMIBCounters
 *
 * @param[in] offset
 *   The offset specifies which MIB counter you want to read
 *
 * @return
 *   The value of the MIB Counter
 *****************************************************************************/
static uint32_t MIBCountersRead(uint16_t offset)
{
  uint16_t data;
  uint32_t counter;

  data = MIB_MASK | offset;
  KSZ8851SNL_SPI_WriteRegister(IACR, data);
  counter  = KSZ8851SNL_SPI_ReadRegister(IADLR);
  counter |= KSZ8851SNL_SPI_ReadRegister(IADHR) << 16;

  return counter;
}

/***************************************************************************//**
 * @brief
 *   Update the Management Information Base Counters
 *
 * @note
 *   Support method used for debugging.
 *****************************************************************************/
void KSZ8851SNL_MIBCountersUpdate(void)
{
  mibCounters.RxByteCnt           += MIBCountersRead(MIB_RxByte);
  mibCounters.RxUndersizePktCnt   += MIBCountersRead(MIB_RxUndersizePkt);
  mibCounters.RxFragmentsCnt      += MIBCountersRead(MIB_RxFragments);
  mibCounters.RxOversizeCnt       += MIBCountersRead(MIB_RxOversize);
  mibCounters.RxJabbersCnt        += MIBCountersRead(MIB_RxJabbers);
  mibCounters.RxSymbolErrorCnt    += MIBCountersRead(MIB_RxSymbolError);
  mibCounters.RxCRCErrorCnt       += MIBCountersRead(MIB_RxCRCError);
  mibCounters.RxPausePktsCnt      += MIBCountersRead(MIB_RxPausePkts);
  mibCounters.RxBroadcastCnt      += MIBCountersRead(MIB_RxBroadcast);
  mibCounters.RxMulticastCnt      += MIBCountersRead(MIB_RxMulticast);
  mibCounters.RxUnicastCnt        += MIBCountersRead(MIB_RxUnicast);
  mibCounters.TxByteCnt           += MIBCountersRead(MIB_TxByte);
  mibCounters.TxPausePktsCnt      += MIBCountersRead(MIB_TxPausePkts);
  mibCounters.TxBroadcastPktsCnt  += MIBCountersRead(MIB_TxBroadcastPkts);
  mibCounters.TxMulticastPktsCnt  += MIBCountersRead(MIB_TxMulticastPkts);
  mibCounters.TxUnicastPktsCnt    += MIBCountersRead(MIB_TxUnicastPkts);
  mibCounters.TxDeferredCnt       += MIBCountersRead(MIB_TxDeferred);
  mibCounters.TxTotalCollisionCnt += MIBCountersRead(MIB_TxTotalCollision);
}

/***************************************************************************//**
 * @brief Dumps the Management Information Base Counters
 * @note  Support method used for debugging.
 *
 *****************************************************************************/
void KSZ8851SNL_MIBCountersDump(void)
{
  printf("####################### MIB COUNTER DUMP ########################\n");
  printf("MIB_RxByteCnt           = %lu\n", mibCounters.RxByteCnt);
  printf("MIB_RxUndersizePktCnt   = %lu\n", mibCounters.RxUndersizePktCnt);
  printf("MIB_RxFragmentsCnt      = %lu\n", mibCounters.RxFragmentsCnt);
  printf("MIB_RxOversizeCnt       = %lu\n", mibCounters.RxOversizeCnt);
  printf("MIB_RxJabbersCnt        = %lu\n", mibCounters.RxJabbersCnt);
  printf("MIB_RxSymbolErrorCnt    = %lu\n", mibCounters.RxSymbolErrorCnt);
  printf("MIB_RxCRCErrorCnt       = %lu\n", mibCounters.RxCRCErrorCnt);
  printf("MIB_RxPausePktsCnt      = %lu\n", mibCounters.RxPausePktsCnt);
  printf("MIB_RxBroadcastCnt      = %lu\n", mibCounters.RxBroadcastCnt);
  printf("MIB_RxMulticastCnt      = %lu\n", mibCounters.RxMulticastCnt);
  printf("MIB_RxUnicastCnt        = %lu\n", mibCounters.RxUnicastCnt);
  printf("MIB_TxByteCnt           = %lu\n", mibCounters.TxByteCnt);
  printf("MIB_TxPausePktsCnt      = %lu\n", mibCounters.TxPausePktsCnt);
  printf("MIB_TxBroadcastPktsCnt  = %lu\n", mibCounters.TxBroadcastPktsCnt);
  printf("MIB_TxMulticastPktsCnt  = %lu\n", mibCounters.TxMulticastPktsCnt);
  printf("MIB_TxUnicastPktsCnt    = %lu\n", mibCounters.TxUnicastPktsCnt);
  printf("MIB_TxDeferredCnt       = %lu\n", mibCounters.TxDeferredCnt);
  printf("MIB_TxTotalCollisionCnt = %lu\n", mibCounters.TxTotalCollisionCnt);
  printf("#################################################################\n");
}

/***************************************************************************//**
 * @brief Prints the value of the registers of the ethernet controller.
 * @note  Support method used for debugging.
 *****************************************************************************/
void KSZ8851SNL_AllRegistersDump(void)
{
  printf("###################### ALL REGISTER DUMP ########################\n");
  int i;
  for (i = 0x00; i < 0xFF; i += 0x02)
  {
    if ((i % 8 == 0) && (i > 0))
    {
      printf("\n");
    }
    printf("REG[0x%02X]=0x%04X ", i, KSZ8851SNL_SPI_ReadRegister(i));
  }
  printf("\n");
  printf("#################################################################\n");
}

/***************************************************************************//**
 * @brief Prints the value of the registers of the ethernet controller.
 * @note  Support method used for debugging.
 *****************************************************************************/
void KSZ8851SNL_RegistersDump(void)
{
  printf("##################### SPECIAL REGISTER DUMP ######################\n");
  printf("MARL  [0x%02X]=0x%04X\n", MARL,   KSZ8851SNL_SPI_ReadRegister(MARL));
  printf("MARM  [0x%02X]=0x%04X\n", MARM,   KSZ8851SNL_SPI_ReadRegister(MARM));
  printf("MARH  [0x%02X]=0x%04X\n", MARH,   KSZ8851SNL_SPI_ReadRegister(MARH));
  printf("OBCR  [0x%02X]=0x%04X\n", OBCR,   KSZ8851SNL_SPI_ReadRegister(OBCR));
  printf("GRR   [0x%02X]=0x%04X\n", GRR,    KSZ8851SNL_SPI_ReadRegister(GRR));
  printf("TXCR  [0x%02X]=0x%04X\n", TXCR,   KSZ8851SNL_SPI_ReadRegister(TXCR));
  printf("RXCR1 [0x%02X]=0x%04X\n", RXCR1,  KSZ8851SNL_SPI_ReadRegister(RXCR1));
  printf("RXCR2 [0x%02X]=0x%04X\n", RXCR2,  KSZ8851SNL_SPI_ReadRegister(RXCR2));
  printf("TXMIR [0x%02X]=0x%04X\n", TXMIR,  KSZ8851SNL_SPI_ReadRegister(TXMIR));
#if (READ_UNSAFE_REGISTERS)
  printf("RXFHSR[0x%02X]=0x%04X\n", RXFHSR, KSZ8851SNL_SPI_ReadRegister(RXFHSR));
#endif
  printf("TXQCR [0x%02X]=0x%04X\n", TXQCR,  KSZ8851SNL_SPI_ReadRegister(TXQCR));
  printf("RXQCR [0x%02X]=0x%04X\n", RXQCR,  KSZ8851SNL_SPI_ReadRegister(RXQCR));
  printf("TXFDPR[0x%02X]=0x%04X\n", TXFDPR, KSZ8851SNL_SPI_ReadRegister(TXFDPR));
  printf("RXFDPR[0x%02X]=0x%04X\n", RXFDPR, KSZ8851SNL_SPI_ReadRegister(RXFDPR));
  printf("IER   [0x%02X]=0x%04X\n", IER,    KSZ8851SNL_SPI_ReadRegister(IER));
  printf("ISR   [0x%02X]=0x%04X\n", ISR,    KSZ8851SNL_SPI_ReadRegister(ISR));
  printf("RXFCTR[0x%02X]=0x%04X\n", RXFCTR, KSZ8851SNL_SPI_ReadRegister(RXFCTR));
#if (READ_UNSAFE_REGISTERS)
  printf("TXNTFSR[0x%02X]=0x%04X\n", TXNTFSR, KSZ8851SNL_SPI_ReadRegister(TXNTFSR));
#endif
  printf("CIDER [0x%02X]=0x%04X\n", CIDER,  KSZ8851SNL_SPI_ReadRegister(CIDER));
  printf("PHYRR [0x%02X]=0x%04X\n", PHYRR,  KSZ8851SNL_SPI_ReadRegister(PHYRR));
  printf("P1MBCR[0x%02X]=0x%04X\n", P1MBCR, KSZ8851SNL_SPI_ReadRegister(P1MBCR));
  printf("P1CR  [0x%02X]=0x%04X\n", P1CR,   KSZ8851SNL_SPI_ReadRegister(P1CR));
  printf("#################################################################\n");
}

/**************************************************************************//**
 * @brief enables the chip interrupts
 *****************************************************************************/
void KSZ8851SNL_IntEnable(void)
{
  if (interruptCnt)
  {
    interruptCnt--;
  }
  if (interruptCnt == 0)
  {
    /* Enable interrupts */
    KSZ8851SNL_SPI_WriteRegister(IER, KSZ8851SNL_INT_ENABLE_MASK);
  }
}

/**************************************************************************//**
 * @brief disables the chip interrupts
 *****************************************************************************/
void KSZ8851SNL_IntDisable(void)
{
  KSZ8851SNL_SPI_WriteRegister(IER, NO_INT);
  interruptCnt++;
}

/**************************************************************************//**
 * @brief
 *   Clear interrupt flags
 *
 * @param[in] flags
 *   The interrupt flags to clear
 *****************************************************************************/
void KSZ8851SNL_IntClear(uint16_t flags)
{
  KSZ8851SNL_SPI_WriteRegister(ISR, flags);
}

/**************************************************************************//**
 * @brief Get interrupt flags
 *****************************************************************************/
uint16_t KSZ8851SNL_IntGet(void)
{
  return KSZ8851SNL_SPI_ReadRegister(ISR);
}

/**************************************************************************//**
 * @brief
 *   Clear PMECR (Power Management Event Control Register) flags
 *
 * @param[in] flags
 *   The @ref PMECR flags to clear
 *****************************************************************************/
void KSZ8851SNL_PMECRStatusClear(uint16_t flags)
{
  uint16_t status = KSZ8851SNL_SPI_ReadRegister(PMECR) | flags;
  KSZ8851SNL_SPI_WriteRegister(PMECR, status);
}

/**************************************************************************//**
 * @brief Get RXQCR register
 *****************************************************************************/
uint16_t KSZ8851SNL_RXQCRGet(void)
{
  return KSZ8851SNL_SPI_ReadRegister(RXQCR);
}

/**************************************************************************//**
 * @brief FrameCounter
 *****************************************************************************/
void KSZ8851SNL_FrameCounterSet(void)
{
  /* Read the frame count and threshold register */
  uint16_t rxftr = KSZ8851SNL_SPI_ReadRegister(RXFCTR);
  /* Extract the actual number of frames from RX_FRAME_THRES_REG*/
  rxFrameCount = rxftr >> MSB_POS;
}

/**************************************************************************//**
 * @brief Reset TxQueue
 *****************************************************************************/
void KSZ8851SNL_TxQueueReset(void)
{
  uint16_t data;

  data = KSZ8851SNL_SPI_ReadRegister(TXCR);
  /* Disable TX */
  data &= ~TX_FLOW_CTRL_ENABLE;
  KSZ8851SNL_SPI_WriteRegister(TXCR, data);
  /* Flush */
  data |= TX_FLOW_CTRL_FLUSH_QUEUE;
  KSZ8851SNL_SPI_WriteRegister(TXCR, data);
  /* normal op - no flush */
  data &= ~TX_FLOW_CTRL_FLUSH_QUEUE;
  KSZ8851SNL_SPI_WriteRegister(TXCR, data);
  /* Enable TX */
  data |= TX_FLOW_CTRL_ENABLE;
  KSZ8851SNL_SPI_WriteRegister(TXCR, data);
}

/**************************************************************************//**
 * @brief Reset RxQueue
 *****************************************************************************/
void KSZ8851SNL_RxQueueReset(void)
{
  uint16_t data;

  data = KSZ8851SNL_SPI_ReadRegister(RXCR1);
  /* Disable RX */
  data &= ~RX_FLOW_CTRL_RX_ENABLE;
  KSZ8851SNL_SPI_WriteRegister(RXCR1, data);
  /* Flush */
  KSZ8851SNL_SPI_WriteRegister(RXCR1, data);
  /* Clear flush */
  data &= ~RX_FLOW_CTRL_FLUSH_QUEUE;
  KSZ8851SNL_SPI_WriteRegister(RXCR1, data);
  /* Write default config with enable set */
  data |= RX_FLOW_CTRL_RX_ENABLE;
  KSZ8851SNL_SPI_WriteRegister(RXCR1, data);
}

/**************************************************************************//**
 * @brief FrameCounter
 *****************************************************************************/
uint16_t KSZ8851SNL_FrameCounterGet(void)
{
  return rxFrameCount;
}

/***************************************************************************//**
 * @brief Initialize the registers of the ethernet controller.
 *****************************************************************************/
void KSZ8851SNL_Init(void)
{
  uint16_t data;

  /* Initialize SPI Interface */
  KSZ8851SNL_SPI_Init();

  /* Reset PHY module */
  KSZ8851SNL_SPI_WriteRegister(PHYRR, PHY_RESET);

  /* Reset Soft (clear registers of PHY, MAC, QMU, DMA) */
  data = KSZ8851SNL_SPI_ReadRegister(GRR);
  data &= ~GLOBAL_SOFT_RESET;
  KSZ8851SNL_SPI_WriteRegister(GRR, data);

  /* Reset Soft (clear registers of PHY, MAC, QMU, DMA) */
  data = KSZ8851SNL_SPI_ReadRegister(GRR);
  data &= ~GLOBAL_SOFT_RESET;
  KSZ8851SNL_SPI_WriteRegister(GRR, data);

  /* Read the chip ID and check if that is correct */
  data = KSZ8851SNL_SPI_ReadRegister(CIDER);

  /* The CIDER lower bits [3..1] are defined as revision number,
   *   thus a mask needs to be applied
   */

  /* this is a hard assert - abort  */
  EFM_ASSERT((data & CHIP_ID_MASK) == KSZ8851SNL_CHIP_ID);

  /* Write the Queue Management Unit MAC Address */
  KSZ8851SNL_MacAddressGet(macAddress);
  /* Write the MAC Address into the MAC registers */
  KSZ8851SNL_SPI_WriteRegister(MARH, (macAddress[0] << 8) | macAddress[1]);
  KSZ8851SNL_SPI_WriteRegister(MARM, (macAddress[2] << 8) | macAddress[3]);
  KSZ8851SNL_SPI_WriteRegister(MARL, (macAddress[4] << 8) | macAddress[5]);

  /* Enable QMU Transmit Frame Data Pointer Auto Increment */
  KSZ8851SNL_SPI_WriteRegister(TXFDPR, FD_PTR_AUTO_INC);

  /* Enable QMU Transmit:
   *  flow control,
   *  padding,
   *  CRC,
   *  checksum generation.
   */
  KSZ8851SNL_SPI_WriteRegister(TXCR, TX_FLOW_CTRL_CONFIG);

  /* Enable QMU Receive Frame Data Pointer Auto Increment */
  KSZ8851SNL_SPI_WriteRegister(RXFDPR, FD_PTR_AUTO_INC);

  /* Configure Receive Frame Threshold for one frame */
  KSZ8851SNL_SPI_WriteRegister(RXFCTR, FRAME_COUNT_THRESHOLD);

  /* Set RX queue low watermark to 10240 bytes for issuing of
     stop PAUSE frames (flow control) */
  data = 0xA00;
  KSZ8851SNL_SPI_WriteRegister(FCLWR, data);

  /* Enable QMU Receive:
   *  flow control,
   *  receive all broadcast frame,
   *  receive unicast frame,
   *  IP/TCP/UDP/ICMP checksum generation.
   */
  KSZ8851SNL_SPI_WriteRegister(RXCR1, RX_FLOW_CTRL1_CONFIG);

  /* Enable QMU Receive:
   *  ICMP/UDP Lite frame checksum verification,
   *  UDP Lite frame checksum generation,
   *  IPv6 UDP fragment frame pass.
   */
  KSZ8851SNL_SPI_WriteRegister(RXCR2, RX_FLOW_CTRL2_CONFIG);

  /* Enable QMU Receive:
   *  IP Header Two-Byte Offset,
   *  Receive Frame Count Threshold,
   *  RXQ Auto-Dequeue frame.
   */
  KSZ8851SNL_SPI_WriteRegister(RXQCR, RXQ_CMD_CONFIG);

  /* Restart Port 1 auto-negotiation */
  KSZ8851SNL_SPI_WriteRegister(P1CR, PORT1_CONFIG | PORT1_AUTO_NEG_RESTART);

  /* Clear interrupts */
  KSZ8851SNL_SPI_WriteRegister(ISR, CLEAR_INT);
}


/***************************************************************************//**
 * @brief Enable RX and TX
 *****************************************************************************/
void KSZ8851SNL_Enable(void)
{
  uint16_t data;

  KSZ8851SNL_SPI_WriteRegister(IER, KSZ8851SNL_INT_ENABLE_MASK);

  /* Enable QMU Transmit */
  data  = KSZ8851SNL_SPI_ReadRegister(TXCR);
  data |= TX_FLOW_CTRL_ENABLE;
  KSZ8851SNL_SPI_WriteRegister(TXCR, data);

  /* Enable QMU Receive */
  data  = KSZ8851SNL_SPI_ReadRegister(RXCR1);
  data |= RX_FLOW_CTRL_RX_ENABLE;
  KSZ8851SNL_SPI_WriteRegister(RXCR1, data);
}


/***************************************************************************//**
 * @brief
 *   Prepares for a transmission of an ethernet frame over the network.
 *
 * @param[in] length
 *   the length of frame to be transmitted
 *****************************************************************************/
bool KSZ8851SNL_TransmitBegin(uint16_t length)
{
  uint16_t txmir;
  uint16_t data, reqSize;
  uint8_t  outbuf[4];

  /* Wait for previous frame to finish before setting up a new one */
  while (KSZ8851SNL_SPI_ReadRegister(TXQCR) & TXQ_ENQUEUE)
  {
    ;
  }

  /* Make sure there is room for
   *
   * 4 bytes Control Word
   * 4 bytes Byte Count
   * n bytes Packet
   * 4 bytes CRC
   */
  reqSize = length + 12;
  txmir = KSZ8851SNL_SPI_ReadRegister(TXMIR) & TX_MEM_AVAIL_MASK;
  LWIP_DEBUGF(NETIF_DEBUG, ("KSZ8851SNL_LongTransmitInit: txmir =%hu  reqSize = %hu \n", txmir, reqSize));

  if (txmir < reqSize)
  {
    /* TXQ is out of memory */
    LWIP_DEBUGF(NETIF_DEBUG | LWIP_DBG_LEVEL_WARNING, ("Not enough TXQ Memory, available=%u required=%u\n", txmir, reqSize));
    return false;
  }

  LWIP_DEBUGF(NETIF_DEBUG, ("KSZ8851SNL_LongTransmitInit: Memory available >  txmir =%hu  reqSize = %hu \n", txmir, reqSize));
  /* Enable TXQ write access */
  data = KSZ8851SNL_SPI_ReadRegister(RXQCR) | RXQ_START_DMA;
  KSZ8851SNL_SPI_WriteRegister(RXQCR, data);

  /* Write frame ID, control word and byte count */
  outbuf[0] = (frameId++ & FRAME_ID_MASK);
  outbuf[1] = 0x80; /*  TX_INT_on_COMPLETION */
  outbuf[2] = length & LSB_MASK;
  outbuf[3] = length >> MSB_POS;

  /* Start the SPI Transfer and send frame header */
  KSZ8851SNL_SPI_WriteFifoBegin();
  KSZ8851SNL_SPI_WriteFifo(4, outbuf);

  return true;
}


/***************************************************************************//**
 * @brief
 *   Transmit a chunk of data to the ethernet controller. The chunk can be
 *   either a full ethernet frame or a partial chunk of an ethernet frame.
 *
 * @note
 *   If the ethernet packet is divided into several packet buffers then this
 *   function should be called for each chunk in order.
 *
 * @param[in] length
 *     the length of the data buffer to be transmitted
 *
 * @param[in] buffer
 *     the buffer that contains data to be transmitted
 *****************************************************************************/
void KSZ8851SNL_Transmit(uint16_t length, const uint8_t *buffer)
{
  EFM_ASSERT(buffer != NULL);
  KSZ8851SNL_SPI_WriteFifo(length, buffer);
}

/***************************************************************************//**
 * @brief
 *   Ends a transmission of an ethernet frame to the ethernet controller.
 *
 * @details
 *   This will make sure that data is correctly padded. And will not return
 *   until the packet is sent.
 *
 * @param[in] length
 *   the length of the complete frame. This is needed so we can pad the
 *   transmission to a 4 byte boundary
 *****************************************************************************/
void KSZ8851SNL_TransmitEnd(uint16_t length)
{
  uint16_t data;
  uint16_t padding;
  uint8_t dummy[4] = {0x00};

  /* Padding packet to 4 byte boundary */
  if (length % 4)
  {
    padding = 4 - (length % 4);
    KSZ8851SNL_SPI_WriteFifo(padding, dummy);
  }

  /* Stop the SPI Transfer */
  KSZ8851SNL_SPI_WriteFifoEnd();

  /* Disable TXQ write access */
  data = KSZ8851SNL_SPI_ReadRegister(RXQCR) & (~RXQ_START_DMA);
  KSZ8851SNL_SPI_WriteRegister(RXQCR, data);

  /* Start TXQ Manual enqueue */
  data = KSZ8851SNL_SPI_ReadRegister(TXQCR) | TXQ_ENQUEUE;
  KSZ8851SNL_SPI_WriteRegister(TXQCR, data);
}

/***************************************************************************//**
 * @brief Release the current frame if it is inconsistent.
 * @note  Support method used for minimizing the code size.
 *****************************************************************************/
static void ReleaseIncosistentFrame(void)
{
  uint16_t data;
  /* Issue the Release error frame command */
  data = KSZ8851SNL_SPI_ReadRegister(RXQCR) | RXQ_RELEASE_ERROR_FRAME;
  KSZ8851SNL_SPI_WriteRegister(RXQCR, data);

  /* Wait for PHY to clear the command/flag */
  while (KSZ8851SNL_SPI_ReadRegister(RXQCR) & RXQ_RELEASE_ERROR_FRAME)
  {
    ;
  }
}

/***************************************************************************//**
 * @brief
 *   Performs the actual receive of a raw frame over the network.
 *
 * @param[in] length
 *   the length of the buffer
 *
 * @param[in] buffer
 *   buffer to fill with an ethernet frame
 *
 * @return
 *   received packet length, 0 in case of failure
 *****************************************************************************/
uint16_t KSZ8851SNL_Receive(uint16_t length, uint8_t *buffer)
{
  uint16_t data;
  uint16_t rxStatus;
  uint16_t rxPacketLength;
  uint16_t frameLength;
  uint16_t bytesToRead;

  EFM_ASSERT(buffer != NULL);

  while (rxFrameCount > 0)
  {
    rxFrameCount--;
    /* Read the received frame status */
    rxStatus = KSZ8851SNL_SPI_ReadRegister(RXFHSR);

    /* Check the consistency of the frame */
    if ((!(rxStatus & VALID_FRAME_MASK)) || (rxStatus & CHECKSUM_VALID_FRAME_MASK))
    {
      /* Issue the Release error frame command */
      ReleaseIncosistentFrame();
      /* continue to next frame */
      continue;
    }
    else
    {
      /* Read the byte size of the received frame */
      rxPacketLength = KSZ8851SNL_SPI_ReadRegister(RXFHBCR) & RX_BYTE_CNT_MASK;

      /* round to dword boundary */
      bytesToRead = 4 * ((rxPacketLength + 3) >> 2);
      LWIP_DEBUGF(NETIF_DEBUG, ("KSZ8851SNL_Receive: rxPacketLength=%u, bytesToRead=%u \n", rxPacketLength, bytesToRead));
      if ((bytesToRead > length) || (rxPacketLength <= 4))
      {
        /* Issue the Release error frame command */
        ReleaseIncosistentFrame();
        /* continue to next frame */
        continue;
      }

      /* Set QMU RXQ frame pointer to start of packet data. Note
       * that we skip the status word and byte count since we
       * already know this from RXFHSR and RXFHBCR.
       */
      KSZ8851SNL_SPI_WriteRegister(RXFDPR, 0x0004 | FD_PTR_AUTO_INC);

      /* Start QMU DMA transfer */
      data = KSZ8851SNL_SPI_ReadRegister(RXQCR) | RXQ_START_DMA;
      KSZ8851SNL_SPI_WriteRegister(RXQCR, data);

      /* Read the whole ethernet frame */
      KSZ8851SNL_SPI_ReadFifo(bytesToRead, buffer);

      /* Stop QMU DMA transfer */
      data = KSZ8851SNL_SPI_ReadRegister(RXQCR) & (~RXQ_START_DMA);
      KSZ8851SNL_SPI_WriteRegister(RXQCR, data);

      /* Return frame length without CRC */
      frameLength = rxPacketLength - 4;
      return frameLength;
    }
  }
  return 0;
}

/***************************************************************************//**
 * @brief
 *   Get the MAC address of the current board.
 *
 * @note
 *   Support method used for minimizing the code size.
 *
 * @param[out] macAddress
 *   data buffer to store the macAddress
 *****************************************************************************/
void KSZ8851SNL_MacAddressGet(uint8_t *macAddress)
{
  int i;

  EFM_ASSERT(macAddress != NULL);

  /* set the first 3 bytes given by the EM MAC Address space */
  macAddress[0] = HIGH_QMU_MAC_H;
  macAddress[1] = HIGH_QMU_MAC_L;
  macAddress[2] = MID_QMU_MAC_H;
  /* set the next 3 bytes given by the CMU unique ID */

  for (i = 0; i < 3; i++)
  {
    macAddress[5 - i] = (DEVINFO->UNIQUEL & (BYTE_MASK << i * BYTE_SIZE)) >> i * BYTE_SIZE;
  }
}

/***************************************************************************//**
 * @brief Get the PHY status
 *****************************************************************************/
uint16_t KSZ8851SNL_PHYStatusGet(void)
{
  return KSZ8851SNL_SPI_ReadRegister(P1SR);
}
