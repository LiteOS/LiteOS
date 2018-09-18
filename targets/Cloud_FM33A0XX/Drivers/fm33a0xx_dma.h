/**
  ******************************************************************************
  * @file    fm33a0xx_dma.h
  * @author  FM33a0xx Application Team
  * @version V2.0.1
  * @date    27-3-2018
  * @brief   This file contains all the functions prototypes for the DMA firmware library.  
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FM33A0XX_DMA_H
#define __FM33A0XX_DMA_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "FM33A0XX.h"


typedef enum {
DMA_CH0 = 0, DMA_CH1 = 1, DMA_CH2 = 2, DMA_CH3 = 3, DMA_CH4 = 4, DMA_CH5 = 5, DMA_CH6 = 6, DMA_CH7 = 7	
}DMA_CH_Type;


typedef struct
{
	DMA_CH_Type CHx;			//DMA通道号
	uint32_t CHxTSIZE;          //通道传输长度
	uint32_t CHxPRI;            //通道优先级
	uint32_t CHxINC;            //CH0~CH6通道地址增长方向,CH7无意义
	uint32_t CHxSSEL;           //CH0~CH6外设通道选择,CH7传输方式选择
	FunState CHxFTIE;           //通道传输完成中断使能
	FunState CHxHTIE;           //通道传输半程中断使能
	FunState CHxEN;             //通道使能
	uint32_t CHxRAMAD;          //CH0~CH7通道RAM指针地址,注意CH7使用的是word地址，驱动里已处理过了，

	uint32_t CH7FLSAD;			//通道FLASH指针地址，仅针对通道7有意义	
}DMA_InitTypeDef;


//Macro_START
//2018-03-02-15-19-03
//FM33A0XX_Driver_Gen_V1.3

#define	DMA_GLOBALCTRL_DMAEN_Pos	0	/* DMA全局使能 */
#define	DMA_GLOBALCTRL_DMAEN_Msk	(0x1U << DMA_GLOBALCTRL_DMAEN_Pos)
	/* 0：DMA关闭 */
	/* 1：DMA使能 */


#define	DMA_CHxCTRL_TSIZE_Pos	16	/* Channelx传输长度 */
#define	DMA_CHxCTRL_TSIZE_Msk	(0x1fffU << DMA_CHxCTRL_TSIZE_Pos)

#define	DMA_CHxCTRL_PRI_Pos	12	/* Channelx优先级 */
#define	DMA_CHxCTRL_PRI_Msk	(0x3U << DMA_CHxCTRL_PRI_Pos)
#define	DMA_CHxCTRL_PRI_LOW	(0x0U << DMA_CHxCTRL_PRI_Pos)	/* 低优先级 */
#define	DMA_CHxCTRL_PRI_MEDIUM	(0x1U << DMA_CHxCTRL_PRI_Pos)	/* 中优先级 */
#define	DMA_CHxCTRL_PRI_HIGH	(0x2U << DMA_CHxCTRL_PRI_Pos)	/* 高优先级 */
#define	DMA_CHxCTRL_PRI_VERY_HIGH	(0x3U << DMA_CHxCTRL_PRI_Pos)	/* 最高优先级 */

#define	DMA_CHxCTRL_INC_Pos	11	/* RAM地址增减设置 */
#define	DMA_CHxCTRL_INC_Msk	(0x1U << DMA_CHxCTRL_INC_Pos)
#define	DMA_CHxCTRL_INC_DEC	(0x0U << DMA_CHxCTRL_INC_Pos)	/* 地址递减 */
#define	DMA_CHxCTRL_INC_INC	(0x1U << DMA_CHxCTRL_INC_Pos)	/* 地址递增 */

#define	DMA_CHxCTRL_FTIE_Pos	2	/* Channelx传输完成中断使能 */
#define	DMA_CHxCTRL_FTIE_Msk	(0x1U << DMA_CHxCTRL_FTIE_Pos)
	/* 0：关闭传输完成中断 */
	/* 1：使能传输完成中断 */

#define	DMA_CHxCTRL_HTIE_Pos	1	/* Channelx半程传输完成中断使能 */
#define	DMA_CHxCTRL_HTIE_Msk	(0x1U << DMA_CHxCTRL_HTIE_Pos)
	/* 0：关闭半程中断 */
	/* 1：使能半程中断 */

#define	DMA_CHxCTRL_EN_Pos	0	/* Channelx使能 */
#define	DMA_CHxCTRL_EN_Msk	(0x1U << DMA_CHxCTRL_EN_Pos)
	/* 0：关闭通道0 */
	/* 1：使能通道0 */

#define	DMA_CHxRAMAD_RAMAD_Pos	0	/* Channelx指针寄存器 */
#define	DMA_CHxRAMAD_RAMAD_Msk	(0xffffU << DMA_CHxRAMAD_RAMAD_Pos)


#define	DMA_CHxCTRL_SSEL_Pos	8	/* Channel0外设通道选择 */
#define	DMA_CHxCTRL_SSEL_Msk	(0x7U << DMA_CH0CTRL_CH0SSEL_Pos)

#define	DMA_CH0CTRL_CH0SSEL_Pos	8	/* Channel0外设通道选择 */
#define	DMA_CH0CTRL_CH0SSEL_Msk	(0x7U << DMA_CH0CTRL_CH0SSEL_Pos)
#define	DMA_CH0CTRL_CH0SSEL_ET4_CAP	(0x0U << DMA_CH0CTRL_CH0SSEL_Pos)
#define	DMA_CH0CTRL_CH0SSEL_ET2_CAP	(0x1U << DMA_CH0CTRL_CH0SSEL_Pos)
#define	DMA_CH0CTRL_CH0SSEL_BT2_CAP	(0x2U << DMA_CH0CTRL_CH0SSEL_Pos)
#define	DMA_CH0CTRL_CH0SSEL_ET3_CAP	(0x3U << DMA_CH0CTRL_CH0SSEL_Pos)
#define	DMA_CH0CTRL_CH0SSEL_BT1_CAP	(0x4U << DMA_CH0CTRL_CH0SSEL_Pos)
#define	DMA_CH0CTRL_CH0SSEL_CRC_OUT	(0x5U << DMA_CH0CTRL_CH0SSEL_Pos)
#define	DMA_CH0CTRL_CH0SSEL_AES_IN	(0x6U << DMA_CH0CTRL_CH0SSEL_Pos)
#define	DMA_CH0CTRL_CH0SSEL_ADC_OUT	(0x7U << DMA_CH0CTRL_CH0SSEL_Pos)

#define	DMA_CH1CTRL_CH1SSEL_Pos	8	/* Channel1外设通道选择 */
#define	DMA_CH1CTRL_CH1SSEL_Msk	(0x7U << DMA_CH1CTRL_CH1SSEL_Pos)
#define	DMA_CH1CTRL_CH1SSEL_ET1_CAP	(0x0U << DMA_CH1CTRL_CH1SSEL_Pos)
#define	DMA_CH1CTRL_CH1SSEL_AES_OUT	(0x1U << DMA_CH1CTRL_CH1SSEL_Pos)
#define	DMA_CH1CTRL_CH1SSEL_I2C_RX	(0x2U << DMA_CH1CTRL_CH1SSEL_Pos)
#define	DMA_CH1CTRL_CH1SSEL_U78160_RX	(0x3U << DMA_CH1CTRL_CH1SSEL_Pos)
#define	DMA_CH1CTRL_CH1SSEL_UART3_RX	(0x4U << DMA_CH1CTRL_CH1SSEL_Pos)
#define	DMA_CH1CTRL_CH1SSEL_UART0_RX	(0x5U << DMA_CH1CTRL_CH1SSEL_Pos)
#define	DMA_CH1CTRL_CH1SSEL_HSPI_RX	(0x6U << DMA_CH1CTRL_CH1SSEL_Pos)
#define	DMA_CH1CTRL_CH1SSEL_ADC_OUT	(0x7U << DMA_CH1CTRL_CH1SSEL_Pos)

#define	DMA_CH2CTRL_CH2SSEL_Pos	8	/* Channel2外设通道选择 */
#define	DMA_CH2CTRL_CH2SSEL_Msk	(0x7U << DMA_CH2CTRL_CH2SSEL_Pos)
#define	DMA_CH2CTRL_CH2SSEL_ET3_CAP	(0x0U << DMA_CH2CTRL_CH2SSEL_Pos)
#define	DMA_CH2CTRL_CH2SSEL_BT1_CAP	(0x1U << DMA_CH2CTRL_CH2SSEL_Pos)
#define	DMA_CH2CTRL_CH2SSEL_AES_OUT	(0x2U << DMA_CH2CTRL_CH2SSEL_Pos)
#define	DMA_CH2CTRL_CH2SSEL_I2C_TX	(0x3U << DMA_CH2CTRL_CH2SSEL_Pos)
#define	DMA_CH2CTRL_CH2SSEL_U78160_TX	(0x4U << DMA_CH2CTRL_CH2SSEL_Pos)
#define	DMA_CH2CTRL_CH2SSEL_UART3_TX	(0x5U << DMA_CH2CTRL_CH2SSEL_Pos)
#define	DMA_CH2CTRL_CH2SSEL_UART0_TX	(0x6U << DMA_CH2CTRL_CH2SSEL_Pos)
#define	DMA_CH2CTRL_CH2SSEL_HSPI_TX	(0x7U << DMA_CH2CTRL_CH2SSEL_Pos)

#define	DMA_CH3CTRL_CH3SSEL_Pos	8	/* Channel3外设通道选择 */
#define	DMA_CH3CTRL_CH3SSEL_Msk	(0x7U << DMA_CH3CTRL_CH3SSEL_Pos)
#define	DMA_CH3CTRL_CH3SSEL_BT2_CAP	(0x0U << DMA_CH3CTRL_CH3SSEL_Pos)
#define	DMA_CH3CTRL_CH3SSEL_ET1_CAP	(0x1U << DMA_CH3CTRL_CH3SSEL_Pos)
#define	DMA_CH3CTRL_CH3SSEL_I2C_TX	(0x2U << DMA_CH3CTRL_CH3SSEL_Pos)
#define	DMA_CH3CTRL_CH3SSEL_U78161_RX	(0x3U << DMA_CH3CTRL_CH3SSEL_Pos)
#define	DMA_CH3CTRL_CH3SSEL_UART4_RX	(0x4U << DMA_CH3CTRL_CH3SSEL_Pos)
#define	DMA_CH3CTRL_CH3SSEL_UART1_RX	(0x5U << DMA_CH3CTRL_CH3SSEL_Pos)
#define	DMA_CH3CTRL_CH3SSEL_UART0_RX	(0x6U << DMA_CH3CTRL_CH3SSEL_Pos)
#define	DMA_CH3CTRL_CH3SSEL_SPI1_RX	(0x7U << DMA_CH3CTRL_CH3SSEL_Pos)

#define	DMA_CH4CTRL_CH4SSEL_Pos	8	/* Channel4外设通道选择 */
#define	DMA_CH4CTRL_CH4SSEL_Msk	(0x7U << DMA_CH4CTRL_CH4SSEL_Pos)
#define	DMA_CH4CTRL_CH4SSEL_ET2_CAP	(0x0U << DMA_CH4CTRL_CH4SSEL_Pos)
#define	DMA_CH4CTRL_CH4SSEL_AES_IN	(0x1U << DMA_CH4CTRL_CH4SSEL_Pos)
#define	DMA_CH4CTRL_CH4SSEL_I2C_RX	(0x2U << DMA_CH4CTRL_CH4SSEL_Pos)
#define	DMA_CH4CTRL_CH4SSEL_U78161_TX	(0x3U << DMA_CH4CTRL_CH4SSEL_Pos)
#define	DMA_CH4CTRL_CH4SSEL_UART4_TX	(0x4U << DMA_CH4CTRL_CH4SSEL_Pos)
#define	DMA_CH4CTRL_CH4SSEL_UART1_TX	(0x5U << DMA_CH4CTRL_CH4SSEL_Pos)
#define	DMA_CH4CTRL_CH4SSEL_UART0_TX	(0x6U << DMA_CH4CTRL_CH4SSEL_Pos)
#define	DMA_CH4CTRL_CH4SSEL_SPI1_TX	(0x7U << DMA_CH4CTRL_CH4SSEL_Pos)

#define	DMA_CH5CTRL_CH5SSEL_Pos	8	/* Channel5外设通道选择 */
#define	DMA_CH5CTRL_CH5SSEL_Msk	(0x7U << DMA_CH5CTRL_CH5SSEL_Pos)
#define	DMA_CH5CTRL_CH5SSEL_LPTIM_CAP	(0x0U << DMA_CH5CTRL_CH5SSEL_Pos)
#define	DMA_CH5CTRL_CH5SSEL_U78161_RX	(0x1U << DMA_CH5CTRL_CH5SSEL_Pos)
#define	DMA_CH5CTRL_CH5SSEL_U78160_RX	(0x2U << DMA_CH5CTRL_CH5SSEL_Pos)
#define	DMA_CH5CTRL_CH5SSEL_UART5_RX	(0x3U << DMA_CH5CTRL_CH5SSEL_Pos)
#define	DMA_CH5CTRL_CH5SSEL_UART2_RX	(0x4U << DMA_CH5CTRL_CH5SSEL_Pos)
#define	DMA_CH5CTRL_CH5SSEL_UART1_RX	(0x5U << DMA_CH5CTRL_CH5SSEL_Pos)
#define	DMA_CH5CTRL_CH5SSEL_SPI2_RX	(0x6U << DMA_CH5CTRL_CH5SSEL_Pos)
#define	DMA_CH5CTRL_CH5SSEL_HSPI_RX	(0x7U << DMA_CH5CTRL_CH5SSEL_Pos)

#define	DMA_CH6CTRL_CH6SSEL_Pos	8	/* Channel5外设通道选择 */
#define	DMA_CH6CTRL_CH6SSEL_Msk	(0x7U << DMA_CH6CTRL_CH6SSEL_Pos)
#define	DMA_CH6CTRL_CH6SSEL_LPTIM_CAP	(0x0U << DMA_CH6CTRL_CH6SSEL_Pos)
#define	DMA_CH6CTRL_CH6SSEL_U78161_TX	(0x1U << DMA_CH6CTRL_CH6SSEL_Pos)
#define	DMA_CH6CTRL_CH6SSEL_U78160_TX	(0x2U << DMA_CH6CTRL_CH6SSEL_Pos)
#define	DMA_CH6CTRL_CH6SSEL_UART5_TX	(0x3U << DMA_CH6CTRL_CH6SSEL_Pos)
#define	DMA_CH6CTRL_CH6SSEL_UART2_TX	(0x4U << DMA_CH6CTRL_CH6SSEL_Pos)
#define	DMA_CH6CTRL_CH6SSEL_UART1_TX	(0x5U << DMA_CH6CTRL_CH6SSEL_Pos)
#define	DMA_CH6CTRL_CH6SSEL_SPI2_TX	(0x6U << DMA_CH6CTRL_CH6SSEL_Pos)
#define	DMA_CH6CTRL_CH6SSEL_HSPI_TX	(0x7U << DMA_CH6CTRL_CH6SSEL_Pos)


//BIT10	CH7DIR	Channel7传输方向
//1：Flash->RAM传输
//0：RAM->Flash传输
//BIT9	CH7RI	Channel7 RAM地址增减设置，仅在Flash->RAM传输中有效
//1：RAM地址递增
//0：RAM地址递减
//BIT8	CH7FI	Channel7 Flash地址增减设置，仅在Flash->RAM传输中有效
//1：Flash地址递增
//0：Flash地址递减
#define	DMA_CH7CTRL_CH7SSEL_Pos	8	/* Channel7外设通道选择 */
#define	DMA_CH7CTRL_CH7SSEL_Msk	(0x7U << DMA_CH7CTRL_CH7SSEL_Pos)
#define	DMA_CH7CTRL_CH7SSEL_RFRMFM	(0x0U << DMA_CH7CTRL_CH7SSEL_Pos)	/* Channel7 RAM->FLASH传输 RAM地址递减 FLASH地址递减 */
#define	DMA_CH7CTRL_CH7SSEL_RFRMFP	(0x1U << DMA_CH7CTRL_CH7SSEL_Pos)	/* Channel7 RAM->FLASH传输 RAM地址递减 FLASH地址递增 */
#define	DMA_CH7CTRL_CH7SSEL_RFRPFM	(0x2U << DMA_CH7CTRL_CH7SSEL_Pos)	/* Channel7 RAM->FLASH传输 RAM地址递增 FLASH地址递减 */
#define	DMA_CH7CTRL_CH7SSEL_RFRPFP	(0x3U << DMA_CH7CTRL_CH7SSEL_Pos)	/* Channel7 RAM->FLASH传输 RAM地址递增 FLASH地址递增 */
#define	DMA_CH7CTRL_CH7SSEL_FRRMFM	(0x4U << DMA_CH7CTRL_CH7SSEL_Pos)	/* Channel7 FLASH->RAM传输 RAM地址递减 FLASH地址递减 */
#define	DMA_CH7CTRL_CH7SSEL_FRRMFP	(0x5U << DMA_CH7CTRL_CH7SSEL_Pos)	/* Channel7 FLASH->RAM传输 RAM地址递减 FLASH地址递增 */
#define	DMA_CH7CTRL_CH7SSEL_FRRPFM	(0x6U << DMA_CH7CTRL_CH7SSEL_Pos)	/* Channel7 FLASH->RAM传输 RAM地址递增 FLASH地址递减 */
#define	DMA_CH7CTRL_CH7SSEL_FRRPFP	(0x7U << DMA_CH7CTRL_CH7SSEL_Pos)	/* Channel7 FLASH->RAM传输 RAM地址递增 FLASH地址递增 */

//#define	DMA_CH7CTRL_CH7DIR_Pos	10	/* Channel7传输方向 */
//#define	DMA_CH7CTRL_CH7DIR_Msk	(0x1U << DMA_CH7CTRL_CH7DIR_Pos)
//#define	DMA_CH7CTRL_CH7DIR_RAM2FLASH	(0x0U << DMA_CH7CTRL_CH7DIR_Pos)	/* RAM->FLASH传输 */
//#define	DMA_CH7CTRL_CH7DIR_FLASH2RAM	(0x1U << DMA_CH7CTRL_CH7DIR_Pos)	/* FLASH->RAM传输 */

//#define	DMA_CH7CTRL_CH7RI_Pos	9	/* Channel7RAM地址增减设置 */
//#define	DMA_CH7CTRL_CH7RI_Msk	(0x1U << DMA_CH7CTRL_CH7RI_Pos)
//#define	DMA_CH7CTRL_CH7RI_DEC	(0x0U << DMA_CH7CTRL_CH7RI_Pos)	/* RAM地址递减 */
//#define	DMA_CH7CTRL_CH7RI_INC	(0x1U << DMA_CH7CTRL_CH7RI_Pos)	/* RAM地址递增 */

//#define	DMA_CH7CTRL_CH7FI_Pos	8	/* Channel7FLASH地址增减设置 */
//#define	DMA_CH7CTRL_CH7FI_Msk	(0x1U << DMA_CH7CTRL_CH7FI_Pos)
//#define	DMA_CH7CTRL_CH7FI_DEC	(0x0U << DMA_CH7CTRL_CH7FI_Pos)	/* FLASH地址递减 */
//#define	DMA_CH7CTRL_CH7FI_INC	(0x1U << DMA_CH7CTRL_CH7FI_Pos)	/* FLASH地址递增 */



#define	DMA_CH7FLSAD_CH7FLSAD_Pos	0	/* Channel7FLASH指针地址 */
#define	DMA_CH7FLSAD_CH7FLSAD_Msk	(0xffffU << DMA_CH7FLSAD_CH7FLSAD_Pos)

#define	DMA_CH7RAMAD_CH7RAMAD_Pos	0	/* Channel7RAM字指针地址 */
#define	DMA_CH7RAMAD_CH7RAMAD_Msk	(0x1fffU << DMA_CH7RAMAD_CH7RAMAD_Pos)


//Macro_End

/* Exported functions --------------------------------------------------------*/ 
extern void DMA_Deinit(void);

/* DMA全局使能 相关函数 */
extern void DMA_GLOBALCTRL_DMAEN_Setable(FunState NewState);
extern FunState DMA_GLOBALCTRL_DMAEN_Getable(void);


//Announce_End

/* Channelx传输长度 相关函数 */
extern void DMA_CHxCTRL_TSIZE_Set(DMA_CH_Type CHx, uint32_t SetValue);
extern uint32_t DMA_CHxCTRL_TSIZE_Get(DMA_CH_Type CHx);

/* Channelx优先级 相关函数 */
extern void DMA_CHxCTRL_PRI_Set(DMA_CH_Type CHx, uint32_t SetValue);
extern uint32_t DMA_CHxCTRL_PRI_Get(DMA_CH_Type CHx);

/* Channelx RAM地址增减设置 相关函数 */
extern void DMA_CHxCTRL_INC_Set(DMA_CH_Type CHx, uint32_t SetValue);
extern uint32_t DMA_CHxCTRL_INC_Get(DMA_CH_Type CHx);

/* Channelx外设通道选择 相关函数 */
extern void DMA_CHxCTRL_SSEL_Set(DMA_CH_Type CHx, uint32_t SetValue);
extern uint32_t DMA_CHxCTRL_SSEL_Get(DMA_CH_Type CHx);

/* Channelx传输完成中断使能 相关函数 */
extern void DMA_CHxCTRL_FTIE_Setable(DMA_CH_Type CHx, FunState NewState);
extern FunState DMA_CHxCTRL_FTIE_Getable(DMA_CH_Type CHx);

/* Channelx半程传输完成中断使能 相关函数 */
extern void DMA_CHxCTRL_HTIE_Setable(DMA_CH_Type CHx, FunState NewState);
extern FunState DMA_CHxCTRL_HTIE_Getable(DMA_CH_Type CHx);

/* Channelx使能 相关函数 */
extern void DMA_CHxCTRL_EN_Setable(DMA_CH_Type CHx, FunState NewState);

/* Channelx指针寄存器 相关函数 */
extern void DMA_CHxRAMAD_Write(DMA_CH_Type CHx, uint32_t SetValue);
extern uint32_t DMA_CHxRAMAD_Read(DMA_CH_Type CHx);

/* Channel7FLASH指针地址 相关函数 */
extern void DMA_CH7FLSAD_Write(uint32_t SetValue);
extern uint32_t DMA_CH7FLSAD_Read(void);

/* Channel7RAM字指针地址 相关函数 */
extern void DMA_CH7RAMAD_Write(uint32_t SetValue);
extern uint32_t DMA_CH7RAMAD_Read(void);

/* Channelx传输半程标志 相关函数 */
extern void DMA_CHSTATUS_DMACHxHT_Clr(DMA_CH_Type CHx);
extern FlagStatus DMA_CHSTATUS_DMACHxHT_Chk(DMA_CH_Type CHx);

/* Channelx传输完成标志 相关函数 */
extern void DMA_CHSTATUS_DMACHxFT_Clr(DMA_CH_Type CHx);
extern FlagStatus DMA_CHSTATUS_DMACHxFT_Chk(DMA_CH_Type CHx);

/* DMA初始化函数 */
extern void DMA_Init(DMA_InitTypeDef* para);



#ifdef __cplusplus
}
#endif

#endif /* __FM33A0XX_DMA_H */



/************************ (C) COPYRIGHT FMSHelectronics *****END OF FILE****/




