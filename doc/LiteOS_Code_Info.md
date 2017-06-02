

## LiteOS代码入口

- LiteOS入口在user/main.c中

		int main(void)
		{
    		UINT32 uwRet;
			/*
				add you hardware init code here
				for example flash, i2c , system clock ....
			*/
			//HAL_init();....

			/*Init LiteOS kernel */
			uwRet = LOS_KernelInit();
			if (uwRet != LOS_OK) {
				return LOS_NOK;
			}
			/* Enable LiteOS system tick interrupt */
			LOS_EnableTick();

			/*
				Notice: add your code here
				here you can create task for your function 
				do some hw init that need after systemtick init
			*/
			LOS_EvbSetup();//init the device on the dev baord

			//LOS_Demo_Entry();

			LOS_Inspect_Entry();

			//LOS_BoadExampleEntry();

			/* Kernel start to run */
			LOS_Start();
			for (;;);
			/* Replace the dots (...) with your own code. */
		}

   其中第一步进行LiteOS的内核初始化  LOS_KernelInit();
   然后使能 system tick中断 LOS_EnableTick();

   之后进行了一些演示用的串口、led等的初始化，以及测试代码的运行LOS_EvbSetup();LOS_Inspect_Entry();

   最后调用LOS_Start();开始task调度，LiteOS开始正常工作，

## LiteOS的代码目录结构说明

关于代码树中各个目录存放的源代码的相关内容简介如下：

<table>
<tr>
	<td width="10%">一级目录</td>
	<td width="25%">二级目录</td>
	<td width="10%">三级目录</td>
	<td >说明</td>
</tr>
<tr>
	<td>doc</td>
	<td></td>
	<td></td>
	<td>此目录存放的是LiteOS的使用文档和API说明文档</td>
</tr>
<tr>
	<td>example</td>
	<td>api</td>
	<td></td>
	<td>此目录存放的是内核功能测试用的相关用例的代码</td>
</tr>
<tr>
	<td></td>
	<td>include</td>
	<td></td>
	<td>aip功能头文件存放目录</td>
</tr>
<tr>
	<td>kernel</td>
	<td>base</td>
	<td>core</td>
	<td>LiteOS内核核心功能代码，包括队列、task调度、软timer、时间片计算等功能</td>
</tr>
<tr>
	<td></td>
	<td></td>
	<td>fs</td>
	<td>LiteOS文件系统的抽象层接口</td>
</tr>
<tr>
	<td></td>
	<td></td>
	<td>include</td>
	<td>LiteOS内核内部使用的头文件</td>
</tr>
<tr>
	<td></td>
	<td></td>
	<td>ipc</td>
	<td>LiteOS中task间通讯的相关接口，包括事件、信号量、消息队列、互斥锁等</td>
</tr>
<tr>
	<td></td>
	<td></td>
	<td>mem</td>
	<td>LiteOS中的内核内存管理的相关代码</td>
</tr>
<tr>
	<td></td>
	<td></td>
	<td>misc</td>
	<td>内存对齐功能以及毫秒级休眠sleep功能</td>
</tr>
<tr>
	<td></td>
	<td>cmsis</td>
	<td></td>
	<td>LiteOS提供的cmsis接口</td>
</tr>
<tr>
	<td></td>
	<td>config</td>
	<td></td>
	<td>此目录下是内核资源配置相关的代码，在头文件中配置了LiteOS所提供的各种资源所占用的内存池的总大小以及各种资源的数量，例如task的最大个数、信号量的最大个数等等</td>
</tr>
<tr>
	<td></td>
	<td>cpu</td>
	<td>arm/cortex-m0</td>
	<td>arm系列M0核的LiteOS的适配代码，包括调度以及开关中断等等的汇编实现</td>
</tr>
<tr>
	<td></td>
	<td></td>
	<td>arm/cortex-m0+</td>
	<td>arm系列M0+核的LiteOS的适配代码，包括调度以及开关中断等等的汇编实现</td>
</tr>
<tr>
	<td></td>
	<td></td>
	<td>arm/cortex-m3</td>
	<td>arm系列M3核的LiteOS的适配代码，包括调度以及开关中断等等的汇编实现</td>
</tr>
<tr>
	<td></td>
	<td></td>
	<td>arm/cortex-m7</td>
	<td>arm系列M7核的LiteOS的适配代码，包括调度以及开关中断等等的汇编实现</td>
</tr>
</tr>
<tr>
	<td></td>
	<td></td>
	<td>arm/cortex-m4</td>
	<td>arm系列M4核的LiteOS的适配代码，包括调度以及开关中断等等的汇编实现</td>
</tr>
</tr>
<tr>
	<td></td>
	<td>include</td>
	<td></td>
	<td>LiteOS内核对外部开放头文件存放目录</td>
</tr>
<tr>
	<td></td>
	<td>link</td>
	<td>gcc</td>
	<td>gcc方式编译时所需要包含的特殊公共头文件</td>
</tr>
<tr>
	<td></td>
	<td></td>
	<td>iar</td>
	<td>IAR方式编译时所需要包含的特殊公共头文件</td>
</tr>
<tr>
	<td></td>
	<td></td>
	<td>keil</td>
	<td>KEIL方式编译时所需要包含的特殊公共头文件</td>
</tr>
<tr>
	<td>platform</td>
	<td>GD32F190R-EVAL</td>
	<td></td>
	<td>GD190开发板systick以及led、uart、key驱动bsp适配代码</td>
</tr>
<tr>
	<td></td>
	<td>GD32F450i-EVAL</td>
	<td></td>
	<td>GD450开发板systick以及led、uart、key驱动bsp适配代码</td>
</tr>
<tr>
	<td></td>
	<td>STM32F412ZG-NUCLEO</td>
	<td></td>
	<td>STM32F412开发板systick以及led、uart、key驱动bsp适配代码</td>
</tr>
<tr>
	<td></td>
	<td>STM32F429I_DISCO</td>
	<td></td>
	<td>STM32F429开发板systick以及led、uart、key驱动bsp适配代码</td>
</tr>
<tr>
	<td></td>
	<td>STM32L476RG_NUCLEO</td>
	<td></td>
	<td>STM32L476开发板systick以及led、uart、key驱动bsp适配代码</td>
</tr>
<tr>
	<td></td>
	<td>LPC824_LITE</td>
	<td></td>
	<td>LPC824Lite开发板systick及驱动相关代码</td>
</tr>
<tr>
	<td></td>
	<td>LPC54110_BOARD</td>
	<td></td>
	<td>LPC54110开发板systick及驱动相关代码</td>
</tr>
<tr>
	<td></td>
	<td>MM32F103_MINI</td>
	<td></td>
	<td>MM32F103开发板systick及驱动相关代码</td>
</tr>
<tr>
	<td></td>
	<td>STM32F746ZG_NUCLEO</td>
	<td></td>
	<td>STM32F746ZG开发板systick以及led、uart、key驱动bsp适配代码</td>
</tr>
<tr>
	<td></td>
	<td>LOS_EXPAND_XXX</td>
	<td></td>
	<td>用于新扩展的开发板systick以及led、uart、key驱动bsp适配代码</td>
</tr>

<tr>
	<td></td>
	<td>Arduino_M0_Pro</td>
	<td></td>
	<td>Arduino_M0_Pro开发板systick以及led、uart、key驱动bsp适配代码</td>
</tr>
<tr>
	<td></td>
	<td>ATSAM4S-XPRO</td>
	<td></td>
	<td>ATSAM4S-XPRO开发板systick以及led、uart、key驱动bsp适配代码</td>
</tr>
<tr>
	<td></td>
	<td>ATSAMD21-XPRO</td>
	<td></td>
	<td>ATSAMD21-XPRO开发板systick以及led、uart、key驱动bsp适配代码</td>
</tr>
<tr>
	<td></td>
	<td>EFM32-SLSTK3400A</td>
	<td></td>
	<td>EFM32-SLSTK3400A开发板systick以及led、uart、key驱动bsp适配代码</td>
</tr>
<tr>
	<td></td>
	<td>EFM32-SLSTK3401A</td>
	<td></td>
	<td>EFM32-SLSTK3401A开发板systick以及led、uart、key驱动bsp适配代码</td>
</tr>
<tr>
	<td></td>
	<td>EFM32-STK3700</td>
	<td></td>
	<td>EFM32-STK3700开发板systick以及led、uart、key驱动bsp适配代码</td>
</tr>
<tr>
	<td></td>
	<td>FRDM-KL26Z</td>
	<td></td>
	<td>FRDM-KL26Z开发板systick以及led、uart、key驱动bsp适配代码</td>
</tr>
<tr>
	<td></td>
	<td>FRDM-KW41Z</td>
	<td></td>
	<td>FRDM-KW41Z开发板systick以及led、uart、key驱动bsp适配代码</td>
</tr>
<tr>
	<td></td>
	<td>STM32F411RE-NUCLEO</td>
	<td></td>
	<td>STM32F411RE-NUCLEO开发板systick以及led、uart、key驱动bsp适配代码</td>
</tr>
<tr>
	<td>projects</td>
	<td>STM32F412ZG-NUCLEO-KEIL</td>
	<td></td>
	<td>stm32f412开发板的keil工程目录</td>
</tr>
<tr>
	<td></td>
	<td>STM32F429I_DISCO_IAR</td>
	<td></td>
	<td>stm32f429开发板的iar工程目录</td>
</tr>
<tr>
	<td></td>
	<td>STM32F429I_DISCO_KEIL</td>
	<td></td>
	<td>stm32f429开发板的keil工程目录</td>
</tr>
<tr>
	<td></td>
	<td>STM32L476R-NUCLEO-KEIL</td>
	<td></td>
	<td>stm32l476开发板的keil工程目录</td>
</tr>
<tr>
	<td></td>
	<td>GD32F190R-EVAL-KEIL</td>
	<td></td>
	<td>gd32f190开发板的keil工程目录</td>
</tr>
<tr>
	<td></td>
	<td>GD32F450i-EVAL-KEIL</td>
	<td></td>
	<td>gd32f450开发板的keil工程目录</td>
</tr>
<tr>
	<td></td>
	<td>LPC824_LITE_KEIL</td>
	<td></td>
	<td>lpc824Lite开发板的keil工程目录</td>
</tr>
<tr>
	<td></td>
	<td>LPC54110_BOARD_KEIL</td>
	<td></td>
	<td>lpc54110开发板的keil工程目录</td>
</tr>
<tr>
	<td></td>
	<td>MM32F103_MINI_KEIL</td>
	<td></td>
	<td>MindMotion MM32 MiniBoard开发板的keil工程目录</td>
</tr>
<tr>
	<td></td>
	<td>STM32F746ZG-NUCLEO-KEIL</td>
	<td></td>
	<td>NUCLEO-F746ZG 开发板的keil工程目录</td>
</tr>


<tr>
	<td></td>
	<td>Arduino_M0_PRO_KEIL</td>
	<td></td>
	<td>Arduino_M0_PRO开发板的keil工程目录</td>
</tr>
<tr>
	<td></td>
	<td>ATSAM4S-XPRO-KEIL</td>
	<td></td>
	<td>ATSAM4S-XPRO开发板的keil工程目录</td>
</tr>
<tr>
	<td></td>
	<td>ATSAMD21-XPRO-KEIL</td>
	<td></td>
	<td>ATSAMD21-XPRO开发板的keil工程目录</td>
</tr>
<tr>
	<td></td>
	<td>EFM32-SLSTK3400A-IAR</td>
	<td></td>
	<td>EFM32-SLSTK3400A开发板的IAR工程目录</td>
</tr>
<tr>
	<td></td>
	<td>EFM32-SLSTK3401A-IAR</td>
	<td></td>
	<td>EFM32-SLSTK3401A开发板的IAR工程目录</td>
</tr>
<tr>
	<td></td>
	<td>EFM32-STK3700-KEIL</td>
	<td></td>
	<td>EFM32-STK3700开发板的keil工程目录</td>
</tr>
<tr>
	<td></td>
	<td>FRDM-KL26Z-KEIL</td>
	<td></td>
	<td>FRDM-KL26Z开发板的keil工程目录</td>
</tr>
<tr>
	<td></td>
	<td>STM32F429I_DISCO_GCC</td>
	<td></td>
	<td>STM32F429I_DISCO开发板的GCC工程目录</td>
</tr>
<tr>
	<td></td>
	<td>FRDM-KW41Z-IAR</td>
	<td></td>
	<td>FRDM-KW41Z开发板的IAR工程目录</td>
</tr>
<tr>
	<td></td>
	<td>STM32F411RE-NUCLEO-KEIL</td>
	<td></td>
	<td>STM32F411RE-NUCLEO开发板的KEIL工程目录</td>
</tr>
<tr>
	<td>user</td>
	<td></td>
	<td></td>
	<td>此目录存放用户测试代码，LiteOS的初始化和使用示例在main.c文件中</td>
</tr>
</table>