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
  
  DAL_RTC_Hour_Format_24             = 0,   /*!< RTC 24小时制       0x00000000u*/
  DAL_RTC_Hour_Format_12             = 1,   /*!< RTC 12小时制       0x00000040u*/
 
}dal_rtc_hour_format;


typedef enum
{
   DAL_RTC_OUTPUT_DISABLE = 0,
   DAL_RTC_OUTPUT_ALARMA  = 1,
   DAL_RTC_OUTPUT_ALARMB  = 2,
   DAL_RTC_OUTPUT_WAKEUP  = 3,

}dal_rtc_output;

typedef enum
{
   DAL_RTC_OUTPUT_REMAP_NONE = 0,
   DAL_RTC_OUTPUT_REMAP_POS1  = 1,
}dal_rtc_output_remap;

typedef enum
{
   DAL_RTC_OUTPUT_POLARITY_HIGH = 0,
   DAL_RTC_OUTPUT_POLARITY_LOW  = 1,
}dal_rtc_output_polarity;

typedef enum
{
   
   DAL_RTC_OUTPUT_TYPE_OPENDRAIN  	= 0,
   DAL_RTC_OUTPUT_TYPE_PUSHPULL 	= 1,
}dal_rtc_output_type;

typedef enum
{
  DAL_RTC_DAYLIGHTSAVING_NONE	= 0,//0x00000000u,
  DAL_RTC_DAYLIGHTSAVING_ADD1H	= 1,//0x00010000u,
  DAL_RTC_DAYLIGHTSAVING_SUB1H	= 2,//0x00020000,
}dal_rtc_daylightsaving;

typedef enum
{
  DAL_RTC_STOREOPERATION_RESET	= 0,//0x00000000u,
  DAL_RTC_STOREOPERATION_SET	= 1,//0x00040000u,
}dal_rtc_storeoperation;

typedef enum
{
  DAL_RTC_FORMAT_BIN	= 0,//0x00000000u,/*!< 二进制形式输入 */
  DAL_RTC_FORMAT_BCD	= 1,//0x00000001u,/*!< BCD码形式输入 */
}dal_rtc_input_parameter_format;

typedef enum
{
  DAL_RTC_ID0             = 0,   /*!< RTC     */
  
}dal_rtc_idx;

/*当前参数仅适用于系统时钟为4MHz的场合，其他时钟频率请参照cubemx根据系统时钟频率中生成的数值配置iic初始化的timeing系数*/
typedef enum
{
  DAL_RTC_LSE_32768Hz = 0,
  DAL_RTC_LSI_32KHz = 1,
  DAL_RTC_HSE_1MHz = 2,
  DAL_RTC_LSI_37KHz =3,
}dal_rtc_clk;

//用作初始化
//将上述的类型选项 涉及的变量定义在结构体中
struct rtc_init_s
{
    dal_rtc_idx              rtc_idx;		//RTC索引号
	dal_rtc_clk				 rtc_clk;			//RTC时钟，默认是32.768KHZ
	dal_rtc_hour_format		 rtc_hour_format;	//默认24小时制
	dal_rtc_output			 rtc_output;		//默认输出关闭
	dal_rtc_output_remap 	 rtc_output_remap;//NONE
	dal_rtc_output_polarity	 rtc_output_polarity;//输出极性为高，无效
	dal_rtc_output_type		 rtc_output_type;	//输出方式，开漏
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
}dal_rtc_cmd;





bool_t uds_rtc_dev_install(const char *name, void *pri);

#ifdef __cplusplus
}
#endif

#endif 
