/**
  ******************************************************************************
  * @file    fm33a0xx_u7816.h
  * @author  FM33a0xx Application Team
  * @version V2.0.1
  * @date    6-MAR-2017
  * @brief   This file contains all the functions prototypes for the U7816 firmware library.  
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FM33A0XX_U7816_H
#define __FM33A0XX_U7816_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "FM33A0XX.h"

/** @addtogroup FM385_StdPeriph_Driver
  * @{
  */


	 
/* Exported constants --------------------------------------------------------*/

/** @defgroup U7816_Exported_Constants
  * @{
  */	 
/* Exported types ------------------------------------------------------------*/


typedef struct
{
	FunState				TXEN;		//发送使能
	FunState				RXEN;		//接受使能
	FunState				CKOEN;		//U7816时钟CLK输出使能控制
	FunState				HPUAT;		//U7816通道数据发送强上拉电阻自动有效控制
	FunState				HPUEN;		//U7816通道强上拉使能控制
	
	uint32_t				ERSW;		//ERROR SIGNAL宽度选择
	uint32_t				ERSGD;		//ERROR SIGNAL后GUARDTIME宽度选择（仅在发送时有效）
	FunState				BGTEN;		//BGT（block guard time）控制
	uint32_t				REP_T;		//控制接收数据奇偶校验出错时自动重发次数
	uint32_t				PAR;		//奇偶校验类型选择
	FunState				FREN;		//Guard Time长度控制位（发送时严格按照协议2etu）
	FunState				TREPEN;		//缓发送数据奇偶校验错的处理方式选择
	FunState				RREPEN;		//接收数据奇偶校验错的处理方式选择
	FunState				DICONV;		//传输次序反向编码使能
	
	uint32_t				TXEGT;		//发送时插入的EGT时间（以ETU为单位）
	uint32_t				CLKDIV;		//U7816时钟输出分频控制寄存器
	uint32_t				PDIV;		//U7816预分频控制寄存器，控制7816通信分频比（波特率）
	
	FunState				RXIE;		//数据接收中断使能位。对应RX_FLAG中断标志位
	FunState				TXIE;		//数据发送中断使能位。对应TX_FLAG中断标志位
	FunState				LSIE;		//线路状态中断使能位。对应ERROR_FLAG中断标志位
	
}U7816_InitTypeDef;

//Macro_START
//2018-04-12-09-21-42
//FM33A0XX_Driver_Gen_V1.4

#define	U7816x_U7816CTRL_TXEN_Pos	5	/* U7816通道发送使能控制 */
#define	U7816x_U7816CTRL_TXEN_Msk	(0x1U << U7816x_U7816CTRL_TXEN_Pos)
	/* 0 = 通道发送禁止，不可发送数据，并关断输出端口 */
	/* 1 = 通道发送使能，可发送数据 */

#define	U7816x_U7816CTRL_RXEN_Pos	4	/* U7816通道接收使能控制 */
#define	U7816x_U7816CTRL_RXEN_Msk	(0x1U << U7816x_U7816CTRL_RXEN_Pos)
	/* 0 = 通道接收禁止，不可接收数据，并关断输入端口 */
	/* 1 = 通道接收使能，可接收数据 */

#define	U7816x_U7816CTRL_CKOEN_Pos	3	/* U7816时钟CLK输出使能控制 */
#define	U7816x_U7816CTRL_CKOEN_Msk	(0x1U << U7816x_U7816CTRL_CKOEN_Pos)
	/* 0 = 7816时钟输出禁止 */
	/* 1 = 7816时钟输出使能 */

#define	U7816x_U7816CTRL_HPUAT_Pos	2	/* U7816通道数据发送强上拉电阻自动有效控制 */
#define	U7816x_U7816CTRL_HPUAT_Msk	(0x1U << U7816x_U7816CTRL_HPUAT_Pos)
	/* 0 = 数据发送时上拉电阻自动有效功能禁止，上拉电阻由HPUEN，LPUEN控制 */
	/* 1 = 数据发送时上拉电阻自动有效，接收态上拉电阻无效 */

#define	U7816x_U7816CTRL_HPUEN_Pos	1	/* U7816通道强上拉使能控制 */
#define	U7816x_U7816CTRL_HPUEN_Msk	(0x1U << U7816x_U7816CTRL_HPUEN_Pos)
	/* 0 = 强上拉无效 */
	/* 1 = 强上拉有效 */

#define	U7816x_U7816FRC_ERSW_Pos	9	/* ERROR SIGNAL宽度选择 */
#define	U7816x_U7816FRC_ERSW_Msk	(0x3U << U7816x_U7816FRC_ERSW_Pos)
#define	U7816x_U7816FRC_ERSW_1ETU	(0x3U << U7816x_U7816FRC_ERSW_Pos)	/* 11 = ERROR SIGNAL宽度为1ETU; */
#define	U7816x_U7816FRC_ERSW_1P5ETU	(0x2U << U7816x_U7816FRC_ERSW_Pos)	/* 10 = ERROR SIGNAL宽度为1.5ETU; */
#define	U7816x_U7816FRC_ERSW_2ETU	(0x0U << U7816x_U7816FRC_ERSW_Pos)	/* 01/00 = ERROR SIGNAL宽度为2ETU; */

#define	U7816x_U7816FRC_ERSGD_Pos	8	/* ERROR SIGNAL后GUARDTIME宽度选择（仅在发送时有效） */
#define	U7816x_U7816FRC_ERSGD_Msk	(0x1U << U7816x_U7816FRC_ERSGD_Pos)
#define	U7816x_U7816FRC_ERSGD_1PETU	(0x1U << U7816x_U7816FRC_ERSGD_Pos)	/* 1 = ERROR SIGNAL后GUARDTIME为1~1.5ETU。 */
#define	U7816x_U7816FRC_ERSGD_2PETU	(0x0U << U7816x_U7816FRC_ERSGD_Pos)	/* 0 = ERROR SIGNAL后GUARDTIME为2~2.5ETU。 */

#define	U7816x_U7816FRC_BGTEN_Pos	7	/* BGT（block guard time）控制 */
#define	U7816x_U7816FRC_BGTEN_Msk	(0x1U << U7816x_U7816FRC_BGTEN_Pos)
	/* 0 = BGT禁止，不插入Block guard time(22 etu); */
	/* 1 = BGT使能，插入Block guard time(22 etu); */

#define	U7816x_U7816FRC_REP_T_Pos	6	/* 控制接收数据奇偶校验出错时自动重发次数 */
#define	U7816x_U7816FRC_REP_T_Msk	(0x1U << U7816x_U7816FRC_REP_T_Pos)
#define	U7816x_U7816FRC_REP_T_1TIME	(0x0U << U7816x_U7816FRC_REP_T_Pos)	/* 0 = 1次 */
#define	U7816x_U7816FRC_REP_T_3TIME	(0x1U << U7816x_U7816FRC_REP_T_Pos)	/* 1 = 3次 */

#define	U7816x_U7816FRC_PAR_Pos	4	/* 奇偶校验类型选择 */
#define	U7816x_U7816FRC_PAR_Msk	(0x3U << U7816x_U7816FRC_PAR_Pos)
#define	U7816x_U7816FRC_PAR_EVEN	(0x0U << U7816x_U7816FRC_PAR_Pos)	/* 00：Even */
#define	U7816x_U7816FRC_PAR_ODD	(0x1U << U7816x_U7816FRC_PAR_Pos)	/* 01：Odd */
#define	U7816x_U7816FRC_PAR_ALWAYS1	(0x2U << U7816x_U7816FRC_PAR_Pos)	/* 10：Always 1 */
#define	U7816x_U7816FRC_PAR_NONE	(0x3U << U7816x_U7816FRC_PAR_Pos)	/* 11：不校验，处理 */

#define	U7816x_U7816FRC_FREN_Pos	3	/* Guard Time长度控制位（发送时严格按照协议2etu） */
#define	U7816x_U7816FRC_FREN_Msk	(0x1U << U7816x_U7816FRC_FREN_Pos)
	/* 0 = Guard time为2 etu */
	/* 1 = Guard time为1 etu */

#define	U7816x_U7816FRC_TREPEN_Pos	2	/* 缓发送数据奇偶校验错的处理方式选择 */
#define	U7816x_U7816FRC_TREPEN_Msk	(0x1U << U7816x_U7816FRC_TREPEN_Pos)
	/* 0 = 收到Error signal时不进行自动回发，置tx_parity_err标志，直接中断 */
	/* 1 = 收到奇偶校验出错标志（error signal），根据T＝0协议自动进行回发。在单一byte重复发送次数超过REP_T后，置tx_parity_err标志，进行中断 */

#define	U7816x_U7816FRC_RREPEN_Pos	1	/* 接收数据奇偶校验错的处理方式选择 */
#define	U7816x_U7816FRC_RREPEN_Msk	(0x1U << U7816x_U7816FRC_RREPEN_Pos)
	/* 0 = 奇偶校验错，不自动发送ERROR SIGNAL，置RX_PARITY_ERR标志，进行中断 */
	/* 1 = 奇偶校验错，根据T=0协议自动回发ERROR SIGNAL。单一BYTE连续接收次数超过REP_T后，置RX_PARITY_ERR标志，进行中断 */

#define	U7816x_U7816FRC_DICONV_Pos	0	/* 传输次序反向编码使能 */
#define	U7816x_U7816FRC_DICONV_Msk	(0x1U << U7816x_U7816FRC_DICONV_Pos)
	/* 0 = 正向编码，先收发LSB ； (收发数据+校验位)正逻辑电平 */
	/* 1 = 反向编码，先收发MSB；(收发数据+校验位)反逻辑电平 */

#define	U7816x_U7816EGT_TXEGT_Pos	0	/* 发送时插入的EGT时间（以ETU为单位） */
#define	U7816x_U7816EGT_TXEGT_Msk	(0xffU << U7816x_U7816EGT_TXEGT_Pos)

#define	U7816x_U7816CLKDIV_CLKDIV_Pos	0	/* U7816时钟输出分频控制寄存器 */
#define	U7816x_U7816CLKDIV_CLKDIV_Msk	(0x1fU << U7816x_U7816CLKDIV_CLKDIV_Pos)

#define	U7816x_U7816PDIV_PDIV_Pos	0	/* U7816预分频控制寄存器，控制7816通信分频比（波特率） */
#define	U7816x_U7816PDIV_PDIV_Msk	(0xfffU << U7816x_U7816PDIV_PDIV_Pos)

#define	U7816x_U7816RXBUF_RXBUF_Pos	0	/* U7816数据接收缓存寄存器 */
#define	U7816x_U7816RXBUF_RXBUF_Msk	(0xffU << U7816x_U7816RXBUF_RXBUF_Pos)

#define	U7816x_U7816TXBUF_TXBUF_Pos	0	/* U7816数据发送缓存寄存器 */
#define	U7816x_U7816TXBUF_TXBUF_Msk	(0xffU << U7816x_U7816TXBUF_TXBUF_Pos)

#define	U7816x_U7816IE_RXIE_Pos	2	/* 数据接收中断使能位。对应RX_FLAG中断标志位 */
#define	U7816x_U7816IE_RXIE_Msk	(0x1U << U7816x_U7816IE_RXIE_Pos)
	/* 0 = 数据接收中断使能位 */
	/* 1 = 数据接收中断使能位 */

#define	U7816x_U7816IE_TXIE_Pos	1	/* 数据发送中断使能位。对应TX_FLAG中断标志位 */
#define	U7816x_U7816IE_TXIE_Msk	(0x1U << U7816x_U7816IE_TXIE_Pos)
	/* 0 = 数据发送中断禁止 */
	/* 1 = 数据发送中断使能 */

#define	U7816x_U7816IE_LSIE_Pos	0	/* 线路状态中断使能位。对应ERROR_FLAG中断标志位 */
#define	U7816x_U7816IE_LSIE_Msk	(0x1U << U7816x_U7816IE_LSIE_Pos)
	/* 0 = 线路状态中断禁止 */
	/* 1 = 线路状态中断使能 */

#define	U7816x_U7816IF_ERRIF_Pos	2	/* 错误标志，寄存器配置出错或传输过程中出错。硬件置位，清U7816ERR中相应错误标志清零 */
#define	U7816x_U7816IF_ERRIF_Msk	(0x1U << U7816x_U7816IF_ERRIF_Pos)

#define	U7816x_U7816IF_TXIF_Pos	1	/* 发送缓冲区空标志 */
#define	U7816x_U7816IF_TXIF_Msk	(0x1U << U7816x_U7816IF_TXIF_Pos)

#define	U7816x_U7816IF_RXIF_Pos	0	/* 接收完成标志 */
#define	U7816x_U7816IF_RXIF_Msk	(0x1U << U7816x_U7816IF_RXIF_Pos)

#define	U7816x_U7816ERR_TPARERR_Pos	3	/* 发送数据奇偶校验错误标志位 */
#define	U7816x_U7816ERR_TPARERR_Msk	(0x1U << U7816x_U7816ERR_TPARERR_Pos)

#define	U7816x_U7816ERR_RPARERR_Pos	2	/* 接收数据奇偶校验错误标志位 */
#define	U7816x_U7816ERR_RPARERR_Msk	(0x1U << U7816x_U7816ERR_RPARERR_Pos)

#define	U7816x_U7816ERR_FRERR_Pos	1	/* 接收帧格式错误标志位 */
#define	U7816x_U7816ERR_FRERR_Msk	(0x1U << U7816x_U7816ERR_FRERR_Pos)

#define	U7816x_U7816ERR_OVERR_Pos	0	/* 接收溢出错误标志位 */
#define	U7816x_U7816ERR_OVERR_Msk	(0x1U << U7816x_U7816ERR_OVERR_Pos)

#define	U7816x_U7816STA_WAIT_RPT_Pos	2	/* U7816接口发送了错误信号，正在等待对方重发数据.状态机进入发送错误信号状态时置位，收到数据起始位或者进入发送状态时硬件清零;
 */
#define	U7816x_U7816STA_WAIT_RPT_Msk	(0x1U << U7816x_U7816STA_WAIT_RPT_Pos)

#define	U7816x_U7816STA_TXBUSY_Pos	1	/* 发送数据忙标志 */
#define	U7816x_U7816STA_TXBUSY_Msk	(0x1U << U7816x_U7816STA_TXBUSY_Pos)

#define	U7816x_U7816STA_RXBUSY_Pos	0	/* 接收数据忙标志 */
#define	U7816x_U7816STA_RXBUSY_Msk	(0x1U << U7816x_U7816STA_RXBUSY_Pos)
//Macro_End

/* Exported functions --------------------------------------------------------*/ 
extern void U7816x_Deinit(U7816x_Type* U7816x);

/* U7816通道发送使能控制 相关函数 */
extern void U7816x_U7816CTRL_TXEN_Setable(U7816x_Type* U7816x, FunState NewState);
extern FunState U7816x_U7816CTRL_TXEN_Getable(U7816x_Type* U7816x);

/* U7816通道接收使能控制 相关函数 */
extern void U7816x_U7816CTRL_RXEN_Setable(U7816x_Type* U7816x, FunState NewState);
extern FunState U7816x_U7816CTRL_RXEN_Getable(U7816x_Type* U7816x);

/* U7816时钟CLK输出使能控制 相关函数 */
extern void U7816x_U7816CTRL_CKOEN_Setable(U7816x_Type* U7816x, FunState NewState);
extern FunState U7816x_U7816CTRL_CKOEN_Getable(U7816x_Type* U7816x);

/* U7816通道数据发送强上拉电阻自动有效控制 相关函数 */
extern void U7816x_U7816CTRL_HPUAT_Setable(U7816x_Type* U7816x, FunState NewState);
extern FunState U7816x_U7816CTRL_HPUAT_Getable(U7816x_Type* U7816x);

/* U7816通道强上拉使能控制 相关函数 */
extern void U7816x_U7816CTRL_HPUEN_Setable(U7816x_Type* U7816x, FunState NewState);
extern FunState U7816x_U7816CTRL_HPUEN_Getable(U7816x_Type* U7816x);

/* ERROR SIGNAL宽度选择 相关函数 */
extern void U7816x_U7816FRC_ERSW_Set(U7816x_Type* U7816x, uint32_t SetValue);
extern uint32_t U7816x_U7816FRC_ERSW_Get(U7816x_Type* U7816x);

/* ERROR SIGNAL后GUARDTIME宽度选择（仅在发送时有效） 相关函数 */
extern void U7816x_U7816FRC_ERSGD_Set(U7816x_Type* U7816x, uint32_t SetValue);
extern uint32_t U7816x_U7816FRC_ERSGD_Get(U7816x_Type* U7816x);

/* BGT（block guard time）控制 相关函数 */
extern void U7816x_U7816FRC_BGTEN_Setable(U7816x_Type* U7816x, FunState NewState);
extern FunState U7816x_U7816FRC_BGTEN_Getable(U7816x_Type* U7816x);

/* 控制接收数据奇偶校验出错时自动重发次数 相关函数 */
extern void U7816x_U7816FRC_REP_T_Set(U7816x_Type* U7816x, uint32_t SetValue);
extern uint32_t U7816x_U7816FRC_REP_T_Get(U7816x_Type* U7816x);

/* 奇偶校验类型选择 相关函数 */
extern void U7816x_U7816FRC_PAR_Set(U7816x_Type* U7816x, uint32_t SetValue);
extern uint32_t U7816x_U7816FRC_PAR_Get(U7816x_Type* U7816x);

/* Guard Time长度控制位（发送时严格按照协议2etu） 相关函数 */
extern void U7816x_U7816FRC_FREN_Setable(U7816x_Type* U7816x, FunState NewState);
extern FunState U7816x_U7816FRC_FREN_Getable(U7816x_Type* U7816x);

/* 缓发送数据奇偶校验错的处理方式选择 相关函数 */
extern void U7816x_U7816FRC_TREPEN_Setable(U7816x_Type* U7816x, FunState NewState);
extern FunState U7816x_U7816FRC_TREPEN_Getable(U7816x_Type* U7816x);

/* 接收数据奇偶校验错的处理方式选择 相关函数 */
extern void U7816x_U7816FRC_RREPEN_Setable(U7816x_Type* U7816x, FunState NewState);
extern FunState U7816x_U7816FRC_RREPEN_Getable(U7816x_Type* U7816x);

/* 传输次序反向编码使能 相关函数 */
extern void U7816x_U7816FRC_DICONV_Setable(U7816x_Type* U7816x, FunState NewState);
extern FunState U7816x_U7816FRC_DICONV_Getable(U7816x_Type* U7816x);

/* 发送时插入的EGT时间（以ETU为单位） 相关函数 */
extern void U7816x_U7816EGT_Write(U7816x_Type* U7816x, uint32_t SetValue);
extern uint32_t U7816x_U7816EGT_Read(U7816x_Type* U7816x);

/* U7816时钟输出分频控制寄存器 相关函数 */
extern void U7816x_U7816CLKDIV_Write(U7816x_Type* U7816x, uint32_t SetValue);
extern uint32_t U7816x_U7816CLKDIV_Read(U7816x_Type* U7816x);

/* U7816预分频控制寄存器，控制7816通信分频比（波特率） 相关函数 */
extern void U7816x_U7816PDIV_Write(U7816x_Type* U7816x, uint32_t SetValue);
extern uint32_t U7816x_U7816PDIV_Read(U7816x_Type* U7816x);

/* U7816数据接收缓存寄存器 相关函数 */
extern uint32_t U7816x_U7816RXBUF_Read(U7816x_Type* U7816x);

/* U7816数据发送缓存寄存器 相关函数 */
extern void U7816x_U7816TXBUF_Write(U7816x_Type* U7816x, uint32_t SetValue);

/* 数据接收中断使能位。对应RX_FLAG中断标志位 相关函数 */
extern void U7816x_U7816IE_RXIE_Setable(U7816x_Type* U7816x, FunState NewState);
extern FunState U7816x_U7816IE_RXIE_Getable(U7816x_Type* U7816x);

/* 数据发送中断使能位。对应TX_FLAG中断标志位 相关函数 */
extern void U7816x_U7816IE_TXIE_Setable(U7816x_Type* U7816x, FunState NewState);
extern FunState U7816x_U7816IE_TXIE_Getable(U7816x_Type* U7816x);

/* 线路状态中断使能位。对应ERROR_FLAG中断标志位 相关函数 */
extern void U7816x_U7816IE_LSIE_Setable(U7816x_Type* U7816x, FunState NewState);
extern FunState U7816x_U7816IE_LSIE_Getable(U7816x_Type* U7816x);

/* 错误标志，寄存器配置出错或传输过程中出错。硬件置位，清U7816ERR中相应错误标志清零 相关函数 */
extern FlagStatus U7816x_U7816IF_ERRIF_Chk(U7816x_Type* U7816x);

/* 发送缓冲区空标志 相关函数 */
extern void U7816x_U7816IF_TXIF_Clr(U7816x_Type* U7816x);
extern FlagStatus U7816x_U7816IF_TXIF_Chk(U7816x_Type* U7816x);

/* 接收完成标志 相关函数 */
extern void U7816x_U7816IF_RXIF_Clr(U7816x_Type* U7816x);
extern FlagStatus U7816x_U7816IF_RXIF_Chk(U7816x_Type* U7816x);

/* 发送数据奇偶校验错误标志位 相关函数 */
extern void U7816x_U7816ERR_TPARERR_Clr(U7816x_Type* U7816x);
extern FlagStatus U7816x_U7816ERR_TPARERR_Chk(U7816x_Type* U7816x);

/* 接收数据奇偶校验错误标志位 相关函数 */
extern void U7816x_U7816ERR_RPARERR_Clr(U7816x_Type* U7816x);
extern FlagStatus U7816x_U7816ERR_RPARERR_Chk(U7816x_Type* U7816x);

/* 接收帧格式错误标志位 相关函数 */
extern void U7816x_U7816ERR_FRERR_Clr(U7816x_Type* U7816x);
extern FlagStatus U7816x_U7816ERR_FRERR_Chk(U7816x_Type* U7816x);

/* 接收溢出错误标志位 相关函数 */
extern void U7816x_U7816ERR_OVERR_Clr(U7816x_Type* U7816x);
extern FlagStatus U7816x_U7816ERR_OVERR_Chk(U7816x_Type* U7816x);

/* U7816接口发送了错误信号，正在等待对方重发数据.状态机进入发送错误信号状态时置位，收到数据起始位或者进入发送状态时硬件清零;
 相关函数 */
extern FlagStatus U7816x_U7816STA_WAIT_RPT_Chk(U7816x_Type* U7816x);

/* 发送数据忙标志 相关函数 */
extern FlagStatus U7816x_U7816STA_TXBUSY_Chk(U7816x_Type* U7816x);

/* 接收数据忙标志 相关函数 */
extern FlagStatus U7816x_U7816STA_RXBUSY_Chk(U7816x_Type* U7816x);
//Announce_End


/* U7816完整参数初始化函数 */
void U7816_Init(U7816x_Type* U7816x, U7816_InitTypeDef* para);



#ifdef __cplusplus
}
#endif

#endif /* __FM33A0XX_U7816_H */



/************************ (C) COPYRIGHT FMSHelectronics *****END OF FILE****/



