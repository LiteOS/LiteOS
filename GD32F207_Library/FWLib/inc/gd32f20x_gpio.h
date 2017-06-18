/**
  ******************************************************************************
  * @file    gd32f20x_gpio.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    15-Jul-2015
  * @brief   GPIO header file of the firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F20x_GPIO_H
#define __GD32F20x_GPIO_H

#ifdef __cplusplus
 extern "C" {
#endif 
  
/* Includes ------------------------------------------------------------------*/
#include "gd32f20x.h"

/** @addtogroup GD32F20x_Firmware
  * @{
  */

/** @addtogroup GPIO
  * @{
  */

/** @defgroup GPIO_Exported_Types
  * @{
  */

/** 
  * @brief Output_Maximum_frequency_enumeration
  */
typedef enum
{
    GPIO_SPEED_10MHZ = 1,
    GPIO_SPEED_2MHZ,
    GPIO_SPEED_50MHZ
}GPIO_SpeedPara;

/** 
  * @brief GPIO_Mode_enumeration
  */
typedef enum
{
    GPIO_MODE_AIN = 0x0,
    GPIO_MODE_IN_FLOATING = 0x04,
    GPIO_MODE_IPD = 0x28,
    GPIO_MODE_IPU = 0x48,
    GPIO_MODE_OUT_OD = 0x14,
    GPIO_MODE_OUT_PP = 0X10,
    GPIO_MODE_AF_OD = 0X1C,
    GPIO_MODE_AF_PP = 0X18
}GPIO_ModePara;

/** 
  * @brief GPIO Initial Parameters
  */
typedef struct
{
    uint16_t GPIO_Pin;                  /*!< The GPIO pins to be configured. choose several from @ref GPIO_pins_define */
    GPIO_SpeedPara GPIO_Speed;          /*!< The speed for This parameter can be a value of @ref GPIOSpeed_TypeDef */
    GPIO_ModePara GPIO_Mode;            /*!< The operating mode for the selected pins. choose one from @ref GPIO_ModePara */
}GPIO_InitPara;

/** 
  * @brief Bit_State_enumeration
  */
typedef enum
{
    Bit_RESET = 0,
    Bit_SET
}BitState;

/**
  * @}
  */

/** @defgroup GPIO_Exported_Constants
  * @{
  */

/** @defgroup GPIO_pins_define
  * @{
  */
#define GPIO_PIN_0                      ((uint16_t)0x0001)
#define GPIO_PIN_1                      ((uint16_t)0x0002)
#define GPIO_PIN_2                      ((uint16_t)0x0004)
#define GPIO_PIN_3                      ((uint16_t)0x0008)
#define GPIO_PIN_4                      ((uint16_t)0x0010)
#define GPIO_PIN_5                      ((uint16_t)0x0020)
#define GPIO_PIN_6                      ((uint16_t)0x0040)
#define GPIO_PIN_7                      ((uint16_t)0x0080)
#define GPIO_PIN_8                      ((uint16_t)0x0100)
#define GPIO_PIN_9                      ((uint16_t)0x0200)
#define GPIO_PIN_10                     ((uint16_t)0x0400)
#define GPIO_PIN_11                     ((uint16_t)0x0800)
#define GPIO_PIN_12                     ((uint16_t)0x1000)
#define GPIO_PIN_13                     ((uint16_t)0x2000)
#define GPIO_PIN_14                     ((uint16_t)0x4000)
#define GPIO_PIN_15                     ((uint16_t)0x8000)
#define GPIO_PIN_ALL                    ((uint16_t)0xFFFF)

/**
  * @}
  */

/** @defgroup GPIO_Remap_define
  * @{
  */
#define GPIO_REMAP_SPI1                 ((uint32_t)0x00000001)
#define GPIO_REMAP_I2C1                 ((uint32_t)0x00000002)
#define GPIO_REMAP_USART1               ((uint32_t)0x00000004)
#define GPIO_REMAP_USART2               ((uint32_t)0x00000008)
#define GPIO_PARTIAL_REMAP_USART3       ((uint32_t)0x00140010)
#define GPIO_FULL_REMAP_USART3          ((uint32_t)0x00140030)
#define GPIO_PARTIAL_REMAP_TIMER1       ((uint32_t)0x00160040)
#define GPIO_FULL_REMAP_TIMER1          ((uint32_t)0x001600C0)
#define GPIO_PARTIAL_REMAP1_TIMER2      ((uint32_t)0x00180100)
#define GPIO_PARTIAL_REMAP2_TIMER2      ((uint32_t)0x00180200)
#define GPIO_FULL_REMAP_TIMER2          ((uint32_t)0x00180300)
#define GPIO_PARTIAL_REMAP_TIMER3       ((uint32_t)0x001A0800)
#define GPIO_FULL_REMAP_TIMER3          ((uint32_t)0x001A0C00)
#define GPIO_REMAP_TIMER4               ((uint32_t)0x00001000)
#define GPIO_REMAP1_CAN1                ((uint32_t)0x001D4000)
#define GPIO_REMAP2_CAN1                ((uint32_t)0x001D6000)
#define GPIO_REMAP_PD01                 ((uint32_t)0x00008000)
#define GPIO_REMAP_TIMER5CH4_LSI        ((uint32_t)0x00200001)
#define GPIO_REMAP_ADC1_ETRGINJ         ((uint32_t)0x00200002)
#define GPIO_REMAP_ADC1_ETRGREG         ((uint32_t)0x00200004)
#define GPIO_REMAP_ADC2_ETRGINJ         ((uint32_t)0x00200008)
#define GPIO_REMAP_ADC2_ETRGREG         ((uint32_t)0x00200010)
#define GPIO_REMAP_ETH                  ((uint32_t)0x00200020)
#define GPIO_REMAP_CAN2                 ((uint32_t)0x00200040)
#define GPIO_REMAP_SWJ_NOJTRST          ((uint32_t)0x00300100)
#define GPIO_REMAP_SWJ_JTAGDISABLE      ((uint32_t)0x00300200)
#define GPIO_REMAP_SWJ_DISABLE          ((uint32_t)0x00300400)
#define GPIO_REMAP_SPI3                 ((uint32_t)0x00201100)
#define GPIO_REMAP_TIMER2ITR1_PTP_SOF   ((uint32_t)0x00202000)
#define GPIO_REMAP_PTP_PPS              ((uint32_t)0x00204000)
#define GPIO_REMAP_TIMER15              ((uint32_t)0x80000001)
#define GPIO_REMAP_TIMER16              ((uint32_t)0x80000002)
#define GPIO_REMAP_TIMER17              ((uint32_t)0x80000004)
#define GPIO_REMAP_CEC                  ((uint32_t)0x80000008)
#define GPIO_REMAP_TIMER1_DMA           ((uint32_t)0x80000010)
#define GPIO_REMAP_TIMER9               ((uint32_t)0x80000020)
#define GPIO_REMAP_TIMER10              ((uint32_t)0x80000040)
#define GPIO_REMAP_TIMER11              ((uint32_t)0x80000080)
#define GPIO_REMAP_TIMER13              ((uint32_t)0x80000100)
#define GPIO_REMAP_TIMER14              ((uint32_t)0x80000200)
#define GPIO_REMAP_EXMC_NADV            ((uint32_t)0x80000400)
#define GPIO_REMAP_TIMER67_DAC_DMA      ((uint32_t)0x80000800)
#define GPIO_REMAP_TIMER12              ((uint32_t)0x80001000)
#define GPIO_REMAP_MISC                 ((uint32_t)0x80002000)
#define PCFR3_REMAP_PH01                ((uint32_t)0x80000000)
#define PCFR3_REMAP_DCI_HSYNC           ((uint32_t)0x20000000)
#define PCFR3_REMAP_DCI_D13_PG15        ((uint32_t)0x08000000)
#define PCFR3_REMAP_DCI_D13_PI0         ((uint32_t)0x18000000)
#define PCFR3_REMAP_DCI_D12             ((uint32_t)0x04000000)
#define PCFR3_REMAP_DCI_D11_PF10        ((uint32_t)0x01000000)
#define PCFR3_REMAP_DCI_D11_PH15        ((uint32_t)0x03000000)
#define PCFR3_REMAP_DCI_D10_PD6         ((uint32_t)0x00400000)
#define PCFR3_REMAP_DCI_D10_PI3         ((uint32_t)0x00C00000)
#define PCFR3_REMAP_DCI_D9_PH7          ((uint32_t)0x00100000)
#define PCFR3_REMAP_DCI_D9_PI2          ((uint32_t)0x00300000)
#define PCFR3_REMAP_DCI_D8_PH6          ((uint32_t)0x00040000)
#define PCFR3_REMAP_DCI_D8_PI1          ((uint32_t)0x000C0000)
#define PCFR3_REMAP_DCI_D7_PE6          ((uint32_t)0x00010000)
#define PCFR3_REMAP_DCI_D7_PI7          ((uint32_t)0x00030000)
#define PCFR3_REMAP_DCI_D6_PE5          ((uint32_t)0x00004000)
#define PCFR3_REMAP_DCI_D6_PI6          ((uint32_t)0x0000C000)
#define PCFR3_REMAP_DCI_D5_PD3          ((uint32_t)0x00001000)
#define PCFR3_REMAP_DCI_D5_PI4          ((uint32_t)0x00003000)
#define PCFR3_REMAP_DCI_D4_PE4          ((uint32_t)0x00000400)
#define PCFR3_REMAP_DCI_D4_PH14         ((uint32_t)0x00000C00)
#define PCFR3_REMAP_DCI_D3_PE1          ((uint32_t)0x00000100)
#define PCFR3_REMAP_DCI_D3_PG11         ((uint32_t)0x00000200)
#define PCFR3_REMAP_DCI_D3_PH12         ((uint32_t)0x00000300)
#define PCFR3_REMAP_DCI_D2_PE0          ((uint32_t)0x00000040)
#define PCFR3_REMAP_DCI_D2_PG10         ((uint32_t)0x00000080)
#define PCFR3_REMAP_DCI_D2_PH11         ((uint32_t)0x000000C0)
#define PCFR3_REMAP_DCI_D1_PC7          ((uint32_t)0x00000010)
#define PCFR3_REMAP_DCI_D1_PH10         ((uint32_t)0x00000030)
#define PCFR3_REMAP_DCI_D0_PC6          ((uint32_t)0x00000004)
#define PCFR3_REMAP_DCI_D0_PH9          ((uint32_t)0x0000000C)
#define PCFR3_REMAP_DCI_VSYNC_PG9       ((uint32_t)0x00000001)
#define PCFR3_REMAP_DCI_VSYNC_PI5       ((uint32_t)0x00000003)
#define PCFR4_REMAP_LCD_B3_PG11         ((uint32_t)0x80000000)
#define PCFR4_REMAP_LCD_B2_PG10         ((uint32_t)0x40000000)
#define PCFR4_REMAP_LCD_G3_PG10         ((uint32_t)0x20000000)
#define PCFR4_REMAP_LCD_CLK_PG7         ((uint32_t)0x10000000)
#define PCFR4_REMAP_LCD_R7_PG6          ((uint32_t)0x08000000)
#define PCFR4_REMAP_LCD_DE_PF10         ((uint32_t)0x04000000)
#define PCFR4_REMAP_LCD_R7_PE15         ((uint32_t)0x02000000)
#define PCFR4_REMAP_LCD_CLK_PE14        ((uint32_t)0x01000000)
#define PCFR4_REMAP_LCD_DE_PE13         ((uint32_t)0x00800000)
#define PCFR4_REMAP_LCD_B4_PE12         ((uint32_t)0x00400000)
#define PCFR4_REMAP_LCD_G3_PE11         ((uint32_t)0x00200000)
#define PCFR4_REMAP_LCD_G1_PE6          ((uint32_t)0x00100000)
#define PCFR4_REMAP_LCD_G0_PE5          ((uint32_t)0x00080000)
#define PCFR4_REMAP_LCD_B0_PE4          ((uint32_t)0x00040000)
#define PCFR4_REMAP_LCD_B3_PD10         ((uint32_t)0x00020000)
#define PCFR4_REMAP_LCD_B2_PD6          ((uint32_t)0x00010000)
#define PCFR4_REMAP_LCD_G7_PD3          ((uint32_t)0x00008000)
#define PCFR4_REMAP_LCD_R2_PC10         ((uint32_t)0x00004000)
#define PCFR4_REMAP_LCD_G6_PC7          ((uint32_t)0x00002000)
#define PCFR4_REMAP_LCD_HSYNC_PC6       ((uint32_t)0x00001000)
#define PCFR4_REMAP_LCD_G5_PB11         ((uint32_t)0x00000800)
#define PCFR4_REMAP_LCD_G4_PB10         ((uint32_t)0x00000400)
#define PCFR4_REMAP_LCD_B7_PB9          ((uint32_t)0x00000200)
#define PCFR4_REMAP_LCD_B6_PB8          ((uint32_t)0x00000100)
#define PCFR4_REMAP_LCD_R6_PB1          ((uint32_t)0x00000080)
#define PCFR4_REMAP_LCD_R3_PB0          ((uint32_t)0x00000040)
#define PCFR4_REMAP_LCD_R5_PA12         ((uint32_t)0x00000020)
#define PCFR4_REMAP_LCD_R4_PA11         ((uint32_t)0x00000010)
#define PCFR4_REMAP_LCD_R6_PA8          ((uint32_t)0x00000008)
#define PCFR4_REMAP_LCD_G2_PA6          ((uint32_t)0x00000004)
#define PCFR4_REMAP_LCD_VSYNC_PA4       ((uint32_t)0x00000002)
#define PCFR4_REMAP_LCD_B5_PA3          ((uint32_t)0x00000001)
#define PCFR5_REMAP_SPI3_MOSI           ((uint32_t)0x01000000)
#define PCFR5_REMAP_SPI2_SCK            ((uint32_t)0x00800000)
#define PCFR5_REMAP_LCD_R1_PI3          ((uint32_t)0x00400000)
#define PCFR5_REMAP_LCD_R0_PH4          ((uint32_t)0x00200000)
#define PCFR5_REMAP_LCD_HSYNC_PI10      ((uint32_t)0x00100000)
#define PCFR5_REMAP_LCD_VSYNC_PI9       ((uint32_t)0x00080000)
#define PCFR5_REMAP_LCD_B7_PI07         ((uint32_t)0x00040000)
#define PCFR5_REMAP_LCD_B6_PI06         ((uint32_t)0x00020000)
#define PCFR5_REMAP_LCD_B5_PI5          ((uint32_t)0x00010000)
#define PCFR5_REMAP_LCD_B4_PI4          ((uint32_t)0x00008000)
#define PCFR5_REMAP_LCD_G7_PI2          ((uint32_t)0x00004000)
#define PCFR5_REMAP_LCD_G6_PI1          ((uint32_t)0x00002000)
#define PCFR5_REMAP_LCD_G5_PI0          ((uint32_t)0x00001000)
#define PCFR5_REMAP_LCD_G4_PH15         ((uint32_t)0x00000800)
#define PCFR5_REMAP_LCD_G3_PH14         ((uint32_t)0x00000400)
#define PCFR5_REMAP_LCD_G2_PH13         ((uint32_t)0x00000200)
#define PCFR5_REMAP_LCD_R6_PH12         ((uint32_t)0x00000100)
#define PCFR5_REMAP_LCD_R5_PH11         ((uint32_t)0x00000080)
#define PCFR5_REMAP_LCD_R4_PH10         ((uint32_t)0x00000040)
#define PCFR5_REMAP_LCD_R3_PH9          ((uint32_t)0x00000020)
#define PCFR5_REMAP_LCD_R2_PH8          ((uint32_t)0x00000010)
#define PCFR5_REMAP_LCD_R1_PH3          ((uint32_t)0x00000008)
#define PCFR5_REMAP_LCD_R0_PH2          ((uint32_t)0x00000004)
#define PCFR5_REMAP_LCD_B1_PG12         ((uint32_t)0x00000002)
#define PCFR5_REMAP_LCD_B4_PG12         ((uint32_t)0x00000001)
#define PCFR6_REMAP_EXMC_SDNE1_PB6      ((uint32_t)0x80000000)
#define PCFR6_REMAP_EXMC_SDNE0_PC2      ((uint32_t)0x40000000)
#define PCFR6_REMAP_EXMC_SDCKE1_PB5     ((uint32_t)0x20000000)
#define PCFR6_REMAP_EXMC_SDCKE0_PC3     ((uint32_t)0x10000000)
#define PCFR6_REMAP_EXMC_SDNWE_PC0      ((uint32_t)0x08000000)
#define PCFR6_REMAP_USART6_RX_PG9       ((uint32_t)0x04000000)
#define PCFR6_REMAP_USART6_TX_PG14      ((uint32_t)0x02000000)
#define PCFR6_REMAP_USART6_CTS_PG13     ((uint32_t)0x01000000)
#define PCFR6_REMAP_USART6_RTS_PG12     ((uint32_t)0x00800000)
#define PCFR6_REMAP_USART6_CK_PG7       ((uint32_t)0x00400000)
#define PCFR6_REMAP_USART7              ((uint32_t)0x00200000)
#define PCFR6_REMAP_ETH_RX_HI           ((uint32_t)0x00100000)
#define PCFR6_REMAP_ETH_CRSCOL          ((uint32_t)0x00080000)
#define PCFR6_REMAP_ETH_TXD01           ((uint32_t)0x00040000)
#define PCFR6_REMAP_ETH_PPS_HI          ((uint32_t)0x00020000)
#define PCFR6_REMAP_ETH_TXD3            ((uint32_t)0x00010000)
#define PCFR6_REMAP_CAN_HI              ((uint32_t)0x00008000)
#define PCFR6_REMAP_TM12                ((uint32_t)0x00004000)
#define PCFR6_REMAP_USART4              ((uint32_t)0x00002000)
#define PCFR6_REMAP_SPI2_IO_1           ((uint32_t)0x00001800)
#define PCFR6_REMAP_SPI2_IO_2           ((uint32_t)0x00000800)
#define PCFR6_REMAP_SPI2_NSCK_1         ((uint32_t)0x00000600)
#define PCFR6_REMAP_SPI2_NSCK_2         ((uint32_t)0x00000400)
#define PCFR6_REMAP_I2C2_1              ((uint32_t)0x00000180)
#define PCFR6_REMAP_I2C2_2              ((uint32_t)0x00000080)
#define PCFR6_REMAP_TM8                 ((uint32_t)0x00000040)
#define PCFR6_REMAP_TM8_CHN_1           ((uint32_t)0x00000030)
#define PCFR6_REMAP_TM8_CHN_2           ((uint32_t)0x00000020)
#define PCFR6_REMAP_TM5                 ((uint32_t)0x00000008)
#define PCFR6_REMAP_TM2_CH1             ((uint32_t)0x00000004)
#define PCFR6_REMAP_I2C3_REMAP2_EN      ((uint32_t)0x00000002)
#define PCFR6_REMAP_I2C3_REMAP1_EN      ((uint32_t)0x00000001)

/**
  * @}
  */

/** @defgroup GPIO_REMAP_Register
  * @{
  */
#define PCFR3 3
#define PCFR4 4
#define PCFR5 5
#define PCFR6 6

/**
  * @}
  */

/** @defgroup GPIO_Port_Sources
  * @{
  */
#define GPIO_PORT_SOURCE_GPIOA          ((uint8_t)0x00)
#define GPIO_PORT_SOURCE_GPIOB          ((uint8_t)0x01)
#define GPIO_PORT_SOURCE_GPIOC          ((uint8_t)0x02)
#define GPIO_PORT_SOURCE_GPIOD          ((uint8_t)0x03)
#define GPIO_PORT_SOURCE_GPIOE          ((uint8_t)0x04)
#define GPIO_PORT_SOURCE_GPIOF          ((uint8_t)0x05)
#define GPIO_PORT_SOURCE_GPIOG          ((uint8_t)0x06)
#define GPIO_PORT_SOURCE_GPIOH          ((uint8_t)0x07)
#define GPIO_PORT_SOURCE_GPIOI          ((uint8_t)0x08)

/**
  * @}
  */

/** @defgroup GPIO_Pin_sources
  * @{
  */
#define GPIO_PINSOURCE0                 ((uint8_t)0x00)
#define GPIO_PINSOURCE1                 ((uint8_t)0x01)
#define GPIO_PINSOURCE2                 ((uint8_t)0x02)
#define GPIO_PINSOURCE3                 ((uint8_t)0x03)
#define GPIO_PINSOURCE4                 ((uint8_t)0x04)
#define GPIO_PINSOURCE5                 ((uint8_t)0x05)
#define GPIO_PINSOURCE6                 ((uint8_t)0x06)
#define GPIO_PINSOURCE7                 ((uint8_t)0x07)
#define GPIO_PINSOURCE8                 ((uint8_t)0x08)
#define GPIO_PINSOURCE9                 ((uint8_t)0x09)
#define GPIO_PINSOURCE10                ((uint8_t)0x0A)
#define GPIO_PINSOURCE11                ((uint8_t)0x0B)
#define GPIO_PINSOURCE12                ((uint8_t)0x0C)
#define GPIO_PINSOURCE13                ((uint8_t)0x0D)
#define GPIO_PINSOURCE14                ((uint8_t)0x0E)
#define GPIO_PINSOURCE15                ((uint8_t)0x0F)

/**
  * @}
  */

/** @defgroup Ethernet_Media_Interface
  * @{
  */
#define GPIO_ETH_MEDIAINTERFACE_MII     ((uint32_t)0x00000000)
#define GPIO_ETH_MEDIAINTERFACE_RMII    ((uint32_t)0x00800000)

/**
  * @}
  */

/** @defgroup AFIO_Event_Output
  * @{
  */
#define AFIO_EVR_EVOE_SET               ((uint32_t)0x00000080)
#define AFIO_EVR_EVOE_RESET             ((uint32_t)0xffffff7f)

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup GPIO_Exported_Functions
  * @{
  */
void GPIO_DeInit(GPIO_TypeDef* GPIOx);
void GPIO_AFDeInit(void);
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitPara* GPIO_InitStruct);
void GPIO_ParaInit(GPIO_InitPara* GPIO_InitStruct);
uint8_t GPIO_ReadInputBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);
uint8_t GPIO_ReadOutputBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitState BitVal);
void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal);
void GPIO_PinLockConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_EventOutputConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource);
void GPIO_EventOutputEnable(TypeState NewState);
void GPIO_PinRemapConfig(uint32_t GPIO_Remap, TypeState NewState);
void GPIO_PinRemapConfig2(uint8_t Remap_Reg,uint32_t GPIO_Remap, TypeState NewState);
void GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource);
void GPIO_ETH_MediaInterfaceConfig(uint32_t GPIO_ETH_MediaInterface);

#ifdef __cplusplus
}
#endif

#endif /* __GD32F20X_GPIO_H */
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
