[![Build Status](https://travis-ci.org/LiteOS/LiteOS.svg?branch=develop)](https://travis-ci.org/LiteOS/LiteOS)

## Huawei LiteOS简介

Huawei LiteOS是华为面向物联网领域开发的一个基于实时内核的轻量级操作系统。本项目属于<a href="https://www.huaweicloud.com/product/liteos.html" target="_blank">华为物联网操作系统[Huawei LiteOS]</a>源码，现有基础内核包括不可裁剪的极小内核和可裁剪的其他模块。极小内核包含任务管理、内存管理、异常管理、系统时钟和中断管理。可裁剪模块包括信号量、互斥锁、队列管理、事件管理、软件定时器等。除了基础内核，Huawei LiteOS还提供了扩展内核，包括C++支持、动态加载、低功耗以及维测模块。低功耗通过支持Tickless机制、run-stop休眠唤醒，可以大大降低系统功耗。维测部分包含了获取CPU占用率、支持串口执行Shell命令等功能。

Huawei LiteOS同时提供端云协同能力，集成了LwM2M、CoAP、mbedtls、LwIP全套IoT互联协议栈，且在LwM2M的基础上，提供了AgentTiny模块，用户只需关注自身的应用，而不必关注LwM2M实现细节，直接使用AgentTiny封装的接口即可简单快速实现与云平台安全可靠的连接。

Huawei LiteOS自开源社区发布以来，围绕NB-IoT物联网市场从技术、生态、解决方案、商用支持等多维度使能合作伙伴，构建开源的物联网生态。目前已经聚合了50+ MCU和解决方案合作伙伴，共同推出一批开源开发套件和行业解决方案，帮助众多行业客户快速推出物联网终端和服务，客户涵盖抄表、停车、路灯、环保、共享单车、物流等众多行业，为开发者提供 “一站式” 完整软件平台，可有效降低开发门槛、缩短开发周期。

## LiteOS 代码导读

- [LiteOS内核源代码目录说明](./doc/LiteOS_Code_Info.md)

该文档描述的是LiteOS内核源代码的详细信息。通过此文档读者可以了解LiteOS的源代码结构，以及LiteOS的main()函数的功能。


## LiteOS 移植指南

- [LiteOS移植指南](./doc/LiteOS_Porting_Guide.md)

该文档基于STM32芯片平台，详细介绍如何快速移植LiteOS。


## LiteOS 开发指南

- [LiteOS开发指南](./doc/Huawei_LiteOS_Kernel_Developer_Guide_zh.md)

该文档详细讲解了LiteOS各模块开发及其实现原理。用户可以通过阅读该文档学习各模块的使用。


## LiteOS 编译介绍和开发工具

- [LiteOS编译介绍和开发工具](./doc/LiteOS_Build_and_IDE.md)

该文档介绍了LiteOS的编译框架，以及如何在Linux和Windows下编译LiteOS。


## LiteOS 维测指南

- [LiteOS维测指南](./doc/LiteOS_Maintenance_Guide.md)

该文档详细介绍了LiteOS具备的维测能力，包括内存调测方法、IPC通信调测方法、Trace、调度统计、获取CPU占用率等。


## LiteOS Demos

[LiteOS demos](./demos)目录下包含了LiteOS提供的各模块Demo及其文档。文档详细介绍了Demo的功能以及运行方法。


## LiteOS Shell

- [Shell使用教程](./shell/README_CN.md)

该文档详细讲解了在LiteOS如何定制用户自定义Shell命令，以及如何执行内置的Shell命令。


## LiteOS 标准库

- [LiteOS标准库](./doc/LiteOS_Standard_Library.md)

该文档列出了LiteOS支持的POSIX、CMSIS等接口。


## LiteOS 支持的硬件

* LiteOS开源项目目前支持ARM Cortex-M0，Cortex-M3，Cortex-M4，Cortex-M7，Cortex-A等芯片架构

* LiteOS联合业界主流MCU厂家，通过开发者活动，目前已经适配了多种通用MCU开发套件


## 开源协议

* 遵循BSD-3开源许可协议
* <a href="https://support.huaweicloud.com/productdesc-LiteOS/zh-cn_topic_0145347224.html">Huawei LiteOS 知识产权政策</a>


## LiteOS 代码贡献必读

- [LiteOS Commit Message规则](./doc/LiteOS_Commit_Message.md)

该文档描述如何提交commit到LiteOS仓库，这是LiteOS开发必须遵守的commit规则，否则提交的commit会被驳回。请点链接了解详细信息。

- [Huawei LiteOS编程规范](./doc/LiteOS_Coding_Standard.md)

该文档描述LiteOS的编程规范，此规范建立在业界通用的编程规范基础上。

- [Huawei LiteOS代码贡献流程](./doc/LiteOS_Contribute_Guide_GitGUI.md)

该文档描述开发者如何创建自己的仓库，开发然后贡献代码到LiteOS仓库。请点链接了解详细信息。


## 加入我们
* 欢迎提交issue对关心的问题发起讨论，欢迎提交PR参与特性建设
* 如您有合作意向，希望加入Huawei LiteOS生态合作伙伴，请发邮件至liteos@huawei.com，或访问<a href="http://www.huawei.com/liteos" target="_blank">LiteOS官网</a>，进一步了解详细信息
