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
static const Pin lcd_ebi_cds_pin = BOARD_EBI_LCD_PIN_CDS;

COMPILER_ALIGNED(32) static sIli9488Dma ili9488EbiDma;
static volatile sIli9488DmaCtl ili9488DmaCtlInEbiMode;

/*----------------------------------------------------------------------------
 *        Local functions
 *----------------------------------------------------------------------------*/
/**
 * \brief Configure SMC timing for static memory (LCD)
 */
static void _ILI9488_ConfigureSmc(void)
{
	/* Enable peripheral clock */
	PMC_EnablePeripheral(ID_SMC);

	/* Configure SMC, NCS3 is assigned to LCD */

	SMC->SMC_CS_NUMBER[SMC_EBI_LCD_CS].SMC_SETUP = SMC_SETUP_NWE_SETUP(2)
			| SMC_SETUP_NCS_WR_SETUP(0)
			| SMC_SETUP_NRD_SETUP(0)
			| SMC_SETUP_NCS_RD_SETUP(0);

	SMC->SMC_CS_NUMBER[SMC_EBI_LCD_CS].SMC_PULSE = SMC_PULSE_NWE_PULSE(6)
			| SMC_PULSE_NCS_WR_PULSE(0xA)
			| SMC_PULSE_NRD_PULSE(0xA)
			| SMC_PULSE_NCS_RD_PULSE(0xA);

	SMC->SMC_CS_NUMBER[SMC_EBI_LCD_CS].SMC_CYCLE = SMC_CYCLE_NWE_CYCLE(0xA)
			| SMC_CYCLE_NRD_CYCLE(0xA);

	SMC->SMC_CS_NUMBER[SMC_EBI_LCD_CS].SMC_MODE  = SMC_MODE_READ_MODE
			| SMC_MODE_WRITE_MODE
			| SMC_MODE_DBW_16_BIT
			| SMC_MODE_EXNW_MODE_DISABLED
			| SMC_MODE_TDF_CYCLES(0xF);
}

/**
 * \brief ILI9488_EBI xDMA Rx callback
 */
static void _ILI9488_Ebi_Rx_CB(void)
{
	if (!ili9488DmaCtlInEbiMode.cmdOrDataFlag)
		ili9488DmaCtlInEbiMode.rxDoneFlag = 1;
}

/**
 * \brief ILI9488_EBI xDMA Tx callback
 */
static void _ILI9488_Ebi_Tx_CB(void)
{
	volatile uint32_t i;

	if (ili9488DmaCtlInEbiMode.cmdOrDataFlag) {
		for (i = 0; i < 0xF; i++);

		PIO_Set(&lcd_ebi_cds_pin);
		ili9488DmaCtlInEbiMode.cmdOrDataFlag = 0;
	}

	ili9488DmaCtlInEbiMode.txDoneFlag = 1;
}

/**
 * \brief Initializes the ILI9488 DMA structure and the corresponding DMA hardware.
 * select value.
 */
static void _ILI9488_EbiDmaInitialize(sXdmad *dmad)
{
	ili9488DmaCtlInEbiMode.cmdOrDataFlag = 1;
	ili9488DmaCtlInEbiMode.rxDoneFlag = 0;
	ili9488DmaCtlInEbiMode.txDoneFlag = 1;

	ili9488EbiDma.xdmaD = dmad;
	ili9488EbiDma.xdmaD->pXdmacs = XDMAC;
	ili9488EbiDma.ili9488DmaTxChannel = 0;
	ili9488EbiDma.ili9488DmaRxChannel = 0;
	ili9488EbiDma.xdmaInt = 0;
	ili9488EbiDma.pSpiHw = 0;
	ili9488EbiDma.spiId = 0;
}

/**
 * \brief This function initialize the appropriate DMA channel for Rx/Tx channel
 * of SPI or SMC.
 * \returns             0 if the transfer has been started successfully;
 * otherwise returns
 * ILI9488_ERROR_XX is the driver is in use, or ILI9488_ERROR_XX if the command
 * is not valid.
 */
static uint8_t _ILI9488_EbiDmaConfigChannels(void)
{
	uint32_t srcType, dstType;

	/* Driver initialize */
	XDMAD_Initialize(ili9488EbiDma.xdmaD, 0);

	XDMAD_FreeChannel(ili9488EbiDma.xdmaD, ili9488EbiDma.ili9488DmaTxChannel);
	XDMAD_FreeChannel(ili9488EbiDma.xdmaD, ili9488EbiDma.ili9488DmaRxChannel);

	srcType = XDMAD_TRANSFER_MEMORY;
	dstType = XDMAD_TRANSFER_MEMORY;

	/* Allocate a DMA channel for  ILI9488_SPI TX. */
	ili9488EbiDma.ili9488DmaTxChannel =
		XDMAD_AllocateChannel(ili9488EbiDma.xdmaD, srcType, dstType);

	if (ili9488EbiDma.ili9488DmaTxChannel == XDMAD_ALLOC_FAILED)
		return ILI9488_ERROR_DMA_ALLOCATE_CHANNEL;

	/* Allocate a DMA channel for ILI9488_SPI  RX. */
	ili9488EbiDma.ili9488DmaRxChannel =
		XDMAD_AllocateChannel(ili9488EbiDma.xdmaD, dstType, srcType);

	if (ili9488EbiDma.ili9488DmaRxChannel == XDMAD_ALLOC_FAILED)
		return ILI9488_ERROR_DMA_ALLOCATE_CHANNEL;

	/* Setup callbacks for ILI9488_SPI RX */
	XDMAD_SetCallback(ili9488EbiDma.xdmaD, ili9488EbiDma.ili9488DmaRxChannel,
					  (XdmadTransferCallback)_ILI9488_Ebi_Rx_CB, &ili9488EbiDma);

	if (XDMAD_PrepareChannel(ili9488EbiDma.xdmaD,
							  ili9488EbiDma.ili9488DmaRxChannel))
		return ILI9488_ERROR_DMA_ALLOCATE_CHANNEL;

	/* Setup callbacks for ILI9488_SPI  TX (ignored) */
	XDMAD_SetCallback(ili9488EbiDma.xdmaD, ili9488EbiDma.ili9488DmaTxChannel,
					  (XdmadTransferCallback)_ILI9488_Ebi_Tx_CB, &ili9488EbiDma);

	if (XDMAD_PrepareChannel(ili9488EbiDma.xdmaD,
							   ili9488EbiDma.ili9488DmaTxChannel))
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
 * \brief Configure the SPI/SMC tx/rx DMA in SMC mode.
 * \returns 0 if the xDMA configuration successfully; otherwise returns
 * ILI9488_ERROR_XXX.
 */
static uint8_t _ILI9488_EbiDmaConfigureRxTx(void)
{
	uint32_t txAddress, rxAddress;
	sXdmad *pXdmad;
	pXdmad = ili9488EbiDma.xdmaD;

	txAddress = rxAddress = (uint32_t)ILI9488_BASE_ADDRESS;

	/* Setup DMA TX channel */
	ili9488EbiDma.xdmadTxCfg.mbr_sa = 0;
	ili9488EbiDma.xdmadTxCfg.mbr_da = txAddress;
	ili9488EbiDma.xdmadTxCfg.mbr_ubc = 0;

	ili9488EbiDma.xdmadTxCfg.mbr_cfg =
		XDMAC_CC_TYPE_MEM_TRAN
		| XDMAC_CC_MBSIZE_SINGLE
		| XDMAC_CC_CSIZE_CHK_1
		| XDMAC_CC_DWIDTH_HALFWORD
		| XDMAC_CC_SIF_AHB_IF0
		| XDMAC_CC_DIF_AHB_IF1
		| XDMAC_CC_SAM_INCREMENTED_AM
		| XDMAC_CC_DAM_FIXED_AM;

	ili9488EbiDma.xdmadTxCfg.mbr_bc = 0;
	ili9488EbiDma.xdmadTxCfg.mbr_sus = 0;
	ili9488EbiDma.xdmadTxCfg.mbr_dus = 0;

	/* Setup RX DMA channel */
	ili9488EbiDma.xdmadRxCfg.mbr_ubc =
		XDMA_UBC_NVIEW_NDV0 | XDMA_UBC_NDE_FETCH_DIS ;
	ili9488EbiDma.xdmadRxCfg.mbr_da = 0;
	ili9488EbiDma.xdmadRxCfg.mbr_sa = rxAddress;

	ili9488EbiDma.xdmadRxCfg.mbr_cfg =
		XDMAC_CC_TYPE_MEM_TRAN
		| XDMAC_CC_MBSIZE_SINGLE
		| XDMAC_CC_CSIZE_CHK_1
		| XDMAC_CC_DWIDTH_WORD
		| XDMAC_CC_SIF_AHB_IF1
		| XDMAC_CC_DIF_AHB_IF1
		| XDMAC_CC_SAM_FIXED_AM
		| XDMAC_CC_DAM_INCREMENTED_AM;

	ili9488EbiDma.xdmadRxCfg.mbr_bc = 0;
	ili9488EbiDma.xdmadRxCfg.mbr_sus = 0;
	ili9488EbiDma.xdmadRxCfg.mbr_dus = 0;

	/* Put all interrupts on for non LLI list setup of DMA */
	ili9488EbiDma.xdmaInt =  (XDMAC_CIE_BIE
							  | XDMAC_CIE_RBIE
							  | XDMAC_CIE_WBIE
							  | XDMAC_CIE_ROIE);

	if (XDMAD_ConfigureTransfer(pXdmad, ili9488EbiDma.ili9488DmaRxChannel,
								 &ili9488EbiDma.xdmadRxCfg, 0, 0, ili9488EbiDma.xdmaInt))
		return ILI9488_ERROR_DMA_CONFIGURE;

	if (XDMAD_ConfigureTransfer(pXdmad, ili9488EbiDma.ili9488DmaTxChannel,
								 &ili9488EbiDma.xdmadTxCfg, 0, 0, ili9488EbiDma.xdmaInt))
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
static uint8_t _ILI9488_EbiDmaUpdateBuffer(uint16_t *pTxBuffer,
		uint32_t wTxSize,
		uint32_t *pRxBuffer, uint32_t wRxSize)
{
	sXdmad *pXdmad;
	pXdmad = ili9488EbiDma.xdmaD;

	ili9488EbiDma.xdmadTxCfg.mbr_sa = (uint32_t)pTxBuffer;
	ili9488EbiDma.xdmadTxCfg.mbr_ubc = wTxSize;

	if (wRxSize) {
		ili9488EbiDma.xdmadRxCfg.mbr_da = (uint32_t)pRxBuffer;
		ili9488EbiDma.xdmadRxCfg.mbr_ubc = wRxSize;

		if (XDMAD_ConfigureTransfer(pXdmad, ili9488EbiDma.ili9488DmaRxChannel,
									 &ili9488EbiDma.xdmadRxCfg, 0, 0, ili9488EbiDma.xdmaInt))
			return ILI9488_ERROR_DMA_CONFIGURE;
	}

	if (XDMAD_ConfigureTransfer(pXdmad, ili9488EbiDma.ili9488DmaTxChannel,
								 &ili9488EbiDma.xdmadTxCfg, 0, 0, ili9488EbiDma.xdmaInt))
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
uint8_t ILI9488_EbiInitializeWithDma(sXdmad *dmad)
{
	_ILI9488_ConfigureSmc();
	_ILI9488_EbiDmaInitialize(dmad);

	if (_ILI9488_EbiDmaConfigChannels()) return ILI9488_ERROR_DMA_ALLOCATE_CHANNEL;

	if (_ILI9488_EbiDmaConfigureRxTx()) return ILI9488_ERROR_DMA_CONFIGURE;

	return 0;
}

/**
 * \brief Start ILI9488 DMA transfer in SMC mode.
 * \param pTxBuffer point to Tx buffer address
 * \param wTxSize  Tx buffer size in byte
 * \returns 0 if the xDMA configuration successfully; otherwise returns
 * ILI9488_DMA_ERROR_XXX.
 */
uint8_t ILI9488_EbiDmaTxTransfer(uint16_t *pTxBuffer, uint32_t wTxSize)
{
	_ILI9488_EbiDmaUpdateBuffer(pTxBuffer, wTxSize, 0, 0);
	//SCB_CleanDCache_by_Addr((uint32_t *)&ili9488EbiDma,sizeof(ili9488EbiDma));
	SCB_CleanDCache_by_Addr((uint32_t *)pTxBuffer, wTxSize * 2);

	if (XDMAD_StartTransfer(ili9488EbiDma.xdmaD,
							 ili9488EbiDma.ili9488DmaTxChannel)) {
		printf("111");
		return ILI9488_ERROR_DMA_TRANSFER;
	}

	while (!ili9488DmaCtlInEbiMode.txDoneFlag);

	ili9488DmaCtlInEbiMode.txDoneFlag = 0;

	return 0;
}

/**
 * \brief Start ILI9488 DMA Rx transfer in SMC mode.
 * \param pRxBuffer point to Rx buffer address
 * \param wRxSize Rx buffer size in byte
 * \returns 0 if the xDMA transfer successfully; otherwise returns
 * ILI9488_DMA_ERROR_XXX.
 */
COMPILER_ALIGNED(32) static uint16_t dummyTxBuffer[5];
uint8_t ILI9488_EbiDmaRxTransfer(uint32_t *pRxBuffer, uint32_t wRxSize)
{
	SCB_CleanDCache_by_Addr((uint32_t *)pRxBuffer, wRxSize * 4);
	_ILI9488_EbiDmaUpdateBuffer(dummyTxBuffer, wRxSize, pRxBuffer, wRxSize);

	if (XDMAD_StartTransfer(ili9488EbiDma.xdmaD,
							 ili9488EbiDma.ili9488DmaRxChannel))
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
COMPILER_ALIGNED(32) static uint16_t command;
uint8_t ILI9488_EbiSendCommand(uint16_t Instr, uint16_t *pTxData,
							   uint32_t *pRxData, AccessIli_t ReadWrite, uint32_t size)
{
	command = Instr;

	if (ReadWrite == AccessInst) {
		PIO_Clear(&lcd_ebi_cds_pin);
		ili9488DmaCtlInEbiMode.cmdOrDataFlag = 1;
		return ILI9488_EbiDmaTxTransfer(&command, 1);
	} else if (ReadWrite == AccessWrite) {
		PIO_Clear(&lcd_ebi_cds_pin);
		ili9488DmaCtlInEbiMode.cmdOrDataFlag = 1;
		ILI9488_EbiDmaTxTransfer(&command, 1);

		if (size == 0) return 0;

		ILI9488_EbiDmaTxTransfer(pTxData, size);
		return 0;
	} else {
		ili9488DmaCtlInEbiMode.rxDoneFlag = 0;
		ILI9488_EbiDmaRxTransfer(pRxData, size);

		while (!ili9488DmaCtlInEbiMode.rxDoneFlag);
	}

	return 0;
}
#endif  //BOARD_LCD_ILI9488
