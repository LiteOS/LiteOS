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

/**
 * \file
 *
 * Implementation of ILI9488 SPI DMA driver.
 *
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/
#include "board.h"

#include <string.h>
#include <stdio.h>

#ifdef BOARD_LCD_ILI9488

/*----------------------------------------------------------------------------
 *        Local variables
 *----------------------------------------------------------------------------*/
/** Pins to configure for the application. */
static const Pin lcd_spi_cds_pin = BOARD_SPI_LCD_PIN_CDS;

static sIli9488Dma ili9488DmaSpiMode;
static sIli9488DmaCtl  ili9488DmaCtlInSpiMode;

/*----------------------------------------------------------------------------
 *        Local functions
 *----------------------------------------------------------------------------*/
/**
 * \brief ILI9488_SPI xDMA Rx callback
 */
static void _ILI9488_Spi_Rx_CB(void)
{
	if (!ili9488DmaCtlInSpiMode.cmdOrDataFlag) {
		ili9488DmaCtlInSpiMode.rxDoneFlag = 1;
		SCB_InvalidateDCache_by_Addr((uint32_t *)ili9488DmaSpiMode.xdmadRxCfg.mbr_da,
									 ili9488DmaSpiMode.xdmadRxCfg.mbr_ubc);
		memory_barrier()
	}
}

/**
 * \brief ILI9488_SPI xDMA Tx callback
 */
static void _ILI9488_Spi_Tx_CB(void)
{
	uint32_t i;

	if (ili9488DmaCtlInSpiMode.cmdOrDataFlag) {
		PIO_Set(&lcd_spi_cds_pin);

		for (i = 0; i < 0xFF; i++);

		ili9488DmaCtlInSpiMode.cmdOrDataFlag = 0;
	}

	ili9488DmaCtlInSpiMode.txDoneFlag = 1;
}

/**
 * \brief Initializes the ili9488DmaSpiMode structure and the corresponding DMA .
 * hardware.
 */
static void _ILI9488_SpiDmaInitialize(sXdmad *dmad)
{
	ili9488DmaCtlInSpiMode.cmdOrDataFlag = 1;
	ili9488DmaCtlInSpiMode.rxDoneFlag = 0;
	ili9488DmaCtlInSpiMode.txDoneFlag = 1;

	ili9488DmaSpiMode.xdmaD = dmad;
	ili9488DmaSpiMode.xdmaD->pXdmacs = XDMAC;
	ili9488DmaSpiMode.ili9488DmaTxChannel = 0;
	ili9488DmaSpiMode.ili9488DmaRxChannel = 0;
	ili9488DmaSpiMode.xdmaInt = 0;
	ili9488DmaSpiMode.pSpiHw = ILI9488_SPI;
	ili9488DmaSpiMode.spiId = ILI9488_SPI_ID;
}

/**
 * \brief This function initialize the appropriate DMA channel for Rx/Tx channel
 * of SPI or SMC
 * \returns             0 if the transfer has been started successfully;
 * otherwise returns ILI9488_ERROR_XX is the driver is in use,
 * or ILI9488_ERROR_XX if the command is not valid.
 */
static uint8_t _ILI9488_SpiDmaConfigChannels(void)
{
	uint32_t srcType, dstType;

	/* Driver initialize */
	XDMAD_Initialize(ili9488DmaSpiMode.xdmaD, 0);

	XDMAD_FreeChannel(ili9488DmaSpiMode.xdmaD,
					   ili9488DmaSpiMode.ili9488DmaTxChannel);
	XDMAD_FreeChannel(ili9488DmaSpiMode.xdmaD,
					   ili9488DmaSpiMode.ili9488DmaRxChannel);

	srcType = XDMAD_TRANSFER_MEMORY;
	dstType = ili9488DmaSpiMode.spiId;

	/* Allocate a DMA channel for  ILI9488_SPI TX. */
	ili9488DmaSpiMode.ili9488DmaTxChannel
		= XDMAD_AllocateChannel(ili9488DmaSpiMode.xdmaD, srcType, dstType);

	if (ili9488DmaSpiMode.ili9488DmaTxChannel == XDMAD_ALLOC_FAILED)
		return ILI9488_ERROR_DMA_ALLOCATE_CHANNEL;

	/* Allocate a DMA channel for ILI9488_SPI  RX. */
	ili9488DmaSpiMode.ili9488DmaRxChannel
		= XDMAD_AllocateChannel(ili9488DmaSpiMode.xdmaD, dstType, srcType);

	if (ili9488DmaSpiMode.ili9488DmaRxChannel == XDMAD_ALLOC_FAILED)
		return ILI9488_ERROR_DMA_ALLOCATE_CHANNEL;

	/* Setup callbacks for ILI9488_SPI RX */
	XDMAD_SetCallback(ili9488DmaSpiMode.xdmaD,
					  ili9488DmaSpiMode.ili9488DmaRxChannel,
					  (XdmadTransferCallback)_ILI9488_Spi_Rx_CB,
					  &ili9488DmaSpiMode);

	if (XDMAD_PrepareChannel(
			ili9488DmaSpiMode.xdmaD, ili9488DmaSpiMode.ili9488DmaRxChannel))
		return ILI9488_ERROR_DMA_ALLOCATE_CHANNEL;

	/* Setup callbacks for ILI9488_SPI  TX (ignored) */
	XDMAD_SetCallback(ili9488DmaSpiMode.xdmaD,
					  ili9488DmaSpiMode.ili9488DmaTxChannel,
					  (XdmadTransferCallback)_ILI9488_Spi_Tx_CB,
					  &ili9488DmaSpiMode);

	if (XDMAD_PrepareChannel(
			 ili9488DmaSpiMode.xdmaD, ili9488DmaSpiMode.ili9488DmaTxChannel))
		return  ILI9488_ERROR_DMA_ALLOCATE_CHANNEL;

	/* Check if DMA IRQ is enable; if not Enable it */
	if (!(NVIC_GetActive(XDMAC_IRQn))) {
		NVIC_SetPriority(XDMAC_IRQn , 1);
		/* Enable interrupt  */
		NVIC_EnableIRQ(XDMAC_IRQn);
	}

	return 0;
}

/**
 * \brief Configure the SPI/SMC tx/rx DMA.
 * \returns 0 if the xDMA configuration successfully; otherwise returns
 * ILI9488_ERROR_XXX.
 */
static uint8_t _ILI9488_SpiDmaConfigureRxTx(void)
{
	uint32_t txAddress, rxAddress;
	sXdmad *pXdmad;
	pXdmad = ili9488DmaSpiMode.xdmaD;

	txAddress = (uint32_t)&ILI9488_SPI->SPI_TDR;
	rxAddress = (uint32_t)&ILI9488_SPI->SPI_RDR;

	/* Setup DMA TX channel */
	ili9488DmaSpiMode.xdmadTxCfg.mbr_sa = 0;
	ili9488DmaSpiMode.xdmadTxCfg.mbr_da = txAddress;
	ili9488DmaSpiMode.xdmadTxCfg.mbr_ubc = 0;

	ili9488DmaSpiMode.xdmadTxCfg.mbr_cfg
		= XDMAC_CC_TYPE_PER_TRAN
		  | XDMAC_CC_MBSIZE_SINGLE
		  | XDMAC_CC_DSYNC_MEM2PER
		  | XDMAC_CC_DWIDTH_BYTE
		  | XDMAC_CC_CSIZE_CHK_1
		  | XDMAC_CC_SIF_AHB_IF1
		  | XDMAC_CC_DIF_AHB_IF1
		  | XDMAC_CC_SAM_INCREMENTED_AM
		  | XDMAC_CC_DAM_FIXED_AM
		  | XDMAC_CC_PERID(XDMAIF_Get_ChannelNumber
						   (ili9488DmaSpiMode.spiId, XDMAD_TRANSFER_TX));

	ili9488DmaSpiMode.xdmadTxCfg.mbr_bc = 0;
	ili9488DmaSpiMode.xdmadTxCfg.mbr_sus = 0;
	ili9488DmaSpiMode.xdmadTxCfg.mbr_dus = 0;

	/* Setup RX DMA channel */
	ili9488DmaSpiMode.xdmadRxCfg.mbr_ubc  = 0;
	ili9488DmaSpiMode.xdmadRxCfg.mbr_da = 0;
	ili9488DmaSpiMode.xdmadRxCfg.mbr_sa = rxAddress;

	ili9488DmaSpiMode.xdmadRxCfg.mbr_cfg =
		XDMAC_CC_TYPE_PER_TRAN
		| XDMAC_CC_MBSIZE_SINGLE
		| XDMAC_CC_DSYNC_PER2MEM
		| XDMAC_CC_CSIZE_CHK_1
		| XDMAC_CC_DWIDTH_WORD
		| XDMAC_CC_SIF_AHB_IF1
		| XDMAC_CC_DIF_AHB_IF1
		| XDMAC_CC_SAM_FIXED_AM
		| XDMAC_CC_DAM_INCREMENTED_AM
		| XDMAC_CC_PERID(XDMAIF_Get_ChannelNumber
						 (ili9488DmaSpiMode.spiId, XDMAD_TRANSFER_RX));
	ili9488DmaSpiMode.xdmadRxCfg.mbr_bc = 0;
	ili9488DmaSpiMode.xdmadRxCfg.mbr_sus = 0;
	ili9488DmaSpiMode.xdmadRxCfg.mbr_dus = 0;

	/* Put all interrupts on for non LLI list setup of DMA */
	ili9488DmaSpiMode.xdmaInt =  (XDMAC_CIE_BIE
								  | XDMAC_CIE_RBIE
								  | XDMAC_CIE_WBIE
								  | XDMAC_CIE_ROIE);

	if (XDMAD_ConfigureTransfer(pXdmad, ili9488DmaSpiMode.ili9488DmaRxChannel,
								 &ili9488DmaSpiMode.xdmadRxCfg, 0, 0, ili9488DmaSpiMode.xdmaInt))
		return ILI9488_ERROR_DMA_CONFIGURE;

	if (XDMAD_ConfigureTransfer(pXdmad, ili9488DmaSpiMode.ili9488DmaTxChannel,
								 &ili9488DmaSpiMode.xdmadTxCfg, 0, 0, ili9488DmaSpiMode.xdmaInt))
		return ILI9488_ERROR_DMA_CONFIGURE;

	return 0;
}

/**
 * \brief Update Rx/Tx DMA configuration with new buffer address and buffer size.
 * \param pTxBuffer point to Tx buffer address
 * \param wTxSize  Tx buffer size in byte
 * \param pRxBuffer point to Rx buffer address
 * \param wRxSize Rx buffer size in byte
 * \returns 0 if the xDMA configuration successfully; otherwise returns
 * ILI9488_DMA_ERROR_XXX.
 */
static uint8_t _ILI9488_SpiDmaUpdateBuffer(uint8_t *pTxBuffer, uint32_t wTxSize,
		uint32_t *pRxBuffer, uint32_t wRxSize)
{
	sXdmad *pXdmad;
	pXdmad = ili9488DmaSpiMode.xdmaD;

	ili9488DmaSpiMode.xdmadTxCfg.mbr_sa = (uint32_t)pTxBuffer;
	ili9488DmaSpiMode.xdmadTxCfg.mbr_ubc = wTxSize;

	if (wRxSize) {
		ili9488DmaSpiMode.xdmadRxCfg.mbr_da = (uint32_t)pRxBuffer;
		ili9488DmaSpiMode.xdmadRxCfg.mbr_ubc = wRxSize;

		if (XDMAD_ConfigureTransfer(pXdmad, ili9488DmaSpiMode.ili9488DmaRxChannel,
									 &ili9488DmaSpiMode.xdmadRxCfg, 0, 0, ili9488DmaSpiMode.xdmaInt))
			return ILI9488_ERROR_DMA_CONFIGURE;
	}

	if (XDMAD_ConfigureTransfer(pXdmad, ili9488DmaSpiMode.ili9488DmaTxChannel,
								 &ili9488DmaSpiMode.xdmadTxCfg, 0, 0, ili9488DmaSpiMode.xdmaInt))
		return ILI9488_ERROR_DMA_CONFIGURE;

	return 0;
}

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/
/**
 * \brief Initialize ILI9488 driver with DMA support.
 * \returns 0 if the xDMA configuration successfully; otherwise returns
 * ILI9488_DMA_ERROR_XXX.
 */
uint8_t ILI9488_SpiInitializeWithDma(sXdmad *dmad)
{
	_ILI9488_SpiDmaInitialize(dmad);

	if (_ILI9488_SpiDmaConfigChannels()) return ILI9488_ERROR_DMA_ALLOCATE_CHANNEL;

	if (_ILI9488_SpiDmaConfigureRxTx()) return ILI9488_ERROR_DMA_CONFIGURE;

	return 0;
}

/**
 * \brief Start ILI9488 DMA transfer .
 * \param pTxBuffer point to Tx buffer address
 * \param wTxSize  Tx buffer size in byte
 * \returns 0 if the xDMA configuration successfully; otherwise returns
 * ILI9488_DMA_ERROR_XXX.
 */
uint8_t ILI9488_SpiDmaTxTransfer(uint8_t *pTxBuffer, uint32_t wTxSize)
{
	while (!ili9488DmaCtlInSpiMode.txDoneFlag);

	_ILI9488_SpiDmaUpdateBuffer(pTxBuffer, wTxSize, 0, 0);
	SCB_CleanDCache_by_Addr((uint32_t *)pTxBuffer, wTxSize);
	ili9488DmaCtlInSpiMode.txDoneFlag = 0;

	if (XDMAD_StartTransfer(
			ili9488DmaSpiMode.xdmaD, ili9488DmaSpiMode.ili9488DmaTxChannel))
		return ILI9488_ERROR_DMA_TRANSFER;

	while (!ili9488DmaCtlInSpiMode.txDoneFlag);

	return 0;
}

/**
 * \brief Start ILI9488 DMA Rx transfer .
 * \param pRxBuffer point to Rx buffer address
 * \param wRxSize Rx buffer size in byte
 * \returns 0 if the xDMA transfer successfully; otherwise returns
 * ILI9488_DMA_ERROR_XXX.
 */
uint8_t ILI9488_SpiDmaRxTransfer(uint32_t *pRxBuffer, uint32_t wRxSize)
{
	_ILI9488_SpiDmaUpdateBuffer((uint8_t *)pRxBuffer, wRxSize,
								(uint32_t *)pRxBuffer, wRxSize);
	SCB_CleanDCache_by_Addr((uint32_t *)pRxBuffer, wRxSize);

	if (XDMAD_StartTransfer(ili9488DmaSpiMode.xdmaD,
							 ili9488DmaSpiMode.ili9488DmaRxChannel))
		return ILI9488_ERROR_DMA_TRANSFER;

	if (XDMAD_StartTransfer(ili9488DmaSpiMode.xdmaD,
							 ili9488DmaSpiMode.ili9488DmaTxChannel))
		return ILI9488_ERROR_DMA_TRANSFER;

	return 0;
}

/**
 * \brief Start ILI9488 DMA Rx transfer .
 * \param Instr Instruct
 * \param pTxData point to Tx buffer address
 * \param pRxData point to Rx buffer address
 * \param ReadWrite Command/Write/Read access
 * \param Size buffer size in byte
 * \returns 0
 */
uint8_t ILI9488_SpiSendCommand(uint8_t Instr, uint8_t *pTxData,
							   uint32_t *pRxData, AccessIli_t ReadWrite, uint32_t size)
{
	if (ReadWrite == AccessInst) {
		PIO_Clear(&lcd_spi_cds_pin);
		ili9488DmaCtlInSpiMode.cmdOrDataFlag = 1;
		return ILI9488_SpiDmaTxTransfer(&Instr, 1);
	} else if (ReadWrite == AccessWrite) {
		PIO_Clear(&lcd_spi_cds_pin);
		ili9488DmaCtlInSpiMode.cmdOrDataFlag = 1;
		ILI9488_SpiDmaTxTransfer(&Instr, 1);

		if (size == 0) return 0;

		ILI9488_SpiDmaTxTransfer(pTxData, size);
		return 0;
	} else {
		ili9488DmaCtlInSpiMode.rxDoneFlag = 0;
		ILI9488_SpiDmaRxTransfer(pRxData, size);

		while (!ili9488DmaCtlInSpiMode.rxDoneFlag);
	}

	return 0;
}

#endif  //BOARD_LCD_ILI9488
