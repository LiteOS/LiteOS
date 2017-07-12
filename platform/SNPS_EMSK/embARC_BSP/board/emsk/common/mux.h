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
 * \brief	header file of emsk mux controller driver
 */

/**
 * \addtogroup	BOARD_EMSK_DRV_MUX
 * @{
 */
#ifndef _MUX_H_
#define _MUX_H_

#include "inc/embARC_toolchain.h"

#define BIT0			(0)
#define BIT1			(1)
#define BIT2			(2)
#define BIT3			(3)
#define PM1_OFFSET		(0)
#define PM2_OFFSET		(4)
#define PM3_OFFSET		(8)
#define PM4_OFFSET		(12)
#define PM5_OFFSET		(16)
#define PM6_OFFSET		(20)
#define PM7_OFFSET		(24)

#define PM1_MASK		(0xf << PM1_OFFSET)
#define PM2_MASK		(0xf << PM2_OFFSET)
#define PM3_MASK		(0xf << PM3_OFFSET)
#define PM4_MASK		(0xf << PM4_OFFSET)
#define PM5_MASK		(0xf << PM5_OFFSET)
#define PM6_MASK		(0xf << PM6_OFFSET)
#define PM7_MASK		(0xf << PM7_OFFSET)


#define SPI_MAP_NORMAL		(0)
#define SPI_MAP_LOOPBACK	(1)

#define UART_MAP_TYPE4		(0xE4)
#define UART_MAP_TYPE3		(0x6C)

/**
 * \name	Default pin muxer settings
 * @{
 */
#define PMOD_MUX_CTRL_DEFAULT	(0)			/*!< all pins are configured as GPIO inputs */
#define SPI_MAP_CTRL_DEFAULT	(SPI_MAP_NORMAL)	/*!< normal SPI mode */
#define UART_MAP_CTRL_DEFAULT	(UART_MAP_TYPE4)	/*!< TYPE4 PMOD compatible */
/** @} end of name */

/**
 * \name	PMOD 1 Multiplexor
 * @{
 */
#define PM1_UR_GPIO_C		((0 << BIT0) << PM1_OFFSET)	/*!< Pmod1[4:1] are connected to DW GPIO Port C[11:8] */
#define PM1_UR_UART_0		((1 << BIT0) << PM1_OFFSET)	/*!< Pmod1[4:1] are connected to DW UART0 signals */

#define PM1_LR_GPIO_A		((0 << BIT2) << PM1_OFFSET)	/*!< Pmod1[10:7] are connected to DW GPIO Port A[11:8] */
#define PM1_LR_SPI_S		((1 << BIT2) << PM1_OFFSET)	/*!< Pmod1[10:7] are connected to DW SPI Slave signals */
/** @} end of name */


/**
 * \name	PMOD 2 Multiplexor
 * @{
 */
#define PM2_GPIO_AC		((0 << BIT0) << PM2_OFFSET)	/*!< Pmod2[4:1]	 are connected to DW GPIO Port C[15:12],
									Pmod2[10:7] are connected to DW GPIO Port A[15:12]  */

#define PM2_I2C_HRI		((1 << BIT0) << PM2_OFFSET)	/*!< connect I2C to Pmod2[4:1] and halt/run interface to Pmod2[10:7] */
/** @} end of name */


/**
 * \name	PMOD 3 Multiplexor
 * @{
 */
#define PM3_GPIO_AC		((0 << BIT0) << PM3_OFFSET)	/*!< Pmod3[4:1]  are connected to DW GPIO Port C[19:16],
									Pmod3[10:7] are connected to DW GPIO Port A[19:16]  */

#define PM3_I2C_GPIO_D		((1 << BIT0) << PM3_OFFSET)	/*!< Pmod3[4:3]  are connected to DW I2C signals,
									Pmod3[2:1]  are connected to DW GPIO Port D[1:0],
									Pmod3[10:7] are connected to DW GPIO Port D[3:2] */
/** @} end of name */


/**
 * \name	PMOD 4 Multiplexor
 * @{
 */
#define PM4_GPIO_AC		((0 << BIT0) << PM4_OFFSET)	/*!< Pmod4[4:1]  are connected to DW GPIO Port C[23:20],
									Pmod4[10:7] are connected to DW GPIO Port A[23:20] */

#define PM4_I2C_GPIO_D		((1 << BIT0) << PM4_OFFSET)	/*!< Pmod4[4:3]  are connected to DW I2C signals,
									Pmod4[2:1]  are connected to DW GPIO Port D[5:4],
									Pmod4[10:7] are connected to DW GPIO Port D[7:6] */
/** @} end of name */

/**
 * \name	PMOD 5 Multiplexor
 * @{
 */
#define PM5_UR_GPIO_C		((0 << BIT0) << PM5_OFFSET)	/*!< Pmod5[4:1] are connected to DW GPIO Port C[27:24] */
#define PM5_UR_SPI_M1		((1 << BIT0) << PM5_OFFSET)	/*!< Pmod5[4:1] are connected to DW SPI Master signals using CS1_N */

#define PM5_LR_GPIO_A		((0 << BIT2) << PM5_OFFSET)	/*!< Pmod5[10:7] are connected to DW GPIO Port A[27:24] */
#define PM5_LR_SPI_M2		((1 << BIT2) << PM5_OFFSET)	/*!< Pmod5[10:7] are connected to DW SPI Master signals using CS2_N */
/** @} end of name */


/**
 * \name	PMOD 6 Multiplexor
 * @{
 */
#define PM6_UR_GPIO_C		((0 << BIT0) << PM6_OFFSET)	/*!< Pmod6[4:1] are connected to DW GPIO Port C[31:28] */
#define PM6_UR_SPI_M0		((1 << BIT0) << PM6_OFFSET)	/*!< Pmod6[4:1] are connected to DW SPI Master signals using CS0_N */

#define PM6_LR_GPIO_A		((0 << BIT2) << PM6_OFFSET)	/*!< Pmod6[10:7] are connected to DW GPIO Port A[31:28] */

#define PM6_LR_CSS_STAT 	((1 << BIT2) << PM6_OFFSET)	/*!< Pmod6[8:7] are connected to the DW SPI Master chip select signals CS1_N and CS2_N,
									Pmod6[6:5] are connected to the ARC EM halt and sleep status signals */
/** @} end of name */

/**
 * \name	PMOD 7 Multiplexor
 * @{
 */
#define PM7_GPIO_D		((0 << BIT0) << PM7_OFFSET)	/*!< Pmod7[4:1] are connected to DW GPIO Port D[11:8] */
#define PM7_STAT		((1 << BIT0) << PM7_OFFSET)	/*!< Pmod7[4:1] are connected to the ARC EM sleep status signals */
/** @} end of name */
typedef volatile uint32_t MUX_REG;

#ifdef __cplusplus
extern "C" {
#endif

extern void mux_init(MUX_REG *mux_regs);
extern MUX_REG *get_mux_regs(void);
extern void set_pmod_mux(MUX_REG *mux_regs, uint32_t val);
extern uint32_t get_pmod_mux(MUX_REG *mux_regs);
extern void change_pmod_mux(MUX_REG *mux_regs, uint32_t val, uint32_t change_bits);
extern void set_spi_map(MUX_REG *mux_regs, uint32_t val);
extern uint32_t get_spi_map(MUX_REG *mux_regs);
extern void set_uart_map(MUX_REG *mux_regs, uint32_t val);
extern uint32_t get_uart_map(MUX_REG *mux_regs);

#ifdef __cplusplus
}
#endif

#endif /* _MUX_H_ */

/** @} end of group BOARD_EMSK_DRV_MUX */
