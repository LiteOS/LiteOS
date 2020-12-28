KERNEL DEMOS使用指南
================
## 目 录
* [1.前言](#1)
* [2.实例介绍](#2)
    * [2.1 任务](#a)
    * [2.2 内存管理](#b)
    * [2.3 中断](#c)
    * [2.4 IPC通信](#d)
    * [2.5 时间管理](#e)
    * [2.6 双向链表](#f)
* [3.运行实例](#3)


>![](public_sys-resources/icon-notice.gif) **须知：**
>如果想更详细了解LiteOS各内核模块，可以访问<a href="https://gitee.com/LiteOS/LiteOS/blob/master/doc/Huawei_LiteOS_Developer_Guide_zh.md" target="_blank">LiteOS开发指南</a>。
<h2 id="1">1.前言</h2>

**使用方法**

用户可以直接参考[3.运行实例](#3)来运行demos/kernel下各个实例。

具体的实例介绍在[2.实例介绍](#2)中，下面是对各个实例的概述：

**任务**

los_api_task.c/.h实现了任务实例。

-   实现了任务的创建、删除、延迟、挂起、恢复等功能，以及锁定和解锁任务调度。

**内存管理**

los_api_dynamic_mem.c/.h实现了动态内存实例
-   实现了动态内存的初始化、申请、使用、释放功能。

los_api_static_mem.c/.h实现了静态内存实例
-   实现了静态内存的初始化、申请、使用、清除和释放功能。

**中断**

los_api_interrupt.c/.h实现了中断实例。
-   实现了中断的创建、删除、使能、禁止、请求位的清除等功能。

**IPC通信**

los_api_msgqueue.c/.h实现了消息队列实例。
-   实现了消息队列的创建、删除、发送和接收功能。

los_api_event.c/.h实现了事件实例。
-   实现了事件的创建、删除、读事件和写事件功能。

los_api_mutex.c/.h实现了互斥锁实例。
-   实现了互斥锁的创建、删除、申请和释放等功能。

los_api_sem.c/.h实现了信号量实例。
-   实现了信号量的创建、删除、申请和释放等功能。

**时间管理**

los_api_systick.c/.h实现了tick时间实例。
-   实现了获取系统Tick数和每个Tick的Cycle数的功能。

los_api_timer.c/.h实现了软件定时器实例。
-   实现了软件定时器的创建、开始、停止、删除等功能。

**双向链表**

los_api_list.c/.h实现了双向链表实例
-   实现了双向链表的初始化，链表的非空检测，节点的增加、删除等功能。

<h2 id="2">2.实例介绍</h2>

<h3 id="a">2.1 任务</h3>

**任务实例：<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/kernel/api/los_api_task.c" target="_blank">los_api_task.c</a>/<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/kernel/api/los_api_task.h" target="_blank">los_api_task.h</a>**

任务是竞争系统资源的最小运行单元。任务可以使用或等待CPU、使用内存空间等系统资源，并独立于其它任务运行。

本实例执行以下步骤：

1.  锁任务LOS\_TaskLock，防止创建任务时发生任务调度。
2.  通过LOS\_TaskCreate创建了2个任务，高优先级任务Example\_TaskHi和低优先级任务Example\_TaskLo，解锁任务LOS\_TaskUnlock。
3.  Example\_TaskHi任务被调度，通过LOS\_TaskSuspend挂起任务。
4.  Example\_TaskLo任务被调度，通过LOS\_TaskResume恢复挂起的Example\_TaskHi任务。
5.  Example\_TaskHi任务被唤醒，执行完成，Example\_TaskLo任务被调度，执行完成。

**实例结果**

```
Kernel task demo begin.
LOS_TaskLock() ok.
Create Example_TaskHi ok.
Create Example_TaskLo ok.
Enter TaskHi Handler.
Enter TaskLo Handler.
TaskHi LOS_TaskDelay Done.
TaskHi LOS_TaskSuspend ok.
TaskHi LOS_TaskResume ok.
Kernel task demo ok.
```

<h3 id="b">2.2 内存管理</h3>

Huawei LiteOS的内存管理分为静态内存管理和动态内存管理，提供内存初始化、分配、释放等功能。

**动态内存管理实例：<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/kernel/api/los_api_dynamic_mem.c" target="_blank">los_api_dynamic_mem.c</a>/<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/kernel/api/los_api_dynamic_mem.h" target="_blank">los_api_dynamic_mem.h</a>**

当用户需要使用内存时，可以通过操作系统的动态内存申请函数申请指定大小的内存，动态内存管理模块会从系统配置的一块连续内存（内存池）中为用户分配指定大小的内存块。一旦使用完毕，用户再通过动态内存释放函数归还所占用内存，使之可以重复使用。

本实例执行以下步骤：

1.  通过LOS\_MemInit初始化一个动态内存池。
2.  使用LOS\_MemAlloc从动态内存池中申请一块内存。
3.  在这个内存块中存放一个数据。
4.  打印出这个内存块中的数据。
5.  通过LOS\_MemFree释放这块内存。

**实例结果**

```
Kernel dynamic memory demo begin.
Mempool init ok.
Mem alloc ok.
*mem = 828.
Mem free ok.
Kernel dynamic memory demo ok.
```

**静态内存管理实例：<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/kernel/api/los_api_static_mem.c" target="_blank">los_api_static_mem.c</a>/<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/kernel/api/los_api_static_mem.h" target="_blank">los_api_static_mem.h</a>**

静态内存池由一个控制块和若干相同大小的内存块构成。控制块位于内存池头部，用于内存块管理。内存块大小在内存池初始化时设定，初始化后内存块的大小不可变更。所以静态内存池是由若干大小相同的内存块构成的。内存块的申请和释放以块大小为粒度。

本实例执行以下步骤：

1.  通过LOS\_MemboxInit初始化一个静态内存池。
2.  使用LOS\_MemboxAlloc从静态内存池中申请一块内存。
3.  在这个内存块中存放一个数据。
4.  打印出内存块中的数据。
5.  通过LOS\_MemboxClr清除内存块中的数据。
6.  通过LOS\_MemboxFree释放这块内存。

**实例结果**

```
Kernel static memory demo begin.
Mem box init ok.
Mem box alloc ok.
*mem = 828.
Clear data ok, *mem = 0.
Mem box free ok.
Kernel static memory demo ok.
```

<h3 id="c">2.3 中断</h3>

**中断实例：<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/kernel/api/los_api_interrupt.c" target="_blank">los_api_interrupt.c</a>/<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/kernel/api/los_api_interrupt.h" target="_blank">los_api_interrupt.h</a>**

中断是指出现需要时，CPU暂停执行当前程序，转而执行新程序的过程。即在程序运行过程中，出现了一个必须由CPU立即处理的事务。此时，CPU暂时中止当前程序的执行转而处理这个事务，这个过程就叫做中断。

本实例以软件中断为例来说明中断使用的一般步骤：

1.  通过LOS\_IntLock关中断。
2.  通过LOS\_HwiCreate创建中断。
3.  通过LOS\_HwiEnable使能中断。
4.  通过LOS\_IntRestore开中断。
5.  通过LOS\_HwiTrigger触发中断。

**实例结果**

```
Kernel interrupt demo begin.
User IRQ test ok.
Kernel interrupt demo ok.
```

**说明：** 目前的中断测试代码提供了基本框架，中断硬件初始化代码请用户根据开发板硬件情况在Example\_Exti0\_Init\(\)函数中自行实现。

以下以野火挑战者F429开发板为例来说明硬件中断使用的一般步骤：

1.  通过LOS\_IntLock关中断。
2.  通过Example_Exti0_Init初始化按键PA0。
2.  通过LOS\_HwiCreate创建中断。
3.  通过LOS\_HwiEnable使能中断。
4.  通过LOS\_IntRestore开中断。
5.  通过硬件按键PA0触发中断。
6.  在中断处理程序中通过\_\_HAL\_GPIO\_EXTI\_CLEAR\_IT清除硬件中断。

**实例结果**

按下PA0引脚对应的按键（即野火挑战者F429开发板上的对应的KEY1按键）串口输出如下：

```
Button IRQ test ok.
```

**注意事项：**当前LiteOS系统对于Cortex-M系列芯片，前16个中断号为系统预留，不开放给用户使用。

<h3 id="d">2.4 IPC通信</h3>

IPC通信提供消息队列、事件、互斥锁和信号量功能。

**消息队列实例：<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/kernel/api/los_api_msgqueue.c" target="_blank">los_api_msgqueue.c</a>/<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/kernel/api/los_api_msgqueue.h" target="_blank">los_api_msgqueue.h</a>**

队列又称消息队列，是一种常用于任务间通信的数据结构，能够接收来自任务或中断的不固定长度的消息，并根据不同的接口选择消息是否存放在自己空间。任务能够从队列里读取消息，当队列中的消息为空时，挂起读取任务；当队列中有新消息时，挂起的读取任务被唤醒并处理新消息。

本实例执行以下步骤：

1.  在Example_MsgQueue中锁任务调度，通过LOS\_TaskCreate创建两个任务 send\_Entry和recv\_Entry。
2.  在Example_MsgQueue中通过LOS\_QueueCreate创建一个消息队列。
3.  解锁任务调度，任务send\_Entry被调度，该任务循环通过LOS\_QueueWrite以非阻塞模式向队列写入消息，写入一条消息后休眠50Tick，循环写API_MSG_NUM条消息。
4.  任务recv\_Entry被唤醒，该任务循环通过LOS\_QueueRead以非阻塞模式从队列中读取消息，读完一条消息后就休眠50Tick，一旦任务无法从队列中读取消息，就退出循环，并通过LOS\_QueueDelete删除队列。
5.  通过LOS_TaskDelete删除send\_Entry和recv\_Entry两个任务。

**实例结果**

```
Kernel message queue demo begin.
Create the queue ok.
Recv message : test is message 0.
Recv message : test is message 1.
Recv message : test is message 2.
Recv message : test is message 3.
Recv message : test is message 4.
Recv message failed, error: 200061d.
Delete the queue ok.
Kernel message queue demo ok.
```

**事件实例：<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/kernel/api/los_api_event.c" target="_blank">los_api_event.c</a>/<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/kernel/api/los_api_event.h" target="_blank">los_api_event.h</a>**

事件是一种实现任务间通信的机制，可用于实现任务间的同步。一个任务可以等待多个事件的发生：可以是任意一个事件发生时唤醒任务进行事件处理，也可以是几个事件都发生后才唤醒任务进行事件处理。

本实例执行以下步骤：

1.  在Example\_SndRcvEvent中初始化事件控制块，并创建任务Example\_Event。任务Example\_Event的优先级高于Example\_SndRcvEvent函数所在任务。
2.  Example\_Event被调度，通过LOS\_EventRead读事件0x00000001，超时时间为100 Ticks，此时无法读取到该事件，Example\_Event阻塞挂起。
3.  Example\_SndRcvEvent被调度，通过LOS\_EventWrite写事件0x00000001。
4.  Example\_Event读取到事件0x00000001，被唤醒，任务结束执行。
5.  Example\_SndRcvEvent被调度，通过LOS\_EventClear清除事件类型，任务结束执行。

**实例结果**

```
Kernel event demo begin.
Example_Event wait event 0x1.
Example_SndRcvEvent write event.
Example_Event, read event : 0x1.
EventMask : 1
EventMask : 0
Kernel event demo ok.
```

**互斥锁实例：<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/kernel/api/los_api_mutex.c" target="_blank">los_api_mutex.c</a>/<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/kernel/api/los_api_mutex.h" target="_blank">los_api_mutex.h</a>**

互斥锁又称互斥型信号量，是一种特殊的二值性信号量，用于实现对共享资源的独占式处理。

本实例执行以下步骤：

1.  在Example\_MutexLock中创建一个互斥锁，锁任务调度，创建两个任务：低优先级任务Example\_MutexTask1和高优先级任务Example\_MutexTask2，解锁任务调度。
2.  Example\_MutexTask2被调度，通过LOS_MuxPend以永久阻塞模式申请互斥锁，此时互斥锁没有被其他任务持有，它成功申请到锁后，开始休眠100Tick，Example\_MutexTask2挂起。
3.  Example\_MutexTask1被调度，申请互斥锁，其申请等待时间为10Tick，因互斥锁仍被Example\_MutexTask2持有，Example\_MutexTask1挂起10Tick后再次被唤醒，因仍未拿到互斥锁，Example\_MutexTask1以永久阻塞模式申请互斥锁，因无法拿到锁，Example\_MutexTask1挂起。
4.  100Tick后Example\_MutexTask2休眠时间结束，其被唤醒后释放互斥锁，Example\_MutexTask2结束执行。
5.  因Example\_MutexTask2已经释放互斥锁，Example\_MutexTask1被唤醒后拿到锁，接着释放锁，Example\_MutexTask1结束执行。
6.  300Tick后Example\_MutexLock被调度运行并删除互斥锁。

**实例结果**

```
Kernel mutex demo begin.
Task2 try to get mutex, wait forever.
Task2 get mutex g_demoMux01 and suspend 100 ticks.
Task1 try to get mutex, wait 10 ticks.
Task1 timeout and try to get mutex, wait forever.
Task2 resumed and post the g_demoMux01.
Task1 wait forever, got mutex g_demoMux01 ok.
Kernel mutex demo ok.
```

**信号量实例：<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/kernel/api/los_api_sem.c" target="_blank">los_api_sem.c</a>/<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/kernel/api/los_api_sem.h" target="_blank">los_api_sem.h</a>**

信号量（Semaphore）是一种实现任务间通信的机制，实现任务之间同步或临界资源的互斥访问。常用于协助一组相互竞争的任务来访问临界资源。

本实例执行以下步骤：

1.  在Example\_Semphore中创建一个计数值为0的信号量，锁任务调度，创建两个任务：低优先级任务Example\_SemTask1和高优先级任务Example\_SemTask2，解锁任务调度。
2.  Example\_SemTask2被调度，通过LOS\_SemPend以永久阻塞模式申请信号量后阻塞挂起。
3.  Example\_SemTask1被调度，通过LOS\_SemPend以定时等待10Tick的方式申请信号量后阻塞挂起。
4.  Example\_Semphore被调度，执行LOS\_SemPost释放信号量。
5.  Example\_SemTask2得到信号量，被唤醒，然后休眠20Tick，Example\_MutexTask2挂起。
6.  Example\_SemTask1被调度，等待10Tick后仍未得到信号量，尝试以永久阻塞模式申请信号量，Example\_SemTask1挂起。
7.  Example\_Semphore被唤醒，开始休眠40Tick，Example\_Semphore挂起。
8.  20Tick的休眠结束后，Example\_SemTask2唤醒，释放信号量，Example\_SemTask2结束执行。
8.  Example\_SemTask1得到信号量，被唤醒后释放信号量，Example\_SemTask1结束执行。
9.  40Tick的休眠结束后，Example\_Semphore被唤醒，并通过LOS\_SemDelete删除信号量。

**实例结果**

```
Kernel semaphore demo begin.
Example_SemTask2 try get sem g_demoSemId wait forever.
Example_SemTask1 try get sem g_demoSemId, timeout 10 ticks.
Example_SemTask2 get sem g_demoSemId and then delay 20ticks.
Example_SemTask1 timeout and try get sem g_demoSemId wait forever.
Example_SemTask2 post sem g_demoSemId.
Example_SemTask1 wait_forever and got sem g_demoSemId ok.
Kernel semaphore demo ok.
```

<h3 id="e">2.5 时间管理</h3>

时间管理以系统时钟为基础。时间管理提供给应用程序所有和时间有关的服务。下面包括时钟管理和软件定时器两个实例。

**时钟管理实例：<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/kernel/api/los_api_systick.c" target="_blank">los_api_systick.c</a>/<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/kernel/api/los_api_systick.h" target="_blank">los_api_systick.h</a>**

本实例执行以下步骤：

1.  通过LOS\_CyclePerTickGet统计一个Tick的Cycle数。
2.  通过LOS\_TickCountGet获取自系统启动以来的Tick数，然后休眠200Tick后，再次调用LOS\_TickCountGet。将这两次获取的Tick数相减，其差应该大于等于休眠的200Tick。

**实例结果**

```
Kernel systick demo begin.
LOS_CyclePerTickGet = 180000.
LOS_TickCountGet = 8987.
LOS_TickCountGet after delay = 9189.
Kernel systick demo ok.
```

**软件定时器实例：<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/kernel/api/los_api_timer.c" target="_blank">los_api_timer.c</a>/<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/kernel/api/los_api_timer.h" target="_blank">los_api_timer.h</a>**

软件定时器，是基于系统Tick时钟中断且由软件来模拟的定时器，当经过设定的Tick时钟计数后会触发用户定义的回调函数。定时精度与系统Tick时钟的周期有关。

本实例执行以下步骤：

1.  通过LOS\_SwtmrCreate创建两个定时器，定时1000Ticks的单次软件定时器Timer1和定时100Tick的周期软件定时器Timer2。
2.  通过LOS\_SwtmrStart启动Timer1。
3.  200Tick后通过LOS\_SwtmrStop停止Timer1，再通过LOS\_SwtmrStart启动Timer1。
4.  再经过1000Ticks后通过LOS\_SwtmrDelete删除Timer1（显示删除失败，此时Timer1定时器已经完成1000Ticks的定时，对于单次定时器，定时时间到达后系统会自动删除该定时器）。
5.  通过LOS\_SwtmrStart启动Timer2。
6.  休眠1000Ticks，在此期间，Timer2会触发10次回调函数。1000Ticks后停止并删除Timer2，任务结束执行。

**实例结果**

```
Kernel swtimer demo begin.
Create Timer1 ok.
Create Timer2 ok.
Start Timer1 ok.
Stop Timer1 ok.
LOS_TickCountGet g_demoTimerCount1 = 1.
LOS_TickCountGet tickLast1 = 11402.
Delete Timer1 failed.
Start Timer2 ok.
LOS_TickCountGet g_demoTimerCount2 = 1.
LOS_TickCountGet tickLast2 = 11510.
LOS_TickCountGet g_demoTimerCount2 = 2.
LOS_TickCountGet tickLast2 = 11610.
LOS_TickCountGet g_demoTimerCount2 = 3.
LOS_TickCountGet tickLast2 = 11710.
LOS_TickCountGet g_demoTimerCount2 = 4.
LOS_TickCountGet tickLast2 = 11810.
LOS_TickCountGet g_demoTimerCount2 = 5.
LOS_TickCountGet tickLast2 = 11910.
LOS_TickCountGet g_demoTimerCount2 = 6.
LOS_TickCountGet tickLast2 = 12010.
LOS_TickCountGet g_demoTimerCount2 = 7.
LOS_TickCountGet tickLast2 = 12110.
LOS_TickCountGet g_demoTimerCount2 = 8.
LOS_TickCountGet tickLast2 = 12210.
LOS_TickCountGet g_demoTimerCount2 = 9.
LOS_TickCountGet tickLast2 = 12310.
LOS_TickCountGet g_demoTimerCount2 = 10.
LOS_TickCountGet tickLast2 = 12410.
Kernel timer demo ok.
```

<h3 id="f">2.6 双向链表</h3>

**双向链表实例：<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/kernel/api/los_api_list.c" target="_blank">los_api_list.c</a>/<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/kernel/api/los_api_list.h" target="_blank">los_api_list.h</a>**

双向链表是指含有往前和往后两个方向的链表，即每个结点中除存放下一个节点指针外，还增加一个指向其前一个节点的指针。其头指针head是唯一确定的。

本实例执行以下步骤：

1.  调用LOS\_MemAlloc生成链表的头指针。
2.  调用LOS\_ListInit初始化双向链表。
3.  调用LOS\_ListEmpty判断链表初始化是否成功。
4.  调用LOS\_MemAlloc生成两个中间节点和一个尾节点。
5.  调用LOS\_ListAdd在头指针之后插入上面的两个中间节点。
6.  调用LOS\_ListTailInsert插入尾节点。
7.  调用LOS\_ListDelete删除其中一个节点。

**实例结果**

```
Kernel list demo begin.
Init list......
Node add and tail add......
Add node ok.
Add tail ok.
Delete node......
Delete node ok.
Kernel list demo ok.
```

<h2 id="3">3.运行实例</h2>

在target文件夹下的main.c或者user_task.c中找到app_init函数。LiteOS系统完成初始化后，会创建用户任务，其任务处理函数就是app_init(), 用户可以直接在该函数中运行<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/kernel" target="_blank">demos/kernel</a>下的各个实例。

这里提供两种方式来运行[2.实例介绍](#2)中的各个实例。

### 实例1：<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/kernel/api/los_inspect_entry.c" target="_blank">los_inspect_entry.c</a>/<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/kernel/api/los_inspect_entry.h" target="_blank">los_inspect_entry.h</a>

**开发流程**

1.使用make menuconfig打开宏开关：

`Demos --> Kernel Demo --> Enable Kernel Demo --> Kernel Demo Entry --> InspectEntry`

2.在实现app_init()的源文件中添加头文件引用：
```c
#include "los_inspect_entry.h"  // 调用测试函数头文件
```
3.在app_init()中调用KernelDemoInspectEntry()函数，该函数会执行所有内核实例：
```c
KernelDemoInspectEntry();    // 调用测试任务测试所有实例
```

### 实例2：<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/kernel/api/los_demo_entry.c" target="_blank">los_demo_entry.c</a>/<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/kernel/api/los_demo_entry.h" target="_blank">los_demo_entry.h</a>

通过宏开关来判断是否调用各内核实例，以实现对[2.实例介绍](#2)中某个或多个实例的执行。

**开发流程**

1.使用make menuconfig打开需要测试功能的宏开关，例如要测试task实例，则选择“DemoEntry”, 并选择实例：

`Demos --> Kernel Demo --> Enable Kernel Demo --> Kernel Demo Entry --> DemoEntry --> Run Kernel Task Demo`

2.在实现app_init()的源文件中添加头文件引用：

```c
#include "los_demo_entry.h" // 调用测试函数头文件
```

3.在app_init()中调用KernelDemoEntry()函数，该函数通过宏开关来判断是否调用某内核实例：

```c
KernelDemoEntry();   // 调用测试任务
```

**测试流程**

用户可以按照<a href="https://liteos.gitee.io/liteos_studio/#/project_stm32" target="_blank">LiteOS Studio STM32工程示例</a>对代码进行编译烧写测试。
