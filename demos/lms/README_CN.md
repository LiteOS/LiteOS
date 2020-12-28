# LMS使用教程

## 基本概念

LMS全称为Lite Memory Sanitizer，是一种实时检测内存操作合法性的算法。LMS能够实时检测缓冲区溢出（buffer overflow），释放后使用（use after free），多重释放（double free）和释放野指针（wild pointer），在异常发生的第一时间通知操作系统，结合操作系统Backtrace等定位手段，能准确定位到问题根因，大大提升内存问题定位效率。

## 使用场景

- 疑似系统内存问题需要定位时。
- 开发过程中需要验证是否存在内存问题。

## 示例程序

示例程序包含堆溢出代码、编译选项、LMS配置项、程序调用方式。

### 示例代码程序

示例代码程序中，申请长度4字节的堆内存，但读取buf[4]，会超出申请的内存长度，导致堆读取溢出内存问题。LMS可以检测出本示例程序中的堆溢出内存问题。

示例代码为<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/lms/lms_demo.c" target="_blank">lms_demo.c</a>，代码片段如下：


```c
    CHAR *buf = NULL;
    CHAR tmp;
    UINT32 ret;

    buf = LOS_MemAlloc(m_aucSysMem0, 4); /* mem size 4 */

    tmp = buf[3]; /* Array 3. No exception information is printed on the LMS. */
    tmp = buf[4]; /* Array 4. When the fourth byte overflows, the LMS prints the read error information. */

    printf("buf is %c.\n", tmp);
    ret = LOS_MemFree(m_aucSysMem0, buf);

```

### 编译选项

要检测内存问题，<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/lms/Makefile" target="_blank">Makefile</a>需要增加LMS的编译选项，如下：

```
C_DEFS = -fsanitize=kernel-address
```


### LMS配置项

LMS只适配了Bestfit内存算法，且不支持SLAB算法，所以需要在配置项中选择Bestfit内存算法，且不开启MEM SLAB：

```
Kernel --> Memory Management --> Dynamic Memory Management Algorithm --> BESTFIT
Kernel --> Memory Management --> Enable Mem SLAB Extension
```

为避免编译器优化，需要配置编译器选项为LOSCFG_COMPILER_OPTIMIZE_NONE：

```
Compiler --> Optimize Option --> Optimize None
```

为输出Backtrace信息，需要开启配置项LOSCFG_BACKTRACE：

```
Debug --> Enable Backtrace
```

使能LMS特性，开启LMS配置项LOSCFG_KERNEL_LMS：

```
Debug --> Enable Lite Memory Sanitizer
```

使能LMS Demo示例程序，开启LMS配置项LOSCFG_DEMOS_LMS：

```
Demos --> LMS Demo --> Enable LMS Demo
```

### Demo程序调用

示例程序是在各个开发板的user_task.c文件中被调用的，比如STM32F769IDISCOVERY开发板的<a href="https://gitee.com/LiteOS/LiteOS/blob/master/targets/STM32F769IDISCOVERY/Src/user_task.c" target="_blank">user_task.c</a>。

调用代码片段如下:
```c
#ifdef LOSCFG_DEMOS_LMS
#include "lms_demo.h"
#endif

......

#ifdef LOSCFG_DEMOS_LMS
    Example_LMSEntry();
#endif

```


## 示例程序运行

编译烧录，打开串口控制台窗口，即可看到输出的内存堆读取溢出检测信息。

串口控制台会有类似如下的输出：

```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.0.0-rc1
build data : Dec  1 2020 16:50:03

**********************************
osAppInit
cpu 0 entering scheduler

Kernel task demo begin.
LOS_TaskLock() ok.
Create Example_TaskLMS ok.
Enter TaskLms Handler.

Huawei LiteOS # [ERR]*****  Kernel Address Sanitizer Error Detected  *****
[ERR]sd value: 0x3
[ERR]Heap buffer overflow error detected!

[ERR]Illegal READ address at: [0x2000c354]
[ERR]Shadow memory address: [0x20078a31]  Shadow memory value: [11]
[ERR]Dump info around address [0x2000c354]:
    [0x2000c340]:   bf    3c    ff    df    04    00    00    00
    [0x2000c348]:   b0    43    00    20    14    00    00    80
    [0x2000c350]:   cc    cc    cc    cc   [80]   34    00    20
    [0x2000c358]:   80    34    00    20    40    c3    00    20
    [0x2000c360]:   00    02    00    00    ca    ca    ca    ca
[ERR]Accessable heap addr     00
[ERR]Heap red zone            11
[ERR]Heap freed buffer        01

----- LMS BackTrace Start -----
runTask->taskName = LMS_NAME
runTask->taskId = 4
*******backtrace begin*******
traceback 1 -- lr = 0x0800803c -- fp = 0x080015c0
traceback 2 -- lr = 0x080038ec -- fp = 0x08007e8c
buf is €.
TaskLms LOS_TaskDelay Done.
```
