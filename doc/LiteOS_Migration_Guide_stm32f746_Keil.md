## 1关于本文档的开源协议说明
**您可以自由地：**

**分享** 

- 在任何媒介以任何形式复制、发行本文档

**演绎** 

- 修改、转换或以本文档为基础进行创作。只要你遵守许可协议条款，许可人就无法收回你的这些权利。

**惟须遵守下列条件：**

**署名** 

- 您必须提供适当的证书，提供一个链接到许可证，并指示是否作出更改。您可以以任何合理的方式这样做，但不是以任何方式表明，许可方赞同您或您的使用。

**非商业性使用** 

- 您不得将本作品用于商业目的。

**相同方式共享** 

- 如果您的修改、转换，或以本文档为基础进行创作，仅得依本素材的
授权条款来散布您的贡献作品。

**没有附加限制** 

- 您不能增设法律条款或科技措施，来限制别人依授权条款本已许可的作为。

**声明：**

-  当您使用本素材中属于公众领域的元素，或当法律有例外或限制条款允许您的使用，
则您不需要遵守本授权条款。
未提供保证。本授权条款未必能完全提供您预期用途所需要的所有许可。例如：形象
权、隐私权、著作人格权等其他权利，可能限制您如何使用本素材。

**注意**

- 为了方便用户理解，这是协议的概述. 可以访问网址 https://creativecommons.org/licenses/by-sa/3.0/legalcode 了解完整协议内容.

## 2前言
### 目的
本文档介绍基于Huawei LiteOS如何移植到第三方开发板，并成功运行基础示例。
### 读者对象
本文档主要适用于Huawei LiteOS Kernel的开发者。
本文档主要适用于以下对象：
- 物联网端软件开发工程师
- 物联网架构设计师

### 符号约定
在本文中可能出现下列标志，它们所代表的含义如下。


![](./meta/keil/danger.png)     用于警示紧急的危险情形，若不避免，将会导致人员死亡或严重的人身伤害

![](./meta/keil/warning.png)    用于警示潜在的危险情形，若不避免，可能会导致人员死亡或严重的人身伤害

![](./meta/keil/careful.png)    用于警示潜在的危险情形，若不避免，可能会导致中度或轻微的人身伤害

![](./meta/keil/notice.png)     用于传递设备或环境安全警示信息，若不避免，可能会导致设备损坏、数据丢失、设备性能降低或其它不可预知的结果“注意”不涉及人身伤害

| 说明	|		“说明”不是安全警示信息，不涉及人身、设备及环境伤害信息	|

### 修订记录
修改记录累积了每次文档更新的说明。最新版本的文档包含以前所有文档版本的更新
内容。

<table>
	<tr>
	<td>日期</td>
	<td>修订版本</td>
	<td>描述</td>
	</tr>
	<tr>
	<td>2017年04月28日</td>
	<td>1.0</td>
	<td>完成初稿</td>
	</tr>
</table>

## 3概述

目前在github上已开源的Huawei LiteOS内核源码已适配好STM32F412、... 、STM32F746ZG芯片，本手册将
以STM32F746芯片为例，介绍基于Cortex M7核芯片的移植过程。

## 4环境准备
基于Huawei LiteOS Kernel开发前，我们首先需要准备好单板运行的环境，包括软件环
境和硬件环境。
硬件环境：

<table>
	<tr>
	<td>所需硬件</td>
	<td>描述</td>
	</tr>
	<tr>
	<td>NUCLEO-F746ZG单板</td>
	<td>STM32开发板(芯片型号STM32F746ZG)</td>
	</tr>
	<tr>
	<td>PC机</td>
	<td>用于编译、加载并调试镜像</td>
	</tr>
	<tr>
	<td>电源（5v）</td>
	<td>开发板供电(使用Mini USB连接线)</td>
	</tr>
</table>


软件环境：

<table>
	<tr>
	<td>软件</td>
	<td>描述</td>
	</tr>
	<tr>
	<td>Window 7 操作系统</td>
	<td>安装Keil和st-link的操作系统</td>
	</tr>
	<tr>
	<td>Keil(5.21以上版本)</td>
	<td>用于编译、链接、调试程序代码
	uVision V5.21.1.0 MDK-Lite uVersion:5.21a</td>
	</tr>
	<tr>
	<td>st-link_v2_usbdriver</td>
	<td>开发板与pc连接的驱动程序，用户加载及调试程序代码</td>
	</tr>
</table>

**说明**

Keil工具需要开发者自行购买，ST-Link的驱动程序可以从st link的相关网站获取，采用J-Link还
是ST-Link需要根据具体使用的开发板来确定。这里以STM32F746ZG为例，使用ST-Link。


## 5获取Huawei LiteOS 源码

首先我们需要通过网络下载获取Huawei LiteOS开发包。目前Huawei LiteOS的代码已经
开源，可以直接从网络上获取，步骤如下：

- 仓库地址是https://github.com/LITEOS/LiteOS_Kernel.git 
![](./meta/keil/git_down.png)

- 点击”clone or download”按钮,下载源代码


- 目录结构如下：Huawei LiteOS的源代码目录的各子目录包含的内容如下：
![](./meta/keil/catal.png)


关于代码树中各个目录存放的源代码的相关内容简介如下：

<table>
<tr>
	<td>一级目录</td>
	<td>二级目录</td>
	<td>说明</td>
</tr>
<tr>
	<td>doc</td>
	<td></td>
	<td>此目录存放的是LiteOS的使用文档和API说明文档</td>
</tr>
<tr>
	<td>example</td>
	<td>api</td>
	<td>此目录存放的是内核功能测试用的相关用例的代码</td>
</tr>
<tr>
	<td></td>
	<td>include</td>
	<td>aip功能头文件存放目录</td>
</tr>
<tr>
	<td>kernel</td>
	<td>base</td>
	<td>此目录存放的是与平台无关的内核代码，包含核心提供给外部调用的接口的头文件以及内核中进程调度、进程通信、内存管理等等功能的核心代码。用户一般不需要修改此目录下的相关内容。</td>
</tr>
<tr>
	<td></td>
	<td>cmsis</td>
	<td>LiteOS提供的cmsis接口</td>
</tr>
<tr>
	<td></td>
	<td>config</td>
	<td>此目录下是内核资源配置相关的代码，在头文件中配置了LiteOS所提供的各种资源所占用的内存池的总大小以及各种资源的数量，例如task的最大个数、信号量的最大个数等等</td>
</tr>
<tr>
	<td></td>
	<td>cpu</td>
	<td>此目录以及以下目录存放的是与体系架构紧密相关的适配LiteOS的代码。比如目前我们适配了arm/cortex-m4及arm/cortex-m3系列对应的初始化内容。</td>
</tr>
<tr>
	<td></td>
	<td>include</td>
	<td>内核的相关头文件存放目录</td>
</tr>
<tr>
	<td></td>
	<td>link</td>
	<td>与IDE相关的编译链接相关宏定义</td>
</tr>
<tr>
	<td>platform</td>
	<td>GD32F190R-EVAL</td>
	<td>GD190开发板systick以及led、uart、key驱动bsp适配代码</td>
</tr>
<tr>
	<td></td>
	<td>.....</td>
	<td>.....</td>
</tr>
<tr>
	<td></td>
	<td>STM32F746ZG_NUCLEO</td>
	<td>STM32F746开发板systick以及led、uart、key驱动bsp适配代码</td>
</tr>
<tr>
	<td></td>
	<td>LOS_EXPAND_XXX</td>
	<td>用于新扩展的开发板systick以及led、uart、key驱动bsp适配代码</td>
</tr>
<tr>
	<td>projects</td>
	<td>STM32F412ZG-NUCLEO-KEIL</td>
	<td>stm32f412开发板的keil工程目录</td>
</tr>
<tr>
	<td></td>
	<td>....</td>
	<td>....</td>
</tr>
<tr>
	<td></td>
	<td>STM32F746ZG-NUCLEO-KEIL</td>
	<td>STM32F746ZG开发板的keil工程目录</td>
</tr>
<tr>
	<td>user</td>
	<td></td>
	<td>此目录存放用户测试代码，LiteOS的初始化和使用示例在main.c中</td>
</tr>
</table>


获取Huawei LiteOS源代码之后，我们可以将自己本地已有工程的代码适配到LiteOS内核工程中进行应用开发。

## 6如何适配LiteOS内核工程开发
本章节描述的内容以stm32cubef7开发包中的LwIP_HTTP_Server_Netconn_RTOS示例工程为基础，适配到LiteOS的STM32F746ZG-NUCLEO-KEIL工程中，演示串口输出、按键中断及LED点亮功能。

### 获取STM32软件开发包

- 在添加代码之前，首先需要从ST的官网上面搜索STM32CubeF7，并下载STM32CubeF7软件包。比如可以通过http://www.st.com/content/st_com/zh.html 这个网址进去搜索。 

- 下载代码之后，首先解压缩开发包，然后找到STM32Cube_FW_F7_V1.6.0\Projects\STM32F746ZG-Nucleo\Applications\LwIP\LwIP_HTTP_Server_Netconn_RTOS这个下面的MDK工程做STM32F746的原始代码作参考。


### 添加STM32的BSP软件包到LiteOS工程中

- 分析STM32F746的源代码工程，我们发现工程包含两个部分的内容Drivers以及应用适配程序两部分以及MDK-ARM/startup_stm32f746xx.s这个汇编文件。(注：lwip的相关内容不在关注范围内)

![](./meta/keil/stm32f746/bsp_src1.png)

![](./meta/keil/stm32f746/bsp_src2.png)

- 我们将Drivers以及应用代码拷贝到LiteOS的工程中，比如在LiteOS顶级目录下创建BSP\STM32F7xx目录，然后将Drivers目录拷贝到下面，将inc和src拷贝到下面，并且将startup_stm32f429xx.s也拷贝到STM32F7xx目录中。

![](./meta/keil/stm32f746/bsp_src3.png)


- 代码准备完毕之后，需要将STM32的lwip示例工程的部分代码（红色标注内容）添加到LiteOS的工程中，原始的STM32示例代码结构如下

![](./meta/keil/stm32f746/bsp_src4.png)

- 参考原始的STM32工程中，将对应的文件直接添加到LiteOS工程中，添加后的结果如下所示

![](./meta/keil/stm32f746/bsp_src6.png)

**注意**

在LiteOS工程中，原始的startup文件需要使用startup_stm32f429xx.s替换掉，因为LiteOS的默认startup汇编文件中只实现了能够使LiteOS运行的最小功能。

### 修改los_bsp_adapter.c文件

- 代码添加完毕之后，需要修改los_bsp_adapter.c中的system tick时钟const unsigned int sys_clk_freq = 16000000;修改为const unsigned int sys_clk_freq = 216000000; 因为代码中是给出的默认system clock的值。


### 修改main.c文件

- 在main.c中添加平台初始化以及系统时钟配置的调用

![](./meta/keil/stm32f746/bsp_src7.png)

- 添加如下代码

		void SystemClock_Config(void)
		{
		  RCC_ClkInitTypeDef RCC_ClkInitStruct; 
		  RCC_OscInitTypeDef RCC_OscInitStruct; 
		  /* Enable HSE Oscillator and activate PLL with HSE as source */
		  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
		  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
		  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
		  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
		  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
		  RCC_OscInitStruct.PLL.PLLM = 8;
		  RCC_OscInitStruct.PLL.PLLN = 432;
		  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
		  RCC_OscInitStruct.PLL.PLLQ = 9; 
		  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
		  {
		    while(1) {};
		  }
 	 
		  /* Activate the OverDrive to reach the 216 Mhz Frequency */
		  if(HAL_PWREx_EnableOverDrive() != HAL_OK)
		  {
		    while(1) {};
		  }

		  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
		     clocks dividers */
		  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | 		RCC_CLOCKTYPE_PCLK2);
		  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
		  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
		  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
		  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
		  {
			while(1) {};
		  }
		}

		/**
		  * @brief  Configures EXTI lines 15 to 10 (connected to PC.13 pin) in interrupt mode
		  * @param  None
		  * @retval None
		  */
		static void EXTI15_10_IRQHandler_Config(void)
		{
		  GPIO_InitTypeDef   GPIO_InitStructure;

		  /* Enable GPIOC clock */
		  __HAL_RCC_GPIOC_CLK_ENABLE();

		  /* Configure PC.13 pin as input floating */
		  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
		  GPIO_InitStructure.Pull = GPIO_NOPULL;
		  GPIO_InitStructure.Pin = GPIO_PIN_13;
		  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

		  /* Enable and set EXTI lines 15 to 10 Interrupt to the lowest priority */
		  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
		  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
		}


### 修改stm32f4xx_it.c文件

- 将stm32f4xx_it.c中的SysTick_Handler的实现删除或者if 0掉。网卡中断去掉。添加按键处理程序。

![](./meta/keil/stm32f746/bsp_src8.png)

![](./meta/keil/stm32f746/bsp_src8_1.png)


### 添加头文件路径包含以及宏定义

- 参考STM32的原始工程，将其中的头文件搜索路径以及宏定义拷贝到LiteOS工程的设置中，并且定义宏LOS_STM32F746ZG

![](./meta/keil/stm32f746/bsp_src9.png)

![](./meta/keil/stm32f746/bsp_src10.png)

USE_HAL_DRIVER,STM32F746xx,USE_STM32F7XX_NUCLEO_144,ADAFRUIT_TFT_JOY_SD_ID802,LOS_STM32F746ZG

### 编译运行

- 以上步骤完成之后，直接编译调试运行就可以看到效果了。串口我们使用uart3 PD8和PD9作为TX和RX脚。串口会每隔500毫秒打印一次。

## 7 其他说明

###如何使用LiteOS 开发###

LiteOS中提供的功能包括如下内容： 任务创建与删除、任务同步（信号量、互斥锁）、动态中断注册机制等等内容，详细内容请参考《HuaweiLiteOSKernelDevGuide》。

###从零开始创建LiteOS工程###

目前在LiteOS的源代码的projects目录下已附带一些开发板的内核示例工程，用户可以直接使用，如果您所使用的开发板（芯片型号）与在示例工程中找不到，您可以从零开始创建LiteOS工程，创建流程请参考《LiteOS_Migration_Guide_Keil》。

###关于中断向量位置选择###

如果您需要使用LiteOS的中断注册机制，详细内容请参考《LiteOS_Migration_Guide_Keil》。

### kernel API测试代码 ###

如果您需要测试LiteOS内核工程运行情况，详细内容请参考《LiteOS_Migration_Guide_Keil》。











