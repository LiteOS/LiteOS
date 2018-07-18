Standard_msp430f5438a_taurus_v4.2支持IAR和TI官方的CCS（Code Composer Studio），这里介绍TI CCS下的工程创建过程。



# 安装TI CCS

TI CCS的安装比较简单，这里就不详细介绍了。



# 创建工程

## 新建workspace

打开TI CCS IDE，它会提示选择“Workspace”的目录，这里指定合适的目录，比如：

> C:\Users\your-username\ccs-wb

然后点击“Launch”。

这个时候会看到CCS的“Getting Started”页面。

## 新建CCS工程

1. 依次点击菜单栏“Project”->“New CCS Project...”

2. “Target”选择“MSP430x5xx Family”，然后在紧随的下拉列表里选择“MSP430F5438A”

3. “Project Name”填入“Huawei_LiteOS”

4. 去掉“Use default location”选项，在当前target目录下创建工程，比如：

   > D:\LiteOS\targets\Standard_msp430f5438a_taurus_v4.2\CCS

5. “Compiler version"选择TI的编译器，比如：`TI v18.1.2.LTS`

6. “Tool-chain”相关设置保持默认

7. “Project templates and examples”处选择“Empty Project”

8. 完成以上步骤后，点击最下方的“Finish”按钮

完成以上步骤后我们我们的工程就创建好了。

## 添加代码

1. 创建需要的目录右键我们的工程依次选择“New”->“Folder”，“Folder name:“处输入”arch“，然后点击”Advanced >>“选择”Link to alternate location (Linked Folder)“。然后输入MSP430的arch源代码目录路径，比如：

   > D:\LiteOS\arch\msp430\src

   然后点击最下方的"Finish"按钮

2. 完成以上步骤后就可以看到创建的目录了，然后展开”arch“目录，右键点击”los_dispatch_iar.s43“文件，然后点击”Exclude from Build“

3. 使用步骤1中的方法添加”kernel“目录，并”link“到kernel的目录（比如：`D:\LiteOS\kernel`）

4. 展开”kernel->base->mem“目录，右键点击”bestfit“目录，然后点击”Exclude from Build“，同样的方法去掉”tlsf“目录和”kernel->extended“目录

5. 使用类似前面几步的方法添加名为”src“的目录，并且link到”Standard_msp430f5438a_taurus_v4.2“下的“SRC”目录，比如：

   > D:\LiteOS\targets\Standard_msp430f5438a_taurus_v4.2\SRC

## 添加”include“目录

右键点击工程名，然后选择”Properites“。找到”Build“->”MSP430 Compiler“->”Include Options“，在弹出的对话框中依次添加如下头文件目录：

> ${PROJECT_ROOT}/../../../kernel/include
>
> ${PROJECT_ROOT}/../../../kernel/base/include
>
> ${PROJECT_ROOT}/../OS_CONFIG
>
> ${PROJECT_ROOT}/../../../arch/msp430/include
>
> ${PROJECT_ROOT}/../../../kernel/extended/include

最后点击”Apply and Close“

# 编译及下载

完成前面的步骤后编译工程以及连接仿真器下载调试了。在这里不再赘述。

# 注意事项

Huawei LiteOS的中断管理会接管所有的外部中断，请使用标准API注册中断处理函数，比如：

> LOS_HwiCreate (USCI_A0_VECTOR, 0, 0, uart_isr, 0);
>
> LOS_HwiCreate (TIMER1_A0_VECTOR, 0, 0, (void (*) (uintptr_t)) osTickHandler, 0);

`USCI_A0_VECTOR`，`TIMER1_A0_VECTOR`这些vector号的定义请查询IAR或CCS的SDK中的`msp430f5438a.h`。



目前Huawei LiteOS仅支持基本的设备：timer a用作系统时钟，异步中断输入输出的uart驱动支持。
