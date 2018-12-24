#ifndef __RTC_H
#define __RTC_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "osdepends/osport.h"


//rtc的时间格式可选项
typedef enum
{
  
  UDS_RTC_Hour_Format_24             = 0,   /*!< RTC 24小时制       0x00000000u*/
  UDS_RTC_Hour_Format_12             = 1,   /*!< RTC 12小时制       0x00000040u*/
 
}uds_rtc_hour_format;


typedef enum
{
   UDS_RTC_OUTPUT_DISABLE = 0,
   UDS_RTC_OUTPUT_ALARMA  = 1,
   UDS_RTC_OUTPUT_ALARMB  = 2,
   UDS_RTC_OUTPUT_WAKEUP  = 3,

}uds_rtc_output;

typedef enum
{
   UDS_RTC_OUTPUT_REMAP_NONE = 0,
   UDS_RTC_OUTPUT_REMAP_POS1  = 1,
}uds_rtc_output_remap;

typedef enum
{
   UDS_RTC_OUTPUT_POLARITY_HIGH = 0,
   UDS_RTC_OUTPUT_POLARITY_LOW  = 1,
}uds_rtc_output_polarity;

typedef enum
{
   
   UDS_RTC_OUTPUT_TYPE_OPENDRAIN  	= 0,
   UDS_RTC_OUTPUT_TYPE_PUSHPULL 	= 1,
}uds_rtc_output_type;

typedef enum
{
  UDS_RTC_DAYLIGHTSAVING_NONE	= 0,//0x00000000u,
  UDS_RTC_DAYLIGHTSAVING_ADD1H	= 1,//0x00010000u,
  UDS_RTC_DAYLIGHTSAVING_SUB1H	= 2,//0x00020000,
}uds_rtc_daylightsaving;

typedef enum
{
  UDS_RTC_STOREOPERATION_RESET	= 0,//0x00000000u,
  UDS_RTC_STOREOPERATION_SET	= 1,//0x00040000u,
}uds_rtc_storeoperation;

typedef enum
{
  UDS_RTC_FORMAT_BIN	= 0,//0x00000000u,/*!< 二进制形式输入 */
  UDS_RTC_FORMAT_BCD	= 1,//0x00000001u,/*!< BCD码形式输入 */
}uds_rtc_input_parameter_format;

typedef enum
{
  UDS_RTC_ID0             = 0,   /*!< RTC     */
  
}uds_rtc_idx;

/*当前参数仅适用于系统时钟为4MHz的场合，其他时钟频率请参照cubemx根据系统时钟频率中生成的数值配置iic初始化的timeing系数*/
typedef enum
{
  UDS_RTC_LSE_32768Hz = 0,
  UDS_RTC_LSI_32KHz = 1,
  UDS_RTC_HSE_1MHz = 2,
  UDS_RTC_LSI_37KHz =3,
}uds_rtc_clk;

//用作初始化
//将上述的类型选项 涉及的变量定义在结构体中
struct rtc_init_s
{
  uds_rtc_idx              rtc_idx;					//RTC索引号
	uds_rtc_clk				 rtc_clk;								//RTC时钟，默认是32.768KHZ
	uds_rtc_hour_format		 rtc_hour_format;		//默认24小时制
	uds_rtc_output			 rtc_output;					//默认输出关闭
	uds_rtc_output_remap 	 rtc_output_remap;	//NONE
	uds_rtc_output_polarity	 rtc_output_polarity;//输出极性为高，无效
	uds_rtc_output_type		 rtc_output_type;		//输出方式，开漏
};
typedef struct rtc_init_s rtc_init_t;



typedef enum
{
    RTC_SET_TIME,	//设置时间
	RTC_SET_DATE,	//设置日期
    RTC_SET_ALARM,	//设置闹钟时间
    RTC_SET_WKUP_TIME,	//设置wakeup时间
    RTC_GET_TIME,	//
    RTC_GET_DATE,//
    RTC_MAX_CMD,			//
}uds_rtc_cmd;





bool_t uds_rtc_dev_install(const char *name, void *pri);

#ifdef __cplusplus
}
#endif

#endif 
