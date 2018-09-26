/**
  ******************************************************************************
  * @file    fm33a0xx_i2c.h
  * @author  FM33a0xx Application Team
  * @version V2.0.1
  * @date    18-1-2018
  * @brief   This file contains all the functions prototypes for the I2C firmware library.  
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FM33A0XX_I2C_H
#define __FM33A0XX_I2C_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "FM33A0XX.h"

/* Defines--------------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define I2C_SEND_STARTBIT()		I2C_I2CCTRL_SEN_Setable(ENABLE)
#define I2C_SEND_RESTARTBIT()		I2C_I2CCTRL_RSEN_Setable(ENABLE)
#define I2C_SEND_STOPBIT()		I2C_I2CCTRL_PEN_Setable(ENABLE)
#define I2C_SEND_ACKBIT()			I2C_I2CCTRL_ACKEN_Setable(ENABLE)
	 
#define I2C_SEND_ACK_0()			I2C_I2CSTA_ACKDT_Set(I2C_I2CSTA_ACKDT_L)
#define I2C_SEND_ACK_1()			I2C_I2CSTA_ACKDT_Set(I2C_I2CSTA_ACKDT_H)
	 

	 
//Macro_START
//2018-04-02-13-21-22
//FM33A0XX_Driver_Gen_V1.4

#define	I2C_I2CCTRL_SCLHL_Pos	14	/* 当I2C模块禁止时，SCL固定为高低电平控制位 */
#define	I2C_I2CCTRL_SCLHL_Msk	(0x1U << I2C_I2CCTRL_SCLHL_Pos)
	/* 0 = 将SCL信号转化为低电平 */
	/* 1 = 将SCL信号转化为高电平，如非必要，建议常态设为1 */

#define	I2C_I2CCTRL_SDAHL_Pos	13	/* 当I2C模块禁止时，SDA固定为高低电平控制位 */
#define	I2C_I2CCTRL_SDAHL_Msk	(0x1U << I2C_I2CCTRL_SDAHL_Pos)
	/* 0 = 将SDA信号转化为低电平 */
	/* 1 = 将SDA信号转化为高电平，如非必要，建议常态设为1 */

#define	I2C_I2CCTRL_ACKEN_Pos	12	/* 主控接收模式下，主机给从机回应使能位 */
#define	I2C_I2CCTRL_ACKEN_Msk	(0x1U << I2C_I2CCTRL_ACKEN_Pos)
	/* 0 = 主机不响应从机 */
	/* 1 = 主机发送回应ACKDT给从机 */

#define	I2C_I2CCTRL_RCEN_Pos	11	/* 主控接收模式下，接收使能位 */
#define	I2C_I2CCTRL_RCEN_Msk	(0x1U << I2C_I2CCTRL_RCEN_Pos)
	/* 0 = 接收禁止 */
	/* 1 = 主机接收使能 */

#define	I2C_I2CCTRL_PEN_Pos	10	/* STOP时序产生使能控制位 */
#define	I2C_I2CCTRL_PEN_Msk	(0x1U << I2C_I2CCTRL_PEN_Pos)
	/* 0 = STOP时序产生禁止 */
	/* 1 = STOP时序产生使能 */

#define	I2C_I2CCTRL_RSEN_Pos	9	/* Repeated START时序产生使能控制位 */
#define	I2C_I2CCTRL_RSEN_Msk	(0x1U << I2C_I2CCTRL_RSEN_Pos)
	/* 0 = Repeated START时序产生禁止 */
	/* 1 = Repeated START时序产生使能 */

#define	I2C_I2CCTRL_SEN_Pos	8	/* START时序产生使能控制位 */
#define	I2C_I2CCTRL_SEN_Msk	(0x1U << I2C_I2CCTRL_SEN_Pos)
	/* 0 = START时序产生禁止 */
	/* 1 = START时序产生使能 */

#define	I2C_I2CCTRL_I2CEN_Pos	0	/* I2C模块使能控制位 */
#define	I2C_I2CCTRL_I2CEN_Msk	(0x1U << I2C_I2CCTRL_I2CEN_Pos)
	/* 0 = I2C禁止(复位I2C) */
	/* 1 = I2C使能 */

#define	I2C_I2CSTA_WCOL_Pos	6	/* 写冲突检测位 */
#define	I2C_I2CSTA_WCOL_Msk	(0x1U << I2C_I2CSTA_WCOL_Pos)

#define	I2C_I2CSTA_RW_Pos	5	/* I2C接口状态位 */
#define	I2C_I2CSTA_RW_Msk	(0x1U << I2C_I2CSTA_RW_Pos)

#define	I2C_I2CSTA_P_Pos	4	/* STOP标志位，MCU查询后硬件清0 */
#define	I2C_I2CSTA_P_Msk	(0x1U << I2C_I2CSTA_P_Pos)

#define	I2C_I2CSTA_S_Pos	3	/* START标志位，MCU查询后硬件清0 */
#define	I2C_I2CSTA_S_Msk	(0x1U << I2C_I2CSTA_S_Pos)

#define	I2C_I2CSTA_BF_Pos	2	/* 缓冲器满状态位 */
#define	I2C_I2CSTA_BF_Msk	(0x1U << I2C_I2CSTA_BF_Pos)

#define	I2C_I2CSTA_ACKSTA_Pos	1	/* 主控发送模式下，来自从机的回应信号 */
#define	I2C_I2CSTA_ACKSTA_Msk	(0x1U << I2C_I2CSTA_ACKSTA_Pos)

#define	I2C_I2CSTA_ACKDT_Pos	0	/* 主控接收模式下，主机回应信号的状态 */
#define	I2C_I2CSTA_ACKDT_Msk	(0x1U << I2C_I2CSTA_ACKDT_Pos)
#define	I2C_I2CSTA_ACKDT_L	(0x0U << I2C_I2CSTA_ACKDT_Pos)	/* 0 = 主机将给从机的回应为0 */
#define	I2C_I2CSTA_ACKDT_H	(0x1U << I2C_I2CSTA_ACKDT_Pos)	/* 1 = 主机将不给从机回应 */

#define	I2C_I2CBRG_SSPBRG_Pos	0	/* 波特率设置寄存器 */
#define	I2C_I2CBRG_SSPBRG_Msk	(0x1ffU << I2C_I2CBRG_SSPBRG_Pos)

#define	I2C_I2CBUF_SSPBUF_Pos	0	/* 收发缓冲寄存器 */
#define	I2C_I2CBUF_SSPBUF_Msk	(0xffU << I2C_I2CBUF_SSPBUF_Pos)

#define	I2C_I2CIR_I2CIE_Pos	1	/* I2C中断使能控制位 */
#define	I2C_I2CIR_I2CIE_Msk	(0x1U << I2C_I2CIR_I2CIE_Pos)
	/* 0 = I2C中断禁止 */
	/* 1 = I2C中断使能 */

#define	I2C_I2CIR_I2CIF_Pos	0	/* I2C中断标志位 */
#define	I2C_I2CIR_I2CIF_Msk	(0x1U << I2C_I2CIR_I2CIF_Pos)

#define	I2C_I2CFSM_I2CFSM_Pos	0	/* I2C主状态机编码 */
#define	I2C_I2CFSM_I2CFSM_Msk	(0xfU << I2C_I2CFSM_I2CFSM_Pos)
#define	I2C_I2CFSM_I2CFSM_IDLE	(0x0U << I2C_I2CFSM_I2CFSM_Pos)	/* 0000 = IDLE */
#define	I2C_I2CFSM_I2CFSM_START1	(0x1U << I2C_I2CFSM_I2CFSM_Pos)	/* 0001 = START1 */
#define	I2C_I2CFSM_I2CFSM_START2	(0x2U << I2C_I2CFSM_I2CFSM_Pos)	/* 0010 = START2 */
#define	I2C_I2CFSM_I2CFSM_START_DONE	(0x3U << I2C_I2CFSM_I2CFSM_Pos)	/* 0011 = START_DONE */
#define	I2C_I2CFSM_I2CFSM_TX_STATE	(0x4U << I2C_I2CFSM_I2CFSM_Pos)	/* 0100 = TX_STATE */
#define	I2C_I2CFSM_I2CFSM_RXACK	(0x5U << I2C_I2CFSM_I2CFSM_Pos)	/* 0101 = RXACK */
#define	I2C_I2CFSM_I2CFSM_OP_IDLE	(0x6U << I2C_I2CFSM_I2CFSM_Pos)	/* 0110 = OP_IDLE */
#define	I2C_I2CFSM_I2CFSM_STOP1	(0x7U << I2C_I2CFSM_I2CFSM_Pos)	/* 0111 = STOP1 */
#define	I2C_I2CFSM_I2CFSM_STOP2	(0x8U << I2C_I2CFSM_I2CFSM_Pos)	/* 1000 = STOP2 */
#define	I2C_I2CFSM_I2CFSM_STOP_DONE	(0x9U << I2C_I2CFSM_I2CFSM_Pos)	/* 1001 = STOP_DONE */
#define	I2C_I2CFSM_I2CFSM_RTP_START	(0xaU << I2C_I2CFSM_I2CFSM_Pos)	/* 1010 = RTP_START */
#define	I2C_I2CFSM_I2CFSM_RX_STATE	(0xbU << I2C_I2CFSM_I2CFSM_Pos)	/* 1011 = RX_STATE */
#define	I2C_I2CFSM_I2CFSM_ACK_STATE	(0xcU << I2C_I2CFSM_I2CFSM_Pos)	/* 1100 = ACK_STATE */
#define	I2C_I2CFSM_I2CFSM_STOP0	(0xdU << I2C_I2CFSM_I2CFSM_Pos)	/* 1101 = STOP0 */

#define	I2C_I2CERR_ERRIE_Pos	8	/* 错误标志中断使能 */
#define	I2C_I2CERR_ERRIE_Msk	(0x1U << I2C_I2CERR_ERRIE_Pos)

#define	I2C_I2CERR_OIERR_Pos	2	/* OP_IDLE状态下错误标志位 */
#define	I2C_I2CERR_OIERR_Msk	(0x1U << I2C_I2CERR_OIERR_Pos)

#define	I2C_I2CERR_SDERR_Pos	1	/* START_DONE状态下错误标志位 */
#define	I2C_I2CERR_SDERR_Msk	(0x1U << I2C_I2CERR_SDERR_Pos)

#define	I2C_I2CERR_IERR_Pos	0	/* IDLE状态下错误标志位 */
#define	I2C_I2CERR_IERR_Msk	(0x1U << I2C_I2CERR_IERR_Pos)
//Macro_End

/* Exported functions --------------------------------------------------------*/ 
extern void I2C_Deinit(void);

/* 当I2C模块禁止时，SCL固定为高低电平控制位 相关函数 */
extern void I2C_I2CCTRL_SCLHL_Setable(FunState NewState);
extern FunState I2C_I2CCTRL_SCLHL_Getable(void);

/* 当I2C模块禁止时，SDA固定为高低电平控制位 相关函数 */
extern void I2C_I2CCTRL_SDAHL_Setable(FunState NewState);
extern FunState I2C_I2CCTRL_SDAHL_Getable(void);

/* 主控接收模式下，主机给从机回应使能位 相关函数 */
extern void I2C_I2CCTRL_ACKEN_Setable(FunState NewState);
extern FunState I2C_I2CCTRL_ACKEN_Getable(void);

/* 主控接收模式下，接收使能位 相关函数 */
extern void I2C_I2CCTRL_RCEN_Setable(FunState NewState);
extern FunState I2C_I2CCTRL_RCEN_Getable(void);

/* STOP时序产生使能控制位 相关函数 */
extern void I2C_I2CCTRL_PEN_Setable(FunState NewState);

/* Repeated START时序产生使能控制位 相关函数 */
extern void I2C_I2CCTRL_RSEN_Setable(FunState NewState);

/* START时序产生使能控制位 相关函数 */
extern void I2C_I2CCTRL_SEN_Setable(FunState NewState);

/* I2C模块使能控制位 相关函数 */
extern void I2C_I2CCTRL_I2CEN_Setable(FunState NewState);
extern FunState I2C_I2CCTRL_I2CEN_Getable(void);

/* 写冲突检测位 相关函数 */
extern void I2C_I2CSTA_WCOL_Clr(void);
extern FlagStatus I2C_I2CSTA_WCOL_Chk(void);

/* I2C接口状态位 相关函数 */
extern FlagStatus I2C_I2CSTA_RW_Chk(void);

/* STOP标志位，MCU查询后硬件清0 相关函数 */
extern FlagStatus I2C_I2CSTA_P_Chk(void);

/* START标志位，MCU查询后硬件清0 相关函数 */
extern FlagStatus I2C_I2CSTA_S_Chk(void);

/* 缓冲器满状态位 相关函数 */
extern FlagStatus I2C_I2CSTA_BF_Chk(void);

/* 主控发送模式下，来自从机的回应信号 相关函数 */
extern void I2C_I2CSTA_ACKSTA_Clr(void);
extern FlagStatus I2C_I2CSTA_ACKSTA_Chk(void);

/* 主控接收模式下，主机回应信号的状态 相关函数 */
extern void I2C_I2CSTA_ACKDT_Set(uint32_t SetValue);
extern uint32_t I2C_I2CSTA_ACKDT_Get(void);

/* 波特率设置寄存器 相关函数 */
extern void I2C_I2CBRG_Write(uint32_t SetValue);
extern uint32_t I2C_I2CBRG_Read(void);

/* 收发缓冲寄存器 相关函数 */
extern void I2C_I2CBUF_Write(uint32_t SetValue);
extern uint32_t I2C_I2CBUF_Read(void);

/* I2C中断使能控制位 相关函数 */
extern void I2C_I2CIR_I2CIE_Setable(FunState NewState);
extern FunState I2C_I2CIR_I2CIE_Getable(void);

/* I2C中断标志位 相关函数 */
extern void I2C_I2CIR_I2CIF_Clr(void);
extern FlagStatus I2C_I2CIR_I2CIF_Chk(void);

/* I2C主状态机编码 相关函数 */
extern uint32_t I2C_I2CFSM_I2CFSM_Get(void);

/* 错误标志中断使能 相关函数 */
extern void I2C_I2CERR_ERRIE_Setable(FunState NewState);
extern FunState I2C_I2CERR_ERRIE_Getable(void);

/* OP_IDLE状态下错误标志位 相关函数 */
extern void I2C_I2CERR_OIERR_Clr(void);
extern FlagStatus I2C_I2CERR_OIERR_Chk(void);

/* START_DONE状态下错误标志位 相关函数 */
extern void I2C_I2CERR_SDERR_Clr(void);
extern FlagStatus I2C_I2CERR_SDERR_Chk(void);

/* IDLE状态下错误标志位 相关函数 */
extern void I2C_I2CERR_IERR_Clr(void);
extern FlagStatus I2C_I2CERR_IERR_Chk(void);
//Announce_End

extern uint32_t I2C_BaudREG_Calc(uint32_t I2CClk, uint32_t APBClk);


#ifdef __cplusplus
}
#endif

#endif /* __FM33A0XX_I2C_H */



/************************ (C) COPYRIGHT FMSHelectronics *****END OF FILE****/

