### AT客户端框架说明

为了让AT框架和硬件做解耦，特优化或者说是更新以前的版本，让AT框架的本质回归到AT。

#### 编译配置

在使用at框架之前，需要系统的配置文件配置CONFIG_AT_ENABLE。

使用KEIL以及IAR以及GCC等，可以定义编译条件宏，如在makefile中定义：
```
gcc -D CONFIG_AT_ENABLE=1  xxx
```

如果使用make工具，则建议在makefile文件中直接包含AT组件目录下的at.mk即可

#### 组件安装
安装该组件必须调用los_at_init进行初始化，需要使用者提供相关的AT通道设备

```
/**
 * @brief: use this function to do the at client framwork initialized
 *
 * @return:0 success while -1 failed
 * */
int at_init(const char *devname);
```

为了保证AT框架和具体的硬件驱动解耦，此处使用的注册的设备驱动，开发者配置驱动设备的名字时。同时应该注意到，at设备接口的特点：开发者需要在意的是读取的一定是一帧AT返回数据，如果使用串口，可以用串口的IDLE中断来区别不同的帧；其他的设备接口也会有自己的相应说明该帧的区分形式（一般是几个字节的时间没有数据到达），当调用该函数之后，我们就可以使用at的接口发送AT命令了，同时我们提供了shell端口的atcmd命令用作调试（如果shell已经被配置的话）；

####  AT命令使用

```
/**
 * @brief:use this function to register a function that monitor the URC message
 * @param[in]:cmd, the command to send
 * @param[in]:cmdlen, the command length
 * @param[in]:index, the command index, if you don't need the response, set it to NULL; this must be a string
 * @param[in]:respbuf, if you need the response, you should supply the buffer
 * @param[in]:respbuflen,the respbuf length
 * @param[in]:timeout, the time you may wait for the response;and the unit is ms
 *
 * @return:0 success while -1 failed
 * */

int at_command(const void *cmd, size_t cmdlen,const char *index,\
                void *respbuf,size_t respbuflen,uint32_t timeout); 
```

####  AT上报数据监控

对于AT设备而言，除了我们发送命令其会返回数据外，其有可能会主动上报数据。为了处理该情况，提供该接口给用户使用监听。

```
typedef int (*fn_at_oob)(void *args,void *data,size_t datalen);
/**
 * @brief:use this function to register a function that monitor the URC message
 * @param[in]:name, which used for the at framework debug
 * @param[in]:inxdex, used for match the out of band data
 * @param[in]:length, index length, this is match length
 * @param[in]:func, supply the function that will execute when the index is matched
 * @paarm[in]:args, supply for the registered function
 *
 * @return:0 success while -1 failed
 * */
int at_oobregister(const char *name,const void *index,size_t len,fn_at_oob func,void *args);
```

用户注册该接口之后，当收到的数据命中index时，则会回调注册的func,同时将注册的arg参数传递给func;

有两点需要注意：
1，注册的OOB的匹配是从接收报文的头进行匹配的；

2，注册的OOB的index允许是非ascii字符串，因为要求注册时必须指定index的长度