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
 * \date 2014-06-18
 * \author Wayne Ren(Wei.Ren@synopsys.com)
--------------------------------------------- */
/**
 *
 * \file
 * \ingroup	BOARD_EMSK_COMMON_INIT
 * \brief	emsk resource definitions
 */

/**
 * \addtogroup BOARD_EMSK_COMMON_INIT
 * @{
 */
#ifndef _EMSK_H_
#define _EMSK_H_

#include "inc/arc/arc_em.h"

#include "board/emsk/uart/dw_uart_obj.h"
#include "board/emsk/gpio/dw_gpio_obj.h"
#include "board/emsk/gpio/emsk_gpio.h"
#include "board/emsk/iic/dw_iic_obj.h"
#include "board/emsk/spi/dw_spi_obj.h"

#include "board/emsk/common/emsk_timer.h"

#include "board/emsk/emsk_hardware.h"

#define EMSK_GPIO_PORT_A		DW_GPIO_PORT_A
#define EMSK_GPIO_PORT_B		DW_GPIO_PORT_B
#define EMSK_GPIO_PORT_C		DW_GPIO_PORT_C
#define EMSK_GPIO_PORT_D		DW_GPIO_PORT_D

/* common macros must be defined by all boards */

#define BOARD_CONSOLE_UART_ID		DW_UART_1_ID
#define BOARD_CONSOLE_UART_BAUD		UART_BAUDRATE_115200
#define BOARD_ADC_IIC_ID		DW_IIC_0_ID
#define BOARD_TEMP_SENSOR_IIC_ID	DW_IIC_0_ID

#define BOARD_TEMP_IIC_SLVADDR		TEMP_I2C_SLAVE_ADDRESS

#ifndef BOARD_SPI_FREQ
#define BOARD_SPI_FREQ			(1000000)
#endif

#ifndef BOARD_SPI_CLKMODE
#define BOARD_SPI_CLKMODE		(SPI_CLK_MODE_0)
#endif

#define BOARD_SYS_TIMER_ID		TIMER_0
#define BOARD_SYS_TIMER_INTNO		INTNO_TIMER0
#define BOARD_SYS_TIMER_HZ		(1000)

/** board timer count frequency (HZ) */
#define BOARD_SYS_TIMER_MS_HZ		(1000)
/** board timer count frequency convention based on the global timer counter */
#define BOARD_SYS_TIMER_MS_CONV		(BOARD_SYS_TIMER_MS_HZ/BOARD_SYS_TIMER_HZ)


#define BOARD_CPU_CLOCK			CLK_CPU
#define BOARD_DEV_CLOCK			CLK_BUS_APB

#define BOARD_LED_MASK			(0x1ff)
#define BOARD_LED_CNT			(9)
#define BOARD_BTN_MASK			(0x7)
#define BOARD_BTN_CNT			(3)
#define BOARD_SWT_MASK			(0xf)
#define BOARD_SWT_CNT			(4)

#define BOARD_ONBOARD_NTSHELL_ID	(EMSK_NTSHELL_0_ID)

#define OSP_DELAY_OS_COMPAT_ENABLE	(1)
#define OSP_DELAY_OS_COMPAT_DISABLE	(0)


#define OSP_GET_CUR_SYSHZ()		(gl_emsk_sys_hz_cnt)
#define OSP_GET_CUR_MS()		(gl_emsk_ms_cnt)
#define OSP_GET_CUR_US()		board_get_cur_us()
#define OSP_GET_CUR_HWTICKS()		board_get_hwticks()

#ifdef __cplusplus
extern "C" {
#endif

extern void board_init(void);
extern void board_timer_update(uint32_t precision);
extern void board_delay_ms(uint32_t ms, uint8_t os_compat);
extern uint64_t board_get_hwticks(void);
extern uint64_t board_get_cur_us(void);

#ifdef __cplusplus
}
#endif

#endif	/* _EMSK_H_ */

/** @} end of group BOARD_EMSK_COMMON_INIT */
