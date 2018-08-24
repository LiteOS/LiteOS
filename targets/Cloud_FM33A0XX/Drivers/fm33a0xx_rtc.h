/**
  ******************************************************************************
  * @file    fm33a0xx_rtc.h
  * @author  FM33a0xx Application Team
  * @version V2.0.1
  * @date    15-1-2017
  * @brief   This file contains all the functions prototypes for the RTC firmware library.  
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FM33A0XX_RTC_H
#define __FM33A0XX_RTC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "FM33A0XX.h"
	 
/** @addtogroup FM385_StdPeriph_Driver
  * @{
  */

/** @addtogroup RTC
  * @{
  */

/* Exported constants --------------------------------------------------------*/
#define RTC_WRITE_DISABLE					((uint32_t)0x53535353)
#define RTC_WRITE_ENABLE					((uint32_t)0xACACACAC)
/* Exported types ------------------------------------------------------------*/	 
typedef struct
{       
	uint8_t			Year;		/*!<RTC 年*/
	uint8_t			Month;		/*!<RTC 月*/
	uint8_t			Date;		/*!<RTC 日*/	
	uint8_t			Hour;		/*!<RTC 时*/
	uint8_t			Minute;		/*!<RTC 分*/	
	uint8_t			Second;		/*!<RTC 秒*/			
	uint8_t			Week;		/*!<RTC 周*/	

}RTC_TimeDateTypeDef,RTC_StampTmieTypeDef;

typedef struct
{     
	uint8_t			Hour;		/*!<RTC闹钟 时*/		
	uint8_t			Minute;		/*!<RTC闹钟 分*/
	uint8_t			Second;		/*!<RTC闹钟 秒*/

}RTC_AlarmTmieTypeDef;


/* Exported macro ------------------------------------------------------------*/
/*StampType def*/				//注意时间戳功能仅在休眠模式下有效
#define STAMP0RISE	0x00		//PB4上升沿时间戳
#define STAMP0FALL	0x01		//PB4下降沿时间戳
#define STAMP1RISE	0x02		//PB5上升沿时间戳
#define STAMP1FALL	0x03		//PB5下降沿时间戳


//从0x811800开始会保存如下信息
#define const_tx_flag  		(*((uint16_t *)(0x1ffffa20))) //调校标志(0x3cc3为编程调校)
#define const_rtc_adj_top 	(*((int16_t *)(0x1ffffa36))) //顶点误差ppm,0.01ppm

//Macro_START
//2018-04-02-13-22-12
//FM33A0XX_Driver_Gen_V1.4

#define	RTC_RTCWE_RTCWE_Pos	0	/* RTC写使能寄存器 */
#define	RTC_RTCWE_RTCWE_Msk	(0xffffffffU << RTC_RTCWE_RTCWE_Pos)

#define	RTC_RTCIE_PB5R_IE_Pos	16	/* PB5上升沿中断使能 */
#define	RTC_RTCIE_PB5R_IE_Msk	(0x1U << RTC_RTCIE_PB5R_IE_Pos)

#define	RTC_RTCIE_PB5F_IE_Pos	15	/* PB5下降沿中断使能 */
#define	RTC_RTCIE_PB5F_IE_Msk	(0x1U << RTC_RTCIE_PB5F_IE_Pos)

#define	RTC_RTCIE_PB4R_IE_Pos	14	/* PB4上升沿中断使能 */
#define	RTC_RTCIE_PB4R_IE_Msk	(0x1U << RTC_RTCIE_PB4R_IE_Pos)

#define	RTC_RTCIE_PB4F_IE_Pos	13	/* PB4下降沿中断使能 */
#define	RTC_RTCIE_PB4F_IE_Msk	(0x1U << RTC_RTCIE_PB4F_IE_Pos)

#define	RTC_RTCIE_ADJ128_IE_Pos	12	/* 128秒中断使能 */
#define	RTC_RTCIE_ADJ128_IE_Msk	(0x1U << RTC_RTCIE_ADJ128_IE_Pos)

#define	RTC_RTCIE_ALARM_IE_Pos	11	/* 闹钟中断使能 */
#define	RTC_RTCIE_ALARM_IE_Msk	(0x1U << RTC_RTCIE_ALARM_IE_Pos)

#define	RTC_RTCIE_1KHZ_IE_Pos	10	/* 1khz中断使能 */
#define	RTC_RTCIE_1KHZ_IE_Msk	(0x1U << RTC_RTCIE_1KHZ_IE_Pos)

#define	RTC_RTCIE_256HZ_IE_Pos	9	/* 256hz中断使能 */
#define	RTC_RTCIE_256HZ_IE_Msk	(0x1U << RTC_RTCIE_256HZ_IE_Pos)

#define	RTC_RTCIE_64HZ_IE_Pos	8	/* 64hz中断使能 */
#define	RTC_RTCIE_64HZ_IE_Msk	(0x1U << RTC_RTCIE_64HZ_IE_Pos)

#define	RTC_RTCIE_16HZ_IE_Pos	7	/* 16hz中断使能 */
#define	RTC_RTCIE_16HZ_IE_Msk	(0x1U << RTC_RTCIE_16HZ_IE_Pos)

#define	RTC_RTCIE_8HZ_IE_Pos	6	/* 8hz中断使能 */
#define	RTC_RTCIE_8HZ_IE_Msk	(0x1U << RTC_RTCIE_8HZ_IE_Pos)

#define	RTC_RTCIE_4HZ_IE_Pos	5	/* 4hz中断使能 */
#define	RTC_RTCIE_4HZ_IE_Msk	(0x1U << RTC_RTCIE_4HZ_IE_Pos)

#define	RTC_RTCIE_2HZ_IE_Pos	4	/* 2hz中断使能 */
#define	RTC_RTCIE_2HZ_IE_Msk	(0x1U << RTC_RTCIE_2HZ_IE_Pos)

#define	RTC_RTCIE_SEC_IE_Pos	3	/* 秒中断使能 */
#define	RTC_RTCIE_SEC_IE_Msk	(0x1U << RTC_RTCIE_SEC_IE_Pos)

#define	RTC_RTCIE_MIN_IE_Pos	2	/* 分中断使能 */
#define	RTC_RTCIE_MIN_IE_Msk	(0x1U << RTC_RTCIE_MIN_IE_Pos)

#define	RTC_RTCIE_HOUR_IE_Pos	1	/* 小时中断使能 */
#define	RTC_RTCIE_HOUR_IE_Msk	(0x1U << RTC_RTCIE_HOUR_IE_Pos)

#define	RTC_RTCIE_DATE_IE_Pos	0	/* 天中断使能 */
#define	RTC_RTCIE_DATE_IE_Msk	(0x1U << RTC_RTCIE_DATE_IE_Pos)

#define	RTC_RTCIF_PB5R_IF_Pos	16	/* PB5上升沿中断标志 */
#define	RTC_RTCIF_PB5R_IF_Msk	(0x1U << RTC_RTCIF_PB5R_IF_Pos)

#define	RTC_RTCIF_PB5F_IF_Pos	15	/* PB5下降沿中断标志 */
#define	RTC_RTCIF_PB5F_IF_Msk	(0x1U << RTC_RTCIF_PB5F_IF_Pos)

#define	RTC_RTCIF_PB4R_IF_Pos	14	/* PB4上升沿中断标志 */
#define	RTC_RTCIF_PB4R_IF_Msk	(0x1U << RTC_RTCIF_PB4R_IF_Pos)

#define	RTC_RTCIF_PB4F_IF_Pos	13	/* PB4下降沿中断标志 */
#define	RTC_RTCIF_PB4F_IF_Msk	(0x1U << RTC_RTCIF_PB4F_IF_Pos)

#define	RTC_RTCIF_ADJ128_IF_Pos	12	/* 128秒中断标志 */
#define	RTC_RTCIF_ADJ128_IF_Msk	(0x1U << RTC_RTCIF_ADJ128_IF_Pos)

#define	RTC_RTCIF_ALARM_IF_Pos	11	/* 闹钟中断标志 */
#define	RTC_RTCIF_ALARM_IF_Msk	(0x1U << RTC_RTCIF_ALARM_IF_Pos)

#define	RTC_RTCIF_1KHZ_IF_Pos	10	/* 1khz中断标志 */
#define	RTC_RTCIF_1KHZ_IF_Msk	(0x1U << RTC_RTCIF_1KHZ_IF_Pos)

#define	RTC_RTCIF_256HZ_IF_Pos	9	/* 256hz中断标志 */
#define	RTC_RTCIF_256HZ_IF_Msk	(0x1U << RTC_RTCIF_256HZ_IF_Pos)

#define	RTC_RTCIF_64HZ_IF_Pos	8	/* 64hz中断标志 */
#define	RTC_RTCIF_64HZ_IF_Msk	(0x1U << RTC_RTCIF_64HZ_IF_Pos)

#define	RTC_RTCIF_16HZ_IF_Pos	7	/* 16hz中断标志 */
#define	RTC_RTCIF_16HZ_IF_Msk	(0x1U << RTC_RTCIF_16HZ_IF_Pos)

#define	RTC_RTCIF_8HZ_IF_Pos	6	/* 8hz中断标志 */
#define	RTC_RTCIF_8HZ_IF_Msk	(0x1U << RTC_RTCIF_8HZ_IF_Pos)

#define	RTC_RTCIF_4HZ_IF_Pos	5	/* 4hz中断标志 */
#define	RTC_RTCIF_4HZ_IF_Msk	(0x1U << RTC_RTCIF_4HZ_IF_Pos)

#define	RTC_RTCIF_2HZ_IF_Pos	4	/* 2hz中断标志 */
#define	RTC_RTCIF_2HZ_IF_Msk	(0x1U << RTC_RTCIF_2HZ_IF_Pos)

#define	RTC_RTCIF_SEC_IF_Pos	3	/* 秒中断标志 */
#define	RTC_RTCIF_SEC_IF_Msk	(0x1U << RTC_RTCIF_SEC_IF_Pos)

#define	RTC_RTCIF_MIN_IF_Pos	2	/* 分中断标志 */
#define	RTC_RTCIF_MIN_IF_Msk	(0x1U << RTC_RTCIF_MIN_IF_Pos)

#define	RTC_RTCIF_HOUR_IF_Pos	1	/* 小时中断标志 */
#define	RTC_RTCIF_HOUR_IF_Msk	(0x1U << RTC_RTCIF_HOUR_IF_Pos)

#define	RTC_RTCIF_DATE_IF_Pos	0	/* 天中断标志 */
#define	RTC_RTCIF_DATE_IF_Msk	(0x1U << RTC_RTCIF_DATE_IF_Pos)

#define	RTC_BCDSEC_BCDSEC_Pos	0	/* BCD时间秒寄存器 */
#define	RTC_BCDSEC_BCDSEC_Msk	(0x7fU << RTC_BCDSEC_BCDSEC_Pos)

#define	RTC_BCDMIN_BCDMIN_Pos	0	/* BCD时间分钟寄存器 */
#define	RTC_BCDMIN_BCDMIN_Msk	(0x7fU << RTC_BCDMIN_BCDMIN_Pos)

#define	RTC_BCDHOUR_BCDHOUR_Pos	0	/* BCD时间小时寄存器 */
#define	RTC_BCDHOUR_BCDHOUR_Msk	(0x3fU << RTC_BCDHOUR_BCDHOUR_Pos)

#define	RTC_BCDDATE_BCDDATE_Pos	0	/* BCD时间天寄存器 */
#define	RTC_BCDDATE_BCDDATE_Msk	(0x3fU << RTC_BCDDATE_BCDDATE_Pos)

#define	RTC_BCDWEEK_BCDWEEK_Pos	0	/* BCD时间星期寄存器 */
#define	RTC_BCDWEEK_BCDWEEK_Msk	(0x7U << RTC_BCDWEEK_BCDWEEK_Pos)

#define	RTC_BCDMONTH_BCDMONTH_Pos	0	/* BCD时间月寄存器 */
#define	RTC_BCDMONTH_BCDMONTH_Msk	(0x1fU << RTC_BCDMONTH_BCDMONTH_Pos)

#define	RTC_BCDYEAR_BCDYEAR_Pos	0	/* BCD时间年寄存器 */
#define	RTC_BCDYEAR_BCDYEAR_Msk	(0xffU << RTC_BCDYEAR_BCDYEAR_Pos)

#define	RTC_ALARM_ALARMHOUR_Pos	16	/* 闹钟的小时数值 */
#define	RTC_ALARM_ALARMHOUR_Msk	(0x3fU << RTC_ALARM_ALARMHOUR_Pos)

#define	RTC_ALARM_ALARMMIN_Pos	8	/* 闹钟的分数值 */
#define	RTC_ALARM_ALARMMIN_Msk	(0x7fU << RTC_ALARM_ALARMMIN_Pos)

#define	RTC_ALARM_ALARMSEC_Pos	0	/* 闹钟的秒数值 */
#define	RTC_ALARM_ALARMSEC_Msk	(0x7fU << RTC_ALARM_ALARMSEC_Pos)

#define	RTC_FSEL_FSEL_Pos	0	/* 频率输出选择信号 */
#define	RTC_FSEL_FSEL_Msk	(0xfU << RTC_FSEL_FSEL_Pos)
#define	RTC_FSEL_FSEL_PLL1HZ	(0x0U << RTC_FSEL_FSEL_Pos)	/* 4’b0000：输出PLL分频得到的精确1秒方波 */
#define	RTC_FSEL_FSEL_PLL1HZ80	(0x1U << RTC_FSEL_FSEL_Pos)	/* 4’b0001：输出PLL分频的高电平宽度80ms的秒时标 */
#define	RTC_FSEL_FSEL_SECOND	(0x2U << RTC_FSEL_FSEL_Pos)	/* 4’b0010：输出秒计数器进位信号，高电平宽度1s */
#define	RTC_FSEL_FSEL_MINUTE	(0x3U << RTC_FSEL_FSEL_Pos)	/* 4’b0011：输出分计数器进位信号，高电平宽度1s */
#define	RTC_FSEL_FSEL_HOUR	(0x4U << RTC_FSEL_FSEL_Pos)	/* 4’b0100：输出小时计数器进位信号，高电平宽度1s */
#define	RTC_FSEL_FSEL_DAY	(0x5U << RTC_FSEL_FSEL_Pos)	/* 4’b0101：输出天计数器进位信号，高电平宽度1s */
#define	RTC_FSEL_FSEL_ALARM	(0x6U << RTC_FSEL_FSEL_Pos)	/* 4’b0110：输出闹钟匹配信号 */
#define	RTC_FSEL_FSEL_128HZ	(0x7U << RTC_FSEL_FSEL_Pos)	/* 4’b0111：输出128秒方波信号 */
#define	RTC_FSEL_FSEL_PLL1HZ80REV	(0x8U << RTC_FSEL_FSEL_Pos)	/* 4’b1000：反向输出PLL分频的高电平宽度80ms的秒时标 */
#define	RTC_FSEL_FSEL_SECONDREV	(0x9U << RTC_FSEL_FSEL_Pos)	/* 4’b1001：反向输出秒计数器进位信号 */
#define	RTC_FSEL_FSEL_MINUTEREV	(0xaU << RTC_FSEL_FSEL_Pos)	/* 4’b1010：反向输出分计数器进位信号 */
#define	RTC_FSEL_FSEL_HOURREV	(0xbU << RTC_FSEL_FSEL_Pos)	/* 4’b1011：反向输出小时计数器进位信号 */
#define	RTC_FSEL_FSEL_DAYREV	(0xcU << RTC_FSEL_FSEL_Pos)	/* 4’b1100：反向输出天计数器进位信号 */
#define	RTC_FSEL_FSEL_ALARMREV	(0xdU << RTC_FSEL_FSEL_Pos)	/* 4’b1101：反向输出闹钟匹配信号 */
#define	RTC_FSEL_FSEL_PLL1HZREV	(0xeU << RTC_FSEL_FSEL_Pos)	/* 4’b1110：反向输出PLL分频的精确1s方波信号 */
#define	RTC_FSEL_FSEL_SECONDRUN	(0xfU << RTC_FSEL_FSEL_Pos)	/* 4’b1111：输出RTC内部秒时标方波 */

#define	RTC_ADJUST_ADJUST_Pos	0	/* LTBC补偿调整数值 */
#define	RTC_ADJUST_ADJUST_Msk	(0x7ffU << RTC_ADJUST_ADJUST_Pos)

#define	RTC_ADSIGN_ADSIGN_Pos	0	/* LTBC补偿方向 */
#define	RTC_ADSIGN_ADSIGN_Msk	(0x1U << RTC_ADSIGN_ADSIGN_Pos)
#define	RTC_ADSIGN_ADSIGN_PLUS	(0x0U << RTC_ADSIGN_ADSIGN_Pos)	/* 0：表示增加计数初值 */
#define	RTC_ADSIGN_ADSIGN_MINUS	(0x1U << RTC_ADSIGN_ADSIGN_Pos)	/* 1：表示减少计数初值 */

#define	RTC_PR1SEN_PR1SEN_Pos	0	/* 虚拟调校使能信号 */
#define	RTC_PR1SEN_PR1SEN_Msk	(0x1U << RTC_PR1SEN_PR1SEN_Pos)
	/* 0：表示禁止虚拟调校功能 */
	/* 1：表示使能虚拟调校功能 */

#define	RTC_MSECCNT_MSCNT_Pos	0	/* 毫秒计数器值。以256Hz为周期计数，精度3.9ms */
#define	RTC_MSECCNT_MSCNT_Msk	(0xffU << RTC_MSECCNT_MSCNT_Pos)

#define	RTC_STAMPEN_STAMP1EN_Pos	1	/* PB5触发的时间戳功能使能位。无复位值，建议软件上电后进行初始化 */
#define	RTC_STAMPEN_STAMP1EN_Msk	(0x1U << RTC_STAMPEN_STAMP1EN_Pos)
	/* 0：关闭时间戳 */
	/* 1：打开时间戳 */

#define	RTC_STAMPEN_STAMP0EN_Pos	0	/* PB4触发的时间戳功能使能位。无复位值，建议软件上电后进行初始化 */
#define	RTC_STAMPEN_STAMP0EN_Msk	(0x1U << RTC_STAMPEN_STAMP0EN_Pos)
	/* 0：关闭时间戳 */
	/* 1：打开时间戳 */

#define	RTC_CLKSTAMP0R_0_Pos	0	/* 检测到PB4上升沿时间戳时分秒数据 */
#define	RTC_CLKSTAMP0R_0_Msk	(0xffffffffU << RTC_CLKSTAMP0R_0_Pos)

#define	RTC_CALSTAMP0R_1_Pos	0	/* 检测到PB4上升沿时间戳年月周日数据 */
#define	RTC_CALSTAMP0R_1_Msk	(0xffffffffU << RTC_CALSTAMP0R_1_Pos)

#define	RTC_CLKSTAMP0F_0_Pos	0	/* 检测到PB4下降沿时间戳时分秒数据 */
#define	RTC_CLKSTAMP0F_0_Msk	(0xffffffffU << RTC_CLKSTAMP0F_0_Pos)

#define	RTC_CALSTAMP0F_1_Pos	0	/* 检测到PB4下降沿时间戳年月周日数据 */
#define	RTC_CALSTAMP0F_1_Msk	(0xffffffffU << RTC_CALSTAMP0F_1_Pos)

#define	RTC_CLKSTAMP1R_0_Pos	0	/* 检测到PB5上升沿时间戳时分秒数据 */
#define	RTC_CLKSTAMP1R_0_Msk	(0xffffffffU << RTC_CLKSTAMP1R_0_Pos)

#define	RTC_CALSTAMP1R_1_Pos	0	/* 检测到PB5上升沿时间戳年月周日数据 */
#define	RTC_CALSTAMP1R_1_Msk	(0xffffffffU << RTC_CALSTAMP1R_1_Pos)

#define	RTC_CLKSTAMP1F_0_Pos	0	/* 检测到PB5下降沿时间戳时分秒数据 */
#define	RTC_CLKSTAMP1F_0_Msk	(0xffffffffU << RTC_CLKSTAMP1F_0_Pos)

#define	RTC_CALSTAMP1F_1_Pos	0	/* 检测到PB5下降沿时间戳年月周日数据 */
#define	RTC_CALSTAMP1F_1_Msk	(0xffffffffU << RTC_CALSTAMP1F_1_Pos)
//Macro_End

/* Exported functions --------------------------------------------------------*/ 
extern void RTC_Deinit(void);

/* RTC写使能寄存器 相关函数 */
extern void RTC_RTCWE_Write(uint32_t SetValue);

/* BCD时间秒寄存器 相关函数 */
extern void RTC_BCDSEC_Write(uint32_t SetValue);
extern uint32_t RTC_BCDSEC_Read(void);

/* BCD时间分钟寄存器 相关函数 */
extern void RTC_BCDMIN_Write(uint32_t SetValue);
extern uint32_t RTC_BCDMIN_Read(void);

/* BCD时间小时寄存器 相关函数 */
extern void RTC_BCDHOUR_Write(uint32_t SetValue);
extern uint32_t RTC_BCDHOUR_Read(void);

/* BCD时间天寄存器 相关函数 */
extern void RTC_BCDDATE_Write(uint32_t SetValue);
extern uint32_t RTC_BCDDATE_Read(void);

/* BCD时间星期寄存器 相关函数 */
extern void RTC_BCDWEEK_Write(uint32_t SetValue);
extern uint32_t RTC_BCDWEEK_Read(void);

/* BCD时间月寄存器 相关函数 */
extern void RTC_BCDMONTH_Write(uint32_t SetValue);
extern uint32_t RTC_BCDMONTH_Read(void);

/* BCD时间年寄存器 相关函数 */
extern void RTC_BCDYEAR_Write(uint32_t SetValue);
extern uint32_t RTC_BCDYEAR_Read(void);

/* 闹钟的小时数值 相关函数 */
extern void RTC_ALARM_ALARMHOUR_Set(uint32_t SetValue);
extern uint32_t RTC_ALARM_ALARMHOUR_Get(void);

/* 闹钟的分数值 相关函数 */
extern void RTC_ALARM_ALARMMIN_Set(uint32_t SetValue);
extern uint32_t RTC_ALARM_ALARMMIN_Get(void);

/* 闹钟的秒数值 相关函数 */
extern void RTC_ALARM_ALARMSEC_Set(uint32_t SetValue);
extern uint32_t RTC_ALARM_ALARMSEC_Get(void);

/* 频率输出选择信号 相关函数 */
extern void RTC_FSEL_FSEL_Set(uint32_t SetValue);
extern uint32_t RTC_FSEL_FSEL_Get(void);

/* LTBC补偿调整数值 相关函数 */
extern void RTC_ADJUST_Write(uint32_t SetValue);
extern uint32_t RTC_ADJUST_Read(void);

/* LTBC补偿方向 相关函数 */
extern void RTC_ADSIGN_ADSIGN_Set(uint32_t SetValue);
extern uint32_t RTC_ADSIGN_ADSIGN_Get(void);

/* 虚拟调校使能信号 相关函数 */
extern void RTC_PR1SEN_PR1SEN_Setable(FunState NewState);
extern FunState RTC_PR1SEN_PR1SEN_Getable(void);

/* 毫秒计数器值。以256Hz为周期计数，精度3.9ms 相关函数 */
extern void RTC_MSECCNT_Write(uint32_t SetValue);
extern uint32_t RTC_MSECCNT_Read(void);

/* PB5触发的时间戳功能使能位。无复位值，建议软件上电后进行初始化 相关函数 */
extern void RTC_STAMPEN_STAMP1EN_Setable(FunState NewState);
extern FunState RTC_STAMPEN_STAMP1EN_Getable(void);

/* PB4触发的时间戳功能使能位。无复位值，建议软件上电后进行初始化 相关函数 */
extern void RTC_STAMPEN_STAMP0EN_Setable(FunState NewState);
extern FunState RTC_STAMPEN_STAMP0EN_Getable(void);

/* 检测到PB4上升沿时间戳时分秒数据 相关函数 */
extern uint32_t RTC_CLKSTAMP0R_Read(void);

/* 检测到PB4上升沿时间戳年月周日数据 相关函数 */
extern uint32_t RTC_CALSTAMP0R_Read(void);

/* 检测到PB4下降沿时间戳时分秒数据 相关函数 */
extern uint32_t RTC_CLKSTAMP0F_Read(void);

/* 检测到PB4下降沿时间戳年月周日数据 相关函数 */
extern uint32_t RTC_CALSTAMP0F_Read(void);

/* 检测到PB5上升沿时间戳时分秒数据 相关函数 */
extern uint32_t RTC_CLKSTAMP1R_Read(void);

/* 检测到PB5上升沿时间戳年月周日数据 相关函数 */
extern uint32_t RTC_CALSTAMP1R_Read(void);

/* 检测到PB5下降沿时间戳时分秒数据 相关函数 */
extern uint32_t RTC_CLKSTAMP1F_Read(void);

/* 检测到PB5下降沿时间戳年月周日数据 相关函数 */
extern uint32_t RTC_CALSTAMP1F_Read(void);
//Announce_End



/* RTC中断使能寄存器 相关函数 */
extern void RTC_RTCIE_SetableEx(FunState NewState, uint32_t ie_def);

extern FunState RTC_RTCIE_GetableEx(uint32_t ie_def);

/* RTC中断标志 相关函数 */
extern void RTC_RTCIF_ClrEx(uint32_t if_def);

extern FlagStatus RTC_RTCIF_ChkEx(uint32_t if_def);

/* RTC设置时间 *///注意要先打开写保护才能设置时间
extern void RTC_TimeDate_SetEx(RTC_TimeDateTypeDef* para);

/* RTC读取时间 */
extern void RTC_TimeDate_GetEx(RTC_TimeDateTypeDef* para);

/* 闹钟时间设置 */
extern void RTC_AlarmTime_SetEx(RTC_AlarmTmieTypeDef* para);

/* 闹钟时间读取 */
extern void RTC_AlarmTime_GetEx(RTC_AlarmTmieTypeDef* para);
	
/* 时间戳数据读取 */
extern void RTC_CLKSTAMPxx_GetEx(uint8_t StampType_def, RTC_StampTmieTypeDef* para);

/* RTC 时钟精度补偿函数 */
/*
	输入 float型参误差ppm
*/
extern void RTC_Trim_Proc(float err_ppm);
	
#ifdef __cplusplus
}
#endif

#endif /* __FM33A0XX_RTC_H */



/************************ (C) COPYRIGHT FMSHelectronics *****END OF FILE****/



