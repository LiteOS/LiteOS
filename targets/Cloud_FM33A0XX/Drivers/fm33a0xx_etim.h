/**
  ******************************************************************************
  * @file    fm33a0xx_etimer.h
  * @author  FM33a0xx Application Team
  * @version V2.0.1
  * @date    26-2-2018
  * @brief   This file contains all the functions prototypes for the ETIM firmware library.  
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FM33A0xx_ETIMER_H
#define __FM33A0xx_ETIMER_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "FM33A0XX.h"
	 

typedef struct
{
	uint32_t	SIG1SEL;		/* 内部信号1源选择（在计数模式下计数源仅由此选择，捕捉模式下计数源） */
	uint32_t	SIG2SEL;		/* 内部信号2源选择(捕捉源) */
	uint32_t	GRP1SEL;		/* GROUP1 信号选择控制 */
	uint32_t	GRP2SEL;		/* GROUP2 信号选择控制 */
	uint32_t	PRESCALE1;		/* ETx预分频寄存器1 */
	uint32_t	PRESCALE2;		/* ETx预分频寄存器2 */

}ETIM_SRCInitType;

typedef struct
{
	FunState	EXFLT;		/* 引脚输入数字滤波使能 */
	uint32_t	MOD;		/* 工作模式选择 */
	FunState	CASEN;		/* 扩展定时器级联使能 */
	uint32_t	EDGESEL;	/* 计数模式采沿方式选择（计数时钟选择mcu_clk时该位无效，总是采用mcu_clk时钟上升沿计数） */
	
	FunState	PWM;		/* PWM输出控制 */
	
	uint32_t	CAPMOD;		/* 捕捉模式控制 */
	FunState	CAPCLR;		/* 带清零捕捉模式控制 */
	FunState	CAPONCE;	/* 单次捕捉控制 */
	uint32_t	CAPEDGE;	/* 捕捉沿选择 */
	
	uint32_t	INITVALUE;	/* ETx初值寄存器 */
	uint32_t	CMP;		/* ETx比较寄存器 */
	
	FunState	CMPIE;		/* 扩展定时器比较中断使能 */
	FunState	CAPIE;		/* 扩展定时器捕捉中断使能 */
	FunState	OVIE;		/* 扩展定时器溢出中断使能 */
	
	FunState	CEN;		/* 启动控制 */

}ETIM_CTRLInitType;

typedef struct
{
	ETIM_SRCInitType	sig_src_para;//信号源参数
	ETIM_CTRLInitType	ctrl_para;//控制类参数
	
}ETIM_InitTypeDef;


//Macro_START
//2018-04-02-13-21-03
//FM33A0XX_Driver_Gen_V1.4

#define	ETIMx_ETxCR_EXFLT_Pos	9	/* 引脚输入数字滤波使能 */
#define	ETIMx_ETxCR_EXFLT_Msk	(0x1U << ETIMx_ETxCR_EXFLT_Pos)
	/* 0 = 关闭引脚输入信号数字滤波 */
	/* 1 = 打开引脚输入信号数字滤波 */

#define	ETIMx_ETxCR_PWM_Pos	8	/* PWM输出控制 */
#define	ETIMx_ETxCR_PWM_Msk	(0x1U << ETIMx_ETxCR_PWM_Pos)
	/* 0 = PWM输出禁止 */
	/* 1 = PWM输出使能 */

#define	ETIMx_ETxCR_CEN_Pos	7	/* 启动控制 */
#define	ETIMx_ETxCR_CEN_Msk	(0x1U << ETIMx_ETxCR_CEN_Pos)
	/* 0 = 停止计数器计数 */
	/* 1 = 启动定时器 */

#define	ETIMx_ETxCR_MOD_Pos	6	/* 工作模式选择 */
#define	ETIMx_ETxCR_MOD_Msk	(0x1U << ETIMx_ETxCR_MOD_Pos)
#define	ETIMx_ETxCR_MOD_COUNTER	(0x0U << ETIMx_ETxCR_MOD_Pos)	/* 0 = 定时/计数模式 */
#define	ETIMx_ETxCR_MOD_CAPTURE	(0x1U << ETIMx_ETxCR_MOD_Pos)	/* 1 = 捕捉模式 */

#define	ETIMx_ETxCR_CASEN_Pos	5	/* 扩展定时器级联使能 */
#define	ETIMx_ETxCR_CASEN_Msk	(0x1U << ETIMx_ETxCR_CASEN_Pos)
	/* 0 = 16bit定时器独立工作 */
	/* 1 = ET1（ET3）和ET2（ET4）级联成32bit定时器 */

#define	ETIMx_ETxCR_EDGESEL_Pos	4	/* 计数模式采沿方式选择（计数时钟选择mcu_clk时该位无效，总是采用mcu_clk时钟上升沿计数） */
#define	ETIMx_ETxCR_EDGESEL_Msk	(0x1U << ETIMx_ETxCR_EDGESEL_Pos)
#define	ETIMx_ETxCR_EDGESEL_RISING	(0x0U << ETIMx_ETxCR_EDGESEL_Pos)	/* 0 = 计数模式采上升沿 */
#define	ETIMx_ETxCR_EDGESEL_FALLING	(0x1U << ETIMx_ETxCR_EDGESEL_Pos)	/* 1 = 计数模式采下降沿 */

#define	ETIMx_ETxCR_CAPMOD_Pos	3	/* 捕捉模式控制 */
#define	ETIMx_ETxCR_CAPMOD_Msk	(0x1U << ETIMx_ETxCR_CAPMOD_Pos)
#define	ETIMx_ETxCR_CAPMOD_PERIOD	(0x0U << ETIMx_ETxCR_CAPMOD_Pos)	/* 0 = 脉冲周期捕捉 */
#define	ETIMx_ETxCR_CAPMOD_PULSE	(0x1U << ETIMx_ETxCR_CAPMOD_Pos)	/* 1 = 脉宽捕捉 */

#define	ETIMx_ETxCR_CAPCLR_Pos	2	/* 带清零捕捉模式控制 */
#define	ETIMx_ETxCR_CAPCLR_Msk	(0x1U << ETIMx_ETxCR_CAPCLR_Pos)
	/* 0 = 捕捉不清零，计数器一直自由计数 */
	/* 1 = 事件触发捕捉：使能后计数器保持0，捕捉到第一个有效沿之后timer才开始计数 */

#define	ETIMx_ETxCR_CAPONCE_Pos	1	/* 单次捕捉控制 */
#define	ETIMx_ETxCR_CAPONCE_Msk	(0x1U << ETIMx_ETxCR_CAPONCE_Pos)
	/* 0 = 连续捕捉 */
	/* 1 = 单次捕捉有效，在捕捉到一次脉冲周期后计数器停止，若需要再次捕捉需重新启动 */

#define	ETIMx_ETxCR_CAPEDGE_Pos	0	/* 捕捉沿选择 */
#define	ETIMx_ETxCR_CAPEDGE_Msk	(0x1U << ETIMx_ETxCR_CAPEDGE_Pos)
#define	ETIMx_ETxCR_CAPEDGE_RISING	(0x0U << ETIMx_ETxCR_CAPEDGE_Pos)	/* 0 = 周期捕捉模式时上沿捕捉 */
#define	ETIMx_ETxCR_CAPEDGE_FALLING	(0x1U << ETIMx_ETxCR_CAPEDGE_Pos)	/* 1 = 周期捕捉模式时下沿捕捉 */

#define	ETIMx_ETxINSEL_SIG2SEL_Pos	7	/* 内部信号2源选择(捕捉源) */
#define	ETIMx_ETxINSEL_SIG2SEL_Msk	(0x1U << ETIMx_ETxINSEL_SIG2SEL_Pos)
#define	ETIMx_ETxINSEL_SIG2SEL_GROUP2	(0x0U << ETIMx_ETxINSEL_SIG2SEL_Pos)	/* 0 = 扩展定时器3的内部信号2选择Group2 */
#define	ETIMx_ETxINSEL_SIG2SEL_GROUP1	(0x1U << ETIMx_ETxINSEL_SIG2SEL_Pos)	/* 1 = 扩展定时器3的内部信号2选择Group1 */

#define	ETIMx_ETxINSEL_SIG1SEL_Pos	6	/* 内部信号1源选择（在计数模式下计数源仅由此选择，捕捉模式下计数源） */
#define	ETIMx_ETxINSEL_SIG1SEL_Msk	(0x1U << ETIMx_ETxINSEL_SIG1SEL_Pos)
#define	ETIMx_ETxINSEL_SIG1SEL_GROUP1	(0x0U << ETIMx_ETxINSEL_SIG1SEL_Pos)	/* 0 = 扩展定时器3的内部信号1选择Group1 */
#define	ETIMx_ETxINSEL_SIG1SEL_GROUP2	(0x1U << ETIMx_ETxINSEL_SIG1SEL_Pos)	/* 1 = 扩展定时器3的内部信号1选择Group2 */

#define	ETIMx_ETxINSEL_GRP2SEL_Pos	2	/* GROUP2 信号选择控制 */
#define	ETIMx_ETxINSEL_GRP2SEL_Msk	(0x7U << ETIMx_ETxINSEL_GRP2SEL_Pos)
#define	ETIMx_ETxINSEL_GRP2SEL_ET1_UART0_RX	(0x0U << ETIMx_ETxINSEL_GRP2SEL_Pos)	/* ET1 000 = UART0_RX */
#define	ETIMx_ETxINSEL_GRP2SEL_ET1_UART1_RX	(0x1U << ETIMx_ETxINSEL_GRP2SEL_Pos)	/* ET1 001 = UART1_RX */
#define	ETIMx_ETxINSEL_GRP2SEL_ET1_XTLF	(0x2U << ETIMx_ETxINSEL_GRP2SEL_Pos)	/* ET1 010 = XTLF */
#define	ETIMx_ETxINSEL_GRP2SEL_ET1_ET1_IN1	(0x3U << ETIMx_ETxINSEL_GRP2SEL_Pos)	/* ET1 011 = ET1_IN1 */
#define	ETIMx_ETxINSEL_GRP2SEL_ET1_ET1_IN2	(0x4U << ETIMx_ETxINSEL_GRP2SEL_Pos)	/* ET1 100 = ET1_IN2 */
#define	ETIMx_ETxINSEL_GRP2SEL_ET1_CMP1O	(0x5U << ETIMx_ETxINSEL_GRP2SEL_Pos)	/* ET1 101 = CMP1O（比较器1输出） */
#define	ETIMx_ETxINSEL_GRP2SEL_ET1_CMP2O	(0x6U << ETIMx_ETxINSEL_GRP2SEL_Pos)	/* ET1 110 = CMP2O（比较器2输出） */
#define	ETIMx_ETxINSEL_GRP2SEL_ET1_LPTO	(0x7U << ETIMx_ETxINSEL_GRP2SEL_Pos)	/* ET1 111 = LPTO */
#define	ETIMx_ETxINSEL_GRP2SEL_ET2_UART2_RX	(0x0U << ETIMx_ETxINSEL_GRP2SEL_Pos)	/* ET2 000 = UART2_RX */
#define	ETIMx_ETxINSEL_GRP2SEL_ET2_UART3_RX	(0x1U << ETIMx_ETxINSEL_GRP2SEL_Pos)	/* ET2 001 = UART3_RX */
#define	ETIMx_ETxINSEL_GRP2SEL_ET2_XTLF	(0x2U << ETIMx_ETxINSEL_GRP2SEL_Pos)	/* ET2 010 = XTLF */
#define	ETIMx_ETxINSEL_GRP2SEL_ET2_ET2_IN1	(0x3U << ETIMx_ETxINSEL_GRP2SEL_Pos)	/* ET2 011 = ET2_IN1 */
#define	ETIMx_ETxINSEL_GRP2SEL_ET2_ET2_IN2	(0x4U << ETIMx_ETxINSEL_GRP2SEL_Pos)	/* ET2 100 = ET2_IN2 */
#define	ETIMx_ETxINSEL_GRP2SEL_ET2_CMP1O	(0x5U << ETIMx_ETxINSEL_GRP2SEL_Pos)	/* ET2 101 = CMP1O（比较器1输出） */
#define	ETIMx_ETxINSEL_GRP2SEL_ET2_CMP2O	(0x6U << ETIMx_ETxINSEL_GRP2SEL_Pos)	/* ET2 110 = CMP2O（比较器2输出） */
#define	ETIMx_ETxINSEL_GRP2SEL_ET2_LPTO	(0x7U << ETIMx_ETxINSEL_GRP2SEL_Pos)	/* ET2 111 = LPTO */
#define	ETIMx_ETxINSEL_GRP2SEL_ET3_ET3_IN1	(0x0U << ETIMx_ETxINSEL_GRP2SEL_Pos)	/* ET3 000 = ET3_IN1 */
#define	ETIMx_ETxINSEL_GRP2SEL_ET3_XTLF	(0x1U << ETIMx_ETxINSEL_GRP2SEL_Pos)	/* ET3 001 = XTLF  */
#define	ETIMx_ETxINSEL_GRP2SEL_ET3_UART4_RX	(0x2U << ETIMx_ETxINSEL_GRP2SEL_Pos)	/* ET3 010 = UART4_RX */
#define	ETIMx_ETxINSEL_GRP2SEL_ET3_UART5_RX	(0x3U << ETIMx_ETxINSEL_GRP2SEL_Pos)	/* ET3 011 = UART5_RX */
#define	ETIMx_ETxINSEL_GRP2SEL_ET3_RTCSEC	(0x4U << ETIMx_ETxINSEL_GRP2SEL_Pos)	/* ET3 100 = RTCSEC */
#define	ETIMx_ETxINSEL_GRP2SEL_ET4_ET4_IN1	(0x0U << ETIMx_ETxINSEL_GRP2SEL_Pos)	/* ET4 000 = ET4_IN1 */
#define	ETIMx_ETxINSEL_GRP2SEL_ET4_XTLF	(0x1U << ETIMx_ETxINSEL_GRP2SEL_Pos)	/* ET4 001 = XTLF */
#define	ETIMx_ETxINSEL_GRP2SEL_ET4_UART_RX2	(0x2U << ETIMx_ETxINSEL_GRP2SEL_Pos)	/* ET4 010 = UART_RX2 */
#define	ETIMx_ETxINSEL_GRP2SEL_ET4_UART_RX0	(0x3U << ETIMx_ETxINSEL_GRP2SEL_Pos)	/* ET4 011 = UART_RX0 */
#define	ETIMx_ETxINSEL_GRP2SEL_ET4_CMP1O	(0x4U << ETIMx_ETxINSEL_GRP2SEL_Pos)	/* ET4 100 = CMP1O（比较器1输出） */
#define	ETIMx_ETxINSEL_GRP2SEL_ET4_CMP2O	(0x5U << ETIMx_ETxINSEL_GRP2SEL_Pos)	/* ET4 101 = CMP2O（比较器2输出） */
#define	ETIMx_ETxINSEL_GRP2SEL_ET4_RTCSEC	(0x6U << ETIMx_ETxINSEL_GRP2SEL_Pos)	/* ET4 110= RTCSEC */
#define	ETIMx_ETxINSEL_GRP2SEL_ET4_LPTO	(0x7U << ETIMx_ETxINSEL_GRP2SEL_Pos)	/* ET4 111= LPTO  */

#define	ETIMx_ETxINSEL_GRP1SEL_Pos	0	/* GROUP1 信号选择控制 */
#define	ETIMx_ETxINSEL_GRP1SEL_Msk	(0x3U << ETIMx_ETxINSEL_GRP1SEL_Pos)
#define	ETIMx_ETxINSEL_GRP1SEL_ET1_APBCLK	(0x0U << ETIMx_ETxINSEL_GRP1SEL_Pos)	/* ET1 00 = APBCLK */
#define	ETIMx_ETxINSEL_GRP1SEL_ET1_XTLF	(0x1U << ETIMx_ETxINSEL_GRP1SEL_Pos)	/* ET1 01 = XTLF */
#define	ETIMx_ETxINSEL_GRP1SEL_ET1_RCLP	(0x2U << ETIMx_ETxINSEL_GRP1SEL_Pos)	/* ET1 10 = RCLP */
#define	ETIMx_ETxINSEL_GRP1SEL_ET1_ET1_IN0	(0x3U << ETIMx_ETxINSEL_GRP1SEL_Pos)	/* ET1 11 = ET1_IN0 */
#define	ETIMx_ETxINSEL_GRP1SEL_ET2_APBCLK	(0x0U << ETIMx_ETxINSEL_GRP1SEL_Pos)	/* ET2 00 = APBCLK */
#define	ETIMx_ETxINSEL_GRP1SEL_ET2_XTLF	(0x1U << ETIMx_ETxINSEL_GRP1SEL_Pos)	/* ET2 01 = XTLF */
#define	ETIMx_ETxINSEL_GRP1SEL_ET2_RCLP	(0x2U << ETIMx_ETxINSEL_GRP1SEL_Pos)	/* ET2 10 = RCLP */
#define	ETIMx_ETxINSEL_GRP1SEL_ET2_ET2_IN0	(0x3U << ETIMx_ETxINSEL_GRP1SEL_Pos)	/* ET2 11 = ET2_IN0 */
#define	ETIMx_ETxINSEL_GRP1SEL_ET3_APBCLK	(0x0U << ETIMx_ETxINSEL_GRP1SEL_Pos)	/* ET3 00 = APBCLK */
#define	ETIMx_ETxINSEL_GRP1SEL_ET3_ET3_IN0	(0x1U << ETIMx_ETxINSEL_GRP1SEL_Pos)	/* ET3 01 = ET3_IN0 */
#define	ETIMx_ETxINSEL_GRP1SEL_ET3_RTCSEC	(0x2U << ETIMx_ETxINSEL_GRP1SEL_Pos)	/* ET3 10 = RTCSEC */
#define	ETIMx_ETxINSEL_GRP1SEL_ET3_RCLP	(0x3U << ETIMx_ETxINSEL_GRP1SEL_Pos)	/* ET3 11 = RCLP */
#define	ETIMx_ETxINSEL_GRP1SEL_ET4_APBCLK	(0x0U << ETIMx_ETxINSEL_GRP1SEL_Pos)	/* ET4 00 = APBCLK */
#define	ETIMx_ETxINSEL_GRP1SEL_ET4_ET4_IN0	(0x1U << ETIMx_ETxINSEL_GRP1SEL_Pos)	/* ET4 01 = ET4_IN0 */
#define	ETIMx_ETxINSEL_GRP1SEL_ET4_RTC64HZ	(0x2U << ETIMx_ETxINSEL_GRP1SEL_Pos)	/* ET4 10 = RTC64HZ */
#define	ETIMx_ETxINSEL_GRP1SEL_ET4_LPTO	(0x3U << ETIMx_ETxINSEL_GRP1SEL_Pos)	/* ET4 11 = LPTO */

#define	ETIMx_ETxPESCALE1_PRESCALE1_Pos	0	/* ETx预分频寄存器1 */
#define	ETIMx_ETxPESCALE1_PRESCALE1_Msk	(0xffU << ETIMx_ETxPESCALE1_PRESCALE1_Pos)

#define	ETIMx_ETxPESCALE2_PRESCALE2_Pos	0	/* ETx预分频寄存器2 */
#define	ETIMx_ETxPESCALE2_PRESCALE2_Msk	(0xffU << ETIMx_ETxPESCALE2_PRESCALE2_Pos)

#define	ETIMx_ETxIVR_INITVALUE_Pos	0	/* ETx初值寄存器 */
#define	ETIMx_ETxIVR_INITVALUE_Msk	(0xffffU << ETIMx_ETxIVR_INITVALUE_Pos)

#define	ETIMx_ETxCMP_CMP_Pos	0	/* ETx比较寄存器 */
#define	ETIMx_ETxCMP_CMP_Msk	(0xffffU << ETIMx_ETxCMP_CMP_Pos)

#define	ETIMx_ETxIE_CMPIE_Pos	2	/* 扩展定时器比较中断使能 */
#define	ETIMx_ETxIE_CMPIE_Msk	(0x1U << ETIMx_ETxIE_CMPIE_Pos)
	/* 0 = 禁止 */
	/* 1 = 使能 */

#define	ETIMx_ETxIE_CAPIE_Pos	1	/* 扩展定时器捕捉中断使能 */
#define	ETIMx_ETxIE_CAPIE_Msk	(0x1U << ETIMx_ETxIE_CAPIE_Pos)
	/* 0 = 禁止 */
	/* 1 = 使能 */

#define	ETIMx_ETxIE_OVIE_Pos	0	/* 扩展定时器溢出中断使能 */
#define	ETIMx_ETxIE_OVIE_Msk	(0x1U << ETIMx_ETxIE_OVIE_Pos)
	/* 1 = 使能 */
	/* 0 = 禁止 */

#define	ETIMx_ETxIF_CMPIF_Pos	3	/* 比较状态中断标志 */
#define	ETIMx_ETxIF_CMPIF_Msk	(0x1U << ETIMx_ETxIF_CMPIF_Pos)

#define	ETIMx_ETxIF_EDGESTA_Pos	2	/* 捕捉沿状态标志 */
#define	ETIMx_ETxIF_EDGESTA_Msk	(0x1U << ETIMx_ETxIF_EDGESTA_Pos)

#define	ETIMx_ETxIF_CAPIF_Pos	1	/* 扩展定时器捕捉产生中断标志 */
#define	ETIMx_ETxIF_CAPIF_Msk	(0x1U << ETIMx_ETxIF_CAPIF_Pos)

#define	ETIMx_ETxIF_OVIF_Pos	0	/* 扩展定时器溢出中断标志 */
#define	ETIMx_ETxIF_OVIF_Msk	(0x1U << ETIMx_ETxIF_OVIF_Pos)
//Macro_End

/* Exported functions --------------------------------------------------------*/ 
extern void ETIMx_Deinit(ETIMx_Type* ETIMx);

/* 引脚输入数字滤波使能 相关函数 */
extern void ETIMx_ETxCR_EXFLT_Setable(ETIMx_Type* ETIMx, FunState NewState);
extern FunState ETIMx_ETxCR_EXFLT_Getable(ETIMx_Type* ETIMx);

/* PWM输出控制 相关函数 */
extern void ETIMx_ETxCR_PWM_Setable(ETIMx_Type* ETIMx, FunState NewState);
extern FunState ETIMx_ETxCR_PWM_Getable(ETIMx_Type* ETIMx);

/* 启动控制 相关函数 */
extern void ETIMx_ETxCR_CEN_Setable(ETIMx_Type* ETIMx, FunState NewState);
extern FunState ETIMx_ETxCR_CEN_Getable(ETIMx_Type* ETIMx);

/* 工作模式选择 相关函数 */
extern void ETIMx_ETxCR_MOD_Set(ETIMx_Type* ETIMx, uint32_t SetValue);
extern uint32_t ETIMx_ETxCR_MOD_Get(ETIMx_Type* ETIMx);

/* 扩展定时器级联使能 相关函数 */
extern void ETIMx_ETxCR_CASEN_Setable(ETIMx_Type* ETIMx, FunState NewState);
extern FunState ETIMx_ETxCR_CASEN_Getable(ETIMx_Type* ETIMx);

/* 计数模式采沿方式选择（计数时钟选择mcu_clk时该位无效，总是采用mcu_clk时钟上升沿计数） 相关函数 */
extern void ETIMx_ETxCR_EDGESEL_Set(ETIMx_Type* ETIMx, uint32_t SetValue);
extern uint32_t ETIMx_ETxCR_EDGESEL_Get(ETIMx_Type* ETIMx);

/* 捕捉模式控制 相关函数 */
extern void ETIMx_ETxCR_CAPMOD_Set(ETIMx_Type* ETIMx, uint32_t SetValue);
extern uint32_t ETIMx_ETxCR_CAPMOD_Get(ETIMx_Type* ETIMx);

/* 带清零捕捉模式控制 相关函数 */
extern void ETIMx_ETxCR_CAPCLR_Setable(ETIMx_Type* ETIMx, FunState NewState);
extern FunState ETIMx_ETxCR_CAPCLR_Getable(ETIMx_Type* ETIMx);

/* 单次捕捉控制 相关函数 */
extern void ETIMx_ETxCR_CAPONCE_Setable(ETIMx_Type* ETIMx, FunState NewState);
extern FunState ETIMx_ETxCR_CAPONCE_Getable(ETIMx_Type* ETIMx);

/* 捕捉沿选择 相关函数 */
extern void ETIMx_ETxCR_CAPEDGE_Set(ETIMx_Type* ETIMx, uint32_t SetValue);
extern uint32_t ETIMx_ETxCR_CAPEDGE_Get(ETIMx_Type* ETIMx);

/* 内部信号2源选择(捕捉源) 相关函数 */
extern void ETIMx_ETxINSEL_SIG2SEL_Set(ETIMx_Type* ETIMx, uint32_t SetValue);
extern uint32_t ETIMx_ETxINSEL_SIG2SEL_Get(ETIMx_Type* ETIMx);

/* 内部信号1源选择（在计数模式下计数源仅由此选择，捕捉模式下计数源） 相关函数 */
extern void ETIMx_ETxINSEL_SIG1SEL_Set(ETIMx_Type* ETIMx, uint32_t SetValue);
extern uint32_t ETIMx_ETxINSEL_SIG1SEL_Get(ETIMx_Type* ETIMx);

/* GROUP2 信号选择控制 相关函数 */
extern void ETIMx_ETxINSEL_GRP2SEL_Set(ETIMx_Type* ETIMx, uint32_t SetValue);
extern uint32_t ETIMx_ETxINSEL_GRP2SEL_Get(ETIMx_Type* ETIMx);

/* GROUP1 信号选择控制 相关函数 */
extern void ETIMx_ETxINSEL_GRP1SEL_Set(ETIMx_Type* ETIMx, uint32_t SetValue);
extern uint32_t ETIMx_ETxINSEL_GRP1SEL_Get(ETIMx_Type* ETIMx);

/* ETx预分频寄存器1 相关函数 */
extern void ETIMx_ETxPESCALE1_Write(ETIMx_Type* ETIMx, uint32_t SetValue);
extern uint32_t ETIMx_ETxPESCALE1_Read(ETIMx_Type* ETIMx);

/* ETx预分频寄存器2 相关函数 */
extern void ETIMx_ETxPESCALE2_Write(ETIMx_Type* ETIMx, uint32_t SetValue);
extern uint32_t ETIMx_ETxPESCALE2_Read(ETIMx_Type* ETIMx);

/* ETx初值寄存器 相关函数 */
extern void ETIMx_ETxIVR_Write(ETIMx_Type* ETIMx, uint32_t SetValue);
extern uint32_t ETIMx_ETxIVR_Read(ETIMx_Type* ETIMx);

/* ETx比较寄存器 相关函数 */
extern void ETIMx_ETxCMP_Write(ETIMx_Type* ETIMx, uint32_t SetValue);
extern uint32_t ETIMx_ETxCMP_Read(ETIMx_Type* ETIMx);

/* 扩展定时器比较中断使能 相关函数 */
extern void ETIMx_ETxIE_CMPIE_Setable(ETIMx_Type* ETIMx, FunState NewState);
extern FunState ETIMx_ETxIE_CMPIE_Getable(ETIMx_Type* ETIMx);

/* 扩展定时器捕捉中断使能 相关函数 */
extern void ETIMx_ETxIE_CAPIE_Setable(ETIMx_Type* ETIMx, FunState NewState);
extern FunState ETIMx_ETxIE_CAPIE_Getable(ETIMx_Type* ETIMx);

/* 扩展定时器溢出中断使能 相关函数 */
extern void ETIMx_ETxIE_OVIE_Setable(ETIMx_Type* ETIMx, FunState NewState);
extern FunState ETIMx_ETxIE_OVIE_Getable(ETIMx_Type* ETIMx);

/* 比较状态中断标志 相关函数 */
extern void ETIMx_ETxIF_CMPIF_Clr(ETIMx_Type* ETIMx);
extern FlagStatus ETIMx_ETxIF_CMPIF_Chk(ETIMx_Type* ETIMx);

/* 捕捉沿状态标志 相关函数 */
extern FlagStatus ETIMx_ETxIF_EDGESTA_Chk(ETIMx_Type* ETIMx);

/* 扩展定时器捕捉产生中断标志 相关函数 */
extern void ETIMx_ETxIF_CAPIF_Clr(ETIMx_Type* ETIMx);
extern FlagStatus ETIMx_ETxIF_CAPIF_Chk(ETIMx_Type* ETIMx);

/* 扩展定时器溢出中断标志 相关函数 */
extern void ETIMx_ETxIF_OVIF_Clr(ETIMx_Type* ETIMx);
extern FlagStatus ETIMx_ETxIF_OVIF_Chk(ETIMx_Type* ETIMx);
//Announce_End

/*ETIMx 初始化配置函数*/
extern void ETIMx_Init(ETIMx_Type* ETIMx, ETIM_InitTypeDef* para);


#ifdef __cplusplus
}
#endif

#endif /* __FM33A0XX_ETIM_H */




/************************ (C) COPYRIGHT FMSHelectronics *****END OF FILE****/



