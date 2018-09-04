/**
  ******************************************************************************
  * @file    fm33a0xx_scu.h
  * @author  FM33a0xx Application Team
  * @version V2.0.1
  * @date    7-12-2017
  * @brief   This file contains all the functions prototypes for the SCU & DCU firmware library.  
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FM33A0XX_SCU_H
#define __FM33A0XX_SCU_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "FM33A0XX.h"
	 
/** @addtogroup FM33a0xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup SCU
  * @{
  */ 

/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
	 
//Macro_START
//2018-04-02-13-22-16
//FM33A0XX_Driver_Gen_V1.4

#define	SCU_MCUDBGCR_DBG_ET4_STOP_Pos	13	/* Debug状态下ET4使能控制 */
#define	SCU_MCUDBGCR_DBG_ET4_STOP_Msk	(0x1U << SCU_MCUDBGCR_DBG_ET4_STOP_Pos)
	/* 0：Debug时保持ET4原来状态 */
	/* 1：Debug时关闭ET4 */

#define	SCU_MCUDBGCR_DBG_ET3_STOP_Pos	12	/* Debug状态下ET3使能控制 */
#define	SCU_MCUDBGCR_DBG_ET3_STOP_Msk	(0x1U << SCU_MCUDBGCR_DBG_ET3_STOP_Pos)
	/* 0：Debug时保持ET3原来状态 */
	/* 1：Debug时关闭ET3 */

#define	SCU_MCUDBGCR_DBG_ET2_STOP_Pos	11	/* Debug状态下ET2使能控制 */
#define	SCU_MCUDBGCR_DBG_ET2_STOP_Msk	(0x1U << SCU_MCUDBGCR_DBG_ET2_STOP_Pos)
	/* 0：Debug时保持ET2原来状态 */
	/* 1：Debug时关闭ET2 */

#define	SCU_MCUDBGCR_DBG_ET1_STOP_Pos	10	/* Debug状态下ET1使能控制 */
#define	SCU_MCUDBGCR_DBG_ET1_STOP_Msk	(0x1U << SCU_MCUDBGCR_DBG_ET1_STOP_Pos)
	/* 0：Debug时保持ET1原来状态 */
	/* 1：Debug时关闭ET1 */

#define	SCU_MCUDBGCR_DBG_BT2_STOP_Pos	9	/* Debug状态下BT2使能控制 */
#define	SCU_MCUDBGCR_DBG_BT2_STOP_Msk	(0x1U << SCU_MCUDBGCR_DBG_BT2_STOP_Pos)
	/* 0：Debug时保持BT2原来状态 */
	/* 1：Debug时关闭BT2 */

#define	SCU_MCUDBGCR_DBG_BT1_STOP_Pos	8	/* Debug状态下BT1使能控制 */
#define	SCU_MCUDBGCR_DBG_BT1_STOP_Msk	(0x1U << SCU_MCUDBGCR_DBG_BT1_STOP_Pos)
	/* 0：Debug时保持BT1原来状态 */
	/* 1：Debug时关闭BT1 */

#define	SCU_MCUDBGCR_DBG_WWDT_STOP_Pos	1	/* Debug状态下WWDT使能控制 */
#define	SCU_MCUDBGCR_DBG_WWDT_STOP_Msk	(0x1U << SCU_MCUDBGCR_DBG_WWDT_STOP_Pos)
	/* 0：Debug时保持WWDT原来状态 */
	/* 1：Debug时关闭WWDT */

#define	SCU_MCUDBGCR_DBG_IWDT_STOP_Pos	0	/* Debug状态下IWDT使能控制 */
#define	SCU_MCUDBGCR_DBG_IWDT_STOP_Msk	(0x1U << SCU_MCUDBGCR_DBG_IWDT_STOP_Pos)
	/* 0：Debug时保持IWDT开启 */
	/* 1：Debug时关闭IWDT */

#define	SCU_HDFFLAG_DABORT_ADDR_FLAG_Pos	6	/* 地址非对齐访问错误标志 */
#define	SCU_HDFFLAG_DABORT_ADDR_FLAG_Msk	(0x1U << SCU_HDFFLAG_DABORT_ADDR_FLAG_Pos)

#define	SCU_HDFFLAG_DABORT_RESP_FLAG_Pos	5	/* 非法地址访问错误标志 */
#define	SCU_HDFFLAG_DABORT_RESP_FLAG_Msk	(0x1U << SCU_HDFFLAG_DABORT_RESP_FLAG_Pos)

#define	SCU_HDFFLAG_SVCUNDEF_FLAG_Pos	4	/* SVC instructions未定义标志 */
#define	SCU_HDFFLAG_SVCUNDEF_FLAG_Msk	(0x1U << SCU_HDFFLAG_SVCUNDEF_FLAG_Pos)

#define	SCU_HDFFLAG_BKPT_FLAG_Pos	3	/* 执行BKPT指令标志 */
#define	SCU_HDFFLAG_BKPT_FLAG_Msk	(0x1U << SCU_HDFFLAG_BKPT_FLAG_Pos)

#define	SCU_HDFFLAG_TBIT_FLAG_Pos	2	/* Thumb-State标志 */
#define	SCU_HDFFLAG_TBIT_FLAG_Msk	(0x1U << SCU_HDFFLAG_TBIT_FLAG_Pos)

#define	SCU_HDFFLAG_SPECIAL_OP_FLAG_Pos	1	/* 特殊指令标志 */
#define	SCU_HDFFLAG_SPECIAL_OP_FLAG_Msk	(0x1U << SCU_HDFFLAG_SPECIAL_OP_FLAG_Pos)

#define	SCU_HDFFLAG_HDF_REQUEST_FLAG_Pos	0	/* hardfault标志位 */
#define	SCU_HDFFLAG_HDF_REQUEST_FLAG_Msk	(0x1U << SCU_HDFFLAG_HDF_REQUEST_FLAG_Pos)
//Macro_End

/* Exported functions --------------------------------------------------------*/ 
extern void SCU_Deinit(void);

/* Debug状态下ET4使能控制 相关函数 */
extern void SCU_MCUDBGCR_DBG_ET4_STOP_Setable(FunState NewState);
extern FunState SCU_MCUDBGCR_DBG_ET4_STOP_Getable(void);

/* Debug状态下ET3使能控制 相关函数 */
extern void SCU_MCUDBGCR_DBG_ET3_STOP_Setable(FunState NewState);
extern FunState SCU_MCUDBGCR_DBG_ET3_STOP_Getable(void);

/* Debug状态下ET2使能控制 相关函数 */
extern void SCU_MCUDBGCR_DBG_ET2_STOP_Setable(FunState NewState);
extern FunState SCU_MCUDBGCR_DBG_ET2_STOP_Getable(void);

/* Debug状态下ET1使能控制 相关函数 */
extern void SCU_MCUDBGCR_DBG_ET1_STOP_Setable(FunState NewState);
extern FunState SCU_MCUDBGCR_DBG_ET1_STOP_Getable(void);

/* Debug状态下BT2使能控制 相关函数 */
extern void SCU_MCUDBGCR_DBG_BT2_STOP_Setable(FunState NewState);
extern FunState SCU_MCUDBGCR_DBG_BT2_STOP_Getable(void);

/* Debug状态下BT1使能控制 相关函数 */
extern void SCU_MCUDBGCR_DBG_BT1_STOP_Setable(FunState NewState);
extern FunState SCU_MCUDBGCR_DBG_BT1_STOP_Getable(void);

/* Debug状态下WWDT使能控制 相关函数 */
extern void SCU_MCUDBGCR_DBG_WWDT_STOP_Setable(FunState NewState);
extern FunState SCU_MCUDBGCR_DBG_WWDT_STOP_Getable(void);

/* Debug状态下IWDT使能控制 相关函数 */
extern void SCU_MCUDBGCR_DBG_IWDT_STOP_Setable(FunState NewState);
extern FunState SCU_MCUDBGCR_DBG_IWDT_STOP_Getable(void);

/* 地址非对齐访问错误标志 相关函数 */
extern void SCU_HDFFLAG_DABORT_ADDR_FLAG_Clr(void);
extern FlagStatus SCU_HDFFLAG_DABORT_ADDR_FLAG_Chk(void);

/* 非法地址访问错误标志 相关函数 */
extern void SCU_HDFFLAG_DABORT_RESP_FLAG_Clr(void);
extern FlagStatus SCU_HDFFLAG_DABORT_RESP_FLAG_Chk(void);

/* SVC instructions未定义标志 相关函数 */
extern void SCU_HDFFLAG_SVCUNDEF_FLAG_Clr(void);
extern FlagStatus SCU_HDFFLAG_SVCUNDEF_FLAG_Chk(void);

/* 执行BKPT指令标志 相关函数 */
extern void SCU_HDFFLAG_BKPT_FLAG_Clr(void);
extern FlagStatus SCU_HDFFLAG_BKPT_FLAG_Chk(void);

/* Thumb-State标志 相关函数 */
extern void SCU_HDFFLAG_TBIT_FLAG_Clr(void);
extern FlagStatus SCU_HDFFLAG_TBIT_FLAG_Chk(void);

/* 特殊指令标志 相关函数 */
extern void SCU_HDFFLAG_SPECIAL_OP_FLAG_Clr(void);
extern FlagStatus SCU_HDFFLAG_SPECIAL_OP_FLAG_Chk(void);

/* hardfault标志位 相关函数 */
extern void SCU_HDFFLAG_HDF_REQUEST_FLAG_Clr(void);
extern FlagStatus SCU_HDFFLAG_HDF_REQUEST_FLAG_Chk(void);
//Announce_End



#ifdef __cplusplus
}
#endif

#endif 



/************************ (C) COPYRIGHT FMSHelectronics *****END OF FILE****/



