/* ---------------------------------------------------------------------------- */
/*                  Atmel Microcontroller Software Support                      */
/*                       SAM Software Package License                           */
/* ---------------------------------------------------------------------------- */
/* Copyright (c) 2015, Atmel Corporation                                        */
/*                                                                              */
/* All rights reserved.                                                         */
/*                                                                              */
/* Redistribution and use in source and binary forms, with or without           */
/* modification, are permitted provided that the following condition is met:    */
/*                                                                              */
/* - Redistributions of source code must retain the above copyright notice,     */
/* this list of conditions and the disclaimer below.                            */
/*                                                                              */
/* Atmel's name may not be used to endorse or promote products derived from     */
/* this software without specific prior written permission.                     */
/*                                                                              */
/* DISCLAIMER:  THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR   */
/* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE   */
/* DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,      */
/* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT */
/* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,  */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    */
/* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING         */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, */
/* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                           */
/* ---------------------------------------------------------------------------- */

#ifndef USBD_HAL_H
#define USBD_HAL_H

/**
 *  \file
 *
 *  This file defines functions for USB Device Hardware Access Level.
 */

/** \addtogroup usbd_hal
 *@{*/

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

/* Introduced in C99 by working group ISO/IEC JTC1/SC22/WG14. */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "USBD.h"
#include <USBDescriptors.h>
#include <USBRequests.h>

/*----------------------------------------------------------------------------
 *        Macros
 *----------------------------------------------------------------------------*/

/** Get bitmap for an endpoint */
#define bmEP(bEP)   (1 << (bEP))

#define EndpointInt     (USBHS_DEVISR_PEP_0 | USBHS_DEVISR_PEP_1 | USBHS_DEVISR_PEP_2 | USBHS_DEVISR_PEP_3 |\
                         USBHS_DEVISR_PEP_4 | USBHS_DEVISR_PEP_5 | USBHS_DEVISR_PEP_6 | USBHS_DEVISR_PEP_7 |\
                         USBHS_DEVISR_PEP_8 | USBHS_DEVISR_PEP_9 | USBHS_DEVISR_PEP_10 | USBHS_DEVISR_PEP_11 )

#define DMAEndpointInt  (USBHS_DEVISR_DMA_1 | USBHS_DEVISR_DMA_2 | USBHS_DEVISR_DMA_3 |\
                         USBHS_DEVISR_DMA_4 | USBHS_DEVISR_DMA_5 | USBHS_DEVISR_DMA_6 | USBHS_DEVISR_DMA_7 )
/*----------------------------------------------------------------------------
 *        Types
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

extern void USBD_HAL_Init(uint8_t forceFS);
extern void USBD_HAL_Connect(void);
extern void USBD_HAL_Disconnect(void);

extern void USBD_HAL_RemoteWakeUp(void);
extern void USBD_HAL_SetConfiguration(uint8_t cfgnum);
extern void USBD_HAL_SetAddress(uint8_t address);
extern uint8_t USBD_HAL_IsHighSpeed(void);

extern void USBD_HAL_Suspend(void);
extern void USBD_HAL_Activate(void);
extern void USBD_HAL_WaitReadData(uint8_t bEndpoint);
extern void USBD_HAL_ResetEPs(uint32_t bmEPs,uint8_t bStatus, uint8_t bKeepCfg);
extern void USBD_HAL_CancelIo(uint32_t bmEPs);
extern uint8_t USBD_HAL_ConfigureEP(const USBEndpointDescriptor * pDescriptor);

extern uint8_t USBD_HAL_SetTransferCallback(uint8_t bEP,
                                            TransferCallback fCallback,
                                            void * pCbData);
extern uint8_t USBD_HAL_SetupMblTransfer(uint8_t bEndpoint,
                                         USBDTransferBuffer * pMbList,
                                         uint16_t mblSize,
                                         uint16_t startOffset);
extern uint8_t USBD_HAL_Write(uint8_t bEndpoint,
                              const void * pData,
                              uint32_t dLength);
extern uint8_t USBD_HAL_WrWithHdr(uint8_t bEndpoint,
                                  const void * pHdr, uint8_t bHdrLen,
                                  const void * pData, uint32_t dLength);
extern uint8_t USBD_HAL_Read(uint8_t bEndpoint,
                             void * pData,
                             uint32_t dLength);
extern uint8_t USBD_HAL_Stall(uint8_t bEP);
extern uint8_t USBD_HAL_Halt(uint8_t bEndpoint,uint8_t ctl);
extern void USBD_HAL_Test(uint8_t bIndex);

void USBD_HAL_DetachUsb(void);
void USBD_HAL_AttachUsb(void);
void USBD_HAL_Disable(void);
void USBD_HAL_Enable(void);
                             
void UDPHS_EndOfTransfer(uint8_t bEndpoint, uint8_t bStatus);
/**@}*/

#endif // #define USBD_HAL_H
