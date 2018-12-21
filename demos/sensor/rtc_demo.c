
#include "uds/uds_rtc.h"
#include "uds/uds.h"
#define SOFT_DELAY 0
#if SOFT_DELAY
void delay_ms(uint16_t Number)
{
    uint32_t i;

     while(Number--){
      i=1250; while(i--);
    }
}
#else

extern void HAL_Delay();

#endif
void demo_mcu_rtc(void)
{
	//uint8_t cmd;
	//s32_t ret;
	char buffer_datetime1[]="2018-12-17 12:13:14";
	char buffer_datetime2[]="2020-03-04 02:02:02";
	char buffer_get_datetime[22]="";
	int i=0;
	//用于模块初始化的变量
	rtc_init_t rtc0;	
	rtc0.rtc_idx 					= UDS_RTC_ID0;
  rtc0.rtc_clk 					= UDS_RTC_LSI_32KHz;//DAL_RTC_LSE_32768Hz;
  rtc0.rtc_hour_format 	= UDS_RTC_Hour_Format_24;
//  rtc0.rtc_output 			= UDS_RTC_OUTPUT_DISABLE;		
//  rtc0.rtc_output_remap 	= UDS_RTC_OUTPUT_REMAP_NONE;
//	rtc0.rtc_output_polarity= UDS_RTC_OUTPUT_POLARITY_HIGH;
//	rtc0.rtc_output_type		= UDS_RTC_OUTPUT_TYPE_OPENDRAIN;

	printf("[info]demo_mcu_rtc\n");
	//创建设备，并安装注册
	uds_driv_t device;
		
    uds_driv_init();
    uds_rtc_dev_install("RTC", (void *)&rtc0);
    printf("[success]install rtc success\n");
	//打开设备
    device = uds_dev_open("RTC",2);
    if(!device)while(1); 
		printf("[success]open rtc success\n");
	//设置时间
	uds_dev_ioctl(device,RTC_SET_TIME,(void *)buffer_datetime1,strlen(buffer_datetime1));
    //设置日期
	uds_dev_ioctl(device,RTC_SET_DATE,(void *)buffer_datetime1,strlen(buffer_datetime1));
	//设置闹钟时间
	uds_dev_ioctl(device,RTC_SET_ALARM,(void *)buffer_datetime1,strlen(buffer_datetime1));
	//设置唤醒时间
	i=15;
	uds_dev_ioctl(device,RTC_SET_WKUP_TIME,(void *)i,1);
	//缺少控制终端开关的功能（20181214）
	
	for(i=0;i<100;i++)
	{
#if SOFT_DELAY
		delay_ms(1000);
#else
		HAL_Delay(1000);
#endif		
		//获取日期
		uds_dev_ioctl(device,RTC_GET_DATE,buffer_get_datetime,22);
		printf("%s\t",buffer_get_datetime);
		//获取时间
		uds_dev_ioctl(device,RTC_GET_TIME,buffer_get_datetime,22);
		printf("%s\n",buffer_get_datetime);

	}
	
	//写入时间
	uds_dev_write(device,0,(uint8_t*)buffer_datetime2,22,1000);
	for(i=0;i<100;i++)
	{
#if SOFT_DELAY
		delay_ms(1000);
#else
		HAL_Delay(1000);
#endif
		//读取时间日期信息
		uds_dev_read(device,0,(uint8_t*)buffer_get_datetime,22,1000);
		printf("%s",buffer_get_datetime);
	}
}


