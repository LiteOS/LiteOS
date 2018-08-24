/**
  ******************************************************************************
  * @file    fm33a0xx_hspi.h
  * @author  FM33a0xx Application Team
  * @version V2.0.1
  * @date    
  * @brief   This file contains all the functions prototypes for the SPI firmware library.  
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FM33A0XX_HSPI_H
#define __FM33A0XX_HSPI_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
//#include "FM33A0XX_define_all.h"
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

typedef struct
{
	//uint32_t DELAY_CFG;       //slave模式下对发送相位进行调整，建议使用复位值，不开放/* SETTING SENING AHESD HALF CYCLE */
	//FunState FLTEN;             //slave模式下使能4ns滤波/* SET PAD(SSN/SCK/MOSI) FILTER ENABLE AT SLAVE  */	
	uint32_t LSBF;             	//先发送高位bit还是低位bit/* SETTING 1 FOR LSB */		
    uint32_t CPHOL;			    //时钟极性选择/* SETTING 1 FOR CLK STOP AT HIGH*/	
	uint32_t CPHA;				//时钟相位选择/* SETTING 1 FOR START AT SECOND CLK EDGE AND 0 FOR FIRST */
}HSPI_Slave_SInitTypeDef;

typedef struct
{
	uint32_t DELAY_CFG;         //slave模式下对发送相位进行调整
	FunState FLTEN;             //slave模式下使能4ns滤波
	uint32_t LSBF;             	//先发送高位bit还是低位bit/* SETTING 1 FOR LSB */		
    uint32_t CPHOL;			    //时钟极性选择/* SETTING 1 FOR CLK STOP AT HIGH*/	
	uint32_t CPHA;				//时钟相位选择/* SETTING 1 FOR START AT SECOND CLK EDGE AND 0 FOR FIRST */
	FunState TXO;               //TXONLY模式
	uint32_t TXO_AC;            //TXONLY硬件自动清除配置

	FunState HSPIEN;             //SPI使能，初始化时建议写为DISABLE
	FunState ERRIE;             //SPI错误中断使能，初始化时建议写为DISABLE
	FunState TXIE;              //SPI发送完成中断使能，初始化时建议写为DISABLE
	FunState RXIE;              //SPI接收完成中断使能，初始化时建议写为DISABLE
}HSPI_Slave_InitTypeDef;

typedef struct
{
	//uint32_t DELAY_CFG        //master模式下对采样相位进行调整，建议使用复位值，不开放
	uint32_t BAUD_RATE;         //波特率/* SETTING BAUD_RATE */	
    uint32_t LSBF;             	//先发送高位bit还是低位bit/* SETTING 1 FOR LSB */		
    uint32_t CPHOL;			    //时钟极性选择/* SETTING 1 FOR CLK STOP AT HIGH*/	
	uint32_t CPHA;				//时钟相位选择/* SETTING 1 FOR START AT SECOND CLK EDGE AND 0 FOR FIRST */
	uint32_t SSNM;				//每次发完8bit后保持SSN为低还是拉高，SSN硬件模式有效/* SETTING SSN AFTER SENDING */	
	uint32_t SSNSEN;			//SSN由硬件控制还是由软件控制/* SETTING 1 FOR CONTROL SSN BY SOFTWARE */

}HSPI_Master_SInitTypeDef;

typedef struct
{
	uint32_t DELAY_CFG;         //master模式下对采样相位进行调整
	uint32_t WAIT;              //master模式下，每次发送完8bit至少加入几个sck周期的等待时间
	uint32_t BAUD_RATE;         //波特率/* SETTING BAUD_RATE */	
    uint32_t LSBF;             	//先发送高位bit还是低位bit/* SETTING 1 FOR LSB */		
    uint32_t CPHOL;			    //时钟极性选择/* SETTING 1 FOR CLK STOP AT HIGH*/	
	uint32_t CPHA;				//时钟相位选择/* SETTING 1 FOR START AT SECOND CLK EDGE AND 0 FOR FIRST */
	FunState TXO;               //TXONLY模式
	uint32_t TXO_AC;            //TXONLY硬件自动清除配置
    uint32_t SSNM;				//每次发完8bit后保持SSN为低还是拉高，SSN硬件模式有效/* SETTING SSN AFTER SENDING */ 
	uint32_t SSNSEN;			//SSN由硬件控制还是由软件控制/* SETTING 1 FOR CONTROL SSN BY SOFTWARE */

	FunState HSPIEN;             //SPI使能，初始化时建议写为DISABLE
	FunState ERRIE;             //SPI错误中断使能，初始化时建议写为DISABLE
	FunState TXIE;              //SPI发送完成中断使能，初始化时建议写为DISABLE
	FunState RXIE;              //SPI接收完成中断使能，初始化时建议写为DISABLE
}HSPI_Master_InitTypeDef;


//Macro_START
//2018-04-02-13-22-40
//FM33A0XX_Driver_Gen_V1.4

#define	HSPI_SPICR1_DELAY_CFG_Pos	9	/* Master模式下，用于调整接收采样相位；Slave模式下，用于调整发送相位 */
#define	HSPI_SPICR1_DELAY_CFG_Msk	(0x7U << HSPI_SPICR1_DELAY_CFG_Pos)
#define	HSPI_SPICR1_DELAY_CFG_MASTER_NO_DELAY	(0x0U << HSPI_SPICR1_DELAY_CFG_Pos)	/* Master模式，无延迟 */
#define	HSPI_SPICR1_DELAY_CFG_SLAVE_NORMAL	(0x0U << HSPI_SPICR1_DELAY_CFG_Pos)	/* Slave模式，正常发送 */
#define	HSPI_SPICR1_DELAY_CFG_MASTER_DLY_L1	(0x1U << HSPI_SPICR1_DELAY_CFG_Pos)	/* Master模式，1级延迟 */
#define	HSPI_SPICR1_DELAY_CFG_SLAVE_PHASE_1	(0x1U << HSPI_SPICR1_DELAY_CFG_Pos)	/* Slave模式，发送相位1 */
#define	HSPI_SPICR1_DELAY_CFG_MASTER_DLY_L2	(0x2U << HSPI_SPICR1_DELAY_CFG_Pos)	/* Master模式，2级延迟 */
#define	HSPI_SPICR1_DELAY_CFG_SLAVE_PHASE_2	(0x2U << HSPI_SPICR1_DELAY_CFG_Pos)	/* Slave模式，发送相位2 */
#define	HSPI_SPICR1_DELAY_CFG_MASTER_DLY_L3	(0x3U << HSPI_SPICR1_DELAY_CFG_Pos)	/* Master模式，3级延迟 */
#define	HSPI_SPICR1_DELAY_CFG_SLAVE_PHASE_3	(0x3U << HSPI_SPICR1_DELAY_CFG_Pos)	/* Slave模式，发送相位3 */
#define	HSPI_SPICR1_DELAY_CFG_MASTER_DLY_L4	(0x4U << HSPI_SPICR1_DELAY_CFG_Pos)	/* Master模式，4级延迟 */
#define	HSPI_SPICR1_DELAY_CFG_SLAVE_PHASE_4	(0x4U << HSPI_SPICR1_DELAY_CFG_Pos)	/* Slave模式，发送相位4 */

#define	HSPI_SPICR1_MM_Pos	8	/* MASTER/SLAVE模式选择 */
#define	HSPI_SPICR1_MM_Msk	(0x1U << HSPI_SPICR1_MM_Pos)
#define	HSPI_SPICR1_MM_SLAVE	(0x0U << HSPI_SPICR1_MM_Pos)	/* Slave模式 */
#define	HSPI_SPICR1_MM_MASTER	(0x1U << HSPI_SPICR1_MM_Pos)	/* Master模式 */

#define	HSPI_SPICR1_WAIT_Pos	6
#define	HSPI_SPICR1_WAIT_Msk	(0x3U << HSPI_SPICR1_WAIT_Pos)
#define	HSPI_SPICR1_WAIT_WAIT_1	(0x0U << HSPI_SPICR1_WAIT_Pos)	/* 每发完8bit至少加入1个sck周期等待时间再传输下一个8Bit数据 */
#define	HSPI_SPICR1_WAIT_WAIT_2	(0x1U << HSPI_SPICR1_WAIT_Pos)	/* 每发完8bit至少加入2个sck周期等待时间再传输下一个8Bit数据 */
#define	HSPI_SPICR1_WAIT_WAIT_3	(0x2U << HSPI_SPICR1_WAIT_Pos)	/* 每发完8bit至少加入3个sck周期等待时间再传输下一个8Bit数据 */
#define	HSPI_SPICR1_WAIT_WAIT_4	(0x3U << HSPI_SPICR1_WAIT_Pos)	/* 每发完8bit至少加入4个sck周期等待时间再传输下一个8Bit数据 */

#define	HSPI_SPICR1_BAUD_Pos	3	/* MASTER模式波特率配置 */
#define	HSPI_SPICR1_BAUD_Msk	(0x7U << HSPI_SPICR1_BAUD_Pos)
#define	HSPI_SPICR1_BAUD_PCLK	(0x0U << HSPI_SPICR1_BAUD_Pos)	/* 外设时钟不分频 */
#define	HSPI_SPICR1_BAUD_PCLK_2	(0x1U << HSPI_SPICR1_BAUD_Pos)	/* 外设时钟2分频 */
#define	HSPI_SPICR1_BAUD_PCLK_4	(0x2U << HSPI_SPICR1_BAUD_Pos)	/* 外设时钟4分频 */
#define	HSPI_SPICR1_BAUD_PCLK_8	(0x3U << HSPI_SPICR1_BAUD_Pos)	/* 外设时钟8分频 */
#define	HSPI_SPICR1_BAUD_PCLK_16	(0x4U << HSPI_SPICR1_BAUD_Pos)	/* 外设时钟16分频 */
#define	HSPI_SPICR1_BAUD_PCLK_32	(0x5U << HSPI_SPICR1_BAUD_Pos)	/* 外设时钟32分频 */
#define	HSPI_SPICR1_BAUD_PCLK_64	(0x6U << HSPI_SPICR1_BAUD_Pos)	/* 外设时钟64分频 */
#define	HSPI_SPICR1_BAUD_PCLK_128	(0x7U << HSPI_SPICR1_BAUD_Pos)	/* 外设时钟128分频 */

#define	HSPI_SPICR1_LSBF_Pos	2	/* 帧格式 */
#define	HSPI_SPICR1_LSBF_Msk	(0x1U << HSPI_SPICR1_LSBF_Pos)
#define	HSPI_SPICR1_LSBF_MSB	(0x0U << HSPI_SPICR1_LSBF_Pos)	/* 先发送MSB */
#define	HSPI_SPICR1_LSBF_LSB	(0x1U << HSPI_SPICR1_LSBF_Pos)	/* 先发送LSB */

#define	HSPI_SPICR1_CPHOL_Pos	1	/* 时钟极性选择 */
#define	HSPI_SPICR1_CPHOL_Msk	(0x1U << HSPI_SPICR1_CPHOL_Pos)
#define	HSPI_SPICR1_CPHOL_LOW	(0x0U << HSPI_SPICR1_CPHOL_Pos)	/* 串行时钟停止在低电平 */
#define	HSPI_SPICR1_CPHOL_HIGH	(0x1U << HSPI_SPICR1_CPHOL_Pos)	/* 串行时钟停止在高电平 */

#define	HSPI_SPICR1_CPHA_Pos	0	/* 时钟相位选择 */
#define	HSPI_SPICR1_CPHA_Msk	(0x1U << HSPI_SPICR1_CPHA_Pos)
#define	HSPI_SPICR1_CPHA_FIRST	(0x0U << HSPI_SPICR1_CPHA_Pos)	/* 第一个时钟边沿是第一个捕捉边沿 */
#define	HSPI_SPICR1_CPHA_SECOND	(0x1U << HSPI_SPICR1_CPHA_Pos)	/* 第二个时钟边沿是第一个捕捉边沿 */

#define	HSPI_SPICR2_FLTEN_Pos	6	/* Slave输入管脚滤波使能（SSN/SCK/MOSI） */
#define	HSPI_SPICR2_FLTEN_Msk	(0x1U << HSPI_SPICR2_FLTEN_Pos)
	/* 0：不使能4ns滤波 */
	/* 1：使能4ns滤波 */

#define	HSPI_SPICR2_SSNM_Pos	5	/* Master模式下SSN控制模式选择 */
#define	HSPI_SPICR2_SSNM_Msk	(0x1U << HSPI_SPICR2_SSNM_Pos)
#define	HSPI_SPICR2_SSNM_LOW	(0x0U << HSPI_SPICR2_SSNM_Pos)	/* 每发送完8bit后Master保持SSN为低 */
#define	HSPI_SPICR2_SSNM_HIGH	(0x1U << HSPI_SPICR2_SSNM_Pos)	/* 每发送完8bit后Master拉高SSN，维持高电平时间由WAIT寄存器控制 */

#define	HSPI_SPICR2_TXO_AC_Pos	4	/* TXONLY硬件自动清空的使能 */
#define	HSPI_SPICR2_TXO_AC_Msk	(0x1U << HSPI_SPICR2_TXO_AC_Pos)
#define	HSPI_SPICR2_TXO_AC_CLR_AT_CLOSE	(0x0U << HSPI_SPICR2_TXO_AC_Pos)	/* 关闭TXONLY硬件自动清零 */
#define	HSPI_SPICR2_TXO_AC_CLR_AT_TX	(0x1U << HSPI_SPICR2_TXO_AC_Pos)	/* TXONLY硬件自动清零有效，软件使能TXO后，等待发送完毕后，硬件清零 */

#define	HSPI_SPICR2_TXO_Pos	3	/* TXONLY控制位 */
#define	HSPI_SPICR2_TXO_Msk	(0x1U << HSPI_SPICR2_TXO_Pos)
	/* 0：关闭Master的单发送模式 */
	/* 1：启动Master的单发送模式 */

#define	HSPI_SPICR2_SSN_Pos	2	/* Master模式下，软件可以通过此位控制SSN输出电平 */
#define	HSPI_SPICR2_SSN_Msk	(0x1U << HSPI_SPICR2_SSN_Pos)
#define	HSPI_SPICR2_SSN_LOW	(0x0U << HSPI_SPICR2_SSN_Pos)	/* SSN输出低电平 */
#define	HSPI_SPICR2_SSN_HIGH	(0x1U << HSPI_SPICR2_SSN_Pos)	/* SSN输出高电平 */

#define	HSPI_SPICR2_SSNSEN_Pos	1	/* Master模式下，SSN的控制方式 */
#define	HSPI_SPICR2_SSNSEN_Msk	(0x1U << HSPI_SPICR2_SSNSEN_Pos)
#define	HSPI_SPICR2_SSNSEN_HARD	(0x0U << HSPI_SPICR2_SSNSEN_Pos)	/* Master模式下SSN输出由硬件自动控制 */
#define	HSPI_SPICR2_SSNSEN_SOFT	(0x1U << HSPI_SPICR2_SSNSEN_Pos)	/* Master模式下SSN输出由软件控制 */

#define	HSPI_SPICR2_HSPIEN_Pos	0	/* HSPI使能 */
#define	HSPI_SPICR2_HSPIEN_Msk	(0x1U << HSPI_SPICR2_HSPIEN_Pos)
	/* 0：关闭HSPI */
	/* 1：使能HSPI */

#define	HSPI_SPICR3_TXBFC_Pos	3
#define	HSPI_SPICR3_TXBFC_Msk	(0x1U << HSPI_SPICR3_TXBFC_Pos)

#define	HSPI_SPICR3_RXBFC_Pos	2
#define	HSPI_SPICR3_RXBFC_Msk	(0x1U << HSPI_SPICR3_RXBFC_Pos)

#define	HSPI_SPICR3_MERRC_Pos	1
#define	HSPI_SPICR3_MERRC_Msk	(0x1U << HSPI_SPICR3_MERRC_Pos)

#define	HSPI_SPICR3_SERRC_Pos	0
#define	HSPI_SPICR3_SERRC_Msk	(0x1U << HSPI_SPICR3_SERRC_Pos)

#define	HSPI_SPIIE_ERRIE_Pos	2
#define	HSPI_SPIIE_ERRIE_Msk	(0x1U << HSPI_SPIIE_ERRIE_Pos)
	/* 0：HSPI错误中断不使能 */
	/* 1：HSPI错误中断使能 */

#define	HSPI_SPIIE_TXIE_Pos	1
#define	HSPI_SPIIE_TXIE_Msk	(0x1U << HSPI_SPIIE_TXIE_Pos)
	/* 0：发送完成中断不使能 */
	/* 1：发送完成中断使能 */

#define	HSPI_SPIIE_RXIE_Pos	0
#define	HSPI_SPIIE_RXIE_Msk	(0x1U << HSPI_SPIIE_RXIE_Pos)
	/* 0：接收完成中断不使能 */

#define	HSPI_SPIIF_MERR_Pos	6	/* Master Error标志 */
#define	HSPI_SPIIF_MERR_Msk	(0x1U << HSPI_SPIIF_MERR_Pos)

#define	HSPI_SPIIF_SERR_Pos	5	/* Slave Error标志 */
#define	HSPI_SPIIF_SERR_Msk	(0x1U << HSPI_SPIIF_SERR_Pos)

#define	HSPI_SPIIF_RXCOL_Pos	4	/* 接收缓存溢出，软件写1清零 */
#define	HSPI_SPIIF_RXCOL_Msk	(0x1U << HSPI_SPIIF_RXCOL_Pos)

#define	HSPI_SPIIF_TXCOL_Pos	3	/* 发送缓存溢出，软件写1清零 */
#define	HSPI_SPIIF_TXCOL_Msk	(0x1U << HSPI_SPIIF_TXCOL_Pos)

#define	HSPI_SPIIF_BUSY_Pos	2	/* HSPI空闲标志，只读 */
#define	HSPI_SPIIF_BUSY_Msk	(0x1U << HSPI_SPIIF_BUSY_Pos)

#define	HSPI_SPIIF_TXBE_Pos	1	/* TX Buffer Empty标志位 */
#define	HSPI_SPIIF_TXBE_Msk	(0x1U << HSPI_SPIIF_TXBE_Pos)

#define	HSPI_SPIIF_RXBF_Pos	0	/* RX Buffer Full标志位 */
#define	HSPI_SPIIF_RXBF_Msk	(0x1U << HSPI_SPIIF_RXBF_Pos)

#define	HSPI_SPITXBUF_TXBUF_Pos	0	/* SPI发送缓存 */
#define	HSPI_SPITXBUF_TXBUF_Msk	(0xffU << HSPI_SPITXBUF_TXBUF_Pos)

#define	HSPI_SPIRXBUF_RXBUF_Pos	0	/* SPI接收缓存 */
#define	HSPI_SPIRXBUF_RXBUF_Msk	(0xffU << HSPI_SPIRXBUF_RXBUF_Pos)
//Macro_End

/* Exported functions --------------------------------------------------------*/ 
extern void HSPI_Deinit(void);

/* Master模式下，用于调整接收采样相位；Slave模式下，用于调整发送相位 相关函数 */
extern void HSPI_SPICR1_DELAY_CFG_Set(uint32_t SetValue);
extern uint32_t HSPI_SPICR1_DELAY_CFG_Get(void);

/* MASTER/SLAVE模式选择 相关函数 */
extern void HSPI_SPICR1_MM_Set(uint32_t SetValue);
extern uint32_t HSPI_SPICR1_MM_Get(void);
extern void HSPI_SPICR1_WAIT_Set(uint32_t SetValue);
extern uint32_t HSPI_SPICR1_WAIT_Get(void);

/* MASTER模式波特率配置 相关函数 */
extern void HSPI_SPICR1_BAUD_Set(uint32_t SetValue);
extern uint32_t HSPI_SPICR1_BAUD_Get(void);

/* 帧格式 相关函数 */
extern void HSPI_SPICR1_LSBF_Set(uint32_t SetValue);
extern uint32_t HSPI_SPICR1_LSBF_Get(void);

/* 时钟极性选择 相关函数 */
extern void HSPI_SPICR1_CPHOL_Set(uint32_t SetValue);
extern uint32_t HSPI_SPICR1_CPHOL_Get(void);

/* 时钟相位选择 相关函数 */
extern void HSPI_SPICR1_CPHA_Set(uint32_t SetValue);
extern uint32_t HSPI_SPICR1_CPHA_Get(void);

/* Slave输入管脚滤波使能（SSN/SCK/MOSI） 相关函数 */
extern void HSPI_SPICR2_FLTEN_Setable(FunState NewState);
extern FunState HSPI_SPICR2_FLTEN_Getable(void);

/* Master模式下SSN控制模式选择 相关函数 */
extern void HSPI_SPICR2_SSNM_Set(uint32_t SetValue);
extern uint32_t HSPI_SPICR2_SSNM_Get(void);

/* TXONLY硬件自动清空的使能 相关函数 */
extern void HSPI_SPICR2_TXO_AC_Set(uint32_t SetValue);
extern uint32_t HSPI_SPICR2_TXO_AC_Get(void);

/* TXONLY控制位 相关函数 */
extern void HSPI_SPICR2_TXO_Setable(FunState NewState);
extern FunState HSPI_SPICR2_TXO_Getable(void);

/* Master模式下，软件可以通过此位控制SSN输出电平 相关函数 */
extern void HSPI_SPICR2_SSN_Set(uint32_t SetValue);
extern uint32_t HSPI_SPICR2_SSN_Get(void);

/* Master模式下，SSN的控制方式 相关函数 */
extern void HSPI_SPICR2_SSNSEN_Set(uint32_t SetValue);
extern uint32_t HSPI_SPICR2_SSNSEN_Get(void);

/* HSPI使能 相关函数 */
extern void HSPI_SPICR2_HSPIEN_Setable(FunState NewState);
extern FunState HSPI_SPICR2_HSPIEN_Getable(void);
extern void HSPI_SPICR3_TXBFC_Clr(void);
extern void HSPI_SPICR3_RXBFC_Clr(void);
extern void HSPI_SPICR3_MERRC_Clr(void);
extern void HSPI_SPICR3_SERRC_Clr(void);
extern void HSPI_SPIIE_ERRIE_Setable(FunState NewState);
extern FunState HSPI_SPIIE_ERRIE_Getable(void);
extern void HSPI_SPIIE_TXIE_Setable(FunState NewState);
extern FunState HSPI_SPIIE_TXIE_Getable(void);
extern void HSPI_SPIIE_RXIE_Setable(FunState NewState);
extern FunState HSPI_SPIIE_RXIE_Getable(void);

/* Master Error标志 相关函数 */
extern FlagStatus HSPI_SPIIF_MERR_Chk(void);

/* Slave Error标志 相关函数 */
extern FlagStatus HSPI_SPIIF_SERR_Chk(void);

/* 接收缓存溢出，软件写1清零 相关函数 */
extern void HSPI_SPIIF_RXCOL_Clr(void);
extern FlagStatus HSPI_SPIIF_RXCOL_Chk(void);

/* 发送缓存溢出，软件写1清零 相关函数 */
extern void HSPI_SPIIF_TXCOL_Clr(void);
extern FlagStatus HSPI_SPIIF_TXCOL_Chk(void);

/* HSPI空闲标志，只读 相关函数 */
extern FlagStatus HSPI_SPIIF_BUSY_Chk(void);

/* TX Buffer Empty标志位 相关函数 */
extern FlagStatus HSPI_SPIIF_TXBE_Chk(void);

/* RX Buffer Full标志位 相关函数 */
extern FlagStatus HSPI_SPIIF_RXBF_Chk(void);

/* SPI发送缓存 相关函数 */
extern void HSPI_SPITXBUF_Write(uint32_t SetValue);
extern uint32_t HSPI_SPITXBUF_Read(void);

/* SPI接收缓存 相关函数 */
extern void HSPI_SPIRXBUF_Write(uint32_t SetValue);
extern uint32_t HSPI_SPIRXBUF_Read(void);
//Announce_End

//Code_Start_HandBy
extern void HSPI_Master_Init(HSPI_Master_SInitTypeDef* para);

extern void HSPI_Slave_Init(HSPI_Slave_SInitTypeDef* para);

extern void HSPI_SSN_Set_Low(void);//驱动级

extern void HSPI_SSN_Set_High(void);//驱动级

extern unsigned char HSPI_Recv_Byte(void);//HSPI接收一字节
extern void HSPI_Send_Byte(unsigned char data);//HSPI发送一字节
extern unsigned char HSPI_RW_Byte(unsigned char data);//SPI发送并接收一字节

#ifdef __cplusplus
  }
#endif
  
#endif /* __FM33A0XX_SPI_H */
  
  
  
  /************************ (C) COPYRIGHT FMSHelectronics *****END OF FILE****/
  
  
  

