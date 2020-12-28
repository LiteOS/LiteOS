# NB IoT

LiteOS提供一套NB IoT的组件和例程。目前支持通过AT指令控制BC95 NB IoT模块连接云服务。本文档通过连接华为云服务介绍怎么运行NB IoT demo实现设备连接和数据上传。对于其他厂商提供的云服务，注册NB IoT设备后，也同样可以实现设备连接和数据上传。

## 接口介绍

### NB IoT初始化函数
```c
int los_nb_init(const int8_t *host, const int8_t *port, sec_param_s *psk);
```
- host：云服务器IPv4地址
- port：云服务器端口
- psk：加密所用的证书或秘钥，如不加密则为空

### NB IoT发送函数
```c
int los_nb_report(const char *buf, int buflen);
```
- buf：待上报数据指针
- buflen：待上报数据长度

### NB IoT注册函数
```c
int los_nb_notify(char *featurestr, int cmdlen, oob_callback callback, oob_cmd_match cmd_match);
```
- featurestr：要注册的AT命令
- cmdlen：要注册的AT命令长度
- callback：设备回调函数
- cmd_match：命令处理函数

### NB IoT取消注册函数
```c
int los_nb_deinit(void);
```

## 配置华为云服务

### 创建产品

登录华为云后找到“物联网平台 -> 产品”，点击右上角“创建产品”，详细操作方法可以参考<a href="https://support.huaweicloud.com/usermanual-iothub/iot_01_0054.html" target="_blank">华为云创建产品指南</a>。

### 注册设备

选择“设备” -> “所有设备”，点击右上角“注册设备”或“批量注册”。在注册设备时，所属资源空间选择创建产品中的产品名称，设备标识码为设备的IMEI或MAC地址，设备名称（任填），设备认证类型根据自己的加密方式进行选择“秘钥”或“证书”。创建成功后保留设备ID和设备秘钥。此时设备列表会显现刚刚注册的设备名称为未激活状态。详细操作方法可以参考<a href="https://support.huaweicloud.com/usermanual-iothub/iot_01_0028.html" target="_blank">华为云注册设备指南</a>。

### profile开发

profile开发的详细操作方法可以参考<a href="https://support.huaweicloud.com/devg-iothub/iot_02_0008.html#ZH-CN_TOPIC_0222727076__section12289612132715" target="_blank">华为云数据上报和命令下发</a>。下面是简单介绍，具体以华为云官方文档为准。

- 选择左侧产品，点击创建好的产品名称。
- 点击模型定义 -> 添加服务，新增服务，此时属性/命令列表里就会显示新增的服务名。
- 点击服务名进行属性和命令的添加（结合自己测试内容进行数据类型选择）。
- 点击插件开发 -> 图形化开发。
- 点击新增消息，分为数据上报和命令下发，添加字段，根据上报和下发命令的数据类型设置，响应字段即数据上报或下发后跟随的响应信息，新增消息后，点击右侧产品模型中之前添加的服务，选择属性按键拖拽到左侧和刚刚新增的数据上报消息匹配，点击命令，拖拽命令按钮至左侧和命令下发字段匹配 ，匹配成功后点击右上角“保存 -> 部署”。

## 配置NB IoT DEMO

### 配置NB IoT DEMO的编译配置项

-  需要在`menuconfig`中打开配置项`LOSCFG_DEMOS_NBIOT_WITHOUT_ATINY`。
   ```
   Demos --> NB-IoT Demo Without Atiny --> Enable NB-IoT Demo
   ```
-  如需加密则打开配置项`LOSCFG_DEMOS_NBIOT_DTLS`。
   ```
   Demos --> NB-IoT Demo Without Atiny --> Enable NB-IoT Demo --> Enable NB-IoT Demo With Dtls
   ```
-  同时在AT设备中选择BC95，即使能`LOSCFG_COMPONENTS_NET_AT_BC95`。
   ```
   Components --> Network --> Enable Network --> Enable AT --> Choose AT Device --> Enable bc95
   ```

**注意**

-  在运行demo前，需要先在云服务上注册NB IoT设备。

-  需要将BC95模块插入支持NB IoT的物联网卡上，并连接到对应的串口上。

### 配置云服务器信息

在`demos/nbiot_without_atiny/nb_demo.c`中有如下配置，在运行demo前需要将这些配置修改为要连接的云服务器信息：
```c
#define TELECON_IP          "119.3.250.80"
#define SECURITY_PORT       "5684"
#define NON_SECURITY_PORT   "5683"
#define DEV_PSKID           "868744031131026"
#define DEV_PSK             "d1e1be0c05ac5b8c78ce196412f0cdb0"
```
- TELECON_IP：云服务器IPv4地址
- SECURITY_PORT：云服务器的加密端口
- NON_SECURITY_PORT：云服务器的非加密端口
- DEV_PSKID：预共享秘钥ID
- DEV_PSK：预共享秘钥

## 启动NB IoT DEMO

完成以上操作后编译LiteOS源码，生成系统镜像文件Huawei_LiteOS.bin，并将.bin文件烧写到开发板，复位重启开发板后，NB IoT demo即启动。此时刷新华为云界面，设备状态就会更新为在线，证明NB IoT设备已经成功接入华为云。

### 在线调试

点击“在线调试 -> 设备名称”，在应用模拟器的数据接收栏就会接收到来自NB IoT demo上报的测试数据。详细操作方法可以参考<a href="https://support.huaweicloud.com/devg-iothub/iot_02_0008.html#ZH-CN_TOPIC_0222727076__section12289612132715" target="_blank">华为云数据上报和命令下发</a>中的“调测编解码插件”章节。