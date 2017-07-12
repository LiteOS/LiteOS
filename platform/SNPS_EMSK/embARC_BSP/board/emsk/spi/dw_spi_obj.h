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
 * \file
 * \ingroup	BOARD_EMSK_DRV_DW_SPI_OBJ
 * \brief	header file of designware spi object instantiation on emsk
 */

/**
 * \addtogroup	BOARD_EMSK_DRV_DW_SPI_OBJ
 * @{
 */
#ifndef _DW_SPI_OBJ_H_
#define _DW_SPI_OBJ_H_

#include "device/device_hal/inc/dev_spi.h"

/**
 * \name	DesignWare SPI Object Number
 * @{
 */
#define DW_SPI_NUM		(2)	/*!< DesignWare SPI valid number */
/** @} end of name */

/**
 * \name	Designware SPI Object ID Macros
 * @{
 */
#define DW_SPI_0_ID		0	/*!< SPI 0 id macro (master node) */
#define DW_SPI_1_ID		1	/*!< SPI 1 id macro (slave node) */
/** @} end of name */

/**
 * \name	Designware SPI Object Control Macros
 * @{
 */
#define USE_DW_SPI_0		1	/*!< enable use designware SPI 0 */
#define USE_DW_SPI_1		1	/*!< enable use designware SPI 1 */
/** @} end of name */

#ifdef __cplusplus
extern "C" {
#endif

extern void dw_spi_all_install(void);

#ifdef __cplusplus
}
#endif

#endif /* _DW_SPI_OBJ_H_ */
/** @} end of group BOARD_EMSK_DRV_DW_SPI_OBJ */
