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
 * \defgroup	BOARD_EMSK_DRV_DW_IIC_OBJ	EMSK DW IIC Object
 * \ingroup	BOARD_EMSK_DRIVER
 * \brief	EMSK Designware IIC Objects
 * \details
 *		Realize the EMSK board iic object using designware iic device driver,
 *	only need to realize some designware iic structures combine with EMSK iic
 *	hardware resource. just like cpp class instantiation.
 */

/**
 * \file
 * \ingroup	BOARD_EMSK_DRV_DW_IIC_OBJ
 * \brief	designware iic object instantiation on emsk
 */

/**
 * \addtogroup	BOARD_EMSK_DRV_DW_IIC_OBJ
 * @{
 */
#include "inc/arc/arc.h"
#include "inc/arc/arc_builtin.h"
#include "inc/embARC_toolchain.h"
#include "inc/embARC_error.h"

#include "inc/arc/arc_exception.h"

#include "device/designware/iic/dw_iic.h"
#include "board/emsk/iic/dw_iic_obj.h"

#include "board/emsk/emsk.h"

#ifdef ARC_FEATURE_DMP_PERIPHERAL
#define PERIPHERAL_BASE		ARC_FEATURE_DMP_PERIPHERAL
#else
#define PERIPHERAL_BASE		_arc_aux_read(AUX_DMP_PERIPHERAL)
#endif

/**
 * \name	EMSK DesignWare IIC 0 Object Instantiation
 * @{
 */
#if (USE_DW_IIC_0)
static void dw_iic_0_isr(void *ptr);
#define DW_IIC_0_RELBASE	(REL_REGBASE_I2C0)	/*!< designware iic 0 relative baseaddr */
#define DW_IIC_0_INTNO		(INTNO_I2C0)		/*!< designware iic 0 interrupt number  */
#define DW_IIC_0_SLVADDR	(0x55)			/*!< iic 0 slave address working in slave mode */
#define DW_IIC_0_TX_FIFO_LEN	(32)
#define DW_IIC_0_RX_FIFO_LEN	(32)
#define DW_IIC_0_MASTER_CODE	(0)
#define DW_IIC_0_TARADDR	(0x55)

DEV_IIC			dw_iic_0;			/*!< designware iic object */
DW_IIC_CTRL		dw_iic_0_ctrl;			/*!< designware iic 0 ctrl */

/** designware iic 0 open */
static int32_t dw_iic_0_open (uint32_t mode, uint32_t param)
{
	return dw_iic_open(&dw_iic_0, mode, param);
}
/** designware iic 0 close */
static int32_t dw_iic_0_close (void)
{
	return dw_iic_close(&dw_iic_0);
}
/** designware iic 0 control */
static int32_t dw_iic_0_control (uint32_t ctrl_cmd, void *param)
{
	return dw_iic_control(&dw_iic_0, ctrl_cmd, param);
}
/** designware iic 0 write */
static int32_t dw_iic_0_write (const void *data, uint32_t len)
{
	return dw_iic_write(&dw_iic_0, data, len);
}
/** designware iic 0 close */
static int32_t dw_iic_0_read (void *data, uint32_t len)
{
	return dw_iic_read(&dw_iic_0, data, len);
}
/** designware iic 0 interrupt routine */
static void dw_iic_0_isr(void *ptr)
{
	dw_iic_isr(&dw_iic_0, ptr);
}
/** install designware iic 0 to system */
static void dw_iic_0_install(void)
{
	uint32_t iic_abs_base = 0;
	DEV_IIC *dw_iic_ptr = &dw_iic_0;
	DEV_IIC_INFO *dw_iic_info_ptr = &(dw_iic_0.iic_info);
	DW_IIC_CTRL *dw_iic_ctrl_ptr = &dw_iic_0_ctrl;
	DW_IIC_REG *dw_iic_reg_ptr;

	/* Info init */
	dw_iic_info_ptr->iic_ctrl = (void *)dw_iic_ctrl_ptr;
	dw_iic_info_ptr->opn_cnt = 0;
	dw_iic_info_ptr->addr_mode = IIC_7BIT_ADDRESS;
	dw_iic_info_ptr->tar_addr = DW_IIC_0_TARADDR;
	/*
	 * get absolute designware base address
	 */
	iic_abs_base = (uint32_t)PERIPHERAL_BASE + DW_IIC_0_RELBASE;
	dw_iic_reg_ptr = (DW_IIC_REG *)iic_abs_base;

	/* iic ctrl init */
	dw_iic_ctrl_ptr->dw_iic_regs = dw_iic_reg_ptr;
	/* Variables which should be set during object implementation */
	dw_iic_ctrl_ptr->support_modes = DW_IIC_BOTH_SUPPORTED;
	dw_iic_ctrl_ptr->tx_fifo_len = DW_IIC_0_TX_FIFO_LEN;
	dw_iic_ctrl_ptr->rx_fifo_len = DW_IIC_0_RX_FIFO_LEN;
	dw_iic_ctrl_ptr->iic_master_code = DW_IIC_0_MASTER_CODE;
	dw_iic_ctrl_ptr->retry_cnt = DW_IIC_MAX_RETRY_COUNT;
	dw_iic_ctrl_ptr->intno = DW_IIC_0_INTNO;
	dw_iic_ctrl_ptr->dw_iic_int_handler = dw_iic_0_isr;
	dw_iic_ctrl_ptr->iic_spklen = dw_iic_spklen_const;
	dw_iic_ctrl_ptr->iic_scl_cnt = dw_iic_sclcnt_const;
	/* Variables which always change during iic operation */
	dw_iic_ctrl_ptr->int_status = 0;
	dw_iic_ctrl_ptr->iic_tx_over = 0;
	dw_iic_ctrl_ptr->iic_rx_over = 0;

	/** iic dev init */
	dw_iic_ptr->iic_open = dw_iic_0_open;
	dw_iic_ptr->iic_close = dw_iic_0_close;
	dw_iic_ptr->iic_control = dw_iic_0_control;
	dw_iic_ptr->iic_write = dw_iic_0_write;
	dw_iic_ptr->iic_read = dw_iic_0_read;
}
#endif /* USE_DW_IIC_0 */
/** @} end of name */

/**
 * \name	EMSK DesignWare IIC 1 Object Instantiation
 * @{
 */
#if (USE_DW_IIC_1)
static void dw_iic_1_isr(void *ptr);
#define DW_IIC_1_RELBASE	(REL_REGBASE_I2C1)	/*!< designware iic 1 relative baseaddr */
#define DW_IIC_1_INTNO		(INTNO_I2C1)		/*!< designware iic 1 interrupt number  */
#define DW_IIC_1_SLVADDR	(0x56)			/*!< iic 1 slave address working in slave mode */
#define DW_IIC_1_TX_FIFO_LEN	(32)
#define DW_IIC_1_RX_FIFO_LEN	(32)
#define DW_IIC_1_MASTER_CODE	(1)
#define DW_IIC_1_TARADDR	(0x56)

DEV_IIC		dw_iic_1;				/*!< designware iic 1 object */
DW_IIC_CTRL 	dw_iic_1_ctrl;				/*!< designware iic 1 ctrl */

/** designware iic 1 open */
static int32_t dw_iic_1_open (uint32_t mode, uint32_t param)
{
	return dw_iic_open(&dw_iic_1, mode, param);
}
/** designware iic 1 close */
static int32_t dw_iic_1_close (void)
{
	return dw_iic_close(&dw_iic_1);
}
/** designware iic 1 control */
static int32_t dw_iic_1_control (uint32_t ctrl_cmd, void *param)
{
	return dw_iic_control(&dw_iic_1, ctrl_cmd, param);
}
/** designware iic 1 write */
static int32_t dw_iic_1_write (const void *data, uint32_t len)
{
	return dw_iic_write(&dw_iic_1, data, len);
}
/** designware iic 1 close */
static int32_t dw_iic_1_read (void *data, uint32_t len)
{
	return dw_iic_read(&dw_iic_1, data, len);
}
/** designware iic 1 interrupt routine */
static void dw_iic_1_isr(void *ptr)
{
	dw_iic_isr(&dw_iic_1, ptr);
}
/** install designware iic 1 to system */
static void dw_iic_1_install(void)
{
	uint32_t iic_abs_base = 0;
	DEV_IIC *dw_iic_ptr = &dw_iic_1;
	DEV_IIC_INFO *dw_iic_info_ptr = &(dw_iic_1.iic_info);
	DW_IIC_CTRL *dw_iic_ctrl_ptr = &dw_iic_1_ctrl;
	DW_IIC_REG *dw_iic_reg_ptr;

	/* Info init */
	dw_iic_info_ptr->iic_ctrl = (void *)dw_iic_ctrl_ptr;
	dw_iic_info_ptr->opn_cnt = 0;
	dw_iic_info_ptr->addr_mode = IIC_7BIT_ADDRESS;
	dw_iic_info_ptr->tar_addr = DW_IIC_1_TARADDR;
	/*
	 * get absolute designware base address
	 */
	iic_abs_base = (uint32_t)PERIPHERAL_BASE + DW_IIC_1_RELBASE;
	dw_iic_reg_ptr = (DW_IIC_REG *)iic_abs_base;

	/* iic ctrl init */
	dw_iic_ctrl_ptr->dw_iic_regs = dw_iic_reg_ptr;
	/* Variables which should be set during object implementation */
	dw_iic_ctrl_ptr->support_modes = DW_IIC_BOTH_SUPPORTED;
	dw_iic_ctrl_ptr->tx_fifo_len = DW_IIC_1_TX_FIFO_LEN;
	dw_iic_ctrl_ptr->rx_fifo_len = DW_IIC_1_RX_FIFO_LEN;
	dw_iic_ctrl_ptr->iic_master_code = DW_IIC_1_MASTER_CODE;
	dw_iic_ctrl_ptr->retry_cnt = DW_IIC_MAX_RETRY_COUNT;
	dw_iic_ctrl_ptr->intno = DW_IIC_1_INTNO;
	dw_iic_ctrl_ptr->dw_iic_int_handler = dw_iic_1_isr;
	dw_iic_ctrl_ptr->iic_spklen = dw_iic_spklen_const;
	dw_iic_ctrl_ptr->iic_scl_cnt = dw_iic_sclcnt_const;
	/* Variables which always change during iic operation */
	dw_iic_ctrl_ptr->int_status = 0;
	dw_iic_ctrl_ptr->iic_tx_over = 0;
	dw_iic_ctrl_ptr->iic_rx_over = 0;

	/** iic dev init */
	dw_iic_ptr->iic_open = dw_iic_1_open;
	dw_iic_ptr->iic_close = dw_iic_1_close;
	dw_iic_ptr->iic_control = dw_iic_1_control;
	dw_iic_ptr->iic_write = dw_iic_1_write;
	dw_iic_ptr->iic_read = dw_iic_1_read;
}
#endif /* USE_DW_IIC_1 */
/** @} end of name */

/** get one designware device structure */
DEV_IIC_PTR iic_get_dev(int32_t iic_id)
{
	static uint32_t install_flag = 0;

	/* intall device objects */
	if (install_flag == 0) {
		install_flag = 1;
		dw_iic_all_install();
	}

	switch (iic_id) {
#if (USE_DW_IIC_0)
		case DW_IIC_0_ID:
			return &dw_iic_0;
			break;
#endif
#if (USE_DW_IIC_1)
		case DW_IIC_1_ID:
			return &dw_iic_1;
			break;
#endif
		default:
			break;
	}
	return NULL;
}

/**
 * \brief	install all iic objects
 * \note	\b MUST be called during system init
 */
void dw_iic_all_install(void)
{
#if (USE_DW_IIC_0)
	dw_iic_0_install();
#endif
#if (USE_DW_IIC_1)
	dw_iic_1_install();
#endif
}

/** @} end of group BOARD_EMSK_DRV_DW_IIC_OBJ */
