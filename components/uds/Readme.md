### 驱动框架使用说明
驱动应该是先注册，然后才能被使用。使用方法有两种：基于驱动层的API调用；基于文件系统的API调用。
驱动框架会建立一个DEVFS，将其注册到VFS中，供开发者使用标准的文件操作方法去操作设备。同时驱动框架还提供上层应用使用的API接口，方便开发者直接调用驱动。
下文就如何注册和使用驱动做详细描述。
#### 注册驱动
开发者可以使用两种方式注册驱动：手动注册（调用API注册）、静态添加（调用宏定义声明）；二者的效果是相同的，不同的是前者注册的时机由开发者把握，后者是操作系统初始化驱动架构后自动加载的。两种方式需要开发者实现或者提供的数据结构是相同的,主要的数据参数包括以下：驱动名字（const char *）、驱动IO方法(const los_driv_op_t *)、驱动私有数据(void *)、驱动打开方式(u32_t)。下面就其数据结构做简要说明。
1. 驱动名字

每个驱动都需要使用独一无二的名字，否则设备就会注册失败；
该名字由开发者提供存储空间；
该名字将会用作以后注销设备或者打开设备；如果使用文件系统模式打开设备，则需要在名字前缀/de/

2. 驱动IO接口

```
typedef void* los_driv_t ;//returned by the driver register
typedef bool_t (*fn_open)  (void *pri,s32_t flag);                                     
typedef s32_t  (*fn_read)  (void *pri,u32_t offset,u8_t *buf,s32_t len,u32_t timeout);
typedef s32_t  (*fn_write) (void *pri,u32_t offset,u8_t *buf,s32_t len,u32_t timeout);
typedef void   (*fn_close) (void *pri); 
typedef bool_t (*fn_ioctl) (void *pri,u32_t cmd, void *para,s32_t len);
typedef bool_t (*fn_init)  (void *pri);
typedef void   (*fn_deinit)(void *pri);
//all the member function of pri is inherited by the register function
typedef struct
{
    fn_open    open;   //triggered by the application
    fn_read    read;   //triggered by the application
    fn_write   write;  //triggered by the application
    fn_close   close;  //triggered by the application
    fn_ioctl   ioctl;  //triggered by the application
    fn_init    init;   //if first open,then will be called
    fn_deinit  deinit; //if the last close, then will be called
}los_driv_op_t
//the member could be NULL,depend on the device property
//attention that whether the device support multi read and write depend on the device itself
```
几个重要形参的说明：
.pri，该参数是在注册的时候由开发者提供的，表示该驱动特有的数据，如SOC的多个串口，使用同样的IO方法，用pri表示操作的不同的串口
.offset，仅仅对存储设备有效，普通设备的io实现可以忽略该参数
IO方法的说明：
.open:用于硬件自己的打开或者关闭；可以为空；如果提供，则返回true表示成功，否则false
.read:用于从硬件读取数据，buf由调用者提供，len表示buf的长度，同时表示想要读取的数据量；如果是存储设 备，offset表示读取的数据偏移量；timeout表示没有数据时，最大的阻塞等待时间，单位（毫秒）;pri表示驱动设备的私有数据
.write:用于从硬件读取数据，buf由调用者提供，len表示buf的长度，同时表示想要读取的数据量；如果是存储设备，offset表示写入的数据偏移量；timeout表示没有数据时，最大的阻塞等待时间，单位（毫秒）;pri表示驱动设备的私有数据
.close：关闭设备
.ioctl:设备的控制接口，其cmd和para由设备自己定义
.关于init和deinit，设备驱动在使用之前可能需要初始化硬件，在设备被第一次打开的时候，则调用init初始化设备，否则不初始化 

3. 设备私有数据

该数据在驱动注册时提供；当上层应用调用驱动的IO时，会将该数据传给驱动设备的IO接口；驱动开发者可以使用该数据结构共享驱动方法；

4. 驱动的打开方式

目前没有具体实现；后续版本使用其用于控制设备的打开方式。
当准备好驱动名字、方法、私有数据、打开方式之后，就可以注册设备驱动了。

#####  手动注册

```
//these interface called by the bsp develop
los_driv_t los_driv_register(const char *name, const los_driv_op_t *op,void *pri,u32_t flagmask);
```

需要注意的是该方法只能在驱动架构初始化完毕之后才能调用
#####  静态定义
```
OSDRIV_EXPORT(varname,drivname,operate,pridata,flagmask)
```
#### 驱动使用

注册设备后，就可以使用设备驱动了。

驱动层的API接口如下：

```
//these interface by the application
typedef void* los_dev_t ;                   //this type is returned by the dev open
los_dev_t  los_dev_open  (const char *name,u32_t flag);
s32_t      los_dev_read  (los_dev_t dev,u32_t offset,u8_t *buf,s32_t len,u32_t timeout);
s32_t      los_dev_write (los_dev_t dev,u32_t offset,u8_t *buf,s32_t len,u32_t timeout);
bool_t     los_dev_close (los_dev_t dev);
bool_t     los_dev_ioctl (los_dev_t dev,u32_t cmd,void *para,s32_t paralen);
```

当然，如果使能了DEVFS和VFS的话，则可以使用文件系统的api直接调用；需要注意的是如果使用文件系统接口，则传入的设备名字必须前缀/dev/，因为所有的设备驱动都在/dev/目录下。

#### 重要的配置选项

1. 如果使能驱动框架，则需要在target_config.h中配置LOSCFG_ENABLE_DRIVER
2. 如果使能设备文件系统，则需要在target_config.h中配置LOSCFG_ENABLE_DEVFS，设备文件系统本身依赖VFS；同时如果需要通过设备文件系统操作设备，则需要配置驱动框架
3. 包含路径：驱动的注册和使用需要包含los_dev.h，需要在编译器的PATH中添加component/driver目录

