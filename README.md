![](http://developer.huawei.com/ict/sites/default/files/ueditor/26/upload/image/20160827/1472287221569855.png)
##Huawei LiteOS基础内核
华为物联网操作系统Huawei LiteOS是华为面向物联网领域开发的一个基于实时内核的轻量级操作系统。本项目属于华为物联网操作系统[Huawei LiteOS](http://developer.huawei.com/ict/cn/site-iot/product/liteos)基础内核源码(图示Basic Kernel)，目前仅适配STM32F411开发板，后续会支持其他类型开发板。现有代码支持任务调度，内存管理，中断机制，队列管理，事件管理，IPC机制，时间管理，软定时器以及双向链表等常用数据结构。开发指南可访问[华为开发者社区](http://developer.huawei.com/ict/cn/site-iot/product/liteos)下载。

##项目更新
* 2016.9.6 - 支持IAR工程构建 

##开发板支持
* STM32F411

##主要特征
* 实时操作系统内核
* 轻量级
* 低功耗
* 快速启动
* 可裁剪
* 分散加载

##内核模块
* core
* include
* ipc
* mem
* misc

##代码导读
* Huawei\_LiteOS - 包含Huawei_LiteOS操作系统的基础内核源码以及支持平台所需移植文件。
* Projects - 包含IAR或Keil等IDE的工程构建文件，目前只支持IAR的工程构建文件。

##新手入门
以下代码为创建不同优先级任务示例代码：

    TSK_HANDLE_T  g_uwTskLoID;
    TSK_HANDLE_T g_uwTskHiID;
    #define TSK_PRIOR_HI 4
    #define TSK_PRIOR_LO 5

    /*高优先级任务入口函数*/
    UINT32 Example_TaskHi()
    {
        ...
    }

    /*低优先级任务入口函数*/
    UINT32 Example_TaskLo()
    {
        ...
    }

    /*任务测试入口函数，在里面创建优先级不一样的两个任务*/
    UINT32 Example_TskCaseEntry(VOID)
    {
        UINT32 uwRet;
        /*任务初始化结构体*/
        TSK_INIT_PARAM_S stInitParam;
    
        /*锁任务调度，可以在Lock与UnLock区间内创建具体任务*/
        LOS_TaskLock();
    
        printf("LOS_TaskLock() Success!\r\n");
    
        /*高优先级任务创建参数*/
        stInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_TaskHi;
        stInitParam.usTaskPrio = TSK_PRIOR_HI;
        stInitParam.pcName = "HIGH_NAME";
        stInitParam.uwStackSize = 0x400;
        stInitParam.uwResved   = LOS_TASK_STATUS_DETACHED;
        /*创建高优先级任务，由于锁任务调度，任务创建成功后不会马上执行*/
        uwRet = LOS_TaskCreate(&g_uwTskHiID, &stInitParam);
        if (uwRet != LOS_OK)
        {
            LOS_TaskUnlock();
     
            printf("Example_TaskHi create Failed!\r\n");
            return LOS_NOK;
        }
    
        printf("Example_TaskHi create Success!\r\n");
    
        /*低优先级任务创建参数*/
        stInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_TaskLo;
        stInitParam.usTaskPrio = TSK_PRIOR_LO;
        stInitParam.pcName = "LOW_NAME";
        stInitParam.uwStackSize = 0x400;
        stInitParam.uwResved   = LOS_TASK_STATUS_DETACHED;
        /*创建低优先级任务，由于锁任务调度，任务创建成功后不会马上执行*/
        uwRet = LOS_TaskCreate(&g_uwTskLoID, &stInitParam);
        if (uwRet != LOS_OK)
        {
            LOS_TaskUnlock();
    
            printf("Example_TaskLo create Failed!\r\n");
            return LOS_NOK;
        }
    
        printf("Example_TaskLo create Success!\r\n");
    
        /*解锁任务调度，此时会发生任务调度，执行就绪列表中最高优先级任务*/
        LOS_TaskUnlock();
    
        while(1){};
    
        return LOS_OK;
    }

##代码贡献
* 目前为只读

##开源协议
* 遵循BSD-3开源许可协议