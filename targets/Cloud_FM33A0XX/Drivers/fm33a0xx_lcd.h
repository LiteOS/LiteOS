/**
  ******************************************************************************
  * @file    fm33a0xx_lcd.h
  * @author  FM33a0xx Application Team
  * @version V2.0.1
  * @date    11-1-2018
  * @brief   This file contains all the functions prototypes for the LCD firmware library.  
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FM33A0XX_LCD_H
#define __FM33A0XX_LCD_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "FM33A0XX.h"
	 
/** @addtogroup FM385_StdPeriph_Driver
  * @{
  */

/** @addtogroup LCD
  * @{
  */
	 
	 
/* Exported constants --------------------------------------------------------*/ 
/* Exported types ------------------------------------------------------------*/
typedef struct
{       
	uint32_t			LMUX;		/*!<COM数量选择*/	
	uint32_t			ENMODE;		/*!<驱动模式选择*/
	uint32_t			WFT;		/*!<驱动波形选择*/
	uint32_t			DF;			/*!<显示频率控制寄存器*/
	uint32_t			BIASMD;		/*!<偏置类型控制*/
	uint32_t			SCFSEL;		/*!<SC频率选择*/  //仅在片外电容模式下有效
	uint32_t			SC_CTRL;	/*!<片外电容驱动模式下，驱动方式控制*/
	uint32_t			IC_CTRL;	/*!<片内电阻模式下，驱动电流大小*/
	uint32_t			LCDBIAS;	/*!<显示灰度设置*/
	FunState			ANTIPOLAR;	/*!<防极化使能控制*/
	
	FunState			TEST;		/*!<测试模式使能控制，仅在DISPMD=1的情况下有效*/
	FunState			DISPMD;		/*!<测试模式使能控制*/
	
	uint32_t			LCCTRL;		/*!<保持为0*/
	FunState			TESTEN;		/*!<保持关闭*/
	
	FunState			FLICK;		/*!<显示闪烁使目刂栖*/
	uint32_t			TON;		/*!<闪烁显示时的点亮时间＝TON×0.25秒*/
	uint32_t			TOFF;		/*!<闪烁显示时的熄灭时间＝TOFF×0.25秒*/
	FunState			DONIE;		/*!<显示点亮中断使能控制*/
	FunState			DOFFIE;		/*!<显示熄灭中断使能控制*/
	
	FunState			LCDEN;		/*!<LCD显示使能控制*/
}LCD_InitTypeDef;

typedef struct
{   
	//使用boost功能需在VLCD脚外接0.1uF电容！
    uint32_t			OSC_CFG;		/*!<BOOST环振频率配置信号,值越大频率越高，驱动越强，功耗越高*/  //推荐使用GRADE1
	uint32_t			VLCDCFG;		/*!<VLCD输出电压配置*/
	FunState			BUFENB;			/*!<基准源Buffer使能控制*/  //需要打开
	FunState			BUFBYP;			/*!<基准源Buffer Bypass使能控制*/  //需要关闭
	FunState			VLCDPD;			/*!<VLCD下拉使能控制*/  //需要关闭
	FunState			BOOST_TEN;		/*!<Booster测试信号*/  //保持关闭即可
	FunState			BOOST_EN;		/*!<Booster使能控制*/
}LCD_Boost_InitTypeDef;
	 
/* Exported macro ------------------------------------------------------------*/

//Macro_START
//2018-04-02-13-21-29
//FM33A0XX_Driver_Gen_V1.4

#define	LCD_DISPCTRL_ANTIPOLAR_Pos	7	/* 防极化 */
#define	LCD_DISPCTRL_ANTIPOLAR_Msk	(0x1U << LCD_DISPCTRL_ANTIPOLAR_Pos)
	/* 0：COM和SEG在LCD关闭情况下浮空 */
	/* 1：COM和SEG在LCD关闭情况下接地 */

#define	LCD_DISPCTRL_LCDEN_Pos	6	/* LCD显示使能 */
#define	LCD_DISPCTRL_LCDEN_Msk	(0x1U << LCD_DISPCTRL_LCDEN_Pos)
	/* 0：关闭LCD显示 */
	/* 1：启动LCD显示 */

#define	LCD_DISPCTRL_FLICK_Pos	2	/* 显示闪烁使能 */
#define	LCD_DISPCTRL_FLICK_Msk	(0x1U << LCD_DISPCTRL_FLICK_Pos)
	/* 0：关闭闪烁 */
	/* 1：显示闪烁，闪烁频率由TON和TOFF寄存器设置 */

#define	LCD_DISPCTRL_TEST_Pos	1	/* 测试模式使能，仅在DISPMD=1的情况下有效 */
#define	LCD_DISPCTRL_TEST_Msk	(0x1U << LCD_DISPCTRL_TEST_Pos)
	/* 0：显示全灭 */
	/* 1：显示全亮 */

#define	LCD_DISPCTRL_DISPMD_Pos	0	/* 测试模式选择 */
#define	LCD_DISPCTRL_DISPMD_Msk	(0x1U << LCD_DISPCTRL_DISPMD_Pos)
	/* 0：正常模式，TEST位无效 */
	/* 1：显示测试模式，TEST位有效 */

#define	LCD_LCDTEST_LCCTRL_Pos	7	/* LCD测试控制位，仅在测试模式下有效 */
#define	LCD_LCDTEST_LCCTRL_Msk	(0x1U << LCD_LCDTEST_LCCTRL_Pos)
#define	LCD_LCDTEST_LCCTRL_0	(0x0U << LCD_LCDTEST_LCCTRL_Pos)
#define	LCD_LCDTEST_LCCTRL_1	(0x1U << LCD_LCDTEST_LCCTRL_Pos)

#define	LCD_LCDTEST_TESTEN_Pos	0	/* 测试模式使能 */
#define	LCD_LCDTEST_TESTEN_Msk	(0x1U << LCD_LCDTEST_TESTEN_Pos)
	/* 正常工作模式 */
	/* LCD测试模式使能 */

#define	LCD_DF_DF_Pos	0	/* 显示频率控制寄存器 */
#define	LCD_DF_DF_Msk	(0xffU << LCD_DF_DF_Pos)

#define	LCD_TON_TON_Pos	0	/* 闪烁显示点亮时间寄存器 */
#define	LCD_TON_TON_Msk	(0xffU << LCD_TON_TON_Pos)

#define	LCD_TOFF_TOFF_Pos	0	/* 闪烁显示熄灭时间寄存器 */
#define	LCD_TOFF_TOFF_Msk	(0xffU << LCD_TOFF_TOFF_Pos)

#define	LCD_DISPIE_DONIE_Pos	1	/* 显示点亮中断使能 */
#define	LCD_DISPIE_DONIE_Msk	(0x1U << LCD_DISPIE_DONIE_Pos)
	/* 0 = 显示点亮中断禁止 */
	/* 1 = 显示点亮中断使能 */

#define	LCD_DISPIE_DOFFIE_Pos	0	/* 显示熄灭中断使能 */
#define	LCD_DISPIE_DOFFIE_Msk	(0x1U << LCD_DISPIE_DOFFIE_Pos)
	/* 0 = 显示熄灭中断禁止 */
	/* 1 = 显示熄灭中断使能 */

#define	LCD_DISPIF_DONIF_Pos	1	/* 显示点亮中断标志 */
#define	LCD_DISPIF_DONIF_Msk	(0x1U << LCD_DISPIF_DONIF_Pos)

#define	LCD_DISPIF_DOFFIF_Pos	0	/* 显示熄灭中断标志 */
#define	LCD_DISPIF_DOFFIF_Msk	(0x1U << LCD_DISPIF_DOFFIF_Pos)

#define	LCD_LCDSET_BIASMD_Pos	4	/* 偏置类型控制 */
#define	LCD_LCDSET_BIASMD_Msk	(0x1U << LCD_LCDSET_BIASMD_Pos)
#define	LCD_LCDSET_BIASMD_4BIAS	(0x0U << LCD_LCDSET_BIASMD_Pos)	/* 0：1/4 Bias */
#define	LCD_LCDSET_BIASMD_3BIAS	(0x1U << LCD_LCDSET_BIASMD_Pos)	/* 1：1/3 Bias */

#define	LCD_LCDSET_WFT_Pos	2	/* 驱动波形选择 */
#define	LCD_LCDSET_WFT_Msk	(0x1U << LCD_LCDSET_WFT_Pos)
#define	LCD_LCDSET_WFT_ATYPE	(0x0U << LCD_LCDSET_WFT_Pos)	/* 0：A类波形 */
#define	LCD_LCDSET_WFT_BTYPE	(0x1U << LCD_LCDSET_WFT_Pos)	/* 1：B类波形 */

#define	LCD_LCDSET_LMUX_Pos	0	/* COM数量选择 */
#define	LCD_LCDSET_LMUX_Msk	(0x3U << LCD_LCDSET_LMUX_Pos)
#define	LCD_LCDSET_LMUX_4COM	(0x0U << LCD_LCDSET_LMUX_Pos)	/* 00：4COM */
#define	LCD_LCDSET_LMUX_6COM	(0x1U << LCD_LCDSET_LMUX_Pos)	/* 01：6COM */
#define	LCD_LCDSET_LMUX_8COM	(0x2U << LCD_LCDSET_LMUX_Pos)	/* 10/11：8COM */

#define	LCD_LCDDRV_SCFSEL_Pos	5	/* SC频率选择 */
#define	LCD_LCDDRV_SCFSEL_Msk	(0x7U << LCD_LCDDRV_SCFSEL_Pos)
#define	LCD_LCDDRV_SCFSEL_X1	(0x0U << LCD_LCDDRV_SCFSEL_Pos)	/* 000 = 频率为帧频*COM数(注意：当选择110或111档位时，如果频率低于帧频*2*COM，则输出结果与000档位相同) */
#define	LCD_LCDDRV_SCFSEL_X8	(0x1U << LCD_LCDDRV_SCFSEL_Pos)	/* 001 = 频率为disp_clk/8（若disp_clk=32KHz，则为4KHz） */
#define	LCD_LCDDRV_SCFSEL_X16	(0x2U << LCD_LCDDRV_SCFSEL_Pos)	/* 010 = 频率为disp_clk/16（若disp_clk=32KHz，则为2KHz） */
#define	LCD_LCDDRV_SCFSEL_X32	(0x3U << LCD_LCDDRV_SCFSEL_Pos)	/* 011 = 频率为disp_clk/32（若disp_clk=32KHz，则为1KHz） */
#define	LCD_LCDDRV_SCFSEL_X64	(0x4U << LCD_LCDDRV_SCFSEL_Pos)	/* 100 = 频率为disp_clk/64（若disp_clk=32KHz，则为500Hz） */
#define	LCD_LCDDRV_SCFSEL_X128	(0x5U << LCD_LCDDRV_SCFSEL_Pos)	/* 101 = 频率为disp_clk/128（若disp_clk=32KHz，则为250Hz） */
#define	LCD_LCDDRV_SCFSEL_X256	(0x6U << LCD_LCDDRV_SCFSEL_Pos)	/* 110 = 频率为disp_clk/256（若disp_clk=32KHz，则为125Hz） */
#define	LCD_LCDDRV_SCFSEL_X512	(0x7U << LCD_LCDDRV_SCFSEL_Pos)	/* 111= 频率为disp_clk/512（若disp_clk=32KHz，则为62.5Hz） */

#define	LCD_LCDDRV_SC_CTRL_Pos	3	/* 片外电容驱动模式下，驱动方式控制 */
#define	LCD_LCDDRV_SC_CTRL_Msk	(0x3U << LCD_LCDDRV_SC_CTRL_Pos)
#define	LCD_LCDDRV_SC_CTRL_ONE	(0x0U << LCD_LCDDRV_SC_CTRL_Pos)	/* 00 = 单次驱动 */
#define	LCD_LCDDRV_SC_CTRL_TWO	(0x1U << LCD_LCDDRV_SC_CTRL_Pos)	/* 01 = 连续驱动2次 */
#define	LCD_LCDDRV_SC_CTRL_FOUR	(0x2U << LCD_LCDDRV_SC_CTRL_Pos)	/* 10 = 连续驱动4次，当SC频率大于等于4KHz时，此选项也为多次驱动 */
#define	LCD_LCDDRV_SC_CTRL_CONTINUE	(0x3U << LCD_LCDDRV_SC_CTRL_Pos)	/* 11 = 多次驱动 */

#define	LCD_LCDDRV_IC_CTRL_Pos	1	/* 驱动电流大小 */
#define	LCD_LCDDRV_IC_CTRL_Msk	(0x3U << LCD_LCDDRV_IC_CTRL_Pos)
#define	LCD_LCDDRV_IC_CTRL_L3	(0x0U << LCD_LCDDRV_IC_CTRL_Pos)	/* 00 = 电流最大 */
#define	LCD_LCDDRV_IC_CTRL_L2	(0x1U << LCD_LCDDRV_IC_CTRL_Pos)	/* 01 = 电流次大 */
#define	LCD_LCDDRV_IC_CTRL_L1	(0x2U << LCD_LCDDRV_IC_CTRL_Pos)	/* 10 = 电流次小 */
#define	LCD_LCDDRV_IC_CTRL_L0	(0x3U << LCD_LCDDRV_IC_CTRL_Pos)	/* 11 = 电流最小 */

#define	LCD_LCDDRV_ENMODE_Pos	0	/* 驱动模式选择 */
#define	LCD_LCDDRV_ENMODE_Msk	(0x1U << LCD_LCDDRV_ENMODE_Pos)
#define	LCD_LCDDRV_ENMODE_EXTERNALCAP	(0x0U << LCD_LCDDRV_ENMODE_Pos)	/* 0 = 片外电容驱动 */
#define	LCD_LCDDRV_ENMODE_INNERRESISTER	(0x1U << LCD_LCDDRV_ENMODE_Pos)	/* 1 = 片内电阻型驱动 */

#define	LCD_LCDBIAS_LCDBIAS_Pos	0	/* 显示灰度设置 */
#define	LCD_LCDBIAS_LCDBIAS_Msk	(0xfU << LCD_LCDBIAS_LCDBIAS_Pos)

#define	LCD_LCDBSTCON_OSC_CFG_Pos	12	/* BOOST环振频率配置信号,值越大频率越高，驱动越强，功耗越高 */
#define	LCD_LCDBSTCON_OSC_CFG_Msk	(0xfU << LCD_LCDBSTCON_OSC_CFG_Pos)
#define	LCD_LCDBSTCON_OSC_CFG_GRADE0	(0x0U << LCD_LCDBSTCON_OSC_CFG_Pos)
#define	LCD_LCDBSTCON_OSC_CFG_GRADE1	(0x1U << LCD_LCDBSTCON_OSC_CFG_Pos)
#define	LCD_LCDBSTCON_OSC_CFG_GRADE2	(0x2U << LCD_LCDBSTCON_OSC_CFG_Pos)
#define	LCD_LCDBSTCON_OSC_CFG_GRADE3	(0x3U << LCD_LCDBSTCON_OSC_CFG_Pos)
#define	LCD_LCDBSTCON_OSC_CFG_GRADE4	(0x4U << LCD_LCDBSTCON_OSC_CFG_Pos)
#define	LCD_LCDBSTCON_OSC_CFG_GRADE5	(0x5U << LCD_LCDBSTCON_OSC_CFG_Pos)
#define	LCD_LCDBSTCON_OSC_CFG_GRADE6	(0x6U << LCD_LCDBSTCON_OSC_CFG_Pos)
#define	LCD_LCDBSTCON_OSC_CFG_GRADE7	(0x7U << LCD_LCDBSTCON_OSC_CFG_Pos)
#define	LCD_LCDBSTCON_OSC_CFG_GRADE8	(0x8U << LCD_LCDBSTCON_OSC_CFG_Pos)

#define	LCD_LCDBSTCON_BUFENB_Pos	9	/* 基准源Buffer使能信号 */
#define	LCD_LCDBSTCON_BUFENB_Msk	(0x1U << LCD_LCDBSTCON_BUFENB_Pos)
	/* 0：打开Buffer */
	/* 1：关闭Buffer */

#define	LCD_LCDBSTCON_BUFBYP_Pos	8	/* 基准源Buffer Bypass */
#define	LCD_LCDBSTCON_BUFBYP_Msk	(0x1U << LCD_LCDBSTCON_BUFBYP_Pos)
	/* 0：不Bypass */
	/* 1：Bypass Buffer */

#define	LCD_LCDBSTCON_VLCDPD_Pos	7	/* VLCD下拉使能 */
#define	LCD_LCDBSTCON_VLCDPD_Msk	(0x1U << LCD_LCDBSTCON_VLCDPD_Pos)
	/* 0：关闭VLCD下拉 */
	/* 1：开启VLCD下拉 */

#define	LCD_LCDBSTCON_VLCDCFG_Pos	4	/* VLCD输出电压配置 */
#define	LCD_LCDBSTCON_VLCDCFG_Msk	(0x7U << LCD_LCDBSTCON_VLCDCFG_Pos)
#define	LCD_LCDBSTCON_VLCDCFG_2P5V	(0x0U << LCD_LCDBSTCON_VLCDCFG_Pos)	/* 000：2.5V */
#define	LCD_LCDBSTCON_VLCDCFG_3P0V	(0x1U << LCD_LCDBSTCON_VLCDCFG_Pos)	/* 001：3.0V */
#define	LCD_LCDBSTCON_VLCDCFG_3P5V	(0x2U << LCD_LCDBSTCON_VLCDCFG_Pos)	/* 010：3.5V */
#define	LCD_LCDBSTCON_VLCDCFG_4P0V	(0x3U << LCD_LCDBSTCON_VLCDCFG_Pos)	/* 011：4.0V */
#define	LCD_LCDBSTCON_VLCDCFG_4P5V	(0x4U << LCD_LCDBSTCON_VLCDCFG_Pos)	/* 100：4.5V */
#define	LCD_LCDBSTCON_VLCDCFG_5P0V	(0x5U << LCD_LCDBSTCON_VLCDCFG_Pos)	/* 101：5.0V */
#define	LCD_LCDBSTCON_VLCDCFG_5P5V	(0x6U << LCD_LCDBSTCON_VLCDCFG_Pos)	/* 110/111：5.5V */

#define	LCD_LCDBSTCON_BOOST_TEN_Pos	1	/* Booster测试信号 */
#define	LCD_LCDBSTCON_BOOST_TEN_Msk	(0x1U << LCD_LCDBSTCON_BOOST_TEN_Pos)
	/* 0：正常模式 */
	/* 1：Booster测试模式 */

#define	LCD_LCDBSTCON_BOOST_EN_Pos	0	/* Booster使能信号 */
#define	LCD_LCDBSTCON_BOOST_EN_Msk	(0x1U << LCD_LCDBSTCON_BOOST_EN_Pos)
	/* 0：关闭VLCD Booster */
	/* 1：开启VLCD Booster */

#define	LCD_COM_EN_COMEN_Pos	0	/* LCD COM0~3输出使能控制 */
#define	LCD_COM_EN_COMEN_Msk	(0xfU << LCD_COM_EN_COMEN_Pos)

#define	LCD_SEG_EN0_SEGENx_Pos	0	/* LCD SEG0~31输出使能控制 */
#define	LCD_SEG_EN0_SEGENx_Msk	(0xffffffffU << LCD_SEG_EN0_SEGENx_Pos)

#define	LCD_SEG_EN1_SEGENx_Pos	0	/* LCD SEG32~43,COM4~7输出使能控制 */
#define	LCD_SEG_EN1_SEGENx_Msk	(0xfffU << LCD_SEG_EN1_SEGENx_Pos)

#define	LCD_COM_EN_COMEN3_Pos	3	/* COM3输出控制@4\6\8COM模式 */
#define	LCD_COM_EN_COMEN3_Msk	(0x1U << LCD_COM_EN_COMEN3_Pos)

#define	LCD_COM_EN_COMEN2_Pos	2	/* COM2输出控制@4\6\8COM模式 */
#define	LCD_COM_EN_COMEN2_Msk	(0x1U << LCD_COM_EN_COMEN2_Pos)

#define	LCD_COM_EN_COMEN1_Pos	1	/* COM1输出控制@4\6\8COM模式 */
#define	LCD_COM_EN_COMEN1_Msk	(0x1U << LCD_COM_EN_COMEN1_Pos)

#define	LCD_COM_EN_COMEN0_Pos	0	/* COM0输出控制@4\6\8COM模式 */
#define	LCD_COM_EN_COMEN0_Msk	(0x1U << LCD_COM_EN_COMEN0_Pos)

#define	LCD_SEG_EN0_SEGEN31_Pos	31	/* SEG31输出控制 */
#define	LCD_SEG_EN0_SEGEN31_Msk	(0x1U << LCD_SEG_EN0_SEGEN31_Pos)

#define	LCD_SEG_EN0_SEGEN30_Pos	30	/* SEG30输出控制 */
#define	LCD_SEG_EN0_SEGEN30_Msk	(0x1U << LCD_SEG_EN0_SEGEN30_Pos)

#define	LCD_SEG_EN0_SEGEN29_Pos	29	/* SEG29输出控制 */
#define	LCD_SEG_EN0_SEGEN29_Msk	(0x1U << LCD_SEG_EN0_SEGEN29_Pos)

#define	LCD_SEG_EN0_SEGEN28_Pos	28	/* SEG28输出控制 */
#define	LCD_SEG_EN0_SEGEN28_Msk	(0x1U << LCD_SEG_EN0_SEGEN28_Pos)

#define	LCD_SEG_EN0_SEGEN27_Pos	27	/* SEG27输出控制 */
#define	LCD_SEG_EN0_SEGEN27_Msk	(0x1U << LCD_SEG_EN0_SEGEN27_Pos)

#define	LCD_SEG_EN0_SEGEN26_Pos	26	/* SEG26输出控制 */
#define	LCD_SEG_EN0_SEGEN26_Msk	(0x1U << LCD_SEG_EN0_SEGEN26_Pos)

#define	LCD_SEG_EN0_SEGEN25_Pos	25	/* SEG25输出控制 */
#define	LCD_SEG_EN0_SEGEN25_Msk	(0x1U << LCD_SEG_EN0_SEGEN25_Pos)

#define	LCD_SEG_EN0_SEGEN24_Pos	24	/* SEG24输出控制 */
#define	LCD_SEG_EN0_SEGEN24_Msk	(0x1U << LCD_SEG_EN0_SEGEN24_Pos)

#define	LCD_SEG_EN0_SEGEN23_Pos	23	/* SEG23输出控制 */
#define	LCD_SEG_EN0_SEGEN23_Msk	(0x1U << LCD_SEG_EN0_SEGEN23_Pos)

#define	LCD_SEG_EN0_SEGEN22_Pos	22	/* SEG22输出控制 */
#define	LCD_SEG_EN0_SEGEN22_Msk	(0x1U << LCD_SEG_EN0_SEGEN22_Pos)

#define	LCD_SEG_EN0_SEGEN21_Pos	21	/* SEG21输出控制 */
#define	LCD_SEG_EN0_SEGEN21_Msk	(0x1U << LCD_SEG_EN0_SEGEN21_Pos)

#define	LCD_SEG_EN0_SEGEN20_Pos	20	/* SEG20输出控制 */
#define	LCD_SEG_EN0_SEGEN20_Msk	(0x1U << LCD_SEG_EN0_SEGEN20_Pos)

#define	LCD_SEG_EN0_SEGEN19_Pos	19	/* SEG19输出控制 */
#define	LCD_SEG_EN0_SEGEN19_Msk	(0x1U << LCD_SEG_EN0_SEGEN19_Pos)

#define	LCD_SEG_EN0_SEGEN18_Pos	18	/* SEG18输出控制 */
#define	LCD_SEG_EN0_SEGEN18_Msk	(0x1U << LCD_SEG_EN0_SEGEN18_Pos)

#define	LCD_SEG_EN0_SEGEN17_Pos	17	/* SEG17输出控制 */
#define	LCD_SEG_EN0_SEGEN17_Msk	(0x1U << LCD_SEG_EN0_SEGEN17_Pos)

#define	LCD_SEG_EN0_SEGEN16_Pos	16	/* SEG16输出控制 */
#define	LCD_SEG_EN0_SEGEN16_Msk	(0x1U << LCD_SEG_EN0_SEGEN16_Pos)

#define	LCD_SEG_EN0_SEGEN15_Pos	15	/* SEG15输出控制 */
#define	LCD_SEG_EN0_SEGEN15_Msk	(0x1U << LCD_SEG_EN0_SEGEN15_Pos)

#define	LCD_SEG_EN0_SEGEN14_Pos	14	/* SEG14输出控制 */
#define	LCD_SEG_EN0_SEGEN14_Msk	(0x1U << LCD_SEG_EN0_SEGEN14_Pos)

#define	LCD_SEG_EN0_SEGEN13_Pos	13	/* SEG13输出控制 */
#define	LCD_SEG_EN0_SEGEN13_Msk	(0x1U << LCD_SEG_EN0_SEGEN13_Pos)

#define	LCD_SEG_EN0_SEGEN12_Pos	12	/* SEG12输出控制 */
#define	LCD_SEG_EN0_SEGEN12_Msk	(0x1U << LCD_SEG_EN0_SEGEN12_Pos)

#define	LCD_SEG_EN0_SEGEN11_Pos	11	/* SEG11输出控制 */
#define	LCD_SEG_EN0_SEGEN11_Msk	(0x1U << LCD_SEG_EN0_SEGEN11_Pos)

#define	LCD_SEG_EN0_SEGEN10_Pos	10	/* SEG10输出控制 */
#define	LCD_SEG_EN0_SEGEN10_Msk	(0x1U << LCD_SEG_EN0_SEGEN10_Pos)

#define	LCD_SEG_EN0_SEGEN9_Pos	9	/* SEG9输出控制 */
#define	LCD_SEG_EN0_SEGEN9_Msk	(0x1U << LCD_SEG_EN0_SEGEN9_Pos)

#define	LCD_SEG_EN0_SEGEN8_Pos	8	/* SEG8输出控制 */
#define	LCD_SEG_EN0_SEGEN8_Msk	(0x1U << LCD_SEG_EN0_SEGEN8_Pos)

#define	LCD_SEG_EN0_SEGEN7_Pos	7	/* SEG7输出控制 */
#define	LCD_SEG_EN0_SEGEN7_Msk	(0x1U << LCD_SEG_EN0_SEGEN7_Pos)

#define	LCD_SEG_EN0_SEGEN6_Pos	6	/* SEG6输出控制 */
#define	LCD_SEG_EN0_SEGEN6_Msk	(0x1U << LCD_SEG_EN0_SEGEN6_Pos)

#define	LCD_SEG_EN0_SEGEN5_Pos	5	/* SEG5输出控制 */
#define	LCD_SEG_EN0_SEGEN5_Msk	(0x1U << LCD_SEG_EN0_SEGEN5_Pos)

#define	LCD_SEG_EN0_SEGEN4_Pos	4	/* SEG4输出控制 */
#define	LCD_SEG_EN0_SEGEN4_Msk	(0x1U << LCD_SEG_EN0_SEGEN4_Pos)

#define	LCD_SEG_EN0_SEGEN3_Pos	3	/* SEG3输出控制 */
#define	LCD_SEG_EN0_SEGEN3_Msk	(0x1U << LCD_SEG_EN0_SEGEN3_Pos)

#define	LCD_SEG_EN0_SEGEN2_Pos	2	/* SEG2输出控制 */
#define	LCD_SEG_EN0_SEGEN2_Msk	(0x1U << LCD_SEG_EN0_SEGEN2_Pos)

#define	LCD_SEG_EN0_SEGEN1_Pos	1	/* SEG1输出控制 */
#define	LCD_SEG_EN0_SEGEN1_Msk	(0x1U << LCD_SEG_EN0_SEGEN1_Pos)

#define	LCD_SEG_EN0_SEGEN0_Pos	0	/* SEG0输出控制 */
#define	LCD_SEG_EN0_SEGEN0_Msk	(0x1U << LCD_SEG_EN0_SEGEN0_Pos)

#define	LCD_SEG_EN1_SEGEN43_Pos	11	/* SEG43输出控制 */
#define	LCD_SEG_EN1_SEGEN43_Msk	(0x1U << LCD_SEG_EN1_SEGEN43_Pos)

#define	LCD_SEG_EN1_SEGEN42_Pos	10	/* SEG42输出控制 */
#define	LCD_SEG_EN1_SEGEN42_Msk	(0x1U << LCD_SEG_EN1_SEGEN42_Pos)

#define	LCD_SEG_EN1_SEGEN41_Pos	9	/* SEG41输出控制 */
#define	LCD_SEG_EN1_SEGEN41_Msk	(0x1U << LCD_SEG_EN1_SEGEN41_Pos)

#define	LCD_SEG_EN1_SEGEN40_Pos	8	/* SEG40输出控制 */
#define	LCD_SEG_EN1_SEGEN40_Msk	(0x1U << LCD_SEG_EN1_SEGEN40_Pos)

#define	LCD_SEG_EN1_SEGEN39_Pos	7	/* SEG39输出控制 */
#define	LCD_SEG_EN1_SEGEN39_Msk	(0x1U << LCD_SEG_EN1_SEGEN39_Pos)

#define	LCD_SEG_EN1_SEGEN38_Pos	6	/* SEG38输出控制 */
#define	LCD_SEG_EN1_SEGEN38_Msk	(0x1U << LCD_SEG_EN1_SEGEN38_Pos)

#define	LCD_SEG_EN1_SEGEN37_Pos	5	/* SEG37输出控制 */
#define	LCD_SEG_EN1_SEGEN37_Msk	(0x1U << LCD_SEG_EN1_SEGEN37_Pos)

#define	LCD_SEG_EN1_SEGEN36_Pos	4	/* SEG36输出控制 */
#define	LCD_SEG_EN1_SEGEN36_Msk	(0x1U << LCD_SEG_EN1_SEGEN36_Pos)

#define	LCD_SEG_EN1_SEGEN35_Pos	3	/* SEG35输出控制 */
#define	LCD_SEG_EN1_SEGEN35_Msk	(0x1U << LCD_SEG_EN1_SEGEN35_Pos)

#define	LCD_SEG_EN1_SEGEN34_Pos	2	/* SEG34输出控制 */
#define	LCD_SEG_EN1_SEGEN34_Msk	(0x1U << LCD_SEG_EN1_SEGEN34_Pos)

#define	LCD_SEG_EN1_SEGEN33_Pos	1	/* SEG33输出控制 */
#define	LCD_SEG_EN1_SEGEN33_Msk	(0x1U << LCD_SEG_EN1_SEGEN33_Pos)

#define	LCD_SEG_EN1_SEGEN32_Pos	0	/* SEG32输出控制 */
#define	LCD_SEG_EN1_SEGEN32_Msk	(0x1U << LCD_SEG_EN1_SEGEN32_Pos)

#define	LCD_SEG_EN1_COM5ENo6COM_Pos	11	/* COM5@6COM输出控制，复用SEGEN43 */
#define	LCD_SEG_EN1_COM5ENo6COM_Msk	(0x1U << LCD_SEG_EN1_COM5ENo6COM_Pos)

#define	LCD_SEG_EN1_COM4ENo6COM_Pos	10	/* COM4@6COM输出控制，复用SEGEN42 */
#define	LCD_SEG_EN1_COM4ENo6COM_Msk	(0x1U << LCD_SEG_EN1_COM4ENo6COM_Pos)

#define	LCD_SEG_EN1_COM7ENo8COM_Pos	11	/* COM7@8COM输出控制，复用SEGEN43 */
#define	LCD_SEG_EN1_COM7ENo8COM_Msk	(0x1U << LCD_SEG_EN1_COM7ENo8COM_Pos)

#define	LCD_SEG_EN1_COM6ENo8COM_Pos	10	/* COM6@8COM输出控制，复用SEGEN42 */
#define	LCD_SEG_EN1_COM6ENo8COM_Msk	(0x1U << LCD_SEG_EN1_COM6ENo8COM_Pos)

#define	LCD_SEG_EN1_COM5ENo8COM_Pos	9	/* COM5@8COM输出控制，复用SEGEN41 */
#define	LCD_SEG_EN1_COM5ENo8COM_Msk	(0x1U << LCD_SEG_EN1_COM5ENo8COM_Pos)

#define	LCD_SEG_EN1_COM4ENo8COM_Pos	8	/* COM4@8COM输出控制，复用SEGEN40 */
#define	LCD_SEG_EN1_COM4ENo8COM_Msk	(0x1U << LCD_SEG_EN1_COM4ENo8COM_Pos)
//Macro_End

/* Exported functions --------------------------------------------------------*/ 
extern void LCD_Deinit(void);

/* 防极化 相关函数 */
extern void LCD_DISPCTRL_ANTIPOLAR_Setable(FunState NewState);
extern FunState LCD_DISPCTRL_ANTIPOLAR_Getable(void);

/* LCD显示使能 相关函数 */
extern void LCD_DISPCTRL_LCDEN_Setable(FunState NewState);
extern FunState LCD_DISPCTRL_LCDEN_Getable(void);

/* 显示闪烁使能 相关函数 */
extern void LCD_DISPCTRL_FLICK_Setable(FunState NewState);
extern FunState LCD_DISPCTRL_FLICK_Getable(void);

/* 测试模式使能，仅在DISPMD=1的情况下有效 相关函数 */
extern void LCD_DISPCTRL_TEST_Setable(FunState NewState);
extern FunState LCD_DISPCTRL_TEST_Getable(void);

/* 测试模式选择 相关函数 */
extern void LCD_DISPCTRL_DISPMD_Setable(FunState NewState);
extern FunState LCD_DISPCTRL_DISPMD_Getable(void);

/* LCD测试控制位，仅在测试模式下有效 相关函数 */
extern void LCD_LCDTEST_LCCTRL_Set(uint32_t SetValue);
extern uint32_t LCD_LCDTEST_LCCTRL_Get(void);

/* 测试模式使能 相关函数 */
extern void LCD_LCDTEST_TESTEN_Setable(FunState NewState);
extern FunState LCD_LCDTEST_TESTEN_Getable(void);

/* 显示频率控制寄存器 相关函数 */
extern void LCD_DF_Write(uint32_t SetValue);
extern uint32_t LCD_DF_Read(void);

/* 闪烁显示点亮时间寄存器 相关函数 */
extern void LCD_TON_Write(uint32_t SetValue);
extern uint32_t LCD_TON_Read(void);

/* 闪烁显示熄灭时间寄存器 相关函数 */
extern void LCD_TOFF_Write(uint32_t SetValue);
extern uint32_t LCD_TOFF_Read(void);

/* 显示点亮中断使能 相关函数 */
extern void LCD_DISPIE_DONIE_Setable(FunState NewState);
extern FunState LCD_DISPIE_DONIE_Getable(void);

/* 显示熄灭中断使能 相关函数 */
extern void LCD_DISPIE_DOFFIE_Setable(FunState NewState);
extern FunState LCD_DISPIE_DOFFIE_Getable(void);

/* 显示点亮中断标志 相关函数 */
extern void LCD_DISPIF_DONIF_Clr(void);
extern FlagStatus LCD_DISPIF_DONIF_Chk(void);

/* 显示熄灭中断标志 相关函数 */
extern void LCD_DISPIF_DOFFIF_Clr(void);
extern FlagStatus LCD_DISPIF_DOFFIF_Chk(void);

/* 偏置类型控制 相关函数 */
extern void LCD_LCDSET_BIASMD_Set(uint32_t SetValue);
extern uint32_t LCD_LCDSET_BIASMD_Get(void);

/* 驱动波形选择 相关函数 */
extern void LCD_LCDSET_WFT_Set(uint32_t SetValue);
extern uint32_t LCD_LCDSET_WFT_Get(void);

/* COM数量选择 相关函数 */
extern void LCD_LCDSET_LMUX_Set(uint32_t SetValue);
extern uint32_t LCD_LCDSET_LMUX_Get(void);

/* SC频率选择 相关函数 */
extern void LCD_LCDDRV_SCFSEL_Set(uint32_t SetValue);
extern uint32_t LCD_LCDDRV_SCFSEL_Get(void);

/* 片外电容驱动模式下，驱动方式控制 相关函数 */
extern void LCD_LCDDRV_SC_CTRL_Set(uint32_t SetValue);
extern uint32_t LCD_LCDDRV_SC_CTRL_Get(void);

/* 驱动电流大小 相关函数 */
extern void LCD_LCDDRV_IC_CTRL_Set(uint32_t SetValue);
extern uint32_t LCD_LCDDRV_IC_CTRL_Get(void);

/* 驱动模式选择 相关函数 */
extern void LCD_LCDDRV_ENMODE_Set(uint32_t SetValue);
extern uint32_t LCD_LCDDRV_ENMODE_Get(void);

/* 显示灰度设置 相关函数 */
extern void LCD_LCDBIAS_Write(uint32_t SetValue);
extern uint32_t LCD_LCDBIAS_Read(void);

/* BOOST环振频率配置信号,值越大频率越高，驱动越强，功耗越高 相关函数 */
extern void LCD_LCDBSTCON_OSC_CFG_Set(uint32_t SetValue);
extern uint32_t LCD_LCDBSTCON_OSC_CFG_Get(void);

/* 基准源Buffer使能信号 相关函数 */
extern void LCD_LCDBSTCON_BUFENB_Setable(FunState NewState);
extern FunState LCD_LCDBSTCON_BUFENB_Getable(void);

/* 基准源Buffer Bypass 相关函数 */
extern void LCD_LCDBSTCON_BUFBYP_Setable(FunState NewState);
extern FunState LCD_LCDBSTCON_BUFBYP_Getable(void);

/* VLCD下拉使能 相关函数 */
extern void LCD_LCDBSTCON_VLCDPD_Setable(FunState NewState);
extern FunState LCD_LCDBSTCON_VLCDPD_Getable(void);

/* VLCD输出电压配置 相关函数 */
extern void LCD_LCDBSTCON_VLCDCFG_Set(uint32_t SetValue);
extern uint32_t LCD_LCDBSTCON_VLCDCFG_Get(void);

/* Booster测试信号 相关函数 */
extern void LCD_LCDBSTCON_BOOST_TEN_Setable(FunState NewState);
extern FunState LCD_LCDBSTCON_BOOST_TEN_Getable(void);

/* Booster使能信号 相关函数 */
extern void LCD_LCDBSTCON_BOOST_EN_Setable(FunState NewState);
extern FunState LCD_LCDBSTCON_BOOST_EN_Getable(void);

/* LCD COM0~3输出使能控制 相关函数 */
extern void LCD_COM_EN_Write(uint32_t SetValue);
extern uint32_t LCD_COM_EN_Read(void);

/* LCD SEG0~31输出使能控制 相关函数 */
extern void LCD_SEG_EN0_Write(uint32_t SetValue);
extern uint32_t LCD_SEG_EN0_Read(void);

/* LCD SEG32~43,COM4~7输出使能控制 相关函数 */
extern void LCD_SEG_EN1_Write(uint32_t SetValue);
extern uint32_t LCD_SEG_EN1_Read(void);
//Announce_End



/*DISPDATAx显示数据寄存器刷新*/
extern void LCD_DISPDATAx_Refresh(uint32_t* DispBuf);

/*lcd boost功能配置*/
extern void LCD_Boost_Init(LCD_Boost_InitTypeDef* para);
	
/*LCD显示功能配置*/
extern void LCD_Init(LCD_InitTypeDef* para);


#ifdef __cplusplus
}
#endif

#endif /* __FM33A0XX_LCD_H */



/************************ (C) COPYRIGHT FMSHelectronics *****END OF FILE****/




