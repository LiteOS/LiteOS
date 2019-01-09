//测试用例，20181030
//定时器中断

#include "dw_timer.h"
#include "drv_timer.h"
#include "stdio.h"
#include "soc.h"
#include "csky_All_Demo.h"

#include "los_hwi.h"						//应为调用了 LOS_XXXX() 函数
#define TEST_MODE_FREE_RUNING   0			//计数模式0 自由运行
#define TEST_MODE_USER_DEFINED  1			//计数模式1 指定数值，计数器重载
static volatile uint8_t time_free_runing_flag = 0;
static volatile uint8_t time_user_defined_flag = 0;
static volatile uint8_t test_mode;

extern void mdelay(uint32_t ms);
/*----------------------------------------------------------------------------------------------------------
根据手册 CH2201 userguide 144页正文描述 ，得知读取EOI可以清除定时器中断标志
-------------------------------------------------------------------------------------------------------------*/
#define readl(addr) ({ unsigned int __v = (*(volatile unsigned int *) (addr)); __v; })	//用于读取寄存器数据
#define A_Timer1EOI	(CSKY_TIM0_BASE+0x0C)												//定时器A1的EOI地址

#if LOSCFG_PLATFORM_HWI
static void timer_event_cb_fun()
#else
static void timer_event_cb_fun(int32_t idx, timer_event_e event) //带传参，不适用
#endif
{
	printf("[info]进入定时器回调函数:");
    switch (test_mode) {
        case TEST_MODE_FREE_RUNING:
            time_free_runing_flag = 1;
            printf("enter cb for mode free_runing \n");
            break;

        case TEST_MODE_USER_DEFINED:
            time_user_defined_flag = 1;
            printf("enter cb for mode user_defined\n");
            break;
    }
	
	//csi_vic_disable_irq(TIMA0_IRQn);//此处的关闭中断，起到的作用是，清零中断标志位和禁用中断服务函数。此处只需要清零的功能。（可以使用该句话，但冗余不完美）
	//uint32_t value = *(uint32_t*)A_Timer1EOI;//此句读取EOI的操作，清标志失败
	readl(A_Timer1EOI);//使用readl读取，清标志成功。
}

//测试定时器--freerun
static int32_t test_free_running_fun(timer_handle_t timer_handle)
{
    uint32_t timeout = 0;	//超时时间 影响因子
    uint32_t value;					//存放定时器的实时值，用于比较
    int32_t ret;					//用于接受返回值
    time_free_runing_flag = 0;		//用于表示是否进入了定时中断
    test_mode = TEST_MODE_FREE_RUNING;								//自由模式
	
    ret = csi_timer_config(timer_handle, TIMER_MODE_FREE_RUNNING);	//配置定时器A1（由最外层传来的0决定）为自由计数模式。（A1 A2 B1 B2）
    if (ret < 0) 	//如果配置失败，返回错误码
	{
		printf("[error]csi_timer_config error\n");
		return -1;
	} 										
	printf("[success]csi_timer_config success\n");
	
    ret = csi_timer_start(timer_handle);							//开启定时器A1
    if (ret < 0) //如果启动失败，返回错误码
	{
		printf("[error]csi_timer_start error\n");
		return -1;
	}		
	printf("[success]csi_timer_start success\n");
	
	csi_timer_get_current_value(timer_handle, &value);
    while (timeout<=10000) 
	{
												//超时计数
		if(timeout%1000==0)
			printf("\r...%d...0x%x",timeout/1000,value);
		mdelay(1);
        timeout++;
		csi_timer_get_current_value(timer_handle, &value);			//读取定时器数值
    }

    ret = csi_timer_stop(timer_handle);								//停止定时器A1

    if (ret < 0) 
	{
		printf("\n[error]csi_timer_stop error\n");
        return -1;
    }
	printf("\n[success]csi_timer_stop success\n");
    return 0;
}



//测试定时器--userdefine
static int32_t test_user_defined_fun(timer_handle_t timer_handle)
{
    uint32_t timeout = 0;
    int32_t ret;
    time_user_defined_flag = 0;

    test_mode = TEST_MODE_USER_DEFINED;

    ret = csi_timer_config(timer_handle, TIMER_MODE_RELOAD);

    if (ret < 0) {
        return -1;
    }
	printf("[success]user csi_timer_config success\n");
	
	
    ret = csi_timer_set_timeout(timer_handle, 10000000);

    if (ret < 0) {
        return -1;
    }
	printf("[success]user csi_timer_set_timeout success\n");
	
	
    ret = csi_timer_start(timer_handle);

    if (ret < 0) {
        return -1;
    }
	printf("[success]user csi_timer_start success\n[info]wait for timeout Interrupt.......\n");
	
	
    while (timeout<12000) 
	{
		mdelay(1);
        timeout++;
		if(timeout%1000==0)
			printf("\r...%d...",timeout/1000);
        if (time_user_defined_flag == 1) {
            break;
        }

		
    }


    if (time_user_defined_flag == 0) {
        printf("[error]time out ，time_user_defined_flag = %d \n",time_user_defined_flag);
        return -1;
    }

    ret = csi_timer_stop(timer_handle);

    if (ret < 0) {
		printf("[error]csi_timer_stop error");
        return -1;
    }
	printf("\n[success]csi_timer_stop success\n");
    return 0;
}

static int32_t test_timer(uint8_t timer_num)
{
    int32_t ret;
    timer_handle_t timer_handle;

    timer_handle = csi_timer_initialize(timer_num, timer_event_cb_fun);//

    if (timer_handle == NULL){
        printf("[error]csi_timer_initialize error\n");
        return -1;
    }
	printf("[success]csi_timer_initialize success\n");

    ret = test_free_running_fun(timer_handle);

    if (ret < 0) {
        printf("[error]test_free_running_fun error\n\n");
        return -1;
    }

    printf("[success]timer free_running test successfully\n\n");
	
	

    ret = test_user_defined_fun(timer_handle);

    if (ret < 0) {
        printf("[error]test_reload_fun error\n");
        return -1;
    }

    printf("[success]time out after 10s successfully\n");

    ret = csi_timer_uninitialize(timer_handle);

    if (ret < 0) {
        printf("[error]csi_timer_uninitialize error\n");
        return -1;
    }

    return 0;
}

//总控制--测试定时器的函数
//传入参数决定是定时器（0-A1 1-A2 2-B1 3-B2）
int example_timer(uint8_t timer_num)
{
    int ret;
    ret = test_timer(timer_num);

    if (ret < 0) 
	{
        printf("[error]test timer%d failed\n", timer_num);
        return -1;
    }

    printf("[success]test timer%d successfully\n", timer_num);
    return 0;
}
//void dw_timer_irqhandler(int32_t idx)
//{
//	printf("....@@@....\n");
//}

//外部调用，demo
int task_Timer(void)
{
	
#if LOSCFG_PLATFORM_HWI			//接管中断模式
#if SHOW_PRECOMPILE_INFO
#pragma Message("接管中断模式 --> Timer")
#endif
	UINTPTR uvIntSave;											//用于保存所有的中断情况
	uvIntSave = LOS_IntLock();									//禁用所有中断，并返回原先中断情况
	LOS_HwiCreate (TIMA0_IRQn, 0, 0, timer_event_cb_fun, 0);	//将硬件中断向量号绑定到系统中，指向中断服务函数
	LOS_IntRestore(uvIntSave);									//恢复所有中断情况
#else							//直接中断模式
#if SHOW_PRECOMPILE_INFO
#pragma Message("非接管中断模式 --> Timer")
#endif
	//csi_vic_enable_irq(TIMA0_IRQn);							//底层层序的中断使能,模块初始化时会使能，此处不需要使能。
#endif
	
	//开始测试
	HopeRun_printf("\033[47;31m开始定时器测试............\033[0m\n");//\033[47;31mThis is a color test.\033[0m
	return example_timer(0);									//0   CSKY_TIM0_BASE, TIMA0_IRQn

}



