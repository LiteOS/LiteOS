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
#include "board.h"

#include "pin_mux.h"

#include "fsl_common.h"
#include "fsl_iocon.h"
#include "fsl_spi.h"

#include <stdbool.h>

#include "app_spiflash.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SPI_FLASH_CS1()    GPIO->B[0][2] = 1
#define SPI_FLASH_CS0()    GPIO->B[0][2] = 0

#define MX25R_BYTE_ADDR1(address) ((address >> 16) & 0xf)
#define MX25R_BYTE_ADDR2(address) ((address >> 8) & 0xf)
#define MX25R_BYTE_ADDR3(address) (address & 0xf)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
struct mx25r_instance mx25r;


/*******************************************************************************
 * Code
 ******************************************************************************/
/* initialize 'mx25r_instance' */
mx25r_err_t mx25r_init(struct mx25r_instance *instance, transfer_cb_t callback, void *callback_prv)
{
    instance->callback = callback;
    instance->prv = callback_prv;
    return mx25r_err_ok;
}


/* read 'rdid' to 'result' */
mx25r_err_t mx25r_cmd_rdid(struct mx25r_instance *instance, struct mx25r_rdid_result *result)
{
    instance->cmd[0] = 0x9f;
    instance->callback(instance->prv, instance->cmd, NULL, 1, false);
    instance->callback(instance->prv, NULL, (uint8_t *)result, sizeof(*result), true);
    return mx25r_err_ok;
}

/* read n bytes starting at 'address' */
mx25r_err_t mx25r_cmd_read(struct mx25r_instance *instance, uint32_t address, uint8_t *buffer, uint32_t size)
{
    if (address & 0xFF000000U)
    {
        return mx25r_err_out_of_range;
    }
    instance->cmd[0] = 0x03;
    instance->cmd[1] = MX25R_BYTE_ADDR1(address);
    instance->cmd[2] = MX25R_BYTE_ADDR2(address);
    instance->cmd[3] = MX25R_BYTE_ADDR3(address);
    instance->callback(instance->prv, instance->cmd, NULL, 4, false);
    instance->callback(instance->prv, NULL, (uint8_t *)buffer, size, true);
    return mx25r_err_ok;
}

/* no operation */
mx25r_err_t mx25r_cmd_nop(struct mx25r_instance *instance)
{
    instance->callback(instance->prv, instance->cmd, NULL, 1, true);
    return mx25r_err_ok;
}

/* read status register */
mx25r_err_t mx25r_cmd_rdsr(struct mx25r_instance *instance, struct mx25r_rdsr_result *result)
{
    instance->cmd[0] = 0x05;
    instance->callback(instance->prv, instance->cmd, NULL, 1, false);
    instance->callback(instance->prv, NULL, (uint8_t *)result, sizeof(*result), true);
    return mx25r_err_ok;
}

/* disable write operations */
mx25r_err_t mx25r_cmd_wrdi(struct mx25r_instance *instance)
{
    instance->cmd[0] = 0x04;
    instance->callback(instance->prv, instance->cmd, NULL, 1, true);
    return mx25r_err_ok;
}

/* enable write operations */
mx25r_err_t mx25r_cmd_wren(struct mx25r_instance *instance)
{
    instance->cmd[0] = 0x06;
    instance->callback(instance->prv, instance->cmd, NULL, 1, true);
    return mx25r_err_ok;
}

/* write n bytes (256 max) starting at 'address' aligned to 256 */
mx25r_err_t mx25r_cmd_write(struct mx25r_instance *instance,
                            uint32_t address_256_align,
                            uint8_t *buffer,
                            uint32_t size_256_max)
{
    struct mx25r_rdsr_result result;
    if (address_256_align & 0xFF000000U)
    {
        return mx25r_err_out_of_range;
    }
    if (address_256_align & 0xFFU)
    {
        return mx25r_err_alignement;
    }
    if (size_256_max > 256)
    {
        return mx25r_err_out_of_range;
    }
    /* enable write and wait until WEL is 1 */
    mx25r_cmd_wren(instance);
    do
    {
        mx25r_cmd_rdsr(instance, &result);
    } while (!(result.sr0 & 0x2));
    /* write sequence */
    instance->cmd[0] = 0x02;
    instance->cmd[1] = MX25R_BYTE_ADDR1(address_256_align);
    instance->cmd[2] = MX25R_BYTE_ADDR2(address_256_align);
    instance->cmd[3] = 0;
    instance->callback(instance->prv, instance->cmd, NULL, 4, false);
    instance->callback(instance->prv, (uint8_t *)buffer, NULL, size_256_max, true);
    /* wait until WRI is 0 and WEL is 0 */
    do
    {
        mx25r_cmd_rdsr(instance, &result);
    } while (result.sr0 & 0x3);
    return mx25r_err_ok;
}

/* erase sector at 'address' aligned to sector size = 4kB */
mx25r_err_t mx25r_cmd_sector_erase(struct mx25r_instance *instance, uint32_t address)
{
    struct mx25r_rdsr_result result;
    /* enable write and wait until WEL is 1 */
    mx25r_cmd_wren(instance);
    do
    {
        mx25r_cmd_rdsr(instance, &result);
    } while (!(result.sr0 & 0x2));
    /* write sequence */
    instance->cmd[0] = 0x20;
    instance->cmd[1] = MX25R_BYTE_ADDR1(address);
    instance->cmd[2] = MX25R_BYTE_ADDR2(address);
    instance->cmd[3] = MX25R_BYTE_ADDR3(address);
    instance->callback(instance->prv, instance->cmd, NULL, 4, true);
    /* wait until WRI is 0 and WEL is 0 */
    do
    {
        mx25r_cmd_rdsr(instance, &result);
    } while (result.sr0 & 0x3);
    return mx25r_err_ok;
}

int flash_transfer_cb(void *transfer_prv, uint8_t *tx_data, uint8_t *rx_data, size_t dataSize, bool eof)
{
//	  SPI_FLASH_CS0();
    spi_transfer_t xfer = {0};
    xfer.txData = tx_data;
    xfer.rxData = rx_data;
    xfer.dataSize = dataSize;
    /* terminate frame */
    if (eof)
    {
        xfer.configFlags |= kSPI_FrameAssert;
    }
    SPI_MasterTransferBlocking((SPI_Type *)transfer_prv, &xfer);
//		SPI_FLASH_CS1();
    return 0;
}


uint8_t spiflash_init(void)
{
	spi_master_config_t masterConfig = {0};
	struct mx25r_rdid_result gIDResult;
		
	/* attach 12 MHz clock to SPI2 */
	CLOCK_AttachClk(kFRO12M_to_FLEXCOMM2);
  /* reset FLEXCOMM for SPI */
  RESET_PeripheralReset(kFC2_RST_SHIFT_RSTn);
	
	/* SPI2 pins */
	IOCON_PinMuxSet(IOCON, 0,  2, (IOCON_FUNC2 | IOCON_MODE_PULLUP | IOCON_GPIO_MODE | IOCON_DIGITAL_EN));  /* SPI2_CS - FLASH */
	IOCON_PinMuxSet(IOCON, 0, 10, (IOCON_FUNC1 | IOCON_MODE_PULLUP | IOCON_GPIO_MODE | IOCON_DIGITAL_EN));  /* SPI2_SCK        */
	IOCON_PinMuxSet(IOCON, 0,  8, (IOCON_FUNC1 | IOCON_MODE_PULLUP | IOCON_GPIO_MODE | IOCON_DIGITAL_EN));  /* SPI2_MOSI       */
	IOCON_PinMuxSet(IOCON, 0,  9, (IOCON_FUNC1 | IOCON_MODE_PULLUP | IOCON_GPIO_MODE | IOCON_DIGITAL_EN));  /* SPI2_MISO       */
	
	SPI_MasterGetDefaultConfig(&masterConfig);
	masterConfig.direction = kSPI_MsbFirst;
	masterConfig.polarity = kSPI_ClockPolarityActiveHigh;
	masterConfig.phase = kSPI_ClockPhaseFirstEdge;
	masterConfig.baudRate_Bps = 100000;
	masterConfig.sselNum = (spi_ssel_t)3; // use GPIO as CS is prefer
	SPI_MasterInit(SPI2, &masterConfig, CLOCK_GetFreq(kCLOCK_Flexcomm2));

  mx25r_init(&mx25r, flash_transfer_cb, SPI2);
	mx25r_cmd_rdid(&mx25r, &gIDResult);
	
	if( (gIDResult.manufacturer == 0x51) && (gIDResult.device[0] == 0x40) && (gIDResult.device[1] == 0x15) )
	{
		return 1;
	}
	return 0;
}

// end file
