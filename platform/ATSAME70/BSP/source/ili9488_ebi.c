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

/*----------------------------------------------------------------------------
 *        Local variables
 *----------------------------------------------------------------------------*/

/** Pio pins to configure. */
static const Pin lcd_ebi_reset_pin = LCD_EBI_PIN_RESET;
static const Pin lcd_ebi_pwm_pin = BOARD_EBI_LCD_PIN_BACKLIGHT;
/** Pins to configure for the application. */
static const Pin lcd_ebi_pins[] = BOARD_EBI_LCD_PINS;
/** Pins to configure for the application. */
static const Pin lcd_ebi_cds_pin = BOARD_EBI_LCD_PIN_CDS;

/*----------------------------------------------------------------------------
 *        Local functions
 *----------------------------------------------------------------------------*/
/**
 * \brief ILI9488 Hardware Initialization for SMC LCD.
 */
static void _ILI9488_EbiHW_Initialize(void)
{
	/* Pin configurations */
	PIO_Configure(&lcd_ebi_reset_pin, 1);
	PIO_Configure(&lcd_ebi_cds_pin, 1);
	PIO_Configure(lcd_ebi_pins, PIO_LISTSIZE(lcd_ebi_pins));
	PIO_Configure(&lcd_ebi_pwm_pin, 1);
	PIO_Set(&lcd_ebi_pwm_pin);
}

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/
COMPILER_ALIGNED(32) static uint32_t chipidBuf[5];
uint32_t ILI9488_EbiReadChipId (void)
{
	uint32_t chipid = 0;

	uint8_t i;
	uint32_t *ptr;
	uint32_t shift_cnt = 2;

	ILI9488_EbiSendCommand(ILI9488_CMD_READ_ID4, 0, 0, AccessInst, 0);
	ILI9488_EbiSendCommand(0, 0, chipidBuf, AccessRead, 4);
	ptr = &chipidBuf[1];

	for (i = 1; i < 4; i++) {
		chipid |= (*ptr & 0xFF) << (shift_cnt << 3);
		ptr++;
		shift_cnt--;
	}

	return chipid;
}

/**
 * \brief Set ILI9488 Pixel Format in SMC mode.
 * \param format Format of pixel
 */
void ILI9488_EbiSetPixelFormat(uint16_t format)
{
	ILI9488_EbiSendCommand(ILI9488_CMD_COLMOD_PIXEL_FORMAT_SET, &format, 0,
						   AccessWrite, 1);
}

/**
 * \brief Initialize the ILI9488 controller in SMC mode.
 */
uint32_t ILI9488_EbiInitialize(sXdmad *dmad)
{
	uint32_t chipid;
	uint16_t param;

	_ILI9488_EbiHW_Initialize();
	ILI9488_EbiInitializeWithDma(dmad);

	ILI9488_EbiSendCommand(ILI9488_CMD_SOFTWARE_RESET, 0, 0, AccessInst, 0);
	Wait(200);

	ILI9488_EbiSendCommand(ILI9488_CMD_SLEEP_OUT, 0, 0, AccessInst, 0);
	Wait(200);

	// make it tRGB and reverse the column order
	param = 0x48;
	ILI9488_EbiSendCommand(ILI9488_CMD_MEMORY_ACCESS_CONTROL, &param, 0,
						   AccessWrite, 1);
	Wait(100);

	param = 0x04;
	ILI9488_EbiSendCommand(ILI9488_CMD_CABC_CONTROL_9, &param, 0, AccessWrite, 1);

	if ((chipid = ILI9488_EbiReadChipId()) != ILI9488_DEVICE_CODE) {
		printf("Read ILI9488 chip ID (0x%04x) error, skip initialization.\r\n",
				(unsigned int)chipid);
		return 1;
	}

	ILI9488_EbiSetPixelFormat(5);
	ILI9488_EbiSendCommand(ILI9488_CMD_NORMAL_DISP_MODE_ON, 0, 0, AccessInst, 0);
	ILI9488_EbiSendCommand(ILI9488_CMD_DISPLAY_ON, 0, 0, AccessInst, 0);
	return 0;
}

/**
 * \brief ILI9488 configure cursor in SMC mode.
 * \Param x X position.
 * \Param y Y position.
 */
void ILI9488_EbiSetCursor(uint16_t x, uint16_t y)
{
	/* Set Horizontal Address Start Position */
	uint32_t cnt = 0;

	uint16_t buf[4];
	cnt = sizeof(buf) / sizeof(uint16_t);

	buf[0] = get_8b_to_16b(x);
	buf[1] = get_0b_to_8b(x);
	x += 1;
	buf[2] = get_8b_to_16b(x);
	buf[3] = get_0b_to_8b(x);
	ILI9488_EbiSendCommand(ILI9488_CMD_COLUMN_ADDRESS_SET, (uint16_t *)buf, 0,
						   AccessWrite, cnt);
	ILI9488_EbiSendCommand(ILI9488_CMD_NOP, 0, 0, AccessInst, 0);


	/* Set Horizontal Address End Position */
	buf[0] = get_8b_to_16b(y);
	buf[1] = get_0b_to_8b(y);
	y += 1;
	buf[2] = get_8b_to_16b(y);
	buf[3] = get_0b_to_8b(y);
	ILI9488_EbiSendCommand(ILI9488_CMD_PAGE_ADDRESS_SET, (uint16_t *)buf, 0,
						   AccessWrite, cnt);
	ILI9488_EbiSendCommand(ILI9488_CMD_NOP, 0, 0, AccessInst, 0);
}

/**
 * \brief ILI9488 configure window.
 * \Param dwX X start position.
 * \Param dwX Y start position.
 * \Param dwWidth  Width of window.
 * \Param dwHeight Height of window.
 */
COMPILER_ALIGNED(32) static uint16_t buf[4];
void ILI9488_EbiSetWindow(
	uint16_t dwX, uint16_t dwY, uint16_t dwWidth, uint16_t dwHeight)
{
	uint16_t ColStart, ColEnd, RowStart, RowEnd;

	uint32_t cnt = 0;


	cnt = sizeof(buf) / sizeof(uint16_t);

	ColStart  =  dwX;
	ColEnd    =  dwWidth + dwX;

	RowStart = dwY;
	RowEnd   = dwHeight + dwY;

	buf[0] = get_8b_to_16b(ColStart);
	buf[1] = get_0b_to_8b(ColStart);
	buf[2] = get_8b_to_16b(ColEnd);
	buf[3] = get_0b_to_8b(ColEnd);
	ILI9488_EbiSendCommand(ILI9488_CMD_COLUMN_ADDRESS_SET, (uint16_t *)buf, 0,
						   AccessWrite, cnt);
	ILI9488_EbiSendCommand(ILI9488_CMD_NOP, 0, 0, AccessInst, 0);

	/* Set Horizontal Address End Position */
	buf[0] = get_8b_to_16b(RowStart);
	buf[1] = get_0b_to_8b(RowStart);
	buf[2] = get_8b_to_16b(RowEnd);
	buf[3] = get_0b_to_8b(RowEnd);
	ILI9488_EbiSendCommand(ILI9488_CMD_PAGE_ADDRESS_SET, (uint16_t *)buf, 0,
						   AccessWrite, cnt);
	ILI9488_EbiSendCommand(ILI9488_CMD_NOP, 0, 0, AccessInst, 0);
}

/**
 * \brief ILI9488 configure window with full size.
 */
void ILI9488_EbiSetFullWindow(void)
{
	uint16_t c_start, c_end, r_start, r_end;
	uint32_t cnt = 0;

	cnt = sizeof(buf) / sizeof(uint16_t);

	c_start  =  0;
	c_end    =  ILI9488_LCD_WIDTH - 1;

	r_start = 0;
	r_end   = ILI9488_LCD_HEIGHT - 1;

	/* Set Horizontal Address Start Position */
	buf[0] = get_8b_to_16b(c_start);
	buf[1] = get_0b_to_8b(c_start);
	buf[2] = get_8b_to_16b(c_end);
	buf[3] = get_0b_to_8b(c_end);
	ILI9488_EbiSendCommand(ILI9488_CMD_COLUMN_ADDRESS_SET, (uint16_t *)buf, 0,
						   AccessWrite, cnt);
	ILI9488_EbiSendCommand(ILI9488_CMD_NOP , 0, 0, AccessInst, 0);

	/* Set Horizontal Address End Position */
	buf[0] = get_8b_to_16b(r_start);
	buf[1] = get_0b_to_8b(r_start);
	buf[2] = get_8b_to_16b(r_end);
	buf[3] = get_0b_to_8b(r_end);
	ILI9488_EbiSendCommand(ILI9488_CMD_COLUMN_ADDRESS_SET, (uint16_t *)buf, 0,
						   AccessWrite, cnt);
	ILI9488_EbiSendCommand(ILI9488_CMD_NOP , 0, 0, AccessInst, 0);
}


/**
 * \brief Turn on the ILI9488.
 */
void ILI9488_EbiOn(void)
{
	ILI9488_EbiSendCommand(ILI9488_CMD_PIXEL_OFF, 0, 0, AccessInst, 0);
	ILI9488_EbiSendCommand(ILI9488_CMD_DISPLAY_ON, 0, 0, AccessInst, 0);
	ILI9488_EbiSendCommand(ILI9488_CMD_NORMAL_DISP_MODE_ON, 0, 0, AccessInst, 0);
}

/**
 * \brief Turn off the ILI9488.
 */
void ILI9488_EbiOff(void)
{
	ILI9488_EbiSendCommand(ILI9488_CMD_DISPLAY_OFF, 0, 0, AccessInst, 0);
}

/**
 * \brief ILI9488 configure landscape.
 * \Param dwRGB RGB mode.
 * \Param LandscaprMode Landscape Mode.
 */
void ILI9488_EbiSetDisplayLandscape(uint8_t dwRGB, uint8_t LandscapeMode)
{
	uint16_t value;

	if (LandscapeMode)   {
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

	ILI9488_EbiSendCommand(ILI9488_CMD_MEMORY_ACCESS_CONTROL, &value, 0,
						   AccessWrite, sizeof(value));
}

#endif
