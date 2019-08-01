### oc_lwm2m_agent说明

oc_lwm2m_agent是处理使用lwm2m协议对接华为OC的流程抽象层，允许使用流程进行对接，也允许使用NB芯片内置的流程进行对接。对于应用程序开发者而言，无需关注对接流程的实现细节，仅仅需要调用该模块提供的api即可。对于lwm2m协议的对接流程的具体实现开发者（基于lwm2m协议或者基于支持lwm2m协议的模组，或者以及内置对接华为OC流程的模组），仅仅需要将实现的流程注册到该agent即可。

#### 安装配置

目前该文件的自动化编译配置支持GCC（MK文件模式），需要在MAKEFILE文件中定义WITH_OC_LWM2M_AGENT并包含huawei_cdp/lwm2m目录下的oc_lwm2m_agent.mk文件；如果使用KEIL或者IAR等IDE，可以参考oc_lwm2m_agent.mk文件将相关的文件加入到工程当中，后续也有计划推出小工具将GCC的工程转为KEIL 或者IAR等的工程。
#### 模块初始化
该模块的初始化调用oc_lwm2m_agent_init即可

#### lwm2m对接流程开发

关于开发基于lwm2m的对接OC的流程，应该实现三个基本函数：config report deconfig。config用于完成和服务器之间的握手（OC称之为设备上线）；report用于上报应用程序的数据；deconfig用于断开服务器连接。开发者调用oc_lwm2m_register将相关的方法注册进oc_lwm2m_agent,供应用程序开发者调用。

相关的使用说明参考oc_lwm2m_agent.h文件；具体实现可以参考boudica150_oc.c文件关于boudica150的相关实现


####  应用开发

对于应用开发者而言，其做的工作流程包括以下几点：

1. 使用oc_lwm2m_config函数进行配置；主要包括服务器IP、端口以及加密方式等，调用该函数成功，则表示成功和服务器握手
2. 使用oc_lwm2m_install_msgdealer注册消息回调函数，可以注册多个，也可以注册一个通用的；需要注意的是不能在钩子函数中再次调用oc_lwm2m_xx相关的api
3. 调用oc_lwm2m_report进行数据上报（按需调用）
4. 如果需要，调用oc_lwm2m_deconfig断开服务器

	需要注意的是，上述调用需要在oc_lwm2m_agent_init进行 成功初始化后，并且有相关的lwm2m对接流程进行成功的对接后才能进行调用，否则会失败；详细的使用说明可以参考oc_lwm2m_agent.h文件

#### 使用说明

相关API或者数据结构请参考oc_lwm2m_agent.h文件。如果您有更好的提议烦请反馈。