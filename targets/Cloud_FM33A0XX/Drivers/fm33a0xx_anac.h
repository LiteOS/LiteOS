/**
  ******************************************************************************
  * @file    fm33a0xx_anac.h
  * @author  FM33a0xx Application Team
  * @version V2.0.1
  * @date    23-1-2018
  * @brief   This file contains all the functions prototypes for the ANAC firmware library.  
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FM33A0XX_ANAC_H
#define __FM33A0XX_ANAC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "FM33A0XX.h"
	 
/** @addtogroup FM33a0xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup ANAC
  * @{
  */ 

	 
/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct
{
	uint32_t	SVDMOD;		/*!<SVD工作模式选择  */
	uint32_t	SVDITVL;	/*!<SVD间歇使能间隔  */
	uint32_t	SVDLVL;		/*!<SVD报警阈值设置  */
	FunState	DFEN;		/*!<SVD数字滤波（SVDMODE=1时必须置1）  */
	FunState	PFIE;		/*!<SVD电源跌落中断  */
	FunState	PRIE;		/*!<SVD电源恢复中断  */
	FunState	SVDEN;		/*!<SVD使能  */
	
}ANAC_SVD_InitTypeDef;


typedef struct
{
	uint32_t	ADC_TRIM;	/*!<等效参考电压调校寄存器,转电压时配置为0x3FF,转温度时配置为0x640  */
	uint32_t	ADC_VANA_EN;/*!<内部通道、外部通道选择  */
	uint32_t	BUFSEL;		/*!<ADC输入通道选择  */
	FunState	BUFEN;		/*!<常开即可  */
	FunState	BUFBYP;		/*!<ADC输入Buffer Bypass，使用ADC测量外部信号输入时，关闭bypass， 使用ADC测量内部信号时，必须将此位置1（bypass） */
	FunState	ADC_IE;		/*!<ADC中断使能  */
	FunState	ADC_EN;		/*!<ADC使能  */
	
}ANAC_ADC_InitTypeDef;


typedef struct
{
	uint32_t	COMPx;		/*!<比较器1/2选择，输入1或2  */
	uint32_t	VxPSEL;		/*!<比较器x正极输入选择 */
	uint32_t	VxNSEL;		/*!<比较器x负极输入选择 */
	uint32_t	CMPxSEL;	/*!<比较器x中断源选择 */
	FunState	CMPxIE;		/*!<较器x中断使能 */
	FunState	CMPxDF;		/*!<比较器x数字滤波使能 */
//	FunState	BUFBYP;		比较器1，2共用一个buffer，buffer控制在外部独立操作，不放入配置结构体中
//	FunState	BUFENB;
	FunState	CMPxEN;		/*!<比较器x使能 */
	
}ANAC_COMPx_InitTypeDef;

/* Exported macro ------------------------------------------------------------*/
//从0x811800开始会保存如下信息
#define const_temp_TestA 	(*((uint8_t *)(0x1FFFFC90)))	//测试时温度整数位
#define const_temp_TestB 	(*((uint8_t *)(0x1FFFFC91)))	//测试时温度小数位
#define const_adc_Test	 	(*((uint16_t *)(0x1FFFFC92)))	//测试时PTAT ADC值
#define const_adc_Slope 	(*((uint16_t *)(0x1FFFFC54)))	//ADC斜率，除1000后使用，单位约2.2mV/lsb
#define const_adc_Offset 	(*((int16_t *)(0x1FFFFC56)))	//ADC截距，除100后使用，单位mV
#define const_ptat_Offset3V	(*((uint8_t *)(0x1FFFFC94)))	//ptat 5v3v差值

#define const_adc_TrimV ((uint16_t)0x03FF)		//测电压时使用
#define const_adc_TrimT ((uint16_t)0x0640)		//测温度时使用
#define const_TmpsL		5.0379					//温度ADC斜率低温段
#define const_TmpsH		5.0379					//温度ADC斜率高温段
#define const_T_Offset5V	(-3.0)				//芯片发热修正@8M
#define const_T_Offset3V	(-2.0)				//芯片发热修正
#define const_xtl_top	25.0					//晶体顶点温度

#define CH_PTAT	0//温度传感器
#define CH_VDD	1//电源电压
#define CH_IN1	2//PC12
#define CH_IN2	3//PC13(电池)
#define CH_IN3	4//PD0
#define CH_IN4	5//PD1
#define CH_IN5	6//PF6
#define CH_IN6	7//PC15
#define CH_IN7	8//PB2
#define CH_IN8	9//PB3

/* Exported functions --------------------------------------------------------*/ 

//Macro_START
//2018-04-02-13-20-01
//FM33A0XX_Driver_Gen_V1.4

#define	ANAC_PDRCON_PDRCFG_Pos	1	/* PDR下电复位电压配置 */
#define	ANAC_PDRCON_PDRCFG_Msk	(0x3U << ANAC_PDRCON_PDRCFG_Pos)
#define	ANAC_PDRCON_PDRCFG_1P5V	(0x0U << ANAC_PDRCON_PDRCFG_Pos)	/* 00—1.5V */
#define	ANAC_PDRCON_PDRCFG_1P25V	(0x1U << ANAC_PDRCON_PDRCFG_Pos)	/* 01—1.25V（默认） */
#define	ANAC_PDRCON_PDRCFG_1P35V	(0x2U << ANAC_PDRCON_PDRCFG_Pos)	/* 10—1.35V */
#define	ANAC_PDRCON_PDRCFG_1P4V	(0x3U << ANAC_PDRCON_PDRCFG_Pos)	/* 11—1.4V */

#define	ANAC_PDRCON_PDREN_Pos	0	/* PDR下电复位使能 */
#define	ANAC_PDRCON_PDREN_Msk	(0x1U << ANAC_PDRCON_PDREN_Pos)
	/* 0：关闭下电复位 */
	/* 1：使能下电复位 */

#define	ANAC_BORCON_BOR_PDRCFG_Pos	1	/* BOR下电复位电压配置 */
#define	ANAC_BORCON_BOR_PDRCFG_Msk	(0x3U << ANAC_BORCON_BOR_PDRCFG_Pos)
#define	ANAC_BORCON_BOR_PDRCFG_1P7V	(0x0U << ANAC_BORCON_BOR_PDRCFG_Pos)	/* 00—1.7V */
#define	ANAC_BORCON_BOR_PDRCFG_1P6V	(0x1U << ANAC_BORCON_BOR_PDRCFG_Pos)	/* 01—1.6V（默认） */
#define	ANAC_BORCON_BOR_PDRCFG_1P65V	(0x2U << ANAC_BORCON_BOR_PDRCFG_Pos)	/* 10—1.65V */
#define	ANAC_BORCON_BOR_PDRCFG_1P75V	(0x3U << ANAC_BORCON_BOR_PDRCFG_Pos)	/* 11—1.75V */

#define	ANAC_BORCON_OFF_BOR_Pos	0	/* BOR关闭控制寄存器 */
#define	ANAC_BORCON_OFF_BOR_Msk	(0x1U << ANAC_BORCON_OFF_BOR_Pos)
	/* 0：关闭BOR关闭（打开BOR） */
	/* 1：使能BOR关闭 */

#define	ANAC_SVDCFG_PFIE_Pos	9	/* SVD电源跌落中断使能 */
#define	ANAC_SVDCFG_PFIE_Msk	(0x1U << ANAC_SVDCFG_PFIE_Pos)
	/* 0：禁止电源跌落中断 */
	/* 1：使能电源跌落中断 */

#define	ANAC_SVDCFG_PRIE_Pos	8	/* SVD电源恢复中断使能 */
#define	ANAC_SVDCFG_PRIE_Msk	(0x1U << ANAC_SVDCFG_PRIE_Pos)
	/* 0：禁止电源恢复中断 */
	/* 1：使能电源恢复中断 */

#define	ANAC_SVDCFG_SVDLVL_Pos	4	/* SVD报警阈值设置 */
#define	ANAC_SVDCFG_SVDLVL_Msk	(0xfU << ANAC_SVDCFG_SVDLVL_Pos)
#define	ANAC_SVDCFG_SVDLVL_1P800V	(0x0U << ANAC_SVDCFG_SVDLVL_Pos)	/* 0000_1.800V_1.900V（SVDLVL_下降阈值_上升阈值） */
#define	ANAC_SVDCFG_SVDLVL_2P014V	(0x1U << ANAC_SVDCFG_SVDLVL_Pos)	/* 0001_2.014V_2.114V */
#define	ANAC_SVDCFG_SVDLVL_2P229V	(0x2U << ANAC_SVDCFG_SVDLVL_Pos)	/* 0010_2.229V_2.329V */
#define	ANAC_SVDCFG_SVDLVL_2P443V	(0x3U << ANAC_SVDCFG_SVDLVL_Pos)	/* 0011_2.443V_2.543V */
#define	ANAC_SVDCFG_SVDLVL_2P657V	(0x4U << ANAC_SVDCFG_SVDLVL_Pos)	/* 0100_2.657V_2.757V */
#define	ANAC_SVDCFG_SVDLVL_2P871V	(0x5U << ANAC_SVDCFG_SVDLVL_Pos)	/* 0101_2.871V_2.971V */
#define	ANAC_SVDCFG_SVDLVL_3P086V	(0x6U << ANAC_SVDCFG_SVDLVL_Pos)	/* 0110_3.086V_3.186V */
#define	ANAC_SVDCFG_SVDLVL_3P300V	(0x7U << ANAC_SVDCFG_SVDLVL_Pos)	/* 0111_3.300V_3.400V */
#define	ANAC_SVDCFG_SVDLVL_3P514V	(0x8U << ANAC_SVDCFG_SVDLVL_Pos)	/* 1000_3.514V_3.614V */
#define	ANAC_SVDCFG_SVDLVL_3P729V	(0x9U << ANAC_SVDCFG_SVDLVL_Pos)	/* 1001_3.729V_3.829V */
#define	ANAC_SVDCFG_SVDLVL_3P943V	(0xaU << ANAC_SVDCFG_SVDLVL_Pos)	/* 1010_3.943V_4.043V */
#define	ANAC_SVDCFG_SVDLVL_4P157V	(0xbU << ANAC_SVDCFG_SVDLVL_Pos)	/* 1011_4.157V_4.257V */
#define	ANAC_SVDCFG_SVDLVL_4P371V	(0xcU << ANAC_SVDCFG_SVDLVL_Pos)	/* 1100_4.371V_4.471V */
#define	ANAC_SVDCFG_SVDLVL_4P586V	(0xdU << ANAC_SVDCFG_SVDLVL_Pos)	/* 1101_4.586V_4.686V */
#define	ANAC_SVDCFG_SVDLVL_4P800V	(0xeU << ANAC_SVDCFG_SVDLVL_Pos)	/* 1110_4.800V_4.900V */
#define	ANAC_SVDCFG_SVDLVL_SVS	(0xfU << ANAC_SVDCFG_SVDLVL_Pos)	/* 1111_SVS_SVS  固定检测0.8V  */

#define	ANAC_SVDCFG_DFEN_Pos	3	/* 数字滤波使能（SVDMODE=1时必须置1） */
#define	ANAC_SVDCFG_DFEN_Msk	(0x1U << ANAC_SVDCFG_DFEN_Pos)
	/* 0：关闭SVD输出的数字滤波 */
	/* 1：启动SVD输出的数字滤波 */

#define	ANAC_SVDCFG_SVDMOD_Pos	2	/* SVD工作模式选择 */
#define	ANAC_SVDCFG_SVDMOD_Msk	(0x1U << ANAC_SVDCFG_SVDMOD_Pos)
#define	ANAC_SVDCFG_SVDMOD_ALWAYSON	(0x0U << ANAC_SVDCFG_SVDMOD_Pos)	/* 0：常使能模式 */
#define	ANAC_SVDCFG_SVDMOD_INTERVAL	(0x1U << ANAC_SVDCFG_SVDMOD_Pos)	/* 1：间歇使能模式 */

#define	ANAC_SVDCFG_SVDITVL_Pos	0	/* SVD间歇使能间隔 */
#define	ANAC_SVDCFG_SVDITVL_Msk	(0x3U << ANAC_SVDCFG_SVDITVL_Pos)
#define	ANAC_SVDCFG_SVDITVL_15P625MS	(0x0U << ANAC_SVDCFG_SVDITVL_Pos)	/* 00：15.625ms */
#define	ANAC_SVDCFG_SVDITVL_62P5MS	(0x1U << ANAC_SVDCFG_SVDITVL_Pos)	/* 01：62.5ms */
#define	ANAC_SVDCFG_SVDITVL_256MS	(0x2U << ANAC_SVDCFG_SVDITVL_Pos)	/* 10：256ms */
#define	ANAC_SVDCFG_SVDITVL_1S	(0x3U << ANAC_SVDCFG_SVDITVL_Pos)	/* 11：1s */

#define	ANAC_SVDCON_SVDTE_Pos	8	/* SVD测试使能，保持为0 */
#define	ANAC_SVDCON_SVDTE_Msk	(0x1U << ANAC_SVDCON_SVDTE_Pos)

#define	ANAC_SVDCON_SVDEN_Pos	0	/* SVD使能 */
#define	ANAC_SVDCON_SVDEN_Msk	(0x1U << ANAC_SVDCON_SVDEN_Pos)
	/* 0：关闭SVD */
	/* 1：启动SVD */

#define	ANAC_SVDSIF_SVDO_Pos	8	/* SVD电源检测输出(模拟模块直接输出的信号,仅在SVD工作的时候可查询) */
#define	ANAC_SVDSIF_SVDO_Msk	(0x1U << ANAC_SVDSIF_SVDO_Pos)

#define	ANAC_SVDSIF_PFF_Pos	1	/* 电源跌落中断标志寄存器，电源电压跌落到SVD阈值之下时置位，软件写1清零 */
#define	ANAC_SVDSIF_PFF_Msk	(0x1U << ANAC_SVDSIF_PFF_Pos)

#define	ANAC_SVDSIF_PRF_Pos	0	/* 电源恢复中断标志寄存器，电源电压上升到SVD阈值之上时置位，软件写1清零 */
#define	ANAC_SVDSIF_PRF_Msk	(0x1U << ANAC_SVDSIF_PRF_Pos)

#define	ANAC_SVDALM_SVDALM_B_Pos	0	/* SVD内部滤波后的电压检测标志，仅在使能数字滤波时有意义；软件避免写此寄存器。 */
#define	ANAC_SVDALM_SVDALM_B_Msk	(0x1U << ANAC_SVDALM_SVDALM_B_Pos)

#define	ANAC_FDETIE_FDET_IE_Pos	0	/* XTLF停振检测报警中断使能,上电默认关闭，避免刚上电时晶振未起振触发中断
 */
#define	ANAC_FDETIE_FDET_IE_Msk	(0x1U << ANAC_FDETIE_FDET_IE_Pos)

#define	ANAC_FDETIF_FDETO_Pos	6	/* 停振检测模块输出 */
#define	ANAC_FDETIF_FDETO_Msk	(0x1U << ANAC_FDETIF_FDETO_Pos)

#define	ANAC_FDETIF_FDETIF_Pos	0	/* 停振检测中断标志寄存器，XTLF停振时硬件异步置位，软件写1清零；只有在FDETO不为0的情况下才能够清除此寄存器 */
#define	ANAC_FDETIF_FDETIF_Msk	(0x1U << ANAC_FDETIF_FDETIF_Pos)

#define	ANAC_ADCCON_ADC_IE_Pos	7	/* ADC中断使能 */
#define	ANAC_ADCCON_ADC_IE_Msk	(0x1U << ANAC_ADCCON_ADC_IE_Pos)

#define	ANAC_ADCCON_ADC_VANA_EN_Pos	1	/* ADC通道选择 */
#define	ANAC_ADCCON_ADC_VANA_EN_Msk	(0x1U << ANAC_ADCCON_ADC_VANA_EN_Pos)
#define	ANAC_ADCCON_ADC_VANA_EN_PTAT	(0x0U << ANAC_ADCCON_ADC_VANA_EN_Pos)	/* 0：ADC用作温度传感器 */
#define	ANAC_ADCCON_ADC_VANA_EN_VOLTAGE	(0x1U << ANAC_ADCCON_ADC_VANA_EN_Pos)	/* 1：ADC用于测量外部电压 */

#define	ANAC_ADCCON_ADC_EN_Pos	0	/* ADC使能信号 */
#define	ANAC_ADCCON_ADC_EN_Msk	(0x1U << ANAC_ADCCON_ADC_EN_Pos)
	/* 0：ADC不使能 */
	/* 1：ADC使能 */

#define	ANAC_ADCTRIM_ADC_TRIM_Pos	0	/* ADC TRIM值,从出厂参数区获取 */
#define	ANAC_ADCTRIM_ADC_TRIM_Msk	(0x7ffU << ANAC_ADCTRIM_ADC_TRIM_Pos)

#define	ANAC_ADCDATA_ADC_DATA_Pos	0	/* ADC输出数据寄存器 */
#define	ANAC_ADCDATA_ADC_DATA_Msk	(0x7ffU << ANAC_ADCDATA_ADC_DATA_Pos)

#define	ANAC_ADCIF_ADC_IF_Pos	0	/* ADC转换完成中断标志，硬件置位，软件写1清零，写0无效 */
#define	ANAC_ADCIF_ADC_IF_Msk	(0x1U << ANAC_ADCIF_ADC_IF_Pos)

#define	ANAC_ADCINSEL_BUFEN_Pos	5	/* ADC输入通道Buffer使能 */
#define	ANAC_ADCINSEL_BUFEN_Msk	(0x1U << ANAC_ADCINSEL_BUFEN_Pos)

#define	ANAC_ADCINSEL_BUFBYP_Pos	4	/* ADC输入Buffer Bypass */
#define	ANAC_ADCINSEL_BUFBYP_Msk	(0x1U << ANAC_ADCINSEL_BUFBYP_Pos)
	/* 使用ADC测量外部信号输入时，不要Bypass Buffer */
	/* 使用ADC测量电源电压时，必须将此位置1 */

#define	ANAC_ADCINSEL_BUFSEL_Pos	0	/* ADC输入通道选择 */
#define	ANAC_ADCINSEL_BUFSEL_Msk	(0xfU << ANAC_ADCINSEL_BUFSEL_Pos)
#define	ANAC_ADCINSEL_BUFSEL_VDD	(0x6U << ANAC_ADCINSEL_BUFSEL_Pos)	/* VDD主电源 */
#define	ANAC_ADCINSEL_BUFSEL_ADC_IN1	(0x8U << ANAC_ADCINSEL_BUFSEL_Pos)	/* 1000：ADC_IN1 (PC12) */
#define	ANAC_ADCINSEL_BUFSEL_ADC_IN2	(0x9U << ANAC_ADCINSEL_BUFSEL_Pos)	/* 1001：ADC_IN2 (PC13) */
#define	ANAC_ADCINSEL_BUFSEL_ADC_IN3	(0xaU << ANAC_ADCINSEL_BUFSEL_Pos)	/* 1010：ADC_IN3 (PD0) */
#define	ANAC_ADCINSEL_BUFSEL_ADC_IN4	(0xbU << ANAC_ADCINSEL_BUFSEL_Pos)	/* 1011：ADC_IN4 (PD1) */
#define	ANAC_ADCINSEL_BUFSEL_ADC_IN5	(0xcU << ANAC_ADCINSEL_BUFSEL_Pos)	/* 1100：ADC_IN5 (PF6) */
#define	ANAC_ADCINSEL_BUFSEL_ADC_IN6	(0xdU << ANAC_ADCINSEL_BUFSEL_Pos)	/* 1101：ADC_IN6 (PC15) */
#define	ANAC_ADCINSEL_BUFSEL_ADC_IN7	(0xeU << ANAC_ADCINSEL_BUFSEL_Pos)	/* 1110：ADC_IN7 (PB2) */
#define	ANAC_ADCINSEL_BUFSEL_ADC_IN8	(0xfU << ANAC_ADCINSEL_BUFSEL_Pos)	/* 1111：ADC_IN8 (PB3) */

#define	ANAC_TRNGCON_TRNGEN_Pos	0	/* TRNG控制 */
#define	ANAC_TRNGCON_TRNGEN_Msk	(0x1U << ANAC_TRNGCON_TRNGEN_Pos)
	/* 0：关闭TRNG */
	/* 1：启动TRNG */

#define	ANAC_COMP1CR_CMP1O_Pos	8	/* 比较器1输出，软件只读 */
#define	ANAC_COMP1CR_CMP1O_Msk	(0x1U << ANAC_COMP1CR_CMP1O_Pos)

#define	ANAC_COMP1CR_V1PSEL_Pos	3	/* 比较器1正极输入选择 */
#define	ANAC_COMP1CR_V1PSEL_Msk	(0x3U << ANAC_COMP1CR_V1PSEL_Pos)
#define	ANAC_COMP1CR_V1PSEL_PF6	(0x0U << ANAC_COMP1CR_V1PSEL_Pos)	/* 00：PF6 */
#define	ANAC_COMP1CR_V1PSEL_PF1	(0x1U << ANAC_COMP1CR_V1PSEL_Pos)	/* 01：PF1 */
#define	ANAC_COMP1CR_V1PSEL_PG2	(0x2U << ANAC_COMP1CR_V1PSEL_Pos)	/* 10：PG2 */
#define	ANAC_COMP1CR_V1PSEL_PG3	(0x3U << ANAC_COMP1CR_V1PSEL_Pos)	/* 11：PG3 */

#define	ANAC_COMP1CR_V1NSEL_Pos	1	/* 比较器1负极输入选择 */
#define	ANAC_COMP1CR_V1NSEL_Msk	(0x3U << ANAC_COMP1CR_V1NSEL_Pos)
#define	ANAC_COMP1CR_V1NSEL_PF5	(0x0U << ANAC_COMP1CR_V1NSEL_Pos)	/* 00：PF5 */
#define	ANAC_COMP1CR_V1NSEL_PF1	(0x1U << ANAC_COMP1CR_V1NSEL_Pos)	/* 01：PF1 */
#define	ANAC_COMP1CR_V1NSEL_VREF0P8V	(0x2U << ANAC_COMP1CR_V1NSEL_Pos)	/* 10：Vref 0.8V */
#define	ANAC_COMP1CR_V1NSEL_VREF0P4V	(0x3U << ANAC_COMP1CR_V1NSEL_Pos)	/* 11：Vref/2 0.4V */

#define	ANAC_COMP1CR_CMP1EN_Pos	0	/* 比较器1使能位 */
#define	ANAC_COMP1CR_CMP1EN_Msk	(0x1U << ANAC_COMP1CR_CMP1EN_Pos)
	/* 0：关闭比较器1 */
	/* 1：使能比较器1 */

#define	ANAC_COMP2CR_CMP2O_Pos	8	/* 比较器2输出，软件只读 */
#define	ANAC_COMP2CR_CMP2O_Msk	(0x1U << ANAC_COMP2CR_CMP2O_Pos)

#define	ANAC_COMP2CR_V2PSEL_Pos	3	/* 比较器2正极输入选择 */
#define	ANAC_COMP2CR_V2PSEL_Msk	(0x1U << ANAC_COMP2CR_V2PSEL_Pos)
#define	ANAC_COMP2CR_V2PSEL_PC15	(0x0U << ANAC_COMP2CR_V2PSEL_Pos)	/* 0：PC15 */
#define	ANAC_COMP2CR_V2PSEL_PE4	(0x1U << ANAC_COMP2CR_V2PSEL_Pos)	/* 1：PE4 */

#define	ANAC_COMP2CR_V2NSEL_Pos	1	/* 比较器2负极输入选择 */
#define	ANAC_COMP2CR_V2NSEL_Msk	(0x3U << ANAC_COMP2CR_V2NSEL_Pos)
#define	ANAC_COMP2CR_V2NSEL_PC14	(0x0U << ANAC_COMP2CR_V2NSEL_Pos)	/* 00：PC14 */
#define	ANAC_COMP2CR_V2NSEL_PE3	(0x1U << ANAC_COMP2CR_V2NSEL_Pos)	/* 01：PE3 */
#define	ANAC_COMP2CR_V2NSEL_VREF0P8V	(0x2U << ANAC_COMP2CR_V2NSEL_Pos)	/* 10：Vref 0.8V */
#define	ANAC_COMP2CR_V2NSEL_VREF0P4V	(0x3U << ANAC_COMP2CR_V2NSEL_Pos)	/* 11：Vref/2 0.4V */

#define	ANAC_COMP2CR_CMP2EN_Pos	0	/* 比较器2使能位 */
#define	ANAC_COMP2CR_CMP2EN_Msk	(0x1U << ANAC_COMP2CR_CMP2EN_Pos)
	/* 0：关闭比较器1 */
	/* 1：使能比较器1 */

#define	ANAC_COMPICR_CMP2DF_Pos	9	/* 比较器2数字滤波使能 */
#define	ANAC_COMPICR_CMP2DF_Msk	(0x1U << ANAC_COMPICR_CMP2DF_Pos)
	/* 0：禁止数字滤波 */
	/* 1：使能数字滤波 */

#define	ANAC_COMPICR_CMP1DF_Pos	8	/* 比较器1数字滤波使能 */
#define	ANAC_COMPICR_CMP1DF_Msk	(0x1U << ANAC_COMPICR_CMP1DF_Pos)
	/* 0：禁止数字滤波 */
	/* 1：使能数字滤波 */

#define	ANAC_COMPICR_BUFBYP_Pos	7	/* 比较器Buffer Bypass */
#define	ANAC_COMPICR_BUFBYP_Msk	(0x1U << ANAC_COMPICR_BUFBYP_Pos)
	/* 0：不Bypass比较器Buffer */
	/* 1：Bypass比较器Buffer */

#define	ANAC_COMPICR_BUFENB_Pos	6	/* 比较器Buffer使能 */
#define	ANAC_COMPICR_BUFENB_Msk	(0x1U << ANAC_COMPICR_BUFENB_Pos)
	/* 0：使能比较器Buffer */
	/* 1：禁止比较器Buffer */

#define	ANAC_COMPICR_CMP2SEL_Pos	4	/* 比较器2中断源选择 */
#define	ANAC_COMPICR_CMP2SEL_Msk	(0x3U << ANAC_COMPICR_CMP2SEL_Pos)
#define	ANAC_COMPICR_CMP2SEL_OUTBOTH	(0x0U << ANAC_COMPICR_CMP2SEL_Pos)	/* 00/11：比较器2输出上升或下降沿产生中断 */
#define	ANAC_COMPICR_CMP2SEL_OUTRISE	(0x1U << ANAC_COMPICR_CMP2SEL_Pos)	/* 01：比较器2输出上升沿产生中断 */
#define	ANAC_COMPICR_CMP2SEL_OUTFALL	(0x2U << ANAC_COMPICR_CMP2SEL_Pos)	/* 10：比较器2输出下降沿产生中断 */

#define	ANAC_COMPICR_CMP1SEL_Pos	2	/* 比较器1中断源选择 */
#define	ANAC_COMPICR_CMP1SEL_Msk	(0x3U << ANAC_COMPICR_CMP1SEL_Pos)
#define	ANAC_COMPICR_CMP1SEL_OUTBOTH	(0x0U << ANAC_COMPICR_CMP1SEL_Pos)	/* 00/11：比较器1输出上升或下降沿产生中断 */
#define	ANAC_COMPICR_CMP1SEL_OUTRISE	(0x1U << ANAC_COMPICR_CMP1SEL_Pos)	/* 01：比较器1输出上升沿产生中断 */
#define	ANAC_COMPICR_CMP1SEL_OUTFALL	(0x2U << ANAC_COMPICR_CMP1SEL_Pos)	/* 10：比较器1输出下降沿产生中断 */

#define	ANAC_COMPICR_CMP2IE_Pos	1	/* 比较器2中断使能 */
#define	ANAC_COMPICR_CMP2IE_Msk	(0x1U << ANAC_COMPICR_CMP2IE_Pos)

#define	ANAC_COMPICR_CMP1IE_Pos	0	/* 比较器1中断使能 */
#define	ANAC_COMPICR_CMP1IE_Msk	(0x1U << ANAC_COMPICR_CMP1IE_Pos)

#define	ANAC_COMPIF_CMP2IF_Pos	1	/* 比较器2中断标志，硬件置位，软件写1清零 */
#define	ANAC_COMPIF_CMP2IF_Msk	(0x1U << ANAC_COMPIF_CMP2IF_Pos)

#define	ANAC_COMPIF_CMP1IF_Pos	0	/* 比较器1中断标志，硬件置位，软件写1清零 */
#define	ANAC_COMPIF_CMP1IF_Msk	(0x1U << ANAC_COMPIF_CMP1IF_Pos)
//Macro_End

/* Exported functions --------------------------------------------------------*/ 
extern void ANAC_Deinit(void);

/* PDR下电复位电压配置 相关函数 */
extern void ANAC_PDRCON_PDRCFG_Set(uint32_t SetValue);
extern uint32_t ANAC_PDRCON_PDRCFG_Get(void);

/* PDR下电复位使能 相关函数 */
extern void ANAC_PDRCON_PDREN_Setable(FunState NewState);
extern FunState ANAC_PDRCON_PDREN_Getable(void);

/* BOR下电复位电压配置 相关函数 */
extern void ANAC_BORCON_BOR_PDRCFG_Set(uint32_t SetValue);
extern uint32_t ANAC_BORCON_BOR_PDRCFG_Get(void);

/* BOR关闭控制寄存器 相关函数 */
extern void ANAC_BORCON_OFF_BOR_Setable(FunState NewState);
extern FunState ANAC_BORCON_OFF_BOR_Getable(void);

/* SVD电源跌落中断使能 相关函数 */
extern void ANAC_SVDCFG_PFIE_Setable(FunState NewState);
extern FunState ANAC_SVDCFG_PFIE_Getable(void);

/* SVD电源恢复中断使能 相关函数 */
extern void ANAC_SVDCFG_PRIE_Setable(FunState NewState);
extern FunState ANAC_SVDCFG_PRIE_Getable(void);

/* SVD报警阈值设置 相关函数 */
extern void ANAC_SVDCFG_SVDLVL_Set(uint32_t SetValue);
extern uint32_t ANAC_SVDCFG_SVDLVL_Get(void);

/* 数字滤波使能（SVDMODE=1时必须置1） 相关函数 */
extern void ANAC_SVDCFG_DFEN_Setable(FunState NewState);
extern FunState ANAC_SVDCFG_DFEN_Getable(void);

/* SVD工作模式选择 相关函数 */
extern void ANAC_SVDCFG_SVDMOD_Set(uint32_t SetValue);
extern uint32_t ANAC_SVDCFG_SVDMOD_Get(void);

/* SVD间歇使能间隔 相关函数 */
extern void ANAC_SVDCFG_SVDITVL_Set(uint32_t SetValue);
extern uint32_t ANAC_SVDCFG_SVDITVL_Get(void);

/* SVD测试使能，保持为0 相关函数 */
extern void ANAC_SVDCON_SVDTE_Setable(FunState NewState);
extern FunState ANAC_SVDCON_SVDTE_Getable(void);

/* SVD使能 相关函数 */
extern void ANAC_SVDCON_SVDEN_Setable(FunState NewState);
extern FunState ANAC_SVDCON_SVDEN_Getable(void);

/* SVD电源检测输出(模拟模块直接输出的信号,仅在SVD工作的时候可查询) 相关函数 */
extern FlagStatus ANAC_SVDSIF_SVDO_Chk(void);

/* 电源跌落中断标志寄存器，电源电压跌落到SVD阈值之下时置位，软件写1清零 相关函数 */
extern void ANAC_SVDSIF_PFF_Clr(void);
extern FlagStatus ANAC_SVDSIF_PFF_Chk(void);

/* 电源恢复中断标志寄存器，电源电压上升到SVD阈值之上时置位，软件写1清零 相关函数 */
extern void ANAC_SVDSIF_PRF_Clr(void);
extern FlagStatus ANAC_SVDSIF_PRF_Chk(void);

/* SVD内部滤波后的电压检测标志，仅在使能数字滤波时有意义；软件避免写此寄存器。 相关函数 */
extern FlagStatus ANAC_SVDALM_SVDALM_B_Chk(void);

/* XTLF停振检测报警中断使能,上电默认关闭，避免刚上电时晶振未起振触发中断
 相关函数 */
extern void ANAC_FDETIE_FDET_IE_Setable(FunState NewState);
extern FunState ANAC_FDETIE_FDET_IE_Getable(void);

/* 停振检测模块输出 相关函数 */
extern FlagStatus ANAC_FDETIF_FDETO_Chk(void);

/* 停振检测中断标志寄存器，XTLF停振时硬件异步置位，软件写1清零；只有在FDETO不为0的情况下才能够清除此寄存器 相关函数 */
extern void ANAC_FDETIF_FDETIF_Clr(void);
extern FlagStatus ANAC_FDETIF_FDETIF_Chk(void);

/* ADC中断使能 相关函数 */
extern void ANAC_ADCCON_ADC_IE_Setable(FunState NewState);
extern FunState ANAC_ADCCON_ADC_IE_Getable(void);

/* ADC通道选择 相关函数 */
extern void ANAC_ADCCON_ADC_VANA_EN_Set(uint32_t SetValue);
extern uint32_t ANAC_ADCCON_ADC_VANA_EN_Get(void);

/* ADC使能信号 相关函数 */
extern void ANAC_ADCCON_ADC_EN_Setable(FunState NewState);
extern FunState ANAC_ADCCON_ADC_EN_Getable(void);

/* ADC TRIM值,从出厂参数区获取 相关函数 */
extern void ANAC_ADCTRIM_Write(uint32_t SetValue);
extern uint32_t ANAC_ADCTRIM_Read(void);

/* ADC输出数据寄存器 相关函数 */
extern uint32_t ANAC_ADCDATA_Read(void);

/* ADC转换完成中断标志，硬件置位，软件写1清零，写0无效 相关函数 */
extern void ANAC_ADCIF_ADC_IF_Clr(void);
extern FlagStatus ANAC_ADCIF_ADC_IF_Chk(void);

/* ADC输入通道Buffer使能 相关函数 */
extern void ANAC_ADCINSEL_BUFEN_Setable(FunState NewState);
extern FunState ANAC_ADCINSEL_BUFEN_Getable(void);

/* ADC输入Buffer Bypass 相关函数 */
extern void ANAC_ADCINSEL_BUFBYP_Setable(FunState NewState);
extern FunState ANAC_ADCINSEL_BUFBYP_Getable(void);

/* ADC输入通道选择 相关函数 */
extern void ANAC_ADCINSEL_BUFSEL_Set(uint32_t SetValue);
extern uint32_t ANAC_ADCINSEL_BUFSEL_Get(void);

/* TRNG控制 相关函数 */
extern void ANAC_TRNGCON_TRNGEN_Setable(FunState NewState);
extern FunState ANAC_TRNGCON_TRNGEN_Getable(void);

/* 比较器1输出，软件只读 相关函数 */
extern FlagStatus ANAC_COMP1CR_CMP1O_Chk(void);

/* 比较器1正极输入选择 相关函数 */
extern void ANAC_COMP1CR_V1PSEL_Set(uint32_t SetValue);
extern uint32_t ANAC_COMP1CR_V1PSEL_Get(void);

/* 比较器1负极输入选择 相关函数 */
extern void ANAC_COMP1CR_V1NSEL_Set(uint32_t SetValue);
extern uint32_t ANAC_COMP1CR_V1NSEL_Get(void);

/* 比较器1使能位 相关函数 */
extern void ANAC_COMP1CR_CMP1EN_Setable(FunState NewState);
extern FunState ANAC_COMP1CR_CMP1EN_Getable(void);

/* 比较器2输出，软件只读 相关函数 */
extern FlagStatus ANAC_COMP2CR_CMP2O_Chk(void);

/* 比较器2正极输入选择 相关函数 */
extern void ANAC_COMP2CR_V2PSEL_Set(uint32_t SetValue);
extern uint32_t ANAC_COMP2CR_V2PSEL_Get(void);

/* 比较器2负极输入选择 相关函数 */
extern void ANAC_COMP2CR_V2NSEL_Set(uint32_t SetValue);
extern uint32_t ANAC_COMP2CR_V2NSEL_Get(void);

/* 比较器2使能位 相关函数 */
extern void ANAC_COMP2CR_CMP2EN_Setable(FunState NewState);
extern FunState ANAC_COMP2CR_CMP2EN_Getable(void);

/* 比较器2数字滤波使能 相关函数 */
extern void ANAC_COMPICR_CMP2DF_Setable(FunState NewState);
extern FunState ANAC_COMPICR_CMP2DF_Getable(void);

/* 比较器1数字滤波使能 相关函数 */
extern void ANAC_COMPICR_CMP1DF_Setable(FunState NewState);
extern FunState ANAC_COMPICR_CMP1DF_Getable(void);

/* 比较器Buffer Bypass 相关函数 */
extern void ANAC_COMPICR_BUFBYP_Setable(FunState NewState);
extern FunState ANAC_COMPICR_BUFBYP_Getable(void);

/* 比较器Buffer使能 相关函数 */
extern void ANAC_COMPICR_BUFENB_Setable(FunState NewState);
extern FunState ANAC_COMPICR_BUFENB_Getable(void);

/* 比较器2中断源选择 相关函数 */
extern void ANAC_COMPICR_CMP2SEL_Set(uint32_t SetValue);
extern uint32_t ANAC_COMPICR_CMP2SEL_Get(void);

/* 比较器1中断源选择 相关函数 */
extern void ANAC_COMPICR_CMP1SEL_Set(uint32_t SetValue);
extern uint32_t ANAC_COMPICR_CMP1SEL_Get(void);

/* 比较器2中断使能 相关函数 */
extern void ANAC_COMPICR_CMP2IE_Setable(FunState NewState);
extern FunState ANAC_COMPICR_CMP2IE_Getable(void);

/* 比较器1中断使能 相关函数 */
extern void ANAC_COMPICR_CMP1IE_Setable(FunState NewState);
extern FunState ANAC_COMPICR_CMP1IE_Getable(void);

/* 比较器2中断标志，硬件置位，软件写1清零 相关函数 */
extern void ANAC_COMPIF_CMP2IF_Clr(void);
extern FlagStatus ANAC_COMPIF_CMP2IF_Chk(void);

/* 比较器1中断标志，硬件置位，软件写1清零 相关函数 */
extern void ANAC_COMPIF_CMP1IF_Clr(void);
extern FlagStatus ANAC_COMPIF_CMP1IF_Chk(void);
//Announce_End
	 

/*SVD 初始化配置函数*/
extern void ANAC_SVD_Init(ANAC_SVD_InitTypeDef* para);

/*ADC 初始化配置函数*/
void ANAC_ADC_Init(ANAC_ADC_InitTypeDef* para);

/*ADC 测量通道配置函数，简化型*/
void ANAC_ADC_Channel_SetEx(uint8_t ChSel_def);

/*ADC 电压计算函数
	输入：AD值	 float型（方便处理平均值）
	输出：电压@mV float型
*/
float ANAC_ADC_VoltageCalc(float fADCData);

/*ADC 温度计算函数
	输入：AD值	 float型（方便处理平均值）
	输出：温度@℃ float型
*/
float ANAC_ADC_TemperatureCalc(float fADCData ,uint8_t Vdd);

/*COMP 初始化配置函数*/
void ANAC_COMPx_Init(ANAC_COMPx_InitTypeDef* para);

#ifdef __cplusplus
}
#endif

#endif /* __FM33A0XX_ANAC_H */



/************************ (C) COPYRIGHT FMSHelectronics *****END OF FILE****/



