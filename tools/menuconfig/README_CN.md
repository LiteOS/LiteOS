## 概述

Huawei LiteOS使用Kconfig文件配置系统。所用的Kconfig语言是一种菜单配置语言，config.in和Kconfig都由该语言编写而成。Huawei LiteOS使用python kconfiglib来解析、展示Kconfig文件。解析Kconfig文件后，会在根目录下生成/更新.config文件，同时在开发板的include文件夹下生成menuconfig.h。使用menuconfig前，需要先安装python和kconfiglib。

## 安装工具

需要安装的工具包括python、pip和kconfiglib。

1.  确保服务器已安装python，python 2.7/3.2+均支持。

    如果服务器没有安装python，根据官网<a href="https://docs.python.org/2.7/using/unix.html#on-linux" target="_blank">python 2.7 安装文档</a>或<a href="https://docs.python.org/3.8/using/unix.html#on-linux" target="_blank">python 3.8 安装文档</a>，完成安装。

2.  确保服务器已安装pip。

    如果pip命令不存在，可以下载源码包进行安装。pip依赖setuptools，如果setuptools不存在，也需要安装。

    -   setuptools安装。

        点击<a href="https://pypi.org/project/setuptools/setuptools" target="_blank">setuptools源代码包下载地址</a>，可以参考下面的命令进行安装：

        ```
        sudo unzip setuptools-50.3.2.zip
        cd setuptools
        sudo python setup.py install
        ```

        **须知：** setuptools最新版本不支持python 2.7，请下载<a href="https://pypi.org/project/setuptools/45.0.0/#files" target="_blank">setuptools 45.0.0版本</a>以支持python 2.7。

    -   pip安装。

        点击<a href="https://pypi.org/project/pip/" target="_blank">pip源代码包下载地址</a>，可以参考下面的命令进行安装：

        ```
        sudo tar -zxvf pip-20.2.4.tar.gz
        cd pip-20.2.4
        sudo python setup.py install
        ```

3.  安装kconfiglib。
    -   对于服务器可以联网的情况。

        可以直接使用如下命令安装kconfiglib：

        ```
        sudo pip install kconfiglib
        ```

    -   对于服务器不能联网的情况。

        可以采用离线的方式安装。首先在其他能联网的环境上<a href="https://pypi.org/project/kconfiglib/#files" target="_blank">下载kconfiglib</a>，当前Kconfiglib的版本号为14.1.0，可以下载kconfiglib的wheel文件kconfiglib-14.1.0-py2.py3-none-any.whl或源代码文件kconfiglib-14.1.0.tar.gz。

        -   wheel文件的安装，可以参考如下命令：

            ```
            sudo pip install kconfiglib-14.1.0-py2.py3-none-any.whl
            ```

        -   源代码文件的安装，可以参考如下命令：

            ```
            sudo tar -zxvf kconfiglib-14.1.0.tar.gz
            cd kconfiglib-14.1.0
            sudo python setup.py install
            ```

## 使用指南

使用menuconfig配置工具前，请确保已经安装编译Huawei LiteOS源码的交叉编译器工具链，并加入环境变量。

为满足不同的使用场景，配置工具支持下述命令。根据使用场景，在根目录下执行下述其中一个命令即可。在执行命令前，先根据开发板拷贝tools/build/config/目录下的默认配置文件$\{platform\}.config到根目录，并重命名为.config。

### make menuconfig

在Huawei\_LiteOS根目录下执行make menuconfig命令会展示图形化配置界面，用户根据需要对系统进行配置即可。

menuconfig的使用方式，主要包括：

-   上下键：选择不同的行，即移动到不同的选项上。
-   空格键/回车：用于开启或关闭选项。
    1.  开启选项：对应的选项前面会显示“ \[ \* \]“，括号里面有一个星号，表示已经开启该选项。
    2.  关闭选项：对应的选项前面只显示中括号“\[  \]”, 括号里面是空。
    3.  如果选项后面有三个短横线加上一个右箭头，即---\>，表示此项下面还有子选项，输入空格键/回车后可以进入子菜单。

-   ESC键：返回上一级菜单，或退出menuconfig并提示保存。
-   问号? ：展示配置项的帮助信息。
-   斜线/ :  进入搜索配置项界面，支持配置项的搜索。
-   字母F：进入帮助模式，在界面下方会显示配置项的帮助信息，再次输入字母F可以退出此模式。
-   字母C：进入name模式，在此模式下，会显示配置项对应的宏定义开关，再次输入字母C可以退出此模式。
-   字母A：进入all模式，在此模式下，会展开显示菜单中的所有子选项，再次输入字母A可以退出此模式。
-   字母S : 保存配置项。
-   字母Q：退出menuconfig并提示保存。

**说明：** 以上字母，不区分大小写。

### make savemenuconfig

在Huawei\_LiteOS根目录下执行make savemenuconfig命令会解析根目录下的.config文件，并在对应的Huawei\_LiteOS开发板工程的子目录include下生成menuconfig. h文件。

### make defconfig

在Huawei\_LiteOS根目录下执行make defconfig命令会解析根目录下的.config文件，使所有的配置项尽可能使用其默认配置，并更新.config，同时在对应的Huawei\_LiteOS开发板工程的子目录include下生成menuconfig.h。

### make allyesconfig

在Huawei\_LiteOS根目录下执行make allyesconfig命令会解析根目录下的.config，使所有的配置项尽可能使能，即设置为Y，并更新.config，同时在对应的Huawei\_LiteOS开发板工程的子目录include下生成menuconfig.h。

### make allnoconfig

在Huawei\_LiteOS根目录下执行make allnoconfig命令会解析根目录下的.config，使所有的配置项尽可能禁用，即设置为is not set，并更新.config，同时在对应的Huawei\_LiteOS开发板工程的子目录include下生成menuconfig.h。