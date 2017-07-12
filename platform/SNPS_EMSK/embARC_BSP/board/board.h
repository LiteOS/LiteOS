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
 *
 * \file
 * \ingroup	BOARD_COMMON
 * \brief	common board header file
 * \details
 * - This header file will contain board related settings for different boards.
 * - Each board configurations are put in its own header file, like emsk/emsk.h
 * - If you want to change the configuration, you need to go to related header file, e.g.
 *   if you want to change EMSK board settings, you need to go to emsk/emsk.h
 * - In embARC 2015.05, all the settings are in this board.h, but now it moved to related board header file
 */

/**
 * \addtogroup BOARD_COMMON
 * @{
 */
#ifndef _EMBARC_BOARD_H_
#define _EMBARC_BOARD_H_

#include "embARC_BSP_config.h"
/**
 * \todo	add comments and documents to describe the macros
 * \note 	the following macros must use the same name, because
 *	they are used by middleware and other applications
 */
/** here is a sample of EMSK board resource definitions */
#ifdef BOARD_EMSK
#include "board/emsk/emsk.h"
#endif /* BOARD_EMSK */

/** you can add your board configuration as BOARD_EMSK defined up */

#endif /* _EMBARC_BOARD_H_ */

/** @} end of group BOARD_COMMON */
