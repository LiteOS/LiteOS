# Agent Lwm2m Demo 使用手册
agent lwm2m demo 实现了lwm2m客户端功能， 提供了psk加密通信验证方式，在需要与lwm2m服务器创建通信的场景中， 可以根据学习及应用需求， 按照以下文档所述切换对应的客户端通信方式。 
该文档也提供了lwm2m服务端与客户端demo创建通信的简易配置方法，供通信验证使用, 测试使用的lwm2m服务端为`Leshan lwm2m server`,  lwm2m服务端搭建测试及`liteos源码`配置和编译，均在"`ubuntu`"系统下进行。  
关于设备端agent lwm2m demo的实现所使用的硬件为stm32开发板 “`野火Cloud_STM32F429IGTx_FIRE`”。   
## 使用 make menuconfig 使能运行Demo需要的配置
开启以下几类组件，来使能 Agent Lwm2m Demo， 功能说明如下：
* Lwm2m, Agenttiny Lwm2m组件，实现lwm2m客户端功能。
* vfs 组件， 实现虚拟文件系统接口。
* Log 组件, 实现调试信息打印输出。
* Lwip, Sal 组件， 实现网络通信功能。
* Mbedtls 组件， 实现加密通信， 使能加密时开启， 不使能加密时，关闭。   

menuconfig 具体配置如下所示：
```c
Componetns --->
        Connectivity --->
                [*] Enable Lwm2m
                [*] Enable Agenttiny Lwm2m
        FileSystem --->
                [*] Enable FileSystem
                -*-     Enable VFS
        Log --->
                [*] Enable Log
        Network --->
                [*] Enable Network
                [*]     Enable Lwip
                [*]     Enable Sal
        Security --->
                [*] Enable Security
                [*]     Enable Mbed TLS Component
Demos --->
        Agent Tiny Lwm2m Demo  --->
                [*] Enable Agent Tiny Lwm2m Demo
```
## 使能Agent Lwm2m Demo

1, 设置设备端IPv4地址
在源码<a href="https://gitee.com/LiteOS/LiteOS/blob/master/targets/Cloud_STM32F429IGTx_FIRE/Src/sys_init.c" target="_blank">targets/Cloud_STM32F429IGTx_FIRE/Src/sys_init.c</a> 中， 如下所示， 如果mosquitto服务搭建在局域网环境中，设备的ip需要与ubuntu主机在同一网段下， 这里的示例设备端ip为"`192.168.3.200`"。
```c
void net_init(void)
{
    /* IP addresses initialization */
    IP_ADDRESS[0] = 192;
    IP_ADDRESS[1] = 168;
    IP_ADDRESS[2] = 3;
    IP_ADDRESS[3] = 200;
    NETMASK_ADDRESS[0] = 255;
    NETMASK_ADDRESS[1] = 255;
    NETMASK_ADDRESS[2] = 255;
    NETMASK_ADDRESS[3] = 0;
    GATEWAY_ADDRESS[0] = 192;
    GATEWAY_ADDRESS[1] = 168;
    GATEWAY_ADDRESS[2] = 3;
    GATEWAY_ADDRESS[3] = 1;
	......
```


2, Agent Lwm2m Demo源码文件为<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/agenttiny_lwm2m/agent_tiny_lwm2m_demo.c" target="_blank">demos/agenttiny_lwm2m/agent_tiny_lwm2m_demo.c</a>中。
```c  
/* 设置lwm2m服务端ip为192.168.3.98，此ip与搭建lwm2m服务端的主机为同一ip, 
 如何搭建在下面“测试Agent lwm2m demo psk加密通信“中介绍。*/
#define DEFAULT_SERVER_IP "192.168.3.98" /* local ipv4 */
#define LWM2M_LIFE_TIME     50000
char *g_endpoint_name = "44440003";
/* 使用加密时，使能“Enable Security”来开启psk加密，不使用加密时，不使能“Enable Security” */
#ifdef LOSCFG_COMPONENTS_SECURITY_MBEDTLS
/* 设置节点名 * /
#define ENDPOINT_NAME_S "20201112_client"
#define ENDPOINT_NAME_IOT "20201112"
#define ENDPOINT_NAME_BS "20201112"
char *g_endpoint_name_s = ENDPOINT_NAME_S;
char *g_endpoint_name_iots = ENDPOINT_NAME_IOT;
char *g_endpoint_name_bs = ENDPOINT_NAME_BS;
/* 设置 psk 值*/
unsigned char g_psk_iot_value[] = {0x68,0xda,0x7a,0xea,0xf6,0x12,0xfd,0x95,0xbb,0xe0,0x91,0x5a,0x67,0xca,0x56,0xb3}; // 0x33 -> 0x32
unsigned char g_psk_bs_value[] = {0x68,0xda,0x7a,0xea,0xf6,0x12,0xfd,0x95,0xbb,0xe0,0x91,0x5a,0x67,0xca,0x56,0xb3};
#endif
```
经过以上内容的操作赫和修改后，进行代码编译。 
## 编译烧录`Agent Lwm2m Demo`
在`LiteOS工程`目录下执行以下命令：
```c
make clean && make -j
```   
生成的`Huawei_LiteOS.bin`固件在路径`out/Cloud_STM32F429IGTx_FIRE`中，烧录到开发板中即可。


---
## 测试Agent lwm2m demo
1， 获取`leshan lwm2m server demo`工程
```c
git clone https://github.com/eclipse/leshan.git
```
2， 在leshan lwm2m工程目录下编译
```c
mvn clean install
```
3， 在leshan lwm2m工程目录下，运行 `leshan lwm2m server demo`
```c
java -jar leshan-bsserver-demo/target/leshan-bsserver-demo-*-SNAPSHOT-jar-with-dependencies.jar 
```
4， 访问Leshan提供的演示页面: http://localhost:8080， 当使用psk加密时， 添加以下psk配置， 不使用加密时，不进行以下操作。

* 演示页面里点击 【SECURITY】 按钮进入配置信息管理页面
* 点击【Add new client security configuration】按钮 增加配置信息
* 【Client endpoint】--> 20201112_client 
* 【Security Mode】  --> Pre-Shared Key
* 【Identity】       -->  20201112
* 【Key】--> 68da7aeaf612fd95bbe0915a67ca56b3

---
5，设备端代码使能加密时，烧录`Atiny Lwm2m Demo`固件，上电运行后， 在Leshan演示主页面内会显示 【Client endpoint】为`20201112_client`。  
6， 设备端代码无加密时，烧录`Atiny Lwm2m Demo`固件，上电运行后， 在Leshan演示主页面内会显示 【Client endpoint】为`44440003`。    
7， 设备串口打印消息显示了与lwm2m server建立连接的过程， 如下所示。
```c
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.0.1
build data : Dec 16 2020 01:17:28

**********************************
osAppInit
cpu 0 entering scheduler
app init!
/* 创建lwip 联网服务 */
lwip test init ok.
[INFO][9184][atiny_data_change:562] not registered
[INFO][9188][atiny_init_objects:191] Trying to init objects
[INFO][9194][lwm2m_connect_server:317] Now come into Connection creation in lwm2m_connect_server 

[INFO][9203][connection_create:205] now come into connection_create!!!
[MBEDTLS][dtls_ssl_new:142] setting up the SSL structure
[MBEDTLS][dtls_ssl_new:209] set SSL structure succeed
[MBEDTLS][dtls_shakehand:263] connecting to server
[SOCKET][atiny_net_connect:270] try to do name resolution now...
[SOCKET][atiny_net_connect:277] do name resolution succeed
/* udp 连接创建成功 */
[SOCKET][atiny_net_connect:321] UDP create socket and bind to server finished
[MBEDTLS][dtls_shakehand:280] performing the SSL/TLS handshake

Huawei LiteOS # [INFO][11188][atiny_data_change:562] not registered
/* mbedtls psk 认证 */
[MBEDTLS][dtls_shakehand:327] handshake succeed、
/* 连接lwm2m server 成功 */
[INFO][11754][lwm2m_connect_server:332] Connection creation successfully in lwm2m_connect_server.
[INFO][11763][lwm2m_buffer_send:426] call connection_send in lwm2m_buffer_send, length is 122
[INFO][12249][atiny_notify_stack_rpt_data_change:128] data change cnt 1 uri(flag:0x7, objId:19, instId:0, resId:0)
```
