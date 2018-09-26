
/**
  ******************************************************************************
  * @file    fm33a0xx_lptim.h
  * @author  FM33a0xx Application Team
  * @version V2.0.1
  * @date    
  * @brief   This file contains all the functions prototypes for the LPTIM firmware library.  
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FM33A0XX_LPTIM_H
#define __FM33A0XX_LPTIM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "FM33A0XX.h"
	 
/** @addtogroup FM385_StdPeriph_Driver
  * @{
  */

/** @addtogroup LPTIM
  * @{
  */ 

/* Exported constants --------------------------------------------------------*/

/** @defgroup LPTIM_Exported_Constants
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  LPTIM Init Structure definition  
  */ 

typedef struct
{
	uint32_t LPTIM_TMODE;  //工作模式
	uint32_t LPTIM_MODE;  //计数模式

	uint32_t LPTIM_PWM;   //脉宽调制模式
	uint32_t LPTIM_POLAR; //时钟极性选择

	uint32_t LPTIM_TRIG_CFG;  //外部触发边沿选择
	FunState LPTIM_FLTEN;  //外部触发滤波使能
	
	uint32_t LPTIM_LPTIN_EDGE;//LPTIN输入边沿选择

	uint32_t LPTIM_CLK_SEL;  //时钟源选择
	uint32_t LPTIM_CLK_DIV;  //时钟分频选择

	uint32_t LPTIM_compare_value;  //比较值
	uint32_t LPTIM_target_value;  //目标值

}LPTIM_InitTypeDef;


//Macro_START
//2018-04-02-13-22-48
//FM33A0XX_Driver_Gen_V1.4

#define	LPTIM_LPTCFG_FLTEN_Pos	15	/* 外部Trigger滤波使能 */
#define	LPTIM_LPTCFG_FLTEN_Msk	(0x1U << LPTIM_LPTCFG_FLTEN_Pos)
	/* 0：有滤波功能 */
	/* 1：无滤波功能 */

#define	LPTIM_LPTCFG_DIVSEL_Pos	10	/* 计数时钟分频选择 */
#define	LPTIM_LPTCFG_DIVSEL_Msk	(0x7U << LPTIM_LPTCFG_DIVSEL_Pos)
#define	LPTIM_LPTCFG_DIVSEL_1	(0x0U << LPTIM_LPTCFG_DIVSEL_Pos)	/* 计数时钟1分频 */
#define	LPTIM_LPTCFG_DIVSEL_2	(0x1U << LPTIM_LPTCFG_DIVSEL_Pos)	/* 计数时钟2分频 */
#define	LPTIM_LPTCFG_DIVSEL_4	(0x2U << LPTIM_LPTCFG_DIVSEL_Pos)	/* 计数时钟4分频 */
#define	LPTIM_LPTCFG_DIVSEL_8	(0x3U << LPTIM_LPTCFG_DIVSEL_Pos)	/* 计数时钟8分频 */
#define	LPTIM_LPTCFG_DIVSEL_16	(0x4U << LPTIM_LPTCFG_DIVSEL_Pos)	/* 计数时钟16分频 */
#define	LPTIM_LPTCFG_DIVSEL_32	(0x5U << LPTIM_LPTCFG_DIVSEL_Pos)	/* 计数时钟32分频 */
#define	LPTIM_LPTCFG_DIVSEL_64	(0x6U << LPTIM_LPTCFG_DIVSEL_Pos)	/* 计数时钟64分频 */
#define	LPTIM_LPTCFG_DIVSEL_128	(0x7U << LPTIM_LPTCFG_DIVSEL_Pos)	/* 计数时钟128分频 */

#define	LPTIM_LPTCFG_CLKSEL_Pos	8	/* 时钟源选择 */
#define	LPTIM_LPTCFG_CLKSEL_Msk	(0x3U << LPTIM_LPTCFG_CLKSEL_Pos)
#define	LPTIM_LPTCFG_CLKSEL_LSCLK	(0x0U << LPTIM_LPTCFG_CLKSEL_Pos)	/* 时钟源选择LSCLK作为计数时钟 */
#define	LPTIM_LPTCFG_CLKSEL_RCLP	(0x1U << LPTIM_LPTCFG_CLKSEL_Pos)	/* 时钟源选择RCLP作为计数时钟 */
#define	LPTIM_LPTCFG_CLKSEL_PCLK	(0x2U << LPTIM_LPTCFG_CLKSEL_Pos)	/* 时钟源选择PCLK作为计数时钟 */
#define	LPTIM_LPTCFG_CLKSEL_LPTIN	(0x3U << LPTIM_LPTCFG_CLKSEL_Pos)	/* 时钟源选择LPTIN作为计数时钟 */

#define	LPTIM_LPTCFG_EDGESEL_Pos	7	/* LPTIN输入边沿选择 */
#define	LPTIM_LPTCFG_EDGESEL_Msk	(0x1U << LPTIM_LPTCFG_EDGESEL_Pos)
#define	LPTIM_LPTCFG_EDGESEL_POS	(0x0U << LPTIM_LPTCFG_EDGESEL_Pos)	/* LPTIN的上升沿计数 */
#define	LPTIM_LPTCFG_EDGESEL_NEG	(0x1U << LPTIM_LPTCFG_EDGESEL_Pos)	/* LPTIN的下降沿计数 */

#define	LPTIM_LPTCFG_TRIGCFG_Pos	5	/* 外部触发边沿选择 */
#define	LPTIM_LPTCFG_TRIGCFG_Msk	(0x3U << LPTIM_LPTCFG_TRIGCFG_Pos)
#define	LPTIM_LPTCFG_TRIGCFG_POS	(0x0U << LPTIM_LPTCFG_TRIGCFG_Pos)	/* 外部输入信号上升沿trigger */
#define	LPTIM_LPTCFG_TRIGCFG_NEG	(0x1U << LPTIM_LPTCFG_TRIGCFG_Pos)	/* 外部输入信号下降沿trigger */
#define	LPTIM_LPTCFG_TRIGCFG_POS_NEG	(0x2U << LPTIM_LPTCFG_TRIGCFG_Pos)	/* 外部输入信号上升下降沿trigger */

#define	LPTIM_LPTCFG_POLARITY_Pos	4	/* 输出波形极性选择 */
#define	LPTIM_LPTCFG_POLARITY_Msk	(0x1U << LPTIM_LPTCFG_POLARITY_Pos)
#define	LPTIM_LPTCFG_POLARITY_POS	(0x0U << LPTIM_LPTCFG_POLARITY_Pos)	/* 第一次计数值=比较值时产生输出波形上升沿 */
#define	LPTIM_LPTCFG_POLARITY_NEG	(0x1U << LPTIM_LPTCFG_POLARITY_Pos)	/* 第一次计数值=比较值时产生输出波形下降沿 */

#define	LPTIM_LPTCFG_PWM_Pos	3	/* 脉宽调制模式 */
#define	LPTIM_LPTCFG_PWM_Msk	(0x1U << LPTIM_LPTCFG_PWM_Pos)
#define	LPTIM_LPTCFG_PWM_TOGGLE	(0x0U << LPTIM_LPTCFG_PWM_Pos)	/* 周期方波输出模式 */
#define	LPTIM_LPTCFG_PWM_PWM	(0x1U << LPTIM_LPTCFG_PWM_Pos)	/* PWM输出模式 */

#define	LPTIM_LPTCFG_MODE_Pos	2	/* 计数模式选择 */
#define	LPTIM_LPTCFG_MODE_Msk	(0x1U << LPTIM_LPTCFG_MODE_Pos)
#define	LPTIM_LPTCFG_MODE_CONTINUE	(0x0U << LPTIM_LPTCFG_MODE_Pos)	/* 连续计数模式 */
#define	LPTIM_LPTCFG_MODE_SINGLE	(0x1U << LPTIM_LPTCFG_MODE_Pos)	/* 单次计数模式 */

#define	LPTIM_LPTCFG_TMODE_Pos	0	/* 工作模式选择 */
#define	LPTIM_LPTCFG_TMODE_Msk	(0x3U << LPTIM_LPTCFG_TMODE_Pos)
#define	LPTIM_LPTCFG_TMODE_PWMIM	(0x0U << LPTIM_LPTCFG_TMODE_Pos)	/* 带波形输出的普通定时器模式 */
#define	LPTIM_LPTCFG_TMODE_INTRIGGER	(0x1U << LPTIM_LPTCFG_TMODE_Pos)	/* Trigger脉冲触发计数模式 */
#define	LPTIM_LPTCFG_TMODE_EXTRIGGER	(0x2U << LPTIM_LPTCFG_TMODE_Pos)	/* 外部异步脉冲计数模式 */
#define	LPTIM_LPTCFG_TMODE_TIMEOUT	(0x3U << LPTIM_LPTCFG_TMODE_Pos)	/* Timeout模式 */

#define	LPTIM_LPTCNT_CNT16_Pos	0
#define	LPTIM_LPTCNT_CNT16_Msk	(0xffffU << LPTIM_LPTCNT_CNT16_Pos)

#define	LPTIM_LPTCMP_COMPARE_REG_Pos	0
#define	LPTIM_LPTCMP_COMPARE_REG_Msk	(0xffffU << LPTIM_LPTCMP_COMPARE_REG_Pos)

#define	LPTIM_LPTTARGET_TARGET_REG_Pos	0
#define	LPTIM_LPTTARGET_TARGET_REG_Msk	(0xffffU << LPTIM_LPTTARGET_TARGET_REG_Pos)

#define	LPTIM_LPTIE_TRIGIE_Pos	2	/* 外部触发中断 */
#define	LPTIM_LPTIE_TRIGIE_Msk	(0x1U << LPTIM_LPTIE_TRIGIE_Pos)
	/* 0：外部触发到来中断禁止 */
	/* 1：外部触发到来中断使能 */

#define	LPTIM_LPTIE_OVIE_Pos	1	/* 计数器溢出中断 */
#define	LPTIM_LPTIE_OVIE_Msk	(0x1U << LPTIM_LPTIE_OVIE_Pos)
	/* 0：计数器溢出中断禁止 */
	/* 1：计数器溢出中断使能 */

#define	LPTIM_LPTIE_COMPIE_Pos	0	/* 比较值匹配中断 */
#define	LPTIM_LPTIE_COMPIE_Msk	(0x1U << LPTIM_LPTIE_COMPIE_Pos)
	/* 0：计数器值和比较值匹配中断禁止 */
	/* 1：计数器值和比较值匹配中断使能 */

#define	LPTIM_LPTIF_TRIGIF_Pos	2	/* 外部触发标志 */
#define	LPTIM_LPTIF_TRIGIF_Msk	(0x1U << LPTIM_LPTIF_TRIGIF_Pos)

#define	LPTIM_LPTIF_OVIF_Pos	1	/* 计数器溢出标志 */
#define	LPTIM_LPTIF_OVIF_Msk	(0x1U << LPTIM_LPTIF_OVIF_Pos)

#define	LPTIM_LPTIF_COMPIF_Pos	0	/* 比较值匹配标志 */
#define	LPTIM_LPTIF_COMPIF_Msk	(0x1U << LPTIM_LPTIF_COMPIF_Pos)

#define	LPTIM_LPTCTRL_LPTEN_Pos	0	/* LPT使能 */
#define	LPTIM_LPTCTRL_LPTEN_Msk	(0x1U << LPTIM_LPTCTRL_LPTEN_Pos)
	/* 0：禁止计数器计数 */
	/* 1：使能计数器计数 */
//Macro_End

/* Exported functions --------------------------------------------------------*/ 
extern void LPTIM_Deinit(void);

/* 外部Trigger滤波使能 相关函数 */
extern void LPTIM_LPTCFG_FLTEN_Setable(FunState NewState);
extern FunState LPTIM_LPTCFG_FLTEN_Getable(void);

/* 计数时钟分频选择 相关函数 */
extern void LPTIM_LPTCFG_DIVSEL_Set(uint32_t SetValue);
extern uint32_t LPTIM_LPTCFG_DIVSEL_Get(void);

/* 时钟源选择 相关函数 */
extern void LPTIM_LPTCFG_CLKSEL_Set(uint32_t SetValue);
extern uint32_t LPTIM_LPTCFG_CLKSEL_Get(void);

/* LPTIN输入边沿选择 相关函数 */
extern void LPTIM_LPTCFG_EDGESEL_Set(uint32_t SetValue);
extern uint32_t LPTIM_LPTCFG_EDGESEL_Get(void);

/* 外部触发边沿选择 相关函数 */
extern void LPTIM_LPTCFG_TRIGCFG_Set(uint32_t SetValue);
extern uint32_t LPTIM_LPTCFG_TRIGCFG_Get(void);

/* 输出波形极性选择 相关函数 */
extern void LPTIM_LPTCFG_POLARITY_Set(uint32_t SetValue);
extern uint32_t LPTIM_LPTCFG_POLARITY_Get(void);

/* 脉宽调制模式 相关函数 */
extern void LPTIM_LPTCFG_PWM_Set(uint32_t SetValue);
extern uint32_t LPTIM_LPTCFG_PWM_Get(void);

/* 计数模式选择 相关函数 */
extern void LPTIM_LPTCFG_MODE_Set(uint32_t SetValue);
extern uint32_t LPTIM_LPTCFG_MODE_Get(void);

/* 工作模式选择 相关函数 */
extern void LPTIM_LPTCFG_TMODE_Set(uint32_t SetValue);
extern uint32_t LPTIM_LPTCFG_TMODE_Get(void);
extern void LPTIM_LPTCNT_Write(uint32_t SetValue);
extern uint32_t LPTIM_LPTCNT_Read(void);
extern void LPTIM_LPTCMP_Write(uint32_t SetValue);
extern uint32_t LPTIM_LPTCMP_Read(void);
extern void LPTIM_LPTTARGET_Write(uint32_t SetValue);
extern uint32_t LPTIM_LPTTARGET_Read(void);

/* 外部触发中断 相关函数 */
extern void LPTIM_LPTIE_TRIGIE_Setable(FunState NewState);
extern FunState LPTIM_LPTIE_TRIGIE_Getable(void);

/* 计数器溢出中断 相关函数 */
extern void LPTIM_LPTIE_OVIE_Setable(FunState NewState);
extern FunState LPTIM_LPTIE_OVIE_Getable(void);

/* 比较值匹配中断 相关函数 */
extern void LPTIM_LPTIE_COMPIE_Setable(FunState NewState);
extern FunState LPTIM_LPTIE_COMPIE_Getable(void);

/* 外部触发标志 相关函数 */
extern void LPTIM_LPTIF_TRIGIF_Clr(void);
extern FlagStatus LPTIM_LPTIF_TRIGIF_Chk(void);

/* 计数器溢出标志 相关函数 */
extern void LPTIM_LPTIF_OVIF_Clr(void);
extern FlagStatus LPTIM_LPTIF_OVIF_Chk(void);

/* 比较值匹配标志 相关函数 */
extern void LPTIM_LPTIF_COMPIF_Clr(void);
extern FlagStatus LPTIM_LPTIF_COMPIF_Chk(void);

/* LPT使能 相关函数 */
extern void LPTIM_LPTCTRL_LPTEN_Setable(FunState NewState);
extern FunState LPTIM_LPTCTRL_LPTEN_Getable(void);
//Announce_End



void LPTIM_Init(LPTIM_InitTypeDef* para);



#ifdef __cplusplus
}
#endif

#endif /* __FM33A0XX_LPTIM_H */




