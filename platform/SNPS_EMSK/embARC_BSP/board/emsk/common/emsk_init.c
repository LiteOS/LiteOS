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
 * \date 2014-07-31
 * \author Wayne Ren(Wei.Ren@synopsys.com)
--------------------------------------------- */
/**
 * \defgroup	BOARD_EMSK_COMMON_INIT	EMSK Common Init Module
 * \ingroup	BOARD_EMSK_COMMON
 * \brief	EMSK Board Common Init Module
 * \details
 * 		EMSK timer/gpio/interrupt init process. Device-driver installation is done while
 *	getting the device object for the first time.
 */

/**
 * \file
 * \ingroup	BOARD_EMSK_COMMON_INIT
 * \brief	common emsk init module
 */

/**
 * \addtogroup	BOARD_EMSK_COMMON_INIT
 * @{
 */
#include "inc/arc/arc_builtin.h"
#include "inc/arc/arc.h"
#include "inc/arc/arc_timer.h"

#include "board/emsk/emsk.h"
#include "board/emsk/common/emsk_timer.h"
#include "board/emsk/common/mux.h"

#ifdef ARC_FEATURE_DMP_PERIPHERAL
#define PERIPHERAL_BASE		ARC_FEATURE_DMP_PERIPHERAL
#else
#define PERIPHERAL_BASE		_arc_aux_read(AUX_DMP_PERIPHERAL)
#endif

/**
 * \brief	emsk-related mux io init
 */
static void emsk_mux_init(void)
{
	MUX_REG *mux_regs;

	mux_regs = (MUX_REG *)(PERIPHERAL_BASE|REL_REGBASE_PINMUX);
	mux_init(mux_regs);

	/**
	 * + Please refer to corresponding EMSK User Guide for detailed information
	 *   -> Appendix: A  Hardware Functional Description
	 *      -> Pmods Configuration summary
	 * + Set up pin-multiplexer of all PMOD connections
	 *   - PM1 J1: Upper row as UART 0, lower row as SPI Slave
	 *   - PM2 J2: IIC 0 and run/halt signals
	 *   - PM3 J3: GPIO Port A and Port C
	 *   - PM4 J4: IIC 1 and Port D
	 *   - PM5 J5: Upper row as SPI Master, lower row as Port A
	 *   - PM6 J6: Upper row as SPI Master, lower row as Port A
	 */
	set_pmod_mux(mux_regs, PM1_UR_UART_0 | PM1_LR_SPI_S	\
				| PM2_I2C_HRI			\
				| PM3_GPIO_AC			\
				| PM4_I2C_GPIO_D		\
				| PM5_UR_SPI_M1 | PM5_LR_GPIO_A	\
				| PM6_UR_SPI_M0 | PM6_LR_GPIO_A );

	/**
	 * PM1 upper row as UART
	 * UM4:RXD, UM3:TXD
	 * UM2:RTS_N, UM1:CTS_N
	 */
	set_uart_map(mux_regs, 0x9c);
}

/**
 * \brief	Board init routine MUST be called in each application
 * \note	It is better to disable interrupts when calling this function
 *	remember to enable interrupt when you want to use them
 */
void board_init(void)
{
	timer_init();
	emsk_mux_init();
	emsk_gpio_init();
	emsk_timer_init();
}

/** @} end of group BOARD_EMSK_COMMON_INIT */
