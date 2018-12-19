
#include "uds/uds_rtc.h"
#include "uds/uds.h"

void delay_ms(uint16_t Number)
{
    uint32_t i;

     while(Number--){
      i=250; while(i--);
    }
}

void demo_mcu_rtc(void)
{
	//uint8_t cmd;
	//s32_t ret;
	char buff[]="2018-12-17 12:13:14";
	char buff2[]="2020-03-04 02:02:02";
	char HHH[22]="";
	int i=0;
	//用于模块初始化的变量
	rtc_init_t rtc0;	
	rtc0.rtc_idx 			 = DAL_RTC_ID0;
    rtc0.rtc_clk 			 = DAL_RTC_LSE_32768Hz;
    rtc0.rtc_hour_format 	 = DAL_RTC_Hour_Format_24;
    rtc0.rtc_output 		 = DAL_RTC_OUTPUT_DISABLE;		
    rtc0.rtc_output_remap 	 = DAL_RTC_OUTPUT_REMAP_NONE;
	rtc0.rtc_output_polarity = DAL_RTC_OUTPUT_POLARITY_HIGH;
	rtc0.rtc_output_type	 = DAL_RTC_OUTPUT_TYPE_OPENDRAIN;
	
	//创建设备，并安装注册
	uds_driv_t device;												
    uds_driv_init();
    uds_rtc_dev_install("RTC", (void *)&rtc0);
    
	//打开设备
    device = uds_dev_open("RTC",2);
 //   if(!device)while(1); 
	//设置时间
	uds_dev_ioctl(device,RTC_SET_TIME,(void *)buff,strlen(buff));
    //设置日期
	uds_dev_ioctl(device,RTC_SET_DATE,(void *)buff,strlen(buff));
	//设置闹钟时间
	uds_dev_ioctl(device,RTC_SET_ALARM,(void *)buff,strlen(buff));
	//设置唤醒时间
	i=15;
	uds_dev_ioctl(device,RTC_SET_WKUP_TIME,(void *)i,1);
	//缺少控制终端开关的功能（20181214）
	
	for(i=0;i<6;i++)
	{
		delay_ms(1000);	
		//获取时间
		uds_dev_ioctl(device,RTC_GET_TIME,HHH,22);
		printf("%s\n",HHH);
		//获取日期
		uds_dev_ioctl(device,RTC_GET_DATE,HHH,22);
		printf("%s\n",HHH);
	}
	
	//写入时间
	uds_dev_write(device,0,(uint8_t*)buff2,22,1000);
	for(i=0;i<6;i++)
	{
		delay_ms(1000);	
		//读取时间日期信息
		uds_dev_read(device,0,(uint8_t*)HHH,22,1000);
		printf("%s",HHH);
	}
}


