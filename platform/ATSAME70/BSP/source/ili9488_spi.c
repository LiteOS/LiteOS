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
 * Implementation of ILI9488 driver.
 *
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/
#include "board.h"
#include <string.h>
#include <stdio.h>

#ifdef BOARD_LCD_ILI9488

#define ILI9488_SPI     SPI0

/*----------------------------------------------------------------------------
 *        Local variables
 *----------------------------------------------------------------------------*/

/** Pio pins to configure. */
static const Pin lcd_spi_reset_pin = LCD_SPI_PIN_RESET;
static const Pin lcd_spi_pwm_pin = BOARD_SPI_LCD_PIN_BACKLIGHT;
/** Pins to configure for the application. */
static const Pin lcd_pins[] = BOARD_SPI_LCD_PINS;
/** Pins to configure for the application. */
static const Pin lcd_spi_cds_pin = BOARD_SPI_LCD_PIN_CDS;

/*----------------------------------------------------------------------------
 *        Local functions
 *----------------------------------------------------------------------------*/
/**
 * \brief ILI9488 Hardware Initialization for SPI/SMC LCD.
 */
static void _ILI9488_Spi_HW_Initialize(void)
{
	/* Pin configurations */
	PIO_Configure(&lcd_spi_reset_pin, 1);
	PIO_Configure(&lcd_spi_cds_pin, 1);
	PIO_Configure(lcd_pins, PIO_LISTSIZE(lcd_pins));
	PIO_Configure(&lcd_spi_pwm_pin, 1);

	/* Enable PWM peripheral clock */
	PMC_EnablePeripheral(ID_PWM0);
	PMC_EnablePeripheral(ID_SPI0);
	/* Set clock A and clock B */
	// set for 14.11 KHz for CABC control
	//mode = PWM_CLK_PREB(0x0A) | (PWM_CLK_DIVB(110)) |
	//PWM_CLK_PREA(0x0A) | (PWM_CLK_DIVA(110));
	PWMC_ConfigureClocks(PWM0, 14200, 0,  BOARD_MCK);

	/* Configure PWM channel 1 for LED0  */
	PWMC_DisableChannel(PWM0, CHANNEL_PWM_LCD);

	PWMC_ConfigureChannel(PWM0, CHANNEL_PWM_LCD, PWM_CMR_CPRE_CLKA, 0,
						  PWM_CMR_CPOL);
	PWMC_SetPeriod(PWM0, CHANNEL_PWM_LCD, 16);
	PWMC_SetDutyCycle(PWM0, CHANNEL_PWM_LCD, 8);
	PWMC_EnableChannel(PWM0, CHANNEL_PWM_LCD);

	SPI_Configure(ILI9488_SPI, ID_SPI0, (SPI_MR_MSTR | SPI_MR_MODFDIS
										 | SPI_PCS(SMC_EBI_LCD_CS)));
	SPI_ConfigureNPCS(ILI9488_SPI,
					  SMC_EBI_LCD_CS,
					  SPI_CSR_CPOL | SPI_CSR_BITS_8_BIT | SPI_DLYBS(6, BOARD_MCK)
					  | SPI_DLYBCT(100, BOARD_MCK) | SPI_SCBR(20000000, BOARD_MCK));
	SPI_Enable(ILI9488_SPI);
}

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/
uint32_t ILI9488_SpiReadChipId (void)
{
	uint32_t chipid = 0;
	uint32_t chipidBuf[2];
	uint8_t i, reg, param;

	reg = 0x81;
	param = 0x0;

	for (i = 3; i > 0; i--) {

		ILI9488_SpiSendCommand(ILI9488_CMD_SPI_READ_SETTINGS, &reg, 0, AccessWrite, 1);
		reg++;
		ILI9488_SpiSendCommand(ILI9488_CMD_READ_ID4, 0, 0, AccessInst, 0);
		ILI9488_SpiSendCommand(0, 0, chipidBuf, AccessRead, 2);
		chipid |= (chipidBuf[1] & 0xFF) << ((i - 1) << 3);
		ILI9488_SpiSendCommand(ILI9488_CMD_SPI_READ_SETTINGS, &param, 0, AccessWrite,
							   1);
	}

	return chipid;
}

/**
 * \brief Set ILI9488 Pixel Format in SPI/SMC mode.
 * \param format Format of pixel
 */
void ILI9488_SpiSetPixelFormat(uint8_t format)
{
	ILI9488_SpiSendCommand(ILI9488_CMD_COLMOD_PIXEL_FORMAT_SET, &format, 0,
						   AccessWrite, 1);
}

/**
 * \brief Initialize the ILI9488 controller in SPI/SMC mode.
 */
uint32_t ILI9488_SpiInitialize(sXdmad *dmad)
{
	uint32_t chipid = 0;
	uint8_t param;

	_ILI9488_Spi_HW_Initialize();
	ILI9488_SpiInitializeWithDma(dmad);

	ILI9488_SpiSendCommand(ILI9488_CMD_SOFTWARE_RESET, 0, 0, AccessInst, 0);
	Wait(200);

	ILI9488_SpiSendCommand(ILI9488_CMD_SLEEP_OUT, 0, 0, AccessInst, 0);
	Wait(200);

	// make it tRGB and reverse the column order
	param = 0x48;
	ILI9488_SpiSendCommand(ILI9488_CMD_MEMORY_ACCESS_CONTROL, &param, 0,
						   AccessWrite, 1);
	Wait(100);

	param = 0x04;
	ILI9488_SpiSendCommand(ILI9488_CMD_CABC_CONTROL_9, &param, 0, AccessWrite, 1);

	if ((chipid = ILI9488_SpiReadChipId()) != ILI9488_DEVICE_CODE) {
		printf("Read ILI9488 chip ID (0x%04x) error, skip initialization.\r\n",
				(unsigned int)chipid);
		return 1;
	}

	ILI9488_SpiSetPixelFormat(6);
	ILI9488_SpiSendCommand(ILI9488_CMD_NORMAL_DISP_MODE_ON, 0, 0, AccessInst, 0);
	ILI9488_SpiSendCommand(ILI9488_CMD_DISPLAY_ON, 0, 0, AccessInst, 0);
	return 0;
}

/**
 * \brief ILI9488 configure cursor in SPI/SMC mode.
 * \Param x X position.
 * \Param y Y position.
 */
void ILI9488_SpiSetCursor(uint16_t x, uint16_t y)
{
	/* Set Horizontal Address Start Position */
	uint32_t cnt = 0;

	uint8_t buf[4];
	cnt = sizeof(buf);
	buf[0] = get_8b_to_16b(x);
	buf[1] = get_0b_to_8b(x);
	x += 1;
	buf[2] = get_8b_to_16b(x);
	buf[3] = get_0b_to_8b(x);
	ILI9488_SpiSendCommand(ILI9488_CMD_COLUMN_ADDRESS_SET, (uint8_t *)buf, 0,
						   AccessWrite, cnt);
	ILI9488_SpiSendCommand(ILI9488_CMD_NOP, 0, 0, AccessInst, 0);

	/* Set Horizontal Address End Position */
	buf[0] = get_8b_to_16b(y);
	buf[1] = get_0b_to_8b(y);
	y += 1;
	buf[2] = get_8b_to_16b(y);
	buf[3] = get_0b_to_8b(y);
	ILI9488_SpiSendCommand(ILI9488_CMD_PAGE_ADDRESS_SET, (uint8_t *)buf, 0,
						   AccessWrite, cnt);
	ILI9488_SpiSendCommand(ILI9488_CMD_NOP, 0, 0, AccessInst, 0);
}

/**
 * \brief ILI9488 configure window.
 * \Param dwX X start position.
 * \Param dwX Y start position.
 * \Param dwWidth  Width of window.
 * \Param dwHeight Height of window.
 */
void ILI9488_SpiSetWindow(uint16_t dwX, uint16_t dwY, uint16_t dwWidth,
						   uint16_t dwHeight)
{
	uint16_t ColStart, ColEnd, RowStart, RowEnd;
	uint32_t cnt = 0;
	uint8_t buf[4];
	cnt = sizeof(buf);
	ColStart  =  dwX;
	ColEnd    =  dwWidth + dwX;

	RowStart = dwY;
	RowEnd   = dwHeight + dwY;

	buf[0] = get_8b_to_16b(ColStart);
	buf[1] = get_0b_to_8b(ColStart);
	buf[2] = get_8b_to_16b(ColEnd);
	buf[3] = get_0b_to_8b(ColEnd);
	ILI9488_SpiSendCommand(ILI9488_CMD_COLUMN_ADDRESS_SET, (uint8_t *)buf, 0,
						   AccessWrite, cnt);
	ILI9488_SpiSendCommand(ILI9488_CMD_NOP, 0, 0, AccessInst, 0);

	/* Set Horizontal Address End Position */
	buf[0] = get_8b_to_16b(RowStart);
	buf[1] = get_0b_to_8b(RowStart);
	buf[2] = get_8b_to_16b(RowEnd);
	buf[3] = get_0b_to_8b(RowEnd);
	ILI9488_SpiSendCommand(ILI9488_CMD_PAGE_ADDRESS_SET, (uint8_t *)buf, 0,
						   AccessWrite, cnt);
	ILI9488_SpiSendCommand(ILI9488_CMD_NOP, 0, 0, AccessInst, 0);
}

/**
 * \brief ILI9488 configure window with full size.
 */
void ILI9488_SpiSetFullWindow(void)
{
	uint16_t c_start, c_end, r_start, r_end;
	uint32_t cnt = 0;
	uint8_t buf[4];
	cnt = sizeof(buf);
	c_start =  0;
	c_end =  ILI9488_LCD_WIDTH - 1;

	r_start = 0;
	r_end   = ILI9488_LCD_HEIGHT - 1;

	/* Set Horizontal Address Start Position */
	buf[0] = get_8b_to_16b(c_start);
	buf[1] = get_0b_to_8b(c_start);
	buf[2] = get_8b_to_16b(c_end);
	buf[3] = get_0b_to_8b(c_end);
	ILI9488_SpiSendCommand(ILI9488_CMD_COLUMN_ADDRESS_SET, (uint8_t *)buf, 0,
						   AccessWrite, cnt);
	ILI9488_SpiSendCommand(ILI9488_CMD_NOP , 0, 0, AccessInst, 0);

	/* Set Horizontal Address End Position */
	buf[0] = get_8b_to_16b(r_start);
	buf[1] = get_0b_to_8b(r_start);
	buf[2] = get_8b_to_16b(r_end);
	buf[3] = get_0b_to_8b(r_end);
	ILI9488_SpiSendCommand(ILI9488_CMD_COLUMN_ADDRESS_SET, (uint8_t *)buf, 0,
						   AccessWrite, cnt);
	ILI9488_SpiSendCommand(ILI9488_CMD_NOP , 0, 0, AccessInst, 0);
}

/**
 * \brief Turn on the ILI9488.
 */
void ILI9488_SpiOn(void)
{
	ILI9488_SpiSendCommand(ILI9488_CMD_PIXEL_OFF, 0, 0, AccessInst, 0);
	ILI9488_SpiSendCommand(ILI9488_CMD_DISPLAY_ON, 0, 0, AccessInst, 0);
	ILI9488_SpiSendCommand(ILI9488_CMD_NORMAL_DISP_MODE_ON, 0, 0, AccessInst, 0);
}

/**
 * \brief Turn off the ILI9488.
 */
void ILI9488_SpiOff(void)
{
	ILI9488_SpiSendCommand(ILI9488_CMD_DISPLAY_OFF, 0, 0, AccessInst, 0);
}

/**
 * \brief ILI9488 configure landscape.
 * \Param dwRGB RGB mode.
 * \Param LandscaprMode Landscape Mode.
 */
void ILI9488_SpiSetDisplayLandscape(uint8_t dwRGB, uint8_t LandscapeMode)
{
	uint8_t value;

	if (LandscapeMode) {
		if (dwRGB)
			value = 0xE8;
		else
			value = 0xE0;
	} else {
		if (dwRGB)
			value = 0x48;
		else
			value = 0x40;
	}

	ILI9488_SpiSendCommand(ILI9488_CMD_MEMORY_ACCESS_CONTROL, &value, 0,
						   AccessWrite, sizeof(value));
}

/**
 * \brief Send single command to ILI9488
 * \param cmd  command.
 */
static void ILI9488_SendCmd(uint8_t cmd)
{
	uint32_t i;
	PIO_Clear(&lcd_spi_cds_pin);
	SPI_Write(ILI9488_SPI, SMC_EBI_LCD_CS, cmd);

	for (i = 0; i < 0x18; i++);
}

/**
 * \brief Draw a pixel on LCD with given color, DMA not used
 * \param x  X-coordinate of pixel.
 * \param y  Y-coordinate of pixel.
 * \param color  Pixel color.
 */
void ILI9488_SetPixelColor(uint32_t x, uint32_t y, uint32_t color)
{
	uint32_t i;

	/* Set Horizontal Address Start Position */
	ILI9488_SendCmd(ILI9488_CMD_COLUMN_ADDRESS_SET);
	PIO_Set(&lcd_spi_cds_pin);
	SPI_Write(ILI9488_SPI, SMC_EBI_LCD_CS, get_8b_to_16b(x));
	SPI_Write(ILI9488_SPI, SMC_EBI_LCD_CS, get_0b_to_8b(x));
	x++;
	SPI_Write(ILI9488_SPI, SMC_EBI_LCD_CS, get_8b_to_16b(x));
	SPI_Write(ILI9488_SPI, SMC_EBI_LCD_CS, get_0b_to_8b(x));

	for (i = 0; i < 0x18; i++);

	ILI9488_SendCmd(ILI9488_CMD_NOP);

	/* Set Horizontal Address End Position */
	ILI9488_SendCmd(ILI9488_CMD_PAGE_ADDRESS_SET);
	PIO_Set(&lcd_spi_cds_pin);
	SPI_Write(ILI9488_SPI, SMC_EBI_LCD_CS, get_8b_to_16b(y));
	SPI_Write(ILI9488_SPI, SMC_EBI_LCD_CS, get_0b_to_8b(y));
	y++;
	SPI_Write(ILI9488_SPI, SMC_EBI_LCD_CS, get_8b_to_16b(y));
	SPI_Write(ILI9488_SPI, SMC_EBI_LCD_CS, get_0b_to_8b(y));

	for (i = 0; i < 0x18; i++);

	ILI9488_SendCmd(ILI9488_CMD_NOP);

	/* Set Color */
	ILI9488_SendCmd(ILI9488_CMD_MEMORY_WRITE);
	PIO_Set(&lcd_spi_cds_pin);
	SPI_Write(ILI9488_SPI, SMC_EBI_LCD_CS, get_16b_to_24b(color));
	SPI_Write(ILI9488_SPI, SMC_EBI_LCD_CS, get_8b_to_16b(color));
	SPI_Write(ILI9488_SPI, SMC_EBI_LCD_CS, get_0b_to_8b(color));

	for (i = 0; i < 0x18; i++);
}

#endif
