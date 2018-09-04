/**
  ******************************************************************************
  * @file    fm33a0xx_pmu.h
  * @author  FM33a0xx Application Team
  * @version V2.0.1
  * @date    27-12-2017
  * @brief   This file contains all the functions prototypes for the PMU firmware library.  
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FM33A0xx_PMU_H
#define __FM33A0xx_PMU_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "FM33A0XX.h"
	 
/** @addtogroup FM33a0xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup PMU
  * @{
  */

	 
/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  PMU Init Structure definition  
  */   
  
typedef struct
{
	uint32_t PMOD;				/*!<低功耗模式配置  */	
	uint32_t SLPDP;				/*!<DeepSleep控制寄存器  */
	uint32_t DSLPRAM_EXT;		/*!<DeepSleep模式下RAM数据保持配置  */	
	FunState CVS;				/*!<内核电压降低使能控制  */
	uint32_t XTOFF;				/*!<关闭XTLF，仅在SLEEP/DEEPSLEEP下起作用  */
	uint32_t SCR;				/*!<M0系统控制寄存器，一般配置为0即可  */	
}PMU_SleepCfg_InitTypeDef;


/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

//Macro_START
//2018-04-02-13-22-03
//FM33A0XX_Driver_Gen_V1.4

#define	PMU_LPMCFG_XTOFF_Pos	18	/* 关闭XTLF */
#define	PMU_LPMCFG_XTOFF_Msk	(0x3U << PMU_LPMCFG_XTOFF_Pos)
#define	PMU_LPMCFG_XTOFF_DIS	(0x1U << PMU_LPMCFG_XTOFF_Pos)	/* 01: 保持XTLF开启 */
#define	PMU_LPMCFG_XTOFF_EN	(0x2U << PMU_LPMCFG_XTOFF_Pos)	/* 10: 关闭XTLF */

#define	PMU_LPMCFG_LDO15EN_Pos	17	/* LDO15使能状态，只读 */
#define	PMU_LPMCFG_LDO15EN_Msk	(0x1U << PMU_LPMCFG_LDO15EN_Pos)

#define	PMU_LPMCFG_DSLPRAM_EXT_Pos	10	/* DeepSleep模式下RAM数据保持配置位 */
#define	PMU_LPMCFG_DSLPRAM_EXT_Msk	(0x1U << PMU_LPMCFG_DSLPRAM_EXT_Pos)
#define	PMU_LPMCFG_DSLPRAM_EXT_ALLRAMON	(0x0U << PMU_LPMCFG_DSLPRAM_EXT_Pos)	/* 0：DeepSleep下保持所有RAM数据 */
#define	PMU_LPMCFG_DSLPRAM_EXT_16KRAMON	(0x1U << PMU_LPMCFG_DSLPRAM_EXT_Pos)	/* 1：DeepSleep下仅保持16KB RAM数据 */

#define	PMU_LPMCFG_SLPDP_Pos	9	/* DeepSleep控制寄 */
#define	PMU_LPMCFG_SLPDP_Msk	(0x1U << PMU_LPMCFG_SLPDP_Pos)
#define	PMU_LPMCFG_SLPDP_SLEEP	(0x0U << PMU_LPMCFG_SLPDP_Pos)	/* 0：Sleep模式 */
#define	PMU_LPMCFG_SLPDP_DEEPSLEEP	(0x1U << PMU_LPMCFG_SLPDP_Pos)	/* 1：DeepSleep模式 */

#define	PMU_LPMCFG_CVS_Pos	8	/* 低功耗模式内核电压降低 */
#define	PMU_LPMCFG_CVS_Msk	(0x1U << PMU_LPMCFG_CVS_Pos)
	/* 0：低功耗模式下不使能内核电压调整 */
	/* 1：低功耗模式下降低内核电压 */

#define	PMU_LPMCFG_PMOD_Pos	0	/* 低功耗模式配置寄存器 */
#define	PMU_LPMCFG_PMOD_Msk	(0x3U << PMU_LPMCFG_PMOD_Pos)
#define	PMU_LPMCFG_PMOD_ACTIVE	(0x0U << PMU_LPMCFG_PMOD_Pos)	/* 00: Active mode */
#define	PMU_LPMCFG_PMOD_LPRUN	(0x1U << PMU_LPMCFG_PMOD_Pos)	/* 01: LPRUN mode */
#define	PMU_LPMCFG_PMOD_SLEEP	(0x2U << PMU_LPMCFG_PMOD_Pos)	/* 10: Sleep mode */

#define	PMU_WKDLYCON_TRHR_Pos	0	/* 唤醒时间控制寄存器 */
#define	PMU_WKDLYCON_TRHR_Msk	(0x3U << PMU_WKDLYCON_TRHR_Pos)
#define	PMU_WKDLYCON_TRHR_12US	(0x0U << PMU_WKDLYCON_TRHR_Pos)	/* 00：12us */
#define	PMU_WKDLYCON_TRHR_16US	(0x1U << PMU_WKDLYCON_TRHR_Pos)	/* 01：16us */
#define	PMU_WKDLYCON_TRHR_24US	(0x2U << PMU_WKDLYCON_TRHR_Pos)	/* 10：24us */
#define	PMU_WKDLYCON_TRHR_32US	(0x3U << PMU_WKDLYCON_TRHR_Pos)	/* 11：32us */

#define	PMU_WKPFLAG_DBGWKF_Pos	8	/* CPU Debugger唤醒标志 */
#define	PMU_WKPFLAG_DBGWKF_Msk	(0x1U << PMU_WKPFLAG_DBGWKF_Pos)

#define	PMU_WKPFLAG_WKP7F_Pos	7	/* NWKUP7 Pin唤醒标志 */
#define	PMU_WKPFLAG_WKP7F_Msk	(0x1U << PMU_WKPFLAG_WKP7F_Pos)

#define	PMU_WKPFLAG_WKP6F_Pos	6	/* NWKUP6 Pin唤醒标志 */
#define	PMU_WKPFLAG_WKP6F_Msk	(0x1U << PMU_WKPFLAG_WKP6F_Pos)

#define	PMU_WKPFLAG_WKP5F_Pos	5	/* NWKUP5 Pin唤醒标志 */
#define	PMU_WKPFLAG_WKP5F_Msk	(0x1U << PMU_WKPFLAG_WKP5F_Pos)

#define	PMU_WKPFLAG_WKP4F_Pos	4	/* NWKUP4 Pin唤醒标志 */
#define	PMU_WKPFLAG_WKP4F_Msk	(0x1U << PMU_WKPFLAG_WKP4F_Pos)

#define	PMU_WKPFLAG_WKP3F_Pos	3	/* NWKUP3 Pin唤醒标志 */
#define	PMU_WKPFLAG_WKP3F_Msk	(0x1U << PMU_WKPFLAG_WKP3F_Pos)

#define	PMU_WKPFLAG_WKP2F_Pos	2	/* NWKUP2 Pin唤醒标志 */
#define	PMU_WKPFLAG_WKP2F_Msk	(0x1U << PMU_WKPFLAG_WKP2F_Pos)

#define	PMU_WKPFLAG_WKP1F_Pos	1	/* NWKUP1 Pin唤醒标志 */
#define	PMU_WKPFLAG_WKP1F_Msk	(0x1U << PMU_WKPFLAG_WKP1F_Pos)

#define	PMU_WKPFLAG_WKP0F_Pos	0	/* NWKUP0 Pin唤醒标志 */
#define	PMU_WKPFLAG_WKP0F_Msk	(0x1U << PMU_WKPFLAG_WKP0F_Pos)

#define	PMU_LPREIE_SLPEIE_Pos	1	/* SLEEP错误中断使能 */
#define	PMU_LPREIE_SLPEIE_Msk	(0x1U << PMU_LPREIE_SLPEIE_Pos)
	/* 0：禁止SLEEP错误中断 */
	/* 1：使能SLEEP错误中断 */

#define	PMU_LPREIE_LPREIE_Pos	0	/* LPRUN错误中断使能 */
#define	PMU_LPREIE_LPREIE_Msk	(0x1U << PMU_LPREIE_LPREIE_Pos)
	/* 0：禁止LPRUN错误中断 */
	/* 1：使能LPRUN错误中断 */

#define	PMU_LPREIF_SLPEIF_Pos	1	/* SLEEP错误中断标志，硬件置位，软件写1清零,在PMOD=2’h2后，CPU执行WFI/WFE指令前置位了SLEEPDEEP寄存器时置位 */
#define	PMU_LPREIF_SLPEIF_Msk	(0x1U << PMU_LPREIF_SLPEIF_Pos)

#define	PMU_LPREIF_LPREIF_Pos	0	/* LPRUN错误中断标志，硬件置位，软件写1清零；软件进入LPRUN模式时如果触发了LPREIF，则芯片仍将停留在ACTIVE模式,LPRUN Condition Error，即进入LPRUN时满足如下情况：
1） HSCLK选择不是LSCLK或RCLP，或
2） RCHF、PLL、ADC使能未关闭
 */
#define	PMU_LPREIF_LPREIF_Msk	(0x1U << PMU_LPREIF_LPREIF_Pos)
//Macro_End

/* Exported functions --------------------------------------------------------*/ 
extern void PMU_Deinit(void);

/* 关闭XTLF 相关函数 */
extern void PMU_LPMCFG_XTOFF_Set(uint32_t SetValue);
extern uint32_t PMU_LPMCFG_XTOFF_Get(void);

/* LDO15使能状态，只读 相关函数 */
extern FlagStatus PMU_LPMCFG_LDO15EN_Chk(void);

/* DeepSleep模式下RAM数据保持配置位 相关函数 */
extern void PMU_LPMCFG_DSLPRAM_EXT_Set(uint32_t SetValue);
extern uint32_t PMU_LPMCFG_DSLPRAM_EXT_Get(void);

/* DeepSleep控制寄 相关函数 */
extern void PMU_LPMCFG_SLPDP_Set(uint32_t SetValue);
extern uint32_t PMU_LPMCFG_SLPDP_Get(void);

/* 低功耗模式内核电压降低 相关函数 */
extern void PMU_LPMCFG_CVS_Setable(FunState NewState);
extern FunState PMU_LPMCFG_CVS_Getable(void);

/* 低功耗模式配置寄存器 相关函数 */
extern void PMU_LPMCFG_PMOD_Set(uint32_t SetValue);
extern uint32_t PMU_LPMCFG_PMOD_Get(void);

/* 唤醒时间控制寄存器 相关函数 */
extern void PMU_WKDLYCON_TRHR_Set(uint32_t SetValue);
extern uint32_t PMU_WKDLYCON_TRHR_Get(void);

/* CPU Debugger唤醒标志 相关函数 */
extern void PMU_WKPFLAG_DBGWKF_Clr(void);
extern FlagStatus PMU_WKPFLAG_DBGWKF_Chk(void);

/* SLEEP错误中断使能 相关函数 */
extern void PMU_LPREIE_SLPEIE_Setable(FunState NewState);
extern FunState PMU_LPREIE_SLPEIE_Getable(void);

/* LPRUN错误中断使能 相关函数 */
extern void PMU_LPREIE_LPREIE_Setable(FunState NewState);
extern FunState PMU_LPREIE_LPREIE_Getable(void);

/* SLEEP错误中断标志，硬件置位，软件写1清零,在PMOD=2’h2后，CPU执行WFI/WFE指令前置位了SLEEPDEEP寄存器时置位 相关函数 */
extern void PMU_LPREIF_SLPEIF_Clr(void);
extern FlagStatus PMU_LPREIF_SLPEIF_Chk(void);

/* LPRUN错误中断标志，硬件置位，软件写1清零；软件进入LPRUN模式时如果触发了LPREIF，则芯片仍将停留在ACTIVE模式,LPRUN Condition Error，即进入LPRUN时满足如下情况：
1） HSCLK选择不是LSCLK或RCLP，或
2） RCHF、PLL、ADC使能未关闭
 相关函数 */
extern void PMU_LPREIF_LPREIF_Clr(void);
extern FlagStatus PMU_LPREIF_LPREIF_Chk(void);
//Announce_End


extern void PMU_WKPFLAG_WKPxF_ClrEx(uint32_t NWKPinDef);
extern FlagStatus PMU_WKPFLAG_WKPxF_ChkEx(uint32_t NWKPinDef);
extern void PMU_SleepCfg_Init(PMU_SleepCfg_InitTypeDef* SleepCfg_InitStruct);




#ifdef __cplusplus
}
#endif

#endif /* __FM33A0xx_PMU_H */



/************************ (C) COPYRIGHT FMSHelectronics *****END OF FILE****/



