/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"

#include "board.h"

#include "pin_mux.h"

#include "fsl_common.h"
#include "fsl_iocon.h"

#include <stdbool.h>

#include "fsl_dma.h"
#include "fsl_i2c.h"
#include "fsl_i2s.h"
#include "fsl_i2s_dma.h"
#include "music.h"

#include "app_wm8904.h"
#include "app_led.h"

#define I2S_TX  I2S1
#define I2S_RX  I2S0

#define I2S_DMA_TX  15
#define I2S_DMA_RX  12

__ALIGN_BEGIN static uint8_t s_Buffer[400] __ALIGN_END; /* 100 samples => time about 2 ms */
static dma_handle_t s_DmaTxHandle;
static dma_handle_t s_DmaRxHandle;
static i2s_config_t s_TxConfig;
static i2s_config_t s_RxConfig;
static i2s_dma_handle_t s_TxHandle;
static i2s_dma_handle_t s_RxHandle;
static i2s_transfer_t s_TxTransfer;
static i2s_transfer_t s_RxTransfer;

wm8904_config_t codecConfig;
wm8904_handle_t codecHandle;
		
const pll_setup_t pllSetup = {
		.syspllctrl = SYSCON_SYSPLLCTRL_BANDSEL_MASK | SYSCON_SYSPLLCTRL_SELP(0x1FU) | SYSCON_SYSPLLCTRL_SELI(0x8U),
		.syspllndec = SYSCON_SYSPLLNDEC_NDEC(0x2DU),
		.syspllpdec = SYSCON_SYSPLLPDEC_PDEC(0x42U),
		.syspllssctrl = {SYSCON_SYSPLLSSCTRL0_MDEC(0x34D3U) | SYSCON_SYSPLLSSCTRL0_SEL_EXT_MASK, 0x00000000U},
		.pllRate = 24576000U, /* 16 bits * 2 channels * 48 kHz * 16 */
		.flags = PLL_SETUPFLAG_WAITLOCK};

		
static const uint8_t allRegisters[] = 
{
    0x00, 0x04, 0x05, 0x06, 0x07, 0x0A, 0x0C, 0x0E, 0x0F, 0x12, 0x14, 0x15, 0x16, 0x18, 0x19, 0x1A, 0x1B,
    0x1E, 0x1F, 0x20, 0x21, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x39,
    0x3A, 0x3B, 0x3C, 0x3D, 0x43, 0x44, 0x45, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x5A, 0x5E, 0x62,
    0x68, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7E, 0x7F,
    0x80, 0x81, 0x82, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93,
    0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0xC6, 0xF7, 0xF8
};


static status_t WM8904_WriteRegister(wm8904_handle_t *handle, uint8_t reg, uint16_t value)
{
    status_t result;
    i2c_master_transfer_t transfer;
    uint8_t buffer[3];

    buffer[0] = reg;
    buffer[1] = (value >> 8U) & 0xFFU;
    buffer[2] = value & 0xFFU;

    transfer.flags = kI2C_TransferDefaultFlag;
    transfer.slaveAddress = WM8904_I2C_ADDRESS;
    transfer.direction = kI2C_Write;
    transfer.subaddress = 0U;
    transfer.subaddressSize = 0U;
    transfer.data = buffer;
    transfer.dataSize = sizeof(buffer);

    result = I2C_MasterTransferBlocking(handle->i2c, &transfer);
    if (result == kStatus_Success)
    {
        return kStatus_WM8904_Success;
    }
    else
    {
        return kStatus_WM8904_Fail;
    }
}

static status_t WM8904_ReadRegister(wm8904_handle_t *handle, uint8_t reg, uint16_t *value)
{
    status_t result;
    i2c_master_transfer_t transfer;
    uint8_t buffer[2];

    *value = 0x0000U;

    transfer.flags = kI2C_TransferDefaultFlag;
    transfer.slaveAddress = WM8904_I2C_ADDRESS;
    transfer.direction = kI2C_Read;
    transfer.subaddress = (uint32_t)reg;
    transfer.subaddressSize = 1U;
    transfer.data = buffer;
    transfer.dataSize = sizeof(buffer);

    result = I2C_MasterTransferBlocking(handle->i2c, &transfer);
    if (result != kStatus_Success)
    {
        return kStatus_WM8904_Fail;
    }

    *value = (uint16_t)((((uint32_t)buffer[0]) << 8U) | ((uint32_t)buffer[1]));

    return kStatus_WM8904_Success;
}

static status_t WM8904_ModifyRegister(wm8904_handle_t *handle, uint8_t reg, uint16_t mask, uint16_t value)
{
    status_t result;
    uint16_t regValue;

    result = WM8904_ReadRegister(handle, reg, &regValue);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    regValue &= (uint16_t)~mask;
    regValue |= value;

    return WM8904_WriteRegister(handle, reg, regValue);
}

static status_t WM8904_WaitOnWriteSequencer(wm8904_handle_t *handle)
{
    status_t result;
    uint16_t value;

    do
    {
        result = WM8904_ReadRegister(handle, WM8904_WRT_SEQUENCER_4, &value);
    } while ((result == kStatus_WM8904_Success) && (value & 1));

    return result;
}

static void TxCallback(I2S_Type *base, i2s_dma_handle_t *handle, status_t completionStatus, void *userData)
{
    /* Enqueue the same original buffer all over again */
    i2s_transfer_t *transfer = (i2s_transfer_t *)userData;
    I2S_TxTransferSendDMA(base, handle, *transfer);
}

static void RxCallback(I2S_Type *base, i2s_dma_handle_t *handle, status_t completionStatus, void *userData)
{
    /* Enqueue the same original buffer all over again */
    i2s_transfer_t *transfer = (i2s_transfer_t *)userData;
    I2S_RxTransferReceiveDMA(base, handle, *transfer);
}


static void StartSoundPlayback(void)
{
    PRINTF("Setup looping playback of sine wave\r\n");

    s_TxTransfer.data = &g_Music[0];
    s_TxTransfer.dataSize = sizeof(g_Music);

    I2S_TxTransferCreateHandleDMA(I2S_TX, &s_TxHandle, &s_DmaTxHandle, TxCallback, (void *)&s_TxTransfer);
    I2S_TxTransferSendDMA(I2S_TX, &s_TxHandle, s_TxTransfer);
    I2S_TxTransferSendDMA(I2S_TX, &s_TxHandle, s_TxTransfer); /* TODO needed to queue twice due to current state of driver */
}

static void StartDigitalLoopback(void)
{
    PRINTF("Setup digital loopback\r\n");

    s_TxTransfer.data = &s_Buffer[0];
    s_TxTransfer.dataSize = sizeof(s_Buffer);

    s_RxTransfer.data = &s_Buffer[0];
    s_RxTransfer.dataSize = sizeof(s_Buffer);

    I2S_TxTransferCreateHandleDMA(I2S_TX, &s_TxHandle, &s_DmaTxHandle, TxCallback, (void *)&s_TxTransfer);
    I2S_RxTransferCreateHandleDMA(I2S_RX, &s_RxHandle, &s_DmaRxHandle, RxCallback, (void *)&s_RxTransfer);

    I2S_RxTransferReceiveDMA(I2S_RX, &s_RxHandle, s_RxTransfer);
    I2S_RxTransferReceiveDMA(I2S_RX, &s_RxHandle, s_RxTransfer); /* TODO needed to queue twice due to current state of driver */
    I2S_TxTransferSendDMA   (I2S_TX, &s_TxHandle, s_TxTransfer);
    I2S_TxTransferSendDMA   (I2S_TX, &s_TxHandle, s_TxTransfer); /* TODO needed to queue twice due to current state of driver */
}

status_t WM8904_Init(wm8904_handle_t *handle, wm8904_config_t *config)
{
    status_t result;

    /* reset */
    result = WM8904_WriteRegister(handle, WM8904_RESET, 0x0000);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    /* MCLK_INV=0, SYSCLK_SRC=0, TOCLK_RATE=0, OPCLK_ENA=1,
     * CLK_SYS_ENA=1, CLK_DSP_ENA=1, TOCLK_ENA=1 */
    result = WM8904_WriteRegister(handle, WM8904_CLK_RATES_2, 0x000F);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    /* WSEQ_ENA=1, WSEQ_WRITE_INDEX=0_0000 */
    result = WM8904_WriteRegister(handle, WM8904_WRT_SEQUENCER_0, 0x0100);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    /* WSEQ_ABORT=0, WSEQ_START=1, WSEQ_START_INDEX=00_0000 */
    result = WM8904_WriteRegister(handle, WM8904_WRT_SEQUENCER_3, 0x0100);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    result = WM8904_WaitOnWriteSequencer(handle);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    /* TOCLK_RATE_DIV16=0, TOCLK_RATE_x4=1, SR_MODE=0, MCLK_DIV=1
     * (Required for MMCs: SGY, KRT see erratum CE000546) */
    result = WM8904_WriteRegister(handle, WM8904_CLK_RATES_0, 0xA45F);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    /* INL_ENA=1, INR ENA=1 */
    result = WM8904_WriteRegister(handle, WM8904_POWER_MGMT_0, 0x0003);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    /* HPL_PGA_ENA=1, HPR_PGA_ENA=1 */
    result = WM8904_WriteRegister(handle, WM8904_POWER_MGMT_2, 0x0003);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    /* DACL_ENA=1, DACR_ENA=1, ADCL_ENA=1, ADCR_ENA=1 */
    result = WM8904_WriteRegister(handle, WM8904_POWER_MGMT_6, 0x000F);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    /* ADC_OSR128=1 */
    result = WM8904_WriteRegister(handle, WM8904_ANALOG_ADC_0, 0x0001);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    /* CLK_SYS_RAT=0101 (512/fs) SAMPLE_RATE=101 (44.1kHz /48kHz) */
    result = WM8904_WriteRegister(handle, WM8904_CLK_RATES_1, 0x1405);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    /* DACL_DATINV=0, DACR_DATINV=0, DAC_BOOST=00, LOOPBACK=0, AIFADCL_SRC=0,
     * AIFADCR_SRC=1, AIFDACL_SRC=0, AIFDACR_SRC=1, ADC_COMP=0, ADC_COMPMODE=0,
     * DAC_COMP=0, DAC_COMPMODE=0 */
    result = WM8904_WriteRegister(handle, WM8904_AUDIO_IF_0, 0x0050);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    /* BCLK_DIR=0 (input), AIF_WL=00 (16-bits) */
    result = WM8904_WriteRegister(handle, WM8904_AUDIO_IF_1, 0x0002);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    /* OPCLK_DIV=0 (sysclk), BCLK_DIV=0c (sysclk/16) */
    result = WM8904_WriteRegister(handle, WM8904_AUDIO_IF_2, 0x000c);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    /* LRCLK_DIR=0 (input), LRCLK_RATE=0010_0000_0000 (BCLK / 32) */
    result = WM8904_WriteRegister(handle, WM8904_AUDIO_IF_3, 0x0020);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    /* DAC_MONO=0, DAC_SB_FILT-0, DAC_MUTERATE=0, DAC_UNMUTE RAMP=0,
     * DAC_OSR128=1, DAC_MUTE=0, DEEMPH=0 (none) */
    result = WM8904_WriteRegister(handle, WM8904_DAC_DIG_1, 0x0040);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    /* INL_CM_ENA=0, L_IP_SEL_N=10, L_IP_SEL_P=01, L_MODE=00 */
    result = WM8904_WriteRegister(handle, WM8904_ANALOG_LEFT_IN_1, 0x0014);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    /* INR CM_ENA=0, R_IP_SEL_N=10, R_IP_SEL_P=01, R_MODE=00 */
    result = WM8904_WriteRegister(handle, WM8904_ANALOG_RIGHT_IN_1, 0x0014);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    /* LINMUTE=0, LIN_VOL=0_0101 */
    result = WM8904_WriteRegister(handle, WM8904_ANALOG_LEFT_IN_0, 0x0005);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    /* RINMUTE=0, RIN VOL=0_0101 LINEOUTL RMV SHORT-1, LINEOUTL ENA_OUTP=1,
     * LINEOUTL_ENA_DLY=1, LINEOUTL_ENA=1, LINEOUTR_RMV_SHORT-1,
     * LINEOUTR_ENA_OUTP=1 */
    result = WM8904_WriteRegister(handle, WM8904_ANALOG_RIGHT_IN_0, 0x0005);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    /* HPL_BYP_ENA=0, HPR_BYP_ENA=0, LINEOUTL_BYP ENA=0, LINEOUTR_BYP ENA=0 */
    result = WM8904_WriteRegister(handle, WM8904_ANALOG_OUT12_ZC, 0x0000);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    /* HPOUTL_MUTE=0, HPOUT_VU=0, HPOUTLZC=0, HPOUTL_VOL=11_1001 */
    result = WM8904_WriteRegister(handle, WM8904_ANALOG_OUT1_LEFT, 0x0039);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    /* HPOUTR_MUTE=0, HPOUT_VU=0, HPOUTRZC=0, HPOUTR_VOL=11_1001 */
    result = WM8904_WriteRegister(handle, WM8904_ANALOG_OUT1_RIGHT, 0x0039);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    /* Enable DC servos for headphone out */
    result = WM8904_WriteRegister(handle, WM8904_DC_SERVO_0, 0x0003);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    /* HPL_RMV_SHORT=1, HPL_ENA_OUTP=1, HPL_ENA_DLY=1, HPL_ENA=1,
     * HPR_RMV_SHORT=1, HPR_ENA_OUTP=1, HPR_ENA_DLY=1, HPR_ENA=1 */
    result = WM8904_WriteRegister(handle, WM8904_ANALOG_HP_0, 0x00FF);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    /* CP_DYN_PWR=1 */
    result = WM8904_WriteRegister(handle, WM8904_CLS_W_0, 0x0001);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    /* CP_ENA=1 */
    result = WM8904_WriteRegister(handle, WM8904_CHRG_PUMP_0, 0x0001);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    result = WM8904_SetMasterSlave(handle, config->master);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    result = WM8904_SetProtocol(handle, config->protocol);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    result = WM8904_SetAudioFormat(handle, &(config->format));
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    return kStatus_WM8904_Success;
}

status_t WM8904_Deinit(wm8904_handle_t *handle)
{
    /* reset */
    return WM8904_WriteRegister(handle, WM8904_RESET, 0x0000);
}

void WM8904_GetDefaultConfig(wm8904_config_t *config)
{
    memset(config, 0, sizeof(wm8904_config_t));

    config->master = false;
    config->protocol = kWM8904_ProtocolI2S;
    config->format.fsRatio = kWM8904_FsRatio64X;
    config->format.sampleRate = kWM8904_SampleRate48kHz;
    config->format.bitWidth = kWM8904_BitWidth16;
}

status_t WM8904_SetMasterSlave(wm8904_handle_t *handle, bool master)
{
    if (master)
    {
        /* only slave currently supported */
        return kStatus_WM8904_Fail;
    }

    return kStatus_WM8904_Success;
}

status_t WM8904_SetProtocol(wm8904_handle_t *handle, wm8904_protocol_t protocol)
{
    return WM8904_ModifyRegister(handle, WM8904_AUDIO_IF_1, 0x0003, (uint16_t)protocol);
}

status_t WM8904_SetAudioFormat(wm8904_handle_t *handle, wm8904_audio_format_t *format)
{
    status_t result;

    /* Disable SYSCLK */
    result = WM8904_WriteRegister(handle, WM8904_CLK_RATES_2, 0x00);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    /* Set Clock ratio and sample rate */
    result = WM8904_WriteRegister(handle, WM8904_CLK_RATES_1, ((uint32_t)format->fsRatio << 10) | format->sampleRate);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    /* Set bit resolution */
    result = WM8904_ModifyRegister(handle, WM8904_AUDIO_IF_1, 0x000C, (uint16_t)format->bitWidth);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    /* Enable SYSCLK */
    result = WM8904_WriteRegister(handle, WM8904_CLK_RATES_2, 0x1007);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    return kStatus_WM8904_Success;
}

status_t WM8904_SetVolume(wm8904_handle_t *handle, uint16_t volumeLeft, uint16_t volumeRight)
{
    status_t result;

    result = WM8904_ModifyRegister(handle, WM8904_ANALOG_OUT1_LEFT, 0x3F, volumeLeft);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    result = WM8904_ModifyRegister(handle, WM8904_ANALOG_OUT1_RIGHT, 0xBF, volumeRight | 0x0080);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    return kStatus_WM8904_Success;
}

status_t WM8904_SetMute(wm8904_handle_t *handle, bool muteLeft, bool muteRight)
{
    status_t result;
    uint16_t left = muteLeft ? 0x0100 : 0x0000;
    uint16_t right = muteRight ? 0x0100 : 0x0000;

    result = WM8904_ModifyRegister(handle, WM8904_ANALOG_OUT1_LEFT, 0x0100, left);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    result = WM8904_ModifyRegister(handle, WM8904_ANALOG_OUT1_RIGHT, 0x0180, right | 0x0080);
    if (result != kStatus_WM8904_Success)
    {
        return result;
    }

    return kStatus_WM8904_Success;
}

status_t WM8904_PrintRegisters(wm8904_handle_t *handle)
{
    status_t result;
    uint16_t value;
    uint32_t i;

    for (i = 0; i < sizeof(allRegisters); i++)
    {
        result = WM8904_ReadRegister(handle, allRegisters[i], &value);
        if (result != kStatus_WM8904_Success)
        {
            PRINTF("\r\n");
            return result;
        }
        PRINTF("%s", ((i % 8) == 0) ? "\r\n" : "\t");
        PRINTF("%02X:%04X", allRegisters[i], value);
    }

    PRINTF("\r\n");
    return result;
}

uint8_t wm8904_i2s_init(void)
{
// we need initial i2c here, but we initial the right i2c at pct2075 already
	
	/* Initialize PLL clock */
	CLOCK_AttachClk(kFRO12M_to_SYS_PLL);
	CLOCK_SetPLLFreq(&pllSetup);

	CLOCK_AttachClk(kSYS_PLL_to_FLEXCOMM6);
	CLOCK_AttachClk(kSYS_PLL_to_FLEXCOMM7);
	/* Attach PLL clock to MCLK for I2S, no divider */
	CLOCK_AttachClk(kSYS_PLL_to_MCLK);
	SYSCON->MCLKDIV = SYSCON_MCLKDIV_DIV(0U);
	/* reset FLEXCOMM for I2S */
	RESET_PeripheralReset(kFC6_RST_SHIFT_RSTn);
	RESET_PeripheralReset(kFC7_RST_SHIFT_RSTn);
	
	/* I2S */
// Flexcomm 6 I2S Rx
	IOCON_PinMuxSet(IOCON, 0, 5, IOCON_FUNC1 | IOCON_DIGITAL_EN); /* Flexcomm 6 / SDA */
	IOCON_PinMuxSet(IOCON, 0, 6, IOCON_FUNC1 | IOCON_DIGITAL_EN); /* Flexcomm 6 / WS */
	IOCON_PinMuxSet(IOCON, 0, 7, IOCON_FUNC1 | IOCON_DIGITAL_EN); /* Flexcomm 6 / SCK */
// Flexcomm 7 I2S Tx
	IOCON_PinMuxSet(IOCON, 1, 12, IOCON_FUNC4 | IOCON_DIGITAL_EN); /* Flexcomm 7 / SCK */
	IOCON_PinMuxSet(IOCON, 1, 13, IOCON_FUNC4 | IOCON_DIGITAL_EN);  /* Flexcomm 7 / SDA */
	IOCON_PinMuxSet(IOCON, 1, 14, IOCON_FUNC4 | IOCON_DIGITAL_EN);  /* Flexcomm 7 / WS */

	/* MCLK output for I2S */
	IOCON_PinMuxSet(IOCON, 1, 17, IOCON_FUNC4 | IOCON_MODE_INACT | IOCON_DIGITAL_EN);
	SYSCON->MCLKIO = 1U;
	/*
	 * enableMaster = true;
	 * baudRate_Bps = 100000U;
	 * enableTimeout = false;
	 */
	WM8904_GetDefaultConfig(&codecConfig);
	codecHandle.i2c = I2C4;
	if (WM8904_Init(&codecHandle, &codecConfig) != kStatus_Success)
	{
		PRINTF("WM8904_Init failed!\r\n");
		led_off(7);
	}
	else
	{
		led_on(7);
	}
	/* Initial volume kept low for hearing safety. */
	/* Adjust it to your needs, 0x0006 for -51 dB, 0x0039 for 0 dB etc. */
	WM8904_SetVolume(&codecHandle, 0x0006, 0x0006);
	
	I2S_TxGetDefaultConfig(&s_TxConfig);
	s_TxConfig.divider = CLOCK_GetPllOutFreq() / 48000U / 16U / 2U;	
	I2S_RxGetDefaultConfig(&s_RxConfig);
	
	I2S_TxInit(I2S_TX, &s_TxConfig);
	I2S_RxInit(I2S_RX, &s_RxConfig);
	
	DMA_Init(DMA0);

	DMA_EnableChannel(DMA0, I2S_DMA_TX);
	DMA_EnableChannel(DMA0, I2S_DMA_RX);
	DMA_SetChannelPriority(DMA0, I2S_DMA_TX, kDMA_ChannelPriority3);
	DMA_SetChannelPriority(DMA0, I2S_DMA_RX, kDMA_ChannelPriority2);
	DMA_CreateHandle(&s_DmaTxHandle, DMA0, I2S_DMA_TX);
	DMA_CreateHandle(&s_DmaRxHandle, DMA0, I2S_DMA_RX);

	if (true)
	{
		StartSoundPlayback();
	}
	else
	{
		StartDigitalLoopback();
	}
}


// end file
