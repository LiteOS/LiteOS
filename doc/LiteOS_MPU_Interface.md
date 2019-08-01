LiteOS MPU功能使用说明

# ARM MPU简介

ARM cortex-m的部分MCU支持MPU功能，MPU是“Memory Protection Unit”的缩写。在支持MPU的ARM MCU上，执行模式除了“thread”和“handler”外，“thread”模式下的任务执行还分为特权模式（Privileged ）和非特权模式（Unprivileged ）。

在启用了MPU后，可以通过MPU的相关设置，设置内存区块的属性，从而达到不同内存不同访问权限的目的，从而可以保护关键的区域。

MPU通过“region”来管理内存区块，而一般MPU支持的MPU个数通常较少，大多为8组。

MPU实现中存在一种叫做“背景映射”的概念，“背景映射”有以下特点：

1. **仅**对特权模式有效
2. 当所有映射都不匹配地址会使用”背景映射“中的属性

MPU背景映射属性：

| 地址区间                | 权限 | cache属性 | 区间内容             |
| ----------------------- | ---- | --------- | -------------------- |
| 0x00000000 ~ 0x1fffffff | XRW  | WT        | ROM/FLASH            |
| 0x20000000 ~ 0x3fffffff | XRW  | WB        | SRAM                 |
| 0x40000000 ~ 0x5fffffff | -RW  | device    | on-chip peripheral   |
| 0x60000000 ~ 0x7fffffff | XRW  | WB        | RAM                  |
| 0x80000000 ~ 0x9fffffff | -RW  | WT        | RAM                  |
| 0xa0000000 ~ 0xbfffffff | -RW  | device    | shared device        |
| 0xc0000000 ~ 0xdfffffff | -RW  | device    | non-shared device    |
| 0xe0000000 ~ 0xe00fffff | -RW  | strongly  | PPB                  |
| 0xe0100000 ~ 0xffffffff | -RW  | device    | vendor system region |

# LiteOS对MPU的支持简介

MPU的主要目标是保护关键数据不被任务的以外跑飞或地址越界所改写。而且LiteOS的使用场景为封闭场景，一旦发布内容就固定，不存在第三方恶意代码的问题。所以为了节省MPU region起见，LiteOS的支持如下：
1. 特权态使用背景映射，如前文所述，背景映射可以很好的满足特权态的需求
2. 每个任务有自己的一组mpu regions属性，在任务上下文切换时设置
3. flash区域设置为全局的（也就是所以任务共享），属性为特权态可读写，非特权态只读，所有模式可执行
4. on-chip peripheral设置为所有模式可读可写不可执行
5. 整个ram区间对非特权任务无映射
6. 非特权任务在创建的时候传入自己的region配置，指定自己需要的内存区域
7. 如果非特权任务需要内存分配，则在创建的时候指定自己希望的heap大小，这里简称”每任务堆“
8. 任务创建时，栈会被设置为当前任务可读可写不可执行

# LiteOS任务创建
当前实验仓（LiteOS_Lab）支持两种不同的方式创建任务：
1. 动态创建任务的方式
2. 静态创建任务的方式
动态创建就是在运行时调用`LOS_TaskCreate`创建任务
静态创建就是在编译器通过`LOS_TASK_DEF`宏定义一个任务。使用这种方法，TCB，栈在编译期就分配好了内存。在运行期只需要很少量的初始化工作。

注意：这两种方式是互斥的。

# MPU相关任务创建
MPU支持两种模式的任务，”特权“任务和”非特权“任务。下面就分开介绍两种任务的创建方法。
## 动态方式创建任务
### 使能MPU支持
在工程配置里或Makefile里为C加上预编译符号`LOSCFG_ENABLE_MPU=YES`
在工程配置里或Makefile里为ASM加上预编译符号`LOSCFG_ENABLE_MPU`
### 创建特权态任务
```C
    TSK_INIT_PARAM_S task_init_param;
    
    task_init_param.usTaskPrio = 1;
    task_init_param.pcName = "t1";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task1;
    task_init_param.uwStackSize = 0x130;
    
    task_init_param.pRegions = NULL;

    uwRet = LOS_TaskCreate(&g_TskHandle, &task_init_param);
```
### 创建非特权态任务
```C
    LOS_MPU_PARA mpuPara[MPU_NR_USR_ENTRIES];

    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 1;
    task_init_param.pcName = "t1";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task1;
    task_init_param.uwStackSize = 0x130;

    memset(&mpuPara, 0, sizeof (mpuPara));
    mpuPara[0].uwRegionAddr = ((UINT32)(&c222) & ~0xff);
    mpuPara[0].uwRegionSize = 256;
    mpuPara[0].uwRegionAttr = MPU_ATTR_RW_RW | MPU_ATTR_WB | MPU_ATTR_EN;

    task_init_param.uwHeapSize = 0x800;
    task_init_param.pRegions = mpuPara;

    uwRet = LOS_TaskCreate(&g_TskHandle, &task_init_param);
```
### 注意
MPU功能开启时任务创建的参数结构体`TSK_INIT_PARAM_S`增加了两个成员：`uwHeapSize`和`pRegions`。
当`pRegions`为`NULL`时，创建特权任务。
当`pRegions`不为`NULL`时，创建非特权任务。同时，`uwHeapSize`表明当前非特权任务期望的每任务堆大小。
## 静态方式创建任务
### 使能静态任务支持
在上一节配置的基础上为C编译额外配置`-DLOSCFG_STATIC_TASK=YES`。
### 创建特权任务
在代码的某处：
```C
LOS_TASK_DEF(t1, "t1", task1, 0, 1, 0x130, 0, NULL);
```
运行期：
```C
LOS_TASK_INIT(t1, &t1);
```
### 创建非特权任务
在代码的某处：
```C
static const LOS_MPU_PARA mpuPara[MPU_NR_USR_ENTRIES] = {/* regions */};
LOS_TASK_DEF(t1, "t1", task1, 0, 1, 0x130, 0x800, (VOID*)mpuPara);
```
运行期：
```C
LOS_TASK_INIT(t1, &t1);
```
### 注意
LOS_TASK_DEF的参数说明：
```C
LOS_TASK_DEF(name, taskName, entry, taskPrio, arg, stackSize, heapSize,
             regions);
```
# 非特权命令任务调用特权接口
系统提供了很多API供用户使用，这些API往往会访问一些内核数据。而很多第三方库往往也会访问一些全局变量。这些全局变量都在用户可访问的内存范围之外。所以非特权任务调用这些接口时需要特权权限。
LiteOS提供了一组函数或宏来支撑这些调用：
1. `osEnterPrivileged`
2. `osExitPrivileged`
3. `LOS_DO_PRIVILEDGED`

对于用户自身需要临时进入特权态做一系列操作的话可以使用`osEnterPrivileged`和`osExitPrivileged`来完成，示例：
```C
UINT32 flags = osEnterPrivileged();
// do_something in priviledged mode
osExitPrivileged(flags);
```

对于一些简短的操作则可以使用封装好的宏：
```C
LOS_DO_PRIVILEDGED(a_priviledged_cnt++);
```

这个宏是对前面提到的两个函数的封装，方便短语句调用，比如很短的少数几条C语句。一条或几条API函数调用等。