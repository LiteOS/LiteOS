### 操作系统：Ubuntu-16.04.2 64bit

### 目标平台：ARM Cortex-M 核

## 1.下载和安装SoftConsole。

SoftConsole集成了下面的开源组件:

    Eclipse/CDT

    Roa Logic RISC-V Eclipse Plug-in

    GNU ARM Eclipse Plug-ins

    GCC RISC-V/ARM compiler

    OpenOCD
    
[下载地址](https://www.microsemi.com/products/fpga-soc/design-resources/design-software/softconsole#downloads): https://www.microsemi.com/products/fpga-soc/design-resources/design-software/softconsole#downloads

选择linux版本。

在安装之前请更新系统：

Ubuntu/Debian 64 bit

a. dpkg --add-architecture i386

b. apt-get update

c. apt-get install libgtk2.0-0:i386

d. apt-get install libxtst6:i386

e. apt-get install lib32ncurses5


安装步骤：

a. chmod +x Microsemi-SoftConsole-v5.1-Linux-x86-Installer

b. ./Microsemi-SoftConsole-v5.1-Linux-x86-Installer

## 2.LitOS Kernel工程代码在SoftConsole中的编译和调试：
a.SotfConsole的主页面就是eclipse的，首先导入LiteOS工程:

选择File->New->Makefile Project with Existing Code:

![](http://otqay9rss.bkt.clouddn.com/image/jpg/New_project.PNG)

b.工程建立好之后，配置工程的C/C++ Build选项：

选择工程的Properties->C/C++ Build
![image](http://otqay9rss.bkt.clouddn.com/image/jpg/C_Build.PNG)

因为Makefile支持多处理器架构、多硬件平台的构建，所以在Build command中确定好编译参数。

以STM32F429I_DISCO为例：

```
make ARCH=arm PLAT=STM32F429I_DISCO CPU=cortex-m4 ARMV=armv7e-m V=2
```

c.配置调试选项：
选择Debug As->Debug Configurations->GDB OpenOCD Debugging

![image](http://otqay9rss.bkt.clouddn.com/image/jpg/debug1.PNG)

OpenOCD和GDB的配置在Debugger页面：

![image](http://otqay9rss.bkt.clouddn.com/image/jpg/debug2.PNG)

OpenOCD的Executable路径使用SoftConsole中openocd的路径：

    /home/baikal/Microsemi_SoftConsole_v5.1/openocd/bin/openocd

GDB port 和 Telnet port不用修改。

Config options要根据具体调试的硬件板子做适配，这里我们使用的是：

    -f /home/baikal/Microsemi_SoftConsole_v5.1/openocd/share/openocd/scripts/board/stm32f429discovery.cfg

GDB的Executable路径使用SoftConsole中gdb的路径：

    /home/baikal/Microsemi_SoftConsole_v5.1/arm-none-eabi-gcc/bin/arm-none-eabi-gdb

Commands中为：

    target remote localhost:3333

最后，配置Startup页面，使用默认的即可。

点击Debug，开始调试，调试页面如下：

![image](http://otqay9rss.bkt.clouddn.com/image/jpg/debug3.PNG)


调试支持断点，寄存器，内存，变量还有汇编窗口。

其中，串口和git也嵌入在工具中。
