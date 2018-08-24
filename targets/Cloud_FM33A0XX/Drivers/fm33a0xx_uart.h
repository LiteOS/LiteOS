/**
******************************************************************************
* @file    fm33a0xx_uart.h
* @author  
* @version V2.0.1
* @date    
* @brief   This file contains all the functions prototypes for the UART firmware     
******************************************************************************
*/ 
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FM33A0XX_UART_H
#define __FM33A0XX_UART_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "FM33A0XX.h"

/** @addtogroup FM33A0XX_StdPeriph_Driver
  * @{
  */

/** @addtogroup UART
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/
typedef enum
{
	RxInt,
	TxInt
	
}UART_IntTypeDef;

typedef enum
{
	Seven7Bit,		//7位数据不支奇偶校验位
	Eight8Bit,
	Nine9Bit		//9位数据不支持奇偶校验位
	
}UART_DataBitTypeDef;

typedef enum
{
	NONE,
	EVEN,
	ODD
	
}UART_ParityBitTypeDef;

typedef enum
{
	OneBit,
	TwoBit
}UART_StopBitTypeDef;

typedef struct
{
	uint32_t				BaudRate;		//波特率
	UART_DataBitTypeDef		DataBit;		//数据位数
	UART_ParityBitTypeDef	ParityBit; 		//校验位
	UART_StopBitTypeDef		StopBit;		//停止位
	
}UART_SInitTypeDef;	

typedef struct
{
	FunState				RXIE;		//接收中断
	FunState				TXIE;		//发送中断
	
	uint32_t				SPBRG;		//波特率产生器寄存器
	
	uint32_t				PDSEL;		//模式选择，00 = 8位数据，无奇偶校验；01 = 8位数据，偶校验；10 = 8位数据，奇校验。11 = 9位数据， 无奇偶校验；
	FunState				ERRIE;		//错误中断使能控制
	FunState				RXEN;		//接收模块使能控制
	uint32_t				STOPSEL;	//停止位选择
	uint32_t				TXIS;		//TX_INTSEL=0时，发送中断选择位
	FunState				TXEN;		//发送模块使能控制
	FunState				IREN;		//发送红外调制使能位
	uint32_t				TX_INTSEL;	//发送中断选择位
	
	FunState				RTX7EN;		//收发7bit数据使能(覆盖PDSEL)
	FunState				RXDFLAG;	//接收数据取反控制位
	FunState				TXDFLAG;	//发送数据取反控制位
	
}UART_InitTypeDef;	


//Macro_START
//2018-04-02-13-17-16
//FM33A0XX_Driver_Gen_V1.4

#define	UART_UARTIE_UARTIE_Pos	0	/* UART中断允许寄存器 */
#define	UART_UARTIE_UARTIE_Msk	(0xfffU << UART_UARTIE_UARTIE_Pos)

#define	UART_UARTIF_UARTIF_Pos	0	/* UART中断标志寄存器 */
#define	UART_UARTIF_UARTIF_Msk	(0xfffU << UART_UARTIF_UARTIF_Pos)

#define	UART_IRCON_IRFLAG_Pos	15	/* 红外调制发送数据反相 */
#define	UART_IRCON_IRFLAG_Msk	(0x1U << UART_IRCON_IRFLAG_Pos)
	/* 0：正常发送 */
	/* 1：反相发送 */

#define	UART_IRCON_TH_Pos	11	/* 红外占空比调制参数 */
#define	UART_IRCON_TH_Msk	(0xfU << UART_IRCON_TH_Pos)

#define	UART_IRCON_TZBRG_Pos	0	/* 红外调制频率 */
#define	UART_IRCON_TZBRG_Msk	(0x7ffU << UART_IRCON_TZBRG_Pos)

#define	UARTx_RXSTA_PDSEL_Pos	6	/* 模式选择位 */
#define	UARTx_RXSTA_PDSEL_Msk	(0x3U << UARTx_RXSTA_PDSEL_Pos)
#define	UARTx_RXSTA_PDSEL_8BIT_NONE	(0x0U << UARTx_RXSTA_PDSEL_Pos)	/* 00 = 8位数据，无奇偶校验； */
#define	UARTx_RXSTA_PDSEL_8BIT_EVEN	(0x1U << UARTx_RXSTA_PDSEL_Pos)	/* 01 = 8位数据，偶校验； */
#define	UARTx_RXSTA_PDSEL_8BIT_ODD	(0x2U << UARTx_RXSTA_PDSEL_Pos)	/* 10 = 8位数据，奇校验。 */
#define	UARTx_RXSTA_PDSEL_9BIT_NONE	(0x3U << UARTx_RXSTA_PDSEL_Pos)	/* 11 = 9位数据， 无奇偶校验； */

#define	UARTx_RXSTA_ERRIE_Pos	5	/* 错误中断允许位,包括奇偶校验错，帧格式错，溢出错 */
#define	UARTx_RXSTA_ERRIE_Msk	(0x1U << UARTx_RXSTA_ERRIE_Pos)

#define	UARTx_RXSTA_RXEN_Pos	4	/* 接收模块使能位 */
#define	UARTx_RXSTA_RXEN_Msk	(0x1U << UARTx_RXSTA_RXEN_Pos)
	/* 0 = 禁止接收模块，接收模块被复位 */
	/* 1 = 使能接收模块； */

#define	UARTx_RXSTA_PERR_Pos	3	/* 奇偶校验错标志位 */
#define	UARTx_RXSTA_PERR_Msk	(0x1U << UARTx_RXSTA_PERR_Pos)

#define	UARTx_RXSTA_FERR_Pos	2	/* 帧格式错标志位 */
#define	UARTx_RXSTA_FERR_Msk	(0x1U << UARTx_RXSTA_FERR_Pos)

#define	UARTx_RXSTA_OERR_Pos	1	/* 溢出错标志位 */
#define	UARTx_RXSTA_OERR_Msk	(0x1U << UARTx_RXSTA_OERR_Pos)

#define	UARTx_RXSTA_RX9D_Pos	0	/* 接收数据的第9位 */
#define	UARTx_RXSTA_RX9D_Msk	(0x1U << UARTx_RXSTA_RX9D_Pos)

#define	UARTx_TXSTA_STOPSEL_Pos	6	/* 停止位选择位 */
#define	UARTx_TXSTA_STOPSEL_Msk	(0x1U << UARTx_TXSTA_STOPSEL_Pos)
#define	UARTx_TXSTA_STOPSEL_1STOPBIT	(0x0U << UARTx_TXSTA_STOPSEL_Pos)	/* 0 = 停止位为1位 */
#define	UARTx_TXSTA_STOPSEL_2STOPBIT	(0x1U << UARTx_TXSTA_STOPSEL_Pos)	/* 1 = 停止位为2位 */

#define	UARTx_TXSTA_TXIS_Pos	5	/* TX_INTSEL=0时，发送中断选择位 */
#define	UARTx_TXSTA_TXIS_Msk	(0x1U << UARTx_TXSTA_TXIS_Pos)
#define	UARTx_TXSTA_TXIS_BUFFEREMPTY	(0x0U << UARTx_TXSTA_TXIS_Pos)	/* 0 = 发送缓冲器空产生中断 */
#define	UARTx_TXSTA_TXIS_SHIFTEMPTY	(0x1U << UARTx_TXSTA_TXIS_Pos)	/* 1 = 移位寄存器空产生中断 */

#define	UARTx_TXSTA_TXEN_Pos	4	/* 发送模块使能位 */
#define	UARTx_TXSTA_TXEN_Msk	(0x1U << UARTx_TXSTA_TXEN_Pos)
	/* 0 = 禁止发送模块，发送模块被复位 */
	/* 1 = 使能发送模块 */

#define	UARTx_TXSTA_IREN_Pos	3	/* 发送红外调制使能位 */
#define	UARTx_TXSTA_IREN_Msk	(0x1U << UARTx_TXSTA_IREN_Pos)
	/* 0 = 禁止发送红外调制 */
	/* 1 = 使能发送红外调制 */

#define	UARTx_TXSTA_TX9D_Pos	0	/* 发送数据的第9位 */
#define	UARTx_TXSTA_TX9D_Msk	(0x1U << UARTx_TXSTA_TX9D_Pos)
#define	UARTx_TXSTA_TX9D_0	(0x0U << UARTx_TXSTA_TX9D_Pos)
#define	UARTx_TXSTA_TX9D_1	(0x1U << UARTx_TXSTA_TX9D_Pos)

#define	UARTx_RXREG_RXREG_Pos	0	/* 接收数据缓冲寄存器 */
#define	UARTx_RXREG_RXREG_Msk	(0xffU << UARTx_RXREG_RXREG_Pos)

#define	UARTx_TXREG_TXREG_Pos	0	/* 发送数据缓冲寄存器 */
#define	UARTx_TXREG_TXREG_Msk	(0xffU << UARTx_TXREG_TXREG_Pos)

#define	UARTx_SPBRG_SPBRG_Pos	0	/* 波特率产生器寄存器 */
#define	UARTx_SPBRG_SPBRG_Msk	(0xffffU << UARTx_SPBRG_SPBRG_Pos)

#define	UARTx_TXBUFSTA_TX_INTSEL_Pos	2	/* 发送中断选择位 */
#define	UARTx_TXBUFSTA_TX_INTSEL_Msk	(0x3U << UARTx_TXBUFSTA_TX_INTSEL_Pos)
#define	UARTx_TXBUFSTA_TX_INTSEL_BYTXIS	(0x0U << UARTx_TXBUFSTA_TX_INTSEL_Pos)	/* 00 =由TXIS决定发送中断在发送缓冲空或者移位寄存器空时产生 */
#define	UARTx_TXBUFSTA_TX_INTSEL_SHIFTEMPTY	(0x1U << UARTx_TXBUFSTA_TX_INTSEL_Pos)	/* 01 = TXBUF空且移位寄存器空产生中断 */
#define	UARTx_TXBUFSTA_TX_INTSEL_BUFFEREMPTY	(0x2U << UARTx_TXBUFSTA_TX_INTSEL_Pos)	/* 10 = TXBUF空产生中断 */
#define	UARTx_TXBUFSTA_TX_INTSEL_NOINT	(0x3U << UARTx_TXBUFSTA_TX_INTSEL_Pos)	/* 11 = 不产生中断 */

#define	UARTx_TXBUFSTA_TXFF_Pos	0	/* TXBUF状态位 */
#define	UARTx_TXBUFSTA_TXFF_Msk	(0x1U << UARTx_TXBUFSTA_TXFF_Pos)

#define	UARTx_RXBUFSTA_RXFF_Pos	0	/* RXBUF状态位 */
#define	UARTx_RXBUFSTA_RXFF_Msk	(0x1U << UARTx_RXBUFSTA_RXFF_Pos)

#define	UARTx_RTXCON_RTX7EN_Pos	2	/* 收发7bit数据使能(覆盖PDSEL) */
#define	UARTx_RTXCON_RTX7EN_Msk	(0x1U << UARTx_RTXCON_RTX7EN_Pos)
	/* 0 = 正常收发 */
	/* 1 = 收发7位数据帧，格式为7位数据位+STOP，收发数据为RXREG/TXREG[6:0] */

#define	UARTx_RTXCON_RXDFLAG_Pos	1	/* 接收数据取反控制位 */
#define	UARTx_RTXCON_RXDFLAG_Msk	(0x1U << UARTx_RTXCON_RXDFLAG_Pos)
	/* 0 = 接收数据不取反 */
	/* 1 = 接收数据取反 */

#define	UARTx_RTXCON_TXDFLAG_Pos	0	/* 发送数据取反控制位 */
#define	UARTx_RTXCON_TXDFLAG_Msk	(0x1U << UARTx_RTXCON_TXDFLAG_Pos)
	/* 0 = 发送数据不取反 */
	/* 1 = 发送数据取反，若红外调制开启时，则取反无效 */
//Macro_End

/* Exported functions --------------------------------------------------------*/ 
extern void UART_Deinit(void);

/* 红外调制发送数据反相 相关函数 */
extern void UART_IRCON_IRFLAG_Setable(FunState NewState);
extern FunState UART_IRCON_IRFLAG_Getable(void);

/* 红外占空比调制参数 相关函数 */
extern void UART_IRCON_TH_Set(uint32_t SetValue);
extern uint32_t UART_IRCON_TH_Get(void);

/* 红外调制频率 相关函数 */
extern void UART_IRCON_TZBRG_Set(uint32_t SetValue);
extern uint32_t UART_IRCON_TZBRG_Get(void);
extern void UARTx_Deinit(UARTx_Type* UARTx);

/* 模式选择位 相关函数 */
extern void UARTx_RXSTA_PDSEL_Set(UARTx_Type* UARTx, uint32_t SetValue);
extern uint32_t UARTx_RXSTA_PDSEL_Get(UARTx_Type* UARTx);

/* 错误中断允许位,包括奇偶校验错，帧格式错，溢出错 相关函数 */
extern void UARTx_RXSTA_ERRIE_Setable(UARTx_Type* UARTx, FunState NewState);
extern FunState UARTx_RXSTA_ERRIE_Getable(UARTx_Type* UARTx);

/* 接收模块使能位 相关函数 */
extern void UARTx_RXSTA_RXEN_Setable(UARTx_Type* UARTx, FunState NewState);
extern FunState UARTx_RXSTA_RXEN_Getable(UARTx_Type* UARTx);

/* 奇偶校验错标志位 相关函数 */
extern void UARTx_RXSTA_PERR_Clr(UARTx_Type* UARTx);
extern FlagStatus UARTx_RXSTA_PERR_Chk(UARTx_Type* UARTx);

/* 帧格式错标志位 相关函数 */
extern void UARTx_RXSTA_FERR_Clr(UARTx_Type* UARTx);
extern FlagStatus UARTx_RXSTA_FERR_Chk(UARTx_Type* UARTx);

/* 溢出错标志位 相关函数 */
extern void UARTx_RXSTA_OERR_Clr(UARTx_Type* UARTx);
extern FlagStatus UARTx_RXSTA_OERR_Chk(UARTx_Type* UARTx);

/* 接收数据的第9位 相关函数 */
extern FlagStatus UARTx_RXSTA_RX9D_Chk(UARTx_Type* UARTx);

/* 停止位选择位 相关函数 */
extern void UARTx_TXSTA_STOPSEL_Set(UARTx_Type* UARTx, uint32_t SetValue);
extern uint32_t UARTx_TXSTA_STOPSEL_Get(UARTx_Type* UARTx);

/* TX_INTSEL=0时，发送中断选择位 相关函数 */
extern void UARTx_TXSTA_TXIS_Set(UARTx_Type* UARTx, uint32_t SetValue);
extern uint32_t UARTx_TXSTA_TXIS_Get(UARTx_Type* UARTx);

/* 发送模块使能位 相关函数 */
extern void UARTx_TXSTA_TXEN_Setable(UARTx_Type* UARTx, FunState NewState);
extern FunState UARTx_TXSTA_TXEN_Getable(UARTx_Type* UARTx);

/* 发送红外调制使能位 相关函数 */
extern void UARTx_TXSTA_IREN_Setable(UARTx_Type* UARTx, FunState NewState);
extern FunState UARTx_TXSTA_IREN_Getable(UARTx_Type* UARTx);

/* 发送数据的第9位 相关函数 */
extern void UARTx_TXSTA_TX9D_Set(UARTx_Type* UARTx, uint32_t SetValue);

/* 接收数据缓冲寄存器 相关函数 */
extern uint32_t UARTx_RXREG_Read(UARTx_Type* UARTx);

/* 发送数据缓冲寄存器 相关函数 */
extern void UARTx_TXREG_Write(UARTx_Type* UARTx, uint32_t SetValue);

/* 波特率产生器寄存器 相关函数 */
extern void UARTx_SPBRG_Write(UARTx_Type* UARTx, uint32_t SetValue);
extern uint32_t UARTx_SPBRG_Read(UARTx_Type* UARTx);

/* 发送中断选择位 相关函数 */
extern void UARTx_TXBUFSTA_TX_INTSEL_Set(UARTx_Type* UARTx, uint32_t SetValue);
extern uint32_t UARTx_TXBUFSTA_TX_INTSEL_Get(UARTx_Type* UARTx);

/* TXBUF状态位 相关函数 */
extern FlagStatus UARTx_TXBUFSTA_TXFF_Chk(UARTx_Type* UARTx);

/* RXBUF状态位 相关函数 */
extern FlagStatus UARTx_RXBUFSTA_RXFF_Chk(UARTx_Type* UARTx);

/* 收发7bit数据使能(覆盖PDSEL) 相关函数 */
extern void UARTx_RTXCON_RTX7EN_Setable(UARTx_Type* UARTx, FunState NewState);
extern FunState UARTx_RTXCON_RTX7EN_Getable(UARTx_Type* UARTx);

/* 接收数据取反控制位 相关函数 */
extern void UARTx_RTXCON_RXDFLAG_Setable(UARTx_Type* UARTx, FunState NewState);
extern FunState UARTx_RTXCON_RXDFLAG_Getable(UARTx_Type* UARTx);

/* 发送数据取反控制位 相关函数 */
extern void UARTx_RTXCON_TXDFLAG_Setable(UARTx_Type* UARTx, FunState NewState);
extern FunState UARTx_RTXCON_TXDFLAG_Getable(UARTx_Type* UARTx);
//Announce_End


extern void UART_All_Deinit(void);

/* uart 接收 发送 中断使能配置函数 */
extern void UART_UARTIE_RxTxIE_SetableEx(UARTx_Type* UARTx, UART_IntTypeDef IntType, FunState NewState);

/* uart 接收 发送 中断使能读取函数 */
extern FunState UART_UARTIE_RxTxIE_GetableEx(UARTx_Type* UARTx, UART_IntTypeDef IntType);

/* uart 发送 中断标志清零函数
	接收中断标志 只能 通过读rxreg清零
	发送中断标志可通过写txreg或者txif写1清零
*/
extern void UART_UARTIF_RxTxIF_ClrEx(UARTx_Type* UARTx);

/* uart 接收 发送 中断标志读取函数 */
extern FlagStatus UART_UARTIF_RxTxIF_ChkEx(UARTx_Type* UARTx, UART_IntTypeDef IntType);

/* 红外调制频率，占空比 配置 */
extern void UART_IRModulation_Init( uint32_t ModuFreq, uint8_t ModuDutyCycle, uint32_t APBClk );

/* UART完整参数初始化函数 */
extern void UART_Init(UARTx_Type* UARTx, UART_InitTypeDef* para);

/* uart波特率寄存器值计算函数 */
extern uint32_t UART_BaudREGCalc(uint32_t BaudRate, uint32_t APBClk);

/* UART简单参数初始化函数 */
extern void UART_SInit(UARTx_Type* UARTx, UART_SInitTypeDef* para, uint32_t APBClk);


#ifdef __cplusplus
}
#endif

#endif /* __FM33A0XX_UART_H */

