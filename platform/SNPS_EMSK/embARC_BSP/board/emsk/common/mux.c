/* ------------------------------------------
 * Copyright (c) 2016, Synopsys, Inc. All rights reserved.

 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:

 * 1) Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.

 * 2) Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.

 * 3) Neither the name of the Synopsys, Inc., nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.

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
 *
 * \version 2016.05
 * \date 2014-07-07
 * \author Huaqi Fang(Huaqi.Fang@synopsys.com)
--------------------------------------------- */
/**
 * \defgroup	BOARD_EMSK_DRV_MUX	EMSK Mux Driver
 * \ingroup	BOARD_EMSK_DRIVER
 * \brief	EMSK Mux Controller Driver
 * \details
 *		Mux controller is the hardware external PMOD port pin connection
 *	controller, it can distribute the external port pins into different
 *	functions like general input/output, spi, iic, uart and so on.
 */

/**
 * \file
 * \ingroup	BOARD_EMSK_DRV_MUX
 * \brief	emsk mux controller driver
 */

/**
 * \addtogroup	BOARD_EMSK_DRV_MUX
 * @{
 */
#include "board/emsk/common/mux_hal.h"
#include "board/emsk/common/mux.h"

static MUX_REG *mux_ctrl_regs = (MUX_REG *)0;

/** initialize i2c controller and set slave device address */
void mux_init(MUX_REG *mux_regs)
{
	// Initialize Mux controller registers by default values
	mux_regs[PMOD_MUX_CTRL] = PMOD_MUX_CTRL_DEFAULT;
	mux_regs[SPI_MAP_CTRL] =  SPI_MAP_CTRL_DEFAULT;
	mux_regs[UART_MAP_CTRL] = UART_MAP_CTRL_DEFAULT;
	mux_ctrl_regs = mux_regs;
}

/** Get mux ctrl register pointer, only valid after mux_init */
MUX_REG *get_mux_regs(void)
{
	return mux_ctrl_regs;
}

/** set PMOD muxer scheme */
void set_pmod_mux(MUX_REG *mux_regs, uint32_t val)
{
	mux_regs[PMOD_MUX_CTRL] = val;
}

/** get PMOD muxer scheme */
uint32_t get_pmod_mux(MUX_REG *mux_regs)
{
	return (uint32_t) mux_regs[PMOD_MUX_CTRL];
}

/** set PMOD muxer scheme */
void change_pmod_mux(MUX_REG *mux_regs, uint32_t val, uint32_t change_bits)
{
	mux_regs[PMOD_MUX_CTRL] = ((mux_regs[PMOD_MUX_CTRL] & ~change_bits) | val);
}

/** set SPI connection scheme */
void set_spi_map(MUX_REG *mux_regs, uint32_t val)
{
	mux_regs[SPI_MAP_CTRL] = val;
}

/** get SPI connection scheme */
uint32_t get_spi_map(MUX_REG *mux_regs)
{
	return (uint32_t) mux_regs[SPI_MAP_CTRL];
}

/** set UART connection scheme */
void set_uart_map(MUX_REG *mux_regs, uint32_t val)
{
	mux_regs[UART_MAP_CTRL] = val;
}

/** get UART connection scheme */
uint32_t get_uart_map(MUX_REG *mux_regs)
{
	return (uint32_t) mux_regs[UART_MAP_CTRL];
}
/** @} end of group BOARD_EMSK_DRV_MUX */

