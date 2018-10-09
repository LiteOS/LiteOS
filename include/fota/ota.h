/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
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
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

/**@defgroup fota FOTA
 * @defgroup ota ota update
 * @ingroup fota
 */

#ifndef _OTA_H_
#define _OTA_H_

#include <stdint.h>
#include <stdio.h>
#include "ota_errno.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    OTA_S_IDLE = 0,
    OTA_S_NEEDUPDATE,
    OTA_S_UPDATING,
    OTA_S_FAILED,
    OTA_S_SUCCEED,
} ota_state;

typedef struct
{
    // used by app & bootloader
    int (*func_init)(void);

#ifndef USE_BOOTLOADER /* used by app */
    int (*func_set_reboot)(int32_t image_len);
    int (*func_check_update_state)(ota_state* st);

#else /* used by bootloader */
    int (*func_update_process)(void);
    int (*func_jump_to_application)(void);
    int (*func_roll_back_image)(void);
 #endif
} ota_module;

typedef struct
{
    int (*func_printf)(const char* format, ...);
    int (*func_ota_read)(void* buf, int32_t len, uint32_t location);
    int (*func_ota_write)(const void* buf, int32_t len, uint32_t location);
} ota_assist;

extern ota_assist g_ota_assist;

#define OTA_DEBUG

#ifdef OTA_DEBUG

#define OTA_LOG(fmt, ...) \
    do \
    { \
        (void)g_ota_assist.func_printf("[OTA][%s:%d] " fmt "\r\n", \
                                       __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#else
#define OTA_LOG(fmt, ...)
#endif

/**
 *@ingroup ota
 *@brief register functions of ota.
 *
 *@par Description:
 *This API is used to register functions of ota. If this interface is
 *ignored by user, default ota method will be used.
 *@attention none.
 *
 *@param module         [IN] ota functions registered by user.
 *
 *@retval none.
 *@par Dependency: none.
 *@see none.
 */
void ota_register_module(ota_module* module);

/**
 *@ingroup ota
 *@brief register assist functions of ota.
 *
 *@par Description:
 *This API is used to register assist functions of ota.
 *@attention none.
 *
 *@param module         [IN] ota assist functions registered by user.
 *
 *@retval none.
 *@par Dependency: none.
 *@see none.
 */
 void ota_register_assist(ota_assist* assist);

/**
 *@ingroup ota
 *@brief obtain ota_flag stored on flash.
 *
 *@par Description:
 *This API is used to obtain ota_flag stored on flash. If read failed
 *(crc check failed or system start at first time), this interface will
 *write default parameters to flash.
 *@attention none.
 *
 *@param none.
 *
 *@retval #int          0 if succeed or -1 if failed.
 *@par Dependency: none.
 *@see none.
 */
int ota_init(void);

#ifndef USE_BOOTLOADER
/**
 *@ingroup ota
 *@brief reboot board by application.
 *
 *@par Description:
 *This API is used to reboot board by application. It will store image
 *len to ota_flag, and set ota_state to need_update
 *and reboot.
 *@attention none.
 *
 *@param image_len     [IN] image length downloaded by application.
 *
 *@retval #int          0 if succeed or -1 if failed.
 *@par Dependency: none.
 *@see none.
 */
int ota_set_reboot(int32_t image_len);

/**
 *@ingroup ota
 *@brief check update state when application start.
 *
 *@par Description:
 *This API is used to check update state when application start.
 *@attention none.
 *
 *@param st             [OUT] update state obtained by update_param.
 *
 *@retval #int          0 if succeed or -1 if failed.
 *@par Dependency: none.
 *@see none.
 */
int ota_check_update_state(ota_state* st);

#else

/**
 *@ingroup ota
 *@brief do the actual update process.
 *
 *@par Description:
 *This API is used to do the actual update process.
 *@attention none.
 *
 *@param none.
 *
 *@retval #int          0 if succeed or -1 if failed.
 *@par Dependency: none.
 *@see none.
 */
int ota_update_process(void);

/**
 *@ingroup ota
 *@brief jump to application by bootloader.
 *
 *@par Description:
 *This API is used to jump to application by bootloader.
 *@attention none.
 *
 *@param none.
 *
 *@retval #int          0 if succeed or -1 if failed.
 *@par Dependency: none.
 *@see none.
 */
int ota_jump_to_application(void);

/**
 *@ingroup ota
 *@brief roll back to older image.
 *
 *@par Description:
 *This API is used to roll back to older image if there is a problem
 *with new image.
 *@attention none.
 *
 *@param none.
 *
 *@retval #int          0 if succeed or -1 if failed.
 *@par Dependency: none.
 *@see none.
 */
int ota_roll_back_image(void);

#endif /* USE_BOOTLOADER */

#ifdef __cplusplus
}
#endif

#endif /* _OTA_H_ */
