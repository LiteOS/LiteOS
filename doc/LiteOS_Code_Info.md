

## LiteOS代码入口

LiteOS入口在工程对应的main.c中，基本流程如下：

int main(void)
{
    UINT32 uwRet = LOS_OK;
    HardWare_Init();

    uwRet = OsMain();
    if (uwRet != LOS_OK)
    {
        return LOS_NOK;
    }

    OsStart();

    return 0;
}


首先进行硬件初始化：HardWare_Init();

初始化LiteOS内核及例程：OsMain();

调用OsStart(),开始task调度,LiteOS开始正常工作。


## LiteOS的代码目录结构说明

关于代码树中各个目录存放的源代码的相关内容简介如下：

| 一级目录   | 二级目录                 | 三级目录             | 说明                                                         |
| ---------- | ------------------------ | -------------------- | ------------------------------------------------------------ |
| arch       | arm                      | cortex-a             | A核内存mmu、异常处理、时钟等相关代码，新增                   |
|            |                          | cortex-m             | M核中断、调度、tick相关代码                                  |
|            |                          | common               | arm核公用的cmsis core接口                                    |
|            |                          | include              | arm头文件                                                    |
|            | common                   |                      | cmsis头文件                                                  |
|            | msp430                   |                      | msp430架构中断、调度、tick相关代码                           |
| build      |                          |                      | 预留的一个空的makefile                                       |
| components | connectivity             | agent_tiny           | agent_tiny端云互通组件，包括公共头文件、示例代码、客户端实现代码、操作系统适配层代码 |
|            |                          | lwm2m                | lwm2m协议实现                                                |
|            |                          | nb_iot               | LiteOS NB-IoT API                                            |
|            |                          | mqtt                 | MQTT开源协议实现                                             |
|            | ota                      |                      | 固件升级代码                                                 |
|            | fs                       |                      | 文件系统，含VFS、spiffs、ramfs、kifs、fatfs、devfs           |
|            | lib                      | libc                 | LiteOS libc优化                                              |
|            |                          | libsec               | 安全函数库                                                   |
|            |                          | cJSON                | c语言json库                                                  |
|            | log                      |                      | 日志等级控制                                                 |
|            | net                      | at_frame             | LiteOS AT框架API                                             |
|            |                          | at_device            | AT设备适配层                                                 |
|            |                          | lwip/lwip_port       | lwip驱动及OS适配代码                                         |
|            |                          | lwip/lwip-2.0.3      | lwip协议实现                                                 |
|            |                          | lwip/ppp_port        | lwip协议ppp端口支持                                          |
|            |                          | sal                  | socket通讯支持                                               |
|            | security                 | mbedtls/mbedtls_port | mbed TLS的OS适配代码                                         |
|            |                          | mbedtls/mbedtl-2.6.0 | mbed TLS协议实现                                             |
| doc        |                          |                      | 此目录存放的是LiteOS的使用文档和API说明等文档                |
| demos      | kernel                   | API                  | 供开发者测试LiteOS内核的demo示例，此目录存放的是内核功能测试用的相关用例的代码 |
|            |                          | include              | API功能头文件存放目录                                        |
|            | agenttiny_lwm2m          |                      | lwm2m协议 demo                                               |
|            | agenttiny_mqtt           |                      | mqtt 协议demo                                                |
|            | dtls_server              |                      | dtls协议demo                                                 |
|            | fs                       |                      | 文件系统demo                                                 |
|            | ipv6_client              |                      | Ipv6协议demo                                                 |
|            | nbiot_without_atiny      |                      | NB_IoT demo                                                  |
| include    |                          |                      | components各个模块所依赖的头文件                             |
| kernel     | base                     | core                 | LiteOS基础内核代码，包括队列、task调度、软timer、时间片计算等功能 |
|            |                          | om                   | 与错误处理相关的文件                                         |
|            |                          | include              | LiteOS内核内部使用的头文件                                   |
|            |                          | ipc                  | LiteOS中task间通讯的相关接口，包括事件、信号量、消息队列、互斥锁等 |
|            |                          | mem                  | LiteOS中的内核内存管理的相关代码                             |
|            |                          | misc                 | 内存对齐功能以及毫秒级休眠sleep功能                          |
|            |                          | mp                   | 预留多CPU支持                                                |
|            |                          | sched                | 任务调度支持                                                 |
|            | include                  |                      | LiteOS开源内核头文件                                         |
|            | extended                 | tickless             | tickless定时机制实现                                         |
| osdepends  | liteos                   | cmsis                | liteos cmsis_os 1.0和2.0支持                                 |
| targets    | Cloud_STM32F429IGTx_FIRE |                      | 野火STM32F429(ARM Cortex M4)开发板对应的编程及开发工程源码包 |
|            | Mini_Project             |                      | 最小工程                                                     |
|            | NXP_LPC51U68             |                      | NXP ARM Cortex M0 ENC28J60以太网LiteOS SDK端云demo           |
|            | STM32F103VET6_NB_GCC     |                      | F103（ARM Cortex M3）内核移植demo，NB-IoT，GCC环境           |
|            | bsp                      |                      | 通用板级支持包                                               |
| test       |                          |                      | 内核及系统库的参考测试代码                                   |