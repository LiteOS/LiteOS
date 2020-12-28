# SENSORHUB说明文档
Sensorhub智能传感集线器的主要功能是连接并处理来自各种传感器设备的数据， Sensorhub Demo展示了其使用示例。   
本文使用`Cloud_STM32F429IGTx_FIRE开发板`和内置的`mpu6050加速陀螺仪`介绍LiteOS的sensorhub的使用。

## Sensorhub Demo 配置及简介
### menuconfig配置
通过`make menuconfig`开启sensorhub组件及demo编译选项
```c
    Targets  --->
        Target (Cloud_STM32F429IGTx_FIRE)  --->
                                           (X) Cloud_STM32F429IGTx_FIRE
    Components --> 
               Sensorhub --> 
                         [*] Enable Sensorhub
    Demos --> 
          Sensorhub Demo --> 
                           *** only support Cloud_STM32F429IGTx_FIRE ***
                         [*] Enable Sensorhub Demo

```
### Sensorhub Demo代码简介
开启Sensorhub Demo示例功能入口在 `targets/bsp/common/demo_entry.c`内。
```c
#ifdef LOSCFG_DEMOS_SENSORHUB
    UINT32 ret = MiscInit();
    if (ret != LOS_OK) {
        PRINT_ERR("Sensorhub Demo Task Creat Fail.\n");
        return ret;
    }
#endif
```
Sensorhub Demo的实现功能在`demos\sensorhub\gyro\src\sensorhub_demo.c`中。
```c
STATIC VOID MiscTask(VOID const * arg)
{
    (VOID)(arg);
    MX_I2C1_Init();
    SensorManagerInit();
    LOS_TaskDelay(1000);
    GyroSensorRegister();
    InitGyro();
    OpenGyro();
    LOS_TaskDelay(20000);
    CloseGyro();
    OpenGyro();
    LOS_TaskDelay(100000);
    CloseGyro();
}

UINT32 MiscInit(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam = {0};

    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)MiscTask;
    taskInitParam.uwStackSize = STASK_STKDEPTH_MISC;
    taskInitParam.pcName = "Misc Task";
    taskInitParam.usTaskPrio = STASK_PRIORITY_MISC;    /* 1~7 */
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED; /* task is detached, the task can deleteself */

    ret = LOS_TaskCreate(&g_miscTskID, &taskInitParam);
    if (ret != LOS_OK) {
        PRINT_ERR("Misc Task create fail\n");
        return ret;
    }
    PRINT_DEBUG("MiscTask init \n");
    return ret;
}
```
以上所述是调用ensorhub Demo示例的主要内容，参照上述menuconfig配置之后，经编译后，固件为`out/Cloud_STM32F429IGTx_FIRE/Huawei_LiteOS.bin`。 
## Sensorhub Demo测试
把编译生成的固件烧录到开发板内， 串口输出mpu6050传感器实时数据，运行效果如下所示：   
```
    ********Hello Huawei LiteOS********

    LiteOS Kernel Version : 5.0.0
    build data : Dec 25 2020 21:55:13

    **********************************
    osAppInit
    cpu 0 entering scheduler
    app init!

    Huawei LiteOS # Get mpu6050 id = 104
    gyro on.
    read data
    Acc:       532     296   15394
    Gyro:      -58     -26     -11
    temp:       28
    tag 2 report
    tag 2 report
    read data
    Acc:       488     282   15410
    Gyro:      -58     -26     -11
    temp:       28
    tag 2 report
    tag 2 report
    read data
    Acc:       510     292   15390
    Gyro:      -57     -26     -11
    temp:       28
    tag 2 report
    read data
    Acc:       492     292   15386
    Gyro:      -58     -27     -11
    temp:       28
    tag 2 report
    tag 2 report
    read data
    Acc:       506     278   15386
    Gyro:      -58     -26     -11
    temp:       28
    tag 2 report
    ```

