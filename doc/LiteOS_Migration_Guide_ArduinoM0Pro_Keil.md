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
本文档介如何移植Huawei LiteOS到Arduino M0 Pro，并成功运行基础示例。
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
	<td>2017年04月18日</td>
	<td>1.0</td>
	<td>完成初稿</td>
	</tr>
</table>

## 3概述

目前在github上已开源的Huawei LiteOS内核源码已适配好STM32F412、STM32F429、STM32L476、GD32F450、GD32F190芯片，本手册将以ATSAMD21G18A为例，介绍基于Contex-M0芯片的移植过程

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
	<td>Arduino M0 Pro</td>
	<td>芯片采用的是ATSAMD21G18A，基于Cortex-M0内核的芯片开发板</td>
	</tr>
	<tr>
	<td>PC机</td>
	<td>用于编译、加载并调试镜像</td>
	</tr>
	<tr>
	<td>电源（5v）</td>
	<td>开发板供电，使用MicroUSB接口</td>
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
	<td>Keil(5.23)</td>
	<td>用于编译、链接、调试程序代码 uVision V5.23.0.0 MDK-Lite </td>
	</tr>
	<tr>
	<td>Atmel Studio</td>
	<td>用于修改ATSAMD21G18A的熔丝位</td>
	</tr>
</table>

**说明**

Keil工具需要开发者自行购买，或者在https://www.keil.com/demo/eval/arm.htm下载试用版。

## 5修改熔丝位

Arduino M0 Pro上的ATSAMD21G18A的熔丝位BOOTPROT默认为0x1，其含义如下：

![](./meta/keil/samd21g18a/BootLoaderSize.png)

如果不修改这个熔丝位，直接使用Atmel Studio或者Keil下载程序会出错，分别如下所示：

![](./meta/keil/samd21g18a/AtStudioError.png)

![](./meta/keil/samd21g18a/keilerro.png)

需要修改这个熔丝位BOOTPROT为0x7，才可以取消保护，进行程序下载，修改使用的工具是Atmel Studio自带的工具。首先将Arduino M0 Pro的Programming这个USB口连接计算机，在设备管理器中应该出现如下新设备。

![](./meta/keil/samd21g18a/virtualcom.png)

打开Atmel Studio，选择Tools菜单下的Device Programming，如下：

![](./meta/keil/samd21g18a/AtStudioMenu.png)



其中Tool选择EDGB，芯片型号选择ATSAM21G18A，Interface选择SWD，点击Apply，然后点击Read，可以读出当前的设置，也就是出厂默认设置，首先擦除芯片，如下：


![](./meta/keil/samd21g18a/erasechip.jpg)


接着在如下窗口修改BOOTPROT的值，默认是0x01，将其修改为0x07，然后点击Program即可。

![](./meta/keil/samd21g18a/ChipProgram.png)

## 6获取Huawei LiteOS 源码

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
	<td>IDE相关宏定义</td>
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
	<td>user</td>
	<td></td>
	<td>此目录存放用户代码，LiteOS启动代码在mian()函数中</td>
</tr>
</table>

获取Huawei LiteOS源代码之后，我们可以将自己本地已有工程的代码适配到LiteOS内核工程中进行应用开发。

## 7创建Huawei LiteOS 工程

### 创建工程
获取到LiteOS内核代码后，在project目录下新建文件夹LiteOS_Arduino_M0_Pro_Expand。重命名platform目录下的文件夹LOS_EXPAND_xxx为LOS_EXPAND_Arduino_M0_Pro。
打开keil，新建工程，保存在project\LiteOS_Arduino_M0_Pro_Expand目录下，工程名为LiteOS，器件选择如下：

![](./meta/keil/samd21g18a/ProjectWizard1.png)

下一个界面，选择如下：

![](./meta/keil/samd21g18a/ProjectWizard2.png)

点击OK即可，至此，我们的工程已经创建完成。

### 添加kernel代码到工程
创建如下目录层级：

![](./meta/keil/samd21g18a/ProjectManage.png)

创建完成目录树之后我们添加源代码到目录树中，最终添加完成的内容如下：

- 将kernel/base目录下的所有C文件添加到工程中的kernel下
- 将kernel/cmsis目录下的所有C文件添加到工程中的cmsis下。
- 将platform\LOS_EXPAND_Arduino_M0_Pro目录下的所有C文件添加到工程中的platform/expand_Arduino_M0_Pro下
- 根据芯片内核型号，将kernel\cpu\arm\cortex-m0目录下的所有C文件以及汇编代码添加到工程中的cpu/m4（cpu/m3）下，除了los_vector.s
- 将kernel\config目录下的所有C文件添加到工程中的config下
- 将user目录下的所有C文件添加到工程中的user下
- 添加example/api目录下的所有C文件到工程的example目录下

![](./meta/keil/samd21g18a/filelist.png)

### 配置工程属性
打开配置文件，如下，在Target界面勾选上“Use MicroLIB”：

![](./meta/keil/samd21g18a/target.png)

在C/C++界面，其中的Define输入框中添加宏定义“RAM_SIZE_LEVEL_1”：

![](./meta/keil/samd21g18a/C++.png)

点击“Include Paths”后面的省略号，添加如下头文件路径：

![](./meta/keil/samd21g18a/path.png)

在Debugger界面，设置如下，大部分都是默认，只有右上角的Use，选择CMSIS-DAP Debugger。

![](./meta/keil/samd21g18a/debu.png)

## 8 适配驱动代码

### 修改时钟参数

修改los_bsp_adapter.c中如下：

`const unsigned int sys_clk_freq = 16000000;`

改为如下：

`const unsigned int sys_clk_freq = 48000000;`

修改system_samd21.c，如下：

`#define __SYSTEM_CLOCK    (1000000)`

改为

`#define __SYSTEM_CLOCK    (48000000)`

### 修改SystemInit

SystemInit函数位于system_samd21.c中，修改如下，其中设置了芯片时钟为48MHz：

```
void SystemInit(void)
{
	// Keep the default device state after reset
	SystemCoreClock = __SYSTEM_CLOCK;
	REG_SYSCTRL_DFLLCTRL = 0x02;	
	SYSCTRL->DFLLVAL.bit.COARSE = *(uint32_t*)(NVMCTRL_OTP4 + 4) >> 26;	
	SYSCTRL->DFLLVAL.bit.FINE = *(uint32_t*)(NVMCTRL_OTP4 + 8) & 0x3ff;	
	while(!(REG_SYSCTRL_PCLKSR&(0x1<<4)) );	
	NVMCTRL->CTRLB.bit.RWS = NVMCTRL_CTRLB_RWS_HALF_Val;	
	REG_GCLK_GENCTRL = (GCLK_SOURCE_DFLL48M<<8) | (0x1<<16);  
}
```

### 修改los_bsp_uart.c

其中UART初始化函数如下，其中使用的是SERCOM5，从Arduino M0 Pro的原理图分析，通过EDGB仿真出来的串口对应的SERCOM5，其中BaudRate定义了波特率，默认是9600：

![](./meta/keil/samd21g18a/usrtinit.png)

UART读写函数如下：

![](./meta/keil/samd21g18a/uartreadwrite.png)

![](./meta/keil/samd21g18a/uartprintf.png)

随后还有一些重定向函数，定义了之后可以使用printf直接从串口输出数据。

![](./meta/keil/samd21g18a/fputc.png)

### 修改los_bsp_led.c

Arduino M0 Pro上的L灯，对应的是数字口13，对应ATSAMD21G18A的PORTA[17]，所以此处设置LED的初始化函数如下，其中设置PORTA[17]为输出口：

![](./meta/keil/samd21g18a/ledinit.png)

因为只控制这一个LED，所以修改其控制函数如下，PORTA[17]为高电平的时候，L灯亮，反之，L灯灭：

![](./meta/keil/samd21g18a/ledcontrol.png)

### 修改los_bsp_key.c

Arduino M0 Pro上除了复位按键，没有其他的按键，需要外接一个按键，才可以测试按键中断，此处选择数字口10，对应PORTA[18]，可以复用为EIC[2]，其初始化函数如下：

![](./meta/keil/samd21g18a/keyinit.png)

修改startup_SAMD21.s，不再使用默认的EIC_Handler，将其注释掉，然后添加如下代码处理外部中断：

![](./meta/keil/samd21g18a/eichandler.png)

上述代码调用了函数EXTInt_IRQHandler进行具体的中断处理，函数EXTInt_IRQHandler在los_bsp_key.c中定义，如下：

![](./meta/keil/samd21g18a/irqhandler.png)

其中清除按键中断，然后输出一段信息。

## 修改LiteOS部分代码

### 修改los_api_msgqueue.c

在测试的时候发现Example_MsgQueue这个测试始终通过不了，只能发送并接收第一个message，发送第二个message的时候，就出现HardFault，原因是调用函数LOS_QueueCreate时的参数usMaxMsgSize如果不是4的整数倍，将会造成向队列第二次写入时，有不对齐的内存写入操作，造成hardfault错误，需要修改如下代码

`/*创建队列*/
uwRet = LOS_QueueCreate("queue", 5, &g_uwQueue, 0, 50);`

改为如下：

`/*创建队列*/
uwRet = LOS_QueueCreate("queue", 5, &g_uwQueue, 0, 48);`

### main函数修改

修改main函数如下，其中调用巡检函数LOS_Inspect_Entry：

```
/*****************************************************************************
 Function    : main
 Description : Main function entry
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
int main(void)
{
    UINT32 uwRet;
    uwRet = LOS_KernelInit();
    if (uwRet != LOS_OK) {
        return LOS_NOK;
    }
    LOS_EnableTick();
    LOS_EvbSetup(); 
    LOS_Inspect_Entry();
    LOS_Start();
    for (;;);
}
```


### 验证移植后的工程

连接Arduino M0 Pro的Programmer接口到计算机，在计算机上会出现一个EDBG虚拟串口的设备，keil编译、下载，打开串口调试工具，选择出现的EDBG虚拟串口，速率选择为9600，然后按下Arduino M0 Pro上的复位按钮，会显示如下结果，表示巡检程序运行正常：

![](./meta/keil/samd21g18a/result.png)

此时找到一根跳线，将数字口10与GND口相连，再断开，用来模拟按键按下，此时会引发按键中断，输出相关的信息，如下：

![](./meta/keil/samd21g18a/keypress.png)


Atmel Stduio下载地址：

http://www.atmel.com/zh/cn/tools/atmelstudio.aspx?tab=overview

ATSAMD21G18A的手册下载地址：

http://www.atmel.com/zh/cn/devices/ATSAMD21G18.aspx?tab=documents


Arduino M0 Pro电路图下载地址：

http://www.arduino.org/products/boards/arduino-m0-pro
