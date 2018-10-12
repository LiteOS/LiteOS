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

/* Includes -----------------------------------------------------------------*/
#include "fs_demo.h"
#include "hal_spi_flash.h"

#if defined (__GNUC__) || defined (__CC_ARM)
#include <sys/fcntl.h>
#include <los_printf.h>
#endif

#include "los_vfs.h"
/* Defines ------------------------------------------------------------------*/
#define TEST_FS_SPIFFS      0
#define TEST_FS_FATFS       1

#define USE_TEST_TYPE       TEST_FS_FATFS

#define SPIFFS_PATH         "/spiffs"
#define FATFS_PATH          "/fatfs"

#define LOS_FILE            "f.txt"
#define LOS_DIR             "d"
/* Typedefs -----------------------------------------------------------------*/
#ifndef FS_PRINTF
#define FS_PRINTF(fmt, arg...)  printf("[%s:%d]" fmt "\r\n", __func__, __LINE__, ##arg)
#endif

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
static char s_ucaWriteBuffer[] = "IP:192.168.0.100\r\nMASK:255.255.255.0\r\nGW:192.168.0.1";
static char s_ucaReadBuffer[100] = {"spiffs read failed"};

char file_name[100] = {0};
char dir_name[100] = {0};

/* Extern variables ---------------------------------------------------------*/

extern int stm32f1xx_spiffs_init(void);
extern int stm32f1xx_fatfs_init(void);
/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
int write_file(const char *name, char *buff, int len)
{
    int fd;
    int ret;
    
    if(name == NULL || buff == NULL || len <= 0)
    {
        FS_PRINTF("invalid parameter.");
        return -1;
    }
    fd = los_open(name, O_CREAT | O_WRONLY | O_TRUNC);
    if(fd < 0)
    {
        FS_PRINTF("los_open file %s failed.", name);
        return -1;
    }
    ret = los_write(fd, buff, len);
    if(ret < 0)
    {
        FS_PRINTF("los_write file %s failed.", name);
        los_close(fd);
        return -1;
    }
    los_close(fd);
    return 0;
}

int read_file(const char *name, char *buff, int len)
{
    int fd;
    int ret;
    
    if(name == NULL || buff == NULL || len <= 0)
    {
        FS_PRINTF("invalid parameter.");
        return -1;
    }
    fd = los_open(name, O_RDONLY);
    if(fd < 0)
    {
        FS_PRINTF("los_open file %s failed.", name);
        return -1;
    }
    ret  = los_read(fd, buff, len);
    if(ret <= 0)
    {
        FS_PRINTF("los_read file %s failed.", name);
        los_close(fd);
        return -1;
    }
    los_close(fd);
    return 0;
}

int open_dir(const char *name, struct dir **dir)
{
    int ret = 0;
    int counter = 3;

    if(name == NULL || dir == NULL)
    {
        FS_PRINTF("invalid parameter.");
        return -1;
    }
    
    do 
    {
        *dir = los_opendir(name);
        if(*dir == NULL)
        {
            FS_PRINTF("los_opendir %s failed, ret=%d.", name, ret);
            ret = los_mkdir(name, 0);
            if(ret != 0)
            {
                FS_PRINTF("los_mkdir %s failed, ret=%d.", name, ret);
            }
            else
            {
                FS_PRINTF("los_mkdir %s successfully.", name);
            }
        }
    }while(*dir == NULL && ret != 0 && --counter > 0);

    if(counter <= 0)
    {
        FS_PRINTF("los_opendir/los_mkdir %s failed, ret=%d.", name, ret);
        return -1;
    }
    return 0;
}

int read_dir(const char *name, struct dir *dir)
{
    int flag = 1;
    struct dirent *pDirent = NULL;

    if(name == NULL || dir == NULL)
    {
        FS_PRINTF("invalid parameter.");
        return -1;
    }
    
    while(1)
    {
        pDirent = los_readdir(dir);
        if(pDirent == NULL || pDirent->name[0] == '\0')
        {
            if(flag == 1)
            {
                FS_PRINTF("los_readdir %s failed.", name);
                return -1;
            }
            else break;
        }
        flag = 0;
        printf("los_readdir %s: name=%s, type=%ld, size=%ld\n", name, pDirent->name, 
            pDirent->type, pDirent->size);
    }
    return 0;
}

void los_fs_test(void)
{
    int ret = 0;
    struct dir *pDir = NULL;
    int wrlen = sizeof(s_ucaWriteBuffer) - 1;
    int rdlen = sizeof(s_ucaReadBuffer);
    
    rdlen = MIN(wrlen, rdlen);

    /**************************
     *  file operation test 
     **************************/
    ret = write_file(file_name, s_ucaWriteBuffer, wrlen);
    if(ret < 0) return;
    
    ret = read_file(file_name, s_ucaReadBuffer, rdlen);
    if(ret < 0) return;
    printf("*********** readed %d data ***********\r\n%s\r\n"
           "**************************************\r\n", rdlen, s_ucaReadBuffer);

    /****************************
     *  dir operation test   
     ****************************/
    sprintf(file_name, "%s/%s", dir_name, LOS_FILE);
    ret = open_dir(dir_name, &pDir);
    if(ret < 0) return;

    // comment this to test los_unlink
    ret = write_file(file_name, s_ucaWriteBuffer, wrlen);
    if(ret < 0) return;

    ret = read_dir(dir_name, pDir);
    if(ret < 0) return;

    ret = los_closedir(pDir);
    if(ret < 0)
    {
        FS_PRINTF("los_closedir %s failed.", dir_name);
        return;
    }
    los_unlink(file_name); // remove file_name
}

#if (USE_TEST_TYPE == TEST_FS_SPIFFS)
void spiffs_demo(void)
{
    int ret = 0;

    ret = stm32f1xx_spiffs_init();
    if(ret == LOS_NOK)
    {
        FS_PRINTF("stm32f1xx_spiffs_init failed.");
        return;
    }
    sprintf(file_name, "%s/%s", SPIFFS_PATH, LOS_FILE);
    sprintf(dir_name, "%s/%s", SPIFFS_PATH, LOS_DIR);

    los_fs_test();
}
#endif

#if (USE_TEST_TYPE == TEST_FS_FATFS)
void fatfs_demo(void)
{
    int8_t drive;

    drive = stm32f1xx_fatfs_init();
    if(drive < 0)
    {
        FS_PRINTF("stm32f1xx_fatfs_init failed.");
        return;
    }
    sprintf(file_name, "%s/%d:/%s", FATFS_PATH, (uint8_t)drive, LOS_FILE);
    sprintf(dir_name,  "%s/%d:/%s", FATFS_PATH, (uint8_t)drive, LOS_DIR);

    los_fs_test();
}
#endif

void fs_demo(void)
{
    printf("Huawei LiteOS File System Demo.\r\n");

//    hal_spi_flash_config();
//    hal_spi_flash_erase_bulk();

#if (USE_TEST_TYPE == TEST_FS_SPIFFS)  
    spiffs_demo();
#elif (USE_TEST_TYPE == TEST_FS_FATFS)
    fatfs_demo();
#endif
    while(1)
    {

    }
}

/* Private functions --------------------------------------------------------*/

