/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Targets Hardware Iwdg Hal HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --------------------------------------------------------------------------- */

/**
 * @defgroup hal_iwdg Watch Dog
 * @ingroup hal
 */

#ifndef _HAL_IWDG_H
#define _HAL_IWDG_H

#include <stdint.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup hal_iwdg
 * @brief config the watch dog.
 *
 * @par Description:
 * This API is used to config the watch dog. The time interval of feeding dog is prvscaler*reload/40.
 * @attention none.
 *
 * @param prvscaler      [IN] select the prescaler of the IWDG.
                             This parameter can be a value of @ref IWDG_Prescaler
 * @param reload         [IN] specifies the IWDG down-counter reload value.
                             This parameter must be a number between Min_Data = 0 and Max_Data = 0x0FFF
 *
 * @retval #int          0 if succeed or -1 if failed.
 * @par Dependency: none.
 * @see none.
 */
int hal_iwdg_config(uint8_t prvscaler, uint16_t reload);

/**
 * @ingroup hal_iwdg
 * @brief feed the watch dog.
 *
 * @par Description:
 * This API is used to feed the watch dog.
 * @attention none.
 *
 * @param none.
 *
 * @retval none.
 * @par Dependency: none.
 * @see none.
 */
void hal_iwdg_feed(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif  /* _HAL_IWDG_H */
