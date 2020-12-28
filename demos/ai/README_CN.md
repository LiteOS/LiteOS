## AI代码生成及使能教程

&emsp;&emsp;阅读本教程前请先查看链接：`https://liteos.gitee.io/liteos_studio/#/introduction`中`AI模型代码生成向导界面介绍`部分的内容。

&emsp;&emsp;目前`AI代码生成`LiteOS提供mnist、mobilenet、resnet三种模型，三种模型均可导出成算子类型为FP32的推理代码(即demo)，其中mnist模型的量化类型支持非量化和训练后量化，mobilenet和resent模型的量化类型支持非量化类型。由于受开发板RAM大小限制，部分开发板无法运行或无法同时运行以上三种模型生成的推理代码，目前Cloud_STM32F429IGTx_FIRE仅能运行mnist训练后量化生成的推理代码，STM32F769IDISCOVERY均能运行(只支持单独运行，无法同时运行三种模型)，另外可在realview-pbx-a9上运用qemu进行模拟运行(可同时运行)。

&emsp;&emsp;后续将陆续增加支持的模型种类数、算子种类数(int8)、量化类型数，并且将针对不同的平台(arm64、arm32、cortex-m)提供高性能的算子。

&emsp;&emsp;本教程将以使用LiteOS官方提供的模型文件`mnist.tflite`，生成量化类型为`训练后量化(PostTraining)`的推理代码，在Cloud_STM32F429IGTx_FIRE开发板上运行为例演示AI代码生成、使能及运行的过程。

#### 相关说明

  * 1. `量化类型`和`配置文件`是对应的，有量化类型才有配置文件，非量化(量化类型为空)则不需要选择配置文件。

  * 2. LiteOS提供的，mnist、mobilenet、resnet三种模型的对应的模型文件`mnist.tflite`、`mobilenet.tflite`、`resnet.tflite`位于`https://gitee.com/LiteOS/LiteOS_Studio/tree/master/tools/models`下，请自行下载。mnist训练后量化的配置文件`config.mnist`及校准数据集`mnist_image`位于此目录下的quant_files目录。`config.mnist`文件中的`image_path`参数为数据集mnist_image的路径，使用时请根据实际情况修改。

  * 3. 使用`mnist.tflite`模型文件`非量化`类型生成的推理代码将位于LiteOS工程源码的demos/ai/mnist目录下。

  * 4. 使用`mnist.tflite`模型文件`训练后量化`类型生成的推理代码将位于LiteOS工程源码的demos/ai/mnist_quant目录下。

  * 5. 使用`mobilenet.tflite`模型文件`非量化`类型生成的推理代码将位于LiteOS工程源码的demos/ai/mobilenet目录下。

  * 6. 使用`resnet.tflite`模型文件`非量化`类型生成的推理代码将位于LiteOS工程源码的demos/ai/resnet目录下。

#### mnist训练后量化的推理代码生成

  1. 使用Huawei_Liteos-Studio打开Huawei_LiteOS工程源码
  2. 在Huawei_Liteos-Studio左菜单栏中点击`LiteOS Studio`选项。
  3. 在弹出的窗口中点击`AI代码生成`选项。
  4. 在`Msmicro目录`的选项中选择`msmicro.exe`。
  5. 在`框架类型`选项中选择`TFLITE`。
  6. 在`AI模型文件`选项中选择模型文件`mnist.tflite`。
  7. 在`量化类型`选项中选择`PostTraining`。
  8. 在`配置文件`选项中选择量化配置文件`config.mnist`。
  9. 点击确定，即可生成mnist训练后量化的推理代码。

  * 注：如需生成mnist、mobilenet、resnet非量化类型的推理代码, 更换第6步中的模型文件(mnist的训练后量化和非量化使用同一模型文件)，第7步中`量化类型`选项选择空，忽略第8步即可。

#### 使能ai demo
  1. 在`目标板`选项下选择目标板 STM32F429IG，点击确定。
  2. 在`组件配置`选项下选择 `Huawei LiteOS Configuration -> Demos -> Ai Demo-> Enable Mnist Quant Modle`，点击确定。
  * 注：编译器、烧录器及其他配置按正常配置即可，组件配置时只能使能已生成推理代码的模型。

&emsp;&emsp;完成以上操作后编译LiteOS源码，生成系统镜像文件Huawei_LiteOS.bin，并将.bin文件烧写到开发板，复位重启开发板后，demo即启动。

#### 通过串口工具查看日志
```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.0.0
build data : Dec 21 2020 15:05:54

**********************************
osAppInit
cpu 0 entering scheduler
app init!
mnist_quant inference Start.
0.000000,0.000000,0.000001,0.999740,0.000000,0.000259,0.000000,0.000000,0.000000, 0.000000

start = 4776216, end = 106817405, consume time = 102041189 ns
mnist_quant inference End.

Huawei LiteOS # 
```

* 更多关于MindSpore的教程，请参考：https://www.mindspore.cn/tutorial/training/zh-CN/master/quick_start/quick_start.html
