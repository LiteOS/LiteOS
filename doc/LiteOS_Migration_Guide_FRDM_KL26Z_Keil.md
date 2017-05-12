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

- 未提供保证。本授权条款未必能完全提供您预期用途所需要的所有许可。例如：形象
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
	<td>2017年5月10日</td>
	<td>1.0</td>
	<td>完成初稿</td>
	</tr>
</table>

## 3概述

目前在github上已开源的Huawei LiteOS内核源码已适配好STM32F412、STM32F429、STM32L476、GD32F450、GD32F190、LPC824、LPC54114、KL25Z芯片，本手册将以KL26Z芯片为例，介绍基于Cortex-M0核芯片的移植过程。

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
	<td>RFDM-KL26Z开发板</td>
	<td>RFDM-KL26Z开发板(芯片型号KL26Z128VLKZ)</td>
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
	<td>Window 7 操作系</td>
	<td>安装Keil的操作系统</td>
	</tr>
	<tr>
	<td>Keil(5.18以上版本)</td>
	<td>用于编译、链接、调试程序代码
	uVision V5.18.0.0 </td>
	</tr>
    <td>PEDrivers_install</td>
	<td>板载OpenSDA驱动程序，用户加载及调试程序代码</td>
	</tr>
    <td>Pemicro_OpenSDA_Debug_MSD_Update_Apps_2017_01_31</td>
	<td>OpenSDA的应用升级固件</td>
	</tr>
</table>

**说明**

Keil工具需要开发者自行购买，OpenSDA驱动程序和固件可以从http://www.pemicro.com/opensda/获取。

## 5获取Huawei LiteOS 源码

首先我们从github上下载Huawei LiteOS内核源代码，步骤如下：

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
	<td>GD190开发板systick及驱动相关代码</td>
</tr>
<tr>
	<td></td>
	<td>GD32F450i-EVAL</td>
	<td>GD450开发板systick及驱动相关代码</td>
</tr>
<tr>
	<td></td>
	<td>STM32F412ZG-NUCLEO</td>
	<td>STM32F412开发板systick及驱动相关代码</td>
</tr>
<tr>
	<td></td>
	<td>STM32F429I_DISCO</td>
	<td>STM32F429开发板systick及驱动相关代码</td>
</tr>
<tr>
	<td></td>
	<td>STM32L476RG_NUCLEO</td>
	<td>STM32L476开发板systick及驱动相关代码</td>
</tr>
<tr>
	<td></td>
	<td>LPC824_LITE</td>
	<td>LPC824Lite开发板systick及驱动相关代码</td>
</tr>
<tr>
	<td></td>
	<td>LPC54110_BOARD</td>
	<td>LPC54110开发板systick及驱动相关代码</td>
</tr>
<tr>
	<td></td>
	<td>FRDM_KL25Z</td>
	<td>FRDM_KL25Z开发板systick及驱动相关代码</td>
</tr>
<tr>
	<td></td>
	<td>FRDM_KL26Z</td>
	<td>FRDM_KL26Z开发板systick及驱动相关代码</td>
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
	<td>STM32F429I_DISCO_IAR</td>
	<td>stm32f429开发板的iar工程目录</td>
</tr>
<tr>
	<td></td>
	<td>STM32F429I_DISCO_KEIL</td>
	<td>stm32f429开发板的keil工程目录</td>
</tr>
<tr>
	<td></td>
	<td>STM32L476R-NUCLEO-KEIL</td>
	<td>stm32l476开发板的keil工程目录</td>
</tr>
<tr>
	<td></td>
	<td>GD32F190R-EVAL-KEIL</td>
	<td>gd32f190开发板的keil工程目录</td>
</tr>
<tr>
	<td></td>
	<td>GD32F450i-EVAL-KEIL</td>
	<td>gd32f450开发板的keil工程目录</td>
</tr>
<tr>
	<td></td>
	<td>LPC824_LITE_KEIL</td>
	<td>lpc824Lite开发板的keil工程目录</td>
</tr>
<tr>
	<td></td>
	<td>LPC54110_BOARD_KEIL</td>
	<td>lpc54110开发板的keil工程目录</td>
</tr>
<tr>
	<td></td>
	<td>FRDM-KL25Z-KEIL</td>
	<td>FRDM-KL25Z开发板的keil工程目录</td>
</tr>
<tr>
	<td></td>
	<td>FRDM-KL26Z-KEIL</td>
	<td>FRDM-KL26Z开发板的keil工程目录</td>
</tr>
<tr>
	<td>user</td>
	<td></td>
	<td>此目录存放用户代码，LiteOS启动代码在mian()函数中</td>
</tr>
</table>

获取Huawei LiteOS源代码之后，我们可以将自己本地工程的驱动代码适配到LiteOS内核工程中进行应用开发。

## 6如何适配LiteOS内核工程开发
本章节描述的内容以开发板SDK中的uart_basic示例工程为基础，适配到LiteOS的FRDM-KL26Z-KEIL工程中，演示串口输出、按键及LED灯点亮功能。

### FRDM-KL26Z开发资料获取

- 从开发板官网获取开发包资料，网址为：http://www.nxp.com/cn/products/software-and-tools/hardware-development-tools/freedom-development-boards:FREDEVPLA

- 从keil官网下载PACK包，网址为：http://www.keil.com/dd2/Pack/

- 从官网下载OpenSDA驱动程序和固件升级包，网址为：http://www.pemicro.com/opensda/

### 安装KL系列单片机的的keil支持包

  安装Keil.Kinetis_KLxx_DFP.1.13.0.pack或者更高版本的pack文件到keil安装目录。

### 安装OpenSDA驱动并升级OpenSDA固件

  安装OpenSDA驱动程序PEDrivers_install.exe。

  将下载好的固件Pemicro_OpenSDA_Debug_MSD_Update_Apps_2017_01_31.zip解压。

  1、把USB先插入SDA口，按住SDA口旁边的按键不放，将USB线插入电脑的USB口，此时进入Bootloader模式，黄灯闪烁，电脑上显示BOOOTLOADER的磁盘。
  
  2、把DEBUG-APP_Pemicro_v108.SDA文件复制到磁盘中，然后断开USB链接，并重新插上USB线，黄灯常亮表示升级完成。
  
  3、按照上述步骤升级MSD-DEBUG-FRDM-KL26Z_Pemicro_v118.SDA和OpenSDA_Bootloader_Update_App_v111_2013_12_11文件夹下的BOOTUPDATEAPP_Pemicro_v111.SDA。至此，OpenSDA可以正常使用。

  OpenSDA也可以通过升级固件方式变成Jlink仿真器，可以在SEGGER官网下载固件，按照上述步骤升级固件。

**添加代码到LiteOS工程**

  按照LiteOS_Migration_Guide_Keil.md的步骤建立工程，并添加文件。

**添加代码到LiteOS工程**

拷贝keil安装目录和开发板资料中的文件到platform\FRDM-KL26Z文件夹中。

![](./meta/keil/kl26z/add_file_0.png)

拷贝完成后，platform\FRDM-KL26Z文件夹内容如下

![](./meta/keil/kl26z/add_file_1.png)

在LiteOS工程的startup加入以下文件

![](./meta/keil/kl26z/add_file_2.png)

**添加头文件搜索路径**

![](./meta/keil/kl26z/add_file_3.png)

**添加编译宏选项**

![](./meta/keil/kl26z/add_file_4.png)

**启动文件**

LiteOS可直接使用官方的启动文件startup_MKL26Z4.s，这样就为芯片所支持的中断注册好了服务函数，不需再使用LiteOS中断注册接口进行动态注册。

**调试器配置**

![](./meta/keil/kl26z/add_file_5.png)

**修改相关代码**

修改los_bsp_adapter.c文件中sys_clk_freq变量的值，使之与SystemCoreClockUpdate()函数实际配置的时钟频率保持一致。

![](./meta/keil/kl26z/add_file_6.png)


![](./meta/keil/kl26z/add_file_9.png)

在los_bsp_key.c、los_bsp_led.c、los_bsp_uart.c中添加相关的驱动代码。

经过以上步骤的修改，完成了代码的初步移植，可以编译代码,连接串口线，并在串口调试工具中打开相应串口，调试运行时可看到串口有打印输出，LED灯闪烁，另外用户可调用LOS_EvbGetKeyVal()函数增加按键测试代码。

## 7 其他说明

###如何使用LiteOS 开发###

LiteOS中提供的功能包括如下内容： 任务创建与删除、任务同步（信号量、互斥锁）、动态中断注册机制等等内容，详细内容请参考《HuaweiLiteOSKernelDevGuide》。

###从零开始创建LiteOS工程###

目前在LiteOS的源代码的projects目录下已附带一些开发板的内核示例工程，用户可以直接使用，如果您所使用的开发板（芯片型号）与在示例工程中找不到，您可以从零开始创建LiteOS工程，创建流程请参考《LiteOS_Migration_Guide_Keil》。

###关于中断向量位置选择###

如果您需要使用LiteOS的中断注册机制，详细内容请参考《LiteOS_Migration_Guide_Keil》。

### kernel API测试代码 ###

如果您需要测试LiteOS内核工程运行情况，详细内容请参考《LiteOS_Migration_Guide_Keil》。








