/**
  ******************************************************************************
  * @file    fm33a0xx_rcc.h
  * @author  FM33a0xx Application Team
  * @version V2.0.1
  * @date    24-12-2017
  * @brief   This file contains all the functions prototypes for the RCC firmware library.  
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FM33A0XX_RCC_H
#define __FM33A0XX_RCC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "FM33A0XX.h"
	 
/* Defines------------------------------------------------------------------*/
#define HXVAR(object, addr)   (*((object *) (addr)))
#define const_rchf_Trim8 	HXVAR( uint32_t, 0x1FFFFD40 )	//RC8M 常温校准值
#define const_rchf_Trim16 	HXVAR( uint32_t, 0x1FFFFD3C )	//RC16M 常温校准值
#define const_rchf_Trim24 	HXVAR( uint32_t, 0x1FFFFD38 )	//RC24M 常温校准值
#define const_rchf_Trim32 	HXVAR( uint32_t, 0x1FFFFD34 )	//RC32M 常温校准值
     
/* Exported constants --------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/
typedef struct
{
	uint32_t			FSEL;		/*!<RCHF频率  */
	FunState			RCHFEN;			/*!<RCHF使能控制  */
}RCC_RCHF_InitTypeDef;

typedef struct
{            
    uint32_t			PLLDB;		/*!<PLL倍频比最大1023,pll倍频数 = PLLDB + 1  */
	uint32_t			PLLOSEL;	/*!<PLL输出选择，当需要输出超过1024倍时开启两倍输出模式  */
	uint32_t			PLLINSEL;	/*!<PLL输入源选择  */
	FunState			PLLEN;		/*!<PLL使能控制  */
}RCC_PLL_InitTypeDef;

typedef struct
{
    uint32_t			SYSCLKSEL;		/*!<系统时钟选择，从休眠状态唤醒后会自动恢复为RCHF8M  */
	uint32_t			AHBPRES;		/*!<AHB时钟分频选择  */
	uint32_t			APBPRES;		/*!<APB时钟分频选择  */
	uint32_t			EXTICKSEL;		/*!<EXTI中断采样,IO数字滤波时钟选择  */
	FunState			SLP_ENEXTI;		/*!<Sleep/DeepSleep模式下EXTI采样设置  */
	FunState			LPM_RCLP_OFF;	/*!<Sleep/DeepSleep模式下禁止RCLP，仅在无晶体配置下有效  */	
}RCC_SYSCLK_InitTypeDef;

typedef struct
{
  uint32_t SYSCLK_Frequency;		/*!<  SYSCLK clock frequency expressed in Hz */
  uint32_t AHBCLK_Frequency;		/*!<  AHB clock frequency expressed in Hz  */
  uint32_t APBCLK_Frequency;		/*!<  APB clock frequency expressed in Hz  */
  uint32_t RCHF_Frequency;			/*!<  RCHF clock frequency expressed in Hz   */
  uint32_t PLL_Frequency;			/*!<  PLL clock frequency expressed in Hz   */
}RCC_ClocksType;


//Macro_START
//2018-04-02-13-22-07
//FM33A0XX_Driver_Gen_V1.4

#define	RCC_SYSCLKSEL_LPM_RCLP_OFF_Pos	31	/* Sleep/DeepSleep模式下禁止RCLP，仅在无晶体配置下有效 */
#define	RCC_SYSCLKSEL_LPM_RCLP_OFF_Msk	(0x1U << RCC_SYSCLKSEL_LPM_RCLP_OFF_Pos)
	/* 0：休眠模式下保持RCLP开启 */
	/* 1：休眠模式下关闭RCLP */

#define	RCC_SYSCLKSEL_SLP_ENEXTI_Pos	25	/* Sleep/DeepSleep模式下EXTI采样设置 */
#define	RCC_SYSCLKSEL_SLP_ENEXTI_Msk	(0x1U << RCC_SYSCLKSEL_SLP_ENEXTI_Pos)
	/* 0：Sleep/DeepSleep模式下禁止外部引脚中断采样（将无法产生EXTI中断） */
	/* 1：Sleep/DeepSleep模式下使能外部引脚中断采样（采样时钟为LSCLK） */

#define	RCC_SYSCLKSEL_EXTICKSEL_Pos	24	/* EXTI中断与数字滤波采样时钟选择 */
#define	RCC_SYSCLKSEL_EXTICKSEL_Msk	(0x1U << RCC_SYSCLKSEL_EXTICKSEL_Pos)
#define	RCC_SYSCLKSEL_EXTICKSEL_AHBCLK	(0x0U << RCC_SYSCLKSEL_EXTICKSEL_Pos)	/* 0：外部引脚中断使用AHBCLK采样 */
#define	RCC_SYSCLKSEL_EXTICKSEL_LSCLK	(0x1U << RCC_SYSCLKSEL_EXTICKSEL_Pos)	/* 1：外部引脚中断使用LSCLK采样 */

#define	RCC_SYSCLKSEL_APBPRES_Pos	16	/* APB时钟分频选择 */
#define	RCC_SYSCLKSEL_APBPRES_Msk	(0x7U << RCC_SYSCLKSEL_APBPRES_Pos)
#define	RCC_SYSCLKSEL_APBPRES_DIV1	(0x0U << RCC_SYSCLKSEL_APBPRES_Pos)
#define	RCC_SYSCLKSEL_APBPRES_DIV2	(0x4U << RCC_SYSCLKSEL_APBPRES_Pos)
#define	RCC_SYSCLKSEL_APBPRES_DIV4	(0x5U << RCC_SYSCLKSEL_APBPRES_Pos)
#define	RCC_SYSCLKSEL_APBPRES_DIV8	(0x6U << RCC_SYSCLKSEL_APBPRES_Pos)
#define	RCC_SYSCLKSEL_APBPRES_DIV16	(0x7U << RCC_SYSCLKSEL_APBPRES_Pos)

#define	RCC_SYSCLKSEL_AHBPRES_Pos	8	/* AHB时钟分频选择 */
#define	RCC_SYSCLKSEL_AHBPRES_Msk	(0x7U << RCC_SYSCLKSEL_AHBPRES_Pos)
#define	RCC_SYSCLKSEL_AHBPRES_DIV1	(0x0U << RCC_SYSCLKSEL_AHBPRES_Pos)
#define	RCC_SYSCLKSEL_AHBPRES_DIV2	(0x4U << RCC_SYSCLKSEL_AHBPRES_Pos)
#define	RCC_SYSCLKSEL_AHBPRES_DIV4	(0x5U << RCC_SYSCLKSEL_AHBPRES_Pos)
#define	RCC_SYSCLKSEL_AHBPRES_DIV8	(0x6U << RCC_SYSCLKSEL_AHBPRES_Pos)
#define	RCC_SYSCLKSEL_AHBPRES_DIV16	(0x7U << RCC_SYSCLKSEL_AHBPRES_Pos)

#define	RCC_SYSCLKSEL_SYSCLKSEL_Pos	0	/* 系统时钟源选择 */
#define	RCC_SYSCLKSEL_SYSCLKSEL_Msk	(0x3U << RCC_SYSCLKSEL_SYSCLKSEL_Pos)
#define	RCC_SYSCLKSEL_SYSCLKSEL_RCHF	(0x0U << RCC_SYSCLKSEL_SYSCLKSEL_Pos)
#define	RCC_SYSCLKSEL_SYSCLKSEL_RCLP	(0x1U << RCC_SYSCLKSEL_SYSCLKSEL_Pos)
#define	RCC_SYSCLKSEL_SYSCLKSEL_LSCLK	(0x2U << RCC_SYSCLKSEL_SYSCLKSEL_Pos)
#define	RCC_SYSCLKSEL_SYSCLKSEL_PLL	(0x3U << RCC_SYSCLKSEL_SYSCLKSEL_Pos)

#define	RCC_RCHFCON_FSEL_Pos	16	/* RCHF频率选择寄存器 */
#define	RCC_RCHFCON_FSEL_Msk	(0x3U << RCC_RCHFCON_FSEL_Pos)
#define	RCC_RCHFCON_FSEL_8MHZ	(0x0U << RCC_RCHFCON_FSEL_Pos)
#define	RCC_RCHFCON_FSEL_16MHZ	(0x1U << RCC_RCHFCON_FSEL_Pos)
#define	RCC_RCHFCON_FSEL_24MHZ	(0x2U << RCC_RCHFCON_FSEL_Pos)
#define	RCC_RCHFCON_FSEL_32MHZ	(0x3U << RCC_RCHFCON_FSEL_Pos)

#define	RCC_RCHFCON_RCHFEN_Pos	0	/* RCHF使能寄存器 */
#define	RCC_RCHFCON_RCHFEN_Msk	(0x1U << RCC_RCHFCON_RCHFEN_Pos)

#define	RCC_RCHFTRIM_RCHFTRIM_Pos	0	/* RCHF频率调校寄存器 */
#define	RCC_RCHFTRIM_RCHFTRIM_Msk	(0x7fU << RCC_RCHFTRIM_RCHFTRIM_Pos)

#define	RCC_PLLCON_PLLDB_Pos	16	/* PLL倍频数 */
#define	RCC_PLLCON_PLLDB_Msk	(0x3ffU << RCC_PLLCON_PLLDB_Pos)

#define	RCC_PLLCON_PLLOSEL_Pos	3	/* PLL输出选择寄存器 */
#define	RCC_PLLCON_PLLOSEL_Msk	(0x1U << RCC_PLLCON_PLLOSEL_Pos)
#define	RCC_PLLCON_PLLOSEL_MUL1	(0x0U << RCC_PLLCON_PLLOSEL_Pos)	/* 0：选择PLL一倍输出作为数字电路内的PLL时钟 */
#define	RCC_PLLCON_PLLOSEL_MUL2	(0x1U << RCC_PLLCON_PLLOSEL_Pos)	/* 1：选择PLL两倍输出作为数字电路内的PLL时钟 */

#define	RCC_PLLCON_PLLINSEL_Pos	1	/* PLL输入选择寄存器 */
#define	RCC_PLLCON_PLLINSEL_Msk	(0x3U << RCC_PLLCON_PLLINSEL_Pos)
#define	RCC_PLLCON_PLLINSEL_XTLF	(0x0U << RCC_PLLCON_PLLINSEL_Pos)
#define	RCC_PLLCON_PLLINSEL_RCLP	(0x1U << RCC_PLLCON_PLLINSEL_Pos)
#define	RCC_PLLCON_PLLINSEL_RCHFDIV256	(0x2U << RCC_PLLCON_PLLINSEL_Pos)

#define	RCC_PLLCON_PLLEN_Pos	0	/* PLL使能寄存器 */
#define	RCC_PLLCON_PLLEN_Msk	(0x1U << RCC_PLLCON_PLLEN_Pos)

#define	RCC_RCLPCON_RCLP_EN_B_Pos	0	/* RCLP使能状态，只读 */
#define	RCC_RCLPCON_RCLP_EN_B_Msk	(0x1U << RCC_RCLPCON_RCLP_EN_B_Pos)

#define	RCC_RCLPTRIM_RCLPTRIM_Pos	0	/* RCLP频率调校寄存器，调校步长约4% */
#define	RCC_RCLPTRIM_RCLPTRIM_Msk	(0xfU << RCC_RCLPTRIM_RCLPTRIM_Pos)

#define	RCC_XTLFIPW_XTLFIPW_Pos	0	/* XTLF工作电流选择 */
#define	RCC_XTLFIPW_XTLFIPW_Msk	(0x7U << RCC_XTLFIPW_XTLFIPW_Pos)
#define	RCC_XTLFIPW_XTLFIPW_450NA	(0x0U << RCC_XTLFIPW_XTLFIPW_Pos)
#define	RCC_XTLFIPW_XTLFIPW_400NA	(0x1U << RCC_XTLFIPW_XTLFIPW_Pos)
#define	RCC_XTLFIPW_XTLFIPW_350NA	(0x2U << RCC_XTLFIPW_XTLFIPW_Pos)
#define	RCC_XTLFIPW_XTLFIPW_300NA	(0x3U << RCC_XTLFIPW_XTLFIPW_Pos)
#define	RCC_XTLFIPW_XTLFIPW_250NA	(0x4U << RCC_XTLFIPW_XTLFIPW_Pos)
#define	RCC_XTLFIPW_XTLFIPW_200NA	(0x5U << RCC_XTLFIPW_XTLFIPW_Pos)
#define	RCC_XTLFIPW_XTLFIPW_150NA	(0x6U << RCC_XTLFIPW_XTLFIPW_Pos)
#define	RCC_XTLFIPW_XTLFIPW_100NA	(0x7U << RCC_XTLFIPW_XTLFIPW_Pos)

#define	RCC_PERCLKCON1_DCUCKEN_Pos	31	/* DCU总线时钟使能，高使能 */
#define	RCC_PERCLKCON1_DCUCKEN_Msk	(0x1U << RCC_PERCLKCON1_DCUCKEN_Pos)
	/* DCU总线时钟使能，高使能 */

#define	RCC_PERCLKCON1_EXTI2CKEN_Pos	10	/* EXTI2外部引脚中断,数字滤波采样时钟使能，高使能 */
#define	RCC_PERCLKCON1_EXTI2CKEN_Msk	(0x1U << RCC_PERCLKCON1_EXTI2CKEN_Pos)

#define	RCC_PERCLKCON1_EXTI1CKEN_Pos	9	/* EXTI1外部引脚中断,数字滤波采样时钟使能，高使能 */
#define	RCC_PERCLKCON1_EXTI1CKEN_Msk	(0x1U << RCC_PERCLKCON1_EXTI1CKEN_Pos)

#define	RCC_PERCLKCON1_EXTI0CKEN_Pos	8	/* EXTI0外部引脚中断,数字滤波采样时钟使能，高使能 */
#define	RCC_PERCLKCON1_EXTI0CKEN_Msk	(0x1U << RCC_PERCLKCON1_EXTI0CKEN_Pos)

#define	RCC_PERCLKCON1_PDCCKEN_Pos	7	/* PADCFG总线时钟使能，高使能 */
#define	RCC_PERCLKCON1_PDCCKEN_Msk	(0x1U << RCC_PERCLKCON1_PDCCKEN_Pos)

#define	RCC_PERCLKCON1_ANACCKEN_Pos	6	/* ANAC总线时钟使能，高使能 */
#define	RCC_PERCLKCON1_ANACCKEN_Msk	(0x1U << RCC_PERCLKCON1_ANACCKEN_Pos)

#define	RCC_PERCLKCON1_IWDTCKEN_Pos	5	/* IWDT总线时钟使能，高使能 */
#define	RCC_PERCLKCON1_IWDTCKEN_Msk	(0x1U << RCC_PERCLKCON1_IWDTCKEN_Pos)

#define	RCC_PERCLKCON1_SCUCKEN_Pos	4	/* SCU总线时钟使能，高使能 */
#define	RCC_PERCLKCON1_SCUCKEN_Msk	(0x1U << RCC_PERCLKCON1_SCUCKEN_Pos)

#define	RCC_PERCLKCON1_PMUCKEN_Pos	3	/* PMU总线时钟使能，高使能 */
#define	RCC_PERCLKCON1_PMUCKEN_Msk	(0x1U << RCC_PERCLKCON1_PMUCKEN_Pos)

#define	RCC_PERCLKCON1_RTCCKEN_Pos	2	/* RTC总线时钟使能，高使能 */
#define	RCC_PERCLKCON1_RTCCKEN_Msk	(0x1U << RCC_PERCLKCON1_RTCCKEN_Pos)

#define	RCC_PERCLKCON1_LPTFCKEN_Pos	1	/* LPTIM功能时钟使能，高使能 */
#define	RCC_PERCLKCON1_LPTFCKEN_Msk	(0x1U << RCC_PERCLKCON1_LPTFCKEN_Pos)

#define	RCC_PERCLKCON1_LPTRCKEN_Pos	0	/* LPTIM总线时钟使能，高使能 */
#define	RCC_PERCLKCON1_LPTRCKEN_Msk	(0x1U << RCC_PERCLKCON1_LPTRCKEN_Pos)

#define	RCC_PERCLKCON2_ADCCKSEL_Pos	16	/* ADC工作时钟选择 */
#define	RCC_PERCLKCON2_ADCCKSEL_Msk	(0x7U << RCC_PERCLKCON2_ADCCKSEL_Pos)
#define	RCC_PERCLKCON2_ADCCKSEL_RCHF	(0x0U << RCC_PERCLKCON2_ADCCKSEL_Pos)
#define	RCC_PERCLKCON2_ADCCKSEL_RCHFDIV2	(0x1U << RCC_PERCLKCON2_ADCCKSEL_Pos)
#define	RCC_PERCLKCON2_ADCCKSEL_RCHFDIV4	(0x2U << RCC_PERCLKCON2_ADCCKSEL_Pos)
#define	RCC_PERCLKCON2_ADCCKSEL_RCHFDIV8	(0x3U << RCC_PERCLKCON2_ADCCKSEL_Pos)
#define	RCC_PERCLKCON2_ADCCKSEL_RCHFDIV16	(0x4U << RCC_PERCLKCON2_ADCCKSEL_Pos)
#define	RCC_PERCLKCON2_ADCCKSEL_RCHFDIV32	(0x5U << RCC_PERCLKCON2_ADCCKSEL_Pos)
#define	RCC_PERCLKCON2_ADCCKSEL_RCHFDIV64	(0x6U << RCC_PERCLKCON2_ADCCKSEL_Pos)
#define	RCC_PERCLKCON2_ADCCKSEL_RCLP	(0x7U << RCC_PERCLKCON2_ADCCKSEL_Pos)

#define	RCC_PERCLKCON2_ADCCKEN_Pos	8	/* ADC时钟使能，高使能 */
#define	RCC_PERCLKCON2_ADCCKEN_Msk	(0x1U << RCC_PERCLKCON2_ADCCKEN_Pos)

#define	RCC_PERCLKCON2_WWDTCKEN_Pos	7	/* WWDT时钟使能，高使能 */
#define	RCC_PERCLKCON2_WWDTCKEN_Msk	(0x1U << RCC_PERCLKCON2_WWDTCKEN_Pos)

#define	RCC_PERCLKCON2_RAMBISTCKEN_Pos	6	/* RAMBIST时钟使能，高使能 */
#define	RCC_PERCLKCON2_RAMBISTCKEN_Msk	(0x1U << RCC_PERCLKCON2_RAMBISTCKEN_Pos)

#define	RCC_PERCLKCON2_FLSEPCKEN_Pos	5	/* FLSC（Flash擦写控制器）时钟使能，高使能 */
#define	RCC_PERCLKCON2_FLSEPCKEN_Msk	(0x1U << RCC_PERCLKCON2_FLSEPCKEN_Pos)

#define	RCC_PERCLKCON2_DMACKEN_Pos	4	/* DMA时钟使能，高使能 */
#define	RCC_PERCLKCON2_DMACKEN_Msk	(0x1U << RCC_PERCLKCON2_DMACKEN_Pos)

#define	RCC_PERCLKCON2_LCDCKEN_Pos	3	/* LCD时钟使能，高使能 */
#define	RCC_PERCLKCON2_LCDCKEN_Msk	(0x1U << RCC_PERCLKCON2_LCDCKEN_Pos)

#define	RCC_PERCLKCON2_AESCKEN_Pos	2	/* AES时钟使能，高使能 */
#define	RCC_PERCLKCON2_AESCKEN_Msk	(0x1U << RCC_PERCLKCON2_AESCKEN_Pos)

#define	RCC_PERCLKCON2_TRNGCKEN_Pos	1	/* RNG时钟使能，高使能 */
#define	RCC_PERCLKCON2_TRNGCKEN_Msk	(0x1U << RCC_PERCLKCON2_TRNGCKEN_Pos)

#define	RCC_PERCLKCON2_CRCCKEN_Pos	0	/* CRC时钟使能，高使能 */
#define	RCC_PERCLKCON2_CRCCKEN_Msk	(0x1U << RCC_PERCLKCON2_CRCCKEN_Pos)

#define	RCC_PERCLKCON3_I2CCKE_Pos	24	/* I2C时钟使能 */
#define	RCC_PERCLKCON3_I2CCKE_Msk	(0x1U << RCC_PERCLKCON3_I2CCKE_Pos)

#define	RCC_PERCLKCON3_U7816CKE1_Pos	17	/* U7816-1时钟使能 */
#define	RCC_PERCLKCON3_U7816CKE1_Msk	(0x1U << RCC_PERCLKCON3_U7816CKE1_Pos)

#define	RCC_PERCLKCON3_U7816CKE0_Pos	16	/* U7816-0时钟使能 */
#define	RCC_PERCLKCON3_U7816CKE0_Msk	(0x1U << RCC_PERCLKCON3_U7816CKE0_Pos)

#define	RCC_PERCLKCON3_UARTCCKEN_Pos	14	/* UART0~5共享寄存器时钟使能，高使能 */
#define	RCC_PERCLKCON3_UARTCCKEN_Msk	(0x1U << RCC_PERCLKCON3_UARTCCKEN_Pos)

#define	RCC_PERCLKCON3_UART5CKE_Pos	13	/* UART5时钟使能 */
#define	RCC_PERCLKCON3_UART5CKE_Msk	(0x1U << RCC_PERCLKCON3_UART5CKE_Pos)

#define	RCC_PERCLKCON3_UART4CKE_Pos	12	/* UART4时钟使能 */
#define	RCC_PERCLKCON3_UART4CKE_Msk	(0x1U << RCC_PERCLKCON3_UART4CKE_Pos)

#define	RCC_PERCLKCON3_UART3CKE_Pos	11	/* UART3时钟使能 */
#define	RCC_PERCLKCON3_UART3CKE_Msk	(0x1U << RCC_PERCLKCON3_UART3CKE_Pos)

#define	RCC_PERCLKCON3_UART2CKE_Pos	10	/* UART2时钟使能 */
#define	RCC_PERCLKCON3_UART2CKE_Msk	(0x1U << RCC_PERCLKCON3_UART2CKE_Pos)

#define	RCC_PERCLKCON3_UART1CKE_Pos	9	/* UART1时钟使能 */
#define	RCC_PERCLKCON3_UART1CKE_Msk	(0x1U << RCC_PERCLKCON3_UART1CKE_Pos)

#define	RCC_PERCLKCON3_UART0CKE_Pos	8	/* UART0时钟使能 */
#define	RCC_PERCLKCON3_UART0CKE_Msk	(0x1U << RCC_PERCLKCON3_UART0CKE_Pos)

#define	RCC_PERCLKCON3_HSPICKE_Pos	2	/* HSPI时钟使能 */
#define	RCC_PERCLKCON3_HSPICKE_Msk	(0x1U << RCC_PERCLKCON3_HSPICKE_Pos)

#define	RCC_PERCLKCON3_SPI2CKE_Pos	1	/* SPI2时钟使能 */
#define	RCC_PERCLKCON3_SPI2CKE_Msk	(0x1U << RCC_PERCLKCON3_SPI2CKE_Pos)

#define	RCC_PERCLKCON3_SPI1CKE_Pos	0	/* SPI1时钟使能 */
#define	RCC_PERCLKCON3_SPI1CKE_Msk	(0x1U << RCC_PERCLKCON3_SPI1CKE_Pos)

#define	RCC_PERCLKCON4_ET4CKE_Pos	5	/* 扩展定时器4时钟使能，高使能 */
#define	RCC_PERCLKCON4_ET4CKE_Msk	(0x1U << RCC_PERCLKCON4_ET4CKE_Pos)

#define	RCC_PERCLKCON4_ET3CKE_Pos	4	/* 扩展定时器3时钟使能，高使能 */
#define	RCC_PERCLKCON4_ET3CKE_Msk	(0x1U << RCC_PERCLKCON4_ET3CKE_Pos)

#define	RCC_PERCLKCON4_ET2CKE_Pos	3	/* 扩展定时器2时钟使能，高使能 */
#define	RCC_PERCLKCON4_ET2CKE_Msk	(0x1U << RCC_PERCLKCON4_ET2CKE_Pos)

#define	RCC_PERCLKCON4_ET1CKE_Pos	2	/* 扩展定时器1时钟使能，高使能 */
#define	RCC_PERCLKCON4_ET1CKE_Msk	(0x1U << RCC_PERCLKCON4_ET1CKE_Pos)

#define	RCC_PERCLKCON4_BT2CKE_Pos	1	/* 基本定时器2时钟使能，高使能 */
#define	RCC_PERCLKCON4_BT2CKE_Msk	(0x1U << RCC_PERCLKCON4_BT2CKE_Pos)

#define	RCC_PERCLKCON4_BT1CKE_Pos	0	/* 基本定时器1时钟使能，高使能 */
#define	RCC_PERCLKCON4_BT1CKE_Msk	(0x1U << RCC_PERCLKCON4_BT1CKE_Pos)

#define	RCC_MPRIL_MPRIL_Pos	0	/* AHB Master优先级配置 */
#define	RCC_MPRIL_MPRIL_Msk	(0x1U << RCC_MPRIL_MPRIL_Pos)
#define	RCC_MPRIL_MPRIL_DMA	(0x0U << RCC_MPRIL_MPRIL_Pos)
#define	RCC_MPRIL_MPRIL_CPU	(0x1U << RCC_MPRIL_MPRIL_Pos)
//Macro_End

/* Exported functions --------------------------------------------------------*/ 
extern void RCC_Deinit(void);

/* Sleep/DeepSleep模式下禁止RCLP，仅在无晶体配置下有效 相关函数 */
extern void RCC_SYSCLKSEL_LPM_RCLP_OFF_Setable(FunState NewState);
extern FunState RCC_SYSCLKSEL_LPM_RCLP_OFF_Getable(void);

/* Sleep/DeepSleep模式下EXTI采样设置 相关函数 */
extern void RCC_SYSCLKSEL_SLP_ENEXTI_Setable(FunState NewState);
extern FunState RCC_SYSCLKSEL_SLP_ENEXTI_Getable(void);

/* EXTI中断与数字滤波采样时钟选择 相关函数 */
extern void RCC_SYSCLKSEL_EXTICKSEL_Set(uint32_t SetValue);
extern uint32_t RCC_SYSCLKSEL_EXTICKSEL_Get(void);

/* APB时钟分频选择 相关函数 */
extern void RCC_SYSCLKSEL_APBPRES_Set(uint32_t SetValue);
extern uint32_t RCC_SYSCLKSEL_APBPRES_Get(void);

/* AHB时钟分频选择 相关函数 */
extern void RCC_SYSCLKSEL_AHBPRES_Set(uint32_t SetValue);
extern uint32_t RCC_SYSCLKSEL_AHBPRES_Get(void);

/* 系统时钟源选择 相关函数 */
extern void RCC_SYSCLKSEL_SYSCLKSEL_Set(uint32_t SetValue);
extern uint32_t RCC_SYSCLKSEL_SYSCLKSEL_Get(void);

/* RCHF频率选择寄存器 相关函数 */
extern void RCC_RCHFCON_FSEL_Set(uint32_t SetValue);
extern uint32_t RCC_RCHFCON_FSEL_Get(void);

/* RCHF使能寄存器 相关函数 */
extern void RCC_RCHFCON_RCHFEN_Setable(FunState NewState);
extern FunState RCC_RCHFCON_RCHFEN_Getable(void);

/* RCHF频率调校寄存器 相关函数 */
extern void RCC_RCHFTRIM_Write(uint32_t SetValue);
extern uint32_t RCC_RCHFTRIM_Read(void);

/* PLL倍频数 相关函数 */
extern void RCC_PLLCON_PLLDB_Set(uint32_t SetValue);
extern uint32_t RCC_PLLCON_PLLDB_Get(void);

/* PLL输出选择寄存器 相关函数 */
extern void RCC_PLLCON_PLLOSEL_Set(uint32_t SetValue);
extern uint32_t RCC_PLLCON_PLLOSEL_Get(void);

/* PLL输入选择寄存器 相关函数 */
extern void RCC_PLLCON_PLLINSEL_Set(uint32_t SetValue);
extern uint32_t RCC_PLLCON_PLLINSEL_Get(void);

/* PLL使能寄存器 相关函数 */
extern void RCC_PLLCON_PLLEN_Setable(FunState NewState);
extern FunState RCC_PLLCON_PLLEN_Getable(void);

/* RCLP使能状态，只读 相关函数 */
extern FlagStatus RCC_RCLPCON_RCLP_EN_B_Chk(void);

/* RCLP频率调校寄存器，调校步长约4% 相关函数 */
extern void RCC_RCLPTRIM_Write(uint32_t SetValue);
extern uint32_t RCC_RCLPTRIM_Read(void);

/* XTLF工作电流选择 相关函数 */
extern void RCC_XTLFIPW_XTLFIPW_Set(uint32_t SetValue);
extern uint32_t RCC_XTLFIPW_XTLFIPW_Get(void);

/* ADC工作时钟选择 相关函数 */
extern void RCC_PERCLKCON2_ADCCKSEL_Set(uint32_t SetValue);
extern uint32_t RCC_PERCLKCON2_ADCCKSEL_Get(void);

/* AHB Master优先级配置 相关函数 */
extern void RCC_MPRIL_MPRIL_Set(uint32_t SetValue);
extern uint32_t RCC_MPRIL_MPRIL_Get(void);
//Announce_End



/* 外设时钟配置函数 */
extern void RCC_PERCLK_SetableEx(uint32_t periph_def, FunState NewState);

/* RCHF初始化 */
void RCC_RCHF_Init(RCC_RCHF_InitTypeDef* para);

/* PLL初始化 */
void RCC_PLL_Init(RCC_PLL_InitTypeDef* para);

/* 系统时钟初始化 */
void RCC_SysClk_Init(RCC_SYSCLK_InitTypeDef* para);

extern void RCC_Init_RCHF_Trim( uint8_t ClkMode );

extern void RCC_GetClocksFreq(RCC_ClocksType* para);
/* 其他宏定义 */
//外设时钟控制参数定义
#define	DCUCLK			(0x01000000 + RCC_PERCLKCON1_DCUCKEN_Pos)	//31	/* DCU总线时钟使能，高使能 */
#define	EXTI2CLK		(0x01000000 + RCC_PERCLKCON1_EXTI2CKEN_Pos)	//10	/* EXTI2外部引脚中断,数字滤波采样时钟使能，高使能 */
#define	EXTI1CLK		(0x01000000 + RCC_PERCLKCON1_EXTI1CKEN_Pos)	//9	/* EXTI1外部引脚中断,数字滤波采样时钟使能，高使能 */
#define	EXTI0CLK		(0x01000000 + RCC_PERCLKCON1_EXTI0CKEN_Pos)	//8	/* EXTI0外部引脚中断,数字滤波采样时钟使能，高使能 */
#define	PDCCLK			(0x01000000 + RCC_PERCLKCON1_PDCCKEN_Pos)	//7	/* PADCFG总线时钟使能，高使能 */
#define	ANACCLK			(0x01000000 + RCC_PERCLKCON1_ANACCKEN_Pos)	//6	/* ANAC总线时钟使能，高使能 */
#define	IWDTCLK			(0x01000000 + RCC_PERCLKCON1_IWDTCKEN_Pos)	//5	/* IWDT总线时钟使能，高使能 */
#define	SCUCLK			(0x01000000 + RCC_PERCLKCON1_SCUCKEN_Pos)	//4	/* SCU总线时钟使能，高使能 */
#define	PMUCLK			(0x01000000 + RCC_PERCLKCON1_PMUCKEN_Pos)	//3	/* PMU总线时钟使能，高使能 */
#define	RTCCLK			(0x01000000 + RCC_PERCLKCON1_RTCCKEN_Pos)	//2	/* RTC总线时钟使能，高使能 */
#define	LPTFCLK			(0x01000000 + RCC_PERCLKCON1_LPTFCKEN_Pos)	//1	/* LPTIM功能时钟使能，高使能 */
#define	LPTRCLK			(0x01000000 + RCC_PERCLKCON1_LPTRCKEN_Pos)	//0	/* LPTIM总线时钟使能，高使能 */
        
#define	ADCCLK			(0x02000000 + RCC_PERCLKCON2_ADCCKEN_Pos)	//8	/* ADC时钟使能，高使能 */
#define	WWDTCLK			(0x02000000 + RCC_PERCLKCON2_WWDTCKEN_Pos)	//7	/* WWDT时钟使能，高使能 */
#define	RAMBISTCLK		(0x02000000 + RCC_PERCLKCON2_RAMBISTCKEN_Pos)	//6	/* RAMBIST时钟使能，高使能 */
#define	FLSEPCLK		(0x02000000 + RCC_PERCLKCON2_FLSEPCKEN_Pos)	//5	/* FLSC（Flash擦写控制器）时钟使能，高使能 */
#define	DMACLK			(0x02000000 + RCC_PERCLKCON2_DMACKEN_Pos)	//4	/* DMA时钟使能，高使能 */
#define	LCDCLK			(0x02000000 + RCC_PERCLKCON2_LCDCKEN_Pos)	//3	/* LCD时钟使能，高使能 */
#define	AESCLK			(0x02000000 + RCC_PERCLKCON2_AESCKEN_Pos)	//2	/* AES时钟使能，高使能 */
#define	TRNGCLK			(0x02000000 + RCC_PERCLKCON2_TRNGCKEN_Pos)	//1	/* RNG时钟使能，高使能 */
#define	CRCCLK			(0x02000000 + RCC_PERCLKCON2_CRCCKEN_Pos)	//0	/* CRC时钟使能，高使能 */

#define	I2CCLK			(0x03000000 + RCC_PERCLKCON3_I2CCKE_Pos)	//24	/* I2C时钟使能 */
#define	U7816CLK1		(0x03000000 + RCC_PERCLKCON3_U7816CKE1_Pos)	//17	/* U7816-1时钟使能 */
#define	U7816CLK0		(0x03000000 + RCC_PERCLKCON3_U7816CKE0_Pos)	//16	/* U7816-0时钟使能 */
#define	UARTCOMCLK		(0x03000000 + RCC_PERCLKCON3_UARTCCKEN_Pos)	//14	/* UART0~5共享寄存器时钟使能，高使能 */
#define	UART5CLK		(0x03000000 + RCC_PERCLKCON3_UART5CKE_Pos)	//13	/* UART5时钟使能 */
#define	UART4CLK		(0x03000000 + RCC_PERCLKCON3_UART4CKE_Pos)	//12	/* UART4时钟使能 */
#define	UART3CLK		(0x03000000 + RCC_PERCLKCON3_UART3CKE_Pos)	//11	/* UART3时钟使能 */
#define	UART2CLK		(0x03000000 + RCC_PERCLKCON3_UART2CKE_Pos)	//10	/* UART2时钟使能 */
#define	UART1CLK		(0x03000000 + RCC_PERCLKCON3_UART1CKE_Pos)	//9	/* UART1时钟使能 */
#define	UART0CLK		(0x03000000 + RCC_PERCLKCON3_UART0CKE_Pos)	//8	/* UART0时钟使能 */
#define	HSPICLK			(0x03000000 + RCC_PERCLKCON3_HSPICKE_Pos)	//2	/* HSPI时钟使能 */
#define	SPI2CLK			(0x03000000 + RCC_PERCLKCON3_SPI2CKE_Pos)	//1	/* SPI2时钟使能 */
#define	SPI1CLK			(0x03000000 + RCC_PERCLKCON3_SPI1CKE_Pos)	//0	/* SPI1时钟使能 */
			
#define	ET4CLK			(0x04000000 + RCC_PERCLKCON4_ET4CKE_Pos)	//5	/* 扩展定时器4时钟使能，高使能 */
#define	ET3CLK			(0x04000000 + RCC_PERCLKCON4_ET3CKE_Pos)	//4	/* 扩展定时器3时钟使能，高使能 */
#define	ET2CLK			(0x04000000 + RCC_PERCLKCON4_ET2CKE_Pos)	//3	/* 扩展定时器2时钟使能，高使能 */
#define	ET1CLK			(0x04000000 + RCC_PERCLKCON4_ET1CKE_Pos)	//2	/* 扩展定时器1时钟使能，高使能 */
#define	BT2CLK			(0x04000000 + RCC_PERCLKCON4_BT2CKE_Pos)	//1	/* 基本定时器2时钟使能，高使能 */
#define	BT1CLK			(0x04000000 + RCC_PERCLKCON4_BT1CKE_Pos)	//0	/* 基本定时器1时钟使能，高使能 */


#ifdef __cplusplus
}
#endif

#endif /* __FM33A0XX_RCC_H */



/************************ (C) COPYRIGHT FMSHelectronics *****END OF FILE****/



