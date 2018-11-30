/*
 * Copyright (C) 2017 C-SKY Microsystems Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/******************************************************************************
 * @file     minilibc_port.c
 * @brief    minilibc port
 * @version  V1.0
 * @date     26. Dec 2017
 ******************************************************************************/

#include <stdio.h>
#include <csi_config.h>
#include <drv_usart.h>

/*20181127 对liteOS宏 进行整理，发现不应该定义CONFIG_KERNEL_NONE，此处代码为去除CONFIG_KERNEL_NONE定义后，针对csi自带的内核文件 进行包含的代码。
#ifndef CONFIG_KERNEL_NONE
#include <csi_kernel.h>
#endif
*/
#include "csky_All_Demo.h"

usart_handle_t console_handle = NULL;

#if LOSCFG_PLATFORM_HWI


#else
int fputc(int ch, FILE *stream)
{
    if (console_handle == NULL) {
        return -1;
    }

    if (ch == '\n') {
        csi_usart_putchar(console_handle, '\r');
    }

    csi_usart_putchar(console_handle, ch);

    return 0;
}

int fgetc(FILE *stream)
{
    uint8_t ch;

    if (console_handle == NULL) {
        return -1;
    }

    csi_usart_getchar(console_handle, &ch);

    return ch;
}
#endif



int os_critical_enter(unsigned int *lock)
{
/*20181127 对liteOS宏 进行整理，发现不应该定义CONFIG_KERNEL_NONE，此处代码csi自带的内核文件的代码,不是liteos的内核函数代码。
#ifndef CONFIG_KERNEL_NONE
    csi_kernel_sched_suspend();
#endif
*/
    return 0;
}

int os_critical_exit(unsigned int *lock)
{
	/*20181127 对liteOS宏 进行整理，发现不应该定义CONFIG_KERNEL_NONE，此处代码csi自带的内核文件的代码,不是liteos的内核函数代码。
#ifndef CONFIG_KERNEL_NONE
    csi_kernel_sched_resume(0);
#endif
*/
    return 0;
}
