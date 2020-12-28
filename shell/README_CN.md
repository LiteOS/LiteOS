# Shell使用教程


## 基本概念
Shell俗称壳（用来区别于核），是“给使用者提供使用界面”的软件（命令解析器），同时又是一种程序设计语言。作为命令解析器，它类似于DOS下的command和后来的cmd.exe，Shell接收用户命令，然后调用相应的应用程序。作为程序设计语言，它定义了各种变量和参数，并提供了许多在高级语言中才具有的控制结构，包括循环和分支。
作为命令语言，它交互式解释和执行用户输入的命令或者自动地解释和执行预先设定好的一串命令。
- Shell可以管理用户与操作系统之间的交互：等待用户输入，向操作系统解释用户输入，并且处理各种各样的操作系统的输出结果。
- 以Shell script（非交互）方式自动地解释和执行预先设定好的一串命令。Shell script是写在文件中的一串Shell和操作系统命令，它们可以被重复使用。


## 使用场景
Huawei LiteOS提供的Shell作为在线调试工具，可以通过串口工具输入输出，支持常用的基本调试功能。同时用户可以新增定制的命令，新增命令需重新编译烧录后才能执行。


## 功能介绍
1. Huawei LiteOS提供的Shell命令参见后面[内置命令](#内置命令)章节。
2. Huawei LiteOS的Shell模块为用户提供下面几个接口以新增命令。
   | 接口名          | 描述                     |
   | --------------- | ------------------------ |
   | SHELLCMD_ENTRY  | 静态注册命令             |
   | osCmdReg        | 动态注册命令             |

    >![](public_sys-resources/icon-note.gif) **说明：** 
    >静态注册命令方式一般用于注册系统常用命令，动态注册命令方式一般用于注册用户命令。
    >静态注册命令有5个入参，动态注册命令有4个入参。下面除去第一个入参是静态注册独有的，剩余的四个入参两个注册命令是一致的。
    >-   第一个入参：命令变量名，用于设置链接选项（build/mk/liteos\_tables\_ldflags.mk的LITEOS\_TABLES\_LDFLAGS变量）。例如变量名为ls\_shellcmd，链接选项就应该设置为：LITEOS\_TABLES\_LDFLAGS += -uls\_shellcmd。这个入参是静态注册独有的，动态注册中没有这个入参。
    >-   第二个入参：命令类型，目前支持两种命令类型。
    >      -   CMD\_TYPE\_EX：不支持标准命令参数输入，会把用户填写的命令关键字屏蔽掉。例如：输入ls /ramfs，传入给命令处理函数的参数只有/ramfs，对应于命令处理函数中的argv\[0\]，而ls命令关键字并不会被传入。
    >      -   CMD\_TYPE\_STD：支持的标准命令参数输入，所有输入的字符都会通过命令解析后被传入。例如：输入ls /ramfs，ls和/ramfs都会被传入命令处理函数，分别对应于命令处理函数中的argv\[0\]和argv\[1\]。
    >-   第三个入参：命令关键字，是命令处理函数在Shell中对应的名称。**命令关键字必须唯一**，即两个不同的命令项不能有相同的命令关键字，否则只会执行其中一个。Shell在执行用户命令时，如果存在多个命令关键字相同的命令，只会执行在“help”命令中排在最前面的那个。
    >-   第四个入参：命令处理函数的入参最大个数。
    >      -   静态注册命令暂不支持设置。
    >      -   动态注册命令支持设置不超过32的入参最大个数，或者设置为XARGS（其在代码中被定义为0xffffffff）表示不限制参数个数。
    >-   第五个入参：命令处理函数名，即在Shell中执行命令时被调用的函数。


## 使能Shell
Shell功能默认关闭，可以通过`make menuconfig`打开Shell组件的开关。
```
Debug --> Enable a Debug Version --> Enable Shell --> Functionality of Shell --> Select shell functionality -->  Enable full functionality of shell
```


## 新增命令开发流程

下面介绍新增Shell命令的典型开发流程。

1. **定义Shell命令处理函数**

    Shell命令处理函数用于处理注册的命令。例如定义一个命令处理函数osShellCmdTest，可以打印新增命令的入参：

    ```c
    #include "stdio.h"
    UINT32 osShellCmdTest(UINT32 argc, CHAR **argv)
    {
        printf("this is a shell test\n");
        if (argc == 1) {
            printf("test cmd arg 1 is %s\n", argv[0]);
        } else if (argc == 2) {
            printf("test cmd arg 2 is %s %s\n", argv[0], argv[1]);
        } else if (argc > 2) {
            printf("input error\n");
        }
        return 0;
    }
    ```

    在头文件中声明命令处理函数：
    ```c
    UINT32 osShellCmdTest(UINT32 argc, CHAR **argv);
    ```

    >![](public_sys-resources/icon-notice.gif) **须知：** 
    >命令处理函数的参数与C语言中main函数的参数类似，包括两个入参：
    >-   argc：Shell命令的参数个数。个数中是否包括命令关键字，和注册命令时的命令类型有关。
    >-   argv：为指针数组，每个元素指向一个字符串，该字符串就是执行shell命令时传入命令处理函数的参数。参数中是否包括命令关键字，和注册命令时的命令类型有关。

2. **注册命令**

   有两种命令注册方式。下面以注册一个名为“test”的命令为例，介绍命令注册的方法。“test”命令的处理函数就是上面例子中的osShellCmdTest()函数：

   -  **静态注册命令**
      ```c
      #include "shcmd.h"
      SHELLCMD_ENTRY(testShellcmd, CMD_TYPE_EX, "test", XARGS, (CMD_CBK_FUNC)osShellCmdTest)
      ```
   -  **动态注册命令**
      ```c
      #include "shell.h"
      osCmdReg(CMD_TYPE_EX, "test", XARGS, (CMD_CBK_FUNC)osShellCmdTest);
      ```

3. **使能Shell**

   通过make menuconfig使能Shell，详见[使能Shell](#使能Shell)。

4. **编译烧录**


## 执行Shell命令

通过串口终端工具（比如xShell），连接串口。

选择SERIAL协议，然后配置SERIAL：选择对应的串口号，设置波特率为115200。

连接后重启开发板，系统启动后可以在串口工具中看到“Huawei LiteOS #”的提示符。
```
Huawei LiteOS #
```

这时可以输入命令，按`回车键`执行。

按`Tab键`可自动联想补全，若有多个匹配项则补全共同字符，并打印多个匹配项。

按`退格键`可删除一个字符，如果在UTF8格式下输入了中文字符，只能通过回退三次来删除。默认模式为英文输入。

按`方向键上下`可选择历史命令。


## 内置命令

### help

#### 命令功能
显示当前操作系统内所有Shell命令。

#### 命令格式
```
help
```

#### 使用示例
```
Huawei LiteOS # help
*******************shell commands:*************************

cpup          date          dlock         dmesg         free          help          hwi
log           memcheck      mutex         queue         sem           stack         swtmr
systeminfo    task          uname         watch
```

### date
#### 命令功能
查询及设置系统时间。

#### 命令格式
```
date

date --help

date +Format

date -s YY/MM/DD

date -s hh:mm:ss

date -r Filename(暂不支持)
```

#### 参数说明
参数|参数说明|取值范围|
|:---:|:---|:---:|
|--help|使用帮助|N/A|
|+Format|根据Format格式打印时间|--help中列出的占位符|
|-s YY/MM/DD|设置系统时间，用“/”分割的年月日|>= 1970/01/01|
|-s hh\:mm\:ss|设置系统时间，用“:”分割的时分秒|N/A|
|-r|查询文件修改时间（暂不支持）|N/A|

#### 使用指南
-  date参数缺省时，默认显示当前系统时间。
-  --help、+Format、-s、-r不能混合使用。

#### 使用示例
```
Huawei LiteOS # date
Fri Jan  2 07:05:54 1970


Huawei LiteOS # date --help

Usage: date [+FORMAT]
   or: date [-s] [YY/MM/DD] [hh:mm:ss]
Display the current time in the given FORMAT, or set the system date.
FORMAT controls the output.  Interpreted sequences are:
  %b     The abbreviated month name according to the current locale.
  %B     The full month name according to the current locale.
  %C     The century number (year/100) as a 2-digit integer. (SU)
  %d     The day of the month as a decimal number (range 01 to 31).
  %e     Like %d, the day of the month as a decimal number, 
         but a leading zero is replaced by a space.
  %h     Equivalent to %b.  (SU)
  %H     The hour as a decimal number using a 24-hour clock (range 00 to 23).
  %I     The hour as a decimal number using a 12-hour clock (range 01 to 12).
  %j     The day of the year as a decimal number (range 001 to 366).
  %k     The hour (24-hour clock) as a decimal number (range  0  to  23); 
         single digits are preceded by a blank.  (See also %H.)  (TZ)
  %l     The hour (12-hour  clock) as a decimal number (range 1 to 12); 
         single digits are preceded by a blank.  (See also %I.)  (TZ)
  %m     The month as a decimal number (range 01 to 12).
  %M     The minute as a decimal number (range 00 to 59).
  %n     A newline character. (SU)
  %p     Either "AM" or "PM" according to the given time value, 
         or the corresponding strings for the current locale.
         Noon is treated as "PM" and midnight as "AM".
  %P     Like %p but in lowercase: "am" or "pm" 
         or a corresponding string for the current locale. (GNU)
  %s     The number of seconds since the Epoch, that is,
         since 1970-01-01 00:00:00 UTC. (TZ)
  %S     The second as a decimal number (range 00 to 60).
         (The range is up to 60 to allow for occasional leap seconds.)
  %t     A tab character. (SU)
  %y     The year as a decimal number without a century (range 00 to 99).
  %Y     The year as a decimal number including the century.
  %%     A literal '%' character.

Examples:
Set system date (2017-01-01)
$ date -s 2017/01/01
Set system time (12:00:00)
$ date -s 12:00:00
Show the time with format Year-Month-Day
$ date +%Y-%m-%d
```

### uname
#### 命令功能
显示操作系统的名称，系统编译时间，版本信息等。

#### 命令格式
```
uname [-a | -s | -t | -v | --help]
```

#### 参数说明
参数|参数说明
|:---:|:---|
|-a|显示全部信息|
|-s|显示操作系统名称|
|-t|显示系统的编译时间|
|-v|显示版本信息|
|--help|显示uname的帮助信息|

#### 使用指南
-  参数缺省时，默认显示操作系统名称。
-  `uname`的参数不能混合使用。

#### 使用示例
```
Huawei LiteOS # uname --help
-a,            print all information
-s,            print the kernel name
-t,            print the build date
-v,            print the kernel version

Huawei LiteOS # uname -a
Huawei LiteOS - Huawei LiteOS 5.0.0-rc1 Nov  2 2020 00:50:54
```

### task
#### 命令功能
查询系统的任务信息。

#### 命令格式
```
task [ID]
```

#### 参数说明
|参数|参数说明|取值范围|
|:---:|:---|:---:|
|ID|任务ID号，输入形式以十进制表示或十六进制表示皆可|[0, 0xFFFFFFFF]|

#### 使用指南
-  参数缺省时，默认打印全部运行任务信息。
-  task后加ID，当ID参数在\[0, 64\]范围内时，返回指定ID号任务的任务名、任务ID、任务的调用栈信息（最大支持15层调用栈），其他取值时返回参数错误的提示。如果指定ID号对应的任务未创建，则提示。
-  如果在task命令中，发现任务是Invalid状态，请确保pthread\_create创建函数时有进行如下操作之一，否则资源无法正常回收。
    -  选择的是阻塞模式应该调用pthread\_join\(\)函数。
    -  选择的是非阻塞模式应该调用pthread\_detach\(\)函数。
    -  如果不想调用前面两个接口，就需要设置pthread\_attr\_t状态为PTHREAD\_STATE\_DETACHED，将attr参数传入pthread\_create，此设置和调用pthread\_detach函数一样，都是非阻塞模式。

#### 使用示例
```
Huawei LiteOS # task
Name          TaskEntryAddr    TID   Priority  Status     StackSize  WaterLine  StackPoint  TopOfStack EventMask SemID  CPUUSE  CPUUSE10s  CPUUSE1s MEMUSE
----          ------------     ---   -------   --------   ---------  --------   ----------  ---------- -------  -----  -------  ---------  ------  -------
Swt_Task          0x40002770   0x0    0        QueuePend  0x6000     0x2cc     0x4015a318  0x401544e8  0x0      0xffff    0.0       0.0     0.0     0
IdleCore000       0x40002dc8   0x1    31       Ready      0x400      0x15c     0x4015a7f4  0x4015a550  0x0      0xffff   98.6      98.2    99.9     0
system_wq         0x400b80fc   0x3    1        Pend       0x6000     0x244     0x40166928  0x40160ab8  0x1      0xffff    0.0       0.0     0.0     0
SerialShellTask   0x40090158   0x5    9        Running    0x3000     0x55c     0x40174918  0x40171e70  0xfff    0xffff    1.2       1.7     0.0     48
SerialEntryTask   0x4008fe30   0x6    9        Pend       0x1000      0x2c4    0x40175c78  0x40174e88  0x1      0xffff    0.0       0.0     0.0     72

Huawei LiteOS # task 1
TaskName = Swt_Task
TaskId = 0x1
```

>![](public_sys-resources/icon-note.gif) **说明：** 
>1.  输出项说明：
>     -   Name：任务名。
>     -   TID：任务ID。
>     -   Priority：任务的优先级。
>     -   Status：任务当前的状态。
>     -   StackSize：任务栈大小。
>     -   WaterLine：该任务栈已经被使用的内存大小。
>     -   StackPoint：任务栈指针，表示栈的起始地址。
>     -   TopOfStack：栈顶的地址。
>     -   EventMask：当前任务的事件掩码，没有使用事件，则默认任务事件掩码为0（如果任务中使用多个事件，则显示的是最近使用的事件掩码）。
>     -   SemID：当前任务拥有的信号量ID，没有使用信号量，则默认0xFFFF（如果任务中使用了多个信号量，则显示的是最近使用的信号量ID）。
>     -   CPUUSE：系统启动以来的CPU占用率。
>     -   CPUUSE10s：系统最近10秒的CPU占用率。
>     -   CPUUSE1s：系统最近1秒的CPU占用率。
>     -   MEMUSE：截止到当前时间，任务所申请的内存大小，以字节为单位显示。MEMUSE仅针对系统内存池进行统计，不包括中断中处理的内存和任务启动之前的内存。
>        任务申请内存，MEMUSE会增加，任务释放内存，MEMUSE会减小，所以MEMUSE会有正值和负值的情况。
>        1）MEMUSE为0：说明该任务没有申请内存，或者申请的内存和释放的内存相同。
>        2）MEMUSE为正值：说明该任务中有内存未释放。
>        3）MEMUSE为负值：说明该任务释放的内存大于申请的内存。
>2.  系统任务说明（Huawei LiteOS系统初始任务有以下几种）：
>     -   Swt\_Task：软件定时器任务，用于处理软件定时器超时回调函数。
>     -   IdleCore000：系统空闲时执行的任务。
>     -   system\_wq：系统默认工作队列处理任务。
>     -   SerialEntryTask：从底层buf读取用户的输入，初步解析命令，例如tab补全，方向键等。
>     -   SerialShellTask：接收命令后进一步解析，并查找匹配的命令处理函数，进行调用。
>3.  任务状态说明：
>     -   Ready：任务处于就绪状态。
>     -   Pend：任务处于阻塞状态。
>     -   PendTime：阻塞的任务处于等待超时状态。
>     -   Suspend：任务处于挂起状态。
>     -   Running：该任务正在运行。
>     -   Delay：任务处于延时等待状态。
>     -   SuspendTime：挂起的任务处于等待超时状态。
>     -   Invalid：非上述任务状态。

### free
#### 命令功能
显示系统内存的使用情况，同时显示系统的text段、data段、rodata段、bss段大小。

#### 命令格式
```
free [-k | -m]
```

#### 参数说明
|参数|参数说明|取值范围|
|:---:|:---|:---:|
|无参数|以Byte为单位显示|N/A|
|-k|以KByte为单位显示|N/A|
|-m|以MByte为单位显示|N/A|

#### 使用指南
-  输入free显示内存使用情况，total表示系统动态内存池的总大小，used表示已使用的内存大小，free表示空闲的内存大小。text表示代码段大小，data表示数据段大小，rodata表示只读数据段大小，bss表示未初始化全局变量占用的内存大小。
-  free命令可以以三种单位来显示内存使用情况，包括Byte、KByte和MByte。

#### 使用示例
```
Huawei LiteOS # free

        total        used          free
Mem:    183748       72200         111548

        text         data          rodata        bss
Mem:    61728        472           14776         5572

Huawei LiteOS # free -k

        total        used          free
Mem:    179          70            108

        text         data          rodata        bss
Mem:    60           0             14            5
```

### memcheck
#### 命令功能
检查动态申请的内存块是否完整，是否存在内存越界造成节点损坏。

#### 命令格式
```
memcheck
```

#### 使用指南
-   该命令依赖于LOSCFG\_BASE\_MEM\_NODE\_INTEGRITY\_CHECK，使用时需要在配置项中开启"Enable integrity check or not"。
    ```
    Debug  ---> Enable a Debug Version---> Enable MEM Debug---> Enable integrity check or not
    ```
    只有`bestfit`内存算法支持该命令，`bestfit_little`不支持，请设置内存算法为`LOSCFG_KERNEL_MEM_BESTFIT`，即在配置项中设置内存算法为"bestfit"。
    ```
    Kernel --> Memory Management --> Dynamic Memory Management Algorithm --> bestfit
    ```
-   当内存池所有节点完整时，输出"memcheck over, all passed!"。
-   当内存池存在节点不完整时，输出被损坏节点的内存块信息。


#### 使用示例
- 没有内存越界时，执行memcheck输出内容如下：
  ```
  Huawei LiteOS # memcheck
  system memcheck over, all passed!
  ```
- 发生内存越界时，执行memcheck输出内容如下：
  ```
  Huawei LiteOS # memcheck
  [ERR][OsMemIntegrityCheck], 1145, memory check error!
  freeNodeInfo.pstPrev:(nil) is out of legal mem range[0x80353540, 0x84000000]

  broken node head: (nil)  (nil)  (nil)  0x0, pre node head: 0x7fc6a31b  0x8  0x80395ccc  0x80000110

  ---------------------------------------------
   dump mem tmpNode:0x80395df4 ~ 0x80395e34

   0x80395df4 :00000000 00000000 00000000 00000000
   0x80395e04 :cacacaca cacacaca cacacaca cacacaca
   0x80395e14 :cacacaca cacacaca cacacaca cacacaca
   0x80395e24 :cacacaca cacacaca cacacaca cacacaca
   
  ---------------------------------------------
   dump mem :0x80395db4 ~ tmpNode:0x80395df4
   
   0x80395db4 :00000000 00000000 00000000 00000000
   0x80395dc4 :00000000 00000000 00000000 00000000
   0x80395dd4 :00000000 00000000 00000000 00000000
   0x80395de4 :00000000 00000000 00000000 00000000
   
  ---------------------------------------------
  cur node: 0x80395df4
  pre node: 0x80395ce4
  pre node was allocated by task:SerialShellTask
  cpu0 is in exc.
  cpu1 is running.
  excType:software interrupt
  taskName = SerialShellTask
  taskId = 8
  task stackSize = 12288
  system mem addr = 0x80353540
  excBuffAddr pc = 0x80210b78
  excBuffAddr lr = 0x80210b7c
  excBuffAddr sp = 0x803b2d50
  excBuffAddr fp = 0x80280368
  R0         = 0x59
  R1         = 0x600101d3
  R2         = 0x0
  R3         = 0x8027a300
  R4         = 0x1
  R5         = 0xa0010113
  R6         = 0x80395e04
  R7         = 0x80317254
  R8         = 0x803b2de4
  R9         = 0x4
  R10        = 0x803afca4
  R11        = 0x80280368
  R12        = 0x1
  CPSR       = 0x600101d3
  ```

    以上各关键输出项含义如下：
    - “mem check error”，表示检测到了内存节点被破坏。
    - “cur node：”，表示该节点内存被踩，并打印内存地址。
    - “pre node：”，表示被踩节点前面的节点，并打印节点地址。
    - “pre node was allocated by task:SerialShellTask”表示在SerialShellTask任务中发生了踩内存。
    - “taskName”和“taskId”，分别表示发生异常的任务名和任务ID。

### memused
#### 命令功能
查看当前系统used节点中保存的函数调用栈LR信息。通过分析数据可检测内存泄露问题。

#### 命令格式
```
memused
```

#### 使用指南

该命令依赖于LOSCFG\_MEM\_LEAKCHECK，使用时需要在配置项中开启"Enable Function call stack of Mem operation recorded"。
```
Debug  ---> Enable a Debug Version---> Enable MEM Debug---> Enable Function call stack of Mem operation recorded
```

此配置项打开时，会在内存操作时，将函数调用关系LR记录到内存节点中，若相同调用栈的内存节点随时间增长而不断增多，则可能存在内存泄露，通过LR可以追溯内存申请的位置。重点关注LR重复的节点。

>![](public_sys-resources/icon-notice.gif) **须知：** 
>此配置打开时，会影响内存占用和内存操作性能，因此仅在检测内存泄露问题时打开。

#### 使用示例
```
Huawei LiteOS # memused
pstNode         LR[0]       LR[1]       LR[2]
0x802d79e4:  0x8011d740  0x8011a990  0x00000000
0x802daa0c:  0x8011d5ec  0x8011d740  0x8011a990
0x802dca28:  0x8006e6f4  0x8006e824  0x8011d5ec
0x802e2a48:  0x8014daac  0x8014db4c  0x800f6da0
0x802e8a68:  0x8011d274  0x8011d654  0x8011d740
0x802e8a94:  0x8014daac  0x8014db4c  0x8011d494
0x802eeab4:  0x8011d4e0  0x8011d658  0x8011d740
0x802f4ad4:  0x8015326c  0x80152a20  0x800702bc
0x802f4b48:  0x8015326c  0x80152a20  0x800702bc
0x802f4bac:  0x801504d8  0x801505d8  0x80150834
0x802f4c08:  0x801504d8  0x801505d8  0x80150834
0x802f4c74:  0x801504d8  0x801505d8  0x80150834
0x802f4e08:  0x801504d8  0x801505d8  0x80150834
0x802f4e60:  0x801030e8  0x801504d8  0x801505d8
0x802f4e88:  0x80103114  0x801504d8  0x801505d8
0x802f4eb4:  0x801504d8  0x801505d8  0x80150834
0x802f4f7c:  0x801504d8  0x801505d8  0x80150834
0x802f5044:  0x801504d8  0x801505d8  0x80150834
0x802f510c:  0x800702bc  0x80118f24  0x00000000
```

### hwi
#### 命令功能
查询当前中断信息。

#### 命令格式
```
hwi
```

#### 使用指南
-  输入hwi即显示当前中断号、中断次数及注册中断名称。
-  若打开宏开关`LOSCFG_CPUP_INCLUDE_IRQ`，还会显示各个中断的处理时间（cycles）、CPU占用率以及中断类型。`LOSCFG_CPUP_INCLUDE_IRQ`的使能方式可以通过`make menuconfig`在配置项中开启`Enable Cpup include irq`。
   ```
   Kernel --> Enable Cpup --> Enable Cpup include irq
   ```

#### 使用示例
**显示中断信息（LOSCFG_CPUP_INCLUDE_IRQ关闭）：**
```
Huawei LiteOS # hwi
 InterruptNo     Count     Name
       15:       975:
       53:         5:
      255:       416:      uart_pl011
```
**显示中断信息（LOSCFG_CPUP_INCLUDE_IRQ开启）：**
```
Huawei LiteOS # hwi
 InterruptNo     Count     Name         CYCLECOST   CPUUSE   CPUUSE10s    CPUUSE1s   mode
       15:       2719                   179278392   99.9      90.0        0.1        normal
       53:       5                      71929026    0.0       9.9         99.9       normal
      255:       536896560 ?            0           0.0       0.0         0.0        normal
```

### queue
#### 命令功能
查看队列使用情况。

#### 命令格式
```
queue
```

#### 使用指南

-  该命令需使能`LOSCFG_DEBUG_QUEUE`，使能方式可以通过`make menuconfig`在配置项中开启`Enable Queue Debugging`。
   ```
   Debug  ---> Enable a Debug Version---> Enable Debug LiteOS Kernel Resource ---> Enable Queue Debugging
   ```

#### 使用示例
```
Huawei LiteOS # queue
used queues information:
Queue ID <0x0> may leak, queue len is 0x10, readable cnt:0x0, writeable cnt:0x10, TaskEntry of creator:0x0x80007d5, Latest operation time: 0x614271
```

以上各关键输出项含义如下：
- Queue ID：队列编号
- queue len：队列消息节点个数
- readable cnt：队列中可读的消息个数
- writeable cnt：队列中可写的消息个数
- TaskEntry of creator：创建队列的接口地址
- Latest operation time：队列最后操作时间

### sem
#### 命令功能
查询系统内核信号量相关信息。

#### 命令格式
```
sem [ID] 
sem fulldata
```

#### 参数说明
参数|参数说明|取值范围
|:---:|:---|:---:|
|ID|信号ID号，输入形式以十进制表示或十六进制表示皆可|[0, 0xFFFFFFFF]|
|fulldata|查询所有在用的信号量信息，打印信息包括：SemID, Count, Original Count, Creater TaskEntry, Last Access Time|N/A|

#### 使用指南
- 参数缺省时，显示所有信号量的使用数及信号量总数。
- sem后加ID，当ID在[0, 1023]范围内时，返回对应ID的信号量状态（如果对应ID的信号量未被使用则进行提示）；其他取值时返回参数错误的提示。
- sem的参数ID和fulldata不可以混用。
- 参数`fulldata`需使能`LOSCFG_DEBUG_SEMAPHORE`，使能方式可以通过`make menuconfig`在配置项中开启`Enable Semaphore Debugging`。
  ```
  Debug ---> Enable a Debug Version ---> Enable Debug LiteOS Kernel Resource ---> Enable Semaphore Debugging
  ```

#### 使用示例
```
Huawei LiteOS # sem 1
   SemID       Count
   ----------  -----
   0x00000001    0x1
No task is pended on this semphore!
```

### mutex
#### 命令功能

查看互斥锁使用情况。

#### 命令格式
```
mutex
```

#### 使用指南
-  该命令需使能`LOSCFG_DEBUG_MUTEX`，使能方式可以通过`make menuconfig`在配置项中开启`Enable Mutex Debugging`。
  ```
  Debug ---> Enable a Debug Version ---> Enable Debug LiteOS Kernel Resource ---> Enable Mutex Debugging
  ```

#### 使用示例
```
Huawei LiteOS # mutex
used mutexs information: 
Mutex ID <0x0> may leak, Owner is null, TaskEntry of creator: 0x8000711,Latest operation time: 0x0
```

以上各关键输出项含义如下：
- Mutex ID：互斥锁序号
- TaskEntry of creator：创建互斥锁的接口地址
- Latest operation time：任务最后调度时间

### dlock
#### 命令功能
检查系统中的任务是否存在互斥锁（Mutex）死锁，输出系统中所有任务持有互斥锁的信息。

#### 命令格式
```
dlock
```

#### 使用指南
-  该命令需使能`LOSCFG_DEBUG_DEADLOCK`，使能方式可以通过`make menuconfig`在配置项中开启`Enable Mutex Deadlock Debugging`。
  ```
  Debug ---> Enable a Debug Version ---> Enable Debug LiteOS Kernel Resource ---> Enable Mutex Deadlock Debugging
  ```
- dlock检测输出的是在超过时间阈值（默认为10min）内没有获取到互斥锁的任务信息，并不能代表这些任务都发生了死锁，需要通过互斥锁信息及任务调用栈信息进一步确认。

#### 使用示例
- 系统中无死锁任务
  ```
  Huawei LiteOS # dlock
  Start mutexs deadlock check:
  Task_name:agenttiny_task, ID:0x0, holds the Mutexs below:
  null
  *******backtrace begin*******
  ********backtrace end********

  Task_name:SerialShellTask, ID:0x4, holds the Mutexs below:
  null
  *******backtrace begin*******
  ********backtrace end********

  -----------End-----------
  ```

- 系统出现死锁
    ```
    Huawei LiteOS # dlock
    Start mutexs deadlock check:
    Task_name:SendToSer, ID:0x0, holds the Mutexs below:
    null
    *******backtrace begin*******
    ********backtrace end********

    Task_name:WowWriteFlashTask, ID:0x3, holds the Mutexs below:
    null
    *******backtrace begin*******
    ********backtrace end********

    Task_name:system_wq, ID:0x4, holds the Mutexs below:
    null
    *******backtrace begin*******
    ********backtrace end********

    Task_name:app_Task, ID:0x5, holds the Mutexs below:
    <Mutex0 info>
    Ptr handle:0x8036104c
    Owner:app_Task
    Count:1
    Pended task: 0. name:mutexDlock_Task, id:0xc
    *******backtrace begin*******
    ********backtrace end********

    Task_name:Swt_Task, ID:0x6, holds the Mutexs below:
    null
    *******backtrace begin*******
    *******backtrace begin*******
    traceback 0 -- lr = 0x4    fp = 0x0
    ********backtrace end********

    Task_name:IdleCore000, ID:0x7, holds the Mutexs below:
    null
    *******backtrace begin*******
    ********backtrace end********

    Task_name:eth_irq_Task, ID:0xb, holds the Mutexs below:
    null
    *******backtrace begin*******
    ********backtrace end********

    Task_name:mutexDlock_Task, ID:0xc, holds the Mutexs below:
    <Mutex0 info>
    Ptr handle:0x80361060
    Owner:mutexDlock_Task
    Count:1
    Pended task: 0. name:app_Task       , id:0x5
    *******backtrace begin*******
    ********backtrace end********

    -----------End-----------
    ```

  以上各关键输出项含义如下：
  - Task_name:xxx, ID:xxx, holds the Mutexs below：表示疑似死锁的任务名和ID，后面几行信息是该任务持有的各个互斥锁信息，如果为“null”表示该任务并没有持有互斥锁。
  - \<MutexN info\>：后面几行是该互斥锁的详细信息，包括：
    | 输出项        | 说明                                        |
    | ------------- | ------------------------------------------- |
    | Ptr handle    | 该互斥锁的控制块地址                        |
    | Owner         | 持有该互斥锁的任务名                        |
    | Count         | 该互斥锁的引用计数                          |
    | Pended Task   | 正在等待该互斥锁的任务名，如果没有则为null  |

### swtmr
#### 命令功能
查询系统软件定时器相关信息。

#### 命令格式
```
swtmr [ID]
```

#### 参数说明
|参数|参数说明|取值范围|
|:---:|:---|:---:|
|ID|软件定时器ID号，输入形式以十进制表示或十六进制表示皆可|[0, 0xFFFFFFFF]|

#### 使用指南
-  参数缺省时，默认显示所有软件定时器的相关信息。
-  swtmr后加ID号时，当ID在[0, 当前软件定时器个数 - 1]范围内时，返回对应ID的软件定时器的状态，其他取值时返回错误提示。

#### 使用示例
```
Huawei LiteOS # swtmr

SwTmrID     State    Mode    Interval  Arg         handlerAddr
----------  -------  ------- --------- ----------  --------
0x00000000  Ticking  Period   1000     0x00000000  0x800442d

Huawei LiteOS # swtmr 1

The SwTimerID is not exist.

Huawei LiteOS # swtmr 0

SwTmrID     State    Mode    Interval  Arg         handlerAddr
----------  -------  ------- --------- ----------  --------
0x00000000  Ticking  Period   1000     0x00000000  0x800442d
```

以上各关键输出项含义如下：
| 输出项      | 说明                   |
| ----------- | --------------------- |
| SwTmrID     | 软件定时器ID           |
| State       | 软件定时器状态         |
| Mode        | 软件定时器模式         |
| Interval    | 软件定时器使用的Tick数 |
| Arg         | 传入的参数             |
| handlerAddr | 回调函数的地址         |

### systeminfo
#### 命令功能
显示当前操作系统的资源使用情况，包括任务、信号量、互斥锁、队列、软件定时器等。
对于信号量、互斥锁、队列和软件定时器，如果在系统镜像中已经裁剪了这些模块，那么说明系统没有使用这些资源，该命令也就不会显示这些资源的情况。

#### 命令格式
```
systeminfo
```

#### 使用示例
```
Huawei LiteOS # systeminfo

   Module    Used      Total
--------------------------------
   Task      5         16
   Sem       0         20
   Mutex     1         20
   Queue     1         10
   SwTmr     1         16
```

以上各关键输出项含义如下：
| 输出项      | 说明            |
| ---------- | --------------- |
| Module     | 模块名称         |
| Used       | 当前使用量       |
| Total      | 最大可用量       |

### log
#### 命令功能
设置&查询系统的日志打印等级。

#### 命令格式
```
log level [levelNum]
```

#### 参数说明
参数|参数说明|取值范围
|:---:|:---|:---:|
|levelNum|设置日志打印等级|[0x0，0x5]|

#### 使用指南
-  该命令暂时不支持指定模块和文件。
-  该命令需使能`LOSCFG_SHELL_LK`，使能方式可以通过`make menuconfig`在配置项中开启`Enable Shell lk`。
    ```
    Debug --> Enable a Debug Version --> Enable Shell --> Functionality of Shell --> Enable Shell lk
    ```
-  `log level`命令用于设置系统的日志打印等级，包括6个等级。
    ```
    TRACE_EMG = 0,
    TRACE_COMMOM = 1,
    TRACE_ERROR = 2,
    TRACE_WARN = 3,
    TRACE_INFO = 4,
    TRACE_DEBUG = 5
    ```
-  若`log level`命令不加`levelNum`参数，则会显示系统当前的日志打印等级，并且提示使用方法。

#### 使用示例
```
Huawei LiteOS # log level 0
Set current log level EMG

Huawei LiteOS # log level 2
Set current log level ERR

Huawei LiteOS # log level 5
Set current log level DEBUG
```

### dmesg
#### 命令功能
dmesg命令用于控制内核dmesg缓存区。

#### 命令格式
```
dmesg

dmesg [-c | -C | -D | -E | -L | -U]

dmesg -s size

dmesg -l level

dmesg > file(暂不支持)
```

#### 参数说明
|参数|参数说明|取值范围|
|:---:|:---|:---|
|-c|打印缓存区内容并清空缓存区|N/A|
|-C|清空缓存区|N/A|
|-D \| -E|关闭/开启控制台打印(暂不支持)|N/A|
|-s size|设置缓存区大小|N/A|
|-l level|设置缓存区的日志打印等级|0 -- 5|
|> file|将缓存区内容写入文件(暂不支持)|N/A|

#### 使用指南
-  该命令需使能`LOSCFG_SHELL_DMESG`，使能方式可以通过`make menuconfig`在配置项中开启`Enable Shell dmesg`。
   ```
   Debug  ---> Enable a Debug Version---> Enable Shell ---> Functionality of Shell ---> Enable Shell dmesg
   ```
-  参数缺省时，默认打印缓存区内容。
-  参数均不能混合使用。

#### 使用示例
```
Huawei LiteOS # dmesg -s 10
Set dmesg buf size 10 success

Huawei LiteOS # dmesg -l 1
Set current dmesg log level COMMOM
```

### stack

#### 命令功能

stack命令用于显示当前操作系统内所有栈的信息。

#### 命令格式
```
stack
```

#### 使用指南

该功能在Cortex A核上默认开启。

#### 使用示例
```
Huawei LiteOS # stack

 stack name    cpu id     stack addr     total size   used size
 ----------    ------     ---------      --------     --------
  udf_stack      3        0x3c800        0x28         0x0   
  udf_stack      2        0x3c828        0x28         0x0   
  udf_stack      1        0x3c850        0x28         0x0   
  udf_stack      0        0x3c878        0x28         0x0   
  abt_stack      3        0x3c8a0        0x28         0x0   
  abt_stack      2        0x3c8c8        0x28         0x0   
  abt_stack      1        0x3c8f0        0x28         0x0   
  abt_stack      0        0x3c918        0x28         0x0   
  fiq_stack      3        0x3ca40        0x40         0x0   
  fiq_stack      2        0x3ca80        0x40         0x0   
  fiq_stack      1        0x3cac0        0x40         0x0   
  fiq_stack      0        0x3cb00        0x40         0x0   
  svc_stack      3        0x3cb40        0x2000       0x524 
  svc_stack      2        0x3eb40        0x2000       0x524 
  svc_stack      1        0x40b40        0x2000       0x524 
  svc_stack      0        0x42b40        0x2000       0x528 
  irq_stack      3        0x3c940        0x40         0x0   
  irq_stack      2        0x3c980        0x40         0x0   
  irq_stack      1        0x3c9c0        0x40         0x0   
  irq_stack      0        0x3ca00        0x40         0x0   
  exc_stack      3        0x44b40        0x1000       0x0   
  exc_stack      2        0x45b40        0x1000       0x0   
  exc_stack      1        0x46b40        0x1000       0x0   
  exc_stack      0        0x47b40        0x1000       0x0
```

### cpup
#### 命令功能
查询系统CPU的占用率，并以百分比显示占用率。

#### 命令格式
```
cpup [mode] [taskID]
```

#### 参数说明
|参数|参数说明|取值范围|
|:---:|:---|:---:|
|mode|缺省：显示系统最近10s的CPU占用率<br>0：显示系统最近10s的CPU占用率<br>1：显示系统最近1s的CPU占用率<br>其他数字：显示系统启动至今总的CPU占用率| [0,0xFFFF]<br>或0xFFFFFFFF|
|taskID|任务ID号|[0,0xFFFF]或0xFFFFFFFF|

#### 使用指南
-  参数缺省时，显示系统最近10s的CPU占用率。

-  只输入一个参数时，该参数为mode，显示系统相应时间的CPU占用率。

-  输入两个参数时，第一个参数为mode，第二个参数为taskID，显示指定任务ID的任务在相应时间的CPU占用率。

#### 使用示例
```
Huawei LiteOS # cpup

SysCpuUsage in 10s: 69.2

Huawei LiteOS # cpup 0

SysCpuUsage in 10s: 71.0

Huawei LiteOS # cpup 1

SysCpuUsage in 1s: 98.0

Huawei LiteOS # cpup 2

SysCpuUsage in all time: 75.9
```

### watch
#### 命令功能
周期性监听一个命令的运行结果。

#### 命令格式
```
watch [-c | -n | -t | --count | --interval | -no-title] [command]
watch --over
```

#### 参数说明

|参数|参数说明|缺省值|取值范围|
|:---:|:---|:---|:---|
|-c / --count|命令执行的总次数|0xFFFFFF|（0,0xFFFFFF]|
|-n / --interval|命令周期性执行的时间间隔（s）|1s|（0,0xFFFFFF]|
|-t / -no-title|关闭顶端的时间显示|N/A|N/A|
|command|需要监听的Shell命令|N/A|N/A|
|--over|关闭当前的监听|N/A|N/A|

#### 使用指南
-  command参数必须是Shell命令，对于非Shell命令，会有错误提示“command is not fount”。
-  如果要监听命令，command是必填参数。
-  --over参数与其他参数不能混合使用。

#### 使用示例
```
Huawei LiteOS # watch -c 5 task 1

Huawei LiteOS # Thu Jan  1 16:26:26 1970

TaskName = Swt_Task
TaskId = 0x1
Thu Jan  1 16:26:27 1970

TaskName = Swt_Task
TaskId = 0x1
Thu Jan  1 16:26:28 1970

TaskName = Swt_Task
TaskId = 0x1
Thu Jan  1 16:26:29 1970

TaskName = Swt_Task
TaskId = 0x1
Thu Jan  1 16:26:30 1970

TaskName = Swt_Task
TaskId = 0x1
```