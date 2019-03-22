### MQTT ABSTRACT LAYER说明

MQTT AL用来解耦基于MQTT的业务和MQTT的具体实现，具体来说以后的MQTT业务层应该有且只能使用MQTT AL提供的相关功能（API 数据结构 流程等）。MQTT AL定义MQTT的标准，用来屏蔽各个MQTT协议实现的差异（如软件库 或者硬件），让上层业务无需关心MQTT的实现部分。

#### 安装配置

目前该文件的自动化编译配置支持GCC（MK文件模式），需要在MAKEFILE文件中定义WITH_MQTT并包含mqtt目录下的mqtt.mk文件；如果使用KEIL或者IAR等IDE，可以参考mqtt.mk文件将相关的文件加入到工程当中，后续也有计划推出小工具将GCC的工程转为KEIL 或者IAR等的工程。

####  基于MQTT服务开发应用

对于上层的应用而言，需要熟知MQTT的基本流程和意义。基本调用顺序为init-->connect-->application(subscribe unsubscribe  publish)-->disconnect-->deinit

其中applicaiton中，根据自己的业务需求选择subscribe unsubscribe publish等功能，这些功能的调用都是基于句柄的（connect返回），同时如果调用返回出错的情况下可以调用check_state来检查当前连接是否正常，如果不正常则调用disconnect关闭句柄。

句柄一旦关闭，则不能再次使用，需要使用connect再次获取句柄。

init或者deinit是对mqtt协议实现库的初始化或者注销，只需要调用一次即可。

####  MQTT协议开发者

对于MQTT协议的实现，不论是基于协议版本，还是借助于外部硬件，都需要完成init deinit connect disconnect subscribe unsubscribe publish等接口，并将相关接口使用mqtt_al_install函数注册到mqtt_al中，如果相关的接口没有实现，则上层调用将返回出错。

#### 使用说明

相关API或者数据结构请参考mqtt_al.h文件。如果您有更好的提议烦请反馈。