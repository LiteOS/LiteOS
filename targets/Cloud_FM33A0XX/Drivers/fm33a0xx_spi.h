/**
  ******************************************************************************
  * @file    fm33a0xx_spi.h
  * @author  FM33a0xx Application Team
  * @version V2.0.1
  * @date    
  * @brief   This file contains all the functions prototypes for the SPI firmware library.  
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FM33A0XX_SPI_H
#define __FM33A0XX_SPI_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "FM33A0XX.h" 

/** @addtogroup FM33a0xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup SPI
  * @{
  */ 

/* Exported constants --------------------------------------------------------*/

/** @defgroup SPI_Exported_Constants
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  SPI Init Structure definition  
  */ 


typedef struct
{
	//uint32_t SSPA;              //slave模式下对发送时机点进行调整，建议使用复位值，不开放/* SETTING SENING AHESD HALF CYCLE */
	//FunState FLTEN;             //slave模式下使能4ns滤波/* SET PAD(SSN/SCK/MOSI) FILTER ENABLE AT SLAVE  */	
	uint32_t LSBF;             	//先发送高位bit还是低位bit/* SETTING 1 FOR LSB */		
    uint32_t CPHOL;			    //时钟极性选择/* SETTING 1 FOR CLK STOP AT HIGH*/	
	uint32_t CPHA;				//时钟相位选择/* SETTING 1 FOR START AT SECOND CLK EDGE AND 0 FOR FIRST */
}SPI_Slave_SInitTypeDef;

typedef struct
{
	uint32_t SSPA;				//slave模式下对发送时机点进行调整（提前）
	FunState FLTEN;             //slave模式下使能4ns滤波/
	uint32_t LSBF;             	//先发送高位bit还是低位bit/* SETTING 1 FOR LSB */		
    uint32_t CPHOL;			    //时钟极性选择/* SETTING 1 FOR CLK STOP AT HIGH*/	
	uint32_t CPHA;				//时钟相位选择/* SETTING 1 FOR START AT SECOND CLK EDGE AND 0 FOR FIRST */
	FunState TXO;               //TXONLY模式
	uint32_t TXO_AC;            //TXONLY硬件自动清除配置

	FunState SPIEN;             //SPI使能，初始化时建议写为DISABLE
	FunState ERRIE;             //SPI错误中断使能，初始化时建议写为DISABLE
	FunState TXIE;              //SPI发送完成中断使能，初始化时建议写为DISABLE
	FunState RXIE;              //SPI接收完成中断使能，初始化时建议写为DISABLE
}SPI_Slave_InitTypeDef;



typedef struct
{
	//uint32_t MSPA;			//master模式下对采样点进行调整，建议使用复位值，不开放/* SETTING SAMPLE DELAY HALF CYCLE */
    //uint32_t WAIT;          	//每次发送完8bit至少加入几个sck周期的等待时间，建议使用复位值，不开放/* WAIT CYCLE BETWEEN TWO BYTE SENDING */	
    uint32_t BAUD_RATE;         //波特率/* SETTING BAUD_RATE */	
    uint32_t LSBF;             	//先发送高位bit还是低位bit/* SETTING 1 FOR LSB */		
    uint32_t CPHOL;			    //时钟极性选择/* SETTING 1 FOR CLK STOP AT HIGH*/	
	uint32_t CPHA;				//时钟相位选择/* SETTING 1 FOR START AT SECOND CLK EDGE AND 0 FOR FIRST */
	uint32_t SSNM;				//每次发完8bit后保持SSN为低还是拉高，SSN硬件模式有效/* SETTING SSN AFTER SENDING */	
	uint32_t SSNSEN;			//SSN由硬件控制还是由软件控制/* SETTING 1 FOR CONTROL SSN BY SOFTWARE */

}SPI_Master_SInitTypeDef;

typedef struct
{
	uint32_t MSPA;              //master模式下对采样点进行调整(滞后)，用于高速通信时补偿PCB走线延迟
	uint32_t WAIT;              //master模式下，每次发送完8bit至少加入几个sck周期的等待时间
	uint32_t BAUD_RATE; 		//波特率/* SETTING BAUD_RATE */	
	uint32_t LSBF;				//先发送高位bit还是低位bit/* SETTING 1 FOR LSB */		
	uint32_t CPHOL; 			//时钟极性选择/* SETTING 1 FOR CLK STOP AT HIGH*/ 
	uint32_t CPHA;				//时钟相位选择/* SETTING 1 FOR START AT SECOND CLK EDGE AND 0 FOR FIRST */
	FunState TXO;               //TXONLY模式
	uint32_t TXO_AC;            //TXONLY硬件自动清除配置
	uint32_t SSNM;				//每次发完8bit后保持SSN为低还是拉高，SSN硬件模式有效/* SETTING SSN AFTER SENDING */ 
	uint32_t SSNSEN;			//SSN由硬件控制还是由软件控制/* SETTING 1 FOR CONTROL SSN BY SOFTWARE */

	FunState SPIEN;             //SPI使能，初始化时建议写为DISABLE
	FunState ERRIE;             //SPI错误中断使能，初始化时建议写为DISABLE
	FunState TXIE;              //SPI发送完成中断使能，初始化时建议写为DISABLE
	FunState RXIE;              //SPI接收完成中断使能，初始化时建议写为DISABLE
	
}SPI_Master_InitTypeDef;



//Macro_START
//2018-04-02-13-22-56
//FM33A0XX_Driver_Gen_V1.4

#define	SPIx_SPICR1_MSPA_Pos	10	/* Master对MISO信号的采样位置调整，用于高速通信时补偿PCB走线延迟 */
#define	SPIx_SPICR1_MSPA_Msk	(0x1U << SPIx_SPICR1_MSPA_Pos)
#define	SPIx_SPICR1_MSPA_NORMAL	(0x0U << SPIx_SPICR1_MSPA_Pos)	/* 采样点不调整 */
#define	SPIx_SPICR1_MSPA_HALF_DELAY	(0x1U << SPIx_SPICR1_MSPA_Pos)	/* 采样点延迟半个SCK周期 */

#define	SPIx_SPICR1_SSPA_Pos	9	/* Slave MISO发送位置调整 */
#define	SPIx_SPICR1_SSPA_Msk	(0x1U << SPIx_SPICR1_SSPA_Pos)
#define	SPIx_SPICR1_SSPA_NORMAL	(0x0U << SPIx_SPICR1_SSPA_Pos)	/* 发送不提前 */
#define	SPIx_SPICR1_SSPA_HALF_AHEAD	(0x1U << SPIx_SPICR1_SSPA_Pos)	/* 提前半个SCK周期发送 */

#define	SPIx_SPICR1_MM_Pos	8	/* MASTER/SLAVE模式选择 */
#define	SPIx_SPICR1_MM_Msk	(0x1U << SPIx_SPICR1_MM_Pos)
#define	SPIx_SPICR1_MM_SLAVE	(0x0U << SPIx_SPICR1_MM_Pos)	/* Slave模式 */
#define	SPIx_SPICR1_MM_MASTER	(0x1U << SPIx_SPICR1_MM_Pos)	/* Master模式 */

#define	SPIx_SPICR1_WAIT_Pos	6
#define	SPIx_SPICR1_WAIT_Msk	(0x3U << SPIx_SPICR1_WAIT_Pos)
#define	SPIx_SPICR1_WAIT_WAIT_1	(0x0U << SPIx_SPICR1_WAIT_Pos)	/* 每发完8bit至少加入1个sck周期等待时间再传输下一个8Bit数据 */
#define	SPIx_SPICR1_WAIT_WAIT_2	(0x1U << SPIx_SPICR1_WAIT_Pos)	/* 每发完8bit至少加入2个sck周期等待时间再传输下一个8Bit数据 */
#define	SPIx_SPICR1_WAIT_WAIT_3	(0x2U << SPIx_SPICR1_WAIT_Pos)	/* 每发完8bit至少加入3个sck周期等待时间再传输下一个8Bit数据 */
#define	SPIx_SPICR1_WAIT_WAIT_4	(0x3U << SPIx_SPICR1_WAIT_Pos)	/* 每发完8bit至少加入4个sck周期等待时间再传输下一个8Bit数据 */

#define	SPIx_SPICR1_BAUD_Pos	3	/* MASTER模式波特率配置 */
#define	SPIx_SPICR1_BAUD_Msk	(0x7U << SPIx_SPICR1_BAUD_Pos)
#define	SPIx_SPICR1_BAUD_PCLK_2	(0x0U << SPIx_SPICR1_BAUD_Pos)	/* 外设时钟2分频 */
#define	SPIx_SPICR1_BAUD_PCLK_4	(0x1U << SPIx_SPICR1_BAUD_Pos)	/* 外设时钟4分频 */
#define	SPIx_SPICR1_BAUD_PCLK_8	(0x2U << SPIx_SPICR1_BAUD_Pos)	/* 外设时钟8分频 */
#define	SPIx_SPICR1_BAUD_PCLK_16	(0x3U << SPIx_SPICR1_BAUD_Pos)	/* 外设时钟16分频 */
#define	SPIx_SPICR1_BAUD_PCLK_32	(0x4U << SPIx_SPICR1_BAUD_Pos)	/* 外设时钟32分频 */
#define	SPIx_SPICR1_BAUD_PCLK_64	(0x5U << SPIx_SPICR1_BAUD_Pos)	/* 外设时钟64分频 */
#define	SPIx_SPICR1_BAUD_PCLK_128	(0x6U << SPIx_SPICR1_BAUD_Pos)	/* 外设时钟128分频 */
#define	SPIx_SPICR1_BAUD_PCLK_256	(0x7U << SPIx_SPICR1_BAUD_Pos)	/* 外设时钟256分频 */

#define	SPIx_SPICR1_LSBF_Pos	2	/* 帧格式 */
#define	SPIx_SPICR1_LSBF_Msk	(0x1U << SPIx_SPICR1_LSBF_Pos)
#define	SPIx_SPICR1_LSBF_MSB	(0x0U << SPIx_SPICR1_LSBF_Pos)	/* 先发送MSB */
#define	SPIx_SPICR1_LSBF_LSB	(0x1U << SPIx_SPICR1_LSBF_Pos)	/* 先发送LSB */

#define	SPIx_SPICR1_CPHOL_Pos	1	/* 时钟极性选择 */
#define	SPIx_SPICR1_CPHOL_Msk	(0x1U << SPIx_SPICR1_CPHOL_Pos)
#define	SPIx_SPICR1_CPHOL_LOW	(0x0U << SPIx_SPICR1_CPHOL_Pos)	/* 串行时钟停止在低电平 */
#define	SPIx_SPICR1_CPHOL_HIGH	(0x1U << SPIx_SPICR1_CPHOL_Pos)	/* 串行时钟停止在高电平 */

#define	SPIx_SPICR1_CPHA_Pos	0	/* 时钟相位选择 */
#define	SPIx_SPICR1_CPHA_Msk	(0x1U << SPIx_SPICR1_CPHA_Pos)
#define	SPIx_SPICR1_CPHA_FIRST	(0x0U << SPIx_SPICR1_CPHA_Pos)	/* 第一个时钟边沿是第一个捕捉边沿 */
#define	SPIx_SPICR1_CPHA_SECOND	(0x1U << SPIx_SPICR1_CPHA_Pos)	/* 第二个时钟边沿是第一个捕捉边沿 */

#define	SPIx_SPICR2_FLTEN_Pos	6	/* Slave输入管脚滤波使能（SSN/SCK/MOSI） */
#define	SPIx_SPICR2_FLTEN_Msk	(0x1U << SPIx_SPICR2_FLTEN_Pos)
	/* 0：不使能4ns滤波 */
	/* 1：使能4ns滤波 */

#define	SPIx_SPICR2_SSNM_Pos	5	/* Master模式下SSN控制模式选择 */
#define	SPIx_SPICR2_SSNM_Msk	(0x1U << SPIx_SPICR2_SSNM_Pos)
#define	SPIx_SPICR2_SSNM_LOW	(0x0U << SPIx_SPICR2_SSNM_Pos)	/* 每发送完8bit后Master保持SSN为低 */
#define	SPIx_SPICR2_SSNM_HIGH	(0x1U << SPIx_SPICR2_SSNM_Pos)	/* 每发送完8bit后Master拉高SSN，维持高电平时间由WAIT寄存器控制 */

#define	SPIx_SPICR2_TXO_AC_Pos	4	/* TXONLY硬件自动清空的使能 */
#define	SPIx_SPICR2_TXO_AC_Msk	(0x1U << SPIx_SPICR2_TXO_AC_Pos)
#define	SPIx_SPICR2_TXO_AC_CLR_AT_CLOSE	(0x0U << SPIx_SPICR2_TXO_AC_Pos)	/* 关闭TXONLY硬件自动清零 */
#define	SPIx_SPICR2_TXO_AC_CLR_AT_TX	(0x1U << SPIx_SPICR2_TXO_AC_Pos)	/* TXONLY硬件自动清零有效，软件使能TXO后，等待发送完毕后，硬件清零 */

#define	SPIx_SPICR2_TXO_Pos	3	/* TXONLY控制位 */
#define	SPIx_SPICR2_TXO_Msk	(0x1U << SPIx_SPICR2_TXO_Pos)
	/* 0：关闭Master的单发送模式 */
	/* 1：启动Master的单发送模式 */

#define	SPIx_SPICR2_SSN_Pos	2	/* Master模式下，软件可以通过此位控制SSN输出电平 */
#define	SPIx_SPICR2_SSN_Msk	(0x1U << SPIx_SPICR2_SSN_Pos)
#define	SPIx_SPICR2_SSN_LOW	(0x0U << SPIx_SPICR2_SSN_Pos)	/* SSN输出低电平 */
#define	SPIx_SPICR2_SSN_HIGH	(0x1U << SPIx_SPICR2_SSN_Pos)	/* SSN输出高电平 */

#define	SPIx_SPICR2_SSNSEN_Pos	1	/* Master模式下，SSN的控制方式 */
#define	SPIx_SPICR2_SSNSEN_Msk	(0x1U << SPIx_SPICR2_SSNSEN_Pos)
#define	SPIx_SPICR2_SSNSEN_HARD	(0x0U << SPIx_SPICR2_SSNSEN_Pos)	/* Master模式下SSN输出由硬件自动控制 */
#define	SPIx_SPICR2_SSNSEN_SOFT	(0x1U << SPIx_SPICR2_SSNSEN_Pos)	/* Master模式下SSN输出由软件控制 */

#define	SPIx_SPICR2_SPIEN_Pos	0	/* SPI使能 */
#define	SPIx_SPICR2_SPIEN_Msk	(0x1U << SPIx_SPICR2_SPIEN_Pos)
	/* 0：关闭SPI */
	/* 1：使能SPI */

#define	SPIx_SPICR3_TXBFC_Pos	3
#define	SPIx_SPICR3_TXBFC_Msk	(0x1U << SPIx_SPICR3_TXBFC_Pos)

#define	SPIx_SPICR3_RXBFC_Pos	2
#define	SPIx_SPICR3_RXBFC_Msk	(0x1U << SPIx_SPICR3_RXBFC_Pos)

#define	SPIx_SPICR3_MERRC_Pos	1
#define	SPIx_SPICR3_MERRC_Msk	(0x1U << SPIx_SPICR3_MERRC_Pos)

#define	SPIx_SPICR3_SERRC_Pos	0
#define	SPIx_SPICR3_SERRC_Msk	(0x1U << SPIx_SPICR3_SERRC_Pos)

#define	SPIx_SPIIE_ERRIE_Pos	2
#define	SPIx_SPIIE_ERRIE_Msk	(0x1U << SPIx_SPIIE_ERRIE_Pos)
	/* 0：HSPI错误中断不使能 */
	/* 1：HSPI错误中断使能 */

#define	SPIx_SPIIE_TXIE_Pos	1
#define	SPIx_SPIIE_TXIE_Msk	(0x1U << SPIx_SPIIE_TXIE_Pos)
	/* 0：发送完成中断不使能 */
	/* 1：发送完成中断使能 */

#define	SPIx_SPIIE_RXIE_Pos	0
#define	SPIx_SPIIE_RXIE_Msk	(0x1U << SPIx_SPIIE_RXIE_Pos)
	/* 0：接收完成中断不使能 */
	/* 1：接收完成中断使能 */

#define	SPIx_SPIIF_MERR_Pos	6	/* Master Error标志 */
#define	SPIx_SPIIF_MERR_Msk	(0x1U << SPIx_SPIIF_MERR_Pos)

#define	SPIx_SPIIF_SERR_Pos	5	/* Slave Error标志 */
#define	SPIx_SPIIF_SERR_Msk	(0x1U << SPIx_SPIIF_SERR_Pos)

#define	SPIx_SPIIF_RXCOL_Pos	4	/* 接收缓存溢出，软件写1清零 */
#define	SPIx_SPIIF_RXCOL_Msk	(0x1U << SPIx_SPIIF_RXCOL_Pos)

#define	SPIx_SPIIF_TXCOL_Pos	3	/* 发送缓存溢出，软件写1清零 */
#define	SPIx_SPIIF_TXCOL_Msk	(0x1U << SPIx_SPIIF_TXCOL_Pos)

#define	SPIx_SPIIF_BUSY_Pos	2	/* SPI空闲标志，只读 */
#define	SPIx_SPIIF_BUSY_Msk	(0x1U << SPIx_SPIIF_BUSY_Pos)

#define	SPIx_SPIIF_TXBE_Pos	1	/* TX Buffer Empty标志位 */
#define	SPIx_SPIIF_TXBE_Msk	(0x1U << SPIx_SPIIF_TXBE_Pos)

#define	SPIx_SPIIF_RXBF_Pos	0	/* RX Buffer Full标志位 */
#define	SPIx_SPIIF_RXBF_Msk	(0x1U << SPIx_SPIIF_RXBF_Pos)

#define	SPIx_SPITXBUF_TXBUF_Pos	0	/* SPI发送缓存 */
#define	SPIx_SPITXBUF_TXBUF_Msk	(0xffU << SPIx_SPITXBUF_TXBUF_Pos)

#define	SPIx_SPIRXBUF_RXBUF_Pos	0	/* SPI接收缓存 */
#define	SPIx_SPIRXBUF_RXBUF_Msk	(0xffU << SPIx_SPIRXBUF_RXBUF_Pos)
//Macro_End

/* Exported functions --------------------------------------------------------*/ 
extern void SPIx_Deinit(SPIx_Type* SPIx);

/* Master对MISO信号的采样位置调整，用于高速通信时补偿PCB走线延迟 相关函数 */
extern void SPIx_SPICR1_MSPA_Set(SPIx_Type* SPIx, uint32_t SetValue);
extern uint32_t SPIx_SPICR1_MSPA_Get(SPIx_Type* SPIx);

/* Slave MISO发送位置调整 相关函数 */
extern void SPIx_SPICR1_SSPA_Set(SPIx_Type* SPIx, uint32_t SetValue);
extern uint32_t SPIx_SPICR1_SSPA_Get(SPIx_Type* SPIx);

/* MASTER/SLAVE模式选择 相关函数 */
extern void SPIx_SPICR1_MM_Set(SPIx_Type* SPIx, uint32_t SetValue);
extern uint32_t SPIx_SPICR1_MM_Get(SPIx_Type* SPIx);
extern void SPIx_SPICR1_WAIT_Set(SPIx_Type* SPIx, uint32_t SetValue);
extern uint32_t SPIx_SPICR1_WAIT_Get(SPIx_Type* SPIx);

/* MASTER模式波特率配置 相关函数 */
extern void SPIx_SPICR1_BAUD_Set(SPIx_Type* SPIx, uint32_t SetValue);
extern uint32_t SPIx_SPICR1_BAUD_Get(SPIx_Type* SPIx);

/* 帧格式 相关函数 */
extern void SPIx_SPICR1_LSBF_Set(SPIx_Type* SPIx, uint32_t SetValue);
extern uint32_t SPIx_SPICR1_LSBF_Get(SPIx_Type* SPIx);

/* 时钟极性选择 相关函数 */
extern void SPIx_SPICR1_CPHOL_Set(SPIx_Type* SPIx, uint32_t SetValue);
extern uint32_t SPIx_SPICR1_CPHOL_Get(SPIx_Type* SPIx);

/* 时钟相位选择 相关函数 */
extern void SPIx_SPICR1_CPHA_Set(SPIx_Type* SPIx, uint32_t SetValue);
extern uint32_t SPIx_SPICR1_CPHA_Get(SPIx_Type* SPIx);

/* Slave输入管脚滤波使能（SSN/SCK/MOSI） 相关函数 */
extern void SPIx_SPICR2_FLTEN_Setable(SPIx_Type* SPIx, FunState NewState);
extern FunState SPIx_SPICR2_FLTEN_Getable(SPIx_Type* SPIx);

/* Master模式下SSN控制模式选择 相关函数 */
extern void SPIx_SPICR2_SSNM_Set(SPIx_Type* SPIx, uint32_t SetValue);
extern uint32_t SPIx_SPICR2_SSNM_Get(SPIx_Type* SPIx);

/* TXONLY硬件自动清空的使能 相关函数 */
extern void SPIx_SPICR2_TXO_AC_Set(SPIx_Type* SPIx, uint32_t SetValue);
extern uint32_t SPIx_SPICR2_TXO_AC_Get(SPIx_Type* SPIx);

/* TXONLY控制位 相关函数 */
extern void SPIx_SPICR2_TXO_Setable(SPIx_Type* SPIx, FunState NewState);
extern FunState SPIx_SPICR2_TXO_Getable(SPIx_Type* SPIx);

/* Master模式下，软件可以通过此位控制SSN输出电平 相关函数 */
extern void SPIx_SPICR2_SSN_Set(SPIx_Type* SPIx, uint32_t SetValue);
extern uint32_t SPIx_SPICR2_SSN_Get(SPIx_Type* SPIx);

/* Master模式下，SSN的控制方式 相关函数 */
extern void SPIx_SPICR2_SSNSEN_Set(SPIx_Type* SPIx, uint32_t SetValue);
extern uint32_t SPIx_SPICR2_SSNSEN_Get(SPIx_Type* SPIx);

/* SPI使能 相关函数 */
extern void SPIx_SPICR2_SPIEN_Setable(SPIx_Type* SPIx, FunState NewState);
extern FunState SPIx_SPICR2_SPIEN_Getable(SPIx_Type* SPIx);
extern void SPIx_SPICR3_TXBFC_Clr(SPIx_Type* SPIx);
extern void SPIx_SPICR3_RXBFC_Clr(SPIx_Type* SPIx);
extern void SPIx_SPICR3_MERRC_Clr(SPIx_Type* SPIx);
extern void SPIx_SPICR3_SERRC_Clr(SPIx_Type* SPIx);
extern void SPIx_SPIIE_ERRIE_Setable(SPIx_Type* SPIx, FunState NewState);
extern FunState SPIx_SPIIE_ERRIE_Getable(SPIx_Type* SPIx);
extern void SPIx_SPIIE_TXIE_Setable(SPIx_Type* SPIx, FunState NewState);
extern FunState SPIx_SPIIE_TXIE_Getable(SPIx_Type* SPIx);
extern void SPIx_SPIIE_RXIE_Setable(SPIx_Type* SPIx, FunState NewState);
extern FunState SPIx_SPIIE_RXIE_Getable(SPIx_Type* SPIx);

/* Master Error标志 相关函数 */
extern FlagStatus SPIx_SPIIF_MERR_Chk(SPIx_Type* SPIx);

/* Slave Error标志 相关函数 */
extern FlagStatus SPIx_SPIIF_SERR_Chk(SPIx_Type* SPIx);

/* 接收缓存溢出，软件写1清零 相关函数 */
extern void SPIx_SPIIF_RXCOL_Clr(SPIx_Type* SPIx);
extern FlagStatus SPIx_SPIIF_RXCOL_Chk(SPIx_Type* SPIx);

/* 发送缓存溢出，软件写1清零 相关函数 */
extern void SPIx_SPIIF_TXCOL_Clr(SPIx_Type* SPIx);
extern FlagStatus SPIx_SPIIF_TXCOL_Chk(SPIx_Type* SPIx);

/* SPI空闲标志，只读 相关函数 */
extern FlagStatus SPIx_SPIIF_BUSY_Chk(SPIx_Type* SPIx);

/* TX Buffer Empty标志位 相关函数 */
extern FlagStatus SPIx_SPIIF_TXBE_Chk(SPIx_Type* SPIx);

/* RX Buffer Full标志位 相关函数 */
extern FlagStatus SPIx_SPIIF_RXBF_Chk(SPIx_Type* SPIx);

/* SPI发送缓存 相关函数 */
extern void SPIx_SPITXBUF_Write(SPIx_Type* SPIx, uint32_t SetValue);
extern uint32_t SPIx_SPITXBUF_Read(SPIx_Type* SPIx);

/* SPI接收缓存 相关函数 */
extern void SPIx_SPIRXBUF_Write(SPIx_Type* SPIx, uint32_t SetValue);
extern uint32_t SPIx_SPIRXBUF_Read(SPIx_Type* SPIx);
//Announce_End


/* Exported functions --------------------------------------------------------*/ 
extern void SPIx_Deinit(SPIx_Type* SPIx);

/* Master对MISO信号的采样位置调整，用于高速通信时补偿PCB走线延迟 相关函数 */
extern void SPIx_SPICR1_MSPA_Set(SPIx_Type* SPIx, uint32_t SetValue);
extern uint32_t SPIx_SPICR1_MSPA_Get(SPIx_Type* SPIx);

/* Slave MISO发送位置调整 相关函数 */
extern void SPIx_SPICR1_SSPA_Set(SPIx_Type* SPIx, uint32_t SetValue);
extern uint32_t SPIx_SPICR1_SSPA_Get(SPIx_Type* SPIx);

/* MASTER/SLAVE模式选择 相关函数 */
extern void SPIx_SPICR1_MM_Set(SPIx_Type* SPIx, uint32_t SetValue);
extern uint32_t SPIx_SPICR1_MM_Get(SPIx_Type* SPIx);
extern void SPIx_SPICR1_WAIT_Set(SPIx_Type* SPIx, uint32_t SetValue);
extern uint32_t SPIx_SPICR1_WAIT_Get(SPIx_Type* SPIx);

/* MASTER模式波特率配置 相关函数 */
extern void SPIx_SPICR1_BAUD_Set(SPIx_Type* SPIx, uint32_t SetValue);
extern uint32_t SPIx_SPICR1_BAUD_Get(SPIx_Type* SPIx);

/* 帧格式 相关函数 */
extern void SPIx_SPICR1_LSBF_Set(SPIx_Type* SPIx, uint32_t SetValue);
extern uint32_t SPIx_SPICR1_LSBF_Get(SPIx_Type* SPIx);

/* 时钟极性选择 相关函数 */
extern void SPIx_SPICR1_CPHOL_Set(SPIx_Type* SPIx, uint32_t SetValue);
extern uint32_t SPIx_SPICR1_CPHOL_Get(SPIx_Type* SPIx);

/* 时钟相位选择 相关函数 */
extern void SPIx_SPICR1_CPHA_Set(SPIx_Type* SPIx, uint32_t SetValue);
extern uint32_t SPIx_SPICR1_CPHA_Get(SPIx_Type* SPIx);

/* Slave输入管脚滤波使能（SSN/SCK/MOSI） 相关函数 */
extern void SPIx_SPICR2_FLTEN_Setable(SPIx_Type* SPIx, FunState NewState);
extern FunState SPIx_SPICR2_FLTEN_Getable(SPIx_Type* SPIx);

/* Master模式下SSN控制模式选择 相关函数 */
extern void SPIx_SPICR2_SSNM_Set(SPIx_Type* SPIx, uint32_t SetValue);
extern uint32_t SPIx_SPICR2_SSNM_Get(SPIx_Type* SPIx);

/* TXONLY硬件自动清空的使能 相关函数 */
extern void SPIx_SPICR2_TXO_AC_Set(SPIx_Type* SPIx, uint32_t SetValue);
extern uint32_t SPIx_SPICR2_TXO_AC_Get(SPIx_Type* SPIx);

/* TXONLY控制位 相关函数 */
extern void SPIx_SPICR2_TXO_Setable(SPIx_Type* SPIx, FunState NewState);
extern FunState SPIx_SPICR2_TXO_Getable(SPIx_Type* SPIx);

/* Master模式下，软件可以通过此位控制SSN输出电平 相关函数 */
extern void SPIx_SPICR2_SSN_Set(SPIx_Type* SPIx, uint32_t SetValue);
extern uint32_t SPIx_SPICR2_SSN_Get(SPIx_Type* SPIx);

/* Master模式下，SSN的控制方式 相关函数 */
extern void SPIx_SPICR2_SSNSEN_Set(SPIx_Type* SPIx, uint32_t SetValue);
extern uint32_t SPIx_SPICR2_SSNSEN_Get(SPIx_Type* SPIx);

/* SPI使能 相关函数 */
extern void SPIx_SPICR2_SPIEN_Setable(SPIx_Type* SPIx, FunState NewState);
extern FunState SPIx_SPICR2_SPIEN_Getable(SPIx_Type* SPIx);
extern void SPIx_SPICR3_TXBFC_Clr(SPIx_Type* SPIx);
extern void SPIx_SPICR3_RXBFC_Clr(SPIx_Type* SPIx);
extern void SPIx_SPICR3_MERRC_Clr(SPIx_Type* SPIx);
extern void SPIx_SPICR3_SERRC_Clr(SPIx_Type* SPIx);
extern void SPIx_SPIIE_ERRIE_Setable(SPIx_Type* SPIx, FunState NewState);
extern FunState SPIx_SPIIE_ERRIE_Getable(SPIx_Type* SPIx);
extern void SPIx_SPIIE_TXIE_Setable(SPIx_Type* SPIx, FunState NewState);
extern FunState SPIx_SPIIE_TXIE_Getable(SPIx_Type* SPIx);
extern void SPIx_SPIIE_RXIE_Setable(SPIx_Type* SPIx, FunState NewState);
extern FunState SPIx_SPIIE_RXIE_Getable(SPIx_Type* SPIx);

/* Master Error标志 相关函数 */
extern FlagStatus SPIx_SPIIF_MERR_Chk(SPIx_Type* SPIx);

/* Slave Error标志 相关函数 */
extern FlagStatus SPIx_SPIIF_SERR_Chk(SPIx_Type* SPIx);

/* 接收缓存溢出，软件写1清零 相关函数 */
extern void SPIx_SPIIF_RXCOL_Clr(SPIx_Type* SPIx);
extern FlagStatus SPIx_SPIIF_RXCOL_Chk(SPIx_Type* SPIx);

/* 发送缓存溢出，软件写1清零 相关函数 */
extern void SPIx_SPIIF_TXCOL_Clr(SPIx_Type* SPIx);
extern FlagStatus SPIx_SPIIF_TXCOL_Chk(SPIx_Type* SPIx);

/* SPI空闲标志，只读 相关函数 */
extern FlagStatus SPIx_SPIIF_BUSY_Chk(SPIx_Type* SPIx);

/* TX Buffer Empty标志位 相关函数 */
extern FlagStatus SPIx_SPIIF_TXBE_Chk(SPIx_Type* SPIx);

/* RX Buffer Full标志位 相关函数 */
extern FlagStatus SPIx_SPIIF_RXBF_Chk(SPIx_Type* SPIx);

/* SPI发送缓存 相关函数 */
extern void SPIx_SPITXBUF_Write(SPIx_Type* SPIx, uint32_t SetValue);
extern uint32_t SPIx_SPITXBUF_Read(SPIx_Type* SPIx);

/* SPI接收缓存 相关函数 */
extern void SPIx_SPIRXBUF_Write(SPIx_Type* SPIx, uint32_t SetValue);
extern uint32_t SPIx_SPIRXBUF_Read(SPIx_Type* SPIx);
//Announce_End


//Announce_HandBy
extern void SPI_Master_Init(SPIx_Type* SPIx, SPI_Master_SInitTypeDef* para);
extern void SPI_Slave_Init(SPIx_Type* SPIx, SPI_Slave_SInitTypeDef* para);

extern void SPI_SSN_Set_Low(SPIx_Type* SPIx);//SSN置低
extern void SPI_SSN_Set_High(SPIx_Type* SPIx);//SSN置高

extern unsigned char SPI_Recv_Byte(SPIx_Type* SPIx);//SPI接收一字节
extern void SPI_Send_Byte(SPIx_Type* SPIx, unsigned char data);//SPI发送一字节

extern unsigned char SPI_RW_Byte(SPIx_Type* SPIx, unsigned char data);//SPI发送并接收一字节
//Announce_End


#ifdef __cplusplus
}
#endif

#endif /* __FM33A0XX_SPI_H */



/************************ (C) COPYRIGHT FMSHelectronics *****END OF FILE****/











