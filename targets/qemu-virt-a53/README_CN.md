# CORTEX A53在QEMU下运行LiteOS
## 安装QEMU
- QEMU是一个开源工具，有关QEMU的介绍可参考：<a href="https://github.com/qemu/qemu" target="_blank">https://github.com/qemu/qemu</a>。

- 以Ubuntu Linux环境为例，如果未安装QEMU工具，可使用如下命令下载安装:
    ```
    $ apt-get install qemu
    $ apt-get install qemu-system
    ```
    由于cortex A53为64位，所以还需要安装64位的QEMU。
    ```
    $ apt-get install qemu-system-aarch64
    ```

## Linux环境下以多核编译VIRT A53

### 部署编译环境
编译VIRT A53需要安装64位编译器，下面以Ubuntu环境为例，按照如下方式安装64位的ARM linux的交叉编译工具`aarch64-linux-gnu-gcc`。
```
$ apt-get install gcc-5-aarch64-linux-gnu
$ apt-get install gcc-aarch64-linux-gnu
```

### 编译运行
- 修改编译需要的`.config`文件为VIRT A53的默认配置文件：

    ```
    $ cp tools/build/config/qemu-virt-a53.config .config
    ```
- 使用`make`编译，默认配置开启了Kernel SMP：

    ```
    $ make clean; make -j
    ```

- 通过QEMU启动guest虚拟机运行LiteOS：

    ```
    $ qemu-system-aarch64 -machine virt -smp 4 -m 512M -cpu cortex-a53 -kernel out/qemu-virt-a53/Huawei_LiteOS.elf -nographic
    ```
    qemu命令行参数含义可以通过qemu-system-arm --help查看，以上命令中的参数含义如下：
    |参数|说明|
    |:---:|---|
    |-machine|表示qemu要仿真的虚拟机类型|
    |-smp|表示guest虚拟机的cpu的个数|
    |-m|为此guest虚拟机预留的内存大小，如果不指定，默认大小是128M|
    |-cpu|表示要仿真的虚拟机的芯片架构|
    |-kernel|表示要运行的镜像文件路径|
    |-nographic|表示启动的是非图形界面|

- 开启Kernel SMP后LiteOS启动打印信息如下所示：

    ```
    ********Hello Huawei LiteOS********

    Processor   : Cortex-A53 * 4
    Run Mode    : SMP
    GIC Rev     : unknown
    build time  : Dec  5 2020 02:50:54

    **********************************

    main core booting up...
    osAppInit
    releasing 3 secondary cores
    cpu 0 entering scheduler
    app_init
    cpu 1 entering scheduler
    cpu 2 entering scheduler
    cpu 3 entering scheduler

    Huawei LiteOS #
    ```

## Linux环境下以单核编译VIRT A53

- 默认的配置开启了Kernel SMP，如需关闭SMP，可以在`make menuconfig`中关闭`LOSCFG_KERNEL_SMP`，其配置如下：
    ```
    Kernel --> Enable Kernel SMP
    ```
- 编译后使用如下命令运行QEMU：
    ```
    $ qemu-system-aarch64 -machine virt -kernel -cpu cortex-a53 out/qemu-virt-a53/Huawei_LiteOS.elf -nographic
    ```
- 关闭Kernel SMP后LiteOS启动打印信息如下所示：
    ```
    ********Hello Huawei LiteOS********

    Processor   : Cortex-A53
    Run Mode    : UP
    GIC Rev     : unknown
    build time  : Dec  1 2020 04:26:08

    **********************************

    main core booting up...
    osAppInit
    cpu 0 entering scheduler

    Huawei LiteOS #
    ```