

#include "drv_pwm.h"
#include "stdio.h"
#include "soc.h"
#include <pin.h>
#include "csky_All_Demo.h"
#include "csi_config.h"
extern void mdelay(uint32_t ms);


void example_pin_pwm_init(void)
{
	printf("[info]pwm pin init\n");
#ifdef CONFIG_YMC2HQFNR1A_CB2201
	drv_pinmux_config(PA12, PA12_PWM_CH8);//D7-GREEN
	drv_pinmux_config(PA13, PA13_PWM_CH9);//D7-red
	drv_pinmux_config(PA14, PA14_PWM_CH10);//D8-GREEN
	drv_pinmux_config(PA15, PA15_PWM_CH11);//D8-red
#else
	drv_pinmux_config(EXAMPLE_PWM_CH, EXAMPLE_PWM_CH_FUNC);//pin.h文件中，#define EXAMPLE_PWM_CH      PA4
#endif
}
#ifdef CONFIG_YMC2HQFNR1A_CB2201
int32_t  pwm_signal_test()
#else
int32_t  pwm_signal_test(uint32_t pwm_pin)
#endif
{
    int32_t ret;
	
#ifdef CONFIG_YMC2HQFNR1A_CB2201
	int8_t i=0,j=0;
	pwm_handle_t pwm_handle1;
	pwm_handle_t pwm_handle2;
	pwm_handle_t pwm_handle3;
	pwm_handle_t pwm_handle4;
#else
    pwm_handle_t pwm_handle;
#endif
	//引脚初始化
    example_pin_pwm_init();
#ifdef CONFIG_YMC2HQFNR1A_CB2201
	pwm_handle1 = drv_pwm_initialize(PA12);
	pwm_handle2 = drv_pwm_initialize(PA13);
	pwm_handle3 = drv_pwm_initialize(PA14);
	pwm_handle4 = drv_pwm_initialize(PA15);
	if (pwm_handle1 == NULL||pwm_handle2 == NULL||pwm_handle3 == NULL||pwm_handle4 == NULL) {
        printf("[error]drv_pwm_initialize error\n");
        return -1;
    }
#else
	pwm_handle = drv_pwm_initialize(pwm_pin);
	if (pwm_handle == NULL) {
        printf("[error]drv_pwm_initialize error\n");
        return -1;
    }
#endif
	printf("[success]drv_pwm_initialize success\n");
	

#ifdef CONFIG_YMC2HQFNR1A_CB2201
	//period_us  the PWM period in us
	//duty      the PMW duty.  ( 0 - 10000 represents 0% - 100% ,other values are invalid)
    ret = drv_pwm_config(pwm_handle1, SYSTEM_CLOCK, 2000, 0);if (ret < 0) { printf("[error]drv_pwm_config error\n");return -1;}
	ret = drv_pwm_config(pwm_handle2, SYSTEM_CLOCK, 2000, 0);	if (ret < 0) { printf("[error]drv_pwm_config error\n");return -1;}
	ret = drv_pwm_config(pwm_handle3, SYSTEM_CLOCK, 2000, 0);	if (ret < 0) { printf("[error]drv_pwm_config error\n");return -1;}
	ret = drv_pwm_config(pwm_handle4, SYSTEM_CLOCK, 2000, 0);if (ret < 0) { printf("[error]drv_pwm_config error\n");return -1;}printf("[success]drv_pwm_config success\n");
    
    ret = drv_pwm_start(pwm_handle1);if (ret < 0) {printf("[error]drv_pwm_start error\n");return -1;}
    ret = drv_pwm_start(pwm_handle2);if (ret < 0) {printf("[error]drv_pwm_start error\n");return -1;}
	ret = drv_pwm_start(pwm_handle3);if (ret < 0) {printf("[error]drv_pwm_start error\n");return -1;}
	ret = drv_pwm_start(pwm_handle4);if (ret < 0) {printf("[error]drv_pwm_start error\n");return -1;}printf("[success]drv_pwm_start success\n");
	
	mdelay(100);
	for(j=0;j<6;j++)//while(1)//
	{
		for(i=1;i<=10;i++)
		{
			ret = drv_pwm_config(pwm_handle1, SYSTEM_CLOCK, 2000, 1000*i);ret = drv_pwm_config(pwm_handle4, SYSTEM_CLOCK, 2000, 1000*i);
			if (ret < 0) 
			{
				printf("[error]drv_pwm_config error\n");
				return -1;
			}
			mdelay(100);//人眼的反应时间是0.1s
		}
		for(i=9;i>=0;i--)
		{
			ret = drv_pwm_config(pwm_handle1, SYSTEM_CLOCK, 2000, 1000*i);ret = drv_pwm_config(pwm_handle4, SYSTEM_CLOCK, 2000, 1000*i);
			if (ret < 0) 
			{
				printf("[error]drv_pwm_config error\n");
				return -1;
			}
			mdelay(100);//人眼的反应时间是0.1s
		}
		
		for(i=1;i<=10;i++)
		{
			ret = drv_pwm_config(pwm_handle2, SYSTEM_CLOCK, 2000, 1000*i);ret = drv_pwm_config(pwm_handle3, SYSTEM_CLOCK, 2000, 1000*i);
			if (ret < 0) 
			{
				printf("[error]drv_pwm_config error\n");
				return -1;
			}
			mdelay(100);//人眼的反应时间是0.1s
		}
		for(i=9;i>=0;i--)
		{
			ret = drv_pwm_config(pwm_handle2, SYSTEM_CLOCK, 2000, 1000*i);ret = drv_pwm_config(pwm_handle3, SYSTEM_CLOCK, 2000, 1000*i);
			if (ret < 0) 
			{
				printf("[error]drv_pwm_config error\n");
				return -1;
			}
			mdelay(100);//人眼的反应时间是0.1s
		}
	}
	
	ret = drv_pwm_stop(pwm_handle1);if (ret < 0) {printf("[error]drv_pwm_stop error\n");return -1;}
	ret = drv_pwm_stop(pwm_handle2);if (ret < 0) {printf("[error]drv_pwm_stop error\n");return -1;}
	ret = drv_pwm_stop(pwm_handle3);if (ret < 0) {printf("[error]drv_pwm_stop error\n");return -1;}
	ret = drv_pwm_stop(pwm_handle4);if (ret < 0) {printf("[error]drv_pwm_stop error\n");return -1;}printf("[success]drv_pwm_stop success\n");
	
    ret = drv_pwm_uninitialize(pwm_handle1); if (ret < 0) {printf("[error]drv_pwm_uninitialize error\n"); return -1;}
	ret = drv_pwm_uninitialize(pwm_handle2); if (ret < 0) {printf("[error]drv_pwm_uninitialize error\n"); return -1;}
	ret = drv_pwm_uninitialize(pwm_handle3); if (ret < 0) {printf("[error]drv_pwm_uninitialize error\n"); return -1;}
	ret = drv_pwm_uninitialize(pwm_handle4); if (ret < 0) {printf("[error]drv_pwm_uninitialize error\n"); return -1;}printf("[success]drv_pwm_uninitialize success\n");
#else
	ret = drv_pwm_config(pwm_handle, SYSTEM_CLOCK, 2000, 2500);
    if (ret < 0) { printf("[error]drv_pwm_config error\n");return -1;}
	printf("[success]drv_pwm_config success\n");
	mdelay(20);
	ret = drv_pwm_start(pwm_handle);
    if (ret < 0) {
        printf("[error]drv_pwm_start error\n");
        return -1;
    }
	printf("[success]drv_pwm_start success\n");
	ret = drv_pwm_config(pwm_handle, SYSTEM_CLOCK, 200, 7500);
    if (ret < 0) { printf("[error]drv_pwm_config error\n");return -1;}
	printf("[success]drv_pwm_config success\n");
	mdelay(20);


    ret = drv_pwm_stop(pwm_handle);

    if (ret < 0) {
        printf("[error]drv_pwm_stop error\n");
        return -1;
    }
	printf("[success]drv_pwm_stop success\n");
	
    ret = drv_pwm_uninitialize(pwm_handle);

    if (ret < 0) {
        printf("[error]drv_pwm_uninitialize error\n");
        return -1;
    }
	printf("[success]drv_pwm_uninitialize success\n");
#endif
    return 0;

}
#ifdef CONFIG_YMC2HQFNR1A_CB2201
int example_pwm()
#else
int example_pwm(uint32_t pwm_pin)
#endif
{
    int32_t ret;
#ifdef CONFIG_YMC2HQFNR1A_CB2201
	ret = pwm_signal_test();
#else
    ret = pwm_signal_test(pwm_pin);
#endif
    if (ret < 0) {
        printf("[error]pwm_signal_test error\n");
        return -1;
    }

    printf("[success]pwm_signal_test success\n");

    return 0;
}

int task_PWM(void)
{
	//开始测试
	HopeRun_printf("开始PWM测试............\n");
#ifdef CONFIG_YMC2HQFNR1A_CB2201
	printf("[info]“CSKY 赤焰剑”的呼吸灯测试，请观察开发板LED变化！\n");
	return example_pwm();
#else
    return example_pwm(EXAMPLE_PWM_CH);
#endif
}
