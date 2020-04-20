# Huawei LiteOS 开发指南

## 目 录

* [1.知识共享许可协议说明](#1)
* [2.前言](#2)
* [3.概述](#3)
* [4.基础内核](#4)
* [4.1.任务](#5)
* [4.2.内存](#6)
* [4.3.中断机制](#7)
* [4.4.队列](#8)
* [4.5.事件](#9)
* [4.6.互斥锁](#10)
* [4.7.信号量](#11)
* [4.8.时间管理](#12)
* [4.9.软件定时器](#13)
* [4.10.错误处理](#14)
* [4.11.双向链表](#15)



<h2 id="1">1.知识共享许可协议说明</h2>

**您可以自由地：**  

**分享** 在任何媒介以任何形式复制、发行本文档

**演绎** 修改、转换或以本文档为基础进行创作

只要你遵守许可协议条款，许可人就无法收回你的这些权利


**惟须遵守下列条件：**  

**署名** 您必须提供适当的证书，提供一个链接到许可证，并指示是否作出更改。您可以以任何合理的方式这样做，但不是以任何方式表明，许可方赞同您或您的使用。

**非商业性使用** 您不得将本文档用于商业目的。

**相同方式共享** 如果您的修改、转换，或以本文档为基础进行创作，仅得依本素材的授权条款来散布您的贡献作品。

**没有附加限制** 您不能增设法律条款或科技措施，来限制别人依授权条款本已许可的作为。


**声明：**  

当您使用本素材中属于公众领域的元素，或当法律有例外或限制条款允许您的使用，则您不需要遵守本授权条款。

未提供保证。本授权条款未必能完全提供您预期用途所需要的所有许可。例如：形象权、隐私权、著作人格权等其他权利，可能限制您如何使用本素材。

>![](public_sys-resources/icon-notice.gif) **须知：**   
>为了方便用户理解，这是协议的概述。可以访问网址<https://creativecommons.org/licenses/by-nc-sa/3.0/legalcode>了解完整协议内容。  

<h2 id="2">2.前言</h2>

**目的**

本文档介绍Huawei LiteOS的体系结构，并介绍如何进行开发和调试。

**读者对象**

本文档主要适用于Huawei LiteOS的开发者，主要适用于以下对象：

- 物联网端侧软件开发工程师
- 物联网架构设计师

**符号约定**

在本文中可能出现下列标志，它们所代表的含义如下。  

| 符号 | 说明 |  
| - | :- |  
|![](./meta/DevGuide/icon_danger.png)| 用于警示紧急的危险情形，若不避免，将会导致人员死亡或严重的人身伤害 |                                                       
|![](./meta/DevGuide/icon_warning.png)| 用于警示潜在的危险情形，若不避免，可能会导致人员死亡或严重的人身伤害 |
|![](./meta/DevGuide/icon_careful.png)| 用于警示潜在的危险情形，若不避免，可能会导致中度或轻微的人身伤害 |
|![](./meta/DevGuide/icon_notice.png)| 用于传递设备或环境安全警示信息，若不避免，可能会导致设备损坏、数据丢失、设备性能降低或其它不可预知的结果，“注意”不涉及人身伤害 |
|![](./meta/DevGuide/icon_note.png)| “说明”不是安全警示信息，不涉及人身、设备及环境伤害信息 |




<h2 id="3">3.概述</h2>

Huawei LiteOS 是华为面向物联网领域开发的一个基于实时内核的轻量级操作系统。基础内核支持任务管理、内存管理、时间管理、通信机制、中断管理、队列管理、事件管理、定时器等操作系统基础组件，更好地支持低功耗场景，支持 Tickless 机制，支持定时器对齐。

Huawei LiteOS 自开源社区发布以来，围绕 NB-IoT 物联网市场从技术、生态、解决方案、商用支持等多维度使能合作伙伴，构建开源的物联网生态,目前已经聚合了 50+ MCU 和解决方案合作伙伴，共同推出一批开源开发套件和行业解决方案，帮助众多行业客户快速的推出物联网终端和服务，客户涵盖抄表、停车、路灯、环保、共享单车、物流等众多行业，为开发者提供 “一站式” 完整软件平台，有效降低开发门槛、缩短开发周期。

### 背景介绍  

Huawei LiteOS是轻量级的实时操作系统，是华为IoT OS。

**图 1**  Huawei LiteOS Kernel的基本框架图<a name="f1eee3c6e1dea487dbd09c754ac1f0383"></a>  
![](figures/Huawei-LiteOS-Kernel的基本框架图.png "Huawei-LiteOS-Kernel的基本框架图")

Huawei LiteOS基础内核是最精简的Huawei LiteOS操作系统代码，包括任务管理、内存管理、时间管理、通信机制、中断管理、队列管理、事件管理、定时器等操作系统基础组件，可以单独运行。

## Huawei LiteOS Kernel的优势<a name="s292bc5c8669a44a99328cbed227b8c8e"></a>

-   高实时性，高稳定性。
-   超小内核，基础内核体积可以裁剪至不到10K。
-   低功耗。
-   支持功能静态裁剪。

## 各模块简介<a name="s49c1136c35e44a8099975fe74a623b43"></a>

**任务**

提供任务的创建、删除、延迟、挂起、恢复等功能，以及锁定和解锁任务调度。支持任务按优先级高低的抢占调度及同优先级时间片轮转调度。

**任务同步**

-   信号量：支持信号量的创建、删除、申请和释放等功能。
-   互斥锁：支持互斥锁的创建、删除、申请和释放等功能。

**硬件相关**

提供中断、定时器等功能。

-   中断：提供中断的创建、删除、使能、禁止、请求位的清除等功能。
-   定时器：提供定时器的创建、删除、启动、停止等功能。

**IPC通信**

提供事件、消息队列功能。

-   事件：支持读事件和写事件功能。
-   消息队列：支持消息队列的创建、删除、发送和接收功能。

**时间管理**

-   系统时间：系统时间是由定时/计数器产生的输出脉冲触发中断而产生的。
-   Tick时间：Tick是操作系统调度的基本时间单位，对应的时长由系统主频及每秒Tick数决定，由用户配置。
-   软件定时器：以Tick为单位的定时器功能，软件定时器的超时处理函数在系统创建的Tick软中断中被调用。

**内存管理**

-   提供静态内存和动态内存两种算法，支持内存申请、释放。目前支持的内存管理算法有固定大小的BOX算法、动态申请SLAB、DLINK算法。
-   提供内存统计、内存越界检测功能。

### 支持的核

**表 1**  Huawei LiteOS开源Kernel支持的核

<a name="table37599927114623"></a>
<table><thead align="left"><tr id="row9045213114623"><th class="cellrowborder" valign="top" width="50%" id="mcps1.2.3.1.1"><p id="p17292119114623"><a name="p17292119114623"></a><a name="p17292119114623"></a>支持的核</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.2.3.1.2"><p id="p58484359114623"><a name="p58484359114623"></a><a name="p58484359114623"></a>芯片</p>
</th>
</tr>
</thead>
<tbody><tr id="row841110219312"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p18412152119315"><a name="p18412152119315"></a><a name="p18412152119315"></a>Cortex-A7</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p9338152362319"><a name="p9338152362319"></a><a name="p9338152362319"></a>Hi3516A <span>V300</span></p>
<p id="p8162927142311"><a name="p8162927142311"></a><a name="p8162927142311"></a><span>Hi3516C V500</span></p>
<p id="p48257298238"><a name="p48257298238"></a><a name="p48257298238"></a>Hi3516E V200</p>
<p id="p6641932122310"><a name="p6641932122310"></a><a name="p6641932122310"></a>Hi3516E V300</p>
<p id="p2352203442320"><a name="p2352203442320"></a><a name="p2352203442320"></a><span>Hi3516D V200</span></p>
<p id="p10601181132411"><a name="p10601181132411"></a><a name="p10601181132411"></a><span>Hi3516D V300</span></p>
<p id="p6412421163111"><a name="p6412421163111"></a><a name="p6412421163111"></a><span>Hi3518E V300</span></p>
<p id="p14289544257"><a name="p14289544257"></a><a name="p14289544257"></a>Hi3536</p>
<p id="p6914470237"><a name="p6914470237"></a><a name="p6914470237"></a>Hi3556 V100</p>
<p id="p982105212304"><a name="p982105212304"></a><a name="p982105212304"></a>Hi3556 V200</p>
<p id="p17638618183117"><a name="p17638618183117"></a><a name="p17638618183117"></a>Hi3559 V200</p>
<p id="p2942652410"><a name="p2942652410"></a><a name="p2942652410"></a><span>Hi3716M V430</span></p>
</td>
</tr>
<tr id="row2957459114010"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p1595825984019"><a name="p1595825984019"></a><a name="p1595825984019"></a>Cortex-A17</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p16766133242919"><a name="p16766133242919"></a><a name="p16766133242919"></a>Hi3536</p>
<p id="p1795895914018"><a name="p1795895914018"></a><a name="p1795895914018"></a>Hi3556 V100</p>
<p id="p13430123973118"><a name="p13430123973118"></a><a name="p13430123973118"></a><span>Hi3559 V100</span></p>
</td>
</tr>
<tr id="row6413839415"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p14132318416"><a name="p14132318416"></a><a name="p14132318416"></a>Cortex-A53</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p1262994312222"><a name="p1262994312222"></a><a name="p1262994312222"></a><span>Hi3519A V100</span></p>
<p id="p11165612298"><a name="p11165612298"></a><a name="p11165612298"></a>Hi3556A V100</p>
<p id="p5770724156"><a name="p5770724156"></a><a name="p5770724156"></a>Hi3559A V100</p>
<p id="p10371947185813"><a name="p10371947185813"></a><a name="p10371947185813"></a><span>Hi3796M V200</span></p>
<p id="p237844913527"><a name="p237844913527"></a><a name="p237844913527"></a><span>Hi3798C V200</span></p>
<p id="p17148162595714"><a name="p17148162595714"></a><a name="p17148162595714"></a><span>Hi3798M V200</span></p>
<p id="p108688417576"><a name="p108688417576"></a><a name="p108688417576"></a><span>Hi3798M V300</span></p>
<p id="p1941214115815"><a name="p1941214115815"></a><a name="p1941214115815"></a><span>Hi3798M V310</span></p>
</td>
</tr>
<tr id="row56597184114623"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p20969186114623"><a name="p20969186114623"></a><a name="p20969186114623"></a>Cortex-M0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p49775295174334"><a name="p49775295174334"></a><a name="p49775295174334"></a>STM32L053R8Tx</p>
<p id="p20782492114623"><a name="p20782492114623"></a><a name="p20782492114623"></a>ATSAMD21G18A</p>
<p id="p56050741173553"><a name="p56050741173553"></a><a name="p56050741173553"></a>ATSAMD21J18A</p>
<p id="p34694625173553"><a name="p34694625173553"></a><a name="p34694625173553"></a>ATSAMR21G18A</p>
<p id="p43816176173553"><a name="p43816176173553"></a><a name="p43816176173553"></a>EFM32HG322F64</p>
<p id="p58801265173553"><a name="p58801265173553"></a><a name="p58801265173553"></a>MKL26Z128</p>
<p id="p59449343173553"><a name="p59449343173553"></a><a name="p59449343173553"></a>MKW41Z512</p>
<p id="p65282042173553"><a name="p65282042173553"></a><a name="p65282042173553"></a>LPC824M201JHI33</p>
<p id="p50667467173553"><a name="p50667467173553"></a><a name="p50667467173553"></a>MM32L073PF</p>
<p id="p53354019173553"><a name="p53354019173553"></a><a name="p53354019173553"></a>nRF51822</p>
<p id="p10424123173553"><a name="p10424123173553"></a><a name="p10424123173553"></a>NANO130KE3BN</p>
</td>
</tr>
<tr id="row52824706114623"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p50942804114623"><a name="p50942804114623"></a><a name="p50942804114623"></a>Cortex-M3</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p13868419317"><a name="p13868419317"></a><a name="p13868419317"></a>K3V3，K3V3+</p>
<p id="p62774815174312"><a name="p62774815174312"></a><a name="p62774815174312"></a>STM32F103RB</p>
<p id="p32296422173629"><a name="p32296422173629"></a><a name="p32296422173629"></a>ATSAM4SD32C</p>
<p id="p22232342173629"><a name="p22232342173629"></a><a name="p22232342173629"></a>EFM32GG990F1024</p>
<p id="p65873350173629"><a name="p65873350173629"></a><a name="p65873350173629"></a>GD32F103VCT6</p>
<p id="p55989239173629"><a name="p55989239173629"></a><a name="p55989239173629"></a>GD32150R8</p>
<p id="p34141106173629"><a name="p34141106173629"></a><a name="p34141106173629"></a>GD32F190R8</p>
<p id="p38834505173629"><a name="p38834505173629"></a><a name="p38834505173629"></a>GD32F207VC</p>
<p id="p13966227173629"><a name="p13966227173629"></a><a name="p13966227173629"></a>MM32F103CBT6</p>
<p id="p58587187173629"><a name="p58587187173629"></a><a name="p58587187173629"></a>MM32L373PS</p>
</td>
</tr>
<tr id="row26102894114623"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p33959707114623"><a name="p33959707114623"></a><a name="p33959707114623"></a>Cortex-M4</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p4109259617442"><a name="p4109259617442"></a><a name="p4109259617442"></a>STM32F411RE</p>
<p id="p3428904817442"><a name="p3428904817442"></a><a name="p3428904817442"></a>STM32F412ZG</p>
<p id="p4016598017442"><a name="p4016598017442"></a><a name="p4016598017442"></a>STM32F429ZI</p>
<p id="p9969113720355"><a name="p9969113720355"></a><a name="p9969113720355"></a>STM32F429IG</p>
<p id="p2594950517442"><a name="p2594950517442"></a><a name="p2594950517442"></a>STM32L476RG</p>
<p id="p5987090317442"><a name="p5987090317442"></a><a name="p5987090317442"></a>EFM32PG1B200F256GM48</p>
<p id="p196722317442"><a name="p196722317442"></a><a name="p196722317442"></a>GD32F450IK</p>
<p id="p1770500717442"><a name="p1770500717442"></a><a name="p1770500717442"></a>CC3220SF</p>
<p id="p2512733717442"><a name="p2512733717442"></a><a name="p2512733717442"></a>LPC54114j256BD64</p>
<p id="p2481944717442"><a name="p2481944717442"></a><a name="p2481944717442"></a>nRF52840</p>
<p id="p2204843217442"><a name="p2204843217442"></a><a name="p2204843217442"></a>nRF52832</p>
<p id="p6421816717442"><a name="p6421816717442"></a><a name="p6421816717442"></a>NUC472HI8AE</p>
<p id="p3221896117442"><a name="p3221896117442"></a><a name="p3221896117442"></a>ATSAMG55J19</p>
<p id="p2153519517442"><a name="p2153519517442"></a><a name="p2153519517442"></a>ADuCM4050LF</p>
</td>
</tr>
<tr id="row60564714114623"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p6794795114623"><a name="p6794795114623"></a><a name="p6794795114623"></a>Cortex-M7</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p20644356163118"><a name="p20644356163118"></a><a name="p20644356163118"></a>K3V5</p>
<p id="p3073032017457"><a name="p3073032017457"></a><a name="p3073032017457"></a>STM32F746ZG</p>
<p id="p6199753217452"><a name="p6199753217452"></a><a name="p6199753217452"></a>ATSAME70Q21</p>
</td>
</tr>
<tr id="row790374417396"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p1490134412399"><a name="p1490134412399"></a><a name="p1490134412399"></a>ARM9</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p91544336454"><a name="p91544336454"></a><a name="p91544336454"></a>Hi3516C V200</p>
<p id="p5431732"><a name="p5431732"></a><a name="p5431732"></a>Hi3516C V300</p>
<p id="p636720265519"><a name="p636720265519"></a><a name="p636720265519"></a>Hi3516E V100</p>
<p id="p1901744143919"><a name="p1901744143919"></a><a name="p1901744143919"></a>Hi3518E V200</p>
<p id="p11272103131012"><a name="p11272103131012"></a><a name="p11272103131012"></a>Hi3911</p>
</td>
</tr>
</tbody>
</table>

### 使用约束

-   Huawei LiteOS提供一套Huawei LiteOS接口，同时支持CMSIS接口，它们功能一致，但混用CMSIS和Huawei LiteOS接口可能会导致不可预知的错误,例如用CMSIS接口申请信号量，但用Huawei LiteOS接口释放信号量。
-   开发驱动程序只能用Huawei LiteOS的接口，上层APP建议用CMSIS接口。

<h2 id="4">4.基础内核</h2>

<h3 id="5">4.1 任务</h3>

### 概述

#### 基本概念

从系统的角度看，任务是竞争系统资源的最小运行单元。任务可以使用或等待CPU、使用内存空间等系统资源，并独立于其它任务运行。  
Huawei LiteOS的任务模块可以给用户提供多个任务，实现了任务之间的切换和通信，帮助用户管理业务程序流程。这样用户可以将更多的精力投入到业务功能的实现中。  
Huawei LiteOS是一个支持多任务的操作系统。在Huawei LiteOS中，一个任务表示一个线程。  
Huawei LiteOS中的任务是抢占式调度机制，同时支持时间片轮转调度方式。  
高优先级的任务可打断低优先级任务，低优先级任务必须在高优先级任务阻塞或结束后才能得到调度。  
Huawei LiteOS的任务一共有32个优先级(0-31)，最高优先级为0，最低优先级为31。  

#### 任务相关概念

**任务状态**

Huawei LiteOS系统中的每一任务都有多种运行状态。系统初始化完成后，创建的任务就可以在系统中竞争一定的资源，由内核进行调度。

任务状态通常分为以下四种：

-   就绪（Ready）：该任务在就绪列表中，只等待CPU。
-   运行（Running）：该任务正在执行。
-   阻塞（Blocked）：该任务不在就绪列表中。包含任务被挂起、任务被延时、任务正在等待信号量、读写队列或者等待读写事件等。
-   退出态（Dead）：该任务运行结束，等待系统回收资源。

**图 1**  任务状态示意图<a name="f7956ae6389594244b0e3132e1737e75d"></a>  
![](figures/任务状态示意图.png "任务状态示意图")

任务状态迁移说明：

-   就绪态→运行态：

任务创建后进入就绪态，发生任务切换时，就绪列表中最高优先级的任务被执行，从而进入运行态，但此刻该任务依旧在就绪列表中。

-   运行态→阻塞态：

正在运行的任务发生阻塞（挂起、延时、获取互斥锁、读消息、读信号量等待等）时，该任务会从就绪列表中删除，任务状态由运行态变成阻塞态，然后发生任务切换，运行就绪列表中剩余最高优先级任务。

-   阻塞态→就绪态（阻塞态→运行态）：

阻塞的任务被恢复后（任务恢复、延时时间超时、读信号量超时或读到信号量等），此时被恢复的任务会被加入就绪列表，从而由阻塞态变成就绪态；此时如果被恢复任务的优先级高于正在运行任务的优先级，则会发生任务切换，将该任务由就绪态变成运行态。

-   就绪态→阻塞态：

任务也有可能在就绪态时被阻塞（挂起），此时任务状态会由就绪态转变为阻塞态，该任务从就绪列表中删除，不会参与任务调度，直到该任务被恢复。

-   运行态→就绪态：

有更高优先级任务创建或者恢复后，会发生任务调度，此刻就绪列表中最高优先级任务变为运行态，那么原先运行的任务由运行态变为就绪态，依然在就绪列表中。

-   运行态→退出态

运行中的任务运行结束，任务状态由运行态变为退出态。退出态包含任务运行结束的正常退出以及Invalid状态。例如，未设置分离属性（LOS\_TASK\_STATUS\_DETACHED）的任务，运行结束后对外呈现的是Invalid状态，即退出态。

-   阻塞态→退出态

阻塞的任务调用删除接口，任务状态由阻塞态变为退出态。

**任务ID**

任务ID，在任务创建时通过参数返回给用户，作为任务的一个非常重要的标识。用户可以通过任务ID对指定任务进行任务挂起、任务恢复、查询任务名等操作。

**任务优先级**

优先级表示任务执行的优先顺序。任务的优先级决定了在发生任务切换时即将要执行的任务。在就绪列表中的最高优先级的任务将得到执行。

**任务入口函数**

每个新任务得到调度后将执行的函数。该函数由用户实现，在任务创建时，通过任务创建结构体指定。

**任务控制块TCB**

每一个任务都含有一个任务控制块\(TCB\)。TCB包含了任务上下文栈指针（stack pointer）、任务状态、任务优先级、任务ID、任务名、任务栈大小等信息。TCB可以反映出每个任务运行情况。

**任务栈**

每一个任务都拥有一个独立的栈空间，我们称为任务栈。栈空间里保存的信息包含局部变量、寄存器、函数参数、函数返回地址等。任务在任务切换时会将切出任务的上下文信息保存在自身的任务栈空间里面，以便任务恢复时还原现场，从而在任务恢复后在切出点继续开始执行。

**任务上下文**

任务在运行过程中使用到的一些资源，如寄存器等，我们称为任务上下文。当这个任务挂起时，其他任务继续执行，在任务恢复后，如果没有把任务上下文保存下来，有可能任务切换会修改寄存器中的值，从而导致未知错误。

因此，Huawei LiteOS在任务挂起的时候会将本任务的任务上下文信息，保存在自己的任务栈里面，以便任务恢复后，从栈空间中恢复挂起时的上下文信息，从而继续执行被挂起时被打断的代码。

**任务切换**

任务切换包含获取就绪列表中最高优先级任务、切出任务上下文保存、切入任务上下文恢复等动作。

#### 运作机制

Huawei LiteOS任务管理模块提供任务创建、任务延时、任务挂起和任务恢复、锁任务调度和解锁任务调度、根据ID查询任务控制块信息功能。

用户创建任务时，系统会将任务栈进行初始化，预置上下文。此外，系统还会将“任务入口函数”地址放在相应位置。这样在任务第一次启动进入运行态时，将会执行“任务入口函数”。

### 开发指导

#### 使用场景

任务创建后，内核可以执行锁任务调度，解锁任务调度，挂起，恢复，延时等操作，同时也可以设置任务优先级，获取任务优先级。任务结束的时候，如果任务的状态是自删除状态（LOS\_TASK\_STATUS\_DETACHED），则进行当前任务自删除操作。

#### 功能

Huawei LiteOS 系统中的任务管理模块为用户提供下面几种功能。

<a name="t394bb3cef56b434484d91fc796c1c503"></a>
<table><thead align="left"><tr id="r18cee586a218484c91d6fcfd920c2f12"><th class="cellrowborder" valign="top" width="26.75%" id="mcps1.1.4.1.1"><p id="af9fc9b8177ad4927b79003e9ebf188d5"><a name="af9fc9b8177ad4927b79003e9ebf188d5"></a><a name="af9fc9b8177ad4927b79003e9ebf188d5"></a>功能分类</p>
</th>
<th class="cellrowborder" valign="top" width="27.13%" id="mcps1.1.4.1.2"><p id="afab4dc3a4ffd4781af12d9c0d0467973"><a name="afab4dc3a4ffd4781af12d9c0d0467973"></a><a name="afab4dc3a4ffd4781af12d9c0d0467973"></a>接口名</p>
</th>
<th class="cellrowborder" valign="top" width="46.12%" id="mcps1.1.4.1.3"><p id="a8ab55588a3784d98a6993d4c6561c358"><a name="a8ab55588a3784d98a6993d4c6561c358"></a><a name="a8ab55588a3784d98a6993d4c6561c358"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="r4cc9e698b01f40f3bdbac5e85bd69a7a"><td class="cellrowborder" rowspan="3" valign="top" width="26.75%" headers="mcps1.1.4.1.1 "><p id="aec257d2dbcb34a59a256213bdb39e4d2"><a name="aec257d2dbcb34a59a256213bdb39e4d2"></a><a name="aec257d2dbcb34a59a256213bdb39e4d2"></a>任务的创建和删除</p>
</td>
<td class="cellrowborder" valign="top" width="27.13%" headers="mcps1.1.4.1.2 "><p id="aa6af849217dc40ebbde6ac9a426e66cf"><a name="aa6af849217dc40ebbde6ac9a426e66cf"></a><a name="aa6af849217dc40ebbde6ac9a426e66cf"></a>LOS_TaskCreateOnly</p>
</td>
<td class="cellrowborder" valign="top" width="46.12%" headers="mcps1.1.4.1.3 "><p id="abfb5d49695ad4df687c4343530265524"><a name="abfb5d49695ad4df687c4343530265524"></a><a name="abfb5d49695ad4df687c4343530265524"></a>创建任务，并使该任务进入suspend状态，并不调度。</p>
</td>
</tr>
<tr id="rc8177f6f073e40ec8b06774a39c85cbc"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="a3f642bb7fd474e17bb5c4008302c0ebb"><a name="a3f642bb7fd474e17bb5c4008302c0ebb"></a><a name="a3f642bb7fd474e17bb5c4008302c0ebb"></a>LOS_TaskCreate</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="a84566c93f1cd49148a68b2875b85337e"><a name="a84566c93f1cd49148a68b2875b85337e"></a><a name="a84566c93f1cd49148a68b2875b85337e"></a>创建任务，并使该任务进入ready状态，并调度。</p>
</td>
</tr>
<tr id="r946ee5c4946448a4963453f1467e8c6e"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="a9b4de63140ed4666a4ea2e294c3f9ff1"><a name="a9b4de63140ed4666a4ea2e294c3f9ff1"></a><a name="a9b4de63140ed4666a4ea2e294c3f9ff1"></a>LOS_TaskDelete</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="ae69792e760fc4c8a97686ddbca3a23a9"><a name="ae69792e760fc4c8a97686ddbca3a23a9"></a><a name="ae69792e760fc4c8a97686ddbca3a23a9"></a>删除指定的任务。</p>
</td>
</tr>
<tr id="r7e4bbea392e042fda03cf8ae312f13a5"><td class="cellrowborder" rowspan="4" valign="top" width="26.75%" headers="mcps1.1.4.1.1 "><p id="a38a7e95dcf994ccbbee3beaa80a2d1f0"><a name="a38a7e95dcf994ccbbee3beaa80a2d1f0"></a><a name="a38a7e95dcf994ccbbee3beaa80a2d1f0"></a>任务状态控制</p>
</td>
<td class="cellrowborder" valign="top" width="27.13%" headers="mcps1.1.4.1.2 "><p id="af606bee26454465fbd54a007d7f3e923"><a name="af606bee26454465fbd54a007d7f3e923"></a><a name="af606bee26454465fbd54a007d7f3e923"></a>LOS_TaskResume</p>
</td>
<td class="cellrowborder" valign="top" width="46.12%" headers="mcps1.1.4.1.3 "><p id="a1a8639fda4c5452394a7e9aa716fa633"><a name="a1a8639fda4c5452394a7e9aa716fa633"></a><a name="a1a8639fda4c5452394a7e9aa716fa633"></a>恢复挂起的任务。</p>
</td>
</tr>
<tr id="re3cd161d35f34ba3b3a06375011c2ebc"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="a43cbccf4da04459093f90f3bbf9d5da4"><a name="a43cbccf4da04459093f90f3bbf9d5da4"></a><a name="a43cbccf4da04459093f90f3bbf9d5da4"></a>LOS_TaskSuspend</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="a196e1e58aa914291baa7d7755d2b20a0"><a name="a196e1e58aa914291baa7d7755d2b20a0"></a><a name="a196e1e58aa914291baa7d7755d2b20a0"></a>挂起指定的任务。</p>
</td>
</tr>
<tr id="r5deb9247550c4ff99ba09c93a0145122"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="a87db4cc4046b449ab7a4adcbc123af1b"><a name="a87db4cc4046b449ab7a4adcbc123af1b"></a><a name="a87db4cc4046b449ab7a4adcbc123af1b"></a>LOS_TaskDelay</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="a74db5b95242444ff9b9a29ec6c861f3b"><a name="a74db5b95242444ff9b9a29ec6c861f3b"></a><a name="a74db5b95242444ff9b9a29ec6c861f3b"></a>任务延时等待。</p>
</td>
</tr>
<tr id="r6895d936983c4425854bf90b181de00a"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="a98a714a8e0944253aa4502238b276e9c"><a name="a98a714a8e0944253aa4502238b276e9c"></a><a name="a98a714a8e0944253aa4502238b276e9c"></a>LOS_TaskYield</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="add59cdbd7873421e8fd62e6dd222c7d9"><a name="add59cdbd7873421e8fd62e6dd222c7d9"></a><a name="add59cdbd7873421e8fd62e6dd222c7d9"></a>显式放权，调整指定优先级的任务调度顺序。</p>
</td>
</tr>
<tr id="r78cd4534b3784d12bb6445529df48f92"><td class="cellrowborder" rowspan="2" valign="top" width="26.75%" headers="mcps1.1.4.1.1 "><p id="abc0d9e62f7084ddd93debb62ed15e49a"><a name="abc0d9e62f7084ddd93debb62ed15e49a"></a><a name="abc0d9e62f7084ddd93debb62ed15e49a"></a>任务调度的控制</p>
</td>
<td class="cellrowborder" valign="top" width="27.13%" headers="mcps1.1.4.1.2 "><p id="a791905821bda4d458bdf7700df83b3da"><a name="a791905821bda4d458bdf7700df83b3da"></a><a name="a791905821bda4d458bdf7700df83b3da"></a>LOS_TaskLock</p>
</td>
<td class="cellrowborder" valign="top" width="46.12%" headers="mcps1.1.4.1.3 "><p id="a7eaefff1d8b64fd69b2ef4352c958116"><a name="a7eaefff1d8b64fd69b2ef4352c958116"></a><a name="a7eaefff1d8b64fd69b2ef4352c958116"></a>锁任务调度。</p>
</td>
</tr>
<tr id="ra3fe96c4732641a395b23217861a2973"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="ac1d325bbc0b8499d8062aa6b96538e9c"><a name="ac1d325bbc0b8499d8062aa6b96538e9c"></a><a name="ac1d325bbc0b8499d8062aa6b96538e9c"></a>LOS_TaskUnlock</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="a28e478dfc5d541ecb92cfb561c27ff7a"><a name="a28e478dfc5d541ecb92cfb561c27ff7a"></a><a name="a28e478dfc5d541ecb92cfb561c27ff7a"></a>解锁任务调度。</p>
</td>
</tr>
<tr id="r8e77b14d09a7435ba16a4f0c9f9b2e72"><td class="cellrowborder" rowspan="3" valign="top" width="26.75%" headers="mcps1.1.4.1.1 "><p id="zh-cn_topic_0015795099_p440345141415"><a name="zh-cn_topic_0015795099_p440345141415"></a><a name="zh-cn_topic_0015795099_p440345141415"></a>任务优先级的控制</p>
</td>
<td class="cellrowborder" valign="top" width="27.13%" headers="mcps1.1.4.1.2 "><p id="af815699dabf94420b004cae7dc5636ef"><a name="af815699dabf94420b004cae7dc5636ef"></a><a name="af815699dabf94420b004cae7dc5636ef"></a>LOS_CurTaskPriSet</p>
</td>
<td class="cellrowborder" valign="top" width="46.12%" headers="mcps1.1.4.1.3 "><p id="a707eec98f3944471a3e718914bbd1217"><a name="a707eec98f3944471a3e718914bbd1217"></a><a name="a707eec98f3944471a3e718914bbd1217"></a>设置当前任务的优先级。</p>
</td>
</tr>
<tr id="raaecb44635d34e9a8f344d23f4d253f0"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="a10c01eecc97f4fad842a338185bc956d"><a name="a10c01eecc97f4fad842a338185bc956d"></a><a name="a10c01eecc97f4fad842a338185bc956d"></a>LOS_TaskPriSet</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="a8a4f9410422f4ae6a7ab31669c6289db"><a name="a8a4f9410422f4ae6a7ab31669c6289db"></a><a name="a8a4f9410422f4ae6a7ab31669c6289db"></a>设置指定任务的优先级。</p>
</td>
</tr>
<tr id="rd9ea6b09409d4191a6cb21a710213671"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="afccbb8a90d354d06b0f5e37ae013a3ea"><a name="afccbb8a90d354d06b0f5e37ae013a3ea"></a><a name="afccbb8a90d354d06b0f5e37ae013a3ea"></a>LOS_TaskPriGet</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="a81a425575cfe494aa6424e1f60853e8e"><a name="a81a425575cfe494aa6424e1f60853e8e"></a><a name="a81a425575cfe494aa6424e1f60853e8e"></a>获取指定任务的优先级。</p>
</td>
</tr>
<tr id="r279361380256435ca3376c8652c7afd0"><td class="cellrowborder" rowspan="6" valign="top" width="26.75%" headers="mcps1.1.4.1.1 "><p id="add966bfadd9d4d368aeb32dd5e0e9dd6"><a name="add966bfadd9d4d368aeb32dd5e0e9dd6"></a><a name="add966bfadd9d4d368aeb32dd5e0e9dd6"></a>任务信息获取</p>
<p id="p23252111868"><a name="p23252111868"></a><a name="p23252111868"></a></p>
<p id="p118292251665"><a name="p118292251665"></a><a name="p118292251665"></a></p>
<p id="p1747514198611"><a name="p1747514198611"></a><a name="p1747514198611"></a></p>
<p id="p1539915364"><a name="p1539915364"></a><a name="p1539915364"></a></p>
</td>
<td class="cellrowborder" valign="top" width="27.13%" headers="mcps1.1.4.1.2 "><p id="ace5293ec6e284e8ca7500d3829173fcf"><a name="ace5293ec6e284e8ca7500d3829173fcf"></a><a name="ace5293ec6e284e8ca7500d3829173fcf"></a>LOS_CurTaskIDGet</p>
</td>
<td class="cellrowborder" valign="top" width="46.12%" headers="mcps1.1.4.1.3 "><p id="a86e6bbe920bb4e6fbecb000d778d9324"><a name="a86e6bbe920bb4e6fbecb000d778d9324"></a><a name="a86e6bbe920bb4e6fbecb000d778d9324"></a>获取当前任务的ID。</p>
</td>
</tr>
<tr id="red7f7c88a3544634acb8912860a954b0"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="a12ac8f1b550b4e478f6172583270bcf0"><a name="a12ac8f1b550b4e478f6172583270bcf0"></a><a name="a12ac8f1b550b4e478f6172583270bcf0"></a>LOS_TaskInfoGet</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="adda69aa141704fcd94793fd198364e81"><a name="adda69aa141704fcd94793fd198364e81"></a><a name="adda69aa141704fcd94793fd198364e81"></a>获取指定任务的信息。</p>
</td>
</tr>
<tr id="row153244111269"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p14325191118616"><a name="p14325191118616"></a><a name="p14325191118616"></a>LOS_TaskStatusGet</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="p53251211262"><a name="p53251211262"></a><a name="p53251211262"></a>获取指定任务的状态。</p>
</td>
</tr>
<tr id="row168296251067"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p382922518617"><a name="p382922518617"></a><a name="p382922518617"></a>LOS_TaskNameGet</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="p882942512613"><a name="p882942512613"></a><a name="p882942512613"></a>获取指定任务的名称。</p>
</td>
</tr>
<tr id="row1747531914613"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8475219369"><a name="p8475219369"></a><a name="p8475219369"></a>LOS_TaskInfoMonitor</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="p184765199611"><a name="p184765199611"></a><a name="p184765199611"></a>监控所有任务，获取所有任务的信息。</p>
</td>
</tr>
<tr id="row953914151763"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p14539121513619"><a name="p14539121513619"></a><a name="p14539121513619"></a>LOS_NextTaskIDGet</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="p153911151169"><a name="p153911151169"></a><a name="p153911151169"></a>获取即将被调度的任务的ID。</p>
</td>
</tr>
</tbody>
</table>

#### 开发流程

以创建任务为例，讲解开发流程。

1.  在los\_config.h中配置任务模块。

    配置LOSCFG\_BASE\_CORE\_TSK\_LIMIT系统支持最大任务数，这个可以根据需求自己配置。

    配置LOSCFG\_BASE\_CORE\_TSK\_IDLE\_STACK\_SIZE 空闲（IDLE）任务栈大小，这个默认即可。

    配置LOSCFG\_BASE\_CORE\_TSK\_DEFAULT\_STACK\_SIZE默认任务栈大小，用户根据自己的需求进行配置，在用户创建任务时，可以进行针对性设置。

    配置LOSCFG\_BASE\_CORE\_TIMESLICE时间片开关为YES。

    配置LOSCFG\_BASE\_CORE\_TIMESLICE\_TIMEOUT时间片，根据实际情况自己配置。

    配置LOSCFG\_BASE\_CORE\_TSK\_MONITOR任务监测模块裁剪开关，可选择是否打开。

2.  锁任务LOS\_TaskLock，锁住任务，防止高优先级任务调度。
3.  创建任务LOS\_TaskCreate。
4.  解锁任务LOS\_TaskUnlock，让任务按照优先级进行调度。
5.  延时任务LOS\_TaskDelay，任务延时等待。
6.  挂起指定的任务LOS\_TaskSuspend，任务挂起等待恢复操作。
7.  恢复挂起的任务LOS\_TaskResume。

#### TASK状态

Huawei LiteOS任务的大多数状态由内核维护，唯有自删除状态对用户可见，需要用户在创建任务时传入：

<a name="table14718192575217"></a>
<table><thead align="left"><tr id="row15719132505210"><th class="cellrowborder" valign="top" width="17.36%" id="mcps1.1.5.1.1"><p id="p18719825185216"><a name="p18719825185216"></a><a name="p18719825185216"></a>序号</p>
</th>
<th class="cellrowborder" valign="top" width="27.01%" id="mcps1.1.5.1.2"><p id="p371911259528"><a name="p371911259528"></a><a name="p371911259528"></a>定义</p>
</th>
<th class="cellrowborder" valign="top" width="27.310000000000002%" id="mcps1.1.5.1.3"><p id="p14719132535218"><a name="p14719132535218"></a><a name="p14719132535218"></a>实际数值</p>
</th>
<th class="cellrowborder" valign="top" width="28.32%" id="mcps1.1.5.1.4"><p id="p9719172525216"><a name="p9719172525216"></a><a name="p9719172525216"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row2719142519525"><td class="cellrowborder" valign="top" width="17.36%" headers="mcps1.1.5.1.1 "><p id="p10719122517526"><a name="p10719122517526"></a><a name="p10719122517526"></a>1</p>
</td>
<td class="cellrowborder" valign="top" width="27.01%" headers="mcps1.1.5.1.2 "><p id="p27191625145212"><a name="p27191625145212"></a><a name="p27191625145212"></a>LOS_TASK_STATUS_DETACHED</p>
</td>
<td class="cellrowborder" valign="top" width="27.310000000000002%" headers="mcps1.1.5.1.3 "><p id="p9719125205216"><a name="p9719125205216"></a><a name="p9719125205216"></a>0x0100</p>
</td>
<td class="cellrowborder" valign="top" width="28.32%" headers="mcps1.1.5.1.4 "><p id="p8719725175211"><a name="p8719725175211"></a><a name="p8719725175211"></a>任务是自删除的</p>
</td>
</tr>
</tbody>
</table>

用户在调用LOS\_TaskCreate接口创建任务时，需要将创建任务的TSK\_INIT\_PARAM\_S参数的uwResved域设置为LOS\_TASK\_STATUS\_DETACHED，即自删除状态，设置成自删除状态的任务会在运行完成时进行自删除动作。

>![](public_sys-resources/icon-notice.gif) **须知：**   
>
>-   在调用内核LOS\_TaskCreate接口创建任务时，默认必须要将任务状态设置为LOS\_TASK\_STATUS\_DETACHED。  

#### TASK错误码

对任务存在失败可能性的操作，包括创建任务、删除任务、挂起任务、恢复任务、延时任务等等，均需要返回对应的错误码，以便快速定位错误原因。

<a name="te1689e67f85946719b454d6bed4e3e52"></a>
<table><thead align="left"><tr id="rfd5750bf1b364d4a8be7e02324048334"><th class="cellrowborder" valign="top" width="7.920792079207921%" id="mcps1.1.6.1.1"><p id="a0cde6fdec537495c8e1ecd87f72c9d79"><a name="a0cde6fdec537495c8e1ecd87f72c9d79"></a><a name="a0cde6fdec537495c8e1ecd87f72c9d79"></a>序号</p>
</th>
<th class="cellrowborder" valign="top" width="26.732673267326735%" id="mcps1.1.6.1.2"><p id="zh-cn_topic_0015795099_p261046995642"><a name="zh-cn_topic_0015795099_p261046995642"></a><a name="zh-cn_topic_0015795099_p261046995642"></a>定义</p>
</th>
<th class="cellrowborder" valign="top" width="15.841584158415841%" id="mcps1.1.6.1.3"><p id="a2eaab4846668426a94a99dcb7e467443"><a name="a2eaab4846668426a94a99dcb7e467443"></a><a name="a2eaab4846668426a94a99dcb7e467443"></a>实际数值</p>
</th>
<th class="cellrowborder" valign="top" width="23.762376237623762%" id="mcps1.1.6.1.4"><p id="a056195aec63840758cb2f5a773c304cd"><a name="a056195aec63840758cb2f5a773c304cd"></a><a name="a056195aec63840758cb2f5a773c304cd"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="25.742574257425744%" id="mcps1.1.6.1.5"><p id="a238e2884d04e405aa815071cf66748c9"><a name="a238e2884d04e405aa815071cf66748c9"></a><a name="a238e2884d04e405aa815071cf66748c9"></a>参考解决方案</p>
</th>
</tr>
</thead>
<tbody><tr id="r65fbdac3933a415f84e0837e58e37f91"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="ad76080441ae74b6cb2daa3537da5c5d3"><a name="ad76080441ae74b6cb2daa3537da5c5d3"></a><a name="ad76080441ae74b6cb2daa3537da5c5d3"></a>1</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="a27fed42633ae4d0b8eadf4b8b88e25b0"><a name="a27fed42633ae4d0b8eadf4b8b88e25b0"></a><a name="a27fed42633ae4d0b8eadf4b8b88e25b0"></a>LOS_ERRNO_TSK_NO_MEMORY</p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="a1289b3442e13462b835022c4a683d3b4"><a name="a1289b3442e13462b835022c4a683d3b4"></a><a name="a1289b3442e13462b835022c4a683d3b4"></a>0x03000200</p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="zh-cn_topic_0015795099_p492720095642"><a name="zh-cn_topic_0015795099_p492720095642"></a><a name="zh-cn_topic_0015795099_p492720095642"></a>内存空间不足</p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="a00afc42ead0049cf9dd708ac3f94f809"><a name="a00afc42ead0049cf9dd708ac3f94f809"></a><a name="a00afc42ead0049cf9dd708ac3f94f809"></a>分配更大的内存分区</p>
</td>
</tr>
<tr id="r93acf97910734a4c8bb97c031665838d"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="a7cfc0a0b0d6a445c8372b4f417bee6bb"><a name="a7cfc0a0b0d6a445c8372b4f417bee6bb"></a><a name="a7cfc0a0b0d6a445c8372b4f417bee6bb"></a>2</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="a955089f87999443fb4d6eed41bdb3e9c"><a name="a955089f87999443fb4d6eed41bdb3e9c"></a><a name="a955089f87999443fb4d6eed41bdb3e9c"></a>LOS_ERRNO_TSK_PTR_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="a563678ed0ed449a4b7f5349c0bfb00e8"><a name="a563678ed0ed449a4b7f5349c0bfb00e8"></a><a name="a563678ed0ed449a4b7f5349c0bfb00e8"></a>0x02000201</p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="a1eb3acec0d304ca09102cc4179699415"><a name="a1eb3acec0d304ca09102cc4179699415"></a><a name="a1eb3acec0d304ca09102cc4179699415"></a>任务参数为空</p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="zh-cn_topic_0015795099_p412423410026"><a name="zh-cn_topic_0015795099_p412423410026"></a><a name="zh-cn_topic_0015795099_p412423410026"></a>检查任务参数</p>
</td>
</tr>
<tr id="reeed656274944fd29c991cfecad53fb0"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="a0a4f156f506e4150891861ed983b0c69"><a name="a0a4f156f506e4150891861ed983b0c69"></a><a name="a0a4f156f506e4150891861ed983b0c69"></a>3</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="af1183400ba3c47f1b4d844dc412f6d17"><a name="af1183400ba3c47f1b4d844dc412f6d17"></a><a name="af1183400ba3c47f1b4d844dc412f6d17"></a>LOS_ERRNO_TSK_STKSZ_NOT_ALIGN</p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="a54958fd2cdc64272b2940f4c135b597c"><a name="a54958fd2cdc64272b2940f4c135b597c"></a><a name="a54958fd2cdc64272b2940f4c135b597c"></a>0x02000202</p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="a4eb30dcdab6d4affa7be05b828903315"><a name="a4eb30dcdab6d4affa7be05b828903315"></a><a name="a4eb30dcdab6d4affa7be05b828903315"></a>任务栈大小未对齐</p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="afebd43f882e84df4b25608f6cd693b55"><a name="afebd43f882e84df4b25608f6cd693b55"></a><a name="afebd43f882e84df4b25608f6cd693b55"></a>对齐任务栈<span id="ph280121595516"><a name="ph280121595516"></a><a name="ph280121595516"></a>，暂不使用该错误码</span></p>
</td>
</tr>
<tr id="r23a37726131f48dd87f3934f8360f673"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="af11fb0c2971946eba05ae0ec86d51597"><a name="af11fb0c2971946eba05ae0ec86d51597"></a><a name="af11fb0c2971946eba05ae0ec86d51597"></a>4</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="a6f6fe3003d774224a61d2b21f9074c45"><a name="a6f6fe3003d774224a61d2b21f9074c45"></a><a name="a6f6fe3003d774224a61d2b21f9074c45"></a>LOS_ERRNO_TSK_PRIOR_ERROR</p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="aa66098c771954729b628f222161dd727"><a name="aa66098c771954729b628f222161dd727"></a><a name="aa66098c771954729b628f222161dd727"></a>0x02000203</p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="zh-cn_topic_0015795099_p915662595642"><a name="zh-cn_topic_0015795099_p915662595642"></a><a name="zh-cn_topic_0015795099_p915662595642"></a>不正确的任务优先级</p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="a0dc8fdda515844ad835f37ab9b5bd746"><a name="a0dc8fdda515844ad835f37ab9b5bd746"></a><a name="a0dc8fdda515844ad835f37ab9b5bd746"></a>检查任务优先级</p>
</td>
</tr>
<tr id="rea1700b81c134ff095a3ec6ec6568675"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="ae301cff2e9d344ae922b2d026e7dadb9"><a name="ae301cff2e9d344ae922b2d026e7dadb9"></a><a name="ae301cff2e9d344ae922b2d026e7dadb9"></a>5</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="a66662e8961f341c5bf7d2c434fed999a"><a name="a66662e8961f341c5bf7d2c434fed999a"></a><a name="a66662e8961f341c5bf7d2c434fed999a"></a>LOS_ERRNO_TSK_ENTRY_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="zh-cn_topic_0015795099_p804161495642"><a name="zh-cn_topic_0015795099_p804161495642"></a><a name="zh-cn_topic_0015795099_p804161495642"></a>0x02000204</p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="a1114d5c28c884640abc27a8fdc3eb618"><a name="a1114d5c28c884640abc27a8fdc3eb618"></a><a name="a1114d5c28c884640abc27a8fdc3eb618"></a>任务入口函数为空</p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="a1ce507eeb73d48c68215d68e70fc3d58"><a name="a1ce507eeb73d48c68215d68e70fc3d58"></a><a name="a1ce507eeb73d48c68215d68e70fc3d58"></a>定义任务入口函数</p>
</td>
</tr>
<tr id="r9e0797f813e84f08b4a220b1c4cc392d"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="a0980ee124e2d49d585d279c2b475e0a5"><a name="a0980ee124e2d49d585d279c2b475e0a5"></a><a name="a0980ee124e2d49d585d279c2b475e0a5"></a>6</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="a79ffd41aa2f94dd9ac1734fba3512c05"><a name="a79ffd41aa2f94dd9ac1734fba3512c05"></a><a name="a79ffd41aa2f94dd9ac1734fba3512c05"></a>LOS_ERRNO_TSK_NAME_EMPTY</p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="aa2ad3eb5017c44e0be0afc57b0134aae"><a name="aa2ad3eb5017c44e0be0afc57b0134aae"></a><a name="aa2ad3eb5017c44e0be0afc57b0134aae"></a>0x02000205</p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="aa31cc53b78ae46558574925b2a92a65c"><a name="aa31cc53b78ae46558574925b2a92a65c"></a><a name="aa31cc53b78ae46558574925b2a92a65c"></a>任务名为空</p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="afd801c90b30747c6ba185c8dc6bbe87f"><a name="afd801c90b30747c6ba185c8dc6bbe87f"></a><a name="afd801c90b30747c6ba185c8dc6bbe87f"></a>设置任务名</p>
</td>
</tr>
<tr id="r506de8212e394fdba7330c2da84b7405"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="a0ebeeba02438427db0dfc8f5fbeead4e"><a name="a0ebeeba02438427db0dfc8f5fbeead4e"></a><a name="a0ebeeba02438427db0dfc8f5fbeead4e"></a>7</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="abf56b112c9f74bde89340b45b9aa3555"><a name="abf56b112c9f74bde89340b45b9aa3555"></a><a name="abf56b112c9f74bde89340b45b9aa3555"></a>LOS_ERRNO_TSK_STKSZ_TOO_SMALL</p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="ade185bf2f4b54524a20c94d2b846c8b9"><a name="ade185bf2f4b54524a20c94d2b846c8b9"></a><a name="ade185bf2f4b54524a20c94d2b846c8b9"></a>0x02000206</p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="aa3e75683f8914c2f83ae5233e1cce52d"><a name="aa3e75683f8914c2f83ae5233e1cce52d"></a><a name="aa3e75683f8914c2f83ae5233e1cce52d"></a>任务栈太小</p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="aaebf5ef9f1ce4bc5a2bc74c74b1f145b"><a name="aaebf5ef9f1ce4bc5a2bc74c74b1f145b"></a><a name="aaebf5ef9f1ce4bc5a2bc74c74b1f145b"></a>扩大任务栈</p>
</td>
</tr>
<tr id="r892f39ef27994c638019328788066b74"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="a278e0debcc5b40bb943dfcdaaef989c0"><a name="a278e0debcc5b40bb943dfcdaaef989c0"></a><a name="a278e0debcc5b40bb943dfcdaaef989c0"></a>8</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="aacbb9f6a7ae34756bd4dda3f74de4d7a"><a name="aacbb9f6a7ae34756bd4dda3f74de4d7a"></a><a name="aacbb9f6a7ae34756bd4dda3f74de4d7a"></a>LOS_ERRNO_TSK_ID_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="a02dcb3e7c2064160b379c402decd15ac"><a name="a02dcb3e7c2064160b379c402decd15ac"></a><a name="a02dcb3e7c2064160b379c402decd15ac"></a>0x02000207</p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="a21ce1068891b41b3a03261d01842a5a8"><a name="a21ce1068891b41b3a03261d01842a5a8"></a><a name="a21ce1068891b41b3a03261d01842a5a8"></a>无效的任务ID</p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="ac17908d7e0504a6294bfec533aa3c388"><a name="ac17908d7e0504a6294bfec533aa3c388"></a><a name="ac17908d7e0504a6294bfec533aa3c388"></a>检查任务ID</p>
</td>
</tr>
<tr id="ra9404156ae5b49ca9ee55969752c57dd"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="aed818fcea01b4eada199cfc5a29efffd"><a name="aed818fcea01b4eada199cfc5a29efffd"></a><a name="aed818fcea01b4eada199cfc5a29efffd"></a>9</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="a22294394f211492d95f86b11c1b7faeb"><a name="a22294394f211492d95f86b11c1b7faeb"></a><a name="a22294394f211492d95f86b11c1b7faeb"></a>LOS_ERRNO_TSK_ALREADY_SUSPENDED</p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="a394afdfeb6bd4ed48bf9bfaceb14b9b0"><a name="a394afdfeb6bd4ed48bf9bfaceb14b9b0"></a><a name="a394afdfeb6bd4ed48bf9bfaceb14b9b0"></a>0x02000208</p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="aee93558602cc43d1a4db7cd86caaf687"><a name="aee93558602cc43d1a4db7cd86caaf687"></a><a name="aee93558602cc43d1a4db7cd86caaf687"></a>任务已经被挂起</p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="aa451d7553493475daf1d6f3120ca6c54"><a name="aa451d7553493475daf1d6f3120ca6c54"></a><a name="aa451d7553493475daf1d6f3120ca6c54"></a>等待这个任务被恢复后，再去尝试挂起这个任务</p>
</td>
</tr>
<tr id="r72036790def04574b16a6ae0ff5e75ba"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="a3bb5759ea29946edbbe27382958edd5d"><a name="a3bb5759ea29946edbbe27382958edd5d"></a><a name="a3bb5759ea29946edbbe27382958edd5d"></a>10</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="ac9726e70812f443caab60c69e0e4bfe2"><a name="ac9726e70812f443caab60c69e0e4bfe2"></a><a name="ac9726e70812f443caab60c69e0e4bfe2"></a>LOS_ERRNO_TSK_NOT_SUSPENDED</p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="a85faa32492c942b48f558f42c35020b4"><a name="a85faa32492c942b48f558f42c35020b4"></a><a name="a85faa32492c942b48f558f42c35020b4"></a>0x02000209</p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="ae14f9a5f03464f9a92bbd61d70aaebef"><a name="ae14f9a5f03464f9a92bbd61d70aaebef"></a><a name="ae14f9a5f03464f9a92bbd61d70aaebef"></a>任务未被挂起</p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="af53d127b08a441adb479c6cbb242f031"><a name="af53d127b08a441adb479c6cbb242f031"></a><a name="af53d127b08a441adb479c6cbb242f031"></a>挂起这个任务</p>
</td>
</tr>
<tr id="r0700eaca16394f61a1b421b5c9eec1e2"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="a0552771b8ec146ada3d1581d31f4a28e"><a name="a0552771b8ec146ada3d1581d31f4a28e"></a><a name="a0552771b8ec146ada3d1581d31f4a28e"></a>11</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="a832023464ee347f085df21d6b2427063"><a name="a832023464ee347f085df21d6b2427063"></a><a name="a832023464ee347f085df21d6b2427063"></a>LOS_ERRNO_TSK_NOT_CREATED</p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="a61c4809c0f6643c3be514e038e9968df"><a name="a61c4809c0f6643c3be514e038e9968df"></a><a name="a61c4809c0f6643c3be514e038e9968df"></a>0x0200020a</p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="a3b4ee00d6fa54f9e8c73c1b5906f3831"><a name="a3b4ee00d6fa54f9e8c73c1b5906f3831"></a><a name="a3b4ee00d6fa54f9e8c73c1b5906f3831"></a>任务未被创建</p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="a2229ad4fd50e4748accb4c2ee257318e"><a name="a2229ad4fd50e4748accb4c2ee257318e"></a><a name="a2229ad4fd50e4748accb4c2ee257318e"></a>创建这个任务</p>
</td>
</tr>
<tr id="r5d96ef35b55442c2942a6427f6ed3bbc"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="af7e3e69c32b647dcb7579c255f839c34"><a name="af7e3e69c32b647dcb7579c255f839c34"></a><a name="af7e3e69c32b647dcb7579c255f839c34"></a>12</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="a602f44adcbbe491db127c148937bf661"><a name="a602f44adcbbe491db127c148937bf661"></a><a name="a602f44adcbbe491db127c148937bf661"></a><span id="ph1331122935610"><a name="ph1331122935610"></a><a name="ph1331122935610"></a>LOS_ERRNO_TSK_DELETE_LOCKE</span></p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="a45deb433e2664130bd226f65734d883d"><a name="a45deb433e2664130bd226f65734d883d"></a><a name="a45deb433e2664130bd226f65734d883d"></a><span id="ph127513875613"><a name="ph127513875613"></a><a name="ph127513875613"></a>0x0300020b</span></p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="p174343449560"><a name="p174343449560"></a><a name="p174343449560"></a>删除任务时，任务处于被锁状态</p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="a8e5f0202eac34ebe830908e4deb0c82c"><a name="a8e5f0202eac34ebe830908e4deb0c82c"></a><a name="a8e5f0202eac34ebe830908e4deb0c82c"></a><span id="ph934365325615"><a name="ph934365325615"></a><a name="ph934365325615"></a>等待解锁任务之后再进行删除操作</span></p>
</td>
</tr>
<tr id="r49de1218021e4492873e12d497a5059f"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="ab93d1b8d17324a648f11426616cb1b3b"><a name="ab93d1b8d17324a648f11426616cb1b3b"></a><a name="ab93d1b8d17324a648f11426616cb1b3b"></a>13</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="abf1eed88bbe9438195d668ca9da01ae9"><a name="abf1eed88bbe9438195d668ca9da01ae9"></a><a name="abf1eed88bbe9438195d668ca9da01ae9"></a>LOS_ERRNO_TSK_MSG_NONZERO</p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="a28307893125546b3829749dbfdc1641f"><a name="a28307893125546b3829749dbfdc1641f"></a><a name="a28307893125546b3829749dbfdc1641f"></a>0x0200020c</p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="a5bf203d47a2e4f10a52b89f275c5391a"><a name="a5bf203d47a2e4f10a52b89f275c5391a"></a><a name="a5bf203d47a2e4f10a52b89f275c5391a"></a>任务信息非零</p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="a205a18d331554f5fa0a1eec3011d64f0"><a name="a205a18d331554f5fa0a1eec3011d64f0"></a><a name="a205a18d331554f5fa0a1eec3011d64f0"></a>暂不使用该错误码</p>
</td>
</tr>
<tr id="r300a015ffa664e2bbc756f72b6926ce0"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="ab1b0ed8a756b422782c25404323e45fe"><a name="ab1b0ed8a756b422782c25404323e45fe"></a><a name="ab1b0ed8a756b422782c25404323e45fe"></a>14</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="a0b00d281f5d243c8b4e5933c2c8e6eb9"><a name="a0b00d281f5d243c8b4e5933c2c8e6eb9"></a><a name="a0b00d281f5d243c8b4e5933c2c8e6eb9"></a>LOS_ERRNO_TSK_DELAY_IN_INT</p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="aca73c6ea64d24017b27b40e60e7eda7a"><a name="aca73c6ea64d24017b27b40e60e7eda7a"></a><a name="aca73c6ea64d24017b27b40e60e7eda7a"></a>0x0300020d</p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="a0584f48591994067854445c934c31508"><a name="a0584f48591994067854445c934c31508"></a><a name="a0584f48591994067854445c934c31508"></a>中断期间，进行任务延时</p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="a56e96069b8334e2fa68a5518938010d0"><a name="a56e96069b8334e2fa68a5518938010d0"></a><a name="a56e96069b8334e2fa68a5518938010d0"></a>等待退出中断后再进行延时操作</p>
</td>
</tr>
<tr id="re0abc4cf76e845418158c461e1166350"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="a3717a237d03645efa3dd43a356ff38a7"><a name="a3717a237d03645efa3dd43a356ff38a7"></a><a name="a3717a237d03645efa3dd43a356ff38a7"></a>15</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="a7e6f405bbbad473da2e09337c7e0bf55"><a name="a7e6f405bbbad473da2e09337c7e0bf55"></a><a name="a7e6f405bbbad473da2e09337c7e0bf55"></a>LOS_ERRNO_TSK_DELAY_IN_LOCK</p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="a5e530dcc3eee4fcc9e8689d75e45a437"><a name="a5e530dcc3eee4fcc9e8689d75e45a437"></a><a name="a5e530dcc3eee4fcc9e8689d75e45a437"></a>0x0200020e</p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="acba271600a15458e9dbf74d7c8de0feb"><a name="acba271600a15458e9dbf74d7c8de0feb"></a><a name="acba271600a15458e9dbf74d7c8de0feb"></a>任务被锁的状态下，进行延时</p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="a12922dba9ae84b849fbc69c50b7b85f8"><a name="a12922dba9ae84b849fbc69c50b7b85f8"></a><a name="a12922dba9ae84b849fbc69c50b7b85f8"></a>等待解锁任务之后再进行延时操作</p>
</td>
</tr>
<tr id="rd76ec4efd1994c82aa10ca071b2102e4"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="a87a7a852cec1419d8d987865e192af0e"><a name="a87a7a852cec1419d8d987865e192af0e"></a><a name="a87a7a852cec1419d8d987865e192af0e"></a>16</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="a5e67e62e12c24924b7f02391744e5999"><a name="a5e67e62e12c24924b7f02391744e5999"></a><a name="a5e67e62e12c24924b7f02391744e5999"></a><span id="ph1443110575717"><a name="ph1443110575717"></a><a name="ph1443110575717"></a>LOS_ERRNO_TSK_YIELD_IN_LOCK</span></p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="zh-cn_topic_0015795099_p224588410124"><a name="zh-cn_topic_0015795099_p224588410124"></a><a name="zh-cn_topic_0015795099_p224588410124"></a><span id="ph5275181545710"><a name="ph5275181545710"></a><a name="ph5275181545710"></a>0x0200020f</span></p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="a364921a7af0847eda2d9cd10b95b63b6"><a name="a364921a7af0847eda2d9cd10b95b63b6"></a><a name="a364921a7af0847eda2d9cd10b95b63b6"></a><span id="ph121414311579"><a name="ph121414311579"></a><a name="ph121414311579"></a>任务被锁的状态下，进行Yield操作</span></p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="afd6b5a332b174560a95de9419c817c6b"><a name="afd6b5a332b174560a95de9419c817c6b"></a><a name="afd6b5a332b174560a95de9419c817c6b"></a><span id="ph16277164017574"><a name="ph16277164017574"></a><a name="ph16277164017574"></a>等待任务解锁后进行Yield操作</span></p>
</td>
</tr>
<tr id="r4e043e3e95c847e7956ad89bc994ea9a"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="a6dcf4e93ff534e0ab2564cdf9802f602"><a name="a6dcf4e93ff534e0ab2564cdf9802f602"></a><a name="a6dcf4e93ff534e0ab2564cdf9802f602"></a>17</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="zh-cn_topic_0015795099_p602173510127"><a name="zh-cn_topic_0015795099_p602173510127"></a><a name="zh-cn_topic_0015795099_p602173510127"></a>LOS_ERRNO_TSK_YIELD_NOT_ENOUGH_TASK</p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="acef1a7141a684cedb47dd0e8ef6a5bf4"><a name="acef1a7141a684cedb47dd0e8ef6a5bf4"></a><a name="acef1a7141a684cedb47dd0e8ef6a5bf4"></a>0x02000210</p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="a638c62b8b59a481592b4d4f61990dd33"><a name="a638c62b8b59a481592b4d4f61990dd33"></a><a name="a638c62b8b59a481592b4d4f61990dd33"></a>没有或者仅有一个可用任务能进行行程安排</p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="a6ca95f46ffeb45158ab47f610a2247cc"><a name="a6ca95f46ffeb45158ab47f610a2247cc"></a><a name="a6ca95f46ffeb45158ab47f610a2247cc"></a>增加任务数</p>
</td>
</tr>
<tr id="re03f4b1f30354120b8e6721c2c71cc7c"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="ad49f84fe6cae4a9eaf232b8621ed04ba"><a name="ad49f84fe6cae4a9eaf232b8621ed04ba"></a><a name="ad49f84fe6cae4a9eaf232b8621ed04ba"></a>18</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="a335f0989b51e47ae99b1c6855e96abfd"><a name="a335f0989b51e47ae99b1c6855e96abfd"></a><a name="a335f0989b51e47ae99b1c6855e96abfd"></a>LOS_ERRNO_TSK_TCB_UNAVAILABLE</p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="a0eaf2e75af9b4855b7e0e4575cd3461e"><a name="a0eaf2e75af9b4855b7e0e4575cd3461e"></a><a name="a0eaf2e75af9b4855b7e0e4575cd3461e"></a>0x02000211</p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="a1105d60c5c5244e0a09a8cd14567b4cb"><a name="a1105d60c5c5244e0a09a8cd14567b4cb"></a><a name="a1105d60c5c5244e0a09a8cd14567b4cb"></a>没有空闲的任务控制块可用</p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="a3ff1d4ed72364b87ba6a393fb39998f6"><a name="a3ff1d4ed72364b87ba6a393fb39998f6"></a><a name="a3ff1d4ed72364b87ba6a393fb39998f6"></a>增加任务控制块数量</p>
</td>
</tr>
<tr id="r9a296a9607894194bda6309394b6cd06"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="ab2c1a516cfce4e0da9a5a711bb165b10"><a name="ab2c1a516cfce4e0da9a5a711bb165b10"></a><a name="ab2c1a516cfce4e0da9a5a711bb165b10"></a>19</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="a6ce795e8c3ac46d093983defa0372baa"><a name="a6ce795e8c3ac46d093983defa0372baa"></a><a name="a6ce795e8c3ac46d093983defa0372baa"></a>LOS_ERRNO_TSK_HOOK_NOT_MATCH</p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="a5d0221b9176e4af296cf7419ee2da1cf"><a name="a5d0221b9176e4af296cf7419ee2da1cf"></a><a name="a5d0221b9176e4af296cf7419ee2da1cf"></a>0x02000212</p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="a2d0b50c9a2ca4ababb74a2a55b23160e"><a name="a2d0b50c9a2ca4ababb74a2a55b23160e"></a><a name="a2d0b50c9a2ca4ababb74a2a55b23160e"></a>任务的钩子函数不匹配</p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="a8526b873e7134715a9a5768dc6777af7"><a name="a8526b873e7134715a9a5768dc6777af7"></a><a name="a8526b873e7134715a9a5768dc6777af7"></a>暂不使用该错误码</p>
</td>
</tr>
<tr id="r54030ab91654404f8800c9849f3dbbde"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="a826c25350f70424595bbd625cc59d685"><a name="a826c25350f70424595bbd625cc59d685"></a><a name="a826c25350f70424595bbd625cc59d685"></a>20</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="a5806d009ea1341debb34584de6399860"><a name="a5806d009ea1341debb34584de6399860"></a><a name="a5806d009ea1341debb34584de6399860"></a>LOS_ERRNO_TSK_HOOK_IS_FULL</p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="ab65a9f96facf4539929c2a28a5baee0f"><a name="ab65a9f96facf4539929c2a28a5baee0f"></a><a name="ab65a9f96facf4539929c2a28a5baee0f"></a>0x02000213</p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="a12aad2a9a0ff42e4a9979b949b6c759b"><a name="a12aad2a9a0ff42e4a9979b949b6c759b"></a><a name="a12aad2a9a0ff42e4a9979b949b6c759b"></a>任务的钩子函数数量超过界限</p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="a17c5f70deb1745b387dd6931aaf4a134"><a name="a17c5f70deb1745b387dd6931aaf4a134"></a><a name="a17c5f70deb1745b387dd6931aaf4a134"></a>暂不使用该错误码</p>
</td>
</tr>
<tr id="r52d65b851ce64e8c9d6106b73e240bca"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="a28a441d2d76f47eb8fe56d778d1519f1"><a name="a28a441d2d76f47eb8fe56d778d1519f1"></a><a name="a28a441d2d76f47eb8fe56d778d1519f1"></a>21</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="aa56b75f2913742e7b32fae223e27291f"><a name="aa56b75f2913742e7b32fae223e27291f"></a><a name="aa56b75f2913742e7b32fae223e27291f"></a><span id="ph196781450105714"><a name="ph196781450105714"></a><a name="ph196781450105714"></a>LOS_ERRNO_TSK_OPERATE_SYSTEM_TASK</span></p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="a6d3cc20e0a694f5cb3cd6aa4c3885122"><a name="a6d3cc20e0a694f5cb3cd6aa4c3885122"></a><a name="a6d3cc20e0a694f5cb3cd6aa4c3885122"></a><span id="ph777455613573"><a name="ph777455613573"></a><a name="ph777455613573"></a>0x02000214</span></p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="a9744fb2f42074796800d60e6a6cd6692"><a name="a9744fb2f42074796800d60e6a6cd6692"></a><a name="a9744fb2f42074796800d60e6a6cd6692"></a><span id="ph19771141114584"><a name="ph19771141114584"></a><a name="ph19771141114584"></a>这是个系统级别的任务</span></p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="p289941955818"><a name="p289941955818"></a><a name="p289941955818"></a>检查任务ID，不要试图操作系统任务</p>
</td>
</tr>
<tr id="r1c427fd36b6a453f96e7c404a2f80b4c"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="a87c0974908ce434d80281d069d9ba593"><a name="a87c0974908ce434d80281d069d9ba593"></a><a name="a87c0974908ce434d80281d069d9ba593"></a>22</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="afb9beef2fb814fb197b5f74f66563a62"><a name="afb9beef2fb814fb197b5f74f66563a62"></a><a name="afb9beef2fb814fb197b5f74f66563a62"></a>LOS_ERRNO_TSK_SUSPEND_LOCKED</p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="a94fce4cb1f03484d9cb171b5749b5ada"><a name="a94fce4cb1f03484d9cb171b5749b5ada"></a><a name="a94fce4cb1f03484d9cb171b5749b5ada"></a>0x03000215</p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="a1f38af036bdc4c79a881d6b88babc207"><a name="a1f38af036bdc4c79a881d6b88babc207"></a><a name="a1f38af036bdc4c79a881d6b88babc207"></a>将被挂起的任务处于被锁状态</p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="a77dac74474584311b240f98e9160ad23"><a name="a77dac74474584311b240f98e9160ad23"></a><a name="a77dac74474584311b240f98e9160ad23"></a>等待任务解锁后再尝试挂起任务</p>
</td>
</tr>
<tr id="radf34932c1994b9dbb897b39a6e5fe50"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="a1dfe7f2eda184c85a7c9b77f7160ad34"><a name="a1dfe7f2eda184c85a7c9b77f7160ad34"></a><a name="a1dfe7f2eda184c85a7c9b77f7160ad34"></a>23</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="a7ef30192a7af4f8c9e60f52352a17478"><a name="a7ef30192a7af4f8c9e60f52352a17478"></a><a name="a7ef30192a7af4f8c9e60f52352a17478"></a>LOS_ERRNO_TSK_FREE_STACK_FAILED</p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="a24a1e254f8c5492c89885f7d2e3754be"><a name="a24a1e254f8c5492c89885f7d2e3754be"></a><a name="a24a1e254f8c5492c89885f7d2e3754be"></a>0x02000217</p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="a121ed5898b4046ff97331ea5f77e1bf8"><a name="a121ed5898b4046ff97331ea5f77e1bf8"></a><a name="a121ed5898b4046ff97331ea5f77e1bf8"></a>任务栈free失败</p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="a0985d5f0ff8d4324a53fad39bb01fb93"><a name="a0985d5f0ff8d4324a53fad39bb01fb93"></a><a name="a0985d5f0ff8d4324a53fad39bb01fb93"></a>该错误码暂不使用</p>
</td>
</tr>
<tr id="rdab5654cc349433485f3a7ba0275bb88"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="a67812ff0a91046f3a1cc5f72895c6a0a"><a name="a67812ff0a91046f3a1cc5f72895c6a0a"></a><a name="a67812ff0a91046f3a1cc5f72895c6a0a"></a>24</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="af19c7333f3de4d2f99105e52675a0a1f"><a name="af19c7333f3de4d2f99105e52675a0a1f"></a><a name="af19c7333f3de4d2f99105e52675a0a1f"></a>LOS_ERRNO_TSK_STKAREA_TOO_SMALL</p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="a32a6126820934c1eb6bdf6445ad5f9fa"><a name="a32a6126820934c1eb6bdf6445ad5f9fa"></a><a name="a32a6126820934c1eb6bdf6445ad5f9fa"></a>0x02000218</p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="a014d5ffa677940efb652d844b05afe65"><a name="a014d5ffa677940efb652d844b05afe65"></a><a name="a014d5ffa677940efb652d844b05afe65"></a>任务栈区域太小</p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="a2a5d45ac743846e999a10e981d6d1562"><a name="a2a5d45ac743846e999a10e981d6d1562"></a><a name="a2a5d45ac743846e999a10e981d6d1562"></a>该错误码暂不使用</p>
</td>
</tr>
<tr id="ra6d8f6020e6840069d80bf24ce242a92"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="a645d83fa8059438782c64094a0f18e97"><a name="a645d83fa8059438782c64094a0f18e97"></a><a name="a645d83fa8059438782c64094a0f18e97"></a>25</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="aa012961d6cf744e090f7b8207cacc8c6"><a name="aa012961d6cf744e090f7b8207cacc8c6"></a><a name="aa012961d6cf744e090f7b8207cacc8c6"></a>LOS_ERRNO_TSK_ACTIVE_FAILED</p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="a9bad2007d4ec420185dd698e7bd6a7ed"><a name="a9bad2007d4ec420185dd698e7bd6a7ed"></a><a name="a9bad2007d4ec420185dd698e7bd6a7ed"></a>0x03000219</p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="a92e71a0d51174fe4998aef7dd1c49112"><a name="a92e71a0d51174fe4998aef7dd1c49112"></a><a name="a92e71a0d51174fe4998aef7dd1c49112"></a>任务触发失败</p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="a391f139ee89747f4aed0f9b1b9b0f066"><a name="a391f139ee89747f4aed0f9b1b9b0f066"></a><a name="a391f139ee89747f4aed0f9b1b9b0f066"></a>创建一个IDLE任务后执行任务转换<span id="ph11333123695817"><a name="ph11333123695817"></a><a name="ph11333123695817"></a>，暂不使用该错误码</span></p>
</td>
</tr>
<tr id="rc67e69d4b594431db7dc59c7a8fbab02"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="ae57e0a7649244da5ad3a390f54ae3391"><a name="ae57e0a7649244da5ad3a390f54ae3391"></a><a name="ae57e0a7649244da5ad3a390f54ae3391"></a>26</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="a6c110aa5cfee449b85655f57e3137ba3"><a name="a6c110aa5cfee449b85655f57e3137ba3"></a><a name="a6c110aa5cfee449b85655f57e3137ba3"></a>LOS_ERRNO_TSK_CONFIG_TOO_MANY</p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="af6cb15945795435886cfa8f49720e56d"><a name="af6cb15945795435886cfa8f49720e56d"></a><a name="af6cb15945795435886cfa8f49720e56d"></a>0x0200021a</p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="a8e7a502e33034e949a4a8afe4e5d9200"><a name="a8e7a502e33034e949a4a8afe4e5d9200"></a><a name="a8e7a502e33034e949a4a8afe4e5d9200"></a>过多的任务配置项</p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="a22240f1aa6974ff6bbdb796e1f6b7cb6"><a name="a22240f1aa6974ff6bbdb796e1f6b7cb6"></a><a name="a22240f1aa6974ff6bbdb796e1f6b7cb6"></a>该错误码暂不使用</p>
</td>
</tr>
<tr id="r3f4f64f7a7974b058f21fe5774276992"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="ac467ce68ea8e43db9610e6fcfadb146c"><a name="ac467ce68ea8e43db9610e6fcfadb146c"></a><a name="ac467ce68ea8e43db9610e6fcfadb146c"></a>27</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="a751d54377e9d4b1c9a93e0c7c756c454"><a name="a751d54377e9d4b1c9a93e0c7c756c454"></a><a name="a751d54377e9d4b1c9a93e0c7c756c454"></a>LOS_ERRNO_TSK_CP_SAVE_AREA_NOT_ALIGN</p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="a5a82de6f81014f85a1e958d108b4ef1a"><a name="a5a82de6f81014f85a1e958d108b4ef1a"></a><a name="a5a82de6f81014f85a1e958d108b4ef1a"></a>0x0200021b</p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="a55091f638c5048a1b9a652114b675d05"><a name="a55091f638c5048a1b9a652114b675d05"></a><a name="a55091f638c5048a1b9a652114b675d05"></a>暂无</p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="a233c6a48990547e9a5b364d8cc3d30ad"><a name="a233c6a48990547e9a5b364d8cc3d30ad"></a><a name="a233c6a48990547e9a5b364d8cc3d30ad"></a>该错误码暂不使用</p>
</td>
</tr>
<tr id="r5952f805efd84a1bb6b6f94804756fac"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="a2b98f3384ac04f56be096bc70f6a470b"><a name="a2b98f3384ac04f56be096bc70f6a470b"></a><a name="a2b98f3384ac04f56be096bc70f6a470b"></a>28</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="a87b81f43254647f9bdb988128997b480"><a name="a87b81f43254647f9bdb988128997b480"></a><a name="a87b81f43254647f9bdb988128997b480"></a>LOS_ERRNO_TSK_MSG_Q_TOO_MANY</p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="zh-cn_topic_0015795099_p761434110130"><a name="zh-cn_topic_0015795099_p761434110130"></a><a name="zh-cn_topic_0015795099_p761434110130"></a>0x0200021d</p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="a374ac875c0934b44b8643749389daf5b"><a name="a374ac875c0934b44b8643749389daf5b"></a><a name="a374ac875c0934b44b8643749389daf5b"></a>暂无</p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="a282d7da145cb483fa24cc8bc36a4dddb"><a name="a282d7da145cb483fa24cc8bc36a4dddb"></a><a name="a282d7da145cb483fa24cc8bc36a4dddb"></a>该错误码暂不使用</p>
</td>
</tr>
<tr id="r0bf589511b18498a9f4447d36005bb17"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="ae7c224f9161b4b5d94d54e0cd8181075"><a name="ae7c224f9161b4b5d94d54e0cd8181075"></a><a name="ae7c224f9161b4b5d94d54e0cd8181075"></a>29</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="a83a36df4f32d467ba1936b98e2623d99"><a name="a83a36df4f32d467ba1936b98e2623d99"></a><a name="a83a36df4f32d467ba1936b98e2623d99"></a>LOS_ERRNO_TSK_CP_SAVE_AREA_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="ab348ac92bb2348b1bf661f3f8755d8a7"><a name="ab348ac92bb2348b1bf661f3f8755d8a7"></a><a name="ab348ac92bb2348b1bf661f3f8755d8a7"></a>0x0200021e</p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="a09bd0d0bf89c42c4979f8d3bd1cc34d3"><a name="a09bd0d0bf89c42c4979f8d3bd1cc34d3"></a><a name="a09bd0d0bf89c42c4979f8d3bd1cc34d3"></a>暂无</p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="a01e585e53a284b5fa781a34e233158c9"><a name="a01e585e53a284b5fa781a34e233158c9"></a><a name="a01e585e53a284b5fa781a34e233158c9"></a>该错误码暂不使用</p>
</td>
</tr>
<tr id="r72ecf415ae0047a4bfc4a5d02226e503"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="af8d2ebeac7844fbda51edb9d68b9bbfc"><a name="af8d2ebeac7844fbda51edb9d68b9bbfc"></a><a name="af8d2ebeac7844fbda51edb9d68b9bbfc"></a>30</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="a7f300a3603154638830fbadbb1bc19f6"><a name="a7f300a3603154638830fbadbb1bc19f6"></a><a name="a7f300a3603154638830fbadbb1bc19f6"></a>LOS_ERRNO_TSK_SELF_DELETE_ERR</p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="a79ba1ad72ee54486ad101e61e70082c5"><a name="a79ba1ad72ee54486ad101e61e70082c5"></a><a name="a79ba1ad72ee54486ad101e61e70082c5"></a>0x0200021f</p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="zh-cn_topic_0015795099_p5161210136"><a name="zh-cn_topic_0015795099_p5161210136"></a><a name="zh-cn_topic_0015795099_p5161210136"></a>暂无</p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="zh-cn_topic_0015795099_p418062210136"><a name="zh-cn_topic_0015795099_p418062210136"></a><a name="zh-cn_topic_0015795099_p418062210136"></a>该错误码暂不使用</p>
</td>
</tr>
<tr id="rc5d2938202434b539f99c4e2742b5ff1"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="a26459001916040d2beda4082d3cbbff3"><a name="a26459001916040d2beda4082d3cbbff3"></a><a name="a26459001916040d2beda4082d3cbbff3"></a>31</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="a54be693ae10d45549b5526cd60c38c03"><a name="a54be693ae10d45549b5526cd60c38c03"></a><a name="a54be693ae10d45549b5526cd60c38c03"></a>LOS_ERRNO_TSK_STKSZ_TOO_LARGE</p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="af929b6b37dbd44ebb80a4fbc84a7429e"><a name="af929b6b37dbd44ebb80a4fbc84a7429e"></a><a name="af929b6b37dbd44ebb80a4fbc84a7429e"></a>0x02000220</p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="a89ebf2b1b38a4190b5a7540dcb328012"><a name="a89ebf2b1b38a4190b5a7540dcb328012"></a><a name="a89ebf2b1b38a4190b5a7540dcb328012"></a>任务栈大小设置过大</p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="a1bb30a97659945bd9b8caf2bee1ab147"><a name="a1bb30a97659945bd9b8caf2bee1ab147"></a><a name="a1bb30a97659945bd9b8caf2bee1ab147"></a>减小任务栈大小</p>
</td>
</tr>
<tr id="r84d95b10c3724e66aaaa0e7bbe6ec7b2"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="aa14912986e724b5cb2bd71dee0179213"><a name="aa14912986e724b5cb2bd71dee0179213"></a><a name="aa14912986e724b5cb2bd71dee0179213"></a>32</p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="ace703c5e2c994aeea6f31d9db5ef6453"><a name="ace703c5e2c994aeea6f31d9db5ef6453"></a><a name="ace703c5e2c994aeea6f31d9db5ef6453"></a>LOS_ERRNO_TSK_SUSPEND_SWTMR_NOT_ALLOWED</p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="a6cd3e3f70a6140be879ae29d37d56586"><a name="a6cd3e3f70a6140be879ae29d37d56586"></a><a name="a6cd3e3f70a6140be879ae29d37d56586"></a>0x02000221</p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="a9b8352e69cf34661b3c0796ff8ec552d"><a name="a9b8352e69cf34661b3c0796ff8ec552d"></a><a name="a9b8352e69cf34661b3c0796ff8ec552d"></a>不允许挂起软件定时器任务</p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="a0418aef880c548e4abe49dc6ba657c3b"><a name="a0418aef880c548e4abe49dc6ba657c3b"></a><a name="a0418aef880c548e4abe49dc6ba657c3b"></a>检查任务ID, 不要试图挂起软件定时器任务<span id="ph1217718487586"><a name="ph1217718487586"></a><a name="ph1217718487586"></a>，暂不使用该错误码</span></p>
</td>
</tr>
<tr id="row141783596589"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="p15178125965813"><a name="p15178125965813"></a><a name="p15178125965813"></a><span id="ph13197921046"><a name="ph13197921046"></a><a name="ph13197921046"></a>33</span></p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="p16179155925814"><a name="p16179155925814"></a><a name="p16179155925814"></a><span id="ph1762212141217"><a name="ph1762212141217"></a><a name="ph1762212141217"></a>LOS_ERRNO_TSK_DELAY_IN_SWTMR_TSK</span></p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="p191791659195812"><a name="p191791659195812"></a><a name="p191791659195812"></a><span id="ph786692219214"><a name="ph786692219214"></a><a name="ph786692219214"></a>0x03000222</span></p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="p1517965935819"><a name="p1517965935819"></a><a name="p1517965935819"></a><span id="ph12779231722"><a name="ph12779231722"></a><a name="ph12779231722"></a>在软件定时器回调中，调用任务延时</span></p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="p11179259195818"><a name="p11179259195818"></a><a name="p11179259195818"></a><span id="ph189191341621"><a name="ph189191341621"></a><a name="ph189191341621"></a>不要在软件定时器中调用任务延时，暂不使用该错误码</span></p>
</td>
</tr>
<tr id="row148131952594"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="p18139585916"><a name="p18139585916"></a><a name="p18139585916"></a><span id="ph152901959419"><a name="ph152901959419"></a><a name="ph152901959419"></a>34</span></p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="p4813452596"><a name="p4813452596"></a><a name="p4813452596"></a><span id="ph141638517218"><a name="ph141638517218"></a><a name="ph141638517218"></a>LOS_ERRNO_TSK_CPU_AFFINITY_MASK_ERR</span></p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="p38139513598"><a name="p38139513598"></a><a name="p38139513598"></a><span id="ph369012573213"><a name="ph369012573213"></a><a name="ph369012573213"></a>0x03000223</span></p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="p20813205125911"><a name="p20813205125911"></a><a name="p20813205125911"></a><span id="ph25801818634"><a name="ph25801818634"></a><a name="ph25801818634"></a>任务CPU亲和性Mask有误</span></p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="p78137525911"><a name="p78137525911"></a><a name="p78137525911"></a><span id="ph3137132714310"><a name="ph3137132714310"></a><a name="ph3137132714310"></a>检查传入的亲和性Mask</span></p>
</td>
</tr>
<tr id="row29369161619"><td class="cellrowborder" valign="top" width="7.920792079207921%" headers="mcps1.1.6.1.1 "><p id="p1293617162010"><a name="p1293617162010"></a><a name="p1293617162010"></a><span id="ph1363419711412"><a name="ph1363419711412"></a><a name="ph1363419711412"></a>35</span></p>
</td>
<td class="cellrowborder" valign="top" width="26.732673267326735%" headers="mcps1.1.6.1.2 "><p id="p1193618161417"><a name="p1193618161417"></a><a name="p1193618161417"></a><span id="ph0471175912316"><a name="ph0471175912316"></a><a name="ph0471175912316"></a>LOS_ERRNO_TSK_YIELD_IN_INT</span></p>
</td>
<td class="cellrowborder" valign="top" width="15.841584158415841%" headers="mcps1.1.6.1.3 "><p id="p1293611618113"><a name="p1293611618113"></a><a name="p1293611618113"></a><span id="ph17793849835"><a name="ph17793849835"></a><a name="ph17793849835"></a>0x02000224</span></p>
</td>
<td class="cellrowborder" valign="top" width="23.762376237623762%" headers="mcps1.1.6.1.4 "><p id="p193631620118"><a name="p193631620118"></a><a name="p193631620118"></a><span id="ph224264312315"><a name="ph224264312315"></a><a name="ph224264312315"></a>任务在中断中进行Yield操作</span></p>
</td>
<td class="cellrowborder" valign="top" width="25.742574257425744%" headers="mcps1.1.6.1.5 "><p id="p19362161516"><a name="p19362161516"></a><a name="p19362161516"></a><span id="ph101846361132"><a name="ph101846361132"></a><a name="ph101846361132"></a>不要在中断中进行Yield操作</span></p>
</td>
</tr>
</tbody>
</table>

**错误码定义：** 错误码是一个32位的存储单元，31\~24位表示错误等级，23\~16位表示错误码标志，15\~8位代表错误码所属模块，7\~0位表示错误码序号，如下：

```
#define LOS_ERRNO_OS_NORMAL(MID,ERRNO)  \
(LOS_ERRTYPE_NORMAL | LOS_ERRNO_OS_ID | ((UINT32)(MID) << 8) | (ERRNO))
LOS_ERRTYPE_NORMAL ：Define the error level as critical
LOS_ERRNO_OS_ID ：OS error code flag.
MID：OS_MOUDLE_ID
ERRNO：error ID number
```

例如：

```
LOS_ERRNO_TSK_NO_MEMORY  LOS_ERRNO_OS_FATAL(LOS_MOD_TSK, 0x00)
```

>![](public_sys-resources/icon-notice.gif) **须知：**   
>错误码序号 0x16、0x1c、0x0b，未被定义，不可用。  

#### 平台差异性

无。

### 注意事项  

-   创建新任务时，会对之前自删除任务的任务控制块和任务栈进行回收，非自删除任务的控制块和栈在任务删除的时候已经回收。
-   任务名是指针没有分配空间，在设置任务名时，禁止将局部变量的地址赋值给任务名指针。
-   若指定的任务栈大小为0，则使用配置项LOSCFG\_BASE\_CORE\_TSK\_DEFAULT\_STACK\_SIZE指定默认的任务栈大小。
-   任务栈的大小按8字节大小对齐。确定任务栈大小的原则是，够用就行：多了浪费，少了任务栈溢出。
-   挂起任务的时候若为当前任务且已锁任务，则不能被挂起。
-   空闲（IDLE）任务及软件定时器任务不能被挂起或者删除。
-   在中断处理函数中或者在锁任务的情况下，执行LOS\_TaskDelay操作会失败。
-   锁任务调度，并不关中断，因此任务仍可被中断打断。
-   锁任务调度必须和解锁任务调度配合使用。
-   设置任务优先级的时候可能会发生任务调度。
-   系统可配置的任务资源个数是指：整个系统的任务资源总个数，而非用户能使用的任务资源个数。例如：系统软件定时器多占用一个任务资源数，那么系统可配置的任务资源就会减少一个。
-   不建议使用LOS\_CurTaskPriSet或者LOS\_TaskPriSet接口来修改软件定时器任务的优先级，否则可能会导致系统出现问题。
-   LOS\_CurTaskPriSet和LOS\_TaskPriSet接口不能在中断中使用。
-   LOS\_TaskPriGet接口传入的task ID对应的任务未创建或者超过最大任务数，统一返回0xffff。
-   在删除任务时要保证任务申请的资源（如互斥锁、信号量等）已被释放。

### 编程实例

#### 实例描述

下面的示例介绍任务的基本操作方法，包含任务创建、任务延时、任务锁与解锁调度、挂起和恢复、查询当前任务PID、根据PID查询任务信息等操作，阐述任务优先级调度的机制以及各接口的应用。

1.  创建了2个任务:TaskHi和TaskLo。
2.  TaskHi为高优先级任务。
3.  TaskLo为低优先级任务。

#### 编程示例  


```
UINT32 g_uwTskLoID;
UINT32 g_uwTskHiID;
#define TSK_PRIOR_HI 4 
#define TSK_PRIOR_LO 5 
 
UINT32 Example_TaskHi(VOID) 
{ 
    UINT32 uwRet; 
    UINT32 uwCurrentID; 
    TSK_INFO_S stTaskInfo; 
   
     printf("Enter TaskHi Handler.\r\n"); 
 
    /*延时2个Tick，延时后该任务会挂起，执行剩余任务中最高优先级的任务(g_uwTskLoID任务)*/ 
    uwRet = LOS_TaskDelay(2); 
    if (uwRet != LOS_OK) 
    { 
        printf("Delay Task Failed.\r\n"); 
        return LOS_NOK; 
    } 
 
    /*2个Tick时间到了后，该任务恢复，继续执行*/ 
    printf("TaskHi LOS_TaskDelay Done.\r\n"); 
 
    /*挂起自身任务*/ 
    uwRet = LOS_TaskSuspend(g_uwTskHiID); 
    if (uwRet != LOS_OK) 
    { 
        printf("Suspend TaskHi Failed.\r\n"); 
        return LOS_NOK; 
    } 
    printf("TaskHi LOS_TaskResume Success.\r\n"); 
} 
 
/*低优先级任务入口函数*/ 
UINT32 Example_TaskLo(VOID) 
{ 
    UINT32 uwRet; 
    UINT32 uwCurrentID; 
    TSK_INFO_S stTaskInfo; 
 
    printf("Enter TaskLo Handler.\r\n"); 
 
    /*延时2个Tick，延时后该任务会挂起，执行剩余任务中就高优先级的任务(背景任务)*/ 
    uwRet = LOS_TaskDelay(2); 
    if (uwRet != LOS_OK) 
    { 
        printf("Delay TaskLo Failed.\r\n"); 
        return LOS_NOK; 
    } 
 
    printf("TaskHi LOS_TaskSuspend Success.\r\n"); 
 
    /*恢复被挂起的任务g_uwTskHiID*/ 
    uwRet = LOS_TaskResume(g_uwTskHiID); 
    if (uwRet != LOS_OK) 
    { 
        printf("Resume TaskHi Failed.\r\n"); 
        return LOS_NOK; 
    } 
 
    printf("TaskHi LOS_TaskDelete Success.\r\n"); 
} 
 
/*任务测试入口函数，在里面创建优先级不一样的两个任务*/ 
UINT32 Example_TskCaseEntry(VOID) 
{ 
    UINT32 uwRet; 
    TSK_INIT_PARAM_S stInitParam; 
 
    /*锁任务调度*/ 
    LOS_TaskLock(); 
 
    printf("LOS_TaskLock() Success!\r\n"); 
 
    stInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_TaskHi; 
    stInitParam.usTaskPrio = TSK_PRIOR_HI; 
    stInitParam.pcName = "HIGH_NAME"; 
    stInitParam.uwStackSize = 0x700; 
    stInitParam.uwResved   = LOS_TASK_STATUS_DETACHED; 
    /*创建高优先级任务，由于锁任务调度，任务创建成功后不会马上执行*/ 
    uwRet = LOS_TaskCreate(&g_uwTskHiID, &stInitParam); 
    if (uwRet != LOS_OK) 
    { 
        LOS_TaskUnlock(); 
  
        printf("Example_TaskHi create Failed!\r\n"); 
        return LOS_NOK; 
    } 
 
    printf("Example_TaskHi create Success!\r\n"); 
 
    stInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_TaskLo; 
    stInitParam.usTaskPrio = TSK_PRIOR_LO; 
    stInitParam.pcName = "LOW_NAME"; 
    stInitParam.uwStackSize = 0x700; 
    stInitParam.uwResved   = LOS_TASK_STATUS_DETACHED; 
    /*创建低优先级任务，由于锁任务调度，任务创建成功后不会马上执行*/ 
    uwRet = LOS_TaskCreate(&g_uwTskLoID, &stInitParam); 
    if (uwRet != LOS_OK) 
    { 
        LOS_TaskUnlock(); 
 
        printf("Example_TaskLo create Failed!\r\n"); 
        return LOS_NOK; 
    } 
 
    printf("Example_TaskLo create Success!\r\n"); 
 
    /*解锁任务调度，此时会发生任务调度，执行就绪列表中最高优先级任务*/ 
    LOS_TaskUnlock(); 
 
    while(1){}; 
 
    return LOS_OK; 
} 
```

#### 结果验证

编译运行得到的结果为：

![](figures/zh-cn_image_0238585887.jpg)

#### 完整实例代码

[sample\_task\_smp.c](resource/sample_task_smp.c)

<h3 id="6">4.2 内存</h3>

### 概述  

#### 基本概念  

内存管理模块管理系统的内存资源，它是操作系统的核心模块之一。主要包括内存的初始化、分配以及释放。

在系统运行过程中，内存管理模块通过对内存的申请/释放操作，来管理用户和OS对内存的使用，使内存的利用率和使用效率达到最优，同时最大限度地解决系统的内存碎片问题。

Huawei LiteOS的内存管理分为静态内存管理和动态内存管理，提供内存初始化、分配、释放等功能。

**动态内存：** 在动态内存池中分配用户指定大小的内存块。

-   优点：按需分配。
-   缺点：内存池中可能出现碎片。

**静态内存：** 在静态内存池中分配用户初始化时预设（固定）大小的内存块。

-   优点：分配和释放效率高，静态内存池中无碎片。
-   缺点：只能申请到初始化预设大小的内存块，不能按需申请。

#### 动态内存运作机制

动态内存管理，即在内存资源充足的情况下，从系统配置的一块比较大的连续内存（内存池），根据用户需求，分配任意大小的内存块。当用户不需要该内存块时，又可以释放回系统供下一次使用。

与静态内存相比，动态内存管理的好处是按需分配，缺点是内存池中容易出现碎片。

LiteOS动态内存支持DLINK和BEST LITTLE两种标准算法。

**1.DLINK**

DLINK动态内存管理结构如图1所示：

**图 1** DLINK动态内存管理结构图<a name="fe27f90bf1ff042a885a9af7d3345a03c"></a>  
![](figures/zh-cn_image_0237405366.png "zh-cn_image_0237405366")

第一部分：堆内存（也称内存池）的起始地址及堆区域总大小。

第二部分：本身是一个数组，每个元素是一个双向链表，所有free节点的控制头都会被分类挂在这个数组的双向链表中。

假设内存允许的最小节点为2<sup>min</sup>字节，则数组的第一个双向链表存储的是所有size为2<sup>min</sup><size< 2<sup>min+1</sup>的free节点，第二个双向链表存储的是所有size为2<sup>min+1</sup><size< 2<sup>min+2</sup>的free节点，依次类推第n个双向链表存储的是所有size为2<sup>min+n-1</sup><size< 2<sup>min+n</sup>的free节点。每次申请内存的时候，会从这个数组检索最合适大小的free节点，进行分配内存。每次释放内存时，会将该片内存作为free节点存储至这个数组，以便下次再利用。

第三部分：占用内存池极大部分的空间，是用于存放各节点的实际区域。以下是LOS\_MEM\_DYN\_NODE节点结构体申明以及简单介绍：
```  
typedef struct tagLOS_MEM_DYN_NODE

{

LOS_DL_LIST stFreeNodeInfo;

struct tagLOS_MEM_DYN_NODE *pstPreNode;

UINT32 uwSizeAndFlag;

}LOS_MEM_DYN_NODE;
```  

**图 2**  LOS\_MEM\_DYN\_NODE节点结构图<a name="f8eb29f14036c4491a603b5c5493eeb28"></a> 
![](figures/zh-cn_image_0237405430.png "zh-cn_image_0237405430")

**2.BEST LITTLE**

LiteOS的动态内存分配支持最佳适配算法，即BEST LITTLE，每次分配时选择堆内存（内存池中）最小最适合的内存块进行分配。

**图 3**  BEST LITTLE动态内存管理结构图<a name="f8eb29f14036c4491a603b5c5493eeb25"></a> 
![](figures/zh-cn_image_0237405390.png)

#### 静态内存运作机制

静态内存实质上是一块静态数组，静态内存池内的块大小在初始化时设定，初始化后块大小不可变更。

静态内存池由一个控制块和若干相同大小的内存块构成。控制块位于内存池头部，用于内存块管理。内存块的申请和释放以块大小为粒度。

**图 4**  静态内存示意图<a name="fcdbf6e117f844cc3a474e4bda129dc5d"></a>  
![](figures/静态内存示意图.png "静态内存示意图")

### 动态内存

#### 开发指导

##### 使用场景

内存管理的主要工作是动态的划分并管理用户分配好的内存区间。

动态内存管理主要是在用户需要使用大小不等的内存块的场景中使用。

当用户需要分配内存时，可以通过操作系统的动态内存申请函数申请指定大小内存块，一旦使用完毕，通过动态内存释放函数归还所占用内存，使之可以重复使用。

##### 功能

Huawei LiteOS系统中的动态内存管理模块为用户提供下面几种功能，具体的API详见接口手册。

<a name="t125a47f086d1410b863f13b9a275c07f"></a>
<table><thead align="left"><tr id="r2b01fbe37cf947c08bb5965aff0c788e"><th class="cellrowborder" valign="top" width="22.24%" id="mcps1.1.4.1.1"><p id="aa1c332d85d984b3ca44b85444210a214"><a name="aa1c332d85d984b3ca44b85444210a214"></a><a name="aa1c332d85d984b3ca44b85444210a214"></a>功能分类</p>
</th>
<th class="cellrowborder" valign="top" width="40.29%" id="mcps1.1.4.1.2"><p id="a2dffaf83b2894662b99e38092a81f4c0"><a name="a2dffaf83b2894662b99e38092a81f4c0"></a><a name="a2dffaf83b2894662b99e38092a81f4c0"></a>接口名</p>
</th>
<th class="cellrowborder" valign="top" width="37.47%" id="mcps1.1.4.1.3"><p id="a78e2e39f5b894c1d9e717315aceb9aeb"><a name="a78e2e39f5b894c1d9e717315aceb9aeb"></a><a name="a78e2e39f5b894c1d9e717315aceb9aeb"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="r72fb7b96b4ce418e9d2bdde04af4fc13"><td class="cellrowborder" valign="top" width="22.24%" headers="mcps1.1.4.1.1 "><p id="a954c12a0d5bf489883972cf44513c4f1"><a name="a954c12a0d5bf489883972cf44513c4f1"></a><a name="a954c12a0d5bf489883972cf44513c4f1"></a>内存初始化</p>
</td>
<td class="cellrowborder" valign="top" width="40.29%" headers="mcps1.1.4.1.2 "><p id="a829cf4b09c0d4265a2ca340ad53aa583"><a name="a829cf4b09c0d4265a2ca340ad53aa583"></a><a name="a829cf4b09c0d4265a2ca340ad53aa583"></a>LOS_MemInit</p>
</td>
<td class="cellrowborder" valign="top" width="37.47%" headers="mcps1.1.4.1.3 "><p id="a2d3b9a48576f4d41a408aa44adbd71b6"><a name="a2d3b9a48576f4d41a408aa44adbd71b6"></a><a name="a2d3b9a48576f4d41a408aa44adbd71b6"></a>初始化一块指定的动态内存池，大小为size。</p>
</td>
</tr>
<tr id="row13835218236"><td class="cellrowborder" valign="top" width="22.24%" headers="mcps1.1.4.1.1 "><p id="p98361915234"><a name="p98361915234"></a><a name="p98361915234"></a><span id="ph6416842311"><a name="ph6416842311"></a><a name="ph6416842311"></a>删除内存池</span></p>
</td>
<td class="cellrowborder" valign="top" width="40.29%" headers="mcps1.1.4.1.2 "><p id="p4836101132318"><a name="p4836101132318"></a><a name="p4836101132318"></a><span id="ph12221181715234"><a name="ph12221181715234"></a><a name="ph12221181715234"></a>LOS_MemDeInit</span></p>
</td>
<td class="cellrowborder" valign="top" width="37.47%" headers="mcps1.1.4.1.3 "><p id="p18836131162319"><a name="p18836131162319"></a><a name="p18836131162319"></a><span id="ph54965233237"><a name="ph54965233237"></a><a name="ph54965233237"></a>删除指定内存池，仅打开LOSCFG_MEM_MUL_POOL时有效。</span></p>
</td>
</tr>
<tr id="r0ceb453df94040c6910e3f7f90c24632"><td class="cellrowborder" valign="top" width="22.24%" headers="mcps1.1.4.1.1 "><p id="a8683e360fe7342b99183ad2105b8d858"><a name="a8683e360fe7342b99183ad2105b8d858"></a><a name="a8683e360fe7342b99183ad2105b8d858"></a>申请动态内存</p>
</td>
<td class="cellrowborder" valign="top" width="40.29%" headers="mcps1.1.4.1.2 "><p id="a1e008fa34be44b7381ed90bc065b8089"><a name="a1e008fa34be44b7381ed90bc065b8089"></a><a name="a1e008fa34be44b7381ed90bc065b8089"></a>LOS_MemAlloc</p>
</td>
<td class="cellrowborder" valign="top" width="37.47%" headers="mcps1.1.4.1.3 "><p id="add076b0f14be4c9ca570794358bda613"><a name="add076b0f14be4c9ca570794358bda613"></a><a name="add076b0f14be4c9ca570794358bda613"></a>从指定动态内存池中申请size长度的内存。</p>
</td>
</tr>
<tr id="rcc3e77f15eac4652b01bfbdc60cb5a3d"><td class="cellrowborder" valign="top" width="22.24%" headers="mcps1.1.4.1.1 "><p id="ada8e5afc3c634b08a2f60e248b4d38fe"><a name="ada8e5afc3c634b08a2f60e248b4d38fe"></a><a name="ada8e5afc3c634b08a2f60e248b4d38fe"></a>释放动态内存</p>
</td>
<td class="cellrowborder" valign="top" width="40.29%" headers="mcps1.1.4.1.2 "><p id="ab4dfe3734a8948d0b3c9af6f04e24bfc"><a name="ab4dfe3734a8948d0b3c9af6f04e24bfc"></a><a name="ab4dfe3734a8948d0b3c9af6f04e24bfc"></a>LOS_MemFree</p>
</td>
<td class="cellrowborder" valign="top" width="37.47%" headers="mcps1.1.4.1.3 "><p id="ae29511bf62ca419493e1ef77a17ea302"><a name="ae29511bf62ca419493e1ef77a17ea302"></a><a name="ae29511bf62ca419493e1ef77a17ea302"></a>释放已申请的内存。</p>
</td>
</tr>
<tr id="r98fa67eb63014499ac9cb3722b01dee1"><td class="cellrowborder" valign="top" width="22.24%" headers="mcps1.1.4.1.1 "><p id="zh-cn_topic_0015795095_p667340699236"><a name="zh-cn_topic_0015795095_p667340699236"></a><a name="zh-cn_topic_0015795095_p667340699236"></a>重新申请内存</p>
</td>
<td class="cellrowborder" valign="top" width="40.29%" headers="mcps1.1.4.1.2 "><p id="zh-cn_topic_0015795095_p146891489236"><a name="zh-cn_topic_0015795095_p146891489236"></a><a name="zh-cn_topic_0015795095_p146891489236"></a>LOS_MemRealloc</p>
</td>
<td class="cellrowborder" valign="top" width="37.47%" headers="mcps1.1.4.1.3 "><p id="ae3748fec27854334b9ca1e60d32ca1d3"><a name="ae3748fec27854334b9ca1e60d32ca1d3"></a><a name="ae3748fec27854334b9ca1e60d32ca1d3"></a>按size大小重新分配内存块，并保留原内存块内容。</p>
</td>
</tr>
<tr id="r7256447dbbce45c0b4c60b5b143a7f0f"><td class="cellrowborder" valign="top" width="22.24%" headers="mcps1.1.4.1.1 "><p id="aed9fec722d2c42f183d1e4bb751108dc"><a name="aed9fec722d2c42f183d1e4bb751108dc"></a><a name="aed9fec722d2c42f183d1e4bb751108dc"></a>内存对齐分配</p>
</td>
<td class="cellrowborder" valign="top" width="40.29%" headers="mcps1.1.4.1.2 "><p id="aa533678646904b07a296f6fbf2247038"><a name="aa533678646904b07a296f6fbf2247038"></a><a name="aa533678646904b07a296f6fbf2247038"></a>LOS_MemAllocAlign</p>
</td>
<td class="cellrowborder" valign="top" width="37.47%" headers="mcps1.1.4.1.3 "><p id="a32603d87ab034af5a785bd201231c610"><a name="a32603d87ab034af5a785bd201231c610"></a><a name="a32603d87ab034af5a785bd201231c610"></a>从指定动态内存池中申请长度为size且地址按boundary字节对齐的内存。</p>
</td>
</tr>
<tr id="row18919103442415"><td class="cellrowborder" valign="top" width="22.24%" headers="mcps1.1.4.1.1 "><p id="p19191534152417"><a name="p19191534152417"></a><a name="p19191534152417"></a><span id="ph562524913244"><a name="ph562524913244"></a><a name="ph562524913244"></a>获取内存大小</span></p>
</td>
<td class="cellrowborder" valign="top" width="40.29%" headers="mcps1.1.4.1.2 "><p id="p129191634122411"><a name="p129191634122411"></a><a name="p129191634122411"></a><span id="ph258995718249"><a name="ph258995718249"></a><a name="ph258995718249"></a>LOS_MemPoolSizeGet</span></p>
</td>
<td class="cellrowborder" valign="top" width="37.47%" headers="mcps1.1.4.1.3 "><p id="p391914349242"><a name="p391914349242"></a><a name="p391914349242"></a><span id="ph7108642257"><a name="ph7108642257"></a><a name="ph7108642257"></a>获取指定的动态内存池总大</span><span id="ph695714515257"><a name="ph695714515257"></a><a name="ph695714515257"></a>小。</span></p>
</td>
</tr>
<tr id="row355653892419"><td class="cellrowborder" valign="top" width="22.24%" headers="mcps1.1.4.1.1 "><p id="p1955693813248"><a name="p1955693813248"></a><a name="p1955693813248"></a><span id="ph61942015122514"><a name="ph61942015122514"></a><a name="ph61942015122514"></a>获取内存大小</span></p>
</td>
<td class="cellrowborder" valign="top" width="40.29%" headers="mcps1.1.4.1.2 "><p id="p7556113815247"><a name="p7556113815247"></a><a name="p7556113815247"></a><span id="ph2015619238259"><a name="ph2015619238259"></a><a name="ph2015619238259"></a>LOS_MemTotalUsedGet</span></p>
</td>
<td class="cellrowborder" valign="top" width="37.47%" headers="mcps1.1.4.1.3 "><p id="p1155610385241"><a name="p1155610385241"></a><a name="p1155610385241"></a><span id="ph141141231142510"><a name="ph141141231142510"></a><a name="ph141141231142510"></a>获取指定动态内存池的总使用量大小。</span></p>
</td>
</tr>
<tr id="row1586117357257"><td class="cellrowborder" valign="top" width="22.24%" headers="mcps1.1.4.1.1 "><p id="p48616359259"><a name="p48616359259"></a><a name="p48616359259"></a><span id="ph297114019259"><a name="ph297114019259"></a><a name="ph297114019259"></a>获取内存块数量</span></p>
</td>
<td class="cellrowborder" valign="top" width="40.29%" headers="mcps1.1.4.1.2 "><p id="p1486153514253"><a name="p1486153514253"></a><a name="p1486153514253"></a><span id="ph155971147172520"><a name="ph155971147172520"></a><a name="ph155971147172520"></a>LOS_MemFreeBlksGet</span></p>
</td>
<td class="cellrowborder" valign="top" width="37.47%" headers="mcps1.1.4.1.3 "><p id="p128611835162511"><a name="p128611835162511"></a><a name="p128611835162511"></a><span id="ph05795532256"><a name="ph05795532256"></a><a name="ph05795532256"></a>获取指定内存池的空闲内存块数量。</span></p>
</td>
</tr>
<tr id="row12897956142516"><td class="cellrowborder" valign="top" width="22.24%" headers="mcps1.1.4.1.1 "><p id="p48971756152514"><a name="p48971756152514"></a><a name="p48971756152514"></a><span id="ph331410242619"><a name="ph331410242619"></a><a name="ph331410242619"></a>获取内存块数量</span></p>
</td>
<td class="cellrowborder" valign="top" width="40.29%" headers="mcps1.1.4.1.2 "><p id="p13897115610259"><a name="p13897115610259"></a><a name="p13897115610259"></a><span id="ph137481771265"><a name="ph137481771265"></a><a name="ph137481771265"></a>LOS_MemUsedBlksGet</span></p>
</td>
<td class="cellrowborder" valign="top" width="37.47%" headers="mcps1.1.4.1.3 "><p id="p17897456102513"><a name="p17897456102513"></a><a name="p17897456102513"></a><span id="ph13217191418265"><a name="ph13217191418265"></a><a name="ph13217191418265"></a>获取指定内存池的已使用的内存块数量。</span></p>
</td>
</tr>
<tr id="row361815183263"><td class="cellrowborder" valign="top" width="22.24%" headers="mcps1.1.4.1.1 "><p id="p5618131814268"><a name="p5618131814268"></a><a name="p5618131814268"></a><span id="ph094732652620"><a name="ph094732652620"></a><a name="ph094732652620"></a>获取分配指定内存区域的任务ID</span></p>
</td>
<td class="cellrowborder" valign="top" width="40.29%" headers="mcps1.1.4.1.2 "><p id="p176181218192619"><a name="p176181218192619"></a><a name="p176181218192619"></a><span id="ph6661632122613"><a name="ph6661632122613"></a><a name="ph6661632122613"></a>LOS_MemTaskIdGet</span></p>
</td>
<td class="cellrowborder" valign="top" width="37.47%" headers="mcps1.1.4.1.3 "><p id="p8618121815269"><a name="p8618121815269"></a><a name="p8618121815269"></a><span id="ph17156639122615"><a name="ph17156639122615"></a><a name="ph17156639122615"></a>获取分配了指定内存区域的任务ID。</span></p>
</td>
</tr>
<tr id="row3706145413265"><td class="cellrowborder" valign="top" width="22.24%" headers="mcps1.1.4.1.1 "><p id="p07060547261"><a name="p07060547261"></a><a name="p07060547261"></a><span id="ph122866132718"><a name="ph122866132718"></a><a name="ph122866132718"></a>获取内存池中最后一个使用内存块的结束地址</span></p>
</td>
<td class="cellrowborder" valign="top" width="40.29%" headers="mcps1.1.4.1.2 "><p id="p117067541267"><a name="p117067541267"></a><a name="p117067541267"></a><span id="ph195845469276"><a name="ph195845469276"></a><a name="ph195845469276"></a>LOS_MemLastUsedGet</span></p>
</td>
<td class="cellrowborder" valign="top" width="37.47%" headers="mcps1.1.4.1.3 "><p id="p070645419261"><a name="p070645419261"></a><a name="p070645419261"></a><span id="ph542417579279"><a name="ph542417579279"></a><a name="ph542417579279"></a>获取内存池最后一个使用内存块的结束地址。</span></p>
</td>
</tr>
<tr id="row37571419278"><td class="cellrowborder" valign="top" width="22.24%" headers="mcps1.1.4.1.1 "><p id="p67587482710"><a name="p67587482710"></a><a name="p67587482710"></a><span id="ph3570194152813"><a name="ph3570194152813"></a><a name="ph3570194152813"></a>获取内存结构信息</span></p>
</td>
<td class="cellrowborder" valign="top" width="40.29%" headers="mcps1.1.4.1.2 "><p id="p67587472711"><a name="p67587472711"></a><a name="p67587472711"></a><span id="ph7601711192820"><a name="ph7601711192820"></a><a name="ph7601711192820"></a>LOS_MemInfoGet</span></p>
</td>
<td class="cellrowborder" valign="top" width="37.47%" headers="mcps1.1.4.1.3 "><p id="p9758842270"><a name="p9758842270"></a><a name="p9758842270"></a><span id="ph394671613281"><a name="ph394671613281"></a><a name="ph394671613281"></a>获取指定内存池的内存结构信息。</span></p>
</td>
</tr>
<tr id="row155543015273"><td class="cellrowborder" valign="top" width="22.24%" headers="mcps1.1.4.1.1 "><p id="p12554140172715"><a name="p12554140172715"></a><a name="p12554140172715"></a><span id="ph20562635112810"><a name="ph20562635112810"></a><a name="ph20562635112810"></a>对指定内存池做完整性检查</span></p>
</td>
<td class="cellrowborder" valign="top" width="40.29%" headers="mcps1.1.4.1.2 "><p id="p1955415016279"><a name="p1955415016279"></a><a name="p1955415016279"></a><span id="ph1653325314286"><a name="ph1653325314286"></a><a name="ph1653325314286"></a>LOS_MemIntegrityCheck</span></p>
</td>
<td class="cellrowborder" valign="top" width="37.47%" headers="mcps1.1.4.1.3 "><p id="p755413092720"><a name="p755413092720"></a><a name="p755413092720"></a><span id="ph138021959102818"><a name="ph138021959102818"></a><a name="ph138021959102818"></a>对指定内存池做完整性检查。</span></p>
</td>
</tr>
<tr id="row7961127172914"><td class="cellrowborder" valign="top" width="22.24%" headers="mcps1.1.4.1.1 "><p id="p8138924102918"><a name="p8138924102918"></a><a name="p8138924102918"></a>获取内存块大小</p>
</td>
<td class="cellrowborder" valign="top" width="40.29%" headers="mcps1.1.4.1.2 "><p id="p1813862492911"><a name="p1813862492911"></a><a name="p1813862492911"></a>LOS_MemNodeSizeCheck</p>
</td>
<td class="cellrowborder" valign="top" width="37.47%" headers="mcps1.1.4.1.3 "><p id="p13138122412915"><a name="p13138122412915"></a><a name="p13138122412915"></a>获取内存块的总大小和可操作大小。</p>
</td>
</tr>
<tr id="row39515275296"><td class="cellrowborder" valign="top" width="22.24%" headers="mcps1.1.4.1.1 "><p id="p1413872414298"><a name="p1413872414298"></a><a name="p1413872414298"></a>设定内存检查级别</p>
</td>
<td class="cellrowborder" valign="top" width="40.29%" headers="mcps1.1.4.1.2 "><p id="p5138224172912"><a name="p5138224172912"></a><a name="p5138224172912"></a>LOS_MemCheckLevelSet</p>
</td>
<td class="cellrowborder" valign="top" width="37.47%" headers="mcps1.1.4.1.3 "><p id="p9138224182919"><a name="p9138224182919"></a><a name="p9138224182919"></a>设定内存检查级别。</p>
</td>
</tr>
<tr id="row1095152732911"><td class="cellrowborder" valign="top" width="22.24%" headers="mcps1.1.4.1.1 "><p id="p5138824132920"><a name="p5138824132920"></a><a name="p5138824132920"></a>获取内存检查级别</p>
</td>
<td class="cellrowborder" valign="top" width="40.29%" headers="mcps1.1.4.1.2 "><p id="p1613819243291"><a name="p1613819243291"></a><a name="p1613819243291"></a>LOS_MemCheckLevelGet</p>
</td>
<td class="cellrowborder" valign="top" width="37.47%" headers="mcps1.1.4.1.3 "><p id="p191381524152920"><a name="p191381524152920"></a><a name="p191381524152920"></a>获取内存检查级别。</p>
</td>
</tr>
<tr id="row4951527152910"><td class="cellrowborder" valign="top" width="22.24%" headers="mcps1.1.4.1.1 "><p id="p713862422918"><a name="p713862422918"></a><a name="p713862422918"></a>显示系统内存池</p>
</td>
<td class="cellrowborder" valign="top" width="40.29%" headers="mcps1.1.4.1.2 "><p id="p12138102452912"><a name="p12138102452912"></a><a name="p12138102452912"></a>LOS_MemPoolList</p>
</td>
<td class="cellrowborder" valign="top" width="37.47%" headers="mcps1.1.4.1.3 "><p id="p613842492913"><a name="p613842492913"></a><a name="p613842492913"></a>打印显示系统中已初始化的所有内存池，仅打开LOSCFG_MEM_MUL_POOL时有效</p>
</td>
</tr>
<tr id="row69515275297"><td class="cellrowborder" valign="top" width="22.24%" headers="mcps1.1.4.1.1 "><p id="p1213842414291"><a name="p1213842414291"></a><a name="p1213842414291"></a>指定模块申请动态内存</p>
</td>
<td class="cellrowborder" valign="top" width="40.29%" headers="mcps1.1.4.1.2 "><p id="p3138152472913"><a name="p3138152472913"></a><a name="p3138152472913"></a>LOS_MemMalloc</p>
</td>
<td class="cellrowborder" valign="top" width="37.47%" headers="mcps1.1.4.1.3 "><p id="p15138132417292"><a name="p15138132417292"></a><a name="p15138132417292"></a>从指定动态内存池分配内存给指定的模块，并纳入模块统计。</p>
</td>
</tr>
<tr id="row29520273293"><td class="cellrowborder" valign="top" width="22.24%" headers="mcps1.1.4.1.1 "><p id="p81385242291"><a name="p81385242291"></a><a name="p81385242291"></a>释放指定模块的一个内存块</p>
</td>
<td class="cellrowborder" valign="top" width="40.29%" headers="mcps1.1.4.1.2 "><p id="p1913811244295"><a name="p1913811244295"></a><a name="p1913811244295"></a>LOS_MemMfree</p>
</td>
<td class="cellrowborder" valign="top" width="37.47%" headers="mcps1.1.4.1.3 "><p id="p14138142417299"><a name="p14138142417299"></a><a name="p14138142417299"></a>释放已经申请的内存块，并纳入模块统计。</p>
</td>
</tr>
<tr id="row4956276293"><td class="cellrowborder" valign="top" width="22.24%" headers="mcps1.1.4.1.1 "><p id="p13138112410292"><a name="p13138112410292"></a><a name="p13138112410292"></a>指定模块内存对齐分配</p>
</td>
<td class="cellrowborder" valign="top" width="40.29%" headers="mcps1.1.4.1.2 "><p id="p9138142417298"><a name="p9138142417298"></a><a name="p9138142417298"></a>LOS_MemMallocAlign</p>
</td>
<td class="cellrowborder" valign="top" width="37.47%" headers="mcps1.1.4.1.3 "><p id="p1413892472917"><a name="p1413892472917"></a><a name="p1413892472917"></a>从指定动态内存池中申请长度为size且地址按boundary字节对齐的内存，并纳入模块统计。</p>
</td>
</tr>
<tr id="row1395102772910"><td class="cellrowborder" valign="top" width="22.24%" headers="mcps1.1.4.1.1 "><p id="p1913814247299"><a name="p1913814247299"></a><a name="p1913814247299"></a>指定模块重新分配内存块</p>
</td>
<td class="cellrowborder" valign="top" width="40.29%" headers="mcps1.1.4.1.2 "><p id="p0138182410291"><a name="p0138182410291"></a><a name="p0138182410291"></a>LOS_MemMrealloc</p>
</td>
<td class="cellrowborder" valign="top" width="37.47%" headers="mcps1.1.4.1.3 "><p id="p713816242294"><a name="p713816242294"></a><a name="p713816242294"></a>按size大小重新分配内存块，并保留原内存块内容，并纳入模块统计。</p>
</td>
</tr>
<tr id="row86483582307"><td class="cellrowborder" valign="top" width="22.24%" headers="mcps1.1.4.1.1 "><p id="p665905718301"><a name="p665905718301"></a><a name="p665905718301"></a>获取模块内存使用量</p>
</td>
<td class="cellrowborder" valign="top" width="40.29%" headers="mcps1.1.4.1.2 "><p id="p76591457183010"><a name="p76591457183010"></a><a name="p76591457183010"></a>LOS_MemMusedGet</p>
</td>
<td class="cellrowborder" valign="top" width="37.47%" headers="mcps1.1.4.1.3 "><p id="p1265925763019"><a name="p1265925763019"></a><a name="p1265925763019"></a>获取指定模块当前内存使用量。</p>
</td>
</tr>
<tr id="row1064810586305"><td class="cellrowborder" valign="top" width="22.24%" headers="mcps1.1.4.1.1 "><p id="p10659557103015"><a name="p10659557103015"></a><a name="p10659557103015"></a>显示空闲内存块的规模和数量</p>
</td>
<td class="cellrowborder" valign="top" width="40.29%" headers="mcps1.1.4.1.2 "><p id="p6659125718309"><a name="p6659125718309"></a><a name="p6659125718309"></a>LOS_MemFreeNodeShow</p>
</td>
<td class="cellrowborder" valign="top" width="37.47%" headers="mcps1.1.4.1.3 "><p id="p1265914576305"><a name="p1265914576305"></a><a name="p1265914576305"></a>显示指定内存池当前空闲内存块的规模及相对应的数量。</p>
</td>
</tr>
</tbody>
</table>

##### DLINK开发流程

1.  配置：

    OS\_SYS\_MEM\_ADDR：系统动态内存池起始地址，一般不需要修改。

    OS\_SYS\_MEM\_SIZE：系统动态内存池大小，以byte为单位，系统默认分配DDR后未使用的空间。

    LOSCFG\_BASE\_MEM\_NODE\_INTEGRITY\_CHECK：内存越界检测开关，默认关闭。打开后，每次申请动态内存时执行动态内存块越界检查。

2.  初始化LOS\_MemInit。

    初始一个内存池后如图，生成一个 EndNode，并且剩余的内存全部被标记为FreeNode节点。注：EndNode作为内存池末尾的节点，size为0。

    ![](figures/zh-cn_image_0238925517.jpg)

3.  申请任意大小的动态内存LOS\_MemAlloc。

    判断动态内存池中是否存在申请量大小的空间，若存在，则划出一块内存块，以指针形式返回，若不存在，返回NULL。

    调用三次LOS\_MemAlloc函数可以创建三个节点,假设名称分别为UsedA，UsedB，UsedC，大小分别为sizeA，sizeB，sizeC。因为刚初始化内存池的时候只有一个大的FreeNode，所以这些内存块是从这个FreeNode中切割出来的。

    ![](figures/zh-cn_image_0237405378.jpg)

    当内存池中存在多个FreeNode的时候进行malloc，将会适配最合适大小的FreeNode用来新建内存块，减少内存碎片。若新建的内存块不等于被使用的FreeNode的大小，则在新建内存块后，多余的内存又会被标记为一个新的FreeNode。

4.  释放动态内存LOS\_MemFree。

    回收内存块，供下一次使用。

    假设调用LOS\_MemFree释放内存块UsedB，则会回收内存块UsedB，并且将其标记为FreeNode。

    ![](figures/zh-cn_image_0237405424.jpg)


##### BEST LITTLE开发流程

1. 配置：

   OS\_SYS\_MEM\_ADDR：系统动态内存池起始地址，需要用户指定。

   OS\_SYS\_MEM\_SIZE：系统动态内存池大小，以byte为单位，需要用户正确计算。

   LOSCFG\_MEMORY\_BESTFIT：置为YES，选择内存管理算法中的BESTFIT算法。

2. 初始化：

   调用LOS\_MemInit函数初始化用户指定的动态内存池。

3. 申请任意大小的动态内存：

   调用LOS\_MemAlloc函数从指定的内存池中申请指定大小的内存块，申请时内存管理向堆内存空间申请，最后将申请结果返回给用户。在向堆内存空间申请时，会存在内存块的切分。

4. 释放动态内存：

   调用LOS\_MemFree函数向指定的动态内存池释放指定的内存块，向堆内存空间释放内存块。在向堆内存空间释放时，会存在内存块的合并。

##### 平台差异性   

无。

#### 注意事项

-   由于系统中动态内存管理需要消耗管理控制块结构的内存，故实际用户可使用空间总量小于在配置文件los\_config.h中配置项OS\_SYS\_MEM\_SIZE的大小。
-   系统中地址对齐申请内存分配LOS\_MemAllocAlign可能会消耗部分对齐导致的空间，故存在一些内存碎片，当系统释放该对齐内存时，同时回收由于对齐导致的内存碎片。
-   系统中重新分配内存LOS\_MemRealloc函数如果分配成功，系统会自己判定是否需要释放原来申请的空间，返回重新分配的空间。如果重新分配失败，原来的空间保持不变，并返回NULL。禁止使用pPtr = LOS\_MemRealloc\(pool, pPtr, uwSize\); 即：不能使用原本的pPtr变量直接去接收返回值。

-   系统中多次调用LOS\_MemFree时，第一次会返回成功，但对同一块内存进行多次重复释放会导致非法指针操作，导致结果不可预知。
-   由于系统动态内存管理内存节点控制块结构体中，节点size数据类型为UINT32，高两位为标志位，因此用户初始化内存池的大小不能超过1G，否则会出现不可预知结果。
-   分模块内存统计依赖于LOSCFG\_MEM\_MUL\_MODULE，使用时需要在配置文件中定义此宏。

#### 编程实例

##### 实例描述

Huawei LiteOS运行期间，用户需要频繁的使用内存资源，而内存资源有限，必须确保将有限的内存资源分配给急需的程序，同时释放不用的内存。

通过Huawei LiteOS内存管理模块可以保证高效、正确的申请、释放内存。

本实例执行以下步骤：

1.  初始化一个动态内存池。
2.  在动态内存池中申请一个内存块。
3.  使用这块内存块存放一个数据。
4.  打印出存放在内存块中的数据。
5.  释放掉这块内存。

##### 编程实例  

```
 UINT8 * m_aucSysMem_Tmp; 
 VOID los_memory_test() { 
     UINT32 *p_num = NULL; 
     UINT32 uwRet; 
     uwRet = LOS_MemInit(m_aucSysMem_Tmp, 32); 
     if (LOS_OK  == uwRet) { 
         dprintf("内存池初始化成功!\n"); 
     } 
     else { 
         dprintf("内存池初始化失败!\n"); 
         return; 
     } 
     /*分配内存*/ 
     p_num = (int*)LOS_MemAlloc(m_aucSysMem_Tmp, 4); 
     if (NULL == p_num) { 
         dprintf("内存分配失败!\n"); 
         return; 
     } 
     dprintf("内存分配成功\n"); 
     /*赋值*/ 
     *p_num = 828; 
     dprintf("*p_num = %d\n", *p_num); 
     /*释放内存*/ 
     uwRet = LOS_MemFree(m_aucSysMem_Tmp, p_num); 
     if (LOS_OK == uwRet) { 
         dprintf("内存释放成功!\n"); 
     } 
     else { 
         dprintf("内存释放失败!\n"); 
     } 
     return; 
 }
```

##### 结果验证 

**图 1**  结果显示<a name="f4f4b67c923f7408ebb57e053a718d6bc"></a>  


![](figures/zh-cn_image_0238601438.gif)

##### 完整实例代码

[sample\_mem.c](resource/sample_mem.c)

### 静态内存  

#### 开发指导  

##### 使用场景


当用户需要使用固定长度的内存时，可以使用静态内存分配的方式获取内存，一旦使用完毕，通过静态内存释放函数归还所占用内存，使之可以重复使用。

##### 功能  

Huawei LiteOS的静态内存管理主要为用户提供以下功能。

<a name="tc6dde07c1ae944a99d48b3f355457357"></a>
<table><thead align="left"><tr id="r61b9400800e44f2f8bef7d1f273f11c3"><th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.1.4.1.1"><p id="a9d1a371e8b5a4eafa3e3f5b18739ef41"><a name="a9d1a371e8b5a4eafa3e3f5b18739ef41"></a><a name="a9d1a371e8b5a4eafa3e3f5b18739ef41"></a>功能分类</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.1.4.1.2"><p id="ad21e1154caf54b398f5d598d5a50a441"><a name="ad21e1154caf54b398f5d598d5a50a441"></a><a name="ad21e1154caf54b398f5d598d5a50a441"></a>接口名</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.1.4.1.3"><p id="a479228b39fdb4c2abed9b4ae6b135c50"><a name="a479228b39fdb4c2abed9b4ae6b135c50"></a><a name="a479228b39fdb4c2abed9b4ae6b135c50"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="r6ef60a272e0b43d38cea792a6e8529b8"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.1 "><p id="a2727eee8b77e4c3886cd6fa7bfd0e8a9"><a name="a2727eee8b77e4c3886cd6fa7bfd0e8a9"></a><a name="a2727eee8b77e4c3886cd6fa7bfd0e8a9"></a>初始化静态内存</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.2 "><p id="a3dc28103f912493492537fa59ed0335f"><a name="a3dc28103f912493492537fa59ed0335f"></a><a name="a3dc28103f912493492537fa59ed0335f"></a>LOS_MemboxInit</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.3 "><p id="aad5af460dd06401d8aa16d421ed6b356"><a name="aad5af460dd06401d8aa16d421ed6b356"></a><a name="aad5af460dd06401d8aa16d421ed6b356"></a>初始化一个静态内存池，设定其起始地址、总大小及每个块大小。</p>
</td>
</tr>
<tr id="r5fde81115e79470f987c5312a4a65110"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.1 "><p id="ae440fd22161b42aab782d9d0e18c4554"><a name="ae440fd22161b42aab782d9d0e18c4554"></a><a name="ae440fd22161b42aab782d9d0e18c4554"></a>清除静态内存内容</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.2 "><p id="a1bdbc3c5d7ce42fa9a91b33043b7b141"><a name="a1bdbc3c5d7ce42fa9a91b33043b7b141"></a><a name="a1bdbc3c5d7ce42fa9a91b33043b7b141"></a>LOS_MemboxClr</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.3 "><p id="a8420ff816d344277b19a393bc367d0bc"><a name="a8420ff816d344277b19a393bc367d0bc"></a><a name="a8420ff816d344277b19a393bc367d0bc"></a>清零静态内存块。</p>
</td>
</tr>
<tr id="r1c3c648669474e38aa2241ec051ba582"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.1 "><p id="a322ffc6b7b044b44a41a66b59f750568"><a name="a322ffc6b7b044b44a41a66b59f750568"></a><a name="a322ffc6b7b044b44a41a66b59f750568"></a>申请一块静态内存</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.2 "><p id="a767794c2b307401ea9371a1a7b03a2b6"><a name="a767794c2b307401ea9371a1a7b03a2b6"></a><a name="a767794c2b307401ea9371a1a7b03a2b6"></a>LOS_MemboxAlloc</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.3 "><p id="ac412a636dd754784bc46b19a9fa24953"><a name="ac412a636dd754784bc46b19a9fa24953"></a><a name="ac412a636dd754784bc46b19a9fa24953"></a>申请一块静态内存块。</p>
</td>
</tr>
<tr id="rbc3b178e131346e39b23e46b9b81200c"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.1 "><p id="a437055e4f4244327a0f5b838cac30f69"><a name="a437055e4f4244327a0f5b838cac30f69"></a><a name="a437055e4f4244327a0f5b838cac30f69"></a>释放内存</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.2 "><p id="a81321216db28460491e18ca6fb3c14eb"><a name="a81321216db28460491e18ca6fb3c14eb"></a><a name="a81321216db28460491e18ca6fb3c14eb"></a>LOS_MemboxFree</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.3 "><p id="aeed69061656e452a891e567bd0bae550"><a name="aeed69061656e452a891e567bd0bae550"></a><a name="aeed69061656e452a891e567bd0bae550"></a>释放一个静态内存块。</p>
</td>
</tr>
<tr id="row15506133010112"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.1 "><p id="p450615301518"><a name="p450615301518"></a><a name="p450615301518"></a>分析静态内存池状态</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.2 "><p id="p950663017112"><a name="p950663017112"></a><a name="p950663017112"></a>LOS_MemboxStatisticsGet</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.3 "><p id="p7506530019"><a name="p7506530019"></a><a name="p7506530019"></a>获取静态内存池的统计信息。</p>
</td>
</tr>
<tr id="row4323134834311"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.1 "><p id="p2239204710434"><a name="p2239204710434"></a><a name="p2239204710434"></a>打印静态内存池内容</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.2 "><p id="p14239447184317"><a name="p14239447184317"></a><a name="p14239447184317"></a>LOS_ShowBox</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.3 "><p id="p923911476431"><a name="p923911476431"></a><a name="p923911476431"></a>打印指定静态内存池所有节点信息（打印等级是LOS_INFO_LEVEL）</p>
</td>
</tr>
</tbody>
</table>

#### 开发流程

本节介绍使用静态内存的典型场景开发流程。

1.  规划一片内存区域作为静态内存池。
2.  调用LOS\_MemboxInit接口。

    系统内部将会初始化静态内存池。将入参指定的内存区域分割为N块（N值取决于静态内存总大小和块大小），将所有内存块挂到空闲链表，在内存起始处放置控制头。

3.  调用LOS\_MemboxAlloc接口。

    系统内部将会从空闲链表中获取第一个空闲块，并返回该块的用户空间地址。

4.  调用LOS\_MemboxFree接口。

    将该块内存加入空闲块链表。

5.  调用LOS\_MemboxClr接口。

    系统内部清零静态内存块，将入参地址对应的内存块清零。

#### 平台差异性  

无。

#### 注意事项  

-   静态内存池区域，可以通过定义全局数组或调用动态内存分配接口方式获取。如果使用动态内存分配方式，在不需要静态内存池时，注意要释放该段内存，避免内存泄露。

#### 编程实例  

##### 实例描述  

Huawei LiteOS运行期间，用户需要频繁的使用内存资源，而内存资源有限，必须确保将有限的内存资源分配给急需的程序，同时释放不用的内存。

通过内存管理模块可以保证正确且高效的申请释放内存。

本实例执行以下步骤：

1.  初始化一个静态内存池。
2.  从静态内存池中申请一块静态内存。
3.  使用这块内存块存放一个数据。
4.  打印出存放在内存块中的数据。
5.  清除内存块中的数据。
6.  释放掉这块内存。

##### 编程实例

```
VOID los_membox_test(void) {
    UINT32 *p_num = NULL; 
    UINT32 uwBlkSize = 10, uwBoxSize = 100; 
    UINT32 uwRet; 
    UINT32 pBoxMem[1000]; 
    uwRet = LOS_MemboxInit(&pBoxMem[0], uwBoxSize, uwBlkSize); 
    if (uwRet != LOS_OK) 
    { 
        dprintf("内存池初始化失败!\n"); 
        return; 
    } 
    else { 
        dprintf("内存池初始化成功!\n"); 
    } 
     
    /*申请内存块*/ 
    p_num = (int*)LOS_MemboxAlloc(pBoxMem); 
    if (NULL == p_num) { 
        dprintf("内存分配失败!\n"); 
        return; 
    } 
    dprintf("内存分配成功\n"); 
    /*赋值*/ 
    *p_num = 828; 
    dprintf("*p_num = %d\n", *p_num); 
     /*清除内存内容*/ 
     LOS_MemboxClr(pBoxMem, p_num); 
     dprintf("清除内存内容成功\n *p_num = %d\n", *p_num); 
    /*释放内存*/ 
    uwRet = LOS_MemboxFree(pBoxMem, p_num); 
    if (LOS_OK == uwRet) { 
        dprintf("内存释放成功!\n"); 
    } 
    else{ 
        dprintf("内存释放失败!\n"); 
    } 
     return; 
}
```

##### 结果验证

**图 1**  结果显示<a name="f4d2fe363cb39425bade25d2435a240e4"></a>  


![](figures/l00167098-中软欧拉开发部-iCOS-image-dd30deb7-1840-4cfc-b51d-e55d71b78f71.png)

##### 完整实例代码

[sample\_membox.c](resource/sample_membox.c)

<h3 id="7">4.3 中断机制</h3>

### 概述


中断是指出现需要时，CPU暂停执行当前程序，转而执行新程序的过程。即在程序运行过程中，系统出现了一个必须由CPU立即处理的事务。此时，CPU暂时中止当前程序的执行转而处理这个事务，这个过程就叫做中断。

众所周知，CPU的处理速度比外设的运行速度快很多，外设可以在没有CPU介入的情况下完成一定的工作，但某些情况下需要CPU为其做一定的工作。

通过中断机制，在外设不需要CPU介入时，CPU可以执行其它任务，而当外设需要CPU时通过产生中断信号使CPU立即中断当前任务来响应中断请求。这样可以使CPU避免把大量时间耗费在等待、查询外设状态的操作上，因此将大大提高系统实时性以及执行效率。

Huawei LiteOS的中断支持：

-   中断初始化
-   中断创建
-   开/关中断
-   恢复中断
-   中断使能
-   中断屏蔽

**中断的介绍**

与中断相关的硬件可以划分为三类：设备、中断控制器、CPU本身。

设备：发起中断的源，当设备需要请求CPU时，产生一个中断信号，该信号连接至中断控制器。

中断控制器：中断控制器是CPU众多外设中的一个，它一方面接收其它外设中断引脚的输入，另一方面，它会发出中断信号给CPU。可以通过对中断控制器编程实现对中断源的优先级、触发方式、打开和关闭源等设置操作。常用的中断控制器有VIC（Vector Interrupt Controller）和GIC（General Interrupt Controller），在ARM Cortex-M系列中使用的中断控制器是NVIC（Nested Vector Interrupt Controller）。在ARM Cortex-A7中使用的中断控制器是GIC。

CPU：CPU会响应中断源的请求，中断当前正在执行的任务，转而执行中断处理程序。

**和中断相关的名词解释**

中断号：每个中断请求信号都会有特定的标志，使得计算机能够判断是哪个设备提出的中断请求，这个标志就是中断号。

中断请求：“紧急事件”需向CPU提出申请（发一个电脉冲信号），要求中断，及要求CPU暂停当前执行的任务，转而处理该“紧急事件”，这一申请过程称为中断申请。

中断优先级：为使系统能够及时响应并处理所有中断，系统根据中断时间的重要性和紧迫程度，将中断源分为若干个级别，称作中断优先级。Huawei LiteOS支持中断控制器的中断优先级及中断嵌套，同时中断管理未对优先级和嵌套进行限制。Huawei LiteOS中所有的中断源优先级相同，不支持中断嵌套或抢占。

中断处理程序：当外设产生中断请求后，CPU暂停当前的任务，转而响应中断申请，即执行中断处理程序。

中断触发：中断源发出并送给CPU控制信号，将接口卡上的中断触发器置“1”，表明该中断源产生了中断，要求CPU去响应该中断,CPU暂停当前任务，执行相应的中断处理程序。

中断触发类型：外部中断申请通过一个物理信号发送到NVIC或GIC，可以是电平触发或边沿触发。

中断向量：中断服务程序的入口地址。

中断向量表：存储中断向量的存储区，中断向量与中断号对应，中断向量在中断向量表中按照中断号顺序存储。

中断共享：当外设较少时，可以实现一个外设对应一个中断号，但为了支持更多的硬件设备，可以让多个设备共享一个中断号，共享同一个中断的中断处理程序形成一个链表，当外部设备产生中断申请时，系统会遍历中断号对应的中断处理程序链表。

中断底半部：中断处理程序耗时应尽可能短，以满足中断的快速响应，为了平衡中断处理程序的性能与工作量，将中断处理程序分解为两部分：顶半部和底半部。

顶半部完成尽可能少的比较紧急的任务，它往往只是简单地读取寄存器中的中断状态并清除中断标志位即进行“登记工作”，将耗时的底半部处理程序挂到系统的底半部执行队列中去。

### 开发指导  

#### 使用场景

当有中断请求产生时，CPU暂停当前的任务，转而去响应外设请求。根据需要，用户通过中断申请，注册中断处理程序，可以指定CPU响应中断请求时所执行的具体操作。

#### 功能  

Huawei LiteOS 系统中的中断模块为用户提供下面几种功能。

<a name="table51790631141320"></a>
<table><thead align="left"><tr id="row38334328141320"><th class="cellrowborder" valign="top" width="41.74%" id="mcps1.1.3.1.1"><p id="p21797599141320"><a name="p21797599141320"></a><a name="p21797599141320"></a><strong id="b61960666141320"><a name="b61960666141320"></a><a name="b61960666141320"></a>接口名</strong></p>
</th>
<th class="cellrowborder" valign="top" width="58.26%" id="mcps1.1.3.1.2"><p id="p52758030141320"><a name="p52758030141320"></a><a name="p52758030141320"></a><strong id="b5060225141320"><a name="b5060225141320"></a><a name="b5060225141320"></a>描述</strong></p>
</th>
</tr>
</thead>
<tbody><tr id="row61775481141320"><td class="cellrowborder" valign="top" width="41.74%" headers="mcps1.1.3.1.1 "><p id="p38504745141320"><a name="p38504745141320"></a><a name="p38504745141320"></a>LOS_HwiCreate</p>
</td>
<td class="cellrowborder" valign="top" width="58.26%" headers="mcps1.1.3.1.2 "><p id="p31876670141320"><a name="p31876670141320"></a><a name="p31876670141320"></a>硬中断创建，注册硬中断处理程序</p>
</td>
</tr>
<tr id="row19581813142446"><td class="cellrowborder" valign="top" width="41.74%" headers="mcps1.1.3.1.1 "><p id="p42623060142446"><a name="p42623060142446"></a><a name="p42623060142446"></a>LOS_IntUnLock</p>
</td>
<td class="cellrowborder" valign="top" width="58.26%" headers="mcps1.1.3.1.2 "><p id="p29915868142446"><a name="p29915868142446"></a><a name="p29915868142446"></a>开中断</p>
</td>
</tr>
<tr id="row18454580141320"><td class="cellrowborder" valign="top" width="41.74%" headers="mcps1.1.3.1.1 "><p id="p18426045141320"><a name="p18426045141320"></a><a name="p18426045141320"></a>LOS_IntRestore</p>
</td>
<td class="cellrowborder" valign="top" width="58.26%" headers="mcps1.1.3.1.2 "><p id="p53066965142013"><a name="p53066965142013"></a><a name="p53066965142013"></a>恢复到关中断之前的状态</p>
</td>
</tr>
<tr id="row56310728142633"><td class="cellrowborder" valign="top" width="41.74%" headers="mcps1.1.3.1.1 "><p id="p64875085142633"><a name="p64875085142633"></a><a name="p64875085142633"></a>LOS_IntLock</p>
</td>
<td class="cellrowborder" valign="top" width="58.26%" headers="mcps1.1.3.1.2 "><p id="p20390510142633"><a name="p20390510142633"></a><a name="p20390510142633"></a>关中断</p>
</td>
</tr>
<tr id="row16367543528"><td class="cellrowborder" valign="top" width="41.74%" headers="mcps1.1.3.1.1 "><p id="p465519533526"><a name="p465519533526"></a><a name="p465519533526"></a>HalIrqMask</p>
</td>
<td class="cellrowborder" valign="top" width="58.26%" headers="mcps1.1.3.1.2 "><p id="p265518535529"><a name="p265518535529"></a><a name="p265518535529"></a>中断屏蔽（通过设置寄存器，禁止CPU响应该中断）</p>
</td>
</tr>
<tr id="row254335205313"><td class="cellrowborder" valign="top" width="41.74%" headers="mcps1.1.3.1.1 "><p id="p172131446537"><a name="p172131446537"></a><a name="p172131446537"></a>HalIrqUnmask</p>
</td>
<td class="cellrowborder" valign="top" width="58.26%" headers="mcps1.1.3.1.2 "><p id="p2213184115314"><a name="p2213184115314"></a><a name="p2213184115314"></a>中断使能（通过设置寄存器，允许CPU响应该中断）</p>
</td>
</tr>
<tr id="row1354265145312"><td class="cellrowborder" valign="top" width="41.74%" headers="mcps1.1.3.1.1 "><p id="p192139415539"><a name="p192139415539"></a><a name="p192139415539"></a>HalIrqSetAffinity</p>
</td>
<td class="cellrowborder" valign="top" width="58.26%" headers="mcps1.1.3.1.2 "><p id="p1321354195315"><a name="p1321354195315"></a><a name="p1321354195315"></a>设置中断的亲和性，即设置中断在固定核响应（该函数仅在SMP模式下支持）</p>
</td>
</tr>
</tbody>
</table>

#### 开发流程

1.  修改配置项
    -   打开硬中断裁剪开关：LOSCFG\_PLATFORM\_HWI定义为YES。
    -   配置硬中断使用最大数：LOSCFG\_PLATFORM\_HWI\_LIMIT。

2.  调用中断创建接口LOS\_HwiCreate创建中断。
3.  调用HalIrqUnmask接口使能指定中断。
4.  调用HalIrqMask接口屏蔽指定中断。

### 注意事项

-   根据具体硬件，配置支持的最大中断数及中断初始化操作的寄存器地址。
-   中断共享机制，支持同一中断处理程序的重复挂载，但中断处理程序的入参dev必须唯一，即同一中断号，同一dev只能挂载一次；但同一中断号，同一中断处理程序，dev不同则可以重复挂载。

-   中断处理程序耗时不能过长，影响CPU对中断的及时响应。
-   Cortex-M系列处理器中1-15中断为内部使用，因此不建议用户去申请和创建。

-   中断响应过程中不能执行引起调度的函数。
-   中断恢复LOS\_IntRestore\(\)的入参必须是与之对应的LOS\_IntLock\(\)保存的关中断之前的CPSR的值。
-   Cortex-A7中0-31中断为内部使用，因此不建议用户去申请和创建。
-   一般不直接调用LOS\_HwiCreate\(\)创建中断；建议使用系统compat中的linux适配接口request\_irq创建中断。

### 编程实例  

#### 实例描述  

本实例实现如下功能：

1.  关中断
2.  中断创建
3.  中断使能
4.  中断恢复
5.  中断屏蔽

#### 编程示例

**前提条件：**

-   在los\_config.h中，将LOSCFG\_PLATFORM\_HWI定义为YES。
-   在los\_config.h中，设置最大硬中断个数LOSCFG\_PLATFORM\_HWI\_LIMIT

**说明：** 目前的中断测试代码提供了基本框架，中断硬件初始化代码请用户根据开发板硬件情况在Example\_Exti0\_Init\(\)函数中自行实现。

代码实现如下：

```
 #include "los_hwi.h" 
 #include "los_typedef.h" 
 #define HWI_NUM_INT50 50 
 void uart_irqhandle(int irq,void *dev) 
 { 
     printf("\n int the func uart_irqhandle \n"); 
 } 
 void hwi_test() 
 { 
     int a = 1; 
     UINTPTR uvIntSave; 
     uvIntSave = LOS_IntLock(); 
     LOS_HwiCreate(HWI_NUM_INT50, 0,0,uart_irqhandle,NULL);//创建中断
     HalIrqUnmask(HWI_NUM_INT50); 
     LOS_IntRestore(uvIntSave); 
     HalIrqMask(HWI_NUM_INT50); 
 }
```

#### 完整实例

[sample\_hwi.c](resource/sample_hwi.c)

<h3 id="8">4.4 队列</h3>

### 概述  

#### 基本概念


队列又称消息队列，是一种常用于任务间通信的数据结构，实现了接收来自任务或中断的不固定长度的消息，并根据不同的接口选择传递消息是否存放在自己空间。任务能够从队列里面读取消息，当队列中的消息是空时，挂起读取任务；当队列中有新消息时，挂起的读取任务被唤醒并处理新消息。

用户在处理业务时，消息队列提供了异步处理机制，允许将一个消息放入队列，但并不立即处理它，同时队列还能起到缓冲消息作用。

Huawei LiteOS中使用队列数据结构实现任务异步通信工作，具有如下特性：

-   消息以先进先出方式排队，支持异步读写工作方式。
-   读队列和写队列都支持超时机制。
-   发送消息类型由通信双方约定，可以允许不同长度（不超过队列节点最大值）消息。
-   一个任务能够从任意一个消息队列接收和发送消息。
-   多个任务能够从同一个消息队列接收和发送消息。
-   当队列使用结束后，如果是动态申请的内存，需要通过释放内存函数回收。

#### 运作机制

**队列控制块**

```
 /** 
   * @ingroup los_queue 
   * Queue information block structure 
   */ 
 typedef struct  
 { 
     UINT8       *queueHandle;             /**< 队列指针 */ 
     UINT16      queueState;          /**< 队列状态 */ 
     UINT16      queueLen;            /**< 队列中消息个数 */ 
     UINT16      queueSize;           /**< 消息节点大小 */ 
     UINT16      queueID;             /**< 队列ID */ 
     UINT16      queueHead;           /**< 消息头节点位置（数组下标）*/ 
     UINT16      queueTail;           /**< 消息尾节点位置（数组下标）*/ 
     UINT16      readWriteableCnt[2]; /**< 队列中可读或可写消息数，
                                        0：可读，1：可写 */ 
     LOS_DL_LIST readWriteList[2];    /**< 读取或写入消息任务等待链表，  
                                        0：读取链表，1：写入链表 */ 
     LOS_DL_LIST memList;             /**< MailBox模块使用 */ 
 } LosQueueCB;
```

每个队列控制块中都含有队列状态，表示该队列的使用情况：

-   OS\_QUEUE\_UNUSED：队列没有使用。
-   OS\_QUEUE\_INUSED：队列被使用。

**队列运作原理**

创建队列时，根据用户传入队列长度和消息节点大小来开辟相应的内存空间以供该队列使用，返回队列ID。

在队列控制块中维护一个消息头节点位置Head和一个消息尾节点位置Tail来表示当前队列中消息存储情况。Head表示队列中被占用消息的起始位置。Tail表示队列中被空闲消息的起始位置。刚创建时Head和Tail均指向队列起始位置。

写队列时，根据Tail找到被占用消息节点末尾的空闲节点作为数据写入对象。如果Tail已经指向队列尾则采用回卷方式。根据readWriteableCnt\[1\]判断队列是否可以写入，不能对已满（readWriteableCnt\[1\]为0）队列进行写队列操作。

读队列时，根据Head找到最先写入队列中的消息节点进行读取。如果Head已经指向队列尾则采用回卷方式。根据readWriteableCnt\[0\]判断队列是否有消息读取，对全部空闲（readWriteableCnt\[0\]为0）队列进行读队列操作会引起任务挂起。

删除队列时，根据传入的队列ID寻找到对应的队列，把队列状态置为未使用，释放原队列所占的空间，对应的队列控制头置为初始状态。

**图 1**  队列读写数据操作示意图<a name="fig1396012213119"></a>  
![](figures/队列读写数据操作示意图.png "队列读写数据操作示意图")

### 开发指导

#### 功能

Huawei LiteOS中Message消息处理模块提供了以下功能。

<a name="table23667517184354"></a>
<table><thead align="left"><tr id="row23800586184354"><th class="cellrowborder" valign="top" width="27.317268273172683%" id="mcps1.1.4.1.1"><p id="p5651424118443"><a name="p5651424118443"></a><a name="p5651424118443"></a>功能分类</p>
</th>
<th class="cellrowborder" valign="top" width="29.387061293870616%" id="mcps1.1.4.1.2"><p id="p1425081918443"><a name="p1425081918443"></a><a name="p1425081918443"></a>接口名</p>
</th>
<th class="cellrowborder" valign="top" width="43.295670432956705%" id="mcps1.1.4.1.3"><p id="p1346571418443"><a name="p1346571418443"></a><a name="p1346571418443"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row460554512564"><td class="cellrowborder" valign="top" width="27.317268273172683%" headers="mcps1.1.4.1.1 "><p id="p173181844105612"><a name="p173181844105612"></a><a name="p173181844105612"></a>创建消息队列</p>
</td>
<td class="cellrowborder" valign="top" width="29.387061293870616%" headers="mcps1.1.4.1.2 "><p id="p73181448566"><a name="p73181448566"></a><a name="p73181448566"></a>LOS_QueueCreate</p>
</td>
<td class="cellrowborder" valign="top" width="43.295670432956705%" headers="mcps1.1.4.1.3 "><p id="p6318104475613"><a name="p6318104475613"></a><a name="p6318104475613"></a>创建一个消息队列。</p>
</td>
</tr>
<tr id="row12605204515568"><td class="cellrowborder" valign="top" width="27.317268273172683%" headers="mcps1.1.4.1.1 "><p id="p1731844475613"><a name="p1731844475613"></a><a name="p1731844475613"></a>读队列（不带拷贝）</p>
</td>
<td class="cellrowborder" valign="top" width="29.387061293870616%" headers="mcps1.1.4.1.2 "><p id="p13187448565"><a name="p13187448565"></a><a name="p13187448565"></a>LOS_QueueRead</p>
</td>
<td class="cellrowborder" valign="top" width="43.295670432956705%" headers="mcps1.1.4.1.3 "><p id="p031884410567"><a name="p031884410567"></a><a name="p031884410567"></a>读取指定队列头部中的数据（队列节点中的数据实际上是一个地址）。</p>
</td>
</tr>
<tr id="row360574585610"><td class="cellrowborder" valign="top" width="27.317268273172683%" headers="mcps1.1.4.1.1 "><p id="p6318744125613"><a name="p6318744125613"></a><a name="p6318744125613"></a>写队列（不带拷贝）</p>
</td>
<td class="cellrowborder" valign="top" width="29.387061293870616%" headers="mcps1.1.4.1.2 "><p id="p2318114410562"><a name="p2318114410562"></a><a name="p2318114410562"></a>LOS_QueueWrite</p>
</td>
<td class="cellrowborder" valign="top" width="43.295670432956705%" headers="mcps1.1.4.1.3 "><p id="p19318174416568"><a name="p19318174416568"></a><a name="p19318174416568"></a>向指定队列尾部写入用户Buffer的地址。</p>
</td>
</tr>
<tr id="row1160515450562"><td class="cellrowborder" valign="top" width="27.317268273172683%" headers="mcps1.1.4.1.1 "><p id="p4318174419565"><a name="p4318174419565"></a><a name="p4318174419565"></a>读队列（带拷贝）</p>
</td>
<td class="cellrowborder" valign="top" width="29.387061293870616%" headers="mcps1.1.4.1.2 "><p id="p5318184413566"><a name="p5318184413566"></a><a name="p5318184413566"></a>LOS_QueueReadCopy</p>
</td>
<td class="cellrowborder" valign="top" width="43.295670432956705%" headers="mcps1.1.4.1.3 "><p id="p4318204435615"><a name="p4318204435615"></a><a name="p4318204435615"></a>读取指定队列头部中的数据。</p>
</td>
</tr>
<tr id="row3605164555617"><td class="cellrowborder" valign="top" width="27.317268273172683%" headers="mcps1.1.4.1.1 "><p id="p163181444165619"><a name="p163181444165619"></a><a name="p163181444165619"></a>写队列（带拷贝）</p>
</td>
<td class="cellrowborder" valign="top" width="29.387061293870616%" headers="mcps1.1.4.1.2 "><p id="p163181442565"><a name="p163181442565"></a><a name="p163181442565"></a>LOS_QueueWriteCopy</p>
</td>
<td class="cellrowborder" valign="top" width="43.295670432956705%" headers="mcps1.1.4.1.3 "><p id="p3318144185614"><a name="p3318144185614"></a><a name="p3318144185614"></a>向指定队列尾部写入用户Buffer的数据。</p>
</td>
</tr>
<tr id="row26058454569"><td class="cellrowborder" valign="top" width="27.317268273172683%" headers="mcps1.1.4.1.1 "><p id="p193181844105616"><a name="p193181844105616"></a><a name="p193181844105616"></a>写队列（不带拷贝）</p>
</td>
<td class="cellrowborder" valign="top" width="29.387061293870616%" headers="mcps1.1.4.1.2 "><p id="p53183448565"><a name="p53183448565"></a><a name="p53183448565"></a>LOS_QueueWriteHead</p>
</td>
<td class="cellrowborder" valign="top" width="43.295670432956705%" headers="mcps1.1.4.1.3 "><p id="p23181144135611"><a name="p23181144135611"></a><a name="p23181144135611"></a>向指定队列的头部写入用户Buffer的地址。</p>
</td>
</tr>
<tr id="row96051145145614"><td class="cellrowborder" valign="top" width="27.317268273172683%" headers="mcps1.1.4.1.1 "><p id="p133181144205617"><a name="p133181144205617"></a><a name="p133181144205617"></a>写队列（带拷贝）</p>
</td>
<td class="cellrowborder" valign="top" width="29.387061293870616%" headers="mcps1.1.4.1.2 "><p id="p1231814444567"><a name="p1231814444567"></a><a name="p1231814444567"></a>LOS_QueueWriteHeadCopy</p>
</td>
<td class="cellrowborder" valign="top" width="43.295670432956705%" headers="mcps1.1.4.1.3 "><p id="p631894475610"><a name="p631894475610"></a><a name="p631894475610"></a>向指定队列的头部写入用户Buffer的数据。</p>
</td>
</tr>
<tr id="row36054459562"><td class="cellrowborder" valign="top" width="27.317268273172683%" headers="mcps1.1.4.1.1 "><p id="p153181644135615"><a name="p153181644135615"></a><a name="p153181644135615"></a>删除队列</p>
</td>
<td class="cellrowborder" valign="top" width="29.387061293870616%" headers="mcps1.1.4.1.2 "><p id="p163181442563"><a name="p163181442563"></a><a name="p163181442563"></a>LOS_QueueDelete</p>
</td>
<td class="cellrowborder" valign="top" width="43.295670432956705%" headers="mcps1.1.4.1.3 "><p id="p931911448563"><a name="p931911448563"></a><a name="p931911448563"></a>删除一个指定的队列。</p>
</td>
</tr>
<tr id="row260464525615"><td class="cellrowborder" valign="top" width="27.317268273172683%" headers="mcps1.1.4.1.1 "><p id="p13319154419565"><a name="p13319154419565"></a><a name="p13319154419565"></a>获取队列信息</p>
</td>
<td class="cellrowborder" valign="top" width="29.387061293870616%" headers="mcps1.1.4.1.2 "><p id="p1431934475618"><a name="p1431934475618"></a><a name="p1431934475618"></a>LOS_QueueInfoGet</p>
</td>
<td class="cellrowborder" valign="top" width="43.295670432956705%" headers="mcps1.1.4.1.3 "><p id="p12319184410563"><a name="p12319184410563"></a><a name="p12319184410563"></a>获取指定队列信息。</p>
</td>
</tr>
</tbody>
</table>

#### 开发流程

使用队列模块的典型流程如下：

1.  创建消息队列LOS\_QueueCreate。

    创建成功后，可以得到消息队列的ID值。

2.  写队列操作函数LOS\_QueueWriteCopy。
3.  读队列操作函数LOS\_QueueReadCopy。
4.  获取队列信息函数LOS\_QueueInfoGet。
5.  删除队列LOS\_QueueDelete。

#### QUEUE错误码

对队列存在失败可能性的操作，包括创建队列、删除队列等等，均需要返回对应的错误码，以便快速定位错误原因。

<a name="table66521465191749"></a>
<table><thead align="left"><tr id="row66647056191749"><th class="cellrowborder" valign="top" width="7.5200000000000005%" id="mcps1.1.6.1.1"><p id="p65995609191749"><a name="p65995609191749"></a><a name="p65995609191749"></a>序号</p>
</th>
<th class="cellrowborder" valign="top" width="21.33%" id="mcps1.1.6.1.2"><p id="p44044076191749"><a name="p44044076191749"></a><a name="p44044076191749"></a>定义</p>
</th>
<th class="cellrowborder" valign="top" width="17.119999999999997%" id="mcps1.1.6.1.3"><p id="p10800441191749"><a name="p10800441191749"></a><a name="p10800441191749"></a>实际数值</p>
</th>
<th class="cellrowborder" valign="top" width="18.12%" id="mcps1.1.6.1.4"><p id="p39597633191844"><a name="p39597633191844"></a><a name="p39597633191844"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="35.91%" id="mcps1.1.6.1.5"><p id="p61844251191749"><a name="p61844251191749"></a><a name="p61844251191749"></a>参考解决方案</p>
</th>
</tr>
</thead>
<tbody><tr id="row6517730619218"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="p4487040819218"><a name="p4487040819218"></a><a name="p4487040819218"></a>1</p>
</td>
<td class="cellrowborder" valign="top" width="21.33%" headers="mcps1.1.6.1.2 "><p id="p1062439819218"><a name="p1062439819218"></a><a name="p1062439819218"></a>LOS_ERRNO_QUEUE_MAXNUM_ZERO</p>
</td>
<td class="cellrowborder" valign="top" width="17.119999999999997%" headers="mcps1.1.6.1.3 "><p id="p5526990919218"><a name="p5526990919218"></a><a name="p5526990919218"></a>0x02000600</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.4 "><p id="p49896875192343"><a name="p49896875192343"></a><a name="p49896875192343"></a>队列资源的最大数目配置为0。</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="p3668333219218"><a name="p3668333219218"></a><a name="p3668333219218"></a>配置要大于0的队列资源的最大数量。如果不使用队列模块，则将配置项设置为将队列资源的最大数量的剪裁设置为NO。</p>
</td>
</tr>
<tr id="row33366968191749"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="p18369922191749"><a name="p18369922191749"></a><a name="p18369922191749"></a>2</p>
</td>
<td class="cellrowborder" valign="top" width="21.33%" headers="mcps1.1.6.1.2 "><p id="p11568741191749"><a name="p11568741191749"></a><a name="p11568741191749"></a>LOS_ERRNO_QUEUE_NO_MEMORY</p>
</td>
<td class="cellrowborder" valign="top" width="17.119999999999997%" headers="mcps1.1.6.1.3 "><p id="p64652808191749"><a name="p64652808191749"></a><a name="p64652808191749"></a>0x02000601</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.4 "><p id="p2386067191749"><a name="p2386067191749"></a><a name="p2386067191749"></a>队列块内存无法初始化。</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="p2094042219267"><a name="p2094042219267"></a><a name="p2094042219267"></a>为队列块分配更大的内存分区，或减少队列资源的最大数量。</p>
<p id="p5424607519267"><a name="p5424607519267"></a><a name="p5424607519267"></a></p>
</td>
</tr>
<tr id="row33397154191749"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="p20814974191749"><a name="p20814974191749"></a><a name="p20814974191749"></a>3</p>
</td>
<td class="cellrowborder" valign="top" width="21.33%" headers="mcps1.1.6.1.2 "><p id="p8291367191749"><a name="p8291367191749"></a><a name="p8291367191749"></a>LOS_ERRNO_QUEUE_CREATE_NO_MEMORY</p>
</td>
<td class="cellrowborder" valign="top" width="17.119999999999997%" headers="mcps1.1.6.1.3 "><p id="p512132191749"><a name="p512132191749"></a><a name="p512132191749"></a>0x02000602</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.4 "><p id="p15257143192818"><a name="p15257143192818"></a><a name="p15257143192818"></a>队列创建的内存未能被请求。</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="p38090089192845"><a name="p38090089192845"></a><a name="p38090089192845"></a>为队列分配更多的内存，或减少要创建的队列中的队列长度和节点的数目。</p>
</td>
</tr>
<tr id="row41391008191749"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="p64337369191749"><a name="p64337369191749"></a><a name="p64337369191749"></a>4</p>
</td>
<td class="cellrowborder" valign="top" width="21.33%" headers="mcps1.1.6.1.2 "><p id="p43944370191749"><a name="p43944370191749"></a><a name="p43944370191749"></a>LOS_ERRNO_QUEUE_SIZE_TOO_BIG</p>
</td>
<td class="cellrowborder" valign="top" width="17.119999999999997%" headers="mcps1.1.6.1.3 "><p id="p2724179191749"><a name="p2724179191749"></a><a name="p2724179191749"></a>0x02000603</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.4 "><p id="p27147516192928"><a name="p27147516192928"></a><a name="p27147516192928"></a>队列创建时消息长度超过上限。</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="p2299572192944"><a name="p2299572192944"></a><a name="p2299572192944"></a>更改创建队列中最大消息的大小至不超过上限。</p>
</td>
</tr>
<tr id="row10026959191749"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="p6877374191749"><a name="p6877374191749"></a><a name="p6877374191749"></a>5</p>
</td>
<td class="cellrowborder" valign="top" width="21.33%" headers="mcps1.1.6.1.2 "><p id="p20196417191749"><a name="p20196417191749"></a><a name="p20196417191749"></a>LOS_ERRNO_QUEUE_CB_UNAVAILABLE</p>
</td>
<td class="cellrowborder" valign="top" width="17.119999999999997%" headers="mcps1.1.6.1.3 "><p id="p25297089191749"><a name="p25297089191749"></a><a name="p25297089191749"></a>0x02000604</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.4 "><p id="p23052339193051"><a name="p23052339193051"></a><a name="p23052339193051"></a>已超过创建的队列的数量的上限。</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="p4265336019315"><a name="p4265336019315"></a><a name="p4265336019315"></a>增加队列的配置资源数量。</p>
</td>
</tr>
<tr id="row59373972191749"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="p44562467191749"><a name="p44562467191749"></a><a name="p44562467191749"></a>6</p>
</td>
<td class="cellrowborder" valign="top" width="21.33%" headers="mcps1.1.6.1.2 "><p id="p52790107191749"><a name="p52790107191749"></a><a name="p52790107191749"></a>LOS_ERRNO_QUEUE_NOT_FOUND</p>
</td>
<td class="cellrowborder" valign="top" width="17.119999999999997%" headers="mcps1.1.6.1.3 "><p id="p48140259191749"><a name="p48140259191749"></a><a name="p48140259191749"></a>0x02000605</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.4 "><p id="p60420222193129"><a name="p60420222193129"></a><a name="p60420222193129"></a>无效的队列。</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="p49643405193152"><a name="p49643405193152"></a><a name="p49643405193152"></a>确保队列ID有效。</p>
</td>
</tr>
<tr id="row36473565191749"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="p1568774191749"><a name="p1568774191749"></a><a name="p1568774191749"></a>7</p>
</td>
<td class="cellrowborder" valign="top" width="21.33%" headers="mcps1.1.6.1.2 "><p id="p59961868191749"><a name="p59961868191749"></a><a name="p59961868191749"></a>LOS_ERRNO_QUEUE_PEND_IN_LOCK</p>
</td>
<td class="cellrowborder" valign="top" width="17.119999999999997%" headers="mcps1.1.6.1.3 "><p id="p25073110191749"><a name="p25073110191749"></a><a name="p25073110191749"></a>0x02000606</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.4 "><p id="p19045334193230"><a name="p19045334193230"></a><a name="p19045334193230"></a>当任务被锁定时，禁止在队列中被阻塞。</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="p20051835193257"><a name="p20051835193257"></a><a name="p20051835193257"></a>使用队列前解锁任务。</p>
</td>
</tr>
<tr id="row51263497192010"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="p58702556192010"><a name="p58702556192010"></a><a name="p58702556192010"></a>8</p>
</td>
<td class="cellrowborder" valign="top" width="21.33%" headers="mcps1.1.6.1.2 "><p id="p57286633192010"><a name="p57286633192010"></a><a name="p57286633192010"></a>LOS_ERRNO_QUEUE_TIMEOUT</p>
</td>
<td class="cellrowborder" valign="top" width="17.119999999999997%" headers="mcps1.1.6.1.3 "><p id="p9705686192010"><a name="p9705686192010"></a><a name="p9705686192010"></a>0x02000607</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.4 "><p id="p53994389193449"><a name="p53994389193449"></a><a name="p53994389193449"></a>等待处理队列的时间超时。</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="p51382338193520"><a name="p51382338193520"></a><a name="p51382338193520"></a>检查设置的超时时间是否合适。</p>
</td>
</tr>
<tr id="row36402998193312"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="p62961745193312"><a name="p62961745193312"></a><a name="p62961745193312"></a>9</p>
</td>
<td class="cellrowborder" valign="top" width="21.33%" headers="mcps1.1.6.1.2 "><p id="p66736597193312"><a name="p66736597193312"></a><a name="p66736597193312"></a>LOS_ERRNO_QUEUE_IN_TSKUSE</p>
</td>
<td class="cellrowborder" valign="top" width="17.119999999999997%" headers="mcps1.1.6.1.3 "><p id="p36955240193312"><a name="p36955240193312"></a><a name="p36955240193312"></a>0x02000608</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.4 "><p id="p3637555120413"><a name="p3637555120413"></a><a name="p3637555120413"></a>阻塞任务的队列不能被删除。</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="p66119138193312"><a name="p66119138193312"></a><a name="p66119138193312"></a>使任务能够获得资源而不是在队列中被阻塞。</p>
</td>
</tr>
<tr id="row51203889193321"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="p53874317193321"><a name="p53874317193321"></a><a name="p53874317193321"></a>10</p>
</td>
<td class="cellrowborder" valign="top" width="21.33%" headers="mcps1.1.6.1.2 "><p id="p1743582193321"><a name="p1743582193321"></a><a name="p1743582193321"></a>LOS_ERRNO_QUEUE_WRITE_IN_INTERRUPT</p>
</td>
<td class="cellrowborder" valign="top" width="17.119999999999997%" headers="mcps1.1.6.1.3 "><p id="p7012492193321"><a name="p7012492193321"></a><a name="p7012492193321"></a>0x02000609</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.4 "><p id="p31140990193321"><a name="p31140990193321"></a><a name="p31140990193321"></a>在中断处理程序中不能写队列。</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="p51817932193749"><a name="p51817932193749"></a><a name="p51817932193749"></a>将写队列设为非阻塞模式。</p>
</td>
</tr>
<tr id="row58997007193325"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="p14028263193325"><a name="p14028263193325"></a><a name="p14028263193325"></a>11</p>
</td>
<td class="cellrowborder" valign="top" width="21.33%" headers="mcps1.1.6.1.2 "><p id="p62547532193325"><a name="p62547532193325"></a><a name="p62547532193325"></a>LOS_ERRNO_QUEUE_NOT_CREATE</p>
</td>
<td class="cellrowborder" valign="top" width="17.119999999999997%" headers="mcps1.1.6.1.3 "><p id="p33185317193325"><a name="p33185317193325"></a><a name="p33185317193325"></a>0x0200060a</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.4 "><p id="p3656155193325"><a name="p3656155193325"></a><a name="p3656155193325"></a>队列未创建。</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="p14251707193839"><a name="p14251707193839"></a><a name="p14251707193839"></a>检查队列中传递的句柄是否有效。</p>
</td>
</tr>
<tr id="row23132180193345"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="p61767288193345"><a name="p61767288193345"></a><a name="p61767288193345"></a>12</p>
</td>
<td class="cellrowborder" valign="top" width="21.33%" headers="mcps1.1.6.1.2 "><p id="p37094461193345"><a name="p37094461193345"></a><a name="p37094461193345"></a>LOS_ERRNO_QUEUE_IN_TSKWRITE</p>
</td>
<td class="cellrowborder" valign="top" width="17.119999999999997%" headers="mcps1.1.6.1.3 "><p id="p51861387193345"><a name="p51861387193345"></a><a name="p51861387193345"></a>0x0200060b</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.4 "><p id="p6555415519399"><a name="p6555415519399"></a><a name="p6555415519399"></a>队列读写不同步。</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="p20620767193345"><a name="p20620767193345"></a><a name="p20620767193345"></a>同步队列的读写。</p>
</td>
</tr>
<tr id="row21687970193352"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="p11895156193352"><a name="p11895156193352"></a><a name="p11895156193352"></a>13</p>
</td>
<td class="cellrowborder" valign="top" width="21.33%" headers="mcps1.1.6.1.2 "><p id="p23983603193352"><a name="p23983603193352"></a><a name="p23983603193352"></a>LOS_ERRNO_QUEUE_CREAT_PTR_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="17.119999999999997%" headers="mcps1.1.6.1.3 "><p id="p63623686193352"><a name="p63623686193352"></a><a name="p63623686193352"></a>0x0200060c</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.4 "><p id="p3279290519404"><a name="p3279290519404"></a><a name="p3279290519404"></a>队列创建过程中传递的参数为空指针。</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="p17873674193352"><a name="p17873674193352"></a><a name="p17873674193352"></a>确保传递的参数不为空指针。</p>
</td>
</tr>
<tr id="row32251036193358"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="p62197112193358"><a name="p62197112193358"></a><a name="p62197112193358"></a>14</p>
</td>
<td class="cellrowborder" valign="top" width="21.33%" headers="mcps1.1.6.1.2 "><p id="p4801272193358"><a name="p4801272193358"></a><a name="p4801272193358"></a>LOS_ERRNO_QUEUE_PARA_ISZERO</p>
</td>
<td class="cellrowborder" valign="top" width="17.119999999999997%" headers="mcps1.1.6.1.3 "><p id="p53358724193358"><a name="p53358724193358"></a><a name="p53358724193358"></a>0x0200060d</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.4 "><p id="p27309930194051"><a name="p27309930194051"></a><a name="p27309930194051"></a>队列创建过程中传递的队列长度或消息节点大小为0。</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="p46758883193358"><a name="p46758883193358"></a><a name="p46758883193358"></a>传入正确的队列长度和消息节点大小。</p>
</td>
</tr>
<tr id="row6471318219343"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="p727640719343"><a name="p727640719343"></a><a name="p727640719343"></a>15</p>
</td>
<td class="cellrowborder" valign="top" width="21.33%" headers="mcps1.1.6.1.2 "><p id="p5251806419343"><a name="p5251806419343"></a><a name="p5251806419343"></a>LOS_ERRNO_QUEUE_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="17.119999999999997%" headers="mcps1.1.6.1.3 "><p id="p2610475319343"><a name="p2610475319343"></a><a name="p2610475319343"></a>0x0200060e</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.4 "><p id="p5223249920528"><a name="p5223249920528"></a><a name="p5223249920528"></a>读取队列、写入队列的handle无效。</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="p329454442063"><a name="p329454442063"></a><a name="p329454442063"></a>检查队列中传递的handle是否有效</p>
</td>
</tr>
<tr id="row3895023819348"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="p85270419348"><a name="p85270419348"></a><a name="p85270419348"></a>16</p>
</td>
<td class="cellrowborder" valign="top" width="21.33%" headers="mcps1.1.6.1.2 "><p id="p196018619348"><a name="p196018619348"></a><a name="p196018619348"></a>LOS_ERRNO_QUEUE_READ_PTR_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="17.119999999999997%" headers="mcps1.1.6.1.3 "><p id="p2455739719348"><a name="p2455739719348"></a><a name="p2455739719348"></a>0x0200060f</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.4 "><p id="p5676363194253"><a name="p5676363194253"></a><a name="p5676363194253"></a>队列读取过程中传递的指针为空。</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="p37131993194325"><a name="p37131993194325"></a><a name="p37131993194325"></a>检查指针中传递的是否为空。</p>
</td>
</tr>
<tr id="row15616099193413"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="p56944481193413"><a name="p56944481193413"></a><a name="p56944481193413"></a>17</p>
</td>
<td class="cellrowborder" valign="top" width="21.33%" headers="mcps1.1.6.1.2 "><p id="p49100215193413"><a name="p49100215193413"></a><a name="p49100215193413"></a>LOS_ERRNO_QUEUE_READSIZE_ISZERO</p>
</td>
<td class="cellrowborder" valign="top" width="17.119999999999997%" headers="mcps1.1.6.1.3 "><p id="p17694478193413"><a name="p17694478193413"></a><a name="p17694478193413"></a>0x02000610</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.4 "><p id="p23966605193413"><a name="p23966605193413"></a><a name="p23966605193413"></a>队列读取过程中传递的缓冲区大小为0。</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="p62246877193413"><a name="p62246877193413"></a><a name="p62246877193413"></a>通过一个正确的缓冲区大小。</p>
</td>
</tr>
<tr id="row24056666194424"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="p2432890194424"><a name="p2432890194424"></a><a name="p2432890194424"></a>18</p>
</td>
<td class="cellrowborder" valign="top" width="21.33%" headers="mcps1.1.6.1.2 "><p id="p62846374194424"><a name="p62846374194424"></a><a name="p62846374194424"></a>LOS_ERRNO_QUEUE_WRITE_PTR_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="17.119999999999997%" headers="mcps1.1.6.1.3 "><p id="p57391559194424"><a name="p57391559194424"></a><a name="p57391559194424"></a>0x02000612</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.4 "><p id="p2160612619463"><a name="p2160612619463"></a><a name="p2160612619463"></a>队列写入过程中传递的指针为空。</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="p65297927194424"><a name="p65297927194424"></a><a name="p65297927194424"></a>检查指针中传递的是否为空。</p>
</td>
</tr>
<tr id="row23479934194430"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="p22826525194430"><a name="p22826525194430"></a><a name="p22826525194430"></a>19</p>
</td>
<td class="cellrowborder" valign="top" width="21.33%" headers="mcps1.1.6.1.2 "><p id="p37009232194430"><a name="p37009232194430"></a><a name="p37009232194430"></a>LOS_ERRNO_QUEUE_WRITESIZE_ISZERO</p>
</td>
<td class="cellrowborder" valign="top" width="17.119999999999997%" headers="mcps1.1.6.1.3 "><p id="p44957824194430"><a name="p44957824194430"></a><a name="p44957824194430"></a>0x02000613</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.4 "><p id="p28691532194638"><a name="p28691532194638"></a><a name="p28691532194638"></a>队列写入过程中传递的缓冲区大小为0。</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="p24827637194430"><a name="p24827637194430"></a><a name="p24827637194430"></a>通过一个正确的缓冲区大小。</p>
</td>
</tr>
<tr id="row2499390194657"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="p1124069194657"><a name="p1124069194657"></a><a name="p1124069194657"></a>20</p>
</td>
<td class="cellrowborder" valign="top" width="21.33%" headers="mcps1.1.6.1.2 "><p id="p23940727194657"><a name="p23940727194657"></a><a name="p23940727194657"></a>LOS_ERRNO_QUEUE_WRITE_SIZE_TOO_BIG</p>
</td>
<td class="cellrowborder" valign="top" width="17.119999999999997%" headers="mcps1.1.6.1.3 "><p id="p60150750194657"><a name="p60150750194657"></a><a name="p60150750194657"></a>0x02000615</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.4 "><p id="p40372610194657"><a name="p40372610194657"></a><a name="p40372610194657"></a>队列写入过程中传递的缓冲区大小比队列大小要大。</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="p7860434194847"><a name="p7860434194847"></a><a name="p7860434194847"></a>减少缓冲区大小，或增大队列节点。</p>
<p id="p3635044194847"><a name="p3635044194847"></a><a name="p3635044194847"></a></p>
<p id="p32715400194847"><a name="p32715400194847"></a><a name="p32715400194847"></a></p>
</td>
</tr>
<tr id="row4823728219471"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="p1490580719471"><a name="p1490580719471"></a><a name="p1490580719471"></a>21</p>
</td>
<td class="cellrowborder" valign="top" width="21.33%" headers="mcps1.1.6.1.2 "><p id="p6651970819471"><a name="p6651970819471"></a><a name="p6651970819471"></a>LOS_ERRNO_QUEUE_ISFULL</p>
</td>
<td class="cellrowborder" valign="top" width="17.119999999999997%" headers="mcps1.1.6.1.3 "><p id="p1938724919471"><a name="p1938724919471"></a><a name="p1938724919471"></a>0x02000616</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.4 "><p id="p32608359195013"><a name="p32608359195013"></a><a name="p32608359195013"></a>在队列写入过程中没有可用的空闲节点。</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="p20301496195025"><a name="p20301496195025"></a><a name="p20301496195025"></a>确保在队列写入之前，可以使用空闲的节点。</p>
<p id="p48495737195025"><a name="p48495737195025"></a><a name="p48495737195025"></a></p>
<p id="p33808449195025"><a name="p33808449195025"></a><a name="p33808449195025"></a></p>
</td>
</tr>
<tr id="row33549139194916"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="p33125773194917"><a name="p33125773194917"></a><a name="p33125773194917"></a>22</p>
</td>
<td class="cellrowborder" valign="top" width="21.33%" headers="mcps1.1.6.1.2 "><p id="p65941984194917"><a name="p65941984194917"></a><a name="p65941984194917"></a>LOS_ERRNO_QUEUE_PTR_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="17.119999999999997%" headers="mcps1.1.6.1.3 "><p id="p39700467194917"><a name="p39700467194917"></a><a name="p39700467194917"></a>0x02000617</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.4 "><p id="p61621230194917"><a name="p61621230194917"></a><a name="p61621230194917"></a>正在获取队列信息时传递的指针为空。</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="p40971599195136"><a name="p40971599195136"></a><a name="p40971599195136"></a>检查指针中传递的是否为空。</p>
</td>
</tr>
<tr id="row39947507194923"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="p14522634194923"><a name="p14522634194923"></a><a name="p14522634194923"></a>23</p>
</td>
<td class="cellrowborder" valign="top" width="21.33%" headers="mcps1.1.6.1.2 "><p id="p35482697194923"><a name="p35482697194923"></a><a name="p35482697194923"></a>LOS_ERRNO_QUEUE_READ_IN_INTERRUPT</p>
</td>
<td class="cellrowborder" valign="top" width="17.119999999999997%" headers="mcps1.1.6.1.3 "><p id="p55526233194923"><a name="p55526233194923"></a><a name="p55526233194923"></a>0x02000618</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.4 "><p id="p1331042194923"><a name="p1331042194923"></a><a name="p1331042194923"></a>在中断处理程序中不能读队列。</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="p4205960195220"><a name="p4205960195220"></a><a name="p4205960195220"></a>将读队列设为非阻塞模式。</p>
</td>
</tr>
<tr id="row50011219194929"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="p24376929194929"><a name="p24376929194929"></a><a name="p24376929194929"></a>24</p>
</td>
<td class="cellrowborder" valign="top" width="21.33%" headers="mcps1.1.6.1.2 "><p id="p28374236194929"><a name="p28374236194929"></a><a name="p28374236194929"></a>LOS_ERRNO_QUEUE_MAIL_HANDLE_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="17.119999999999997%" headers="mcps1.1.6.1.3 "><p id="p16611755194929"><a name="p16611755194929"></a><a name="p16611755194929"></a>0x02000619</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.4 "><p id="p3374903194929"><a name="p3374903194929"></a><a name="p3374903194929"></a>正在释放队列的内存时传递的队列的handle无效。</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="p4931687194929"><a name="p4931687194929"></a><a name="p4931687194929"></a>检查队列中传递的handle是否有效。</p>
</td>
</tr>
<tr id="row41671503194933"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="p19948548194933"><a name="p19948548194933"></a><a name="p19948548194933"></a>25</p>
</td>
<td class="cellrowborder" valign="top" width="21.33%" headers="mcps1.1.6.1.2 "><p id="p5219669194933"><a name="p5219669194933"></a><a name="p5219669194933"></a>LOS_ERRNO_QUEUE_MAIL_PTR_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="17.119999999999997%" headers="mcps1.1.6.1.3 "><p id="p20140076194933"><a name="p20140076194933"></a><a name="p20140076194933"></a>0x0200061a</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.4 "><p id="p44396808195347"><a name="p44396808195347"></a><a name="p44396808195347"></a>传入的消息内存池指针为空。</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="p41085460195422"><a name="p41085460195422"></a><a name="p41085460195422"></a>检查指针是否为空。</p>
</td>
</tr>
<tr id="row24094539194938"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="p5500604194938"><a name="p5500604194938"></a><a name="p5500604194938"></a>26</p>
</td>
<td class="cellrowborder" valign="top" width="21.33%" headers="mcps1.1.6.1.2 "><p id="p42895774194938"><a name="p42895774194938"></a><a name="p42895774194938"></a>LOS_ERRNO_QUEUE_MAIL_FREE_ERROR</p>
</td>
<td class="cellrowborder" valign="top" width="17.119999999999997%" headers="mcps1.1.6.1.3 "><p id="p52005656194938"><a name="p52005656194938"></a><a name="p52005656194938"></a>0x0200061b</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.4 "><p id="p6044992819550"><a name="p6044992819550"></a><a name="p6044992819550"></a>membox内存释放失败。</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="p621335195517"><a name="p621335195517"></a><a name="p621335195517"></a>传入非空membox内存指针。</p>
</td>
</tr>
<tr id="row32525342195533"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="p17307058195533"><a name="p17307058195533"></a><a name="p17307058195533"></a>27</p>
</td>
<td class="cellrowborder" valign="top" width="21.33%" headers="mcps1.1.6.1.2 "><p id="p59694421195533"><a name="p59694421195533"></a><a name="p59694421195533"></a>LOS_ERRNO_QUEUE_ISEMPTY</p>
</td>
<td class="cellrowborder" valign="top" width="17.119999999999997%" headers="mcps1.1.6.1.3 "><p id="p3409970195533"><a name="p3409970195533"></a><a name="p3409970195533"></a>0x0200061d</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.4 "><p id="p7772143195533"><a name="p7772143195533"></a><a name="p7772143195533"></a>队列已空。</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="p65865508195654"><a name="p65865508195654"></a><a name="p65865508195654"></a>确保在读取队列时包含消息。</p>
</td>
</tr>
<tr id="row14065892195540"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="p65595428195540"><a name="p65595428195540"></a><a name="p65595428195540"></a>28</p>
</td>
<td class="cellrowborder" valign="top" width="21.33%" headers="mcps1.1.6.1.2 "><p id="p11629424195540"><a name="p11629424195540"></a><a name="p11629424195540"></a>LOS_ERRNO_QUEUE_READ_SIZE_TOO_SMALL</p>
</td>
<td class="cellrowborder" valign="top" width="17.119999999999997%" headers="mcps1.1.6.1.3 "><p id="p2459313195540"><a name="p2459313195540"></a><a name="p2459313195540"></a>0x0200061f</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.4 "><p id="p64986703195540"><a name="p64986703195540"></a><a name="p64986703195540"></a>读缓冲区大小小于队列大小。</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="p4713788520034"><a name="p4713788520034"></a><a name="p4713788520034"></a>增加缓冲区大小，或减小队列节点大小。</p>
</td>
</tr>
</tbody>
</table>

#### 平台差异性

无。

### 注意事项

-   系统可配置的队列资源个数是指：整个系统的队列资源总个数，而非用户能使用的个数。例如：系统软件定时器多占用一个队列资源，那么系统可配置的队列资源就会减少一个。
-   调用 LOS\_QueueCreate 函数时所传入的队列名暂时未使用，作为以后的预留参数。
-   队列接口函数中的入参uwTimeOut是指相对时间。
-   LOS\_QueueReadCopy和LOS\_QueueWriteCopy及LOS\_QueueWriteHeadCopy是一组接口，LOS\_QueueRead和LOS\_QueueWrite及LOS\_QueueWriteHead是一组接口，两组接口需要配套使用。
-   鉴于LOS\_QueueWriteHead和LOS\_QueueRead这组接口实际操作的是数据地址，用户必须保证调用LOS\_QueueRead获取到的指针所指向内存区域在读队列期间没有被异常修改或释放，否则可能会导致不可预知的后果。
-   鉴于LOS\_QueueWrite和LOS\_QueueWriteHead和LOS\_QueueRead这组接口实际操作的是数据地址，也就意味着实际Write和Read的长度仅仅是一个指针数据，因此用户使用这组接口之前，需确保相关队列在创建时的消息长度为一个指针的长度，避免不必要的浪费和读取失败。
-   LOS\_QueueWrite和LOS\_QueueWriteHead和LOS\_QueueRead这组接口正在被逐步废弃，建议使用LOS\_QueueWriteCopy和LOS\_QueueWriteHeadCopy和LOS\_QueueReadCopy做为替代。

### 编程实例

#### 实例描述


创建一个队列，两个任务。任务1调用发送接口发送消息；任务2通过接收接口接收消息。

1.  通过LOS\_TaskCreate创建任务1和任务2。
2.  通过LOS\_QueueCreate创建一个消息队列。
3.  在任务1  send\_Entry中发送消息。
4.  在任务2  recv\_Entry中接收消息。
5.  通过LOS\_QueueDelete删除队列。

#### 编程示例

```
 #include "los_task.h" 
 #include "los_queue.h" 
 static UINT32 g_uwQueue; 
 CHAR abuf[] = "test is message x"; 
  
 /*任务1发送数据*/ 
 void *send_Entry(void *arg) 
 { 
     UINT32 i = 0,uwRet = 0; 
     UINT32 uwlen = sizeof(abuf); 
  
     while (i <5) 
     { 
         abuf[uwlen -2] = '0' + i; 
         i++; 
  
         /*将abuf里的数据写入队列*/ 
         uwRet = LOS_QueueWrite(g_uwQueue, abuf, uwlen, 0); 
         if(uwRet != LOS_OK) 
         { 
             dprintf("send message failure,error:%x\n",uwRet); 
         } 
  
         LOS_TaskDelay(5); 
     } 
 } 
  
 /*任务2接收数据*/ 
 void *recv_Entry(void *arg) 
 { 
     UINT32 uwReadbuf; 
     UINT32 uwRet = 0; 
  
     while (1) 
     { 
  
         /*读取队列里的数据存入uwReadbuf里*/ 
         uwRet = LOS_QueueRead(g_uwQueue, &uwReadbuf, 50, 0); 
         if(uwRet != LOS_OK) 
         { 
             dprintf("recv message failure,error:%x\n",uwRet); 
             break; 
         } 
  
         dprintf("recv message:%s\n", (char *)uwReadbuf); 
         LOS_TaskDelay(5); 
     } 
     /*删除队列*/ 
     while (LOS_OK != LOS_QueueDelete(g_uwQueue)) 
     { 
         LOS_TaskDelay(1); 
     } 
  
     dprintf("delete the queue success!\n"); 
 } 
  
 int Example_creat_task(void) 
 { 
     UINT32 uwRet = 0; 
     UINT32 uwTask1, uwTask2; 
     TSK_INIT_PARAM_S stInitParam1; 
  
     /*创建任务1*/ 
     stInitParam1.pfnTaskEntry = send_Entry; 
     stInitParam1.usTaskPrio = 9; 
     stInitParam1.uwStackSize = 0x400; 
     stInitParam1.pcName = "sendQueue"; 
     stInitParam1.uwResved = LOS_TASK_STATUS_DETACHED; 
     LOS_TaskLock();//锁住任务，防止新创建的任务比本任务高而发生调度
     uwRet = LOS_TaskCreate(&uwTask1, &stInitParam1); 
     if(uwRet != LOS_OK) 
     { 
         dprintf("create task1 failed!,error:%x\n",uwRet); 
         return uwRet; 
     } 
  
     /*创建任务2*/ 
     stInitParam1.pfnTaskEntry = recv_Entry; 
     uwRet = LOS_TaskCreate(&uwTask2, &stInitParam1); 
     if(uwRet != LOS_OK) 
     { 
         dprintf("create task2 failed!,error:%x\n",uwRet); 
         return uwRet; 
     } 
  
     /*创建队列*/ 
     uwRet = LOS_QueueCreate("queue", 5, &g_uwQueue, 0, 50); 
     if(uwRet != LOS_OK) 
     { 
         dprintf("create queue failure!,error:%x\n",uwRet); 
     } 
  
     dprintf("create the queue success!\n"); 
     LOS_TaskUnlock();//解锁任务，只有队列创建后才开始任务调度
 }
```

#### 结果验证

![](figures/wwx300029-中软欧拉开发部-iCOS-image-7af87939-8a3f-4948-9de4-4032c6292d6e.png)

#### 完整实例代码

[sample\_queue.c](resource/sample_queue.c)

<h3 id="9">4.5 事件</h3>

### 概述

#### 基本概念


事件是一种实现任务间通信的机制，可用于实现任务间的同步。一个任务可以等待多个事件的发生：可以是任意一个事件发生时唤醒任务进行事件处理;也可以是几个事件都发生后才唤醒任务进行事件处理。

多任务环境下，任务之间往往需要同步操作，一个等待即是一个同步。事件可以提供一对多、多对多的同步操作。一对多同步模型：一个任务等待多个事件的触发；多对多同步模型：多个任务等待多个事件的触发。

任务可以通过创建事件控制块来实现对事件的触发和等待操作。

Huawei LiteOS提供的事件具有如下特点：

-   事件不与任务相关联，事件相互独立，一个32位的无符号整型变量，用于标识该任务发生的事件类型，其中每一位表示一种事件类型（0表示该事件类型未发生、1表示该事件类型已经发生）。
-   事件仅用于任务间的同步，不提供数据传输功能。
-   多次向任务发送同一事件类型，等效于只发送一次。
-   允许多个任务对同一事件进行读写操作。
-   支持事件读写超时机制。

**事件控制块**

```
/**
  * @ingroup los_event
  * Event control structure
  */
typedef struct tagEvent
{
    UINT32 uwEventID;            /**标识发生的事件类型位*/
    LOS_DL_LIST    stEventList;  /**读取事件任务链表*/
} EVENT_CB_S, *PEVENT_CB_S;
```

uwEventID：用于标识该任务发生的事件类型，其中每一位表示一种事件类型（0表示该事件类型未发生、1表示该事件类型已经发生），一共31种事件类型，第25位系统保留。

**事件读取模式**

在读事件时，可以选择读取模式。读取模式如下：

所有事件（LOS\_WAITMODE\_AND）：读取掩码中所有事件类型，只有读取的所有事件类型都发生了，才能读取成功。

任一事件（LOS\_WAITMODE\_OR）： 读取掩码中任一事件类型，读取的事件中任意一种事件类型发生了，就可以读取成功。

清除事件（LOS\_WAITMODE\_CLR）：这是一种附加读取模式，可以与LOS\_WAITMODE\_AND和LOS\_WAITMODE\_OR结合使用。（LOS\_WAITMODE\_AND| LOS\_WAITMODE\_CLR或 LOS\_WAITMODE\_OR| LOS\_WAITMODE\_CLR），设置该模式读取成功后，对应事件类型位会自动清除。

#### 运作机制

读事件时，可以根据入参事件掩码类型uwEventMask读取事件的单个或者多个事件类型。事件读取成功后，如果设置LOS\_WAITMODE\_CLR会清除已读取到的事件类型，反之不会清除已读到的事件类型，需显式清除。可以通过入参选择读取模式，读取事件掩码类型中所有事件还是读取事件掩码类型中任意事件。

写事件时，对指定事件写入指定的事件类型，可以一次同时写多个事件类型。写事件会触发任务调度。

清除事件时，根据入参事件和待清除的事件类型，对事件对应位进行清0操作。

**图 1**  事件唤醒任务示意图<a name="fig5480864217647"></a>  
![](figures/事件唤醒任务示意图.png "事件唤醒任务示意图")

### 开发指导  

#### 使用场景

事件可应用于多种任务同步场合，能够一定程度替代信号量。  

#### 功能

Huawei LiteOS系统中的事件模块为用户提供下面几个接口。

<a name="table31878844162710"></a>
<table><thead align="left"><tr id="row24909577162710"><th class="cellrowborder" valign="top" width="17.349999999999998%" id="mcps1.1.4.1.1"><p id="p4409895162710"><a name="p4409895162710"></a><a name="p4409895162710"></a>功能分类</p>
</th>
<th class="cellrowborder" valign="top" width="22.24%" id="mcps1.1.4.1.2"><p id="p21657225162710"><a name="p21657225162710"></a><a name="p21657225162710"></a>接口名</p>
</th>
<th class="cellrowborder" valign="top" width="60.41%" id="mcps1.1.4.1.3"><p id="p9404824162710"><a name="p9404824162710"></a><a name="p9404824162710"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row24183005155142"><td class="cellrowborder" valign="top" width="17.349999999999998%" headers="mcps1.1.4.1.1 "><p id="p34065167155340"><a name="p34065167155340"></a><a name="p34065167155340"></a>事件初始化</p>
</td>
<td class="cellrowborder" valign="top" width="22.24%" headers="mcps1.1.4.1.2 "><p id="p19341936155142"><a name="p19341936155142"></a><a name="p19341936155142"></a>LOS_EventInit</p>
</td>
<td class="cellrowborder" valign="top" width="60.41%" headers="mcps1.1.4.1.3 "><p id="p23193017155142"><a name="p23193017155142"></a><a name="p23193017155142"></a>初始化一个事件控制块。</p>
</td>
</tr>
<tr id="row23593275162710"><td class="cellrowborder" valign="top" width="17.349999999999998%" headers="mcps1.1.4.1.1 "><p id="p32007131162710"><a name="p32007131162710"></a><a name="p32007131162710"></a>读事件</p>
</td>
<td class="cellrowborder" valign="top" width="22.24%" headers="mcps1.1.4.1.2 "><p id="p42440799162710"><a name="p42440799162710"></a><a name="p42440799162710"></a>LOS_EventRead</p>
</td>
<td class="cellrowborder" valign="top" width="60.41%" headers="mcps1.1.4.1.3 "><p id="p15152688162710"><a name="p15152688162710"></a><a name="p15152688162710"></a>读取指定事件类型，超时时间为相对时间：单位为Tick。</p>
</td>
</tr>
<tr id="row2156464162710"><td class="cellrowborder" valign="top" width="17.349999999999998%" headers="mcps1.1.4.1.1 "><p id="p40455902162710"><a name="p40455902162710"></a><a name="p40455902162710"></a>写事件</p>
</td>
<td class="cellrowborder" valign="top" width="22.24%" headers="mcps1.1.4.1.2 "><p id="p55702669162710"><a name="p55702669162710"></a><a name="p55702669162710"></a>LOS_EventWrite</p>
</td>
<td class="cellrowborder" valign="top" width="60.41%" headers="mcps1.1.4.1.3 "><p id="p15622373162710"><a name="p15622373162710"></a><a name="p15622373162710"></a>写指定的事件类型。</p>
</td>
</tr>
<tr id="row6383631162710"><td class="cellrowborder" valign="top" width="17.349999999999998%" headers="mcps1.1.4.1.1 "><p id="p47312118162710"><a name="p47312118162710"></a><a name="p47312118162710"></a>清除事件</p>
</td>
<td class="cellrowborder" valign="top" width="22.24%" headers="mcps1.1.4.1.2 "><p id="p7076378162710"><a name="p7076378162710"></a><a name="p7076378162710"></a>LOS_EventClear</p>
</td>
<td class="cellrowborder" valign="top" width="60.41%" headers="mcps1.1.4.1.3 "><p id="p36315746162710"><a name="p36315746162710"></a><a name="p36315746162710"></a>清除指定的事件类型。</p>
</td>
</tr>
<tr id="row13860287164240"><td class="cellrowborder" valign="top" width="17.349999999999998%" headers="mcps1.1.4.1.1 "><p id="p48941496164240"><a name="p48941496164240"></a><a name="p48941496164240"></a>校验事件掩码</p>
</td>
<td class="cellrowborder" valign="top" width="22.24%" headers="mcps1.1.4.1.2 "><p id="p4838245164240"><a name="p4838245164240"></a><a name="p4838245164240"></a>LOS_EventPoll</p>
</td>
<td class="cellrowborder" valign="top" width="60.41%" headers="mcps1.1.4.1.3 "><p id="p56353583164240"><a name="p56353583164240"></a><a name="p56353583164240"></a>根据用户传入的事件值、事件掩码及校验模式，返回用户传入的事件是否符合预期。</p>
</td>
</tr>
<tr id="row5784255164347"><td class="cellrowborder" valign="top" width="17.349999999999998%" headers="mcps1.1.4.1.1 "><p id="p65871477164347"><a name="p65871477164347"></a><a name="p65871477164347"></a>销毁事件</p>
</td>
<td class="cellrowborder" valign="top" width="22.24%" headers="mcps1.1.4.1.2 "><p id="p33989420164347"><a name="p33989420164347"></a><a name="p33989420164347"></a>LOS_EventDestroy</p>
</td>
<td class="cellrowborder" valign="top" width="60.41%" headers="mcps1.1.4.1.3 "><p id="p1679627164347"><a name="p1679627164347"></a><a name="p1679627164347"></a>销毁指定的事件控制块。</p>
</td>
</tr>
</tbody>
</table>

#### 开发流程

使用事件模块的典型流程如下：

1.  调用事件初始化LOS\_EventInit接口，初始化事件等待队列。
2.  写事件LOS\_EventWrite，配置事件掩码类型。
3.  读事件LOS\_EventRead，可以选择读取模式。
4.  清除事件LOS\_EventClear，清除指定的事件类型。

#### Event错误码

对事件存在失败的可能性操作，包括事件初始化，事件销毁，事件读写，事件清除。

<a name="table874491812737"></a>
<table><thead align="left"><tr id="row2830196712737"><th class="cellrowborder" valign="top" width="20%" id="mcps1.1.6.1.1"><p id="p2971281012737"><a name="p2971281012737"></a><a name="p2971281012737"></a>序号</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.1.6.1.2"><p id="p5792739612737"><a name="p5792739612737"></a><a name="p5792739612737"></a>定义</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.1.6.1.3"><p id="p6160751912737"><a name="p6160751912737"></a><a name="p6160751912737"></a>实际值</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.1.6.1.4"><p id="p2415315512737"><a name="p2415315512737"></a><a name="p2415315512737"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.1.6.1.5"><p id="p1024850212737"><a name="p1024850212737"></a><a name="p1024850212737"></a>参考解决方案</p>
</th>
</tr>
</thead>
<tbody><tr id="row35955500121027"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.1 "><p id="p26714373121027"><a name="p26714373121027"></a><a name="p26714373121027"></a>1</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.2 "><p id="p16380603121027"><a name="p16380603121027"></a><a name="p16380603121027"></a>LOS_ERRNO_EVENT_SETBIT_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.3 "><p id="p12580778121054"><a name="p12580778121054"></a><a name="p12580778121054"></a>0x02001c00</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.4 "><p id="p31845851121027"><a name="p31845851121027"></a><a name="p31845851121027"></a>事件ID的第25个bit不能设置为1，因为该位已经作为错误码使用</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.5 "><p id="p29377175121027"><a name="p29377175121027"></a><a name="p29377175121027"></a>事件ID的第25bit置为0</p>
</td>
</tr>
<tr id="row2512766212737"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.1 "><p id="p2207475012737"><a name="p2207475012737"></a><a name="p2207475012737"></a>2</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.2 "><p id="p4322430612737"><a name="p4322430612737"></a><a name="p4322430612737"></a>LOS_ERRNO_EVENT_READ_TIMEOUT</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.3 "><p id="p1150789112737"><a name="p1150789112737"></a><a name="p1150789112737"></a>0x02001c01</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.4 "><p id="p5972394412737"><a name="p5972394412737"></a><a name="p5972394412737"></a>读超时</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.5 "><p id="p580127312737"><a name="p580127312737"></a><a name="p580127312737"></a>增加等待时间或者重新读取</p>
</td>
</tr>
<tr id="row5221146412737"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.1 "><p id="p127022412737"><a name="p127022412737"></a><a name="p127022412737"></a>3</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.2 "><p id="p3577930412737"><a name="p3577930412737"></a><a name="p3577930412737"></a>LOS_ERRNO_EVENT_EVENTMASK_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.3 "><p id="p1244248812737"><a name="p1244248812737"></a><a name="p1244248812737"></a>0x02001c02</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.4 "><p id="p120861712737"><a name="p120861712737"></a><a name="p120861712737"></a>入参的事件ID是无效的</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.5 "><p id="p3078915412737"><a name="p3078915412737"></a><a name="p3078915412737"></a>传入有效的事件ID参数</p>
</td>
</tr>
<tr id="row866693812737"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.1 "><p id="p3093336312737"><a name="p3093336312737"></a><a name="p3093336312737"></a>4</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.2 "><p id="p2257447312737"><a name="p2257447312737"></a><a name="p2257447312737"></a>LOS_ERRNO_EVENT_READ_IN_INTERRUPT</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.3 "><p id="p1659299512737"><a name="p1659299512737"></a><a name="p1659299512737"></a>0x02001c03</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.4 "><p id="p185533712737"><a name="p185533712737"></a><a name="p185533712737"></a>在中断中读取事件</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.5 "><p id="p1606464512737"><a name="p1606464512737"></a><a name="p1606464512737"></a>启动新的任务来获取事件</p>
</td>
</tr>
<tr id="row1036407712737"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.1 "><p id="p3418393812737"><a name="p3418393812737"></a><a name="p3418393812737"></a>5</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.2 "><p id="p1743561612737"><a name="p1743561612737"></a><a name="p1743561612737"></a>LOS_ERRNO_EVENT_FLAGS_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.3 "><p id="p299876612737"><a name="p299876612737"></a><a name="p299876612737"></a>0x02001c04</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.4 "><p id="p4157346012737"><a name="p4157346012737"></a><a name="p4157346012737"></a>读取事件的mode无效</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.5 "><p id="p1200709912737"><a name="p1200709912737"></a><a name="p1200709912737"></a>传入有效的mode参数</p>
</td>
</tr>
<tr id="row4095503012737"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.1 "><p id="p2902313712737"><a name="p2902313712737"></a><a name="p2902313712737"></a>6</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.2 "><p id="p206387212737"><a name="p206387212737"></a><a name="p206387212737"></a>LOS_ERRNO_EVENT_READ_IN_LOCK</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.3 "><p id="p3295593212737"><a name="p3295593212737"></a><a name="p3295593212737"></a>0x02001c05</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.4 "><p id="p5218479612737"><a name="p5218479612737"></a><a name="p5218479612737"></a>任务锁住，不能读取事件</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.5 "><p id="p6621892012737"><a name="p6621892012737"></a><a name="p6621892012737"></a>解锁任务，再读取事件</p>
</td>
</tr>
<tr id="row3924015812752"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.1 "><p id="p2433626212752"><a name="p2433626212752"></a><a name="p2433626212752"></a>7</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.2 "><p id="p2508019012752"><a name="p2508019012752"></a><a name="p2508019012752"></a>LOS_ERRNO_EVENT_PTR_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.3 "><p id="p1822948612752"><a name="p1822948612752"></a><a name="p1822948612752"></a>0x02001c06</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.4 "><p id="p19340712752"><a name="p19340712752"></a><a name="p19340712752"></a>传入的参数为空指针</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.5 "><p id="p1566600812752"><a name="p1566600812752"></a><a name="p1566600812752"></a>传入非空入参</p>
</td>
</tr>
<tr id="row165793521346"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.1 "><p id="p25753518411"><a name="p25753518411"></a><a name="p25753518411"></a>8</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.2 "><p id="p1557517515419"><a name="p1557517515419"></a><a name="p1557517515419"></a>LOS_ERRNO_EVENT_READ_IN_SYSTEM_TASK</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.3 "><p id="p95753519416"><a name="p95753519416"></a><a name="p95753519416"></a>0x02001c07</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.4 "><p id="p557565116415"><a name="p557565116415"></a><a name="p557565116415"></a>在系统任务中读取事件，如idle和软件定时器</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.5 "><p id="p1757515513413"><a name="p1757515513413"></a><a name="p1757515513413"></a>启动新的任务来获取事件</p>
</td>
</tr>
<tr id="row155781652048"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.1 "><p id="p1957515112415"><a name="p1957515112415"></a><a name="p1957515112415"></a>9</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.2 "><p id="p65759511419"><a name="p65759511419"></a><a name="p65759511419"></a>LOS_ERRNO_EVENT_SHOULD_NOT_DESTORY</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.3 "><p id="p257513511547"><a name="p257513511547"></a><a name="p257513511547"></a>0x02001c08</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.4 "><p id="p75752511415"><a name="p75752511415"></a><a name="p75752511415"></a>事件链表上仍有任务，无法被销毁</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.5 "><p id="p2575175111418"><a name="p2575175111418"></a><a name="p2575175111418"></a>先检查事件链表是否为空</p>
</td>
</tr>
</tbody>
</table>

**错误码定义：** 错误码是一个32位的无符号整型数存储单元，31\~24位表示错误等级，23\~16位表示错误码标志，15\~8位代表错误码所属模块，7\~0位表示错误码序号，如下所示：
```    
#define LOS_ERRNO_OS_ERROR(MID, ERRNO)  \

(LOS_ERRTYPE_ERROR | LOS_ERRNO_OS_ID | ((UINT32)(MID) << 8) | (ERRNO))

LOS_ERRTYPE_ERROR：Define critical OS errors

LOS_ERRNO_OS_ID：OS error code flag

MID：OS_MOUDLE_ID

LOS_MOD_EVENT：Event module ID

ERRNO：error ID number
```  

例如：
```  
#define LOS_ERRNO_EVENT_READ_IN_LOCK  LOS_ERRNO_OS_ERROR(LOS_MOD_EVENT, 0x05)
```  

#### 平台差异性
无。

### 注意事项

-   在系统初始化之前不能调用读写事件接口。如果调用，则系统运行会不正常。
-   在中断中，可以对事件对象进行写操作，但不能读操作。
-   在锁任务调度状态下，禁止任务阻塞与读事件。
-   LOS\_EventClear 入参值是：要清除的指定事件类型的反码（\~uwEvents）。
-   事件掩码的第25位不能使用，原因是为了区别LOS\_EventRead接口返回的是事件还是错误码。

### 编程实例

#### 实例描述

示例中，任务Example\_TaskEntry创建一个任务Example\_Event，Example\_Event读事件阻塞，Example\_TaskEntry向该任务写事件。

1.  在任务Example\_TaskEntry创建任务Example\_Event，其中任务Example\_Event优先级高于Example\_TaskEntry。
2.  在任务Example\_Event中读事件0x00000001，阻塞，发生任务切换，执行任务Example\_TaskEntry。
3.  在任务Example\_TaskEntry向任务Example\_Event写事件0x00000001，发生任务切换，执行任务Example\_Event。
4.  Example\_Event得以执行，直到任务结束。
5.  Example\_TaskEntry得以执行，直到任务结束。

#### 编程示例

可以通过打印的先后顺序理解事件操作时伴随的任务切换。

代码实现如下：

```
#include "los_event.h" 
#include "los_task.h" 
  
/*任务PID*/ 
UINT32 g_TestTaskID01; 
  
/*事件控制结构体*/ 
EVENT_CB_S  example_event; 
  
/*等待的事件类型*/ 
#define event_wait 0x00000001 
  
/*用例任务入口函数*/ 
VOID Example_Event() 
{ 
     UINT32 uwRet; 
     UINT32 uwEvent; 
  
     /*超时等待方式读事件,超时时间为100 Tick 
      若100 Tick 后未读取到指定事件，读事件超时，任务直接唤醒*/ 
     printf("Example_Event wait event 0x%x \n",event_wait); 
  
     uwEvent = LOS_EventRead(&example_event, event_wait, LOS_WAITMODE_AND, 100); 
     if(uwEvent == event_wait) 
     { 
         printf("Example_Event,read event :0x%x\n",uwEvent); 
     } 
     else 
         printf("Example_Event,read event timeout\n"); 
     return; 
} 
  
UINT32 Example_TaskEntry() 
{ 
     UINT32 uwRet; 
     TSK_INIT_PARAM_S stTask1; 
  
     /*事件初始化*/ 
     uwRet = LOS_EventInit(&example_event); 
     if(uwRet != LOS_OK) 
     { 
         printf("init event failed .\n"); 
         return -1; 
     } 
  
     /*创建任务*/ 
     memset(&stTask1, 0, sizeof(TSK_INIT_PARAM_S)); 
     stTask1.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_Event; 
     stTask1.pcName       = "EventTsk1"; 
     stTask1.uwStackSize  = OS_TSK_DEFAULT_STACK_SIZE; 
     stTask1.usTaskPrio   = 5; 
     uwRet = LOS_TaskCreate(&g_TestTaskID01, &stTask1); 
     if(uwRet != LOS_OK) 
     { 
         printf("task create failed .\n"); 
         return LOS_NOK; 
     } 
  
     /*写用例任务等待的事件类型*/ 
     printf("Example_TaskEntry write event .\n"); 
  
     uwRet = LOS_EventWrite(&example_event, event_wait); 
     if(uwRet != LOS_OK) 
     { 
         printf("event write failed .\n"); 
         return LOS_NOK; 
     } 
  
     /*清标志位*/ 
     printf("EventMask:%d\n",example_event.uwEventID); 
     LOS_EventClear(&example_event, ~example_event.uwEventID); 
     printf("EventMask:%d\n",example_event.uwEventID); 
  
     /*删除任务*/ 
     uwRet = LOS_TaskDelete(g_TestTaskID01); 
     if(uwRet != LOS_OK) 
     { 
         printf("task delete failed .\n"); 
         return LOS_NOK; 
     } 
  
     return LOS_OK; 
}
```


#### 结果验证

编译运行得到的结果为：

```
 Example_Event wait event 0x1  
 Example_TaskEntry write event . 
 Example_Event,read event :0x1 
 EventMask:1 
 EventMask:0
```

#### 完整实例代码

[sample\_event.c](resource/sample_event.c)

<h3 id="10">4.6 互斥锁</h3>

### 概述

#### 基本概念

互斥锁又称互斥型信号量，是一种特殊的二值性信号量，用于实现对共享资源的独占式处理。    

任意时刻互斥锁的状态只有两种，开锁或闭锁。当有任务持有时，互斥锁处于闭锁状态，这个任务获得该互斥锁的所有权。当该任务释放它时，该互斥锁被开锁，任务失去该互斥锁的所有权。当一个任务持有互斥锁时，其他任务将不能再对该互斥锁进行开锁或持有。

多任务环境下往往存在多个任务竞争同一共享资源的应用场景，互斥锁可被用于对共享资源的保护从而实现独占式访问。另外，互斥锁可以解决信号量存在的优先级翻转问题。

Huawei LiteOS提供的互斥锁具有如下特点：

-   通过优先级继承算法，解决优先级翻转问题。

#### 运作机制  

**互斥锁运作原理**  

多任务环境下会存在多个任务访问同一公共资源的场景，而有些公共资源是非共享的，需要任务进行独占式处理。互斥锁怎样来避免这种冲突呢？

用互斥锁处理非共享资源的同步访问时，如果有任务访问该资源，则互斥锁为加锁状态。此时其他任务如果想访问这个公共资源则会被阻塞，直到互斥锁被持有该锁的任务释放后，其他任务才能重新访问该公共资源，此时互斥锁再次上锁，如此确保同一时刻只有一个任务正在访问这个公共资源，保证了公共资源操作的完整性。

**图 1**  互斥锁运作示意图<a name="fig25049494142655"></a>  
![](figures/互斥锁运作示意图.png "互斥锁运作示意图")

### 开发指导

#### 使用场景  


互斥锁可以提供任务之间的互斥机制，用来防止两个任务在同一时刻访问相同的共享资源。

#### 功能  

Huawei LiteOS 系统中的互斥锁模块为用户提供下面几种功能。

<a name="table14234001162410"></a>
<table><thead align="left"><tr id="row20709259162410"><th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.1.4.1.1"><p id="p66837260162410"><a name="p66837260162410"></a><a name="p66837260162410"></a>功能分类</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.1.4.1.2"><p id="p45109011162410"><a name="p45109011162410"></a><a name="p45109011162410"></a>接口名</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.1.4.1.3"><p id="p29951237162410"><a name="p29951237162410"></a><a name="p29951237162410"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row10131148162410"><td class="cellrowborder" rowspan="2" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.1 "><p id="p15316688162410"><a name="p15316688162410"></a><a name="p15316688162410"></a>互斥锁的创建和删除</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.2 "><p id="p32692216162410"><a name="p32692216162410"></a><a name="p32692216162410"></a>LOS_MuxCreate</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.3 "><p id="p30823853162410"><a name="p30823853162410"></a><a name="p30823853162410"></a>创建互斥锁</p>
</td>
</tr>
<tr id="row8979223162410"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p56228490162410"><a name="p56228490162410"></a><a name="p56228490162410"></a>LOS_MuxDelete</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="p58213821162410"><a name="p58213821162410"></a><a name="p58213821162410"></a>删除指定的互斥锁</p>
</td>
</tr>
<tr id="row54162342162410"><td class="cellrowborder" rowspan="2" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.1 "><p id="p25073564162410"><a name="p25073564162410"></a><a name="p25073564162410"></a>互斥锁的申请和释放</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.2 "><p id="p17692840162410"><a name="p17692840162410"></a><a name="p17692840162410"></a>LOS_MuxPend</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.3 "><p id="p23833903162410"><a name="p23833903162410"></a><a name="p23833903162410"></a>申请指定的互斥锁</p>
</td>
</tr>
<tr id="row13178541162410"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p60828923162410"><a name="p60828923162410"></a><a name="p60828923162410"></a>LOS_MuxPost</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="p28195705162410"><a name="p28195705162410"></a><a name="p28195705162410"></a>释放指定的互斥锁</p>
</td>
</tr>
</tbody>
</table>

#### 开发流程

互斥锁典型场景的开发流程：

1.  创建互斥锁LOS\_MuxCreate。
2.  申请互斥锁LOS\_MuxPend。

    申请模式有三种：无阻塞模式、永久阻塞模式、定时阻塞模式。

    -   无阻塞模式：任务需要申请互斥锁，若该互斥锁当前没有任务持有，或者持有该互斥锁的任务和申请该互斥锁的任务为同一个任务，则申请成功。
    -   永久阻塞模式：任务需要申请互斥锁，若该互斥锁当前没有被占用，则申请成功。否则，该任务进入阻塞态，系统切换到就绪任务中优先级最高者继续执行。任务进入阻塞态后，直到有其他任务释放该互斥锁，阻塞任务才会重新得以执行。
    -   定时阻塞模式：任务需要申请互斥锁，若该互斥锁当前没有被占用，则申请成功。否则该任务进入阻塞态，系统切换到就绪任务中优先级最高者继续执行。任务进入阻塞态后，指定时间超时前有其他任务释放该互斥锁，或者用户指定时间超时后，阻塞任务才会重新得以执行。

3.  释放互斥锁LOS\_MuxPost。
    -   如果有任务阻塞于指定互斥锁，则唤醒最早被阻塞的任务中优先级最高的，该任务进入就绪态，并进行任务调度。
    -   如果没有任务阻塞于指定互斥锁，则互斥锁释放成功。

4.  删除互斥锁LOS\_MuxDelete。

#### 互斥锁错误码

对互斥锁存在失败的可能性操作，包括互斥锁创建，互斥锁删除，互斥锁申请，互斥锁释放。

<a name="table1155510185440"></a>
<table><thead align="left"><tr id="row30398401185440"><th class="cellrowborder" valign="top" width="20%" id="mcps1.1.6.1.1"><p id="p14509506185440"><a name="p14509506185440"></a><a name="p14509506185440"></a>序号</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.1.6.1.2"><p id="p34419344185440"><a name="p34419344185440"></a><a name="p34419344185440"></a>定义</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.1.6.1.3"><p id="p36503519185440"><a name="p36503519185440"></a><a name="p36503519185440"></a>实际数值</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.1.6.1.4"><p id="p3995095185440"><a name="p3995095185440"></a><a name="p3995095185440"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.1.6.1.5"><p id="p55167263185440"><a name="p55167263185440"></a><a name="p55167263185440"></a>参考解决方案</p>
</th>
</tr>
</thead>
<tbody><tr id="row26743320185440"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.1 "><p id="p18725340185440"><a name="p18725340185440"></a><a name="p18725340185440"></a>1</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.2 "><p id="p40357572185440"><a name="p40357572185440"></a><a name="p40357572185440"></a>LOS_ERRNO_MUX_NO_MEMORY</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.3 "><p id="p47737913185440"><a name="p47737913185440"></a><a name="p47737913185440"></a>0x02001d00</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.4 "><p id="p41565730185440"><a name="p41565730185440"></a><a name="p41565730185440"></a>内存请求失败</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.5 "><p id="p11380968185440"><a name="p11380968185440"></a><a name="p11380968185440"></a>减少互斥锁限制数量的上限</p>
</td>
</tr>
<tr id="row35319849185440"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.1 "><p id="p42335492185440"><a name="p42335492185440"></a><a name="p42335492185440"></a>2</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.2 "><p id="p6622832185440"><a name="p6622832185440"></a><a name="p6622832185440"></a>LOS_ERRNO_MUX_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.3 "><p id="p66687380185440"><a name="p66687380185440"></a><a name="p66687380185440"></a>0x02001d01</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.4 "><p id="p32968729185440"><a name="p32968729185440"></a><a name="p32968729185440"></a>互斥锁不可用</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.5 "><p id="p53221415185440"><a name="p53221415185440"></a><a name="p53221415185440"></a>传入有效的互斥锁的ID</p>
</td>
</tr>
<tr id="row9230688185440"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.1 "><p id="p9488269185440"><a name="p9488269185440"></a><a name="p9488269185440"></a>3</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.2 "><p id="p30352343185440"><a name="p30352343185440"></a><a name="p30352343185440"></a>LOS_ERRNO_MUX_PTR_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.3 "><p id="p42620709185440"><a name="p42620709185440"></a><a name="p42620709185440"></a>0x02001d02</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.4 "><p id="p29725408185440"><a name="p29725408185440"></a><a name="p29725408185440"></a>入参为空</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.5 "><p id="p58947823185440"><a name="p58947823185440"></a><a name="p58947823185440"></a>确保入参可用</p>
</td>
</tr>
<tr id="row60768362185440"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.1 "><p id="p23290259185440"><a name="p23290259185440"></a><a name="p23290259185440"></a>4</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.2 "><p id="p7462788185440"><a name="p7462788185440"></a><a name="p7462788185440"></a>LOS_ERRNO_MUX_ALL_BUSY</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.3 "><p id="p506086185440"><a name="p506086185440"></a><a name="p506086185440"></a>0x02001d03</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.4 "><p id="p40992992185440"><a name="p40992992185440"></a><a name="p40992992185440"></a>没有互斥锁可用</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.5 "><p id="p32098075185440"><a name="p32098075185440"></a><a name="p32098075185440"></a>增加互斥锁限制数量的上限</p>
</td>
</tr>
<tr id="row20447224185440"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.1 "><p id="p45612471185440"><a name="p45612471185440"></a><a name="p45612471185440"></a>5</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.2 "><p id="p3622666185440"><a name="p3622666185440"></a><a name="p3622666185440"></a>LOS_ERRNO_MUX_UNAVAILABLE</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.3 "><p id="p25000538185440"><a name="p25000538185440"></a><a name="p25000538185440"></a>0x02001d04</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.4 "><p id="p11777678185440"><a name="p11777678185440"></a><a name="p11777678185440"></a>锁失败，因为锁被其他线程使用</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.5 "><p id="p14467855185440"><a name="p14467855185440"></a><a name="p14467855185440"></a>等待其他线程解锁或者设置等待时间</p>
</td>
</tr>
<tr id="row63101831185440"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.1 "><p id="p10974693185440"><a name="p10974693185440"></a><a name="p10974693185440"></a>6</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.2 "><p id="p16534980185440"><a name="p16534980185440"></a><a name="p16534980185440"></a>LOS_ERRNO_MUX_PEND_INTERR</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.3 "><p id="p64265036185440"><a name="p64265036185440"></a><a name="p64265036185440"></a>0x02001d05</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.4 "><p id="p38085412185440"><a name="p38085412185440"></a><a name="p38085412185440"></a>在中断中使用互斥锁</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.5 "><p id="p65019508185440"><a name="p65019508185440"></a><a name="p65019508185440"></a>在中断中禁止调用此接口</p>
</td>
</tr>
<tr id="row48304665185440"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.1 "><p id="p20363793185440"><a name="p20363793185440"></a><a name="p20363793185440"></a>7</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.2 "><p id="p38854550185440"><a name="p38854550185440"></a><a name="p38854550185440"></a>LOS_ERRNO_MUX_PEND_IN_LOCK</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.3 "><p id="p60210831185440"><a name="p60210831185440"></a><a name="p60210831185440"></a>0x02001d06</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.4 "><p id="p45239141185440"><a name="p45239141185440"></a><a name="p45239141185440"></a>任务调度没有使能，线程等待另一个线程释放锁<span id="ph95220526912"><a name="ph95220526912"></a><a name="ph95220526912"></a>。</span></p>
<p id="p1431111531491"><a name="p1431111531491"></a><a name="p1431111531491"></a>锁任务调度情况下，不允许阻塞调度</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.5 "><p id="p40491775185440"><a name="p40491775185440"></a><a name="p40491775185440"></a>设置PEND为非阻塞模式或者使能任务调度</p>
</td>
</tr>
<tr id="row28881657185440"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.1 "><p id="p57712885185440"><a name="p57712885185440"></a><a name="p57712885185440"></a>8</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.2 "><p id="p44232105185440"><a name="p44232105185440"></a><a name="p44232105185440"></a>LOS_ERRNO_MUX_TIMEOUT</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.3 "><p id="p26030782185440"><a name="p26030782185440"></a><a name="p26030782185440"></a>0x02001d07</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.4 "><p id="p28118620185440"><a name="p28118620185440"></a><a name="p28118620185440"></a>互斥锁PEND超时</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.5 "><p id="p63015722185440"><a name="p63015722185440"></a><a name="p63015722185440"></a>增加等待时间或者设置一直等待模式</p>
</td>
</tr>
<tr id="row30270586185440"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.1 "><p id="p35998420185440"><a name="p35998420185440"></a><a name="p35998420185440"></a>9</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.2 "><p id="p30190898185440"><a name="p30190898185440"></a><a name="p30190898185440"></a>LOS_ERRNO_MUX_OVERFLOW</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.3 "><p id="p29543677185440"><a name="p29543677185440"></a><a name="p29543677185440"></a>0x02001d08</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.4 "><p id="p5187305519220"><a name="p5187305519220"></a><a name="p5187305519220"></a>暂未使用，待扩展</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.5 "><p id="p25666808185440"><a name="p25666808185440"></a><a name="p25666808185440"></a>无</p>
</td>
</tr>
<tr id="row29674680185440"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.1 "><p id="p54838841185440"><a name="p54838841185440"></a><a name="p54838841185440"></a>10</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.2 "><p id="p12761131185440"><a name="p12761131185440"></a><a name="p12761131185440"></a>LOS_ERRNO_MUX_PENDED</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.3 "><p id="p27018677185440"><a name="p27018677185440"></a><a name="p27018677185440"></a>0x02001d09</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.4 "><p id="p41029248185440"><a name="p41029248185440"></a><a name="p41029248185440"></a>删除正在使用的锁</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.5 "><p id="p35034804185440"><a name="p35034804185440"></a><a name="p35034804185440"></a>等待解锁再删除锁</p>
</td>
</tr>
<tr id="row46877780185440"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.1 "><p id="p39003831185440"><a name="p39003831185440"></a><a name="p39003831185440"></a>11</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.2 "><p id="p5193751185440"><a name="p5193751185440"></a><a name="p5193751185440"></a>LOS_ERRNO_MUX_GET_COUNT_ERR</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.3 "><p id="p18040667185440"><a name="p18040667185440"></a><a name="p18040667185440"></a>0x02001d0a</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.4 "><p id="p902268419229"><a name="p902268419229"></a><a name="p902268419229"></a>暂未使用，待扩展</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.5 "><p id="p51891633185440"><a name="p51891633185440"></a><a name="p51891633185440"></a>无</p>
</td>
</tr>
<tr id="row41674524185752"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.1 "><p id="p20193313185752"><a name="p20193313185752"></a><a name="p20193313185752"></a>12</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.2 "><p id="p25045679185752"><a name="p25045679185752"></a><a name="p25045679185752"></a>LOS_ERRNO_MUX_REG_ERROR</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.3 "><p id="p15434093185752"><a name="p15434093185752"></a><a name="p15434093185752"></a>0x02001d0b</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.4 "><p id="p42201987185752"><a name="p42201987185752"></a><a name="p42201987185752"></a>暂未使用，待扩展</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.5 "><p id="p62917786185752"><a name="p62917786185752"></a><a name="p62917786185752"></a>无</p>
</td>
</tr>
<tr id="row154421932191016"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.1 "><p id="p4423143113101"><a name="p4423143113101"></a><a name="p4423143113101"></a>13</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.2 "><p id="p442333114103"><a name="p442333114103"></a><a name="p442333114103"></a>LOS_ERRNO_MUX_PEND_IN_SYSTEM_TASK</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.3 "><p id="p174235314107"><a name="p174235314107"></a><a name="p174235314107"></a>0x02001d0c</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.4 "><p id="p19423931151011"><a name="p19423931151011"></a><a name="p19423931151011"></a>系统任务中获取mutex，如idle和软件定时器</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.6.1.5 "><p id="p114231431161019"><a name="p114231431161019"></a><a name="p114231431161019"></a>不要在系统任务中获取mutex</p>
</td>
</tr>
</tbody>
</table>

**错误码定义：** 错误码是一个32位的存储单元，31\~24位表示错误等级，23\~16位表示错误码标志，15\~8位代表错误码所属模块，7\~0位表示错误码序号，如下所示：
```  
#define LOS_ERRNO_OS_ERROR(MID, ERRNO)  \

(LOS_ERRTYPE_ERROR | LOS_ERRNO_OS_ID | ((UINT32)(MID) << 8) | (ERRNO))

LOS_ERRTYPE_ERROR：Define critical OS errors

LOS_ERRNO_OS_ID：OS error code flag

LOS_MOD_MUX：Mutex module ID

MID：OS_MOUDLE_ID

ERRNO：error ID number
```  

例如：
```  
LOS_ERRNO_MUX_TIMEOUT       LOS_ERRNO_OS_ERROR(LOS_MOD_MUX, 0x07)
```  

#### 平台差异性
无。

### 注意事项

-   两个任务不能对同一把互斥锁加锁。如果某任务对已被持有的互斥锁加锁，则该任务会被挂起，直到持有该锁的任务对互斥锁解锁，才能执行对这把互斥锁的加锁操作。
-   互斥锁不能在中断服务程序中使用。

-   Huawei LiteOS作为实时操作系统需要保证任务调度的实时性，尽量避免任务的长时间阻塞，因此在获得互斥锁之后，应该尽快释放互斥锁。
-   持有互斥锁的过程中，不得再调用LOS\_TaskPriSet等接口更改持有互斥锁任务的优先级。

### 编程实例

#### 实例描述

本实例实现如下流程。

1.  任务Example\_TaskEntry创建一个互斥锁，锁任务调度，创建两个任务Example\_MutexTask1、Example\_MutexTask2,Example\_MutexTask2优先级高于Example\_MutexTask1，解锁任务调度。
2.  Example\_MutexTask2被调度，永久申请互斥锁，然后任务休眠100Tick，Example\_MutexTask2挂起，Example\_MutexTask1被唤醒。
3.  Example\_MutexTask1申请互斥锁，等待时间为10Tick，因互斥锁仍被Example\_MutexTask2持有，Example\_MutexTask1挂起，10Tick后未拿到互斥锁，Example\_MutexTask1被唤醒，试图以永久等待申请互斥锁，Example\_MutexTask1挂起。
4.  100Tick后Example\_MutexTask2唤醒， 释放互斥锁后，Example\_MutexTask1被调度运行，最后释放互斥锁。
5.  Example\_MutexTask1执行完，300Tick后任务Example\_TaskEntry被调度运行，删除互斥锁。

#### 编程示例

前提条件：

-   在los\_config.h中，将LOSCFG\_BASE\_IPC\_MUX配置项打开。
-   配好LOSCFG\_BASE\_IPC\_MUX\_LIMIT最大的互斥锁个数。

代码实现如下：

```
 #include "los_mux.h" 
 #include "los_task.h" 
  
 /*互斥锁句柄ID*/ 
 MUX_HANDLE_T  g_Testmux01; 
 /*任务PID*/ 
 UINT32 g_TestTaskID01; 
 UINT32 g_TestTaskID02; 
  
 VOID Example_MutexTask1() 
 { 
     UINT32 uwRet; 
  
     printf("task1 try to get mutex, wait 10 Tick.\n"); 
     /*申请互斥锁*/ 
     uwRet=LOS_MuxPend(g_Testmux01, 10); 
  
     if(uwRet == LOS_OK) 
     { 
         printf("task1 get mutex g_Testmux01.\n"); 
         /*释放互斥锁*/ 
         LOS_MuxPost(g_Testmux01); 
         return; 
     } 
     else if(uwRet == LOS_ERRNO_MUX_TIMEOUT ) 
     { 
             printf("task1 timeout and try to get  mutex, wait forever.\n"); 
             /*申请互斥锁*/ 
             uwRet = LOS_MuxPend(g_Testmux01, LOS_WAIT_FOREVER); 
             if(uwRet == LOS_OK) 
             { 
                 printf("task1 wait forever,get mutex g_Testmux01.\n"); 
                 /*释放互斥锁*/ 
                 LOS_MuxPost(g_Testmux01); 
                 return; 
             } 
     } 
     return; 
 } 
  
 VOID Example_MutexTask2() 
 { 
     UINT32 uwRet; 
  
     printf("task2 try to get mutex, wait forever.\n"); 
     /*申请互斥锁*/ 
     uwRet=LOS_MuxPend(g_Testmux01, LOS_WAIT_FOREVER); 
  
     printf("task2 get mutex g_Testmux01 and suspend 100 Tick.\n"); 
  
     /*任务休眠100 Tick*/ 
     LOS_TaskDelay(100); 
  
     printf("task2 resumed and post the g_Testmux01\n"); 
     /*释放互斥锁*/ 
     LOS_MuxPost(g_Testmux01); 
     return; 
  
 } 
  
 UINT32 Example_TaskEntry() 
 { 
     UINT32 uwRet; 
     TSK_INIT_PARAM_S stTask1; 
     TSK_INIT_PARAM_S stTask2; 
  
     /*创建互斥锁*/ 
     LOS_MuxCreate(&g_Testmux01); 
  
     /*锁任务调度*/ 
     LOS_TaskLock(); 
  
     /*创建任务1*/ 
     memset(&stTask1, 0, sizeof(TSK_INIT_PARAM_S)); 
     stTask1.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_MutexTask1; 
     stTask1.pcName       = "MutexTsk1"; 
     stTask1.uwStackSize  = OS_TSK_DEFAULT_STACK_SIZE; 
     stTask1.usTaskPrio   = 5; 
     uwRet = LOS_TaskCreate(&g_TestTaskID01, &stTask1); 
     if(uwRet != LOS_OK) 
     { 
         printf("task1 create failed .\n"); 
         return LOS_NOK; 
     } 
  
     /*创建任务2*/ 
     memset(&stTask2, 0, sizeof(TSK_INIT_PARAM_S)); 
     stTask2.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_MutexTask2; 
     stTask2.pcName       = "MutexTsk2"; 
     stTask2.uwStackSize  = OS_TSK_DEFAULT_STACK_SIZE; 
     stTask2.usTaskPrio   = 4; 
     uwRet = LOS_TaskCreate(&g_TestTaskID02, &stTask2); 
     if(uwRet != LOS_OK) 
     { 
         printf("task2 create failed .\n"); 
         return LOS_NOK; 
     } 
  
     /*解锁任务调度*/ 
     LOS_TaskUnlock(); 
     /*任务休眠300 Tick*/ 
     LOS_TaskDelay(300); 
  
     /*删除互斥锁*/ 
     LOS_MuxDelete(g_Testmux01); 
  
     /*删除任务1*/ 
     uwRet = LOS_TaskDelete(g_TestTaskID01); 
     if(uwRet != LOS_OK) 
     { 
         printf("task1 delete failed .\n"); 
         return LOS_NOK; 
     } 
     /*删除任务2*/ 
     uwRet = LOS_TaskDelete(g_TestTaskID02); 
     if(uwRet != LOS_OK) 
     { 
         printf("task2 delete failed .\n"); 
         return LOS_NOK; 
     } 
  
     return LOS_OK; 
 }
```

#### 结果验证

编译运行得到的结果为：

```
task2 try to get  mutex, wait forever.
task2 get mutex g_Testmux01 and suspend 100 ticks.
task1 try to get  mutex, wait 10 ticks.
task1 timeout and try to get  mutex, wait forever.
task2 resumed and post the g_Testmux01
task1 wait forever,get mutex g_Testmux01.
```

#### 完整实例代码

[sample\_mutex.c](resource/sample_mutex.c)

<h3 id="11">4.7 信号量</h3>

### 概述

#### 基本概念

信号量（Semaphore）是一种实现任务间通信的机制，实现任务之间同步或临界资源的互斥访问。常用于协助一组相互竞争的任务来访问临界资源。  

在多任务系统中，各任务之间需要同步或互斥实现临界资源的保护，信号量功能可以为用户提供这方面的支持。

通常一个信号量的计数值用于对应有效的资源数，表示剩下的可被占用的互斥资源数。其值的含义分两种情况：

-   0，表示没有积累下来的Post操作，且有可能有在此信号量上阻塞的任务。
-   正值，表示有一个或多个Post下来的释放操作。

以同步为目的的信号量和以互斥为目的的信号量在使用有如下不同：

-   用作互斥时，信号量创建后记数是满的，在需要使用临界资源时，先取信号量，使其变空，这样其他任务需要使用临界资源时就会因为无法取到信号量而阻塞，从而保证了临界资源的安全。
-   用作同步时，信号量在创建后被置为空，任务1取信号量而阻塞，任务2在某种条件发生后，释放信号量，于是任务1得以进入READY或RUNNING态，从而达到了两个任务间的同步。

#### 运作机制

**信号量控制块**

```
 /** 
   * @ingroup los_sem 
   * Semaphore control structure. 
   */ 
 typedef struct 
 { 
     UINT8           semStat;          /**是否使用标志位*/ 
     UINT16          semCount;         /**信号量计数*/ 
     UINT32          semID;            /**信号量索引号*/ 
     LOS_DL_LIST     semList;          /**挂接阻塞于该信号量的任务*/ 
 }LosSemCB;
```

**信号量运作原理**

信号量初始化，为配置的N个信号量申请内存（N值可以由用户自行配置，受内存限制），并把所有的信号量初始化成未使用，并加入到未使用链表中供系统使用。

信号量创建，从未使用的信号量链表中获取一个信号量资源，并设定初值。

信号量申请，若其计数器值大于0，则直接减1返回成功。否则任务阻塞，等待其它任务释放该信号量，等待的超时时间可设定。当任务被一个信号量阻塞时，将该任务挂到信号量等待任务队列的队尾。

信号量释放，若没有任务等待该信号量，则直接将计数器加1返回。否则唤醒该信号量等待任务队列上的第一个任务。

信号量删除，将正在使用的信号量置为未使用信号量，并挂回到未使用链表。

信号量允许多个任务在同一时刻访问同一资源，但会限制同一时刻访问此资源的最大任务数目。访问同一资源的任务数达到该资源的最大数量时，会阻塞其他试图获取该资源的任务，直到有任务释放该信号量。

**图 1**  信号量运作示意图<a name="f0eb1f40dd27c4934b3ffcc9f7f377b72"></a>  
![](figures/信号量运作示意图.png "信号量运作示意图")

### 开发指导

#### 使用场景

信号量是一种非常灵活的同步方式，可以运用在多种场合中，实现锁、同步、资源计数等功能，也能方便的用于任务与任务，中断与任务的同步中。

#### 功能

Huawei LiteOS 系统中的信号量模块为用户提供下面几种功能。

<a name="t28c1fc7e673f4cb4930058156187c699"></a>
<table><thead align="left"><tr id="r927cb6bd1e324f9abe077f305a7a093e"><th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.1.4.1.1"><p id="a742b6c0d7b514d52a2229c72f9b1f216"><a name="a742b6c0d7b514d52a2229c72f9b1f216"></a><a name="a742b6c0d7b514d52a2229c72f9b1f216"></a>功能分类</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.1.4.1.2"><p id="a6ad583f70e074a218e4726874116de41"><a name="a6ad583f70e074a218e4726874116de41"></a><a name="a6ad583f70e074a218e4726874116de41"></a>接口名</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.1.4.1.3"><p id="a89d65d26732643659bdc4ef0e28b9095"><a name="a89d65d26732643659bdc4ef0e28b9095"></a><a name="a89d65d26732643659bdc4ef0e28b9095"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="r5a051177ff144ab39d1dbe562df8cfe8"><td class="cellrowborder" rowspan="3" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.1 "><p id="a0c59bb40999c431183de51bb63072691"><a name="a0c59bb40999c431183de51bb63072691"></a><a name="a0c59bb40999c431183de51bb63072691"></a>信号量的创建和删除</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.2 "><p id="a93ead5d8e7e845e8afbf84757bf15c92"><a name="a93ead5d8e7e845e8afbf84757bf15c92"></a><a name="a93ead5d8e7e845e8afbf84757bf15c92"></a>LOS_SemCreate</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.3 "><p id="aa5cf7d1a382c40de8b3a7c7379756f6c"><a name="aa5cf7d1a382c40de8b3a7c7379756f6c"></a><a name="aa5cf7d1a382c40de8b3a7c7379756f6c"></a>创建信号量</p>
</td>
</tr>
<tr id="row1036914452572"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p16369114545712"><a name="p16369114545712"></a><a name="p16369114545712"></a>LOS_BinarySemCreate</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="p236994565712"><a name="p236994565712"></a><a name="p236994565712"></a>创建二进制信号量</p>
</td>
</tr>
<tr id="r24c0f3bfcbbf4992afdeb3d3ec559b63"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="zh-cn_topic_0015796604_p161317214406"><a name="zh-cn_topic_0015796604_p161317214406"></a><a name="zh-cn_topic_0015796604_p161317214406"></a>LOS_SemDelete</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="a37b86358398740fd92ae8af48d19c0b9"><a name="a37b86358398740fd92ae8af48d19c0b9"></a><a name="a37b86358398740fd92ae8af48d19c0b9"></a>删除指定的信号量</p>
</td>
</tr>
<tr id="r35be22c92cdb4082b86d54ebbbc4b3fb"><td class="cellrowborder" rowspan="2" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.1 "><p id="aecf1d71161694dda83763b4a9389fb9d"><a name="aecf1d71161694dda83763b4a9389fb9d"></a><a name="aecf1d71161694dda83763b4a9389fb9d"></a>信号量的申请和释放</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.2 "><p id="a7cde8fb77c304a9cb3e49353ae69e11a"><a name="a7cde8fb77c304a9cb3e49353ae69e11a"></a><a name="a7cde8fb77c304a9cb3e49353ae69e11a"></a>LOS_SemPend</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.3 "><p id="ac2701f7d1055466f81f1b2cf8ba96591"><a name="ac2701f7d1055466f81f1b2cf8ba96591"></a><a name="ac2701f7d1055466f81f1b2cf8ba96591"></a>申请指定的信号量</p>
</td>
</tr>
<tr id="r9612396d00174084a19916c914524680"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="a3d99d71ec99f425fb8843efcc987020e"><a name="a3d99d71ec99f425fb8843efcc987020e"></a><a name="a3d99d71ec99f425fb8843efcc987020e"></a>LOS_SemPost</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="a5415b9a1021640f8b3bd27904e5273bd"><a name="a5415b9a1021640f8b3bd27904e5273bd"></a><a name="a5415b9a1021640f8b3bd27904e5273bd"></a>释放指定的信号量</p>
</td>
</tr>
</tbody>
</table>

#### 开发流程

信号量的开发典型流程：

1.  创建信号量LOS\_SemCreate。
2.  申请信号量LOS\_SemPend。

    信号量有三种申请模式：无阻塞模式、永久阻塞模式、定时阻塞模式。

    -   无阻塞模式：任务需要申请信号量，若当前信号量的任务数没有到信号量设定的上限，则申请成功。否则，立即返回申请失败。
    -   永久阻塞模式：任务需要申请信号量，若当前信号量的任务数没有到信号量设定的上限，则申请成功。否则，该任务进入阻塞态，系统切换到就绪任务中优先级最高者继续执行。任务进入阻塞态后，直到有其他任务释放该信号量，阻塞任务才会重新得以执行。
    -   定时阻塞模式：任务需要申请信号量，若当前信号量的任务数没有到信号量设定的上限，则申请成功。否则，该任务进入阻塞态，系统切换到就绪任务中优先级最高者继续执行。任务进入阻塞态后，指定时间超时前有其他任务释放该信号量，或者用户指定时间超时后，阻塞任务才会重新得以执行。

3.  释放信号量LOS\_SemPost。
    -   如果有任务阻塞于指定信号量，则唤醒该信号量阻塞队列上的第一个任务。该任务进入就绪态，并进行调度。
    -   如果没有任务阻塞于指定信号量，释放信号量成功。

4.  删除信号量LOS\_SemDelete。


#### 信号量错误码

对可能导致信号量操作失败的情况，包括创建信号量、申请信号量、释放信号量、删除信号量等，均需要返回对应的错误码，以便快速定位错误原因。

<a name="t56e7d693658c4e4da3a8ea5312c023c7"></a>
<table><thead align="left"><tr id="r8641959187cc42838ceec51ece1da8c6"><th class="cellrowborder" valign="top" width="7.5200000000000005%" id="mcps1.1.6.1.1"><p id="af964febd3faf4c3baa055d3fcbbd4669"><a name="af964febd3faf4c3baa055d3fcbbd4669"></a><a name="af964febd3faf4c3baa055d3fcbbd4669"></a>序号</p>
</th>
<th class="cellrowborder" valign="top" width="26.75%" id="mcps1.1.6.1.2"><p id="zh-cn_topic_0015796604_p261046995642"><a name="zh-cn_topic_0015796604_p261046995642"></a><a name="zh-cn_topic_0015796604_p261046995642"></a>定义</p>
</th>
<th class="cellrowborder" valign="top" width="15.85%" id="mcps1.1.6.1.3"><p id="a315389d98cfd4377a3ea32e6e7d4020b"><a name="a315389d98cfd4377a3ea32e6e7d4020b"></a><a name="a315389d98cfd4377a3ea32e6e7d4020b"></a>实际数值</p>
</th>
<th class="cellrowborder" valign="top" width="24.13%" id="mcps1.1.6.1.4"><p id="abe0bad2247fc441c9df02612db832894"><a name="abe0bad2247fc441c9df02612db832894"></a><a name="abe0bad2247fc441c9df02612db832894"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="25.75%" id="mcps1.1.6.1.5"><p id="a3f4ad73eaa964bed8f8d38a13ab62feb"><a name="a3f4ad73eaa964bed8f8d38a13ab62feb"></a><a name="a3f4ad73eaa964bed8f8d38a13ab62feb"></a>参考解决方案</p>
</th>
</tr>
</thead>
<tbody><tr id="r60b94aff9f2b4583bc8c76cebacd9b57"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="a5ca9b4f4f9484332b43199e23a19756e"><a name="a5ca9b4f4f9484332b43199e23a19756e"></a><a name="a5ca9b4f4f9484332b43199e23a19756e"></a>1</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.6.1.2 "><p id="ace602ae1339548078306b3258977d9b3"><a name="ace602ae1339548078306b3258977d9b3"></a><a name="ace602ae1339548078306b3258977d9b3"></a>LOS_ERRNO_SEM_NO_MEMORY</p>
</td>
<td class="cellrowborder" valign="top" width="15.85%" headers="mcps1.1.6.1.3 "><p id="a731ccbf1c4a24021a0dc9f4bc9cfe710"><a name="a731ccbf1c4a24021a0dc9f4bc9cfe710"></a><a name="a731ccbf1c4a24021a0dc9f4bc9cfe710"></a>0x02000700</p>
</td>
<td class="cellrowborder" valign="top" width="24.13%" headers="mcps1.1.6.1.4 "><p id="zh-cn_topic_0015796604_p492720095642"><a name="zh-cn_topic_0015796604_p492720095642"></a><a name="zh-cn_topic_0015796604_p492720095642"></a>内存空间不足。</p>
</td>
<td class="cellrowborder" valign="top" width="25.75%" headers="mcps1.1.6.1.5 "><p id="a710d2127537d491b844e06abf8e5478f"><a name="a710d2127537d491b844e06abf8e5478f"></a><a name="a710d2127537d491b844e06abf8e5478f"></a>分配更大的内存分区。</p>
</td>
</tr>
<tr id="r1bac74f19a114557a3c6a7b725b8dc26"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="a09bfd2b32dbc4a838cb00ce6b656ef32"><a name="a09bfd2b32dbc4a838cb00ce6b656ef32"></a><a name="a09bfd2b32dbc4a838cb00ce6b656ef32"></a>2</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.6.1.2 "><p id="ae061fe0f0c204736a92b8c8c2d062bcf"><a name="ae061fe0f0c204736a92b8c8c2d062bcf"></a><a name="ae061fe0f0c204736a92b8c8c2d062bcf"></a>LOS_ERRNO_SEM_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="15.85%" headers="mcps1.1.6.1.3 "><p id="a41e8513b98494396acb01ec4a7fdf591"><a name="a41e8513b98494396acb01ec4a7fdf591"></a><a name="a41e8513b98494396acb01ec4a7fdf591"></a>0x02000701</p>
</td>
<td class="cellrowborder" valign="top" width="24.13%" headers="mcps1.1.6.1.4 "><p id="a35d9ee34863d4b909e18182e11654574"><a name="a35d9ee34863d4b909e18182e11654574"></a><a name="a35d9ee34863d4b909e18182e11654574"></a>非法传参。</p>
</td>
<td class="cellrowborder" valign="top" width="25.75%" headers="mcps1.1.6.1.5 "><p id="zh-cn_topic_0015796604_p412423410026"><a name="zh-cn_topic_0015796604_p412423410026"></a><a name="zh-cn_topic_0015796604_p412423410026"></a>改变传数为合法值。</p>
</td>
</tr>
<tr id="r382730385dba4b0c951a92d6a3a02e75"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="a99569d09a45d4f309cd4a559b5bab224"><a name="a99569d09a45d4f309cd4a559b5bab224"></a><a name="a99569d09a45d4f309cd4a559b5bab224"></a>3</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.6.1.2 "><p id="af36aa7cff56946efa19cb8de476507f6"><a name="af36aa7cff56946efa19cb8de476507f6"></a><a name="af36aa7cff56946efa19cb8de476507f6"></a>LOS_ERRNO_SEM_PTR_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="15.85%" headers="mcps1.1.6.1.3 "><p id="a32d34a48d68e4394ae1620813071e398"><a name="a32d34a48d68e4394ae1620813071e398"></a><a name="a32d34a48d68e4394ae1620813071e398"></a>0x02000702</p>
</td>
<td class="cellrowborder" valign="top" width="24.13%" headers="mcps1.1.6.1.4 "><p id="aa3ad26e4ae4c4fbb890f72617fbf5d21"><a name="aa3ad26e4ae4c4fbb890f72617fbf5d21"></a><a name="aa3ad26e4ae4c4fbb890f72617fbf5d21"></a>传入空指针。</p>
</td>
<td class="cellrowborder" valign="top" width="25.75%" headers="mcps1.1.6.1.5 "><p id="a2f472fc469034f4cb41982a6707b6da1"><a name="a2f472fc469034f4cb41982a6707b6da1"></a><a name="a2f472fc469034f4cb41982a6707b6da1"></a>传入合法指针。</p>
</td>
</tr>
<tr id="r9b80bef61da047279fde52dc4a45f75c"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="a4f9b6878a73a4d65951f4cf705292152"><a name="a4f9b6878a73a4d65951f4cf705292152"></a><a name="a4f9b6878a73a4d65951f4cf705292152"></a>4</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.6.1.2 "><p id="a973aee19f2fe417aac6e6426230d401b"><a name="a973aee19f2fe417aac6e6426230d401b"></a><a name="a973aee19f2fe417aac6e6426230d401b"></a>LOS_ERRNO_SEM_ALL_BUSY</p>
</td>
<td class="cellrowborder" valign="top" width="15.85%" headers="mcps1.1.6.1.3 "><p id="a38927a37386d4e688f5054dcd3c3c03b"><a name="a38927a37386d4e688f5054dcd3c3c03b"></a><a name="a38927a37386d4e688f5054dcd3c3c03b"></a>0x02000703</p>
</td>
<td class="cellrowborder" valign="top" width="24.13%" headers="mcps1.1.6.1.4 "><p id="zh-cn_topic_0015796604_p915662595642"><a name="zh-cn_topic_0015796604_p915662595642"></a><a name="zh-cn_topic_0015796604_p915662595642"></a>信号量控制块不可用。</p>
</td>
<td class="cellrowborder" valign="top" width="25.75%" headers="mcps1.1.6.1.5 "><p id="ac8834f2462fe4b31ae22c88399dca62a"><a name="ac8834f2462fe4b31ae22c88399dca62a"></a><a name="ac8834f2462fe4b31ae22c88399dca62a"></a>释放资源信号量资源。</p>
</td>
</tr>
<tr id="r778a665e19674851aed734175b7aa513"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="a0a76b7d2df194358971109e4eb5df16b"><a name="a0a76b7d2df194358971109e4eb5df16b"></a><a name="a0a76b7d2df194358971109e4eb5df16b"></a>5</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.6.1.2 "><p id="af2587bfc1967452880e55470749ae3c5"><a name="af2587bfc1967452880e55470749ae3c5"></a><a name="af2587bfc1967452880e55470749ae3c5"></a>LOS_ERRNO_SEM_UNAVAILABLE</p>
</td>
<td class="cellrowborder" valign="top" width="15.85%" headers="mcps1.1.6.1.3 "><p id="zh-cn_topic_0015796604_p804161495642"><a name="zh-cn_topic_0015796604_p804161495642"></a><a name="zh-cn_topic_0015796604_p804161495642"></a>0x02000704</p>
</td>
<td class="cellrowborder" valign="top" width="24.13%" headers="mcps1.1.6.1.4 "><p id="a1725880d67094409afb5203f7e0b746d"><a name="a1725880d67094409afb5203f7e0b746d"></a><a name="a1725880d67094409afb5203f7e0b746d"></a>定时时间非法。</p>
</td>
<td class="cellrowborder" valign="top" width="25.75%" headers="mcps1.1.6.1.5 "><p id="aefe321af35574be299f6dd944f5f380d"><a name="aefe321af35574be299f6dd944f5f380d"></a><a name="aefe321af35574be299f6dd944f5f380d"></a>传入正确的定时时间。</p>
</td>
</tr>
<tr id="ra7d5108b8d51423db9383e442e8fa665"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="a33a9bb958c664d50bb941d692a6802f2"><a name="a33a9bb958c664d50bb941d692a6802f2"></a><a name="a33a9bb958c664d50bb941d692a6802f2"></a>6</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.6.1.2 "><p id="a32d5abc13d2e4c0c9856dd7f985a8a25"><a name="a32d5abc13d2e4c0c9856dd7f985a8a25"></a><a name="a32d5abc13d2e4c0c9856dd7f985a8a25"></a>LOS_ERRNO_SEM_PEND_INTERR</p>
</td>
<td class="cellrowborder" valign="top" width="15.85%" headers="mcps1.1.6.1.3 "><p id="ad2ef3afcbb4444708ce051d8943632af"><a name="ad2ef3afcbb4444708ce051d8943632af"></a><a name="ad2ef3afcbb4444708ce051d8943632af"></a>0x02000705</p>
</td>
<td class="cellrowborder" valign="top" width="24.13%" headers="mcps1.1.6.1.4 "><p id="af7c8bf4264a6460a8473fd25a8df17fe"><a name="af7c8bf4264a6460a8473fd25a8df17fe"></a><a name="af7c8bf4264a6460a8473fd25a8df17fe"></a>中断期间非法调用LOS_SemPend。</p>
</td>
<td class="cellrowborder" valign="top" width="25.75%" headers="mcps1.1.6.1.5 "><p id="a48827464b66049fea10eb9ca923f502b"><a name="a48827464b66049fea10eb9ca923f502b"></a><a name="a48827464b66049fea10eb9ca923f502b"></a>中断期间禁止调用LOS_SemPend。</p>
</td>
</tr>
<tr id="ra4557a53be2d4c179b0a6a3ad7821641"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="a5e8ee65880134339bb90ca98097da745"><a name="a5e8ee65880134339bb90ca98097da745"></a><a name="a5e8ee65880134339bb90ca98097da745"></a>7</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.6.1.2 "><p id="a457ed8e561404582806dbc950ad20cde"><a name="a457ed8e561404582806dbc950ad20cde"></a><a name="a457ed8e561404582806dbc950ad20cde"></a>LOS_ERRNO_SEM_PEND_IN_LOCK</p>
</td>
<td class="cellrowborder" valign="top" width="15.85%" headers="mcps1.1.6.1.3 "><p id="a5e356084ec484c6f90dcef9e5ed02ca1"><a name="a5e356084ec484c6f90dcef9e5ed02ca1"></a><a name="a5e356084ec484c6f90dcef9e5ed02ca1"></a>0x02000706</p>
</td>
<td class="cellrowborder" valign="top" width="24.13%" headers="mcps1.1.6.1.4 "><p id="ac0b0708fbe804f0dba4eee980f363a10"><a name="ac0b0708fbe804f0dba4eee980f363a10"></a><a name="ac0b0708fbe804f0dba4eee980f363a10"></a>任务被锁，无法获得信号量。</p>
</td>
<td class="cellrowborder" valign="top" width="25.75%" headers="mcps1.1.6.1.5 "><p id="a4085fc7767c0484998cd9cef6fc97a11"><a name="a4085fc7767c0484998cd9cef6fc97a11"></a><a name="a4085fc7767c0484998cd9cef6fc97a11"></a>在任务被锁时，不能调用LOS_SemPend。</p>
</td>
</tr>
<tr id="rcd5941a10d30491cabb902f8ac53b06c"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="a498950b28810479abaeafabd7d005c49"><a name="a498950b28810479abaeafabd7d005c49"></a><a name="a498950b28810479abaeafabd7d005c49"></a>8</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.6.1.2 "><p id="a899ea31b3cb942cb90010dbf7a833221"><a name="a899ea31b3cb942cb90010dbf7a833221"></a><a name="a899ea31b3cb942cb90010dbf7a833221"></a>LOS_ERRNO_SEM_TIMEOUT</p>
</td>
<td class="cellrowborder" valign="top" width="15.85%" headers="mcps1.1.6.1.3 "><p id="ab64dd20d5fcd4fbb892273cfc9f617cf"><a name="ab64dd20d5fcd4fbb892273cfc9f617cf"></a><a name="ab64dd20d5fcd4fbb892273cfc9f617cf"></a>0x02000707</p>
</td>
<td class="cellrowborder" valign="top" width="24.13%" headers="mcps1.1.6.1.4 "><p id="a65615b5f0fc64791a4ec52aa8a83e431"><a name="a65615b5f0fc64791a4ec52aa8a83e431"></a><a name="a65615b5f0fc64791a4ec52aa8a83e431"></a>获取信号量时间超时。</p>
</td>
<td class="cellrowborder" valign="top" width="25.75%" headers="mcps1.1.6.1.5 "><p id="a5e513110045d4af6adb509c9fd658925"><a name="a5e513110045d4af6adb509c9fd658925"></a><a name="a5e513110045d4af6adb509c9fd658925"></a>将时间设置在合理范围内。</p>
</td>
</tr>
<tr id="rd5ffcfd9ecdf43fcbde154fc6f32c424"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="a1da93d9ec9614a8db84e09041e9c5dd1"><a name="a1da93d9ec9614a8db84e09041e9c5dd1"></a><a name="a1da93d9ec9614a8db84e09041e9c5dd1"></a>9</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.6.1.2 "><p id="a7433cb3b9953437cab163eaf79725b70"><a name="a7433cb3b9953437cab163eaf79725b70"></a><a name="a7433cb3b9953437cab163eaf79725b70"></a>LOS_ERRNO_SEM_OVERFLOW</p>
</td>
<td class="cellrowborder" valign="top" width="15.85%" headers="mcps1.1.6.1.3 "><p id="aefcd77ff4a3d4002ab5566f4d6be90ec"><a name="aefcd77ff4a3d4002ab5566f4d6be90ec"></a><a name="aefcd77ff4a3d4002ab5566f4d6be90ec"></a>0x02000708</p>
</td>
<td class="cellrowborder" valign="top" width="24.13%" headers="mcps1.1.6.1.4 "><p id="abb0b0f3b45a4436ba783104458cbc2d4"><a name="abb0b0f3b45a4436ba783104458cbc2d4"></a><a name="abb0b0f3b45a4436ba783104458cbc2d4"></a>信号量允许pend次数超过最大值。</p>
</td>
<td class="cellrowborder" valign="top" width="25.75%" headers="mcps1.1.6.1.5 "><p id="af0cd524fd5664518a54a874e9eb2c02f"><a name="af0cd524fd5664518a54a874e9eb2c02f"></a><a name="af0cd524fd5664518a54a874e9eb2c02f"></a>传入合法的值。</p>
</td>
</tr>
<tr id="r78d187524d8645c1885b5e0d33466256"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="af80265f9e39e4d24a52047f7f1117e6f"><a name="af80265f9e39e4d24a52047f7f1117e6f"></a><a name="af80265f9e39e4d24a52047f7f1117e6f"></a>10</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.6.1.2 "><p id="adf6fb59e7c204e6ea4af888e9ead790e"><a name="adf6fb59e7c204e6ea4af888e9ead790e"></a><a name="adf6fb59e7c204e6ea4af888e9ead790e"></a>LOS_ERRNO_SEM_PENDED</p>
</td>
<td class="cellrowborder" valign="top" width="15.85%" headers="mcps1.1.6.1.3 "><p id="a8045eb3297cf4464b47b1da0cb7b1171"><a name="a8045eb3297cf4464b47b1da0cb7b1171"></a><a name="a8045eb3297cf4464b47b1da0cb7b1171"></a>0x02000709</p>
</td>
<td class="cellrowborder" valign="top" width="24.13%" headers="mcps1.1.6.1.4 "><p id="a2debc14eb0534bfd81a0c5a46ba7d5ec"><a name="a2debc14eb0534bfd81a0c5a46ba7d5ec"></a><a name="a2debc14eb0534bfd81a0c5a46ba7d5ec"></a>等待信号量的任务队列不为空。</p>
</td>
<td class="cellrowborder" valign="top" width="25.75%" headers="mcps1.1.6.1.5 "><p id="a4203fe9fc4fc4bcdb6c507eea981bfdd"><a name="a4203fe9fc4fc4bcdb6c507eea981bfdd"></a><a name="a4203fe9fc4fc4bcdb6c507eea981bfdd"></a>唤醒所有等待该型号量的任务后删除该信号量。</p>
</td>
</tr>
<tr id="row1588124218146"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="p4809194061414"><a name="p4809194061414"></a><a name="p4809194061414"></a>11</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.6.1.2 "><p id="p1380910407142"><a name="p1380910407142"></a><a name="p1380910407142"></a>LOS_ERRNO_SEM_PEND_IN_SYSTEM_TASK</p>
</td>
<td class="cellrowborder" valign="top" width="15.85%" headers="mcps1.1.6.1.3 "><p id="p14809204011415"><a name="p14809204011415"></a><a name="p14809204011415"></a>0x0200070a</p>
</td>
<td class="cellrowborder" valign="top" width="24.13%" headers="mcps1.1.6.1.4 "><p id="p3809340171415"><a name="p3809340171415"></a><a name="p3809340171415"></a>系统任务中获取sem，如idle和软件定时器</p>
</td>
<td class="cellrowborder" valign="top" width="25.75%" headers="mcps1.1.6.1.5 "><p id="p4809840141419"><a name="p4809840141419"></a><a name="p4809840141419"></a>不要在系统任务中获取sem</p>
</td>
</tr>
</tbody>
</table>

**错误码定义：** 错误码是一个32位的存储单元，31\~24位表示错误等级，23\~16位表示错误码标志，15\~8位代表错误码所属模块，7\~0位表示错误码序号，如下所示：

```
#define LOS_ERRNO_OS_NORMAL(MID,ERRNO)  \
(LOS_ERRTYPE_NORMAL | LOS_ERRNO_OS_ID | ((UINT32)(MID) << 8) | (ERRNO))
LOS_ERRTYPE_NORMAL ：Define the error level as critical
LOS_ERRNO_OS_ID ：OS error code flag.
MID：OS_MOUDLE_ID
ERRNO：error ID number
```

例如：

```
LOS_ERRNO_SEM_NO_MEMORY        LOS_ERRNO_OS_ERROR(LOS_MOD_SEM, 0x00))
```

#### 平台差异性
无。  

### 注意事项

-   由于中断不能被阻塞，因此在申请信号量时，阻塞模式不能在中断中使用。

### 编程实例

#### 实例描述

本实例实现如下功能：

1.  测试任务Example\_TaskEntry创建一个信号量，锁任务调度，创建两个任务Example\_SemTask1、Example\_SemTask2,Example\_SemTask2优先级高于Example\_SemTask1，两个任务中申请同一信号量，解锁任务调度后两任务阻塞，测试任务Example\_TaskEntry释放信号量。
2.  Example\_SemTask2得到信号量，被调度，然后任务休眠20Tick，Example\_SemTask2延迟，Example\_SemTask1被唤醒。
3.  Example\_SemTask1定时阻塞模式申请信号量，等待时间为10Tick，因信号量仍被Example\_SemTask2持有，Example\_SemTask1挂起，10Tick后仍未得到信号量，Example\_SemTask1被唤醒，试图以永久阻塞模式申请信号量，Example\_SemTask1挂起。
4.  20Tick后Example\_SemTask2唤醒， 释放信号量后，Example\_SemTask1得到信号量被调度运行，最后释放信号量。
5.  Example\_SemTask1执行完，40Tick后任务Example\_TaskEntry被唤醒。执行删除信号量，删除两个任务。

#### 编程示例

前提条件：

-   在los\_config.h中，将LOSCFG\_BASE\_IPC\_SEM配置为YES。
-   配置用户定义的LOSCFG\_BASE\_IPC\_SEM\_LIMIT最大的信号量数，如1024。

代码实现如下：

```
 #include "los_sem.h" 
   
 /*任务PID*/ 
 static UINT32 g_TestTaskID01,g_TestTaskID02; 
 /*测试任务优先级*/ 
 #define TASK_PRIO_TEST  5 
 /*信号量结构体ID*/ 
 static SEM_HANDLE_T g_usSemID; 
  
  
 VOID Example_SemTask1(void) 
 { 
     UINT32 uwRet; 
  
     printf("Example_SemTask1 try get sem g_usSemID ,timeout 10 ticks.\n"); 
     /*定时阻塞模式申请信号量，定时时间为10Tick*/ 
     uwRet = LOS_SemPend(g_usSemID, 10); 
  
     /*申请到信号量*/ 
     if(LOS_OK == uwRet) 
     { 
          LOS_SemPost(g_usSemID); 
          return; 
     } 
     /*定时时间到，未申请到信号量*/ 
     if(LOS_ERRNO_SEM_TIMEOUT == uwRet) 
     { 
         printf("Example_SemTask1 timeout and try get sem g_usSemID wait forever.\n"); 
         /*永久阻塞模式申请信号量*/ 
         uwRet = LOS_SemPend(g_usSemID, LOS_WAIT_FOREVER); 
         printf("Example_SemTask1 wait_forever and get sem g_usSemID .\n"); 
         if(LOS_OK == uwRet) 
         { 
             LOS_SemPost(g_usSemID); 
             return; 
         } 
     } 
     return; 
  
 } 
  VOID   Example_SemTask2(void) 
 { 
     UINT32 uwRet; 
     printf("Example_SemTask2 try get sem g_usSemID wait forever.\n"); 
     /*永久阻塞模式申请信号量*/ 
     uwRet = LOS_SemPend(g_usSemID, LOS_WAIT_FOREVER); 
  
     if(LOS_OK == uwRet) 
     printf("Example_SemTask2 get sem g_usSemID and then delay 20ticks .\n"); 
  
     /*任务休眠20 Tick*/ 
     LOS_TaskDelay(20); 
  
     printf("Example_SemTask2 post sem g_usSemID .\n"); 
     /*释放信号量*/ 
     LOS_SemPost(g_usSemID); 
  
     return; 
  
 } 
 UINT32 Example_TaskEntry() 
 { 
     UINT32 uwRet; 
     TSK_INIT_PARAM_S stTask1; 
     TSK_INIT_PARAM_S stTask2; 
  
    /*创建信号量*/ 
     LOS_SemCreate(0,&g_usSemID); 
  
     /*锁任务调度*/ 
     LOS_TaskLock(); 
  
     /*创建任务1*/ 
     memset(&stTask1, 0, sizeof(TSK_INIT_PARAM_S)); 
     stTask1.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_SemTask1; 
     stTask1.pcName       = "MutexTsk1"; 
     stTask1.uwStackSize  = OS_TSK_DEFAULT_STACK_SIZE; 
     stTask1.usTaskPrio   = TASK_PRIO_TEST; 
     uwRet = LOS_TaskCreate(&g_TestTaskID01, &stTask1); 
     if(uwRet != LOS_OK) 
     { 
         printf("task1 create failed .\n"); 
         return LOS_NOK; 
     } 
  
     /*创建任务2*/ 
     memset(&stTask2, 0, sizeof(TSK_INIT_PARAM_S)); 
     stTask2.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_SemTask2; 
     stTask2.pcName       = "MutexTsk2"; 
     stTask2.uwStackSize  = OS_TSK_DEFAULT_STACK_SIZE; 
     stTask2.usTaskPrio   = (TASK_PRIO_TEST - 1); 
     uwRet = LOS_TaskCreate(&g_TestTaskID02, &stTask2); 
     if(uwRet != LOS_OK) 
     { 
         printf("task2 create failed .\n"); 
         return LOS_NOK; 
     } 
  
     /*解锁任务调度*/ 
     LOS_TaskUnlock(); 
  
     uwRet = LOS_SemPost(g_usSemID); 
  
     /*任务休眠40 Tick*/ 
     LOS_TaskDelay(40); 
  
     /*删除信号量*/ 
     LOS_SemDelete(g_usSemID); 
  
     /*删除任务1*/ 
     uwRet = LOS_TaskDelete(g_TestTaskID01); 
     if(uwRet != LOS_OK) 
     { 
         printf("task1 delete failed .\n"); 
         return LOS_NOK; 
     } 
     /*删除任务2*/ 
     uwRet = LOS_TaskDelete(g_TestTaskID02); 
     if(uwRet != LOS_OK) 
     { 
         printf("task2 delete failed .\n"); 
         return LOS_NOK; 
     } 
  
     return LOS_OK; 
 }
```


#### 结果验证

编译运行得到的结果为：

```
Example_SemTask2 try get sem g_usSemID wait forever.
Example_SemTask1 try get sem g_usSemID ,timeout 10 ticks.
Example_SemTask2 get sem g_usSemID and then delay 20ticks .
Example_SemTask1 timeout and try get sem g_usSemID wait forever.
Example_SemTask2 post sem g_usSemID .
Example_SemTask1 wait_forever and get sem g_usSemID .
```

#### 完整实例代码

[sample\_sem.c](resource/sample_sem.c)

<h3 id="12">4.8 时间管理</h3>

### 概述

#### 基本概念

时间管理以系统时钟为基础。时间管理提供给应用程序所有和时间有关的服务。  

系统时钟是由定时/计数器产生的输出脉冲触发中断而产生的，一般定义为整数或长整数。输出脉冲的周期叫做一个“时钟滴答”。系统时钟也称为时标或者Tick。一个Tick的时长可以静态配置。

用户是以秒、毫秒为单位计时，而操作系统时钟芯片CPU的计时是以Tick为单位的，当用户需要对系统操作时，例如任务挂起、延时等，输入秒为单位的数值，此时需要时间管理模块对二者进行转换。

Tick与秒之间的对应关系可以配置。

Huawei LiteOS的时间管理模块提供时间转换、统计、延迟功能以满足用户对时间相关需求的实现。

#### 相关概念

-   Cycle

系统最小的计时单位。Cycle的时长由系统主频决定，系统主频就是每秒钟的Cycle数。

-   Tick

Tick是操作系统的基本时间单位，对应的时长由系统主频及每秒Tick数决定，由用户配置。

### 开发指导

#### 使用场景

用户需要了解当前系统运行的时间以及Tick与秒、毫秒之间的转换关系等。

#### 功能

Huawei LiteOS系统中的时间管理主要提供以下两种功能：

-   时间转换：根据主频实现CPU Tick数到毫秒、微秒的转换。
-   时间统计：获取系统Tick数。

<a name="table1597633114853"></a>
<table><thead align="left"><tr id="row6547625614853"><th class="cellrowborder" valign="top" width="14.91%" id="mcps1.1.4.1.1"><p id="p197652314853"><a name="p197652314853"></a><a name="p197652314853"></a>功能分类</p>
</th>
<th class="cellrowborder" valign="top" width="34.089999999999996%" id="mcps1.1.4.1.2"><p id="p3159926114853"><a name="p3159926114853"></a><a name="p3159926114853"></a>接口名</p>
</th>
<th class="cellrowborder" valign="top" width="51%" id="mcps1.1.4.1.3"><p id="p1752099814853"><a name="p1752099814853"></a><a name="p1752099814853"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row37761567115936"><td class="cellrowborder" rowspan="2" valign="top" width="14.91%" headers="mcps1.1.4.1.1 "><p id="p66321365112810"><a name="p66321365112810"></a><a name="p66321365112810"></a>时间转换</p>
</td>
<td class="cellrowborder" valign="top" width="34.089999999999996%" headers="mcps1.1.4.1.2 "><p id="p38788111115936"><a name="p38788111115936"></a><a name="p38788111115936"></a>LOS_MS2Tick</p>
</td>
<td class="cellrowborder" valign="top" width="51%" headers="mcps1.1.4.1.3 "><p id="p54829304115936"><a name="p54829304115936"></a><a name="p54829304115936"></a>毫秒转换成Tick。</p>
</td>
</tr>
<tr id="row2021456214853"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2676682114853"><a name="p2676682114853"></a><a name="p2676682114853"></a>LOS_Tick2MS</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="p2062890114853"><a name="p2062890114853"></a><a name="p2062890114853"></a>Tick转化为毫秒。</p>
</td>
</tr>
<tr id="row4646930012230"><td class="cellrowborder" rowspan="2" valign="top" width="14.91%" headers="mcps1.1.4.1.1 "><p id="p54265225145715"><a name="p54265225145715"></a><a name="p54265225145715"></a>时间统计</p>
</td>
<td class="cellrowborder" valign="top" width="34.089999999999996%" headers="mcps1.1.4.1.2 "><p id="p951174812230"><a name="p951174812230"></a><a name="p951174812230"></a>LOS_CyclePerTickGet</p>
</td>
<td class="cellrowborder" valign="top" width="51%" headers="mcps1.1.4.1.3 "><p id="p3225415312230"><a name="p3225415312230"></a><a name="p3225415312230"></a>每个Tick多少Cycle数。</p>
</td>
</tr>
<tr id="row5082319714571"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p33407133145715"><a name="p33407133145715"></a><a name="p33407133145715"></a>LOS_TickCountGet</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="p21623253145715"><a name="p21623253145715"></a><a name="p21623253145715"></a>获取当前的Tick数。</p>
</td>
</tr>
</tbody>
</table>

## 时间管理错误码<a name="section8414152117171"></a>

时间转换存在出错的可能性，需要返回对应的错误码，以便快速定位错误原因。

<a name="table12415102171712"></a>
<table><thead align="left"><tr id="row1653352181710"><th class="cellrowborder" valign="top" width="7.220722072207221%" id="mcps1.1.6.1.1"><p id="p12533152141718"><a name="p12533152141718"></a><a name="p12533152141718"></a>序号</p>
</th>
<th class="cellrowborder" valign="top" width="26.8026802680268%" id="mcps1.1.6.1.2"><p id="p953317219179"><a name="p953317219179"></a><a name="p953317219179"></a>定义</p>
</th>
<th class="cellrowborder" valign="top" width="15.46154615461546%" id="mcps1.1.6.1.3"><p id="p11533132191715"><a name="p11533132191715"></a><a name="p11533132191715"></a>实际数值</p>
</th>
<th class="cellrowborder" valign="top" width="24.74247424742474%" id="mcps1.1.6.1.4"><p id="p10533122151710"><a name="p10533122151710"></a><a name="p10533122151710"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="25.77257725772577%" id="mcps1.1.6.1.5"><p id="p165330214173"><a name="p165330214173"></a><a name="p165330214173"></a>参考解决方案</p>
</th>
</tr>
</thead>
<tbody><tr id="row10533182115179"><td class="cellrowborder" valign="top" width="7.220722072207221%" headers="mcps1.1.6.1.1 "><p id="p3533142141719"><a name="p3533142141719"></a><a name="p3533142141719"></a>1</p>
</td>
<td class="cellrowborder" valign="top" width="26.8026802680268%" headers="mcps1.1.6.1.2 "><p id="p1653314215177"><a name="p1653314215177"></a><a name="p1653314215177"></a>LOS_ERRNO_SYS_PTR_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="15.46154615461546%" headers="mcps1.1.6.1.3 "><p id="p15331121131716"><a name="p15331121131716"></a><a name="p15331121131716"></a>0x02000010</p>
</td>
<td class="cellrowborder" valign="top" width="24.74247424742474%" headers="mcps1.1.6.1.4 "><p id="p4533721181716"><a name="p4533721181716"></a><a name="p4533721181716"></a>入参指针为空</p>
</td>
<td class="cellrowborder" valign="top" width="25.77257725772577%" headers="mcps1.1.6.1.5 "><p id="p6533192114175"><a name="p6533192114175"></a><a name="p6533192114175"></a>检查入参，传入非空入参</p>
</td>
</tr>
<tr id="row1553332117178"><td class="cellrowborder" valign="top" width="7.220722072207221%" headers="mcps1.1.6.1.1 "><p id="p13533321121714"><a name="p13533321121714"></a><a name="p13533321121714"></a>2</p>
</td>
<td class="cellrowborder" valign="top" width="26.8026802680268%" headers="mcps1.1.6.1.2 "><p id="p5533192113176"><a name="p5533192113176"></a><a name="p5533192113176"></a>LOS_ERRNO_SYS_CLOCK_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="15.46154615461546%" headers="mcps1.1.6.1.3 "><p id="p353392111178"><a name="p353392111178"></a><a name="p353392111178"></a>0x02000011</p>
</td>
<td class="cellrowborder" valign="top" width="24.74247424742474%" headers="mcps1.1.6.1.4 "><p id="p95335210179"><a name="p95335210179"></a><a name="p95335210179"></a>无效的系统时钟配置</p>
</td>
<td class="cellrowborder" valign="top" width="25.77257725772577%" headers="mcps1.1.6.1.5 "><p id="p65339211176"><a name="p65339211176"></a><a name="p65339211176"></a>在los_config.h配置有效的时钟</p>
</td>
</tr>
<tr id="row13533521111719"><td class="cellrowborder" valign="top" width="7.220722072207221%" headers="mcps1.1.6.1.1 "><p id="p0533321131714"><a name="p0533321131714"></a><a name="p0533321131714"></a>3</p>
</td>
<td class="cellrowborder" valign="top" width="26.8026802680268%" headers="mcps1.1.6.1.2 "><p id="p145331021111710"><a name="p145331021111710"></a><a name="p145331021111710"></a>LOS_ERRNO_SYS_MAXNUMOFCORES_IS_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="15.46154615461546%" headers="mcps1.1.6.1.3 "><p id="p353322114173"><a name="p353322114173"></a><a name="p353322114173"></a>0x02000012</p>
</td>
<td class="cellrowborder" valign="top" width="24.74247424742474%" headers="mcps1.1.6.1.4 "><p id="p8533621111719"><a name="p8533621111719"></a><a name="p8533621111719"></a>错误码暂时没有使用</p>
</td>
<td class="cellrowborder" valign="top" width="25.77257725772577%" headers="mcps1.1.6.1.5 "><p id="p10533102112178"><a name="p10533102112178"></a><a name="p10533102112178"></a>错误码暂时没有使用</p>
</td>
</tr>
<tr id="row15533421121718"><td class="cellrowborder" valign="top" width="7.220722072207221%" headers="mcps1.1.6.1.1 "><p id="p13533721101714"><a name="p13533721101714"></a><a name="p13533721101714"></a>4</p>
</td>
<td class="cellrowborder" valign="top" width="26.8026802680268%" headers="mcps1.1.6.1.2 "><p id="p453392181710"><a name="p453392181710"></a><a name="p453392181710"></a>LOS_ERRNO_SYS_PERIERRCOREID_IS_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="15.46154615461546%" headers="mcps1.1.6.1.3 "><p id="p14533142114178"><a name="p14533142114178"></a><a name="p14533142114178"></a>0x02000013</p>
</td>
<td class="cellrowborder" valign="top" width="24.74247424742474%" headers="mcps1.1.6.1.4 "><p id="p13533202111720"><a name="p13533202111720"></a><a name="p13533202111720"></a>错误码暂时没有使用</p>
</td>
<td class="cellrowborder" valign="top" width="25.77257725772577%" headers="mcps1.1.6.1.5 "><p id="p65331821131714"><a name="p65331821131714"></a><a name="p65331821131714"></a>错误码暂时没有使用</p>
</td>
</tr>
<tr id="row115331921181716"><td class="cellrowborder" valign="top" width="7.220722072207221%" headers="mcps1.1.6.1.1 "><p id="p953362111170"><a name="p953362111170"></a><a name="p953362111170"></a>5</p>
</td>
<td class="cellrowborder" valign="top" width="26.8026802680268%" headers="mcps1.1.6.1.2 "><p id="p1053342118178"><a name="p1053342118178"></a><a name="p1053342118178"></a>LOS_ERRNO_SYS_HOOK_IS_FULL</p>
</td>
<td class="cellrowborder" valign="top" width="15.46154615461546%" headers="mcps1.1.6.1.3 "><p id="p853342181716"><a name="p853342181716"></a><a name="p853342181716"></a>0x02000014</p>
</td>
<td class="cellrowborder" valign="top" width="24.74247424742474%" headers="mcps1.1.6.1.4 "><p id="p195331221131715"><a name="p195331221131715"></a><a name="p195331221131715"></a>错误码暂时没有使用</p>
</td>
<td class="cellrowborder" valign="top" width="25.77257725772577%" headers="mcps1.1.6.1.5 "><p id="p1153372181715"><a name="p1153372181715"></a><a name="p1153372181715"></a>错误码暂时没有使用</p>
</td>
</tr>
</tbody>
</table>

#### 开发流程

时间管理的典型开发流程：

1.  确认配置项LOSCFG\_BASE\_CORE\_TICK\_HW\_TIME为YES开启状态。
    -   在los\_config.h中配置每秒的Tick数LOSCFG\_BASE\_CORE\_TICK\_PER\_SECOND。

2.  调用时钟转换接口。
3.  获取系统Tick数完成时间统计。
    -   通过LOS\_TickCountGet获取系统当前Tick值。


### 注意事项

-   获取系统Tick数需要在系统时钟使能之后。
-   时间管理不是单独的功能模块，依赖于los\_config.h中的OS\_SYS\_CLOCK和LOSCFG\_BASE\_CORE\_TICK\_PER\_SECOND两个配置选项。
-   系统的Tick数在关中断的情况下不进行计数，故系统Tick数不能作为准确时间计算。

### 编程实例

#### 实例描述

在下面的例子中，介绍了时间管理的基本方法，包括：

1.  时间转换：将毫秒数转换为Tick数，或将Tick数转换为毫秒数。
2.  时间统计和时间延迟：统计每秒的Cycle数、Tick数和延迟后的Tick数。

    >![](public_sys-resources/icon-note.gif) **说明：**   
    >示例中系统时钟频率为80MHZ。  


#### 编程示例

前提条件：

-   配好LOSCFG\_BASE\_CORE\_TICK\_PER\_SECOND每秒的Tick数。
-   配好OS\_SYS\_CLOCK 系统时钟，单位: Hz。

时间转换：

```
VOID Example_TransformTime(VOID)
{  
    UINT32 uwMs;
    UINT32 uwTick;

    uwTick = LOS_MS2Tick(10000);//10000 ms数转换为Tick数
    printf("uwTick = %d \n",uwTick);
    uwMs= LOS_Tick2MS(100);//100 Tick数转换为ms数
    printf("uwMs = %d \n",uwMs);
}
```

时间统计和时间延迟：

```
 VOID Example_GetTime(VOID) 
 { 
     UINT32 uwcyclePerTick; 
     UINT64 uwTickCount; 
  
     uwcyclePerTick = LOS_CyclePerTickGet();//每个Tick多少Cycle数
     if(0 != uwcyclePerTick) 
     { 
         dprintf("LOS_CyclePerTickGet = %d \n", uwcyclePerTick); 
     } 
  
     uwTickCount = LOS_TickCountGet();//获取Tick数
     if(0 != uwTickCount) 
     { 
         dprintf("LOS_TickCountGet = %d \n", （UINT32）uwTickCount); 
     } 
     LOS_TaskDelay(200);//延迟200 Tick 
     uwTickCount = LOS_TickCountGet(); 
     if(0 != uwTickCount) 
     { 
         dprintf("LOS_TickCountGet after delay = %d \n", (UINT32)uwTickCount); 
     } 
 }
```

#### 结果验证

编译运行得到的结果为：

时间转换：

```
uwTick = 1000 
uwMs = 1000
```

时间统计和时间延迟：

```
LOS_CyclePerTickGet = 495000  
LOS_TickCountGet = 1  
LOS_TickCountGet after delay = 201
```

#### 完整实例代码

[sample\_time.c](resource/sample_time.c)

<h3 id="13">4.9 软件定时器</h3>

### 概述

#### 基本概念

软件定时器，是基于系统Tick时钟中断且由软件来模拟的定时器，当经过设定的Tick时钟计数值后会触发用户定义的回调函数。定时精度与系统Tick时钟的周期有关。  

硬件定时器受硬件的限制，数量上不足以满足用户的实际需求，因此为了满足用户需求，提供更多的定时器，Huawei LiteOS操作系统提供软件定时器功能。

软件定时器扩展了定时器的数量，允许创建更多的定时业务。

软件定时器功能上支持：

-   静态裁剪：能通过宏关闭软件定时器功能。
-   软件定时器创建。
-   软件定时器启动。
-   软件定时器停止。
-   软件定时器删除。
-   软件定时器剩余Tick数获取。

#### 运作机制

软件定时器是系统资源，在模块初始化的时候已经分配了一块连续的内存，系统支持的最大定时器个数由los\_config.h中的LOSCFG\_BASE\_CORE\_SWTMR\_LIMIT宏配置。

软件定时器使用了系统的一个队列和一个任务资源，软件定时器的触发遵循队列规则，先进先出。定时时间短的定时器总是比定时时间长的靠近队列头，满足优先被触发的准则。

软件定时器以Tick为基本计时单位，当用户创建并启动一个软件定时器时，Huawei LiteOS会根据当前系统Tick时间及用户设置的定时间隔确定该定时器的到期Tick时间，并将该定时器控制结构挂入计时全局链表。

当Tick中断到来时，在Tick中断处理函数中扫描软件定时器的计时全局链表，看是否有定时器超时，若有则将超时的定时器记录下来。

Tick中断处理函数结束后，软件定时器任务（优先级为最高）被唤醒，在该任务中调用之前记录下来的定时器的超时回调函数。

**定时器状态**

-   OS\_SWTMR\_STATUS\_UNUSED（未使用）

系统在定时器模块初始化的时候将系统中所有定时器资源初始化成该状态。

-   OS\_SWTMR\_STATUS\_CREATED（创建未启动/停止）

在未使用状态下调用LOS\_SwtmrCreate接口或者启动后调用LOS\_SwtmrStop接口后，定时器将变成该状态。

-   OS\_SWTMR\_STATUS\_TICKING（计数）

在定时器创建后调用LOS\_SwtmrStart接口，定时器将变成该状态，表示定时器运行时的状态。

**定时器模式**

Huawei LiteOS的软件定时器提供二类定时器机制：

-   第一类是单次触发定时器，这类定时器在启动后只会触发一次定时器事件，然后定时器自动删除。
-   第二类是周期触发定时器，这类定时器会周期性的触发定时器事件，直到用户手动地停止定时器，否则将永远持续执行下去。
-   第三类也是单次触发定时器，但与第一类不同之处在于这类定时器超时后不会自动删除，需要调用定时器删除接口删除定时器。

### 开发指导

#### 使用场景


-   创建一个单次触发的定时器，超时后执行用户自定义的回调函数。
-   创建一个周期性触发的定时器，超时后执行用户自定义的回调函数。

#### 功能

Huawei LiteOS系统中的软件定时器模块为用户提供下面几种功能，下面具体的API详见软件定时器对外接口手册。

**表 1** 

<a name="t9a496132ebf541f4a9a57c77a047acb0"></a>
<table><thead align="left"><tr id="r730baae1ca4d48da808987e4793bd5d8"><th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.1"><p id="a477b08d35e4d4d54af77e97193890ccc"><a name="a477b08d35e4d4d54af77e97193890ccc"></a><a name="a477b08d35e4d4d54af77e97193890ccc"></a>功能分类</p>
</th>
<th class="cellrowborder" valign="top" width="38.033803380338036%" id="mcps1.2.4.1.2"><p id="ae06e86ac260f4414a3e575171f6e044a"><a name="ae06e86ac260f4414a3e575171f6e044a"></a><a name="ae06e86ac260f4414a3e575171f6e044a"></a>接口名</p>
</th>
<th class="cellrowborder" valign="top" width="28.63286328632863%" id="mcps1.2.4.1.3"><p id="a09ee74dd4fb14585a5cbecd27ef979f2"><a name="a09ee74dd4fb14585a5cbecd27ef979f2"></a><a name="a09ee74dd4fb14585a5cbecd27ef979f2"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="r3c0b764752f24fd39902605e9c027215"><td class="cellrowborder" rowspan="2" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="a41decb899b66404db02d0fdd1584ae8d"><a name="a41decb899b66404db02d0fdd1584ae8d"></a><a name="a41decb899b66404db02d0fdd1584ae8d"></a>创建、删除定时器</p>
</td>
<td class="cellrowborder" valign="top" width="38.033803380338036%" headers="mcps1.2.4.1.2 "><p id="afe8019ad8ef74b21bca724da44dea7b5"><a name="afe8019ad8ef74b21bca724da44dea7b5"></a><a name="afe8019ad8ef74b21bca724da44dea7b5"></a>LOS_SwtmrCreate</p>
</td>
<td class="cellrowborder" valign="top" width="28.63286328632863%" headers="mcps1.2.4.1.3 "><p id="a8ae9a95a713b462aaf93e492ba5f9bc5"><a name="a8ae9a95a713b462aaf93e492ba5f9bc5"></a><a name="a8ae9a95a713b462aaf93e492ba5f9bc5"></a>创建定时器</p>
</td>
</tr>
<tr id="r2e0f709a552f41e3aa1388ec48387252"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="af91103fcb0ee4ae0b903844a61c9a049"><a name="af91103fcb0ee4ae0b903844a61c9a049"></a><a name="af91103fcb0ee4ae0b903844a61c9a049"></a>LOS_SwtmrDelete</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="a55d4e4c6dfc24af9a2cc0430c961f9da"><a name="a55d4e4c6dfc24af9a2cc0430c961f9da"></a><a name="a55d4e4c6dfc24af9a2cc0430c961f9da"></a>删除定时器</p>
</td>
</tr>
<tr id="rb81a470d69b94126853b7d29f0e0356a"><td class="cellrowborder" rowspan="2" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="ace0ba174cf224d799507140d4dcbdbd5"><a name="ace0ba174cf224d799507140d4dcbdbd5"></a><a name="ace0ba174cf224d799507140d4dcbdbd5"></a>启动、停止定时器</p>
</td>
<td class="cellrowborder" valign="top" width="38.033803380338036%" headers="mcps1.2.4.1.2 "><p id="ab2860d7de9454eb7856c55cd0aca0a19"><a name="ab2860d7de9454eb7856c55cd0aca0a19"></a><a name="ab2860d7de9454eb7856c55cd0aca0a19"></a>LOS_SwtmrStart</p>
</td>
<td class="cellrowborder" valign="top" width="28.63286328632863%" headers="mcps1.2.4.1.3 "><p id="a0ac6a47eb9b946caa1439d6423e9e9c1"><a name="a0ac6a47eb9b946caa1439d6423e9e9c1"></a><a name="a0ac6a47eb9b946caa1439d6423e9e9c1"></a>启动定时器</p>
</td>
</tr>
<tr id="rf313f8d7013a4a9b9b009de45f06f5ee"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="a47781a078366413a90093cdae6b46a00"><a name="a47781a078366413a90093cdae6b46a00"></a><a name="a47781a078366413a90093cdae6b46a00"></a>LOS_SwtmrStop</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="a9ab21ed778b94db3abc7e06a88cbf177"><a name="a9ab21ed778b94db3abc7e06a88cbf177"></a><a name="a9ab21ed778b94db3abc7e06a88cbf177"></a>停止定时器</p>
</td>
</tr>
<tr id="rcb21dea3491b479490b5e58ae4ae3f78"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="ac91b7d9fe84940eaaf7d0a2c2fe4cea0"><a name="ac91b7d9fe84940eaaf7d0a2c2fe4cea0"></a><a name="ac91b7d9fe84940eaaf7d0a2c2fe4cea0"></a>获得软件定时器剩余Tick数</p>
</td>
<td class="cellrowborder" valign="top" width="38.033803380338036%" headers="mcps1.2.4.1.2 "><p id="adf883540d9104c2e8994390deaa16026"><a name="adf883540d9104c2e8994390deaa16026"></a><a name="adf883540d9104c2e8994390deaa16026"></a>LOS_SwtmrTimeGet</p>
</td>
<td class="cellrowborder" valign="top" width="28.63286328632863%" headers="mcps1.2.4.1.3 "><p id="a756e06dcb51040849843e68218a84a6a"><a name="a756e06dcb51040849843e68218a84a6a"></a><a name="a756e06dcb51040849843e68218a84a6a"></a>获得软件定时器剩余Tick数</p>
</td>
</tr>
</tbody>
</table>

#### 开发流程

软件定时器的典型开发流程：

1.  配置软件定时器。
    -   确认配置项LOSCFG\_BASE\_CORE\_SWTMR和LOSCFG\_BASE\_IPC\_QUEUE为YES打开状态。
    -   配置LOSCFG\_BASE\_CORE\_SWTMR\_LIMIT最大支持的软件定时器数。
    -   配置OS\_SWTMR\_HANDLE\_QUEUE\_SIZE软件定时器队列最大长度。

2.  创建定时器LOS\_SwtmrCreate。
    -   创建一个指定计时时长、指定超时处理函数、指定触发模式的软件定时器。
    -   返回函数运行结果，成功或失败。

3.  启动定时器LOS\_SwtmrStart。
4.  获得软件定时器剩余Tick数LOS\_SwtmrTimeGet。
5.  停止定时器LOS\_SwtmrStop。
6.  删除定时器LOS\_SwtmrDelete。

#### 软件定时器错误码

对软件定时器存在失败可能性的操作，包括创建、删除、暂停、重启定时器等等，均需要返回对应的错误码，以便快速定位错误原因。

<a name="t930cad663aff44da97e95af7b3265cbc"></a>
<table><thead align="left"><tr id="r0409738913214771a44588efed28ff3c"><th class="cellrowborder" valign="top" width="7.5200000000000005%" id="mcps1.1.6.1.1"><p id="af205e9260f2d46c186fd85c9778fb930"><a name="af205e9260f2d46c186fd85c9778fb930"></a><a name="af205e9260f2d46c186fd85c9778fb930"></a>序号</p>
</th>
<th class="cellrowborder" valign="top" width="26.75%" id="mcps1.1.6.1.2"><p id="zh-cn_topic_0015795101_p261046995642"><a name="zh-cn_topic_0015795101_p261046995642"></a><a name="zh-cn_topic_0015795101_p261046995642"></a>定义</p>
</th>
<th class="cellrowborder" valign="top" width="15.85%" id="mcps1.1.6.1.3"><p id="ae2c5c316ed014c6ab632df1477bb6208"><a name="ae2c5c316ed014c6ab632df1477bb6208"></a><a name="ae2c5c316ed014c6ab632df1477bb6208"></a>实际数值</p>
</th>
<th class="cellrowborder" valign="top" width="24.13%" id="mcps1.1.6.1.4"><p id="ab3d9d844189e41e3b16d6fb5c81e0dc4"><a name="ab3d9d844189e41e3b16d6fb5c81e0dc4"></a><a name="ab3d9d844189e41e3b16d6fb5c81e0dc4"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="25.75%" id="mcps1.1.6.1.5"><p id="a694459f0426942b5a0fefe891b1dc268"><a name="a694459f0426942b5a0fefe891b1dc268"></a><a name="a694459f0426942b5a0fefe891b1dc268"></a>参考解决方案</p>
</th>
</tr>
</thead>
<tbody><tr id="ra7637f33f6cd4d1fbbddfd71a344e269"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="a00876a64937f4e60a0f45db123969405"><a name="a00876a64937f4e60a0f45db123969405"></a><a name="a00876a64937f4e60a0f45db123969405"></a>1</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.6.1.2 "><p id="a978ddfac305d46c2b09ffd8dc7658fd5"><a name="a978ddfac305d46c2b09ffd8dc7658fd5"></a><a name="a978ddfac305d46c2b09ffd8dc7658fd5"></a>LOS_ERRNO_SWTMR_PTR_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="15.85%" headers="mcps1.1.6.1.3 "><p id="a33e32cf9a20b42f59ea96dc87e89256a"><a name="a33e32cf9a20b42f59ea96dc87e89256a"></a><a name="a33e32cf9a20b42f59ea96dc87e89256a"></a>0x02000300</p>
</td>
<td class="cellrowborder" valign="top" width="24.13%" headers="mcps1.1.6.1.4 "><p id="zh-cn_topic_0015795101_p492720095642"><a name="zh-cn_topic_0015795101_p492720095642"></a><a name="zh-cn_topic_0015795101_p492720095642"></a>软件定时器回调函数为空。</p>
</td>
<td class="cellrowborder" valign="top" width="25.75%" headers="mcps1.1.6.1.5 "><p id="ae9a31057a9684325b14d08d271cb58d9"><a name="ae9a31057a9684325b14d08d271cb58d9"></a><a name="ae9a31057a9684325b14d08d271cb58d9"></a>定义软件定时器回调函数。</p>
</td>
</tr>
<tr id="r27c5821490d54490a069498d083f7628"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="a2439ffb5187641e982b44aa617188f2a"><a name="a2439ffb5187641e982b44aa617188f2a"></a><a name="a2439ffb5187641e982b44aa617188f2a"></a>2</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.6.1.2 "><p id="a7b7eaf06aa874c81937bb227958e1e1b"><a name="a7b7eaf06aa874c81937bb227958e1e1b"></a><a name="a7b7eaf06aa874c81937bb227958e1e1b"></a>LOS_ERRNO_SWTMR_INTERVAL_NOT_SUITED</p>
</td>
<td class="cellrowborder" valign="top" width="15.85%" headers="mcps1.1.6.1.3 "><p id="a65e2e67aab424d07b7a648cc3decce2f"><a name="a65e2e67aab424d07b7a648cc3decce2f"></a><a name="a65e2e67aab424d07b7a648cc3decce2f"></a>0x02000301</p>
</td>
<td class="cellrowborder" valign="top" width="24.13%" headers="mcps1.1.6.1.4 "><p id="a5d07c6cf9c494fa4a5300b8a9e5a808b"><a name="a5d07c6cf9c494fa4a5300b8a9e5a808b"></a><a name="a5d07c6cf9c494fa4a5300b8a9e5a808b"></a>软件定时器间隔时间为0。</p>
</td>
<td class="cellrowborder" valign="top" width="25.75%" headers="mcps1.1.6.1.5 "><p id="zh-cn_topic_0015795101_p412423410026"><a name="zh-cn_topic_0015795101_p412423410026"></a><a name="zh-cn_topic_0015795101_p412423410026"></a>重新定义间隔时间。</p>
</td>
</tr>
<tr id="re2c19af100434144934cf26116902b20"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="a5e5ba596ecdf46548b6b070a40b266af"><a name="a5e5ba596ecdf46548b6b070a40b266af"></a><a name="a5e5ba596ecdf46548b6b070a40b266af"></a>3</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.6.1.2 "><p id="a7e00a902db3946deaf31f1b17e830be2"><a name="a7e00a902db3946deaf31f1b17e830be2"></a><a name="a7e00a902db3946deaf31f1b17e830be2"></a>LOS_ERRNO_SWTMR_MODE_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="15.85%" headers="mcps1.1.6.1.3 "><p id="ae595d8ffcae748aabd3ac75a7b8632b5"><a name="ae595d8ffcae748aabd3ac75a7b8632b5"></a><a name="ae595d8ffcae748aabd3ac75a7b8632b5"></a>0x02000302</p>
</td>
<td class="cellrowborder" valign="top" width="24.13%" headers="mcps1.1.6.1.4 "><p id="a44004f73238846d89a15c60bb46b21b9"><a name="a44004f73238846d89a15c60bb46b21b9"></a><a name="a44004f73238846d89a15c60bb46b21b9"></a>不正确的软件定时器模式。</p>
</td>
<td class="cellrowborder" valign="top" width="25.75%" headers="mcps1.1.6.1.5 "><p id="af2cf597a839f479cb6782a0fe73d7504"><a name="af2cf597a839f479cb6782a0fe73d7504"></a><a name="af2cf597a839f479cb6782a0fe73d7504"></a>确认软件定时器模式，范围为[0,2]。</p>
</td>
</tr>
<tr id="r9655ef852c7a4580a2b1d32b21e14b5a"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="aaf9ab41e99e94e5bb28602e7c7e37e4a"><a name="aaf9ab41e99e94e5bb28602e7c7e37e4a"></a><a name="aaf9ab41e99e94e5bb28602e7c7e37e4a"></a>4</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.6.1.2 "><p id="a2460e15fa9e04ee69fd457ed753e6a6a"><a name="a2460e15fa9e04ee69fd457ed753e6a6a"></a><a name="a2460e15fa9e04ee69fd457ed753e6a6a"></a>LOS_ERRNO_SWTMR_RET_PTR_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="15.85%" headers="mcps1.1.6.1.3 "><p id="a404ecb9b638444edbda65eea5b745bf8"><a name="a404ecb9b638444edbda65eea5b745bf8"></a><a name="a404ecb9b638444edbda65eea5b745bf8"></a>0x02000303</p>
</td>
<td class="cellrowborder" valign="top" width="24.13%" headers="mcps1.1.6.1.4 "><p id="zh-cn_topic_0015795101_p915662595642"><a name="zh-cn_topic_0015795101_p915662595642"></a><a name="zh-cn_topic_0015795101_p915662595642"></a>软件定时器ID指针入参为NULL。</p>
</td>
<td class="cellrowborder" valign="top" width="25.75%" headers="mcps1.1.6.1.5 "><p id="ad841a944c2324eb4b9cb1a490969781f"><a name="ad841a944c2324eb4b9cb1a490969781f"></a><a name="ad841a944c2324eb4b9cb1a490969781f"></a>定义ID变量，传入指针。</p>
</td>
</tr>
<tr id="r069a3081a7ec49ca90ed70740b2a7446"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="a11c4e741db4d477795fb61a8fc42569f"><a name="a11c4e741db4d477795fb61a8fc42569f"></a><a name="a11c4e741db4d477795fb61a8fc42569f"></a>5</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.6.1.2 "><p id="a5ef8528a79274de5bc2a5cb93f74724e"><a name="a5ef8528a79274de5bc2a5cb93f74724e"></a><a name="a5ef8528a79274de5bc2a5cb93f74724e"></a>LOS_ERRNO_SWTMR_MAXSIZE</p>
</td>
<td class="cellrowborder" valign="top" width="15.85%" headers="mcps1.1.6.1.3 "><p id="zh-cn_topic_0015795101_p804161495642"><a name="zh-cn_topic_0015795101_p804161495642"></a><a name="zh-cn_topic_0015795101_p804161495642"></a>0x02000304</p>
</td>
<td class="cellrowborder" valign="top" width="24.13%" headers="mcps1.1.6.1.4 "><p id="abf904a87dfb84c7cb3f4236f9a56565f"><a name="abf904a87dfb84c7cb3f4236f9a56565f"></a><a name="abf904a87dfb84c7cb3f4236f9a56565f"></a>软件定时器个数超过最大值。</p>
</td>
<td class="cellrowborder" valign="top" width="25.75%" headers="mcps1.1.6.1.5 "><p id="ad5e18e503acb48c0b07b232bb10af028"><a name="ad5e18e503acb48c0b07b232bb10af028"></a><a name="ad5e18e503acb48c0b07b232bb10af028"></a>重新定义软件定时器最大个数，或者等待一个软件定时器释放资源。</p>
</td>
</tr>
<tr id="re0be8bbe110e44ceab2c7841b9564ad7"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="a275e6e1ddc1442b2a66643cfe915bb98"><a name="a275e6e1ddc1442b2a66643cfe915bb98"></a><a name="a275e6e1ddc1442b2a66643cfe915bb98"></a>6</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.6.1.2 "><p id="a7aca231877e9438c8e635c43e4211933"><a name="a7aca231877e9438c8e635c43e4211933"></a><a name="a7aca231877e9438c8e635c43e4211933"></a>LOS_ERRNO_SWTMR_ID_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="15.85%" headers="mcps1.1.6.1.3 "><p id="a764ace6c102847289a025bfa429063bf"><a name="a764ace6c102847289a025bfa429063bf"></a><a name="a764ace6c102847289a025bfa429063bf"></a>0x02000305</p>
</td>
<td class="cellrowborder" valign="top" width="24.13%" headers="mcps1.1.6.1.4 "><p id="aa049d1b9abdc4ca788c1cd2cfeeda713"><a name="aa049d1b9abdc4ca788c1cd2cfeeda713"></a><a name="aa049d1b9abdc4ca788c1cd2cfeeda713"></a>不正确的软件定时器ID入参。</p>
</td>
<td class="cellrowborder" valign="top" width="25.75%" headers="mcps1.1.6.1.5 "><p id="ab745193cb55d492398dafa59f9db3303"><a name="ab745193cb55d492398dafa59f9db3303"></a><a name="ab745193cb55d492398dafa59f9db3303"></a>确保入参合法。</p>
</td>
</tr>
<tr id="rce2ace2c93a34504b9bad65f1446c8ba"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="a8b2084e788f34f669e2f273f2f209fd8"><a name="a8b2084e788f34f669e2f273f2f209fd8"></a><a name="a8b2084e788f34f669e2f273f2f209fd8"></a>7</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.6.1.2 "><p id="a0c729d9827274a238a17abb19ac7cddc"><a name="a0c729d9827274a238a17abb19ac7cddc"></a><a name="a0c729d9827274a238a17abb19ac7cddc"></a>LOS_ERRNO_SWTMR_NOT_CREATED</p>
</td>
<td class="cellrowborder" valign="top" width="15.85%" headers="mcps1.1.6.1.3 "><p id="a9fa61bd366ad4954b3159fe15cc81a60"><a name="a9fa61bd366ad4954b3159fe15cc81a60"></a><a name="a9fa61bd366ad4954b3159fe15cc81a60"></a>0x02000306</p>
</td>
<td class="cellrowborder" valign="top" width="24.13%" headers="mcps1.1.6.1.4 "><p id="aa634d9a37fcd43ceb30448f794407794"><a name="aa634d9a37fcd43ceb30448f794407794"></a><a name="aa634d9a37fcd43ceb30448f794407794"></a>软件定时器未创建。</p>
</td>
<td class="cellrowborder" valign="top" width="25.75%" headers="mcps1.1.6.1.5 "><p id="a3100120d2b6f436789b010e5c21d536c"><a name="a3100120d2b6f436789b010e5c21d536c"></a><a name="a3100120d2b6f436789b010e5c21d536c"></a>创建软件定时器。</p>
</td>
</tr>
<tr id="rc6d344188f964f3dbf7ab07ac62e81b3"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="af4f6fa621f434db8b72894545a766591"><a name="af4f6fa621f434db8b72894545a766591"></a><a name="af4f6fa621f434db8b72894545a766591"></a>8</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.6.1.2 "><p id="a4d44255e5f20465dae6e12bd5007a6e0"><a name="a4d44255e5f20465dae6e12bd5007a6e0"></a><a name="a4d44255e5f20465dae6e12bd5007a6e0"></a>LOS_ERRNO_SWTMR_NO_MEMORY</p>
</td>
<td class="cellrowborder" valign="top" width="15.85%" headers="mcps1.1.6.1.3 "><p id="aa673f806f0ac41578dcd0098116513c8"><a name="aa673f806f0ac41578dcd0098116513c8"></a><a name="aa673f806f0ac41578dcd0098116513c8"></a>0x02000307</p>
</td>
<td class="cellrowborder" valign="top" width="24.13%" headers="mcps1.1.6.1.4 "><p id="acb9f85d334ee4d29ae48c1a944064181"><a name="acb9f85d334ee4d29ae48c1a944064181"></a><a name="acb9f85d334ee4d29ae48c1a944064181"></a>软件定时器链表创建内存不足。</p>
</td>
<td class="cellrowborder" valign="top" width="25.75%" headers="mcps1.1.6.1.5 "><p id="a486a76184f1f4758a9f0d6bffe3c8ec3"><a name="a486a76184f1f4758a9f0d6bffe3c8ec3"></a><a name="a486a76184f1f4758a9f0d6bffe3c8ec3"></a>申请一块足够大的内存供软件定时器使用。</p>
</td>
</tr>
<tr id="r72b1b0fed7f7436b9678a92c40b02811"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="a57a5994cabee42278967623b2393193f"><a name="a57a5994cabee42278967623b2393193f"></a><a name="a57a5994cabee42278967623b2393193f"></a>9</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.6.1.2 "><p id="a876750b82d9448febf6087c84794f970"><a name="a876750b82d9448febf6087c84794f970"></a><a name="a876750b82d9448febf6087c84794f970"></a>LOS_ERRNO_SWTMR_MAXSIZE_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="15.85%" headers="mcps1.1.6.1.3 "><p id="a23bd8780dbb04d4d95a1eead7e666cc2"><a name="a23bd8780dbb04d4d95a1eead7e666cc2"></a><a name="a23bd8780dbb04d4d95a1eead7e666cc2"></a>0x02000308</p>
</td>
<td class="cellrowborder" valign="top" width="24.13%" headers="mcps1.1.6.1.4 "><p id="a9eda22fc5d1645ab885d24c47ea28579"><a name="a9eda22fc5d1645ab885d24c47ea28579"></a><a name="a9eda22fc5d1645ab885d24c47ea28579"></a>不正确的软件定时器个数最大值。</p>
</td>
<td class="cellrowborder" valign="top" width="25.75%" headers="mcps1.1.6.1.5 "><p id="a50da82f4e6614ee59183fb36794a0489"><a name="a50da82f4e6614ee59183fb36794a0489"></a><a name="a50da82f4e6614ee59183fb36794a0489"></a>重新定义该值。</p>
</td>
</tr>
<tr id="r1b66e918ce524165b3af7bc7eafb7724"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="a8343915b2a084ea1af10452859b8174f"><a name="a8343915b2a084ea1af10452859b8174f"></a><a name="a8343915b2a084ea1af10452859b8174f"></a>10</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.6.1.2 "><p id="ac5070acbcfa643d186a70d7eb2c43e19"><a name="ac5070acbcfa643d186a70d7eb2c43e19"></a><a name="ac5070acbcfa643d186a70d7eb2c43e19"></a>LOS_ERRNO_SWTMR_HWI_ACTIVE</p>
</td>
<td class="cellrowborder" valign="top" width="15.85%" headers="mcps1.1.6.1.3 "><p id="a92f1631c2e4743b7af9310fc69b2caa1"><a name="a92f1631c2e4743b7af9310fc69b2caa1"></a><a name="a92f1631c2e4743b7af9310fc69b2caa1"></a>0x02000309</p>
</td>
<td class="cellrowborder" valign="top" width="24.13%" headers="mcps1.1.6.1.4 "><p id="af3d171a33c6c4aa8ae0d6bb5d3e1de48"><a name="af3d171a33c6c4aa8ae0d6bb5d3e1de48"></a><a name="af3d171a33c6c4aa8ae0d6bb5d3e1de48"></a>在中断中使用定时器。</p>
</td>
<td class="cellrowborder" valign="top" width="25.75%" headers="mcps1.1.6.1.5 "><p id="a9579f753e8584379b6fe0ba4152f1bfa"><a name="a9579f753e8584379b6fe0ba4152f1bfa"></a><a name="a9579f753e8584379b6fe0ba4152f1bfa"></a>修改源代码确保不在中断中使用。</p>
</td>
</tr>
<tr id="r8fc95ae17d71439ba307efd33f6cbc9d"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="a8af71f7d2883497395009d56c035972f"><a name="a8af71f7d2883497395009d56c035972f"></a><a name="a8af71f7d2883497395009d56c035972f"></a>11</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.6.1.2 "><p id="a3b58e0ce2b024472b951dd73d749b467"><a name="a3b58e0ce2b024472b951dd73d749b467"></a><a name="a3b58e0ce2b024472b951dd73d749b467"></a>LOS_ERRNO_SWTMR_HANDLER_POOL_NO_MEM</p>
</td>
<td class="cellrowborder" valign="top" width="15.85%" headers="mcps1.1.6.1.3 "><p id="a54ac3cceb6584aaeadfc291d2a637bd4"><a name="a54ac3cceb6584aaeadfc291d2a637bd4"></a><a name="a54ac3cceb6584aaeadfc291d2a637bd4"></a>0x0200030a</p>
</td>
<td class="cellrowborder" valign="top" width="24.13%" headers="mcps1.1.6.1.4 "><p id="ab9754826f94e4b31aff64e173931c042"><a name="ab9754826f94e4b31aff64e173931c042"></a><a name="ab9754826f94e4b31aff64e173931c042"></a>membox内存不足。</p>
</td>
<td class="cellrowborder" valign="top" width="25.75%" headers="mcps1.1.6.1.5 "><p id="aa85fea1a3d3e4858b45fa0477e3965da"><a name="aa85fea1a3d3e4858b45fa0477e3965da"></a><a name="aa85fea1a3d3e4858b45fa0477e3965da"></a>扩大内存。</p>
</td>
</tr>
<tr id="r4eac8973183e4f75832d84249155a086"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="a5e8e0b23087d45bfb0fb5f178cbfac31"><a name="a5e8e0b23087d45bfb0fb5f178cbfac31"></a><a name="a5e8e0b23087d45bfb0fb5f178cbfac31"></a>12</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.6.1.2 "><p id="a33b9e83657ce491c9eab6c46e452bfc8"><a name="a33b9e83657ce491c9eab6c46e452bfc8"></a><a name="a33b9e83657ce491c9eab6c46e452bfc8"></a>LOS_ERRNO_SWTMR_QUEUE_CREATE_FAILED</p>
</td>
<td class="cellrowborder" valign="top" width="15.85%" headers="mcps1.1.6.1.3 "><p id="ada92803a0e764939984725b83b1e9449"><a name="ada92803a0e764939984725b83b1e9449"></a><a name="ada92803a0e764939984725b83b1e9449"></a>0x0200030b</p>
</td>
<td class="cellrowborder" valign="top" width="24.13%" headers="mcps1.1.6.1.4 "><p id="a9cec9082da8143df93b3331090bd84f8"><a name="a9cec9082da8143df93b3331090bd84f8"></a><a name="a9cec9082da8143df93b3331090bd84f8"></a>软件定时器队列创建失败。</p>
</td>
<td class="cellrowborder" valign="top" width="25.75%" headers="mcps1.1.6.1.5 "><p id="ac984355a5b6643a491f07f312c32cf8a"><a name="ac984355a5b6643a491f07f312c32cf8a"></a><a name="ac984355a5b6643a491f07f312c32cf8a"></a>检查用以创建队列的内存是否足够。</p>
</td>
</tr>
<tr id="r569652a7be704d76aff6c929da001df4"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="a1abca9c852c44a75a69dee6a9cf5254b"><a name="a1abca9c852c44a75a69dee6a9cf5254b"></a><a name="a1abca9c852c44a75a69dee6a9cf5254b"></a>13</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.6.1.2 "><p id="a503735074ef44c99833a17f07a18445a"><a name="a503735074ef44c99833a17f07a18445a"></a><a name="a503735074ef44c99833a17f07a18445a"></a>LOS_ERRNO_SWTMR_TASK_CREATE_FAILED</p>
</td>
<td class="cellrowborder" valign="top" width="15.85%" headers="mcps1.1.6.1.3 "><p id="ad3b53c704fac4f939fe56adf0115812e"><a name="ad3b53c704fac4f939fe56adf0115812e"></a><a name="ad3b53c704fac4f939fe56adf0115812e"></a>0x0200030c</p>
</td>
<td class="cellrowborder" valign="top" width="24.13%" headers="mcps1.1.6.1.4 "><p id="af3bc41167ec346e48fed46b0452e7b16"><a name="af3bc41167ec346e48fed46b0452e7b16"></a><a name="af3bc41167ec346e48fed46b0452e7b16"></a>软件定时器任务创建失败。</p>
</td>
<td class="cellrowborder" valign="top" width="25.75%" headers="mcps1.1.6.1.5 "><p id="acc4797e9d76e4579980995e6dd59cb7d"><a name="acc4797e9d76e4579980995e6dd59cb7d"></a><a name="acc4797e9d76e4579980995e6dd59cb7d"></a>检查用以创建软件定时器任务的内存是否足够并重新创建。</p>
</td>
</tr>
<tr id="rdd78a8ce2c88485da84fbadbb75c50b3"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="a175c172a5ec14f51a12fa67e59e8f36c"><a name="a175c172a5ec14f51a12fa67e59e8f36c"></a><a name="a175c172a5ec14f51a12fa67e59e8f36c"></a>14</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.6.1.2 "><p id="af92bac38a4f8498dbe1dc355bf20ce7b"><a name="af92bac38a4f8498dbe1dc355bf20ce7b"></a><a name="af92bac38a4f8498dbe1dc355bf20ce7b"></a>LOS_ERRNO_SWTMR_NOT_STARTED</p>
</td>
<td class="cellrowborder" valign="top" width="15.85%" headers="mcps1.1.6.1.3 "><p id="a9e6165adc0614db390c9e299de00b325"><a name="a9e6165adc0614db390c9e299de00b325"></a><a name="a9e6165adc0614db390c9e299de00b325"></a>0x0200030d</p>
</td>
<td class="cellrowborder" valign="top" width="24.13%" headers="mcps1.1.6.1.4 "><p id="aaa73f19852ff464790cbb6201d570f5b"><a name="aaa73f19852ff464790cbb6201d570f5b"></a><a name="aaa73f19852ff464790cbb6201d570f5b"></a>未启动软件定时器。</p>
</td>
<td class="cellrowborder" valign="top" width="25.75%" headers="mcps1.1.6.1.5 "><p id="adca45a2ce9654759b505071b25b7c374"><a name="adca45a2ce9654759b505071b25b7c374"></a><a name="adca45a2ce9654759b505071b25b7c374"></a>启动软件定时器。</p>
</td>
</tr>
<tr id="r89971f846c924946b80f4521ee190ec2"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="a0be412a56d9f4799bbf13b37dbfb8245"><a name="a0be412a56d9f4799bbf13b37dbfb8245"></a><a name="a0be412a56d9f4799bbf13b37dbfb8245"></a>15</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.6.1.2 "><p id="a2700c75538594abfaf893bb2f2f6145c"><a name="a2700c75538594abfaf893bb2f2f6145c"></a><a name="a2700c75538594abfaf893bb2f2f6145c"></a>LOS_ERRNO_SWTMR_STATUS_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="15.85%" headers="mcps1.1.6.1.3 "><p id="ae4cd506c0bb541d18ef51f9e944356f0"><a name="ae4cd506c0bb541d18ef51f9e944356f0"></a><a name="ae4cd506c0bb541d18ef51f9e944356f0"></a>0x0200030e</p>
</td>
<td class="cellrowborder" valign="top" width="24.13%" headers="mcps1.1.6.1.4 "><p id="a137d20767c634b2abe926052f966893b"><a name="a137d20767c634b2abe926052f966893b"></a><a name="a137d20767c634b2abe926052f966893b"></a>不正确的软件定时器状态。</p>
</td>
<td class="cellrowborder" valign="top" width="25.75%" headers="mcps1.1.6.1.5 "><p id="afd9e91ad60f6492a9d9c0c7aed4ff03d"><a name="afd9e91ad60f6492a9d9c0c7aed4ff03d"></a><a name="afd9e91ad60f6492a9d9c0c7aed4ff03d"></a>检查确认软件定时器状态。</p>
</td>
</tr>
<tr id="r145e4dcea0064dc9838fd4255c877dbd"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="af343f72939e94ff68f288a1000b01795"><a name="af343f72939e94ff68f288a1000b01795"></a><a name="af343f72939e94ff68f288a1000b01795"></a>16</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.6.1.2 "><p id="ae41c2d72f5884bad9102bc8324e0fa09"><a name="ae41c2d72f5884bad9102bc8324e0fa09"></a><a name="ae41c2d72f5884bad9102bc8324e0fa09"></a>LOS_ERRNO_SWTMR_SORTLIST_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="15.85%" headers="mcps1.1.6.1.3 "><p id="zh-cn_topic_0015795101_p224588410124"><a name="zh-cn_topic_0015795101_p224588410124"></a><a name="zh-cn_topic_0015795101_p224588410124"></a>null</p>
</td>
<td class="cellrowborder" valign="top" width="24.13%" headers="mcps1.1.6.1.4 "><p id="ab0141a2b1728420ea19c476246d26d5b"><a name="ab0141a2b1728420ea19c476246d26d5b"></a><a name="ab0141a2b1728420ea19c476246d26d5b"></a>暂无。</p>
</td>
<td class="cellrowborder" valign="top" width="25.75%" headers="mcps1.1.6.1.5 "><p id="a0524cd9727674dfa9fb2118887725788"><a name="a0524cd9727674dfa9fb2118887725788"></a><a name="a0524cd9727674dfa9fb2118887725788"></a>该错误码暂不使用。</p>
</td>
</tr>
<tr id="r5ec3bfbd9e3c4cf38fede8a75fce2c1f"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="ab58f9b82b0254b5eab8a77884eea9010"><a name="ab58f9b82b0254b5eab8a77884eea9010"></a><a name="ab58f9b82b0254b5eab8a77884eea9010"></a>17</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.6.1.2 "><p id="zh-cn_topic_0015795101_p602173510127"><a name="zh-cn_topic_0015795101_p602173510127"></a><a name="zh-cn_topic_0015795101_p602173510127"></a>LOS_ERRNO_SWTMR_TICK_PTR_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="15.85%" headers="mcps1.1.6.1.3 "><p id="ab569f90f9eb14bd49a4eb57130fa9b75"><a name="ab569f90f9eb14bd49a4eb57130fa9b75"></a><a name="ab569f90f9eb14bd49a4eb57130fa9b75"></a>0x02000310</p>
</td>
<td class="cellrowborder" valign="top" width="24.13%" headers="mcps1.1.6.1.4 "><p id="zh-cn_topic_0015795101_p401190419321"><a name="zh-cn_topic_0015795101_p401190419321"></a><a name="zh-cn_topic_0015795101_p401190419321"></a>用以获取软件定时器超时tick数的入参指针为NULL。</p>
</td>
<td class="cellrowborder" valign="top" width="25.75%" headers="mcps1.1.6.1.5 "><p id="a65a58555afcb436391d38bb8b64c5346"><a name="a65a58555afcb436391d38bb8b64c5346"></a><a name="a65a58555afcb436391d38bb8b64c5346"></a>创建一个有效的变量。</p>
</td>
</tr>
<tr id="row192161612017"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="p1838214182010"><a name="p1838214182010"></a><a name="p1838214182010"></a>18</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.6.1.2 "><p id="p683811432011"><a name="p683811432011"></a><a name="p683811432011"></a>LOS_ERRNO_SWTMR_SORTLINK_CREATE_FAILED</p>
</td>
<td class="cellrowborder" valign="top" width="15.85%" headers="mcps1.1.6.1.3 "><p id="p283810144206"><a name="p283810144206"></a><a name="p283810144206"></a>0x02000311</p>
</td>
<td class="cellrowborder" valign="top" width="24.13%" headers="mcps1.1.6.1.4 "><p id="p11838101416204"><a name="p11838101416204"></a><a name="p11838101416204"></a>软件定时器链表创建失败</p>
</td>
<td class="cellrowborder" valign="top" width="25.75%" headers="mcps1.1.6.1.5 "><p id="p383881462015"><a name="p383881462015"></a><a name="p383881462015"></a>检查内存是否够用，然后重新创建链表</p>
</td>
</tr>
</tbody>
</table>

**错误码定义：** 错误码是一个32位的存储单元，31\~24位表示错误等级，23\~16位表示错误码标志，15\~8位代表错误码所属模块，7\~0位表示错误码序号，如下所示：

```
#define LOS_ERRNO_OS_NORMAL(MID,ERRNO)  \
(LOS_ERRTYPE_NORMAL | LOS_ERRNO_OS_ID | ((UINT32)(MID) << 8) | (ERRNO))
LOS_ERRTYPE_NORMAL ：Define the error level as critical
LOS_ERRNO_OS_ID ：OS error code flag.
MID：OS_MOUDLE_ID
ERRNO：error ID number
```

例如：

```
#define LOS_ERRNO_SWTMR_PTR_NULL  \
LOS_ERRNO_OS_ERROR(LOS_MOD_SWTMR, 0x00)
```

### 注意事项

-   软件定时器的回调函数中不要做过多操作，不要使用可能引起任务挂起或者阻塞的接口或操作。
-   软件定时器使用了系统的一个队列和一个任务资源，软件定时器任务的优先级设定为0，且不允许修改 。
-   系统可配置的软件定时器资源个数是指：整个系统可使用的软件定时器资源总个数，而并非是用户可使用的软件定时器资源个数。例如：系统软件定时器多占用一个软件定时器资源数，那么用户能使用的软件定时器资源就会减少一个。
-   创建单次软件定时器，该定时器超时执行完回调函数后，系统会自动删除该软件定时器，并回收资源。
-   创建单次不自删除属性的定时器，用户需要调用定时器删除接口删除定时器，回收定时器资源，避免资源泄露。

### 编程实例

#### 实例描述


在下面的例子中，演示如下功能：

1.  软件定时器创建、启动、删除、暂停、重启操作。
2.  单次软件定时器，周期软件定时器使用方法。

#### 编程示例

前提条件：

-   在los\_config.h中，将LOSCFG\_BASE\_CORE\_SWTMR配置项打开。
-   配置好LOSCFG\_BASE\_CORE\_SWTMR\_LIMIT最大支持的软件定时器数。
-   配置好OS\_SWTMR\_HANDLE\_QUEUE\_SIZE软件定时器队列最大长度。

代码实现如下：

```
 void Timer1_Callback(uint32_t arg);  // callback fuction  
                    
 void Timer2_Callback(uint32_t arg); 
  
 UINT32 g_timercount1 = 0;   
 UINT32 g_timercount2 = 0;  
  
 void Timer1_Callback(uint32_t arg)//回调函数1 
 { 
   unsigned long tick_last1;    
   g_timercount1++; 
   tick_last1=(UINT32)LOS_TickCountGet();//获取当前Tick数
   dprintf("g_timercount1=%d\n",g_timercount1); 
   dprintf("tick_last1=%d\n",tick_last1); 
    
 } 
  
 void Timer2_Callback(uint32_t arg)//回调函数2 
 { 
   unsigned long tick_last2;   
   tick_last2=(UINT32)LOS_TickCountGet(); 
   g_timercount2 ++; 
   dprintf("g_timercount2=%d\n",g_timercount2); 
   dprintf("tick_last2=%d\n",tick_last2); 
 } 
  
 void Timer_example (void)  {                                                          
   UINT16 id1;   
   UINT16 id2;// timer id 
   UINT32 uwTick; 
   /*创建单次软件定时器，Tick数为1000，启动到1000Tick数时执行回调函数1 */  
   LOS_SwtmrCreate (1000, LOS_SWTMR_MODE_ONCE,Timer1_Callback,&id1,1); 
   /*创建周期性软件定时器，每100Tick数执行回调函数2 */ 
   LOS_SwtmrCreate(100,LOS_SWTMR_MODE_PERIOD,Timer2_Callback,&id2,1); 
   dprintf("create Timer1 success\n"); 
     
   LOS_SwtmrStart (id1); //启动单次软件定时器
   dprintf("start Timer1 sucess\n"); 
    
   LOS_TaskDelay(200);//延时200Tick数
   LOS_SwtmrTimeGet(id1,&uwTick);//获得单次软件定时器剩余Tick数
   dprintf("uwTick =%d\n",uwTick); 
    
   LOS_SwtmrStop(id1);//停止软件定时器
   dprintf("stop Timer1 sucess\n"); 
    
   LOS_SwtmrStart(id1); 
   LOS_TaskDelay(1000); 
   LOS_SwtmrDelete(id1);//删除软件定时器
   dprintf("delete Timer1 sucess\n"); 
    
   LOS_SwtmrStart(id2);//启动周期性软件定时器
   dprintf("start Timer2\n"); 
    
   LOS_TaskDelay(1000); 
   LOS_SwtmrStop(id2); 
   LOS_SwtmrDelete(id2);  
  }
```

####  结果验证

得到的结果为：

![](figures/zh-cn_image_0238785960.gif)

#### 完整实例代码

[sample\_Timer.c](resource/sample_Timer.c)

<h3 id="14">4.10 错误处理</h3>

### 概述

#### 基本概念

错误处理指用户代码发生错误时，系统调用错误处理模块的接口函数，完成上报错误信息，并调用用户自己的钩子函数，进行特定的处理。

错误处理模块实现OS内部错误码记录功能。OS内部错误码无法通过接口返回，通常会上报错误处理模块进行记录。用户可以通过挂接错误处理的钩子函数，进行特定的处理。如果OS上报的错误是致命错误，系统会进行异常流程接管，从而可以保存现场以便定位问题。

通过错误处理，我们可以把用户在程序中的非法输入进行控制和提示，以防程序崩溃。

#### 运作机制

错误处理是一种机制，用于处理异常状况。当程序出现异常错误的时候，会显示相应的错误码用于提示用户。此外，如果有相应的错误处理程序，则会执行这个程序。

**图 1**  错误处理示意图<a name="fig19530112013248"></a>  


![](figures/zh-cn_image_0238816605.jpg)

### 开发指导

#### 功能

错误处理模块为用户提供下面几种功能。

<a name="table17575811209"></a>
<table><thead align="left"><tr id="row165973112209"><th class="cellrowborder" valign="top" width="24.240000000000002%" id="mcps1.1.4.1.1"><p id="p1159712110209"><a name="p1159712110209"></a><a name="p1159712110209"></a>功能分类</p>
</th>
<th class="cellrowborder" valign="top" width="25.25%" id="mcps1.1.4.1.2"><p id="p359715111205"><a name="p359715111205"></a><a name="p359715111205"></a>接口名</p>
</th>
<th class="cellrowborder" valign="top" width="50.51%" id="mcps1.1.4.1.3"><p id="p1059715111206"><a name="p1059715111206"></a><a name="p1059715111206"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row75971216203"><td class="cellrowborder" valign="top" width="24.240000000000002%" headers="mcps1.1.4.1.1 "><p id="p1597161112010"><a name="p1597161112010"></a><a name="p1597161112010"></a>错误处理</p>
</td>
<td class="cellrowborder" valign="top" width="25.25%" headers="mcps1.1.4.1.2 "><p id="p125973113205"><a name="p125973113205"></a><a name="p125973113205"></a>LOS_ErrHandle</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.4.1.3 "><p id="p059717117201"><a name="p059717117201"></a><a name="p059717117201"></a>根据错误处理函数来对错误进行处理</p>
</td>
</tr>
</tbody>
</table>

### 注意事项

无。

### 编程实例

#### 实例描述

在下面的例子中，演示如下功能：

执行错误处理函数。

#### 编程示例

代码实现如下：

```
extern USER_ERR_FUNC_S  g_stUserErrFunc; 
void err_handler(CHAR *pcFileName,UINT32 uwLineNo, 
                 UINT32 uwErrorNo,UINT32 uwParaLen,VOID  *pPara) 
{ 
    printf("err handle ok\n"); 
} 
UINT32 Example_ErrCaseEntry(VOID) 
{ 
    g_stUserErrFunc.pfnHook = err_handler; 
    /*执行错误处理函数*/ 
    LOS_ErrHandle(NULL, 0,0,0, NULL); 
    return LOS_OK; 
}
```

#### 结果验证

编译运行得到的结果为：

```
Huawei LiteOS # err handle ok
```

#### 完整实例代码

[sample\_err.c](resource/sample_err.c)

<h3 id="15">4.11 双向链表</h3>

### 概述

#### 基本概念

双向链表是指含有往前和往后两个方向的链表，即每个结点中除存放下一个节点指针外，还增加一个指向其前一个节点的指针。其头指针head是唯一确定的。  

从双向链表中的任意一个结点开始，都可以很方便地访问它的前驱结点和后继结点，这种数据结构形式使得双向链表在查找时更加方便，特别是大量数据的遍历。由于双向链表具有对称性，能方便地完成各种插入、删除等操作，但需要注意前后方向的操作。

### 开发指导

#### 功能

Huawei LiteOS系统中的双向链表模块为用户提供下面几个接口。

<a name="table31878844162710"></a>
<table><thead align="left"><tr id="row24909577162710"><th class="cellrowborder" valign="top" width="17.349999999999998%" id="mcps1.1.4.1.1"><p id="p4409895162710"><a name="p4409895162710"></a><a name="p4409895162710"></a>功能分类</p>
</th>
<th class="cellrowborder" valign="top" width="26.75%" id="mcps1.1.4.1.2"><p id="p21657225162710"><a name="p21657225162710"></a><a name="p21657225162710"></a>接口名</p>
</th>
<th class="cellrowborder" valign="top" width="55.900000000000006%" id="mcps1.1.4.1.3"><p id="p9404824162710"><a name="p9404824162710"></a><a name="p9404824162710"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row3332575162537"><td class="cellrowborder" valign="top" width="17.349999999999998%" headers="mcps1.1.4.1.1 "><p id="p1503143162537"><a name="p1503143162537"></a><a name="p1503143162537"></a>初始化链表</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.4.1.2 "><p id="p54645735162537"><a name="p54645735162537"></a><a name="p54645735162537"></a>LOS_ListInit</p>
</td>
<td class="cellrowborder" valign="top" width="55.900000000000006%" headers="mcps1.1.4.1.3 "><p id="p64228424162537"><a name="p64228424162537"></a><a name="p64228424162537"></a>对链表进行初始化。</p>
</td>
</tr>
<tr id="row23593275162710"><td class="cellrowborder" valign="top" width="17.349999999999998%" headers="mcps1.1.4.1.1 "><p id="p20608486161731"><a name="p20608486161731"></a><a name="p20608486161731"></a>增加节点</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.4.1.2 "><p id="p42440799162710"><a name="p42440799162710"></a><a name="p42440799162710"></a>LOS_ListAdd</p>
</td>
<td class="cellrowborder" valign="top" width="55.900000000000006%" headers="mcps1.1.4.1.3 "><p id="p15152688162710"><a name="p15152688162710"></a><a name="p15152688162710"></a>将新节点添加到链表中。</p>
</td>
</tr>
<tr id="row2156464162710"><td class="cellrowborder" valign="top" width="17.349999999999998%" headers="mcps1.1.4.1.1 "><p id="p40455902162710"><a name="p40455902162710"></a><a name="p40455902162710"></a>在链表尾端插入节点</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.4.1.2 "><p id="p55702669162710"><a name="p55702669162710"></a><a name="p55702669162710"></a>LOS_ListTailInsert</p>
</td>
<td class="cellrowborder" valign="top" width="55.900000000000006%" headers="mcps1.1.4.1.3 "><p id="p15622373162710"><a name="p15622373162710"></a><a name="p15622373162710"></a>将节点插入到双向链表尾端。</p>
</td>
</tr>
<tr id="row15258173016234"><td class="cellrowborder" valign="top" width="17.349999999999998%" headers="mcps1.1.4.1.1 "><p id="p1517610334234"><a name="p1517610334234"></a><a name="p1517610334234"></a>在链表头端插入节点</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.4.1.2 "><p id="p18176633172314"><a name="p18176633172314"></a><a name="p18176633172314"></a>LOS_ListHeadInsert</p>
</td>
<td class="cellrowborder" valign="top" width="55.900000000000006%" headers="mcps1.1.4.1.3 "><p id="p917653392317"><a name="p917653392317"></a><a name="p917653392317"></a>将节点插入到双向链表头端</p>
</td>
</tr>
<tr id="row448985511526"><td class="cellrowborder" valign="top" width="17.349999999999998%" headers="mcps1.1.4.1.1 "><p id="p49686117162150"><a name="p49686117162150"></a><a name="p49686117162150"></a>删除节点</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.4.1.2 "><p id="p6426197611526"><a name="p6426197611526"></a><a name="p6426197611526"></a>LOS_ListDelete</p>
</td>
<td class="cellrowborder" valign="top" width="55.900000000000006%" headers="mcps1.1.4.1.3 "><p id="p52712195115237"><a name="p52712195115237"></a><a name="p52712195115237"></a>将指定的节点从链表中删除。</p>
</td>
</tr>
<tr id="row20058016115551"><td class="cellrowborder" valign="top" width="17.349999999999998%" headers="mcps1.1.4.1.1 "><p id="p14086595115551"><a name="p14086595115551"></a><a name="p14086595115551"></a>判断双向链表是否为空</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.4.1.2 "><p id="p163555115551"><a name="p163555115551"></a><a name="p163555115551"></a>LOS_ListEmpty</p>
</td>
<td class="cellrowborder" valign="top" width="55.900000000000006%" headers="mcps1.1.4.1.3 "><p id="p13247975115551"><a name="p13247975115551"></a><a name="p13247975115551"></a>判断链表是否为空。</p>
</td>
</tr>
<tr id="row63552992171522"><td class="cellrowborder" valign="top" width="17.349999999999998%" headers="mcps1.1.4.1.1 "><p id="p35996298171524"><a name="p35996298171524"></a><a name="p35996298171524"></a>删除节点并初始化链表</p>
</td>
<td class="cellrowborder" valign="top" width="26.75%" headers="mcps1.1.4.1.2 "><p id="p30019024171524"><a name="p30019024171524"></a><a name="p30019024171524"></a>LOS_ListDelInit</p>
</td>
<td class="cellrowborder" valign="top" width="55.900000000000006%" headers="mcps1.1.4.1.3 "><p id="p15621850171524"><a name="p15621850171524"></a><a name="p15621850171524"></a>将指定的节点从链表中删除，使用该节点初始化链表。</p>
</td>
</tr>
</tbody>
</table>

### 开发流程

双向链表的典型开发流程：

1.  调用LOS\_ListInit初始双向链表。
2.  调用LOS\_ListAdd向链表中增加节点。
3.  调用LOS\_ListTailInsert向链表尾部插入节点。
4.  调用LOS\_ListDelete删除指定节点。
5.  调用LOS\_ListEmpty判断链表是否为空。
6.  调用LOS\_ListDelInit删除指定节点并以此节点初始化链表。

### 注意事项

-   需要注意节点指针前后方向的操作。

### 编程实例

#### 实例描述

使用双向链表，首先要申请内存，删除节点的时候要注意释放掉内存。

本实例实现如下功能：

1.  调用函数进行初始化双向链表。
2.  增加节点。
3.  删除节点。
4.  测试操作是否成功。

#### 编程示例

代码实现如下：

```
#include "stdio.h"
#include "los_list.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

static UINT32 DLlist_sample(VOID)
{
    LOS_DL_LIST DLlist = {NULL,NULL};
    LOS_DL_LIST DLlistNode01 = {NULL,NULL};
    LOS_DL_LIST DLlistNode02 = {NULL,NULL};
    LOS_DL_LIST DLlistNode03 = {NULL,NULL};

    PRINTK("Initial head\n");
    LOS_ListInit(&DLlist);

    LOS_ListAdd(&DLlist,&DLlistNode01);
    if (DLlistNode01.pstNext == &DLlist && DLlistNode01.pstPrev == &DLlist)
    {
        PRINTK("Add DLlistNode01 success \n");
    }

    LOS_ListTailInsert(&DLlist,&DLlistNode02);
    if (DLlistNode02.pstNext == &DLlist && DLlistNode02.pstPrev == &DLlistNode01)
    {
        PRINTK("Tail insert DLlistNode02 success \n");
    }

    LOS_ListHeadInsert(&DLlistNode02,&DLlistNode03);
    if (DLlistNode03.pstNext == &DLlist && DLlistNode03.pstPrev == &DLlistNode02)
    {
        PRINTK("Head insert DLlistNode03 success \n");
    }

    LOS_ListDelInit(&DLlistNode03);
    LOS_ListDelete(&DLlistNode01);
    LOS_ListDelete(&DLlistNode02);

    if (LOS_ListEmpty(&DLlist))
    {
        PRINTK("Delete success \n");
    }

    return LOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
```

#### 结果验证

编译运行得到的结果为：

```
Initial head 
Add DLlistNode01 success 
Tail insert DLlistNode02 success 
Head insert DLlistNode03 success 
Delete success
```

