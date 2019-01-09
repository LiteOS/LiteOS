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
#include <target_config.h>
#if CN_OS_SHELL
#include <shell.h>
#include <string.h>
//test fucntion:print the params passed in
static int shell_test_func(int argc, const char *argv[]){
    int i = 0;
    printf("argc:%d \n\r",argc);
    for(i=0;i<argc;i++){
        printf("args:%d %s\n\r",i,argv[i]);
    }
    return 0;
}
extern UINT32 osGetAllTskInfo(VOID);
static int shell_taskinfo(int argc,const char *argv[])
{
	int32_t ret=0;
    ret = osGetAllTskInfo();	//los_printf.h  #define LOS_ERR_LEVEL           (LOS_COMMOM_LEVEL )//+ 1)//20181130
	if(ret<0)printf("get OS_ERROR\n");
    return 0;
}
static u32_t test_var123 =123;
static u32_t test_var12345 =12345;


#define BEGIN_WITH_NEW_TASK 0 //是否新启线程 开始测试任务
typedef int (*funtask)(void);
extern int task_ALL(void);//全测试
extern int task_Timer(void);
extern int task_SPI_EEPROM(void);
extern int task_AES(void);
extern int task_WatchDog(void);
extern int task_RTC(void);
extern int task_DMA(void);
extern int task_EFLASH(void);
extern int task_Usart(void);
extern int task_PWM(void);
extern int task_RSA(void);
extern int task_SHA(void);
extern int task_CRC(void);
extern int task_TRNG(void);
extern int task_GPIO(void);
extern int task_IIC(void);
extern int task_IIS(void);
extern int task_ADC(void);
//csky_test all
//csky_test timer/wdt/spi/dma/aes/eflash/rtc/usart/pwm/rsa/sha/crc/trng/gpio/iic/iis/adc
#if 1
static int shell_test_CSKY_driver_demo(int argc,const char *argv[])
{
    funtask funcation=NULL;

	if(argc!=2)
	{
			printf("友情提醒：-----------------\n");
			printf("	csky_test  all		备注：测试通过\n");
			printf("	csky_test  timer	备注：测试通过\n");
			printf("	csky_test  dma		备注：测试通过\n");
			printf("	csky_test  aes		备注：测试通过\n");
			printf("	csky_test  eflash	备注：测试通过\n");
			printf("	csky_test  rtc		备注：测试通过\n");
			printf("	csky_test  usart	备注：测试通过\n");
			printf("	csky_test  pwm		备注：测试通过\n");
			printf("	csky_test  rsa		备注：测试通过\n");
			printf("	csky_test  sha		备注：测试通过\n");
			printf("	csky_test  crc		备注：测试通过\n");
			printf("	csky_test  trng		备注：测试通过\n");
			printf("	csky_test  wdt		备注：测试通过\n");
			printf("	csky_test  gpio		备注：测试通过，需要拉低PA17引脚（通过手指触碰）\n");
			printf("	csky_test  spi		备注：模块正常，缺少交互硬件\n");
			printf("	csky_test  iic		备注：模块正常，缺少交互硬件\n");
			printf("	csky_test  iis		备注：模块正常，缺少Audio Codec硬件驱动文件\n");
			printf("	csky_test  adc		备注：未编写\n");
			printf("--------------------------");
			return 0;
	}
	//此处可以编辑一个函数，将字符串中的字母变为小写 20181129
	//略

	//匹配命令
	if(!strcmp(argv[1],"all"))	{printf("开始测试所有驱动DEMO！\n");task_create("CSKY_ALL",(fnTaskEntry)task_ALL,0x800,NULL,NULL,10);return 0;}
	if(!strcmp(argv[1],"timer")){printf("开始测试定时器！\n");	funcation=task_Timer;}
	if(!strcmp(argv[1],"wdt"))	{printf("开始测试看门狗！\n");	task_create("CSKY_WDT",(fnTaskEntry)task_WatchDog,0x400,NULL,NULL,10);return 0;}
	if(!strcmp(argv[1],"spi"))	{printf("开始测试SPI！\n");		funcation=task_SPI_EEPROM;}
	if(!strcmp(argv[1],"dma"))	{printf("开始测试DMAC！\n");	funcation=task_DMA;}
	if(!strcmp(argv[1],"aes"))	{printf("开始测试AES！\n");		funcation=task_AES;}
	if(!strcmp(argv[1],"eflash")){printf("开始测试Eflash！\n");	funcation=task_EFLASH;}
	if(!strcmp(argv[1],"rtc"))	{printf("开始测试RTC！\n");		funcation=task_RTC;}
	if(!strcmp(argv[1],"usart")){printf("开始测试USART！\n");	funcation=task_Usart;}
	if(!strcmp(argv[1],"pwm"))	{printf("开始测试PWM！\n");		funcation=task_PWM;}
	if(!strcmp(argv[1],"rsa"))	{printf("开始测试RSA！\n");		funcation=task_RSA;}
	if(!strcmp(argv[1],"sha"))	{printf("开始测试SHA！\n");		funcation=task_SHA;}
	if(!strcmp(argv[1],"crc"))	{printf("开始测试CRC！\n");		funcation=task_CRC;}
	if(!strcmp(argv[1],"trng"))	{printf("开始测试TRNG！\n");	funcation=task_TRNG;}
	if(!strcmp(argv[1],"gpio"))	{printf("开始测试GPIO！\n");	funcation=task_GPIO;}
	if(!strcmp(argv[1],"iic"))	{printf("开始测试IIC！\n");	    funcation=task_IIC;}
	if(!strcmp(argv[1],"iis"))	{printf("开始测试IIS！\n");	    funcation=task_IIS;}
	if(!strcmp(argv[1],"adc"))	{printf("开始测试ADC！\n");	    funcation=task_ADC;}
	
	//如果没有匹配成功
	if(funcation==NULL){printf("您输入的命令不正确(输入csky_test查询可使用的命令)\n");return 0;}
	
	//匹配成功
	if(BEGIN_WITH_NEW_TASK)
	{
		task_create("CSKY_TEST",(fnTaskEntry)funcation,0x400,NULL,NULL,10);
	}
	else
	{
		funcation();
	}	
    return 0;
}
#else

static int shell_test_CSKY_driver_demo(int argc,const char *argv[])
{
	int8_t ret=0;
    if(argc!=2)
	{
			printf("友情提醒：csky_test  all/timer/wdt/spi/dma/aes/eflash/rtc/usart\n");
			return 0;
	}
	ret = strcmp(argv[1],"all");
	if(!ret)
	{
			printf("开始测试所有驱动DEMO！\n");
			task_create("task_ALL",(fnTaskEntry)task_ALL,0x800,NULL,NULL,10);//测试全部
			return 0;
	}
	ret = strcmp(argv[1],"timer");
	if(!ret)
	{
			printf("开始测试定时器！\n");
			task_create("task_Timer",(fnTaskEntry)task_Timer,0x400,NULL,NULL,10);
			
			return 0;
	}
	ret = strcmp(argv[1],"wdt");
	if(!ret)
	{
			printf("开始测试看门狗！\n");
			task_create("task_WatchDog",(fnTaskEntry)task_WatchDog,0x400,NULL,NULL,10);
			return 0;
	}
	ret = strcmp(argv[1],"spi");
	if(!ret)
	{
			printf("开始测试SPI！\n");
			task_create("task_SPI_EEPROM",(fnTaskEntry)task_SPI_EEPROM,0x400,NULL,NULL,10);
			return 0;
	}
	
	ret = strcmp(argv[1],"dma");
	if(!ret)
	{
			printf("开始测试DMAC！\n");
			task_create("task_DMA",(fnTaskEntry)task_DMA,0x400,NULL,NULL,10);
			return 0;
	}
		ret = strcmp(argv[1],"aes");
	if(!ret)
	{
			printf("开始测试AES！\n");
			task_create("task_AES",(fnTaskEntry)task_AES,0x400,NULL,NULL,10);
			return 0;
	}
		ret = strcmp(argv[1],"eflash");
	if(!ret)
	{
			printf("开始测试Eflash！\n");
			task_create("task_EFLASH",(fnTaskEntry)task_EFLASH,0x400,NULL,NULL,10);
			return 0;
	}
		ret = strcmp(argv[1],"rtc");
	if(!ret)
	{
			printf("开始测试RTC！\n");
			task_create("task_RTC",(fnTaskEntry)task_RTC,0x400,NULL,NULL,10);
			return 0;
	}
		ret = strcmp(argv[1],"usart");
	if(!ret)
	{
			printf("未编写串口测试代码！\n");
			return 0;
	}
		
    return 0;
}
#endif

/*
 * 20181130
 * 来源：https://blog.csdn.net/csdn_kou/article/details/81297796
 * clear
 * */
int clear_window(int argc, char **argv) 
{
    fputs("\x1b[2J\x1b[H", stdout);
    return 0;
}



//here we export to shell command,you could add your own shell like this
OSSHELL_EXPORT_VAR(test_var12345,				"var12345",			"[测试]显示变量的数值");
OSSHELL_EXPORT_VAR(test_var123,					"var123",			"[测试]显示变量的数值");
OSSHELL_EXPORT_CMD(shell_test_func,				"shell_test_func",	"[测试]展开命令及参数");
OSSHELL_EXPORT_CMD(shell_taskinfo,				"taskinfo",			"shell show taskinfo");
OSSHELL_EXPORT_CMD(shell_test_CSKY_driver_demo,	"csky_test",		"[测试]中天微CH2201开发板");
OSSHELL_EXPORT_CMD(clear_window,				"clear",			"清空界面");//20181130




#endif