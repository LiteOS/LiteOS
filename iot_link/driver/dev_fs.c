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
 *  2019-05-14 09:39  zhangqianfu  The first version  
 *
 */

#if CFG_DEVFS_ENABLE

#include <fs/los_vfs.h>
#include <driver.h>

#define cn_devfs_timeout 0xffffffff
#define cn_devfs_name    "devfs"
#define cn_devfs_path    "/dev/"

//make the corresponding interface for the file system
static s32_t devfs_open (struct file *file, const char *devname,s32_t flag)
{
    s32_t ret = -1;
    los_dev_t dev;

    dev = los_dev_open(devname,(u32_t)flag);
    if(NULL != dev)
    {
        file->f_data = dev;
        ret = 0;
    }
    return ret;
}

static s32_t devfs_close (struct file *file)
{
    s32_t ret = -1;
    los_dev_t dev;

    dev = file->f_data;
    if(los_dev_close(dev))
    {
        ret = 0;
    }
    return ret;
}

static ssize_t devfs_read (struct file *file, char *buf, size_t len)
{
    ssize_t ret = 0;
    los_dev_t dev;

    dev = file->f_data;
    ret = los_dev_read (dev,file->f_offset,(void *)buf,len,cn_devfs_timeout);
    if(ret > 0)
    {
        file->f_offset += ret;
    }

    return ret;

}


static ssize_t devfs_write (struct file *file, const char *buf, size_t len)
{
    ssize_t ret = 0;
    los_dev_t dev;

    dev = file->f_data;
    ret = los_dev_write (dev,file->f_offset,(const void *)buf,len,cn_devfs_timeout);
    if(ret > 0)
    {
        file->f_offset += ret;
    }
    return ret;
}

static s32_t devfs_ioctl (struct file *file, int cmd, unsigned long para)
{

    s32_t ret = -1;
    los_dev_t dev;

    dev = file->f_data;

    if(los_dev_ioctl(dev,cmd,(void *)para,0))
    {
        ret = 0;
    }

    return ret;
}

static off_t devfs_seek (struct file *file, off_t offset, int fromwhere)
{

    s32_t ret = -1;
    los_dev_t dev;

    dev = file->f_data;

    ret = los_dev_seek(dev,offset,fromwhere);

    return ret;
}


static const struct file_ops  s_devfs_ops ={
    .open = devfs_open,
    .close = devfs_close,
    .read = devfs_read,
    .write = devfs_write,
    .ioctl = devfs_ioctl,
};

static struct file_system s_devfs =
{
    cn_devfs_name,
    (struct file_ops *)&s_devfs_ops,
    NULL,
    0
};
bool_t devfs_install()
{
    los_fs_register(&s_devfs);
    los_fs_mount(cn_devfs_name,cn_devfs_path,NULL);

    return  true;  //make nothing check, should we do it?
}

#endif

