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
 * \file
 * \ingroup	BOARD_EMSK_DRV_MUX
 * \brief	emsk mux control register index header
 */

/**
 * \addtogroup	BOARD_EMSK_DRV_MUX
 * @{
 */
#ifndef _MUX_HAL_H_
#define _MUX_HAL_H_

/**
 * \name	Mux Control Register Index
 * @{
 */
#define PMOD_MUX_CTRL	0	/*!< 32-bits, offset 0x0,
					This register controls mapping of the peripheral device signals on Pmod connectors */

#define I2C_MAP_CTRL	1	/*!< 32-bits, offset 0x4 */

#define SPI_MAP_CTRL	2	/*!< 32-bits, offset 0x8,
					SPI_MAP_CTRL[0] selects the mode of operation of the SPI Slave:
					- Normal operation, SPI_MAP_CTRL[0]=0: SPI Slave is connected to Pmod1 at connector J1.
					- Loop-back mode, SPI_MAP_CTRL[0]=1: SPI Slave is connected to the SPI Master inside the FPGA using CS4.
							 */

#define UART_MAP_CTRL	3	/*!< 32-bits, offset 0x8,
					This register controls the mapping of the UART signals on the Pmod1 connector. */
/** @} end of name */


#endif	/* _MUX_HAL_H_ */

/** @} end of group BOARD_EMSK_DRV_MUX */
