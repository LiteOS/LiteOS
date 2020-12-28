# GUI Demo说明文档
该GUI示例使用的是LVGL图形库， 该图形库控件丰富， 移植方便， 本示例使用`STM32F769I-DISCO`开发板演示了LVGL图形库的功能和效果。

# GUI Demo配置及简介
## menuconfig 配置说明
```
Targets  --->
        Target (STM32F769IDISCOVERY)  --->
                                      (X) STM32F769IDISCOVERY
Components  --->
            Graphical User Interface  --->
                                        *** only support STM32F769IDISCOVERY ***
                                      [*] Enable Gui (NEW)
Demos  --->
       Graphical User Interface Demo  --->                 
                                          *** only support STM32F769IDISCOVERY ***
                                      [*] Enable Gui Demo (NEW)
```

## GUI Demo 代码简介
开启GUI Demo示例功能入口在 `targets/bsp/common/demo_entry.c`内。

```c
#ifdef LOSCFG_GUI_ENABLE
    ret = LvglDemo();
    if (ret != LOS_OK) {
        PRINT_ERR("Lvgl Demo Failed.\n");
        return ret;
    }
#endif
```
在`demos/gui/lvgl_demo.c`内创建上述`LvglDemo`任务， 用户可在`LvglTaskEntry`丰富lvgl的显示功能。

```c
STATIC VOID LvglTaskEntry(VOID)
{
    /* Enable the CPU Cache */
    EnableCache();

    lv_init();

    tft_init();
    touchpad_init();

#ifdef LV_USE_DEMO_WIDGETS
    lv_demo_widgets();
#endif

    while (1) {
        lv_task_handler();
        LOS_Msleep(LVGL_TASK_POLL_PERIOD);
    }
}

UINT32 LvglDemo(VOID)
{
    UINT32 ret;
    UINT32 taskId;
    TSK_INIT_PARAM_S lvglTask;

    (VOID)memset_s(&lvglTask, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    lvglTask.pfnTaskEntry = (TSK_ENTRY_FUNC)LvglTaskEntry;
    lvglTask.uwStackSize = LVGL_TASK_STACK_SIZE;
    lvglTask.pcName = "Lvgl_Task";
    lvglTask.usTaskPrio = LOSCFG_BASE_CORE_TSK_DEFAULT_PRIO;
    lvglTask.uwResved = LOS_TASK_STATUS_DETACHED;

    ret = LOS_TaskCreate(&taskId, &lvglTask);
    if (ret != LOS_OK) {
        return ret;
    }
    return ret;
}
```
`STM32F769IDISCOVERY`开发板的GUI Demo示例默认使能了`lvgl触控`功能, 其实现方法在 `targets/STM32F769IDISCOVERY/Drivers/touchpad/touchpad.c`中。代码展示如下：
```c
void touchpad_init(void)
{
    BSP_TS_Init(TFT_HOR_RES, TFT_VER_RES);

    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.read_cb = touchpad_read_cb;
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    lv_indev_drv_register(&indev_drv);
}
```
参照上述menuconfig配置之后，经编译后，固件为`out/STM32F769IDISCOVERY/Huawei_LiteOS.bin`。   

# GUI Demo 测试   
把编译生成的固件烧录到开发板内，在开发板屏幕上可以使用触控滑动并查看`LvglTaskEntry`内代码实现的效果。

