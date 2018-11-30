

#include "../Drivers/CSKY_CK802T_CSI_Driver/Inc/drv_wdt.h"
#include "stdio.h"
#include "../Drivers/CSKY_CK802T_CSI_Driver/Inc/soc.h"
#include "csky_All_Demo.h"
extern void mdelay(uint32_t ms);
#define WDT_PRINT_FLAG 0
#define WDT_TIMEOUT (0x10000 << 11) / (SYSTEM_CLOCK / 1000)

static void wdt_event_cb_fun(int32_t idx, wdt_event_e event)
{
    printf("\n[info]进入看门狗的回调函数\n");
}

static int32_t wdt_fun_feedog(wdt_handle_t wdt_handle)
{
    int32_t ret;
    //int i;
    ret = csi_wdt_start(wdt_handle);

    if(ret < 0) {
	printf("[error]csi_wdt_start error\n");
	return -1;
    }
    //for(i = 0; i < 100; i++)
	while(1)
	{
		mdelay(WDT_TIMEOUT - 3000);//如果发现看门狗喂狗失败，请写3000或更大,如果发现喂狗太快，其他的线程响应不及时，请写1000或者更小
		ret = csi_wdt_restart(wdt_handle); // 计时重启
		if(ret < 0) 
		{
			printf("[error]csi_wdt_restart error\n");
			return -1;
		}
		#if WDT_PRINT_FLAG
			printf("\n[汪]喂狗成功\n");
		#endif
    }
    //printf("\nwdt_fun_feedog OK \n");

    return 0;
}



static int32_t test_wdt(void)
{
    int32_t ret;
    wdt_handle_t wdt_handle = NULL;

    wdt_handle = csi_wdt_initialize(0, wdt_event_cb_fun); //初始化看门狗，指定回调函数
	printf("初始化看门狗\n");
    if(wdt_handle == NULL) {
	printf("csi_wdt_initialize error\n");
	return -1;
    }

    ret = csi_wdt_set_timeout(wdt_handle, WDT_TIMEOUT); //设置超时时间
	printf("设置看门狗超时时间\n");
    if(ret < 0) {
	printf("csi_wdt_set_timeout error\n");
	return -1;
    }
	#if WDT_PRINT_FLAG
	printf("\n开始喂狗工作！(喂狗有显示)\n");
	#else
	printf("\n开始喂狗工作！(喂狗无显示)\n");
	#endif
    ret = wdt_fun_feedog(wdt_handle);//暂时写了喂狗的死循环，
    if(ret < 0) {
	printf("wdt_fun_feedog error\n");
	return -1;
    }
	/*
	 * 经咨询中天微技术人员，了解到硬件决定，看门狗打开之后无法关闭。
	 * */
	printf("准备停止watchdog------------\n");
    ret = csi_wdt_stop(wdt_handle);
    if(ret < 0) {
	printf("csi_wdt_stop error\n");
	return -1;
    }
    ret = csi_wdt_uninitialize(wdt_handle);
    if(ret < 0) {
	printf("csi_wdt_uninitialize error\n");
	return -1;
    }

    return 0;
}

int example_wdt(void)
{
    int ret;
    ret = test_wdt();

    if(ret < 0) {
	printf("test_wdt failed\n");
	return -1;
    }

    printf("test_wdt successfully\n");
    return 0;
}

int task_WatchDog(void)
{
#if SHOW_PRECOMPILE_INFO
#pragma Message("非接管中断模式 --> WDT")
#endif
	HopeRun_printf("开始看门狗测试............\n");
    return example_wdt();
}
