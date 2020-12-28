# 文件系统
本文介绍LiteOS中的文件系统及其例程。
## VFS
VFS是Virtual File System（虚拟文件系统）的缩写，是LiteOS文件系统的统一接口，其他文件系统基于VFS提供的接口实现。

### VFS提供的接口

#### 初始化VFS
```c
int los_vfs_init (void);
```
-  返回值：

    **LOS_OK**：成功

    **LOS_NOK**：失败

#### 注册文件系统
```c
int los_fs_register(struct file_system *);
```
-  file_system：对应注册文件系统的结构体指针
   ```c
   struct file_system {
       const char            fs_name [LOS_FS_MAX_NAME_LEN];
       struct file_ops      *fs_fops;
       struct file_system   *fs_next;
       volatile uint32_t     fs_refs;
   };
   ```
-  返回值：

    **LOS_OK**：成功

    **LOS_NOK**：失败

#### 注销文件系统
```c
int los_fs_unregister(struct file_system *);
```
-  file_system：对应注册文件系统的结构体指针
   ```c
   struct file_system {
       const char            fs_name [LOS_FS_MAX_NAME_LEN];
       struct file_ops      *fs_fops;
       struct file_system   *fs_next;
       volatile uint32_t     fs_refs;
   };
   ```
-  返回值：

    **LOS_OK**：成功

    **LOS_NOK**：失败

#### 挂载文件系统
```c
int los_fs_mount(const char *fsname, const char *path, void *data);
```
-  fsname：文件系统名称
-  path：文件系统的挂载路径
-  data：挂载的文件系统指针
-  返回值：

    **LOS_OK**：成功

    **LOS_NOK**：失败

#### 卸载文件系统
```c
int los_fs_unmount(const char *path);
```
-  path：文件系统的挂载路径
-  返回值：

    **LOS_OK**：成功

    **LOS_NOK**：失败

#### 文件操作的接口

```c
int             los_open(const char *, int);
int             los_close(int);
ssize_t         los_read(int, char *, size_t);
ssize_t         los_write(int, const void *, size_t);
off_t           los_lseek(int, off_t, int);
int             los_stat(const char *, struct stat *);
int             los_unlink(const char *);
int             los_rename(const char *, const char *);
int             los_ioctl(int, int, ...);
dir             *los_opendir(const char *path);
struct dirent   *los_readdir(struct dir *dir);
int             los_closedir(struct dir *dir);
int             los_mkdir(const char *path, int mode);
int             los_sync(int fd);
```

## RAMFS
RAMFS是基于内存的文件系统。在RAMFS中，文件存储在内存中，所有的读/写操作均发生在内存中，由于访问内存比存储器快，从而可以提升文件读写效率。一般可用RAMFS来存储临时文件或者修改频繁的数据。LiteOS的RAMFS基于虚拟文件系统层（VFS)，不能格式化。

### RAMFS提供的接口

-  初始化RAMFS

   ```c
   int ramfs_init(void);
   ```
-  返回值：

    **LOS_OK**：成功

    **LOS_NOK**：失败

-  挂载RAMFS
   ```c
   int ramfs_mount(const char *path, size_t block_size);
   ```
-  path：RAMFS的挂载路径
-  block_size：RAMFS挂载的块大小
-  返回值：

    **LOS_OK**：成功

    **LOS_NOK**：失败

### 注意事项

-  RAMFS文件系统支持的操作有：`los_open`，`los_close`，`los_read`，`los_write`，`los_lseek`，`los_stat`，`los_ulink`，`los_rename`，`los_opendir`，`los_readdir`，`los_closedir`，`los_mkdir`。
-  RAMFS只能挂载一次，挂载成功后，后面不能继续挂载到其他目录。
-  RAMFS需要根据设备内存大小来调整挂载的大小，如超出系统剩余内存，会挂载失败。
-  RAMFS需使能文件系统和RAMFS：
   ```
   Components --> FileSystem --> Enable FileSystem
   Components --> FileSystem --> Enable RAMFS
   ```

## SPIFFS

SPIFFS是基于spi flash的嵌入式文件系统，它不需要设备具有很大内存，只需要一定的RAM作为缓存区，这让它可以适用在很多地方。

### SPIFFS提供的接口

#### 初始化SPIFFS

```c
int spiffs_init (void);
```
-  返回值：

    **LOS_OK**：成功

    **LOS_NOK**：失败

#### 挂载SPIFFS

```c
int spiffs_mount(const char *path, u32_t phys_addr, u32_t phys_size,
                  u32_t phys_erase_block, u32_t log_block_size,
                  u32_t log_page_size,
                  s32_t (*spi_rd) (struct spiffs_t *, u32_t, u32_t, u8_t *),
                  s32_t (*spi_wr) (struct spiffs_t *, u32_t, u32_t, u8_t *),
                  s32_t (*spi_er) (struct spiffs_t *, u32_t, u32_t));
```
-  path：SPIFFS的挂载路径
-  phys_addr：spi flash的物理地址
-  phys_size：spi flash的存储空间大小
-  phys_erase_block：spi flash擦除块大小
-  log_block_size：log块大小
-  log_page_size：log页大小
-  spi_rd：spi flash读函数指针
-  spi_wr：spi flash写函数指针
-  spi_er：spi flash擦除函数指针
-  返回值：

    **LOS_OK**：成功

    **LOS_NOK**：失败
#### 卸载SPIFFS
```c
int spiffs_unmount(const char *path);
```
-  path：SPIFFS的挂载路径
-  返回值：

    **LOS_OK**：成功

    **LOS_NOK**：失败

### 注意事项

-  SPIFFS文件系统支持的操作有：`los_open`，`los_close`，`los_read`，`los_write`，`los_lseek`，`los_stat`，`los_ulink`，`los_rename`，`los_sync`，`los_opendir`，`los_readdir`，`los_closedir`。
-  SPIFFS文件系统目前仅支持STM32F429开发板。
-  SPIFFS需使能文件系统和SPIFFS：
   ```
   Components --> FileSystem --> Enable FileSystem
   Components --> FileSystem --> Enable SPIFFS
   ```

## FATFS
FAT（File Allocation Table，即文件分配表）文件系统，是一种在Windows操作系统中常用的文件系统。FAT文件系统支持多种介质，在可移动存储介质（U盘、SD卡、移动硬盘等）上广泛使用。它可以使嵌入式设备和Windows、Linux等桌面操作系统保持良好的兼容性，方便用户管理和操作文件。

### FATFS提供的接口

#### 初始化FATFS
```c
int fatfs_init(void);
```
-  返回值：

    **LOS_OK**：成功

    **LOS_NOK**：失败

#### 挂载FATFS
```c
int fatfs_mount(const char *path, struct diskio_drv *drv, uint8_t *drive);
```
-  path：FATFS的挂载路径
-  drv：FATFS的设备驱动结构体指针
   ```c
   struct diskio_drv {
       DSTATUS (*initialize)   (BYTE);                             /*!< Initialize Disk Drive  */
       DSTATUS (*status)       (BYTE);                             /*!< Get Disk Status        */
       DRESULT (*read)         (BYTE, BYTE *, DWORD, UINT);        /*!< Read Sector(s)         */
       DRESULT (*write)        (BYTE, const BYTE *, DWORD, UINT);  /*!< Write Sector(s)        */
       DRESULT (*ioctl)        (BYTE, BYTE, void *);               /*!< I/O control operation  */
   };
   ```
-  drive：注册FATFS的函数返回值，该返回值会在fatfs_mount函数内写入drive指向的内存
-  返回值：

    **LOS_OK**：成功

    **LOS_NOK**：失败

#### 卸载FATFS
```c
int fatfs_unmount(const char *path, uint8_t drive);
```
-  path：FATFS的挂载路径
-  drive：注册FATFS函数的返回值，通过fatfs_mount接口获取
-  返回值：

    **LOS_OK**：成功

    **LOS_NOK**：失败

### 注意事项

- FATFS文件系统支持的操作有：`los_open`，`los_close`，`los_read`，`los_write`，`los_lseek`，`los_stat`，`los_ulink`，`los_rename`，`los_sync`，`los_opendir`，`los_readdir`，`los_closedir`，`los_mkdir`。
- FATFS文件系统目前仅支持STM32F429和STM32F769开发板。
- FATFSFATFS需使能文件系统和FATFS：
   ```
   Components --> FileSystem --> Enable FileSystem
   Components --> FileSystem --> Enable FATFS
   ```

## 文件系统的使用

LiteOS的文件系统提供一套demo来演示文件系统的一些基本操作。

- 对于不同开发板，目前文件系统的支持情况如下：

  |开发板|RAMFS|FATFS|SPIFFS|
  |:---:|:---:|:---:|:---:|
  |STM32F429|支持|支持|支持|
  |STM32F769|支持|支持|不支持|
  |STM32L431|支持|不支持|不支持|

- 其中STM32F429的FAT文件系统支持两种介质，SPI Flash或SD Card，可以选择，默认为SD Card。
而STM32F769的FAT文件系统目前暂时只支持SD Card。

- SD Card为介质时，需要在开发板上插入SD卡，否则会挂载失败。

- 在STM32F429上，三种文件系统可同时挂载。在STM32F769上，也可同时挂载RAMFS和FATFS。

- 通过menuconfig使能demo后，编译LiteOS源码，生成系统镜像文件Huawei_LiteOS.bin，并将.bin文件烧写到开发板，复位重启开发板后，demo即启动。

下面介绍各个demo及其使能方法。

### fs_common

<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/fs/fs_common.c" target="_blank">fs_common.c</a>和<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/fs/fs_common.h" target="_blank">fs_common.h</a>文件，封装了一层文件操作函数，方便使用。
#### 写文件
```c
int  write_file(const char *name, char *buff, int len);
```
通过`O_CREAT | O_WRONLY | O_TRUNC`方式`open`，并调用`write`和`close`。

-  name：文件名
-  buff：写入文件的数据
-  len：写入文件的数据长度
-  返回值：

    **LOS_OK**：成功

    **LOS_NOK**：失败

#### 读文件
```c
int  read_file(const char *name, char *buff, int len);
```
通过`O_RDONLY`方式`open`，并调用`read`和`close`。
-  name：文件名
-  buff：读取到的文件数据
-  len：读取的数据长度
-  返回值：

    **LOS_OK**：成功

    **LOS_NOK**：失败

#### 打开目录
```c
int  open_dir(const char *name, struct dir **dir);
```
打开一个目录，并把目录结构传给dir结构体指针，如果目录不存在调用mkdir创建目录。
-  name：要打开的目录名称
-  dir：返回的目录结构体指针
   ```c
   struct dir {
       struct mount_point * d_mp;      /* can get private mount data here */
       struct dirent        d_dent;
       off_t                d_offset;
       void               * d_data;
   };
   ```
-  返回值：

    **LOS_OK**：成功

    **LOS_NOK**：失败

#### 读目录
```c
int  read_dir(const char *name, struct dir *dir);
```
读取目录下的所有文件，并打印出来。

-  name：要读取的目录名称
-  dir：要读取的目录结构体指针
-  返回值：

    **LOS_OK**：成功

    **LOS_NOK**：失败

#### 新建目录
```c
void make_dir(const char *name);
```
新建一个目录。

-  name：新建的目录名称

#### 打印目录
```c
void print_dir(const char *name, int level);
```
打印目录下所有文件及目录，可输出不同的层级。

-  name：要打印的目录名称
-  level：打印的最大目录层级

#### demo接口
```c
void los_vfs_io(char *file_name, char *dir_name);
```
为demo提供的一个测试接口，内部调用了`write_file`，`read_file`，`open_dir`，`read_dir`等函数。
-  file_name：要操作的文件名
-  dir_name：要操作的目录名

### RAMFS demo

<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/fs/ramfs_demo.c" target="_blank">ramfs_demo.c</a>文件提供了一套ramfs的例程。

通过调用`ramfs_init`函数初始化ramfs，并挂载`/ramfs`目录，然后调用`fs_common`中的函数进行文件和目录的读写操作，最后调用`los_fs_unmount`卸载`/ramfs`目录。

#### 使能方法
通过`make menuconfig`打开配置，因为RAMFS demo依赖RAMFS，所以首先使能RAMFS，然后再使能RAMFS demo。
- 使能RAMFS
  ```
  Components --> FileSystem --> Enable FileSystem --> Enable RAMFS
  ```
- 使能RAMFS demo
  ```
  Demos --> FileSystem Demo --> Enable Ram FileSystem Demo
  ```

### FATFS demo

<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/fs/fatfs_demo.c" target="_blank">fatfs_demo.c</a>文件提供了一套fatfs的例程。

通过调用`hal_fatfs_init`初始化FATFS并挂载`/fatfs`目录，然后调用`los_vfs_io`函数进行文件和目录的读写操作，最后调用`fatfs_unmount`卸载`/fatfs`目录。

#### 使能方法
通过`make menuconfig`打开配置，因为FATFS demo依赖FATFS，所以首先使能FATFS，然后再使能FATFS demo。
- 使能FATFS
  ```
  Components --> FileSystem --> Enable FileSystem --> Enable FATFS
  ```
- 使能FATFS demo
  ```
  Demos --> FileSystem Demo --> Enable Fat FileSystem Demo
  ```

- 在STM32F429上可以选择使用SPI Flash或SD Card为介质
  ```
  Demos --> FileSystem Demo --> Enable Fat FileSystem Demo -->  Choose Fat Hardware Driver
  ```

### SPIFFS demo

<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/fs/spiffs_demo.c" target="_blank">spiffs_demo.c</a>文件提供了一套spiffs的例程。

通过调用`hal_spiffs_init`初始化SPIFFS并挂载`/spiffs`目录，然后调用`los_vfs_io`函数进行文件和目录的读写操作，最后调用`spiffs_unmount`卸载`/spiffs`目录。

#### 使能方法
通过`make menuconfig`打开配置，因为SPIFFS demo依赖SPIFFS，所以首先使能SPIFFS，然后再使能SPIFFS demo。
- 使能SPIFFS
  ```
  Components --> FileSystem --> Enable FileSystem --> Enable SPIFFS
  ```
- 使能SPIFFS demo
  ```
  Demos --> FileSystem Demo --> Enable Spiffs FileSystem Demo
  ```