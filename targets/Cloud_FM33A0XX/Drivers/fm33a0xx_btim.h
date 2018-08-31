/**
  ******************************************************************************
  * @file    fm33a0xx_btim.h
  * @author  FM33a0xx Application Team
  * @version V2.0.1
  * @date    
  * @brief   This file contains all the functions prototypes for the BTIM firmware library.  
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FM33A0XX_BTIM_H
#define __FM33A0XX_BTIM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "FM33A0XX.h"
	 
/** @addtogroup FM33a0xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup BTIM
  * @{
  */ 

/* Exported constants --------------------------------------------------------*/

/** @defgroup BTIM_Exported_Constants
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  BTIM Init Structure definition  
  */ 

typedef struct
{
  uint32_t RTCSEL1;            /* SLELECT RTC SOUCE 1 */
  uint32_t RTCSEL2;          	/* SLELECT RTC SOUCE 2 */
  uint32_t INSEL1;            	/* SLELECT INNER SOUCE 1 */
  uint32_t INSEL2;             /* SLELECT INNER SOUCE 2 */										   	
  uint32_t EXSEL1;							/* SLELECT EX SOUCE 1 */
  uint32_t EXSEL2;							/* SLELECT EX SOUCE 2 */
  uint32_t GRP1SEL;						/* SLELECT GROUP1 SOUCE */
  uint32_t GRP2SEL;						/* SLELECT GROUP2 SOUCE */
  uint32_t CAPSRC;							/* SLELECT CAPTURE SOUCE */
  uint32_t CNTSRC;							/* SLELECT COUNT SOUCE */
  uint32_t SRCSEL;            	/* SET 1 TO LOW COUNTER */
  uint32_t CNTHSEL;							/* SELECT H COUNT SOURCE */
} BTIM_SRCSelType;

typedef struct
{
  uint32_t CAPONCE;            /* 0-CONTINUOUS 1-ONCE */
  uint32_t CAPCLR;          	 	/* CLR AT CAPTURE */
  uint32_t CAPMOD;            	/* 0-CYCLE 1-PULSE */
} BTIM_CAPInitType;

typedef struct
{
  FunState DIREN;            	/* DIR SIGNAL ENABLE */
  //uint32_t STDIR;          	 	/* SET DIR */
  uint32_t DIRPO;             	/* 1- REVERSE THE DIR SIGNAL*/										   	
} BTIM_DIRInitType;

typedef struct
{
  uint32_t OUTCNT;             /* SET OUTPUT PULSE WIDTH,0-FFF */										   	
  uint32_t OUTCLR;							/* SET OUTPUT TO 0 */
  uint32_t OUTINV;							/* SET OUTPUT SIGNAL REVERSE */
  uint32_t OUTMOD;							/* 0-PULSE 1-LEVEL */	
  uint32_t OUTSEL;							/* SELECT OUTPUT SOUCE */
	
} BTIM_OUTInitType;


typedef struct
{
//  BTIM_SRCSelType 		BTIM_CLK_SEL_SETTING;            		/* SLELECT CLK SOUCE */
//  BTIM_CAPInitType 		BTIM_CAPTUTE_SETTING;          	 	/* SLELECT CAPTURE SETTIG */
//  BTIM_DIRInitType 	  BTIM_DIR_SETTING;            	 	  /* SLELECT DIR SETTIG*/
//  BTIM_OUTInitType 		BTIM_OUTPUT_SETTING;             	/* SLELECT OUTPUT SETTIG*/	
	
  uint32_t PRESCALE;		
  uint32_t PRESETH;
  uint32_t PRESETL;
  uint32_t LOADL;	
  uint32_t LOADH;	
  //uint32_t CMPL;
  //uint32_t CMPH;
	
  uint32_t MODE;
  uint32_t EDGESEL;			/* 0-POS 1-NEG */
	
} BTIM_CTRLInitType;


typedef struct
{
	BTIM_CTRLInitType ctrl_para;//控制类参数
	
	BTIM_SRCSelType sig_src_para;//信号来源参数
	BTIM_CAPInitType cap_para;//捕捉模式参数
	BTIM_DIRInitType dir_para;//正反向脉冲计数参数
	BTIM_OUTInitType out_para;//输出参数
	
}BTIM_InitTypeDef;


//Macro_START
//2018-04-02-13-20-21
//FM33A0XX_Driver_Gen_V1.4

#define	BTIMx_BTCR1_CHEN_Pos	7	/* 高位计数器启动控制 */
#define	BTIMx_BTCR1_CHEN_Msk	(0x1U << BTIMx_BTCR1_CHEN_Pos)
	/* 0：启动高位计数器 */
	/* 1：停止高位计数器 */

#define	BTIMx_BTCR1_CLEN_Pos	6	/* 低位计数器启动控制 */
#define	BTIMx_BTCR1_CLEN_Msk	(0x1U << BTIMx_BTCR1_CLEN_Pos)
	/* 0：启动低位计数器 */
	/* 1：停止低位计数器 */

#define	BTIMx_BTCR1_MODE_Pos	5	/* 工作模式选择 */
#define	BTIMx_BTCR1_MODE_Msk	(0x1U << BTIMx_BTCR1_MODE_Pos)
#define	BTIMx_BTCR1_MODE_8BITS_TIM_CNT	(0x0U << BTIMx_BTCR1_MODE_Pos)	/* 8bit定时计数模式 */
#define	BTIMx_BTCR1_MODE_16BITS_CAP	(0x1U << BTIMx_BTCR1_MODE_Pos)	/* 16bit捕捉模式 */

#define	BTIMx_BTCR1_EDGESEL_Pos	4	/* 计数沿或捕捉沿选择 */
#define	BTIMx_BTCR1_EDGESEL_Msk	(0x1U << BTIMx_BTCR1_EDGESEL_Pos)
#define	BTIMx_BTCR1_EDGESEL_POS	(0x0U << BTIMx_BTCR1_EDGESEL_Pos)	/* 上升沿计数或捕捉 */
#define	BTIMx_BTCR1_EDGESEL_NEG	(0x1U << BTIMx_BTCR1_EDGESEL_Pos)	/* 下降沿计数或捕捉 */

#define	BTIMx_BTCR1_CAPMOD_Pos	3	/* 捕捉模式控制（仅捕捉模式下有效） */
#define	BTIMx_BTCR1_CAPMOD_Msk	(0x1U << BTIMx_BTCR1_CAPMOD_Pos)
#define	BTIMx_BTCR1_CAPMOD_PAUSE_PERIOD	(0x0U << BTIMx_BTCR1_CAPMOD_Pos)	/* 脉冲周期捕捉 */
#define	BTIMx_BTCR1_CAPMOD_PAUSE_WIDTH	(0x1U << BTIMx_BTCR1_CAPMOD_Pos)	/* 脉冲宽度捕捉 */

#define	BTIMx_BTCR1_CAPCLR_Pos	2	/* 带清零捕捉模式控制 */
#define	BTIMx_BTCR1_CAPCLR_Msk	(0x1U << BTIMx_BTCR1_CAPCLR_Pos)
#define	BTIMx_BTCR1_CAPCLR_CAP_CNT_NO_CLR	(0x0U << BTIMx_BTCR1_CAPCLR_Pos)	/* 捕捉后计数器不清零 */
#define	BTIMx_BTCR1_CAPCLR_CAP_CNT_CLR	(0x1U << BTIMx_BTCR1_CAPCLR_Pos)	/* 捕捉后计数器清零 */

#define	BTIMx_BTCR1_CAPONCE_Pos	1	/* 单次捕捉控制 */
#define	BTIMx_BTCR1_CAPONCE_Msk	(0x1U << BTIMx_BTCR1_CAPONCE_Pos)
#define	BTIMx_BTCR1_CAPONCE_CONTINUE	(0x0U << BTIMx_BTCR1_CAPONCE_Pos)	/* 连续捕捉 */
#define	BTIMx_BTCR1_CAPONCE_SINGLE	(0x1U << BTIMx_BTCR1_CAPONCE_Pos)	/* 单次捕捉 */

#define	BTIMx_BTCR1_PWM_Pos	0	/* PWM模式输出 */
#define	BTIMx_BTCR1_PWM_Msk	(0x1U << BTIMx_BTCR1_PWM_Pos)
	/* 0：PWM输出不使能 */
	/* 1：PWM输出使能 */

#define	BTIMx_BTCR2_SIG2SEL_Pos	7	/* 计数器内部计数源信号选择 */
#define	BTIMx_BTCR2_SIG2SEL_Msk	(0x1U << BTIMx_BTCR2_SIG2SEL_Pos)
#define	BTIMx_BTCR2_SIG2SEL_GROUP1	(0x0U << BTIMx_BTCR2_SIG2SEL_Pos)	/* 内部计数源选择Group1 */
#define	BTIMx_BTCR2_SIG2SEL_GROUP2	(0x1U << BTIMx_BTCR2_SIG2SEL_Pos)	/* 内部计数源选择Group2 */

#define	BTIMx_BTCR2_SIG1SEL_Pos	6	/* 计数器内部捕捉源信号选择 */
#define	BTIMx_BTCR2_SIG1SEL_Msk	(0x1U << BTIMx_BTCR2_SIG1SEL_Pos)
#define	BTIMx_BTCR2_SIG1SEL_GROUP2	(0x0U << BTIMx_BTCR2_SIG1SEL_Pos)	/* 内部计数源选择Group2 */
#define	BTIMx_BTCR2_SIG1SEL_GROUP1	(0x1U << BTIMx_BTCR2_SIG1SEL_Pos)	/* 内部计数源选择Group1 */

#define	BTIMx_BTCR2_CNTHSEL_Pos	4	/* 高位计数器源选择 */
#define	BTIMx_BTCR2_CNTHSEL_Msk	(0x3U << BTIMx_BTCR2_CNTHSEL_Pos)
#define	BTIMx_BTCR2_CNTHSEL_CNTL	(0x0U << BTIMx_BTCR2_CNTHSEL_Pos)	/* 与低位计数器组成16位计数器 */
#define	BTIMx_BTCR2_CNTHSEL_CAPSRC	(0x1U << BTIMx_BTCR2_CNTHSEL_Pos)	/* 选择内部捕捉信号源 */
#define	BTIMx_BTCR2_CNTHSEL_CNTSRC_DIR	(0x2U << BTIMx_BTCR2_CNTHSEL_Pos)	/* 选择内部计数信号源或外部DIR输入组合 */

#define	BTIMx_BTCR2_DIREN_Pos	3	/* 外部输入DIR控制使能 */
#define	BTIMx_BTCR2_DIREN_Msk	(0x1U << BTIMx_BTCR2_DIREN_Pos)
	/* 0：外部输入DIR信号无效 */
	/* 1：外部输入DIR信号有效 */

#define	BTIMx_BTCR2_STDIR_Pos	2	/* 内部DIR控制信号使能 */
#define	BTIMx_BTCR2_STDIR_Msk	(0x1U << BTIMx_BTCR2_STDIR_Pos)
	/* 0：内部DIR信号无效 */
	/* 1：内部DIR信号为高电平，则高8位计数器计数 */

#define	BTIMx_BTCR2_SRCSEL_Pos	1	/* 低位计数器使能控制选择信号 */
#define	BTIMx_BTCR2_SRCSEL_Msk	(0x1U << BTIMx_BTCR2_SRCSEL_Pos)
#define	BTIMx_BTCR2_SRCSEL_STDIR_EXSIG2	(0x0U << BTIMx_BTCR2_SRCSEL_Pos)	/* 低位计数器由STDIR或外部EX_SIG2输入控制 */
#define	BTIMx_BTCR2_SRCSEL_WITHOUT_DIR	(0x1U << BTIMx_BTCR2_SRCSEL_Pos)	/* 低位计数器不受DIR控制 */

#define	BTIMx_BTCR2_DIRPO_Pos	0	/* 输入信号2极性选择 */
#define	BTIMx_BTCR2_DIRPO_Msk	(0x1U << BTIMx_BTCR2_DIRPO_Pos)
#define	BTIMx_BTCR2_DIRPO_NO_ANTI	(0x0U << BTIMx_BTCR2_DIRPO_Pos)	/* 对外部输入DIR信号不反向 */
#define	BTIMx_BTCR2_DIRPO_ANTI	(0x1U << BTIMx_BTCR2_DIRPO_Pos)	/* 对外部输入DIR信号取反向 */

#define	BTIMx_BTCFG1_RTCSEL2_Pos	6	/* RTCOUT2信号控制2 */
#define	BTIMx_BTCFG1_RTCSEL2_Msk	(0x3U << BTIMx_BTCFG1_RTCSEL2_Pos)
#define	BTIMx_BTCFG1_RTCSEL2_RTC_32768	(0x0U << BTIMx_BTCFG1_RTCSEL2_Pos)	/* 由RTC模块输出的32768Hz信号 */
#define	BTIMx_BTCFG1_RTCSEL2_RTC_SEC	(0x1U << BTIMx_BTCFG1_RTCSEL2_Pos)	/* 由RTC模块输出的秒信号 */
#define	BTIMx_BTCFG1_RTCSEL2_RTC_MIN	(0x2U << BTIMx_BTCFG1_RTCSEL2_Pos)	/* 由RTC模块输出的分钟信号 */
#define	BTIMx_BTCFG1_RTCSEL2_LPTIM_OUT	(0x3U << BTIMx_BTCFG1_RTCSEL2_Pos)	/* 由LPTIM模块输出的信号 */

#define	BTIMx_BTCFG1_RTCSEL1_Pos	4	/* RTCOUT1信号控制1 */
#define	BTIMx_BTCFG1_RTCSEL1_Msk	(0x3U << BTIMx_BTCFG1_RTCSEL1_Pos)
#define	BTIMx_BTCFG1_RTCSEL1_RTC_32768	(0x0U << BTIMx_BTCFG1_RTCSEL1_Pos)	/* 由RTC模块输出的32768Hz信号 */
#define	BTIMx_BTCFG1_RTCSEL1_RTC_SEC	(0x1U << BTIMx_BTCFG1_RTCSEL1_Pos)	/* 由RTC模块输出的秒信号 */
#define	BTIMx_BTCFG1_RTCSEL1_RTC_MIN	(0x2U << BTIMx_BTCFG1_RTCSEL1_Pos)	/* 由RTC模块输出的分钟信号 */
#define	BTIMx_BTCFG1_RTCSEL1_LPTIM_OUT	(0x3U << BTIMx_BTCFG1_RTCSEL1_Pos)	/* 由LPTIM模块输出的信号 */

#define	BTIMx_BTCFG1_GRP2SEL_Pos	2	/* Group2信号选择控制 */
#define	BTIMx_BTCFG1_GRP2SEL_Msk	(0x3U << BTIMx_BTCFG1_GRP2SEL_Pos)
#define	BTIMx_BTCFG1_GRP2SEL_APBCLK	(0x0U << BTIMx_BTCFG1_GRP2SEL_Pos)
#define	BTIMx_BTCFG1_GRP2SEL_RTCOUT2	(0x1U << BTIMx_BTCFG1_GRP2SEL_Pos)
#define	BTIMx_BTCFG1_GRP2SEL_IN_SIG2	(0x2U << BTIMx_BTCFG1_GRP2SEL_Pos)	/* 内部输入信号2 */
#define	BTIMx_BTCFG1_GRP2SEL_EX_SIG2	(0x3U << BTIMx_BTCFG1_GRP2SEL_Pos)	/* 外部输入信号2 */

#define	BTIMx_BTCFG1_GRP1SEL_Pos	0
#define	BTIMx_BTCFG1_GRP1SEL_Msk	(0x3U << BTIMx_BTCFG1_GRP1SEL_Pos)
#define	BTIMx_BTCFG1_GRP1SEL_APBCLK	(0x0U << BTIMx_BTCFG1_GRP1SEL_Pos)
#define	BTIMx_BTCFG1_GRP1SEL_RTCOUT1	(0x1U << BTIMx_BTCFG1_GRP1SEL_Pos)
#define	BTIMx_BTCFG1_GRP1SEL_IN_SIG1	(0x2U << BTIMx_BTCFG1_GRP1SEL_Pos)	/* 内部输入信号1 */
#define	BTIMx_BTCFG1_GRP1SEL_EX_SIG1	(0x3U << BTIMx_BTCFG1_GRP1SEL_Pos)	/* 外部输入信号1 */

#define	BTIMx_BTCFG2_EXSEL2_Pos	6	/* 外部输入信号选择控制2 */
#define	BTIMx_BTCFG2_EXSEL2_Msk	(0x3U << BTIMx_BTCFG2_EXSEL2_Pos)
#define	BTIMx_BTCFG2_EXSEL2_BT_IN0	(0x0U << BTIMx_BTCFG2_EXSEL2_Pos)
#define	BTIMx_BTCFG2_EXSEL2_BT_IN1	(0x1U << BTIMx_BTCFG2_EXSEL2_Pos)
#define	BTIMx_BTCFG2_EXSEL2_BT_IN2	(0x2U << BTIMx_BTCFG2_EXSEL2_Pos)
#define	BTIMx_BTCFG2_EXSEL2_BT_IN3	(0x3U << BTIMx_BTCFG2_EXSEL2_Pos)

#define	BTIMx_BTCFG2_EXSEL1_Pos	4	/* 外部输入信号选择控制1 */
#define	BTIMx_BTCFG2_EXSEL1_Msk	(0x3U << BTIMx_BTCFG2_EXSEL1_Pos)
#define	BTIMx_BTCFG2_EXSEL1_BT_IN0	(0x0U << BTIMx_BTCFG2_EXSEL1_Pos)
#define	BTIMx_BTCFG2_EXSEL1_BT_IN1	(0x1U << BTIMx_BTCFG2_EXSEL1_Pos)
#define	BTIMx_BTCFG2_EXSEL1_BT_IN2	(0x2U << BTIMx_BTCFG2_EXSEL1_Pos)
#define	BTIMx_BTCFG2_EXSEL1_BT_IN3	(0x3U << BTIMx_BTCFG2_EXSEL1_Pos)

#define	BTIMx_BTCFG2_INSEL2_Pos	2	/* 内部输入信号选择控制2 */
#define	BTIMx_BTCFG2_INSEL2_Msk	(0x3U << BTIMx_BTCFG2_INSEL2_Pos)
#define	BTIMx_BTCFG2_INSEL2_UART_RX3	(0x0U << BTIMx_BTCFG2_INSEL2_Pos)
#define	BTIMx_BTCFG2_INSEL2_UART_RX4	(0x1U << BTIMx_BTCFG2_INSEL2_Pos)
#define	BTIMx_BTCFG2_INSEL2_UART_RX5	(0x2U << BTIMx_BTCFG2_INSEL2_Pos)
#define	BTIMx_BTCFG2_INSEL2_RCLP	(0x3U << BTIMx_BTCFG2_INSEL2_Pos)	/* 仅针对BT1 */
#define	BTIMx_BTCFG2_INSEL2_BT1_OUT	(0x3U << BTIMx_BTCFG2_INSEL2_Pos)	/* 仅针对BT2 */

#define	BTIMx_BTCFG2_INSEL1_Pos	0
#define	BTIMx_BTCFG2_INSEL1_Msk	(0x3U << BTIMx_BTCFG2_INSEL1_Pos)
#define	BTIMx_BTCFG2_INSEL1_UART_RX0	(0x0U << BTIMx_BTCFG2_INSEL1_Pos)
#define	BTIMx_BTCFG2_INSEL1_UART_RX1	(0x1U << BTIMx_BTCFG2_INSEL1_Pos)
#define	BTIMx_BTCFG2_INSEL1_UART_RX2	(0x2U << BTIMx_BTCFG2_INSEL1_Pos)
#define	BTIMx_BTCFG2_INSEL1_RCLP	(0x3U << BTIMx_BTCFG2_INSEL1_Pos)

#define	BTIMx_BTPRES_PRESCALE_Pos	0	/* 输入Group1的预分频寄存器 */
#define	BTIMx_BTPRES_PRESCALE_Msk	(0xffU << BTIMx_BTPRES_PRESCALE_Pos)

#define	BTIMx_BTLOADCR_LHEN_Pos	4
#define	BTIMx_BTLOADCR_LHEN_Msk	(0x1U << BTIMx_BTLOADCR_LHEN_Pos)
	/* 0：写0无效 */
	/* 1：写1表示将预置数寄存器PRESETH和加载寄存器LOADH分别加载到CNTH和CMPH */

#define	BTIMx_BTLOADCR_LLEN_Pos	0
#define	BTIMx_BTLOADCR_LLEN_Msk	(0x1U << BTIMx_BTLOADCR_LLEN_Pos)
	/* 0：写0无效 */
	/* 1：写1表示将预置数寄存器PRESETL和加载寄存器LOADL分别加载到CNTL和CMPL */

#define	BTIMx_BTCNTL_CNTL_Pos	0	/* 计数器低位计数值寄存器 */
#define	BTIMx_BTCNTL_CNTL_Msk	(0xffU << BTIMx_BTCNTL_CNTL_Pos)

#define	BTIMx_BTCNTH_CNTH_Pos	0	/* 计数器高位计数值寄存器 */
#define	BTIMx_BTCNTH_CNTH_Msk	(0xffU << BTIMx_BTCNTH_CNTH_Pos)

#define	BTIMx_BTPRESET_PRESETH_Pos	8	/* 计数器高位预置数寄存器 */
#define	BTIMx_BTPRESET_PRESETH_Msk	(0xffU << BTIMx_BTPRESET_PRESETH_Pos)

#define	BTIMx_BTPRESET_PRESETL_Pos	0	/* 计数器低位预置数寄存器 */
#define	BTIMx_BTPRESET_PRESETL_Msk	(0xffU << BTIMx_BTPRESET_PRESETL_Pos)

#define	BTIMx_BTLOADL_LOADL_Pos	0	/* 计数器低位加载寄存器 */
#define	BTIMx_BTLOADL_LOADL_Msk	(0xffU << BTIMx_BTLOADL_LOADL_Pos)

#define	BTIMx_BTLOADH_LOADH_Pos	0	/* 计数器高位加载寄存器 */
#define	BTIMx_BTLOADH_LOADH_Msk	(0xffU << BTIMx_BTLOADH_LOADH_Pos)

#define	BTIMx_BTCMPL_CMPL_Pos	0	/* 计数器低位比较寄存器 */
#define	BTIMx_BTCMPL_CMPL_Msk	(0xffU << BTIMx_BTCMPL_CMPL_Pos)

#define	BTIMx_BTCMPH_CMPH_Pos	0	/* 计数器高位比较寄存器 */
#define	BTIMx_BTCMPH_CMPH_Msk	(0xffU << BTIMx_BTCMPH_CMPH_Pos)

#define	BTIMx_BTOUTCNT_OUTCNT_Pos	0	/* 计数器输出脉冲宽度计数器 */
#define	BTIMx_BTOUTCNT_OUTCNT_Msk	(0x7ffU << BTIMx_BTOUTCNT_OUTCNT_Pos)

#define	BTIMx_BTOCR_OUTCLR_Pos	5	/* 输出清零 */
#define	BTIMx_BTOCR_OUTCLR_Msk	(0x1U << BTIMx_BTOCR_OUTCLR_Pos)
#define	BTIMx_BTOCR_OUTCLR_	(0x0U << BTIMx_BTOCR_OUTCLR_Pos)	/* 写0无效 */
#define	BTIMx_BTOCR_OUTCLR_CLR	(0x1U << BTIMx_BTOCR_OUTCLR_Pos)	/* 输出清零 */

#define	BTIMx_BTOCR_OUTINV_Pos	4	/* 输出电平反向选择 */
#define	BTIMx_BTOCR_OUTINV_Msk	(0x1U << BTIMx_BTOCR_OUTINV_Pos)
#define	BTIMx_BTOCR_OUTINV_NOT_ANTI	(0x0U << BTIMx_BTOCR_OUTINV_Pos)	/* 输出电平不取反 */
#define	BTIMx_BTOCR_OUTINV_ANTI	(0x1U << BTIMx_BTOCR_OUTINV_Pos)	/* 输出电平取反 */

#define	BTIMx_BTOCR_OUTMOD_Pos	3	/* 输出模式选择 */
#define	BTIMx_BTOCR_OUTMOD_Msk	(0x1U << BTIMx_BTOCR_OUTMOD_Pos)
#define	BTIMx_BTOCR_OUTMOD_PULSE	(0x0U << BTIMx_BTOCR_OUTMOD_Pos)	/* 输出脉冲，宽度可调 */
#define	BTIMx_BTOCR_OUTMOD_ANTI_LEVEL	(0x1U << BTIMx_BTOCR_OUTMOD_Pos)	/* 输出之前的反向电平 */

#define	BTIMx_BTOCR_OUTSEL_Pos	0	/* 输出信号源选择 */
#define	BTIMx_BTOCR_OUTSEL_Msk	(0x7U << BTIMx_BTOCR_OUTSEL_Pos)
#define	BTIMx_BTOCR_OUTSEL_CMPH	(0x0U << BTIMx_BTOCR_OUTSEL_Pos)	/* 输出高位计数器比较信号，仅计数模式有效 */
#define	BTIMx_BTOCR_OUTSEL_CMPL	(0x1U << BTIMx_BTOCR_OUTSEL_Pos)	/* 输出低位计数器比较信号，仅计数模式有效 */
#define	BTIMx_BTOCR_OUTSEL_GROUP1	(0x2U << BTIMx_BTOCR_OUTSEL_Pos)	/* 输出Group1的输入信号，计数，捕捉模式有效 */
#define	BTIMx_BTOCR_OUTSEL_GROUP2	(0x3U << BTIMx_BTOCR_OUTSEL_Pos)	/* 输出Group2的输入信号，计数，捕捉模式有效 */
#define	BTIMx_BTOCR_OUTSEL_PWM	(0x4U << BTIMx_BTOCR_OUTSEL_Pos)	/* PWM输出 */

#define	BTIMx_BTIE_CMPHIE_Pos	4	/* 定时器高位比较发生信号 */
#define	BTIMx_BTIE_CMPHIE_Msk	(0x1U << BTIMx_BTIE_CMPHIE_Pos)
	/* 0：中断禁止 */
	/* 1：中断使能 */

#define	BTIMx_BTIE_CMPLIE_Pos	3	/* 定时器低位比较发生信号 */
#define	BTIMx_BTIE_CMPLIE_Msk	(0x1U << BTIMx_BTIE_CMPLIE_Pos)
	/* 0：中断禁止 */
	/* 1：中断使能 */

#define	BTIMx_BTIE_OVHIE_Pos	2	/* 定时器高位溢出信号 */
#define	BTIMx_BTIE_OVHIE_Msk	(0x1U << BTIMx_BTIE_OVHIE_Pos)
	/* 0：中断禁止 */
	/* 1：中断使能 */

#define	BTIMx_BTIE_OVLIE_Pos	1	/* 定时器低位溢出信号 */
#define	BTIMx_BTIE_OVLIE_Msk	(0x1U << BTIMx_BTIE_OVLIE_Pos)
	/* 0：中断禁止 */
	/* 1：中断使能 */

#define	BTIMx_BTIE_CAPIE_Pos	0	/* 定时器捕捉产生信号 */
#define	BTIMx_BTIE_CAPIE_Msk	(0x1U << BTIMx_BTIE_CAPIE_Pos)
	/* 0：中断禁止 */
	/* 1：中断使能 */

#define	BTIMx_BTIF_EDGESTA_Pos	5	/* 捕捉沿状态 */
#define	BTIMx_BTIF_EDGESTA_Msk	(0x1U << BTIMx_BTIF_EDGESTA_Pos)

#define	BTIMx_BTIF_CMPHIF_Pos	4	/* 高位比较发生信号 */
#define	BTIMx_BTIF_CMPHIF_Msk	(0x1U << BTIMx_BTIF_CMPHIF_Pos)

#define	BTIMx_BTIF_CMPLIF_Pos	3	/* 低位比较发生信号 */
#define	BTIMx_BTIF_CMPLIF_Msk	(0x1U << BTIMx_BTIF_CMPLIF_Pos)

#define	BTIMx_BTIF_OVHIF_Pos	2	/* 定时器高位溢出信号 */
#define	BTIMx_BTIF_OVHIF_Msk	(0x1U << BTIMx_BTIF_OVHIF_Pos)

#define	BTIMx_BTIF_OVLIF_Pos	1	/* 定时器低位溢出信号 */
#define	BTIMx_BTIF_OVLIF_Msk	(0x1U << BTIMx_BTIF_OVLIF_Pos)

#define	BTIMx_BTIF_CAPIF_Pos	0	/* 定时器捕捉产生信号 */
#define	BTIMx_BTIF_CAPIF_Msk	(0x1U << BTIMx_BTIF_CAPIF_Pos)
//Macro_End

/* Exported functions --------------------------------------------------------*/ 
extern void BTIMx_Deinit(BTIMx_Type* BTIMx);

/* 高位计数器启动控制 相关函数 */
extern void BTIMx_BTCR1_CHEN_Setable(BTIMx_Type* BTIMx, FunState NewState);
extern FunState BTIMx_BTCR1_CHEN_Getable(BTIMx_Type* BTIMx);

/* 低位计数器启动控制 相关函数 */
extern void BTIMx_BTCR1_CLEN_Setable(BTIMx_Type* BTIMx, FunState NewState);
extern FunState BTIMx_BTCR1_CLEN_Getable(BTIMx_Type* BTIMx);

/* 工作模式选择 相关函数 */
extern void BTIMx_BTCR1_MODE_Set(BTIMx_Type* BTIMx, uint32_t SetValue);
extern uint32_t BTIMx_BTCR1_MODE_Get(BTIMx_Type* BTIMx);

/* 计数沿或捕捉沿选择 相关函数 */
extern void BTIMx_BTCR1_EDGESEL_Set(BTIMx_Type* BTIMx, uint32_t SetValue);
extern uint32_t BTIMx_BTCR1_EDGESEL_Get(BTIMx_Type* BTIMx);

/* 捕捉模式控制（仅捕捉模式下有效） 相关函数 */
extern void BTIMx_BTCR1_CAPMOD_Set(BTIMx_Type* BTIMx, uint32_t SetValue);
extern uint32_t BTIMx_BTCR1_CAPMOD_Get(BTIMx_Type* BTIMx);

/* 带清零捕捉模式控制 相关函数 */
extern void BTIMx_BTCR1_CAPCLR_Set(BTIMx_Type* BTIMx, uint32_t SetValue);
extern uint32_t BTIMx_BTCR1_CAPCLR_Get(BTIMx_Type* BTIMx);

/* 单次捕捉控制 相关函数 */
extern void BTIMx_BTCR1_CAPONCE_Set(BTIMx_Type* BTIMx, uint32_t SetValue);
extern uint32_t BTIMx_BTCR1_CAPONCE_Get(BTIMx_Type* BTIMx);

/* PWM模式输出 相关函数 */
extern void BTIMx_BTCR1_PWM_Setable(BTIMx_Type* BTIMx, FunState NewState);
extern FunState BTIMx_BTCR1_PWM_Getable(BTIMx_Type* BTIMx);

/* 计数器内部计数源信号选择 相关函数 */
extern void BTIMx_BTCR2_SIG2SEL_Set(BTIMx_Type* BTIMx, uint32_t SetValue);
extern uint32_t BTIMx_BTCR2_SIG2SEL_Get(BTIMx_Type* BTIMx);

/* 计数器内部捕捉源信号选择 相关函数 */
extern void BTIMx_BTCR2_SIG1SEL_Set(BTIMx_Type* BTIMx, uint32_t SetValue);
extern uint32_t BTIMx_BTCR2_SIG1SEL_Get(BTIMx_Type* BTIMx);

/* 高位计数器源选择 相关函数 */
extern void BTIMx_BTCR2_CNTHSEL_Set(BTIMx_Type* BTIMx, uint32_t SetValue);
extern uint32_t BTIMx_BTCR2_CNTHSEL_Get(BTIMx_Type* BTIMx);

/* 外部输入DIR控制使能 相关函数 */
extern void BTIMx_BTCR2_DIREN_Setable(BTIMx_Type* BTIMx, FunState NewState);
extern FunState BTIMx_BTCR2_DIREN_Getable(BTIMx_Type* BTIMx);

/* 内部DIR控制信号使能 相关函数 */
extern void BTIMx_BTCR2_STDIR_Setable(BTIMx_Type* BTIMx, FunState NewState);
extern FunState BTIMx_BTCR2_STDIR_Getable(BTIMx_Type* BTIMx);

/* 低位计数器使能控制选择信号 相关函数 */
extern void BTIMx_BTCR2_SRCSEL_Set(BTIMx_Type* BTIMx, uint32_t SetValue);
extern uint32_t BTIMx_BTCR2_SRCSEL_Get(BTIMx_Type* BTIMx);

/* 输入信号2极性选择 相关函数 */
extern void BTIMx_BTCR2_DIRPO_Set(BTIMx_Type* BTIMx, uint32_t SetValue);
extern uint32_t BTIMx_BTCR2_DIRPO_Get(BTIMx_Type* BTIMx);

/* RTCOUT2信号控制2 相关函数 */
extern void BTIMx_BTCFG1_RTCSEL2_Set(BTIMx_Type* BTIMx, uint32_t SetValue);
extern uint32_t BTIMx_BTCFG1_RTCSEL2_Get(BTIMx_Type* BTIMx);

/* RTCOUT1信号控制1 相关函数 */
extern void BTIMx_BTCFG1_RTCSEL1_Set(BTIMx_Type* BTIMx, uint32_t SetValue);
extern uint32_t BTIMx_BTCFG1_RTCSEL1_Get(BTIMx_Type* BTIMx);

/* Group2信号选择控制 相关函数 */
extern void BTIMx_BTCFG1_GRP2SEL_Set(BTIMx_Type* BTIMx, uint32_t SetValue);
extern uint32_t BTIMx_BTCFG1_GRP2SEL_Get(BTIMx_Type* BTIMx);
extern void BTIMx_BTCFG1_GRP1SEL_Set(BTIMx_Type* BTIMx, uint32_t SetValue);
extern uint32_t BTIMx_BTCFG1_GRP1SEL_Get(BTIMx_Type* BTIMx);

/* 外部输入信号选择控制2 相关函数 */
extern void BTIMx_BTCFG2_EXSEL2_Set(BTIMx_Type* BTIMx, uint32_t SetValue);
extern uint32_t BTIMx_BTCFG2_EXSEL2_Get(BTIMx_Type* BTIMx);

/* 外部输入信号选择控制1 相关函数 */
extern void BTIMx_BTCFG2_EXSEL1_Set(BTIMx_Type* BTIMx, uint32_t SetValue);
extern uint32_t BTIMx_BTCFG2_EXSEL1_Get(BTIMx_Type* BTIMx);

/* 内部输入信号选择控制2 相关函数 */
extern void BTIMx_BTCFG2_INSEL2_Set(BTIMx_Type* BTIMx, uint32_t SetValue);
extern uint32_t BTIMx_BTCFG2_INSEL2_Get(BTIMx_Type* BTIMx);
extern void BTIMx_BTCFG2_INSEL1_Set(BTIMx_Type* BTIMx, uint32_t SetValue);
extern uint32_t BTIMx_BTCFG2_INSEL1_Get(BTIMx_Type* BTIMx);

/* 输入Group1的预分频寄存器 相关函数 */
extern void BTIMx_BTPRES_Write(BTIMx_Type* BTIMx, uint32_t SetValue);
extern uint32_t BTIMx_BTPRES_Read(BTIMx_Type* BTIMx);
extern void BTIMx_BTLOADCR_LHEN_Setable(BTIMx_Type* BTIMx, FunState NewState);
extern void BTIMx_BTLOADCR_LLEN_Setable(BTIMx_Type* BTIMx, FunState NewState);

/* 计数器低位计数值寄存器 相关函数 */
extern uint32_t BTIMx_BTCNTL_Read(BTIMx_Type* BTIMx);

/* 计数器高位计数值寄存器 相关函数 */
extern uint32_t BTIMx_BTCNTH_Read(BTIMx_Type* BTIMx);

/* 计数器高位预置数寄存器 相关函数 */
extern void BTIMx_BTPRESET_PRESETH_Set(BTIMx_Type* BTIMx, uint32_t SetValue);
extern uint32_t BTIMx_BTPRESET_PRESETH_Get(BTIMx_Type* BTIMx);

/* 计数器低位预置数寄存器 相关函数 */
extern void BTIMx_BTPRESET_PRESETL_Set(BTIMx_Type* BTIMx, uint32_t SetValue);
extern uint32_t BTIMx_BTPRESET_PRESETL_Get(BTIMx_Type* BTIMx);

/* 计数器低位加载寄存器 相关函数 */
extern void BTIMx_BTLOADL_Write(BTIMx_Type* BTIMx, uint32_t SetValue);
extern uint32_t BTIMx_BTLOADL_Read(BTIMx_Type* BTIMx);

/* 计数器高位加载寄存器 相关函数 */
extern void BTIMx_BTLOADH_Write(BTIMx_Type* BTIMx, uint32_t SetValue);
extern uint32_t BTIMx_BTLOADH_Read(BTIMx_Type* BTIMx);

/* 计数器低位比较寄存器 相关函数 */
extern void BTIMx_BTCMPL_Write(BTIMx_Type* BTIMx, uint32_t SetValue);
extern uint32_t BTIMx_BTCMPL_Read(BTIMx_Type* BTIMx);

/* 计数器高位比较寄存器 相关函数 */
extern void BTIMx_BTCMPH_Write(BTIMx_Type* BTIMx, uint32_t SetValue);
extern uint32_t BTIMx_BTCMPH_Read(BTIMx_Type* BTIMx);

/* 计数器输出脉冲宽度计数器 相关函数 */
extern void BTIMx_BTOUTCNT_Write(BTIMx_Type* BTIMx, uint32_t SetValue);
extern uint32_t BTIMx_BTOUTCNT_Read(BTIMx_Type* BTIMx);

/* 输出清零 相关函数 */
extern void BTIMx_BTOCR_OUTCLR_Set(BTIMx_Type* BTIMx, uint32_t SetValue);
extern uint32_t BTIMx_BTOCR_OUTCLR_Get(BTIMx_Type* BTIMx);

/* 输出电平反向选择 相关函数 */
extern void BTIMx_BTOCR_OUTINV_Set(BTIMx_Type* BTIMx, uint32_t SetValue);
extern uint32_t BTIMx_BTOCR_OUTINV_Get(BTIMx_Type* BTIMx);

/* 输出模式选择 相关函数 */
extern void BTIMx_BTOCR_OUTMOD_Set(BTIMx_Type* BTIMx, uint32_t SetValue);
extern uint32_t BTIMx_BTOCR_OUTMOD_Get(BTIMx_Type* BTIMx);

/* 输出信号源选择 相关函数 */
extern void BTIMx_BTOCR_OUTSEL_Set(BTIMx_Type* BTIMx, uint32_t SetValue);
extern uint32_t BTIMx_BTOCR_OUTSEL_Get(BTIMx_Type* BTIMx);

/* 定时器高位比较发生信号 相关函数 */
extern void BTIMx_BTIE_CMPHIE_Setable(BTIMx_Type* BTIMx, FunState NewState);
extern FunState BTIMx_BTIE_CMPHIE_Getable(BTIMx_Type* BTIMx);

/* 定时器低位比较发生信号 相关函数 */
extern void BTIMx_BTIE_CMPLIE_Setable(BTIMx_Type* BTIMx, FunState NewState);
extern FunState BTIMx_BTIE_CMPLIE_Getable(BTIMx_Type* BTIMx);

/* 定时器高位溢出信号 相关函数 */
extern void BTIMx_BTIE_OVHIE_Setable(BTIMx_Type* BTIMx, FunState NewState);
extern FunState BTIMx_BTIE_OVHIE_Getable(BTIMx_Type* BTIMx);

/* 定时器低位溢出信号 相关函数 */
extern void BTIMx_BTIE_OVLIE_Setable(BTIMx_Type* BTIMx, FunState NewState);
extern FunState BTIMx_BTIE_OVLIE_Getable(BTIMx_Type* BTIMx);

/* 定时器捕捉产生信号 相关函数 */
extern void BTIMx_BTIE_CAPIE_Setable(BTIMx_Type* BTIMx, FunState NewState);
extern FunState BTIMx_BTIE_CAPIE_Getable(BTIMx_Type* BTIMx);

/* 捕捉沿状态 相关函数 */
extern void BTIMx_BTIF_EDGESTA_Clr(BTIMx_Type* BTIMx);
extern FlagStatus BTIMx_BTIF_EDGESTA_Chk(BTIMx_Type* BTIMx);

/* 高位比较发生信号 相关函数 */
extern void BTIMx_BTIF_CMPHIF_Clr(BTIMx_Type* BTIMx);
extern FlagStatus BTIMx_BTIF_CMPHIF_Chk(BTIMx_Type* BTIMx);

/* 低位比较发生信号 相关函数 */
extern void BTIMx_BTIF_CMPLIF_Clr(BTIMx_Type* BTIMx);
extern FlagStatus BTIMx_BTIF_CMPLIF_Chk(BTIMx_Type* BTIMx);

/* 定时器高位溢出信号 相关函数 */
extern void BTIMx_BTIF_OVHIF_Clr(BTIMx_Type* BTIMx);
extern FlagStatus BTIMx_BTIF_OVHIF_Chk(BTIMx_Type* BTIMx);

/* 定时器低位溢出信号 相关函数 */
extern void BTIMx_BTIF_OVLIF_Clr(BTIMx_Type* BTIMx);
extern FlagStatus BTIMx_BTIF_OVLIF_Chk(BTIMx_Type* BTIMx);

/* 定时器捕捉产生信号 相关函数 */
extern void BTIMx_BTIF_CAPIF_Clr(BTIMx_Type* BTIMx);
extern FlagStatus BTIMx_BTIF_CAPIF_Chk(BTIMx_Type* BTIMx);
//Announce_End

extern void BTIMx_Init(BTIMx_Type* BTIMx, BTIM_InitTypeDef* para);//btimer初始化


#ifdef __cplusplus
}
#endif

#endif /* __FM33A0XX_BTIM_H */



/************************ (C) COPYRIGHT FMSHelectronics *****END OF FILE****/



