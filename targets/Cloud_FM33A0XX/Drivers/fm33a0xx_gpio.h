/**
  ******************************************************************************
  * @file    fm33a0xx_gpio.h
  * @author  FM33a0xx Application Team
  * @version V2.0.0
  * @date    
  * @brief   This file contains all the functions prototypes for the GPIO firmware library.  
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FM33A0XX_GPIO_H
#define __FM33A0XX_GPIO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "FM33A0XX.h"
 
/** @addtogroup FM33a0xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup GPIO
  * @{
  */
	 
/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  GPIO Configuration Mode enumeration 
  */
typedef enum
{ 
    GPIO_FCR_IN		= 0x00, /*!< GPIO 输入 */
    GPIO_FCR_OUT	= 0x01, /*!< GPIO 输出 */
    GPIO_FCR_DIG	= 0x02, /*!< GPIO 数字特殊功能 */
    GPIO_FCR_ANA	= 0x03  /*!< GPIO 模拟功能 */
}GPIO_FCR_TypeDef;

/** 
  * @brief  GPIO Output type enumeration 
  */  
typedef enum
{ 
    GPIO_OD_Dis = 0x00,
    GPIO_OD_En = 0x01
}GPIO_ODEN_TypeDef;

/** 
  * @brief  GPIO Configuration PullUp enumeration 
  */ 
typedef enum
{ 
    GPIO_PU_Dis = 0x00,
    GPIO_PU_En  = 0x01 
}GPIO_PUEN_TypeDef;

/** 
  * @brief  GPIO Configuration PullUp enumeration 
  */ 
typedef enum
{ 
    GPIO_IN_Dis		= 0x00,
    GPIO_IN_En		= 0x01 
}GPIO_INEN_TypeDef;

/** 
  * @brief   GPIO Init structure definition  
  */ 
typedef struct
{
    uint32_t					Pin;     	//PIN选择
    GPIO_FCR_TypeDef			PxFCR;		//IO功能选择
    GPIO_ODEN_TypeDef			PxODEN;		//开漏输出使能控制
    GPIO_PUEN_TypeDef			PxPUEN;		//上拉电阻使能控制
    GPIO_INEN_TypeDef			PxINEN;		//输入使能控制
}GPIO_InitTypeDef;

/** 
  * @brief  GPIO Bit SET and Bit RESET enumeration 
  */ 
typedef enum
{ 
    Bit_RESET = 0,
    Bit_SET
}BitAction;
#define IS_GPIO_BIT_ACTION(ACTION) (((ACTION) == Bit_RESET) || ((ACTION) == Bit_SET))

typedef enum
{ 
    EXTI_RISING,
    EXTI_FALLING,
    EXTI_BOTH,
    EXTI_DISABLE,
	
}GPIOExtiEdge;
#define IS_GPIO_INTERRUPT_TRIGER_EDGE(EDGE) (((EDGE) == GPIO_RISING) || ((EDGE) == GPIO_FALLING)|| ((EDGE) == GPIO_BOTH)|| ((EDGE) == GPIO_DISABLE))


/* Exported constants --------------------------------------------------------*/

/** @defgroup GPIO_Exported_Constants
  * @{
  */
	 
/**
* @}
*/ 

/** @defgroup GPIO_Exported_Types
  * @{
  */

#define IS_GPIO_ALL_PERIPH(PERIPH) (((PERIPH) == GPIOA) || \
                                    ((PERIPH) == GPIOB) || \
                                    ((PERIPH) == GPIOC) || \
                                    ((PERIPH) == GPIOD) || \
                                    ((PERIPH) == GPIOE) || \
                                    ((PERIPH) == GPIOF) || \
                                    ((PERIPH) == GPIOG))
                                     
/** @defgroup GPIO_pins_define 
  * @{
  */ 
#define GPIO_Pin_0                 ((uint32_t)0x00000001)  /* Pin 0 selected */
#define GPIO_Pin_1                 ((uint32_t)0x00000002)  /* Pin 1 selected */
#define GPIO_Pin_2                 ((uint32_t)0x00000004)  /* Pin 2 selected */
#define GPIO_Pin_3                 ((uint32_t)0x00000008)  /* Pin 3 selected */
#define GPIO_Pin_4                 ((uint32_t)0x00000010)  /* Pin 4 selected */
#define GPIO_Pin_5                 ((uint32_t)0x00000020)  /* Pin 5 selected */
#define GPIO_Pin_6                 ((uint32_t)0x00000040)  /* Pin 6 selected */
#define GPIO_Pin_7                 ((uint32_t)0x00000080)  /* Pin 7 selected */
#define GPIO_Pin_8                 ((uint32_t)0x00000100)  /* Pin 8 selected */
#define GPIO_Pin_9                 ((uint32_t)0x00000200)  /* Pin 9 selected */
#define GPIO_Pin_10                ((uint32_t)0x00000400)  /* Pin 10 selected */
#define GPIO_Pin_11                ((uint32_t)0x00000800)  /* Pin 11 selected */
#define GPIO_Pin_12                ((uint32_t)0x00001000)  /* Pin 12 selected */
#define GPIO_Pin_13                ((uint32_t)0x00002000)  /* Pin 13 selected */
#define GPIO_Pin_14                ((uint32_t)0x00004000)  /* Pin 14 selected */
#define GPIO_Pin_15                ((uint32_t)0x00008000)  /* Pin 15 selected */
#define GPIO_Pin_All               ((uint16_t)0xFFFF)  /*!< All pins selected */

#define IS_GPIO_PIN(PIN) ((((PIN) & (uint16_t)0x00) == 0x00) && ((PIN) != (uint16_t)0x00))
#define IS_GET_GPIO_PIN(PIN) (((PIN) == GPIO_Pin_0) || \
                              ((PIN) == GPIO_Pin_1) || \
                              ((PIN) == GPIO_Pin_2) || \
                              ((PIN) == GPIO_Pin_3) || \
                              ((PIN) == GPIO_Pin_4) || \
                              ((PIN) == GPIO_Pin_5) || \
                              ((PIN) == GPIO_Pin_6) || \
                              ((PIN) == GPIO_Pin_7) || \
                              ((PIN) == GPIO_Pin_8) || \
                              ((PIN) == GPIO_Pin_9) || \
                              ((PIN) == GPIO_Pin_10) || \
                              ((PIN) == GPIO_Pin_11) || \
                              ((PIN) == GPIO_Pin_12) || \
                              ((PIN) == GPIO_Pin_13) || \
                              ((PIN) == GPIO_Pin_14) || \
                              ((PIN) == GPIO_Pin_15))



/* Exported macro ------------------------------------------------------------*/

/* 数字滤波配置PIN宏定义 */
#define		IODF_PF3		BIT23
#define		IODF_PF2		BIT22
#define		IODF_PF1		BIT21
#define		IODF_PE9		BIT20
#define		IODF_PE8		BIT19
#define		IODF_PE7		BIT18
#define		IODF_PE6		BIT17
#define		IODF_PE2		BIT16
#define		IODF_PD3		BIT15
#define		IODF_PD2		BIT14
#define		IODF_PD1		BIT13
#define		IODF_PD0		BIT12
#define		IODF_PC15		BIT11
#define		IODF_PC14		BIT10
#define		IODF_PC13		BIT9
#define		IODF_PC12		BIT8
#define		IODF_PB7		BIT7
#define		IODF_PB6		BIT6
#define		IODF_PB5		BIT5
#define		IODF_PB4		BIT4
#define		IODF_PA11		BIT3
#define		IODF_PA10		BIT2
#define		IODF_PA9		BIT1
#define		IODF_PA8		BIT0

/* WKUP引脚使能PIN宏定义 */
#define		PINWKEN_PD6		BIT7
#define		PINWKEN_PE9		BIT6
#define		PINWKEN_PE2		BIT5
#define		PINWKEN_PA13	BIT4
#define		PINWKEN_PG7		BIT3
#define		PINWKEN_PC13	BIT2
#define		PINWKEN_PB0		BIT1
#define		PINWKEN_PF5		BIT0

//Macro_START
//2018-04-02-13-21-16
//FM33A0XX_Driver_Gen_V1.4

#define	GPIOx_INEN_INEN_Pos	0	/* PortX输入使能寄存器 */
#define	GPIOx_INEN_INEN_Msk	(0xffffU << GPIOx_INEN_INEN_Pos)

#define	GPIOx_PUEN_PUEN_Pos	0	/* PortX上拉使能寄存器 */
#define	GPIOx_PUEN_PUEN_Msk	(0xffffU << GPIOx_PUEN_PUEN_Pos)

#define	GPIOx_ODEN_ODEN_Pos	0	/* PortX开漏使能寄存器 */
#define	GPIOx_ODEN_ODEN_Msk	(0xffffU << GPIOx_ODEN_ODEN_Pos)

#define	GPIOx_FCR_FCR_Pos	0	/* PortX功能选择寄存器 */
#define	GPIOx_FCR_FCR_Msk	(0xffffffffU << GPIOx_FCR_FCR_Pos)

#define	GPIOx_DO_DO_Pos	0	/* PortX输出数据寄存器 */
#define	GPIOx_DO_DO_Msk	(0xffffU << GPIOx_DO_DO_Pos)

#define	GPIOx_DSET_DSET_Pos	0	/* PortX输出数据置位寄存器 */
#define	GPIOx_DSET_DSET_Msk	(0xffffU << GPIOx_DSET_DSET_Pos)

#define	GPIOx_DRESET_DRESET_Pos	0	/* PortX输出数据复位寄存器 */
#define	GPIOx_DRESET_DRESET_Msk	(0xffffU << GPIOx_DRESET_DRESET_Pos)

#define	GPIOx_DIN_DIN_Pos	0	/* PortX输入数据寄存器 */
#define	GPIOx_DIN_DIN_Msk	(0xffffU << GPIOx_DIN_DIN_Pos)

#define	GPIO_EXTI0_SEL_EXTI0_SEL_Pos	0	/* 外部输入中断选择寄存器0 */
#define	GPIO_EXTI0_SEL_EXTI0_SEL_Msk	(0xffffffffU << GPIO_EXTI0_SEL_EXTI0_SEL_Pos)

#define	GPIO_EXTI1_SEL_EXTI1_SEL_Pos	0	/* 外部输入中断选择寄存器1 */
#define	GPIO_EXTI1_SEL_EXTI1_SEL_Msk	(0xffffffffU << GPIO_EXTI1_SEL_EXTI1_SEL_Pos)

#define	GPIO_EXTI2_SEL_EXTI2_SEL_Pos	0	/* 外部输入中断选择寄存器2 */
#define	GPIO_EXTI2_SEL_EXTI2_SEL_Msk	(0xffffffffU << GPIO_EXTI2_SEL_EXTI2_SEL_Pos)

#define	GPIO_EXTI0IF_EXTI0IF_Pos	0	/* 外部输入中断标志寄存器0 */
#define	GPIO_EXTI0IF_EXTI0IF_Msk	(0xffU << GPIO_EXTI0IF_EXTI0IF_Pos)

#define	GPIO_EXTI1IF_EXTI1IF_Pos	0	/* 外部输入中断标志寄存器1 */
#define	GPIO_EXTI1IF_EXTI1IF_Msk	(0xffU << GPIO_EXTI1IF_EXTI1IF_Pos)

#define	GPIO_EXTI2IF_EXTI2IF_Pos	0	/* 外部输入中断标志寄存器2 */
#define	GPIO_EXTI2IF_EXTI2IF_Msk	(0xffU << GPIO_EXTI2IF_EXTI2IF_Pos)

#define	GPIO_FOUTSEL_FOUTSEL_Pos	0	/* FOUT输出频率选择信号 */
#define	GPIO_FOUTSEL_FOUTSEL_Msk	(0xfU << GPIO_FOUTSEL_FOUTSEL_Pos)
#define	GPIO_FOUTSEL_FOUTSEL_XTLF	(0x0U << GPIO_FOUTSEL_FOUTSEL_Pos)	/* 0000: XTLF */
#define	GPIO_FOUTSEL_FOUTSEL_RCLP	(0x1U << GPIO_FOUTSEL_FOUTSEL_Pos)	/* 0001: RCLP */
#define	GPIO_FOUTSEL_FOUTSEL_RCHFD64	(0x2U << GPIO_FOUTSEL_FOUTSEL_Pos)	/* 0010: RCHF/64 */
#define	GPIO_FOUTSEL_FOUTSEL_LSCLK	(0x3U << GPIO_FOUTSEL_FOUTSEL_Pos)	/* 0011: LSCLK */
#define	GPIO_FOUTSEL_FOUTSEL_AHBCLKD64	(0x4U << GPIO_FOUTSEL_FOUTSEL_Pos)	/* 0100: AHBCLK/64 */
#define	GPIO_FOUTSEL_FOUTSEL_RTCTM	(0x5U << GPIO_FOUTSEL_FOUTSEL_Pos)	/* 0101: RTCTM */
#define	GPIO_FOUTSEL_FOUTSEL_PLLOD64	(0x6U << GPIO_FOUTSEL_FOUTSEL_Pos)	/* 0110: PLLO/64 */
#define	GPIO_FOUTSEL_FOUTSEL_RTCCLK64HZ	(0x7U << GPIO_FOUTSEL_FOUTSEL_Pos)	/* 0111: RTCCLK64Hz */
#define	GPIO_FOUTSEL_FOUTSEL_APBCLKD64	(0x8U << GPIO_FOUTSEL_FOUTSEL_Pos)	/* 1000: APBCLK/64 */
#define	GPIO_FOUTSEL_FOUTSEL_LCDBOOSTERCLK	(0x9U << GPIO_FOUTSEL_FOUTSEL_Pos)	/* 1001: LCD Booster Clock */
#define	GPIO_FOUTSEL_FOUTSEL_LVMOS	(0xaU << GPIO_FOUTSEL_FOUTSEL_Pos)	/* 1010: LVMOS-Monitor-RCOSC/64 */

#define	GPIO_HDSEL_PG6HDEN_Pos	1	/* PG6强驱动使能 */
#define	GPIO_HDSEL_PG6HDEN_Msk	(0x1U << GPIO_HDSEL_PG6HDEN_Pos)
	/* 0：关闭强驱动模式 */
	/* 1：使能强驱动模式 */

#define	GPIO_HDSEL_PE2HDEN_Pos	0	/* PE2强驱动使能 */
#define	GPIO_HDSEL_PE2HDEN_Msk	(0x1U << GPIO_HDSEL_PE2HDEN_Pos)
	/* 0：关闭强驱动模式 */
	/* 1：使能强驱动模式 */

#define	GPIO_ANASEL_PE4ANS_Pos	5	/* PE4模拟功能选择 */
#define	GPIO_ANASEL_PE4ANS_Msk	(0x1U << GPIO_ANASEL_PE4ANS_Pos)
#define	GPIO_ANASEL_PE4ANS_ACMP1_INP1	(0x0U << GPIO_ANASEL_PE4ANS_Pos)	/* 0：选择作为ACMP1_INP1 */
#define	GPIO_ANASEL_PE4ANS_SEG19	(0x1U << GPIO_ANASEL_PE4ANS_Pos)	/* 1：选择作为SEG19 */

#define	GPIO_ANASEL_PE3ANS_Pos	4	/* PE3模拟功能选择 */
#define	GPIO_ANASEL_PE3ANS_Msk	(0x1U << GPIO_ANASEL_PE3ANS_Pos)
#define	GPIO_ANASEL_PE3ANS_ACMP1_INN1	(0x0U << GPIO_ANASEL_PE3ANS_Pos)	/* 0：选择作为ACMP1_INN1 */
#define	GPIO_ANASEL_PE3ANS_SEG18	(0x1U << GPIO_ANASEL_PE3ANS_Pos)	/* 1：选择作为SEG18 */

#define	GPIO_ANASEL_PC15ANS_Pos	3	/* PC15模拟功能选择 */
#define	GPIO_ANASEL_PC15ANS_Msk	(0x1U << GPIO_ANASEL_PC15ANS_Pos)
#define	GPIO_ANASEL_PC15ANS_ACMP1_INP0	(0x0U << GPIO_ANASEL_PC15ANS_Pos)	/* 0：选择作为ACMP1_INP0 */
#define	GPIO_ANASEL_PC15ANS_SEG39	(0x1U << GPIO_ANASEL_PC15ANS_Pos)	/* 1：选择作为SEG39 */

#define	GPIO_ANASEL_PC14ANS_Pos	2	/* PC14模拟功能选择 */
#define	GPIO_ANASEL_PC14ANS_Msk	(0x1U << GPIO_ANASEL_PC14ANS_Pos)
#define	GPIO_ANASEL_PC14ANS_ACMP1_INN0	(0x0U << GPIO_ANASEL_PC14ANS_Pos)	/* 0：选择作为ACMP1_INN0 */
#define	GPIO_ANASEL_PC14ANS_SEG38	(0x1U << GPIO_ANASEL_PC14ANS_Pos)	/* 1：选择作为SEG38 */

#define	GPIO_ANASEL_PC13ANS_Pos	1	/* PC13模拟功能选择 */
#define	GPIO_ANASEL_PC13ANS_Msk	(0x1U << GPIO_ANASEL_PC13ANS_Pos)
#define	GPIO_ANASEL_PC13ANS_ADC_IN2	(0x0U << GPIO_ANASEL_PC13ANS_Pos)	/* 0：选择作为ADC_IN2 */
#define	GPIO_ANASEL_PC13ANS_SEG37	(0x1U << GPIO_ANASEL_PC13ANS_Pos)	/* 1：选择作为SEG37 */

#define	GPIO_ANASEL_PC12ANS_Pos	0	/* PC12模拟功能选择 */
#define	GPIO_ANASEL_PC12ANS_Msk	(0x1U << GPIO_ANASEL_PC12ANS_Pos)
#define	GPIO_ANASEL_PC12ANS_ADC_IN1	(0x0U << GPIO_ANASEL_PC12ANS_Pos)	/* 0：选择作为ADC_IN1 */
#define	GPIO_ANASEL_PC12ANS_SEG36	(0x1U << GPIO_ANASEL_PC12ANS_Pos)	/* 1：选择作为SEG36 */

#define	GPIO_IODF_IODF_Pos	0	/* GPIO输入数字滤波寄存器 */
#define	GPIO_IODF_IODF_Msk	(0xffffffU << GPIO_IODF_IODF_Pos)

#define	GPIO_PINWKEN_PINWKEN_Pos	0	/* WKUP引脚使能寄存器 */
#define	GPIO_PINWKEN_PINWKEN_Msk	(0xffU << GPIO_PINWKEN_PINWKEN_Pos)
//Macro_End

/* Exported functions --------------------------------------------------------*/ 
extern void GPIOx_Deinit(GPIOx_Type* GPIOx);

/* PortX输出数据寄存器 相关函数 */
extern void GPIOx_DO_Write(GPIOx_Type* GPIOx, uint32_t SetValue);
extern uint32_t GPIOx_DO_Read(GPIOx_Type* GPIOx);

/* PortX输出数据置位寄存器 相关函数 */
extern void GPIOx_DSET_Write(GPIOx_Type* GPIOx, uint32_t SetValue);

/* PortX输出数据复位寄存器 相关函数 */
extern void GPIOx_DRESET_Write(GPIOx_Type* GPIOx, uint32_t SetValue);

/* PortX输入数据寄存器 相关函数 */
extern uint32_t GPIOx_DIN_Read(GPIOx_Type* GPIOx);
extern void GPIO_Deinit(void);

/* FOUT输出频率选择信号 相关函数 */
extern void GPIO_FOUTSEL_FOUTSEL_Set(uint32_t SetValue);
extern uint32_t GPIO_FOUTSEL_FOUTSEL_Get(void);

/* PG6强驱动使能 相关函数 */
extern void GPIO_HDSEL_PG6HDEN_Setable(FunState NewState);
extern FunState GPIO_HDSEL_PG6HDEN_Getable(void);

/* PE2强驱动使能 相关函数 */
extern void GPIO_HDSEL_PE2HDEN_Setable(FunState NewState);
extern FunState GPIO_HDSEL_PE2HDEN_Getable(void);

/* PE4模拟功能选择 相关函数 */
extern void GPIO_ANASEL_PE4ANS_Set(uint32_t SetValue);
extern uint32_t GPIO_ANASEL_PE4ANS_Get(void);

/* PE3模拟功能选择 相关函数 */
extern void GPIO_ANASEL_PE3ANS_Set(uint32_t SetValue);
extern uint32_t GPIO_ANASEL_PE3ANS_Get(void);

/* PC15模拟功能选择 相关函数 */
extern void GPIO_ANASEL_PC15ANS_Set(uint32_t SetValue);
extern uint32_t GPIO_ANASEL_PC15ANS_Get(void);

/* PC14模拟功能选择 相关函数 */
extern void GPIO_ANASEL_PC14ANS_Set(uint32_t SetValue);
extern uint32_t GPIO_ANASEL_PC14ANS_Get(void);

/* PC13模拟功能选择 相关函数 */
extern void GPIO_ANASEL_PC13ANS_Set(uint32_t SetValue);
extern uint32_t GPIO_ANASEL_PC13ANS_Get(void);

/* PC12模拟功能选择 相关函数 */
extern void GPIO_ANASEL_PC12ANS_Set(uint32_t SetValue);
extern uint32_t GPIO_ANASEL_PC12ANS_Get(void);
//Announce_End



/* Exported functions --------------------------------------------------------*/ 

extern void GPIO_ALL_Deinit(void);

/* GPIO 初始化配置 */
extern void GPIO_Init(GPIOx_Type* GPIOx, GPIO_InitTypeDef* para);

/* 获取一个IO口的配置参数结构体 
	注意一次只能读取一个IO的配置*/
extern void GPIO_Get_InitPara(GPIOx_Type* GPIOx, uint32_t GPIO_Pin, GPIO_InitTypeDef* para);

/* 读取GPIOx输入数据寄存器 */
extern uint32_t GPIO_ReadInputData(GPIOx_Type* GPIOx);

/* 读取GPIOx输入数据寄存器bit */
extern uint8_t GPIO_ReadInputDataBit(GPIOx_Type* GPIOx, uint32_t GPIO_Pin);

/* GPIOx输出数据置1 */
extern void GPIO_SetBits(GPIOx_Type* GPIOx, uint32_t GPIO_Pin);

/* GPIOx输出数据置0 */
extern void GPIO_ResetBits(GPIOx_Type* GPIOx, uint32_t GPIO_Pin);

/* GPIOx输出数据置翻转 */
extern void GPIO_ToggleBits(GPIOx_Type* GPIOx, uint32_t GPIO_Pin);

/* 读取GPIOx输出数据寄存器 */
extern uint32_t GPIO_ReadOutputData(GPIOx_Type* GPIOx);

/* 读取GPIOx输出数据寄存器bit */
extern uint8_t GPIO_ReadOutputDataBit(GPIOx_Type* GPIOx, uint32_t GPIO_Pin);

/* GPIO输入数字滤波配置 */
extern void GPIO_IODF_SetableEx(uint32_t DFPinDef, FunState NewState);

/* WKUP引脚使能 */
extern void GPIO_PINWKEN_SetableEx(uint32_t NWKPinDef, FunState NewState);

/*************************************************************************
 函数名称：GPIO_EXTI_Select_Pin
 功能说明：GPIO EXTI 外部引脚选择
 输入参数：GPIOx 端口 GPIO_Pinpin 端口内引脚 
 输出参数：无
 返回参数无
 *************************************************************************/
extern void GPIO_EXTI_Select_Pin(GPIOx_Type* GPIOx,uint32_t GPIO_Pin);

/*************************************************************************
 函数名称：GPIO_EXTI_Select_Edge
 功能说明：GPIO EXTI_Select_Edge 外部中断边沿选择
 输入参数：GPIOx 端口 ，GPIO_Pin 端口内引脚 ,edge_select 触发沿
 输出参数：无
 返回参数无
 *************************************************************************/
extern void GPIO_EXTI_Select_Edge(GPIOx_Type* GPIOx,uint32_t GPIO_Pin,GPIOExtiEdge edge_select);

/*************************************************************************
 函数名称：GPIO_EXTI_EXTIxIF_ClrEx
 功能说明：GPIO_EXTI_EXTIxIF_ClrEx 清除中断标志
 输入参数：GPIOx 端口 ，GPIO_Pin 端口内引脚 
 输出参数：无
 返回参数无
 *************************************************************************/
extern void GPIO_EXTI_EXTIxIF_ClrEx(GPIOx_Type* GPIOx,uint32_t GPIO_Pin);

/*************************************************************************
 函数名称：GPIO_EXTI_EXTIxIF_ChkEx
 功能说明：GPIO_EXTI_EXTIxIF_ChkEx 读取中断标志状态
 输入参数：GPIOx 端口 ，GPIO_Pin 端口内引脚 
 输出参数：无
 返回参数FlagStatus 中断标志状态
 *************************************************************************/
extern FlagStatus GPIO_EXTI_EXTIxIF_ChkEx(GPIOx_Type* GPIOx,uint32_t GPIO_Pin);

/*************************************************************************
 函数名称：GPIO_EXTI_Init
 功能说明：GPIO_EXTI_Init 外部中断初始化
 输入参数：port 端口 ，pin 端口内引脚 ,edge 触发沿
 输出参数：无
 返回参数无
 *************************************************************************/
extern void GPIO_EXTI_Init(GPIOx_Type* GPIOx,uint32_t GPIO_Pin,GPIOExtiEdge edge_select);

/*************************************************************************
 函数名称：GPIO_EXTI_Close
 功能说明：GPIO_EXTI_Close 外部中断关闭
 输入参数：port 端口 ，pin 端口内引脚 
 输出参数：无
 返回参数无
 *************************************************************************/
extern void GPIO_EXTI_Close(GPIOx_Type* GPIOx,uint32_t GPIO_Pin);
	
#ifdef __cplusplus
}
#endif

#endif /* __FM33A0XX_GPIO_H */



/************************ (C) COPYRIGHT FMSHelectronics *****END OF FILE****/



