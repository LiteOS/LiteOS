/*----------------------------------------------------------------------------
 * Copyright (c) <2018>, <Huawei Technologies Co., Ltd>
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
/**
 *  DATE                AUTHOR      INSTRUCTION
 *  2019-05-30 11:18  zhangqianfu  The first version
 *
 */
#ifndef LITEOS_LAB_IOT_LINK_OTA_OTA_STORAGE_OTA_STORAGE_H_
#define LITEOS_LAB_IOT_LINK_OTA_OTA_STORAGE_OTA_STORAGE_H_


typedef enum
{
    en_ota_storage_msg_type_runimg = 0,  ///< the one is running
    en_ota_storage_msg_type_bakimg,      ///< the one for backup
    en_ota_storage_msg_type_newimg,      ///< the one for downloading
    en_ota_storage_msg_type_bootflag,    ///< storage for the bootflag,128 bytes
    en_ota_storage_msg_type_transflag,   ///< storage for the transfer flag, 896 bytes

}en_ota_storage_msg_type;

/**
 * @brief: use these function to do the write and read for the ota
 *
 *       :take care that you may have different ways to do it
 * */

typedef int (*fn_ota_write)(en_ota_storage_msg_type type,void *msg,int len, int offset);
typedef int (*fn_ota_write)(en_ota_storage_msg_type type,void *msg,int len, int offset);

typedef struct
{
    fn_ota_write write;
    fn_ota_read  read;
}ota_device_opt_t;


typedef struct
{
    const char        *name;
    int                boot_flag_page_size;
    int                trans_flag_page_size;
    int                run_img_page_size;
    int                bak_img_page_size;
    int                new_img_page_size;
    int                run_img_max_size;
    int                bak_img_max_size;
    int                new_img_max_size;
    ota_device_opt_t   opt;
}ota_storage_device;

/**
 * @brief:you must call this function first to initialize the ota storage module
 *
 * @return: 0 sucess while false failed
 * */
int ota_storage_init();

/**
 * @brief: you should call this function to register a device for the ota
 * @param[in]:device, which used for the storage of the firmware and flag
 *
 * @return:0 success while -1 failed
 * */
int ota_storage_install(const ota_storage_device *device);

/**
 * @brief: the loader and the tansfer protocol will use these function
 * @param[in]:type,direct which flash section to write
 * @param[in]:msg,this is a  message buffer for read or write
 * @param[in]:offset,from where to read or write
 *
 * */
int ota_read_msg(en_ota_storage_msg_type type,void *msg,int len, int offset);
int ota_write_msg(en_ota_storage_msg_type type,void *msg,int len, int offset);


ota_storage_device *ota_storage_get_info();


#endif /* LITEOS_LAB_IOT_LINK_OTA_OTA_STORAGE_OTA_STORAGE_H_ */
