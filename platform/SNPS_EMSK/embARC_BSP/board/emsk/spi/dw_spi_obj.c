/* ------------------------------------------
 * Copyright (c) 2017, Synopsys, Inc. All rights reserved.

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
 * \version 2017.03
 * \date 2014-07-03
 * \author Huaqi Fang(Huaqi.Fang@synopsys.com)
--------------------------------------------- */

/**
 * \defgroup	BOARD_EMSK_DRV_DW_SPI_OBJ	EMSK DW SPI Object
 * \ingroup	BOARD_EMSK_DRIVER
 * \brief	EMSK Designware SPI Objects
 * \details
 *		realize the EMSK board spi object using designware spi device driver,
 *	only need to realize some designware spi structures combine with EMSK spi
 *	hardware resource. just like cpp class instantiation.
 */

/**
 * \file
 * \ingroup	BOARD_EMSK_DRV_DW_SPI_OBJ
 * \brief	designware spi object instantiation on emsk
 */

/**
 * \addtogroup	BOARD_EMSK_DRV_DW_SPI_OBJ
 * @{
 */
#include "inc/arc/arc.h"
#include "inc/arc/arc_builtin.h"
#include "inc/embARC_toolchain.h"
#include "inc/embARC_error.h"
#include "inc/arc/arc_exception.h"

#include "device/designware/spi/dw_spi.h"
#include "board/emsk/spi/dw_spi_obj.h"

#include "board/emsk/emsk.h"

#ifdef ARC_FEATURE_DMP_PERIPHERAL
#define PERIPHERAL_BASE		ARC_FEATURE_DMP_PERIPHERAL
#else
#define PERIPHERAL_BASE		_arc_aux_read(AUX_DMP_PERIPHERAL)
#endif

/**
 * \name	EMSK DesignWare SPI 0 Object Instantiation
 * @{
 */
#if (USE_DW_SPI_0)
static void dw_spi_0_isr(void *ptr);
#define DW_SPI_0_RELBASE	(REL_REGBASE_SPI0)	/*!< designware spi 0 relative baseaddr */
#define DW_SPI_0_INTNO		(INTNO_SPI_MASTER)	/*!< designware spi 0 interrupt number  */

#define DW_SPI_0_TX_FIFO_LEN	(32)
#define DW_SPI_0_RX_FIFO_LEN	(32)

#define DW_SPI_0_SUPPORTED_MODE	DW_SPI_MASTER_SUPPORTED

DEV_SPI			dw_spi_0;			/*!< designware spi object */
DW_SPI_CTRL		dw_spi_0_ctrl;			/*!< designware spi 0 ctrl */
#if HW_VERSION >= 22
static uint32_t spi_mst_cs_ctrl_creg = 0;
#endif

/** designware spi 0 open */
static int32_t dw_spi_0_open (uint32_t mode, uint32_t param)
{
	return dw_spi_open(&dw_spi_0, mode, param);
}
/** designware spi 0 close */
static int32_t dw_spi_0_close (void)
{
	return dw_spi_close(&dw_spi_0);
}
/** designware spi 0 control */
static int32_t dw_spi_0_control (uint32_t ctrl_cmd, void *param)
{
#if HW_VERSION >= 22
	int32_t ercd;
	ercd = dw_spi_control(&dw_spi_0, ctrl_cmd, param);
	if (ctrl_cmd == SPI_CMD_MST_SEL_DEV) {
		_arc_write_uncached_32((void *)spi_mst_cs_ctrl_creg, 1 << ((uint32_t)param));
	} else if (ctrl_cmd == SPI_CMD_MST_DSEL_DEV) {
		_arc_write_uncached_32((void *)spi_mst_cs_ctrl_creg, 0);
	}
	return ercd;
#else
	return dw_spi_control(&dw_spi_0, ctrl_cmd, param);
#endif
}
/** designware spi 0 write */
static int32_t dw_spi_0_write (const void *data, uint32_t len)
{
	return dw_spi_write(&dw_spi_0, data, len);
}
/** designware spi 0 close */
static int32_t dw_spi_0_read (void *data, uint32_t len)
{
	return dw_spi_read(&dw_spi_0, data, len);
}
/** designware spi 0 interrupt routine */
static void dw_spi_0_isr(void *ptr)
{
	dw_spi_isr(&dw_spi_0, ptr);
}
/** install designware spi 0 to system */
static void dw_spi_0_install(void)
{
	uint32_t spi_abs_base = 0;
	DEV_SPI *dw_spi_ptr = &dw_spi_0;
	DEV_SPI_INFO *dw_spi_info_ptr = &(dw_spi_0.spi_info);
	DW_SPI_CTRL *dw_spi_ctrl_ptr = &dw_spi_0_ctrl;
	DW_SPI_REG *dw_spi_reg_ptr;

	/**
	 * get absolute designware base address
	 */
	spi_abs_base = (uint32_t)PERIPHERAL_BASE + DW_SPI_0_RELBASE;

#if HW_VERSION >= 22
	spi_mst_cs_ctrl_creg = spi_abs_base + REL_REGBASE_SPI_MST_CS_CTRL;
#endif

	dw_spi_reg_ptr = (DW_SPI_REG *)spi_abs_base;

	/** spi info init */
	dw_spi_info_ptr->spi_ctrl = (void *)dw_spi_ctrl_ptr;
	dw_spi_info_ptr->status = DEV_DISABLED;
	dw_spi_info_ptr->freq = 1000;
	dw_spi_info_ptr->opn_cnt = 0;
	dw_spi_info_ptr->mode = DEV_MASTER_MODE;
	dw_spi_info_ptr->clk_mode = SPI_CLK_MODE_DEFAULT;
	dw_spi_info_ptr->slave = SPI_SLAVE_NOT_SELECTED;
	dw_spi_info_ptr->dfs = SPI_DFS_DEFAULT;
	dw_spi_info_ptr->dummy = 0xff;

	/* spi ctrl init */
	/* Variables which should be set during object implementation */
	dw_spi_ctrl_ptr->dw_spi_regs = dw_spi_reg_ptr;
	dw_spi_ctrl_ptr->intno = DW_SPI_0_INTNO;
	dw_spi_ctrl_ptr->support_modes = DW_SPI_0_SUPPORTED_MODE;
	dw_spi_ctrl_ptr->tx_fifo_len = DW_SPI_0_TX_FIFO_LEN;
	dw_spi_ctrl_ptr->rx_fifo_len = DW_SPI_0_TX_FIFO_LEN;
	dw_spi_ctrl_ptr->dw_apb_bus_freq = CLK_BUS_APB;
	dw_spi_ctrl_ptr->dw_spi_int_handler = dw_spi_0_isr;
	/* Variables which always change during spi operation */
	dw_spi_ctrl_ptr->int_status = 0;

	/** spi dev init */
	dw_spi_ptr->spi_open = dw_spi_0_open;
	dw_spi_ptr->spi_close = dw_spi_0_close;
	dw_spi_ptr->spi_control = dw_spi_0_control;
	dw_spi_ptr->spi_write = dw_spi_0_write;
	dw_spi_ptr->spi_read = dw_spi_0_read;
}
#endif /* USE_DW_SPI_0 */
/** @} end of name */

/**
 * \name	EMSK DesignWare SPI 1 Object Instantiation
 * @{
 */
#if (USE_DW_SPI_1)
static void dw_spi_1_isr(void *ptr);
#define DW_SPI_1_RELBASE	(REL_REGBASE_SPI1)	/*!< designware spi 1 relative baseaddr */
#define DW_SPI_1_INTNO		(INTNO_SPI_SLAVE)	/*!< designware spi 1 interrupt number  */

#define DW_SPI_1_TX_FIFO_LEN	(32)
#define DW_SPI_1_RX_FIFO_LEN	(32)

#define DW_SPI_1_SUPPORTED_MODE	DW_SPI_SLAVE_SUPPORTED

DEV_SPI			dw_spi_1;			/*!< designware spi 1 object */
DW_SPI_CTRL		dw_spi_1_ctrl;			/*!< designware spi 1 ctrl */

/** designware spi 1 open */
static int32_t dw_spi_1_open (uint32_t mode, uint32_t param)
{
	return dw_spi_open(&dw_spi_1, mode, param);
}
/** designware spi 1 close */
static int32_t dw_spi_1_close (void)
{
	return dw_spi_close(&dw_spi_1);
}
/** designware spi 1 control */
static int32_t dw_spi_1_control (uint32_t ctrl_cmd, void *param)
{
	return dw_spi_control(&dw_spi_1, ctrl_cmd, param);
}
/** designware spi 1 write */
static int32_t dw_spi_1_write (const void *data, uint32_t len)
{
	return dw_spi_write(&dw_spi_1, data, len);
}
/** designware spi 1 close */
static int32_t dw_spi_1_read (void *data, uint32_t len)
{
	return dw_spi_read(&dw_spi_1, data, len);
}
/** designware spi 1 interrupt routine */
static void dw_spi_1_isr(void *ptr)
{
	dw_spi_isr(&dw_spi_1, ptr);
}
/** install designware spi 1 to system */
static void dw_spi_1_install(void)
{
	uint32_t spi_abs_base = 0;
	DEV_SPI *dw_spi_ptr = &dw_spi_1;
	DEV_SPI_INFO *dw_spi_info_ptr = &(dw_spi_1.spi_info);
	DW_SPI_CTRL *dw_spi_ctrl_ptr = &dw_spi_1_ctrl;
	DW_SPI_REG *dw_spi_reg_ptr;

	/**
	 * get absolute designware base address
	 */
	spi_abs_base = (uint32_t)PERIPHERAL_BASE + DW_SPI_1_RELBASE;
	dw_spi_reg_ptr = (DW_SPI_REG *)spi_abs_base;

	/** spi info init */
	dw_spi_info_ptr->spi_ctrl = (void *)dw_spi_ctrl_ptr;
	dw_spi_info_ptr->status = DEV_DISABLED;
	dw_spi_info_ptr->freq = 1000;
	dw_spi_info_ptr->opn_cnt = 0;
	dw_spi_info_ptr->mode = DEV_SLAVE_MODE;
	dw_spi_info_ptr->clk_mode = SPI_CLK_MODE_DEFAULT;
	dw_spi_info_ptr->slave = SPI_SLAVE_NOT_SELECTED;
	dw_spi_info_ptr->dfs = SPI_DFS_DEFAULT;
	dw_spi_info_ptr->dummy = 0xff;

	/* spi ctrl init */
	/* Variables which should be set during object implementation */
	dw_spi_ctrl_ptr->dw_spi_regs = dw_spi_reg_ptr;
	dw_spi_ctrl_ptr->intno = DW_SPI_1_INTNO;
	dw_spi_ctrl_ptr->support_modes = DW_SPI_1_SUPPORTED_MODE;
	dw_spi_ctrl_ptr->tx_fifo_len = DW_SPI_1_TX_FIFO_LEN;
	dw_spi_ctrl_ptr->rx_fifo_len = DW_SPI_1_TX_FIFO_LEN;
	dw_spi_ctrl_ptr->dw_apb_bus_freq = CLK_BUS_APB;
	dw_spi_ctrl_ptr->dw_spi_int_handler = dw_spi_1_isr;
	/* Variables which always change during spi operation */
	dw_spi_ctrl_ptr->int_status = 0;

	/** spi dev init */
	dw_spi_ptr->spi_open = dw_spi_1_open;
	dw_spi_ptr->spi_close = dw_spi_1_close;
	dw_spi_ptr->spi_control = dw_spi_1_control;
	dw_spi_ptr->spi_write = dw_spi_1_write;
	dw_spi_ptr->spi_read = dw_spi_1_read;
}
#endif /* USE_DW_SPI_1 */
/** @} end of name */

/** get one designware device structure */
DEV_SPI_PTR spi_get_dev(int32_t spi_id)
{
	static uint32_t install_flag = 0;

	/* install device objects */
	if (install_flag == 0) {
		install_flag = 1;
		dw_spi_all_install();
	}

	switch (spi_id) {
#if (USE_DW_SPI_0)
		case DW_SPI_0_ID:
			return &dw_spi_0;
			break;
#endif
#if (USE_DW_SPI_1)
		case DW_SPI_1_ID:
			return &dw_spi_1;
			break;
#endif
		default:
			break;
	}
	return NULL;
}

/**
 * \brief	install all spi objects
 * \note	\b MUST be called during system init
 */
void dw_spi_all_install(void)
{
#if (USE_DW_SPI_0)
	dw_spi_0_install();
#endif
#if (USE_DW_SPI_1)
	dw_spi_1_install();
#endif
}

/** @} end of group BOARD_EMSK_DRV_DW_SPI_OBJ */
