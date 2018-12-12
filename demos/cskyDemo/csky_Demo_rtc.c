


#include "drv_rtc.h"
#include "stdio.h"
#include "soc.h"
#include "csi_core.h"
#include "csi_config.h"
#include "csky_All_Demo.h"
#include "los_hwi.h"	



static volatile uint8_t cb_rtc_flag;

extern void mdelay(uint32_t ms);

#define RTC_TIME_SECS 5

#define RTC_TIMEOUT2_SECS   10


#if LOSCFG_PLATFORM_HWI
static int32_t idx;
static rtc_event_e event;
static void rtc_event_cb_fun()
#else
static void rtc_event_cb_fun(int32_t idx, rtc_event_e event)
#endif
{
    if (event == RTC_EVENT_TIMER_INTRERRUPT) 
	{
        cb_rtc_flag = 1;
        printf("[info]进入RTC回调函数\n");
    }
}

static int32_t test_rtc_time(rtc_handle_t rtc_handle)
{
    struct tm current_time, last_time;
    int32_t ret;
	uint8_t ccc=4;
	
	//初始化RTC时间 2018-11-28 16：09 ：0
    current_time.tm_sec     = 00;
    current_time.tm_min     = 9;
    current_time.tm_hour    = 16;
    current_time.tm_mday    = 28;
    current_time.tm_mon     = 11;
    current_time.tm_year    = 118;//+1900
    ret = csi_rtc_set_time(rtc_handle, &current_time);
    if (ret < 0) 
	{
        printf("[error]设置RTC时间失败\n");
        return -1;
    }
	printf("[success]设置RTC时间成功\n");
	
	while(ccc--)
	{

		ret = csi_rtc_get_time(rtc_handle, &last_time);
		if (ret < 0) 
		{
			printf("[error]读取RTC时间失败\n");
			return -1;
		}
		last_time.tm_year = last_time.tm_year + 1900;//2018
		last_time.tm_mon = last_time.tm_mon;//
		printf("[info]The time is %d-%d-%d %d:%d:%d\n", last_time.tm_year, last_time.tm_mon, last_time.tm_mday, last_time.tm_hour, last_time.tm_min, last_time.tm_sec);
		
		printf("[ing]等待5秒。。。\n");
		//延迟5秒，再读取时间
		mdelay(RTC_TIME_SECS * 1000);//延迟5s
	}
    printf("[success]get rtc timer success\n");
    return 0;
}

static int32_t test_rtc_timeout(rtc_handle_t rtc_handle)
{
    int32_t ret;
    struct tm set_time, last_time;
    set_time.tm_sec     = 50;
    set_time.tm_min     = 9;
    set_time.tm_hour    = 16;
    set_time.tm_mday    = 28;
    set_time.tm_mon     = 11;
    set_time.tm_year    = 118;//+1900

    cb_rtc_flag = 0;

	//设置闹钟时间 2018-11-28 16：09 ：50
    ret = csi_rtc_set_alarm(rtc_handle, &set_time);

    if (ret < 0) {
        printf("[error]csi_rtc_set_alarm error\n");
        return -1;
    }
    printf("[info]设置闹钟时间 2018-11-28 16:09:50\n");

	//开启闹钟
    ret = csi_rtc_enable_alarm(rtc_handle, 1);
    if (ret < 0) {
        printf("[error]csi_rtc_enable_alarm error\n");
        return -1;
    }
	printf("[success]csi_rtc_enable_alarm success\n");
	
	//当前时间
	 csi_rtc_get_time(rtc_handle, &last_time);
	last_time.tm_year = last_time.tm_year + 1900;//2018
	last_time.tm_mon = last_time.tm_mon;//
	printf("[info]The time is %d-%d-%d %d:%d:%d\n", last_time.tm_year, last_time.tm_mon, last_time.tm_mday, last_time.tm_hour, last_time.tm_min, last_time.tm_sec);
	
	//等待闹钟到时
	while(!cb_rtc_flag)
	{
		printf(".");
		mdelay(1000);
	}

    if (cb_rtc_flag == 1) 
	{
        printf("[success]test rtc timeout function OK\n");

        ret = csi_rtc_get_time(rtc_handle, &last_time);

        if (ret < 0) {
            printf("[error]csi_rtc_get_time error\n");
            return -1;
        }
		last_time.tm_year = last_time.tm_year + 1900;//2018
		last_time.tm_mon = last_time.tm_mon;//
        printf("[info]The time is %d-%d-%d %d:%d:%d\n", last_time.tm_year, last_time.tm_mon, last_time.tm_mday, last_time.tm_hour, last_time.tm_min, last_time.tm_sec);
        
		//关闭闹钟
		ret = csi_rtc_enable_alarm(rtc_handle, 0);
        if (ret < 0) 
		{
            printf("[error]csi_rtc_disable_alarm error\n");
            return -1;
        }
		printf("[success]csi_rtc_disable_alarm success\n");
        return 0;
    } 
	else 
	{
        return -1;
    }

}



static int32_t test_rtc(void)
{
    int32_t ret;
    rtc_handle_t rtc_handle = NULL;
	//--------------------------------------
    rtc_handle = csi_rtc_initialize(0, rtc_event_cb_fun);
    if (rtc_handle == NULL) {
        printf("[error]初始化RTC失败\n");
        return -1;
    }
	printf("[success]初始化RTC成功\n");
	
#if 1
	ret = csi_rtc_start(rtc_handle);//返回值无任何意义，被写死为0

#if 0 //---------------该段不是rtc模块的状态，是rtc中断的状态-----------------------
	rtc_status_t rst=csi_rtc_get_status(rtc_handle);
	if(rst.active==1)
	{
			printf("[success]RTC正在运行\n");
			break;
	}
	else 
	{
			printf("[error]RTC未能运行\n");
			return -1;
	}
#endif //--------------------------------------
	
    ret = test_rtc_time(rtc_handle);
    if (ret < 0) {
        printf("[error]test_rtc_time error\n");
        return -1;
    }
	printf("[success]test_rtc_time success\n\n");
	
	
	//2 在1的基础上，设置闹钟
    ret = test_rtc_timeout(rtc_handle);

    if (ret < 0) {
        printf("test_rtc_timeout error\n");
        return -1;
    }
	printf("[success]test_rtc_timeout success\n");

	
    ret = csi_rtc_stop(rtc_handle);

    if (ret < 0) {
        printf("csi_rtc_stop error\n");
        return -1;
    }
	printf("[success]csi_rtc_stop success\n\n");
#endif
	
    ret = csi_rtc_uninitialize(rtc_handle);

    if (ret < 0) {
        printf("[error]csi_rtc_uninitialize error\n");
        return -1;
    }
	printf("[success]csi_rtc_uninitialize success\n");
    return 0;

}
int example_rtc(void)
{
    int ret;

    csi_coret_config(SYSTEM_CLOCK / 100, CORET_IRQn);    //10ms
    csi_vic_enable_irq(CORET_IRQn);

    ret = test_rtc();

    if (ret < 0) {
        printf("[error]test rtc failed\n");
        return -1;
    }

    printf("[success]test rtc successfully\n");
    return 0;
}

int task_RTC(void)
{
#if LOSCFG_PLATFORM_HWI		//接管中断模式
#if SHOW_PRECOMPILE_INFO
#pragma Message("接管中断模式 --> RTC")
#endif
	UINTPTR uvIntSave;										//用于保存所有的中断情况
	uvIntSave = LOS_IntLock();								//禁用所有中断，并返回原先中断情况
	LOS_HwiCreate (RTC_IRQn, 0, 0, rtc_event_cb_fun,0);//将硬件中断向量号绑定到系统中，指向中断服务函数
	LOS_IntRestore(uvIntSave);								//恢复所有中断情况
#else							//直接中断模式
#if SHOW_PRECOMPILE_INFO
#pragma Message("非接管中断模式 --> RTC")
#endif
	//csi_vic_enable_irq(RTC_IRQn);						//初始化时已经开启，此处不需要开启
#endif

	HopeRun_printf("开始RTC测试............\n");
    return example_rtc();									//RTC_IRQn RTC1_IRQn
}
