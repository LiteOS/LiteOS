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
#ifndef KSZ8851SNL_H__
#define KSZ8851SNL_H__

/**************************************************************************//**
* @addtogroup kitdrv
* @{
******************************************************************************/

/**************************************************************************//**
* @addtogroup ksz8851snl
* @{ This module provides a driver for the ksz8851snl ethernet controller.
*
* The ksz8851snl is a single-chip ethernet controller which supports
* 10/100 Mbps full and half-duplex communication. It has integrated 12KiB RX
* FIFO and a 6KiB TX FIFO and it is controlled using SPI. This module contains
* functions for the necessary SPI commands for reading/writing registers
* and reading from and writing to the FIFO's.
******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif


/* Interrupt Enable Register Options */
/** Enable link change interrupt */
#define   KSZ8851SNL_INT_LINK_CHANGE     0x8000
/** Enable transmit done interrupt */
#define   KSZ8851SNL_INT_TX_DONE         0x4000
/** Enable receive interrupt */
#define   KSZ8851SNL_INT_RX_DONE         0x2000
/** Enable receive overrun interrupt */
#define   KSZ8851SNL_INT_RX_OVERRUN      0x0800
/** Enable transmit process stopped interrupt */
#define   KSZ8851SNL_INT_TX_STOPPED      0x0200
/** Enable receive process stopped interrupt */
#define   KSZ8851SNL_INT_RX_STOPPED      0x0100
/** Enable transmit space available interrupt */
#define   KSZ8851SNL_INT_TX_SPACE        0x0040
/** Enable WOL on receive wake-up frame detect interrupt */
#define   KSZ8851SNL_INT_RX_WOL_FRAME    0x0020
/** Enable magic packet detect interrupt */
#define   KSZ8851SNL_INT_MAGIC           0x0010
/** Enable link up detect interrupt */
#define   KSZ8851SNL_INT_LINKUP          0x0008
/** Enable detect interrupt */
#define   KSZ8851SNL_INT_ENERGY          0x0004
/** Enable receive SPI bus error interrupt */
#define   KSZ8851SNL_INT_SPI_ERROR       0x0002

/** Service RX done, link change and error IRQs. */
#define   KSZ8851SNL_INT_ENABLE_MASK     (KSZ8851SNL_INT_RX_DONE | \
                                          KSZ8851SNL_INT_RX_STOPPED | \
                                          KSZ8851SNL_INT_TX_STOPPED | \
                                          KSZ8851SNL_INT_LINK_CHANGE | \
                                          KSZ8851SNL_INT_SPI_ERROR)

/**
 * @brief
 *   The MIB (Management Information Base) Counters that the ksz8851snl device
 *   expose to the host.
 */
typedef struct KSZ8851SLN_mib_s
{
  /** Rx octet count including bad packets */
  uint32_t RxByteCnt;
  /** Rx undersize packets w/ good CRC */
  uint32_t RxUndersizePktCnt;
  /** Rx fragment packets w/ bad CRC, symbol errors or alignment errors */
  uint32_t RxFragmentsCnt;
  /** Rx oversize packets w/ good CRC (max: 1536 bytes) */
  uint32_t RxOversizeCnt;
  /** Rx packets longer than 1536 bytes
   * w/ either CRC errors, alignment errors, or symbol errors */
  uint32_t RxJabbersCnt;
  /** Rx packets w/ invalid data symbol and legal packet size */
  uint32_t RxSymbolErrorCnt;
  /** Rx packets within (64,2000) bytes w/ an
   * integral number of bytes and a bad CRC */
  uint32_t RxCRCErrorCnt;
  /** Number of PAUSE frames received by a port. */
  uint32_t RxPausePktsCnt;
  /** Rx good broadcast packets */
  uint32_t RxBroadcastCnt;
  /** Rx good multicast packets */
  uint32_t RxMulticastCnt;
  /** Rx good unicast packets */
  uint32_t RxUnicastCnt;
  /** Tx good octet count, including PAUSE packets */
  uint32_t TxByteCnt;
  /** Number of PAUSE frames transmitted */
  uint32_t TxPausePktsCnt;
  /** Tx good broadcast packets */
  uint32_t TxBroadcastPktsCnt;
  /** Tx good multicast packets */
  uint32_t TxMulticastPktsCnt;
  /** Tx good unicast packets */
  uint32_t TxUnicastPktsCnt;
  /** Tx packets by a port for which the 1st Tx attempt is delayed due
   * to the busy medium */
  uint32_t TxDeferredCnt;
  /** Tx total collision, half duplex only */
  uint32_t TxTotalCollisionCnt;
} KSZ8851SLN_mib_t;


void     KSZ8851SNL_Init(void);
void     KSZ8851SNL_Enable(void);
uint16_t KSZ8851SNL_Receive(uint16_t length, uint8_t *buffer);
bool     KSZ8851SNL_TransmitBegin(uint16_t length);
void     KSZ8851SNL_Transmit(uint16_t length, const uint8_t *buffer);
void     KSZ8851SNL_TransmitEnd(uint16_t length);

void     KSZ8851SNL_MacAddressGet(uint8_t *macAddress);
void     KSZ8851SNL_PMECRStatusClear(uint16_t flags);
void     KSZ8851SNL_FrameCounterSet(void);
uint16_t KSZ8851SNL_FrameCounterGet(void);
void     KSZ8851SNL_IntEnable(void);
void     KSZ8851SNL_IntDisable(void);
void     KSZ8851SNL_IntClear(uint16_t flags);
uint16_t KSZ8851SNL_IntGet(void);
uint16_t KSZ8851SNL_RXQCRGet(void);
uint16_t KSZ8851SNL_PHYStatusGet(void);
void     KSZ8851SNL_TxQueueReset(void);
void     KSZ8851SNL_RxQueueReset(void);

void     KSZ8851SNL_RegistersDump(void);
void     KSZ8851SNL_AllRegistersDump(void);
void     KSZ8851SNL_MIBCountersDump(void);
void     KSZ8851SNL_MIBCountersUpdate(void);
uint16_t KSZ8851SNL_FrameCounterGet(void);

#ifdef __cplusplus
}
#endif

/** @} (end group ksz8851snl) */
/** @} (end group Drivers) */

#endif
