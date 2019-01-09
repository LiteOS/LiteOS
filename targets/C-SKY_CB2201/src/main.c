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
 * @file     main.c
 * @brief    Hwi LiteOS
 * @version  V2.0
 * @date     8. Nov 2018
 ******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <csi_config.h>
#include "soc.h"
#include "csi_core.h"
#include "pin.h"
#include "los_task.h"
#include "csky_All_Demo.h"
#include "osport.h"

extern VOID *main_ppp(UINT32  args);
extern VOID *testTask(UINT32  args);

//系统初始化
void SystemInit(void)
{
    /* Here we may setting exception vector, MGU, cache, and so on. */
    __set_VBR((uint32_t) & (__Vectors));
    //
    //csi_coret_config(SYSTEM_CLOCK / 100, CORET_IRQn);    //10ms
    //csi_vic_enable_irq(CORET_IRQn);
}

//开发板相关的初始化工作
void board_init(void)
{
	//初始化
}
int task_ALL(void)
{
		task_DMA();		//测试通过
		task_EFLASH();	//测试通过
		task_RTC();		//测试通过
		task_Timer();	//测试通过
		task_AES();		//测试通过
		task_PWM();		//测试通过
		task_RSA();		//测试通过
		task_SHA();		//测试通过
		task_CRC();		//测试通过
		task_TRNG();	//测试通过
		task_Usart();	//测试通过
		task_GPIO();	//测试通过，需要手指触碰一下PA17引脚，产生电平变化
		task_SPI_EEPROM();	//缺少硬件交互
		task_IIC();			//缺少硬件交互
		task_ADC();		//未编写
		task_IIS();		//无Audio Codec驱动文件
		task_WatchDog();//测试通过（死循环）
		return 0;
}



extern void CSKY_USART_INIT();
extern int usart_init(int);
extern void shell_install();

int main(void)
{
	/*
	 *1、内核初始化{
	 * 			配置最大的任务数限制+1（IDLE空闲任务）
	 * 			堆得首地址从传递
	 * 			内存初始化
	 * 			进行华为相关的初始化
	 * 			任务初始化
	 * 			等等
	 * }
	 */
	LOS_KernelInit();
	
	//2、串口初始化
    ioreuse_initial();
#if LOSCFG_PLATFORM_HWI
	usart_init(2);
	usart_init(0);
#else
	CSKY_USART_INIT();
#endif

	//3、shell模块装载
	shell_install();
	
	//4、创建任务（在shell中使用命令启动）
	//task_create("main_ppp",(fnTaskEntry)main_ppp,0x800,NULL,NULL,0);
	//task_create("task_ALL",(fnTaskEntry)task_ALL,0x800,NULL,NULL,10);//测试
	
	//5、启动系统
	LOS_Start();
    return 0;
}

