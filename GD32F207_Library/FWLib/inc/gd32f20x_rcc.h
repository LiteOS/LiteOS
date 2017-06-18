/**
  ******************************************************************************
  * @file    gd32f20x_rcc.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    15-Jul-2015
  * @brief   RCC header file of the firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F20X_RCC_H
#define __GD32F20X_RCC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gd32f20x.h"

/** @addtogroup GD32F20x_Firmware
  * @{
  */

/** @addtogroup RCC
  * @{
  */

/** @defgroup RCC_Exported_Types
  * @{
  */

/** 
  * @brief  RCC Initial Parameters
  */

typedef struct
{
  uint32_t CK_SYS_Frequency;             /*!< The frequency of the CK_SYS.     */
  uint32_t AHB_Frequency;                /*!< The frequency of the AHB.        */
  uint32_t APB1_Frequency;              /*!< The frequency of the APB1.       */
  uint32_t APB2_Frequency;              /*!< The frequency of the APB2.       */
  uint32_t ADCCLK_Frequency;            /*!< The frequency of the ADCCLK.     */
}RCC_ClocksPara;

/**
  * @}
  */

/** @defgroup RCC_Exported_Constants
  * @{
  */

/** @defgroup RCC_HSE_configuration 
  * @{
  */

#define RCC_HSE_OFF                      ((uint32_t)0x00000000)
#define RCC_HSE_ON                       RCC_GCCR_HSEEN
#define RCC_HSE_BYPASS                   RCC_GCCR_HSEEN | RCC_GCCR_HSEBPS

/**
  * @}
  */ 

/** @defgroup RCC_PLL_input_clock_source 
  * @{
  */

#define RCC_PLLSOURCE_HSI_DIV2           RCC_GCFGR_PLLSEL_HSI_DIV2
#define RCC_PLLSOURCE_PREDIV1            RCC_GCFGR_PLLSEL_PREDIV1

/**
  * @}
  */ 

/** @defgroup RCC_PLL_Multiplication_factor 
  * @{
  */
#define  RCC_PLLMUL_2                         RCC_GCFGR_PLLMF2
#define  RCC_PLLMUL_3                         RCC_GCFGR_PLLMF3
#define  RCC_PLLMUL_4                         RCC_GCFGR_PLLMF4
#define  RCC_PLLMUL_5                         RCC_GCFGR_PLLMF5
#define  RCC_PLLMUL_6                         RCC_GCFGR_PLLMF6
#define  RCC_PLLMUL_7                         RCC_GCFGR_PLLMF7
#define  RCC_PLLMUL_8                         RCC_GCFGR_PLLMF8
#define  RCC_PLLMUL_9                         RCC_GCFGR_PLLMF9
#define  RCC_PLLMUL_10                        RCC_GCFGR_PLLMF10
#define  RCC_PLLMUL_11                        RCC_GCFGR_PLLMF11
#define  RCC_PLLMUL_12                        RCC_GCFGR_PLLMF12
#define  RCC_PLLMUL_13                        RCC_GCFGR_PLLMF13
#define  RCC_PLLMUL_14                        RCC_GCFGR_PLLMF14

#define  RCC_PLLMUL_16                        RCC_GCFGR_PLLMF16
#define  RCC_PLLMUL_17                        RCC_GCFGR_PLLMF17
#define  RCC_PLLMUL_18                        RCC_GCFGR_PLLMF18
#define  RCC_PLLMUL_19                        RCC_GCFGR_PLLMF19
#define  RCC_PLLMUL_20                        RCC_GCFGR_PLLMF20
#define  RCC_PLLMUL_21                        RCC_GCFGR_PLLMF21
#define  RCC_PLLMUL_22                        RCC_GCFGR_PLLMF22
#define  RCC_PLLMUL_23                        RCC_GCFGR_PLLMF23
#define  RCC_PLLMUL_24                        RCC_GCFGR_PLLMF24
#define  RCC_PLLMUL_25                        RCC_GCFGR_PLLMF25
#define  RCC_PLLMUL_26                        RCC_GCFGR_PLLMF26
#define  RCC_PLLMUL_27                        RCC_GCFGR_PLLMF27
#define  RCC_PLLMUL_28                        RCC_GCFGR_PLLMF28
#define  RCC_PLLMUL_29                        RCC_GCFGR_PLLMF29
#define  RCC_PLLMUL_30                        RCC_GCFGR_PLLMF30
#define  RCC_PLLMUL_31                        RCC_GCFGR_PLLMF31
#define  RCC_PLLMUL_32                        RCC_GCFGR_PLLMF32
#define  RCC_PLLMUL_6_5                       RCC_GCFGR_PLLMF6_5

/**
  * @}
  */

/** @defgroup RCC_PREDIV1_division_factor
  * @{
  */

#define  RCC_PREDIV1_DIV1               RCC_GCFGR2_PREDV1_DIV1
#define  RCC_PREDIV1_DIV2               RCC_GCFGR2_PREDV1_DIV2
#define  RCC_PREDIV1_DIV3               RCC_GCFGR2_PREDV1_DIV3
#define  RCC_PREDIV1_DIV4               RCC_GCFGR2_PREDV1_DIV4
#define  RCC_PREDIV1_DIV5               RCC_GCFGR2_PREDV1_DIV5
#define  RCC_PREDIV1_DIV6               RCC_GCFGR2_PREDV1_DIV6
#define  RCC_PREDIV1_DIV7               RCC_GCFGR2_PREDV1_DIV7
#define  RCC_PREDIV1_DIV8               RCC_GCFGR2_PREDV1_DIV8
#define  RCC_PREDIV1_DIV9               RCC_GCFGR2_PREDV1_DIV9
#define  RCC_PREDIV1_DIV10              RCC_GCFGR2_PREDV1_DIV10
#define  RCC_PREDIV1_DIV11              RCC_GCFGR2_PREDV1_DIV11
#define  RCC_PREDIV1_DIV12              RCC_GCFGR2_PREDV1_DIV12
#define  RCC_PREDIV1_DIV13              RCC_GCFGR2_PREDV1_DIV13
#define  RCC_PREDIV1_DIV14              RCC_GCFGR2_PREDV1_DIV14
#define  RCC_PREDIV1_DIV15              RCC_GCFGR2_PREDV1_DIV15
#define  RCC_PREDIV1_DIV16              RCC_GCFGR2_PREDV1_DIV16

/**
  * @}
  */


/** @defgroup RCC_PREDIV1_clock_source
  * @{
  */

#define  RCC_PREDIV1_SOURCE_HSE         RCC_GCFGR2_PREDV1SEL_HSE 
#define  RCC_PREDIV1_SOURCE_PLL2        RCC_GCFGR2_PREDV1SEL_PLL2 

/**
  * @}
  */


/** @defgroup RCC_PREDIV2_division_factor
  * @{
  */ 
#define  RCC_PREDIV2_DIV1               RCC_GCFGR2_PREDV2_DIV1
#define  RCC_PREDIV2_DIV2               RCC_GCFGR2_PREDV2_DIV2
#define  RCC_PREDIV2_DIV3               RCC_GCFGR2_PREDV2_DIV3
#define  RCC_PREDIV2_DIV4               RCC_GCFGR2_PREDV2_DIV4
#define  RCC_PREDIV2_DIV5               RCC_GCFGR2_PREDV2_DIV5
#define  RCC_PREDIV2_DIV6               RCC_GCFGR2_PREDV2_DIV6
#define  RCC_PREDIV2_DIV7               RCC_GCFGR2_PREDV2_DIV7
#define  RCC_PREDIV2_DIV8               RCC_GCFGR2_PREDV2_DIV8
#define  RCC_PREDIV2_DIV9               RCC_GCFGR2_PREDV2_DIV9
#define  RCC_PREDIV2_DIV10              RCC_GCFGR2_PREDV2_DIV10
#define  RCC_PREDIV2_DIV11              RCC_GCFGR2_PREDV2_DIV11
#define  RCC_PREDIV2_DIV12              RCC_GCFGR2_PREDV2_DIV12
#define  RCC_PREDIV2_DIV13              RCC_GCFGR2_PREDV2_DIV13
#define  RCC_PREDIV2_DIV14              RCC_GCFGR2_PREDV2_DIV14
#define  RCC_PREDIV2_DIV15              RCC_GCFGR2_PREDV2_DIV15
#define  RCC_PREDIV2_DIV16              RCC_GCFGR2_PREDV2_DIV16

/**
  * @}
  */


/** @defgroup RCC_PLL2_multiplication_factor
  * @{
  */ 
#define  RCC_PLL2MUL_8                  RCC_GCFGR2_PLL2MF8
#define  RCC_PLL2MUL_9                  RCC_GCFGR2_PLL2MF9
#define  RCC_PLL2MUL_10                 RCC_GCFGR2_PLL2MF10
#define  RCC_PLL2MUL_11                 RCC_GCFGR2_PLL2MF11
#define  RCC_PLL2MUL_12                 RCC_GCFGR2_PLL2MF12
#define  RCC_PLL2MUL_13                 RCC_GCFGR2_PLL2MF13
#define  RCC_PLL2MUL_14                 RCC_GCFGR2_PLL2MF14
#define  RCC_PLL2MUL_16                 RCC_GCFGR2_PLL2MF16
#define  RCC_PLL2MUL_20                 RCC_GCFGR2_PLL2MF20

/**
  * @}
  */


/** @defgroup RCC_PLL3_multiplication_factor
  * @{
  */
#define  RCC_PLL3MUL_8                  RCC_GCFGR2_PLL3MF8
#define  RCC_PLL3MUL_9                  RCC_GCFGR2_PLL3MF9
#define  RCC_PLL3MUL_10                 RCC_GCFGR2_PLL3MF10
#define  RCC_PLL3MUL_11                 RCC_GCFGR2_PLL3MF11
#define  RCC_PLL3MUL_12                 RCC_GCFGR2_PLL3MF12
#define  RCC_PLL3MUL_13                 RCC_GCFGR2_PLL3MF13
#define  RCC_PLL3MUL_14                 RCC_GCFGR2_PLL3MF14
#define  RCC_PLL3MUL_16                 RCC_GCFGR2_PLL3MF16
#define  RCC_PLL3MUL_20                 RCC_GCFGR2_PLL3MF20

/**
  * @}
  */


/** @defgroup RCC_System_Clock_Source 
  * @{
  */
#define  RCC_SYSCLKSOURCE_HSI                RCC_GCFGR_SCS_HSI
#define  RCC_SYSCLKSOURCE_HSE                RCC_GCFGR_SCS_HSE
#define  RCC_SYSCLKSOURCE_PLLCLK             RCC_GCFGR_SCS_PLL

/**
  * @}
  */

/** @defgroup RCC_AHB_Clock_Source
  * @{
  */
#define  RCC_SYSCLK_DIV1                     RCC_GCFGR_AHBPS_DIV1
#define  RCC_SYSCLK_DIV2                     RCC_GCFGR_AHBPS_DIV2
#define  RCC_SYSCLK_DIV4                     RCC_GCFGR_AHBPS_DIV4
#define  RCC_SYSCLK_DIV8                     RCC_GCFGR_AHBPS_DIV8
#define  RCC_SYSCLK_DIV16                    RCC_GCFGR_AHBPS_DIV16
#define  RCC_SYSCLK_DIV64                    RCC_GCFGR_AHBPS_DIV64
#define  RCC_SYSCLK_DIV128                   RCC_GCFGR_AHBPS_DIV128
#define  RCC_SYSCLK_DIV256                   RCC_GCFGR_AHBPS_DIV256
#define  RCC_SYSCLK_DIV512                   RCC_GCFGR_AHBPS_DIV512

/**
  * @}
  */ 

/** @defgroup RCC_APB_Clock_Source
  * @{
  */
#define  RCC_APB1AHB_DIV1                    RCC_GCFGR_APB1PS_DIV1
#define  RCC_APB1AHB_DIV2                    RCC_GCFGR_APB1PS_DIV2
#define  RCC_APB1AHB_DIV4                    RCC_GCFGR_APB1PS_DIV4
#define  RCC_APB1AHB_DIV8                    RCC_GCFGR_APB1PS_DIV8
#define  RCC_APB1AHB_DIV16                   RCC_GCFGR_APB1PS_DIV16

#define  RCC_APB2AHB_DIV1                    RCC_GCFGR_APB2PS_DIV1
#define  RCC_APB2AHB_DIV2                    RCC_GCFGR_APB2PS_DIV2
#define  RCC_APB2AHB_DIV4                    RCC_GCFGR_APB2PS_DIV4
#define  RCC_APB2AHB_DIV8                    RCC_GCFGR_APB2PS_DIV8
#define  RCC_APB2AHB_DIV16                   RCC_GCFGR_APB2PS_DIV16
/**
  * @}
  */

/** @defgroup RCC_ADC_clock_source 
  * @{
  */
#define  RCC_ADCCLK_APB2_DIV2                RCC_GCFGR_ADCPS_DIV2
#define  RCC_ADCCLK_APB2_DIV4                RCC_GCFGR_ADCPS_DIV4
#define  RCC_ADCCLK_APB2_DIV6                RCC_GCFGR_ADCPS_DIV6
#define  RCC_ADCCLK_APB2_DIV8                RCC_GCFGR_ADCPS_DIV8
#define  RCC_ADCCLK_APB2_DIV12               RCC_GCFGR_ADCPS_DIV12
#define  RCC_ADCCLK_APB2_DIV16               RCC_GCFGR_ADCPS_DIV16

/**
  * @}
  */

/** @defgroup RCC_USB_OTG_clock_source 
  * @{
  */
#define  RCC_OTGCLK_PLL_DIV1                RCC_GCFGR_OTGFSPS_Div1
#define  RCC_OTGCLK_PLL_DIV1_5              RCC_GCFGR_OTGFSPS_Div1_5
#define  RCC_OTGCLK_PLL_DIV2                RCC_GCFGR_OTGFSPS_Div2
#define  RCC_OTGCLK_PLL_DIV2_5              RCC_GCFGR_OTGFSPS_Div2_5

/**
  * @}
  */


/** @defgroup RCC_CK_OUT_Clock_Source
  * @{
  */
#define  RCC_CKOUTSRC_NOCLOCK               RCC_GCFGR_CKOUTSEL_NoClock
#define  RCC_CKOUTSRC_SYSCLK                RCC_GCFGR_CKOUTSEL_SYSCLK
#define  RCC_CKOUTSRC_HSI                   RCC_GCFGR_CKOUTSEL_HSI
#define  RCC_CKOUTSRC_HSE                   RCC_GCFGR_CKOUTSEL_HSE
#define  RCC_CKOUTSRC_PLLCLK_DIV2           RCC_GCFGR_CKOUTSEL_PLL_DIV2
#define  RCC_CKOUTSRC_PLL2CLK               RCC_GCFGR_CKOUTSEL_PLL2
#define  RCC_CKOUTSRC_PLL3CLK               RCC_GCFGR_CKOUTSEL_PLL3
#define  RCC_CKOUTSRC_PLL3CLK_DIV2          RCC_GCFGR_CKOUTSEL_PLL3_DIV2
#define  RCC_CKOUTSRC_EXT1                  RCC_GCFGR_CKOUTSEL_EXT1

/**
  * @}
  */

/** @defgroup RCC_Interrupt_Source
  * @{
  */
#define  RCC_INT_LSISTB                      ((uint32_t)0x00000001)
#define  RCC_INT_LSESTB                      ((uint32_t)0x00000002)
#define  RCC_INT_HSISTB                      ((uint32_t)0x00000004)
#define  RCC_INT_HSESTB                      ((uint32_t)0x00000008)
#define  RCC_INT_PLLSTB                      ((uint32_t)0x00000010)
#define  RCC_INT_PLL2STB                     ((uint32_t)0x00000020)
#define  RCC_INT_PLL3STB                     ((uint32_t)0x00000040)
#define  RCC_INT_CKM                         ((uint32_t)0x00000080)
#define  RCC_INT_PLLTSTB                     ((uint32_t)0x10000040)

/**
  * @}
  */

/** @defgroup RCC_I2S2_clock_source 
  * @{
  */
#define  RCC_I2S2CLK_SYSCLK        RCC_GCFGR2_I2S2SEL_CK_SYS
#define  RCC_I2S2CLK_PLL3          RCC_GCFGR2_I2S2SEL_PLL3

/**
  * @}
  */

/** @defgroup RCC_I2S3_clock_source 
  * @{
  */
#define  RCC_I2S3CLK_SYSCLK        RCC_GCFGR2_I2S3SEL_CK_SYS
#define  RCC_I2S3CLK_PLL3          RCC_GCFGR2_I2S3SEL_PLL3

/**
  * @}
  */

/** @defgroup RCC_LSE_configuration 
  * @{
  */
#define RCC_LSE_OFF                         ((uint32_t)0x00000000)
#define RCC_LSE_EN                           RCC_BDCR_LSEEN
#define RCC_LSE_BYPASS                      ((uint32_t)(RCC_BDCR_LSEEN | RCC_BDCR_LSEBPS))

/**
  * @}
  */

/** @defgroup RCC_LSE_oscillator_drive_capability 
  * @{
  */
#define RCC_LSEDRV_LOW                      ((uint32_t)0x00000000)
#define RCC_LSEDRV_MEDIUM_LOW               ((uint32_t)0x00000008)
#define RCC_LSEDRV_MEDIUM_HIGH              ((uint32_t)0x00000010)
#define RCC_LSEDRV_HIGH                     ((uint32_t)0x00000018)
/**
  * @}
  */

/** @defgroup RCC_RTC_clock_source 
  * @{
  */
#define RCC_RTCCLKSOURCE_LSE             RCC_BDCR_RTCSEL_LSE
#define RCC_RTCCLKSOURCE_LSI             RCC_BDCR_RTCSEL_LSI
#define RCC_RTCCLKSOURCE_HSE_DIV128      RCC_BDCR_RTCSEL_HSE128

/**
  * @}
  */

/** @defgroup RCC_AHB_peripheral 
  * @{
  */
#define  RCC_AHBPERIPH_DMA1               RCC_AHBCCR_DMA1EN
#define  RCC_AHBPERIPH_DMA2               RCC_AHBCCR_DMA2EN
#define  RCC_AHBPERIPH_SRAM               RCC_AHBCCR_SRAMEN
#define  RCC_AHBPERIPH_FMC                RCC_AHBCCR_FMCEN 
#define  RCC_AHBPERIPH_CRC                RCC_AHBCCR_CRCEN    
#define  RCC_AHBPERIPH_EXMC               RCC_AHBCCR_EXMCEN
#define  RCC_AHBPERIPH_SDIO               RCC_AHBCCR_SDIOEN
#define  RCC_AHBPERIPH_OTG_FS             RCC_AHBCCR_OTGFSEN        
#define  RCC_AHBPERIPH_ETH_MAC            RCC_AHBCCR_ETHMACEN       
#define  RCC_AHBPERIPH_ETH_MAC_RX         RCC_AHBCCR_ETHMACRXEN   
#define  RCC_AHBPERIPH_ETH_MAC_TX         RCC_AHBCCR_ETHMACTXEN     

/**
  * @}
  */

/** @defgroup RCC_AHB_Peripherals_RST 
  * @{
  */
#define  RCC_AHBPERIPH_OTGFSRST          RCC_AHBRCR_OTGFSRST
#define  RCC_AHBPERIPH_ETHMACRST         RCC_AHBRCR_ETHMACRST

/**
  * @}
  */


/** @defgroup RCC_APB2_peripheral 
  * @{
  */
#define  RCC_APB2PERIPH_AF                RCC_APB2CCR_AFEN
#define  RCC_APB2PERIPH_GPIOA             RCC_APB2CCR_PAEN    
#define  RCC_APB2PERIPH_GPIOB             RCC_APB2CCR_PBEN             
#define  RCC_APB2PERIPH_GPIOC             RCC_APB2CCR_PCEN        
#define  RCC_APB2PERIPH_GPIOD             RCC_APB2CCR_PDEN          
#define  RCC_APB2PERIPH_GPIOE             RCC_APB2CCR_PEEN        
#define  RCC_APB2PERIPH_GPIOF             RCC_APB2CCR_PFEN
#define  RCC_APB2PERIPH_GPIOG             RCC_APB2CCR_PGEN
#define  RCC_APB2PERIPH_ADC1              RCC_APB2CCR_ADC1EN  
#define  RCC_APB2PERIPH_ADC2              RCC_APB2CCR_ADC2EN   
#define  RCC_APB2PERIPH_TIMER1            RCC_APB2CCR_TIMER1EN  
#define  RCC_APB2PERIPH_SPI1              RCC_APB2CCR_SPI1EN   
#define  RCC_APB2PERIPH_TIMER8            RCC_APB2CCR_TIMER8EN     
#define  RCC_APB2PERIPH_USART1            RCC_APB2CCR_USART1EN     
#define  RCC_APB2PERIPH_ADC3              RCC_APB2CCR_ADC3EN     
#define  RCC_APB2PERIPH_TIMER9            RCC_APB2CCR_TIMER9EN   
#define  RCC_APB2PERIPH_TIMER10           RCC_APB2CCR_TIMER10EN  
#define  RCC_APB2PERIPH_TIMER11           RCC_APB2CCR_TIMER11EN    

/**
  * @}
  */ 

 
/** @defgroup RCC_APB2_Peripherals_RST 
  * @{
  */
#define  RCC_APB2PERIPH_AFRST             RCC_APB2RCR_AFRST
#define  RCC_APB2PERIPH_GPIOARST          RCC_APB2RCR_PARST
#define  RCC_APB2PERIPH_GPIOBRST          RCC_APB2RCR_PBRST
#define  RCC_APB2PERIPH_GPIOCRST          RCC_APB2RCR_PCRST
#define  RCC_APB2PERIPH_GPIODRST          RCC_APB2RCR_PDRST
#define  RCC_APB2PERIPH_GPIOERST          RCC_APB2RCR_PERST
#define  RCC_APB2PERIPH_GPIOFRST          RCC_APB2RCR_PFRST
#define  RCC_APB2PERIPH_GPIOGRST          RCC_APB2RCR_PGRST
#define  RCC_APB2PERIPH_ADC1RST           RCC_APB2RCR_ADC1RST
#define  RCC_APB2PERIPH_ADC2RST           RCC_APB2RCR_ADC2RST
#define  RCC_APB2PERIPH_TIMER1RST         RCC_APB2RCR_TIMER1RST
#define  RCC_APB2PERIPH_SPI1RST           RCC_APB2RCR_SPI1RST
#define  RCC_APB2PERIPH_TIMER8RST         RCC_APB2RCR_TIMER8RST
#define  RCC_APB2PERIPH_USART1RST         RCC_APB2RCR_USART1RST
#define  RCC_APB2PERIPH_ADC3RST           RCC_APB2RCR_ADC3RST
#define  RCC_APB2PERIPH_TIMER9RST         RCC_APB2RCR_TIMER9RST
#define  RCC_APB2PERIPH_TIMER10RST        RCC_APB2RCR_TIMER10RST
#define  RCC_APB2PERIPH_TIMER11RST        RCC_APB2RCR_TIMER11RST

/**
  * @}
  */


/** @defgroup RCC_APB1_peripheral 
  * @{
  */
#define  RCC_APB1PERIPH_TIMER2            RCC_APB1CCR_TIMER2EN
#define  RCC_APB1PERIPH_TIMER3            RCC_APB1CCR_TIMER3EN
#define  RCC_APB1PERIPH_TIMER4            RCC_APB1CCR_TIMER4EN
#define  RCC_APB1PERIPH_TIMER5            RCC_APB1CCR_TIMER5EN
#define  RCC_APB1PERIPH_TIMER6            RCC_APB1CCR_TIMER6EN
#define  RCC_APB1PERIPH_TIMER7            RCC_APB1CCR_TIMER7EN
#define  RCC_APB1PERIPH_TIMER12           RCC_APB1CCR_TIMER12EN
#define  RCC_APB1PERIPH_TIMER13           RCC_APB1CCR_TIMER13EN
#define  RCC_APB1PERIPH_TIMER14           RCC_APB1CCR_TIMER14EN
#define  RCC_APB1PERIPH_WWDG              RCC_APB1CCR_WWDGEN
#define  RCC_APB1PERIPH_SPI2              RCC_APB1CCR_SPI2EN
#define  RCC_APB1PERIPH_SPI3              RCC_APB1CCR_SPI3EN
#define  RCC_APB1PERIPH_USART2            RCC_APB1CCR_USART2EN
#define  RCC_APB1PERIPH_USART3            RCC_APB1CCR_USART3EN
#define  RCC_APB1PERIPH_UART4             RCC_APB1CCR_UART4EN
#define  RCC_APB1PERIPH_UART5             RCC_APB1CCR_UART5EN
#define  RCC_APB1PERIPH_I2C1              RCC_APB1CCR_I2C1EN
#define  RCC_APB1PERIPH_I2C2              RCC_APB1CCR_I2C2EN
#define  RCC_APB1PERIPH_USB               RCC_APB1CCR_USBEN
#define  RCC_APB1PERIPH_CAN1              RCC_APB1CCR_CAN1EN
#define  RCC_APB1PERIPH_CAN2              RCC_APB1CCR_CAN2EN
#define  RCC_APB1PERIPH_BKP               RCC_APB1CCR_BKPEN
#define  RCC_APB1PERIPH_PWR               RCC_APB1CCR_PWREN
#define  RCC_APB1PERIPH_DAC               RCC_APB1CCR_DACEN

/**
  * @}
  */

/** @defgroup RCC_APB1_Peripherals_RST 
  * @{
  */
#define  RCC_APB1PERIPH_TIMER2RST         RCC_APB1RCR_TIMER2RST
#define  RCC_APB1PERIPH_TIMER3RST         RCC_APB1RCR_TIMER3RST
#define  RCC_APB1PERIPH_TIMER4RST         RCC_APB1RCR_TIMER4RST
#define  RCC_APB1PERIPH_TIMER5RST         RCC_APB1RCR_TIMER5RST
#define  RCC_APB1PERIPH_TIMER6RST         RCC_APB1RCR_TIMER6RST
#define  RCC_APB1PERIPH_TIMER7RST         RCC_APB1RCR_TIMER7RST
#define  RCC_APB1PERIPH_TIMER12RST        RCC_APB1RCR_TIMER12RST 
#define  RCC_APB1PERIPH_TIMER13RST        RCC_APB1RCR_TIMER13RST
#define  RCC_APB1PERIPH_TIMER14RST        RCC_APB1RCR_TIMER14RST 
#define  RCC_APB1PERIPH_WWDGRST           RCC_APB1RCR_WWDGRST
#define  RCC_APB1PERIPH_SPI2RST           RCC_APB1RCR_SPI2RST
#define  RCC_APB1PERIPH_SPI3RST           RCC_APB1RCR_SPI3RST
#define  RCC_APB1PERIPH_USART2RST         RCC_APB1RCR_USART2RST   
#define  RCC_APB1PERIPH_USART3RST         RCC_APB1RCR_USART3RST   
#define  RCC_APB1PERIPH_UART4RST          RCC_APB1RCR_UART4RST   
#define  RCC_APB1PERIPH_UART5RST          RCC_APB1RCR_UART5RST   
#define  RCC_APB1PERIPH_I2C1RST           RCC_APB1RCR_I2C1RST    
#define  RCC_APB1PERIPH_I2C2RST           RCC_APB1RCR_I2C2RST
#define  RCC_APB1PERIPH_USBRST            RCC_APB1RCR_USBRST 
#define  RCC_APB1PERIPH_CAN1RST           RCC_APB1RCR_CAN1RST  
#define  RCC_APB1PERIPH_CAN2RST           RCC_APB1RCR_CAN2RST   
#define  RCC_APB1PERIPH_BKPRST            RCC_APB1RCR_BKPRST    
#define  RCC_APB1PERIPH_PWRRST            RCC_APB1RCR_PWRRST    
#define  RCC_APB1PERIPH_DACRST            RCC_APB1RCR_DACRST    

/**
  * @}
  */

/** @defgroup RCC_AHB2_peripheral 
  * @{
  */
/*GD32F20x*/
#define  RCC_AHB2PERIPH_DCI                   ((uint32_t)0x00000001)             
#define  RCC_AHB2PERIPH_CAU                   ((uint32_t)0x00000010)            
#define  RCC_AHB2PERIPH_HAU                   ((uint32_t)0x00000020)            
#define  RCC_AHB2PERIPH_RNG                   ((uint32_t)0x00000040)  

/**
  * @}
  */

/** @defgroup RCC_AHB2_Peripherals_RST 
  * @{
  */
/*GD32F20x*/
#define  RCC_AHB2PERIPH_DCIRST                ((uint32_t)0x00000001)             
#define  RCC_AHB2PERIPH_CAURST                ((uint32_t)0x00000010)            
#define  RCC_AHB2PERIPH_HAURST                ((uint32_t)0x00000020)            
#define  RCC_AHB2PERIPH_RNGRST                ((uint32_t)0x00000040)  

/**
  * @}
  */

/** @defgroup RCC_APB2_peripheral2 
  * @{
  */
/*GD32F20x*/
#define  RCC_APB2PERIPH2_USART6               ((uint32_t)0x01000000)            
#define  RCC_APB2PERIPH2_TLDI                 ((uint32_t)0x04000000)            
#define  RCC_APB2PERIPH2_GPIOH                ((uint32_t)0x40000000)           
#define  RCC_APB2PERIPH2_GPIOI                ((uint32_t)0x80000000)           

/**
  * @}
  */

/** @defgroup RCC_APB2_Peripherals2_RST 
  * @{
  */
/*GD32F20x*/
#define  RCC_APB2PERIPH2_USART6RST            ((uint32_t)0x01000000)            
#define  RCC_APB2PERIPH2_TLDIRST              ((uint32_t)0x04000000)            
#define  RCC_APB2PERIPH2_GPIOHRST             ((uint32_t)0x40000000)           
#define  RCC_APB2PERIPH2_GPIOIRST             ((uint32_t)0x80000000)           

/**
  * @}
  */

/** @defgroup RCC_APB1_peripheral2 
  * @{
  */
/*GD32F20x*/
#define  RCC_APB1PERIPH2_I2C3                 ((uint32_t)0x00800000)            
#define  RCC_APB1PERIPH2_UART7                ((uint32_t)0x40000000)            
#define  RCC_APB1PERIPH2_UART8                ((uint32_t)0x80000000)            

/**
  * @}
  */

/** @defgroup RCC_APB1_Peripherals2_RST 
  * @{
  */
/*GD32F20x*/
#define  RCC_APB1PERIPH2_I2C3RST              ((uint32_t)0x00800000)            
#define  RCC_APB1PERIPH2_UART7RST             ((uint32_t)0x40000000)            
#define  RCC_APB1PERIPH2_UART8RST             ((uint32_t)0x80000000)            

/**
  * @}
  */

/** @defgroup RCC_Flag 
  * @{
  */
/* The flag to check is in GCCR register */
#define RCC_FLAG_HSISTB                  ((uint8_t)0x21)
#define RCC_FLAG_HSESTB                  ((uint8_t)0x31)
#define RCC_FLAG_PLLSTB                  ((uint8_t)0x39)

/* The flag to check is in BDCR register */
#define RCC_FLAG_LSESTB                  ((uint8_t)0x41)

/* The flag to check is in GCSR register */
#define RCC_FLAG_LSISTB                  ((uint8_t)0x61)

#define RCC_FLAG_EPRST                   ((uint8_t)0x7A)
#define RCC_FLAG_POPDRST                 ((uint8_t)0x7B)
#define RCC_FLAG_SWRRST                  ((uint8_t)0x7C)
#define RCC_FLAG_IWDGRST                 ((uint8_t)0x7D)
#define RCC_FLAG_WWDGRST                 ((uint8_t)0x7E)
#define RCC_FLAG_LPRRST                  ((uint8_t)0x7F)

/*GD32F20x*/
#define RCC_FLAG_PLLTSTB                 ((uint8_t)0x9D)

/* The flag to check is in GCCR register */
#define RCC_FLAG_PLL2STB                 ((uint8_t)0x3B)
#define RCC_FLAG_PLL3STB                 ((uint8_t)0x3D)

/**
  * @}
  */


/** @defgroup Clock_source_to_output_on_CKOUT2_pin 
  * @{
  */

#define RCC_CKOUT2SRC_NOCLOCK                  ((uint32_t)0x00000000)
#define RCC_CKOUT2SRC_SYSCLK                   ((uint32_t)0x00040000)
#define RCC_CKOUT2SRC_HSI                      ((uint32_t)0x00050000)
#define RCC_CKOUT2SRC_HSE                      ((uint32_t)0x00060000)
#define RCC_CKOUT2SRC_PLLCLK_DIV2              ((uint32_t)0x00070000)
#define RCC_CKOUT2SRC_PLL2CLK                  ((uint32_t)0x00080000)
#define RCC_CKOUT2SRC_PLL3CLK_DIV2             ((uint32_t)0x00090000)
#define RCC_CKOUT2SRC_XT1                      ((uint32_t)0x000A0000)
#define RCC_CKOUT2SRC_PLL3CLK                  ((uint32_t)0x000B0000)

#define  RCC_CKOUT2DIV1                   ((uint32_t)0x00000000)        /*!< CKOUT2 clock divided by 1 */
#define  RCC_CKOUT2DIV2                   ((uint32_t)0x00000100)        /*!< CKOUT2 clock divided by 2 */
#define  RCC_CKOUT2DIV3                   ((uint32_t)0x00000200)        /*!< CKOUT2 clock divided by 3 */
#define  RCC_CKOUT2DIV4                   ((uint32_t)0x00000300)        /*!< CKOUT2 clock divided by 4 */
#define  RCC_CKOUT2DIV5                   ((uint32_t)0x00000400)        /*!< CKOUT2 clock divided by 5 */
#define  RCC_CKOUT2DIV6                   ((uint32_t)0x00000500)        /*!< CKOUT2 clock divided by 6 */
#define  RCC_CKOUT2DIV7                   ((uint32_t)0x00000600)        /*!< CKOUT2 clock divided by 7 */
#define  RCC_CKOUT2DIV8                   ((uint32_t)0x00000700)        /*!< CKOUT2 clock divided by 8 */
#define  RCC_CKOUT2DIV9                   ((uint32_t)0x00000800)        /*!< CKOUT2 clock divided by 9 */
#define  RCC_CKOUT2DIV10                  ((uint32_t)0x00000900)        /*!< CKOUT2 clock divided by 10 */
#define  RCC_CKOUT2DIV11                  ((uint32_t)0x00000A00)        /*!< CKOUT2 clock divided by 11 */
#define  RCC_CKOUT2DIV12                  ((uint32_t)0x00000B00)        /*!< CKOUT2 clock divided by 12 */
#define  RCC_CKOUT2DIV13                  ((uint32_t)0x00000C00)        /*!< CKOUT2 clock divided by 13 */
#define  RCC_CKOUT2DIV14                  ((uint32_t)0x00000D00)        /*!< CKOUT2 clock divided by 14 */
#define  RCC_CKOUT2DIV15                  ((uint32_t)0x00000E00)        /*!< CKOUT2 clock divided by 15 */
#define  RCC_CKOUT2DIV16                  ((uint32_t)0x00000F00)        /*!< CKOUT2 clock divided by 16 */
#define  RCC_CKOUT2DIV17                  ((uint32_t)0x00001000)        /*!< CKOUT2 clock divided by 17 */
#define  RCC_CKOUT2DIV18                  ((uint32_t)0x00001100)        /*!< CKOUT2 clock divided by 18 */
#define  RCC_CKOUT2DIV19                  ((uint32_t)0x00001200)        /*!< CKOUT2 clock divided by 19 */
#define  RCC_CKOUT2DIV20                  ((uint32_t)0x00001300)        /*!< CKOUT2 clock divided by 20 */
#define  RCC_CKOUT2DIV21                  ((uint32_t)0x00001400)        /*!< CKOUT2 clock divided by 21 */
#define  RCC_CKOUT2DIV22                  ((uint32_t)0x00001500)        /*!< CKOUT2 clock divided by 22 */
#define  RCC_CKOUT2DIV23                  ((uint32_t)0x00001600)        /*!< CKOUT2 clock divided by 23 */
#define  RCC_CKOUT2DIV24                  ((uint32_t)0x00001700)        /*!< CKOUT2 clock divided by 24 */
#define  RCC_CKOUT2DIV25                  ((uint32_t)0x00001800)        /*!< CKOUT2 clock divided by 25 */
#define  RCC_CKOUT2DIV26                  ((uint32_t)0x00001900)        /*!< CKOUT2 clock divided by 26 */
#define  RCC_CKOUT2DIV27                  ((uint32_t)0x00001A00)        /*!< CKOUT2 clock divided by 27 */
#define  RCC_CKOUT2DIV28                  ((uint32_t)0x00001B00)        /*!< CKOUT2 clock divided by 28 */
#define  RCC_CKOUT2DIV29                  ((uint32_t)0x00001C00)        /*!< CKOUT2 clock divided by 29 */
#define  RCC_CKOUT2DIV30                  ((uint32_t)0x00001D00)        /*!< CKOUT2 clock divided by 30 */
#define  RCC_CKOUT2DIV31                  ((uint32_t)0x00001E00)        /*!< CKOUT2 clock divided by 31 */
#define  RCC_CKOUT2DIV32                  ((uint32_t)0x00001F00)        /*!< CKOUT2 clock divided by 32 */
#define  RCC_CKOUT2DIV33                  ((uint32_t)0x00002000)        /*!< CKOUT2 clock divided by 33 */
#define  RCC_CKOUT2DIV34                  ((uint32_t)0x00002100)        /*!< CKOUT2 clock divided by 34 */
#define  RCC_CKOUT2DIV35                  ((uint32_t)0x00002200)        /*!< CKOUT2 clock divided by 35 */
#define  RCC_CKOUT2DIV36                  ((uint32_t)0x00002300)        /*!< CKOUT2 clock divided by 36 */
#define  RCC_CKOUT2DIV37                  ((uint32_t)0x00002400)        /*!< CKOUT2 clock divided by 37 */
#define  RCC_CKOUT2DIV38                  ((uint32_t)0x00002500)        /*!< CKOUT2 clock divided by 38 */
#define  RCC_CKOUT2DIV39                  ((uint32_t)0x00002600)        /*!< CKOUT2 clock divided by 39 */
#define  RCC_CKOUT2DIV40                  ((uint32_t)0x00002700)        /*!< CKOUT2 clock divided by 40 */
#define  RCC_CKOUT2DIV41                  ((uint32_t)0x00002800)        /*!< CKOUT2 clock divided by 41 */
#define  RCC_CKOUT2DIV42                  ((uint32_t)0x00002900)        /*!< CKOUT2 clock divided by 42 */
#define  RCC_CKOUT2DIV43                  ((uint32_t)0x00002A00)        /*!< CKOUT2 clock divided by 43 */
#define  RCC_CKOUT2DIV44                  ((uint32_t)0x00002B00)        /*!< CKOUT2 clock divided by 44 */
#define  RCC_CKOUT2DIV45                  ((uint32_t)0x00002C00)        /*!< CKOUT2 clock divided by 45 */
#define  RCC_CKOUT2DIV46                  ((uint32_t)0x00002D00)        /*!< CKOUT2 clock divided by 46 */
#define  RCC_CKOUT2DIV47                  ((uint32_t)0x00002E00)        /*!< CKOUT2 clock divided by 47 */
#define  RCC_CKOUT2DIV48                  ((uint32_t)0x00002F00)        /*!< CKOUT2 clock divided by 48 */
#define  RCC_CKOUT2DIV49                  ((uint32_t)0x00003000)        /*!< CKOUT2 clock divided by 49 */
#define  RCC_CKOUT2DIV50                  ((uint32_t)0x00003100)        /*!< CKOUT2 clock divided by 50 */
#define  RCC_CKOUT2DIV51                  ((uint32_t)0x00003200)        /*!< CKOUT2 clock divided by 51 */
#define  RCC_CKOUT2DIV52                  ((uint32_t)0x00003300)        /*!< CKOUT2 clock divided by 52 */
#define  RCC_CKOUT2DIV53                  ((uint32_t)0x00003400)        /*!< CKOUT2 clock divided by 53 */
#define  RCC_CKOUT2DIV54                  ((uint32_t)0x00003500)        /*!< CKOUT2 clock divided by 54 */
#define  RCC_CKOUT2DIV55                  ((uint32_t)0x00003600)        /*!< CKOUT2 clock divided by 55 */
#define  RCC_CKOUT2DIV56                  ((uint32_t)0x00003700)        /*!< CKOUT2 clock divided by 56 */
#define  RCC_CKOUT2DIV57                  ((uint32_t)0x00003800)        /*!< CKOUT2 clock divided by 57 */
#define  RCC_CKOUT2DIV58                  ((uint32_t)0x00003900)        /*!< CKOUT2 clock divided by 58 */
#define  RCC_CKOUT2DIV59                  ((uint32_t)0x00003A00)        /*!< CKOUT2 clock divided by 59 */
#define  RCC_CKOUT2DIV60                  ((uint32_t)0x00003B00)        /*!< CKOUT2 clock divided by 60 */
#define  RCC_CKOUT2DIV61                  ((uint32_t)0x00003C00)        /*!< CKOUT2 clock divided by 61 */
#define  RCC_CKOUT2DIV62                  ((uint32_t)0x00003D00)        /*!< CKOUT2 clock divided by 62 */
#define  RCC_CKOUT2DIV63                  ((uint32_t)0x00003E00)        /*!< CKOUT2 clock divided by 63 */
#define  RCC_CKOUT2DIV64                  ((uint32_t)0x00003F00)        /*!< CKOUT2 clock divided by 64 */
/**
  * @}
  */

/** @defgroup Clock_source_to_output_on_CKOUT_pin 
  * @{
  */
#define  RCC_CKOUTDIV1                   ((uint32_t)0x00000000)        /*!< CKOUT clock divided by 1 */
#define  RCC_CKOUTDIV2                   ((uint32_t)0x00000001)        /*!< CKOUT clock divided by 2 */
#define  RCC_CKOUTDIV3                   ((uint32_t)0x00000002)        /*!< CKOUT clock divided by 3 */
#define  RCC_CKOUTDIV4                   ((uint32_t)0x00000003)        /*!< CKOUT clock divided by 4 */
#define  RCC_CKOUTDIV5                   ((uint32_t)0x00000004)        /*!< CKOUT clock divided by 5 */
#define  RCC_CKOUTDIV6                   ((uint32_t)0x00000005)        /*!< CKOUT clock divided by 6 */
#define  RCC_CKOUTDIV7                   ((uint32_t)0x00000006)        /*!< CKOUT clock divided by 7 */
#define  RCC_CKOUTDIV8                   ((uint32_t)0x00000007)        /*!< CKOUT clock divided by 8 */
#define  RCC_CKOUTDIV9                   ((uint32_t)0x00000008)        /*!< CKOUT clock divided by 9 */
#define  RCC_CKOUTDIV10                  ((uint32_t)0x00000009)        /*!< CKOUT clock divided by 10 */
#define  RCC_CKOUTDIV11                  ((uint32_t)0x0000000A)        /*!< CKOUT clock divided by 11 */
#define  RCC_CKOUTDIV12                  ((uint32_t)0x0000000B)        /*!< CKOUT clock divided by 12 */
#define  RCC_CKOUTDIV13                  ((uint32_t)0x0000000C)        /*!< CKOUT clock divided by 13 */
#define  RCC_CKOUTDIV14                  ((uint32_t)0x0000000D)        /*!< CKOUT clock divided by 14 */
#define  RCC_CKOUTDIV15                  ((uint32_t)0x0000000E)        /*!< CKOUT clock divided by 15 */
#define  RCC_CKOUTDIV16                  ((uint32_t)0x0000000F)        /*!< CKOUT clock divided by 16 */
#define  RCC_CKOUTDIV17                  ((uint32_t)0x00000010)        /*!< CKOUT clock divided by 17 */
#define  RCC_CKOUTDIV18                  ((uint32_t)0x00000011)        /*!< CKOUT clock divided by 18 */
#define  RCC_CKOUTDIV19                  ((uint32_t)0x00000012)        /*!< CKOUT clock divided by 19 */
#define  RCC_CKOUTDIV20                  ((uint32_t)0x00000013)        /*!< CKOUT clock divided by 20 */
#define  RCC_CKOUTDIV21                  ((uint32_t)0x00000014)        /*!< CKOUT clock divided by 21 */
#define  RCC_CKOUTDIV22                  ((uint32_t)0x00000015)        /*!< CKOUT clock divided by 22 */
#define  RCC_CKOUTDIV23                  ((uint32_t)0x00000016)        /*!< CKOUT clock divided by 23 */
#define  RCC_CKOUTDIV24                  ((uint32_t)0x00000017)        /*!< CKOUT clock divided by 24 */
#define  RCC_CKOUTDIV25                  ((uint32_t)0x00000018)        /*!< CKOUT clock divided by 25 */
#define  RCC_CKOUTDIV26                  ((uint32_t)0x00000019)        /*!< CKOUT clock divided by 26 */
#define  RCC_CKOUTDIV27                  ((uint32_t)0x0000001A)        /*!< CKOUT clock divided by 27 */
#define  RCC_CKOUTDIV28                  ((uint32_t)0x0000001B)        /*!< CKOUT clock divided by 28 */
#define  RCC_CKOUTDIV29                  ((uint32_t)0x0000001C)        /*!< CKOUT clock divided by 29 */
#define  RCC_CKOUTDIV30                  ((uint32_t)0x0000001D)        /*!< CKOUT clock divided by 30 */
#define  RCC_CKOUTDIV31                  ((uint32_t)0x0000001E)        /*!< CKOUT clock divided by 31 */
#define  RCC_CKOUTDIV32                  ((uint32_t)0x0000001F)        /*!< CKOUT clock divided by 32 */
#define  RCC_CKOUTDIV33                  ((uint32_t)0x00000020)        /*!< CKOUT clock divided by 33 */
#define  RCC_CKOUTDIV34                  ((uint32_t)0x00000021)        /*!< CKOUT clock divided by 34 */
#define  RCC_CKOUTDIV35                  ((uint32_t)0x00000022)        /*!< CKOUT clock divided by 35 */
#define  RCC_CKOUTDIV36                  ((uint32_t)0x00000023)        /*!< CKOUT clock divided by 36 */
#define  RCC_CKOUTDIV37                  ((uint32_t)0x00000024)        /*!< CKOUT clock divided by 37 */
#define  RCC_CKOUTDIV38                  ((uint32_t)0x00000025)        /*!< CKOUT clock divided by 38 */
#define  RCC_CKOUTDIV39                  ((uint32_t)0x00000026)        /*!< CKOUT clock divided by 39 */
#define  RCC_CKOUTDIV40                  ((uint32_t)0x00000027)        /*!< CKOUT clock divided by 40 */
#define  RCC_CKOUTDIV41                  ((uint32_t)0x00000028)        /*!< CKOUT clock divided by 41 */
#define  RCC_CKOUTDIV42                  ((uint32_t)0x00000029)        /*!< CKOUT clock divided by 42 */
#define  RCC_CKOUTDIV43                  ((uint32_t)0x0000002A)        /*!< CKOUT clock divided by 43 */
#define  RCC_CKOUTDIV44                  ((uint32_t)0x0000002B)        /*!< CKOUT clock divided by 44 */
#define  RCC_CKOUTDIV45                  ((uint32_t)0x0000002C)        /*!< CKOUT clock divided by 45 */
#define  RCC_CKOUTDIV46                  ((uint32_t)0x0000002D)        /*!< CKOUT clock divided by 46 */
#define  RCC_CKOUTDIV47                  ((uint32_t)0x0000002E)        /*!< CKOUT clock divided by 47 */
#define  RCC_CKOUTDIV48                  ((uint32_t)0x0000002F)        /*!< CKOUT clock divided by 48 */
#define  RCC_CKOUTDIV49                  ((uint32_t)0x00000030)        /*!< CKOUT clock divided by 49 */
#define  RCC_CKOUTDIV50                  ((uint32_t)0x00000031)        /*!< CKOUT clock divided by 50 */
#define  RCC_CKOUTDIV51                  ((uint32_t)0x00000032)        /*!< CKOUT clock divided by 51 */
#define  RCC_CKOUTDIV52                  ((uint32_t)0x00000033)        /*!< CKOUT clock divided by 52 */
#define  RCC_CKOUTDIV53                  ((uint32_t)0x00000034)        /*!< CKOUT clock divided by 53 */
#define  RCC_CKOUTDIV54                  ((uint32_t)0x00000035)        /*!< CKOUT clock divided by 54 */
#define  RCC_CKOUTDIV55                  ((uint32_t)0x00000036)        /*!< CKOUT clock divided by 55 */
#define  RCC_CKOUTDIV56                  ((uint32_t)0x00000037)        /*!< CKOUT clock divided by 56 */
#define  RCC_CKOUTDIV57                  ((uint32_t)0x00000038)        /*!< CKOUT clock divided by 57 */
#define  RCC_CKOUTDIV58                  ((uint32_t)0x00000039)        /*!< CKOUT clock divided by 58 */
#define  RCC_CKOUTDIV59                  ((uint32_t)0x0000003A)        /*!< CKOUT clock divided by 59 */
#define  RCC_CKOUTDIV60                  ((uint32_t)0x0000003B)        /*!< CKOUT clock divided by 60 */
#define  RCC_CKOUTDIV61                  ((uint32_t)0x0000003C)        /*!< CKOUT clock divided by 61 */
#define  RCC_CKOUTDIV62                  ((uint32_t)0x0000003D)        /*!< CKOUT clock divided by 62 */
#define  RCC_CKOUTDIV63                  ((uint32_t)0x0000003E)        /*!< CKOUT clock divided by 63 */
#define  RCC_CKOUTDIV64                  ((uint32_t)0x0000003F)        /*!< CKOUT clock divided by 64 */
/**
  * @}
  */

/** @defgroup RCC_PLLT_clock_source 
  * @{
  */
#define  RCC_PLLTCLKSOURCE_HSI           ((uint32_t)0x00000000)        /*!< HSI selected as PLLT clock */
#define  RCC_PLLTCLKSOURCE_HSE           ((uint32_t)0x80000000)        /*!< HSE selected as PLLT clock */

/**
  * @}
  */

/** @defgroup TCDI prescaler selection 
  * @{
  */

#define  RCC_PLLT_TLDIPS_DIV2            ((uint32_t)0x00000000)        /*!< TLDI clock = PLLT_R/2 */
#define  RCC_PLLT_TLDIPS_DIV4            ((uint32_t)0x00010000)        /*!< TLDI clock = PLLT_R/4 */
#define  RCC_PLLT_TLDIPS_DIV8            ((uint32_t)0x00020000)        /*!< TLDI clock = PLLT_R/8 */
#define  RCC_PLLT_TLDIPS_DIV16           ((uint32_t)0x00030000)        /*!< TCDI clock = PLLT_R/16 */

/**
  * @}
  */

/**
  * @}
  */
  
/** @defgroup RCC_Exported_Functions
  * @{
  */
/* Reset the RCC clock configuration to the default reset state */

void RCC_DeInit(void);

/* Internal/external clocks, PLL, CKM and CK_OUT configuration functions */

void RCC_HSEConfig(uint32_t RCC_HSE);
TypeState RCC_WaitForHSEStartUp(void);
void RCC_AdjustHSICalibrationValue(uint8_t HSICalibrationValue);
void RCC_HSI_Enable(TypeState NewValue);
void RCC_PLLConfig(uint32_t RCC_PLLSelect, uint32_t RCC_PLLMF);
void RCC_PLL_Enable(TypeState NewValue);
void RCC_LSEConfig(uint32_t RCC_LSE);
void RCC_LSEDRVConfig(uint32_t RCC_LSEDRV);
void RCC_LSI_Enable(TypeState NewValue);
void RCC_HSEClockMonitor_Enable(TypeState NewValue);
void RCC_CKOUTSELConfig(uint32_t RCC_CKOUTSRC, uint32_t RCC_CKOUTDIVx);
void RCC_CKOUT2SELConfig(uint32_t RCC_CKOUT2SRC, uint32_t RCC_CKOUT2DIVx);
void RCC_PREDV1Config(uint32_t RCC_PREDV1_Source, uint32_t RCC_PREDV1_Div);
void RCC_PREDV2Config(uint32_t RCC_PREDV2_Div);
void RCC_PLL2Config(uint32_t RCC_PLL2MF);
void RCC_PLL2_Enable(TypeState NewValue);
void RCC_PLL3Config(uint32_t RCC_PLL3MF);
void RCC_PLL3_Enable(TypeState NewValue);

/* System, AHB, APB1 and APB2 busses clocks configuration functions */

void RCC_CK_SYSConfig(uint32_t RCC_SYSCLKSource);
uint8_t RCC_GetCK_SYSSource(void);
void RCC_AHBConfig(uint32_t RCC_CK_SYSDiv);
void RCC_APB1Config(uint32_t RCC_APB1);
void RCC_APB2Config(uint32_t RCC_APB2);

void RCC_OTGFSCLKConfig(uint32_t RCC_OTGFSCLK);

void RCC_ADCCLKConfig(uint32_t RCC_ADCCLK);

void RCC_I2S2CLKConfig(uint32_t RCC_I2S2CLK);                                  
void RCC_I2S3CLKConfig(uint32_t RCC_I2S3CLK);

void RCC_RTCCLKConfig(uint32_t RCC_RTCCLKSource);
void RCC_GetClocksFreq(RCC_ClocksPara* RCC_Clocks);

/* Peripheral clocks configuration functions */

void RCC_AHBPeriphClock_Enable(uint32_t RCC_AHBPeriph, TypeState NewValue);
void RCC_APB2PeriphClock_Enable(uint32_t RCC_APB2Periph, TypeState NewValue);
void RCC_APB1PeriphClock_Enable(uint32_t RCC_APB1Periph, TypeState NewValue);
void RCC_RTCCLK_Enable(TypeState NewValue);

void RCC_AHBPeriphReset_Enable(uint32_t RCC_AHBPeriphRST, TypeState NewValue);

void RCC_APB2PeriphReset_Enable(uint32_t RCC_APB2PeriphRST, TypeState NewValue);
void RCC_APB1PeriphReset_Enable(uint32_t RCC_APB1PeriphRST, TypeState NewValue);
void RCC_BackupReset_Enable(TypeState NewValue);

/* Interrupts and flags management functions */

void RCC_INTConfig(uint32_t RCC_INT, TypeState NewValue);
TypeState RCC_GetIntBitState(uint32_t RCC_INT);
void RCC_ClearIntBitState(uint32_t RCC_INT);
TypeState RCC_GetBitState(uint8_t RCC_FLAG);
void RCC_ClearBitState(void);

void RCC_KERNELVOLConfig(uint32_t RCC_KERNEL_VOL);

/* Peripheral clocks configuration functions */

void RCC_AHB2PeriphClock_Enable(uint32_t RCC_AHB2Periph, TypeState NewValue);
void RCC_APB2Periph2Clock_Enable(uint32_t RCC_APB2Periph2, TypeState NewValue);
void RCC_APB1Periph2Clock_Enable(uint32_t RCC_APB1Periph2, TypeState NewValue);

void RCC_AHB2PeriphReset_Enable(uint32_t RCC_AHB2PeriphRST, TypeState NewValue);
void RCC_APB2Periph2Reset_Enable(uint32_t RCC_APB2Periph2RST, TypeState NewValue);
void RCC_APB1Periph2Reset_Enable(uint32_t RCC_APB1Periph2RST, TypeState NewValue);

void RCC_PLLT_Enable(TypeState NewValue);
void RCC_PLLTCLKConfig(uint32_t RCC_PLLTCLKSource);
void RCC_PLLTVCOConfig(uint32_t PLLTPS, uint32_t PLLTMF, uint32_t PLLTRPS);
void RCC_TLDICLKDivConfig(uint32_t RCC_PLLT_TLDIPS_Divx);

#ifdef __cplusplus
}
#endif

#endif /* __GD32F20x_RCC_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2015 GIGADEVICE *****END OF FILE****/
