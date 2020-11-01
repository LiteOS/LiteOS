#Shell使用教程


##使用场景
Shell命令可以通过串口工具输入输出，用户可以新增定制的命令，需重新编译链接后运行。

##编译Shell
通过make menuconfig打开shell组件的开关

<mark>Debug --> Enable a Debug Version --> Enable Shell --> Functionality of Shell --> Select shell functionality -->  Enable full functionality of shell


##开发流程

Shell的开发流程如下：

####1.增加shell命令

创建shell命令处理函数

    #include "shell.h"
    #include "shcmd.h"
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

####2.注册命令
有两种注册方式：
·静态注册命令
·系统运行时动态注册命令
#####a.静态注册命令方式

    SHELLCMD_ENTRY(testShellcmd, CMD_TYPE_EX, "test", XARGS, (CMD_CBK_FUNC)osShellCmdTest)
#####b.动态注册命令方式
    osCmdReg(CMD_TYPE_EX, "test", XARGS, (CMD_CBK_FUNC)osShellCmdTest);
######testShellcmd
静态注册命令比动态注册命令，增加了一个命名变量名。
静态注册时定义的变量名,需要在链接选项中增加

    LITEOS_TABLES_LDFLAGS += -utestShellcmd

######命令类型
命令类型填写为：CMD_TYPE_EX，不支持标准命令参数输入，会把用户填写的命令关键字屏蔽掉；例如输入test -a -b，传入注册的函数的只有-a，-b，分别为argv[0],argv[1]
命令类型填写为: CMD_TYPE_STD，支持的标准参数输入，所有输入的字符都会通过命令解析后被传入。例如输入test -a -b，会把test，-a，-b都传入注册的函数,分别为argv[0],argv[1],argv[2]
######"test"
命令名，即在shell中访问执行函数的命令名。
######XARGS
调用执行函数入参最大个数，暂不支持；
######(CMD_CBK_FUNC)osShellCmdTest
函数地址，即执行函数。
######注意
静态注册命令方式一般用在系统常用命令注册，动态注册命令一般用在用户命令注册。
静态命令注册方式需要在build/mk/liteos_tables_ld.mk的LITEOS_TABLES_LDFLAGS变量中添加-utestShellcmd。
命令关键词必须是唯一的，两个不同的执行函数不能有相同的命令关键字，否则只会执行其中一个。
####3.添加的命令处理函数原型
    UINT32 osShellCmdTest(UINT32 argc, CHAR **argv)
######argc
shell命令中参数的个数。
######argv
执行shell命令时传入命令处理函数的参数。
和注册时命令类型有关。
####4.输入shell命令
通过串口发送shell命令，以换行为串口接收结束标志位。
发送命令时，需要在命令末尾添加回车后点击发送(回车会自动在命令末尾追加"\r\n")。

##内置命令

###help
####命令功能
help命令用于显示当前操作系统内所有操作指令。
####命令格式
    help
####参数说明
无
####输出说明
查看系统所有操作指令。

###cpup
####命令功能
cpup命令用于查询系统CPU的占用率。
####命令格式
    cpup [mode] [taskID]
####参数说明
参数|参数说明|取值范围
:---:|:---|:---:
mode|缺省：显示系统10s前的CPU占用率<br>0：显示系统10s前的CPU占用率<br>1：显示系统1s前的CPU占用率<br>其他数字：显示系统启动至今总的CPU占用率| [0,0xFFFF]<br>或0xFFFFFFFF
taskID|任务ID号|[0,0xFFFF]或0xFFFFFFFF
####使用指南
参数缺省时，显示系统10s前的CPU占用率。
只有一个参数时，该参数为mode，显示系统相应时间前的CPU占用率。
输入两个参数时，第一个参数为mode，第二个参数为taskID，显示对应ID号任务的相应时间前的CPU占用率。

###date

####命令功能
date命令用于查询及设置系统时间。
####命令格式
    date

    date --help

    date +Format

    date -s YY/MM/DD

    date -s hh:mm:ss

    date -r Filename(暂不支持)

####参数说明
参数|参数说明|取值范围
:---:|:---|:---:
--help|使用帮助|
+Format|根据Format格式打印时间|--help中列出的占位符
-s YY/MM/DD|设置系统时间，用“/”分割的年月日|>= 1970/01/01
-s hh:mm:ss|设置系统时间，用“:”分割的时分秒|
-r|查询文件修改时间（暂不支持）

####使用指南
date参数缺省时，默认显示当前系统时间。
--help、+Format、-s、-r不能混合使用。

###free
####命令功能
free命令可显示系统内存的使用情况，同时显示系统的text段、data段、rodata段、bss段大小。

####命令格式
    free [-k | -m]
####参数说明
参数|参数说明|取值范围
:---:|:---|:---:
无参数|以Byte为单位显示
-k|以KB为单位显示
-m|以MB为单位显示

####使用指南
输入free显示内存使用情况，total表示系统动态内存池总量，used表示已使用空间总量，free表示未被分配的内存大小。text表示代码段大小，data表示数据段大小，rodata表示只读数据段大小，bss表示未初始化全局变量占用内存大小。
free命令可以以三种方式来显示内存使用情况，分别是以Byte为单位、以KB为单位和以MB为单位。

###log
####命令功能
log命令用于修改&查询日志配置。
####命令格式
    log level [levelNum]
module、path的设置暂时不支持。
####参数说明
参数|参数说明|取值范围
:---:|:---|:---:
levelNum|配置日志打印等级|[0x0，0x5]

####使用指南
该命令依赖于LOSCFG_SHELL_LK，使用时在配置项中开启"Enable Shell lk"。

    $ make menuconfig

<mark>Debug --> Enable a Debug Version Enable Shell --> Functionality of Shell --> Enable Shell lk
log level命令用于配置日志的打印等级，包括5个等级。

    TRACE_EMG = 0,

    TRACE_COMMOM = 1,

    TRACE_ERROR = 2,

    TRACE_WARN = 3,

    TRACE_INFO = 4,

    TRACE_DEBUG = 5

若log level命令不加[levelNum]参数，则默认查看当前打印等级，并且提示使用方法。
###memcheck
####命令功能
检查动态申请的内存块是否完整，是否存在内存越界造成节点损坏。
####命令格式
    memcheck
####参数说明
无
####使用指南
需使能LOSCFG_KERNEL_MEM_BESTFIT
当内存池所有节点完整时，输出"memcheck over, all passed!"。
当内存池存在节点不完整时，输出被损坏节点的内存块信息。
###sem
####命令功能
sem命令用于查询系统内核信号量相关信息。
####命令格式
sem [ID] / fulldata
####参数说明
参数|参数说明|取值范围
:---:|:---|:---:
ID|信号ID号。|[0, 0xFFFFFFFF]
fulldata|查询所有在用的信号量信息，打印信息包括如下：SemID, Count, Original Count, Creater TaskEntry, Last Access Time。

####使用指南
参数缺省时，显示所有的信号量的使用数及信号量总数。
sem后加ID，显示对应ID信号量的使用数。
参数fulldata依赖于LOSCFG_DEBUG_SEMAPHORE，使用时在配置项中开启"Enable Semaphore Debugging"。
<mark>Debug  ---> Enable a Debug Version---> Enable Debug LiteOS Kernel Resource---> Enable Semaphore Debugging



###swtmr

####命令功能
swtmr命令用于查询系统软件定时器相关信息。

####命令格式
    swtmr [ID]
####参数说明
参数|参数说明|取值范围
:---:|:---|:---:
ID|软件定时器ID号。|[0, 0xFFFFFFFF]
####使用指南

参数缺省时，默认显示所有软件定时器的相关信息。
swtmr后加ID号时，显示ID对应的软件定时器相关信息。

###systeminfo
####命令功能
systeminfo命令用于显示当前操作系统内资源使用情况，包括任务、信号量、互斥量、队列、定时器等。
####命令格式

    systeminfo

####参数说明
无
####使用指南
systeminfo用于显示当前操作系统内资源使用情况。

###task
####命令功能
task命令用于查询系统任务信息。
####命令格式

    task [ID]

####参数说明
参数|参数说明|取值范围
:---:|:---|:---:
ID|任务ID号。|[0, 0xFFFFFFFF]
####使用指南

参数缺省时默认打印全部运行任务信息。
task后加ID，显示ID对应的任务名、任务TID、任务的调用栈信息以及当前系统运行的全部任务信息。


###uname
####命令功能
uname命令用于显示当前操作系统的名称，数据创建时间，系统名称，版本信息等。
####命令格式
    uname[-a | -s | -t | -v | --help]

####参数说明
参数|参数说明
:---:|:---
-a|显示全部信息
-t|显示数据创建的时间
-s|显示操作系统名称
-v|显示版本信息
--help|显示uname指令格式提示
####使用指南
uname用于显示当前操作系统名称。
语法uname -a | -t| -s| -v 描述uname 命令将正在使用的操作系统名写到标准输出中，这几个参数不能混合使用。


###watch
####命令功能
watch命令用于周期性的监视一个命令的运行结果。
####命令格式

    watch [-c/-n/-t/--count/--interval/-no-title/--over] [command]
####参数说明
参数|参数说明|缺省值|取值范围
:---:|:---|:---|:---|
-c / --count|命令执行的总次数|0xFFFFFF|（0,0xFFFFFF]
-n / --interval|命令周期性执行的时间间隔（s）|1s|（0,0xFFFFFF]
-t / -no-title|关闭顶端的时间显示
command|需要监听的命令
--over|关闭当前监测指令



###dmesg
####命令功能
dmesg命令用于控制内核dmesg缓存区。
命令格式

    dmesg

    dmesg [-c/-C/-D/-E/-L/-U]

    dmesg -s size

    dmesg -l level

    dmesg > fileA(暂不支持)

####参数说明
参数|参数说明|取值范围
:---:|:---|:---|:---
-c|打印缓存区内容并清空缓存区
-C|清空缓存区
-D/-E|关闭/开启控制台打印(暂不支持)
-L/-U|关闭/开启串口打印
-s size|设置缓存区大小 size是要设置的大小
-l level|设置缓存等级|0 -- 5
> fileA|将缓存区内容写入文件(暂不支持)
####使用指南
该命令依赖于LOSCFG_SHELL_DMESG，使用时在配置项中开启"Enable Shell dmesg"。
<mark>Debug  ---> Enable a Debug Version---> Enable Shell---> Enable Shell dmesg

dmesg参数缺省时，默认打印缓存区内容。
各“ - ”选项不能混合使用。
关闭串口打印会影响shell使用
