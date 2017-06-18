/**
  ******************************************************************************
  * @file    gd32f20x.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    15-Jul-2015
  * @brief   CMSIS Cortex-M3 Device Peripheral Access Layer Header File..
  ******************************************************************************
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup GD32f20X
  * @{
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F20X_H
#define __GD32F20X_H

#ifdef __cplusplus
 extern "C" {
#endif 
  
/** @addtogroup Library_configuration_section
  * @{
  */
  
/* Uncomment the line below according to the target gd32f20x device used in your 
   application 
  */




#if !defined (GD32F20X_CL)
 #error "Please select first the target gd32f20x device used in your application (in gd32f20x.h file)"
#endif /* GD32F20X */

#if !defined  USE_STDPERIPH_DRIVER
/**
 * @brief Comment the line below if you will not use the peripherals drivers.
   In this case, these drivers will not be included and the application code will 
   be based on direct access to peripherals registers 
   */
  /*#define USE_STDPERIPH_DRIVER*/
#endif /* USE_STDPERIPH_DRIVER */

/**
 * @brief In the following line adjust the value of External High Speed oscillator (HSE)
   used in your application 
   
   Tip: To avoid modifying this file each time you need to use different HSE, you
        can define the HSE value in your toolchain compiler preprocessor.
  */
#if !defined  HSE_VALUE    
#ifdef GD32F20X_CL   
#define HSE_VALUE    ((uint32_t)25000000) /* !< From 4M to 25M *!< Value of the External oscillator in Hz*/
#endif /* HSE_VALUE */
#endif

#define HSE_STARTUP_TIMEOUT   ((uint16_t)0xFFFF) /*!< Time out for HSI start up */


#if !defined  (HSI_VALUE) 
#define HSI_VALUE  ((uint32_t)8000000)       /*!< Value of the Internal High Speed oscillator in Hz.
                                                  The real value may vary depending on the variations
                                                  in voltage and temperature.  */
#endif /* HSI_VALUE */


#if !defined  (LSI_VALUE) 
#define LSI_VALUE  ((uint32_t)40000)         /*!< Value of the Internal Low Speed oscillator in Hz
                                                  The real value may vary depending on the variations
                                                  in voltage and temperature.  */
#endif /* LSI_VALUE */

#if !defined  (LSE_VALUE) 
#define LSE_VALUE  ((uint32_t)32768)         /*!< Value of the External Low Speed oscillator in Hz */
#endif /* LSE_VALUE */

/**
 * @brief GD32F20X Firmware Library version number V1.0
   */
#define __GD32F20X_STDPERIPH_VERSION_MAIN   (0x01) /*!< [31:24] main version */
#define __GD32F20X_STDPERIPH_VERSION_SUB1   (0x00) /*!< [23:16] sub1 version */
#define __GD32F20X_STDPERIPH_VERSION_SUB2   (0x00) /*!< [15:8]  sub2 version */
#define __GD32F20X_STDPERIPH_VERSION_RC     (0x00) /*!< [7:0]  release candidate */ 
#define __GD32F20X_STDPERIPH_VERSION        ((__GD32F20X_STDPERIPH_VERSION_MAIN << 24)\
                                             |(__GD32F20X_STDPERIPH_VERSION_SUB1 << 16)\
                                             |(__GD32F20X_STDPERIPH_VERSION_SUB2 << 8)\
                                             |(__GD32F20X_STDPERIPH_VERSION_RC))

/**
  * @}
  */

/** @addtogroup Configuration_section_for_CMSIS
  * @{
  */

/**
 * @brief Configuration of the Cortex-M3 Processor and Core Peripherals 
 */
#define __MPU_PRESENT             0        /*!< GD32 devices does not provide an MPU */
#define __NVIC_PRIO_BITS          4        /*!< GD32F20X uses 4 Bits for the Priority Levels             */
#define __VENDOR_SYSTICKCONFIG    0        /*!< Set to 1 if different SysTick Config is used             */

/*!< Interrupt Number Definition */
typedef enum IRQn
{
/******  Cortex-M3 Processor Exceptions Numbers ***************************************************/
    NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                             */
    MemoryManagement_IRQn       = -12,    /*!< 4 Cortex-M3 Memory Management Interrupt              */
    BusFault_IRQn               = -11,    /*!< 5 Cortex-M3 Bus Fault Interrupt                      */
    UsageFault_IRQn             = -10,    /*!< 6 Cortex-M3 Usage Fault Interrupt                    */
    SVCall_IRQn                 = -5,     /*!< 11 Cortex-M3 SV Call Interrupt                       */
    DebugMonitor_IRQn           = -4,     /*!< 12 Cortex-M3 Debug Monitor Interrupt                 */
    PendSV_IRQn                 = -2,     /*!< 14 Cortex-M3 Pend SV Interrupt                       */
    SysTick_IRQn                = -1,     /*!< 15 Cortex-M3 System Tick Interrupt                   */

/******  GD32F20X specific Interrupt Numbers *********************************************************/
    WWDG_IRQn                   = 0,      /*!< Window WatchDog Interrupt                            */
    LVD_IRQn                    = 1,      /*!< PVD through EXTI Line detection Interrupt            */
    TAMPER_IRQn                 = 2,      /*!< Tamper Interrupt                                     */
    RTC_IRQn                    = 3,      /*!< RTC global Interrupt                                 */
    FLASH_IRQn                  = 4,      /*!< FLASH global Interrupt                               */
    RCC_IRQn                    = 5,      /*!< RCC global Interrupt                                 */
    EXTI0_IRQn                  = 6,      /*!< EXTI Line0 Interrupt                                 */
    EXTI1_IRQn                  = 7,      /*!< EXTI Line1 Interrupt                                 */
    EXTI2_IRQn                  = 8,      /*!< EXTI Line2 Interrupt                                 */
    EXTI3_IRQn                  = 9,      /*!< EXTI Line3 Interrupt                                 */
    EXTI4_IRQn                  = 10,     /*!< EXTI Line4 Interrupt                                 */
    DMA1_Channel1_IRQn          = 11,     /*!< DMA1 Channel 1 global Interrupt                      */
    DMA1_Channel2_IRQn          = 12,     /*!< DMA1 Channel 2 global Interrupt                      */
    DMA1_Channel3_IRQn          = 13,     /*!< DMA1 Channel 3 global Interrupt                      */
    DMA1_Channel4_IRQn          = 14,     /*!< DMA1 Channel 4 global Interrupt                      */
    DMA1_Channel5_IRQn          = 15,     /*!< DMA1 Channel 5 global Interrupt                      */
    DMA1_Channel6_IRQn          = 16,     /*!< DMA1 Channel 6 global Interrupt                      */
    DMA1_Channel7_IRQn          = 17,     /*!< DMA1 Channel 7 global Interrupt                      */
    ADC1_2_IRQn                 = 18,     /*!< ADC1 and ADC2 global Interrupt                       */
    CAN1_TX_IRQn                = 19,     /*!< CAN1 TX Interrupt                                    */
    CAN1_RX0_IRQn               = 20,     /*!< CAN1 RX0 Interrupt                                   */
    CAN1_RX1_IRQn               = 21,     /*!< CAN1 RX1 Interrupt                                   */
    CAN1_SCE_IRQn               = 22,     /*!< CAN1 SCE Interrupt                                   */
    EXTI9_5_IRQn                = 23,     /*!< External Line[9:5] Interrupts                        */
    TIMER1_BRK_TIMER9_IRQn      = 24,     /*!< TIMER1 Break Interrupt and TIMER9 global Interrupt   */
    TIMER1_UP_TIMER10_IRQn      = 25,     /*!< TIMER1 Update Interrupt and TIMER10 global Interrupt */
    TIMER1_TRG_COM_TIMER11_IRQn = 26,     /*!< TIMER1 Trigger and Commutation Interrupt and TIMER11 global Interrupt */
    TIMER1_CC_IRQn              = 27,     /*!< TIMER1 Capture Compare Interrupt                     */
    TIMER2_IRQn                 = 28,     /*!< TIMER2 global Interrupt                              */
    TIMER3_IRQn                 = 29,     /*!< TIMER3 global Interrupt                              */
    TIMER4_IRQn                 = 30,     /*!< TIMER4 global Interrupt                              */
    I2C1_EV_IRQn                = 31,     /*!< I2C1 Event Interrupt                                 */
    I2C1_ER_IRQn                = 32,     /*!< I2C1 Error Interrupt                                 */
    I2C2_EV_IRQn                = 33,     /*!< I2C2 Event Interrupt                                 */
    I2C2_ER_IRQn                = 34,     /*!< I2C2 Error Interrupt                                 */
    SPI1_IRQn                   = 35,     /*!< SPI1 global Interrupt                                */
    SPI2_IRQn                   = 36,     /*!< SPI2 global Interrupt                                */
    USART1_IRQn                 = 37,     /*!< USART1 global Interrupt                              */
    USART2_IRQn                 = 38,     /*!< USART2 global Interrupt                              */
    USART3_IRQn                 = 39,     /*!< USART3 global Interrupt                              */
    EXTI15_10_IRQn              = 40,     /*!< External Line[15:10] Interrupts                      */
    RTCAlarm_IRQn               = 41,     /*!< RTC Alarm through EXTI Line Interrupt                */
    OTG_FS_WKUP_IRQn            = 42,     /*!< USB OTG FS WakeUp from suspend through EXTI Line Interrupt */
    TIMER8_BRK_TIMER12_IRQn     = 43,     /*!< TIMER8 Break Interrupt and TIMER12 global Interrupt  */
    TIMER8_UP_TIMER13_IRQn      = 44,     /*!< TIMER8 Update Interrupt and TIMER13 global Interrupt */
    TIMER8_TRG_COM_TIMER14_IRQn = 45,     /*!< TIMER8 Trigger and Commutation Interrupt and TIMER14 global Interrupt */
    TIMER8_CC_IRQn              = 46,     /*!< TIMER8 Capture Compare Interrupt                     */
    ADC3_IRQn                   = 47,     /*!< ADC3 global Interrupt                                */
    EXMC_IRQn                   = 48,     /*!< EXMC global Interrupt                                */
    SDIO_IRQn                   = 49,     /*!< SDIO global Interrupt                                */
    TIMER5_IRQn                 = 50,     /*!< TIMER5 global Interrupt                              */
    SPI3_IRQn                   = 51,     /*!< SPI3 global Interrupt                                */
    UART4_IRQn                  = 52,     /*!< UART4 global Interrupt                               */
    UART5_IRQn                  = 53,     /*!< UART5 global Interrupt                               */
    TIMER6_IRQn                 = 54,     /*!< TIMER6 global Interrupt                              */
    TIMER7_IRQn                 = 55,     /*!< TIMER7 global Interrupt                              */
    DMA2_Channel1_IRQn          = 56,     /*!< DMA2 Channel 1 global Interrupt                      */
    DMA2_Channel2_IRQn          = 57,     /*!< DMA2 Channel 2 global Interrupt                      */
    DMA2_Channel3_IRQn          = 58,     /*!< DMA2 Channel 3 global Interrupt                      */
    DMA2_Channel4_IRQn          = 59,     /*!< DMA2 Channel 4 global Interrupt                      */
    DMA2_Channel5_IRQn          = 60,     /*!< DMA2 Channel 5 global Interrupt                      */
    ETH_IRQn                    = 61,     /*!< Ethernet global Interrupt                            */
    ETH_WKUP_IRQn               = 62,     /*!< Ethernet Wakeup through EXTI line Interrupt          */
    CAN2_TX_IRQn                = 63,     /*!< CAN2 TX Interrupt                                    */
    CAN2_RX0_IRQn               = 64,     /*!< CAN2 RX0 Interrupt                                   */
    CAN2_RX1_IRQn               = 65,     /*!< CAN2 RX1 Interrupt                                   */
    CAN2_SCE_IRQn               = 66,     /*!< CAN2 SCE Interrupt                                   */
    OTG_FS_IRQn                 = 67,      /*!< USB OTG FS global Interrupt                         */
    DMA2_Channel6_IRQn          = 69,      /*!< DMA2 Channel 6_irq                                  */
    DMA2_Channel7_IRQn          = 70,      /*!< DMA2 Channel 7_irq                                  */
    USART6_IRQn                 = 71,      /*!< USART6 global Interrupt                             */
    I2C3_EV_IRQn                = 72,      /*!< I2C3 event Interrupt                                */
    I2C3_ER_IRQn                = 73,      /*!< I2C3 error Interrupt                                */
    DCI_IRQn                    = 78,      /*!< DCI global Interrupt                                */
    CRYP_IRQn                   = 79,      /*!< CRYP global Interrupt                               */
    HASH_RNG_IRQn               = 80,      /*!< HASH or RNG global Interrupt                        */
    UART7_IRQn                  = 82,      /*!< UART7 global Interrupt                              */		
    UART8_IRQn                  = 83,      /*!< UART8 global Interrupt                              */
    TLDI_IRQn                   = 88,      /*!< TLDI global Interrupt                               */
    TLDI_ER_IRQn                = 89       /*!< TLDI global error Interrupt                         */
} IRQn_Type;


/**
  * @}
  */

/* Includes ------------------------------------------------------------------*/
#include "core_cm3.h"
#include "system_gd32f20x.h"
#include <stdint.h>

/** @addtogroup Exported_types
  * @{
  */  
typedef enum {ERROR = 0, SUCCESS = !ERROR, RESET = 0, SET = !RESET, DISABLE = 0, ENABLE = !DISABLE} TypeState;
typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  /*!< Read Only */
typedef const int16_t sc16;  /*!< Read Only */
typedef const int8_t sc8;   /*!< Read Only */

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  /*!< Read Only */
typedef __I int16_t vsc16;  /*!< Read Only */
typedef __I int8_t vsc8;   /*!< Read Only */

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint8_t uc8;   /*!< Read Only */

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  /*!< Read Only */
typedef __I uint16_t vuc16;  /*!< Read Only */
typedef __I uint8_t vuc8;   /*!< Read Only */

/** @addtogroup Peripheral_registers_structures
  * @{
  */   

/** 
  * @brief Analog to Digital Converter  
  */
typedef struct
{
    __IO uint32_t STR;
    __IO uint32_t CTLR1;
    __IO uint32_t CTLR2;
    __IO uint32_t SPT1;
    __IO uint32_t SPT2;
    __IO uint32_t ICOS1;
    __IO uint32_t ICOS2;
    __IO uint32_t ICOS3;
    __IO uint32_t ICOS4;
    __IO uint32_t AWHT;
    __IO uint32_t AWLT;
    __IO uint32_t RSQ1;
    __IO uint32_t RSQ2;
    __IO uint32_t RSQ3;
    __IO uint32_t ISQ;
    __IO uint32_t IDTR1;
    __IO uint32_t IDTR2;
    __IO uint32_t IDTR3;
    __IO uint32_t IDTR4;
    __IO uint32_t RDTR;   
    uint32_t  RESERVED0;
    uint32_t  RESERVED1;
    uint32_t  RESERVED2;
    uint32_t  RESERVED3;
    uint32_t  RESERVED4;
    uint32_t  RESERVED5;
    uint32_t  RESERVED6;
    uint32_t  RESERVED7;
    uint32_t  RESERVED8;
    uint32_t  RESERVED9;
    uint32_t  RESERVED10;
    uint32_t  RESERVED11;
    __IO uint32_t OVSCR ;   
} ADC_TypeDef;

/** 
  * @brief Backup Registers
  */
typedef struct
{
    uint32_t  RESERVED0; 
    __IO uint16_t DR1;               /*!<Backup data register 1,                Address offset: 0x04 */
    uint16_t  RESERVED1;  
    __IO uint16_t DR2;               /*!<Backup data register 2,                Address offset: 0x08 */
    uint16_t  RESERVED2;
    __IO uint16_t DR3;               /*!<Backup data register 3,                Address offset: 0x0C */
    uint16_t  RESERVED3;
    __IO uint16_t DR4;               /*!<Backup data register 4,                Address offset: 0x10 */
    uint16_t  RESERVED4;  
    __IO uint16_t DR5;               /*!<Backup data register 5,                Address offset: 0x14 */
    uint16_t  RESERVED5;
    __IO uint16_t DR6;               /*!<Backup data register 6,                Address offset: 0x18 */
    uint16_t  RESERVED6;
    __IO uint16_t DR7;               /*!<Backup data register 7,                Address offset: 0x1C */
    uint16_t  RESERVED7;
    __IO uint16_t DR8;               /*!<Backup data register 8,                Address offset: 0x20 */
    uint16_t  RESERVED8;
    __IO uint16_t DR9;               /*!<Backup data register 9,                Address offset: 0x24 */
    uint16_t  RESERVED9;
    __IO uint16_t DR10;              /*!<Backup data register 10,               Address offset: 0x28 */
    uint16_t  RESERVED10; 
    __IO uint16_t RCCR;              /*!<RTC clock calibration register,        Address offset: 0x2C */
    uint16_t  RESERVED11;
    __IO uint16_t TPCR1;             /*!<Tamper pin control register 1,         Address offset: 0x30 */
    uint16_t  RESERVED12;
    __IO uint16_t TIER;              /*!<Tamper interrupt event register,       Address offset: 0x34 */
    uint16_t  RESERVED13;
    __IO uint16_t TPCR2;             /*!<Tamper pin control register 2,         Address offset: 0x38 */
    uint16_t  RESERVED14[3];
    __IO uint16_t DR11;              /*!<Backup data register 11,               Address offset: 0x40 */
    uint16_t  RESERVED15;
    __IO uint16_t DR12;              /*!<Backup data register 12,               Address offset: 0x44 */
    uint16_t  RESERVED16;
    __IO uint16_t DR13;              /*!<Backup data register 13,               Address offset: 0x48 */
    uint16_t  RESERVED17;
    __IO uint16_t DR14;              /*!<Backup data register 14,               Address offset: 0x4C */
    uint16_t  RESERVED18;
    __IO uint16_t DR15;              /*!<Backup data register 15,               Address offset: 0x50 */
    uint16_t  RESERVED19;
    __IO uint16_t DR16;              /*!<Backup data register 16,               Address offset: 0x54 */
    uint16_t  RESERVED20;
    __IO uint16_t DR17;              /*!<Backup data register 17,               Address offset: 0x58 */
    uint16_t  RESERVED21;
    __IO uint16_t DR18;              /*!<Backup data register 18,               Address offset: 0x5C */
    uint16_t  RESERVED22;
    __IO uint16_t DR19;              /*!<Backup data register 19,               Address offset: 0x60 */
    uint16_t  RESERVED23;
    __IO uint16_t DR20;              /*!<Backup data register 20,               Address offset: 0x64 */
    uint16_t  RESERVED24;
    __IO uint16_t DR21;              /*!<Backup data register 21,               Address offset: 0x68 */
    uint16_t  RESERVED25;
    __IO uint16_t DR22;              /*!<Backup data register 22,               Address offset: 0x6C */
    uint16_t  RESERVED26;
    __IO uint16_t DR23;              /*!<Backup data register 23,               Address offset: 0x70 */
    uint16_t  RESERVED27; 
    __IO uint16_t DR24;              /*!<Backup data register 24,               Address offset: 0x74 */
    uint16_t  RESERVED28;
    __IO uint16_t DR25;              /*!<Backup data register 25,               Address offset: 0x78 */
    uint16_t  RESERVED29;
    __IO uint16_t DR26;              /*!<Backup data register 26,               Address offset: 0x7C */
    uint16_t  RESERVED30;
    __IO uint16_t DR27;              /*!<Backup data register 27,               Address offset: 0x80 */
    uint16_t  RESERVED31;
    __IO uint16_t DR28;              /*!<Backup data register 28,               Address offset: 0x84 */
    uint16_t  RESERVED32;
    __IO uint16_t DR29;              /*!<Backup data register 29,               Address offset: 0x88 */
    uint16_t  RESERVED33;
    __IO uint16_t DR30;              /*!<Backup data register 30,               Address offset: 0x8C */
    uint16_t  RESERVED34; 
    __IO uint16_t DR31;              /*!<Backup data register 31,               Address offset: 0x90 */
    uint16_t  RESERVED35;
    __IO uint16_t DR32;              /*!<Backup data register 32,               Address offset: 0x94 */
    uint16_t  RESERVED36;
    __IO uint16_t DR33;              /*!<Backup data register 33,               Address offset: 0x98 */
    uint16_t  RESERVED37;
    __IO uint16_t DR34;              /*!<Backup data register 34,               Address offset: 0x9C */
    uint16_t  RESERVED38;
    __IO uint16_t DR35;              /*!<Backup data register 35,               Address offset: 0xA0 */
    uint16_t  RESERVED39;
    __IO uint16_t DR36;              /*!<Backup data register 36,               Address offset: 0xA4 */
    uint16_t  RESERVED40;
    __IO uint16_t DR37;              /*!<Backup data register 37,               Address offset: 0xA8 */
    uint16_t  RESERVED41;
    __IO uint16_t DR38;              /*!<Backup data register 38,               Address offset: 0xAC */
    uint16_t  RESERVED42;
    __IO uint16_t DR39;              /*!<Backup data register 39,               Address offset: 0xB0 */
    uint16_t  RESERVED43;
    __IO uint16_t DR40;              /*!<Backup data register 40,               Address offset: 0xB4 */
    uint16_t  RESERVED44;
    __IO uint16_t DR41;              /*!<Backup data register 41,               Address offset: 0xB8 */
    uint16_t  RESERVED45;
    __IO uint16_t DR42;              /*!<Backup data register 42,               Address offset: 0xBC */
    uint16_t  RESERVED46;
} BKP_TypeDef;

/** 
  * @brief Controller Area Network TxMailBox 
  */
typedef struct
{
    __IO uint32_t TMIR;                /*!< CAN transmit mailbox identifier register, Address offset: 0x180, 0x190, 0x1A0 */
    __IO uint32_t TMPR;                /*!< CAN transmit mailbox property register,   Address offset: 0x184, 0x194, 0x1A4 */
    __IO uint32_t TMD0R;               /*!< CAN transmit mailbox data0 register,      Address offset: 0x188, 0x198, 0x1A8 */
    __IO uint32_t TMD1R;               /*!< CAN transmit mailbox data1 register,      Address offset: 0x18C, 0x19C, 0x1AC */
} CAN_TxMailBox_TypeDef;

/** 
  * @brief Controller Area Network FIFOMailBox 
  */ 
typedef struct
{
    __IO uint32_t RFMIR;               /*!< CAN receive FIFO mailbox identifier register, Address offset: 0x1B0, 0x1C0 */
    __IO uint32_t RFMPR;               /*!< CAN receive FIFO mailbox property register,   Address offset: 0x1B4, 0x1C4 */
    __IO uint32_t RFMD0R;              /*!< CAN receive FIFO mailbox data0 register,      Address offset: 0x1B8, 0x1C8 */
    __IO uint32_t RFMD1R;              /*!< CAN receive FIFO mailbox data1 register,      Address offset: 0x1BC, 0x1CC */
} CAN_FIFOMailBox_TypeDef;

/** 
  * @brief Controller Area Network FilterRegister 
  */
typedef struct
{                          
    __IO uint32_t FD0R;                /*!< CAN filter x data 0 register */
    __IO uint32_t FD1R;                /*!< CAN filter x data 1 register */
} CAN_FilterRegister_TypeDef;

/** 
  * @brief Controller Area Network 
  */
typedef struct
{
    __IO uint32_t CTLR;                /*!< CAN control register,                 Address offset: 0x00 */
    __IO uint32_t STR;                 /*!< CAN status register,                  Address offset: 0x04 */
    __IO uint32_t TSTR;                /*!< CAN transmit status register,         Address offset: 0x08 */
    __IO uint32_t RFR0;                /*!< CAN receive FIFO0 register,           Address offset: 0x0C */
    __IO uint32_t RFR1;                /*!< CAN receive FIFO0 register,           Address offset: 0x10 */
    __IO uint32_t IER;                 /*!< CAN interrupt enable register,        Address offset: 0x14 */
    __IO uint32_t ER;                  /*!< CAN error register,                   Address offset: 0x18 */
    __IO uint32_t BTR;                 /*!< CAN bit timing register,              Address offset: 0x1C */
    uint32_t  RESERVED0[88];
    CAN_TxMailBox_TypeDef TxMailBox[3];
    CAN_FIFOMailBox_TypeDef FIFOMailBox[2];
    uint32_t  RESERVED1[12];
    __IO uint32_t FCTLR;               /*!< CAN filter control register,          Address offset: 0x200 */   
    __IO uint32_t FMR;                 /*!< CAN filter mode register,             Address offset: 0x204 */
    uint32_t  RESERVED2;      
    __IO uint32_t FSR;                 /*!< CAN filter scale register,            Address offset: 0x20C */
    uint32_t  RESERVED3;
    __IO uint32_t FAFR;                /*!< CAN filter associated FIFO register,  Address offset: 0x214 */
    uint32_t  RESERVED4;
    __IO uint32_t FWR;                 /*!< CAN filter working register,          Address offset: 0x21C */
    uint32_t  RESERVED5[8];
    CAN_FilterRegister_TypeDef FilterRegister[28];
} CAN_TypeDef;

/** 
  * @brief CRC calculation unit 
  */
typedef struct
{
    __IO uint32_t DTR;                 /*!< CRC Data register,                           Address offset: 0x00 */
    __IO uint8_t  FDTR;                /*!< CRC Independent data register,               Address offset: 0x04 */
    uint8_t   RESERVED0;               /*!< Reserved,                                                    0x05 */
    uint16_t  RESERVED1;               /*!< Reserved,                                                    0x06 */
    __IO uint32_t CTLR;                /*!< CRC Control register,                        Address offset: 0x08 */
} CRC_TypeDef;

/** 
  * @brief Digital to Analog Converter
  */
typedef struct
{
    __IO uint32_t CTLR;
    __IO uint32_t SWTR;
    __IO uint32_t C1R12DHR;
    __IO uint32_t C1L12DHR;
    __IO uint32_t C1R8DHR;

    __IO uint32_t C2R12DHR;
    __IO uint32_t C2L12DHR;
    __IO uint32_t C2R8DHR;

    __IO uint32_t DCR12DHR;
    __IO uint32_t DCL12DHR;
    __IO uint32_t DCR8RD;

    __IO uint32_t C1ODR;
    __IO uint32_t C2ODR;
} DAC_TypeDef;

/** 
  * @brief Debug MCU
  */
typedef struct
{
    __IO uint32_t IDR;                   /*!< MCU device ID code,                          Address offset: 0x00 */
    __IO uint32_t CTLR;                  /*!< Debug MCU CTLR freeze register,              Address offset: 0x04 */
}MCUDBG_TypeDef;

/** 
  * @brief DMA Controller
  */
typedef struct
{
    __IO uint32_t CTLR;                  /*!< DMA channel x configuration register */
    __IO uint32_t RCNT;                  /*!< DMA channel x number of data register */
    __IO uint32_t PBAR;                  /*!< DMA channel x peripheral address register */
    __IO uint32_t MBAR;                  /*!< DMA channel x memory address register */
} DMA_Channel_TypeDef;

typedef struct
{
    __IO uint32_t IFR;                   /*!< DMA interrupt status register,  Address offset: 0x00 */
    __IO uint32_t ICR;                   /*!< DMA interrupt flag clear register,Address offset: 0x04 */
    __IO uint32_t RESERVED[190];
    __IO uint32_t ACR;                   /*!< DMA additional configuration register ,Address offset: 0x300 */
} DMA_TypeDef;

/** 
  * @brief Ethernet
  */
typedef struct
{
    __IO uint32_t CFR;                   /*!< Ethernet MAC configuration register,  Address offset: 0x00 */    
    __IO uint32_t FRMFR;                 /*!< Ethernet MAC frame filter register,  Address offset: 0x04 */    
    __IO uint32_t HLHR;                  /*!< Ethernet MAC hash list high register,  Address offset: 0x08 */    
    __IO uint32_t HLLR;                  /*!< Ethernet MAC hash list low register,  Address offset: 0x0C */    
    __IO uint32_t PHYAR;                 /*!< Ethernet MAC PHY address register,  Address offset: 0x10 */    
    __IO uint32_t PHYDR;                 /*!< Ethernet MAC PHY data register,  Address offset: 0x14 */
    __IO uint32_t FCTLR;                 /*!< Ethernet MAC flow control register,  Address offset: 0x18 */
    __IO uint32_t VLTR;                  /*!< Ethernet MAC VLAN tag register,  Address offset: 0x1C */     
    uint32_t RESERVED0[2];               /*!< Reserved,    0x24 */
    __IO uint32_t RWFFR;                 /*!< Ethernet MAC remote wakeup frame filter register,  Address offset: 0x28 */        
    __IO uint32_t WUMR;                  /*!< Ethernet MAC wake up management register,  Address offset: 0x2C */
    uint32_t RESERVED1[2];               /*!< Reserved,    0x34 */
    __IO uint32_t ISR;                   /*!< Ethernet MAC interrupt status register,  Address offset: 0x38 */         
    __IO uint32_t IMR;                   /*!< Ethernet MAC interrupt mask register,  Address offset: 0x3C */
    __IO uint32_t ADDR0HR;               /*!< Ethernet MAC address 0 high register,  Address offset: 0x40 */
    __IO uint32_t ADDR0LR;               /*!< Ethernet MAC address 0 low register,  Address offset: 0x44 */
    __IO uint32_t ADDR1HR;               /*!< Ethernet MAC address 1 high register,  Address offset: 0x48 */
    __IO uint32_t ADDR1LR;               /*!< Ethernet MAC address 1 low register,  Address offset: 0x4C */
    __IO uint32_t ADDR2HR;               /*!< Ethernet MAC address 2 high register,  Address offset: 0x50 */
    __IO uint32_t ADDR2LR;               /*!< Ethernet MAC address 2 low register,  Address offset: 0x54 */
    __IO uint32_t ADDR3HR;               /*!< Ethernet MAC address 3 high register,  Address offset: 0x58 */
    __IO uint32_t ADDR3LR;               /*!< Ethernet MAC address 3 low register,  Address offset: 0x5C */            
    uint32_t RESERVED2[1032];            /*!< Reserved,    0x107C */
    __IO uint32_t FCTHR;                 /*!< Ethernet MAC flow control threshold register,  Address offset: 0x1080 */ 
} ETH_MAC_TypeDef;

typedef struct
{
    __IO uint32_t CTLR;                  /*!< Ethernet MSC control register,  Address offset: 0x100 */ 
    __IO uint32_t RISR;                  /*!< Ethernet MSC receive interrupt status register,  Address offset: 0x104 */    
    __IO uint32_t TISR;                  /*!< Ethernet MSC transmit interrupt status register,  Address offset: 0x108 */
    __IO uint32_t RIMR;                  /*!< Ethernet MSC receive interrupt mask register,  Address offset: 0x10C */
    __IO uint32_t TIMR;                  /*!< Ethernet MSC transmit interrupt mask register,  Address offset: 0x110 */       
    uint32_t RESERVED3[14];              /*!< Reserved,    0x148 */
    __IO uint32_t SCCNT;                 /*!< Ethernet MSC transmitted good frames after a single collision counterregister,  Address offset: 0x14C */  
    __IO uint32_t MSCCNT;                /*!< Ethernet MSC transmitted good frames after more than a single collision counterregister,  Address offset: 0x150 */    
    uint32_t RESERVED4[5];               /*!< Reserved,    0x164 */
    __IO uint32_t TGFCNT;                /*!< Ethernet MSC transmitted good frames counter register,  Address offset: 0x168 */
    uint32_t RESERVED5[10];              /*!< Reserved,    0x190 */
    __IO uint32_t RFCECNT;               /*!< Ethernet MSC received frames with CRC error counter register,  Address offset: 0x194 */
    __IO uint32_t RFAECNT;               /*!< Ethernet MSC received frames with alignment error counter register,  Address offset: 0x198 */
    uint32_t RESERVED6[10];              /*!< Reserved,    0x1C0 */
    __IO uint32_t RGUFCNT;               /*!< Ethernet MSC received good unicast frames counter register,  Address offset: 0x1C4 */    
} ETH_MSC_TypeDef;

typedef struct
{
    __IO uint32_t TSCTLR;                /*!< Ethernet PTP time stamp control register,  Address offset: 0x700 */
    __IO uint32_t SSINCR;                /*!< Ethernet PTP subsecond increment register,  Address offset: 0x704 */
    __IO uint32_t TMSHR;                 /*!< Ethernet PTP time stamp high register,  Address offset: 0x708 */
    __IO uint32_t TMSLR;                 /*!< Ethernet PTP time stamp low register,  Address offset: 0x70C */
    __IO uint32_t TMSHUR;                /*!< Ethernet PTP time stamp high update register,  Address offset: 0x710 */
    __IO uint32_t TMSLUR;                /*!< Ethernet PTP time stamp low update register,  Address offset: 0x714 */
    __IO uint32_t TSACNT;                /*!< Ethernet PTP time stamp addend register,  Address offset: 0x718 */
    __IO uint32_t ETHR;                  /*!< Ethernet PTP expected time high register,  Address offset: 0x71C */
    __IO uint32_t ETLR;                  /*!< Ethernet PTP expected time low register,  Address offset: 0x720 */
} ETH_PTP_TypeDef;

typedef struct
{
    __IO uint32_t BCR;                   /*!< Ethernet DMA bus control register,  Address offset: 0x1000 */
    __IO uint32_t TPER;                  /*!< Ethernet DMA transmit poll enable register,  Address offset: 0x1004 */
    __IO uint32_t RPER;                  /*!< Ethernet DMA receive poll enable register,  Address offset: 0x1008 */
    __IO uint32_t RDTAR;                 /*!< Ethernet DMA receive descriptor tab address register,  Address offset: 0x100C */
    __IO uint32_t TDTAR;                 /*!< Ethernet DMA transmit descriptor tab address register,  Address offset: 0x1010 */
    __IO uint32_t STR;                   /*!< Ethernet DMA status register,  Address offset: 0x1014 */
    __IO uint32_t CTLR;                  /*!< Ethernet DMA control register,  Address offset: 0x1018 */
    __IO uint32_t IER;                   /*!< Ethernet DMA interrupt enable register,  Address offset: 0x1018 */
    __IO uint32_t MFBOCNT;               /*!< Ethernet DMA missed frame and buffer overflow counter register,  Address offset: 0x101C */
    uint32_t RESERVED7[9];               /*!< Reserved,    0x1044 */
    __IO uint32_t CTDAR;                 /*!< Ethernet DMA current transmit descriptor address register,  Address offset: 0x1048 */
    __IO uint32_t CRDAR;                 /*!< Ethernet DMA current receive descriptor address register,  Address offset: 0x104C */
    __IO uint32_t CTBAR;                 /*!< Ethernet DMA current transmit buffer address register,  Address offset: 0x1050 */
    __IO uint32_t CRBAR;                 /*!< Ethernet DMA current receive buffer address register,  Address offset: 0x1054 */
} ETH_DMA_Typedef;

/** 
  * @brief External Interrupt/Event Controller
  */
typedef struct
{
    __IO uint32_t IER;                   /*!<EXTI Interrupt enable register,                Address offset: 0x00 */
    __IO uint32_t EER;                   /*!<EXTI Event enable register,                    Address offset: 0x04 */
    __IO uint32_t RTE;                   /*!<EXTI Rising edge trigger enable register,      Address offset: 0x08 */
    __IO uint32_t FTE;                   /*!<EXTI Falling edge trigger enable register,     Address offset: 0x0C */
    __IO uint32_t SIE;                   /*!<EXTI Software interrupt event register,        Address offset: 0x10 */
    __IO uint32_t PD;                    /*!<EXTI Pending register,                         Address offset: 0x14 */
} EXTI_TypeDef;


/** 
  * @brief FMC Registers
  */
typedef struct
{
    __IO uint32_t RESR;                  /*!<FMC access control register,                   Address offset: 0x00 */
    __IO uint32_t UKEYR;                 /*!<FMC key register,                              Address offset: 0x04 */
    __IO uint32_t OBKEYR;                /*!<FMC OBKEYR register,                           Address offset: 0x08 */
    __IO uint32_t CSR;                   /*!<FMC state register,                            Address offset: 0x0C */
    __IO uint32_t CMR;                   /*!<FMC control register,                          Address offset: 0x10 */
    __IO uint32_t AR;                    /*!<FMC address register,                          Address offset: 0x14 */
    __IO uint32_t RESERVED;              /*!< Reserved,                                     Address offset: 0x18 */
    __IO uint32_t OPTR;                  /*!<FMC option bytes register,                     Address offset: 0x1C */
    __IO uint32_t WPR;                   /*!<FMC option bytes register,                     Address offset: 0x20 */
    uint32_t RESERVED1[8];               /*!< Reserved,                                     Address offset: 0x24 */
    __IO uint32_t UKEYR2;                /*!< FMC key register for bank2                    Address offset: 0x44 */
    uint32_t RESERVED2;                  /*!< Reserved,                                     Address offset: 0x48 */
    __IO uint32_t CSR2;                  /*!< FMC state register for bank2,                 Address offset: 0x4C */
    __IO uint32_t CMR2;                  /*!< FMC control register for bank2,               Address offset: 0x50 */
    __IO uint32_t AR2;                   /*!< FMC address register for bank2,               Address offset: 0x54 */
    uint32_t RESERVED3[41];              /*!< Reserved                                      Address offset: 0x58 */
    __IO uint32_t WSCR;                  /*!< FMC wait state control register               Address offset: 0xFC */
    __IO uint32_t RES_ID1;               /*!< FMC product reserved ID register 1,           Address offset: 0x100 */
    __IO uint32_t RES_ID2;               /*!< FMC product reserved ID register 2,           Address offset: 0x104 */
} FMC_TypeDef;

/** 
  * @brief Option Bytes Registers
  */
typedef struct
{
    __IO uint16_t RDP;                   /*!<FMC option byte Read protection,               Address offset: 0x00 */
    __IO uint16_t USER;                  /*!<FMC option byte user options,                  Address offset: 0x02 */
    uint16_t RESERVED0;                  /*!< Reserved,                                     Address offset: 0x04 */
    uint16_t RESERVED1;                  /*!< Reserved,                                     Address offset: 0x06 */
    __IO uint16_t WRP0;                  /*!<FMC option byte write protection 0,            Address offset: 0x08 */
    __IO uint16_t WRP1;                  /*!<FMC option byte write protection 1,            Address offset: 0x0C */
    __IO uint16_t WRP2;                  /*!<FMC option byte write protection 2,            Address offset: 0x10 */
    __IO uint16_t WRP3;                  /*!<FMC option byte write protection 3,            Address offset: 0x14 */
} OB_TypeDef;

/** 
  * @brief External Memory Controller Bank1
  */
typedef struct
{
    __IO uint32_t SCTLR1;  
    __IO uint32_t STR1; 
    __IO uint32_t SCTLR2; 
    __IO uint32_t STR2; 
    __IO uint32_t SCTLR3; 
    __IO uint32_t STR3;  
    __IO uint32_t SCTLR4; 
    __IO uint32_t STR4; 
} EXMC_Bank1_TypeDef;

/** 
  * @brief External Memory Controller Bank1_WT
  */
typedef struct
{
    __IO uint32_t SWTR1;
    uint32_t  RESERVED0;
    __IO uint32_t SWTR2;
    uint32_t  RESERVED1;
    __IO uint32_t SWTR3;
    uint32_t  RESERVED2;
    __IO uint32_t SWTR4;
} EXMC_Bank1_WT_TypeDef;

/** 
  * @brief External Memory Controller SQPI-PSRAM
  */
typedef struct
{
    __IO uint32_t SINIR;
    uint32_t  RESERVED1[3];
    __IO uint32_t SRCR;
    uint32_t  RESERVED2[3];
    __IO uint32_t SWCR;
    uint32_t  RESERVED3[3];
    __IO uint32_t SIDLR;
    uint32_t  RESERVED4[3];
    __IO uint32_t SIDHR;
} EXMC_Bank1_SQPI_TypeDef;

/** 
  * @brief External Memory Controller Bank2
  */
typedef struct
{
    __IO uint32_t CTLR2;
    __IO uint32_t SIR2;
    __IO uint32_t COMTR2;
    __IO uint32_t ATTR2;
    uint32_t  RESERVED; 
    __IO uint32_t ECCR2; 
} EXMC_Bank2_TypeDef;

/** 
  * @brief External Memory Controller Bank3
  */
typedef struct
{
    __IO uint32_t CTLR3;
    __IO uint32_t SIR3;
    __IO uint32_t COMTR3;
    __IO uint32_t ATTR3;
    uint32_t  RESERVED;
    __IO uint32_t ECCR3; 
} EXMC_Bank3_TypeDef;

/** 
  * @brief External Memory Controller Bank4
  */
typedef struct
{
    __IO uint32_t CTLR4;
    __IO uint32_t SIR4;
    __IO uint32_t COMTR4;
    __IO uint32_t ATTR4;
    __IO uint32_t IOTR4; 
} EXMC_Bank4_TypeDef;

/** 
  * @brief External Memory Controller Bank5 and Bank6
  */
typedef struct
{
    __IO uint32_t SDCTLR1;
    __IO uint32_t SDCTLR2;
    __IO uint32_t SDTR1;
    __IO uint32_t SDTR2;
    __IO uint32_t SDCMDR;
    __IO uint32_t SDARFIR;
    __IO uint32_t SDSTR;
    uint32_t  RESERVED[9]; 
    __IO uint32_t SDRSCR; 
} EXMC_Bank5_6_TypeDef;

/** 
  * @brief gpio
  */ 
typedef struct
{
    __IO uint32_t CTLR1;
    __IO uint32_t CTLR2;
    __IO uint32_t DIR;
    __IO uint32_t DOR;
    __IO uint32_t BOR;
    __IO uint32_t BCR;
    __IO uint32_t LOCKR;
} GPIO_TypeDef;

/** 
  * @brief AFIO
  */
typedef struct
{
    __IO uint32_t AFIO_EVR;
    __IO uint32_t AFIO_PCFR1;
    __IO uint32_t AFIO_ESSR1;
    __IO uint32_t AFIO_ESSR2;
    __IO uint32_t AFIO_ESSR3;
    __IO uint32_t AFIO_ESSR4;
    uint32_t RESERVED0;
    __IO uint32_t AFIO_PCFR2;
    uint32_t RESERVED1;
    uint32_t RESERVED2;
    uint32_t RESERVED3;
    uint32_t RESERVED4;
    uint32_t RESERVED5;
    uint32_t RESERVED6;
    uint32_t RESERVED7;
    __IO uint32_t AFIO_PCFR3;
    __IO uint32_t AFIO_PCFR4;
    __IO uint32_t AFIO_PCFR5;
    __IO uint32_t AFIO_PCFR6;
} AFIO_TypeDef;

/** 
  * @brief Inter Integrated Circuit Interface
  */
typedef struct
{
    __IO uint16_t CTLR1;
    uint16_t  RESERVED0;
    __IO uint16_t CTLR2;
    uint16_t  RESERVED1;
    __IO uint16_t AR1;
    uint16_t  RESERVED2;
    __IO uint16_t AR2;
    uint16_t  RESERVED3;
    __IO uint16_t DTR;
    uint16_t  RESERVED4;
    __IO uint16_t STR1;
    uint16_t  RESERVED5;
    __IO uint16_t STR2;
    uint16_t  RESERVED6;
    __IO uint16_t CLKR;
    uint16_t  RESERVED7;
    __IO uint16_t RTR;
    uint16_t  RESERVED8;
} I2C_TypeDef;

/** 
  * @brief Independent WATCHDOG
  */
typedef struct
{
    __IO uint32_t CTLR;                /*!< IWDG Key register,                 Address offset: 0x00 */
    __IO uint32_t PSR;                 /*!< IWDG Prescaler register,           Address offset: 0x04 */
    __IO uint32_t RLDR;                /*!< IWDG Reload register,              Address offset: 0x08 */
    __IO uint32_t STR;                 /*!< IWDG Status register,              Address offset: 0x0C */
} IWDG_TypeDef;

/** 
  * @brief Power Control
  */
typedef struct
{
    __IO uint32_t CTLR;                /*!< PWR power control register,        Address offset: 0x00 */
    __IO uint32_t STR;                 /*!< PWR power status register,         Address offset: 0x04 */
} PWR_TypeDef;

/** 
  * @brief Reset and Clock Control
  */
typedef struct
{
    __IO uint32_t GCCR;                /*!< RCC Global Clock control register,         Address offset: 0x00 */
    __IO uint32_t GCFGR;               /*!< RCC Global Clock configuration register,   Address offset: 0x04  */
    __IO uint32_t GCIR;                /*!< RCC Global Clock interrupt register,       Address offset: 0x08  */
    __IO uint32_t APB2RCR;             /*!< RCC APB2 Reset Control Register,           Address offset: 0x0C  */
    __IO uint32_t APB1RCR;             /*!< RCC APB1 Reset Control Register,           Address offset: 0x10  */
    __IO uint32_t AHBCCR;              /*!< RCC AHB Clock Control Register,            Address offset: 0x14  */
    __IO uint32_t APB2CCR;             /*!< RCC APB2 Clock Control Register,           Address offset: 0x18  */
    __IO uint32_t APB1CCR;             /*!< RCC APB1 Clock Control Register,           Address offset: 0x1C  */
    __IO uint32_t BDCR;                /*!< RCC Backup domain control register,        Address offset: 0x20  */
    __IO uint32_t GCSR;                /*!< RCC Global Control/Status Register ,       Address offset: 0x24  */
    __IO uint32_t AHBRCR;              /*!< RCC AHB Reset Control Register,            Address offset: 0x28  */
    __IO uint32_t GCFGR2;              /*!< RCC Global Clock configuration register 2, Address offset: 0x2C  */
    __IO uint32_t RESERVED;
    __IO uint32_t RCC_DEEPSLEEP_VC;    /*!< RCC deep-sleep mode voltage register ,     Address offset: 0x34 */
    __IO uint32_t RESERVED0[10];
    __IO uint32_t AHB2CCR;             /*!< RCC AHB2 Clock Control Register,           Address offset: 0x60 */
    __IO uint32_t APB2CCR2;            /*!< RCC APB2 Clock Control Register 2,         Address offset: 0x64 */
    __IO uint32_t APB1CCR2;            /*!< RCC APB1 Clock Control Register 2,         Address offset: 0x68 */
    __IO uint32_t RESERVED1;
    __IO uint32_t AHB2RCR;             /*!< RCC AHB2 Reset Control Register,            Address offset: 0x70 */
    __IO uint32_t APB2RCR2;            /*!< RCC APB2 Reset Control Register 2,          Address offset: 0x74 */
    __IO uint32_t APB1RCR2;            /*!< RCC APB1 Reset Control Register 2,          Address offset: 0x78 */
    __IO uint32_t RESERVED2;
    __IO uint32_t GCFGR3;              /*!< RCC Global Clock configuration register 3,  Address offset: 0x80 */
    __IO uint32_t RESERVED3[3];
    __IO uint32_t GPLLTCR;             /*!< RCC Global PLLT Clock control register,     Address offset: 0x90 */
    __IO uint32_t GPLLTIR;             /*!< RCC Global PLLT Clock interrupt register,   Address offset: 0x94 */
    __IO uint32_t PLLTCFGR;            /*!< RCC PLLT configuration register,            Address offset: 0x98 */
} RCC_TypeDef;

/** 
  * @brief Real-Time Clock
  */
typedef struct
{
    __IO uint16_t CTLR1;               /*!< RTC Control register1,             Address offset: 0x00 */
    uint16_t  RESERVED0;  
    __IO uint16_t CTLR2;               /*!< RTC Control register2,             Address offset: 0x04 */
    uint16_t  RESERVED1;
    __IO uint16_t PLR1;                /*!< RTC Prescaler Load register1,      Address offset: 0x08 */ 
    uint16_t  RESERVED2;
    __IO uint16_t PLR2;                /*!< RTC Prescaler Load register2,      Address offset: 0x0C */
    uint16_t  RESERVED3; 
    __IO uint16_t PREDIV1;             /*!< RTC Prescaler Divider register1,   Address offset: 0x10 */ 
    uint16_t  RESERVED4;
    __IO uint16_t PREDIV2;             /*!< RTC Prescaler Divider register2,   Address offset: 0x14 */
    uint16_t  RESERVED5;
    __IO uint16_t CNT1;                /*!< RTC Counter register1,             Address offset: 0x18 */
    uint16_t  RESERVED6;
    __IO uint16_t CNT2;                /*!< RTC Counter register2,             Address offset: 0x1C */
    uint16_t  RESERVED7;
    __IO uint16_t ALRMR1;              /*!< RTC Alarm register1,               Address offset: 0x20 */
    uint16_t  RESERVED8;              
    __IO uint16_t ALRMR2;              /*!< RTC Alarm register2,               Address offset: 0x24 */
    uint16_t  RESERVED9;
} RTC_TypeDef;

/** 
  * @brief SD host Interface
  */
typedef struct
{
    __IO uint32_t POWER;
    __IO uint32_t CLKCTLR;
    __IO uint32_t PARA;
    __IO uint32_t CMD;
    __I uint32_t RESPCMD;
    __I uint32_t RESP1;
    __I uint32_t RESP2;
    __I uint32_t RESP3;
    __I uint32_t RESP4;
    __IO uint32_t DTTR;
    __IO uint32_t DTLEN;
    __IO uint32_t DTCTLR;
    __I uint32_t DTCNT;
    __I uint32_t STR;
    __IO uint32_t ICR;
    __IO uint32_t IER;
    uint32_t  RESERVED0[2];
    __I uint32_t FIFOCNT;
    uint32_t  RESERVED1[13];
    __IO uint32_t FIFO;
} SDIO_TypeDef;

/** 
  * @brief Serial Peripheral Interface
  */
typedef struct
{
    __IO uint16_t CTLR1;               /*!< SPI Control register 1 (not used in I2S mode),             Address offset: 0x00 */
    uint16_t  RESERVED0;               /*!< Reserved,                                                                  0x02 */
    __IO uint16_t CTLR2;               /*!< SPI Control register 2,                                    Address offset: 0x04 */
    uint16_t  RESERVED1;               /*!< Reserved,                                                                  0x06 */
    __IO uint16_t STR;                 /*!< SPI Status register,                                       Address offset: 0x08 */
    uint16_t  RESERVED2;               /*!< Reserved,                                                                  0x0A */
    __IO uint16_t DTR;                 /*!< SPI data register,                                         Address offset: 0x0C */
    uint16_t  RESERVED3;               /*!< Reserved,                                                                  0x0E */
    __IO uint16_t CPR;                 /*!< SPI CRC polynomial register (not used in I2S mode),        Address offset: 0x10 */
    uint16_t  RESERVED4;               /*!< Reserved,                                                                  0x12 */
    __IO uint16_t RCR;                 /*!< SPI Rx CRC register (not used in I2S mode),                Address offset: 0x14 */
    uint16_t  RESERVED5;               /*!< Reserved,                                                                  0x16 */
    __IO uint16_t TCR;                 /*!< SPI Tx CRC register (not used in I2S mode),                Address offset: 0x18 */
    uint16_t  RESERVED6;               /*!< Reserved,                                                                  0x1A */
    __IO uint16_t I2SCTLR;             /*!< SPI_I2S configuration register,                            Address offset: 0x1C */
    uint16_t  RESERVED7;               /*!< Reserved,                                                                  0x1E */
    __IO uint16_t I2SCKP;              /*!< SPI_I2S prescaler register,                                Address offset: 0x20 */
    uint16_t  RESERVED8;               /*!< Reserved,                                                                  0x22 */
    uint16_t  RESERVED9[46];           /*!< Reserved,                                                                  0x24 */
    __IO uint16_t SPIQWCR;             /*!< SPI quad wire control register,                            Address offset: 0x80 */
    uint16_t  RESERVED10;              /*!< Reserved,                                                                  0x82 */
} SPI_TypeDef;

/** 
  * @brief TIMER
  */
typedef struct
{
    __IO uint16_t CTLR1;               /*!< TIMER control register 1,                           Address offset: 0x00 */
    uint16_t      RESERVED0;           /*!< Reserved,                                                           0x02 */
    __IO uint16_t CTLR2;               /*!< TIMER control register 2,                           Address offset: 0x04 */
    uint16_t      RESERVED1;           /*!< Reserved,                                                           0x06 */
    __IO uint16_t SMC;                 /*!< TIMER slave Mode Control register,                  Address offset: 0x08 */
    uint16_t      RESERVED2;           /*!< Reserved,                                                           0x0A */
    __IO uint16_t DIE;                 /*!< TIMER DMA/interrupt enable register,                Address offset: 0x0C */
    uint16_t      RESERVED3;           /*!< Reserved,                                                           0x0E */
    __IO uint16_t STR;                 /*!< TIMER status register,                              Address offset: 0x10 */
    uint16_t      RESERVED4;           /*!< Reserved,                                                           0x12 */
    __IO uint16_t EVG;                 /*!< TIMER event generation register,                    Address offset: 0x14 */
    uint16_t      RESERVED5;           /*!< Reserved,                                                           0x16 */
    __IO uint16_t CHCTLR1;             /*!< TIMER  capture/compare mode register 1,             Address offset: 0x18 */
    uint16_t      RESERVED6;           /*!< Reserved,                                                           0x1A */
    __IO uint16_t CHCTLR2;             /*!< TIMER  capture/compare mode register 2,             Address offset: 0x1C */
    uint16_t      RESERVED7;           /*!< Reserved,                                                           0x1E */
    __IO uint16_t CHE;                 /*!< TIMER capture/compare enable register,              Address offset: 0x20 */
    uint16_t      RESERVED8;           /*!< Reserved,                                                           0x22 */
    __IO uint16_t CNT;                 /*!< TIMER counter register,                             Address offset: 0x24 */
    uint16_t      RESERVED9;           /*!< Reserved,                                                           0x26 */
    __IO uint16_t PSC;                 /*!< TIMER prescaler register,                           Address offset: 0x28 */
    uint16_t      RESERVED10;          /*!< Reserved,                                                           0x2A */
    __IO uint16_t CARL;                /*!< TIMER auto-reload register,                         Address offset: 0x2C */
    uint16_t      RESERVED11;          /*!< Reserved,                                                           0x2E */
    __IO uint16_t CREP;                /*!< TIMER  repetition counter register,                 Address offset: 0x30 */
    uint16_t      RESERVED12;          /*!< Reserved,                                                           0x32 */
    __IO uint16_t CHCC1;               /*!< TIMER capture/compare register 1,                   Address offset: 0x34 */
    uint16_t      RESERVED13;          /*!< Reserved,                                                           0x36 */
    __IO uint16_t CHCC2;               /*!< TIMER capture/compare register 2,                   Address offset: 0x38 */
    uint16_t      RESERVED14;          /*!< Reserved,                                                           0x3A */
    __IO uint16_t CHCC3;               /*!< TIMER capture/compare register 3,                   Address offset: 0x3C */
    uint16_t      RESERVED15;          /*!< Reserved,                                                           0x3E */
    __IO uint16_t CHCC4;               /*!< TIMER capture/compare register 4,                   Address offset: 0x40 */
    uint16_t      RESERVED16;          /*!< Reserved,                                                           0x42 */
    __IO uint16_t BKDT;                /*!< TIMER break and dead-time register,                 Address offset: 0x44 */
    uint16_t      RESERVED17;          /*!< Reserved,                                                           0x46 */
    __IO uint16_t DCTLR;               /*!< TIMER DMA control register,                         Address offset: 0x48 */
    uint16_t      RESERVED18;          /*!< Reserved,                                                           0x4A */
    __IO uint16_t DTRSF;               /*!< TIMER DMA address for full transfer register,       Address offset: 0x4C */
    uint16_t      RESERVED19;          /*!< Reserved,                                                           0x4E */
} TIMER_TypeDef;

/** 
  * @brief USART
  */
typedef struct
{
    __IO uint32_t STR;                 /*!< USART Interrupt and status register,                Address offset: 0x00 */
    __IO uint32_t DR;                  /*!< USART Data register,                                Address offset: 0x04 */
    __IO uint32_t BRR;                 /*!< USART Baud rate register,                           Address offset: 0x08 */
    __IO uint32_t CTLR1;               /*!< USART Control register 1,                           Address offset: 0x0C */ 
    __IO uint32_t CTLR2;               /*!< USART Control register 2,                           Address offset: 0x10 */ 
    __IO uint32_t CTLR3;               /*!< USART Control register 3,                           Address offset: 0x14 */
    uint32_t  RESERVED1;               /*!< Reserved,                                                           0x18 */  
    __IO uint32_t GTPR;                /*!< USART Guard time and prescaler register,            Address offset: 0x1C */
    uint32_t  RESERVED2[24];           /*!< Reserved,                                                           0x12 */
    __IO uint32_t CTLR4;               /*!< USART Control register 4,                           Address offset: 0x80 */
    __IO uint32_t RTR;                 /*!< USART Receiver Time Out register,                   Address offset: 0x84 */  
    __IO uint32_t STR2;                /*!< USART Interrupt and status register,                Address offset: 0x88 */
} USART_TypeDef;

/** 
  * @brief Window WATCHDOG
  */
typedef struct
{
    __IO uint32_t CTLR;                /*!< WWDG Control register,                              Address offset: 0x00 */
    __IO uint32_t CFR;                 /*!< WWDG Configuration register,                        Address offset: 0x04 */
    __IO uint32_t STR;                 /*!< WWDG Status register,                               Address offset: 0x08 */
} WWDG_TypeDef;

/** 
  * @brief CAU 
  */
typedef struct
{
    __IO uint32_t CTLR;                /*!< CAU control register,                               Address offset: 0x00 */
    __IO uint32_t SFR;                 /*!< CAU status flag register,                           Address offset: 0x04 */
    __IO uint32_t DI;                  /*!< CAU data input register,                            Address offset: 0x08 */
    __IO uint32_t DO;                  /*!< CAU data output register,                           Address offset: 0x0C */
    __IO uint32_t DCTLR;               /*!< CAU DMA control register,                           Address offset: 0x10 */
    __IO uint32_t IER;                 /*!< CAU interrupt enable register,                      Address offset: 0x14 */
    __IO uint32_t IFR;                 /*!< CAU interrupt status flag register,                 Address offset: 0x18 */
    __IO uint32_t EIFR;                /*!< CAU enabled interrupt status register,              Address offset: 0x1C */
    __IO uint32_t K0HR;                /*!< CAU key high  register 0,                           Address offset: 0x20 */
    __IO uint32_t K0LR;                /*!< CAU key low register 0,                             Address offset: 0x24 */
    __IO uint32_t K1HR;                /*!< CAU key high  register 1,                           Address offset: 0x28 */
    __IO uint32_t K1LR;                /*!< CAU key low register 1,                             Address offset: 0x2C */
    __IO uint32_t K2HR;                /*!< CAU key high  register 2,                           Address offset: 0x30 */
    __IO uint32_t K2LR;                /*!< CAU key low register 2,                             Address offset: 0x34 */
    __IO uint32_t K3HR;                /*!< CAU key high  register 3,                           Address offset: 0x38 */
    __IO uint32_t K3LR;                /*!< CAU key low register 3,                             Address offset: 0x3C */
    __IO uint32_t IV0HR;               /*!< CAU initialization vector high-word  register 0,    Address offset: 0x40 */
    __IO uint32_t IV0LR;               /*!< CAU initialization vector low-word register 0,      Address offset: 0x44 */
    __IO uint32_t IV1HR;               /*!< CAU initialization vector high-word  register 1,    Address offset: 0x48 */
    __IO uint32_t IV1LR;               /*!< CAU initialization vector low-word register 1,      Address offset: 0x4C */
} CAU_TypeDef;

/** 
  * @brief RNG 
  */
typedef struct
{
    __IO uint32_t CTLR;                /*!< RNG control register,                               Address offset: 0x00 */
    __IO uint32_t STR;                 /*!< RNG status register,                                Address offset: 0x04 */
    __IO uint32_t DTR;                 /*!< RNG data register,                                  Address offset: 0x08 */
} RNG_TypeDef;

/** 
  * @brief HAU 
  */
typedef struct 
{
    __IO uint32_t CTLR;                /*!< HAU control register,                               Address offset: 0x00 */
    __IO uint32_t DI;                  /*!< HAU data input register,                            Address offset: 0x04 */
    __IO uint32_t STR;                 /*!< HAU start register,                                 Address offset: 0x08 */
    __IO uint32_t DOR[5];              /*!< HAU digest output registers,                        Address offset: 0x0C */
    __IO uint32_t IER;                 /*!< HAU interrupt enable register,                      Address offset: 0x20 */
    __IO uint32_t IFR;                 /*!< HAU interrupt flag register,                        Address offset: 0x24 */
} HAU_TypeDef;

/** 
  * @brief HAU_DIGEST 
  */
typedef struct 
{
    __IO uint32_t DOR[8];              /*!< HAU digest registers,                               Address offset: 0x310-0x32C */ 
} HAU_DIGEST_TypeDef;

/** 
  * @brief dci
  */
typedef struct
{
    __IO uint32_t CTLR;                /*!< DCI control register 1,                             Address offset: 0x00 */
    __IO uint32_t STR;                 /*!< DCI status register,                                Address offset: 0x04 */
    __IO uint32_t FIR;                 /*!< DCI raw interrupt status register,                  Address offset: 0x08 */
    __IO uint32_t IMR;                 /*!< DCI interrupt enable register,                      Address offset: 0x0C */
    __IO uint32_t MFR;                 /*!< DCI masked interrupt status register,               Address offset: 0x10 */
    __IO uint32_t CFR;                 /*!< DCI interrupt clear register,                       Address offset: 0x14 */
    __IO uint32_t ECR;                 /*!< DCI embedded synchronization code register,         Address offset: 0x18 */
    __IO uint32_t ECMR;                /*!< DCI embedded synchronization unmask register,       Address offset: 0x1C */
    __IO uint32_t WSPR;                /*!< DCI crop window start,                              Address offset: 0x20 */
    __IO uint32_t WSZR;                /*!< DCI crop window size,                               Address offset: 0x24 */
    __IO uint32_t PBR;                 /*!< DCI Pixel buffer register,                          Address offset: 0x28 */
} DCI_TypeDef;

/** 
  * @brief tldi
  */typedef struct
{
    uint32_t RESERVED0[2];             /*!< Reserved,                                            Address offset: 0x00 */
    __IO uint32_t PSCR;                /*!< TLDI Synchronization Size Configuration Register,    Address offset: 0x08 */
    __IO uint32_t BPSCR;               /*!< TLDI Back Porch Configuration Register,              Address offset: 0x0C */
    __IO uint32_t ASCR;                /*!< TLDI Active Size Configuration Register,             Address offset: 0x10 */
    __IO uint32_t TSCR;                /*!< TLDI Total Width Configuration Register,             Address offset: 0x14 */
    __IO uint32_t CTLR;                /*!< TLDI Global Control Register,                        Address offset: 0x18 */
    uint32_t RESERVED1[2];             /*!< Reserved,                                            Address offset: 0x1C */
    __IO uint32_t RLCR;                /*!< TLDI Shadow Reload Configuration Register,           Address offset: 0x24 */
    uint32_t RESERVED2[1];             /*!< Reserved,                                            Address offset: 0x28 */
    __IO uint32_t BVCR;                /*!< TLDI Background Color Configuration Register,        Address offset: 0x2C */
    uint32_t RESERVED3[1];             /*!< Reserved,                                            Address offset: 0x30 */
    __IO uint32_t IMR;                 /*!< TLDI Interrupt Enable Register,                      Address offset: 0x34 */
    __IO uint32_t STR;                 /*!< TLDI Interrupt Status Register,                      Address offset: 0x38 */
    __IO uint32_t CFR;                 /*!< TLDI Interrupt Clear Register,                       Address offset: 0x3C */
    __IO uint32_t LRFR;                /*!< TLDI Line Interrupt Position Configuration Register, Address offset: 0x40 */
    __IO uint32_t CPPR;                /*!< TLDI Current Position Status Register,               Address offset: 0x44 */
    __IO uint32_t CISR;                /*!< TLDI Current Display Status Register,                Address offset: 0x48 */
} TLDI_TypeDef; 
 
typedef struct
{  
    __IO uint32_t CTLR;                /*!< TLDI Layerx Control Register                                  Address offset: 0x84 */
    __IO uint32_t HWCR;                /*!< TLDI Layerx Window Horizontal Position Configuration Register Address offset: 0x88 */
    __IO uint32_t VWCR;                /*!< TLDI Layerx Window Vertical Position Configuration Register   Address offset: 0x8C */
    __IO uint32_t CKVR;                /*!< TLDI Layerx Color Keying Configuration Register               Address offset: 0x90 */
    __IO uint32_t PFR;                 /*!< TLDI Layerx Pixel Format Configuration Register               Address offset: 0x94 */
    __IO uint32_t SAR;                 /*!< TLDI Layerx Constant Alpha Configuration Register             Address offset: 0x98 */
    __IO uint32_t DCR;                 /*!< TLDI Layerx Default Color Configuration Register              Address offset: 0x9C */
    __IO uint32_t BMR;                 /*!< TLDI Layerx Blending Factors Configuration Register           Address offset: 0xA0 */
    uint32_t RESERVED0[2];             /*!< Reserved */
    __IO uint32_t FBSAR;               /*!< TLDI Layerx Color Frame Buffer Address Register               Address offset: 0xAC */
    __IO uint32_t FBLR;                /*!< TLDI Layerx Color Frame Buffer Length Register                Address offset: 0xB0 */
    __IO uint32_t FBLNR;               /*!< TLDI Layerx ColorFrame Buffer Line Number Register            Address offset: 0xB4 */
    uint32_t RESERVED1[3];             /*!< Reserved */
    __IO uint32_t LUTWR;               /*!< TLDI Layerx CLUT Write Register                               Address offset: 0x144 */
} TLDI_Layer_TypeDef;

/** @addtogroup Peripheral_memory_map
  * @{
  */

#define FLASH_BASE              ((uint32_t)0x08000000) /*!< FLASH base address in the alias region */
#define SRAM_BASE               ((uint32_t)0x20000000) /*!< SRAM base address in the alias region */
#define PERIPH_BASE             ((uint32_t)0x40000000) /*!< Peripheral base address in the alias region */
#define SRAM_BB_BASE            ((uint32_t)0x22000000) /*!< SRAM base address in the bit-band region */
#define PERIPH_BB_BASE          ((uint32_t)0x42000000) /*!< Peripheral base address in the bit-band region */
#define EXMC_R_BASE             ((uint32_t)0xA0000000) /*!< EXMC registers base address */

/*!< Peripheral memory map */
#define APB1PERIPH_BASE         PERIPH_BASE
#define APB2PERIPH_BASE         (PERIPH_BASE + 0x10000)
#define AHBPERIPH_BASE          (PERIPH_BASE + 0x20000)
#define AHB2PERIPH_BASE         (PERIPH_BASE + 0x10000000)

#define TIMER2_BASE             (APB1PERIPH_BASE + 0x0000)
#define TIMER3_BASE             (APB1PERIPH_BASE + 0x0400)
#define TIMER4_BASE             (APB1PERIPH_BASE + 0x0800)
#define TIMER5_BASE             (APB1PERIPH_BASE + 0x0C00)
#define TIMER6_BASE             (APB1PERIPH_BASE + 0x1000)
#define TIMER7_BASE             (APB1PERIPH_BASE + 0x1400)
#define TIMER12_BASE            (APB1PERIPH_BASE + 0x1800)
#define TIMER13_BASE            (APB1PERIPH_BASE + 0x1C00)
#define TIMER14_BASE            (APB1PERIPH_BASE + 0x2000)
#define RTC_BASE                (APB1PERIPH_BASE + 0x2800)
#define WWDG_BASE               (APB1PERIPH_BASE + 0x2C00)
#define IWDG_BASE               (APB1PERIPH_BASE + 0x3000)
#define SPI2_BASE               (APB1PERIPH_BASE + 0x3800)
#define SPI3_BASE               (APB1PERIPH_BASE + 0x3C00)
#define USART2_BASE             (APB1PERIPH_BASE + 0x4400)
#define USART3_BASE             (APB1PERIPH_BASE + 0x4800)
#define UART4_BASE              (APB1PERIPH_BASE + 0x4C00)
#define UART5_BASE              (APB1PERIPH_BASE + 0x5000)
#define I2C1_BASE               (APB1PERIPH_BASE + 0x5400)
#define I2C2_BASE               (APB1PERIPH_BASE + 0x5800)
#define I2C3_BASE               (APB1PERIPH_BASE + 0xC000)
#define CAN1_BASE               (APB1PERIPH_BASE + 0x6400)
#define CAN2_BASE               (APB1PERIPH_BASE + 0x6800)
#define BKP_BASE                (APB1PERIPH_BASE + 0x6C00)
#define PWR_BASE                (APB1PERIPH_BASE + 0x7000)
#define DAC_BASE                (APB1PERIPH_BASE + 0x7400)
#define UART7_BASE              (APB1PERIPH_BASE + 0x7800)
#define UART8_BASE              (APB1PERIPH_BASE + 0x7C00)

#define AFIO_BASE               (APB2PERIPH_BASE + 0x0000)
#define EXTI_BASE               (APB2PERIPH_BASE + 0x0400)
#define GPIOA_BASE              (APB2PERIPH_BASE + 0x0800)
#define GPIOB_BASE              (APB2PERIPH_BASE + 0x0C00)
#define GPIOC_BASE              (APB2PERIPH_BASE + 0x1000)
#define GPIOD_BASE              (APB2PERIPH_BASE + 0x1400)
#define GPIOE_BASE              (APB2PERIPH_BASE + 0x1800)
#define GPIOF_BASE              (APB2PERIPH_BASE + 0x1C00)
#define GPIOG_BASE              (APB2PERIPH_BASE + 0x2000)
#define ADC1_BASE               (APB2PERIPH_BASE + 0x2400)
#define ADC2_BASE               (APB2PERIPH_BASE + 0x2800)
#define TIMER1_BASE             (APB2PERIPH_BASE + 0x2C00)
#define SPI1_BASE               (APB2PERIPH_BASE + 0x3000)
#define TIMER8_BASE             (APB2PERIPH_BASE + 0x3400)
#define USART1_BASE             (APB2PERIPH_BASE + 0x3800)
#define ADC3_BASE               (APB2PERIPH_BASE + 0x3C00)
#define TIMER15_BASE            (APB2PERIPH_BASE + 0x4000)
#define TIMER16_BASE            (APB2PERIPH_BASE + 0x4400)
#define TIMER17_BASE            (APB2PERIPH_BASE + 0x4800)
#define TIMER9_BASE             (APB2PERIPH_BASE + 0x4C00)
#define TIMER10_BASE            (APB2PERIPH_BASE + 0x5000)
#define TIMER11_BASE            (APB2PERIPH_BASE + 0x5400)
#define TLDI_BASE               (APB2PERIPH_BASE + 0x6800)
#define TLDI_Layer1_BASE        (TLDI_BASE + 0x84)
#define TLDI_Layer2_BASE        (TLDI_BASE + 0x104)
#define USART6_BASE             (APB2PERIPH_BASE + 0x7000)
#define GPIOH_BASE              (APB2PERIPH_BASE + 0x7400)
#define GPIOI_BASE              (APB2PERIPH_BASE + 0x7800)

#define SDIO_BASE               (PERIPH_BASE + 0x18000)
#define DMA1_BASE               (AHBPERIPH_BASE + 0x0000)
#define DMA1_CHANNEL1_BASE      (AHBPERIPH_BASE + 0x0008)
#define DMA1_CHANNEL2_BASE      (AHBPERIPH_BASE + 0x001C)
#define DMA1_CHANNEL3_BASE      (AHBPERIPH_BASE + 0x0030)
#define DMA1_CHANNEL4_BASE      (AHBPERIPH_BASE + 0x0044)
#define DMA1_CHANNEL5_BASE      (AHBPERIPH_BASE + 0x0058)
#define DMA1_CHANNEL6_BASE      (AHBPERIPH_BASE + 0x006C)
#define DMA1_CHANNEL7_BASE      (AHBPERIPH_BASE + 0x0080)
#define DMA2_BASE               (AHBPERIPH_BASE + 0x0400)
#define DMA2_CHANNEL1_BASE      (AHBPERIPH_BASE + 0x0408)
#define DMA2_CHANNEL2_BASE      (AHBPERIPH_BASE + 0x041C)
#define DMA2_CHANNEL3_BASE      (AHBPERIPH_BASE + 0x0430)
#define DMA2_CHANNEL4_BASE      (AHBPERIPH_BASE + 0x0444)
#define DMA2_CHANNEL5_BASE      (AHBPERIPH_BASE + 0x0458)
#define DMA2_CHANNEL6_BASE      (AHBPERIPH_BASE + 0x046C)      
#define DMA2_CHANNEL7_BASE      (AHBPERIPH_BASE + 0x0480)
#define RCC_BASE                (AHBPERIPH_BASE + 0x1000)
#define CRC_BASE                (AHBPERIPH_BASE + 0x3000)
#define FMC_R_BASE              (AHBPERIPH_BASE + 0x2000) 
#define OB_BASE                 ((uint32_t)0x1FFFF800)    
#define ETH_BASE                (AHBPERIPH_BASE + 0x8000)
#define ETH_MAC_BASE            (ETH_BASE)
#define ETH_MSC_BASE            (ETH_BASE + 0x0100)
#define ETH_PTP_BASE            (ETH_BASE + 0x0700)
#define ETH_DMA_BASE            (ETH_BASE + 0x1000)
#define EXMC_BANK1_R_BASE       (EXMC_R_BASE + 0x0000)
#define EXMC_BANK1_WT_R_BASE    (EXMC_R_BASE + 0x0104)
#define EXMC_BANK1_SQPI_R_BASE  (EXMC_R_BASE + 0x0310)
#define EXMC_BANK2_R_BASE       (EXMC_R_BASE + 0x0060)
#define EXMC_BANK3_R_BASE       (EXMC_R_BASE + 0x0080)
#define EXMC_BANK4_R_BASE       (EXMC_R_BASE + 0x00A0)
#define EXMC_BANK5_6_R_BASE     (EXMC_R_BASE + 0x0140)
#define MCUDBG_BASE             ((uint32_t)0xE0042000)

#define DCI_BASE                (AHB2PERIPH_BASE + 0x50000)
#define CAU_BASE                (AHB2PERIPH_BASE + 0x60000)
#define HAU_BASE                (AHB2PERIPH_BASE + 0x60400)
#define RNG_BASE                (AHB2PERIPH_BASE + 0x60800)
#define HAU_DIGEST_BASE         (AHB2PERIPH_BASE + 0x60710)


/**
  * @}
  */
  
/** @addtogroup Peripheral_declaration
  * @{
  */  

#define TIMER2                  ((TIMER_TypeDef *) TIMER2_BASE)
#define TIMER3                  ((TIMER_TypeDef *) TIMER3_BASE)
#define TIMER4                  ((TIMER_TypeDef *) TIMER4_BASE)
#define TIMER5                  ((TIMER_TypeDef *) TIMER5_BASE)
#define TIMER6                  ((TIMER_TypeDef *) TIMER6_BASE)
#define TIMER7                  ((TIMER_TypeDef *) TIMER7_BASE)
#define TIMER12                 ((TIMER_TypeDef *) TIMER12_BASE)
#define TIMER13                 ((TIMER_TypeDef *) TIMER13_BASE)
#define TIMER14                 ((TIMER_TypeDef *) TIMER14_BASE)
#define RTC                     ((RTC_TypeDef *) RTC_BASE)
#define WWDG                    ((WWDG_TypeDef *) WWDG_BASE)
#define IWDG                    ((IWDG_TypeDef *) IWDG_BASE)
#define SPI2                    ((SPI_TypeDef *) SPI2_BASE)
#define SPI3                    ((SPI_TypeDef *) SPI3_BASE)
#define USART2                  ((USART_TypeDef *) USART2_BASE)
#define USART3                  ((USART_TypeDef *) USART3_BASE)
#define UART4                   ((USART_TypeDef *) UART4_BASE)
#define UART5                   ((USART_TypeDef *) UART5_BASE)
#define USART6                  ((USART_TypeDef *) USART6_BASE)
#define UART7                   ((USART_TypeDef *) UART7_BASE)
#define UART8                   ((USART_TypeDef *) UART8_BASE)
#define I2C1                    ((I2C_TypeDef *) I2C1_BASE)
#define I2C2                    ((I2C_TypeDef *) I2C2_BASE)
#define I2C3                    ((I2C_TypeDef *) I2C3_BASE)
#define CAN1                    ((CAN_TypeDef *) CAN1_BASE)
#define CAN2                    ((CAN_TypeDef *) CAN2_BASE)
#define BKP                     ((BKP_TypeDef *) BKP_BASE)
#define PWR                     ((PWR_TypeDef *) PWR_BASE)
#define DAC                     ((DAC_TypeDef *) DAC_BASE)
#define AFIO                    ((AFIO_TypeDef *) AFIO_BASE)
#define EXTI                    ((EXTI_TypeDef *) EXTI_BASE)
#define GPIOA                   ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB                   ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC                   ((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD                   ((GPIO_TypeDef *) GPIOD_BASE)
#define GPIOE                   ((GPIO_TypeDef *) GPIOE_BASE)
#define GPIOF                   ((GPIO_TypeDef *) GPIOF_BASE)
#define GPIOG                   ((GPIO_TypeDef *) GPIOG_BASE)
#define GPIOH                   ((GPIO_TypeDef *) GPIOH_BASE)
#define GPIOI                   ((GPIO_TypeDef *) GPIOI_BASE)
#define ADC1                    ((ADC_TypeDef *) ADC1_BASE)
#define ADC2                    ((ADC_TypeDef *) ADC2_BASE)
#define TIMER1                  ((TIMER_TypeDef *) TIMER1_BASE)
#define SPI1                    ((SPI_TypeDef *) SPI1_BASE)
#define TIMER8                  ((TIMER_TypeDef *) TIMER8_BASE)
#define USART1                  ((USART_TypeDef *) USART1_BASE)
#define ADC3                    ((ADC_TypeDef *) ADC3_BASE)
#define TIMER15                 ((TIMER_TypeDef *) TIMER15_BASE)
#define TIMER16                 ((TIMER_TypeDef *) TIMER16_BASE)
#define TIMER17                 ((TIMER_TypeDef *) TIMER17_BASE)
#define TIMER9                  ((TIMER_TypeDef *) TIMER9_BASE)
#define TIMER10                 ((TIMER_TypeDef *) TIMER10_BASE)
#define TIMER11                 ((TIMER_TypeDef *) TIMER11_BASE)
#define SDIO                    ((SDIO_TypeDef *) SDIO_BASE)
#define DMA1                    ((DMA_TypeDef *) DMA1_BASE)
#define DMA2                    ((DMA_TypeDef *) DMA2_BASE)
#define DMA1_CHANNEL1           ((DMA_Channel_TypeDef *) DMA1_CHANNEL1_BASE)
#define DMA1_CHANNEL2           ((DMA_Channel_TypeDef *) DMA1_CHANNEL2_BASE)
#define DMA1_CHANNEL3           ((DMA_Channel_TypeDef *) DMA1_CHANNEL3_BASE)
#define DMA1_CHANNEL4           ((DMA_Channel_TypeDef *) DMA1_CHANNEL4_BASE)
#define DMA1_CHANNEL5           ((DMA_Channel_TypeDef *) DMA1_CHANNEL5_BASE)
#define DMA1_CHANNEL6           ((DMA_Channel_TypeDef *) DMA1_CHANNEL6_BASE)
#define DMA1_CHANNEL7           ((DMA_Channel_TypeDef *) DMA1_CHANNEL7_BASE)
#define DMA2_CHANNEL1           ((DMA_Channel_TypeDef *) DMA2_CHANNEL1_BASE)
#define DMA2_CHANNEL2           ((DMA_Channel_TypeDef *) DMA2_CHANNEL2_BASE)
#define DMA2_CHANNEL3           ((DMA_Channel_TypeDef *) DMA2_CHANNEL3_BASE)
#define DMA2_CHANNEL4           ((DMA_Channel_TypeDef *) DMA2_CHANNEL4_BASE)
#define DMA2_CHANNEL5           ((DMA_Channel_TypeDef *) DMA2_CHANNEL5_BASE)
#define DMA2_CHANNEL6           ((DMA_Channel_TypeDef *) DMA2_CHANNEL6_BASE) 
#define DMA2_CHANNEL7           ((DMA_Channel_TypeDef *) DMA2_CHANNEL7_BASE)  
#define RCC                     ((RCC_TypeDef *) RCC_BASE)
#define CRC                     ((CRC_TypeDef *) CRC_BASE)
#define FMC                     ((FMC_TypeDef *) FMC_R_BASE)
#define OB                      ((OB_TypeDef *) OB_BASE) 
#define EXMC_BANK1              ((EXMC_Bank1_TypeDef *) EXMC_BANK1_R_BASE)
#define EXMC_BANK1_WT           ((EXMC_Bank1_WT_TypeDef *) EXMC_BANK1_WT_R_BASE)
#define EXMC_BANK1_SQPI         ((EXMC_Bank1_SQPI_TypeDef *) EXMC_BANK1_SQPI_R_BASE)
#define EXMC_BANK2              ((EXMC_Bank2_TypeDef *) EXMC_BANK2_R_BASE)
#define EXMC_BANK3              ((EXMC_Bank3_TypeDef *) EXMC_BANK3_R_BASE)
#define EXMC_BANK4              ((EXMC_Bank4_TypeDef *) EXMC_BANK4_R_BASE)
#define EXMC_BANK5_6            ((EXMC_Bank5_6_TypeDef *) EXMC_BANK5_6_R_BASE)
#define MCUDBG                  ((MCUDBG_TypeDef *) MCUDBG_BASE)
#define ETH_MAC                 ((ETH_MAC_TypeDef *) ETH_MAC_BASE)
#define ETH_MSC                 ((ETH_MSC_TypeDef *) ETH_MSC_BASE)
#define ETH_PTP                 ((ETH_PTP_TypeDef *) ETH_PTP_BASE)
#define ETH_DMA                 ((ETH_DMA_Typedef *) ETH_DMA_BASE)
#define RNG                     ((RNG_TypeDef *) RNG_BASE)
#define HAU                     ((HAU_TypeDef *) HAU_BASE)
#define HAU_DIGEST              ((HAU_DIGEST_TypeDef *) HAU_DIGEST_BASE)
#define CAU                     ((CAU_TypeDef *) CAU_BASE)
#define DCI                     ((DCI_TypeDef *) DCI_BASE)
#define TLDI                    ((TLDI_TypeDef *)TLDI_BASE)
#define TLDI_Layer1             ((TLDI_Layer_TypeDef *)TLDI_Layer1_BASE)
#define TLDI_Layer2             ((TLDI_Layer_TypeDef *)TLDI_Layer2_BASE)

/**
  * @}
  */

/** @addtogroup Exported_constants
  * @{
  */
  
  /** @addtogroup Peripheral_Registers_Bits_Definition
  * @{
  */

/******************************************************************************/
/*                         Peripheral Registers Bits Definition               */
/******************************************************************************/
/******************************************************************************/
/*                                                                            */
/*                        Analog to Digital Converter                         */
/*                                                                            */
/******************************************************************************/

/********************  Bit definition for ADC_STR register  ********************/
#define  ADC_STR_AWE                                ((uint8_t)0x01)               /*!< Analog watchdog flag */
#define  ADC_STR_EORC                               ((uint8_t)0x02)               /*!< End of conversion */
#define  ADC_STR_EOIC                               ((uint8_t)0x04)               /*!< Injected channel end of conversion */
#define  ADC_STR_STIC                               ((uint8_t)0x08)               /*!< Injected channel Start flag */
#define  ADC_STR_STRC                               ((uint8_t)0x10)               /*!< Regular channel Start flag */

/*******************  Bit definition for ADC_CTLR1 register  ********************/
#define  ADC_CTLR1_AWCS                             ((uint32_t)0x0000001F)        /*!< AWCS[4:0] bits (Analog watchdog channel select bits) */
#define  ADC_CTLR1_AWCS_0                           ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  ADC_CTLR1_AWCS_1                           ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  ADC_CTLR1_AWCS_2                           ((uint32_t)0x00000004)        /*!< Bit 2 */
#define  ADC_CTLR1_AWCS_3                           ((uint32_t)0x00000008)        /*!< Bit 3 */
#define  ADC_CTLR1_AWCS_4                           ((uint32_t)0x00000010)        /*!< Bit 4 */

#define  ADC_CTLR1_EORCIE                           ((uint32_t)0x00000020)        /*!< Interrupt enable for EOC */
#define  ADC_CTLR1_AWEIE                            ((uint32_t)0x00000040)        /*!< Analog Watchdog interrupt enable */
#define  ADC_CTLR1_EOICIE                           ((uint32_t)0x00000080)        /*!< Interrupt enable for injected channels */
#define  ADC_CTLR1_SM                               ((uint32_t)0x00000100)        /*!< Scan mode */
#define  ADC_CTLR1_AWSSM                            ((uint32_t)0x00000200)        /*!< Enable the watchdog on a single channel in scan mode */
#define  ADC_CTLR1_ICA                              ((uint32_t)0x00000400)        /*!< Automatic injected group conversion */
#define  ADC_CTLR1_DISRC                            ((uint32_t)0x00000800)        /*!< Discontinuous mode on regular channels */
#define  ADC_CTLR1_DISIC                            ((uint32_t)0x00001000)        /*!< Discontinuous mode on injected channels */

#define  ADC_CTLR1_DISNUM                           ((uint32_t)0x0000E000)        /*!< DISNUM[2:0] bits (Discontinuous mode channel count) */
#define  ADC_CTLR1_DISNUM_0                         ((uint32_t)0x00002000)        /*!< Bit 0 */
#define  ADC_CTLR1_DISNUM_1                         ((uint32_t)0x00004000)        /*!< Bit 1 */
#define  ADC_CTLR1_DISNUM_2                         ((uint32_t)0x00008000)        /*!< Bit 2 */

#define  ADC_CTLR1_AWIEN                            ((uint32_t)0x00400000)        /*!< Analog watchdog enable on injected channels */
#define  ADC_CTLR1_AWREN                            ((uint32_t)0x00800000)        /*!< Analog watchdog enable on regular channels */

/*******************  Bit definition for ADC_CTLR2 register  ********************/
#define  ADC_CTLR2_ADCON                            ((uint32_t)0x00000001)        /*!< A/D Converter ON / OFF */
#define  ADC_CTLR2_CTN                              ((uint32_t)0x00000002)        /*!< Continuous Conversion */
#define  ADC_CTLR2_CLB                              ((uint32_t)0x00000004)        /*!< A/D Calibration */
#define  ADC_CTLR2_RSTCLB                           ((uint32_t)0x00000008)        /*!< Reset Calibration */
#define  ADC_CTLR2_DMA                              ((uint32_t)0x00000100)        /*!< Direct Memory access mode */
#define  ADC_CTLR2_DAL                              ((uint32_t)0x00000800)        /*!< Data Alignment */

#define  ADC_CTLR2_ETSIC                            ((uint32_t)0x00007000)        /*!< ETSIC[2:0] bits (External event select for injected group) */
#define  ADC_CTLR2_ETSIC_0                          ((uint32_t)0x00001000)        /*!< Bit 0 */
#define  ADC_CTLR2_ETSIC_1                          ((uint32_t)0x00002000)        /*!< Bit 1 */
#define  ADC_CTLR2_ETSIC_2                          ((uint32_t)0x00004000)        /*!< Bit 2 */

#define  ADC_CTLR2_ETEIC                            ((uint32_t)0x00008000)        /*!< External Trigger Conversion mode for injected channels */

#define  ADC_CTLR2_ETSRC                            ((uint32_t)0x000E0000)        /*!< ETSRC[2:0] bits (External Event Select for regular group) */
#define  ADC_CTLR2_ETSRC_0                          ((uint32_t)0x00020000)        /*!< Bit 0 */
#define  ADC_CTLR2_ETSRC_1                          ((uint32_t)0x00040000)        /*!< Bit 1 */
#define  ADC_CTLR2_ETSRC_2                          ((uint32_t)0x00080000)        /*!< Bit 2 */

#define  ADC_CTLR2_ETERC                            ((uint32_t)0x00100000)        /*!< External Trigger Conversion mode for regular channels */
#define  ADC_CTLR2_SWICST                           ((uint32_t)0x00200000)        /*!< Start Conversion of injected channels */
#define  ADC_CTLR2_SWRCST                           ((uint32_t)0x00400000)        /*!< Start Conversion of regular channels */
#define  ADC_CTLR2_TSVREN                           ((uint32_t)0x00800000)        /*!< Temperature Sensor and VREFINT Enable */
#define  ADC_CTLR2_VBATEN                           ((uint32_t)0x00C00000)        /*!< VBAT  Enable */
/******************  Bit definition for ADC_SPT1 register  *******************/
#define  ADC_SPT1_SPT10                             ((uint32_t)0x00000007)        /*!< SPT10[2:0] bits (Channel 10 Sample time selection) */
#define  ADC_SPT1_SPT10_0                           ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  ADC_SPT1_SPT10_1                           ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  ADC_SPT1_SPT10_2                           ((uint32_t)0x00000004)        /*!< Bit 2 */

#define  ADC_SPT1_SPT11                             ((uint32_t)0x00000038)        /*!< SPT11[2:0] bits (Channel 11 Sample time selection) */
#define  ADC_SPT1_SPT11_0                           ((uint32_t)0x00000008)        /*!< Bit 0 */
#define  ADC_SPT1_SPT11_1                           ((uint32_t)0x00000010)        /*!< Bit 1 */
#define  ADC_SPT1_SPT11_2                           ((uint32_t)0x00000020)        /*!< Bit 2 */

#define  ADC_SPT1_SPT12                             ((uint32_t)0x000001C0)        /*!< SPT12[2:0] bits (Channel 12 Sample time selection) */
#define  ADC_SPT1_SPT12_0                           ((uint32_t)0x00000040)        /*!< Bit 0 */
#define  ADC_SPT1_SPT12_1                           ((uint32_t)0x00000080)        /*!< Bit 1 */
#define  ADC_SPT1_SPT12_2                           ((uint32_t)0x00000100)        /*!< Bit 2 */

#define  ADC_SPT1_SPT13                             ((uint32_t)0x00000E00)        /*!< SPT13[2:0] bits (Channel 13 Sample time selection) */
#define  ADC_SPT1_SPT13_0                           ((uint32_t)0x00000200)        /*!< Bit 0 */
#define  ADC_SPT1_SPT13_1                           ((uint32_t)0x00000400)        /*!< Bit 1 */
#define  ADC_SPT1_SPT13_2                           ((uint32_t)0x00000800)        /*!< Bit 2 */

#define  ADC_SPT1_SPT14                             ((uint32_t)0x00007000)        /*!< SPT14[2:0] bits (Channel 14 Sample time selection) */
#define  ADC_SPT1_SPT14_0                           ((uint32_t)0x00001000)        /*!< Bit 0 */
#define  ADC_SPT1_SPT14_1                           ((uint32_t)0x00002000)        /*!< Bit 1 */
#define  ADC_SPT1_SPT14_2                           ((uint32_t)0x00004000)        /*!< Bit 2 */

#define  ADC_SPT1_SPT15                             ((uint32_t)0x00038000)        /*!< SPT15[2:0] bits (Channel 15 Sample time selection) */
#define  ADC_SPT1_SPT15_0                           ((uint32_t)0x00008000)        /*!< Bit 0 */
#define  ADC_SPT1_SPT15_1                           ((uint32_t)0x00010000)        /*!< Bit 1 */
#define  ADC_SPT1_SPT15_2                           ((uint32_t)0x00020000)        /*!< Bit 2 */

#define  ADC_SPT1_SPT16                             ((uint32_t)0x001C0000)        /*!< SPT16[2:0] bits (Channel 16 Sample time selection) */
#define  ADC_SPT1_SPT16_0                           ((uint32_t)0x00040000)        /*!< Bit 0 */
#define  ADC_SPT1_SPT16_1                           ((uint32_t)0x00080000)        /*!< Bit 1 */
#define  ADC_SPT1_SPT16_2                           ((uint32_t)0x00100000)        /*!< Bit 2 */

#define  ADC_SPT1_SPT17                             ((uint32_t)0x00E00000)        /*!< SPT17[2:0] bits (Channel 17 Sample time selection) */
#define  ADC_SPT1_SPT17_0                           ((uint32_t)0x00200000)        /*!< Bit 0 */
#define  ADC_SPT1_SPT17_1                           ((uint32_t)0x00400000)        /*!< Bit 1 */
#define  ADC_SPT1_SPT17_2                           ((uint32_t)0x00800000)        /*!< Bit 2 */

/******************  Bit definition for ADC_SPT2 register  *******************/
#define  ADC_SPT2_SPT0                              ((uint32_t)0x00000007)        /*!< SPT0[2:0] bits (Channel 0 Sample time selection) */
#define  ADC_SPT2_SPT0_0                            ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  ADC_SPT2_SPT0_1                            ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  ADC_SPT2_SPT0_2                            ((uint32_t)0x00000004)        /*!< Bit 2 */

#define  ADC_SPT2_SPT1                              ((uint32_t)0x00000038)        /*!< SPT1[2:0] bits (Channel 1 Sample time selection) */
#define  ADC_SPT2_SPT1_0                            ((uint32_t)0x00000008)        /*!< Bit 0 */
#define  ADC_SPT2_SPT1_1                            ((uint32_t)0x00000010)        /*!< Bit 1 */
#define  ADC_SPT2_SPT1_2                            ((uint32_t)0x00000020)        /*!< Bit 2 */

#define  ADC_SPT2_SPT2                              ((uint32_t)0x000001C0)        /*!< SPT2[2:0] bits (Channel 2 Sample time selection) */
#define  ADC_SPT2_SPT2_0                            ((uint32_t)0x00000040)        /*!< Bit 0 */
#define  ADC_SPT2_SPT2_1                            ((uint32_t)0x00000080)        /*!< Bit 1 */
#define  ADC_SPT2_SPT2_2                            ((uint32_t)0x00000100)        /*!< Bit 2 */
							        
#define  ADC_SPT2_SPT3                              ((uint32_t)0x00000E00)        /*!< SPT3[2:0] bits (Channel 3 Sample time selection) */
#define  ADC_SPT2_SPT3_0                            ((uint32_t)0x00000200)        /*!< Bit 0 */
#define  ADC_SPT2_SPT3_1                            ((uint32_t)0x00000400)        /*!< Bit 1 */
#define  ADC_SPT2_SPT3_2                            ((uint32_t)0x00000800)        /*!< Bit 2 */

#define  ADC_SPT2_SPT4                              ((uint32_t)0x00007000)        /*!< SPT4[2:0] bits (Channel 4 Sample time selection) */
#define  ADC_SPT2_SPT4_0                            ((uint32_t)0x00001000)        /*!< Bit 0 */
#define  ADC_SPT2_SPT4_1                            ((uint32_t)0x00002000)        /*!< Bit 1 */
#define  ADC_SPT2_SPT4_2                            ((uint32_t)0x00004000)        /*!< Bit 2 */

#define  ADC_SPT2_SPT5                              ((uint32_t)0x00038000)        /*!< SPT5[2:0] bits (Channel 5 Sample time selection) */
#define  ADC_SPT2_SPT5_0                            ((uint32_t)0x00008000)        /*!< Bit 0 */
#define  ADC_SPT2_SPT5_1                            ((uint32_t)0x00010000)        /*!< Bit 1 */
#define  ADC_SPT2_SPT5_2                            ((uint32_t)0x00020000)        /*!< Bit 2 */

#define  ADC_SPT2_SPT6                              ((uint32_t)0x001C0000)        /*!< SPT6[2:0] bits (Channel 6 Sample time selection) */
#define  ADC_SPT2_SPT6_0                            ((uint32_t)0x00040000)        /*!< Bit 0 */
#define  ADC_SPT2_SPT6_1                            ((uint32_t)0x00080000)        /*!< Bit 1 */
#define  ADC_SPT2_SPT6_2                            ((uint32_t)0x00100000)        /*!< Bit 2 */

#define  ADC_SPT2_SPT7                              ((uint32_t)0x00E00000)        /*!< SPT7[2:0] bits (Channel 7 Sample time selection) */
#define  ADC_SPT2_SPT7_0                            ((uint32_t)0x00200000)        /*!< Bit 0 */
#define  ADC_SPT2_SPT7_1                            ((uint32_t)0x00400000)        /*!< Bit 1 */
#define  ADC_SPT2_SPT7_2                            ((uint32_t)0x00800000)        /*!< Bit 2 */
							         
#define  ADC_SPT2_SPT8                              ((uint32_t)0x07000000)        /*!< SPT8[2:0] bits (Channel 8 Sample time selection) */
#define  ADC_SPT2_SPT8_0                            ((uint32_t)0x01000000)        /*!< Bit 0 */
#define  ADC_SPT2_SPT8_1                            ((uint32_t)0x02000000)        /*!< Bit 1 */
#define  ADC_SPT2_SPT8_2                            ((uint32_t)0x04000000)        /*!< Bit 2 */

#define  ADC_SPT2_SPT9                              ((uint32_t)0x38000000)        /*!< SPT9[2:0] bits (Channel 9 Sample time selection) */
#define  ADC_SPT2_SPT9_0                            ((uint32_t)0x08000000)        /*!< Bit 0 */
#define  ADC_SPT2_SPT9_1                            ((uint32_t)0x10000000)        /*!< Bit 1 */
#define  ADC_SPT2_SPT9_2                            ((uint32_t)0x20000000)        /*!< Bit 2 */

/******************  Bit definition for ADC_ICOS1 register  *******************/
#define  ADC_ICOS1_ICOS1                            ((uint16_t)0x0FFF)            /*!< Data offset for injected channel 1 */

/******************  Bit definition for ADC_ICOS2 register  *******************/
#define  ADC_ICOS2_ICOS2                            ((uint16_t)0x0FFF)            /*!< Data offset for injected channel 2 */

/******************  Bit definition for ADC_ICOS3 register  *******************/
#define  ADC_ICOS3_ICOS3                            ((uint16_t)0x0FFF)            /*!< Data offset for injected channel 3 */

/******************  Bit definition for ADC_ICOS4 register  *******************/
#define  ADC_ICOS4_ICOS4                            ((uint16_t)0x0FFF)            /*!< Data offset for injected channel 4 */

/*******************  Bit definition for ADC_AWHT register  ********************/
#define  ADC_AWHT_AWHT                              ((uint16_t)0x0FFF)            /*!< Analog watchdog high threshold */

/*******************  Bit definition for ADC_AWLT register  ********************/
#define  ADC_AWLT_AWLT                              ((uint16_t)0x0FFF)            /*!< Analog watchdog low threshold */

/*******************  Bit definition for ADC_RSQ1 register  *******************/
#define  ADC_RSQ1_RSQ13                             ((uint32_t)0x0000001F)        /*!< RSQ13[4:0] bits (13th conversion in regular sequence) */
#define  ADC_RSQ1_RSQ13_0                           ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  ADC_RSQ1_RSQ13_1                           ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  ADC_RSQ1_RSQ13_2                           ((uint32_t)0x00000004)        /*!< Bit 2 */
#define  ADC_RSQ1_RSQ13_3                           ((uint32_t)0x00000008)        /*!< Bit 3 */
#define  ADC_RSQ1_RSQ13_4                           ((uint32_t)0x00000010)        /*!< Bit 4 */

#define  ADC_RSQ1_RSQ14                             ((uint32_t)0x000003E0)        /*!< RSQ14[4:0] bits (14th conversion in regular sequence) */
#define  ADC_RSQ1_RSQ14_0                           ((uint32_t)0x00000020)        /*!< Bit 0 */
#define  ADC_RSQ1_RSQ14_1                           ((uint32_t)0x00000040)        /*!< Bit 1 */
#define  ADC_RSQ1_RSQ14_2                           ((uint32_t)0x00000080)        /*!< Bit 2 */
#define  ADC_RSQ1_RSQ14_3                           ((uint32_t)0x00000100)        /*!< Bit 3 */
#define  ADC_RSQ1_RSQ14_4                           ((uint32_t)0x00000200)        /*!< Bit 4 */

#define  ADC_RSQ1_RSQ15                             ((uint32_t)0x00007C00)        /*!< RSQ15[4:0] bits (15th conversion in regular sequence) */
#define  ADC_RSQ1_RSQ15_0                           ((uint32_t)0x00000400)        /*!< Bit 0 */
#define  ADC_RSQ1_RSQ15_1                           ((uint32_t)0x00000800)        /*!< Bit 1 */
#define  ADC_RSQ1_RSQ15_2                           ((uint32_t)0x00001000)        /*!< Bit 2 */
#define  ADC_RSQ1_RSQ15_3                           ((uint32_t)0x00002000)        /*!< Bit 3 */
#define  ADC_RSQ1_RSQ15_4                           ((uint32_t)0x00004000)        /*!< Bit 4 */

#define  ADC_RSQ1_RSQ16                             ((uint32_t)0x000F8000)        /*!< RSQ16[4:0] bits (16th conversion in regular sequence) */
#define  ADC_RSQ1_RSQ16_0                           ((uint32_t)0x00008000)        /*!< Bit 0 */
#define  ADC_RSQ1_RSQ16_1                           ((uint32_t)0x00010000)        /*!< Bit 1 */
#define  ADC_RSQ1_RSQ16_2                           ((uint32_t)0x00020000)        /*!< Bit 2 */
#define  ADC_RSQ1_RSQ16_3                           ((uint32_t)0x00040000)        /*!< Bit 3 */
#define  ADC_RSQ1_RSQ16_4                           ((uint32_t)0x00080000)        /*!< Bit 4 */
										 
#define  ADC_RSQ1_RL                                ((uint32_t)0x00F00000)        /*!< RL[3:0] bits (Regular channel sequence length) */
#define  ADC_RSQ1_RL_0                              ((uint32_t)0x00100000)        /*!< Bit 0 */
#define  ADC_RSQ1_RL_1                              ((uint32_t)0x00200000)        /*!< Bit 1 */
#define  ADC_RSQ1_RL_2                              ((uint32_t)0x00400000)        /*!< Bit 2 */
#define  ADC_RSQ1_RL_3                              ((uint32_t)0x00800000)        /*!< Bit 3 */

/*******************  Bit definition for ADC_RSQ2 register  *******************/
#define  ADC_RSQ2_RSQ7                              ((uint32_t)0x0000001F)        /*!< RSQ7[4:0] bits (7th conversion in regular sequence) */
#define  ADC_RSQ2_RSQ7_0                            ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  ADC_RSQ2_RSQ7_1                            ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  ADC_RSQ2_RSQ7_2                            ((uint32_t)0x00000004)        /*!< Bit 2 */
#define  ADC_RSQ2_RSQ7_3                            ((uint32_t)0x00000008)        /*!< Bit 3 */
#define  ADC_RSQ2_RSQ7_4                            ((uint32_t)0x00000010)        /*!< Bit 4 */
									     
#define  ADC_RSQ2_RSQ8                              ((uint32_t)0x000003E0)        /*!< RSQ8[4:0] bits (8th conversion in regular sequence) */
#define  ADC_RSQ2_RSQ8_0                            ((uint32_t)0x00000020)        /*!< Bit 0 */
#define  ADC_RSQ2_RSQ8_1                            ((uint32_t)0x00000040)        /*!< Bit 1 */
#define  ADC_RSQ2_RSQ8_2                            ((uint32_t)0x00000080)        /*!< Bit 2 */
#define  ADC_RSQ2_RSQ8_3                            ((uint32_t)0x00000100)        /*!< Bit 3 */
#define  ADC_RSQ2_RSQ8_4                            ((uint32_t)0x00000200)        /*!< Bit 4 */
									 	    
#define  ADC_RSQ2_RSQ9                              ((uint32_t)0x00007C00)        /*!< RSQ9[4:0] bits (9th conversion in regular sequence) */
#define  ADC_RSQ2_RSQ9_0                            ((uint32_t)0x00000400)        /*!< Bit 0 */
#define  ADC_RSQ2_RSQ9_1                            ((uint32_t)0x00000800)        /*!< Bit 1 */
#define  ADC_RSQ2_RSQ9_2                            ((uint32_t)0x00001000)        /*!< Bit 2 */
#define  ADC_RSQ2_RSQ9_3                            ((uint32_t)0x00002000)        /*!< Bit 3 */
#define  ADC_RSQ2_RSQ9_4                            ((uint32_t)0x00004000)        /*!< Bit 4 */

#define  ADC_RSQ2_RSQ10                             ((uint32_t)0x000F8000)        /*!< RSQ10[4:0] bits (10th conversion in regular sequence) */
#define  ADC_RSQ2_RSQ10_0                           ((uint32_t)0x00008000)        /*!< Bit 0 */
#define  ADC_RSQ2_RSQ10_1                           ((uint32_t)0x00010000)        /*!< Bit 1 */
#define  ADC_RSQ2_RSQ10_2                           ((uint32_t)0x00020000)        /*!< Bit 2 */
#define  ADC_RSQ2_RSQ10_3                           ((uint32_t)0x00040000)        /*!< Bit 3 */
#define  ADC_RSQ2_RSQ10_4                           ((uint32_t)0x00080000)        /*!< Bit 4 */

#define  ADC_RSQ2_RSQ11                             ((uint32_t)0x01F00000)        /*!< RSQ11[4:0] bits (11th conversion in regular sequence) */
#define  ADC_RSQ2_RSQ11_0                           ((uint32_t)0x00100000)        /*!< Bit 0 */
#define  ADC_RSQ2_RSQ11_1                           ((uint32_t)0x00200000)        /*!< Bit 1 */
#define  ADC_RSQ2_RSQ11_2                           ((uint32_t)0x00400000)        /*!< Bit 2 */
#define  ADC_RSQ2_RSQ11_3                           ((uint32_t)0x00800000)        /*!< Bit 3 */
#define  ADC_RSQ2_RSQ11_4                           ((uint32_t)0x01000000)        /*!< Bit 4 */

#define  ADC_RSQ2_RSQ12                             ((uint32_t)0x3E000000)        /*!< RSQ12[4:0] bits (12th conversion in regular sequence) */
#define  ADC_RSQ2_RSQ12_0                           ((uint32_t)0x02000000)        /*!< Bit 0 */
#define  ADC_RSQ2_RSQ12_1                           ((uint32_t)0x04000000)        /*!< Bit 1 */
#define  ADC_RSQ2_RSQ12_2                           ((uint32_t)0x08000000)        /*!< Bit 2 */
#define  ADC_RSQ2_RSQ12_3                           ((uint32_t)0x10000000)        /*!< Bit 3 */
#define  ADC_RSQ2_RSQ12_4                           ((uint32_t)0x20000000)        /*!< Bit 4 */

/*******************  Bit definition for ADC_RSQ3 register  *******************/
#define  ADC_RSQ3_RSQ1                              ((uint32_t)0x0000001F)        /*!< RSQ1[4:0] bits (1st conversion in regular sequence) */
#define  ADC_RSQ3_RSQ1_0                            ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  ADC_RSQ3_RSQ1_1                            ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  ADC_RSQ3_RSQ1_2                            ((uint32_t)0x00000004)        /*!< Bit 2 */
#define  ADC_RSQ3_RSQ1_3                            ((uint32_t)0x00000008)        /*!< Bit 3 */
#define  ADC_RSQ3_RSQ1_4                            ((uint32_t)0x00000010)        /*!< Bit 4 */

#define  ADC_RSQ3_RSQ2                              ((uint32_t)0x000003E0)        /*!< RSQ2[4:0] bits (2nd conversion in regular sequence) */
#define  ADC_RSQ3_RSQ2_0                            ((uint32_t)0x00000020)        /*!< Bit 0 */
#define  ADC_RSQ3_RSQ2_1                            ((uint32_t)0x00000040)        /*!< Bit 1 */
#define  ADC_RSQ3_RSQ2_2                            ((uint32_t)0x00000080)        /*!< Bit 2 */
#define  ADC_RSQ3_RSQ2_3                            ((uint32_t)0x00000100)        /*!< Bit 3 */
#define  ADC_RSQ3_RSQ2_4                            ((uint32_t)0x00000200)        /*!< Bit 4 */

#define  ADC_RSQ3_RSQ3                              ((uint32_t)0x00007C00)        /*!< RSQ3[4:0] bits (3rd conversion in regular sequence) */
#define  ADC_RSQ3_RSQ3_0                            ((uint32_t)0x00000400)        /*!< Bit 0 */
#define  ADC_RSQ3_RSQ3_1                            ((uint32_t)0x00000800)        /*!< Bit 1 */
#define  ADC_RSQ3_RSQ3_2                            ((uint32_t)0x00001000)        /*!< Bit 2 */
#define  ADC_RSQ3_RSQ3_3                            ((uint32_t)0x00002000)        /*!< Bit 3 */
#define  ADC_RSQ3_RSQ3_4                            ((uint32_t)0x00004000)        /*!< Bit 4 */
											 
#define  ADC_RSQ3_RSQ4                              ((uint32_t)0x000F8000)        /*!< RSQ4[4:0] bits (4th conversion in regular sequence) */
#define  ADC_RSQ3_RSQ4_0                            ((uint32_t)0x00008000)        /*!< Bit 0 */
#define  ADC_RSQ3_RSQ4_1                            ((uint32_t)0x00010000)        /*!< Bit 1 */
#define  ADC_RSQ3_RSQ4_2                            ((uint32_t)0x00020000)        /*!< Bit 2 */
#define  ADC_RSQ3_RSQ4_3                            ((uint32_t)0x00040000)        /*!< Bit 3 */
#define  ADC_RSQ3_RSQ4_4                            ((uint32_t)0x00080000)        /*!< Bit 4 */

#define  ADC_RSQ3_RSQ5                              ((uint32_t)0x01F00000)        /*!< RSQ5[4:0] bits (5th conversion in regular sequence) */
#define  ADC_RSQ3_RSQ5_0                            ((uint32_t)0x00100000)        /*!< Bit 0 */
#define  ADC_RSQ3_RSQ5_1                            ((uint32_t)0x00200000)        /*!< Bit 1 */
#define  ADC_RSQ3_RSQ5_2                            ((uint32_t)0x00400000)        /*!< Bit 2 */
#define  ADC_RSQ3_RSQ5_3                            ((uint32_t)0x00800000)        /*!< Bit 3 */
#define  ADC_RSQ3_RSQ5_4                            ((uint32_t)0x01000000)        /*!< Bit 4 */
									     
#define  ADC_RSQ3_RSQ6                              ((uint32_t)0x3E000000)        /*!< RSQ6[4:0] bits (6th conversion in regular sequence) */
#define  ADC_RSQ3_RSQ6_0                            ((uint32_t)0x02000000)        /*!< Bit 0 */
#define  ADC_RSQ3_RSQ6_1                            ((uint32_t)0x04000000)        /*!< Bit 1 */
#define  ADC_RSQ3_RSQ6_2                            ((uint32_t)0x08000000)        /*!< Bit 2 */
#define  ADC_RSQ3_RSQ6_3                            ((uint32_t)0x10000000)        /*!< Bit 3 */
#define  ADC_RSQ3_RSQ6_4                            ((uint32_t)0x20000000)        /*!< Bit 4 */

/*******************  Bit definition for ADC_ISQ register  *******************/
#define  ADC_ISQ_ISQ1                               ((uint32_t)0x0000001F)        /*!< ISQ1[4:0] bits (1st conversion in injected sequence) */  
#define  ADC_ISQ_ISQ1_0                             ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  ADC_ISQ_ISQ1_1                             ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  ADC_ISQ_ISQ1_2                             ((uint32_t)0x00000004)        /*!< Bit 2 */
#define  ADC_ISQ_ISQ1_3                             ((uint32_t)0x00000008)        /*!< Bit 3 */
#define  ADC_ISQ_ISQ1_4                             ((uint32_t)0x00000010)        /*!< Bit 4 */

#define  ADC_ISQ_ISQ2                               ((uint32_t)0x000003E0)        /*!< ISQ2[4:0] bits (2nd conversion in injected sequence) */
#define  ADC_ISQ_ISQ2_0                             ((uint32_t)0x00000020)        /*!< Bit 0 */
#define  ADC_ISQ_ISQ2_1                             ((uint32_t)0x00000040)        /*!< Bit 1 */
#define  ADC_ISQ_ISQ2_2                             ((uint32_t)0x00000080)        /*!< Bit 2 */
#define  ADC_ISQ_ISQ2_3                             ((uint32_t)0x00000100)        /*!< Bit 3 */
#define  ADC_ISQ_ISQ2_4                             ((uint32_t)0x00000200)        /*!< Bit 4 */

#define  ADC_ISQ_ISQ3                               ((uint32_t)0x00007C00)        /*!< ISQ3[4:0] bits (3rd conversion in injected sequence) */
#define  ADC_ISQ_ISQ3_0                             ((uint32_t)0x00000400)        /*!< Bit 0 */
#define  ADC_ISQ_ISQ3_1                             ((uint32_t)0x00000800)        /*!< Bit 1 */
#define  ADC_ISQ_ISQ3_2                             ((uint32_t)0x00001000)        /*!< Bit 2 */
#define  ADC_ISQ_ISQ3_3                             ((uint32_t)0x00002000)        /*!< Bit 3 */
#define  ADC_ISQ_ISQ3_4                             ((uint32_t)0x00004000)        /*!< Bit 4 */
										  
#define  ADC_ISQ_ISQ4                               ((uint32_t)0x000F8000)        /*!< ISQ4[4:0] bits (4th conversion in injected sequence) */
#define  ADC_ISQ_ISQ4_0                             ((uint32_t)0x00008000)        /*!< Bit 0 */
#define  ADC_ISQ_ISQ4_1                             ((uint32_t)0x00010000)        /*!< Bit 1 */
#define  ADC_ISQ_ISQ4_2                             ((uint32_t)0x00020000)        /*!< Bit 2 */
#define  ADC_ISQ_ISQ4_3                             ((uint32_t)0x00040000)        /*!< Bit 3 */
#define  ADC_ISQ_ISQ4_4                             ((uint32_t)0x00080000)        /*!< Bit 4 */

#define  ADC_ISQ_IL                                 ((uint32_t)0x00300000)        /*!< IL[1:0] bits (Injected Sequence length) */
#define  ADC_ISQ_IL_0                               ((uint32_t)0x00100000)        /*!< Bit 0 */
#define  ADC_ISQ_IL_1                               ((uint32_t)0x00200000)        /*!< Bit 1 */

/*******************  Bit definition for ADC_IDTR1 register  *******************/
#define  ADC_IDTR1_IDTR                             ((uint16_t)0xFFFF)            /*!< Injected data */

/*******************  Bit definition for ADC_IDTR2 register  *******************/
#define  ADC_IDTR2_IDTR                             ((uint16_t)0xFFFF)            /*!< Injected data */

/*******************  Bit definition for ADC_IDTR3 register  *******************/
#define  ADC_IDTR3_IDTR                             ((uint16_t)0xFFFF)            /*!< Injected data */

/*******************  Bit definition for ADC_IDTR4 register  *******************/
#define  ADC_IDTR4_IDTR                             ((uint16_t)0xFFFF)            /*!< Injected data */

/********************  Bit definition for ADC_RDTR register  ********************/
#define  ADC_RDTR_RDTR                              ((uint32_t)0x0000FFFF)        /*!< Regular data */

/********************  Bit definition for ADC_OVSCR register  ********************/
#define ADC_OVSCR_OVSE                              ((uint32_t)0x00000001)        /*!< OVSE bit */

#define ADC_OVSCR_OVSR_RATIO                        ((uint32_t)0x0000001C)        /*!< OVERSAMPLING_RATIO[2:0] bits */
#define ADC_OVSCR_OVSR_RATIO_0                      ((uint32_t)0x00000004)        /*!< Bit 0 */
#define ADC_OVSCR_OVSR_RATIO_1                      ((uint32_t)0x00000008)        /*!< Bit 1 */
#define ADC_OVSCR_OVSR_RATIO_2                      ((uint32_t)0x00000010)        /*!< Bit 2 */

#define ADC_OVSCR_OVSS_SHIFT                        ((uint32_t)0x000001E0)        /*!< OVERSAMPLING_SHIFT[3:0] bits */  
#define ADC_OVSCR_OVSS_SHIFT_0                      ((uint32_t)0x00000020)        /*!< Bit 0 */
#define ADC_OVSCR_OVSS_SHIFT_1                      ((uint32_t)0x00000040)        /*!< Bit 1 */
#define ADC_OVSCR_OVSS_SHIFT_2                      ((uint32_t)0x00000080)        /*!< Bit 2 */
#define ADC_OVSCR_OVSS_SHIFT_3                      ((uint32_t)0x00000100)        /*!< Bit 3 */

#define ADC_OVSCR_TOVS                              ((uint32_t)0x00000200)        /*!< TOVS bit */

#define ADC_OVSCR_DRES                              ((uint32_t)0x00003000)        /*!< RESOLUTION[1:0] bits */
#define ADC_OVSCR_DRES_0                            ((uint32_t)0x00001000)        /*!< Bit 0 */
#define ADC_OVSCR_DRES_1                            ((uint32_t)0x00002000)        /*!< Bit 1 */










/******************************************************************************/
/*                                                                            */
/*                            Backup registers                                */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for BKP_DRx register  ********************/
#define  BKP_DR1_BKD                                ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR2_BKD                                ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR3_BKD                                ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR4_BKD                                ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR5_BKD                                ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR6_BKD                                ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR7_BKD                                ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR8_BKD                                ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR9_BKD                                ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR10_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR11_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR12_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR13_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR14_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR15_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR16_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR17_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR18_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR19_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR20_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR21_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR22_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR23_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR24_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR25_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR26_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR27_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR28_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR29_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR30_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR31_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR32_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR33_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR34_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR35_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR36_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR37_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR38_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR39_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR40_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR41_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */
#define  BKP_DR42_BKD                               ((uint16_t)0xFFFF)            /*!< Backup data */

/******************  Bit definition for BKP_RCCR register  *******************/
#define  BKP_RCCR_RCCV                              ((uint16_t)0x007F)            /*!< RTC Clock Calibration Value */
#define  BKP_RCCR_RCCOE                             ((uint16_t)0x0080)            /*!< RTC Clock Calibration Output Enable */
#define  BKP_RCCR_ROE                               ((uint16_t)0x0100)            /*!< RTC Output Enable */
#define  BKP_RCCR_ROS                               ((uint16_t)0x0200)            /*!< RTC Output Selection */
#define  BKP_RCCR_CCOSEL                            ((uint16_t)0x4000)            /*!< RTC Clock Output Selection */
#define  BKP_RCCR_CALDIR                            ((uint16_t)0x8000)            /*!< RTC Clock Calibration Direction */

/********************  Bit definition for BKP_TPCR1 register  ********************/
#define  BKP_TPCR1_TPE_1                            ((uint8_t)0x01)               /*!< TAMPER_1 Pin Enable */
#define  BKP_TPCR1_TPAL_1                           ((uint8_t)0x02)               /*!< TAMPER_1 Pin Active Level */

/*******************  Bit definition for BKP_TIER register  ********************/
#define  BKP_TIER_TER_1                             ((uint16_t)0x0001)            /*!< Tamper_1 Event Reset */
#define  BKP_TIER_TIR_1                             ((uint16_t)0x0002)            /*!< Tamper_1 Interrupt Reset */
#define  BKP_TIER_TIE_1                             ((uint16_t)0x0004)            /*!< Tamper_1 Interrupt Enable */
#define  BKP_TIER_TER_2                             ((uint16_t)0x0020)            /*!< Tamper_2/Waveform Detect Event Reset */
#define  BKP_TIER_TIR_2                             ((uint16_t)0x0040)            /*!< Tamper_2/Waveform Detect Interrupt Reset */
#define  BKP_TIER_TIE_2                             ((uint16_t)0x0080)            /*!< Tamper_2/Waveform Detect Interrupt Enable */
#define  BKP_TIER_TEF_1                             ((uint16_t)0x0100)            /*!< Tamper_1 Event Flag */
#define  BKP_TIER_TIF_1                             ((uint16_t)0x0200)            /*!< Tamper_1 Interrupt Flag */
#define  BKP_TIER_TEF_2                             ((uint16_t)0x4000)            /*!< Tamper_2/Waveform Detect Event Flag */
#define  BKP_TIER_TIF_2                             ((uint16_t)0x8000)            /*!< Tamper_2/Waveform Detect Interrupt Flag */

/********************  Bit definition for BKP_TPCR2 register  ********************/
#define  BKP_TPCR2_TPE_2                            ((uint16_t)0x0100)            /*!< TAMPER_2 Pin Enable */
#define  BKP_TPCR2_TPAL_2                           ((uint16_t)0x0200)            /*!< TAMPER_2 Pin Active Level */
#define  BKP_TPCR2_TPM_2                            ((uint16_t)0x4000)            /*!< The Second Waveform Detection Enable */
#define  BKP_TPCR2_TPM_1                            ((uint16_t)0x8000)            /*!< The First Waveform Detection Enable */


/******************************************************************************/
/*                                                                            */
/*                         Controller Area Network                            */
/*                                                                            */
/******************************************************************************/
/*!< CAN control and status registers */
/*******************  Bit definition for CAN_CTLR register  ********************/
#define  CAN_CTLR_IWM                               ((uint16_t)0x0001)            /*!< Initial working mode */
#define  CAN_CTLR_SWM                               ((uint16_t)0x0002)            /*!< Sleep working mode */
#define  CAN_CTLR_TFO                               ((uint16_t)0x0004)            /*!< Transmit FIFO order */
#define  CAN_CTLR_RFOD                              ((uint16_t)0x0008)            /*!< Receive FIFO overwrite disable */
#define  CAN_CTLR_ARD                               ((uint16_t)0x0010)            /*!< Automatic retransmission disable */
#define  CAN_CTLR_AWK                               ((uint16_t)0x0020)            /*!< Automatic Wakeup */
#define  CAN_CTLR_ABOR                              ((uint16_t)0x0040)            /*!< Automatic Bus-Off recovery */
#define  CAN_CTLR_TTC                               ((uint16_t)0x0080)            /*!< Time Triggered Communication */
#define  CAN_CTLR_SR                                ((uint16_t)0x8000)            /*!< CAN software reset */
#define  CAN_CTLR_DFZ                               ((uint32_t)0x00010000)        /*!< CAN Debug freeze */

/*******************  Bit definition for CAN_STR register  ********************/
#define  CAN_STR_IWS                                ((uint16_t)0x0001)            /*!< Initial working state */
#define  CAN_STR_SWS                                ((uint16_t)0x0002)            /*!< Sleep working state */
#define  CAN_STR_EIF                                ((uint16_t)0x0004)            /*!< Error Interrupt Flag*/
#define  CAN_STR_WIF                                ((uint16_t)0x0008)            /*!< Status change interrupt flag of wakeup from sleep working mode */
#define  CAN_STR_SEIF                               ((uint16_t)0x0010)            /*!< Status change interrupt flag of sleep working mode entering */
#define  CAN_STR_TS                                 ((uint16_t)0x0100)            /*!< Transmitting state */
#define  CAN_STR_RS                                 ((uint16_t)0x0200)            /*!< Receiving state */
#define  CAN_STR_LASTRX                             ((uint16_t)0x0400)            /*!< Last sample value of Rx pin */
#define  CAN_STR_RX                                 ((uint16_t)0x0800)            /*!< CAN Rx Signal */

/*******************  Bit definition for CAN_TSTR register  ********************/
#define  CAN_TSTR_MTF0                              ((uint32_t)0x00000001)        /*!< Mailbox 0 transmit finished */
#define  CAN_TSTR_MTFNE0                            ((uint32_t)0x00000002)        /*!< Mailbox 0 transmit finished and no error */
#define  CAN_TSTR_MAL0                              ((uint32_t)0x00000004)        /*!< Mailbox 0 arbitration lost */
#define  CAN_TSTR_MTE0                              ((uint32_t)0x00000008)        /*!< Mailbox 0 transmit error */
#define  CAN_TSTR_MST0                              ((uint32_t)0x00000080)        /*!< Mailbox 0 stop transmitting */

#define  CAN_TSTR_MTF1                              ((uint32_t)0x00000100)        /*!< Mailbox 1 transmit finished */
#define  CAN_TSTR_MTFNE1                            ((uint32_t)0x00000200)        /*!< Mailbox 1 transmit finished and no error */
#define  CAN_TSTR_MAL1                              ((uint32_t)0x00000400)        /*!< Mailbox 1 arbitration lost */
#define  CAN_TSTR_MTE1                              ((uint32_t)0x00000800)        /*!< Mailbox 1 transmit error */
#define  CAN_TSTR_MST1                              ((uint32_t)0x00008000)        /*!< Mailbox 1 stop transmitting */

#define  CAN_TSTR_MTF2                              ((uint32_t)0x00010000)        /*!< Mailbox 2 transmit finished */
#define  CAN_TSTR_MTFNE2                            ((uint32_t)0x00020000)        /*!< Mailbox 2 transmit finished and no error */
#define  CAN_TSTR_MAL2                              ((uint32_t)0x00040000)        /*!< Mailbox 2 arbitration lost */
#define  CAN_TSTR_MTE2                              ((uint32_t)0x00080000)        /*!< Mailbox 2 transmit error */
#define  CAN_TSTR_MST2                              ((uint32_t)0x00800000)        /*!< Mailbox 2 stop transmitting */
								 
#define  CAN_TSTR_NUM                               ((uint32_t)0x03000000)        /*!< Mailbox number */

#define  CAN_TSTR_TME                               ((uint32_t)0x1C000000)        /*!< TME[2:0] bits */
#define  CAN_TSTR_TME0                              ((uint32_t)0x04000000)        /*!< Transmit mailbox 0 empty */
#define  CAN_TSTR_TME1                              ((uint32_t)0x08000000)        /*!< Transmit mailbox 1 empty */
#define  CAN_TSTR_TME2                              ((uint32_t)0x10000000)        /*!< Transmit mailbox 2 empty */
									   
#define  CAN_TSTR_TMLS                              ((uint32_t)0xE0000000)        /*!< TMLS[2:0] bits */
#define  CAN_TSTR_TMLS0                             ((uint32_t)0x20000000)        /*!< Last sending Priority Flag for Mailbox 0 */
#define  CAN_TSTR_TMLS1                             ((uint32_t)0x40000000)        /*!< Last sending Priority Flag for Mailbox 1 */
#define  CAN_TSTR_TMLS2                             ((uint32_t)0x80000000)        /*!< Last sending Priority Flag for Mailbox 2 */

/*******************  Bit definition for CAN_RFR0 register  *******************/
#define  CAN_RFR0_RFL0                              ((uint8_t)0x03)               /*!< Receive FIFO 0 length */
#define  CAN_RFR0_RFF0                              ((uint8_t)0x08)               /*!< Receive FIFO 0 full */
#define  CAN_RFR0_RFO0                              ((uint8_t)0x10)               /*!< Receive FIFO 0 overfull */
#define  CAN_RFR0_RFD0                              ((uint8_t)0x20)               /*!< Receive FIFO 0 dequeue */
								    
/*******************  Bit definition for CAN_RFR1 register  *******************/
#define  CAN_RFR1_RFL1                              ((uint8_t)0x03)               /*!< Receive FIFO 1 length */
#define  CAN_RFR1_RFF1                              ((uint8_t)0x08)               /*!< Receive FIFO 1 full */
#define  CAN_RFR1_RFO1                              ((uint8_t)0x10)               /*!< Receive FIFO 1 overfull */
#define  CAN_RFR1_RFD1                              ((uint8_t)0x20)               /*!< Receive FIFO 1 dequeue */

/********************  Bit definition for CAN_IER register  *******************/
#define  CAN_IER_TMEIE                              ((uint32_t)0x00000001)        /*!< Transmit mailbox empty interrupt enable */
#define  CAN_IER_RFNEIE0                            ((uint32_t)0x00000002)        /*!< Receive FIFO0 not empty interrupt enable */
#define  CAN_IER_RFFIE0                             ((uint32_t)0x00000004)        /*!< Receive FIFO0 full interrupt enable */
#define  CAN_IER_RFOIE0                             ((uint32_t)0x00000008)        /*!< Receive FIFO0 overfull interrupt enable */
#define  CAN_IER_RFNEIE1                            ((uint32_t)0x00000010)        /*!< Receive FIFO1 not empty interrupt enable */
#define  CAN_IER_RFFIE1                             ((uint32_t)0x00000020)        /*!< Receive FIFO1 full interrupt enable */
#define  CAN_IER_RFOIE1                             ((uint32_t)0x00000040)        /*!< Receive FIFO1 overfull interrupt enable */
#define  CAN_IER_WEIE                               ((uint32_t)0x00000100)        /*!< Warning error interrupt enable */
#define  CAN_IER_PEIE                               ((uint32_t)0x00000200)        /*!< Passive error interrupt enable */
#define  CAN_IER_BOIE                               ((uint32_t)0x00000400)        /*!< Bus-off interrupt enable */
#define  CAN_IER_ENIE                               ((uint32_t)0x00000800)        /*!< Error number interrupt enable */
#define  CAN_IER_EIE                                ((uint32_t)0x00008000)        /*!< Error Interrupt enable */
#define  CAN_IER_WIE                                ((uint32_t)0x00010000)        /*!< Wakeup Interrupt enable */
#define  CAN_IER_SWIE                               ((uint32_t)0x00020000)        /*!< Sleep working interrupt enable */
									  
/********************  Bit definition for CAN_ER register  *******************/
#define  CAN_ER_WE                                  ((uint32_t)0x00000001)        /*!< Warning error */
#define  CAN_ER_PE                                  ((uint32_t)0x00000002)        /*!< Passive error */
#define  CAN_ER_BOE                                 ((uint32_t)0x00000004)        /*!< Bus-off error */

#define  CAN_ER_ET                                  ((uint32_t)0x00000070)        /*!< ET[2:0] bits (Error type) */
#define  CAN_ER_ET_0                                ((uint32_t)0x00000010)        /*!< Bit 0 */
#define  CAN_ER_ET_1                                ((uint32_t)0x00000020)        /*!< Bit 1 */
#define  CAN_ER_ET_2                                ((uint32_t)0x00000040)        /*!< Bit 2 */
											  
#define  CAN_ER_TEC                                 ((uint32_t)0x00FF0000)        /*!< Transmit Error Count */
#define  CAN_ER_REC                                 ((uint32_t)0xFF000000)        /*!< Receive Error Count */

/*******************  Bit definition for CAN_BTR register  ********************/
#define  CAN_BTR_BRP                                ((uint32_t)0x000003FF)        /*!< Baud rate prescaler */
#define  CAN_BTR_BS1                                ((uint32_t)0x000F0000)        /*!< Bit Segment 1 */
#define  CAN_BTR_BS2                                ((uint32_t)0x00700000)        /*!< Bit Segment 2 */
#define  CAN_BTR_SJW                                ((uint32_t)0x03000000)        /*!< Resynchronization jump width */
#define  CAN_BTR_LCM                                ((uint32_t)0x40000000)        /*!< Loopback communication mode */
#define  CAN_BTR_SCM                                ((uint32_t)0x80000000)        /*!< Silent communication mode */

/*!< Mailbox registers */
/******************  Bit definition for CAN_TMIR0 register  ********************/
#define  CAN_TMIR0_TE                               ((uint32_t)0x00000001)        /*!< Transmit enable */
#define  CAN_TMIR0_FT                               ((uint32_t)0x00000002)        /*!< Frame type */
#define  CAN_TMIR0_FF                               ((uint32_t)0x00000004)        /*!< Frame format */
#define  CAN_TMIR0_EFID                             ((uint32_t)0x001FFFF8)        /*!< The frame identifier */
#define  CAN_TMIR0_SFID                             ((uint32_t)0xFFE00000)        /*!< The frame identifier */

/******************  Bit definition for CAN_TMPR0 register  *******************/
#define  CAN_TMPR0_DLC                              ((uint32_t)0x0000000F)        /*!< Data length code */
#define  CAN_TMPR0_TSE                              ((uint32_t)0x00000100)        /*!< Time stamp enable */
#define  CAN_TMPR0_TS                               ((uint32_t)0xFFFF0000)        /*!< Time stamp */

/******************  Bit definition for CAN_TMD0R0 register  *******************/
#define  CAN_TMD0R0_DB0                             ((uint32_t)0x000000FF)        /*!< Data byte 0 */
#define  CAN_TMD0R0_DB1                             ((uint32_t)0x0000FF00)        /*!< Data byte 1 */
#define  CAN_TMD0R0_DB2                             ((uint32_t)0x00FF0000)        /*!< Data byte 2 */
#define  CAN_TMD0R0_DB3                             ((uint32_t)0xFF000000)        /*!< Data byte 3 */

/******************  Bit for CAN_TMD1R0 register  *******************/
#define  CAN_TMD1R0_DB4                             ((uint32_t)0x000000FF)        /*!< Data byte 4 */
#define  CAN_TMD1R0_DB5                             ((uint32_t)0x0000FF00)        /*!< Data byte 5 */
#define  CAN_TMD1R0_DB6                             ((uint32_t)0x00FF0000)        /*!< Data byte 6 */
#define  CAN_TMD1R0_DB7                             ((uint32_t)0xFF000000)        /*!< Data byte 7 */

/******************  Bit definition for CAN_TMIR1 register  ********************/
#define  CAN_TMIR1_TE                               ((uint32_t)0x00000001)        /*!< Transmit enable */
#define  CAN_TMIR1_FT                               ((uint32_t)0x00000002)        /*!< Frame type */
#define  CAN_TMIR1_FF                               ((uint32_t)0x00000004)        /*!< Frame format */
#define  CAN_TMIR1_EFID                             ((uint32_t)0x001FFFF8)        /*!< The frame identifier */
#define  CAN_TMIR1_SFID                             ((uint32_t)0xFFE00000)        /*!< The frame identifier */

/******************  Bit definition for CAN_TMPR1 register  *******************/
#define  CAN_TMPR1_DLC                              ((uint32_t)0x0000000F)        /*!< Data length code */
#define  CAN_TMPR1_TSE                              ((uint32_t)0x00000100)        /*!< Time stamp enable */
#define  CAN_TMPR1_TS                               ((uint32_t)0xFFFF0000)        /*!< Time stamp */

/******************  Bit definition for CAN_TMD0R1 register  *******************/
#define  CAN_TMD0R1_DB0                             ((uint32_t)0x000000FF)        /*!< Data byte 0 */
#define  CAN_TMD0R1_DB1                             ((uint32_t)0x0000FF00)        /*!< Data byte 1 */
#define  CAN_TMD0R1_DB2                             ((uint32_t)0x00FF0000)        /*!< Data byte 2 */
#define  CAN_TMD0R1_DB3                             ((uint32_t)0xFF000000)        /*!< Data byte 3 */
									       
/******************  Bit definition for CAN_TMD1R1 register  *******************/
#define  CAN_TMD1R1_DB4                             ((uint32_t)0x000000FF)        /*!< Data byte 4 */
#define  CAN_TMD1R1_DB5                             ((uint32_t)0x0000FF00)        /*!< Data byte 5 */
#define  CAN_TMD1R1_DB6                             ((uint32_t)0x00FF0000)        /*!< Data byte 6 */
#define  CAN_TMD1R1_DB7                             ((uint32_t)0xFF000000)        /*!< Data byte 7 */

/******************  Bit definition for CAN_TMIR2 register  ********************/
#define  CAN_TMIR2_TE                               ((uint32_t)0x00000001)        /*!< Transmit enable */
#define  CAN_TMIR2_FT                               ((uint32_t)0x00000002)        /*!< Frame type */
#define  CAN_TMIR2_FF                               ((uint32_t)0x00000004)        /*!< Frame format */
#define  CAN_TMIR2_EFID                             ((uint32_t)0x001FFFF8)        /*!< The frame identifier */
#define  CAN_TMIR2_SFID                             ((uint32_t)0xFFE00000)        /*!< The frame identifier */

/******************  Bit definition for CAN_TMPR2 register  *******************/
#define  CAN_TMPR2_DLC                              ((uint32_t)0x0000000F)        /*!< Data length code */
#define  CAN_TMPR2_TSE                              ((uint32_t)0x00000100)        /*!< Time stamp enable */
#define  CAN_TMPR2_TS                               ((uint32_t)0xFFFF0000)        /*!< Time stamp */

/******************  Bit definition for CAN_TMD0R2 register  *******************/
#define  CAN_TMD0R2_DB0                             ((uint32_t)0x000000FF)        /*!< Data byte 0 */
#define  CAN_TMD0R2_DB1                             ((uint32_t)0x0000FF00)        /*!< Data byte 1 */
#define  CAN_TMD0R2_DB2                             ((uint32_t)0x00FF0000)        /*!< Data byte 2 */
#define  CAN_TMD0R2_DB3                             ((uint32_t)0xFF000000)        /*!< Data byte 3 */

/******************  Bit definition for CAN_TMD1R2 register  *******************/
#define  CAN_TMD1R2_DB4                             ((uint32_t)0x000000FF)        /*!< Data byte 4 */
#define  CAN_TMD1R2_DB5                             ((uint32_t)0x0000FF00)        /*!< Data byte 5 */
#define  CAN_TMD1R2_DB6                             ((uint32_t)0x00FF0000)        /*!< Data byte 6 */
#define  CAN_TMD1R2_DB7                             ((uint32_t)0xFF000000)        /*!< Data byte 7 */

/******************  Bit definition for CAN_RFMIR0 register  ********************/
#define  CAN_RFMIR0_FT                              ((uint32_t)0x00000002)        /*!< Frame type */
#define  CAN_RFMIR0_FF                              ((uint32_t)0x00000004)        /*!< Frame format */
#define  CAN_RFMIR0_EFID                            ((uint32_t)0x001FFFF8)        /*!< The frame identifier */
#define  CAN_RFMIR0_SFID                            ((uint32_t)0xFFE00000)        /*!< The frame identifier */

/******************  Bit definition for CAN_RFMPR0 register  *******************/
#define  CAN_RFMPR0_DLC                             ((uint32_t)0x0000000F)        /*!< Data length code */
#define  CAN_RFMPR0_TSE                             ((uint32_t)0x00000100)        /*!< Time stamp enable */
#define  CAN_RFMPR0_TS                              ((uint32_t)0xFFFF0000)        /*!< Time stamp */

/******************  Bit definition for CAN_RFMD0R0 register  *******************/
#define  CAN_RFMD0R0_DB0                            ((uint32_t)0x000000FF)        /*!< Data byte 0 */
#define  CAN_RFMD0R0_DB1                            ((uint32_t)0x0000FF00)        /*!< Data byte 1 */
#define  CAN_RFMD0R0_DB2                            ((uint32_t)0x00FF0000)        /*!< Data byte 2 */
#define  CAN_RFMD0R0_DB3                            ((uint32_t)0xFF000000)        /*!< Data byte 3 */

/******************  Bit definition for CAN_RFMD1R0 register  *******************/
#define  CAN_RFMD1R0_DB4                            ((uint32_t)0x000000FF)        /*!< Data byte 4 */
#define  CAN_RFMD1R0_DB5                            ((uint32_t)0x0000FF00)        /*!< Data byte 5 */
#define  CAN_RFMD1R0_DB6                            ((uint32_t)0x00FF0000)        /*!< Data byte 6 */
#define  CAN_RFMD1R0_DB7                            ((uint32_t)0xFF000000)        /*!< Data byte 7 */

/******************  Bit definition for CAN_RFMIR1 register  ********************/
#define  CAN_RFMIR1_FT                              ((uint32_t)0x00000002)        /*!< Frame type */
#define  CAN_RFMIR1_FF                              ((uint32_t)0x00000004)        /*!< Frame format */
#define  CAN_RFMIR1_EFID                            ((uint32_t)0x001FFFF8)        /*!< The frame identifier */
#define  CAN_RFMIR1_SFID                            ((uint32_t)0xFFE00000)        /*!< The frame identifier */

/******************  Bit definition for CAN_RFMPR1 register  *******************/
#define  CAN_RFMPR1_DLC                             ((uint32_t)0x0000000F)        /*!< Data length code */
#define  CAN_RFMPR1_TSE                             ((uint32_t)0x00000100)        /*!< Time stamp enable */
#define  CAN_RFMPR1_TS                              ((uint32_t)0xFFFF0000)        /*!< Time stamp */

/******************  Bit definition for CAN_RFMD0R1 register  *******************/
#define  CAN_RFMD0R1_DB0                            ((uint32_t)0x000000FF)        /*!< Data byte 0 */
#define  CAN_RFMD0R1_DB1                            ((uint32_t)0x0000FF00)        /*!< Data byte 1 */
#define  CAN_RFMD0R1_DB2                            ((uint32_t)0x00FF0000)        /*!< Data byte 2 */
#define  CAN_RFMD0R1_DB3                            ((uint32_t)0xFF000000)        /*!< Data byte 3 */

/******************  Bit definition for CAN_RFMD1R1 register  *******************/
#define  CAN_RFMD1R1_DB4                            ((uint32_t)0x000000FF)        /*!< Data byte 4 */
#define  CAN_RFMD1R1_DB5                            ((uint32_t)0x0000FF00)        /*!< Data byte 5 */
#define  CAN_RFMD1R1_DB6                            ((uint32_t)0x00FF0000)        /*!< Data byte 6 */
#define  CAN_RFMD1R1_DB7                            ((uint32_t)0xFF000000)        /*!< Data byte 7 */

/*!< CAN filter registers */
/*******************  Bit definition for CAN_FCTLR register  ********************/
#define  CAN_FCTLR_FLD                              ((uint32_t)0x00000001)         /*!< Filter lock disable */
#define  CAN_FCTLR_HBC2F                            ((uint32_t)0x003F0000)        /*!< Header bank of CAN2 filter */

/*******************  Bit definition for CAN_FMR register  *******************/
#define  CAN_FMR_FM                                 ((uint32_t)0x0FFFFFFF)        /*!< Filter Mode */
#define  CAN_FMR_FM0                                ((uint32_t)0x00000001)        /*!< Filter Init Mode bit 0 */
#define  CAN_FMR_FM1                                ((uint32_t)0x00000002)        /*!< Filter Init Mode bit 1 */
#define  CAN_FMR_FM2                                ((uint32_t)0x00000004)        /*!< Filter Init Mode bit 2 */
#define  CAN_FMR_FM3                                ((uint32_t)0x00000008)        /*!< Filter Init Mode bit 3 */
#define  CAN_FMR_FM4                                ((uint32_t)0x00000010)        /*!< Filter Init Mode bit 4 */
#define  CAN_FMR_FM5                                ((uint32_t)0x00000020)        /*!< Filter Init Mode bit 5 */
#define  CAN_FMR_FM6                                ((uint32_t)0x00000040)        /*!< Filter Init Mode bit 6 */
#define  CAN_FMR_FM7                                ((uint32_t)0x00000080)        /*!< Filter Init Mode bit 7 */
#define  CAN_FMR_FM8                                ((uint32_t)0x00000100)        /*!< Filter Init Mode bit 8 */
#define  CAN_FMR_FM9                                ((uint32_t)0x00000200)        /*!< Filter Init Mode bit 9 */
#define  CAN_FMR_FM10                               ((uint32_t)0x00000400)        /*!< Filter Init Mode bit 10 */
#define  CAN_FMR_FM11                               ((uint32_t)0x00000800)        /*!< Filter Init Mode bit 11 */
#define  CAN_FMR_FM12                               ((uint32_t)0x00001000)        /*!< Filter Init Mode bit 12 */
#define  CAN_FMR_FM13                               ((uint32_t)0x00002000)        /*!< Filter Init Mode bit 13 */
#define  CAN_FMR_FM14                               ((uint32_t)0x00004000)        /*!< Filter Init Mode bit 14 */
#define  CAN_FMR_FM15                               ((uint32_t)0x00008000)        /*!< Filter Init Mode bit 15 */
#define  CAN_FMR_FM16                               ((uint32_t)0x00010000)        /*!< Filter Init Mode bit 16 */
#define  CAN_FMR_FM17                               ((uint32_t)0x00020000)        /*!< Filter Init Mode bit 17 */
#define  CAN_FMR_FM18                               ((uint32_t)0x00040000)        /*!< Filter Init Mode bit 18 */
#define  CAN_FMR_FM19                               ((uint32_t)0x00080000)        /*!< Filter Init Mode bit 19 */
#define  CAN_FMR_FM20                               ((uint32_t)0x00100000)        /*!< Filter Init Mode bit 20 */
#define  CAN_FMR_FM21                               ((uint32_t)0x00200000)        /*!< Filter Init Mode bit 21 */
#define  CAN_FMR_FM22                               ((uint32_t)0x00400000)        /*!< Filter Init Mode bit 22 */
#define  CAN_FMR_FM23                               ((uint32_t)0x00800000)        /*!< Filter Init Mode bit 23 */
#define  CAN_FMR_FM24                               ((uint32_t)0x01000000)        /*!< Filter Init Mode bit 24 */
#define  CAN_FMR_FM25                               ((uint32_t)0x02000000)        /*!< Filter Init Mode bit 25 */
#define  CAN_FMR_FM26                               ((uint32_t)0x04000000)        /*!< Filter Init Mode bit 26 */
#define  CAN_FMR_FM27                               ((uint32_t)0x08000000)        /*!< Filter Init Mode bit 27 */

/*******************  Bit definition for CAN_FSR register  *******************/
#define  CAN_FSR_FS                                 ((uint32_t)0x0FFFFFFF)        /*!< Filter Scale */
#define  CAN_FSR_FS0                                ((uint32_t)0x00000001)        /*!< Filter Scale bit 0 */
#define  CAN_FSR_FS1                                ((uint32_t)0x00000002)        /*!< Filter Scale bit 1 */
#define  CAN_FSR_FS2                                ((uint32_t)0x00000004)        /*!< Filter Scale bit 2 */
#define  CAN_FSR_FS3                                ((uint32_t)0x00000008)        /*!< Filter Scale bit 3 */
#define  CAN_FSR_FS4                                ((uint32_t)0x00000010)        /*!< Filter Scale bit 4 */
#define  CAN_FSR_FS5                                ((uint32_t)0x00000020)        /*!< Filter Scale bit 5 */
#define  CAN_FSR_FS6                                ((uint32_t)0x00000040)        /*!< Filter Scale bit 6 */
#define  CAN_FSR_FS7                                ((uint32_t)0x00000080)        /*!< Filter Scale bit 7 */
#define  CAN_FSR_FS8                                ((uint32_t)0x00000100)        /*!< Filter Scale bit 8 */
#define  CAN_FSR_FS9                                ((uint32_t)0x00000200)        /*!< Filter Scale bit 9 */
#define  CAN_FSR_FS10                               ((uint32_t)0x00000400)        /*!< Filter Scale bit 10 */
#define  CAN_FSR_FS11                               ((uint32_t)0x00000800)        /*!< Filter Scale bit 11 */
#define  CAN_FSR_FS12                               ((uint32_t)0x00001000)        /*!< Filter Scale bit 12 */
#define  CAN_FSR_FS13                               ((uint32_t)0x00002000)        /*!< Filter Scale bit 13 */
#define  CAN_FSR_FS14                               ((uint32_t)0x00004000)        /*!< Filter Scale bit 14 */
#define  CAN_FSR_FS15                               ((uint32_t)0x00008000)        /*!< Filter Scale bit 15 */
#define  CAN_FSR_FS16                               ((uint32_t)0x00010000)        /*!< Filter Scale bit 16 */
#define  CAN_FSR_FS17                               ((uint32_t)0x00020000)        /*!< Filter Scale bit 17 */
#define  CAN_FSR_FS18                               ((uint32_t)0x00040000)        /*!< Filter Scale bit 18 */
#define  CAN_FSR_FS19                               ((uint32_t)0x00080000)        /*!< Filter Scale bit 19 */
#define  CAN_FSR_FS20                               ((uint32_t)0x00100000)        /*!< Filter Scale bit 20 */
#define  CAN_FSR_FS21                               ((uint32_t)0x00200000)        /*!< Filter Scale bit 21 */
#define  CAN_FSR_FS22                               ((uint32_t)0x00400000)        /*!< Filter Scale bit 22 */
#define  CAN_FSR_FS23                               ((uint32_t)0x00800000)        /*!< Filter Scale bit 23*/
#define  CAN_FSR_FS24                               ((uint32_t)0x01000000)        /*!< Filter Scale bit 24 */
#define  CAN_FSR_FS25                               ((uint32_t)0x02000000)        /*!< Filter Scale bit 25 */
#define  CAN_FSR_FS26                               ((uint32_t)0x04000000)        /*!< Filter Scale bit 26 */
#define  CAN_FSR_FS27                               ((uint32_t)0x08000000)        /*!< Filter Scale bit 27 */
									      
/******************  Bit definition for CAN_FAFR register  *******************/
#define  CAN_FAFR_FAF                               ((uint32_t)0x0FFFFFFF)        /*!< Filter associated with FIFO */
#define  CAN_FAFR_FAF0                              ((uint32_t)0x00000001)        /*!< Filter 0 associated with FIFO */
#define  CAN_FAFR_FAF1                              ((uint32_t)0x00000002)        /*!< Filter 1 associated with FIFO */
#define  CAN_FAFR_FAF2                              ((uint32_t)0x00000004)        /*!< Filter 2 associated with FIFO */
#define  CAN_FAFR_FAF3                              ((uint32_t)0x00000008)        /*!< Filter 3 associated with FIFO */
#define  CAN_FAFR_FAF4                              ((uint32_t)0x00000010)        /*!< Filter 4 associated with FIFO */
#define  CAN_FAFR_FAF5                              ((uint32_t)0x00000020)        /*!< Filter 5 associated with FIFO */
#define  CAN_FAFR_FAF6                              ((uint32_t)0x00000040)        /*!< Filter 6 associated with FIFO */
#define  CAN_FAFR_FAF7                              ((uint32_t)0x00000080)        /*!< Filter 7 associated with FIFO */
#define  CAN_FAFR_FAF8                              ((uint32_t)0x00000100)        /*!< Filter 8 associated with FIFO */
#define  CAN_FAFR_FAF9                              ((uint32_t)0x00000200)        /*!< Filter 9 associated with FIFO */
#define  CAN_FAFR_FAF10                             ((uint32_t)0x00000400)        /*!< Filter 10 associated with FIFO */
#define  CAN_FAFR_FAF11                             ((uint32_t)0x00000800)        /*!< Filter 11 associated with FIFO */
#define  CAN_FAFR_FAF12                             ((uint32_t)0x00001000)        /*!< Filter 12 associated with FIFO */
#define  CAN_FAFR_FAF13                             ((uint32_t)0x00002000)        /*!< Filter 13 associated with FIFO */
#define  CAN_FAFR_FAF14                             ((uint32_t)0x00004000)        /*!< Filter 14 associated with FIFO */
#define  CAN_FAFR_FAF15                             ((uint32_t)0x00008000)        /*!< Filter 15 associated with FIFO */
#define  CAN_FAFR_FAF16                             ((uint32_t)0x00010000)        /*!< Filter 16 associated with FIFO */
#define  CAN_FAFR_FAF17                             ((uint32_t)0x00020000)        /*!< Filter 17 associated with FIFO */
#define  CAN_FAFR_FAF18                             ((uint32_t)0x00040000)        /*!< Filter 18 associated with FIFO */
#define  CAN_FAFR_FAF19                             ((uint32_t)0x00080000)        /*!< Filter 19 associated with FIFO */
#define  CAN_FAFR_FAF20                             ((uint32_t)0x00100000)        /*!< Filter 20 associated with FIFO */
#define  CAN_FAFR_FAF21                             ((uint32_t)0x00200000)        /*!< Filter 21 associated with FIFO */
#define  CAN_FAFR_FAF22                             ((uint32_t)0x00400000)        /*!< Filter 22 associated with FIFO */
#define  CAN_FAFR_FAF23                             ((uint32_t)0x00800000)        /*!< Filter 23 associated with FIFO */
#define  CAN_FAFR_FAF24                             ((uint32_t)0x01000000)        /*!< Filter 24 associated with FIFO */
#define  CAN_FAFR_FAF25                             ((uint32_t)0x02000000)        /*!< Filter 25 associated with FIFO */
#define  CAN_FAFR_FAF26                             ((uint32_t)0x04000000)        /*!< Filter 26 associated with FIFO */
#define  CAN_FAFR_FAF27                             ((uint32_t)0x08000000)        /*!< Filter 27 associated with FIFO */

/*******************  Bit definition for CAN_FWR register  *******************/
#define  CAN_FWR_FW                                 ((uint32_t)0x0FFFFFFF)        /*!< Filter working */
#define  CAN_FWR_FW0                                ((uint32_t)0x00000001)        /*!< Filter 0 working */
#define  CAN_FWR_FW1                                ((uint32_t)0x00000002)        /*!< Filter 1 working */
#define  CAN_FWR_FW2                                ((uint32_t)0x00000004)        /*!< Filter 2 working */
#define  CAN_FWR_FW3                                ((uint32_t)0x00000008)        /*!< Filter 3 working */
#define  CAN_FWR_FW4                                ((uint32_t)0x00000010)        /*!< Filter 4 working */
#define  CAN_FWR_FW5                                ((uint32_t)0x00000020)        /*!< Filter 5 working */
#define  CAN_FWR_FW6                                ((uint32_t)0x00000040)        /*!< Filter 6 working */
#define  CAN_FWR_FW7                                ((uint32_t)0x00000080)        /*!< Filter 7 working */
#define  CAN_FWR_FW8                                ((uint32_t)0x00000100)        /*!< Filter 8 working */
#define  CAN_FWR_FW9                                ((uint32_t)0x00000200)        /*!< Filter 9 working */
#define  CAN_FWR_FW10                               ((uint32_t)0x00000400)        /*!< Filter 10 working */
#define  CAN_FWR_FW11                               ((uint32_t)0x00000800)        /*!< Filter 11 working */
#define  CAN_FWR_FW12                               ((uint32_t)0x00001000)        /*!< Filter 12 working */
#define  CAN_FWR_FW13                               ((uint32_t)0x00002000)        /*!< Filter 13 working */
#define  CAN_FWR_FW14                               ((uint32_t)0x00004000)        /*!< Filter 14 working */
#define  CAN_FWR_FW15                               ((uint32_t)0x00008000)        /*!< Filter 15 working */
#define  CAN_FWR_FW16                               ((uint32_t)0x00010000)        /*!< Filter 16 working */
#define  CAN_FWR_FW17                               ((uint32_t)0x00020000)        /*!< Filter 17 working */
#define  CAN_FWR_FW18                               ((uint32_t)0x00040000)        /*!< Filter 18 working */
#define  CAN_FWR_FW19                               ((uint32_t)0x00080000)        /*!< Filter 19 working */
#define  CAN_FWR_FW20                               ((uint32_t)0x00100000)        /*!< Filter 20 working */
#define  CAN_FWR_FW21                               ((uint32_t)0x00200000)        /*!< Filter 21 working */
#define  CAN_FWR_FW22                               ((uint32_t)0x00400000)        /*!< Filter 22 working */
#define  CAN_FWR_FW23                               ((uint32_t)0x00800000)        /*!< Filter 23 working */
#define  CAN_FWR_FW24                               ((uint32_t)0x01000000)        /*!< Filter 24 working */
#define  CAN_FWR_FW25                               ((uint32_t)0x02000000)        /*!< Filter 25 working */
#define  CAN_FWR_FW26                               ((uint32_t)0x04000000)        /*!< Filter 26 working */
#define  CAN_FWR_FW27                               ((uint32_t)0x08000000)        /*!< Filter 27 working */

/*******************  Bit definition for CAN_F0D0R register  *******************/
#define  CAN_F0D0R_FD0                              ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F0D0R_FD1                              ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F0D0R_FD2                              ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F0D0R_FD3                              ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F0D0R_FD4                              ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F0D0R_FD5                              ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F0D0R_FD6                              ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F0D0R_FD7                              ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F0D0R_FD8                              ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F0D0R_FD9                              ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F0D0R_FD10                             ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F0D0R_FD11                             ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F0D0R_FD12                             ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F0D0R_FD13                             ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F0D0R_FD14                             ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F0D0R_FD15                             ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F0D0R_FD16                             ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F0D0R_FD17                             ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F0D0R_FD18                             ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F0D0R_FD19                             ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F0D0R_FD20                             ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F0D0R_FD21                             ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F0D0R_FD22                             ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F0D0R_FD23                             ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F0D0R_FD24                             ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F0D0R_FD25                             ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F0D0R_FD26                             ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F0D0R_FD27                             ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F0D0R_FD28                             ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F0D0R_FD29                             ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F0D0R_FD30                             ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F0D0R_FD31                             ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F1D0R register  *******************/
#define  CAN_F1D0R_FD0                              ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F1D0R_FD1                              ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F1D0R_FD2                              ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F1D0R_FD3                              ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F1D0R_FD4                              ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F1D0R_FD5                              ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F1D0R_FD6                              ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F1D0R_FD7                              ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F1D0R_FD8                              ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F1D0R_FD9                              ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F1D0R_FD10                             ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F1D0R_FD11                             ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F1D0R_FD12                             ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F1D0R_FD13                             ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F1D0R_FD14                             ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F1D0R_FD15                             ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F1D0R_FD16                             ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F1D0R_FD17                             ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F1D0R_FD18                             ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F1D0R_FD19                             ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F1D0R_FD20                             ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F1D0R_FD21                             ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F1D0R_FD22                             ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F1D0R_FD23                             ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F1D0R_FD24                             ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F1D0R_FD25                             ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F1D0R_FD26                             ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F1D0R_FD27                             ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F1D0R_FD28                             ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F1D0R_FD29                             ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F1D0R_FD30                             ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F1D0R_FD31                             ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F2D0R register  *******************/
#define  CAN_F2D0R_FD0                              ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F2D0R_FD1                              ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F2D0R_FD2                              ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F2D0R_FD3                              ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F2D0R_FD4                              ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F2D0R_FD5                              ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F2D0R_FD6                              ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F2D0R_FD7                              ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F2D0R_FD8                              ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F2D0R_FD9                              ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F2D0R_FD10                             ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F2D0R_FD11                             ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F2D0R_FD12                             ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F2D0R_FD13                             ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F2D0R_FD14                             ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F2D0R_FD15                             ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F2D0R_FD16                             ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F2D0R_FD17                             ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F2D0R_FD18                             ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F2D0R_FD19                             ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F2D0R_FD20                             ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F2D0R_FD21                             ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F2D0R_FD22                             ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F2D0R_FD23                             ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F2D0R_FD24                             ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F2D0R_FD25                             ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F2D0R_FD26                             ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F2D0R_FD27                             ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F2D0R_FD28                             ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F2D0R_FD29                             ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F2D0R_FD30                             ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F2D0R_FD31                             ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F3D0R register  *******************/
#define  CAN_F3D0R_FD0                              ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F3D0R_FD1                              ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F3D0R_FD2                              ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F3D0R_FD3                              ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F3D0R_FD4                              ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F3D0R_FD5                              ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F3D0R_FD6                              ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F3D0R_FD7                              ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F3D0R_FD8                              ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F3D0R_FD9                              ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F3D0R_FD10                             ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F3D0R_FD11                             ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F3D0R_FD12                             ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F3D0R_FD13                             ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F3D0R_FD14                             ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F3D0R_FD15                             ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F3D0R_FD16                             ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F3D0R_FD17                             ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F3D0R_FD18                             ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F3D0R_FD19                             ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F3D0R_FD20                             ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F3D0R_FD21                             ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F3D0R_FD22                             ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F3D0R_FD23                             ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F3D0R_FD24                             ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F3D0R_FD25                             ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F3D0R_FD26                             ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F3D0R_FD27                             ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F3D0R_FD28                             ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F3D0R_FD29                             ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F3D0R_FD30                             ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F3D0R_FD31                             ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F4D0R register  *******************/
#define  CAN_F4D0R_FD0                              ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F4D0R_FD1                              ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F4D0R_FD2                              ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F4D0R_FD3                              ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F4D0R_FD4                              ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F4D0R_FD5                              ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F4D0R_FD6                              ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F4D0R_FD7                              ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F4D0R_FD8                              ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F4D0R_FD9                              ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F4D0R_FD10                             ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F4D0R_FD11                             ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F4D0R_FD12                             ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F4D0R_FD13                             ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F4D0R_FD14                             ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F4D0R_FD15                             ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F4D0R_FD16                             ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F4D0R_FD17                             ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F4D0R_FD18                             ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F4D0R_FD19                             ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F4D0R_FD20                             ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F4D0R_FD21                             ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F4D0R_FD22                             ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F4D0R_FD23                             ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F4D0R_FD24                             ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F4D0R_FD25                             ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F4D0R_FD26                             ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F4D0R_FD27                             ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F4D0R_FD28                             ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F4D0R_FD29                             ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F4D0R_FD30                             ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F4D0R_FD31                             ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F5D0R register  *******************/
#define  CAN_F5D0R_FD0                              ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F5D0R_FD1                              ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F5D0R_FD2                              ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F5D0R_FD3                              ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F5D0R_FD4                              ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F5D0R_FD5                              ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F5D0R_FD6                              ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F5D0R_FD7                              ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F5D0R_FD8                              ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F5D0R_FD9                              ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F5D0R_FD10                             ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F5D0R_FD11                             ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F5D0R_FD12                             ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F5D0R_FD13                             ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F5D0R_FD14                             ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F5D0R_FD15                             ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F5D0R_FD16                             ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F5D0R_FD17                             ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F5D0R_FD18                             ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F5D0R_FD19                             ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F5D0R_FD20                             ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F5D0R_FD21                             ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F5D0R_FD22                             ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F5D0R_FD23                             ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F5D0R_FD24                             ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F5D0R_FD25                             ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F5D0R_FD26                             ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F5D0R_FD27                             ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F5D0R_FD28                             ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F5D0R_FD29                             ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F5D0R_FD30                             ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F5D0R_FD31                             ((uint32_t)0x80000000)        /*!< Filter bit 31 */
									       
/*******************  Bit definition for CAN_F6D0R register  *******************/
#define  CAN_F6D0R_FD0                              ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F6D0R_FD1                              ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F6D0R_FD2                              ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F6D0R_FD3                              ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F6D0R_FD4                              ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F6D0R_FD5                              ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F6D0R_FD6                              ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F6D0R_FD7                              ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F6D0R_FD8                              ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F6D0R_FD9                              ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F6D0R_FD10                             ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F6D0R_FD11                             ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F6D0R_FD12                             ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F6D0R_FD13                             ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F6D0R_FD14                             ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F6D0R_FD15                             ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F6D0R_FD16                             ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F6D0R_FD17                             ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F6D0R_FD18                             ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F6D0R_FD19                             ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F6D0R_FD20                             ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F6D0R_FD21                             ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F6D0R_FD22                             ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F6D0R_FD23                             ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F6D0R_FD24                             ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F6D0R_FD25                             ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F6D0R_FD26                             ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F6D0R_FD27                             ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F6D0R_FD28                             ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F6D0R_FD29                             ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F6D0R_FD30                             ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F6D0R_FD31                             ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F7D0R register  *******************/
#define  CAN_F7D0R_FD0                              ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F7D0R_FD1                              ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F7D0R_FD2                              ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F7D0R_FD3                              ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F7D0R_FD4                              ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F7D0R_FD5                              ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F7D0R_FD6                              ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F7D0R_FD7                              ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F7D0R_FD8                              ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F7D0R_FD9                              ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F7D0R_FD10                             ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F7D0R_FD11                             ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F7D0R_FD12                             ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F7D0R_FD13                             ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F7D0R_FD14                             ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F7D0R_FD15                             ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F7D0R_FD16                             ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F7D0R_FD17                             ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F7D0R_FD18                             ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F7D0R_FD19                             ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F7D0R_FD20                             ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F7D0R_FD21                             ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F7D0R_FD22                             ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F7D0R_FD23                             ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F7D0R_FD24                             ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F7D0R_FD25                             ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F7D0R_FD26                             ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F7D0R_FD27                             ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F7D0R_FD28                             ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F7D0R_FD29                             ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F7D0R_FD30                             ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F7D0R_FD31                             ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F8D0R register  *******************/
#define  CAN_F8D0R_FD0                              ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F8D0R_FD1                              ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F8D0R_FD2                              ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F8D0R_FD3                              ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F8D0R_FD4                              ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F8D0R_FD5                              ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F8D0R_FD6                              ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F8D0R_FD7                              ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F8D0R_FD8                              ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F8D0R_FD9                              ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F8D0R_FD10                             ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F8D0R_FD11                             ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F8D0R_FD12                             ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F8D0R_FD13                             ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F8D0R_FD14                             ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F8D0R_FD15                             ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F8D0R_FD16                             ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F8D0R_FD17                             ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F8D0R_FD18                             ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F8D0R_FD19                             ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F8D0R_FD20                             ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F8D0R_FD21                             ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F8D0R_FD22                             ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F8D0R_FD23                             ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F8D0R_FD24                             ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F8D0R_FD25                             ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F8D0R_FD26                             ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F8D0R_FD27                             ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F8D0R_FD28                             ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F8D0R_FD29                             ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F8D0R_FD30                             ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F8D0R_FD31                             ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F9D0R register  *******************/
#define  CAN_F9D0R_FD0                              ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F9D0R_FD1                              ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F9D0R_FD2                              ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F9D0R_FD3                              ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F9D0R_FD4                              ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F9D0R_FD5                              ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F9D0R_FD6                              ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F9D0R_FD7                              ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F9D0R_FD8                              ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F9D0R_FD9                              ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F9D0R_FD10                             ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F9D0R_FD11                             ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F9D0R_FD12                             ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F9D0R_FD13                             ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F9D0R_FD14                             ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F9D0R_FD15                             ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F9D0R_FD16                             ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F9D0R_FD17                             ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F9D0R_FD18                             ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F9D0R_FD19                             ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F9D0R_FD20                             ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F9D0R_FD21                             ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F9D0R_FD22                             ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F9D0R_FD23                             ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F9D0R_FD24                             ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F9D0R_FD25                             ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F9D0R_FD26                             ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F9D0R_FD27                             ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F9D0R_FD28                             ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F9D0R_FD29                             ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F9D0R_FD30                             ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F9D0R_FD31                             ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F10D0R register  ******************/
#define  CAN_F10D0R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F10D0R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F10D0R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F10D0R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F10D0R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F10D0R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F10D0R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F10D0R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F10D0R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F10D0R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F10D0R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F10D0R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F10D0R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F10D0R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F10D0R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F10D0R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F10D0R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F10D0R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F10D0R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F10D0R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F10D0R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F10D0R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F10D0R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F10D0R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F10D0R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F10D0R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F10D0R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F10D0R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F10D0R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F10D0R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F10D0R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F10D0R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F11D0R register  ******************/
#define  CAN_F11D0R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F11D0R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F11D0R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F11D0R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F11D0R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F11D0R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F11D0R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F11D0R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F11D0R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F11D0R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F11D0R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F11D0R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F11D0R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F11D0R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F11D0R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F11D0R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F11D0R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F11D0R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F11D0R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F11D0R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F11D0R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F11D0R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F11D0R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F11D0R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F11D0R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F11D0R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F11D0R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F11D0R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F11D0R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F11D0R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F11D0R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F11D0R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F12D0R register  ******************/
#define  CAN_F12D0R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F12D0R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F12D0R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F12D0R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F12D0R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F12D0R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F12D0R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F12D0R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F12D0R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F12D0R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F12D0R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F12D0R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F12D0R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F12D0R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F12D0R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F12D0R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F12D0R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F12D0R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F12D0R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F12D0R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F12D0R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F12D0R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F12D0R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F12D0R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F12D0R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F12D0R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F12D0R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F12D0R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F12D0R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F12D0R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F12D0R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F12D0R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F13D0R register  ******************/
#define  CAN_F13D0R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F13D0R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F13D0R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F13D0R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F13D0R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F13D0R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F13D0R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F13D0R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F13D0R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F13D0R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F13D0R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F13D0R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F13D0R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F13D0R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F13D0R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F13D0R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F13D0R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F13D0R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F13D0R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F13D0R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F13D0R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F13D0R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F13D0R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F13D0R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F13D0R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F13D0R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F13D0R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F13D0R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F13D0R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F13D0R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F13D0R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F13D0R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F14D0R register  ******************/
#define  CAN_F14D0R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F14D0R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F14D0R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F14D0R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F14D0R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F14D0R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F14D0R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F14D0R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F14D0R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F14D0R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F14D0R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F14D0R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F14D0R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F14D0R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F14D0R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F14D0R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F14D0R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F14D0R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F14D0R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F14D0R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F14D0R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F14D0R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F14D0R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F14D0R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F14D0R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F14D0R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F14D0R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F14D0R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F14D0R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F14D0R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F14D0R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F14D0R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F15D0R register  ******************/
#define  CAN_F15D0R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F15D0R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F15D0R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F15D0R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F15D0R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F15D0R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F15D0R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F15D0R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F15D0R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F15D0R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F15D0R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F15D0R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F15D0R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F15D0R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F15D0R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F15D0R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F15D0R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F15D0R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F15D0R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F15D0R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F15D0R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F15D0R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F15D0R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F15D0R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F15D0R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F15D0R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F15D0R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F15D0R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F15D0R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F15D0R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F15D0R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F15D0R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F16D0R register  ******************/
#define  CAN_F16D0R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F16D0R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F16D0R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F16D0R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F16D0R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F16D0R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F16D0R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F16D0R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F16D0R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F16D0R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F16D0R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F16D0R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F16D0R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F16D0R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F16D0R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F16D0R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F16D0R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F16D0R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F16D0R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F16D0R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F16D0R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F16D0R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F16D0R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F16D0R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F16D0R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F16D0R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F16D0R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F16D0R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F16D0R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F16D0R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F16D0R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F16D0R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F17D0R register  ******************/
#define  CAN_F17D0R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F17D0R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F17D0R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F17D0R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F17D0R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F17D0R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F17D0R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F17D0R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F17D0R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F17D0R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F17D0R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F17D0R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F17D0R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F17D0R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F17D0R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F17D0R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F17D0R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F17D0R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F17D0R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F17D0R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F17D0R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F17D0R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F17D0R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F17D0R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F17D0R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F17D0R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F17D0R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F17D0R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F17D0R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F17D0R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F17D0R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F17D0R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F18D0R register  ******************/
#define  CAN_F18D0R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F18D0R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F18D0R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F18D0R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F18D0R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F18D0R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F18D0R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F18D0R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F18D0R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F18D0R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F18D0R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F18D0R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F18D0R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F18D0R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F18D0R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F18D0R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F18D0R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F18D0R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F18D0R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F18D0R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F18D0R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F18D0R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F18D0R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F18D0R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F18D0R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F18D0R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F18D0R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F18D0R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F18D0R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F18D0R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F18D0R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F18D0R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F19D0R register  ******************/
#define  CAN_F19D0R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F19D0R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F19D0R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F19D0R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F19D0R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F19D0R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F19D0R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F19D0R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F19D0R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F19D0R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F19D0R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F19D0R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F19D0R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F19D0R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F19D0R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F19D0R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F19D0R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F19D0R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F19D0R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F19D0R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F19D0R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F19D0R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F19D0R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F19D0R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F19D0R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F19D0R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F19D0R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F19D0R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F19D0R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F19D0R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F19D0R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F19D0R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F20D0R register  ******************/
#define  CAN_F20D0R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F20D0R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F20D0R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F20D0R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F20D0R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F20D0R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F20D0R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F20D0R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F20D0R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F20D0R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F20D0R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F20D0R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F20D0R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F20D0R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F20D0R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F20D0R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F20D0R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F20D0R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F20D0R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F20D0R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F20D0R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F20D0R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F20D0R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F20D0R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F20D0R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F20D0R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F20D0R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F20D0R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F20D0R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F20D0R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F20D0R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F20D0R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F21D0R register  ******************/
#define  CAN_F21D0R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F21D0R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F21D0R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F21D0R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F21D0R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F21D0R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F21D0R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F21D0R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F21D0R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F21D0R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F21D0R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F21D0R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F21D0R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F21D0R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F21D0R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F21D0R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F21D0R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F21D0R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F21D0R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F21D0R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F21D0R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F21D0R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F21D0R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F21D0R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F21D0R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F21D0R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F21D0R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F21D0R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F21D0R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F21D0R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F21D0R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F21D0R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F22D0R register  ******************/
#define  CAN_F22D0R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F22D0R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F22D0R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F22D0R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F22D0R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F22D0R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F22D0R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F22D0R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F22D0R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F22D0R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F22D0R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F22D0R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F22D0R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F22D0R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F22D0R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F22D0R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F22D0R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F22D0R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F22D0R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F22D0R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F22D0R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F22D0R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F22D0R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F22D0R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F22D0R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F22D0R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F22D0R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F22D0R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F22D0R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F22D0R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F22D0R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F22D0R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F23D0R register  ******************/
#define  CAN_F23D0R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F23D0R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F23D0R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F23D0R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F23D0R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F23D0R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F23D0R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F23D0R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F23D0R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F23D0R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F23D0R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F23D0R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F23D0R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F23D0R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F23D0R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F23D0R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F23D0R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F23D0R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F23D0R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F23D0R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F23D0R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F23D0R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F23D0R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F23D0R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F23D0R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F23D0R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F23D0R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F23D0R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F23D0R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F23D0R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F23D0R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F23D0R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F24D0R register  ******************/
#define  CAN_F24D0R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F24D0R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F24D0R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F24D0R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F24D0R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F24D0R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F24D0R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F24D0R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F24D0R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F24D0R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F24D0R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F24D0R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F24D0R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F24D0R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F24D0R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F24D0R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F24D0R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F24D0R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F24D0R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F24D0R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F24D0R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F24D0R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F24D0R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F24D0R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F24D0R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F24D0R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F24D0R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F24D0R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F24D0R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F24D0R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F24D0R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F24D0R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F25D0R register  ******************/
#define  CAN_F25D0R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F25D0R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F25D0R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F25D0R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F25D0R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F25D0R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F25D0R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F25D0R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F25D0R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F25D0R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F25D0R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F25D0R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F25D0R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F25D0R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F25D0R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F25D0R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F25D0R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F25D0R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F25D0R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F25D0R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F25D0R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F25D0R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F25D0R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F25D0R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F25D0R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F25D0R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F25D0R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F25D0R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F25D0R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F25D0R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F25D0R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F25D0R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F26D0R register  ******************/
#define  CAN_F26D0R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F26D0R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F26D0R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F26D0R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F26D0R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F26D0R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F26D0R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F26D0R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F26D0R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F26D0R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F26D0R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F26D0R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F26D0R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F26D0R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F26D0R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F26D0R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F26D0R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F26D0R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F26D0R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F26D0R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F26D0R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F26D0R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F26D0R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F26D0R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F26D0R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F26D0R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F26D0R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F26D0R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F26D0R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F26D0R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F26D0R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F26D0R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F27D0R register  ******************/
#define  CAN_F27D0R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F27D0R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F27D0R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F27D0R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F27D0R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F27D0R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F27D0R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F27D0R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F27D0R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F27D0R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F27D0R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F27D0R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F27D0R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F27D0R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F27D0R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F27D0R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F27D0R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F27D0R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F27D0R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F27D0R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F27D0R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F27D0R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F27D0R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F27D0R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F27D0R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F27D0R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F27D0R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F27D0R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F27D0R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F27D0R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F27D0R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F27D0R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F0D1R register  *******************/
#define  CAN_F0D1R_FD0                              ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F0D1R_FD1                              ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F0D1R_FD2                              ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F0D1R_FD3                              ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F0D1R_FD4                              ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F0D1R_FD5                              ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F0D1R_FD6                              ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F0D1R_FD7                              ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F0D1R_FD8                              ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F0D1R_FD9                              ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F0D1R_FD10                             ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F0D1R_FD11                             ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F0D1R_FD12                             ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F0D1R_FD13                             ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F0D1R_FD14                             ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F0D1R_FD15                             ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F0D1R_FD16                             ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F0D1R_FD17                             ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F0D1R_FD18                             ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F0D1R_FD19                             ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F0D1R_FD20                             ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F0D1R_FD21                             ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F0D1R_FD22                             ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F0D1R_FD23                             ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F0D1R_FD24                             ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F0D1R_FD25                             ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F0D1R_FD26                             ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F0D1R_FD27                             ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F0D1R_FD28                             ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F0D1R_FD29                             ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F0D1R_FD30                             ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F0D1R_FD31                             ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F1D1R register  *******************/
#define  CAN_F1D1R_FD0                              ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F1D1R_FD1                              ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F1D1R_FD2                              ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F1D1R_FD3                              ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F1D1R_FD4                              ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F1D1R_FD5                              ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F1D1R_FD6                              ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F1D1R_FD7                              ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F1D1R_FD8                              ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F1D1R_FD9                              ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F1D1R_FD10                             ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F1D1R_FD11                             ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F1D1R_FD12                             ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F1D1R_FD13                             ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F1D1R_FD14                             ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F1D1R_FD15                             ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F1D1R_FD16                             ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F1D1R_FD17                             ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F1D1R_FD18                             ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F1D1R_FD19                             ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F1D1R_FD20                             ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F1D1R_FD21                             ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F1D1R_FD22                             ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F1D1R_FD23                             ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F1D1R_FD24                             ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F1D1R_FD25                             ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F1D1R_FD26                             ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F1D1R_FD27                             ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F1D1R_FD28                             ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F1D1R_FD29                             ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F1D1R_FD30                             ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F1D1R_FD31                             ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F2D1R register  *******************/
#define  CAN_F2D1R_FD0                              ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F2D1R_FD1                              ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F2D1R_FD2                              ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F2D1R_FD3                              ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F2D1R_FD4                              ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F2D1R_FD5                              ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F2D1R_FD6                              ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F2D1R_FD7                              ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F2D1R_FD8                              ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F2D1R_FD9                              ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F2D1R_FD10                             ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F2D1R_FD11                             ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F2D1R_FD12                             ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F2D1R_FD13                             ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F2D1R_FD14                             ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F2D1R_FD15                             ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F2D1R_FD16                             ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F2D1R_FD17                             ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F2D1R_FD18                             ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F2D1R_FD19                             ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F2D1R_FD20                             ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F2D1R_FD21                             ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F2D1R_FD22                             ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F2D1R_FD23                             ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F2D1R_FD24                             ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F2D1R_FD25                             ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F2D1R_FD26                             ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F2D1R_FD27                             ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F2D1R_FD28                             ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F2D1R_FD29                             ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F2D1R_FD30                             ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F2D1R_FD31                             ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F3D1R register  *******************/
#define  CAN_F3D1R_FD0                              ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F3D1R_FD1                              ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F3D1R_FD2                              ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F3D1R_FD3                              ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F3D1R_FD4                              ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F3D1R_FD5                              ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F3D1R_FD6                              ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F3D1R_FD7                              ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F3D1R_FD8                              ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F3D1R_FD9                              ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F3D1R_FD10                             ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F3D1R_FD11                             ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F3D1R_FD12                             ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F3D1R_FD13                             ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F3D1R_FD14                             ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F3D1R_FD15                             ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F3D1R_FD16                             ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F3D1R_FD17                             ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F3D1R_FD18                             ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F3D1R_FD19                             ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F3D1R_FD20                             ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F3D1R_FD21                             ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F3D1R_FD22                             ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F3D1R_FD23                             ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F3D1R_FD24                             ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F3D1R_FD25                             ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F3D1R_FD26                             ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F3D1R_FD27                             ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F3D1R_FD28                             ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F3D1R_FD29                             ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F3D1R_FD30                             ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F3D1R_FD31                             ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F4D1R register  *******************/
#define  CAN_F4D1R_FD0                              ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F4D1R_FD1                              ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F4D1R_FD2                              ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F4D1R_FD3                              ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F4D1R_FD4                              ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F4D1R_FD5                              ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F4D1R_FD6                              ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F4D1R_FD7                              ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F4D1R_FD8                              ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F4D1R_FD9                              ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F4D1R_FD10                             ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F4D1R_FD11                             ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F4D1R_FD12                             ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F4D1R_FD13                             ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F4D1R_FD14                             ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F4D1R_FD15                             ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F4D1R_FD16                             ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F4D1R_FD17                             ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F4D1R_FD18                             ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F4D1R_FD19                             ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F4D1R_FD20                             ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F4D1R_FD21                             ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F4D1R_FD22                             ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F4D1R_FD23                             ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F4D1R_FD24                             ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F4D1R_FD25                             ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F4D1R_FD26                             ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F4D1R_FD27                             ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F4D1R_FD28                             ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F4D1R_FD29                             ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F4D1R_FD30                             ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F4D1R_FD31                             ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F5D1R register  *******************/
#define  CAN_F5D1R_FD0                              ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F5D1R_FD1                              ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F5D1R_FD2                              ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F5D1R_FD3                              ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F5D1R_FD4                              ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F5D1R_FD5                              ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F5D1R_FD6                              ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F5D1R_FD7                              ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F5D1R_FD8                              ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F5D1R_FD9                              ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F5D1R_FD10                             ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F5D1R_FD11                             ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F5D1R_FD12                             ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F5D1R_FD13                             ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F5D1R_FD14                             ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F5D1R_FD15                             ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F5D1R_FD16                             ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F5D1R_FD17                             ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F5D1R_FD18                             ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F5D1R_FD19                             ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F5D1R_FD20                             ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F5D1R_FD21                             ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F5D1R_FD22                             ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F5D1R_FD23                             ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F5D1R_FD24                             ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F5D1R_FD25                             ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F5D1R_FD26                             ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F5D1R_FD27                             ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F5D1R_FD28                             ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F5D1R_FD29                             ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F5D1R_FD30                             ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F5D1R_FD31                             ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F6D1R register  *******************/
#define  CAN_F6D1R_FD0                              ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F6D1R_FD1                              ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F6D1R_FD2                              ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F6D1R_FD3                              ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F6D1R_FD4                              ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F6D1R_FD5                              ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F6D1R_FD6                              ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F6D1R_FD7                              ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F6D1R_FD8                              ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F6D1R_FD9                              ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F6D1R_FD10                             ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F6D1R_FD11                             ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F6D1R_FD12                             ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F6D1R_FD13                             ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F6D1R_FD14                             ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F6D1R_FD15                             ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F6D1R_FD16                             ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F6D1R_FD17                             ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F6D1R_FD18                             ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F6D1R_FD19                             ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F6D1R_FD20                             ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F6D1R_FD21                             ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F6D1R_FD22                             ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F6D1R_FD23                             ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F6D1R_FD24                             ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F6D1R_FD25                             ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F6D1R_FD26                             ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F6D1R_FD27                             ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F6D1R_FD28                             ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F6D1R_FD29                             ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F6D1R_FD30                             ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F6D1R_FD31                             ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F7D1R register  *******************/
#define  CAN_F7D1R_FD0                              ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F7D1R_FD1                              ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F7D1R_FD2                              ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F7D1R_FD3                              ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F7D1R_FD4                              ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F7D1R_FD5                              ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F7D1R_FD6                              ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F7D1R_FD7                              ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F7D1R_FD8                              ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F7D1R_FD9                              ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F7D1R_FD10                             ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F7D1R_FD11                             ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F7D1R_FD12                             ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F7D1R_FD13                             ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F7D1R_FD14                             ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F7D1R_FD15                             ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F7D1R_FD16                             ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F7D1R_FD17                             ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F7D1R_FD18                             ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F7D1R_FD19                             ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F7D1R_FD20                             ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F7D1R_FD21                             ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F7D1R_FD22                             ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F7D1R_FD23                             ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F7D1R_FD24                             ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F7D1R_FD25                             ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F7D1R_FD26                             ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F7D1R_FD27                             ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F7D1R_FD28                             ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F7D1R_FD29                             ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F7D1R_FD30                             ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F7D1R_FD31                             ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F8D1R register  *******************/
#define  CAN_F8D1R_FD0                              ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F8D1R_FD1                              ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F8D1R_FD2                              ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F8D1R_FD3                              ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F8D1R_FD4                              ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F8D1R_FD5                              ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F8D1R_FD6                              ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F8D1R_FD7                              ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F8D1R_FD8                              ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F8D1R_FD9                              ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F8D1R_FD10                             ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F8D1R_FD11                             ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F8D1R_FD12                             ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F8D1R_FD13                             ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F8D1R_FD14                             ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F8D1R_FD15                             ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F8D1R_FD16                             ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F8D1R_FD17                             ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F8D1R_FD18                             ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F8D1R_FD19                             ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F8D1R_FD20                             ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F8D1R_FD21                             ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F8D1R_FD22                             ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F8D1R_FD23                             ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F8D1R_FD24                             ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F8D1R_FD25                             ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F8D1R_FD26                             ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F8D1R_FD27                             ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F8D1R_FD28                             ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F8D1R_FD29                             ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F8D1R_FD30                             ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F8D1R_FD31                             ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F9D1R register  *******************/
#define  CAN_F9D1R_FD0                              ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F9D1R_FD1                              ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F9D1R_FD2                              ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F9D1R_FD3                              ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F9D1R_FD4                              ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F9D1R_FD5                              ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F9D1R_FD6                              ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F9D1R_FD7                              ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F9D1R_FD8                              ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F9D1R_FD9                              ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F9D1R_FD10                             ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F9D1R_FD11                             ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F9D1R_FD12                             ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F9D1R_FD13                             ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F9D1R_FD14                             ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F9D1R_FD15                             ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F9D1R_FD16                             ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F9D1R_FD17                             ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F9D1R_FD18                             ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F9D1R_FD19                             ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F9D1R_FD20                             ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F9D1R_FD21                             ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F9D1R_FD22                             ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F9D1R_FD23                             ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F9D1R_FD24                             ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F9D1R_FD25                             ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F9D1R_FD26                             ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F9D1R_FD27                             ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F9D1R_FD28                             ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F9D1R_FD29                             ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F9D1R_FD30                             ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F9D1R_FD31                             ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F10D1R register  *******************/
#define  CAN_F10D1R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F10D1R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F10D1R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F10D1R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F10D1R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F10D1R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F10D1R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F10D1R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F10D1R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F10D1R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F10D1R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F10D1R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F10D1R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F10D1R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F10D1R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F10D1R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F10D1R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F10D1R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F10D1R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F10D1R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F10D1R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F10D1R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F10D1R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F10D1R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F10D1R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F10D1R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F10D1R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F10D1R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F10D1R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F10D1R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F10D1R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F10D1R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F11D1R register  *******************/
#define  CAN_F11D1R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F11D1R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F11D1R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F11D1R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F11D1R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F11D1R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F11D1R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F11D1R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F11D1R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F11D1R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F11D1R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F11D1R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F11D1R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F11D1R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F11D1R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F11D1R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F11D1R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F11D1R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F11D1R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F11D1R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F11D1R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F11D1R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F11D1R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F11D1R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F11D1R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F11D1R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F11D1R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F11D1R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F11D1R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F11D1R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F11D1R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F11D1R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F12D1R register  *******************/
#define  CAN_F12D1R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F12D1R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F12D1R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F12D1R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F12D1R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F12D1R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F12D1R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F12D1R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F12D1R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F12D1R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F12D1R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F12D1R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F12D1R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F12D1R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F12D1R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F12D1R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F12D1R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F12D1R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F12D1R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F12D1R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F12D1R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F12D1R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F12D1R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F12D1R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F12D1R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F12D1R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F12D1R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F12D1R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F12D1R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F12D1R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F12D1R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F12D1R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F13D1R register  *******************/
#define  CAN_F13D1R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F13D1R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F13D1R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F13D1R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F13D1R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F13D1R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F13D1R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F13D1R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F13D1R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F13D1R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F13D1R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F13D1R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F13D1R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F13D1R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F13D1R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F13D1R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F13D1R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F13D1R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F13D1R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F13D1R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F13D1R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F13D1R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F13D1R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F13D1R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F13D1R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F13D1R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F13D1R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F13D1R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F13D1R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F13D1R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F13D1R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F13D1R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F14D1R register  *******************/
#define  CAN_F14D1R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F14D1R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F14D1R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F14D1R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F14D1R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F14D1R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F14D1R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F14D1R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F14D1R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F14D1R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F14D1R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F14D1R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F14D1R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F14D1R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F14D1R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F14D1R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F14D1R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F14D1R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F14D1R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F14D1R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F14D1R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F14D1R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F14D1R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F14D1R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F14D1R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F14D1R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F14D1R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F14D1R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F14D1R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F14D1R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F14D1R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F14D1R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit for CAN_F15D1R register  *******************/
#define  CAN_F15D1R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F15D1R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F15D1R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F15D1R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F15D1R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F15D1R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F15D1R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F15D1R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F15D1R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F15D1R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F15D1R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F15D1R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F15D1R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F15D1R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F15D1R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F15D1R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F15D1R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F15D1R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F15D1R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F15D1R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F15D1R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F15D1R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F15D1R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F15D1R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F15D1R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F15D1R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F15D1R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F15D1R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F15D1R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F15D1R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F15D1R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F15D1R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F16D1R register  *******************/
#define  CAN_F16D1R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F16D1R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F16D1R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F16D1R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F16D1R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F16D1R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F16D1R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F16D1R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F16D1R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F16D1R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F16D1R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F16D1R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F16D1R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F16D1R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F16D1R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F16D1R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F16D1R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F16D1R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F16D1R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F16D1R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F16D1R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F16D1R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F16D1R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F16D1R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F16D1R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F16D1R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F16D1R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F16D1R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F16D1R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F16D1R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F16D1R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F16D1R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F17D1R register  *******************/
#define  CAN_F17D1R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F17D1R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F17D1R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F17D1R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F17D1R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F17D1R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F17D1R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F17D1R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F17D1R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F17D1R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F17D1R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F17D1R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F17D1R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F17D1R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F17D1R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F17D1R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F17D1R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F17D1R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F17D1R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F17D1R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F17D1R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F17D1R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F17D1R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F17D1R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F17D1R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F17D1R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F17D1R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F17D1R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F17D1R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F17D1R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F17D1R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F17D1R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F18D1R register  *******************/
#define  CAN_F18D1R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F18D1R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F18D1R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F18D1R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F18D1R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F18D1R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F18D1R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F18D1R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F18D1R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F18D1R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F18D1R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F18D1R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F18D1R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F18D1R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F18D1R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F18D1R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F18D1R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F18D1R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F18D1R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F18D1R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F18D1R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F18D1R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F18D1R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F18D1R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F18D1R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F18D1R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F18D1R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F18D1R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F18D1R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F18D1R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F18D1R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F18D1R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F19D1R register  *******************/
#define  CAN_F19D1R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F19D1R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F19D1R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F19D1R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F19D1R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F19D1R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F19D1R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F19D1R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F19D1R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F19D1R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F19D1R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F19D1R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F19D1R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F19D1R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F19D1R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F19D1R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F19D1R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F19D1R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F19D1R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F19D1R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F19D1R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F19D1R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F19D1R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F19D1R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F19D1R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F19D1R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F19D1R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F19D1R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F19D1R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F19D1R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F19D1R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F19D1R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F20D1R register  *******************/
#define  CAN_F20D1R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F20D1R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F20D1R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F20D1R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F20D1R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F20D1R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F20D1R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F20D1R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F20D1R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F20D1R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F20D1R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F20D1R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F20D1R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F20D1R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F20D1R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F20D1R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F20D1R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F20D1R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F20D1R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F20D1R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F20D1R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F20D1R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F20D1R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F20D1R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F20D1R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F20D1R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F20D1R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F20D1R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F20D1R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F20D1R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F20D1R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F20D1R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F21D1R register  *******************/
#define  CAN_F21D1R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F21D1R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F21D1R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F21D1R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F21D1R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F21D1R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F21D1R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F21D1R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F21D1R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F21D1R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F21D1R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F21D1R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F21D1R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F21D1R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F21D1R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F21D1R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F21D1R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F21D1R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F21D1R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F21D1R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F21D1R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F21D1R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F21D1R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F21D1R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F21D1R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F21D1R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F21D1R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F21D1R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F21D1R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F21D1R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F21D1R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F21D1R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F22D1R register  *******************/
#define  CAN_F22D1R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F22D1R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F22D1R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F22D1R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F22D1R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F22D1R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F22D1R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F22D1R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F22D1R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F22D1R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F22D1R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F22D1R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F22D1R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F22D1R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F22D1R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F22D1R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F22D1R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F22D1R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F22D1R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F22D1R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F22D1R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F22D1R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F22D1R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F22D1R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F22D1R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F22D1R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F22D1R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F22D1R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F22D1R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F22D1R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F22D1R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F22D1R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F23D1R register  *******************/
#define  CAN_F23D1R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F23D1R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F23D1R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F23D1R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F23D1R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F23D1R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F23D1R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F23D1R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F23D1R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F23D1R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F23D1R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F23D1R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F23D1R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F23D1R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F23D1R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F23D1R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F23D1R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F23D1R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F23D1R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F23D1R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F23D1R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F23D1R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F23D1R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F23D1R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F23D1R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F23D1R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F23D1R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F23D1R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F23D1R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F23D1R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F23D1R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F23D1R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F24D1R register  *******************/
#define  CAN_F24D1R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F24D1R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F24D1R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F24D1R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F24D1R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F24D1R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F24D1R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F24D1R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F24D1R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F24D1R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F24D1R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F24D1R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F24D1R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F24D1R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F24D1R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F24D1R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F24D1R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F24D1R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F24D1R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F24D1R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F24D1R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F24D1R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F24D1R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F24D1R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F24D1R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F24D1R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F24D1R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F24D1R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F24D1R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F24D1R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F24D1R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F24D1R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F25D1R register  *******************/
#define  CAN_F25D1R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F25D1R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F25D1R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F25D1R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F25D1R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F25D1R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F25D1R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F25D1R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F25D1R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F25D1R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F25D1R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F25D1R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F25D1R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F25D1R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F25D1R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F25D1R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F25D1R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F25D1R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F25D1R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F25D1R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F25D1R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F25D1R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F25D1R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F25D1R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F25D1R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F25D1R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F25D1R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F25D1R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F25D1R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F25D1R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F25D1R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F25D1R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F26D1R register  *******************/
#define  CAN_F26D1R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F26D1R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F26D1R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F26D1R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F26D1R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F26D1R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F26D1R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F26D1R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F26D1R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F26D1R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F26D1R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F26D1R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F26D1R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F26D1R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F26D1R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F26D1R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F26D1R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F26D1R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F26D1R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F26D1R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F26D1R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F26D1R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F26D1R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F26D1R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F26D1R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F26D1R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F26D1R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F26D1R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F26D1R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F26D1R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F26D1R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F26D1R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/*******************  Bit definition for CAN_F27D1R register  *******************/
#define  CAN_F27D1R_FD0                             ((uint32_t)0x00000001)        /*!< Filter bit 0 */
#define  CAN_F27D1R_FD1                             ((uint32_t)0x00000002)        /*!< Filter bit 1 */
#define  CAN_F27D1R_FD2                             ((uint32_t)0x00000004)        /*!< Filter bit 2 */
#define  CAN_F27D1R_FD3                             ((uint32_t)0x00000008)        /*!< Filter bit 3 */
#define  CAN_F27D1R_FD4                             ((uint32_t)0x00000010)        /*!< Filter bit 4 */
#define  CAN_F27D1R_FD5                             ((uint32_t)0x00000020)        /*!< Filter bit 5 */
#define  CAN_F27D1R_FD6                             ((uint32_t)0x00000040)        /*!< Filter bit 6 */
#define  CAN_F27D1R_FD7                             ((uint32_t)0x00000080)        /*!< Filter bit 7 */
#define  CAN_F27D1R_FD8                             ((uint32_t)0x00000100)        /*!< Filter bit 8 */
#define  CAN_F27D1R_FD9                             ((uint32_t)0x00000200)        /*!< Filter bit 9 */
#define  CAN_F27D1R_FD10                            ((uint32_t)0x00000400)        /*!< Filter bit 10 */
#define  CAN_F27D1R_FD11                            ((uint32_t)0x00000800)        /*!< Filter bit 11 */
#define  CAN_F27D1R_FD12                            ((uint32_t)0x00001000)        /*!< Filter bit 12 */
#define  CAN_F27D1R_FD13                            ((uint32_t)0x00002000)        /*!< Filter bit 13 */
#define  CAN_F27D1R_FD14                            ((uint32_t)0x00004000)        /*!< Filter bit 14 */
#define  CAN_F27D1R_FD15                            ((uint32_t)0x00008000)        /*!< Filter bit 15 */
#define  CAN_F27D1R_FD16                            ((uint32_t)0x00010000)        /*!< Filter bit 16 */
#define  CAN_F27D1R_FD17                            ((uint32_t)0x00020000)        /*!< Filter bit 17 */
#define  CAN_F27D1R_FD18                            ((uint32_t)0x00040000)        /*!< Filter bit 18 */
#define  CAN_F27D1R_FD19                            ((uint32_t)0x00080000)        /*!< Filter bit 19 */
#define  CAN_F27D1R_FD20                            ((uint32_t)0x00100000)        /*!< Filter bit 20 */
#define  CAN_F27D1R_FD21                            ((uint32_t)0x00200000)        /*!< Filter bit 21 */
#define  CAN_F27D1R_FD22                            ((uint32_t)0x00400000)        /*!< Filter bit 22 */
#define  CAN_F27D1R_FD23                            ((uint32_t)0x00800000)        /*!< Filter bit 23 */
#define  CAN_F27D1R_FD24                            ((uint32_t)0x01000000)        /*!< Filter bit 24 */
#define  CAN_F27D1R_FD25                            ((uint32_t)0x02000000)        /*!< Filter bit 25 */
#define  CAN_F27D1R_FD26                            ((uint32_t)0x04000000)        /*!< Filter bit 26 */
#define  CAN_F27D1R_FD27                            ((uint32_t)0x08000000)        /*!< Filter bit 27 */
#define  CAN_F27D1R_FD28                            ((uint32_t)0x10000000)        /*!< Filter bit 28 */
#define  CAN_F27D1R_FD29                            ((uint32_t)0x20000000)        /*!< Filter bit 29 */
#define  CAN_F27D1R_FD30                            ((uint32_t)0x40000000)        /*!< Filter bit 30 */
#define  CAN_F27D1R_FD31                            ((uint32_t)0x80000000)        /*!< Filter bit 31 */

/******************************************************************************/
/*                                                                            */
/*                       CRC calculation unit (CRC)                           */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for CRC_DTR register  *********************/
#define  CRC_DTR_DATA                               ((uint32_t)0xFFFFFFFF)        /*!< Data register bits */

/*******************  Bit definition for CRC_FDTR register  ********************/
#define  CRC_FDTR_FDR                               ((uint8_t)0xFF)               /*!< General-purpose 8-bit data register bits */

/********************  Bit definition for CRC_CTLR register  ********************/
#define  CRC_CTLR_RESET                             ((uint32_t)0x00000001)        /*!< RESET the CRC computation unit bit */

/******************************************************************************/
/*                                                                            */
/*                    Digital to Analog Converter (DAC)                       */
/*                                                                            */
/******************************************************************************/

/********************  Bit definition for DAC_CTLR register  ********************/
#define  DAC_CTLR_DEN1                              ((uint32_t)0x00000001)        /*!< DAC channel1 enable */
#define  DAC_CTLR_DBOFF1                            ((uint32_t)0x00000002)        /*!< DAC channel1 output buffer disable */
#define  DAC_CTLR_DTEN1                             ((uint32_t)0x00000004)        /*!< DAC channel1 Trigger enable */

#define  DAC_CTLR_DTSEL1                            ((uint32_t)0x00000038)        /*!< TSEL1[2:0] (DAC channel1 Trigger selection) */
#define  DAC_CTLR_DTSEL1_0                          ((uint32_t)0x00000008)        /*!< Bit 0 */
#define  DAC_CTLR_DTSEL1_1                          ((uint32_t)0x00000010)        /*!< Bit 1 */
#define  DAC_CTLR_DTSEL1_2                          ((uint32_t)0x00000020)        /*!< Bit 2 */

#define  DAC_CTLR_DWAVE1                            ((uint32_t)0x000000C0)        /*!< WAVE1[1:0] (DAC channel1 noise/triangle wave generation enable) */
#define  DAC_CTLR_DWAVE1_0                          ((uint32_t)0x00000040)        /*!< Bit 0 */
#define  DAC_CTLR_DWAVE1_1                          ((uint32_t)0x00000080)        /*!< Bit 1 */

#define  DAC_CTLR_DMAMP1                            ((uint32_t)0x00000F00)        /*!< MAMP1[3:0] (DAC channel1 Mask/Amplitude selector) */
#define  DAC_CTLR_DMAMP1_0                          ((uint32_t)0x00000100)        /*!< Bit 0 */
#define  DAC_CTLR_DMAMP1_1                          ((uint32_t)0x00000200)        /*!< Bit 1 */
#define  DAC_CTLR_DMAMP1_2                          ((uint32_t)0x00000400)        /*!< Bit 2 */
#define  DAC_CTLR_DMAMP1_3                          ((uint32_t)0x00000800)        /*!< Bit 3 */

#define  DAC_CTLR_DDMAEN1                           ((uint32_t)0x00001000)        /*!< DAC channel1 DMA enable */
#define  DAC_CTLR_DEN2                              ((uint32_t)0x00010000)        /*!< DAC channel2 enable */
#define  DAC_CTLR_DBOFF2                            ((uint32_t)0x00020000)        /*!< DAC channel2 output buffer disable */
#define  DAC_CTLR_DTEN2                             ((uint32_t)0x00040000)        /*!< DAC channel2 Trigger enable */

#define  DAC_CTLR_DTSEL2                            ((uint32_t)0x00380000)        /*!< TSEL2[2:0] (DAC channel2 Trigger selection) */
#define  DAC_CTLR_DTSEL2_0                          ((uint32_t)0x00080000)        /*!< Bit 0 */
#define  DAC_CTLR_DTSEL2_1                          ((uint32_t)0x00100000)        /*!< Bit 1 */
#define  DAC_CTLR_DTSEL2_2                          ((uint32_t)0x00200000)        /*!< Bit 2 */

#define  DAC_CTLR_DWAVE2                            ((uint32_t)0x00C00000)        /*!< WAVE2[1:0] (DAC channel2 noise/triangle wave generation enable) */
#define  DAC_CTLR_DWAVE2_0                          ((uint32_t)0x00400000)        /*!< Bit 0 */
#define  DAC_CTLR_DWAVE2_1                          ((uint32_t)0x00800000)        /*!< Bit 1 */

#define  DAC_CTLR_DMAMP2                            ((uint32_t)0x0F000000)        /*!< MAMP2[3:0] (DAC channel2 Mask/Amplitude selector) */
#define  DAC_CTLR_DMAMP2_0                          ((uint32_t)0x01000000)        /*!< Bit 0 */
#define  DAC_CTLR_DMAMP2_1                          ((uint32_t)0x02000000)        /*!< Bit 1 */
#define  DAC_CTLR_DMAMP2_2                          ((uint32_t)0x04000000)        /*!< Bit 2 */
#define  DAC_CTLR_DMAMP2_3                          ((uint32_t)0x08000000)        /*!< Bit 3 */

#define  DAC_CTLR_DDMAEN2                           ((uint32_t)0x10000000)        /*!< DAC channel2 DMA enabled */

     
/*****************  Bit definition for DAC_SWTR register  ******************/
#define  DAC_SWTR_SWTR1                             ((uint8_t)0x01)               /*!< DAC channel1 software trigger */
#define  DAC_SWTR_SWTR2                             ((uint8_t)0x02)               /*!< DAC channel2 software trigger */

/*****************  Bit definition for DAC_C1R12DHR register  ******************/
#define  DAC_C1R12DHR_DAC_C1DHR                     ((uint16_t)0x0FFF)            /*!< DAC channel1 12-bit Right aligned data */

/*****************  Bit definition for DAC_C1L12DHR register  ******************/
#define  DAC_C1L12DHR_DAC_C1DHR                     ((uint16_t)0xFFF0)            /*!< DAC channel1 12-bit Left aligned data */

/******************  Bit definition for DAC_C1R8DHR register  ******************/
#define  DAC_C1R8DHR_DAC_C1DHR                      ((uint8_t)0xFF)               /*!< DAC channel1 8-bit Right aligned data */


/*****************  Bit definition for DAC_C2R12DHR register  ******************/
#define  DAC_C2R12DHR_DAC_C2DHR                     ((uint16_t)0x0FFF)            /*!< DAC channel2 12-bit Right aligned data */

/*****************  Bit definition for DAC_C2L12DHR register  ******************/
#define  DAC_C2L12DHR_DAC_C2DHR                     ((uint16_t)0xFFF0)            /*!< DAC channel2 12-bit Left aligned data */

/******************  Bit definition for DAC_C2R8DHR register  ******************/
#define  DAC_C2R8DHR_DAC_C2DHR                      ((uint8_t)0xFF)               /*!< DAC channel2 8-bit Right aligned data */


/*****************  Bit definition for DAC_DCR12DHR register  ******************/
#define  DAC_DCR12DHR_DAC_C1DHR                     ((uint32_t)0x00000FFF)        /*!< DAC channel1 12-bit Right aligned data */
#define  DAC_DCR12DHR_DAC_C2DHR                     ((uint32_t)0x0FFF0000)        /*!< DAC channel2 12-bit Right aligned data */


/*****************  Bit definition for DAC_DCL12DHR register  ******************/
#define  DAC_DCL12DHR_DAC_C1DHR                     ((uint32_t)0x0000FFF0)        /*!< DAC channel1 12-bit Left aligned data */
#define  DAC_DCL12DHR_DAC_C2DHR                     ((uint32_t)0xFFF00000)        /*!< DAC channel2 12-bit Left aligned data */

/******************  Bit definition for DAC_DCR8DHR register  ******************/
#define  DAC_DCR8DHR_DAC_C1DHR                      ((uint16_t)0x00FF)          /*!< DAC channel1 8-bit Right aligned data */
#define  DAC_DCR8DHR_DAC_C2DHR                      ((uint16_t)0xFF00)            /*!< DAC channel2 8-bit Right aligned data */

/*******************  Bit definition for DAC_C1ODR register  *******************/
#define  DAC_C1ODR_DAC_ODR1                         ((uint16_t)0x0FFF)            /*!< DAC channel1 data output */

/*******************  Bit definition for DAC_C2ODR register  *******************/
#define  DAC_C2ODR_DAC_ODR2                         ((uint16_t)0x0FFF)            /*!< DAC channel2 data output */

/******************************************************************************/
/*                                                                            */
/*                           Debug MCU (MCUDBG)                               */
/*                                                                            */
/******************************************************************************/

/****************  Bit definition for MCUDBG_IDR register  *****************/
#define  MCUDBG_IDR_DEV_ID                          ((uint32_t)0x00000FFF)        /*!< Device Identifier */
#define  MCUDBG_IDR_REV_ID                          ((uint32_t)0xFFFF0000)        /*!< REV_ID[15:0] bits (Revision Identifier) */
#define  MCUDBG_IDR_REV_ID_0                        ((uint32_t)0x00010000)        /*!< Bit 0 */
#define  MCUDBG_IDR_REV_ID_1                        ((uint32_t)0x00020000)        /*!< Bit 1 */
#define  MCUDBG_IDR_REV_ID_2                        ((uint32_t)0x00040000)        /*!< Bit 2 */
#define  MCUDBG_IDR_REV_ID_3                        ((uint32_t)0x00080000)        /*!< Bit 3 */
#define  MCUDBG_IDR_REV_ID_4                        ((uint32_t)0x00100000)        /*!< Bit 4 */
#define  MCUDBG_IDR_REV_ID_5                        ((uint32_t)0x00200000)        /*!< Bit 5 */
#define  MCUDBG_IDR_REV_ID_6                        ((uint32_t)0x00400000)        /*!< Bit 6 */
#define  MCUDBG_IDR_REV_ID_7                        ((uint32_t)0x00800000)        /*!< Bit 7 */
#define  MCUDBG_IDR_REV_ID_8                        ((uint32_t)0x01000000)        /*!< Bit 8 */
#define  MCUDBG_IDR_REV_ID_9                        ((uint32_t)0x02000000)        /*!< Bit 9 */
#define  MCUDBG_IDR_REV_ID_10                       ((uint32_t)0x04000000)        /*!< Bit 10 */
#define  MCUDBG_IDR_REV_ID_11                       ((uint32_t)0x08000000)        /*!< Bit 11 */
#define  MCUDBG_IDR_REV_ID_12                       ((uint32_t)0x10000000)        /*!< Bit 12 */
#define  MCUDBG_IDR_REV_ID_13                       ((uint32_t)0x20000000)        /*!< Bit 13 */
#define  MCUDBG_IDR_REV_ID_14                       ((uint32_t)0x40000000)        /*!< Bit 14 */
#define  MCUDBG_IDR_REV_ID_15                       ((uint32_t)0x80000000)        /*!< Bit 15 */

/******************  Bit definition for MCUDBG_CR register  *******************/
#define  MCUDBG_CR_DBG_DEEPSLEEP                    ((uint32_t)0x00000002)        /*!< Debug Deep-sleep Mode */
#define  MCUDBG_CR_DBG_STANDBY                      ((uint32_t)0x00000004)        /*!< Debug Standby mode */
										 
/******************  Bit definition for MCUDBG_APB1_FZ register  **************/
#define  MCUDBG_APB1_FZ_DBG_TIMER2_STOP             ((uint32_t)0x00000001)        /*!< TIMER2 counter stopped when core is halted */
#define  MCUDBG_APB1_FZ_DBG_TIMER3_STOP             ((uint32_t)0x00000002)        /*!< TIMER3 counter stopped when core is halted */
#define  MCUDBG_APB1_FZ_DBG_TIMER6_STOP             ((uint32_t)0x00000010)        /*!< TIMER6 counter stopped when core is halted */
#define  MCUDBG_APB1_FZ_DBG_TIMER14_STOP            ((uint32_t)0x00000100)        /*!< TIMER14 counter stopped when core is halted */
#define  MCUDBG_CTLR2_RTC_HOLD                      ((uint32_t)0x00000400)        /*!< RTC Calendar frozen when core is halted */
#define  MCUDBG_APB1_FZ_DBG_WWDG_STOP               ((uint32_t)0x00000800)        /*!< Debug Window Watchdog stopped when Core is halted */
#define  MCUDBG_APB1_FZ_DBG_IWDG_STOP               ((uint32_t)0x00001000)        /*!< Debug Independent Watchdog stopped when Core is halted */
#define  MCUDBG_APB1_FZ_DBG_I2C1_SMBUS_TIMEREOUT    ((uint32_t)0x00200000)        /*!< I2C1 SMBUS timeout mode stopped when Core is halted */

/******************  Bit definition for MCUDBG_APB2_FZ register  **************/
#define  MCUDBG_APB2_FZ_DBG_TIMER1_STOP             ((uint32_t)0x00000800)        /*!< TIMER1 counter stopped when core is halted */
#define  MCUDBG_APB2_FZ_DBG_TIMER15_STOP            ((uint32_t)0x00010000)        /*!< TIMER15 counter stopped when core is halted */
#define  MCUDBG_APB2_FZ_DBG_TIMER16_STOP            ((uint32_t)0x00020000)        /*!< TIMER16 counter stopped when core is halted */
#define  MCUDBG_APB2_FZ_DBG_TIMER17_STOP            ((uint32_t)0x00040000)        /*!< TIMER17 counter stopped when core is halted */

/******************************************************************************/
/*                                                                            */
/*                           DMA Controller (DMA)                             */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for DMA_IFR register  ********************/
#define  DMA_IFR_GIF1                               ((uint32_t)0x00000001)        /*!< Channel 1 Global interrupt flag */
#define  DMA_IFR_TCIF1                              ((uint32_t)0x00000002)        /*!< Channel 1 Transfer Complete flag */
#define  DMA_IFR_HTIF1                              ((uint32_t)0x00000004)        /*!< Channel 1 Half Transfer flag */
#define  DMA_IFR_ERRIF1                             ((uint32_t)0x00000008)        /*!< Channel 1 Transfer Error flag */
#define  DMA_IFR_GIF2                               ((uint32_t)0x00000010)        /*!< Channel 2 Global interrupt flag */
#define  DMA_IFR_TCIF2                              ((uint32_t)0x00000020)        /*!< Channel 2 Transfer Complete flag */
#define  DMA_IFR_HTIF2                              ((uint32_t)0x00000040)        /*!< Channel 2 Half Transfer flag */
#define  DMA_IFR_ERRIF2                             ((uint32_t)0x00000080)        /*!< Channel 2 Transfer Error flag */
#define  DMA_IFR_GIF3                               ((uint32_t)0x00000100)        /*!< Channel 3 Global interrupt flag */
#define  DMA_IFR_TCIF3                              ((uint32_t)0x00000200)        /*!< Channel 3 Transfer Complete flag */
#define  DMA_IFR_HTIF3                              ((uint32_t)0x00000400)        /*!< Channel 3 Half Transfer flag */
#define  DMA_IFR_ERRIF3                             ((uint32_t)0x00000800)        /*!< Channel 3 Transfer Error flag */
#define  DMA_IFR_GIF4                               ((uint32_t)0x00001000)        /*!< Channel 4 Global interrupt flag */
#define  DMA_IFR_TCIF4                              ((uint32_t)0x00002000)        /*!< Channel 4 Transfer Complete flag */
#define  DMA_IFR_HTIF4                              ((uint32_t)0x00004000)        /*!< Channel 4 Half Transfer flag */
#define  DMA_IFR_ERRIF4                             ((uint32_t)0x00008000)        /*!< Channel 4 Transfer Error flag */
#define  DMA_IFR_GIF5                               ((uint32_t)0x00010000)        /*!< Channel 5 Global interrupt flag */
#define  DMA_IFR_TCIF5                              ((uint32_t)0x00020000)        /*!< Channel 5 Transfer Complete flag */
#define  DMA_IFR_HTIF5                              ((uint32_t)0x00040000)        /*!< Channel 5 Half Transfer flag */
#define  DMA_IFR_ERRIF5                             ((uint32_t)0x00080000)        /*!< Channel 5 Transfer Error flag */
#define  DMA_IFR_GIF6                               ((uint32_t)0x00100000)        /*!< Channel 6 Global interrupt flag */
#define  DMA_IFR_TCIF6                              ((uint32_t)0x00200000)        /*!< Channel 6 Transfer Complete flag */
#define  DMA_IFR_HTIF6                              ((uint32_t)0x00400000)        /*!< Channel 6 Half Transfer flag */
#define  DMA_IFR_ERRIF6                             ((uint32_t)0x00800000)        /*!< Channel 6 Transfer Error flag */
#define  DMA_IFR_GIF7                               ((uint32_t)0x01000000)        /*!< Channel 7 Global interrupt flag */
#define  DMA_IFR_TCIF7                              ((uint32_t)0x02000000)        /*!< Channel 7 Transfer Complete flag */
#define  DMA_IFR_HTIF7                              ((uint32_t)0x04000000)        /*!< Channel 7 Half Transfer flag */
#define  DMA_IFR_ERRIF7                             ((uint32_t)0x08000000)        /*!< Channel 7 Transfer Error flag */

/*******************  Bit definition for DMA_ICR register  *******************/
#define  DMA_ICR_GIC1                               ((uint32_t)0x00000001)        /*!< Channel 1 Global interrupt clear    */
#define  DMA_ICR_TCIC1                              ((uint32_t)0x00000002)        /*!< Channel 1 Transfer Complete clear   */
#define  DMA_ICR_HTIC1                              ((uint32_t)0x00000004)        /*!< Channel 1 Half Transfer clear       */
#define  DMA_ICR_ERRIC1                             ((uint32_t)0x00000008)        /*!< Channel 1 Transfer Error clear      */
#define  DMA_ICR_GIC2                               ((uint32_t)0x00000010)        /*!< Channel 2 Global interrupt clear    */
#define  DMA_ICR_TCIC2                              ((uint32_t)0x00000020)        /*!< Channel 2 Transfer Complete clear   */
#define  DMA_ICR_HTIC2                              ((uint32_t)0x00000040)        /*!< Channel 2 Half Transfer clear       */
#define  DMA_ICR_ERRIC2                             ((uint32_t)0x00000080)        /*!< Channel 2 Transfer Error clear      */
#define  DMA_ICR_GIC3                               ((uint32_t)0x00000100)        /*!< Channel 3 Global interrupt clear    */
#define  DMA_ICR_TCIC3                              ((uint32_t)0x00000200)        /*!< Channel 3 Transfer Complete clear   */
#define  DMA_ICR_HTIC3                              ((uint32_t)0x00000400)        /*!< Channel 3 Half Transfer clear       */
#define  DMA_ICR_ERRIC3                             ((uint32_t)0x00000800)        /*!< Channel 3 Transfer Error clear      */
#define  DMA_ICR_GIC4                               ((uint32_t)0x00001000)        /*!< Channel 4 Global interrupt clear    */
#define  DMA_ICR_TCIC4                              ((uint32_t)0x00002000)        /*!< Channel 4 Transfer Complete clear   */
#define  DMA_ICR_HTIC4                              ((uint32_t)0x00004000)        /*!< Channel 4 Half Transfer clear       */
#define  DMA_ICR_ERRIC4                             ((uint32_t)0x00008000)        /*!< Channel 4 Transfer Error clear      */
#define  DMA_ICR_GIC5                               ((uint32_t)0x00010000)        /*!< Channel 5 Global interrupt clear    */
#define  DMA_ICR_TCIC5                              ((uint32_t)0x00020000)        /*!< Channel 5 Transfer Complete clear   */
#define  DMA_ICR_HTIC5                              ((uint32_t)0x00040000)        /*!< Channel 5 Half Transfer clear       */
#define  DMA_ICR_ERRIC5                             ((uint32_t)0x00080000)        /*!< Channel 5 Transfer Error clear      */
#define  DMA_ICR_GIC6                               ((uint32_t)0x00100000)        /*!< Channel 6 Global interrupt clear    */
#define  DMA_ICR_TCIC6                              ((uint32_t)0x00200000)        /*!< Channel 6 Transfer Complete clear   */
#define  DMA_ICR_HTIC6                              ((uint32_t)0x00400000)        /*!< Channel 6 Half Transfer clear       */
#define  DMA_ICR_ERRIC6                             ((uint32_t)0x00800000)        /*!< Channel 6 Transfer Error clear      */
#define  DMA_ICR_GIC7                               ((uint32_t)0x01000000)        /*!< Channel 7 Global interrupt clear    */
#define  DMA_ICR_TCIC7                              ((uint32_t)0x02000000)        /*!< Channel 7 Transfer Complete clear   */
#define  DMA_ICR_HTIC7                              ((uint32_t)0x04000000)        /*!< Channel 6 Half Transfer clear       */
#define  DMA_ICR_ERRIC7                             ((uint32_t)0x08000000)        /*!< Channel 7 Transfer Error clear      */

/*******************  Bit definition for DMA_CTLRx register  ********************/
#define  DMA_CTLR_CHEN                              ((uint32_t)0x00000001)        /*!< Channel enable                      */
#define  DMA_CTLR_TCIE                              ((uint32_t)0x00000002)        /*!< Transfer complete interrupt enable  */
#define  DMA_CTLR_HTIE                              ((uint32_t)0x00000004)        /*!< Half Transfer interrupt enable      */
#define  DMA_CTLR_ERRIE                             ((uint32_t)0x00000008)        /*!< Transfer error interrupt enable     */
#define  DMA_CTLR_DIR                               ((uint32_t)0x00000010)        /*!< Data transfer direction             */
#define  DMA_CTLR_CIRC                              ((uint32_t)0x00000020)        /*!< Circular mode                       */
#define  DMA_CTLR_PNAGA                             ((uint32_t)0x00000040)        /*!< Peripheral increment mode           */
#define  DMA_CTLR_MNAGA                             ((uint32_t)0x00000080)        /*!< Memory increment mode               */
								 
#define  DMA_CTLR_PSIZE                             ((uint32_t)0x00000300)        /*!< PSIZE[1:0] bits (Peripheral size)   */
#define  DMA_CTLR_PSIZE_0                           ((uint32_t)0x00000100)        /*!< Bit 0                               */
#define  DMA_CTLR_PSIZE_1                           ((uint32_t)0x00000200)        /*!< Bit 1                               */
									 
#define  DMA_CTLR_MSIZE                             ((uint32_t)0x00000C00)        /*!< MSIZE[1:0] bits (Memory size)       */
#define  DMA_CTLR_MSIZE_0                           ((uint32_t)0x00000400)        /*!< Bit 0                               */
#define  DMA_CTLR_MSIZE_1                           ((uint32_t)0x00000800)        /*!< Bit 1                               */
										 
#define  DMA_CTLR_PRIO                              ((uint32_t)0x00003000)        /*!< PRIO[1:0] bits(Channel Priority level)*/
#define  DMA_CTLR_PRIO_0                            ((uint32_t)0x00001000)        /*!< Bit 0                               */
#define  DMA_CTLR_PRIO_1                            ((uint32_t)0x00002000)        /*!< Bit 1                               */

#define  DMA_CTLR_MEMTOMEM                          ((uint32_t)0x00004000)        /*!< Memory to memory mode               */

/*******************  Bit definition for DMA_ACR register  ********************/
#define  DMA_ACR_FD_MODE6                           ((uint32_t)0x00000020)        /*!< Enable the channel 6 Full_Data transfer mode */                      

/******************************************************************************/
/*                         Peripheral Registers_Bits_Definition                */
/******************************************************************************/
/******************************************************************************/
/*                Ethernet MAC Registers bits definitions                     */
/******************************************************************************/

/* Bit definition for Ethernet MAC Configuration register */
#define ETH_MAC_CFR_WDD                             ((uint32_t)0x00800000)        /* Watchdog disable */
#define ETH_MAC_CFR_JBD                             ((uint32_t)0x00400000)        /* Jabber disable */
#define ETH_MAC_CFR_IG                              ((uint32_t)0x000E0000)        /* Inter-frame gap */
#define ETH_MAC_CFR_IG_96BIT                        ((uint32_t)0x00000000)        /* Minimum IG 96Bit */ 
#define ETH_MAC_CFR_IG_88BIT                        ((uint32_t)0x00020000)        /* Minimum IG 88Bit */
#define ETH_MAC_CFR_IG_80BIT                        ((uint32_t)0x00040000)        /* Minimum IG 80Bit */
#define ETH_MAC_CFR_IG_72BIT                        ((uint32_t)0x00060000)        /* Minimum IG 72Bit */
#define ETH_MAC_CFR_IG_64BIT                        ((uint32_t)0x00080000)        /* Minimum IG 64Bit */        
#define ETH_MAC_CFR_IG_56BIT                        ((uint32_t)0x000A0000)        /* Minimum IG 56Bit */
#define ETH_MAC_CFR_IG_48BIT                        ((uint32_t)0x000C0000)        /* Minimum IG 48Bit */
#define ETH_MAC_CFR_IG_40BIT                        ((uint32_t)0x000E0000)        /* Minimum IG 40Bit */              
#define ETH_MAC_CFR_CSD                             ((uint32_t)0x00010000)        /* Carrier sense disable */
#define ETH_MAC_CFR_SPD                             ((uint32_t)0x00004000)        /* Ethernet speed */
#define ETH_MAC_CFR_ROD                             ((uint32_t)0x00002000)        /* Receive own disable */
#define ETH_MAC_CFR_LBM                             ((uint32_t)0x00001000)        /* loopback mode */
#define ETH_MAC_CFR_DPM                             ((uint32_t)0x00000800)        /* Duplex mode */
#define ETH_MAC_CFR_IP4CO                           ((uint32_t)0x00000400)        /* IP Checksum offload */
#define ETH_MAC_CFR_RTD                             ((uint32_t)0x00000200)        /* Retry disable */
#define ETH_MAC_CFR_APCD                            ((uint32_t)0x00000080)        /* automatic pad/CRC drop */
#define ETH_MAC_CFR_BOL                             ((uint32_t)0x00000060)        /* Back-off limit */
#define ETH_MAC_CFR_BOL_10                          ((uint32_t)0x00000000)        /* min (n, 10) */
#define ETH_MAC_CFR_BOL_8                           ((uint32_t)0x00000020)        /* min (n, 8) */
#define ETH_MAC_CFR_BOL_4                           ((uint32_t)0x00000040)        /* min (n, 4) */
#define ETH_MAC_CFR_BOL_1                           ((uint32_t)0x00000060)        /* min (n, 1) */ 
#define ETH_MAC_CFR_DFC                             ((uint32_t)0x00000010)        /* Defferal check */
#define ETH_MAC_CFR_TEN                             ((uint32_t)0x00000008)        /* Transmitter enable */
#define ETH_MAC_CFR_REN                             ((uint32_t)0x00000004)        /* Receiver enable */
  
  /* Bit definition for Ethernet MAC Frame Filter Register */
#define ETH_MAC_FRMFR_FD                            ((uint32_t)0x80000000)        /* Filter disable */ 
#define ETH_MAC_FRMFR_HPFLT                         ((uint32_t)0x00000400)        /* Hash or perfect filter */ 
#define ETH_MAC_FRMFR_SAFLT                         ((uint32_t)0x00000200)        /* Source address filter enable */ 
#define ETH_MAC_FRMFR_SAIFLT                        ((uint32_t)0x00000100)        /* SA inverse filtering enable */ 
#define ETH_MAC_FRMFR_PCFRM                         ((uint32_t)0x000000C0)        /* Pass control frames */
#define ETH_MAC_FRMFR_PCF_BLOCKALL                  ((uint32_t)0x00000040)        /* Prevent all control frames received */
#define ETH_MAC_FRMFR_PCF_FORWARDALL                ((uint32_t)0x00000080)        /* Accept all control frames received */
#define ETH_MAC_FRMFR_PCF_FORWARDPASSEDADDRFILTER   ((uint32_t)0x000000C0)        /* Accept control frames that pass the Address Filter */ 
#define ETH_MAC_FRMFR_BFRMD                         ((uint32_t)0x00000020)        /* Broadcast frame disable */ 
#define ETH_MAC_FRMFR_MFD                           ((uint32_t)0x00000010)        /* Multicast filter disable */ 
#define ETH_MAC_FRMFR_DAIFLT                        ((uint32_t)0x00000008)        /* DA Inverse filtering enable */ 
#define ETH_MAC_FRMFR_HMF                           ((uint32_t)0x00000004)        /* Hash multicast filter enable */ 
#define ETH_MAC_FRMFR_HUF                           ((uint32_t)0x00000002)        /* Hash unicast filter enable */
#define ETH_MAC_FRMFR_PM                            ((uint32_t)0x00000001)        /* Promiscuous mode */
  
/* Bit definition for Ethernet MAC Hash List High Register */
#define ETH_MAC_HLHR_HLH                            ((uint32_t)0xFFFFFFFF)        /* Hash list high */
  
/* Bit definition for Ethernet MAC Hash List Low Register */			         
#define ETH_MAC_HLLR_HLL                            ((uint32_t)0xFFFFFFFF)        /* Hash list low */
  
/* Bit definition for Ethernet MAC PHY Address Register */
#define ETH_MAC_PHYAR_PA                            ((uint32_t)0x0000F800)        /* PHY address */ 
#define ETH_MAC_PHYAR_PR                            ((uint32_t)0x000007C0)        /* PHY register address  */ 
#define ETH_MAC_PHYAR_CLR                           ((uint32_t)0x0000001C)        /* MDC clock range */ 
#define ETH_MAC_PHYAR_CLR_DIV42                     ((uint32_t)0x00000000)        /* HCLK:60-90 MHz; MDC clock= HCLK/42 */
#define ETH_MAC_PHYAR_CLR_DIV64                     ((uint32_t)0x00000004)        /* HCLK:90-108 MHz; MDC clock= HCLK/64 */
#define ETH_MAC_PHYAR_CLR_DIV16                     ((uint32_t)0x00000008)        /* HCLK:20-35 MHz; MDC clock= HCLK/16 */
#define ETH_MAC_PHYAR_CLR_DIV26                     ((uint32_t)0x0000000C)        /* HCLK:35-60 MHz; MDC clock= HCLK/26 */
#define ETH_MAC_PHYAR_PW                            ((uint32_t)0x00000002)        /* PHY write */ 
#define ETH_MAC_PHYAR_PB                            ((uint32_t)0x00000001)        /* PHY busy */ 
    
/* Bit definition for Ethernet PHY Data Register */
#define ETH_MAC_PHYDR_PD                            ((uint32_t)0x0000FFFF)        /* PHY data: read/write data from/to PHY */
  
/* Bit definition for Ethernet MAC Flow Control Register */
#define ETH_MAC_FCTLR_PTM                           ((uint32_t)0xFFFF0000)        /* Pause time */
#define ETH_MAC_FCTLR_ZQPD                          ((uint32_t)0x00000080)        /* Zero-quanta pause disable */
#define ETH_MAC_FCTLR_PLTS                          ((uint32_t)0x00000030)        /* Pause low threshold */
#define ETH_MAC_FCTLR_PLTS_MINUS4                   ((uint32_t)0x00000000)        /* Pause time minus 4 slot times */
#define ETH_MAC_FCTLR_PLTS_MINUS28                  ((uint32_t)0x00000010)        /* Pause time minus 28 slot times */
#define ETH_MAC_FCTLR_PLTS_MINUS144                 ((uint32_t)0x00000020)        /* Pause time minus 144 slot times */
#define ETH_MAC_FCTLR_PLTS_MINUS256                 ((uint32_t)0x00000030)        /* Pause time minus 256 slot times */      
#define ETH_MAC_FCTLR_UPFDT                         ((uint32_t)0x00000008)        /* Unicast pause frame detect enable*/
#define ETH_MAC_FCTLR_RFCEN                         ((uint32_t)0x00000004)        /* Receive flow control enable */
#define ETH_MAC_FCTLR_TFCEN                         ((uint32_t)0x00000002)        /* Transmit flow control enable */
#define ETH_MAC_FCTLR_FLCBBKPA                      ((uint32_t)0x00000001)        /* Flow control busy(in full duplex mode)/backpressure activate(in half duplex mode) */
  
/* Bit definition for Ethernet MAC VLAN Tag Register */
#define ETH_MAC_VLTR_VLTC                           ((uint32_t)0x00010000)        /* 12-bit VLAN tag comparison enable*/
#define ETH_MAC_VLTR_VLTI                           ((uint32_t)0x0000FFFF)        /* VLAN tag identifier */
  
/* Bit definition for Ethernet MAC Remote Wake-UpFrame Filter Register */ 
#define ETH_MAC_RWUFFR_D                            ((uint32_t)0xFFFFFFFF)        /* Wake-up frame filter register data */
/* Eight sequential Writes to this address (offset 0x28) will write all Wake-UpFrame Filter Registers.
   Eight sequential Reads from this address (offset 0x28) will read all Wake-UpFrame Filter Registers. */
/* Wake-UpFrame Filter Reg0 : Filter 0 Byte Mask
   Wake-UpFrame Filter Reg1 : Filter 1 Byte Mask
   Wake-UpFrame Filter Reg2 : Filter 2 Byte Mask
   Wake-UpFrame Filter Reg3 : Filter 3 Byte Mask
   Wake-UpFrame Filter Reg4 : RSVD - Filter3 Command - RSVD - Filter2 Command - 
                              RSVD - Filter1 Command - RSVD - Filter0 Command
   Wake-UpFrame Filter Re5 : Filter3 Offset - Filter2 Offset - Filter1 Offset - Filter0 Offset
   Wake-UpFrame Filter Re6 : Filter1 CRC16 - Filter0 CRC16
   Wake-UpFrame Filter Re7 : Filter3 CRC16 - Filter2 CRC16 */
  
/* Bit definition for Ethernet MAC WUM Register */ 
#define ETH_MAC_WUMR_WUFFRPR                        ((uint32_t)0x80000000)        /* Wake-Up Frame Filter Register Pointer Reset */
#define ETH_MAC_WUMR_GU                             ((uint32_t)0x00000200)        /* Global Unicast */
#define ETH_MAC_WUMR_WUFR                           ((uint32_t)0x00000040)        /* Wake-Up Frame Received */
#define ETH_MAC_WUMR_MPKR                           ((uint32_t)0x00000020)        /* Magic Packet Received */
#define ETH_MAC_WUMR_WFEN                           ((uint32_t)0x00000004)        /* Wake-Up Frame Enable */
#define ETH_MAC_WUMR_MPEN                           ((uint32_t)0x00000002)        /* Magic Packet Enable */
#define ETH_MAC_WUMR_PWD                            ((uint32_t)0x00000001)        /* Power Down */
  
/* Bit definition for Ethernet MAC Status Register */
#define ETH_MAC_ISR_TMST                            ((uint32_t)0x00000200)        /* Time stamp trigger status */
#define ETH_MAC_ISR_MSCT                            ((uint32_t)0x00000040)        /* MSC transmit status */
#define ETH_MAC_ISR_MSCR                            ((uint32_t)0x00000020)        /* MSC receive status */
#define ETH_MAC_ISR_MSC                             ((uint32_t)0x00000010)        /* MSC status */
#define ETH_MAC_ISR_WUM                             ((uint32_t)0x00000008)        /* WUM status */
  										   
/* Bit definition for Ethernet MAC Interrupt Mask Register */
#define ETH_MAC_IMR_TMSTIM                          ((uint32_t)0x00000200)        /* Time stamp trigger interrupt mask */
#define ETH_MAC_IMR_WUMIM                           ((uint32_t)0x00000008)        /* WUM interrupt mask */
  											   
/* Bit definition for Ethernet MAC Address0 High Register */
#define ETH_MAC_A0HR_ADDR0H                         ((uint32_t)0x0000FFFF)        /* MAC address0 high */
  
  /* Bit definition for Ethernet MAC Address0 Low Register */
#define ETH_MAC_A0HR_ADDR0L                         ((uint32_t)0xFFFFFFFF)        /* MAC address0 low */
  
/* Bit definition for Ethernet MAC Address1 High Register */
#define ETH_MAC_A1HR_AFE                            ((uint32_t)0x80000000)        /* Address filter enable */
#define ETH_MAC_A1HR_SAF                            ((uint32_t)0x40000000)        /* Source address filter */
#define ETH_MAC_A1HR_MB                             ((uint32_t)0x3F000000)        /* Mask byte control */
#define ETH_MAC_A1HR_MB_HBITS15_8                   ((uint32_t)0x20000000)        /* High register bits [15:8] */
#define ETH_MAC_A1HR_MB_HBITS7_0                    ((uint32_t)0x10000000)        /* High register bits [7:0] */
#define ETH_MAC_A1HR_MB_LBITS31_24                  ((uint32_t)0x08000000)        /* Low register bits [31:24] */
#define ETH_MAC_A1HR_MB_LBITS23_16                  ((uint32_t)0x04000000)        /* Low register bits [23:16] */
#define ETH_MAC_A1HR_MB_LBITS15_8                   ((uint32_t)0x02000000)        /* Low register bits [15:8] */
#define ETH_MAC_A1HR_MB_LBITS7_0                    ((uint32_t)0x01000000)        /* Low register bits [7:0] */ 
#define ETH_MAC_A1HR_ADDR1H                         ((uint32_t)0x0000FFFF)        /* MAC address1 high */
  
/* Bit definition for Ethernet MAC Address1 Low Register */
#define ETH_MAC_A1LR_ADDR1L                         ((uint32_t)0xFFFFFFFF)        /* MAC address1 low */
  
/* Bit definition for Ethernet MAC Address2 High Register */
#define ETH_MAC_A2HR_AFE                            ((uint32_t)0x80000000)        /* Address filter enable */
#define ETH_MAC_A2HR_SAF                            ((uint32_t)0x40000000)        /* Source address filter */
#define ETH_MAC_A2HR_MB                             ((uint32_t)0x3F000000)        /* Mask byte control */
#define ETH_MAC_A2HR_MB_HBITS15_8                   ((uint32_t)0x20000000)        /* High register bits [15:8] */
#define ETH_MAC_A2HR_MB_HBITS7_0                    ((uint32_t)0x10000000)        /* High register bits [7:0] */
#define ETH_MAC_A2HR_MB_LBITS31_24                  ((uint32_t)0x08000000)        /* Low register bits [31:24] */
#define ETH_MAC_A2HR_MB_LBITS23_16                  ((uint32_t)0x04000000)        /* Low register bits [23:16] */
#define ETH_MAC_A2HR_MB_LBITS15_8                   ((uint32_t)0x02000000)        /* Low register bits [15:8] */
#define ETH_MAC_A2HR_MB_LBITS7_0                    ((uint32_t)0x01000000)        /* Low register bits [7:0] */
#define ETH_MAC_A2HR_ADDR2H                         ((uint32_t)0x0000FFFF)        /* MAC address2 high */
  
/* Bit definition for Ethernet MAC Address2 Low Register */
#define ETH_MAC_A2LR_ADDR2L                         ((uint32_t)0xFFFFFFFF)        /* MAC address2 low */
  
/* Bit definition for Ethernet MAC Address3 High Register */
#define ETH_MAC_A3HR_AFE                            ((uint32_t)0x80000000)        /* Address filter enable */
#define ETH_MAC_A3HR_SAF                            ((uint32_t)0x40000000)        /* Source address filter */
#define ETH_MAC_A3HR_MB                             ((uint32_t)0x3F000000)        /* Mask byte control */
#define ETH_MAC_A3HR_MB_HBITS15_8                   ((uint32_t)0x20000000)        /* High register bits [15:8] */
#define ETH_MAC_A3HR_MB_HBITS7_0                    ((uint32_t)0x10000000)        /* High register bits [7:0] */
#define ETH_MAC_A3HR_MB_LBITS31_24                  ((uint32_t)0x08000000)        /* Low register bits [31:24] */
#define ETH_MAC_A3HR_MB_LBITS23_16                  ((uint32_t)0x04000000)        /* Low register bits [23:16] */
#define ETH_MAC_A3HR_MB_LBITS15_8                   ((uint32_t)0x02000000)        /* Low register bits [15:8] */
#define ETH_MAC_A3HR_MB_LBITS7_0                    ((uint32_t)0x01000000)        /* Low register bits [7:0] */
#define ETH_MAC_A3HR_ADDR3H                         ((uint32_t)0x0000FFFF)        /* MAC address3 high */
  											    
/* Bit definition for Ethernet MAC Address3 Low Register */
#define ETH_MAC_A3LR_ADDR3L                         ((uint32_t)0xFFFFFFFF)        /* MAC address3 low */
  
/* Bit definition for Ethernet MAC Flow Control threshold Register */
#define ETH_MAC_FCTHR_RFD                           ((uint32_t)0x00000070)        /* Threshold of deactive flow control */
#define ETH_MAC_FCTHR_RFD_256BYTES                  ((uint32_t)0x00000000)        /* Threshold level is 256 bytes */
#define ETH_MAC_FCTHR_RFD_512BYTES                  ((uint32_t)0x00000010)        /* Threshold level is 512 bytes */
#define ETH_MAC_FCTHR_RFD_768BYTES                  ((uint32_t)0x00000020)        /* Threshold level is 768 bytes */
#define ETH_MAC_FCTHR_RFD_1024BYTES                 ((uint32_t)0x00000030)        /* Threshold level is 1024 bytes */
#define ETH_MAC_FCTHR_RFD_1280BYTES                 ((uint32_t)0x00000040)        /* Threshold level is 1280 bytes */
#define ETH_MAC_FCTHR_RFD_1536BYTES                 ((uint32_t)0x00000050)        /* Threshold level is 1536 bytes */
#define ETH_MAC_FCTHR_RFD_1792BYTES                 ((uint32_t)0x00000060)        /* Threshold level is 1792 bytes */
#define ETH_MAC_FCTHR_RFA                           ((uint32_t)0x00000007)        /* Threshold of active flow control */
#define ETH_MAC_FCTHR_RFA_256BYTES                  ((uint32_t)0x00000000)        /* Threshold level is 256 bytes */
#define ETH_MAC_FCTHR_RFA_512BYTES                  ((uint32_t)0x00000001)        /* Threshold level is 512 bytes */
#define ETH_MAC_FCTHR_RFA_768BYTES                  ((uint32_t)0x00000002)        /* Threshold level is 768 bytes */
#define ETH_MAC_FCTHR_RFA_1024BYTES                 ((uint32_t)0x00000003)        /* Threshold level is 1024 bytes */
#define ETH_MAC_FCTHR_RFA_1280BYTES                 ((uint32_t)0x00000004)        /* Threshold level is 1280 bytes */
#define ETH_MAC_FCTHR_RFA_1536BYTES                 ((uint32_t)0x00000005)        /* Threshold level is 1536 bytes */
#define ETH_MAC_FCTHR_RFA_1792BYTES                 ((uint32_t)0x00000006)        /* Threshold level is 1792 bytes */
/******************************************************************************/
/*                Ethernet MSC Registers bits definition                        */
/******************************************************************************/
  
/* Bit definition for Ethernet MSC Contol Register */
#define ETH_MSC_CTLR_MCFZ                           ((uint32_t)0x00000008)        /* MSC Counter Freeze */
#define ETH_MSC_CTLR_RTOR                           ((uint32_t)0x00000004)        /* Reset on Read */
#define ETH_MSC_CTLR_CTSR                           ((uint32_t)0x00000002)        /* Counter Stop Rollover */
#define ETH_MSC_CTLR_CTR                            ((uint32_t)0x00000001)        /* Counters Reset */
  								    
/* Bit definition for Ethernet MSC Receive Interrupt Status Register */
#define ETH_MSC_RISR_RGUF                           ((uint32_t)0x00020000)        /* receive good unicast frames counter reaches half the maximum value */
#define ETH_MSC_RISR_RFAE                           ((uint32_t)0x00000040)        /* receive alignment error counter reaches half the maximum value */
#define ETH_MSC_RISR_RFCE                           ((uint32_t)0x00000020)        /* receive crc error counter reaches half the maximum value */
  
/* Bit definition for Ethernet MSC Transmit Interrupt Status Register */
#define ETH_MSC_TISR_TGF                            ((uint32_t)0x00200000)        /* Transmit good frame count counter reaches half the maximum value */
#define ETH_MSC_TISR_TGFMSC                         ((uint32_t)0x00008000)        /* Transmit good multi col counter reaches half the maximum value */
#define ETH_MSC_TISR_TGFSC                          ((uint32_t)0x00004000)        /* Transmit good single col counter reaches half the maximum value */
  										  
/* Bit definition for Ethernet MSC Receive Interrupt Mask Register */
#define ETH_MSC_RIMR_RGUFIM                         ((uint32_t)0x00020000)        /* RGUF interrupt Mask */
#define ETH_MSC_RIMR_RFAEIM                         ((uint32_t)0x00000040)        /* RFAE interrupt Mask */
#define ETH_MSC_RIMR_RFCEIM                         ((uint32_t)0x00000020)        /* RFCE interrupt Mask */
  
  /* Bit definition for Ethernet MSC Transmit Interrupt Mask Register */
#define ETH_MSC_TIMR_TGFIM                          ((uint32_t)0x00200000)        /* TGF interrupt Mask */
#define ETH_MSC_TIMR_TGFMSCIM                       ((uint32_t)0x00008000)        /* TGFMSC interrupt Mask */
#define ETH_MSC_TIMR_TGFSCIM                        ((uint32_t)0x00004000)        /* TGFSC interrupt Mask */
  
/* Bit definition for Ethernet MSC Transmitted Good Frames after Single Collision Counter Register */
#define ETH_MSC_SCCNT_SCC                           ((uint32_t)0xFFFFFFFF)        /* Transmitted good frames single collision counter */
  
/* Bit definition for Ethernet MSC Transmitted Good Frames after More than a Single Collision Counter Register */
#define ETH_MSC_MSCCNT_MSCC                         ((uint32_t)0xFFFFFFFF)        /* Transmitted good frames after more single collision counter */
  
/* Bit definition for Ethernet MSC Transmitted Good Frames Counter Register */
#define ETH_MSC_TGFCNT_TGF                          ((uint32_t)0xFFFFFFFF)        /* Transmitted good frames counter */
  
/* Bit definition for Ethernet MSC Received Frames with CRC Error Counter Register */
#define ETH_MSC_RFCECNT_RFCER                       ((uint32_t)0xFFFFFFFF)        /* Received frames CRC error counter */
  
/* Bit definition for Ethernet MSC Received Frames with Alignement Error Counter Register */
#define ETH_MSC_RFAECNT_RFAER                       ((uint32_t)0xFFFFFFFF)        /* Received frames alignment error counter */
  
/* Bit definition for Ethernet MSC Received Good Unicast Frames Counter Register */
#define ETH_MSC_RGUFCNT_RGUF                        ((uint32_t)0xFFFFFFFF)        /* Received good unicast frames counter */
  
/******************************************************************************/
/*               Ethernet PTP Registers bits definition                        */
/******************************************************************************/
  
/* Bit definition for Ethernet PTP Time Stamp Contol Register */
#define ETH_PTP_TSCTLR_TMSARU                       ((uint32_t)0x00000020)        /* Time stamp addend register update */
#define ETH_PTP_TSCTLR_TMSITEN                      ((uint32_t)0x00000010)        /* Time stamp interrupt trigger enable */
#define ETH_PTP_TSCTLR_TMSSTU                       ((uint32_t)0x00000008)        /* Time stamp system time update */
#define ETH_PTP_TSCTLR_TMSSTI                       ((uint32_t)0x00000004)        /* Time stamp system time initialize */
#define ETH_PTP_TSCTLR_TMSFCU                       ((uint32_t)0x00000002)        /* Time stamp fine or coarse update */
#define ETH_PTP_TSCTLR_TMSEN                        ((uint32_t)0x00000001)        /* Time stamp enable */
  
/* Bit definition for Ethernet PTP Sub-Second Increment Register */
#define ETH_PTP_SSINCR_STMSSI                       ((uint32_t)0x000000FF)        /* System time Subsecond increment */
  
/* Bit definition for Ethernet PTP Time Stamp High Register */
#define ETH_PTP_TMSHR_STMS                          ((uint32_t)0xFFFFFFFF)        /* System Time second */
  
/* Bit definition for Ethernet PTP Time Stamp Low Register */
#define ETH_PTP_TMSLR_STS                           ((uint32_t)0x80000000)        /* System Time Positive or negative time */
#define ETH_PTP_TMSLR_STMSS                         ((uint32_t)0x7FFFFFFF)        /* System Time subseconds */
  
/* Bit definition for Ethernet PTP Time Stamp High Update Register */
#define ETH_PTP_TMSHUR_TMSUS                        ((uint32_t)0xFFFFFFFF)        /* Time stamp update seconds */
  
/* Bit definition for Ethernet PTP Time Stamp Low Update Register */
#define ETH_PTP_TMSLUR_TMSUPNS                      ((uint32_t)0x80000000)        /* Time stamp update Positive or negative time */
#define ETH_PTP_TMSLUR_TMSUSS                       ((uint32_t)0x7FFFFFFF)        /* Time stamp update subseconds */
  										    
/* Bit definition for Ethernet PTP Time Stamp Addend Register */
#define ETH_PTP_TSACNT_TMSA                         ((uint32_t)0xFFFFFFFF)        /* Time stamp addend */
  
/* Bit definition for Ethernet PTP Expected Time High Register */
#define ETH_PTP_ETHR_ETSH                           ((uint32_t)0xFFFFFFFF)        /* Expected time stamp high */
  
/* Bit definition for Ethernet PTP Expected Time Low Register */
#define ETH_PTP_ETLR_ETSL                           ((uint32_t)0xFFFFFFFF)  /* Expected time stamp low */
  
/******************************************************************************/
/*                 Ethernet DMA Registers bits definition                     */
/******************************************************************************/
  
/* Bit definition for Ethernet DMA Bus Control Register */
#define ETH_DMA_BCR_AA                              ((uint32_t)0x02000000)        /* Address-aligned */
#define ETH_DMA_BCR_FPBL                            ((uint32_t)0x01000000)        /* four x PBL mode */
#define ETH_DMA_BCR_UIP                             ((uint32_t)0x00800000)        /* Use Independent PBL */
#define ETH_DMA_BCR_RXDP                            ((uint32_t)0x007E0000)        /* receive DMA PBL */
#define ETH_DMA_BCR_RXDP_1BEAT                      ((uint32_t)0x00020000)        /* maximum number of beats 1 */
#define ETH_DMA_BCR_RXDP_2BEAT                      ((uint32_t)0x00040000)        /* maximum number of beats 2 */
#define ETH_DMA_BCR_RXDP_4BEAT                      ((uint32_t)0x00080000)        /* maximum number of beats 4 */
#define ETH_DMA_BCR_RXDP_8BEAT                      ((uint32_t)0x00100000)        /* maximum number of beats 8 */
#define ETH_DMA_BCR_RXDP_16BEAT                     ((uint32_t)0x00200000)        /* maximum number of beats 16 */
#define ETH_DMA_BCR_RXDP_32BEAT                     ((uint32_t)0x00400000)        /* maximum number of beats 32 */                
#define ETH_DMA_BCR_RXDP_4xPBL_4BEAT                ((uint32_t)0x01020000)        /* maximum number of beats 4 */
#define ETH_DMA_BCR_RXDP_4xPBL_8BEAT                ((uint32_t)0x01040000)        /* maximum number of beats 8 */
#define ETH_DMA_BCR_RXDP_4xPBL_16BEAT               ((uint32_t)0x01080000)        /* maximum number of beats 16 */
#define ETH_DMA_BCR_RXDP_4xPBL_32BEAT               ((uint32_t)0x01100000)        /* maximum number of beats 32 */
#define ETH_DMA_BCR_RXDP_4xPBL_64BEAT               ((uint32_t)0x01200000)        /* maximum number of beats 64 */
#define ETH_DMA_BCR_RXDP_4xPBL_128BEAT              ((uint32_t)0x01400000)        /* maximum number of beats 128 */  
#define ETH_DMA_BCR_FB                              ((uint32_t)0x00010000)        /* Fixed Burst */
#define ETH_DMA_BCR_RTPR                            ((uint32_t)0x0000C000)        /* receive and transmit priority ratio */
#define ETH_DMA_BCR_RTPR_1_1                        ((uint32_t)0x00000000)        /* receive and transmit priority ratio */
#define ETH_DMA_BCR_RTPR_2_1                        ((uint32_t)0x00004000)        /* receive and transmit priority ratio */
#define ETH_DMA_BCR_RTPR_3_1                        ((uint32_t)0x00008000)        /* receive and transmit priority ratio */
#define ETH_DMA_BCR_RTPR_4_1                        ((uint32_t)0x0000C000)        /* receive and transmit priority ratio */  
#define ETH_DMA_BCR_PGBL                            ((uint32_t)0x00003F00)        /* Programmable burst length */
#define ETH_DMA_BCR_PBL_1BEAT                       ((uint32_t)0x00000100)        /* maximum number of beats is 1 */
#define ETH_DMA_BCR_PBL_2BEAT                       ((uint32_t)0x00000200)        /* maximum number of beats is 2 */
#define ETH_DMA_BCR_PBL_4BEAT                       ((uint32_t)0x00000400)        /* maximum number of beats is 4 */
#define ETH_DMA_BCR_PBL_8BEAT                       ((uint32_t)0x00000800)        /* maximum number of beats is 8 */
#define ETH_DMA_BCR_PBL_16BEAT                      ((uint32_t)0x00001000)        /* maximum number of beats is 16 */
#define ETH_DMA_BCR_PBL_32BEAT                      ((uint32_t)0x00002000)        /* maximum number of beats is 32 */                
#define ETH_DMA_BCR_PBL_4xPBL_4BEAT                 ((uint32_t)0x01000100)        /* maximum number of beats is 4 */
#define ETH_DMA_BCR_PBL_4xPBL_8BEAT                 ((uint32_t)0x01000200)        /* maximum number of beats is 8 */
#define ETH_DMA_BCR_PBL_4xPBL_16BEAT                ((uint32_t)0x01000400)        /* maximum number of beats is 16 */
#define ETH_DMA_BCR_PBL_4xPBL_32BEAT                ((uint32_t)0x01000800)        /* maximum number of beats is 32 */
#define ETH_DMA_BCR_PBL_4xPBL_64BEAT                ((uint32_t)0x01001000)        /* maximum number of beats is 64 */
#define ETH_DMA_BCR_PBL_4xPBL_128BEAT               ((uint32_t)0x01002000)        /* maximum number of beats is 128 */
#define ETH_DMA_BCR_DPSL                            ((uint32_t)0x0000007C)        /* Descriptor Skip Length */
#define ETH_DMA_BCR_DAB                             ((uint32_t)0x00000002)        /* DMA arbitration */
#define ETH_DMA_BCR_SWR                             ((uint32_t)0x00000001)        /* Software reset */
  
/* Bit definition for Ethernet DMA Transmit Poll Enable Register */
#define ETH_DMA_TPER_TPE                            ((uint32_t)0xFFFFFFFF)        /* Transmit poll enable */
  
/* Bit definition for Ethernet DMA Receive Poll Enable Register */
#define ETH_DMA_RPER_RPE                            ((uint32_t)0xFFFFFFFF)        /* Receive poll enable  */
  										   
/* Bit definition for Ethernet DMA Receive Descriptor Tab Address Register */
#define ETH_DMA_RDTAR_SRT                           ((uint32_t)0xFFFFFFFF)        /* Start of receive tab */
  
/* Bit definition for Ethernet DMA Transmit Descriptor Tab Address Register */
#define ETH_DMA_TDTAR_STT                           ((uint32_t)0xFFFFFFFF)        /* Start of transmit tab */
  
/* Bit definition for Ethernet DMA Status Register */
#define ETH_DMA_STR_TST                             ((uint32_t)0x20000000)        /* Time-stamp trigger status */
#define ETH_DMA_STR_WUM                             ((uint32_t)0x10000000)        /* WUM status */
#define ETH_DMA_STR_MSC                             ((uint32_t)0x08000000)        /* MSC status */
#define ETH_DMA_STR_EB                              ((uint32_t)0x03800000)        /* Error bits status */
/* combination with EB[2:0] for GetBitState function */
#define ETH_DMA_STR_EB_DESCACCESS                   ((uint32_t)0x02000000)        /* Error during descriptor or buffer access */
#define ETH_DMA_STR_EB_READTRANSF                   ((uint32_t)0x01000000)        /* Error during write trnsf of read transfr */
#define ETH_DMA_STR_EB_DATATRANSFTX                 ((uint32_t)0x00800000)        /* Error during data transfer by TxDMA or RxDMA */
#define ETH_DMA_STR_TP                              ((uint32_t)0x00700000)        /* Transmit process state */
#define ETH_DMA_STR_TP_STOPPED                      ((uint32_t)0x00000000)        /* Reset or Stop Tx Command issued  */
#define ETH_DMA_STR_TP_FETCHING                     ((uint32_t)0x00100000)        /* fetching the Tx descriptor */
#define ETH_DMA_STR_TP_WAITING                      ((uint32_t)0x00200000)        /* waiting for status */
#define ETH_DMA_STR_TP_READING                      ((uint32_t)0x00300000)        /* reading the data from host memory buffer and queuing it to transmit buffer */
#define ETH_DMA_STR_TP_SUSPENDED                    ((uint32_t)0x00600000)        /* Tx Descriptor unavailabe or transmit buffer underflow */
#define ETH_DMA_STR_TP_CLOSING                      ((uint32_t)0x00700000)        /* closing Tx descriptor */
#define ETH_DMA_STR_RP                              ((uint32_t)0x000E0000)        /* Receive process state */
#define ETH_DMA_STR_RP_STOPPED                      ((uint32_t)0x00000000)        /* Reset or Stop Rx Command issued */
#define ETH_DMA_STR_RP_FETCHING                     ((uint32_t)0x00020000)        /* fetching the Rx descriptor */
#define ETH_DMA_STR_RP_WAITING                      ((uint32_t)0x00060000)        /* waiting for receive packet */
#define ETH_DMA_STR_RP_SUSPENDED                    ((uint32_t)0x00080000)        /* Rx Descriptor unavailable */
#define ETH_DMA_STR_RP_CLOSING                      ((uint32_t)0x000A0000)        /* closing receive descriptor */
#define ETH_DMA_STR_RP_QUEUING                      ((uint32_t)0x000E0000)        /* transferring the receive packet data from recevie buffer to host memory */
#define ETH_DMA_STR_NI                              ((uint32_t)0x00010000)        /* Normal interrupt summary */
#define ETH_DMA_STR_AI                              ((uint32_t)0x00008000)        /* Abnormal interrupt summary */
#define ETH_DMA_STR_ER                              ((uint32_t)0x00004000)        /* Early receive status */
#define ETH_DMA_STR_FBE                             ((uint32_t)0x00002000)        /* Fatal bus error status */
#define ETH_DMA_STR_ET                              ((uint32_t)0x00000400)        /* Early transmit status */
#define ETH_DMA_STR_RWT                             ((uint32_t)0x00000200)        /* Receive watchdog timeout status */
#define ETH_DMA_STR_RPS                             ((uint32_t)0x00000100)        /* Receive process stopped status */
#define ETH_DMA_STR_RBU                             ((uint32_t)0x00000080)        /* Receive buffer unavailable status */
#define ETH_DMA_STR_RS                              ((uint32_t)0x00000040)        /* Receive status */
#define ETH_DMA_STR_TU                              ((uint32_t)0x00000020)        /* Transmit underflow status */
#define ETH_DMA_STR_RO                              ((uint32_t)0x00000010)        /* Receive overflow status */
#define ETH_DMA_STR_TJT                             ((uint32_t)0x00000008)        /* Transmit jabber timeout status */
#define ETH_DMA_STR_TBU                             ((uint32_t)0x00000004)        /* Transmit buffer unavailable status */
#define ETH_DMA_STR_TPS                             ((uint32_t)0x00000002)        /* Transmit process stopped status */
#define ETH_DMA_STR_TS                              ((uint32_t)0x00000001)        /* Transmit status */
 
/* Bit definition for Ethernet DMA Control Register */
#define ETH_DMA_CTLR_DTCERFD                        ((uint32_t)0x04000000)        /* Dropping of TCP/IP checksum error frames disable */
#define ETH_DMA_CTLR_RSFD                           ((uint32_t)0x02000000)        /* Receive store and forward */
#define ETH_DMA_CTLR_DAFRF                          ((uint32_t)0x01000000)        /* Disable flushing of received frames */
#define ETH_DMA_CTLR_TSFD                           ((uint32_t)0x00200000)        /* Transmit store and forward */
#define ETH_DMA_CTLR_FTF                            ((uint32_t)0x00100000)        /* Flush transmit FIFO */
#define ETH_DMA_CTLR_TTHC                           ((uint32_t)0x0001C000)        /* Transmit threshold control */
#define ETH_DMAOMR_TTHC_64BYTES                     ((uint32_t)0x00000000)        /* threshold level is 64 Bytes */
#define ETH_DMAOMR_TTHC_128BYTES                    ((uint32_t)0x00004000)        /* threshold level is 128 Bytes */
#define ETH_DMAOMR_TTHC_192BYTES                    ((uint32_t)0x00008000)        /* threshold level is 192 Bytes */
#define ETH_DMAOMR_TTHC_256BYTES                    ((uint32_t)0x0000C000)        /* threshold level is 256 Bytes */
#define ETH_DMAOMR_TTHC_40BYTES                     ((uint32_t)0x00010000)        /* threshold level is 40 Bytes */
#define ETH_DMAOMR_TTHC_32BYTES                     ((uint32_t)0x00014000)        /* threshold level is 32 Bytes */
#define ETH_DMAOMR_TTHC_24BYTES                     ((uint32_t)0x00018000)        /* threshold level is 24 Bytes */
#define ETH_DMAOMR_TTHC_16BYTES                     ((uint32_t)0x0001C000)        /* threshold level is 16 Bytes */
#define ETH_DMA_CTLR_STE                            ((uint32_t)0x00002000)        /* Start/stop transmission */
#define ETH_DMA_CTLR_FERF                           ((uint32_t)0x00000080)        /* Forward error frames */
#define ETH_DMA_CTLR_FUF                            ((uint32_t)0x00000040)        /* Forward undersized good frames */
#define ETH_DMA_CTLR_RTHC                           ((uint32_t)0x00000018)        /* receive threshold control */
#define ETH_DMAOMR_RTHC_64BYTES                     ((uint32_t)0x00000000)        /* threshold level is 64 Bytes */
#define ETH_DMAOMR_RTHC_32BYTES                     ((uint32_t)0x00000008)        /* threshold level is 32 Bytes */
#define ETH_DMAOMR_RTHC_96BYTES                     ((uint32_t)0x00000010)        /* threshold level is 96 Bytes */
#define ETH_DMAOMR_RTHC_128BYTES                    ((uint32_t)0x00000018)        /* threshold level is 128 Bytes */
#define ETH_DMA_CTLR_OSF                            ((uint32_t)0x00000004)        /* operate on second frame */
#define ETH_DMA_CTLR_SRE                            ((uint32_t)0x00000002)        /* Start/stop receive */
  
/* Bit definition for Ethernet DMA Interrupt Enable Register */
#define ETH_DMA_IER_NISEN                           ((uint32_t)0x00010000)        /* Normal interrupt summary enable */
#define ETH_DMA_IER_AISEN                           ((uint32_t)0x00008000)        /* Abnormal interrupt summary enable */
#define ETH_DMA_IER_ERIEN                           ((uint32_t)0x00004000)        /* Early receive interrupt enable */
#define ETH_DMA_IER_FBEIEN                          ((uint32_t)0x00002000)        /* Fatal bus error interrupt enable */
#define ETH_DMA_IER_ETIEN                           ((uint32_t)0x00000400)        /* Early transmit interrupt enable */
#define ETH_DMA_IER_RWTIEN                          ((uint32_t)0x00000200)        /* Receive watchdog timeout interrupt enable */
#define ETH_DMA_IER_RPSIEN                          ((uint32_t)0x00000100)        /* Receive process stopped interrupt enable */
#define ETH_DMA_IER_RBUIEN                          ((uint32_t)0x00000080)        /* Receive buffer unavailable interrupt enable */
#define ETH_DMA_IER_RIEN                            ((uint32_t)0x00000040)        /* Receive interrupt enable */
#define ETH_DMA_IER_TUIEN                           ((uint32_t)0x00000020)        /* Transmit Underflow interrupt enable */
#define ETH_DMA_IER_ROIEN                           ((uint32_t)0x00000010)        /* Receive Overflow interrupt enable */
#define ETH_DMA_IER_TJTIEN                          ((uint32_t)0x00000008)        /* Transmit jabber timeout interrupt enable */
#define ETH_DMA_IER_TBUIEN                          ((uint32_t)0x00000004)        /* Transmit buffer unavailable interrupt enable */
#define ETH_DMA_IER_TPSIEN                          ((uint32_t)0x00000002)        /* Transmit process stopped interrupt enable */
#define ETH_DMA_IER_TIEN                            ((uint32_t)0x00000001)        /* Transmit interrupt enable */
  
/* Bit definition for Ethernet DMA Missed Frame and Buffer Overflow Counter Register */
#define ETH_DMA_MFBOCNT_OBFOC                       ((uint32_t)0x10000000)        /* Overflow bit for FIFO overflow counter */
#define ETH_DMA_MFBOCNT_MSFA                        ((uint32_t)0x0FFE0000)        /* Missed frames by the application */
#define ETH_DMA_MFBOCNT_OBMFC                       ((uint32_t)0x00010000)        /* Overflow bit for missed frame counter */
#define ETH_DMA_MFBOCNT_MSFC                        ((uint32_t)0x0000FFFF)        /* missed frames by the controller */
  									   
/* Bit definition for Ethernet DMA Current Transmit Descriptor Address Register */
#define ETH_DMA_CTDAR_TDAP                          ((uint32_t)0xFFFFFFFF)        /* Transmit descriptor address pointer */
  
/* Bit definition for Ethernet DMA Current Receive Descriptor Address Register */
#define ETH_DMA_CRDAR_RDAP                          ((uint32_t)0xFFFFFFFF)        /* Receive descriptor address pointer */
  
/* Bit definition for Ethernet DMA Current Transmit Buffer Address Register */
#define ETH_DMA_CTBAR_TBAP                          ((uint32_t)0xFFFFFFFF)        /* Transmit buffer address pointer */
  
/* Bit definition for Ethernet DMA Current Receive Buffer Address Register */
#define ETH_DMA_CRBAR_RBAP                          ((uint32_t)0xFFFFFFFF)        /* Receive buffer address pointer */

/******************************************************************************/
/*                                                                            */
/*                       External Memory Controller                           */
/*                                                                            */
/******************************************************************************/
/******************  Bit definition for EXMC_SCTLR1 register  *******************/
#define  EXMC_SCTLR1_BAKEN                          ((uint32_t)0x00000001)        /*!< Memory bank enable */
#define  EXMC_SCTLR1_MULEN                          ((uint32_t)0x00000002)        /*!< Address/data multiplexing enable */
       
#define  EXMC_SCTLR1_EMTYP                          ((uint32_t)0x0000000C)        /*!< EMTYP[1:0] bits (Memory type) */
#define  EXMC_SCTLR1_EMTYP_0                        ((uint32_t)0x00000004)        /*!< Bit 0 */
#define  EXMC_SCTLR1_EMTYP_1                        ((uint32_t)0x00000008)        /*!< Bit 1 */
       
#define  EXMC_SCTLR1_EMWID                          ((uint32_t)0x00000030)        /*!< EMWID[1:0] bits (Memory data bus width) */
#define  EXMC_SCTLR1_EMWID_0                        ((uint32_t)0x00000010)        /*!< Bit 0 */
#define  EXMC_SCTLR1_EMWID_1                        ((uint32_t)0x00000020)        /*!< Bit 1 */
       
#define  EXMC_SCTLR1_NOREN                          ((uint32_t)0x00000040)        /*!< NOR Flash access enable */
#define  EXMC_SCTLR1_BMODEN                         ((uint32_t)0x00000100)        /*!< Synchronous burst mode enable */
#define  EXMC_SCTLR1_WSIGP                          ((uint32_t)0x00000200)        /*!< NWAIT signal polarity bit */
#define  EXMC_SCTLR1_WRAPEN                         ((uint32_t)0x00000400)        /*!< Wrapped burst mode enable */
#define  EXMC_SCTLR1_WSIGCFG                        ((uint32_t)0x00000800)        /*!< NWAIT signal timing configuration */
#define  EXMC_SCTLR1_WREN                           ((uint32_t)0x00001000)        /*!< Write enable */
#define  EXMC_SCTLR1_WSIGEN                         ((uint32_t)0x00002000)        /*!< NWAIT signal enable */
#define  EXMC_SCTLR1_EXMODEN                        ((uint32_t)0x00004000)        /*!< Extended mode enable */
#define  EXMC_SCTLR1_ASYNCWAIT                      ((uint32_t)0x00008000)        /*!< Asynchronous wait feature enable */
#define  EXMC_SCTLR1_WRMOD                          ((uint32_t)0x00080000)        /*!< write mode select */
       
/******************  Bit definition for EXMC_SCTLR2 register  *******************/
#define  EXMC_SCTLR2_BAKEN                          ((uint32_t)0x00000001)        /*!< Memory bank enable */
#define  EXMC_SCTLR2_MULEN                          ((uint32_t)0x00000002)        /*!< Address/data multiplexing enable */

#define  EXMC_SCTLR2_EMTYP                          ((uint32_t)0x0000000C)        /*!< EMTYP[1:0] bits (Memory type) */
#define  EXMC_SCTLR2_EMTYP_0                        ((uint32_t)0x00000004)        /*!< Bit 0 */
#define  EXMC_SCTLR2_EMTYP_1                        ((uint32_t)0x00000008)        /*!< Bit 1 */
       
#define  EXMC_SCTLR2_EMWID                          ((uint32_t)0x00000030)        /*!< EMWID[1:0] bits (Memory data bus width) */
#define  EXMC_SCTLR2_EMWID_0                        ((uint32_t)0x00000010)        /*!< Bit 0 */
#define  EXMC_SCTLR2_EMWID_1                        ((uint32_t)0x00000020)        /*!< Bit 1 */
       
#define  EXMC_SCTLR2_NOREN                          ((uint32_t)0x00000040)        /*!< NOR Flash access enable */
#define  EXMC_SCTLR2_BMODEN                         ((uint32_t)0x00000100)        /*!< Synchronous burst mode enable */
#define  EXMC_SCTLR2_WSIGP                          ((uint32_t)0x00000200)        /*!< NWAIT signal polarity */
#define  EXMC_SCTLR2_WRAPEN                         ((uint32_t)0x00000400)        /*!< Wrapped burst mode enable */
#define  EXMC_SCTLR2_WSIGCFG                        ((uint32_t)0x00000800)        /*!< NWAIT signal timing configuration */
#define  EXMC_SCTLR2_WREN                           ((uint32_t)0x00001000)        /*!< Write enable */
#define  EXMC_SCTLR2_WSIGEN                         ((uint32_t)0x00002000)        /*!< NWAIT signal enable */
#define  EXMC_SCTLR2_EXMODEN                        ((uint32_t)0x00004000)        /*!< Extended mode enable */
#define  EXMC_SCTLR2_ASYNCWAIT                      ((uint32_t)0x00008000)        /*!< Asynchronous wait feature enable */
#define  EXMC_SCTLR2_WRMOD                          ((uint32_t)0x00080000)        /*!< write mode select */
       
/******************  Bit definition for EXMC_SCTLR3 register  *******************/
#define  EXMC_SCTLR3_BAKEN                          ((uint32_t)0x00000001)        /*!< Memory bank enable */
#define  EXMC_SCTLR3_MULEN                          ((uint32_t)0x00000002)        /*!< Address/data multiplexing enable */
       
#define  EXMC_SCTLR3_EMTYP                          ((uint32_t)0x0000000C)        /*!< EMTYP[1:0] bits (Memory type) */
#define  EXMC_SCTLR3_EMTYP_0                        ((uint32_t)0x00000004)        /*!< Bit 0 */
#define  EXMC_SCTLR3_EMTYP_1                        ((uint32_t)0x00000008)        /*!< Bit 1 */
       
#define  EXMC_SCTLR3_EMWID                          ((uint32_t)0x00000030)        /*!< EMWID[1:0] bits (Memory data bus width) */
#define  EXMC_SCTLR3_EMWID_0                        ((uint32_t)0x00000010)        /*!< Bit 0 */
#define  EXMC_SCTLR3_EMWID_1                        ((uint32_t)0x00000020)        /*!< Bit 1 */
       
#define  EXMC_SCTLR3_NOREN                          ((uint32_t)0x00000040)        /*!< NOR Flash access enable */
#define  EXMC_SCTLR3_BMODEN                         ((uint32_t)0x00000100)        /*!< Synchronous burst mode enable */
#define  EXMC_SCTLR3_WSIGP                          ((uint32_t)0x00000200)        /*!< NWAIT signal polarity */
#define  EXMC_SCTLR3_WRAPEN                         ((uint32_t)0x00000400)        /*!< Wrapped burst mode enable */
#define  EXMC_SCTLR3_WSIGCFG                        ((uint32_t)0x00000800)        /*!< NWAIT signal timing configuration */
#define  EXMC_SCTLR3_WREN                           ((uint32_t)0x00001000)        /*!< Write enable */
#define  EXMC_SCTLR3_WSIGEN                         ((uint32_t)0x00002000)        /*!< NWAIT signal enable */
#define  EXMC_SCTLR3_EXMODEN                        ((uint32_t)0x00004000)        /*!< Extended mode enable */
#define  EXMC_SCTLR3_ASYNCWAIT                      ((uint32_t)0x00008000)        /*!< Asynchronous wait feature enable */
#define  EXMC_SCTLR3_WRMOD                          ((uint32_t)0x00080000)        /*!< write mode select */
       
/******************  Bit definition for EXMC_SCTLR4 register  *******************/
#define  EXMC_SCTLR4_BAKEN                          ((uint32_t)0x00000001)        /*!< Memory bank enable */
#define  EXMC_SCTLR4_MULEN                          ((uint32_t)0x00000002)        /*!< Address/data multiplexing enable */
       
#define  EXMC_SCTLR4_EMTYP                          ((uint32_t)0x0000000C)        /*!< EMTYP[1:0] bits (Memory type) */
#define  EXMC_SCTLR4_EMTYP_0                        ((uint32_t)0x00000004)        /*!< Bit 0 */
#define  EXMC_SCTLR4_EMTYP_1                        ((uint32_t)0x00000008)        /*!< Bit 1 */
       
#define  EXMC_SCTLR4_EMWID                          ((uint32_t)0x00000030)        /*!< EMWID[1:0] bits (Memory data bus width) */
#define  EXMC_SCTLR4_EMWID_0                        ((uint32_t)0x00000010)        /*!< Bit 0 */
#define  EXMC_SCTLR4_EMWID_1                        ((uint32_t)0x00000020)        /*!< Bit 1 */
       
#define  EXMC_SCTLR4_NOREN                          ((uint32_t)0x00000040)        /*!< NOR Flash access enable */
#define  EXMC_SCTLR4_BMODEN                         ((uint32_t)0x00000100)        /*!< Synchronous burst mode enable */
#define  EXMC_SCTLR4_WSIGP                          ((uint32_t)0x00000200)        /*!< NWAIT signal polarity */
#define  EXMC_SCTLR4_WRAPEN                         ((uint32_t)0x00000400)        /*!< Wrapped burst mode enable */
#define  EXMC_SCTLR4_WSIGCFG                        ((uint32_t)0x00000800)        /*!< NWAIT signal timing configuration */
#define  EXMC_SCTLR4_WREN                           ((uint32_t)0x00001000)        /*!< Write enable */
#define  EXMC_SCTLR4_WSIGEN                         ((uint32_t)0x00002000)        /*!< NWAIT signal enable */
#define  EXMC_SCTLR4_EXMODEN                        ((uint32_t)0x00004000)        /*!< Extended mode enable */
#define  EXMC_SCTLR4_ASYNCWAIT                      ((uint32_t)0x00008000)        /*!< Asynchronous wait feature enable */
#define  EXMC_SCTLR4_WRMOD                          ((uint32_t)0x00080000)        /*!< write mode select */
       
/******************  Bit definition for EXMC_STR1 register  ******************/
#define  EXMC_STR1_AST                              ((uint32_t)0x0000000F)        /*!< AST[3:0] bits (Address setup time) */
#define  EXMC_STR1_AST_0                            ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  EXMC_STR1_AST_1                            ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  EXMC_STR1_AST_2                            ((uint32_t)0x00000004)        /*!< Bit 2 */
#define  EXMC_STR1_AST_3                            ((uint32_t)0x00000008)        /*!< Bit 3 */
       
#define  EXMC_STR1_AHT                              ((uint32_t)0x000000F0)        /*!< AHT[3:0] bits (Address hold time) */
#define  EXMC_STR1_AHT_0                            ((uint32_t)0x00000010)        /*!< Bit 0 */
#define  EXMC_STR1_AHT_1                            ((uint32_t)0x00000020)        /*!< Bit 1 */
#define  EXMC_STR1_AHT_2                            ((uint32_t)0x00000040)        /*!< Bit 2 */
#define  EXMC_STR1_AHT_3                            ((uint32_t)0x00000080)        /*!< Bit 3 */
       
#define  EXMC_STR1_DST                              ((uint32_t)0x0000FF00)        /*!< DST [3:0] bits (Data setup time) */
#define  EXMC_STR1_DST_0                            ((uint32_t)0x00000100)        /*!< Bit 0 */
#define  EXMC_STR1_DST_1                            ((uint32_t)0x00000200)        /*!< Bit 1 */
#define  EXMC_STR1_DST_2                            ((uint32_t)0x00000400)        /*!< Bit 2 */
#define  EXMC_STR1_DST_3                            ((uint32_t)0x00000800)        /*!< Bit 3 */
       
#define  EXMC_STR1_BUSLAT                           ((uint32_t)0x000F0000)        /*!< BUSLAT[3:0] bits (Bus latency) */
#define  EXMC_STR1_BUSLAT_0                         ((uint32_t)0x00010000)        /*!< Bit 0 */
#define  EXMC_STR1_BUSLAT_1                         ((uint32_t)0x00020000)        /*!< Bit 1 */
#define  EXMC_STR1_BUSLAT_2                         ((uint32_t)0x00040000)        /*!< Bit 2 */
#define  EXMC_STR1_BUSLAT_3                         ((uint32_t)0x00080000)        /*!< Bit 3 */
       
#define  EXMC_STR1_CDIV                             ((uint32_t)0x00F00000)        /*!< CDIV[3:0] bits (Synchronous clock divide ratio) */
#define  EXMC_STR1_CDIV_0                           ((uint32_t)0x00100000)        /*!< Bit 0 */
#define  EXMC_STR1_CDIV_1                           ((uint32_t)0x00200000)        /*!< Bit 1 */
#define  EXMC_STR1_CDIV_2                           ((uint32_t)0x00400000)        /*!< Bit 2 */
#define  EXMC_STR1_CDIV_3                           ((uint32_t)0x00800000)        /*!< Bit 3 */
       
#define  EXMC_STR1_DLAT                             ((uint32_t)0x0F000000)        /*!< DLAT[3:0] bits (Data latency for nor flash) */
#define  EXMC_STR1_DLAT_0                           ((uint32_t)0x01000000)        /*!< Bit 0 */
#define  EXMC_STR1_DLAT_1                           ((uint32_t)0x02000000)        /*!< Bit 1 */
#define  EXMC_STR1_DLAT_2                           ((uint32_t)0x04000000)        /*!< Bit 2 */
#define  EXMC_STR1_DLAT_3                           ((uint32_t)0x08000000)        /*!< Bit 3 */
       
#define  EXMC_STR1_ASYNMOD                          ((uint32_t)0x30000000)        /*!< ASYNMOD[1:0] bits (Asynchronous access mode) */
#define  EXMC_STR1_ASYNMOD_0                        ((uint32_t)0x10000000)        /*!< Bit 0 */
#define  EXMC_STR1_ASYNMOD_1                        ((uint32_t)0x20000000)        /*!< Bit 1 */
       
/******************  Bit definition for EXMC_STR2 register  *******************/
#define  EXMC_STR2_AST                              ((uint32_t)0x0000000F)        /*!< AST[3:0] bits (Address setup time) */
#define  EXMC_STR2_AST_0                            ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  EXMC_STR2_AST_1                            ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  EXMC_STR2_AST_2                            ((uint32_t)0x00000004)        /*!< Bit 2 */
#define  EXMC_STR2_AST_3                            ((uint32_t)0x00000008)        /*!< Bit 3 */
       
#define  EXMC_STR2_AHT                              ((uint32_t)0x000000F0)        /*!< AHT[3:0] bits (Address hold time) */
#define  EXMC_STR2_AHT_0                            ((uint32_t)0x00000010)        /*!< Bit 0 */
#define  EXMC_STR2_AHT_1                            ((uint32_t)0x00000020)        /*!< Bit 1 */
#define  EXMC_STR2_AHT_2                            ((uint32_t)0x00000040)        /*!< Bit 2 */
#define  EXMC_STR2_AHT_3                            ((uint32_t)0x00000080)        /*!< Bit 3 */
       
#define  EXMC_STR2_DST                              ((uint32_t)0x0000FF00)        /*!< DST [3:0] bits (Data setup time) */
#define  EXMC_STR2_DST_0                            ((uint32_t)0x00000100)        /*!< Bit 0 */
#define  EXMC_STR2_DST_1                            ((uint32_t)0x00000200)        /*!< Bit 1 */
#define  EXMC_STR2_DST_2                            ((uint32_t)0x00000400)        /*!< Bit 2 */
#define  EXMC_STR2_DST_3                            ((uint32_t)0x00000800)        /*!< Bit 3 */
       
#define  EXMC_STR2_BUSLAT                           ((uint32_t)0x000F0000)        /*!< BUSLAT[3:0] bits (Bus latency) */
#define  EXMC_STR2_BUSLAT_0                         ((uint32_t)0x00010000)        /*!< Bit 0 */
#define  EXMC_STR2_BUSLAT_1                         ((uint32_t)0x00020000)        /*!< Bit 1 */
#define  EXMC_STR2_BUSLAT_2                         ((uint32_t)0x00040000)        /*!< Bit 2 */
#define  EXMC_STR2_BUSLAT_3                         ((uint32_t)0x00080000)        /*!< Bit 3 */
       
#define  EXMC_STR2_CDIV                             ((uint32_t)0x00F00000)        /*!< CDIV[3:0] bits (Synchronous clock divide ratio) */
#define  EXMC_STR2_CDIV_0                           ((uint32_t)0x00100000)        /*!< Bit 0 */
#define  EXMC_STR2_CDIV_1                           ((uint32_t)0x00200000)        /*!< Bit 1 */
#define  EXMC_STR2_CDIV_2                           ((uint32_t)0x00400000)        /*!< Bit 2 */
#define  EXMC_STR2_CDIV_3                           ((uint32_t)0x00800000)        /*!< Bit 3 */
       
#define  EXMC_STR2_DLAT                             ((uint32_t)0x0F000000)        /*!< DLAT[3:0] bits (Data latency for nor flash) */
#define  EXMC_STR2_DLAT_0                           ((uint32_t)0x01000000)        /*!< Bit 0 */
#define  EXMC_STR2_DLAT_1                           ((uint32_t)0x02000000)        /*!< Bit 1 */
#define  EXMC_STR2_DLAT_2                           ((uint32_t)0x04000000)        /*!< Bit 2 */
#define  EXMC_STR2_DLAT_3                           ((uint32_t)0x08000000)        /*!< Bit 3 */
       
#define  EXMC_STR2_ASYNMOD                          ((uint32_t)0x30000000)        /*!< ASYNMOD[1:0] bits (Asynchronous access mode) */
#define  EXMC_STR2_ASYNMOD_0                        ((uint32_t)0x10000000)        /*!< Bit 0 */
#define  EXMC_STR2_ASYNMOD_1                        ((uint32_t)0x20000000)        /*!< Bit 1 */
       
/*******************  Bit definition for EXMC_STR3 register  *******************/
#define  EXMC_STR3_AST                              ((uint32_t)0x0000000F)        /*!< AST[3:0] bits (Address setup time) */
#define  EXMC_STR3_AST_0                            ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  EXMC_STR3_AST_1                            ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  EXMC_STR3_AST_2                            ((uint32_t)0x00000004)        /*!< Bit 2 */
#define  EXMC_STR3_AST_3                            ((uint32_t)0x00000008)        /*!< Bit 3 */
       
#define  EXMC_STR3_AHT                              ((uint32_t)0x000000F0)        /*!< AHT[3:0] bits (Address hold time) */
#define  EXMC_STR3_AHT_0                            ((uint32_t)0x00000010)        /*!< Bit 0 */
#define  EXMC_STR3_AHT_1                            ((uint32_t)0x00000020)        /*!< Bit 1 */
#define  EXMC_STR3_AHT_2                            ((uint32_t)0x00000040)        /*!< Bit 2 */
#define  EXMC_STR3_AHT_3                            ((uint32_t)0x00000080)        /*!< Bit 3 */
       
#define  EXMC_STR3_DST                              ((uint32_t)0x0000FF00)        /*!< DST [3:0] bits (Data setup time) */
#define  EXMC_STR3_DST_0                            ((uint32_t)0x00000100)        /*!< Bit 0 */
#define  EXMC_STR3_DST_1                            ((uint32_t)0x00000200)        /*!< Bit 1 */
#define  EXMC_STR3_DST_2                            ((uint32_t)0x00000400)        /*!< Bit 2 */
#define  EXMC_STR3_DST_3                            ((uint32_t)0x00000800)        /*!< Bit 3 */
       
#define  EXMC_STR3_BUSLAT                           ((uint32_t)0x000F0000)        /*!< BUSLAT[3:0] bits (Bus latency) */
#define  EXMC_STR3_BUSLAT_0                         ((uint32_t)0x00010000)        /*!< Bit 0 */
#define  EXMC_STR3_BUSLAT_1                         ((uint32_t)0x00020000)        /*!< Bit 1 */
#define  EXMC_STR3_BUSLAT_2                         ((uint32_t)0x00040000)        /*!< Bit 2 */
#define  EXMC_STR3_BUSLAT_3                         ((uint32_t)0x00080000)        /*!< Bit 3 */
       
#define  EXMC_STR3_CDIV                             ((uint32_t)0x00F00000)        /*!< CDIV[3:0] bits (Synchronous clock divide ratio) */
#define  EXMC_STR3_CDIV_0                           ((uint32_t)0x00100000)        /*!< Bit 0 */
#define  EXMC_STR3_CDIV_1                           ((uint32_t)0x00200000)        /*!< Bit 1 */
#define  EXMC_STR3_CDIV_2                           ((uint32_t)0x00400000)        /*!< Bit 2 */
#define  EXMC_STR3_CDIV_3                           ((uint32_t)0x00800000)        /*!< Bit 3 */
       
#define  EXMC_STR3_DLAT                             ((uint32_t)0x0F000000)        /*!< DLAT[3:0] bits (Data latency for nor flash) */
#define  EXMC_STR3_DLAT_0                           ((uint32_t)0x01000000)        /*!< Bit 0 */
#define  EXMC_STR3_DLAT_1                           ((uint32_t)0x02000000)        /*!< Bit 1 */
#define  EXMC_STR3_DLAT_2                           ((uint32_t)0x04000000)        /*!< Bit 2 */
#define  EXMC_STR3_DLAT_3                           ((uint32_t)0x08000000)        /*!< Bit 3 */
       
#define  EXMC_STR3_ASYNMOD                          ((uint32_t)0x30000000)        /*!< ASYNMOD[1:0] bits (Asynchronous access mode) */
#define  EXMC_STR3_ASYNMOD_0                        ((uint32_t)0x10000000)        /*!< Bit 0 */
#define  EXMC_STR3_ASYNMOD_1                        ((uint32_t)0x20000000)        /*!< Bit 1 */
       
/******************  Bit definition for EXMC_STR4 register  *******************/
#define  EXMC_STR4_AST                              ((uint32_t)0x0000000F)        /*!< AST[3:0] bits (Address setup time) */
#define  EXMC_STR4_AST_0                            ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  EXMC_STR4_AST_1                            ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  EXMC_STR4_AST_2                            ((uint32_t)0x00000004)        /*!< Bit 2 */
#define  EXMC_STR4_AST_3                            ((uint32_t)0x00000008)        /*!< Bit 3 */
       
#define  EXMC_STR4_AHT                              ((uint32_t)0x000000F0)        /*!< AHT[3:0] bits (Address hold time) */
#define  EXMC_STR4_AHT_0                            ((uint32_t)0x00000010)        /*!< Bit 0 */
#define  EXMC_STR4_AHT_1                            ((uint32_t)0x00000020)        /*!< Bit 1 */
#define  EXMC_STR4_AHT_2                            ((uint32_t)0x00000040)        /*!< Bit 2 */
#define  EXMC_STR4_AHT_3                            ((uint32_t)0x00000080)        /*!< Bit 3 */
       
#define  EXMC_STR4_DST                              ((uint32_t)0x0000FF00)        /*!< DST [3:0] bits (Data setup time) */
#define  EXMC_STR4_DST_0                            ((uint32_t)0x00000100)        /*!< Bit 0 */
#define  EXMC_STR4_DST_1                            ((uint32_t)0x00000200)        /*!< Bit 1 */
#define  EXMC_STR4_DST_2                            ((uint32_t)0x00000400)        /*!< Bit 2 */
#define  EXMC_STR4_DST_3                            ((uint32_t)0x00000800)        /*!< Bit 3 */
       
#define  EXMC_STR4_BUSLAT                           ((uint32_t)0x000F0000)        /*!< BUSLAT[3:0] bits (Bus latency) */
#define  EXMC_STR4_BUSLAT_0                         ((uint32_t)0x00010000)        /*!< Bit 0 */
#define  EXMC_STR4_BUSLAT_1                         ((uint32_t)0x00020000)        /*!< Bit 1 */
#define  EXMC_STR4_BUSLAT_2                         ((uint32_t)0x00040000)        /*!< Bit 2 */
#define  EXMC_STR4_BUSLAT_3                         ((uint32_t)0x00080000)        /*!< Bit 3 */

#define  EXMC_STR4_CDIV                             ((uint32_t)0x00F00000)        /*!< CDIV[3:0] bits (Synchronous clock divide ratio) */
#define  EXMC_STR4_CDIV_0                           ((uint32_t)0x00100000)        /*!< Bit 0 */
#define  EXMC_STR4_CDIV_1                           ((uint32_t)0x00200000)        /*!< Bit 1 */
#define  EXMC_STR4_CDIV_2                           ((uint32_t)0x00400000)        /*!< Bit 2 */
#define  EXMC_STR4_CDIV_3                           ((uint32_t)0x00800000)        /*!< Bit 3 */
       
#define  EXMC_STR4_DLAT                             ((uint32_t)0x0F000000)        /*!< DLAT[3:0] bits (Data latency for nor flash) */
#define  EXMC_STR4_DLAT_0                           ((uint32_t)0x01000000)        /*!< Bit 0 */
#define  EXMC_STR4_DLAT_1                           ((uint32_t)0x02000000)        /*!< Bit 1 */
#define  EXMC_STR4_DLAT_2                           ((uint32_t)0x04000000)        /*!< Bit 2 */
#define  EXMC_STR4_DLAT_3                           ((uint32_t)0x08000000)        /*!< Bit 3 */
       
#define  EXMC_STR4_ASYNMOD                          ((uint32_t)0x30000000)        /*!< ASYNMOD[1:0] bits (Asynchronous access mode) */
#define  EXMC_STR4_ASYNMOD_0                        ((uint32_t)0x10000000)        /*!< Bit 0 */
#define  EXMC_STR4_ASYNMOD_1                        ((uint32_t)0x20000000)        /*!< Bit 1 */
       
/******************  Bit definition for EXMC_SWTR1 register  ******************/
#define  EXMC_SWTR1_AST                             ((uint32_t)0x0000000F)        /*!< AST[3:0] bits (Address setup time) */
#define  EXMC_SWTR1_AST_0                           ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  EXMC_SWTR1_AST_1                           ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  EXMC_SWTR1_AST_2                           ((uint32_t)0x00000004)        /*!< Bit 2 */
#define  EXMC_SWTR1_AST_3                           ((uint32_t)0x00000008)        /*!< Bit 3 */
       
#define  EXMC_SWTR1_AHT                             ((uint32_t)0x000000F0)        /*!< AHT[3:0] bits (Address hold time) */
#define  EXMC_SWTR1_AHT_0                           ((uint32_t)0x00000010)        /*!< Bit 0 */
#define  EXMC_SWTR1_AHT_1                           ((uint32_t)0x00000020)        /*!< Bit 1 */
#define  EXMC_SWTR1_AHT_2                           ((uint32_t)0x00000040)        /*!< Bit 2 */
#define  EXMC_SWTR1_AHT_3                           ((uint32_t)0x00000080)        /*!< Bit 3 */
       
#define  EXMC_SWTR1_DST                             ((uint32_t)0x0000FF00)        /*!< DST [3:0] bits (Data setup time) */
#define  EXMC_SWTR1_DST_0                           ((uint32_t)0x00000100)        /*!< Bit 0 */
#define  EXMC_SWTR1_DST_1                           ((uint32_t)0x00000200)        /*!< Bit 1 */
#define  EXMC_SWTR1_DST_2                           ((uint32_t)0x00000400)        /*!< Bit 2 */
#define  EXMC_SWTR1_DST_3                           ((uint32_t)0x00000800)        /*!< Bit 3 */
       
#define  EXMC_SWTR1_CDIV                            ((uint32_t)0x00F00000)        /*!< CDIV[3:0] bits (Synchronous clock divide ratio) */
#define  EXMC_SWTR1_CDIV_0                          ((uint32_t)0x00100000)        /*!< Bit 0 */
#define  EXMC_SWTR1_CDIV_1                          ((uint32_t)0x00200000)        /*!< Bit 1 */
#define  EXMC_SWTR1_CDIV_2                          ((uint32_t)0x00400000)        /*!< Bit 2 */
#define  EXMC_SWTR1_CDIV_3                          ((uint32_t)0x00800000)        /*!< Bit 3 */
       
#define  EXMC_SWTR1_DLAT                            ((uint32_t)0x0F000000)        /*!< DLAT[3:0] bits (Data latency for nor flash) */
#define  EXMC_SWTR1_DLAT_0                          ((uint32_t)0x01000000)        /*!< Bit 0 */
#define  EXMC_SWTR1_DLAT_1                          ((uint32_t)0x02000000)        /*!< Bit 1 */
#define  EXMC_SWTR1_DLAT_2                          ((uint32_t)0x04000000)        /*!< Bit 2 */
#define  EXMC_SWTR1_DLAT_3                          ((uint32_t)0x08000000)        /*!< Bit 3 */

#define  EXMC_SWTR1_ASYNMOD                         ((uint32_t)0x30000000)        /*!< ASYNMOD[1:0] bits (Asynchronous access mode) */
#define  EXMC_SWTR1_ASYNMOD_0                       ((uint32_t)0x10000000)        /*!< Bit 0 */
#define  EXMC_SWTR1_ASYNMOD_1                       ((uint32_t)0x20000000)        /*!< Bit 1 */
       
/******************  Bit definition for EXMC_SWTR2 register  ******************/
#define  EXMC_SWTR2_AST                             ((uint32_t)0x0000000F)        /*!< AST[3:0] bits (Address setup time) */
#define  EXMC_SWTR2_AST_0                           ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  EXMC_SWTR2_AST_1                           ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  EXMC_SWTR2_AST_2                           ((uint32_t)0x00000004)        /*!< Bit 2 */
#define  EXMC_SWTR2_AST_3                           ((uint32_t)0x00000008)        /*!< Bit 3 */
       
#define  EXMC_SWTR2_AHT                             ((uint32_t)0x000000F0)        /*!< AHT[3:0] bits (Address hold time) */
#define  EXMC_SWTR2_AHT_0                           ((uint32_t)0x00000010)        /*!< Bit 0 */
#define  EXMC_SWTR2_AHT_1                           ((uint32_t)0x00000020)        /*!< Bit 1 */
#define  EXMC_SWTR2_AHT_2                           ((uint32_t)0x00000040)        /*!< Bit 2 */
#define  EXMC_SWTR2_AHT_3                           ((uint32_t)0x00000080)        /*!< Bit 3 */
       
#define  EXMC_SWTR2_DST                             ((uint32_t)0x0000FF00)        /*!< DST [3:0] bits (Data setup time) */
#define  EXMC_SWTR2_DST_0                           ((uint32_t)0x00000100)        /*!< Bit 0 */
#define  EXMC_SWTR2_DST_1                           ((uint32_t)0x00000200)        /*!< Bit 1 */
#define  EXMC_SWTR2_DST_2                           ((uint32_t)0x00000400)        /*!< Bit 2 */
#define  EXMC_SWTR2_DST_3                           ((uint32_t)0x00000800)        /*!< Bit 3 */
       
#define  EXMC_SWTR2_CDIV                            ((uint32_t)0x00F00000)        /*!< CDIV[3:0] bits (Synchronous clock divide ratio) */
#define  EXMC_SWTR2_CDIV_0                          ((uint32_t)0x00100000)        /*!< Bit 0 */
#define  EXMC_SWTR2_CDIV_1                          ((uint32_t)0x00200000)        /*!< Bit 1 */
#define  EXMC_SWTR2_CDIV_2                          ((uint32_t)0x00400000)        /*!< Bit 2 */
#define  EXMC_SWTR2_CDIV_3                          ((uint32_t)0x00800000)        /*!< Bit 3 */
       
#define  EXMC_SWTR2_DLAT                            ((uint32_t)0x0F000000)        /*!< DLAT[3:0] bits (Data latency for nor flash) */
#define  EXMC_SWTR2_DLAT_0                          ((uint32_t)0x01000000)        /*!< Bit 0 */
#define  EXMC_SWTR2_DLAT_1                          ((uint32_t)0x02000000)        /*!< Bit 1 */
#define  EXMC_SWTR2_DLAT_2                          ((uint32_t)0x04000000)        /*!< Bit 2 */
#define  EXMC_SWTR2_DLAT_3                          ((uint32_t)0x08000000)        /*!< Bit 3 */
       
#define  EXMC_SWTR2_ASYNMOD                         ((uint32_t)0x30000000)        /*!< ASYNMOD[1:0] bits (Asynchronous access mode) */
#define  EXMC_SWTR2_ASYNMOD_0                       ((uint32_t)0x10000000)        /*!< Bit 0 */
#define  EXMC_SWTR2_ASYNMOD_1                       ((uint32_t)0x20000000)        /*!< Bit 1 */
       
/******************  Bit definition for EXMC_SWTR3 register  ******************/
#define  EXMC_SWTR3_AST                             ((uint32_t)0x0000000F)        /*!< AST[3:0] bits (Address setup time) */
#define  EXMC_SWTR3_AST_0                           ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  EXMC_SWTR3_AST_1                           ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  EXMC_SWTR3_AST_2                           ((uint32_t)0x00000004)        /*!< Bit 2 */
#define  EXMC_SWTR3_AST_3                           ((uint32_t)0x00000008)        /*!< Bit 3 */
       
#define  EXMC_SWTR3_AHT                             ((uint32_t)0x000000F0)        /*!< AHT[3:0] bits (Address hold time) */
#define  EXMC_SWTR3_AHT_0                           ((uint32_t)0x00000010)        /*!< Bit 0 */
#define  EXMC_SWTR3_AHT_1                           ((uint32_t)0x00000020)        /*!< Bit 1 */
#define  EXMC_SWTR3_AHT_2                           ((uint32_t)0x00000040)        /*!< Bit 2 */
#define  EXMC_SWTR3_AHT_3                           ((uint32_t)0x00000080)        /*!< Bit 3 */
       
#define  EXMC_SWTR3_DST                             ((uint32_t)0x0000FF00)        /*!< DST [3:0] bits (Data setup time) */
#define  EXMC_SWTR3_DST_0                           ((uint32_t)0x00000100)        /*!< Bit 0 */
#define  EXMC_SWTR3_DST_1                           ((uint32_t)0x00000200)        /*!< Bit 1 */
#define  EXMC_SWTR3_DST_2                           ((uint32_t)0x00000400)        /*!< Bit 2 */
#define  EXMC_SWTR3_DST_3                           ((uint32_t)0x00000800)        /*!< Bit 3 */
       
#define  EXMC_SWTR3_CDIV                            ((uint32_t)0x00F00000)        /*!< CDIV[3:0] bits (Synchronous clock divide ratio) */
#define  EXMC_SWTR3_CDIV_0                          ((uint32_t)0x00100000)        /*!< Bit 0 */
#define  EXMC_SWTR3_CDIV_1                          ((uint32_t)0x00200000)        /*!< Bit 1 */
#define  EXMC_SWTR3_CDIV_2                          ((uint32_t)0x00400000)        /*!< Bit 2 */
#define  EXMC_SWTR3_CDIV_3                          ((uint32_t)0x00800000)        /*!< Bit 3 */
       
#define  EXMC_SWTR3_DLAT                            ((uint32_t)0x0F000000)        /*!< DLAT[3:0] bits (Data latency for nor flash) */
#define  EXMC_SWTR3_DLAT_0                          ((uint32_t)0x01000000)        /*!< Bit 0 */
#define  EXMC_SWTR3_DLAT_1                          ((uint32_t)0x02000000)        /*!< Bit 1 */
#define  EXMC_SWTR3_DLAT_2                          ((uint32_t)0x04000000)        /*!< Bit 2 */
#define  EXMC_SWTR3_DLAT_3                          ((uint32_t)0x08000000)        /*!< Bit 3 */
       
#define  EXMC_SWTR3_ASYNMOD                         ((uint32_t)0x30000000)        /*!< ASYNMOD[1:0] bits (Asynchronous access mode) */
#define  EXMC_SWTR3_ASYNMOD_0                       ((uint32_t)0x10000000)        /*!< Bit 0 */
#define  EXMC_SWTR3_ASYNMOD_1                       ((uint32_t)0x20000000)        /*!< Bit 1 */
       
/******************  Bit definition for EXMC_SWTR4 register  ******************/
#define  EXMC_SWTR4_AST                             ((uint32_t)0x0000000F)        /*!< AST[3:0] bits (Address setup time) */
#define  EXMC_SWTR4_AST_0                           ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  EXMC_SWTR4_AST_1                           ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  EXMC_SWTR4_AST_2                           ((uint32_t)0x00000004)        /*!< Bit 2 */
#define  EXMC_SWTR4_AST_3                           ((uint32_t)0x00000008)        /*!< Bit 3 */
       
#define  EXMC_SWTR4_AHT                             ((uint32_t)0x000000F0)        /*!< AHT[3:0] bits (Address hold time) */
#define  EXMC_SWTR4_AHT_0                           ((uint32_t)0x00000010)        /*!< Bit 0 */
#define  EXMC_SWTR4_AHT_1                           ((uint32_t)0x00000020)        /*!< Bit 1 */
#define  EXMC_SWTR4_AHT_2                           ((uint32_t)0x00000040)        /*!< Bit 2 */
#define  EXMC_SWTR4_AHT_3                           ((uint32_t)0x00000080)        /*!< Bit 3 */
       
#define  EXMC_SWTR4_DST                             ((uint32_t)0x0000FF00)        /*!< DST [3:0] bits (Data setup time) */
#define  EXMC_SWTR4_DST_0                           ((uint32_t)0x00000100)        /*!< Bit 0 */
#define  EXMC_SWTR4_DST_1                           ((uint32_t)0x00000200)        /*!< Bit 1 */
#define  EXMC_SWTR4_DST_2                           ((uint32_t)0x00000400)        /*!< Bit 2 */
#define  EXMC_SWTR4_DST_3                           ((uint32_t)0x00000800)        /*!< Bit 3 */
       
#define  EXMC_SWTR4_CDIV                            ((uint32_t)0x00F00000)        /*!< CDIV[3:0] bits (Synchronous clock divide ratio) */
#define  EXMC_SWTR4_CDIV_0                          ((uint32_t)0x00100000)        /*!< Bit 0 */
#define  EXMC_SWTR4_CDIV_1                          ((uint32_t)0x00200000)        /*!< Bit 1 */
#define  EXMC_SWTR4_CDIV_2                          ((uint32_t)0x00400000)        /*!< Bit 2 */
#define  EXMC_SWTR4_CDIV_3                          ((uint32_t)0x00800000)        /*!< Bit 3 */
       
#define  EXMC_SWTR4_DLAT                            ((uint32_t)0x0F000000)        /*!< DLAT[3:0] bits (Data latency for nor flash) */
#define  EXMC_SWTR4_DLAT_0                          ((uint32_t)0x01000000)        /*!< Bit 0 */
#define  EXMC_SWTR4_DLAT_1                          ((uint32_t)0x02000000)        /*!< Bit 1 */
#define  EXMC_SWTR4_DLAT_2                          ((uint32_t)0x04000000)        /*!< Bit 2 */
#define  EXMC_SWTR4_DLAT_3                          ((uint32_t)0x08000000)        /*!< Bit 3 */
       
#define  EXMC_SWTR4_ASYNMOD                         ((uint32_t)0x30000000)        /*!< ASYNMOD[1:0] bits (Asynchronous access mode) */
#define  EXMC_SWTR4_ASYNMOD_0                       ((uint32_t)0x10000000)        /*!< Bit 0 */
#define  EXMC_SWTR4_ASYNMOD_1                       ((uint32_t)0x20000000)        /*!< Bit 1 */
       
/******************  Bit definition for EXMC_CTLR2 register  *******************/
#define  EXMC_CTLR2_WSIGEN                          ((uint32_t)0x00000002)        /*!< Wait feature enable */
#define  EXMC_CTLR2_BAKEN                           ((uint32_t)0x00000004)        /*!< Memory bank enable */
#define  EXMC_CTLR2_EMTYP                           ((uint32_t)0x00000008)        /*!< Memory type */
       
#define  EXMC_CTLR2_EMWID                           ((uint32_t)0x00000030)        /*!< EMWID[1:0] bits (external memory databus width) */
#define  EXMC_CTLR2_EMWID_0                         ((uint32_t)0x00000010)        /*!< Bit 0 */
#define  EXMC_CTLR2_EMWID_1                         ((uint32_t)0x00000020)        /*!< Bit 1 */
       
#define  EXMC_CTLR2_ECCEN                           ((uint32_t)0x00000040)        /*!< ECC enable */
       
#define  EXMC_CTLR2_CTR                             ((uint32_t)0x00001E00)        /*!< CTR[3:0] bits (CLE to RE delay) */
#define  EXMC_CTLR2_CTR_0                           ((uint32_t)0x00000200)        /*!< Bit 0 */
#define  EXMC_CTLR2_CTR_1                           ((uint32_t)0x00000400)        /*!< Bit 1 */
#define  EXMC_CTLR2_CTR_2                           ((uint32_t)0x00000800)        /*!< Bit 2 */
#define  EXMC_CTLR2_CTR_3                           ((uint32_t)0x00001000)        /*!< Bit 3 */
       
#define  EXMC_CTLR2_ATR                             ((uint32_t)0x0001E000)        /*!< ATR[3:0] bits (ALE to RE delay) */
#define  EXMC_CTLR2_ATR_0                           ((uint32_t)0x00002000)        /*!< Bit 0 */
#define  EXMC_CTLR2_ATR_1                           ((uint32_t)0x00004000)        /*!< Bit 1 */
#define  EXMC_CTLR2_ATR_2                           ((uint32_t)0x00008000)        /*!< Bit 2 */
#define  EXMC_CTLR2_ATR_3                           ((uint32_t)0x00010000)        /*!< Bit 3 */
       
#define  EXMC_CTLR2_ECCSZ                           ((uint32_t)0x000E0000)        /*!< ECCSZ[2:0] bits (ECC size) */
#define  EXMC_CTLR2_ECCSZ_0                         ((uint32_t)0x00020000)        /*!< Bit 0 */
#define  EXMC_CTLR2_ECCSZ_1                         ((uint32_t)0x00040000)        /*!< Bit 1 */
#define  EXMC_CTLR2_ECCSZ_2                         ((uint32_t)0x00080000)        /*!< Bit 2 */
       
/******************  Bit definition for EXMC_CTLR3 register  *******************/
#define  EXMC_CTLR3_WSIGEN                          ((uint32_t)0x00000002)        /*!< Wait feature enable */
#define  EXMC_CTLR3_BAKEN                           ((uint32_t)0x00000004)        /*!< Memory bank enable */
#define  EXMC_CTLR3_EMTYP                           ((uint32_t)0x00000008)        /*!< Memory type */
       
#define  EXMC_CTLR3_EMWID                           ((uint32_t)0x00000030)        /*!< EMWID[1:0] bits (external memory databus width) */
#define  EXMC_CTLR3_EMWID_0                         ((uint32_t)0x00000010)        /*!< Bit 0 */
#define  EXMC_CTLR3_EMWID_1                         ((uint32_t)0x00000020)        /*!< Bit 1 */
       
#define  EXMC_CTLR3_ECCEN                           ((uint32_t)0x00000040)        /*!< ECC enable */
       
#define  EXMC_CTLR3_CTR                             ((uint32_t)0x00001E00)        /*!< CTR[3:0] bits (CLE to RE delay) */
#define  EXMC_CTLR3_CTR_0                           ((uint32_t)0x00000200)        /*!< Bit 0 */
#define  EXMC_CTLR3_CTR_1                           ((uint32_t)0x00000400)        /*!< Bit 1 */
#define  EXMC_CTLR3_CTR_2                           ((uint32_t)0x00000800)        /*!< Bit 2 */
#define  EXMC_CTLR3_CTR_3                           ((uint32_t)0x00001000)        /*!< Bit 3 */
       
#define  EXMC_CTLR3_ATR                             ((uint32_t)0x0001E000)        /*!< ATR[3:0] bits (ALE to RE delay) */
#define  EXMC_CTLR3_ATR_0                           ((uint32_t)0x00002000)        /*!< Bit 0 */
#define  EXMC_CTLR3_ATR_1                           ((uint32_t)0x00004000)        /*!< Bit 1 */
#define  EXMC_CTLR3_ATR_2                           ((uint32_t)0x00008000)        /*!< Bit 2 */
#define  EXMC_CTLR3_ATR_3                           ((uint32_t)0x00010000)        /*!< Bit 3 */
       
#define  EXMC_CTLR3_ECCSZ                           ((uint32_t)0x000E0000)        /*!< ECCSZ[2:0] bits (ECC size) */
#define  EXMC_CTLR3_ECCSZ_0                         ((uint32_t)0x00020000)        /*!< Bit 0 */
#define  EXMC_CTLR3_ECCSZ_1                         ((uint32_t)0x00040000)        /*!< Bit 1 */
#define  EXMC_CTLR3_ECCSZ_2                         ((uint32_t)0x00080000)        /*!< Bit 2 */
       
/******************  Bit definition for EXMC_CTLR4 register  *******************/
#define  EXMC_CTLR4_WSIGEN                          ((uint32_t)0x00000002)        /*!< Wait feature enable */
#define  EXMC_CTLR4_BAKEN                           ((uint32_t)0x00000004)        /*!< Memory bank enable */
#define  EXMC_CTLR4_EMTYP                           ((uint32_t)0x00000008)        /*!< Memory type */
       
#define  EXMC_CTLR4_EMWID                           ((uint32_t)0x00000030)        /*!< EMWID[1:0] bits (external memory databus width) */
#define  EXMC_CTLR4_EMWID_0                         ((uint32_t)0x00000010)        /*!< Bit 0 */
#define  EXMC_CTLR4_EMWID_1                         ((uint32_t)0x00000020)        /*!< Bit 1 */
       
#define  EXMC_CTLR4_ECCEN                           ((uint32_t)0x00000040)        /*!< ECC enable */
       
#define  EXMC_CTLR4_CTR                             ((uint32_t)0x00001E00)        /*!< CTR[3:0] bits (CLE to RE delay) */
#define  EXMC_CTLR4_CTR_0                           ((uint32_t)0x00000200)        /*!< Bit 0 */
#define  EXMC_CTLR4_CTR_1                           ((uint32_t)0x00000400)        /*!< Bit 1 */
#define  EXMC_CTLR4_CTR_2                           ((uint32_t)0x00000800)        /*!< Bit 2 */
#define  EXMC_CTLR4_CTR_3                           ((uint32_t)0x00001000)        /*!< Bit 3 */
       
#define  EXMC_CTLR4_ATR                             ((uint32_t)0x0001E000)        /*!< ATR[3:0] bits (ALE to RE delay) */
#define  EXMC_CTLR4_ATR_0                           ((uint32_t)0x00002000)        /*!< Bit 0 */
#define  EXMC_CTLR4_ATR_1                           ((uint32_t)0x00004000)        /*!< Bit 1 */
#define  EXMC_CTLR4_ATR_2                           ((uint32_t)0x00008000)        /*!< Bit 2 */
#define  EXMC_CTLR4_ATR_3                           ((uint32_t)0x00010000)        /*!< Bit 3 */
       
#define  EXMC_CTLR4_ECCSZ                           ((uint32_t)0x000E0000)        /*!< ECCSZ[2:0] bits (ECC size) */
#define  EXMC_CTLR4_ECCSZ_0                         ((uint32_t)0x00020000)        /*!< Bit 0 */
#define  EXMC_CTLR4_ECCSZ_1                         ((uint32_t)0x00040000)        /*!< Bit 1 */
#define  EXMC_CTLR4_ECCSZ_2                         ((uint32_t)0x00080000)        /*!< Bit 2 */
       
/*******************  Bit definition for EXMC_SIR2 register  *******************/
#define  EXMC_SIR2_ITRS                             ((uint8_t)0x01)               /*!< Interrupt rising edge status */
#define  EXMC_SIR2_ITHS                             ((uint8_t)0x02)               /*!< Interrupt high-level status */
#define  EXMC_SIR2_ITFS                             ((uint8_t)0x04)               /*!< Interrupt falling edge status */
#define  EXMC_SIR2_ITREN                            ((uint8_t)0x08)               /*!< Interrupt rising edge detection Enable bit */
#define  EXMC_SIR2_ITHEN                            ((uint8_t)0x10)               /*!< Interrupt high level detection Enable bit */
#define  EXMC_SIR2_ITFEN                            ((uint8_t)0x20)               /*!< Interrupt falling edge detection Enable bit */
#define  EXMC_SIR2_FIFOE                            ((uint8_t)0x40)               /*!< FIFO empty flag */
       
/*******************  Bit definition for EXMC_SIR3 register  *******************/
#define  EXMC_SIR3_ITRS                             ((uint8_t)0x01)               /*!< Interrupt rising edge status */
#define  EXMC_SIR3_ITHS                             ((uint8_t)0x02)               /*!< Interrupt high-level status */
#define  EXMC_SIR3_ITFS                             ((uint8_t)0x04)               /*!< Interrupt falling edge status */
#define  EXMC_SIR3_ITREN                            ((uint8_t)0x08)               /*!< Interrupt rising edge detection Enable bit */
#define  EXMC_SIR3_ITHEN                            ((uint8_t)0x10)               /*!< Interrupt high level detection Enable bit */
#define  EXMC_SIR3_ITFEN                            ((uint8_t)0x20)               /*!< Interrupt falling edge detection Enable bit */
#define  EXMC_SIR3_FIFOE                            ((uint8_t)0x40)               /*!< FIFO empty flag */
       
/*******************  Bit definition for EXMC_SIR4 register  *******************/
#define  EXMC_SIR4_ITRS                             ((uint8_t)0x01)               /*!< Interrupt rising edge status */
#define  EXMC_SIR4_ITHS                             ((uint8_t)0x02)               /*!< Interrupt high-level status */
#define  EXMC_SIR4_ITFS                             ((uint8_t)0x04)               /*!< Interrupt falling edge status */
#define  EXMC_SIR4_ITREN                            ((uint8_t)0x08)               /*!< Interrupt rising edge detection Enable bit */
#define  EXMC_SIR4_ITHEN                            ((uint8_t)0x10)               /*!< Interrupt high level detection Enable bit */
#define  EXMC_SIR4_ITFEN                            ((uint8_t)0x20)               /*!< Interrupt falling edge detection Enable bit */
#define  EXMC_SIR4_FIFOE                            ((uint8_t)0x40)               /*!< FIFO empty flag */
       
/******************  Bit definition for EXMC_COMTR2 register  ******************/
#define  EXMC_COMTR2_COMST                          ((uint32_t)0x000000FF)        /*!< COMST[7:0] bits (Common memory 2 setup time) */
#define  EXMC_COMTR2_COMST_0                        ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  EXMC_COMTR2_COMST_1                        ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  EXMC_COMTR2_COMST_2                        ((uint32_t)0x00000004)        /*!< Bit 2 */
#define  EXMC_COMTR2_COMST_3                        ((uint32_t)0x00000008)        /*!< Bit 3 */
#define  EXMC_COMTR2_COMST_4                        ((uint32_t)0x00000010)        /*!< Bit 4 */
#define  EXMC_COMTR2_COMST_5                        ((uint32_t)0x00000020)        /*!< Bit 5 */
#define  EXMC_COMTR2_COMST_6                        ((uint32_t)0x00000040)        /*!< Bit 6 */
#define  EXMC_COMTR2_COMST_7                        ((uint32_t)0x00000080)        /*!< Bit 7 */
       
#define  EXMC_COMTR2_COMWT                          ((uint32_t)0x0000FF00)        /*!< COMWT[7:0] bits (Common memory 2 wait time) */
#define  EXMC_COMTR2_COMWT_0                        ((uint32_t)0x00000100)        /*!< Bit 0 */
#define  EXMC_COMTR2_COMWT_1                        ((uint32_t)0x00000200)        /*!< Bit 1 */
#define  EXMC_COMTR2_COMWT_2                        ((uint32_t)0x00000400)        /*!< Bit 2 */
#define  EXMC_COMTR2_COMWT_3                        ((uint32_t)0x00000800)        /*!< Bit 3 */
#define  EXMC_COMTR2_COMWT_4                        ((uint32_t)0x00001000)        /*!< Bit 4 */
#define  EXMC_COMTR2_COMWT_5                        ((uint32_t)0x00002000)        /*!< Bit 5 */
#define  EXMC_COMTR2_COMWT_6                        ((uint32_t)0x00004000)        /*!< Bit 6 */
#define  EXMC_COMTR2_COMWT_7                        ((uint32_t)0x00008000)        /*!< Bit 7 */
       
#define  EXMC_COMTR2_COMHT                          ((uint32_t)0x00FF0000)        /*!< COMHT[7:0] bits (Common memory 2 hold time) */
#define  EXMC_COMTR2_COMHT_0                        ((uint32_t)0x00010000)        /*!< Bit 0 */
#define  EXMC_COMTR2_COMHT_1                        ((uint32_t)0x00020000)        /*!< Bit 1 */
#define  EXMC_COMTR2_COMHT_2                        ((uint32_t)0x00040000)        /*!< Bit 2 */
#define  EXMC_COMTR2_COMHT_3                        ((uint32_t)0x00080000)        /*!< Bit 3 */
#define  EXMC_COMTR2_COMHT_4                        ((uint32_t)0x00100000)        /*!< Bit 4 */
#define  EXMC_COMTR2_COMHT_5                        ((uint32_t)0x00200000)        /*!< Bit 5 */
#define  EXMC_COMTR2_COMHT_6                        ((uint32_t)0x00400000)        /*!< Bit 6 */
#define  EXMC_COMTR2_COMHT_7                        ((uint32_t)0x00800000)        /*!< Bit 7 */
       
#define  EXMC_COMTR2_COMHIZT                        ((uint32_t)0xFF000000)        /*!< COMHIZ[7:0] bits (Common memory 2 databus HiZ time) */
#define  EXMC_COMTR2_COMHIZT_0                      ((uint32_t)0x01000000)        /*!< Bit 0 */
#define  EXMC_COMTR2_COMHIZT_1                      ((uint32_t)0x02000000)        /*!< Bit 1 */
#define  EXMC_COMTR2_COMHIZT_2                      ((uint32_t)0x04000000)        /*!< Bit 2 */
#define  EXMC_COMTR2_COMHIZT_3                      ((uint32_t)0x08000000)        /*!< Bit 3 */
#define  EXMC_COMTR2_COMHIZT_4                      ((uint32_t)0x10000000)        /*!< Bit 4 */
#define  EXMC_COMTR2_COMHIZT_5                      ((uint32_t)0x20000000)        /*!< Bit 5 */
#define  EXMC_COMTR2_COMHIZT_6                      ((uint32_t)0x40000000)        /*!< Bit 6 */
#define  EXMC_COMTR2_COMHIZT_7                      ((uint32_t)0x80000000)        /*!< Bit 7 */
       
/******************  Bit definition for EXMC_COMTR3 register  ******************/
#define  EXMC_COMTR3_COMST                          ((uint32_t)0x000000FF)        /*!< COMST[7:0] bits (Common memory 3 setup time) */
#define  EXMC_COMTR3_COMST_0                        ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  EXMC_COMTR3_COMST_1                        ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  EXMC_COMTR3_COMST_2                        ((uint32_t)0x00000004)        /*!< Bit 2 */
#define  EXMC_COMTR3_COMST_3                        ((uint32_t)0x00000008)        /*!< Bit 3 */
#define  EXMC_COMTR3_COMST_4                        ((uint32_t)0x00000010)        /*!< Bit 4 */
#define  EXMC_COMTR3_COMST_5                        ((uint32_t)0x00000020)        /*!< Bit 5 */
#define  EXMC_COMTR3_COMST_6                        ((uint32_t)0x00000040)        /*!< Bit 6 */
#define  EXMC_COMTR3_COMST_7                        ((uint32_t)0x00000080)        /*!< Bit 7 */
       
#define  EXMC_COMTR3_COMWT                          ((uint32_t)0x0000FF00)        /*!< COMWT[7:0] bits (Common memory 3 wait time) */
#define  EXMC_COMTR3_COMWT_0                        ((uint32_t)0x00000100)        /*!< Bit 0 */
#define  EXMC_COMTR3_COMWT_1                        ((uint32_t)0x00000200)        /*!< Bit 1 */
#define  EXMC_COMTR3_COMWT_2                        ((uint32_t)0x00000400)        /*!< Bit 2 */
#define  EXMC_COMTR3_COMWT_3                        ((uint32_t)0x00000800)        /*!< Bit 3 */
#define  EXMC_COMTR3_COMWT_4                        ((uint32_t)0x00001000)        /*!< Bit 4 */
#define  EXMC_COMTR3_COMWT_5                        ((uint32_t)0x00002000)        /*!< Bit 5 */
#define  EXMC_COMTR3_COMWT_6                        ((uint32_t)0x00004000)        /*!< Bit 6 */
#define  EXMC_COMTR3_COMWT_7                        ((uint32_t)0x00008000)        /*!< Bit 7 */
       
#define  EXMC_COMTR3_COMHT                          ((uint32_t)0x00FF0000)        /*!< COMHT[7:0] bits (Common memory 3 hold time) */
#define  EXMC_COMTR3_COMHT_0                        ((uint32_t)0x00010000)        /*!< Bit 0 */
#define  EXMC_COMTR3_COMHT_1                        ((uint32_t)0x00020000)        /*!< Bit 1 */
#define  EXMC_COMTR3_COMHT_2                        ((uint32_t)0x00040000)        /*!< Bit 2 */
#define  EXMC_COMTR3_COMHT_3                        ((uint32_t)0x00080000)        /*!< Bit 3 */
#define  EXMC_COMTR3_COMHT_4                        ((uint32_t)0x00100000)        /*!< Bit 4 */
#define  EXMC_COMTR3_COMHT_5                        ((uint32_t)0x00200000)        /*!< Bit 5 */
#define  EXMC_COMTR3_COMHT_6                        ((uint32_t)0x00400000)        /*!< Bit 6 */
#define  EXMC_COMTR3_COMHT_7                        ((uint32_t)0x00800000)        /*!< Bit 7 */
       
#define  EXMC_COMTR3_COMHIZT                        ((uint32_t)0xFF000000)        /*!< COMHIZ[7:0] bits (Common memory 3 databus HiZ time) */
#define  EXMC_COMTR3_COMHIZT_0                      ((uint32_t)0x01000000)        /*!< Bit 0 */
#define  EXMC_COMTR3_COMHIZT_1                      ((uint32_t)0x02000000)        /*!< Bit 1 */
#define  EXMC_COMTR3_COMHIZT_2                      ((uint32_t)0x04000000)        /*!< Bit 2 */
#define  EXMC_COMTR3_COMHIZT_3                      ((uint32_t)0x08000000)        /*!< Bit 3 */
#define  EXMC_COMTR3_COMHIZT_4                      ((uint32_t)0x10000000)        /*!< Bit 4 */
#define  EXMC_COMTR3_COMHIZT_5                      ((uint32_t)0x20000000)        /*!< Bit 5 */
#define  EXMC_COMTR3_COMHIZT_6                      ((uint32_t)0x40000000)        /*!< Bit 6 */
#define  EXMC_COMTR3_COMHIZT_7                      ((uint32_t)0x80000000)        /*!< Bit 7 */
       
/******************  Bit definition for EXMC_COMTR4 register  ******************/
#define  EXMC_COMTR4_COMST                          ((uint32_t)0x000000FF)        /*!< COMST[7:0] bits (Common memory 4 setup time) */
#define  EXMC_COMTR4_COMST_0                        ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  EXMC_COMTR4_COMST_1                        ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  EXMC_COMTR4_COMST_2                        ((uint32_t)0x00000004)        /*!< Bit 2 */
#define  EXMC_COMTR4_COMST_3                        ((uint32_t)0x00000008)        /*!< Bit 3 */
#define  EXMC_COMTR4_COMST_4                        ((uint32_t)0x00000010)        /*!< Bit 4 */
#define  EXMC_COMTR4_COMST_5                        ((uint32_t)0x00000020)        /*!< Bit 5 */
#define  EXMC_COMTR4_COMST_6                        ((uint32_t)0x00000040)        /*!< Bit 6 */
#define  EXMC_COMTR4_COMST_7                        ((uint32_t)0x00000080)        /*!< Bit 7 */
       
#define  EXMC_COMTR4_COMWT                          ((uint32_t)0x0000FF00)        /*!< COMWT[7:0] bits (Common memory 4 wait time) */
#define  EXMC_COMTR4_COMWT_0                        ((uint32_t)0x00000100)        /*!< Bit 0 */
#define  EXMC_COMTR4_COMWT_1                        ((uint32_t)0x00000200)        /*!< Bit 1 */
#define  EXMC_COMTR4_COMWT_2                        ((uint32_t)0x00000400)        /*!< Bit 2 */
#define  EXMC_COMTR4_COMWT_3                        ((uint32_t)0x00000800)        /*!< Bit 3 */
#define  EXMC_COMTR4_COMWT_4                        ((uint32_t)0x00001000)        /*!< Bit 4 */
#define  EXMC_COMTR4_COMWT_5                        ((uint32_t)0x00002000)        /*!< Bit 5 */
#define  EXMC_COMTR4_COMWT_6                        ((uint32_t)0x00004000)        /*!< Bit 6 */
#define  EXMC_COMTR4_COMWT_7                        ((uint32_t)0x00008000)        /*!< Bit 7 */
       
#define  EXMC_COMTR4_COMHT                          ((uint32_t)0x00FF0000)        /*!< COMHT[7:0] bits (Common memory 4 hold time) */
#define  EXMC_COMTR4_COMHT_0                        ((uint32_t)0x00010000)        /*!< Bit 0 */
#define  EXMC_COMTR4_COMHT_1                        ((uint32_t)0x00020000)        /*!< Bit 1 */
#define  EXMC_COMTR4_COMHT_2                        ((uint32_t)0x00040000)        /*!< Bit 2 */
#define  EXMC_COMTR4_COMHT_3                        ((uint32_t)0x00080000)        /*!< Bit 3 */
#define  EXMC_COMTR4_COMHT_4                        ((uint32_t)0x00100000)        /*!< Bit 4 */
#define  EXMC_COMTR4_COMHT_5                        ((uint32_t)0x00200000)        /*!< Bit 5 */
#define  EXMC_COMTR4_COMHT_6                        ((uint32_t)0x00400000)        /*!< Bit 6 */
#define  EXMC_COMTR4_COMHT_7                        ((uint32_t)0x00800000)        /*!< Bit 7 */
       
#define  EXMC_COMTR4_COMHIZT                        ((uint32_t)0xFF000000)        /*!< COMHIZ[7:0] bits (Common memory 4 databus HiZ time) */
#define  EXMC_COMTR4_COMHIZT_0                      ((uint32_t)0x01000000)        /*!< Bit 0 */
#define  EXMC_COMTR4_COMHIZT_1                      ((uint32_t)0x02000000)        /*!< Bit 1 */
#define  EXMC_COMTR4_COMHIZT_2                      ((uint32_t)0x04000000)        /*!< Bit 2 */
#define  EXMC_COMTR4_COMHIZT_3                      ((uint32_t)0x08000000)        /*!< Bit 3 */
#define  EXMC_COMTR4_COMHIZT_4                      ((uint32_t)0x10000000)        /*!< Bit 4 */
#define  EXMC_COMTR4_COMHIZT_5                      ((uint32_t)0x20000000)        /*!< Bit 5 */
#define  EXMC_COMTR4_COMHIZT_6                      ((uint32_t)0x40000000)        /*!< Bit 6 */
#define  EXMC_COMTR4_COMHIZT_7                      ((uint32_t)0x80000000)        /*!< Bit 7 */
       
/******************  Bit definition for EXMC_ATTR2 register  ******************/
#define  EXMC_ATTR2_ATTST                           ((uint32_t)0x000000FF)        /*!< ATTST[7:0] bits (Attribute memory 2 setup time) */
#define  EXMC_ATTR2_ATTST_0                         ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  EXMC_ATTR2_ATTST_1                         ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  EXMC_ATTR2_ATTST_2                         ((uint32_t)0x00000004)        /*!< Bit 2 */
#define  EXMC_ATTR2_ATTST_3                         ((uint32_t)0x00000008)        /*!< Bit 3 */
#define  EXMC_ATTR2_ATTST_4                         ((uint32_t)0x00000010)        /*!< Bit 4 */
#define  EXMC_ATTR2_ATTST_5                         ((uint32_t)0x00000020)        /*!< Bit 5 */
#define  EXMC_ATTR2_ATTST_6                         ((uint32_t)0x00000040)        /*!< Bit 6 */
#define  EXMC_ATTR2_ATTST_7                         ((uint32_t)0x00000080)        /*!< Bit 7 */
       
#define  EXMC_ATTR2_ATTWT                           ((uint32_t)0x0000FF00)        /*!< ATTWT[7:0] bits (Attribute memory 2 wait time) */
#define  EXMC_ATTR2_ATTWT_0                         ((uint32_t)0x00000100)        /*!< Bit 0 */
#define  EXMC_ATTR2_ATTWT_1                         ((uint32_t)0x00000200)        /*!< Bit 1 */
#define  EXMC_ATTR2_ATTWT_2                         ((uint32_t)0x00000400)        /*!< Bit 2 */
#define  EXMC_ATTR2_ATTWT_3                         ((uint32_t)0x00000800)        /*!< Bit 3 */
#define  EXMC_ATTR2_ATTWT_4                         ((uint32_t)0x00001000)        /*!< Bit 4 */
#define  EXMC_ATTR2_ATTWT_5                         ((uint32_t)0x00002000)        /*!< Bit 5 */
#define  EXMC_ATTR2_ATTWT_6                         ((uint32_t)0x00004000)        /*!< Bit 6 */
#define  EXMC_ATTR2_ATTWT_7                         ((uint32_t)0x00008000)        /*!< Bit 7 */
       
#define  EXMC_ATTR2_ATTHT                           ((uint32_t)0x00FF0000)        /*!< ATTHT[7:0] bits (Attribute memory 2 hold time) */
#define  EXMC_ATTR2_ATTHT_0                         ((uint32_t)0x00010000)        /*!< Bit 0 */
#define  EXMC_ATTR2_ATTHT_1                         ((uint32_t)0x00020000)        /*!< Bit 1 */
#define  EXMC_ATTR2_ATTHT_2                         ((uint32_t)0x00040000)        /*!< Bit 2 */
#define  EXMC_ATTR2_ATTHT_3                         ((uint32_t)0x00080000)        /*!< Bit 3 */
#define  EXMC_ATTR2_ATTHT_4                         ((uint32_t)0x00100000)        /*!< Bit 4 */
#define  EXMC_ATTR2_ATTHT_5                         ((uint32_t)0x00200000)        /*!< Bit 5 */
#define  EXMC_ATTR2_ATTHT_6                         ((uint32_t)0x00400000)        /*!< Bit 6 */
#define  EXMC_ATTR2_ATTHT_7                         ((uint32_t)0x00800000)        /*!< Bit 7 */
       
#define  EXMC_ATTR2_ATTHIZT                         ((uint32_t)0xFF000000)        /*!< ATTHIZ[7:0] bits (Attribute memory 2 databus HiZ time) */
#define  EXMC_ATTR2_ATTHIZT_0                       ((uint32_t)0x01000000)        /*!< Bit 0 */
#define  EXMC_ATTR2_ATTHIZT_1                       ((uint32_t)0x02000000)        /*!< Bit 1 */
#define  EXMC_ATTR2_ATTHIZT_2                       ((uint32_t)0x04000000)        /*!< Bit 2 */
#define  EXMC_ATTR2_ATTHIZT_3                       ((uint32_t)0x08000000)        /*!< Bit 3 */
#define  EXMC_ATTR2_ATTHIZT_4                       ((uint32_t)0x10000000)        /*!< Bit 4 */
#define  EXMC_ATTR2_ATTHIZT_5                       ((uint32_t)0x20000000)        /*!< Bit 5 */
#define  EXMC_ATTR2_ATTHIZT_6                       ((uint32_t)0x40000000)        /*!< Bit 6 */
#define  EXMC_ATTR2_ATTHIZT_7                       ((uint32_t)0x80000000)        /*!< Bit 7 */
       
/******************  Bit definition for EXMC_ATTR3 register  ******************/
#define  EXMC_ATTR3_ATTST                           ((uint32_t)0x000000FF)        /*!< ATTST[7:0] bits (Attribute memory 3 setup time) */
#define  EXMC_ATTR3_ATTST_0                         ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  EXMC_ATTR3_ATTST_1                         ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  EXMC_ATTR3_ATTST_2                         ((uint32_t)0x00000004)        /*!< Bit 2 */
#define  EXMC_ATTR3_ATTST_3                         ((uint32_t)0x00000008)        /*!< Bit 3 */
#define  EXMC_ATTR3_ATTST_4                         ((uint32_t)0x00000010)        /*!< Bit 4 */
#define  EXMC_ATTR3_ATTST_5                         ((uint32_t)0x00000020)        /*!< Bit 5 */
#define  EXMC_ATTR3_ATTST_6                         ((uint32_t)0x00000040)        /*!< Bit 6 */
#define  EXMC_ATTR3_ATTST_7                         ((uint32_t)0x00000080)        /*!< Bit 7 */
       
#define  EXMC_ATTR3_ATTWT                           ((uint32_t)0x0000FF00)        /*!< ATTWT[7:0] bits (Attribute memory 3 wait time) */
#define  EXMC_ATTR3_ATTWT_0                         ((uint32_t)0x00000100)        /*!< Bit 0 */
#define  EXMC_ATTR3_ATTWT_1                         ((uint32_t)0x00000200)        /*!< Bit 1 */
#define  EXMC_ATTR3_ATTWT_2                         ((uint32_t)0x00000400)        /*!< Bit 2 */
#define  EXMC_ATTR3_ATTWT_3                         ((uint32_t)0x00000800)        /*!< Bit 3 */
#define  EXMC_ATTR3_ATTWT_4                         ((uint32_t)0x00001000)        /*!< Bit 4 */
#define  EXMC_ATTR3_ATTWT_5                         ((uint32_t)0x00002000)        /*!< Bit 5 */
#define  EXMC_ATTR3_ATTWT_6                         ((uint32_t)0x00004000)        /*!< Bit 6 */
#define  EXMC_ATTR3_ATTWT_7                         ((uint32_t)0x00008000)        /*!< Bit 7 */
       
#define  EXMC_ATTR3_ATTHT                           ((uint32_t)0x00FF0000)        /*!< ATTHT[7:0] bits (Attribute memory 3 hold time) */
#define  EXMC_ATTR3_ATTHT_0                         ((uint32_t)0x00010000)        /*!< Bit 0 */
#define  EXMC_ATTR3_ATTHT_1                         ((uint32_t)0x00020000)        /*!< Bit 1 */
#define  EXMC_ATTR3_ATTHT_2                         ((uint32_t)0x00040000)        /*!< Bit 2 */
#define  EXMC_ATTR3_ATTHT_3                         ((uint32_t)0x00080000)        /*!< Bit 3 */
#define  EXMC_ATTR3_ATTHT_4                         ((uint32_t)0x00100000)        /*!< Bit 4 */
#define  EXMC_ATTR3_ATTHT_5                         ((uint32_t)0x00200000)        /*!< Bit 5 */
#define  EXMC_ATTR3_ATTHT_6                         ((uint32_t)0x00400000)        /*!< Bit 6 */
#define  EXMC_ATTR3_ATTHT_7                         ((uint32_t)0x00800000)        /*!< Bit 7 */
       
#define  EXMC_ATTR3_ATTHIZT                         ((uint32_t)0xFF000000)        /*!< ATTHIZ[7:0] bits (Attribute memory 3 databus HiZ time) */
#define  EXMC_ATTR3_ATTHIZT_0                       ((uint32_t)0x01000000)        /*!< Bit 0 */
#define  EXMC_ATTR3_ATTHIZT_1                       ((uint32_t)0x02000000)        /*!< Bit 1 */
#define  EXMC_ATTR3_ATTHIZT_2                       ((uint32_t)0x04000000)        /*!< Bit 2 */
#define  EXMC_ATTR3_ATTHIZT_3                       ((uint32_t)0x08000000)        /*!< Bit 3 */
#define  EXMC_ATTR3_ATTHIZT_4                       ((uint32_t)0x10000000)        /*!< Bit 4 */
#define  EXMC_ATTR3_ATTHIZT_5                       ((uint32_t)0x20000000)        /*!< Bit 5 */
#define  EXMC_ATTR3_ATTHIZT_6                       ((uint32_t)0x40000000)        /*!< Bit 6 */
#define  EXMC_ATTR3_ATTHIZT_7                       ((uint32_t)0x80000000)        /*!< Bit 7 */
       
/******************  Bit definition for EXMC_ATTR4 register  ******************/
#define  EXMC_ATTR4_ATTST                           ((uint32_t)0x000000FF)        /*!< ATTST[7:0] bits (Attribute memory 4 setup time) */
#define  EXMC_ATTR4_ATTST_0                         ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  EXMC_ATTR4_ATTST_1                         ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  EXMC_ATTR4_ATTST_2                         ((uint32_t)0x00000004)        /*!< Bit 2 */
#define  EXMC_ATTR4_ATTST_3                         ((uint32_t)0x00000008)        /*!< Bit 3 */
#define  EXMC_ATTR4_ATTST_4                         ((uint32_t)0x00000010)        /*!< Bit 4 */
#define  EXMC_ATTR4_ATTST_5                         ((uint32_t)0x00000020)        /*!< Bit 5 */
#define  EXMC_ATTR4_ATTST_6                         ((uint32_t)0x00000040)        /*!< Bit 6 */
#define  EXMC_ATTR4_ATTST_7                         ((uint32_t)0x00000080)        /*!< Bit 7 */
       
#define  EXMC_ATTR4_ATTWT                           ((uint32_t)0x0000FF00)        /*!< ATTWT[7:0] bits (Attribute memory 4 wait time) */
#define  EXMC_ATTR4_ATTWT_0                         ((uint32_t)0x00000100)        /*!< Bit 0 */
#define  EXMC_ATTR4_ATTWT_1                         ((uint32_t)0x00000200)        /*!< Bit 1 */
#define  EXMC_ATTR4_ATTWT_2                         ((uint32_t)0x00000400)        /*!< Bit 2 */
#define  EXMC_ATTR4_ATTWT_3                         ((uint32_t)0x00000800)        /*!< Bit 3 */
#define  EXMC_ATTR4_ATTWT_4                         ((uint32_t)0x00001000)        /*!< Bit 4 */
#define  EXMC_ATTR4_ATTWT_5                         ((uint32_t)0x00002000)        /*!< Bit 5 */
#define  EXMC_ATTR4_ATTWT_6                         ((uint32_t)0x00004000)        /*!< Bit 6 */
#define  EXMC_ATTR4_ATTWT_7                         ((uint32_t)0x00008000)        /*!< Bit 7 */

#define  EXMC_ATTR4_ATTHT                           ((uint32_t)0x00FF0000)        /*!< ATTHT[7:0] bits (Attribute memory 4 hold time) */
#define  EXMC_ATTR4_ATTHT_0                         ((uint32_t)0x00010000)        /*!< Bit 0 */
#define  EXMC_ATTR4_ATTHT_1                         ((uint32_t)0x00020000)        /*!< Bit 1 */
#define  EXMC_ATTR4_ATTHT_2                         ((uint32_t)0x00040000)        /*!< Bit 2 */
#define  EXMC_ATTR4_ATTHT_3                         ((uint32_t)0x00080000)        /*!< Bit 3 */
#define  EXMC_ATTR4_ATTHT_4                         ((uint32_t)0x00100000)        /*!< Bit 4 */
#define  EXMC_ATTR4_ATTHT_5                         ((uint32_t)0x00200000)        /*!< Bit 5 */
#define  EXMC_ATTR4_ATTHT_6                         ((uint32_t)0x00400000)        /*!< Bit 6 */
#define  EXMC_ATTR4_ATTHT_7                         ((uint32_t)0x00800000)        /*!< Bit 7 */
       
#define  EXMC_ATTR4_ATTHIZT                         ((uint32_t)0xFF000000)        /*!< ATTHIZ[7:0] bits (Attribute memory 4 databus HiZ time) */
#define  EXMC_ATTR4_ATTHIZT_0                       ((uint32_t)0x01000000)        /*!< Bit 0 */
#define  EXMC_ATTR4_ATTHIZT_1                       ((uint32_t)0x02000000)        /*!< Bit 1 */
#define  EXMC_ATTR4_ATTHIZT_2                       ((uint32_t)0x04000000)        /*!< Bit 2 */
#define  EXMC_ATTR4_ATTHIZT_3                       ((uint32_t)0x08000000)        /*!< Bit 3 */
#define  EXMC_ATTR4_ATTHIZT_4                       ((uint32_t)0x10000000)        /*!< Bit 4 */
#define  EXMC_ATTR4_ATTHIZT_5                       ((uint32_t)0x20000000)        /*!< Bit 5 */
#define  EXMC_ATTR4_ATTHIZT_6                       ((uint32_t)0x40000000)        /*!< Bit 6 */
#define  EXMC_ATTR3_ATTHIZT_7                       ((uint32_t)0x80000000)        /*!< Bit 7 */
       
/******************  Bit definition for EXMC_IOTR4 register  ******************/
#define  EXMC_IOTR4_IOST                            ((uint32_t)0x000000FF)        /*!< IOST[7:0] bits (I/O memory 4 setup time) */
#define  EXMC_IOTR4_IOST_0                          ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  EXMC_IOTR4_IOST_1                          ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  EXMC_IOTR4_IOST_2                          ((uint32_t)0x00000004)        /*!< Bit 2 */
#define  EXMC_IOTR4_IOST_3                          ((uint32_t)0x00000008)        /*!< Bit 3 */
#define  EXMC_IOTR4_IOST_4                          ((uint32_t)0x00000010)        /*!< Bit 4 */
#define  EXMC_IOTR4_IOST_5                          ((uint32_t)0x00000020)        /*!< Bit 5 */
#define  EXMC_IOTR4_IOST_6                          ((uint32_t)0x00000040)        /*!< Bit 6 */
#define  EXMC_IOTR4_IOST_7                          ((uint32_t)0x00000080)        /*!< Bit 7 */
       
#define  EXMC_IOTR4_IOWT                            ((uint32_t)0x0000FF00)        /*!< IOWT[7:0] bits (I/O memory 4 wait time) */
#define  EXMC_IOTR4_IOWT_0                          ((uint32_t)0x00000100)        /*!< Bit 0 */
#define  EXMC_IOTR4_IOWT_1                          ((uint32_t)0x00000200)        /*!< Bit 1 */
#define  EXMC_IOTR4_IOWT_2                          ((uint32_t)0x00000400)        /*!< Bit 2 */
#define  EXMC_IOTR4_IOWT_3                          ((uint32_t)0x00000800)        /*!< Bit 3 */
#define  EXMC_IOTR4_IOWT_4                          ((uint32_t)0x00001000)        /*!< Bit 4 */
#define  EXMC_IOTR4_IOWT_5                          ((uint32_t)0x00002000)        /*!< Bit 5 */
#define  EXMC_IOTR4_IOWT_6                          ((uint32_t)0x00004000)        /*!< Bit 6 */
#define  EXMC_IOTR4_IOWT_7                          ((uint32_t)0x00008000)        /*!< Bit 7 */
       
#define  EXMC_IOTR4_IOHT                            ((uint32_t)0x00FF0000)        /*!< IOHT[7:0] bits (I/O memory 4 hold time) */
#define  EXMC_IOTR4_IOHT_0                          ((uint32_t)0x00010000)        /*!< Bit 0 */
#define  EXMC_IOTR4_IOHT_1                          ((uint32_t)0x00020000)        /*!< Bit 1 */
#define  EXMC_IOTR4_IOHT_2                          ((uint32_t)0x00040000)        /*!< Bit 2 */
#define  EXMC_IOTR4_IOHT_3                          ((uint32_t)0x00080000)        /*!< Bit 3 */
#define  EXMC_IOTR4_IOHT_4                          ((uint32_t)0x00100000)        /*!< Bit 4 */
#define  EXMC_IOTR4_IOHT_5                          ((uint32_t)0x00200000)        /*!< Bit 5 */
#define  EXMC_IOTR4_IOHT_6                          ((uint32_t)0x00400000)        /*!< Bit 6 */
#define  EXMC_IOTR4_IOHT_7                          ((uint32_t)0x00800000)        /*!< Bit 7 */
       
#define  EXMC_IOTR4_IOHIZT                          ((uint32_t)0xFF000000)        /*!< IOHIZ[7:0] bits (I/O memory 4 databus HiZ time) */
#define  EXMC_IOTR4_IOHIZT_0                        ((uint32_t)0x01000000)        /*!< Bit 0 */
#define  EXMC_IOTR4_IOHIZT_1                        ((uint32_t)0x02000000)        /*!< Bit 1 */
#define  EXMC_IOTR4_IOHIZT_2                        ((uint32_t)0x04000000)        /*!< Bit 2 */
#define  EXMC_IOTR4_IOHIZT_3                        ((uint32_t)0x08000000)        /*!< Bit 3 */
#define  EXMC_IOTR4_IOHIZT_4                        ((uint32_t)0x10000000)        /*!< Bit 4 */
#define  EXMC_IOTR4_IOHIZT_5                        ((uint32_t)0x20000000)        /*!< Bit 5 */
#define  EXMC_IOTR4_IOHIZT_6                        ((uint32_t)0x40000000)        /*!< Bit 6 */
#define  EXMC_IOTR4_IOHIZT_7                        ((uint32_t)0x80000000)        /*!< Bit 7 */
       
/******************  Bit definition for EXMC_ECCR2 register  ******************/
#define  EXMC_ECCR2_ECC                             ((uint32_t)0xFFFFFFFF)        /*!< ECC result */
       
/******************  Bit definition for EXMC_ECCR3 register  ******************/
#define  EXMC_ECCR3_ECC                             ((uint32_t)0xFFFFFFFF)        /*!< ECC result */

/******************  Bit definition for EXMC_SINIR register   ******************/
#define  EXMC_SINIR_CMDBIT                          ((uint32_t)0x00030000)        /*!< CMDBIT[1:0] bits (Bit number of SPI PSRAM command phase) */
#define  EXMC_SINIR_CMDBIT_0                        ((uint32_t)0x00010000)        /*!< Bit 0 */
#define  EXMC_SINIR_CMDBIT_1                        ((uint32_t)0x00020000)        /*!< Bit 1 */

#define  EXMC_SINIR_ADRBIT                          ((uint32_t)0x1F000000)        /*!< ADRBIT[4:0] bits (Bit number of SPI PSRAM address phase) */
#define  EXMC_SINIR_ADRBIT_0                        ((uint32_t)0x01000000)        /*!< Bit 0 */
#define  EXMC_SINIR_ADRBIT_1                        ((uint32_t)0x02000000)        /*!< Bit 1 */
#define  EXMC_SINIR_ADRBIT_2                        ((uint32_t)0x04000000)        /*!< Bit 2 */
#define  EXMC_SINIR_ADRBIT_3                        ((uint32_t)0x08000000)        /*!< Bit 3 */
#define  EXMC_SINIR_ADRBIT_4                        ((uint32_t)0x10000000)        /*!< Bit 4 */
 
#define  EXMC_SINIR_IDL                             ((uint32_t)0x60000000)        /*!< IDL[1:0] bits (SPI PSRAM ID Length) */
#define  EXMC_SINIR_IDL_0                           ((uint32_t)0x20000000)        /*!< Bit 0 */              
#define  EXMC_SINIR_IDL_1                           ((uint32_t)0x40000000)        /*!< Bit 1 */

#define  EXMC_SINIR_POL                             ((uint32_t)0x80000000)        /*!< Sample data at falling edge */

/******************  Bit definition for EXMC_SRCR register  ******************/
#define  EXMC_SRCR_RCMD                             ((uint32_t)0x0000FFFF)        /*!< RCMD[15:0] bits (SPI Read Command for AHB read transfer) */
#define  EXMC_SRCR_RCMD_0                           ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  EXMC_SRCR_RCMD_1                           ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  EXMC_SRCR_RCMD_2                           ((uint32_t)0x00000004)        /*!< Bit 2 */
#define  EXMC_SRCR_RCMD_3                           ((uint32_t)0x00000008)        /*!< Bit 3 */
#define  EXMC_SRCR_RCMD_4                           ((uint32_t)0x00000010)        /*!< Bit 4 */
#define  EXMC_SRCR_RCMD_5                           ((uint32_t)0x00000020)        /*!< Bit 5 */
#define  EXMC_SRCR_RCMD_6                           ((uint32_t)0x00000040)        /*!< Bit 6 */
#define  EXMC_SRCR_RCMD_7                           ((uint32_t)0x00000080)        /*!< Bit 7 */
#define  EXMC_SRCR_RCMD_8                           ((uint32_t)0x00000100)        /*!< Bit 8 */
#define  EXMC_SRCR_RCMD_9                           ((uint32_t)0x00000200)        /*!< Bit 9 */
#define  EXMC_SRCR_RCMD_10                          ((uint32_t)0x00000400)        /*!< Bit 10 */
#define  EXMC_SRCR_RCMD_11                          ((uint32_t)0x00000800)        /*!< Bit 11 */
#define  EXMC_SRCR_RCMD_12                          ((uint32_t)0x00001000)        /*!< Bit 12 */
#define  EXMC_SRCR_RCMD_13                          ((uint32_t)0x00002000)        /*!< Bit 13 */
#define  EXMC_SRCR_RCMD_14                          ((uint32_t)0x00004000)        /*!< Bit 14 */
#define  EXMC_SRCR_RCMD_15                          ((uint32_t)0x00008000)        /*!< Bit 15 */

#define  EXMC_SRCR_RWAITCYCLE                       ((uint32_t)0x000F0000)        /*!< RWAITCYCLE[3:0] bits (SPI Read Wait Cycle number after address phase) */
#define  EXMC_SRCR_RWAITCYCLE_0                     ((uint32_t)0x00010000)        /*!< Bit 0 */
#define  EXMC_SRCR_RWAITCYCLE_1                     ((uint32_t)0x00020000)        /*!< Bit 1 */
#define  EXMC_SRCR_RWAITCYCLE_2                     ((uint32_t)0x00040000)        /*!< Bit 2 */
#define  EXMC_SRCR_RWAITCYCLE_3                     ((uint32_t)0x00080000)        /*!< Bit 3 */

#define  EXMC_SRCR_RMODE                            ((uint32_t)0x00300000)        /*!< RMODE[1:0] bits (SPI PSRAM Read command mode) */
#define  EXMC_SRCR_RMODE_0                          ((uint32_t)0x00100000)        /*!< Bit 0 */
#define  EXMC_SRCR_RMODE_1                          ((uint32_t)0x00200000)        /*!< Bit 1 */

#define  EXMC_SRCR_RDID                             ((uint32_t)0x80000000)        /*!< Send SPI Read ID Command */

/******************  Bit definition for EXMC_SWCR register  ******************/
#define  EXMC_SWCR_WCMD                             ((uint32_t)0x0000FFFF)        /*!< WCMD[15:0] bits (SPI Write Command for AHB write transfer) */
#define  EXMC_SWCR_WCMD_0                           ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  EXMC_SWCR_WCMD_1                           ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  EXMC_SWCR_WCMD_2                           ((uint32_t)0x00000004)        /*!< Bit 2 */
#define  EXMC_SWCR_WCMD_3                           ((uint32_t)0x00000008)        /*!< Bit 3 */
#define  EXMC_SWCR_WCMD_4                           ((uint32_t)0x00000010)        /*!< Bit 4 */
#define  EXMC_SWCR_WCMD_5                           ((uint32_t)0x00000020)        /*!< Bit 5 */
#define  EXMC_SWCR_WCMD_6                           ((uint32_t)0x00000040)        /*!< Bit 6 */
#define  EXMC_SWCR_WCMD_7                           ((uint32_t)0x00000080)        /*!< Bit 7 */
#define  EXMC_SWCR_WCMD_8                           ((uint32_t)0x00000100)        /*!< Bit 8 */
#define  EXMC_SWCR_WCMD_9                           ((uint32_t)0x00000200)        /*!< Bit 9 */
#define  EXMC_SWCR_WCMD_10                          ((uint32_t)0x00000400)        /*!< Bit 10 */
#define  EXMC_SWCR_WCMD_11                          ((uint32_t)0x00000800)        /*!< Bit 11 */
#define  EXMC_SWCR_WCMD_12                          ((uint32_t)0x00001000)        /*!< Bit 12 */
#define  EXMC_SWCR_WCMD_13                          ((uint32_t)0x00002000)        /*!< Bit 13 */
#define  EXMC_SWCR_WCMD_14                          ((uint32_t)0x00004000)        /*!< Bit 14 */
#define  EXMC_SWCR_WCMD_15                          ((uint32_t)0x00008000)        /*!< Bit 15 */
                                                                  
#define  EXMC_SWCR_WWAITCYCLE                       ((uint32_t)0x000F0000)        /*!< WWAITCYCLE[3:0] bits (SPI Write Wait Cycle number after address phase) */
#define  EXMC_SWCR_WWAITCYCLE_0                     ((uint32_t)0x00010000)        /*!< Bit 0 */
#define  EXMC_SWCR_WWAITCYCLE_1                     ((uint32_t)0x00020000)        /*!< Bit 1 */
#define  EXMC_SWCR_WWAITCYCLE_2                     ((uint32_t)0x00040000)        /*!< Bit 2 */
#define  EXMC_SWCR_WWAITCYCLE_3                     ((uint32_t)0x00080000)        /*!< Bit 3 */
                                                                  
#define  EXMC_SWCR_WMODE                            ((uint32_t)0x00300000)        /*!< WMODE[1:0] bits (SPI PSRAM Write command mode) */
#define  EXMC_SWCR_WMODE_0                          ((uint32_t)0x00100000)        /*!< Bit 0 */
#define  EXMC_SWCR_WMODE_1                          ((uint32_t)0x00200000)        /*!< Bit 1 */
                                                                  
#define  EXMC_SWCR_SC                               ((uint32_t)0x80000000)        /*!< Send SPI Special Command which does not have address and data phase */

/******************  Bit definition for EXMC_SIDLR register ******************/
#define  EXMC_SIDLR_SIDLR                           ((uint32_t)0xFFFFFFFF)        /*!< SIDLR[31:0] bits (ID Low Data saved for SPI Read ID Command) */
#define  EXMC_SIDLR_SIDLR_0                         ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  EXMC_SIDLR_SIDLR_1                         ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  EXMC_SIDLR_SIDLR_2                         ((uint32_t)0x00000004)        /*!< Bit 2 */
#define  EXMC_SIDLR_SIDLR_3                         ((uint32_t)0x00000008)        /*!< Bit 3 */
#define  EXMC_SIDLR_SIDLR_4                         ((uint32_t)0x00000010)        /*!< Bit 4 */
#define  EXMC_SIDLR_SIDLR_5                         ((uint32_t)0x00000020)        /*!< Bit 5 */
#define  EXMC_SIDLR_SIDLR_6                         ((uint32_t)0x00000040)        /*!< Bit 6 */
#define  EXMC_SIDLR_SIDLR_7                         ((uint32_t)0x00000080)        /*!< Bit 7 */
#define  EXMC_SIDLR_SIDLR_8                         ((uint32_t)0x00000100)        /*!< Bit 8 */
#define  EXMC_SIDLR_SIDLR_9                         ((uint32_t)0x00000200)        /*!< Bit 9 */
#define  EXMC_SIDLR_SIDLR_10                        ((uint32_t)0x00000400)        /*!< Bit 10 */
#define  EXMC_SIDLR_SIDLR_11                        ((uint32_t)0x00000800)        /*!< Bit 11 */
#define  EXMC_SIDLR_SIDLR_12                        ((uint32_t)0x00001000)        /*!< Bit 12 */
#define  EXMC_SIDLR_SIDLR_13                        ((uint32_t)0x00002000)        /*!< Bit 13 */
#define  EXMC_SIDLR_SIDLR_14                        ((uint32_t)0x00004000)        /*!< Bit 14 */
#define  EXMC_SIDLR_SIDLR_15                        ((uint32_t)0x00008000)        /*!< Bit 15 */
#define  EXMC_SIDLR_SIDLR_16                        ((uint32_t)0x00010000)        /*!< Bit 16 */
#define  EXMC_SIDLR_SIDLR_17                        ((uint32_t)0x00020000)        /*!< Bit 17 */
#define  EXMC_SIDLR_SIDLR_18                        ((uint32_t)0x00040000)        /*!< Bit 18 */
#define  EXMC_SIDLR_SIDLR_19                        ((uint32_t)0x00080000)        /*!< Bit 19 */
#define  EXMC_SIDLR_SIDLR_20                        ((uint32_t)0x00100000)        /*!< Bit 20 */
#define  EXMC_SIDLR_SIDLR_21                        ((uint32_t)0x00200000)        /*!< Bit 21 */
#define  EXMC_SIDLR_SIDLR_22                        ((uint32_t)0x00400000)        /*!< Bit 22 */
#define  EXMC_SIDLR_SIDLR_23                        ((uint32_t)0x00800000)        /*!< Bit 23 */
#define  EXMC_SIDLR_SIDLR_24                        ((uint32_t)0x01000000)        /*!< Bit 24 */
#define  EXMC_SIDLR_SIDLR_25                        ((uint32_t)0x02000000)        /*!< Bit 25 */
#define  EXMC_SIDLR_SIDLR_26                        ((uint32_t)0x04000000)        /*!< Bit 26 */
#define  EXMC_SIDLR_SIDLR_27                        ((uint32_t)0x08000000)        /*!< Bit 27 */
#define  EXMC_SIDLR_SIDLR_28                        ((uint32_t)0x10000000)        /*!< Bit 28 */
#define  EXMC_SIDLR_SIDLR_29                        ((uint32_t)0x20000000)        /*!< Bit 29 */
#define  EXMC_SIDLR_SIDLR_30                        ((uint32_t)0x40000000)        /*!< Bit 30 */
#define  EXMC_SIDLR_SIDLR_31                        ((uint32_t)0x80000000)        /*!< Bit 31 */

/******************  Bit definition for EXMC_SIDHR register ******************/
                 
#define  EXMC_SIDHR_SIDHR                           ((uint32_t)0xFFFFFFFF)        /*!< SIDHR[31:0] bits (ID High Data saved for SPI Read ID Command)*/
#define  EXMC_SIDHR_SIDHR_0                         ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  EXMC_SIDHR_SIDHR_1                         ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  EXMC_SIDHR_SIDHR_2                         ((uint32_t)0x00000004)        /*!< Bit 2 */
#define  EXMC_SIDHR_SIDHR_3                         ((uint32_t)0x00000008)        /*!< Bit 3 */
#define  EXMC_SIDHR_SIDHR_4                         ((uint32_t)0x00000010)        /*!< Bit 4 */
#define  EXMC_SIDHR_SIDHR_5                         ((uint32_t)0x00000020)        /*!< Bit 5 */
#define  EXMC_SIDHR_SIDHR_6                         ((uint32_t)0x00000040)        /*!< Bit 6 */
#define  EXMC_SIDHR_SIDHR_7                         ((uint32_t)0x00000080)        /*!< Bit 7 */
#define  EXMC_SIDHR_SIDHR_8                         ((uint32_t)0x00000100)        /*!< Bit 8 */
#define  EXMC_SIDHR_SIDHR_9                         ((uint32_t)0x00000200)        /*!< Bit 9 */
#define  EXMC_SIDHR_SIDHR_10                        ((uint32_t)0x00000400)        /*!< Bit 10 */
#define  EXMC_SIDHR_SIDHR_11                        ((uint32_t)0x00000800)        /*!< Bit 11 */
#define  EXMC_SIDHR_SIDHR_12                        ((uint32_t)0x00001000)        /*!< Bit 12 */
#define  EXMC_SIDHR_SIDHR_13                        ((uint32_t)0x00002000)        /*!< Bit 13 */
#define  EXMC_SIDHR_SIDHR_14                        ((uint32_t)0x00004000)        /*!< Bit 14 */
#define  EXMC_SIDHR_SIDHR_15                        ((uint32_t)0x00008000)        /*!< Bit 15 */
#define  EXMC_SIDHR_SIDHR_16                        ((uint32_t)0x00010000)        /*!< Bit 16 */
#define  EXMC_SIDHR_SIDHR_17                        ((uint32_t)0x00020000)        /*!< Bit 17 */
#define  EXMC_SIDHR_SIDHR_18                        ((uint32_t)0x00040000)        /*!< Bit 18 */
#define  EXMC_SIDHR_SIDHR_19                        ((uint32_t)0x00080000)        /*!< Bit 19 */
#define  EXMC_SIDHR_SIDHR_20                        ((uint32_t)0x00100000)        /*!< Bit 20 */
#define  EXMC_SIDHR_SIDHR_21                        ((uint32_t)0x00200000)        /*!< Bit 21 */
#define  EXMC_SIDHR_SIDHR_22                        ((uint32_t)0x00400000)        /*!< Bit 22 */
#define  EXMC_SIDHR_SIDHR_23                        ((uint32_t)0x00800000)        /*!< Bit 23 */
#define  EXMC_SIDLR_SIDHR_24                        ((uint32_t)0x01000000)        /*!< Bit 24 */
#define  EXMC_SIDLR_SIDHR_25                        ((uint32_t)0x02000000)        /*!< Bit 25 */
#define  EXMC_SIDLR_SIDHR_26                        ((uint32_t)0x04000000)        /*!< Bit 26 */
#define  EXMC_SIDLR_SIDHR_27                        ((uint32_t)0x08000000)        /*!< Bit 27 */
#define  EXMC_SIDLR_SIDHR_28                        ((uint32_t)0x10000000)        /*!< Bit 28 */
#define  EXMC_SIDLR_SIDHR_29                        ((uint32_t)0x20000000)        /*!< Bit 29 */
#define  EXMC_SIDLR_SIDLR_30                        ((uint32_t)0x40000000)        /*!< Bit 30 */
#define  EXMC_SIDLR_SIDLR_31                        ((uint32_t)0x80000000)        /*!< Bit 31 */

/****************** Bit definition for EXMC_SDCTLR1 register *****************/
#define  EXMC_SDCTLR1_CAW                           ((uint32_t)0x00000003)        /*!< CAW[1:0] bits (Column address bit width) */
#define  EXMC_SDCTLR1_CAW_0                         ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  EXMC_SDCTLR1_CAW_1                         ((uint32_t)0x00000002)        /*!< Bit 1 */

#define  EXMC_SDCTLR1_RAW                           ((uint32_t)0x0000000C)        /*!< RAW[1:0] bits (Row address bit width) */
#define  EXMC_SDCTLR1_RAW_0                         ((uint32_t)0x00000004)        /*!< Bit 0 */
#define  EXMC_SDCTLR1_RAW_1                         ((uint32_t)0x00000008)        /*!< Bit 1 */

#define  EXMC_SDCTLR1_MDW                          ((uint32_t)0x00000030)         /*!< MDW[1:0] bits (Memory data bus width) */
#define  EXMC_SDCTLR1_MDW_0                        ((uint32_t)0x00000010)         /*!< Bit 0 */
#define  EXMC_SDCTLR1_MDW_1                        ((uint32_t)0x00000020)         /*!< Bit 1 */

#define  EXMC_SDCTLR1_NIB                           ((uint32_t)0x00000040)        /*!< Number of internal banks */

#define  EXMC_SDCTLR1_CL                            ((uint32_t)0x00000180)        /*!< CL[1:0] bits (CAS Latency) */
#define  EXMC_SDCTLR1_CL_0                          ((uint32_t)0x00000080)        /*!< Bit 0 */
#define  EXMC_SDCTLR1_CL_1                          ((uint32_t)0x00000100)        /*!< Bit 1 */

#define  EXMC_SDCTLR1_WP                            ((uint32_t)0x00000200)        /*!< Write protection */

#define  EXMC_SDCTLR1_MCLK                          ((uint32_t)0x00000C00)        /*!< MCLK[1:0] bits (SDRAM memory clock configuration) */
#define  EXMC_SDCTLR1_MCLK_0                        ((uint32_t)0x00000400)        /*!< Bit 0 */
#define  EXMC_SDCTLR1_MCLK_1                        ((uint32_t)0x00000800)        /*!< Bit 1 */

#define  EXMC_SDCTLR1_BR                            ((uint32_t)0x00001000)        /*!< Burst read switch */

#define  EXMC_SDCTLR1_PRD                           ((uint32_t)0x00006000)        /*!< PRD[1:0] bits (Pipeline read data) */
#define  EXMC_SDCTLR1_PRD_0                         ((uint32_t)0x00002000)        /*!< Bit 0 */
#define  EXMC_SDCTLR1_PRD_1                         ((uint32_t)0x00004000)        /*!< Bit 1 */

/****************** Bit definition for EXMC_SDCTLR2 register *****************/
#define  EXMC_SDCTLR2_CAW                           ((uint32_t)0x00000003)        /*!< CAW[1:0] bits (Column address bit width) */
#define  EXMC_SDCTLR2_CAW_0                         ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  EXMC_SDCTLR2_CAW_1                         ((uint32_t)0x00000002)        /*!< Bit 1 */
                                 
#define  EXMC_SDCTLR2_RAW                           ((uint32_t)0x0000000C)        /*!< RAW[1:0] bits (Row address bit width) */
#define  EXMC_SDCTLR2_RAW_0                         ((uint32_t)0x00000004)        /*!< Bit 0 */
#define  EXMC_SDCTLR2_RAW_1                         ((uint32_t)0x00000008)        /*!< Bit 1 */
                                 
#define  EXMC_SDCTLR2_MDW                          ((uint32_t)0x00000030)         /*!< MDW[1:0] bits (Memory data bus width) */
#define  EXMC_SDCTLR2_MDW_0                        ((uint32_t)0x00000010)         /*!< Bit 0 */
#define  EXMC_SDCTLR2_MDW_1                        ((uint32_t)0x00000020)         /*!< Bit 1 */
                                 
#define  EXMC_SDCTLR2_NIB                           ((uint32_t)0x00000040)        /*!< Number of internal banks */
                                 
#define  EXMC_SDCTLR2_CL                            ((uint32_t)0x00000180)        /*!< CL[1:0] bits (CAS Latency) */
#define  EXMC_SDCTLR2_CL_0                          ((uint32_t)0x00000080)        /*!< Bit 0 */
#define  EXMC_SDCTLR2_CL_1                          ((uint32_t)0x00000100)        /*!< Bit 1 */
                                 
#define  EXMC_SDCTLR2_WP                            ((uint32_t)0x00000200)        /*!< Write protection */
                                 
#define  EXMC_SDCTLR2_MCLK                          ((uint32_t)0x00000C00)        /*!< MCLK[1:0] bits (SDRAM memory clock configuration) */
#define  EXMC_SDCTLR2_MCLK_0                        ((uint32_t)0x00000400)        /*!< Bit 0 */
#define  EXMC_SDCTLR2_MCLK_1                        ((uint32_t)0x00000800)        /*!< Bit 1 */
                                 
#define  EXMC_SDCTLR2_BR                            ((uint32_t)0x00001000)        /*!< Burst read switch */
                                 
#define  EXMC_SDCTLR2_PRD                           ((uint32_t)0x00006000)        /*!< PRD[1:0] bits (Pipeline read data) */
#define  EXMC_SDCTLR2_PRD_0                         ((uint32_t)0x00002000)        /*!< Bit 0 */
#define  EXMC_SDCTLR2_PRD_1                         ((uint32_t)0x00004000)        /*!< Bit 1 */

/******************  Bit definition for EXMC_SDTR1 register  *****************/
#define  EXMC_SDTR1_LMRD                            ((uint32_t)0x0000000F)        /*!< LMRD[3:0] bits (Load Mode Register Delay) */
#define  EXMC_SDTR1_LMRD_0                          ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  EXMC_SDTR1_LMRD_1                          ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  EXMC_SDTR1_LMRD_2                          ((uint32_t)0x00000004)        /*!< Bit 2 */
#define  EXMC_SDTR1_LMRD_3                          ((uint32_t)0x00000008)        /*!< Bit 3 */

#define  EXMC_SDTR1_XSRD                            ((uint32_t)0x000000F0)        /*!< XSRD[3:0] bits (Exit Self-refresh delay) */
#define  EXMC_SDTR1_XSRD_0                          ((uint32_t)0x00000010)        /*!< Bit 0 */
#define  EXMC_SDTR1_XSRD_1                          ((uint32_t)0x00000020)        /*!< Bit 1 */
#define  EXMC_SDTR1_XSRD_2                          ((uint32_t)0x00000040)        /*!< Bit 2 */
#define  EXMC_SDTR1_XSRD_3                          ((uint32_t)0x00000080)        /*!< Bit 3 */

#define  EXMC_SDTR1_RASD                            ((uint32_t)0x00000F00)        /*!< RASD[3:0] bits (Row address select delay) */
#define  EXMC_SDTR1_RASD_0                          ((uint32_t)0x00000100)        /*!< Bit 0 */
#define  EXMC_SDTR1_RASD_1                          ((uint32_t)0x00000200)        /*!< Bit 1 */
#define  EXMC_SDTR1_RASD_2                          ((uint32_t)0x00000400)        /*!< Bit 2 */
#define  EXMC_SDTR1_RASD_3                          ((uint32_t)0x00000800)        /*!< Bit 3 */

#define  EXMC_SDTR1_ARFD                            ((uint32_t)0x0000F000)        /*!< ARFD[3:0] bits (Auto refresh delay) */
#define  EXMC_SDTR1_ARFD_0                          ((uint32_t)0x00001000)        /*!< Bit 0 */
#define  EXMC_SDTR1_ARFD_1                          ((uint32_t)0x00002000)        /*!< Bit 1 */
#define  EXMC_SDTR1_ARFD_2                          ((uint32_t)0x00004000)        /*!< Bit 2 */
#define  EXMC_SDTR1_ARFD_3                          ((uint32_t)0x00008000)        /*!< Bit 3 */

#define  EXMC_SDTR1_WRD                             ((uint32_t)0x000F0000)        /*!< WRD[3:0] bits (Write recovery delay) */
#define  EXMC_SDTR1_WRD_0                           ((uint32_t)0x00010000)        /*!< Bit 0 */
#define  EXMC_SDTR1_WRD_1                           ((uint32_t)0x00020000)        /*!< Bit 1 */
#define  EXMC_SDTR1_WRD_2                           ((uint32_t)0x00040000)        /*!< Bit 2 */
#define  EXMC_SDTR1_WRD_3                           ((uint32_t)0x00080000)        /*!< Bit 3 */

#define  EXMC_SDTR1_RPD                             ((uint32_t)0x00F00000)        /*!< RPD[3:0] bits (Row precharge delay) */
#define  EXMC_SDTR1_RPD_0                           ((uint32_t)0x00100000)        /*!< Bit 0 */
#define  EXMC_SDTR1_RPD_1                           ((uint32_t)0x00200000)        /*!< Bit 1 */
#define  EXMC_SDTR1_RPD_2                           ((uint32_t)0x00400000)        /*!< Bit 2 */
#define  EXMC_SDTR1_RPD_3                           ((uint32_t)0x00800000)        /*!< Bit 3 */

#define  EXMC_SDTR1_RCD                             ((uint32_t)0x0F000000)        /*!< RCD[3:0] bits (Row to column delay) */
#define  EXMC_SDTR1_RCD_0                           ((uint32_t)0x01000000)        /*!< Bit 0 */
#define  EXMC_SDTR1_RCD_1                           ((uint32_t)0x02000000)        /*!< Bit 1 */
#define  EXMC_SDTR1_RCD_2                           ((uint32_t)0x04000000)        /*!< Bit 2 */
#define  EXMC_SDTR1_RCD_3                           ((uint32_t)0x08000000)        /*!< Bit 3 */

/******************  Bit definition for EXMC_SDTR2 register  *****************/
#define  EXMC_SDTR2_LMRD                            ((uint32_t)0x0000000F)        /*!< LMRD[3:0] bits (Load Mode Register Delay) */
#define  EXMC_SDTR2_LMRD_0                          ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  EXMC_SDTR2_LMRD_1                          ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  EXMC_SDTR2_LMRD_2                          ((uint32_t)0x00000004)        /*!< Bit 2 */
#define  EXMC_SDTR2_LMRD_3                          ((uint32_t)0x00000008)        /*!< Bit 3 */
                         
#define  EXMC_SDTR2_XSRD                            ((uint32_t)0x000000F0)        /*!< XSRD[3:0] bits (Exit Self-refresh delay) */
#define  EXMC_SDTR2_XSRD_0                          ((uint32_t)0x00000010)        /*!< Bit 0 */
#define  EXMC_SDTR2_XSRD_1                          ((uint32_t)0x00000020)        /*!< Bit 1 */
#define  EXMC_SDTR2_XSRD_2                          ((uint32_t)0x00000040)        /*!< Bit 2 */
#define  EXMC_SDTR2_XSRD_3                          ((uint32_t)0x00000080)        /*!< Bit 3 */
                         
#define  EXMC_SDTR2_RASD                            ((uint32_t)0x00000F00)        /*!< RASD[3:0] bits (Row address select delay) */
#define  EXMC_SDTR2_RASD_0                          ((uint32_t)0x00000100)        /*!< Bit 0 */
#define  EXMC_SDTR2_RASD_1                          ((uint32_t)0x00000200)        /*!< Bit 1 */
#define  EXMC_SDTR2_RASD_2                          ((uint32_t)0x00000400)        /*!< Bit 2 */
#define  EXMC_SDTR2_RASD_3                          ((uint32_t)0x00000800)        /*!< Bit 3 */
                         
#define  EXMC_SDTR2_ARFD                            ((uint32_t)0x0000F000)        /*!< ARFD[3:0] bits (Auto refresh delay) */
#define  EXMC_SDTR2_ARFD_0                          ((uint32_t)0x00001000)        /*!< Bit 0 */
#define  EXMC_SDTR2_ARFD_1                          ((uint32_t)0x00002000)        /*!< Bit 1 */
#define  EXMC_SDTR2_ARFD_2                          ((uint32_t)0x00004000)        /*!< Bit 2 */
#define  EXMC_SDTR2_ARFD_3                          ((uint32_t)0x00008000)        /*!< Bit 3 */
                         
#define  EXMC_SDTR2_WRD                             ((uint32_t)0x000F0000)        /*!< WRD[3:0] bits (Write recovery delay) */
#define  EXMC_SDTR2_WRD_0                           ((uint32_t)0x00010000)        /*!< Bit 0 */
#define  EXMC_SDTR2_WRD_1                           ((uint32_t)0x00020000)        /*!< Bit 1 */
#define  EXMC_SDTR2_WRD_2                           ((uint32_t)0x00040000)        /*!< Bit 2 */
#define  EXMC_SDTR2_WRD_3                           ((uint32_t)0x00080000)        /*!< Bit 3 */
                         
#define  EXMC_SDTR2_RPD                             ((uint32_t)0x00F00000)        /*!< RPD[3:0] bits (Row precharge delay) */
#define  EXMC_SDTR2_RPD_0                           ((uint32_t)0x00100000)        /*!< Bit 0 */
#define  EXMC_SDTR2_RPD_1                           ((uint32_t)0x00200000)        /*!< Bit 1 */
#define  EXMC_SDTR2_RPD_2                           ((uint32_t)0x00400000)        /*!< Bit 2 */
#define  EXMC_SDTR2_RPD_3                           ((uint32_t)0x00800000)        /*!< Bit 3 */
                         
#define  EXMC_SDTR2_RCD                             ((uint32_t)0x0F000000)        /*!< RCD[3:0] bits (Row to column delay) */
#define  EXMC_SDTR2_RCD_0                           ((uint32_t)0x01000000)        /*!< Bit 0 */
#define  EXMC_SDTR2_RCD_1                           ((uint32_t)0x02000000)        /*!< Bit 1 */
#define  EXMC_SDTR2_RCD_2                           ((uint32_t)0x04000000)        /*!< Bit 2 */
#define  EXMC_SDTR2_RCD_3                           ((uint32_t)0x08000000)        /*!< Bit 3 */

/******************  Bit definition for EXMC_SDCMDR register *****************/
#define  EXMC_SDCMDR_CMD                            ((uint32_t)0x00000007)        /*!< CMD[2:0] bits (Command) */
#define  EXMC_SDCMDR_CMD_0                          ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  EXMC_SDCMDR_CMD_1                          ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  EXMC_SDCMDR_CMD_2                          ((uint32_t)0x00000004)        /*!< Bit 2 */

#define  EXMC_SDCMDR_BS1                            ((uint32_t)0x00000008)        /*!< Bank select 2 (Bank 6) */

#define  EXMC_SDCMDR_BS0                            ((uint32_t)0x00000010)        /*!< Bank select 1 (Bank 5) */

#define  EXMC_SDCMDR_NARF                           ((uint32_t)0x000001E0)        /*!< NARF[3:0] bits (Number of successive Auto-refresh) */
#define  EXMC_SDCMDR_NARF_0                         ((uint32_t)0x00000020)        /*!< Bit 0 */
#define  EXMC_SDCMDR_NARF_1                         ((uint32_t)0x00000040)        /*!< Bit 1 */
#define  EXMC_SDCMDR_NARF_2                         ((uint32_t)0x00000080)        /*!< Bit 2 */
#define  EXMC_SDCMDR_NARF_3                         ((uint32_t)0x00000100)        /*!< Bit 3 */

#define  EXMC_SDCMDR_MRC                            ((uint32_t)0x003FFE00)        /*!< MRC[12:0] bits (Mode register content) */
#define  EXMC_SDCMDR_MRC_0                          ((uint32_t)0x00000200)        /*!< Bit 0 */
#define  EXMC_SDCMDR_MRC_1                          ((uint32_t)0x00000400)        /*!< Bit 1 */
#define  EXMC_SDCMDR_MRC_2                          ((uint32_t)0x00000800)        /*!< Bit 2 */
#define  EXMC_SDCMDR_MRC_3                          ((uint32_t)0x00001000)        /*!< Bit 3 */
#define  EXMC_SDCMDR_MRC_4                          ((uint32_t)0x00002000)        /*!< Bit 4 */
#define  EXMC_SDCMDR_MRC_5                          ((uint32_t)0x00004000)        /*!< Bit 5 */
#define  EXMC_SDCMDR_MRC_6                          ((uint32_t)0x00008000)        /*!< Bit 6 */
#define  EXMC_SDCMDR_MRC_7                          ((uint32_t)0x00010000)        /*!< Bit 7 */
#define  EXMC_SDCMDR_MRC_8                          ((uint32_t)0x00020000)        /*!< Bit 8 */
#define  EXMC_SDCMDR_MRC_9                          ((uint32_t)0x00040000)        /*!< Bit 9 */
#define  EXMC_SDCMDR_MRC_10                         ((uint32_t)0x00080000)        /*!< Bit 10 */
#define  EXMC_SDCMDR_MRC_11                         ((uint32_t)0x00100000)        /*!< Bit 11 */
#define  EXMC_SDCMDR_MRC_12                         ((uint32_t)0x00200000)        /*!< Bit 12 */

/****************** Bit definition for EXMC_SDARFIR register *****************/
#define  EXMC_SDARFIR_CRFE                          ((uint32_t)0x00000001)        /*!< Clear Refresh error flag */

#define  EXMC_SDARFIR_ARFITV                        ((uint32_t)0x00003FFE)        /*!< ARFITV[12:0] bits (Auto -Refresh Interval) */
#define  EXMC_SDARFIR_ARFITV_0                      ((uint32_t)0x00000002)        /*!< Bit 0 */
#define  EXMC_SDARFIR_ARFITV_1                      ((uint32_t)0x00000004)        /*!< Bit 1 */
#define  EXMC_SDARFIR_ARFITV_2                      ((uint32_t)0x00000008)        /*!< Bit 2 */
#define  EXMC_SDARFIR_ARFITV_3                      ((uint32_t)0x00000010)        /*!< Bit 3 */
#define  EXMC_SDARFIR_ARFITV_4                      ((uint32_t)0x00000020)        /*!< Bit 4 */
#define  EXMC_SDARFIR_ARFITV_5                      ((uint32_t)0x00000040)        /*!< Bit 5 */
#define  EXMC_SDARFIR_ARFITV_6                      ((uint32_t)0x00000080)        /*!< Bit 6 */
#define  EXMC_SDARFIR_ARFITV_7                      ((uint32_t)0x00000100)        /*!< Bit 7 */
#define  EXMC_SDARFIR_ARFITV_8                      ((uint32_t)0x00000200)        /*!< Bit 8 */
#define  EXMC_SDARFIR_ARFITV_9                      ((uint32_t)0x00000400)        /*!< Bit 9 */
#define  EXMC_SDARFIR_ARFITV_10                     ((uint32_t)0x00000800)        /*!< Bit 10 */
#define  EXMC_SDARFIR_ARFITV_11                     ((uint32_t)0x00001000)        /*!< Bit 11 */
#define  EXMC_SDARFIR_ARFITV_12                     ((uint32_t)0x00002000)        /*!< Bit 12 */

#define  EXMC_SDARFIR_RFEIE                         ((uint32_t)0x00004000)        /*!< Refresh error interrupt Enable */

/******************  Bit definition for EXMC_SDSTR register  ******************/
#define  EXMC_SDSTR_RFE                              ((uint32_t)0x00000001)       /*!< Refresh error flag */

#define  EXMC_SDSTR_STA1                             ((uint32_t)0x00000006)       /*!< STA1[1:0] bits (Bank 1 status) */
#define  EXMC_SDSTR_STA1_0                           ((uint32_t)0x00000002)       /*!< Bit 0 */
#define  EXMC_SDSTR_STA1_1                           ((uint32_t)0x00000004)       /*!< Bit 1 */

#define  EXMC_SDSTR_STA2                             ((uint32_t)0x00000018)       /*!< STA2[1:0] bits (Bank 2 status) */
#define  EXMC_SDSTR_STA2_0                           ((uint32_t)0x00000008)       /*!< Bit 0 */
#define  EXMC_SDSTR_STA2_1                           ((uint32_t)0x00000010)       /*!< Bit 1 */

#define  EXMC_SDTR_RDY                              ((uint32_t)0x00000020)        /*!< Ready status */

/****************** Bit definition for EXMC_SDRSCR register ******************/
#define  EXMC_SDRSCR_RSEN                           ((uint32_t)0x00000001)        /*!< Read sample Enable */
 
#define  EXMC_SDRSCR_SSCR                           ((uint32_t)0x00000002)        /*!< Select sample cycle of read data */

#define  EXMC_SDRSCR_SDSC                           ((uint32_t)0x000000F0)        /*!< SDSC[3:0] bits (Select the delayed sample clock of read data) */
#define  EXMC_SDRSCR_SDSC_0                         ((uint32_t)0x00000010)        /*!< Bit 0 */
#define  EXMC_SDRSCR_SDSC_1                         ((uint32_t)0x00000020)        /*!< Bit 1 */
#define  EXMC_SDRSCR_SDSC_2                         ((uint32_t)0x00000040)        /*!< Bit 2 */
#define  EXMC_SDRSCR_SDSC_3                         ((uint32_t)0x00000080)        /*!< Bit 3 */

/******************************************************************************/
/*                                                                            */
/*                               SystemTick                                   */
/*                                                                            */
/******************************************************************************/

/*****************  Bit definition for SysTick_CTRL register  *****************/
#define  SYSTICK_CTRL_ENABLE                        ((uint32_t)0x00000001)        /*!< Counter enable */
#define  SYSTICK_CTRL_TICKINT                       ((uint32_t)0x00000002)        /*!< Counting down to 0 pends the SysTick handler */
#define  SYSTICK_CTRL_CLKSOURCE                     ((uint32_t)0x00000004)        /*!< Clock source */
#define  SYSTICK_CTRL_COUNTFLAG                     ((uint32_t)0x00010000)        /*!< Count Flag */

/*****************  Bit definition for SysTick_LOAD register  *****************/
#define  SYSTICK_LOAD_RELOAD                        ((uint32_t)0x00FFFFFF)        /*!< Value to load into the SysTick Current Value Register when the counter reaches 0 */

/*****************  Bit definition for SysTick_VAL register  ******************/
#define  SYSTICK_VAL_CURRENT                        ((uint32_t)0x00FFFFFF)        /*!< Current value at the time the register is accessed */

/*****************  Bit definition for SysTick_CALIB register  ****************/
#define  SYSTICK_CALIB_TENMS                        ((uint32_t)0x00FFFFFF)        /*!< Reload value to use for 10ms timing */
#define  SYSTICK_CALIB_SKEW                         ((uint32_t)0x40000000)        /*!< Calibration value is not exactly 10 ms */
#define  SYSTICK_CALIB_NOREF                        ((uint32_t)0x80000000)        /*!< The reference clock is not provided */

/******************************************************************************/
/*                                                                            */
/*                  Nested Vectored Interrupt Controller                      */
/*                                                                            */
/******************************************************************************/

/******************  Bit definition for NVIC_ISER register  *******************/
#define  NVIC_ISER_SETENA                           ((uint32_t)0xFFFFFFFF)        /*!< Interrupt set enable bits */
#define  NVIC_ISER_SETENA_0                         ((uint32_t)0x00000001)        /*!< bit 0 */
#define  NVIC_ISER_SETENA_1                         ((uint32_t)0x00000002)        /*!< bit 1 */
#define  NVIC_ISER_SETENA_2                         ((uint32_t)0x00000004)        /*!< bit 2 */
#define  NVIC_ISER_SETENA_3                         ((uint32_t)0x00000008)        /*!< bit 3 */
#define  NVIC_ISER_SETENA_4                         ((uint32_t)0x00000010)        /*!< bit 4 */
#define  NVIC_ISER_SETENA_5                         ((uint32_t)0x00000020)        /*!< bit 5 */
#define  NVIC_ISER_SETENA_6                         ((uint32_t)0x00000040)        /*!< bit 6 */
#define  NVIC_ISER_SETENA_7                         ((uint32_t)0x00000080)        /*!< bit 7 */
#define  NVIC_ISER_SETENA_8                         ((uint32_t)0x00000100)        /*!< bit 8 */
#define  NVIC_ISER_SETENA_9                         ((uint32_t)0x00000200)        /*!< bit 9 */
#define  NVIC_ISER_SETENA_10                        ((uint32_t)0x00000400)        /*!< bit 10 */
#define  NVIC_ISER_SETENA_11                        ((uint32_t)0x00000800)        /*!< bit 11 */
#define  NVIC_ISER_SETENA_12                        ((uint32_t)0x00001000)        /*!< bit 12 */
#define  NVIC_ISER_SETENA_13                        ((uint32_t)0x00002000)        /*!< bit 13 */
#define  NVIC_ISER_SETENA_14                        ((uint32_t)0x00004000)        /*!< bit 14 */
#define  NVIC_ISER_SETENA_15                        ((uint32_t)0x00008000)        /*!< bit 15 */
#define  NVIC_ISER_SETENA_16                        ((uint32_t)0x00010000)        /*!< bit 16 */
#define  NVIC_ISER_SETENA_17                        ((uint32_t)0x00020000)        /*!< bit 17 */
#define  NVIC_ISER_SETENA_18                        ((uint32_t)0x00040000)        /*!< bit 18 */
#define  NVIC_ISER_SETENA_19                        ((uint32_t)0x00080000)        /*!< bit 19 */
#define  NVIC_ISER_SETENA_20                        ((uint32_t)0x00100000)        /*!< bit 20 */
#define  NVIC_ISER_SETENA_21                        ((uint32_t)0x00200000)        /*!< bit 21 */
#define  NVIC_ISER_SETENA_22                        ((uint32_t)0x00400000)        /*!< bit 22 */
#define  NVIC_ISER_SETENA_23                        ((uint32_t)0x00800000)        /*!< bit 23 */
#define  NVIC_ISER_SETENA_24                        ((uint32_t)0x01000000)        /*!< bit 24 */
#define  NVIC_ISER_SETENA_25                        ((uint32_t)0x02000000)        /*!< bit 25 */
#define  NVIC_ISER_SETENA_26                        ((uint32_t)0x04000000)        /*!< bit 26 */
#define  NVIC_ISER_SETENA_27                        ((uint32_t)0x08000000)        /*!< bit 27 */
#define  NVIC_ISER_SETENA_28                        ((uint32_t)0x10000000)        /*!< bit 28 */
#define  NVIC_ISER_SETENA_29                        ((uint32_t)0x20000000)        /*!< bit 29 */
#define  NVIC_ISER_SETENA_30                        ((uint32_t)0x40000000)        /*!< bit 30 */
#define  NVIC_ISER_SETENA_31                        ((uint32_t)0x80000000)        /*!< bit 31 */

/******************  Bit definition for NVIC_ICER register  *******************/
#define  NVIC_ICER_CLRENA                           ((uint32_t)0xFFFFFFFF)        /*!< Interrupt clear-enable bits */
#define  NVIC_ICER_CLRENA_0                         ((uint32_t)0x00000001)        /*!< bit 0 */
#define  NVIC_ICER_CLRENA_1                         ((uint32_t)0x00000002)        /*!< bit 1 */
#define  NVIC_ICER_CLRENA_2                         ((uint32_t)0x00000004)        /*!< bit 2 */
#define  NVIC_ICER_CLRENA_3                         ((uint32_t)0x00000008)        /*!< bit 3 */
#define  NVIC_ICER_CLRENA_4                         ((uint32_t)0x00000010)        /*!< bit 4 */
#define  NVIC_ICER_CLRENA_5                         ((uint32_t)0x00000020)        /*!< bit 5 */
#define  NVIC_ICER_CLRENA_6                         ((uint32_t)0x00000040)        /*!< bit 6 */
#define  NVIC_ICER_CLRENA_7                         ((uint32_t)0x00000080)        /*!< bit 7 */
#define  NVIC_ICER_CLRENA_8                         ((uint32_t)0x00000100)        /*!< bit 8 */
#define  NVIC_ICER_CLRENA_9                         ((uint32_t)0x00000200)        /*!< bit 9 */
#define  NVIC_ICER_CLRENA_10                        ((uint32_t)0x00000400)        /*!< bit 10 */
#define  NVIC_ICER_CLRENA_11                        ((uint32_t)0x00000800)        /*!< bit 11 */
#define  NVIC_ICER_CLRENA_12                        ((uint32_t)0x00001000)        /*!< bit 12 */
#define  NVIC_ICER_CLRENA_13                        ((uint32_t)0x00002000)        /*!< bit 13 */
#define  NVIC_ICER_CLRENA_14                        ((uint32_t)0x00004000)        /*!< bit 14 */
#define  NVIC_ICER_CLRENA_15                        ((uint32_t)0x00008000)        /*!< bit 15 */
#define  NVIC_ICER_CLRENA_16                        ((uint32_t)0x00010000)        /*!< bit 16 */
#define  NVIC_ICER_CLRENA_17                        ((uint32_t)0x00020000)        /*!< bit 17 */
#define  NVIC_ICER_CLRENA_18                        ((uint32_t)0x00040000)        /*!< bit 18 */
#define  NVIC_ICER_CLRENA_19                        ((uint32_t)0x00080000)        /*!< bit 19 */
#define  NVIC_ICER_CLRENA_20                        ((uint32_t)0x00100000)        /*!< bit 20 */
#define  NVIC_ICER_CLRENA_21                        ((uint32_t)0x00200000)        /*!< bit 21 */
#define  NVIC_ICER_CLRENA_22                        ((uint32_t)0x00400000)        /*!< bit 22 */
#define  NVIC_ICER_CLRENA_23                        ((uint32_t)0x00800000)        /*!< bit 23 */
#define  NVIC_ICER_CLRENA_24                        ((uint32_t)0x01000000)        /*!< bit 24 */
#define  NVIC_ICER_CLRENA_25                        ((uint32_t)0x02000000)        /*!< bit 25 */
#define  NVIC_ICER_CLRENA_26                        ((uint32_t)0x04000000)        /*!< bit 26 */
#define  NVIC_ICER_CLRENA_27                        ((uint32_t)0x08000000)        /*!< bit 27 */
#define  NVIC_ICER_CLRENA_28                        ((uint32_t)0x10000000)        /*!< bit 28 */
#define  NVIC_ICER_CLRENA_29                        ((uint32_t)0x20000000)        /*!< bit 29 */
#define  NVIC_ICER_CLRENA_30                        ((uint32_t)0x40000000)        /*!< bit 30 */
#define  NVIC_ICER_CLRENA_31                        ((uint32_t)0x80000000)        /*!< bit 31 */

/******************  Bit definition for NVIC_ISPR register  *******************/
#define  NVIC_ISPR_SETPEND                          ((uint32_t)0xFFFFFFFF)        /*!< Interrupt set-pending bits */
#define  NVIC_ISPR_SETPEND_0                        ((uint32_t)0x00000001)        /*!< bit 0 */
#define  NVIC_ISPR_SETPEND_1                        ((uint32_t)0x00000002)        /*!< bit 1 */
#define  NVIC_ISPR_SETPEND_2                        ((uint32_t)0x00000004)        /*!< bit 2 */
#define  NVIC_ISPR_SETPEND_3                        ((uint32_t)0x00000008)        /*!< bit 3 */
#define  NVIC_ISPR_SETPEND_4                        ((uint32_t)0x00000010)        /*!< bit 4 */
#define  NVIC_ISPR_SETPEND_5                        ((uint32_t)0x00000020)        /*!< bit 5 */
#define  NVIC_ISPR_SETPEND_6                        ((uint32_t)0x00000040)        /*!< bit 6 */
#define  NVIC_ISPR_SETPEND_7                        ((uint32_t)0x00000080)        /*!< bit 7 */
#define  NVIC_ISPR_SETPEND_8                        ((uint32_t)0x00000100)        /*!< bit 8 */
#define  NVIC_ISPR_SETPEND_9                        ((uint32_t)0x00000200)        /*!< bit 9 */
#define  NVIC_ISPR_SETPEND_10                       ((uint32_t)0x00000400)        /*!< bit 10 */
#define  NVIC_ISPR_SETPEND_11                       ((uint32_t)0x00000800)        /*!< bit 11 */
#define  NVIC_ISPR_SETPEND_12                       ((uint32_t)0x00001000)        /*!< bit 12 */
#define  NVIC_ISPR_SETPEND_13                       ((uint32_t)0x00002000)        /*!< bit 13 */
#define  NVIC_ISPR_SETPEND_14                       ((uint32_t)0x00004000)        /*!< bit 14 */
#define  NVIC_ISPR_SETPEND_15                       ((uint32_t)0x00008000)        /*!< bit 15 */
#define  NVIC_ISPR_SETPEND_16                       ((uint32_t)0x00010000)        /*!< bit 16 */
#define  NVIC_ISPR_SETPEND_17                       ((uint32_t)0x00020000)        /*!< bit 17 */
#define  NVIC_ISPR_SETPEND_18                       ((uint32_t)0x00040000)        /*!< bit 18 */
#define  NVIC_ISPR_SETPEND_19                       ((uint32_t)0x00080000)        /*!< bit 19 */
#define  NVIC_ISPR_SETPEND_20                       ((uint32_t)0x00100000)        /*!< bit 20 */
#define  NVIC_ISPR_SETPEND_21                       ((uint32_t)0x00200000)        /*!< bit 21 */
#define  NVIC_ISPR_SETPEND_22                       ((uint32_t)0x00400000)        /*!< bit 22 */
#define  NVIC_ISPR_SETPEND_23                       ((uint32_t)0x00800000)        /*!< bit 23 */
#define  NVIC_ISPR_SETPEND_24                       ((uint32_t)0x01000000)        /*!< bit 24 */
#define  NVIC_ISPR_SETPEND_25                       ((uint32_t)0x02000000)        /*!< bit 25 */
#define  NVIC_ISPR_SETPEND_26                       ((uint32_t)0x04000000)        /*!< bit 26 */
#define  NVIC_ISPR_SETPEND_27                       ((uint32_t)0x08000000)        /*!< bit 27 */
#define  NVIC_ISPR_SETPEND_28                       ((uint32_t)0x10000000)        /*!< bit 28 */
#define  NVIC_ISPR_SETPEND_29                       ((uint32_t)0x20000000)        /*!< bit 29 */
#define  NVIC_ISPR_SETPEND_30                       ((uint32_t)0x40000000)        /*!< bit 30 */
#define  NVIC_ISPR_SETPEND_31                       ((uint32_t)0x80000000)        /*!< bit 31 */

/******************  Bit definition for NVIC_ICPR register  *******************/
#define  NVIC_ICPR_CLRPEND                          ((uint32_t)0xFFFFFFFF)        /*!< Interrupt clear-pending bits */
#define  NVIC_ICPR_CLRPEND_0                        ((uint32_t)0x00000001)        /*!< bit 0 */
#define  NVIC_ICPR_CLRPEND_1                        ((uint32_t)0x00000002)        /*!< bit 1 */
#define  NVIC_ICPR_CLRPEND_2                        ((uint32_t)0x00000004)        /*!< bit 2 */
#define  NVIC_ICPR_CLRPEND_3                        ((uint32_t)0x00000008)        /*!< bit 3 */
#define  NVIC_ICPR_CLRPEND_4                        ((uint32_t)0x00000010)        /*!< bit 4 */
#define  NVIC_ICPR_CLRPEND_5                        ((uint32_t)0x00000020)        /*!< bit 5 */
#define  NVIC_ICPR_CLRPEND_6                        ((uint32_t)0x00000040)        /*!< bit 6 */
#define  NVIC_ICPR_CLRPEND_7                        ((uint32_t)0x00000080)        /*!< bit 7 */
#define  NVIC_ICPR_CLRPEND_8                        ((uint32_t)0x00000100)        /*!< bit 8 */
#define  NVIC_ICPR_CLRPEND_9                        ((uint32_t)0x00000200)        /*!< bit 9 */
#define  NVIC_ICPR_CLRPEND_10                       ((uint32_t)0x00000400)        /*!< bit 10 */
#define  NVIC_ICPR_CLRPEND_11                       ((uint32_t)0x00000800)        /*!< bit 11 */
#define  NVIC_ICPR_CLRPEND_12                       ((uint32_t)0x00001000)        /*!< bit 12 */
#define  NVIC_ICPR_CLRPEND_13                       ((uint32_t)0x00002000)        /*!< bit 13 */
#define  NVIC_ICPR_CLRPEND_14                       ((uint32_t)0x00004000)        /*!< bit 14 */
#define  NVIC_ICPR_CLRPEND_15                       ((uint32_t)0x00008000)        /*!< bit 15 */
#define  NVIC_ICPR_CLRPEND_16                       ((uint32_t)0x00010000)        /*!< bit 16 */
#define  NVIC_ICPR_CLRPEND_17                       ((uint32_t)0x00020000)        /*!< bit 17 */
#define  NVIC_ICPR_CLRPEND_18                       ((uint32_t)0x00040000)        /*!< bit 18 */
#define  NVIC_ICPR_CLRPEND_19                       ((uint32_t)0x00080000)        /*!< bit 19 */
#define  NVIC_ICPR_CLRPEND_20                       ((uint32_t)0x00100000)        /*!< bit 20 */
#define  NVIC_ICPR_CLRPEND_21                       ((uint32_t)0x00200000)        /*!< bit 21 */
#define  NVIC_ICPR_CLRPEND_22                       ((uint32_t)0x00400000)        /*!< bit 22 */
#define  NVIC_ICPR_CLRPEND_23                       ((uint32_t)0x00800000)        /*!< bit 23 */
#define  NVIC_ICPR_CLRPEND_24                       ((uint32_t)0x01000000)        /*!< bit 24 */
#define  NVIC_ICPR_CLRPEND_25                       ((uint32_t)0x02000000)        /*!< bit 25 */
#define  NVIC_ICPR_CLRPEND_26                       ((uint32_t)0x04000000)        /*!< bit 26 */
#define  NVIC_ICPR_CLRPEND_27                       ((uint32_t)0x08000000)        /*!< bit 27 */
#define  NVIC_ICPR_CLRPEND_28                       ((uint32_t)0x10000000)        /*!< bit 28 */
#define  NVIC_ICPR_CLRPEND_29                       ((uint32_t)0x20000000)        /*!< bit 29 */
#define  NVIC_ICPR_CLRPEND_30                       ((uint32_t)0x40000000)        /*!< bit 30 */
#define  NVIC_ICPR_CLRPEND_31                       ((uint32_t)0x80000000)        /*!< bit 31 */
								         
/******************  Bit definition for NVIC_IABR register  *******************/
#define  NVIC_IABR_ACTIVE                           ((uint32_t)0xFFFFFFFF)        /*!< Interrupt active flags */
#define  NVIC_IABR_ACTIVE_0                         ((uint32_t)0x00000001)        /*!< bit 0 */
#define  NVIC_IABR_ACTIVE_1                         ((uint32_t)0x00000002)        /*!< bit 1 */
#define  NVIC_IABR_ACTIVE_2                         ((uint32_t)0x00000004)        /*!< bit 2 */
#define  NVIC_IABR_ACTIVE_3                         ((uint32_t)0x00000008)        /*!< bit 3 */
#define  NVIC_IABR_ACTIVE_4                         ((uint32_t)0x00000010)        /*!< bit 4 */
#define  NVIC_IABR_ACTIVE_5                         ((uint32_t)0x00000020)        /*!< bit 5 */
#define  NVIC_IABR_ACTIVE_6                         ((uint32_t)0x00000040)        /*!< bit 6 */
#define  NVIC_IABR_ACTIVE_7                         ((uint32_t)0x00000080)        /*!< bit 7 */
#define  NVIC_IABR_ACTIVE_8                         ((uint32_t)0x00000100)        /*!< bit 8 */
#define  NVIC_IABR_ACTIVE_9                         ((uint32_t)0x00000200)        /*!< bit 9 */
#define  NVIC_IABR_ACTIVE_10                        ((uint32_t)0x00000400)        /*!< bit 10 */
#define  NVIC_IABR_ACTIVE_11                        ((uint32_t)0x00000800)        /*!< bit 11 */
#define  NVIC_IABR_ACTIVE_12                        ((uint32_t)0x00001000)        /*!< bit 12 */
#define  NVIC_IABR_ACTIVE_13                        ((uint32_t)0x00002000)        /*!< bit 13 */
#define  NVIC_IABR_ACTIVE_14                        ((uint32_t)0x00004000)        /*!< bit 14 */
#define  NVIC_IABR_ACTIVE_15                        ((uint32_t)0x00008000)        /*!< bit 15 */
#define  NVIC_IABR_ACTIVE_16                        ((uint32_t)0x00010000)        /*!< bit 16 */
#define  NVIC_IABR_ACTIVE_17                        ((uint32_t)0x00020000)        /*!< bit 17 */
#define  NVIC_IABR_ACTIVE_18                        ((uint32_t)0x00040000)        /*!< bit 18 */
#define  NVIC_IABR_ACTIVE_19                        ((uint32_t)0x00080000)        /*!< bit 19 */
#define  NVIC_IABR_ACTIVE_20                        ((uint32_t)0x00100000)        /*!< bit 20 */
#define  NVIC_IABR_ACTIVE_21                        ((uint32_t)0x00200000)        /*!< bit 21 */
#define  NVIC_IABR_ACTIVE_22                        ((uint32_t)0x00400000)        /*!< bit 22 */
#define  NVIC_IABR_ACTIVE_23                        ((uint32_t)0x00800000)        /*!< bit 23 */
#define  NVIC_IABR_ACTIVE_24                        ((uint32_t)0x01000000)        /*!< bit 24 */
#define  NVIC_IABR_ACTIVE_25                        ((uint32_t)0x02000000)        /*!< bit 25 */
#define  NVIC_IABR_ACTIVE_26                        ((uint32_t)0x04000000)        /*!< bit 26 */
#define  NVIC_IABR_ACTIVE_27                        ((uint32_t)0x08000000)        /*!< bit 27 */
#define  NVIC_IABR_ACTIVE_28                        ((uint32_t)0x10000000)        /*!< bit 28 */
#define  NVIC_IABR_ACTIVE_29                        ((uint32_t)0x20000000)        /*!< bit 29 */
#define  NVIC_IABR_ACTIVE_30                        ((uint32_t)0x40000000)        /*!< bit 30 */
#define  NVIC_IABR_ACTIVE_31                        ((uint32_t)0x80000000)        /*!< bit 31 */

/******************  Bit definition for NVIC_PRI0 register  *******************/
#define  NVIC_IPR0_PRI_0                            ((uint32_t)0x000000FF)        /*!< Priority of interrupt 0 */
#define  NVIC_IPR0_PRI_1                            ((uint32_t)0x0000FF00)        /*!< Priority of interrupt 1 */
#define  NVIC_IPR0_PRI_2                            ((uint32_t)0x00FF0000)        /*!< Priority of interrupt 2 */
#define  NVIC_IPR0_PRI_3                            ((uint32_t)0xFF000000)        /*!< Priority of interrupt 3 */
									        
/******************  Bit definition for NVIC_PRI1 register  *******************/
#define  NVIC_IPR1_PRI_4                            ((uint32_t)0x000000FF)        /*!< Priority of interrupt 4 */
#define  NVIC_IPR1_PRI_5                            ((uint32_t)0x0000FF00)        /*!< Priority of interrupt 5 */
#define  NVIC_IPR1_PRI_6                            ((uint32_t)0x00FF0000)        /*!< Priority of interrupt 6 */
#define  NVIC_IPR1_PRI_7                            ((uint32_t)0xFF000000)        /*!< Priority of interrupt 7 */

/******************  Bit definition for NVIC_PRI2 register  *******************/
#define  NVIC_IPR2_PRI_8                            ((uint32_t)0x000000FF)        /*!< Priority of interrupt 8 */
#define  NVIC_IPR2_PRI_9                            ((uint32_t)0x0000FF00)        /*!< Priority of interrupt 9 */
#define  NVIC_IPR2_PRI_10                           ((uint32_t)0x00FF0000)        /*!< Priority of interrupt 10 */
#define  NVIC_IPR2_PRI_11                           ((uint32_t)0xFF000000)        /*!< Priority of interrupt 11 */
								         
/******************  Bit definition for NVIC_PRI3 register  *******************/
#define  NVIC_IPR3_PRI_12                           ((uint32_t)0x000000FF)        /*!< Priority of interrupt 12 */
#define  NVIC_IPR3_PRI_13                           ((uint32_t)0x0000FF00)        /*!< Priority of interrupt 13 */
#define  NVIC_IPR3_PRI_14                           ((uint32_t)0x00FF0000)        /*!< Priority of interrupt 14 */
#define  NVIC_IPR3_PRI_15                           ((uint32_t)0xFF000000)        /*!< Priority of interrupt 15 */
								         
/******************  Bit definition for NVIC_PRI4 register  *******************/
#define  NVIC_IPR4_PRI_16                           ((uint32_t)0x000000FF)        /*!< Priority of interrupt 16 */
#define  NVIC_IPR4_PRI_17                           ((uint32_t)0x0000FF00)        /*!< Priority of interrupt 17 */
#define  NVIC_IPR4_PRI_18                           ((uint32_t)0x00FF0000)        /*!< Priority of interrupt 18 */
#define  NVIC_IPR4_PRI_19                           ((uint32_t)0xFF000000)        /*!< Priority of interrupt 19 */

/******************  Bit definition for NVIC_PRI5 register  *******************/
#define  NVIC_IPR5_PRI_20                           ((uint32_t)0x000000FF)        /*!< Priority of interrupt 20 */
#define  NVIC_IPR5_PRI_21                           ((uint32_t)0x0000FF00)        /*!< Priority of interrupt 21 */
#define  NVIC_IPR5_PRI_22                           ((uint32_t)0x00FF0000)        /*!< Priority of interrupt 22 */
#define  NVIC_IPR5_PRI_23                           ((uint32_t)0xFF000000)        /*!< Priority of interrupt 23 */

/******************  Bit definition for NVIC_PRI6 register  *******************/
#define  NVIC_IPR6_PRI_24                           ((uint32_t)0x000000FF)        /*!< Priority of interrupt 24 */
#define  NVIC_IPR6_PRI_25                           ((uint32_t)0x0000FF00)        /*!< Priority of interrupt 25 */
#define  NVIC_IPR6_PRI_26                           ((uint32_t)0x00FF0000)        /*!< Priority of interrupt 26 */
#define  NVIC_IPR6_PRI_27                           ((uint32_t)0xFF000000)        /*!< Priority of interrupt 27 */

/******************  Bit definition for NVIC_PRI7 register  *******************/
#define  NVIC_IPR7_PRI_28                           ((uint32_t)0x000000FF)        /*!< Priority of interrupt 28 */
#define  NVIC_IPR7_PRI_29                           ((uint32_t)0x0000FF00)        /*!< Priority of interrupt 29 */
#define  NVIC_IPR7_PRI_30                           ((uint32_t)0x00FF0000)        /*!< Priority of interrupt 30 */
#define  NVIC_IPR7_PRI_31                           ((uint32_t)0xFF000000)        /*!< Priority of interrupt 31 */

/******************  Bit definition for SCB_CPUID register  *******************/
#define  SCB_CPUID_REVISION                         ((uint32_t)0x0000000F)        /*!< Implementation defined revision number */
#define  SCB_CPUID_PARTNO                           ((uint32_t)0x0000FFF0)        /*!< Number of processor within family */
#define  SCB_CPUID_Constant                         ((uint32_t)0x000F0000)        /*!< Reads as 0x0F */
#define  SCB_CPUID_VARIANT                          ((uint32_t)0x00F00000)        /*!< Implementation defined variant number */
#define  SCB_CPUID_IMPLEMENTER                      ((uint32_t)0xFF000000)        /*!< Implementer code. ARM is 0x41 */

/*******************  Bit definition for SCB_ICSR register  *******************/
#define  SCB_ICSR_VECTACTIVE                        ((uint32_t)0x000001FF)        /*!< Active ISR number field */
#define  SCB_ICSR_RETTOBASE                         ((uint32_t)0x00000800)        /*!< All active exceptions minus the IPSR_current_exception yields the empty set */
#define  SCB_ICSR_VECTPENDING                       ((uint32_t)0x003FF000)        /*!< Pending ISR number field */
#define  SCB_ICSR_ISRPENDING                        ((uint32_t)0x00400000)        /*!< Interrupt pending flag */
#define  SCB_ICSR_ISRPREEMPT                        ((uint32_t)0x00800000)        /*!< It indicates that a pending interrupt becomes active in the next running cycle */
#define  SCB_ICSR_PENDSTCLR                         ((uint32_t)0x02000000)        /*!< Clear pending SysTick bit */
#define  SCB_ICSR_PENDSTSET                         ((uint32_t)0x04000000)        /*!< Set pending SysTick bit */
#define  SCB_ICSR_PENDSVCLR                         ((uint32_t)0x08000000)        /*!< Clear pending pendSV bit */
#define  SCB_ICSR_PENDSVSET                         ((uint32_t)0x10000000)        /*!< Set pending pendSV bit */
#define  SCB_ICSR_NMIPENDSET                        ((uint32_t)0x80000000)        /*!< Set pending NMI bit */

/*******************  Bit definition for SCB_VTOR register  *******************/
#define  SCB_VTOR_TBLOFF                            ((uint32_t)0x1FFFFF80)        /*!< Vector table base offset field */
#define  SCB_VTOR_TBLBASE                           ((uint32_t)0x20000000)        /*!< Table base in code(0) or RAM(1) */
									       
/*!<*****************  Bit definition for SCB_AIRCR register  *******************/
#define  SCB_AIRCR_VECTRESET                        ((uint32_t)0x00000001)        /*!< System Reset bit */
#define  SCB_AIRCR_VECTCLRACTIVE                    ((uint32_t)0x00000002)        /*!< Clear active vector bit */
#define  SCB_AIRCR_SYSRESETREQ                      ((uint32_t)0x00000004)        /*!< Requests chip control logic to generate a reset */

#define  SCB_AIRCR_PRIGROUP                         ((uint32_t)0x00000700)        /*!< PRIGROUP[2:0] bits (Priority group) */
#define  SCB_AIRCR_PRIGROUP_0                       ((uint32_t)0x00000100)        /*!< Bit 0 */
#define  SCB_AIRCR_PRIGROUP_1                       ((uint32_t)0x00000200)        /*!< Bit 1 */
#define  SCB_AIRCR_PRIGROUP_2                       ((uint32_t)0x00000400)        /*!< Bit 2  */

/* prority group configuration */
#define  SCB_AIRCR_PRIGROUP0                        ((uint32_t)0x00000000)        /*!< Priority group=0 (7 bits of pre-emption priority, 1 bit of subpriority) */
#define  SCB_AIRCR_PRIGROUP1                        ((uint32_t)0x00000100)        /*!< Priority group=1 (6 bits of pre-emption priority, 2 bits of subpriority) */
#define  SCB_AIRCR_PRIGROUP2                        ((uint32_t)0x00000200)        /*!< Priority group=2 (5 bits of pre-emption priority, 3 bits of subpriority) */
#define  SCB_AIRCR_PRIGROUP3                        ((uint32_t)0x00000300)        /*!< Priority group=3 (4 bits of pre-emption priority, 4 bits of subpriority) */
#define  SCB_AIRCR_PRIGROUP4                        ((uint32_t)0x00000400)        /*!< Priority group=4 (3 bits of pre-emption priority, 5 bits of subpriority) */
#define  SCB_AIRCR_PRIGROUP5                        ((uint32_t)0x00000500)        /*!< Priority group=5 (2 bits of pre-emption priority, 6 bits of subpriority) */
#define  SCB_AIRCR_PRIGROUP6                        ((uint32_t)0x00000600)        /*!< Priority group=6 (1 bit of pre-emption priority, 7 bits of subpriority) */
#define  SCB_AIRCR_PRIGROUP7                        ((uint32_t)0x00000700)        /*!< Priority group=7 (no pre-emption priority, 8 bits of subpriority) */

#define  SCB_AIRCR_ENDIANESS                        ((uint32_t)0x00008000)        /*!< Data endianness bit */
#define  SCB_AIRCR_VECTKEY                          ((uint32_t)0xFFFF0000)        /*!< Register key (VECTKEY) - Reads as 0xFA05 (VECTKEYSTAT) */
								          
/*******************  Bit definition for SCB_SCR register  ********************/
#define  SCB_SCR_SLEEPONEXIT                        ((uint8_t)0x02)               /*!< Sleep on exit bit */
#define  SCB_SCR_SLEEPDEEP                          ((uint8_t)0x04)               /*!< Sleep deep bit */
#define  SCB_SCR_SEVONPEND                          ((uint8_t)0x10)               /*!< Wake up from WFE */

/********************  Bit definition for SCB_CCR register  *******************/
#define  SCB_CCR_NONBASETHRDENA                     ((uint16_t)0x0001)            /*!< Thread mode can be entered from any level in Handler mode by controlled return value */
#define  SCB_CCR_USERSETMPEND                       ((uint16_t)0x0002)            /*!< Enables user code to write the Software Trigger Interrupt register to trigger (pend) a Main exception */
#define  SCB_CCR_UNALIGN_TRP                        ((uint16_t)0x0008)            /*!< Trap for unaligned access */
#define  SCB_CCR_DIV_0_TRP                          ((uint16_t)0x0010)            /*!< Trap on Divide by 0 */
#define  SCB_CCR_BFHFNMIGN                          ((uint16_t)0x0100)            /*!< Handlers running at priority -1 and -2 */
#define  SCB_CCR_STKALIGN                           ((uint16_t)0x0200)            /*!< On exception entry, the SP used prior to the exception is adjusted to be 8-byte aligned */

/*******************  Bit definition for SCB_SHPR register ********************/
#define  SCB_SHPR_PRI_N                             ((uint32_t)0x000000FF)        /*!< Priority of system handler 4,8, and 12. Mem Manage, reserved and Debug Monitor */
#define  SCB_SHPR_PRI_N1                            ((uint32_t)0x0000FF00)        /*!< Priority of system handler 5,9, and 13. Bus Fault, reserved and reserved */
#define  SCB_SHPR_PRI_N2                            ((uint32_t)0x00FF0000)        /*!< Priority of system handler 6,10, and 14. Usage Fault, reserved and PendSV */
#define  SCB_SHPR_PRI_N3                            ((uint32_t)0xFF000000)        /*!< Priority of system handler 7,11, and 15. Reserved, SVCall and SysTick */
									       
/******************  Bit definition for SCB_SHCSR register  *******************/
#define  SCB_SHCSR_MEMFAULTACT                      ((uint32_t)0x00000001)        /*!< MemManage is active */
#define  SCB_SHCSR_BUSFAULTACT                      ((uint32_t)0x00000002)        /*!< BusFault is active */
#define  SCB_SHCSR_USGFAULTACT                      ((uint32_t)0x00000008)        /*!< UsageFault is active */
#define  SCB_SHCSR_SVCALLACT                        ((uint32_t)0x00000080)        /*!< SVCall is active */
#define  SCB_SHCSR_MONITORACT                       ((uint32_t)0x00000100)        /*!< Monitor is active */
#define  SCB_SHCSR_PENDSVACT                        ((uint32_t)0x00000400)        /*!< PendSV is active */
#define  SCB_SHCSR_SYSTICKACT                       ((uint32_t)0x00000800)        /*!< SysTick is active */
#define  SCB_SHCSR_USGFAULTPENDED                   ((uint32_t)0x00001000)        /*!< Usage Fault is pended */
#define  SCB_SHCSR_MEMFAULTPENDED                   ((uint32_t)0x00002000)        /*!< MemManage is pended */
#define  SCB_SHCSR_BUSFAULTPENDED                   ((uint32_t)0x00004000)        /*!< Bus Fault is pended */
#define  SCB_SHCSR_SVCALLPENDED                     ((uint32_t)0x00008000)        /*!< SVCall is pended */
#define  SCB_SHCSR_MEMFAULTENA                      ((uint32_t)0x00010000)        /*!< MemManage enable */
#define  SCB_SHCSR_BUSFAULTENA                      ((uint32_t)0x00020000)        /*!< Bus Fault enable */
#define  SCB_SHCSR_USGFAULTENA                      ((uint32_t)0x00040000)        /*!< UsageFault enable */

/*******************  Bit definition for SCB_CFSR register  *******************/
/*!< MFSR */
#define  SCB_CFSR_IACCVIOL                          ((uint32_t)0x00000001)        /*!< Instruction access violation */
#define  SCB_CFSR_DACCVIOL                          ((uint32_t)0x00000002)        /*!< Data access violation */
#define  SCB_CFSR_MUNSTKERR                         ((uint32_t)0x00000008)        /*!< Unstacking error */
#define  SCB_CFSR_MSTKERR                           ((uint32_t)0x00000010)        /*!< Stacking error */
#define  SCB_CFSR_MMARVALID                         ((uint32_t)0x00000080)        /*!< Memory Manage Address Register address valid flag */
/*!< BFSR */
#define  SCB_CFSR_IBUSERR                           ((uint32_t)0x00000100)        /*!< Instruction bus error flag */
#define  SCB_CFSR_PRECISERR                         ((uint32_t)0x00000200)        /*!< Precise data bus error */
#define  SCB_CFSR_IMPRECISERR                       ((uint32_t)0x00000400)        /*!< Imprecise data bus error */
#define  SCB_CFSR_UNSTKERR                          ((uint32_t)0x00000800)        /*!< Unstacking error */
#define  SCB_CFSR_STKERR                            ((uint32_t)0x00001000)        /*!< Stacking error */
#define  SCB_CFSR_BFARVALID                         ((uint32_t)0x00008000)        /*!< Bus Fault Address Register address valid flag */
/*!< UFSR */
#define  SCB_CFSR_UNDEFINSTR                        ((uint32_t)0x00010000)        /*!< The processor attempt to execute an undefined instruction */
#define  SCB_CFSR_INVSTATE                          ((uint32_t)0x00020000)        /*!< Invalid combination of EPSR and instruction */
#define  SCB_CFSR_INVPC                             ((uint32_t)0x00040000)        /*!< Attempt to load EXC_RETURN into pc illegally */
#define  SCB_CFSR_NOCP                              ((uint32_t)0x00080000)        /*!< Attempt to use a coprocessor instruction */
#define  SCB_CFSR_UNALIGNED                         ((uint32_t)0x01000000)        /*!< Fault occurs when there is an attempt to make an unaligned memory access */
#define  SCB_CFSR_DIVBYZERO                         ((uint32_t)0x02000000)        /*!< Fault occurs when SDIV or DIV instruction is used with a divisor of 0 */

/*******************  Bit definition for SCB_HFSR register  *******************/
#define  SCB_HFSR_VECTTBL                           ((uint32_t)0x00000002)        /*!< Fault occurs because of vector table read on exception processing */
#define  SCB_HFSR_FORCED                            ((uint32_t)0x40000000)        /*!< Hard Fault activated when a configurable Fault was received and cannot activate */
#define  SCB_HFSR_DEBUGEVT                          ((uint32_t)0x80000000)        /*!< Fault related to debug */

/*******************  Bit definition for SCB_DFSR register  *******************/
#define  SCB_DFSR_HALTED                            ((uint8_t)0x01)               /*!< Halt request flag */
#define  SCB_DFSR_BKPT                              ((uint8_t)0x02)               /*!< BKPT flag */
#define  SCB_DFSR_DWTTRAP                           ((uint8_t)0x04)               /*!< Data Watchpoint and Trace (DWT) flag */
#define  SCB_DFSR_VCATCH                            ((uint8_t)0x08)               /*!< Vector catch flag */
#define  SCB_DFSR_EXTERNAL                          ((uint8_t)0x10)               /*!< External debug request flag */

/*******************  Bit definition for SCB_MMFAR register  ******************/
#define  SCB_MMFAR_ADDRESS                          ((uint32_t)0xFFFFFFFF)        /*!< Mem Manage fault address field */

/*******************  Bit definition for SCB_BFAR register  *******************/
#define  SCB_BFAR_ADDRESS                           ((uint32_t)0xFFFFFFFF)        /*!< Bus fault address field */

/*******************  Bit definition for SCB_afsr register  *******************/
#define  SCB_AFSR_IMPDEF                            ((uint32_t)0xFFFFFFFF)        /*!< Implementation defined */


/******************************************************************************/
/*                                                                            */
/*                 External Interrupt/Event Controller (EXTI)                 */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for EXTI_IER register  *******************/
#define  EXTI_IER_IER0                              ((uint32_t)0x00000001)        /*!< Interrupt Enable Control on line 0 */
#define  EXTI_IER_IER1                              ((uint32_t)0x00000002)        /*!< Interrupt Enable Control on line 1 */
#define  EXTI_IER_IER2                              ((uint32_t)0x00000004)        /*!< Interrupt Enable Control on line 2 */
#define  EXTI_IER_IER3                              ((uint32_t)0x00000008)        /*!< Interrupt Enable Control on line 3 */
#define  EXTI_IER_IER4                              ((uint32_t)0x00000010)        /*!< Interrupt Enable Control on line 4 */
#define  EXTI_IER_IER5                              ((uint32_t)0x00000020)        /*!< Interrupt Enable Control on line 5 */
#define  EXTI_IER_IER6                              ((uint32_t)0x00000040)        /*!< Interrupt Enable Control on line 6 */
#define  EXTI_IER_IER7                              ((uint32_t)0x00000080)        /*!< Interrupt Enable Control on line 7 */
#define  EXTI_IER_IER8                              ((uint32_t)0x00000100)        /*!< Interrupt Enable Control on line 8 */
#define  EXTI_IER_IER9                              ((uint32_t)0x00000200)        /*!< Interrupt Enable Control on line 9 */
#define  EXTI_IER_IER10                             ((uint32_t)0x00000400)        /*!< Interrupt Enable Control on line 10 */
#define  EXTI_IER_IER11                             ((uint32_t)0x00000800)        /*!< Interrupt Enable Control on line 11 */
#define  EXTI_IER_IER12                             ((uint32_t)0x00001000)        /*!< Interrupt Enable Control on line 12 */
#define  EXTI_IER_IER13                             ((uint32_t)0x00002000)        /*!< Interrupt Enable Control on line 13 */
#define  EXTI_IER_IER14                             ((uint32_t)0x00004000)        /*!< Interrupt Enable Control on line 14 */
#define  EXTI_IER_IER15                             ((uint32_t)0x00008000)        /*!< Interrupt Enable Control on line 15 */
#define  EXTI_IER_IER16                             ((uint32_t)0x00010000)        /*!< Interrupt Enable Control on line 16 */
#define  EXTI_IER_IER17                             ((uint32_t)0x00020000)        /*!< Interrupt Enable Control on line 17 */
#define  EXTI_IER_IER18                             ((uint32_t)0x00040000)        /*!< Interrupt Enable Control on line 18 */

#define  EXTI_IER_IER19                             ((uint32_t)0x00080000)        /*!< Interrupt Enable Control on line 19 */

/******************  Bit definition for EXTI_EER register  ********************/
#define  EXTI_EER_EER0                              ((uint32_t)0x00000001)        /*!< Event Enable Control on line 0 */
#define  EXTI_EER_EER1                              ((uint32_t)0x00000002)        /*!< Event Enable Control on line 1 */
#define  EXTI_EER_EER2                              ((uint32_t)0x00000004)        /*!< Event Enable Control on line 2 */
#define  EXTI_EER_EER3                              ((uint32_t)0x00000008)        /*!< Event Enable Control on line 3 */
#define  EXTI_EER_EER4                              ((uint32_t)0x00000010)        /*!< Event Enable Control on line 4 */
#define  EXTI_EER_EER5                              ((uint32_t)0x00000020)        /*!< Event Enable Control on line 5 */
#define  EXTI_EER_EER6                              ((uint32_t)0x00000040)        /*!< Event Enable Control on line 6 */
#define  EXTI_EER_EER7                              ((uint32_t)0x00000080)        /*!< Event Enable Control on line 7 */
#define  EXTI_EER_EER8                              ((uint32_t)0x00000100)        /*!< Event Enable Control on line 8 */
#define  EXTI_EER_EER9                              ((uint32_t)0x00000200)        /*!< Event Enable Control on line 9 */
#define  EXTI_EER_EER10                             ((uint32_t)0x00000400)        /*!< Event Enable Control on line 10 */
#define  EXTI_EER_EER11                             ((uint32_t)0x00000800)        /*!< Event Enable Control on line 11 */
#define  EXTI_EER_EER12                             ((uint32_t)0x00001000)        /*!< Event Enable Control on line 12 */
#define  EXTI_EER_EER13                             ((uint32_t)0x00002000)        /*!< Event Enable Control on line 13 */
#define  EXTI_EER_EER14                             ((uint32_t)0x00004000)        /*!< Event Enable Control on line 14 */
#define  EXTI_EER_EER15                             ((uint32_t)0x00008000)        /*!< Event Enable Control on line 15 */
#define  EXTI_EER_EER16                             ((uint32_t)0x00010000)        /*!< Event Enable Control on line 16 */
#define  EXTI_EER_EER17                             ((uint32_t)0x00020000)        /*!< Event Enable Control on line 17 */
#define  EXTI_EER_EER18                             ((uint32_t)0x00040000)        /*!< Event Enable Control on line 18 */

#define  EXTI_EER_EER19                             ((uint32_t)0x00080000)        /*!< Event Enable Control on line 19 */

/*******************  Bit definition for EXTI_RTE register  ******************/
#define  EXTI_RTE_RTE0                              ((uint32_t)0x00000001)        /*!< Rising Edge Trigger Enabled for line 0 */
#define  EXTI_RTE_RTE1                              ((uint32_t)0x00000002)        /*!< Rising Edge Trigger Enabled for line 1 */
#define  EXTI_RTE_RTE2                              ((uint32_t)0x00000004)        /*!< Rising Edge Trigger Enabled for line 2 */
#define  EXTI_RTE_RTE3                              ((uint32_t)0x00000008)        /*!< Rising Edge Trigger Enabled for line 3 */
#define  EXTI_RTE_RTE4                              ((uint32_t)0x00000010)        /*!< Rising Edge Trigger Enabled for line 4 */
#define  EXTI_RTE_RTE5                              ((uint32_t)0x00000020)        /*!< Rising Edge Trigger Enabled for line 5 */
#define  EXTI_RTE_RTE6                              ((uint32_t)0x00000040)        /*!< Rising Edge Trigger Enabled for line 6 */
#define  EXTI_RTE_RTE7                              ((uint32_t)0x00000080)        /*!< Rising Edge Trigger Enabled for line 7 */
#define  EXTI_RTE_RTE8                              ((uint32_t)0x00000100)        /*!< Rising Edge Trigger Enabled for line 8 */
#define  EXTI_RTE_RTE9                              ((uint32_t)0x00000200)        /*!< Rising Edge Trigger Enabled for line 9 */
#define  EXTI_RTE_RTE10                             ((uint32_t)0x00000400)        /*!< Rising Edge Trigger Enabled for line 10 */
#define  EXTI_RTE_RTE11                             ((uint32_t)0x00000800)        /*!< Rising Edge Trigger Enabled for line 11 */
#define  EXTI_RTE_RTE12                             ((uint32_t)0x00001000)        /*!< Rising Edge Trigger Enabled for line 12 */
#define  EXTI_RTE_RTE13                             ((uint32_t)0x00002000)        /*!< Rising Edge Trigger Enabled for line 13 */
#define  EXTI_RTE_RTE14                             ((uint32_t)0x00004000)        /*!< Rising Edge Trigger Enabled for line 14 */
#define  EXTI_RTE_RTE15                             ((uint32_t)0x00008000)        /*!< Rising Edge Trigger Enabled for line 15 */
#define  EXTI_RTE_RTE16                             ((uint32_t)0x00010000)        /*!< Rising Edge Trigger Enabled for line 16 */
#define  EXTI_RTE_RTE17                             ((uint32_t)0x00020000)        /*!< Rising Edge Trigger Enabled for line 17 */
#define  EXTI_RTE_RTE18                             ((uint32_t)0x00040000)        /*!< Rising Edge Trigger Enabled for line 18 */

#define  EXTI_RTE_RTE19                             ((uint32_t)0x00080000)        /*!< Rising Edge Trigger Enabled for line 19 */

/*******************  Bit definition for EXTI_FTE register *******************/
#define  EXTI_FTE_FTE0                              ((uint32_t)0x00000001)        /*!< Falling Edge Trigger Enabled for line 0 */
#define  EXTI_FTE_FTE1                              ((uint32_t)0x00000002)        /*!< Falling Edge Trigger Enabled for line 1 */
#define  EXTI_FTE_FTE2                              ((uint32_t)0x00000004)        /*!< Falling Edge Trigger Enabled for line 2 */
#define  EXTI_FTE_FTE3                              ((uint32_t)0x00000008)        /*!< Falling Edge Trigger Enabled for line 3 */
#define  EXTI_FTE_FTE4                              ((uint32_t)0x00000010)        /*!< Falling Edge Trigger Enabled for line 4 */
#define  EXTI_FTE_FTE5                              ((uint32_t)0x00000020)        /*!< Falling Edge Trigger Enabled for line 5 */
#define  EXTI_FTE_FTE6                              ((uint32_t)0x00000040)        /*!< Falling Edge Trigger Enabled for line 6 */
#define  EXTI_FTE_FTE7                              ((uint32_t)0x00000080)        /*!< Falling Edge Trigger Enabled for line 7 */
#define  EXTI_FTE_FTE8                              ((uint32_t)0x00000100)        /*!< Falling Edge Trigger Enabled for line 8 */
#define  EXTI_FTE_FTE9                              ((uint32_t)0x00000200)        /*!< Falling Edge Trigger Enabled for line 9 */
#define  EXTI_FTE_FTE10                             ((uint32_t)0x00000400)        /*!< Falling Edge Trigger Enabled for line 10 */
#define  EXTI_FTE_FTE11                             ((uint32_t)0x00000800)        /*!< Falling Edge Trigger Enabled for line 11 */
#define  EXTI_FTE_FTE12                             ((uint32_t)0x00001000)        /*!< Falling Edge Trigger Enabled for line 12 */
#define  EXTI_FTE_FTE13                             ((uint32_t)0x00002000)        /*!< Falling Edge Trigger Enabled for line 13 */
#define  EXTI_FTE_FTE14                             ((uint32_t)0x00004000)        /*!< Falling Edge Trigger Enabled for line 14 */
#define  EXTI_FTE_FTE15                             ((uint32_t)0x00008000)        /*!< Falling Edge Trigger Enabled for line 15 */
#define  EXTI_FTE_FTE16                             ((uint32_t)0x00010000)        /*!< Falling Edge Trigger Enabled for line 16 */
#define  EXTI_FTE_FTE17                             ((uint32_t)0x00020000)        /*!< Falling Edge Trigger Enabled for line 17 */
#define  EXTI_FTE_FTE18                             ((uint32_t)0x00040000)        /*!< Falling Edge Trigger Enabled for line 18 */

#define  EXTI_FTE_FTE19                             ((uint32_t)0x00080000)        /*!< Falling Edge Trigger Enabled for line 19 */

/******************* Bit definition for EXTI_SIE register *******************/
#define  EXTI_SIE_SIE0                              ((uint32_t)0x00000001)        /*!< Interrupt/Event Software Trigger on line 0 */
#define  EXTI_SIE_SIE1                              ((uint32_t)0x00000002)        /*!< Interrupt/Event Software Trigger on line 1 */
#define  EXTI_SIE_SIE2                              ((uint32_t)0x00000004)        /*!< Interrupt/Event Software Trigger on line 2 */
#define  EXTI_SIE_SIE3                              ((uint32_t)0x00000008)        /*!< Interrupt/Event Software Trigger on line 3 */
#define  EXTI_SIE_SIE4                              ((uint32_t)0x00000010)        /*!< Interrupt/Event Software Trigger on line 4 */
#define  EXTI_SIE_SIE5                              ((uint32_t)0x00000020)        /*!< Interrupt/Event Software Trigger on line 5 */
#define  EXTI_SIE_SIE6                              ((uint32_t)0x00000040)        /*!< Interrupt/Event Software Trigger on line 6 */
#define  EXTI_SIE_SIE7                              ((uint32_t)0x00000080)        /*!< Interrupt/Event Software Trigger on line 7 */
#define  EXTI_SIE_SIE8                              ((uint32_t)0x00000100)        /*!< Interrupt/Event Software Trigger on line 8 */
#define  EXTI_SIE_SIE9                              ((uint32_t)0x00000200)        /*!< Interrupt/Event Software Trigger on line 9 */
#define  EXTI_SIE_SIE10                             ((uint32_t)0x00000400)        /*!< Interrupt/Event Software Trigger on line 10 */
#define  EXTI_SIE_SIE11                             ((uint32_t)0x00000800)        /*!< Interrupt/Event Software Trigger on line 11 */
#define  EXTI_SIE_SIE12                             ((uint32_t)0x00001000)        /*!< Interrupt/Event Software Trigger on line 12 */
#define  EXTI_SIE_SIE13                             ((uint32_t)0x00002000)        /*!< Interrupt/Event Software Trigger on line 13 */
#define  EXTI_SIE_SIE14                             ((uint32_t)0x00004000)        /*!< Interrupt/Event Software Trigger on line 14 */
#define  EXTI_SIE_SIE15                             ((uint32_t)0x00008000)        /*!< Interrupt/Event Software Trigger on line 15 */
#define  EXTI_SIE_SIE16                             ((uint32_t)0x00010000)        /*!< Interrupt/Event Software Trigger on line 16 */
#define  EXTI_SIE_SIE17                             ((uint32_t)0x00020000)        /*!< Interrupt/Event Software Trigger on line 17 */
#define  EXTI_SIE_SIE18                             ((uint32_t)0x00040000)        /*!< Interrupt/Event Software Trigger on line 18 */

#define  EXTI_SIE_SIE19                             ((uint32_t)0x00080000)        /*!< Interrupt/Event Software Trigger on line 19 */

/******************  Bit definition for EXTI_PD register  *********************/
#define  EXTI_PD_PD0                                ((uint32_t)0x00000001)        /*!< Interrupt/Event Pending Status on line 0 */
#define  EXTI_PD_PD1                                ((uint32_t)0x00000002)        /*!< Interrupt/Event Pending Status on line 1 */
#define  EXTI_PD_PD2                                ((uint32_t)0x00000004)        /*!< Interrupt/Event Pending Status on line 2 */
#define  EXTI_PD_PD3                                ((uint32_t)0x00000008)        /*!< Interrupt/Event Pending Status on line 3 */
#define  EXTI_PD_PD4                                ((uint32_t)0x00000010)        /*!< Interrupt/Event Pending Status on line 4 */
#define  EXTI_PD_PD5                                ((uint32_t)0x00000020)        /*!< Interrupt/Event Pending Status on line 5 */
#define  EXTI_PD_PD6                                ((uint32_t)0x00000040)        /*!< Interrupt/Event Pending Status on line 6 */
#define  EXTI_PD_PD7                                ((uint32_t)0x00000080)        /*!< Interrupt/Event Pending Status on line 7 */
#define  EXTI_PD_PD8                                ((uint32_t)0x00000100)        /*!< Interrupt/Event Pending Status on line 8 */
#define  EXTI_PD_PD9                                ((uint32_t)0x00000200)        /*!< Interrupt/Event Pending Status on line 9 */
#define  EXTI_PD_PD10                               ((uint32_t)0x00000400)        /*!< Interrupt/Event Pending Status on line 10 */
#define  EXTI_PD_PD11                               ((uint32_t)0x00000800)        /*!< Interrupt/Event Pending Status on line 11 */
#define  EXTI_PD_PD12                               ((uint32_t)0x00001000)        /*!< Interrupt/Event Pending Status on line 12 */
#define  EXTI_PD_PD13                               ((uint32_t)0x00002000)        /*!< Interrupt/Event Pending Status on line 13 */
#define  EXTI_PD_PD14                               ((uint32_t)0x00004000)        /*!< Interrupt/Event Pending Status on line 14 */
#define  EXTI_PD_PD15                               ((uint32_t)0x00008000)        /*!< Interrupt/Event Pending Status on line 15 */
#define  EXTI_PD_PD16                               ((uint32_t)0x00010000)        /*!< Interrupt/Event Pending Status on line 16 */
#define  EXTI_PD_PD17                               ((uint32_t)0x00020000)        /*!< Interrupt/Event Pending Status on line 17 */
#define  EXTI_PD_PD18                               ((uint32_t)0x00040000)        /*!< Interrupt/Event Pending Status on line 18 */

#define  EXTI_PD_PD19                               ((uint32_t)0x00080000)        /*!< Interrupt/Event Pending Status on line 19 */

/******************************************************************************/
/*                                                                            */
/*                      FMC Registers                                         */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for FMC_RESR register  ******************/
#define  FMC_RESR_WSCNT                             ((uint32_t)0x00000007)        /*!< LATENCY bit (Latency) */

/******************  Bit definition for FMC_UKEYR register  ******************/
#define  FMC_UKEYR_UKEY                             ((uint32_t)0xFFFFFFFF)        /*!< FPEC Key */

/*****************  Bit definition for FMC_OBKEYR register  ****************/
#define  FMC_OBKEYR_OBKEY                           ((uint32_t)0xFFFFFFFF)        /*!< Option Byte Key */

/******************  FMC Keys  **********************************************/
#define FMC_KEY1                                    ((uint32_t)0x45670123)        /*!< Flash program erase key1 */
#define FMC_KEY2                                    ((uint32_t)0xCDEF89AB)        /*!< Flash program erase key2 */
								          
/******************  Bit definition for FMC_CSR register  *******************/
#define  FMC_CSR_BUSY                               ((uint32_t)0x00000001)        /*!< Busy */
#define  FMC_CSR_PGEF                               ((uint32_t)0x00000004)        /*!< Programming Error */
#define  FMC_CSR_WPEF                               ((uint32_t)0x00000010)        /*!< Write Protection Error */
#define  FMC_CSR_ENDF                               ((uint32_t)0x00000020)        /*!< End of operation */
#define  FMC_CSR2_BUSY                              ((uint32_t)0x80000001)        /*!< Busy For Bank2 */
#define  FMC_CSR2_PGEF                              ((uint32_t)0x80000004)        /*!< Programming Error For Bank2*/
#define  FMC_CSR2_WPEF                              ((uint32_t)0x80000010)        /*!< Write Protection Error For Bank2*/
#define  FMC_CSR2_ENDF                              ((uint32_t)0x80000020)        /*!< End of operation For Bank2*/

/*******************  Bit definition for FMC_CMR register  *******************/
#define  FMC_CMR_PG                                 ((uint32_t)0x00000001)        /*!< Programming */
#define  FMC_CMR_PE                                 ((uint32_t)0x00000002)        /*!< Page Erase */
#define  FMC_CMR_ME                                 ((uint32_t)0x00000004)        /*!< Mass Erase */
#define  FMC_CMR_OBPG                               ((uint32_t)0x00000010)        /*!< Option Byte Programming */
#define  FMC_CMR_OBER                               ((uint32_t)0x00000020)        /*!< Option Byte Erase */
#define  FMC_CMR_START                              ((uint32_t)0x00000040)        /*!< Start */
#define  FMC_CMR_LK                                 ((uint32_t)0x00000080)        /*!< Lock */
#define  FMC_CMR_OBWE                               ((uint32_t)0x00000200)        /*!< Option Bytes Write Enable */
#define  FMC_CMR_ERIE                               ((uint32_t)0x00000400)        /*!< Error Interrupt Enable */
#define  FMC_CMR_ENDIE                              ((uint32_t)0x00001000)        /*!< End of operation interrupt enable */
#define  FMC_CMR_OPTR                               ((uint32_t)0x00002000)        /*!< Option Bytes Loader Launch */

/*******************  Bit definition for FMC_AR register  *******************/
#define  FMC_AR_AR                                  ((uint32_t)0xFFFFFFFF)        /*!< Flash Address */

/******************  Bit definition for FMC_OPTR register  *******************/
#define  FMC_OPTR_OBER                              ((uint32_t)0x00000001)        /*!< Option Byte Error */
#define  FMC_OPTR_PLEVEL1                           ((uint32_t)0x00000002)        /*!< Read protection Level 1 */
#define  FMC_OPTR_OB_USER                           ((uint32_t)0x00003700)        /*!< User Option Bytes */

/******************  Bit definition for FMC_WPR register  ******************/
#define  FMC_WPR_WRP                                ((uint32_t)0xFFFFFFFF)        /*!< Write Protect */

/*----------------------------------------------------------------------------*/

/******************  Bit definition for OB_RDP register  **********************/
#define  OB_RDP_RDP                                 ((uint32_t)0x000000FF)        /*!< Read protection option byte */
#define  OB_RDP_nRDP                                ((uint32_t)0x0000FF00)        /*!< Read protection complemented option byte */

/******************  Bit definition for OB_USER register  *********************/
#define  OB_USER_USER                               ((uint32_t)0x00FF0000)        /*!< User option byte */
#define  OB_USER_nUSER                              ((uint32_t)0xFF000000)        /*!< User complemented option byte */

/******************  Bit definition for OB_WRP0 register  *********************/
#define  OB_WRP0_WRP0                               ((uint32_t)0x000000FF)        /*!< Flash memory write protection option bytes */
#define  OB_WRP0_nWRP0                              ((uint32_t)0x0000FF00)        /*!< Flash memory write protection complemented option bytes */

/******************  Bit definition for OB_WRP1 register  *********************/
#define  OB_WRP1_WRP1                               ((uint32_t)0x00FF0000)        /*!< Flash memory write protection option bytes */
#define  OB_WRP1_nWRP1                              ((uint32_t)0xFF000000)        /*!< Flash memory write protection complemented option bytes */

/******************  Bit definition for OB_WRP2 register  *********************/
#define  OB_WRP2_WRP2                               ((uint32_t)0x000000FF)        /*!< Flash memory write protection option bytes */
#define  OB_WRP2_nWRP2                              ((uint32_t)0x0000FF00)        /*!< Flash memory write protection complemented option bytes */

/******************  Bit definition for OB_WRP3 register  *********************/
#define  OB_WRP3_WRP3                               ((uint32_t)0x00FF0000)        /*!< Flash memory write protection option bytes */
#define  OB_WRP3_nWRP3                              ((uint32_t)0xFF000000)        /*!< Flash memory write protection complemented option bytes */

/******************************************************************************/
/*                                                                            */
/*                       General Purpose IOs (GPIO)                           */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for GPIO_CTLR1 register  *******************/
#define  GPIO_CTLR1_MD                              ((uint32_t)0x33333333)        /*!< Port x mode bits */

#define  GPIO_CTLR1_MD0                             ((uint32_t)0x00000003)        /*!< MODE0[1:0] bits (Port x mode bits, pin 0) */
#define  GPIO_CTLR1_MD0_0                           ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  GPIO_CTLR1_MD0_1                           ((uint32_t)0x00000002)        /*!< Bit 1 */
								          
#define  GPIO_CTLR1_MD1                             ((uint32_t)0x00000030)        /*!< MODE1[1:0] bits (Port x mode bits, pin 1) */
#define  GPIO_CTLR1_MD1_0                           ((uint32_t)0x00000010)        /*!< Bit 0 */
#define  GPIO_CTLR1_MD1_1                           ((uint32_t)0x00000020)        /*!< Bit 1 */

#define  GPIO_CTLR1_MD2                             ((uint32_t)0x00000300)        /*!< MODE2[1:0] bits (Port x mode bits, pin 2) */
#define  GPIO_CTLR1_MD2_0                           ((uint32_t)0x00000100)        /*!< Bit 0 */
#define  GPIO_CTLR1_MD2_1                           ((uint32_t)0x00000200)        /*!< Bit 1 */

#define  GPIO_CTLR1_MD3                             ((uint32_t)0x00003000)        /*!< MODE3[1:0] bits (Port x mode bits, pin 3) */
#define  GPIO_CTLR1_MD3_0                           ((uint32_t)0x00001000)        /*!< Bit 0 */
#define  GPIO_CTLR1_MD3_1                           ((uint32_t)0x00002000)        /*!< Bit 1 */

#define  GPIO_CTLR1_MD4                             ((uint32_t)0x00030000)        /*!< MODE4[1:0] bits (Port x mode bits, pin 4) */
#define  GPIO_CTLR1_MD4_0                           ((uint32_t)0x00010000)        /*!< Bit 0 */
#define  GPIO_CTLR1_MD4_1                           ((uint32_t)0x00020000)        /*!< Bit 1 */
								         
#define  GPIO_CTLR1_MD5                             ((uint32_t)0x00300000)        /*!< MODE5[1:0] bits (Port x mode bits, pin 5) */
#define  GPIO_CTLR1_MD5_0                           ((uint32_t)0x00100000)        /*!< Bit 0 */
#define  GPIO_CTLR1_MD5_1                           ((uint32_t)0x00200000)        /*!< Bit 1 */

#define  GPIO_CTLR1_MD6                             ((uint32_t)0x03000000)        /*!< MODE6[1:0] bits (Port x mode bits, pin 6) */
#define  GPIO_CTLR1_MD6_0                           ((uint32_t)0x01000000)        /*!< Bit 0 */
#define  GPIO_CTLR1_MD6_1                           ((uint32_t)0x02000000)        /*!< Bit 1 */

#define  GPIO_CTLR1_MD7                             ((uint32_t)0x30000000)        /*!< MODE7[1:0] bits (Port x mode bits, pin 7) */
#define  GPIO_CTLR1_MD7_0                           ((uint32_t)0x10000000)        /*!< Bit 0 */
#define  GPIO_CTLR1_MD7_1                           ((uint32_t)0x20000000)        /*!< Bit 1 */

#define  GPIO_CTLR1_CF                              ((uint32_t)0xCCCCCCCC)        /*!< Port x configuration bits */

#define  GPIO_CTLR1_CF0                             ((uint32_t)0x0000000C)        /*!< CNF0[1:0] bits (Port x configuration bits, pin 0) */
#define  GPIO_CTLR1_CF0_0                           ((uint32_t)0x00000004)        /*!< Bit 0 */
#define  GPIO_CTLR1_CF0_1                           ((uint32_t)0x00000008)        /*!< Bit 1 */

#define  GPIO_CTLR1_CF1                             ((uint32_t)0x000000C0)        /*!< CNF1[1:0] bits (Port x configuration bits, pin 1) */
#define  GPIO_CTLR1_CF1_0                           ((uint32_t)0x00000040)        /*!< Bit 0 */
#define  GPIO_CTLR1_CF1_1                           ((uint32_t)0x00000080)        /*!< Bit 1 */

#define  GPIO_CTLR1_CF2                             ((uint32_t)0x00000C00)        /*!< CNF2[1:0] bits (Port x configuration bits, pin 2) */
#define  GPIO_CTLR1_CF2_0                           ((uint32_t)0x00000400)        /*!< Bit 0 */
#define  GPIO_CTLR1_CF2_1                           ((uint32_t)0x00000800)        /*!< Bit 1 */

#define  GPIO_CTLR1_CF3                             ((uint32_t)0x0000C000)        /*!< CNF3[1:0] bits (Port x configuration bits, pin 3) */
#define  GPIO_CTLR1_CF3_0                           ((uint32_t)0x00004000)        /*!< Bit 0 */
#define  GPIO_CTLR1_CF3_1                           ((uint32_t)0x00008000)        /*!< Bit 1 */

#define  GPIO_CTLR1_CF4                             ((uint32_t)0x000C0000)        /*!< CNF4[1:0] bits (Port x configuration bits, pin 4) */
#define  GPIO_CTLR1_CF4_0                           ((uint32_t)0x00040000)        /*!< Bit 0 */
#define  GPIO_CTLR1_CF4_1                           ((uint32_t)0x00080000)        /*!< Bit 1 */

#define  GPIO_CTLR1_CF5                             ((uint32_t)0x00C00000)        /*!< CNF5[1:0] bits (Port x configuration bits, pin 5) */
#define  GPIO_CTLR1_CF5_0                           ((uint32_t)0x00400000)        /*!< Bit 0 */
#define  GPIO_CTLR1_CF5_1                           ((uint32_t)0x00800000)        /*!< Bit 1 */

#define  GPIO_CTLR1_CF6                             ((uint32_t)0x0C000000)        /*!< CNF6[1:0] bits (Port x configuration bits, pin 6) */
#define  GPIO_CTLR1_CF6_0                           ((uint32_t)0x04000000)        /*!< Bit 0 */
#define  GPIO_CTLR1_CF6_1                           ((uint32_t)0x08000000)        /*!< Bit 1 */

#define  GPIO_CTLR1_CF7                             ((uint32_t)0xC0000000)        /*!< CNF7[1:0] bits (Port x configuration bits, pin 7) */
#define  GPIO_CTLR1_CF7_0                           ((uint32_t)0x40000000)        /*!< Bit 0 */
#define  GPIO_CTLR1_CF7_1                           ((uint32_t)0x80000000)        /*!< Bit 1 */

/*******************  Bit definition for GPIO_CRH register  *******************/
#define  GPIO_CTLR2_MD                              ((uint32_t)0x33333333)        /*!< Port x mode bits */

#define  GPIO_CTLR2_MD8                             ((uint32_t)0x00000003)        /*!< MODE8[1:0] bits (Port x mode bits, pin 8) */
#define  GPIO_CTLR2_MD8_0                           ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  GPIO_CTLR2_MD8_1                           ((uint32_t)0x00000002)        /*!< Bit 1 */

#define  GPIO_CTLR2_MD9                             ((uint32_t)0x00000030)        /*!< MODE9[1:0] bits (Port x mode bits, pin 9) */
#define  GPIO_CTLR2_MD9_0                           ((uint32_t)0x00000010)        /*!< Bit 0 */
#define  GPIO_CTLR2_MD9_1                           ((uint32_t)0x00000020)        /*!< Bit 1 */

#define  GPIO_CTLR2_MD10                            ((uint32_t)0x00000300)        /*!< MODE10[1:0] bits (Port x mode bits, pin 10) */
#define  GPIO_CTLR2_MD10_0                          ((uint32_t)0x00000100)        /*!< Bit 0 */
#define  GPIO_CTLR2_MD10_1                          ((uint32_t)0x00000200)        /*!< Bit 1 */

#define  GPIO_CTLR2_MD11                            ((uint32_t)0x00003000)        /*!< MODE11[1:0] bits (Port x mode bits, pin 11) */
#define  GPIO_CTLR2_MD11_0                          ((uint32_t)0x00001000)        /*!< Bit 0 */
#define  GPIO_CTLR2_MD11_1                          ((uint32_t)0x00002000)        /*!< Bit 1 */

#define  GPIO_CTLR2_MD12                            ((uint32_t)0x00030000)        /*!< MODE12[1:0] bits (Port x mode bits, pin 12) */
#define  GPIO_CTLR2_MD12_0                          ((uint32_t)0x00010000)        /*!< Bit 0 */
#define  GPIO_CTLR2_MD12_1                          ((uint32_t)0x00020000)        /*!< Bit 1 */

#define  GPIO_CTLR2_MD13                            ((uint32_t)0x00300000)        /*!< MODE13[1:0] bits (Port x mode bits, pin 13) */
#define  GPIO_CTLR2_MD13_0                          ((uint32_t)0x00100000)        /*!< Bit 0 */
#define  GPIO_CTLR2_MD13_1                          ((uint32_t)0x00200000)        /*!< Bit 1 */
                                             
#define  GPIO_CTLR2_MD14                            ((uint32_t)0x03000000)        /*!< MODE14[1:0] bits (Port x mode bits, pin 14) */
#define  GPIO_CTLR2_MD14_0                          ((uint32_t)0x01000000)        /*!< Bit 0 */
#define  GPIO_CTLR2_MD14_1                          ((uint32_t)0x02000000)        /*!< Bit 1 */
							          
#define  GPIO_CTLR2_MD15                            ((uint32_t)0x30000000)        /*!< MODE15[1:0] bits (Port x mode bits, pin 15) */
#define  GPIO_CTLR2_MD15_0                          ((uint32_t)0x10000000)        /*!< Bit 0 */
#define  GPIO_CTLR2_MD15_1                          ((uint32_t)0x20000000)        /*!< Bit 1 */


#define  GPIO_CTLR2_CF                              ((uint32_t)0xCCCCCCCC)        /*!< Port x configuration bits */

#define  GPIO_CTLR2_CF8                             ((uint32_t)0x0000000C)        /*!< CNF8[1:0] bits (Port x configuration bits, pin 8) */
#define  GPIO_CTLR2_CF8_0                           ((uint32_t)0x00000004)        /*!< Bit 0 */
#define  GPIO_CTLR2_CF8_1                           ((uint32_t)0x00000008)        /*!< Bit 1 */

#define  GPIO_CTLR2_CF9                             ((uint32_t)0x000000C0)        /*!< CNF9[1:0] bits (Port x configuration bits, pin 9) */
#define  GPIO_CTLR2_CF9_0                           ((uint32_t)0x00000040)        /*!< Bit 0 */
#define  GPIO_CTLR2_CF9_1                           ((uint32_t)0x00000080)        /*!< Bit 1 */

#define  GPIO_CTLR2_CF10                            ((uint32_t)0x00000C00)        /*!< CNF10[1:0] bits (Port x configuration bits, pin 10) */
#define  GPIO_CTLR2_CF10_0                          ((uint32_t)0x00000400)        /*!< Bit 0 */
#define  GPIO_CTLR2_CF10_1                          ((uint32_t)0x00000800)        /*!< Bit 1 */

#define  GPIO_CTLR2_CF11                            ((uint32_t)0x0000C000)        /*!< CNF11[1:0] bits (Port x configuration bits, pin 11) */
#define  GPIO_CTLR2_CF11_0                          ((uint32_t)0x00004000)        /*!< Bit 0 */
#define  GPIO_CTLR2_CF11_1                          ((uint32_t)0x00008000)        /*!< Bit 1 */

#define  GPIO_CTLR2_CF12                            ((uint32_t)0x000C0000)        /*!< CNF12[1:0] bits (Port x configuration bits, pin 12) */
#define  GPIO_CTLR2_CF12_0                          ((uint32_t)0x00040000)        /*!< Bit 0 */
#define  GPIO_CTLR2_CF12_1                          ((uint32_t)0x00080000)        /*!< Bit 1 */
                                             
#define  GPIO_CTLR2_CF13                            ((uint32_t)0x00C00000)        /*!< CNF13[1:0] bits (Port x configuration bits, pin 13) */
#define  GPIO_CTLR2_CF13_0                          ((uint32_t)0x00400000)        /*!< Bit 0 */
#define  GPIO_CTLR2_CF13_1                          ((uint32_t)0x00800000)        /*!< Bit 1 */
									       
#define  GPIO_CTLR2_CF14                            ((uint32_t)0x0C000000)        /*!< CNF14[1:0] bits (Port x configuration bits, pin 14) */
#define  GPIO_CTLR2_CF14_0                          ((uint32_t)0x04000000)        /*!< Bit 0 */
#define  GPIO_CTLR2_CF14_1                          ((uint32_t)0x08000000)        /*!< Bit 1 */

#define  GPIO_CTLR2_CF15                            ((uint32_t)0xC0000000)        /*!< CNF15[1:0] bits (Port x configuration bits, pin 15) */
#define  GPIO_CTLR2_CF15_0                          ((uint32_t)0x40000000)        /*!< Bit 0 */
#define  GPIO_CTLR2_CF15_1                          ((uint32_t)0x80000000)        /*!< Bit 1 */

/*!<******************  Bit definition for GPIO_DIR register  *******************/
#define  GPIO_DIR_DIR0                              ((uint16_t)0x0001)            /*!< Port input data, bit 0 */
#define  GPIO_DIR_DIR1                              ((uint16_t)0x0002)            /*!< Port input data, bit 1 */
#define  GPIO_DIR_DIR2                              ((uint16_t)0x0004)            /*!< Port input data, bit 2 */
#define  GPIO_DIR_DIR3                              ((uint16_t)0x0008)            /*!< Port input data, bit 3 */
#define  GPIO_DIR_DIR4                              ((uint16_t)0x0010)            /*!< Port input data, bit 4 */
#define  GPIO_DIR_DIR5                              ((uint16_t)0x0020)            /*!< Port input data, bit 5 */
#define  GPIO_DIR_DIR6                              ((uint16_t)0x0040)            /*!< Port input data, bit 6 */
#define  GPIO_DIR_DIR7                              ((uint16_t)0x0080)            /*!< Port input data, bit 7 */
#define  GPIO_DIR_DIR8                              ((uint16_t)0x0100)            /*!< Port input data, bit 8 */
#define  GPIO_DIR_DIR9                              ((uint16_t)0x0200)            /*!< Port input data, bit 9 */
#define  GPIO_DIR_DIR10                             ((uint16_t)0x0400)            /*!< Port input data, bit 10 */
#define  GPIO_DIR_DIR11                             ((uint16_t)0x0800)            /*!< Port input data, bit 11 */
#define  GPIO_DIR_DIR12                             ((uint16_t)0x1000)            /*!< Port input data, bit 12 */
#define  GPIO_DIR_DIR13                             ((uint16_t)0x2000)            /*!< Port input data, bit 13 */
#define  GPIO_DIR_DIR14                             ((uint16_t)0x4000)            /*!< Port input data, bit 14 */
#define  GPIO_DIR_DIR15                             ((uint16_t)0x8000)            /*!< Port input data, bit 15 */
								         
/*******************  Bit definition for GPIO_DOR register  *******************/
#define  GPIO_DOR_DOR0                              ((uint16_t)0x0001)            /*!< Port output data, bit 0 */
#define  GPIO_DOR_DOR1                              ((uint16_t)0x0002)            /*!< Port output data, bit 1 */
#define  GPIO_DOR_DOR2                              ((uint16_t)0x0004)            /*!< Port output data, bit 2 */
#define  GPIO_DOR_DOR3                              ((uint16_t)0x0008)            /*!< Port output data, bit 3 */
#define  GPIO_DOR_DOR4                              ((uint16_t)0x0010)            /*!< Port output data, bit 4 */
#define  GPIO_DOR_DOR5                              ((uint16_t)0x0020)            /*!< Port output data, bit 5 */
#define  GPIO_DOR_DOR6                              ((uint16_t)0x0040)            /*!< Port output data, bit 6 */
#define  GPIO_DOR_DOR7                              ((uint16_t)0x0080)            /*!< Port output data, bit 7 */
#define  GPIO_DOR_DOR8                              ((uint16_t)0x0100)            /*!< Port output data, bit 8 */
#define  GPIO_DOR_DOR9                              ((uint16_t)0x0200)            /*!< Port output data, bit 9 */
#define  GPIO_DOR_DOR10                             ((uint16_t)0x0400)            /*!< Port output data, bit 10 */
#define  GPIO_DOR_DOR11                             ((uint16_t)0x0800)            /*!< Port output data, bit 11 */
#define  GPIO_DOR_DOR12                             ((uint16_t)0x1000)            /*!< Port output data, bit 12 */
#define  GPIO_DOR_DOR13                             ((uint16_t)0x2000)            /*!< Port output data, bit 13 */
#define  GPIO_DOR_DOR14                             ((uint16_t)0x4000)            /*!< Port output data, bit 14 */
#define  GPIO_DOR_DOR15                             ((uint16_t)0x8000)            /*!< Port output data, bit 15 */

/******************  Bit definition for GPIO_BOR register  *******************/
#define  GPIO_BOR_BOR0                              ((uint32_t)0x00000001)        /*!< Port x Set bit 0 */
#define  GPIO_BOR_BOR1                              ((uint32_t)0x00000002)        /*!< Port x Set bit 1 */
#define  GPIO_BOR_BOR2                              ((uint32_t)0x00000004)        /*!< Port x Set bit 2 */
#define  GPIO_BOR_BOR3                              ((uint32_t)0x00000008)        /*!< Port x Set bit 3 */
#define  GPIO_BOR_BOR4                              ((uint32_t)0x00000010)        /*!< Port x Set bit 4 */
#define  GPIO_BOR_BOR5                              ((uint32_t)0x00000020)        /*!< Port x Set bit 5 */
#define  GPIO_BOR_BOR6                              ((uint32_t)0x00000040)        /*!< Port x Set bit 6 */
#define  GPIO_BOR_BOR7                              ((uint32_t)0x00000080)        /*!< Port x Set bit 7 */
#define  GPIO_BOR_BOR8                              ((uint32_t)0x00000100)        /*!< Port x Set bit 8 */
#define  GPIO_BOR_BOR9                              ((uint32_t)0x00000200)        /*!< Port x Set bit 9 */
#define  GPIO_BOR_BOR10                             ((uint32_t)0x00000400)        /*!< Port x Set bit 10 */
#define  GPIO_BOR_BOR11                             ((uint32_t)0x00000800)        /*!< Port x Set bit 11 */
#define  GPIO_BOR_BOR12                             ((uint32_t)0x00001000)        /*!< Port x Set bit 12 */
#define  GPIO_BOR_BOR13                             ((uint32_t)0x00002000)        /*!< Port x Set bit 13 */
#define  GPIO_BOR_BOR14                             ((uint32_t)0x00004000)        /*!< Port x Set bit 14 */
#define  GPIO_BOR_BOR15                             ((uint32_t)0x00008000)        /*!< Port x Set bit 15 */
                                               
#define  GPIO_BOR_COR0                              ((uint32_t)0x00010000)        /*!< Port x Reset bit 0 */
#define  GPIO_BOR_COR1                              ((uint32_t)0x00020000)        /*!< Port x Reset bit 1 */
#define  GPIO_BOR_COR2                              ((uint32_t)0x00040000)        /*!< Port x Reset bit 2 */
#define  GPIO_BOR_COR3                              ((uint32_t)0x00080000)        /*!< Port x Reset bit 3 */
#define  GPIO_BOR_COR4                              ((uint32_t)0x00100000)        /*!< Port x Reset bit 4 */
#define  GPIO_BOR_COR5                              ((uint32_t)0x00200000)        /*!< Port x Reset bit 5 */
#define  GPIO_BOR_COR6                              ((uint32_t)0x00400000)        /*!< Port x Reset bit 6 */
#define  GPIO_BOR_COR7                              ((uint32_t)0x00800000)        /*!< Port x Reset bit 7 */
#define  GPIO_BOR_COR8                              ((uint32_t)0x01000000)        /*!< Port x Reset bit 8 */
#define  GPIO_BOR_COR9                              ((uint32_t)0x02000000)        /*!< Port x Reset bit 9 */
#define  GPIO_BOR_COR10                             ((uint32_t)0x04000000)        /*!< Port x Reset bit 10 */
#define  GPIO_BOR_COR11                             ((uint32_t)0x08000000)        /*!< Port x Reset bit 11 */
#define  GPIO_BOR_COR12                             ((uint32_t)0x10000000)        /*!< Port x Reset bit 12 */
#define  GPIO_BOR_COR13                             ((uint32_t)0x20000000)        /*!< Port x Reset bit 13 */
#define  GPIO_BOR_COR14                             ((uint32_t)0x40000000)        /*!< Port x Reset bit 14 */
#define  GPIO_BOR_COR15                             ((uint32_t)0x80000000)        /*!< Port x Reset bit 15 */

/*******************  Bit definition for GPIO_BCR register  *******************/
#define  GPIO_BCR_BR0                               ((uint16_t)0x0001)            /*!< Port x Reset bit 0 */
#define  GPIO_BCR_BR1                               ((uint16_t)0x0002)            /*!< Port x Reset bit 1 */
#define  GPIO_BCR_BR2                               ((uint16_t)0x0004)            /*!< Port x Reset bit 2 */
#define  GPIO_BCR_BR3                               ((uint16_t)0x0008)            /*!< Port x Reset bit 3 */
#define  GPIO_BCR_BR4                               ((uint16_t)0x0010)            /*!< Port x Reset bit 4 */
#define  GPIO_BCR_BR5                               ((uint16_t)0x0020)            /*!< Port x Reset bit 5 */
#define  GPIO_BCR_BR6                               ((uint16_t)0x0040)            /*!< Port x Reset bit 6 */
#define  GPIO_BCR_BR7                               ((uint16_t)0x0080)            /*!< Port x Reset bit 7 */
#define  GPIO_BCR_BR8                               ((uint16_t)0x0100)            /*!< Port x Reset bit 8 */
#define  GPIO_BCR_BR9                               ((uint16_t)0x0200)            /*!< Port x Reset bit 9 */
#define  GPIO_BCR_BR10                              ((uint16_t)0x0400)            /*!< Port x Reset bit 10 */
#define  GPIO_BCR_BR11                              ((uint16_t)0x0800)            /*!< Port x Reset bit 11 */
#define  GPIO_BCR_BR12                              ((uint16_t)0x1000)            /*!< Port x Reset bit 12 */
#define  GPIO_BCR_BR13                              ((uint16_t)0x2000)            /*!< Port x Reset bit 13 */
#define  GPIO_BCR_BR14                              ((uint16_t)0x4000)            /*!< Port x Reset bit 14 */
#define  GPIO_BCR_BR15                              ((uint16_t)0x8000)            /*!< Port x Reset bit 15 */
		 
/******************  Bit definition for GPIO_LOCKR register  *******************/
#define  GPIO_LOCKR_LK0                             ((uint32_t)0x00000001)        /*!< Port x Lock bit 0 */
#define  GPIO_LOCKR_LK1                             ((uint32_t)0x00000002)        /*!< Port x Lock bit 1 */
#define  GPIO_LOCKR_LK2                             ((uint32_t)0x00000004)        /*!< Port x Lock bit 2 */
#define  GPIO_LOCKR_LK3                             ((uint32_t)0x00000008)        /*!< Port x Lock bit 3 */
#define  GPIO_LOCKR_LK4                             ((uint32_t)0x00000010)        /*!< Port x Lock bit 4 */
#define  GPIO_LOCKR_LK5                             ((uint32_t)0x00000020)        /*!< Port x Lock bit 5 */
#define  GPIO_LOCKR_LK6                             ((uint32_t)0x00000040)        /*!< Port x Lock bit 6 */
#define  GPIO_LOCKR_LK7                             ((uint32_t)0x00000080)        /*!< Port x Lock bit 7 */
#define  GPIO_LOCKR_LK8                             ((uint32_t)0x00000100)        /*!< Port x Lock bit 8 */
#define  GPIO_LOCKR_LK9                             ((uint32_t)0x00000200)        /*!< Port x Lock bit 9 */
#define  GPIO_LOCKR_LK10                            ((uint32_t)0x00000400)        /*!< Port x Lock bit 10 */
#define  GPIO_LOCKR_LK11                            ((uint32_t)0x00000800)        /*!< Port x Lock bit 11 */
#define  GPIO_LOCKR_LK12                            ((uint32_t)0x00001000)        /*!< Port x Lock bit 12 */
#define  GPIO_LOCKR_LK13                            ((uint32_t)0x00002000)        /*!< Port x Lock bit 13 */
#define  GPIO_LOCKR_LK14                            ((uint32_t)0x00004000)        /*!< Port x Lock bit 14 */
#define  GPIO_LOCKR_LK15                            ((uint32_t)0x00008000)        /*!< Port x Lock bit 15 */
#define  GPIO_LOCKR_LKK                             ((uint32_t)0x00010000)        /*!< Lock key */

/*----------------------------------------------------------------------------*/

/******************  Bit definition for AFIO_ECR register  *******************/
#define  AFIO_ECR_PIN                               ((uint8_t)0x0F)               /*!< PIN[3:0] bits (Pin selection) */
#define  AFIO_ECR_PIN_0                             ((uint8_t)0x01)               /*!< Bit 0 */
#define  AFIO_ECR_PIN_1                             ((uint8_t)0x02)               /*!< Bit 1 */
#define  AFIO_ECR_PIN_2                             ((uint8_t)0x04)               /*!< Bit 2 */
#define  AFIO_ECR_PIN_3                             ((uint8_t)0x08)               /*!< Bit 3 */

/*!< PIN configuration */
#define  AFIO_ECR_PIN_PX0                           ((uint8_t)0x00)               /*!< Pin 0 selected */
#define  AFIO_ECR_PIN_PX1                           ((uint8_t)0x01)               /*!< Pin 1 selected */
#define  AFIO_ECR_PIN_PX2                           ((uint8_t)0x02)               /*!< Pin 2 selected */
#define  AFIO_ECR_PIN_PX3                           ((uint8_t)0x03)               /*!< Pin 3 selected */
#define  AFIO_ECR_PIN_PX4                           ((uint8_t)0x04)               /*!< Pin 4 selected */
#define  AFIO_ECR_PIN_PX5                           ((uint8_t)0x05)               /*!< Pin 5 selected */
#define  AFIO_ECR_PIN_PX6                           ((uint8_t)0x06)               /*!< Pin 6 selected */
#define  AFIO_ECR_PIN_PX7                           ((uint8_t)0x07)               /*!< Pin 7 selected */
#define  AFIO_ECR_PIN_PX8                           ((uint8_t)0x08)               /*!< Pin 8 selected */
#define  AFIO_ECR_PIN_PX9                           ((uint8_t)0x09)               /*!< Pin 9 selected */
#define  AFIO_ECR_PIN_PX10                          ((uint8_t)0x0A)               /*!< Pin 10 selected */
#define  AFIO_ECR_PIN_PX11                          ((uint8_t)0x0B)               /*!< Pin 11 selected */
#define  AFIO_ECR_PIN_PX12                          ((uint8_t)0x0C)               /*!< Pin 12 selected */
#define  AFIO_ECR_PIN_PX13                          ((uint8_t)0x0D)               /*!< Pin 13 selected */
#define  AFIO_ECR_PIN_PX14                          ((uint8_t)0x0E)               /*!< Pin 14 selected */
#define  AFIO_ECR_PIN_PX15                          ((uint8_t)0x0F)               /*!< Pin 15 selected */

#define  AFIO_ECR_PORT                              ((uint8_t)0x70)               /*!< PORT[2:0] bits (Port selection) */
#define  AFIO_ECR_PORT_0                            ((uint8_t)0x10)               /*!< Bit 0 */
#define  AFIO_ECR_PORT_1                            ((uint8_t)0x20)               /*!< Bit 1 */
#define  AFIO_ECR_PORT_2                            ((uint8_t)0x40)               /*!< Bit 2 */

/*!< PORT configuration */
#define  AFIO_ECR_PORT_PA                           ((uint8_t)0x00)               /*!< Port A selected */
#define  AFIO_ECR_PORT_PB                           ((uint8_t)0x10)               /*!< Port B selected */
#define  AFIO_ECR_PORT_PC                           ((uint8_t)0x20)               /*!< Port C selected */
#define  AFIO_ECR_PORT_PD                           ((uint8_t)0x30)               /*!< Port D selected */
#define  AFIO_ECR_PORT_PE                           ((uint8_t)0x40)               /*!< Port E selected */

#define  AFIO_ECR_EOE                               ((uint8_t)0x80)               /*!< Event Output Enable */

/******************  Bit definition for AFIO_MAPR register  *******************/
#define  AFIO_PCFR1_SPI1_REMAP                      ((uint32_t)0x00000001)        /*!< SPI1 remapping */
#define  AFIO_PCFR1_I2C1_REMAP                      ((uint32_t)0x00000002)        /*!< I2C1 remapping */
#define  AFIO_PCFR1_USART1_REMAP                    ((uint32_t)0x00000004)        /*!< USART1 remapping */
#define  AFIO_PCFR1_USART2_REMAP                    ((uint32_t)0x00000008)        /*!< USART2 remapping */

#define  AFIO_PCFR1_USART3_REMAP                    ((uint32_t)0x00000030)        /*!< USART3_REMAP[1:0] bits (USART3 remapping) */
#define  AFIO_PCFR1_USART3_REMAP_0                  ((uint32_t)0x00000010)        /*!< Bit 0 */
#define  AFIO_PCFR1_USART3_REMAP_1                  ((uint32_t)0x00000020)        /*!< Bit 1 */

/* USART3_REMAP configuration */
#define  AFIO_PCFR1_USART3_REMAP_NOREMAP            ((uint32_t)0x00000000)        /*!< No remap (TX/PB10, RX/PB11, CK/PB12, CTS/PB13, RTS/PB14) */
#define  AFIO_PCFR1_USART3_REMAP_PARTIALREMAP       ((uint32_t)0x00000010)        /*!< Partial remap (TX/PC10, RX/PC11, CK/PC12, CTS/PB13, RTS/PB14) */
#define  AFIO_PCFR1_USART3_REMAP_FULLREMAP          ((uint32_t)0x00000030)        /*!< Full remap (TX/PD8, RX/PD9, CK/PD10, CTS/PD11, RTS/PD12) */
		 
#define  AFIO_PCFR1_TIM1_REMAP                      ((uint32_t)0x000000C0)        /*!< TIM1_REMAP[1:0] bits (TIM1 remapping) */
#define  AFIO_PCFR1_TIM1_REMAP_0                    ((uint32_t)0x00000040)        /*!< Bit 0 */
#define  AFIO_PCFR1_TIM1_REMAP_1                    ((uint32_t)0x00000080)        /*!< Bit 1 */

/*!< TIM1_REMAP configuration */
#define  AFIO_PCFR1_TIM1_REMAP_NOREMAP              ((uint32_t)0x00000000)        /*!< No remap (ETR/PA12, CH1/PA8, CH2/PA9, CH3/PA10, CH4/PA11, BKIN/PB12, CH1N/PB13, CH2N/PB14, CH3N/PB15) */
#define  AFIO_PCFR1_TIM1_REMAP_PARTIALREMAP         ((uint32_t)0x00000040)        /*!< Partial remap (ETR/PA12, CH1/PA8, CH2/PA9, CH3/PA10, CH4/PA11, BKIN/PA6, CH1N/PA7, CH2N/PB0, CH3N/PB1) */
#define  AFIO_PCFR1_TIM1_REMAP_FULLREMAP            ((uint32_t)0x000000C0)        /*!< Full remap (ETR/PE7, CH1/PE9, CH2/PE11, CH3/PE13, CH4/PE14, BKIN/PE15, CH1N/PE8, CH2N/PE10, CH3N/PE12) */

#define  AFIO_PCFR1_TIM2_REMAP                      ((uint32_t)0x00000300)        /*!< TIM2_REMAP[1:0] bits (TIM2 remapping) */
#define  AFIO_PCFR1_TIM2_REMAP_0                    ((uint32_t)0x00000100)        /*!< Bit 0 */
#define  AFIO_PCFR1_TIM2_REMAP_1                    ((uint32_t)0x00000200)        /*!< Bit 1 */

/*!< TIM2_REMAP configuration */
#define  AFIO_PCFR1_TIM2_REMAP_NOREMAP              ((uint32_t)0x00000000)        /*!< No remap (CH1/ETR/PA0, CH2/PA1, CH3/PA2, CH4/PA3) */
#define  AFIO_PCFR1_TIM2_REMAP_PARTIALREMAP1        ((uint32_t)0x00000100)        /*!< Partial remap (CH1/ETR/PA15, CH2/PB3, CH3/PA2, CH4/PA3) */
#define  AFIO_PCFR1_TIM2_REMAP_PARTIALREMAP2        ((uint32_t)0x00000200)        /*!< Partial remap (CH1/ETR/PA0, CH2/PA1, CH3/PB10, CH4/PB11) */
#define  AFIO_PCFR1_TIM2_REMAP_FULLREMAP            ((uint32_t)0x00000300)        /*!< Full remap (CH1/ETR/PA15, CH2/PB3, CH3/PB10, CH4/PB11) */

#define  AFIO_PCFR1_TIM3_REMAP                      ((uint32_t)0x00000C00)        /*!< TIM3_REMAP[1:0] bits (TIM3 remapping) */
#define  AFIO_PCFR1_TIM3_REMAP_0                    ((uint32_t)0x00000400)        /*!< Bit 0 */
#define  AFIO_PCFR1_TIM3_REMAP_1                    ((uint32_t)0x00000800)        /*!< Bit 1 */

/*!< TIM3_REMAP configuration */
#define  AFIO_PCFR1_TIM3_REMAP_NOREMAP              ((uint32_t)0x00000000)        /*!< No remap (CH1/PA6, CH2/PA7, CH3/PB0, CH4/PB1) */
#define  AFIO_PCFR1_TIM3_REMAP_PARTIALREMAP         ((uint32_t)0x00000800)        /*!< Partial remap (CH1/PB4, CH2/PB5, CH3/PB0, CH4/PB1) */
#define  AFIO_PCFR1_TIM3_REMAP_FULLREMAP            ((uint32_t)0x00000C00)        /*!< Full remap (CH1/PC6, CH2/PC7, CH3/PC8, CH4/PC9) */

#define  AFIO_PCFR1_TIM4_REMAP                      ((uint32_t)0x00001000)        /*!< TIM4_REMAP bit (TIM4 remapping) */

#define  AFIO_PCFR1_CAN_REMAP                       ((uint32_t)0x00006000)        /*!< CAN_REMAP[1:0] bits (CAN Alternate function remapping) */
#define  AFIO_PCFR1_CAN_REMAP_0                     ((uint32_t)0x00002000)        /*!< Bit 0 */
#define  AFIO_PCFR1_CAN_REMAP_1                     ((uint32_t)0x00004000)        /*!< Bit 1 */

/*!< CAN_REMAP configuration */
#define  AFIO_PCFR1_CAN_REMAP_REMAP1                ((uint32_t)0x00000000)        /*!< CANRX mapped to PA11, CANTX mapped to PA12 */
#define  AFIO_PCFR1_CAN_REMAP_REMAP2                ((uint32_t)0x00004000)        /*!< CANRX mapped to PB8, CANTX mapped to PB9 */
#define  AFIO_PCFR1_CAN_REMAP_REMAP3                ((uint32_t)0x00006000)        /*!< CANRX mapped to PD0, CANTX mapped to PD1 */

#define  AFIO_PCFR1_PD01_REMAP                      ((uint32_t)0x00008000)        /*!< Port D0/Port D1 mapping on OSC_IN/OSC_OUT */
#define  AFIO_PCFR1_TIM5CH4_IREMAP                  ((uint32_t)0x00010000)        /*!< TIM5 Channel4 Internal Remap */
#define  AFIO_PCFR1_ADC1_ETRGINJ_REMAP              ((uint32_t)0x00020000)        /*!< ADC 1 External Trigger Injected Conversion remapping */
#define  AFIO_PCFR1_ADC1_ETRGREG_REMAP              ((uint32_t)0x00040000)        /*!< ADC 1 External Trigger Regular Conversion remapping */
#define  AFIO_PCFR1_ADC2_ETRGINJ_REMAP              ((uint32_t)0x00080000)        /*!< ADC 2 External Trigger Injected Conversion remapping */
#define  AFIO_PCFR1_ADC2_ETRGREG_REMAP              ((uint32_t)0x00100000)        /*!< ADC 2 External Trigger Regular Conversion remapping */

/*!< SWJ_CFG configuration */
#define  AFIO_PCFR1_SWJ_CFG                         ((uint32_t)0x07000000)        /*!< SWJ_CFG[2:0] bits (Serial Wire JTAG configuration) */
#define  AFIO_PCFR1_SWJ_CFG_0                       ((uint32_t)0x01000000)        /*!< Bit 0 */
#define  AFIO_PCFR1_SWJ_CFG_1                       ((uint32_t)0x02000000)        /*!< Bit 1 */
#define  AFIO_PCFR1_SWJ_CFG_2                       ((uint32_t)0x04000000)        /*!< Bit 2 */
										      
#define  AFIO_PCFR1_SWJ_CFG_RESET                   ((uint32_t)0x00000000)        /*!< Full SWJ (JTAG-DP + SW-DP) : Reset State */
#define  AFIO_PCFR1_SWJ_CFG_NOJNTRST                ((uint32_t)0x01000000)        /*!< Full SWJ (JTAG-DP + SW-DP) but without JNTRST */
#define  AFIO_PCFR1_SWJ_CFG_JTAGDISABLE             ((uint32_t)0x02000000)        /*!< JTAG-DP Disabled and SW-DP Enabled */
#define  AFIO_PCFR1_SWJ_CFG_DISABLE                 ((uint32_t)0x04000000)        /*!< JTAG-DP Disabled and SW-DP Disabled */

/*!< ETH_REMAP configuration */
#define  AFIO_PCFR1_ETH_REMAP                       ((uint32_t)0x00200000)        /*!< SPI3_REMAP bit (Ethernet MAC I/O remapping) */

/*!< CAN2_REMAP configuration */
#define  AFIO_PCFR1_CAN2_REMAP                      ((uint32_t)0x00400000)        /*!< CAN2_REMAP bit (CAN2 I/O remapping) */

/*!< MII_RMII_SEL configuration */
#define  AFIO_PCFR1_MII_RMII_SEL                    ((uint32_t)0x00800000)        /*!< MII_RMII_SEL bit (Ethernet MII or RMII selection) */

/*!< SPI3_REMAP configuration */
#define  AFIO_PCFR1_SPI3_REMAP                      ((uint32_t)0x10000000)        /*!< SPI3_REMAP bit (SPI3 remapping) */

/*!< TIM2ITR1_IREMAP configuration */
#define  AFIO_PCFR1_TIM2ITR1_IREMAP                 ((uint32_t)0x20000000)        /*!< TIM2ITR1_IREMAP bit (TIM2 internal trigger 1 remapping) */

/*!< PTP_PPS_REMAP configuration */
#define  AFIO_PCFR1_PTP_PPS_REMAP                   ((uint32_t)0x40000000)        /*!< PTP_PPS_REMAP bit (Ethernet PTP PPS remapping) */


/*****************  Bit definition for AFIO_EXTICR1 register  *****************/
#define  AFIO_ESSR1_EXTI0                           ((uint16_t)0x000F)            /*!< EXTI 0 configuration */
#define  AFIO_ESSR1_EXTI1                           ((uint16_t)0x00F0)            /*!< EXTI 1 configuration */
#define  AFIO_ESSR1_EXTI2                           ((uint16_t)0x0F00)            /*!< EXTI 2 configuration */
#define  AFIO_ESSR1_EXTI3                           ((uint16_t)0xF000)            /*!< EXTI 3 configuration */

/*!< EXTI0 configuration */
#define  AFIO_ESSR1_EXTI0_PA                        ((uint16_t)0x0000)            /*!< PA[0] pin */
#define  AFIO_ESSR1_EXTI0_PB                        ((uint16_t)0x0001)            /*!< PB[0] pin */
#define  AFIO_ESSR1_EXTI0_PC                        ((uint16_t)0x0002)            /*!< PC[0] pin */
#define  AFIO_ESSR1_EXTI0_PD                        ((uint16_t)0x0003)            /*!< PD[0] pin */
#define  AFIO_ESSR1_EXTI0_PE                        ((uint16_t)0x0004)            /*!< PE[0] pin */
#define  AFIO_ESSR1_EXTI0_PF                        ((uint16_t)0x0005)            /*!< PF[0] pin */
#define  AFIO_ESSR1_EXTI0_PG                        ((uint16_t)0x0006)            /*!< PG[0] pin */
		 
/*!< EXTI1 configuration */
#define  AFIO_ESSR1_EXTI1_PA                        ((uint16_t)0x0000)            /*!< PA[1] pin */
#define  AFIO_ESSR1_EXTI1_PB                        ((uint16_t)0x0010)            /*!< PB[1] pin */
#define  AFIO_ESSR1_EXTI1_PC                        ((uint16_t)0x0020)            /*!< PC[1] pin */
#define  AFIO_ESSR1_EXTI1_PD                        ((uint16_t)0x0030)            /*!< PD[1] pin */
#define  AFIO_ESSR1_EXTI1_PE                        ((uint16_t)0x0040)            /*!< PE[1] pin */
#define  AFIO_ESSR1_EXTI1_PF                        ((uint16_t)0x0050)            /*!< PF[1] pin */
#define  AFIO_ESSR1_EXTI1_PG                        ((uint16_t)0x0060)            /*!< PG[1] pin */
								       
/*!< EXTI2 configuration */  
#define  AFIO_ESSR1_EXTI2_PA                        ((uint16_t)0x0000)            /*!< PA[2] pin */
#define  AFIO_ESSR1_EXTI2_PB                        ((uint16_t)0x0100)            /*!< PB[2] pin */
#define  AFIO_ESSR1_EXTI2_PC                        ((uint16_t)0x0200)            /*!< PC[2] pin */
#define  AFIO_ESSR1_EXTI2_PD                        ((uint16_t)0x0300)            /*!< PD[2] pin */
#define  AFIO_ESSR1_EXTI2_PE                        ((uint16_t)0x0400)            /*!< PE[2] pin */
#define  AFIO_ESSR1_EXTI2_PF                        ((uint16_t)0x0500)            /*!< PF[2] pin */
#define  AFIO_ESSR1_EXTI2_PG                        ((uint16_t)0x0600)            /*!< PG[2] pin */

/*!< EXTI3 configuration */
#define  AFIO_ESSR1_EXTI3_PA                        ((uint16_t)0x0000)            /*!< PA[3] pin */
#define  AFIO_ESSR1_EXTI3_PB                        ((uint16_t)0x1000)            /*!< PB[3] pin */
#define  AFIO_ESSR1_EXTI3_PC                        ((uint16_t)0x2000)            /*!< PC[3] pin */
#define  AFIO_ESSR1_EXTI3_PD                        ((uint16_t)0x3000)            /*!< PD[3] pin */
#define  AFIO_ESSR1_EXTI3_PE                        ((uint16_t)0x4000)            /*!< PE[3] pin */
#define  AFIO_ESSR1_EXTI3_PF                        ((uint16_t)0x5000)            /*!< PF[3] pin */
#define  AFIO_ESSR1_EXTI3_PG                        ((uint16_t)0x6000)            /*!< PG[3] pin */

/*****************  Bit definition for AFIO_EXTICR2 register  *****************/
#define  AFIO_ESSR2_EXTI4                           ((uint16_t)0x000F)            /*!< EXTI 4 configuration */
#define  AFIO_ESSR2_EXTI5                           ((uint16_t)0x00F0)            /*!< EXTI 5 configuration */
#define  AFIO_ESSR2_EXTI6                           ((uint16_t)0x0F00)            /*!< EXTI 6 configuration */
#define  AFIO_ESSR2_EXTI7                           ((uint16_t)0xF000)            /*!< EXTI 7 configuration */

/*!< EXTI4 configuration */
#define  AFIO_ESSR2_EXTI4_PA                        ((uint16_t)0x0000)            /*!< PA[4] pin */
#define  AFIO_ESSR2_EXTI4_PB                        ((uint16_t)0x0001)            /*!< PB[4] pin */
#define  AFIO_ESSR2_EXTI4_PC                        ((uint16_t)0x0002)            /*!< PC[4] pin */
#define  AFIO_ESSR2_EXTI4_PD                        ((uint16_t)0x0003)            /*!< PD[4] pin */
#define  AFIO_ESSR2_EXTI4_PE                        ((uint16_t)0x0004)            /*!< PE[4] pin */
#define  AFIO_ESSR2_EXTI4_PF                        ((uint16_t)0x0005)            /*!< PF[4] pin */
#define  AFIO_ESSR2_EXTI4_PG                        ((uint16_t)0x0006)            /*!< PG[4] pin */

/* EXTI5 configuration */
#define  AFIO_ESSR2_EXTI5_PA                        ((uint16_t)0x0000)            /*!< PA[5] pin */
#define  AFIO_ESSR2_EXTI5_PB                        ((uint16_t)0x0010)            /*!< PB[5] pin */
#define  AFIO_ESSR2_EXTI5_PC                        ((uint16_t)0x0020)            /*!< PC[5] pin */
#define  AFIO_ESSR2_EXTI5_PD                        ((uint16_t)0x0030)            /*!< PD[5] pin */
#define  AFIO_ESSR2_EXTI5_PE                        ((uint16_t)0x0040)            /*!< PE[5] pin */
#define  AFIO_ESSR2_EXTI5_PF                        ((uint16_t)0x0050)            /*!< PF[5] pin */
#define  AFIO_ESSR2_EXTI5_PG                        ((uint16_t)0x0060)            /*!< PG[5] pin */

/*!< EXTI6 configuration */  
#define  AFIO_ESSR2_EXTI6_PA                        ((uint16_t)0x0000)            /*!< PA[6] pin */
#define  AFIO_ESSR2_EXTI6_PB                        ((uint16_t)0x0100)            /*!< PB[6] pin */
#define  AFIO_ESSR2_EXTI6_PC                        ((uint16_t)0x0200)            /*!< PC[6] pin */
#define  AFIO_ESSR2_EXTI6_PD                        ((uint16_t)0x0300)            /*!< PD[6] pin */
#define  AFIO_ESSR2_EXTI6_PE                        ((uint16_t)0x0400)            /*!< PE[6] pin */
#define  AFIO_ESSR2_EXTI6_PF                        ((uint16_t)0x0500)            /*!< PF[6] pin */
#define  AFIO_ESSR2_EXTI6_PG                        ((uint16_t)0x0600)            /*!< PG[6] pin */

/*!< EXTI7 configuration */
#define  AFIO_ESSR2_EXTI7_PA                        ((uint16_t)0x0000)            /*!< PA[7] pin */
#define  AFIO_ESSR2_EXTI7_PB                        ((uint16_t)0x1000)            /*!< PB[7] pin */
#define  AFIO_ESSR2_EXTI7_PC                        ((uint16_t)0x2000)            /*!< PC[7] pin */
#define  AFIO_ESSR2_EXTI7_PD                        ((uint16_t)0x3000)            /*!< PD[7] pin */
#define  AFIO_ESSR2_EXTI7_PE                        ((uint16_t)0x4000)            /*!< PE[7] pin */
#define  AFIO_ESSR2_EXTI7_PF                        ((uint16_t)0x5000)            /*!< PF[7] pin */
#define  AFIO_ESSR2_EXTI7_PG                        ((uint16_t)0x6000)            /*!< PG[7] pin */

/*****************  Bit definition for AFIO_EXTICR3 register  *****************/
#define  AFIO_ESSR3_EXTI8                           ((uint16_t)0x000F)            /*!< EXTI 8 configuration */
#define  AFIO_ESSR3_EXTI9                           ((uint16_t)0x00F0)            /*!< EXTI 9 configuration */
#define  AFIO_ESSR3_EXTI10                          ((uint16_t)0x0F00)            /*!< EXTI 10 configuration */
#define  AFIO_ESSR3_EXTI11                          ((uint16_t)0xF000)            /*!< EXTI 11 configuration */

/*!< EXTI8 configuration */
#define  AFIO_ESSR3_EXTI8_PA                        ((uint16_t)0x0000)            /*!< PA[8] pin */
#define  AFIO_ESSR3_EXTI8_PB                        ((uint16_t)0x0001)            /*!< PB[8] pin */
#define  AFIO_ESSR3_EXTI8_PC                        ((uint16_t)0x0002)            /*!< PC[8] pin */
#define  AFIO_ESSR3_EXTI8_PD                        ((uint16_t)0x0003)            /*!< PD[8] pin */
#define  AFIO_ESSR3_EXTI8_PE                        ((uint16_t)0x0004)            /*!< PE[8] pin */
#define  AFIO_ESSR3_EXTI8_PF                        ((uint16_t)0x0005)            /*!< PF[8] pin */
#define  AFIO_ESSR3_EXTI8_PG                        ((uint16_t)0x0006)            /*!< PG[8] pin */

/*!< EXTI9 configuration */
#define  AFIO_ESSR3_EXTI9_PA                        ((uint16_t)0x0000)            /*!< PA[9] pin */
#define  AFIO_ESSR3_EXTI9_PB                        ((uint16_t)0x0010)            /*!< PB[9] pin */
#define  AFIO_ESSR3_EXTI9_PC                        ((uint16_t)0x0020)            /*!< PC[9] pin */
#define  AFIO_ESSR3_EXTI9_PD                        ((uint16_t)0x0030)            /*!< PD[9] pin */
#define  AFIO_ESSR3_EXTI9_PE                        ((uint16_t)0x0040)            /*!< PE[9] pin */
#define  AFIO_ESSR3_EXTI9_PF                        ((uint16_t)0x0050)            /*!< PF[9] pin */
#define  AFIO_ESSR3_EXTI9_PG                        ((uint16_t)0x0060)            /*!< PG[9] pin */

/*!< EXTI10 configuration */  
#define  AFIO_ESSR3_EXTI10_PA                       ((uint16_t)0x0000)            /*!< PA[10] pin */
#define  AFIO_ESSR3_EXTI10_PB                       ((uint16_t)0x0100)            /*!< PB[10] pin */
#define  AFIO_ESSR3_EXTI10_PC                       ((uint16_t)0x0200)            /*!< PC[10] pin */
#define  AFIO_ESSR3_EXTI10_PD                       ((uint16_t)0x0300)            /*!< PD[10] pin */
#define  AFIO_ESSR3_EXTI10_PE                       ((uint16_t)0x0400)            /*!< PE[10] pin */
#define  AFIO_ESSR3_EXTI10_PF                       ((uint16_t)0x0500)            /*!< PF[10] pin */
#define  AFIO_ESSR3_EXTI10_PG                       ((uint16_t)0x0600)            /*!< PG[10] pin */

/*!< EXTI11 configuration */
#define  AFIO_ESSR3_EXTI11_PA                       ((uint16_t)0x0000)            /*!< PA[11] pin */
#define  AFIO_ESSR3_EXTI11_PB                       ((uint16_t)0x1000)            /*!< PB[11] pin */
#define  AFIO_ESSR3_EXTI11_PC                       ((uint16_t)0x2000)            /*!< PC[11] pin */
#define  AFIO_ESSR3_EXTI11_PD                       ((uint16_t)0x3000)            /*!< PD[11] pin */
#define  AFIO_ESSR3_EXTI11_PE                       ((uint16_t)0x4000)            /*!< PE[11] pin */
#define  AFIO_ESSR3_EXTI11_PF                       ((uint16_t)0x5000)            /*!< PF[11] pin */
#define  AFIO_ESSR3_EXTI11_PG                       ((uint16_t)0x6000)            /*!< PG[11] pin */

/*****************  Bit definition for AFIO_EXTICR4 register  *****************/
#define  AFIO_ESSR4_EXTI12                          ((uint16_t)0x000F)            /*!< EXTI 12 configuration */
#define  AFIO_ESSR4_EXTI13                          ((uint16_t)0x00F0)            /*!< EXTI 13 configuration */
#define  AFIO_ESSR4_EXTI14                          ((uint16_t)0x0F00)            /*!< EXTI 14 configuration */
#define  AFIO_ESSR4_EXTI15                          ((uint16_t)0xF000)            /*!< EXTI 15 configuration */

/* EXTI12 configuration */
#define  AFIO_ESSR4_EXTI12_PA                       ((uint16_t)0x0000)            /*!< PA[12] pin */
#define  AFIO_ESSR4_EXTI12_PB                       ((uint16_t)0x0001)            /*!< PB[12] pin */
#define  AFIO_ESSR4_EXTI12_PC                       ((uint16_t)0x0002)            /*!< PC[12] pin */
#define  AFIO_ESSR4_EXTI12_PD                       ((uint16_t)0x0003)            /*!< PD[12] pin */
#define  AFIO_ESSR4_EXTI12_PE                       ((uint16_t)0x0004)            /*!< PE[12] pin */
#define  AFIO_ESSR4_EXTI12_PF                       ((uint16_t)0x0005)            /*!< PF[12] pin */
#define  AFIO_ESSR4_EXTI12_PG                       ((uint16_t)0x0006)            /*!< PG[12] pin */

/* EXTI13 configuration */
#define  AFIO_ESSR4_EXTI13_PA                       ((uint16_t)0x0000)            /*!< PA[13] pin */
#define  AFIO_ESSR4_EXTI13_PB                       ((uint16_t)0x0010)            /*!< PB[13] pin */
#define  AFIO_ESSR4_EXTI13_PC                       ((uint16_t)0x0020)            /*!< PC[13] pin */
#define  AFIO_ESSR4_EXTI13_PD                       ((uint16_t)0x0030)            /*!< PD[13] pin */
#define  AFIO_ESSR4_EXTI13_PE                       ((uint16_t)0x0040)            /*!< PE[13] pin */
#define  AFIO_ESSR4_EXTI13_PF                       ((uint16_t)0x0050)            /*!< PF[13] pin */
#define  AFIO_ESSR4_EXTI13_PG                       ((uint16_t)0x0060)            /*!< PG[13] pin */

/*!< EXTI14 configuration */  
#define  AFIO_ESSR4_EXTI14_PA                       ((uint16_t)0x0000)            /*!< PA[14] pin */
#define  AFIO_ESSR4_EXTI14_PB                       ((uint16_t)0x0100)            /*!< PB[14] pin */
#define  AFIO_ESSR4_EXTI14_PC                       ((uint16_t)0x0200)            /*!< PC[14] pin */
#define  AFIO_ESSR4_EXTI14_PD                       ((uint16_t)0x0300)            /*!< PD[14] pin */
#define  AFIO_ESSR4_EXTI14_PE                       ((uint16_t)0x0400)            /*!< PE[14] pin */
#define  AFIO_ESSR4_EXTI14_PF                       ((uint16_t)0x0500)            /*!< PF[14] pin */
#define  AFIO_ESSR4_EXTI14_PG                       ((uint16_t)0x0600)            /*!< PG[14] pin */

/*!< EXTI15 configuration */
#define  AFIO_ESSR4_EXTI15_PA                       ((uint16_t)0x0000)            /*!< PA[15] pin */
#define  AFIO_ESSR4_EXTI15_PB                       ((uint16_t)0x1000)            /*!< PB[15] pin */
#define  AFIO_ESSR4_EXTI15_PC                       ((uint16_t)0x2000)            /*!< PC[15] pin */
#define  AFIO_ESSR4_EXTI15_PD                       ((uint16_t)0x3000)            /*!< PD[15] pin */
#define  AFIO_ESSR4_EXTI15_PE                       ((uint16_t)0x4000)            /*!< PE[15] pin */
#define  AFIO_ESSR4_EXTI15_PF                       ((uint16_t)0x5000)            /*!< PF[15] pin */
#define  AFIO_ESSR4_EXTI15_PG                       ((uint16_t)0x6000)            /*!< PG[15] pin */

/******************  Bit definition for AFIO_PCFR3 register  ******************/
#define AFIO_PCFR3_REMAP_PH01                       ((uint32_t)0x80000000)        /*!< PH01 remapping */
#define AFIO_PCFR3_REMAP_DCI_HSYNC                  ((uint32_t)0x20000000)        /*!< DCI_HSYNC remapping */
#define AFIO_PCFR3_REMAP_DCI_D13_PG15               ((uint32_t)0x08000000)        /*!< DCI_D13_PG15 remapping */
#define AFIO_PCFR3_REMAP_DCI_D13_PI0                ((uint32_t)0x18000000)        /*!< DCI_D13_PI0 remapping */
#define AFIO_PCFR3_REMAP_DCI_D12_PG6                ((uint32_t)0x04000000)        /*!< DCI_D12 remapping */
#define AFIO_PCFR3_REMAP_DCI_D11_PF10               ((uint32_t)0x01000000)        /*!< DCI_D11_PF10 remapping */
#define AFIO_PCFR3_REMAP_DCI_D11_PH15               ((uint32_t)0x03000000)        /*!< DCI_D11_PH15 remapping */
#define AFIO_PCFR3_REMAP_DCI_D10_PD6                ((uint32_t)0x00400000)        /*!< DCI_D10_PD6  remapping */
#define AFIO_PCFR3_REMAP_DCI_D10_PI3                ((uint32_t)0x00C00000)        /*!< DCI_D10_PI3 remapping */
#define AFIO_PCFR3_REMAP_DCI_D9_PH7                 ((uint32_t)0x00100000)        /*!< DCI_D9_PH7 remapping */
#define AFIO_PCFR3_REMAP_DCI_D9_PI2                 ((uint32_t)0x00300000)        /*!< DCI_D9_PI2 remapping */
#define AFIO_PCFR3_REMAP_DCI_D8_PH6                 ((uint32_t)0x00040000)        /*!< DCI_D8_PH6 remapping */
#define AFIO_PCFR3_REMAP_DCI_D8_PI1                 ((uint32_t)0x000C0000)        /*!< DCI_D8_PI11 remapping */
#define AFIO_PCFR3_REMAP_DCI_D7_PE6                 ((uint32_t)0x00010000)        /*!< DCI_D7_PE6 remapping */
#define AFIO_PCFR3_REMAP_DCI_D7_PI7                 ((uint32_t)0x00030000)        /*!< DCI_D7_PI7 remapping */
#define AFIO_PCFR3_REMAP_DCI_D6_PE5                 ((uint32_t)0x00004000)        /*!< DCI_D6_PE6 remapping */
#define AFIO_PCFR3_REMAP_DCI_D6_PI6                 ((uint32_t)0x0000C000)        /*!< DCI_D6_PI6 remapping */
#define AFIO_PCFR3_REMAP_DCI_D5_PD3                 ((uint32_t)0x00001000)        /*!< DCI_D5_PD3 remapping */
#define AFIO_PCFR3_REMAP_DCI_D5_PI4                 ((uint32_t)0x00003000)        /*!< DCI_D5_PI4 remapping */
#define AFIO_PCFR3_REMAP_DCI_D4_PE4                 ((uint32_t)0x00000400)        /*!< DCI_D4_PE4 remapping */
#define AFIO_PCFR3_REMAP_DCI_D4_PH14                ((uint32_t)0x00000C00)        /*!< DCI_D4_PH14 remapping */
#define AFIO_PCFR3_REMAP_DCI_D3_PE1                 ((uint32_t)0x00000100)        /*!< DCI_D3_PE1 remapping */
#define AFIO_PCFR3_REMAP_DCI_D3_PG11                ((uint32_t)0x00000200)        /*!< DCI_D3_PG11 remapping */
#define AFIO_PCFR3_REMAP_DCI_D3_PH12                ((uint32_t)0x00000300)        /*!< DCI_D3_PH12 remapping */
#define AFIO_PCFR3_REMAP_DCI_D2_PE0                 ((uint32_t)0x00000040)        /*!< DCI_D2_PE0 remapping */
#define AFIO_PCFR3_REMAP_DCI_D2_PG10                ((uint32_t)0x00000080)        /*!< DCI_D2_PG10 remapping */
#define AFIO_PCFR3_REMAP_DCI_D2_PH11                ((uint32_t)0x000000C0)        /*!< DCI_D2_PH11 remapping */
#define AFIO_PCFR3_REMAP_DCI_D1_PC7                 ((uint32_t)0x00000010)        /*!< DCI_D1_PC7 remapping */
#define AFIO_PCFR3_REMAP_DCI_D1_PH10                ((uint32_t)0x00000030)        /*!< DCI_D1_PH10 remapping */
#define AFIO_PCFR3_REMAP_DCI_D0_PC6                 ((uint32_t)0x00000004)        /*!< DCI_D0_PC6 remapping */
#define AFIO_PCFR3_REMAP_DCI_D0_PH9                 ((uint32_t)0x0000000C)        /*!< DCI_D0_PH9 remapping */
#define AFIO_PCFR3_REMAP_DCI_VSYNC_PG9              ((uint32_t)0x00000001)        /*!< DCI_VSYNC_PG9 remapping */
#define AFIO_PCFR3_REMAP_DCI_VSYNC_PI5              ((uint32_t)0x00000003)        /*!< DCI_VSYNC_PI5remapping */

/******************  Bit definition for AFIO_PCFR4 register  ******************/
#define AFIO_PCFR4_REMAP_LCD_B3_PG11                ((uint32_t)0x80000000)        /*!< LCD_B3_PG11 remapping */
#define AFIO_PCFR4_REMAP_LCD_B2_PG10                ((uint32_t)0x40000000)        /*!< LCD_B2_PG10 remapping */
#define AFIO_PCFR4_REMAP_LCD_G3_PG10                ((uint32_t)0x20000000)        /*!< LCD_G3_PG10 remapping */
#define AFIO_PCFR4_REMAP_LCD_CLK_PG7                ((uint32_t)0x10000000)        /*!< LCD_CLK_PG7 remapping */
#define AFIO_PCFR4_REMAP_LCD_R7_PG6                 ((uint32_t)0x08000000)        /*!< LCD_R7_PG6 remapping */
#define AFIO_PCFR4_REMAP_LCD_DE_PF10                ((uint32_t)0x04000000)        /*!< LCD_DE_PF10 remapping */
#define AFIO_PCFR4_REMAP_LCD_R7_PE15                ((uint32_t)0x02000000)        /*!< LCD_R7_PE15 remapping */
#define AFIO_PCFR4_REMAP_LCD_CLK_PE14               ((uint32_t)0x01000000)        /*!< LCD_CLK_PE14 remapping */
#define AFIO_PCFR4_REMAP_LCD_DE_PE13                ((uint32_t)0x00800000)        /*!< LCD_DE_PE13 remapping */
#define AFIO_PCFR4_REMAP_LCD_B4_PE12                ((uint32_t)0x00400000)        /*!< LCD_B4_PE12 remapping */
#define AFIO_PCFR4_REMAP_LCD_G3_PE11                ((uint32_t)0x00200000)        /*!< LCD_G3_PE11 remapping */
#define AFIO_PCFR4_REMAP_LCD_G1_PE6                 ((uint32_t)0x00100000)        /*!< LCD_G1_PE6 remapping */  
#define AFIO_PCFR4_REMAP_LCD_G0_PE5                 ((uint32_t)0x00080000)        /*!< LCD_G0_PE5 remapping */
#define AFIO_PCFR4_REMAP_LCD_B0_PE4                 ((uint32_t)0x00040000)        /*!< LCD_B0_PE4 remapping */
#define AFIO_PCFR4_REMAP_LCD_B3_PD10                ((uint32_t)0x00020000)        /*!< LCD_B3_PD10 remapping */
#define AFIO_PCFR4_REMAP_LCD_B2_P6                  ((uint32_t)0x00010000)        /*!< LCD_B3_PD10 remapping */ 
#define AFIO_PCFR4_REMAP_LCD_G7_PD3                 ((uint32_t)0x00008000)        /*!< LCD_G7_PD3 remapping */ 
#define AFIO_PCFR4_REMAP_LCD_R2_PC10                ((uint32_t)0x00004000)        /*!< LCD_R2_PC10 remapping */
#define AFIO_PCFR4_REMAP_LCD_G6_PC7                 ((uint32_t)0x00002000)        /*!< LCD_G6_PC7 remapping */
#define AFIO_PCFR4_REMAP_LCD_HSYNC_PC6              ((uint32_t)0x00001000)        /*!< LCD_HSYNC_PC6 remapping */
#define AFIO_PCFR4_REMAP_LCD_G5_PB11                ((uint32_t)0x00000800)        /*!< LCD_G5_PB11 remapping */
#define AFIO_PCFR4_REMAP_LCD_G4_PB10                ((uint32_t)0x00000400)        /*!< LCD_G4_PB10 remapping */
#define AFIO_PCFR4_REMAP_LCD_B7_PB9                 ((uint32_t)0x00000200)        /*!< LCD_B7_PB9  remapping */
#define AFIO_PCFR4_REMAP_LCD_B6_PB8                 ((uint32_t)0x00000100)        /*!< LCD_B6_PB8  remapping */  
#define AFIO_PCFR4_REMAP_LCD_R6_PB1                 ((uint32_t)0x00000080)        /*!< LCD_R6_PB1 remapping */
#define AFIO_PCFR4_REMAP_LCD_R3_PB0                 ((uint32_t)0x00000040)        /*!< LCD_R3_PB0 remapping */
#define AFIO_PCFR4_REMAP_LCD_R5_PA12                ((uint32_t)0x00000020)        /*!< LCD_R5_PA12 remapping */
#define AFIO_PCFR4_REMAP_LCD_R4_PA11                ((uint32_t)0x00000010)        /*!< LCD_R4_PA11 remapping */  
#define AFIO_PCFR4_REMAP_LCD_R6_PA8                 ((uint32_t)0x00000008)        /*!< LCD_R6_PA8 remapping */
#define AFIO_PCFR4_REMAP_LCD_G2_PA6                 ((uint32_t)0x00000004)        /*!< LCD_G2_PA6 remapping */
#define AFIO_PCFR4_REMAP_LCD_VSYNC_PA4              ((uint32_t)0x00000002)        /*!< LCD_VSYNC_PA4 remapping */
#define AFIO_PCFR4_REMAP_LCD_B5_PA3                 ((uint32_t)0x00000001)        /*!< LCD_B5_PA3 remapping */

/******************  Bit definition for AFIO_PCFR5 register  ******************/
#define AFIO_PCFR5_REMAP_SPI3_MOSI                  ((uint32_t)0x01000000)        /*!< SPI3_MOSI remapping */
#define AFIO_PCFR5_REMAP_SPI2_SCK                   ((uint32_t)0x00800000)        /*!< SPI2_SCK remapping */
#define AFIO_PCFR5_REMAP_LCD_R1_PI3                 ((uint32_t)0x00400000)        /*!< LCD_R1_PI3 remapping */
#define AFIO_PCFR5_REMAP_LCD_R0_PH4                 ((uint32_t)0x00200000)        /*!< LCD_R0_PH4 remapping */
#define AFIO_PCFR5_REMAP_LCD_HSYNC_PI10             ((uint32_t)0x00100000)        /*!< LCD_HSYNC_PI10 remapping */
#define AFIO_PCFR5_REMAP_LCD_VSYNC_PI9              ((uint32_t)0x00080000)        /*!< LCD_VSYNC_PI9 remapping */
#define AFIO_PCFR5_REMAP_LCD_B7_PI7                 ((uint32_t)0x00040000)        /*!< LCD_B7_PI07 remapping */
#define AFIO_PCFR5_REMAP_LCD_B6_PI6                 ((uint32_t)0x00020000)        /*!< LCD_B6_PI06 remapping */
#define AFIO_PCFR5_REMAP_LCD_B5_PI5                 ((uint32_t)0x00010000)        /*!< LCD_B5_PI5 remapping */
#define AFIO_PCFR5_REMAP_LCD_B4_PI4                 ((uint32_t)0x00008000)        /*!< LCD_B4_PI4 remapping */
#define AFIO_PCFR5_REMAP_LCD_G7_PI2                 ((uint32_t)0x00004000)        /*!< LCD_G7_PI2 remapping */
#define AFIO_PCFR5_REMAP_LCD_G6_PI1                 ((uint32_t)0x00002000)        /*!< LCD_G6_PI1 remapping */
#define AFIO_PCFR5_REMAP_LCD_G5_PI0                 ((uint32_t)0x00001000)        /*!< LCD_G5_PI0 remapping */
#define AFIO_PCFR5_REMAP_LCD_G4_PH15                ((uint32_t)0x00000800)        /*!< LCD_G4_PH15 remapping */
#define AFIO_PCFR5_REMAP_LCD_G3_PH14                ((uint32_t)0x00000400)        /*!< LCD_G3_PH14 remapping */
#define AFIO_PCFR5_REMAP_LCD_G2_PH13                ((uint32_t)0x00000200)        /*!< LCD_G2_PH13 remapping */
#define AFIO_PCFR5_REMAP_LCD_R6_PH12                ((uint32_t)0x00000100)        /*!< LCD_R6_PH12 remapping */
#define AFIO_PCFR5_REMAP_LCD_R5_PH11                ((uint32_t)0x00000080)        /*!< LCD_R5_PH11 remapping */
#define AFIO_PCFR5_REMAP_LCD_R4_PH10                ((uint32_t)0x00000040)        /*!< LCD_R4_PH10 remapping */
#define AFIO_PCFR5_REMAP_LCD_R3_PH9                 ((uint32_t)0x00000020)        /*!< LCD_R3_PH9 remapping */
#define AFIO_PCFR5_REMAP_LCD_R2_PH8                 ((uint32_t)0x00000010)        /*!< LCD_R2_PH8 remapping */
#define AFIO_PCFR5_REMAP_LCD_R1_PH3                 ((uint32_t)0x00000008)        /*!< LCD_R1_PH3 remapping */
#define AFIO_PCFR5_REMAP_LCD_R0_PH2                 ((uint32_t)0x00000004)        /*!< LCD_R0_PH2 remapping */
#define AFIO_PCFR5_REMAP_LCD_B1_PG12                ((uint32_t)0x00000002)        /*!< LCD_B1_PG12 remapping */
#define AFIO_PCFR5_REMAP_LCD_B4_PG12                ((uint32_t)0x00000001)        /*!< LCD_B4_PG12 remapping */
/******************  Bit definition for AFIO_PCFR6 register  ******************/
#define AFIO_PCFR6_REMAP_FMC_SDNE1_PB6              ((uint32_t)0x80000000)        /*!< FMC_SDNE1_PB6 remapping */
#define AFIO_PCFR6_REMAP_FMC_SDNE0_PC2              ((uint32_t)0x40000000)        /*!< FMC_SDNE0_PC2 remapping */
#define AFIO_PCFR6_REMAP_FMC_SDCKE1_PB5             ((uint32_t)0x20000000)        /*!< FMC_SDCKE1_PB5 remapping */
#define AFIO_PCFR6_REMAP_FMC_SDCKE0_PC3             ((uint32_t)0x10000000)        /*!< FMC_SDCKE0_PC3 remapping */
#define AFIO_PCFR6_REMAP_FMC_SDNWE_PC0              ((uint32_t)0x08000000)        /*!< USART6_RX_PG9 remapping */
#define AFIO_PCFR6_REMAP_USART6_RX_PG9              ((uint32_t)0x04000000)        /*!< USART6_TX_PG14 remapping */
#define AFIO_PCFR6_REMAP_USART6_TX_PG14             ((uint32_t)0x02000000)        /*!< USART6_TX_PG14 remapping */
#define AFIO_PCFR6_REMAP_USART6_CTS_PG13            ((uint32_t)0x01000000)        /*!< USART6_CTS_PG13 remapping */
#define AFIO_PCFR6_REMAP_USART6_RTS_PG12            ((uint32_t)0x00800000)        /*!< USART6_RTS_PG12 remapping */
#define AFIO_PCFR6_REMAP_USART6_CK_PG7              ((uint32_t)0x00400000)        /*!< USART6_CK_PG7 remapping */
#define AFIO_PCFR6_REMAP_USART7                     ((uint32_t)0x00200000)        /*!< USART7 remapping */
#define AFIO_PCFR6_REMAP_ETH_RX_HI                  ((uint32_t)0x00100000)        /*!< REMAP_ETH_RX_HI remapping */
#define AFIO_PCFR6_REMAP_ETH_CRSCOL                 ((uint32_t)0x00080000)        /*!< ETH_CRSCOL remapping */
#define AFIO_PCFR6_REMAP_ETH_TXD01                  ((uint32_t)0x00040000)        /*!< ETH_TXD01 remapping */
#define AFIO_PCFR6_REMAP_ETH_PPS_HI                 ((uint32_t)0x00020000)        /*!< ETH_PPS_HI remapping */
#define AFIO_PCFR6_REMAP_ETH_TXD3                   ((uint32_t)0x00010000)        /*!< ETH_TXD3 remapping */
#define AFIO_PCFR6_REMAP_CAN_HI                     ((uint32_t)0x00008000)        /*!< CAN_HI remapping */
#define AFIO_PCFR6_REMAP_TM12                       ((uint32_t)0x00004000)        /*!< TM12 remapping */
#define AFIO_PCFR6_REMAP_USART4                     ((uint32_t)0x00002000)        /*!< USART4 remapping */
#define AFIO_PCFR6_REMAP_SPI2                       ((uint32_t)0x00001800)        /*!< SPI2 remapping */
#define AFIO_PCFR6_REMAP_SPI2_NSCK                  ((uint32_t)0x00000600)        /*!< SPI2_NSCK remapping */
#define AFIO_PCFR6_REMAP_I2C2                       ((uint32_t)0x00000180)        /*!< I2C2 remapping */
#define AFIO_PCFR6_REMAP_TM8                        ((uint32_t)0x00000030)        /*!< TM8 remapping */
#define AFIO_PCFR6_REMAP_TM5                        ((uint32_t)0x00000008)        /*!< TM5 remapping */
#define AFIO_PCFR6_REMAP_TM2_CH1                    ((uint32_t)0x00000004)        /*!< TM2_CH1 remapping */
#define AFIO_PCFR6_REMAP_I2C3_REMAP2_EN             ((uint32_t)0x00000002)        /*!< I2C3_REMAP2_EN remapping */
#define AFIO_PCFR6_REMAP_I2C3_REMAP1_EN             ((uint32_t)0x00000001)        /*!< I2C3_REMAP1_EN remapping */


/******************************************************************************/
/*                                                                            */
/*                      Inter-integrated Circuit Interface                    */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for I2C_CTLR1 register  *******************/
#define  I2C_CTLR1_I2CEN                            ((uint16_t)0x0001)            /*!< Peripheral Enable */
#define  I2C_CTLR1_SMBEN                            ((uint16_t)0x0002)            /*!< SMBus Mode */
#define  I2C_CTLR1_SMBSEL                           ((uint16_t)0x0008)            /*!< SMBus Type */
#define  I2C_CTLR1_ARPEN                            ((uint16_t)0x0010)            /*!< ARP Enable */
#define  I2C_CTLR1_PECEN                            ((uint16_t)0x0020)            /*!< PEC Enable */
#define  I2C_CTLR1_GCEN                             ((uint16_t)0x0040)            /*!< General Call Enable */
#define  I2C_CTLR1_DISSTRC                          ((uint16_t)0x0080)            /*!< Clock Stretching Disable (Slave mode) */
#define  I2C_CTLR1_GENSTA                           ((uint16_t)0x0100)            /*!< Start Generation */
#define  I2C_CTLR1_GENSTP                           ((uint16_t)0x0200)            /*!< Stop Generation */
#define  I2C_CTLR1_ACKEN                            ((uint16_t)0x0400)            /*!< Acknowledge Enable */
#define  I2C_CTLR1_POAP                             ((uint16_t)0x0800)            /*!< Acknowledge/PEC Position (for data reception) */
#define  I2C_CTLR1_PECTRANS                         ((uint16_t)0x1000)            /*!< Packet Error Checking */
#define  I2C_CTLR1_SALT                             ((uint16_t)0x2000)            /*!< SMBus Alert */
#define  I2C_CTLR1_SRESET                           ((uint16_t)0x8000)            /*!< Software Reset */

/*******************  Bit definition for I2C_CTLR2 register  ********************/
#define  I2C_CTLR2_I2CCLK                           ((uint16_t)0x003F)            /*!< I2CCLK[5:0] bits (Peripheral Clock Frequency) */
#define  I2C_CTLR2_I2CCLK_0                         ((uint16_t)0x0001)            /*!< Bit 0 */
#define  I2C_CTLR2_I2CCLK_1                         ((uint16_t)0x0002)            /*!< Bit 1 */
#define  I2C_CTLR2_I2CCLK_2                         ((uint16_t)0x0004)            /*!< Bit 2 */
#define  I2C_CTLR2_I2CCLK_3                         ((uint16_t)0x0008)            /*!< Bit 3 */
#define  I2C_CTLR2_I2CCLK_4                         ((uint16_t)0x0010)            /*!< Bit 4 */
#define  I2C_CTLR2_I2CCLK_5                         ((uint16_t)0x0020)            /*!< Bit 5 */

#define  I2C_CTLR2_EIE                              ((uint16_t)0x0100)            /*!< Error Interrupt Enable */
#define  I2C_CTLR2_EE                               ((uint16_t)0x0200)            /*!< Event Interrupt Enable */
#define  I2C_CTLR2_BIE                              ((uint16_t)0x0400)            /*!< Buffer Interrupt Enable */
#define  I2C_CTLR2_DMAON                            ((uint16_t)0x0800)            /*!< DMA Requests Enable */
#define  I2C_CTLR2_DMALST                           ((uint16_t)0x1000)            /*!< DMA Last Transfer */

/*******************  Bit definition for I2C_AR1 register  *******************/
#define  I2C_AR1_ADDRESS1_7                         ((uint16_t)0x00FE)            /*!< Interface Address */
#define  I2C_AR1_ADDRESS8_9                         ((uint16_t)0x0300)            /*!< Interface Address */
											  
#define  I2C_AR1_ADDRESS0                           ((uint16_t)0x0001)            /*!< Bit 0 */
#define  I2C_AR1_ADDRESS1                           ((uint16_t)0x0002)            /*!< Bit 1 */
#define  I2C_AR1_ADDRESS2                           ((uint16_t)0x0004)            /*!< Bit 2 */
#define  I2C_AR1_ADDRESS3                           ((uint16_t)0x0008)            /*!< Bit 3 */
#define  I2C_AR1_ADDRESS4                           ((uint16_t)0x0010)            /*!< Bit 4 */
#define  I2C_AR1_ADDRESS5                           ((uint16_t)0x0020)            /*!< Bit 5 */
#define  I2C_AR1_ADDRESS6                           ((uint16_t)0x0040)            /*!< Bit 6 */
#define  I2C_AR1_ADDRESS7                           ((uint16_t)0x0080)            /*!< Bit 7 */
#define  I2C_AR1_ADDRESS8                           ((uint16_t)0x0100)            /*!< Bit 8 */
#define  I2C_AR1_ADDRESS9                           ((uint16_t)0x0200)            /*!< Bit 9 */

#define  I2C_AR1_ADDFORMAT                          ((uint16_t)0x8000)            /*!< Addressing Mode (Slave mode) */

/*******************  Bit definition for I2C_AR2 register  *******************/
#define  I2C_AR2_DUADEN                             ((uint8_t)0x01)               /*!< Dual addressing mode enable */
#define  I2C_AR2_ADDRESS2                           ((uint8_t)0xFE)               /*!< Interface address */

/********************  Bit definition for I2C_DTR register  ********************/
#define  I2C_DTR_TRB                                ((uint8_t)0xFF)               /*!< 8-bit Data Register */

/*******************  Bit definition for I2C_STR1 register  ********************/
#define  I2C_STR1_SBSEND                            ((uint16_t)0x0001)            /*!< Start Bit (Master mode) */
#define  I2C_STR1_ADDSEND                           ((uint16_t)0x0002)            /*!< Address sent (master mode)/matched (slave mode) */
#define  I2C_STR1_BTC                               ((uint16_t)0x0004)            /*!< Byte Transfer Finished */
#define  I2C_STR1_ADD10SEND                         ((uint16_t)0x0008)            /*!< 10-bit header sent (Master mode) */
#define  I2C_STR1_STPDET                            ((uint16_t)0x0010)            /*!< Stop detection (Slave mode) */
#define  I2C_STR1_RBNE                              ((uint16_t)0x0040)            /*!< Data Register not Empty (receivers) */
#define  I2C_STR1_TBE                               ((uint16_t)0x0080)            /*!< Data Register Empty (transmitters) */
#define  I2C_STR1_BE                                ((uint16_t)0x0100)            /*!< Bus Error */
#define  I2C_STR1_LOSTARB                           ((uint16_t)0x0200)            /*!< Arbitration Lost (master mode) */
#define  I2C_STR1_AE                                ((uint16_t)0x0400)            /*!< Acknowledge Failure */
#define  I2C_STR1_RXORE                             ((uint16_t)0x0800)            /*!< Overrun/Underrun */
#define  I2C_STR1_PECE                              ((uint16_t)0x1000)            /*!< PEC Error in reception */
#define  I2C_STR1_SMBTO                             ((uint16_t)0x4000)            /*!< Timeout or Tlow Error */
#define  I2C_STR1_SMBALTS                           ((uint16_t)0x8000)            /*!< SMBus Alert */

/*******************  Bit definition for I2C_STR2 register  ********************/
#define  I2C_STR2_MASTER                            ((uint16_t)0x0001)            /*!< Master/Slave */
#define  I2C_STR2_I2CBSY                            ((uint16_t)0x0002)            /*!< Bus Busy */
#define  I2C_STR2_TRS                               ((uint16_t)0x0004)            /*!< Transmitter/Receiver */
#define  I2C_STR2_RXGC                              ((uint16_t)0x0010)            /*!< General Call Address (Slave mode) */
#define  I2C_STR2_DEFSMB                            ((uint16_t)0x0020)            /*!< SMBus Device Default Address (Slave mode) */
#define  I2C_STR2_HSTSMB                            ((uint16_t)0x0040)            /*!< SMBus Host Header (Slave mode) */
#define  I2C_STR2_DUMODF                            ((uint16_t)0x0080)            /*!< Dual Flag (Slave mode) */
#define  I2C_STR2_ECV                               ((uint16_t)0xFF00)            /*!< Packet Error Checking Register */

/*******************  Bit definition for I2C_CLKR register  ********************/
#define  I2C_CLKR_CLKC                              ((uint16_t)0x0FFF)            /*!< Clock Control Register in Fast/Standard mode (Master mode) */
#define  I2C_CLKR_DTCY                              ((uint16_t)0x4000)            /*!< Fast Mode Duty Cycle */
#define  I2C_CLKR_FAST                              ((uint16_t)0x8000)            /*!< I2C speed selection in master mode */

/******************  Bit definition for I2C_RTR register  *******************/
#define  I2C_RTR_RISETIME                           ((uint8_t)0x3F)               /*!< Maximum Rise Time in Fast/Standard mode (Master mode) */

/******************************************************************************/
/*                                                                            */
/*                        Independent WATCHDOG (IWDG)                         */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for IWDG_CTLR register  ******************/
#define  IWDG_CTLR_CT                               ((uint16_t)0xFFFF)            /*!< Key value (write only, read 0000h) */

/*******************  Bit definition for IWDG_PSR register  ********************/
#define  IWDG_PSR_PS                                ((uint8_t)0x07)               /*!< PS[2:0] (Prescaler divider) */
#define  IWDG_PSR_PS_0                              ((uint8_t)0x01)               /*!< Bit 0 */
#define  IWDG_PSR_PS_1                              ((uint8_t)0x02)               /*!< Bit 1 */
#define  IWDG_PSR_PS_2                              ((uint8_t)0x04)               /*!< Bit 2 */

/*******************  Bit definition for IWDG_RLDR register  *******************/
#define  IWDG_RLDR_RLD                              ((uint16_t)0x0FFF)            /*!< Watchdog counter reload value */

/*******************  Bit definition for IWDG_STR register  ********************/
#define  IWDG_STR_PUD                               ((uint8_t)0x01)               /*!< Watchdog prescaler value update */
#define  IWDG_STR_RUD                               ((uint8_t)0x02)               /*!< Watchdog counter reload value update */
#define  IWDG_STR_WUD                               ((uint8_t)0x04)               /*!< Watchdog counter window value update */


/******************************************************************************/
/*                                                                            */
/*                             Power Control                                  */
/*                                                                            */
/******************************************************************************/

/********************  Bit definition for PWR_CTLR register  ********************/
#define  PWR_CTLR_LDOLP                             ((uint16_t)0x0001)            /*!< LDO Low Power Mode */
#define  PWR_CTLR_SDBM                              ((uint16_t)0x0002)            /*!< Standby Mode */
#define  PWR_CTLR_WUFR                              ((uint16_t)0x0004)            /*!< Wakeup Flag Reset */
#define  PWR_CTLR_SBFR                              ((uint16_t)0x0008)            /*!< Standby Flag Reset */
#define  PWR_CTLR_LVDE                              ((uint16_t)0x0010)            /*!< Low Voltage Detector Enable */

#define  PWR_CTLR_LVDT                              ((uint16_t)0x00E0)            /*!< LVDT[2:0] bits (Low Voltage Detector Threshold) */
#define  PWR_CTLR_LVDT_0                            ((uint16_t)0x0020)            /*!< Bit 0 */
#define  PWR_CTLR_LVDT_1                            ((uint16_t)0x0040)            /*!< Bit 1 */
#define  PWR_CTLR_LVDT_2                            ((uint16_t)0x0080)            /*!< Bit 2 */

/*!< LVDT configuration */
#define  PWR_CTLR_LVDT_2V2                          ((uint16_t)0x0000)            /*!< LVDT is 2.2V */
#define  PWR_CTLR_LVDT_2V3                          ((uint16_t)0x0020)            /*!< LVDT is 2.3V */
#define  PWR_CTLR_LVDT_2V4                          ((uint16_t)0x0040)            /*!< LVDT is 2.4V */
#define  PWR_CTLR_LVDT_2V5                          ((uint16_t)0x0060)            /*!< LVDT is 2.5V */
#define  PWR_CTLR_LVDT_2V6                          ((uint16_t)0x0080)            /*!< LVDT is 2.6V */
#define  PWR_CTLR_LVDT_2V7                          ((uint16_t)0x00A0)            /*!< LVDT is 2.7V */
#define  PWR_CTLR_LVDT_2V8                          ((uint16_t)0x00C0)            /*!< LVDT is 2.8V */
#define  PWR_CTLR_LVDT_2V9                          ((uint16_t)0x00E0)            /*!< LVDT is 2.9V */
								        								       
#define  PWR_CTLR_BKPWE                             ((uint16_t)0x0100)            /*!< Backup Domain Write Enable */

/*******************  Bit definition for PWR_STR register  ********************/
#define  PWR_STR_WUF                                ((uint16_t)0x0001)            /*!< Wakeup Flag */
#define  PWR_STR_SBF                                ((uint16_t)0x0002)            /*!< Standby Flag */
#define  PWR_STR_LVDF                               ((uint16_t)0x0004)            /*!< Low Voltage Detector Status Flag */
#define  PWR_STR_WUPE                               ((uint16_t)0x0100)            /*!< WKUP Pin Enable */

/******************************************************************************/
/*                                                                            */
/*                         Reset and Clock Control                            */
/*                                                                            */
/******************************************************************************/


/********************  Bit definition for RCC_GCCR register  ********************/
#define  RCC_GCCR_HSIEN                             ((uint32_t)0x00000001)        /*!< Internal High Speed clock enable */
#define  RCC_GCCR_HSISTB                            ((uint32_t)0x00000002)        /*!< Internal High Speed clock ready flag */
#define  RCC_GCCR_HSIADJ                            ((uint32_t)0x000000F8)        /*!< Internal High Speed clock trimming */
#define  RCC_GCCR_HSICALIB                          ((uint32_t)0x0000FF00)        /*!< Internal High Speed clock Calibration */
#define  RCC_GCCR_HSEEN                             ((uint32_t)0x00010000)        /*!< External High Speed clock enable */
#define  RCC_GCCR_HSESTB                            ((uint32_t)0x00020000)        /*!< External High Speed clock ready flag */
#define  RCC_GCCR_HSEBPS                            ((uint32_t)0x00040000)        /*!< External High Speed clock Bypass */
#define  RCC_GCCR_CKMEN                             ((uint32_t)0x00080000)        /*!< Clock Security System enable */
#define  RCC_GCCR_PLLEN                             ((uint32_t)0x01000000)        /*!< PLL enable */
#define  RCC_GCCR_PLLSTB                            ((uint32_t)0x02000000)        /*!< PLL clock ready flag */
#define  RCC_GCCR_PLL2EN                            ((uint32_t)0x04000000)        /*!< PLL2 enable */
#define  RCC_GCCR_PLL2STB                           ((uint32_t)0x08000000)        /*!< PLL2 clock ready flag */
#define  RCC_GCCR_PLL3EN                            ((uint32_t)0x10000000)        /*!< PLL3 enable */
#define  RCC_GCCR_PLL3STB                           ((uint32_t)0x20000000)        /*!< PLL3 clock ready flag */

/*******************  Bit definition for RCC_GCFGR register  *******************/
/*!< SW configuration */
#define  RCC_GCFGR_SCS                              ((uint32_t)0x00000003)        /*!< SCS[1:0] bits (System clock Switch) */
#define  RCC_GCFGR_SCS_0                            ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  RCC_GCFGR_SCS_1                            ((uint32_t)0x00000002)        /*!< Bit 1 */

#define  RCC_GCFGR_SCS_HSI                          ((uint32_t)0x00000000)        /*!< HSI selected as system clock */
#define  RCC_GCFGR_SCS_HSE                          ((uint32_t)0x00000001)        /*!< HSE selected as system clock */
#define  RCC_GCFGR_SCS_PLL                          ((uint32_t)0x00000002)        /*!< PLL selected as system clock */

/*!< SWS configuration */
#define  RCC_GCFGR_SCSS                             ((uint32_t)0x0000000C)        /*!< SCSS[1:0] bits (System Clock Switch Status) */
#define  RCC_GCFGR_SCSS_0                           ((uint32_t)0x00000004)        /*!< Bit 0 */
#define  RCC_GCFGR_SCSS_1                           ((uint32_t)0x00000008)        /*!< Bit 1 */

#define  RCC_GCFGR_SCSS_HSI                         ((uint32_t)0x00000000)        /*!< HSI oscillator used as system clock */
#define  RCC_GCFGR_SCSS_HSE                         ((uint32_t)0x00000004)        /*!< HSE oscillator used as system clock */
#define  RCC_GCFGR_SCSS_PLL                         ((uint32_t)0x00000008)        /*!< PLL used as system clock */

/*!< AHBPS configuration */
#define  RCC_GCFGR_AHBPS                            ((uint32_t)0x000000F0)        /*!< AHBPS[3:0] bits (AHB prescaler) */
#define  RCC_GCFGR_AHBPS_0                          ((uint32_t)0x00000010)        /*!< Bit 0 */
#define  RCC_GCFGR_AHBPS_1                          ((uint32_t)0x00000020)        /*!< Bit 1 */
#define  RCC_GCFGR_AHBPS_2                          ((uint32_t)0x00000040)        /*!< Bit 2 */
#define  RCC_GCFGR_AHBPS_3                          ((uint32_t)0x00000080)        /*!< Bit 3 */

#define  RCC_GCFGR_AHBPS_DIV1                       ((uint32_t)0x00000000)        /*!< SYSCLK not divided */
#define  RCC_GCFGR_AHBPS_DIV2                       ((uint32_t)0x00000080)        /*!< SYSCLK divided by 2 */
#define  RCC_GCFGR_AHBPS_DIV4                       ((uint32_t)0x00000090)        /*!< SYSCLK divided by 4 */
#define  RCC_GCFGR_AHBPS_DIV8                       ((uint32_t)0x000000A0)        /*!< SYSCLK divided by 8 */
#define  RCC_GCFGR_AHBPS_DIV16                      ((uint32_t)0x000000B0)        /*!< SYSCLK divided by 16 */
#define  RCC_GCFGR_AHBPS_DIV64                      ((uint32_t)0x000000C0)        /*!< SYSCLK divided by 64 */
#define  RCC_GCFGR_AHBPS_DIV128                     ((uint32_t)0x000000D0)        /*!< SYSCLK divided by 128 */
#define  RCC_GCFGR_AHBPS_DIV256                     ((uint32_t)0x000000E0)        /*!< SYSCLK divided by 256 */
#define  RCC_GCFGR_AHBPS_DIV512                     ((uint32_t)0x000000F0)        /*!< SYSCLK divided by 512 */
									        
/*!< APB1PS configuration */
#define  RCC_GCFGR_APB1PS                           ((uint32_t)0x00000700)        /*!< APB1PS[2:0] bits (APB1 prescaler) */
#define  RCC_GCFGR_APB1PS_0                         ((uint32_t)0x00000100)        /*!< Bit 0 */
#define  RCC_GCFGR_APB1PS_1                         ((uint32_t)0x00000200)        /*!< Bit 1 */
#define  RCC_GCFGR_APB1PS_2                         ((uint32_t)0x00000400)        /*!< Bit 2 */

#define  RCC_GCFGR_APB1PS_DIV1                      ((uint32_t)0x00000000)        /*!< AHB not divided */
#define  RCC_GCFGR_APB1PS_DIV2                      ((uint32_t)0x00000400)        /*!< AHB divided by 2 */
#define  RCC_GCFGR_APB1PS_DIV4                      ((uint32_t)0x00000500)        /*!< AHB divided by 4 */
#define  RCC_GCFGR_APB1PS_DIV8                      ((uint32_t)0x00000600)        /*!< AHB divided by 8 */
#define  RCC_GCFGR_APB1PS_DIV16                     ((uint32_t)0x00000700)        /*!< AHB divided by 16 */

/*!< APB2PS configuration */
#define  RCC_GCFGR_APB2PS                           ((uint32_t)0x00003800)        /*!< APB2PS[2:0] bits (APB2 prescaler) */
#define  RCC_GCFGR_APB2PS_0                         ((uint32_t)0x00000800)        /*!< Bit 0 */
#define  RCC_GCFGR_APB2PS_1                         ((uint32_t)0x00001000)        /*!< Bit 1 */
#define  RCC_GCFGR_APB2PS_2                         ((uint32_t)0x00002000)        /*!< Bit 2 */

#define  RCC_GCFGR_APB2PS_DIV1                      ((uint32_t)0x00000000)        /*!< AHB not divided */
#define  RCC_GCFGR_APB2PS_DIV2                      ((uint32_t)0x00002000)        /*!< AHB divided by 2 */
#define  RCC_GCFGR_APB2PS_DIV4                      ((uint32_t)0x00002800)        /*!< AHB divided by 4 */
#define  RCC_GCFGR_APB2PS_DIV8                      ((uint32_t)0x00003000)        /*!< AHB divided by 8 */
#define  RCC_GCFGR_APB2PS_DIV16                     ((uint32_t)0x00003800)        /*!< AHB divided by 16 */

/*!< ADCPS configuration */
#define  RCC_GCFGR_ADCPS                            ((uint32_t)0x1000C000)        /*!< ADCPS[2:0] bits (ADC prescaler) */
#define  RCC_GCFGR_ADCPS_0                          ((uint32_t)0x00004000)        /*!< Bit 0 */
#define  RCC_GCFGR_ADCPS_1                          ((uint32_t)0x00008000)        /*!< Bit 1 */
#define  RCC_GCFGR_ADCPS_2                          ((uint32_t)0x10000000)        /*!< Bit 2 */
									       
#define  RCC_GCFGR_ADCPS_DIV2                       ((uint32_t)0x00000000)        /*!< APB2 divided by 2 */
#define  RCC_GCFGR_ADCPS_DIV4                       ((uint32_t)0x00004000)        /*!< APB2 divided by 4 */
#define  RCC_GCFGR_ADCPS_DIV6                       ((uint32_t)0x00008000)        /*!< APB2 divided by 6 */
#define  RCC_GCFGR_ADCPS_DIV8                       ((uint32_t)0x0000C000)        /*!< APB2 divided by 8 */
#define  RCC_GCFGR_ADCPS_DIV12                      ((uint32_t)0x10004000)        /*!< APB2 divided by 12 */
#define  RCC_GCFGR_ADCPS_DIV16                      ((uint32_t)0x1000C000)        /*!< APB2 divided by 16 */

/*!< PLLSEL configuration */
#define  RCC_GCFGR_PLLSEL                           ((uint32_t)0x00010000)        /*!< PLLSEL bit for entry clock source */
#define  RCC_GCFGR_PLLSEL_HSI_DIV2                  ((uint32_t)0x00000000)        /*!< HSI clock divided by 2 selected as PLL entry clock source */
#define  RCC_GCFGR_PLLSEL_PREDIV1                   ((uint32_t)0x00010000)        /*!< PREDIV1 clock selected as PLL entry clock source */

/*!< PLLPREDV configuration */
#define  RCC_GCFGR_PLLPREDV                         ((uint32_t)0x00020000)        /*!< PLLPREDV bit for PLL entry */
#define  RCC_GCFGR_PLLPREDV_PREDIV1                 ((uint32_t)0x00000000)        /*!< PREDIV1 clock not divided for PLL entry */
#define  RCC_GCFGR_PLLPREDV_PREDIV1_DIV2            ((uint32_t)0x00020000)        /*!< PREDIV1 clock divided by 2 for PLL entry */

/*!< PLLMF configuration */
#define  RCC_GCFGR_PLLMF                            ((uint32_t)0x203C0000)        /*!< PLLMF[4:0] bits (PLL multiplication factor) */
#define  RCC_GCFGR_PLLMF_0                          ((uint32_t)0x00040000)        /*!< Bit 0 */
#define  RCC_GCFGR_PLLMF_1                          ((uint32_t)0x00080000)        /*!< Bit 1 */
#define  RCC_GCFGR_PLLMF_2                          ((uint32_t)0x00100000)        /*!< Bit 2 */
#define  RCC_GCFGR_PLLMF_3                          ((uint32_t)0x00200000)        /*!< Bit 3 */
#define  RCC_GCFGR_PLLMF_4                          ((uint32_t)0x20000000)        /*!< Bit 4 */
								     	       
#define  RCC_GCFGR_PLLMF2                           ((uint32_t)0x00000000)        /*!< PLL input clock*2 */
#define  RCC_GCFGR_PLLMF3                           ((uint32_t)0x00040000)        /*!< PLL input clock*3 */
#define  RCC_GCFGR_PLLMF4                           ((uint32_t)0x00080000)        /*!< PLL input clock*4 */
#define  RCC_GCFGR_PLLMF5                           ((uint32_t)0x000C0000)        /*!< PLL input clock*5 */
#define  RCC_GCFGR_PLLMF6                           ((uint32_t)0x00100000)        /*!< PLL input clock*6 */
#define  RCC_GCFGR_PLLMF7                           ((uint32_t)0x00140000)        /*!< PLL input clock*7 */
#define  RCC_GCFGR_PLLMF8                           ((uint32_t)0x00180000)        /*!< PLL input clock*8 */
#define  RCC_GCFGR_PLLMF9                           ((uint32_t)0x001C0000)        /*!< PLL input clock*9 */
#define  RCC_GCFGR_PLLMF10                          ((uint32_t)0x00200000)        /*!< PLL input clock10 */
#define  RCC_GCFGR_PLLMF11                          ((uint32_t)0x00240000)        /*!< PLL input clock*11 */
#define  RCC_GCFGR_PLLMF12                          ((uint32_t)0x00280000)        /*!< PLL input clock*12 */
#define  RCC_GCFGR_PLLMF13                          ((uint32_t)0x002C0000)        /*!< PLL input clock*13 */
#define  RCC_GCFGR_PLLMF14                          ((uint32_t)0x00300000)        /*!< PLL input clock*14 */
#define  RCC_GCFGR_PLLMF6_5                         ((uint32_t)0x00340000)        /*!< PLL input clock*6.5 */
#define  RCC_GCFGR_PLLMF16                          ((uint32_t)0x00380000)        /*!< PLL input clock*16 */
#define  RCC_GCFGR_PLLMF17                          ((uint32_t)0x20000000)        /*!< PLL input clock*17 */
#define  RCC_GCFGR_PLLMF18                          ((uint32_t)0x20040000)        /*!< PLL input clock*18 */
#define  RCC_GCFGR_PLLMF19                          ((uint32_t)0x20080000)        /*!< PLL input clock*19 */
#define  RCC_GCFGR_PLLMF20                          ((uint32_t)0x200C0000)        /*!< PLL input clock*20 */
#define  RCC_GCFGR_PLLMF21                          ((uint32_t)0x20100000)        /*!< PLL input clock*21 */
#define  RCC_GCFGR_PLLMF22                          ((uint32_t)0x20140000)        /*!< PLL input clock*22 */
#define  RCC_GCFGR_PLLMF23                          ((uint32_t)0x20180000)        /*!< PLL input clock*23 */
#define  RCC_GCFGR_PLLMF24                          ((uint32_t)0x201C0000)        /*!< PLL input clock*24 */
#define  RCC_GCFGR_PLLMF25                          ((uint32_t)0x20200000)        /*!< PLL input clock*25 */
#define  RCC_GCFGR_PLLMF26                          ((uint32_t)0x20240000)        /*!< PLL input clock*26 */
#define  RCC_GCFGR_PLLMF27                          ((uint32_t)0x20280000)        /*!< PLL input clock*27 */
#define  RCC_GCFGR_PLLMF28                          ((uint32_t)0x202C0000)        /*!< PLL input clock*28 */
#define  RCC_GCFGR_PLLMF29                          ((uint32_t)0x20300000)        /*!< PLL input clock*29 */
#define  RCC_GCFGR_PLLMF30                          ((uint32_t)0x20340000)        /*!< PLL input clock*30 */
#define  RCC_GCFGR_PLLMF31                          ((uint32_t)0x20380000)        /*!< PLL input clock*31 */
#define  RCC_GCFGR_PLLMF32                          ((uint32_t)0x203C0000)        /*!< PLL input clock*32 */

#define  RCC_GCFGR_OTGFSPS                          ((uint32_t)0x00C00000)        /*!< OTGFSPS[1:0] bits USB OTGDevice prescaler */
#define  RCC_GCFGR_OTGFSPS_0                        ((uint32_t)0x00400000)        /*!< Bit 0 */
#define  RCC_GCFGR_OTGFSPS_1                        ((uint32_t)0x00800000)        /*!< Bit 1 */

#define  RCC_GCFGR_OTGFSPS_Div1_5                   ((uint32_t)0x00000000)        /*!< PLL divided by 1.5 */
#define  RCC_GCFGR_OTGFSPS_Div1                     ((uint32_t)0x00400000)        /*!< PLL not divided */
#define  RCC_GCFGR_OTGFSPS_Div2_5                   ((uint32_t)0x00800000)        /*!< PLL divided by 2.5 */
#define  RCC_GCFGR_OTGFSPS_Div2                     ((uint32_t)0x00C00000)        /*!< PLL divided by 2 */

/*!< CKOUTSEL configuration */		         
#define  RCC_GCFGR_CKOUTSEL                         ((uint32_t)0x0F000000)        /*!< CKOUTSEL[3:0] bits (Microcontroller Clock Output) */
#define  RCC_GCFGR_CKOUTSEL_0                       ((uint32_t)0x01000000)        /*!< Bit 0 */
#define  RCC_GCFGR_CKOUTSEL_1                       ((uint32_t)0x02000000)        /*!< Bit 1 */
#define  RCC_GCFGR_CKOUTSEL_2                       ((uint32_t)0x04000000)        /*!< Bit 2 */
#define  RCC_GCFGR_CKOUTSEL_3                       ((uint32_t)0x08000000)        /*!< Bit 3 */

#define  RCC_GCFGR_CKOUTSEL_NOCLOCK                 ((uint32_t)0x00000000)        /*!< No clock */
#define  RCC_GCFGR_CKOUTSEL_SYSCLK                  ((uint32_t)0x04000000)        /*!< System clock selected as CKOUTSEL source */
#define  RCC_GCFGR_CKOUTSEL_HSI                     ((uint32_t)0x05000000)        /*!< HSI clock selected as CKOUTSEL source */
#define  RCC_GCFGR_CKOUTSEL_HSE                     ((uint32_t)0x06000000)        /*!< HSE clock selected as CKOUTSEL source  */
#define  RCC_GCFGR_CKOUTSEL_PLL_DIV2                ((uint32_t)0x07000000)        /*!< PLL clock divided by 2 selected as CKOUTSEL source */
#define  RCC_GCFGR_CKOUTSEL_PLL2                    ((uint32_t)0x08000000)        /*!< PLL2 clock selected as CKOUTSEL source */
#define  RCC_GCFGR_CKOUTSEL_PLL3_DIV2               ((uint32_t)0x09000000)        /*!< PLL3 clock divided by 2 selected as CKOUTSEL source */
#define  RCC_GCFGR_CKOUTSEL_EXT1                    ((uint32_t)0x0A000000)        /*!< EXT1 external 3-25 MHz oscillator clock selected as CKOUTSEL source */
#define  RCC_GCFGR_CKOUTSEL_PLL3                    ((uint32_t)0x0B000000)        /*!< PLL3 clock  selected as CKOUTSEL source */


/*!<******************  Bit definition for RCC_GCIR register  ********************/
#define  RCC_GCIR_LSISTBF                           ((uint32_t)0x00000001)        /*!< LSI Ready Interrupt flag */
#define  RCC_GCIR_LSESTBF                           ((uint32_t)0x00000002)        /*!< LSE Ready Interrupt flag */
#define  RCC_GCIR_HSISTBF                           ((uint32_t)0x00000004)        /*!< HSI Ready Interrupt flag */
#define  RCC_GCIR_HSESTBF                           ((uint32_t)0x00000008)        /*!< HSE Ready Interrupt flag */
#define  RCC_GCIR_PLLSTBF                           ((uint32_t)0x00000010)        /*!< PLL Ready Interrupt flag */
#define  RCC_GCIR_PLL2STBF                          ((uint32_t)0x00000020)        /*!< PLL2 Ready Interrupt flag */
#define  RCC_GCIR_PLL3STBF                          ((uint32_t)0x00000040)        /*!< PLL3 Ready Interrupt flag */
#define  RCC_GCIR_CKMF                              ((uint32_t)0x00000080)        /*!< Clock Security System Interrupt flag */
#define  RCC_GCIR_LSISTBIE                          ((uint32_t)0x00000100)        /*!< LSI Ready Interrupt Enable */
#define  RCC_GCIR_LSESTBIE                          ((uint32_t)0x00000200)        /*!< LSE Ready Interrupt Enable */
#define  RCC_GCIR_HSISTBIE                          ((uint32_t)0x00000400)        /*!< HSI Ready Interrupt Enable */
#define  RCC_GCIR_HSESTBIE                          ((uint32_t)0x00000800)        /*!< HSE Ready Interrupt Enable */
#define  RCC_GCIR_PLLSTBIE                          ((uint32_t)0x00001000)        /*!< PLL Ready Interrupt Enable */
#define  RCC_GCIR_PLL2STBIE                         ((uint32_t)0x00002000)        /*!< PLL2 Ready Interrupt Enable */
#define  RCC_GCIR_PLL3STBIE                         ((uint32_t)0x00004000)        /*!< PLL3 Ready Interrupt Enable */
#define  RCC_GCIR_LSISTBR                           ((uint32_t)0x00010000)        /*!< LSI Ready Interrupt Clear */
#define  RCC_GCIR_LSESTBR                           ((uint32_t)0x00020000)        /*!< LSE Ready Interrupt Clear */
#define  RCC_GCIR_HSISTBR                           ((uint32_t)0x00040000)        /*!< HSI Ready Interrupt Clear */
#define  RCC_GCIR_HSESTBR                           ((uint32_t)0x00080000)        /*!< HSE Ready Interrupt Clear */
#define  RCC_GCIR_PLLSTBR                           ((uint32_t)0x00100000)        /*!< PLL Ready Interrupt Clear */
#define  RCC_GCIR_PLL2STBR                          ((uint32_t)0x00200000)        /*!< PLL2 Ready Interrupt Clear */
#define  RCC_GCIR_PLL3STBR                          ((uint32_t)0x00400000)        /*!< PLL3 Ready Interrupt Clear */
#define  RCC_GCIR_CKMC                              ((uint32_t)0x00800000)        /*!< Clock Security System Interrupt Clear */

/*****************  Bit definition for RCC_APB2RCR register  *****************/

#define  RCC_APB2RCR_AFRST                          ((uint32_t)0x00000001)        /*!< Alternate Function I/O clock reset */
#define  RCC_APB2RCR_PARST                          ((uint32_t)0x00000004)        /*!< GPIOA clock reset */
#define  RCC_APB2RCR_PBRST                          ((uint32_t)0x00000008)        /*!< GPIOB clock reset */
#define  RCC_APB2RCR_PCRST                          ((uint32_t)0x00000010)        /*!< GPIOC clock reset */
#define  RCC_APB2RCR_PDRST                          ((uint32_t)0x00000020)        /*!< GPIOD clock reset */
#define  RCC_APB2RCR_PERST                          ((uint32_t)0x00000040)        /*!< GPIOE clock reset */
#define  RCC_APB2RCR_PFRST                          ((uint32_t)0x00000080)        /*!< GPIOF clock reset */
#define  RCC_APB2RCR_PGRST                          ((uint32_t)0x00000100)        /*!< GPIOF clock reset */
#define  RCC_APB2RCR_ADC1RST                        ((uint32_t)0x00000200)        /*!< ADC1 clock reset */
#define  RCC_APB2RCR_ADC2RST                        ((uint32_t)0x00000400)        /*!< ADC2 clock reset */
#define  RCC_APB2RCR_TIMER1RST                      ((uint32_t)0x00000800)        /*!< TIMER1 clock reset */
#define  RCC_APB2RCR_SPI1RST                        ((uint32_t)0x00001000)        /*!< SPI1 clock reset */
#define  RCC_APB2RCR_TIMER8RST                      ((uint32_t)0x00002000)        /*!< TIMER8 clock reset */
#define  RCC_APB2RCR_USART1RST                      ((uint32_t)0x00004000)        /*!< USART1 clock reset */
#define  RCC_APB2RCR_ADC3RST                        ((uint32_t)0x00008000)        /*!< ADC3 clock reset */
#define  RCC_APB2RCR_TIMER9RST                      ((uint32_t)0x00080000)        /*!< TIMER9 clock reset */
#define  RCC_APB2RCR_TIMER10RST                     ((uint32_t)0x00100000)        /*!< TIMER10 clock reset */
#define  RCC_APB2RCR_TIMER11RST                     ((uint32_t)0x00200000)        /*!< TIMER11 clock reset */

/*****************  Bit definition for RCC_APB1RCR register  *****************/

#define  RCC_APB1RCR_TIMER2RST                      ((uint32_t)0x00000001)        /*!< TIMER2 clock reset */
#define  RCC_APB1RCR_TIMER3RST                      ((uint32_t)0x00000002)        /*!< TIMER3 clock reset */
#define  RCC_APB1RCR_TIMER4RST                      ((uint32_t)0x00000004)        /*!< TIMER4 clock reset */
#define  RCC_APB1RCR_TIMER5RST                      ((uint32_t)0x00000008)        /*!< TIMER8 clock reset */
#define  RCC_APB1RCR_TIMER6RST                      ((uint32_t)0x00000010)        /*!< TIMER6 clock reset */
#define  RCC_APB1RCR_TIMER7RST                      ((uint32_t)0x00000020)        /*!< TIMER7 clock reset */
#define  RCC_APB1RCR_TIMER12RST                     ((uint32_t)0x00000040)        /*!< TIMER12 clock reset */
#define  RCC_APB1RCR_TIMER13RST                     ((uint32_t)0x00000080)        /*!< TIMER13 clock reset */
#define  RCC_APB1RCR_TIMER14RST                     ((uint32_t)0x00000100)        /*!< TIMER14 clock reset */
#define  RCC_APB1RCR_WWDGRST                        ((uint32_t)0x00000800)        /*!< Window Watchdog clock reset */
#define  RCC_APB1RCR_SPI2RST                        ((uint32_t)0x00004000)        /*!< SPI2 clock reset */
#define  RCC_APB1RCR_SPI3RST                        ((uint32_t)0x00008000)        /*!< SPI3 clock reset */
#define  RCC_APB1RCR_USART2RST                      ((uint32_t)0x00020000)        /*!< USART 2 clock reset */
#define  RCC_APB1RCR_USART3RST                      ((uint32_t)0x00040000)        /*!< USART 3 clock reset */
#define  RCC_APB1RCR_UART4RST                       ((uint32_t)0x00080000)        /*!< UART 4 clock reset */
#define  RCC_APB1RCR_UART5RST                       ((uint32_t)0x00100000)        /*!< UART 5 clock reset */
#define  RCC_APB1RCR_I2C1RST                        ((uint32_t)0x00200000)        /*!< I2C 1 clock reset */
#define  RCC_APB1RCR_I2C2RST                        ((uint32_t)0x00400000)        /*!< I2C 2 clock reset */
#define  RCC_APB1RCR_USBRST                         ((uint32_t)0x00800000)        /*!< USB Device reset */
#define  RCC_APB1RCR_CAN1RST                        ((uint32_t)0x02000000)        /*!< CAN1 Device reset */
#define  RCC_APB1RCR_CAN2RST                        ((uint32_t)0x04000000)        /*!< CAN2 Device reset */
#define  RCC_APB1RCR_BKPRST                         ((uint32_t)0x08000000)        /*!< BKP Device reset */
#define  RCC_APB1RCR_PWRRST                         ((uint32_t)0x10000000)        /*!< PWR clock reset */
#define  RCC_APB1RCR_DACRST                         ((uint32_t)0x20000000)        /*!< DAC clock reset */

/******************  Bit definition for RCC_AHBCCR register  ******************/
#define  RCC_AHBCCR_DMA1EN                          ((uint32_t)0x00000001)        /*!< DMA1 clock enable */
#define  RCC_AHBCCR_DMA2EN                          ((uint32_t)0x00000002)        /*!< DMA2 clock enable */
#define  RCC_AHBCCR_SRAMEN                          ((uint32_t)0x00000004)        /*!< SRAM interface clock enable */
#define  RCC_AHBCCR_FMCEN                           ((uint32_t)0x00000010)        /*!< FMC clock enable */
#define  RCC_AHBCCR_CRCEN                           ((uint32_t)0x00000040)        /*!< CRC clock enable */
#define  RCC_AHBCCR_EXMCEN                          ((uint32_t)0x00000100)        /*!< EXMC clock enable */
#define  RCC_AHBCCR_SDIOEN                          ((uint32_t)0x00000400)        /*!< SDIO clock enable */
#define  RCC_AHBCCR_OTGFSEN                         ((uint32_t)0x00001000)        /*!< OTGFS clock enable */
#define  RCC_AHBCCR_ETHMACEN                        ((uint32_t)0x00004000)        /*!< ETHMAC clock enable */
#define  RCC_AHBCCR_ETHMACRXEN                      ((uint32_t)0x00008000)        /*!< ETHMACRX clock enable */
#define  RCC_AHBCCR_ETHMACTXEN                      ((uint32_t)0x00010000)        /*!< ETHMACTX clock enable */

/*****************  Bit definition for RCC_APB2CCR register  ******************/
#define  RCC_APB2CCR_AFEN                           ((uint32_t)0x00000001)        /*!< Alternate Function I/O clock enable */
#define  RCC_APB2CCR_PAEN                           ((uint32_t)0x00000004)        /*!< GPIOA clock enable */
#define  RCC_APB2CCR_PBEN                           ((uint32_t)0x00000008)        /*!< GPIOB clock enable */
#define  RCC_APB2CCR_PCEN                           ((uint32_t)0x00000010)        /*!< GPIOC clock enable */
#define  RCC_APB2CCR_PDEN                           ((uint32_t)0x00000020)        /*!< GPIOD clock enable */
#define  RCC_APB2CCR_PEEN                           ((uint32_t)0x00000040)        /*!< GPIOE clock enable */
#define  RCC_APB2CCR_PFEN                           ((uint32_t)0x00000080)        /*!< GPIOF clock enable */
#define  RCC_APB2CCR_PGEN                           ((uint32_t)0x00000100)        /*!< GPIOF clock enable */
#define  RCC_APB2CCR_ADC1EN                         ((uint32_t)0x00000200)        /*!< ADC1 clock enable */
#define  RCC_APB2CCR_ADC2EN                         ((uint32_t)0x00000400)        /*!< ADC2 clock enable */
#define  RCC_APB2CCR_TIMER1EN                       ((uint32_t)0x00000800)        /*!< TIMER1 clock enable */
#define  RCC_APB2CCR_SPI1EN                         ((uint32_t)0x00001000)        /*!< SPI1 clock enable */
#define  RCC_APB2CCR_TIMER8EN                       ((uint32_t)0x00002000)        /*!< TIMER8 clock enable */
#define  RCC_APB2CCR_USART1EN                       ((uint32_t)0x00004000)        /*!< USART1 clock enable */
#define  RCC_APB2CCR_ADC3EN                         ((uint32_t)0x00008000)        /*!< ADC3 clock enable */
#define  RCC_APB2CCR_TIMER9EN                       ((uint32_t)0x00080000)        /*!< TIMER9 clock enable */
#define  RCC_APB2CCR_TIMER10EN                      ((uint32_t)0x00100000)        /*!< TIMER10 clock enable */
#define  RCC_APB2CCR_TIMER11EN                      ((uint32_t)0x00200000)        /*!< TIMER11 clock enable */

/*****************  Bit definition for RCC_APB1CCR register  ******************/

#define  RCC_APB1CCR_TIMER2EN                       ((uint32_t)0x00000001)        /*!< TIMER2 clock enable */
#define  RCC_APB1CCR_TIMER3EN                       ((uint32_t)0x00000002)        /*!< TIMER3 clock enable */
#define  RCC_APB1CCR_TIMER4EN                       ((uint32_t)0x00000004)        /*!< TIMER4 clock enable */
#define  RCC_APB1CCR_TIMER5EN                       ((uint32_t)0x00000008)        /*!< TIMER8 clock enable */
#define  RCC_APB1CCR_TIMER6EN                       ((uint32_t)0x00000010)        /*!< TIMER6 clock enable */
#define  RCC_APB1CCR_TIMER7EN                       ((uint32_t)0x00000020)        /*!< TIMER7 clock enable */
#define  RCC_APB1CCR_TIMER12EN                      ((uint32_t)0x00000040)        /*!< TIMER12 clock enable */
#define  RCC_APB1CCR_TIMER13EN                      ((uint32_t)0x00000080)        /*!< TIMER13 clock enable */
#define  RCC_APB1CCR_TIMER14EN                      ((uint32_t)0x00000100)        /*!< TIMER14 clock enable */
#define  RCC_APB1CCR_WWDGEN                         ((uint32_t)0x00000800)        /*!< Window Watchdog clock enable */
#define  RCC_APB1CCR_SPI2EN                         ((uint32_t)0x00004000)        /*!< SPI2 clock enable */
#define  RCC_APB1CCR_SPI3EN                         ((uint32_t)0x00008000)        /*!< SPI3 clock enable*/
#define  RCC_APB1CCR_USART2EN                       ((uint32_t)0x00020000)        /*!< USART 2 clock enable */
#define  RCC_APB1CCR_USART3EN                       ((uint32_t)0x00040000)        /*!< USART 3 clock enable */
#define  RCC_APB1CCR_UART4EN                        ((uint32_t)0x00080000)        /*!< UART 4 clock enable */
#define  RCC_APB1CCR_UART5EN                        ((uint32_t)0x00100000)        /*!< UART 5 clock enable */
#define  RCC_APB1CCR_I2C1EN                         ((uint32_t)0x00200000)        /*!< I2C 1 clock enable */
#define  RCC_APB1CCR_I2C2EN                         ((uint32_t)0x00400000)        /*!< I2C 2 clock enable */
#define  RCC_APB1CCR_USBEN                          ((uint32_t)0x00800000)        /*!< USB Device enable */
#define  RCC_APB1CCR_CAN1EN                         ((uint32_t)0x02000000)        /*!< CAN1 Device enable */
#define  RCC_APB1CCR_CAN2EN                         ((uint32_t)0x04000000)        /*!< CAN2 Device enable */
#define  RCC_APB1CCR_BKPEN                          ((uint32_t)0x08000000)        /*!< BKP Device enable */
#define  RCC_APB1CCR_PWREN                          ((uint32_t)0x10000000)        /*!< PWR clock enable */
#define  RCC_APB1CCR_DACEN                          ((uint32_t)0x20000000)        /*!< DAC clock enable */

/*******************  Bit definition for RCC_BDCR register  *******************/
#define  RCC_BDCR_LSEEN                             ((uint32_t)0x00000001)        /*!< External Low Speed oscillator enable */
#define  RCC_BDCR_LSESTB                            ((uint32_t)0x00000002)        /*!< External Low Speed oscillator Ready */
#define  RCC_BDCR_LSEBPS                            ((uint32_t)0x00000004)        /*!< External Low Speed oscillator Bypass */

#define  RCC_BDCR_LSEDRV                            ((uint32_t)0x00000018)        /*!< LSEDRV[1:0] bits (LSE oscillator drive capability) */
#define  RCC_BDCR_LSEDRV_0                          ((uint32_t)0x00000008)        /*!< Bit 0 */
#define  RCC_BDCR_LSEDRV_1                          ((uint32_t)0x00000010)        /*!< Bit 1 */
									       
#define  RCC_BDCR_RTCSEL                            ((uint32_t)0x00000300)        /*!< RTCSEL[1:0] bits (RTC clock source selection) */
#define  RCC_BDCR_RTCSEL_0                          ((uint32_t)0x00000100)        /*!< Bit 0 */
#define  RCC_BDCR_RTCSEL_1                          ((uint32_t)0x00000200)        /*!< Bit 1 */

/*!< RTC congiguration */
#define  RCC_BDCR_RTCSEL_NOCLOCK                    ((uint32_t)0x00000000)        /*!< No clock */
#define  RCC_BDCR_RTCSEL_LSE                        ((uint32_t)0x00000100)        /*!< LSE oscillator clock used as RTC clock */
#define  RCC_BDCR_RTCSEL_LSI                        ((uint32_t)0x00000200)        /*!< LSI oscillator clock used as RTC clock */
#define  RCC_BDCR_RTCSEL_HSE128                     ((uint32_t)0x00000300)        /*!< HSE oscillator clock divided by 128 used as RTC clock */

#define  RCC_BDCR_RTCEN                             ((uint32_t)0x00008000)        /*!< RTC clock enable */
#define  RCC_BDCR_BKPRST                            ((uint32_t)0x00010000)        /*!< Backup domain software reset  */

/*******************  Bit definition for RCC_GCSR register  ********************/  
#define  RCC_GCSR_LSIEN                             ((uint32_t)0x00000001)        /*!< Internal Low Speed oscillator enable */
#define  RCC_GCSR_LSISTB                            ((uint32_t)0x00000002)        /*!< Internal Low Speed oscillator Ready */
#define  RCC_GCSR_RSTFC                             ((uint32_t)0x01000000)        /*!< Remove reset flag */
#define  RCC_GCSR_EPRSTF                            ((uint32_t)0x04000000)        /*!< PIN reset flag */
#define  RCC_GCSR_POPDRSTF                          ((uint32_t)0x08000000)        /*!< POR/PDR reset flag */
#define  RCC_GCSR_SWRSTF                            ((uint32_t)0x10000000)        /*!< Software Reset flag */
#define  RCC_GCSR_IWDGRSTF                          ((uint32_t)0x20000000)        /*!< Independent Watchdog reset flag */
#define  RCC_GCSR_WWDGRSTF                          ((uint32_t)0x40000000)        /*!< Window watchdog reset flag */
#define  RCC_GCSR_LPRSTF                            ((uint32_t)0x80000000)        /*!< Low-Power reset flag */

/*****************  Bit definition for RCC_DEEPSLEEP_VC register  *****************/
#define  RCC_DEEPSLEEP_VC_CLEAR                     ((uint32_t)0x00000003)        /*!< RCC_DEEPSLEEP_VC[2:0] bits */
#define  RCC_DEEPSLEEP_VC_0                         ((uint32_t)0x00000000)        /*!< Bit 0 bits */
#define  RCC_DEEPSLEEP_VC_1                         ((uint32_t)0x00000002)        /*!< Bit 1 bits */
#define  RCC_DEEPSLEEP_VC_2                         ((uint32_t)0x00000004)        /*!< Bit 2 bits */
#define  RCC_DEEPSLEEP_VC1_2                        ((uint32_t)0x00000000)        /*!< The kernel voltage in Deep-sleep mode is 1.2V */
#define  RCC_DEEPSLEEP_VC1_1                        ((uint32_t)0x00000001)        /*!< The kernel voltage in Deep-sleep is 1.1V */
#define  RCC_DEEPSLEEP_VC1_0                        ((uint32_t)0x00000002)        /*!< The kernel voltage in Deep-sleep is 1.0V */
#define  RCC_DEEPSLEEP_VC0_9                        ((uint32_t)0x00000003)        /*!< The kernel voltage in Deep-sleep is 0.9V */

/*******************  Bit definition for RCC_AHBRCR register  ****************/
#define  RCC_AHBRCR_OTGFSRST                        ((uint32_t)0x00001000)        /*!< OTGFS clock reset */
#define  RCC_AHBRCR_ETHMACRST                       ((uint32_t)0x00004000)        /*!< ETHMAC clock reset */

/*******************  Bit definition for RCC_GCFGR2 register  ******************/
/*!< PREDIV1 configuration */
#define  RCC_GCFGR2_PREDV1                          ((uint32_t)0x0000000F)        /*!< PREDIV1[3:0] bits */
#define  RCC_GCFGR2_PREDV1_0                        ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  RCC_GCFGR2_PREDV1_1                        ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  RCC_GCFGR2_PREDV1_2                        ((uint32_t)0x00000004)        /*!< Bit 2 */
#define  RCC_GCFGR2_PREDV1_3                        ((uint32_t)0x00000008)        /*!< Bit 3 */

#define  RCC_GCFGR2_PREDV1_DIV1                     ((uint32_t)0x00000000)        /*!< PREDV1 input clock not divided */
#define  RCC_GCFGR2_PREDV1_DIV2                     ((uint32_t)0x00000001)        /*!< PREDV1 input clock divided by 2 */
#define  RCC_GCFGR2_PREDV1_DIV3                     ((uint32_t)0x00000002)        /*!< PREDV1 input clock divided by 3 */
#define  RCC_GCFGR2_PREDV1_DIV4                     ((uint32_t)0x00000003)        /*!< PREDV1 input clock divided by 4 */
#define  RCC_GCFGR2_PREDV1_DIV5                     ((uint32_t)0x00000004)        /*!< PREDV1 input clock divided by 5 */
#define  RCC_GCFGR2_PREDV1_DIV6                     ((uint32_t)0x00000005)        /*!< PREDV1 input clock divided by 6 */
#define  RCC_GCFGR2_PREDV1_DIV7                     ((uint32_t)0x00000006)        /*!< PREDV1 input clock divided by 7 */
#define  RCC_GCFGR2_PREDV1_DIV8                     ((uint32_t)0x00000007)        /*!< PREDV1 input clock divided by 8 */
#define  RCC_GCFGR2_PREDV1_DIV9                     ((uint32_t)0x00000008)        /*!< PREDV1 input clock divided by 9 */
#define  RCC_GCFGR2_PREDV1_DIV10                    ((uint32_t)0x00000009)        /*!< PREDV1 input clock divided by 10 */
#define  RCC_GCFGR2_PREDV1_DIV11                    ((uint32_t)0x0000000A)        /*!< PREDV1 input clock divided by 11 */
#define  RCC_GCFGR2_PREDV1_DIV12                    ((uint32_t)0x0000000B)        /*!< PREDV1 input clock divided by 12 */
#define  RCC_GCFGR2_PREDV1_DIV13                    ((uint32_t)0x0000000C)        /*!< PREDV1 input clock divided by 13 */
#define  RCC_GCFGR2_PREDV1_DIV14                    ((uint32_t)0x0000000D)        /*!< PREDV1 input clock divided by 14 */
#define  RCC_GCFGR2_PREDV1_DIV15                    ((uint32_t)0x0000000E)        /*!< PREDV1 input clock divided by 15 */
#define  RCC_GCFGR2_PREDV1_DIV16                    ((uint32_t)0x0000000F)        /*!< PREDV1 input clock divided by 16 */


/*!< PREDIV2 configuration */
#define  RCC_GCFGR2_PREDV2                          ((uint32_t)0x000000F0)        /*!< PREDIV2[3:0] bits */
#define  RCC_GCFGR2_PREDV2_0                        ((uint32_t)0x00000010)        /*!< Bit 0 */
#define  RCC_GCFGR2_PREDV2_1                        ((uint32_t)0x00000020)        /*!< Bit 1 */
#define  RCC_GCFGR2_PREDV2_2                        ((uint32_t)0x00000040)        /*!< Bit 2 */
#define  RCC_GCFGR2_PREDV2_3                        ((uint32_t)0x00000080)        /*!< Bit 3 */

#define  RCC_GCFGR2_PREDV2_DIV1                     ((uint32_t)0x00000000)        /*!< PREDV1 input clock not divided */
#define  RCC_GCFGR2_PREDV2_DIV2                     ((uint32_t)0x00000010)        /*!< PREDV1 input clock divided by 2 */
#define  RCC_GCFGR2_PREDV2_DIV3                     ((uint32_t)0x00000020)        /*!< PREDV1 input clock divided by 3 */
#define  RCC_GCFGR2_PREDV2_DIV4                     ((uint32_t)0x00000030)        /*!< PREDV1 input clock divided by 4 */
#define  RCC_GCFGR2_PREDV2_DIV5                     ((uint32_t)0x00000040)        /*!< PREDV1 input clock divided by 5 */
#define  RCC_GCFGR2_PREDV2_DIV6                     ((uint32_t)0x00000050)        /*!< PREDV1 input clock divided by 6 */
#define  RCC_GCFGR2_PREDV2_DIV7                     ((uint32_t)0x00000060)        /*!< PREDV1 input clock divided by 7 */
#define  RCC_GCFGR2_PREDV2_DIV8                     ((uint32_t)0x00000070)        /*!< PREDV1 input clock divided by 8 */
#define  RCC_GCFGR2_PREDV2_DIV9                     ((uint32_t)0x00000080)        /*!< PREDV1 input clock divided by 9 */
#define  RCC_GCFGR2_PREDV2_DIV10                    ((uint32_t)0x00000090)        /*!< PREDV1 input clock divided by 10 */
#define  RCC_GCFGR2_PREDV2_DIV11                    ((uint32_t)0x000000A0)        /*!< PREDV1 input clock divided by 11 */
#define  RCC_GCFGR2_PREDV2_DIV12                    ((uint32_t)0x000000B0)        /*!< PREDV1 input clock divided by 12 */
#define  RCC_GCFGR2_PREDV2_DIV13                    ((uint32_t)0x000000C0)        /*!< PREDV1 input clock divided by 13 */
#define  RCC_GCFGR2_PREDV2_DIV14                    ((uint32_t)0x000000D0)        /*!< PREDV1 input clock divided by 14 */
#define  RCC_GCFGR2_PREDV2_DIV15                    ((uint32_t)0x000000E0)        /*!< PREDV1 input clock divided by 15 */
#define  RCC_GCFGR2_PREDV2_DIV16                    ((uint32_t)0x000000F0)        /*!< PREDV1 input clock divided by 16 */

/*!< PLL2MF configuration */		        
#define  RCC_GCFGR2_PLL2MF                          ((uint32_t)0x00000F00)        /*!< PLL2MF[4:0] bits (PLL2 multiplication factor) */
#define  RCC_GCFGR2_PLL2MF_0                        ((uint32_t)0x00000100)        /*!< Bit 0 */
#define  RCC_GCFGR2_PLL2MF_1                        ((uint32_t)0x00000200)        /*!< Bit 1 */
#define  RCC_GCFGR2_PLL2MF_2                        ((uint32_t)0x00000400)        /*!< Bit 2 */
#define  RCC_GCFGR2_PLL2MF_3                        ((uint32_t)0x00000800)        /*!< Bit 3 */

#define  RCC_GCFGR2_PLL2MF8                         ((uint32_t)0x00000600)        /*!< PLL2 input clock*8 */
#define  RCC_GCFGR2_PLL2MF9                         ((uint32_t)0x00000700)        /*!< PLL2 input clock*9 */
#define  RCC_GCFGR2_PLL2MF10                        ((uint32_t)0x00000800)        /*!< PLL2 input clock10 */
#define  RCC_GCFGR2_PLL2MF11                        ((uint32_t)0x00000900)        /*!< PLL2 input clock*11 */
#define  RCC_GCFGR2_PLL2MF12                        ((uint32_t)0x00000A00)        /*!< PLL2 input clock*12 */
#define  RCC_GCFGR2_PLL2MF13                        ((uint32_t)0x00000B00)        /*!< PLL2 input clock*13 */
#define  RCC_GCFGR2_PLL2MF14                        ((uint32_t)0x00000C00)        /*!< PLL2 input clock*14 */
#define  RCC_GCFGR2_PLL2MF16                        ((uint32_t)0x00000E00)        /*!< PLL2 input clock*16 */
#define  RCC_GCFGR2_PLL2MF20                        ((uint32_t)0x00000F00)        /*!< PLL2 input clock*20 */

/*!< PLL3MF configuration */
#define  RCC_GCFGR2_PLL3MF                          ((uint32_t)0x0000F000)        /*!< PLL3MF[4:0] bits (PLL3 multiplication factor) */
#define  RCC_GCFGR2_PLL3MF_0                        ((uint32_t)0x00001000)        /*!< Bit 0 */
#define  RCC_GCFGR2_PLL3MF_1                        ((uint32_t)0x00002000)        /*!< Bit 1 */
#define  RCC_GCFGR2_PLL3MF_2                        ((uint32_t)0x00004000)        /*!< Bit 2 */
#define  RCC_GCFGR2_PLL3MF_3                        ((uint32_t)0x00008000)        /*!< Bit 3 */

#define  RCC_GCFGR2_PLL3MF8                         ((uint32_t)0x00006000)        /*!< PLL input clock*8 */
#define  RCC_GCFGR2_PLL3MF9                         ((uint32_t)0x00007000)        /*!< PLL input clock*9 */
#define  RCC_GCFGR2_PLL3MF10                        ((uint32_t)0x00008000)        /*!< PLL input clock10 */
#define  RCC_GCFGR2_PLL3MF11                        ((uint32_t)0x00009000)        /*!< PLL input clock*11 */
#define  RCC_GCFGR2_PLL3MF12                        ((uint32_t)0x0000A000)        /*!< PLL input clock*12 */
#define  RCC_GCFGR2_PLL3MF13                        ((uint32_t)0x0000B000)        /*!< PLL input clock*13 */
#define  RCC_GCFGR2_PLL3MF14                        ((uint32_t)0x0000C000)        /*!< PLL input clock*14 */
#define  RCC_GCFGR2_PLL3MF16                        ((uint32_t)0x0000E000)        /*!< PLL input clock*16 */
#define  RCC_GCFGR2_PLL3MF20                        ((uint32_t)0x0000F000)        /*!< PLL input clock*20 */

/*!< PREDV1SEL configuration */
#define  RCC_GCFGR2_PREDV1SEL                       ((uint32_t)0x00010000)        /*!< PREDV1SEL bit for entry clock source */
#define  RCC_GCFGR2_PREDV1SEL_HSE                   ((uint32_t)0x00000000)        /*!< HSE clock  selected as PREDV1 entry clock source */
#define  RCC_GCFGR2_PREDV1SEL_PLL2                  ((uint32_t)0x00010000)        /*!< PLL2 clock selected as PREDV1 entry clock source */

/*!< PREDV1SEL configuration */
#define  RCC_GCFGR2_I2S2SEL                         ((uint32_t)0x00020000)        /*!< I2S2SEL bit for entry clock source */
#define  RCC_GCFGR2_I2S2SEL_CK_SYS                  ((uint32_t)0x00000000)        /*!< CK_SYS clock  selected as I2S2 entry clock source */
#define  RCC_GCFGR2_I2S2SEL_PLL3                    ((uint32_t)0x00020000)        /*!< PLL3 clock selected as I2S2 entry clock source */


/*!< PREDV1SEL configuration */
#define  RCC_GCFGR2_I2S3SEL                         ((uint32_t)0x00040000)        /*!< I2S3SEL bit for entry clock source */
#define  RCC_GCFGR2_I2S3SEL_CK_SYS                  ((uint32_t)0x00000000)        /*!< CK_SYS clock  selected as I2S3 entry clock source */
#define  RCC_GCFGR2_I2S3SEL_PLL3                    ((uint32_t)0x00040000)        /*!< PLL3 clock selected as I2S3 entry clock source */

/******************  Bit definition for RCC_AHB2CCR register  ******************/
#define  RCC_AHB2CCR_DCIEN                          ((uint32_t)0x00000001)        /*!< DCI clock enable */
#define  RCC_AHB2CCR_CAUEN                          ((uint32_t)0x00000010)        /*!< CAU clock enable */
#define  RCC_AHB2CCR_HAUEN                          ((uint32_t)0x00000020)        /*!< HAU clock enable */
#define  RCC_AHB2CCR_RNGEN                          ((uint32_t)0x00000040)        /*!< RNG clock enable */

/******************  Bit definition for RCC_APB2CCR2 register  *****************/
#define  RCC_APB2CCR2_USART6EN                      ((uint32_t)0x01000000)        /*!< USART6 clock enable */
#define  RCC_APB2CCR2_TLDIEN                        ((uint32_t)0x04000000)        /*!< TLDI clock enable */
#define  RCC_APB2CCR2_PHEN                          ((uint32_t)0x40000000)        /*!< I/O port H clock enable */
#define  RCC_APB2CCR2_PIEN                          ((uint32_t)0x80000000)        /*!< I/O port I clock enable */

/******************  Bit definition for RCC_APB1CCR2 register  *****************/
#define  RCC_APB1CCR2_I2C3EN                        ((uint32_t)0x00800000)        /*!< I2C3 clock enable */
#define  RCC_APB1CCR2_UART7EN                       ((uint32_t)0x40000000)        /*!< UART7 clock enable */
#define  RCC_APB1CCR2_UART8EN                       ((uint32_t)0x80000000)        /*!< UART8 clock enable */

/*******************  Bit definition for RCC_AHB2RCR register  ****************/
#define  RCC_AHB2RCR_DCIRST                         ((uint32_t)0x00000001)        /*!< DCI reset */
#define  RCC_AHB2RCR_CAURST                         ((uint32_t)0x00000010)        /*!< CAU reset */
#define  RCC_AHB2RCR_HAURST                         ((uint32_t)0x00000020)        /*!< HAU reset */
#define  RCC_AHB2RCR_RNGRST                         ((uint32_t)0x00000040)        /*!< RNG reset */

/*****************  Bit definition for RCC_APB2RCR2 register  *****************/
#define  RCC_APB2RCR2_USART6RST                     ((uint32_t)0x01000000)        /*!< USART6 reset */
#define  RCC_APB2RCR2_TLDIRST                       ((uint32_t)0x04000000)        /*!< TLDI reset */
#define  RCC_APB2RCR2_PHRST                         ((uint32_t)0x40000000)        /*!< I/O port H reset */
#define  RCC_APB2RCR2_PIRST                         ((uint32_t)0x80000000)        /*!< I/O port I reset */

/*****************  Bit definition for RCC_APB1RCR2 register  *****************/
#define  RCC_APB1RCR2_I2C3RST                       ((uint32_t)0x00800000)        /*!< I2C3 reset */
#define  RCC_APB1RCR2_UART7RST                      ((uint32_t)0x40000000)        /*!< UART7 reset */
#define  RCC_APB1RCR2_UART8RST                      ((uint32_t)0x80000000)        /*!< UART8 reset */

/*****************  Bit definition for RCC_GCFGR3 register  ********************/
#define  RCC_GCFGR3_CKOUTDIV                        ((uint32_t)0x0000003F)        /*!< CKOUTDIV[5:0] bits (Microcontroller Clock Output) */
#define  RCC_GCFGR3_CKOUTDIV_0                      ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  RCC_GCFGR3_CKOUTDIV_1                      ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  RCC_GCFGR3_CKOUTDIV_2                      ((uint32_t)0x00000004)        /*!< Bit 2 */
#define  RCC_GCFGR3_CKOUTDIV_3                      ((uint32_t)0x00000008)        /*!< Bit 3 */
#define  RCC_GCFGR3_CKOUTDIV_4                      ((uint32_t)0x00000010)        /*!< Bit 4 */
#define  RCC_GCFGR3_CKOUTDIV_5                      ((uint32_t)0x00000020)        /*!< Bit 5 */

#define  RCC_GCFGR3_CKOUTDIV1                       ((uint32_t)0x00000000)        /*!< CKOUT clock divided by 1 */
#define  RCC_GCFGR3_CKOUTDIV2                       ((uint32_t)0x00000001)        /*!< CKOUT clock divided by 2 */
#define  RCC_GCFGR3_CKOUTDIV3                       ((uint32_t)0x00000002)        /*!< CKOUT clock divided by 3 */
#define  RCC_GCFGR3_CKOUTDIV4                       ((uint32_t)0x00000003)        /*!< CKOUT clock divided by 4 */
#define  RCC_GCFGR3_CKOUTDIV5                       ((uint32_t)0x00000004)        /*!< CKOUT clock divided by 5 */
#define  RCC_GCFGR3_CKOUTDIV6                       ((uint32_t)0x00000005)        /*!< CKOUT clock divided by 6 */
#define  RCC_GCFGR3_CKOUTDIV7                       ((uint32_t)0x00000006)        /*!< CKOUT clock divided by 7 */
#define  RCC_GCFGR3_CKOUTDIV8                       ((uint32_t)0x00000007)        /*!< CKOUT clock divided by 8 */
#define  RCC_GCFGR3_CKOUTDIV9                       ((uint32_t)0x00000008)        /*!< CKOUT clock divided by 9 */
#define  RCC_GCFGR3_CKOUTDIV10                      ((uint32_t)0x00000009)        /*!< CKOUT clock divided by 10 */
#define  RCC_GCFGR3_CKOUTDIV11                      ((uint32_t)0x0000000A)        /*!< CKOUT clock divided by 11 */
#define  RCC_GCFGR3_CKOUTDIV12                      ((uint32_t)0x0000000B)        /*!< CKOUT clock divided by 12 */
#define  RCC_GCFGR3_CKOUTDIV13                      ((uint32_t)0x0000000C)        /*!< CKOUT clock divided by 13 */
#define  RCC_GCFGR3_CKOUTDIV14                      ((uint32_t)0x0000000D)        /*!< CKOUT clock divided by 14 */
#define  RCC_GCFGR3_CKOUTDIV15                      ((uint32_t)0x0000000E)        /*!< CKOUT clock divided by 15 */
#define  RCC_GCFGR3_CKOUTDIV16                      ((uint32_t)0x0000000F)        /*!< CKOUT clock divided by 16 */
#define  RCC_GCFGR3_CKOUTDIV17                      ((uint32_t)0x00000010)        /*!< CKOUT clock divided by 17 */
#define  RCC_GCFGR3_CKOUTDIV18                      ((uint32_t)0x00000011)        /*!< CKOUT clock divided by 18 */
#define  RCC_GCFGR3_CKOUTDIV19                      ((uint32_t)0x00000012)        /*!< CKOUT clock divided by 19 */
#define  RCC_GCFGR3_CKOUTDIV20                      ((uint32_t)0x00000013)        /*!< CKOUT clock divided by 20 */
#define  RCC_GCFGR3_CKOUTDIV21                      ((uint32_t)0x00000014)        /*!< CKOUT clock divided by 21 */
#define  RCC_GCFGR3_CKOUTDIV22                      ((uint32_t)0x00000015)        /*!< CKOUT clock divided by 22 */
#define  RCC_GCFGR3_CKOUTDIV23                      ((uint32_t)0x00000016)        /*!< CKOUT clock divided by 23 */
#define  RCC_GCFGR3_CKOUTDIV24                      ((uint32_t)0x00000017)        /*!< CKOUT clock divided by 24 */
#define  RCC_GCFGR3_CKOUTDIV25                      ((uint32_t)0x00000018)        /*!< CKOUT clock divided by 25 */
#define  RCC_GCFGR3_CKOUTDIV26                      ((uint32_t)0x00000019)        /*!< CKOUT clock divided by 26 */
#define  RCC_GCFGR3_CKOUTDIV27                      ((uint32_t)0x0000001A)        /*!< CKOUT clock divided by 27 */
#define  RCC_GCFGR3_CKOUTDIV28                      ((uint32_t)0x0000001B)        /*!< CKOUT clock divided by 28 */
#define  RCC_GCFGR3_CKOUTDIV29                      ((uint32_t)0x0000001C)        /*!< CKOUT clock divided by 29 */
#define  RCC_GCFGR3_CKOUTDIV30                      ((uint32_t)0x0000001D)        /*!< CKOUT clock divided by 30 */
#define  RCC_GCFGR3_CKOUTDIV31                      ((uint32_t)0x0000001E)        /*!< CKOUT clock divided by 31 */
#define  RCC_GCFGR3_CKOUTDIV32                      ((uint32_t)0x0000001F)        /*!< CKOUT clock divided by 32 */
#define  RCC_GCFGR3_CKOUTDIV33                      ((uint32_t)0x00000020)        /*!< CKOUT clock divided by 33 */
#define  RCC_GCFGR3_CKOUTDIV34                      ((uint32_t)0x00000021)        /*!< CKOUT clock divided by 34 */
#define  RCC_GCFGR3_CKOUTDIV35                      ((uint32_t)0x00000022)        /*!< CKOUT clock divided by 35 */
#define  RCC_GCFGR3_CKOUTDIV36                      ((uint32_t)0x00000023)        /*!< CKOUT clock divided by 36 */
#define  RCC_GCFGR3_CKOUTDIV37                      ((uint32_t)0x00000024)        /*!< CKOUT clock divided by 37 */
#define  RCC_GCFGR3_CKOUTDIV38                      ((uint32_t)0x00000025)        /*!< CKOUT clock divided by 38 */
#define  RCC_GCFGR3_CKOUTDIV39                      ((uint32_t)0x00000026)        /*!< CKOUT clock divided by 39 */
#define  RCC_GCFGR3_CKOUTDIV40                      ((uint32_t)0x00000027)        /*!< CKOUT clock divided by 40 */
#define  RCC_GCFGR3_CKOUTDIV41                      ((uint32_t)0x00000028)        /*!< CKOUT clock divided by 41 */
#define  RCC_GCFGR3_CKOUTDIV42                      ((uint32_t)0x00000029)        /*!< CKOUT clock divided by 42 */
#define  RCC_GCFGR3_CKOUTDIV43                      ((uint32_t)0x0000002A)        /*!< CKOUT clock divided by 43 */
#define  RCC_GCFGR3_CKOUTDIV44                      ((uint32_t)0x0000002B)        /*!< CKOUT clock divided by 44 */
#define  RCC_GCFGR3_CKOUTDIV45                      ((uint32_t)0x0000002C)        /*!< CKOUT clock divided by 45 */
#define  RCC_GCFGR3_CKOUTDIV46                      ((uint32_t)0x0000002D)        /*!< CKOUT clock divided by 46 */
#define  RCC_GCFGR3_CKOUTDIV47                      ((uint32_t)0x0000002E)        /*!< CKOUT clock divided by 47 */
#define  RCC_GCFGR3_CKOUTDIV48                      ((uint32_t)0x0000002F)        /*!< CKOUT clock divided by 48 */
#define  RCC_GCFGR3_CKOUTDIV49                      ((uint32_t)0x00000030)        /*!< CKOUT clock divided by 49 */
#define  RCC_GCFGR3_CKOUTDIV50                      ((uint32_t)0x00000031)        /*!< CKOUT clock divided by 50 */
#define  RCC_GCFGR3_CKOUTDIV51                      ((uint32_t)0x00000032)        /*!< CKOUT clock divided by 51 */
#define  RCC_GCFGR3_CKOUTDIV52                      ((uint32_t)0x00000033)        /*!< CKOUT clock divided by 52 */
#define  RCC_GCFGR3_CKOUTDIV53                      ((uint32_t)0x00000034)        /*!< CKOUT clock divided by 53 */
#define  RCC_GCFGR3_CKOUTDIV54                      ((uint32_t)0x00000035)        /*!< CKOUT clock divided by 54 */
#define  RCC_GCFGR3_CKOUTDIV55                      ((uint32_t)0x00000036)        /*!< CKOUT clock divided by 55 */
#define  RCC_GCFGR3_CKOUTDIV56                      ((uint32_t)0x00000037)        /*!< CKOUT clock divided by 56 */
#define  RCC_GCFGR3_CKOUTDIV57                      ((uint32_t)0x00000038)        /*!< CKOUT clock divided by 57 */
#define  RCC_GCFGR3_CKOUTDIV58                      ((uint32_t)0x00000039)        /*!< CKOUT clock divided by 58 */
#define  RCC_GCFGR3_CKOUTDIV59                      ((uint32_t)0x0000003A)        /*!< CKOUT clock divided by 59 */
#define  RCC_GCFGR3_CKOUTDIV60                      ((uint32_t)0x0000003B)        /*!< CKOUT clock divided by 60 */
#define  RCC_GCFGR3_CKOUTDIV61                      ((uint32_t)0x0000003C)        /*!< CKOUT clock divided by 61 */
#define  RCC_GCFGR3_CKOUTDIV62                      ((uint32_t)0x0000003D)        /*!< CKOUT clock divided by 62 */
#define  RCC_GCFGR3_CKOUTDIV63                      ((uint32_t)0x0000003E)        /*!< CKOUT clock divided by 63 */
#define  RCC_GCFGR3_CKOUTDIV64                      ((uint32_t)0x0000003F)        /*!< CKOUT clock divided by 64 */

/*!< CKOUT2 configuration */
#define  RCC_GCFGR3_CKOUT2SEL                       ((uint32_t)0x000F0000)        /*!< CKOUT2SEL[3:0] bits (Microcontroller Clock Output 2) */
#define  RCC_GCFGR3_CKOUT2SEL_0                     ((uint32_t)0x00010000)        /*!< Bit 0 */
#define  RCC_GCFGR3_CKOUT2SEL_1                     ((uint32_t)0x00020000)        /*!< Bit 1 */
#define  RCC_GCFGR3_CKOUT2SEL_2                     ((uint32_t)0x00040000)        /*!< Bit 2 */
#define  RCC_GCFGR3_CKOUT2SEL_3                     ((uint32_t)0x00080000)        /*!< Bit 3 */

#define  RCC_GCFGR3_CKOUT2SEL_NOCLOCK               ((uint32_t)0x00000000)        /*!< No clock */
#define  RCC_GCFGR3_CKOUT2SEL_SYSCLK                ((uint32_t)0x00040000)        /*!< System clock selected as CKOUT2 source */
#define  RCC_GCFGR3_CKOUT2SEL_HSI                   ((uint32_t)0x00050000)        /*!< HSI clock selected as CKOUT2 source */
#define  RCC_GCFGR3_CKOUT2SEL_HSE                   ((uint32_t)0x00060000)        /*!< HSE clock selected as CKOUT2 source */
#define  RCC_GCFGR3_CKOUT2SEL_PLLCLK_Div2           ((uint32_t)0x00070000)        /*!< PLL clock divided by 2 selected as CKOUT2 source */
#define  RCC_GCFGR3_CKOUT2SEL_PLL2CLK               ((uint32_t)0x00080000)        /*!< PLL2 clock selected as CKOUT2 source*/
#define  RCC_GCFGR3_CKOUT2SEL_PLL3CLK_Div2          ((uint32_t)0x00090000)        /*!< PLL3 clock divided by 2 selected as CKOUT2 source*/
#define  RCC_GCFGR3_CKOUT2SEL_Ext_HSE               ((uint32_t)0x000A0000)        /*!< XT1 external 3-25 MHz oscillator clock selected as CKOUT2 source */
#define  RCC_GCFGR3_CKOUT2SEL_PLL3CLK               ((uint32_t)0x000B0000)        /*!< PLL3 clock selected as CKOUT2 source */

#define  RCC_GCFGR3_CKOUT2DIV                       ((uint32_t)0x00003F00)        /*!< CKOUT2DIV[5:0] bits (Microcontroller Clock Output 2) */
#define  RCC_GCFGR3_CKOUT2DIV_0                     ((uint32_t)0x00000100)        /*!< Bit 0 */
#define  RCC_GCFGR3_CKOUT2DIV_1                     ((uint32_t)0x00000200)        /*!< Bit 1 */
#define  RCC_GCFGR3_CKOUT2DIV_2                     ((uint32_t)0x00000400)        /*!< Bit 2 */
#define  RCC_GCFGR3_CKOUT2DIV_3                     ((uint32_t)0x00000800)        /*!< Bit 3 */
#define  RCC_GCFGR3_CKOUT2DIV_4                     ((uint32_t)0x00001000)        /*!< Bit 4 */
#define  RCC_GCFGR3_CKOUT2DIV_5                     ((uint32_t)0x00002000)        /*!< Bit 5 */

#define  RCC_GCFGR3_CKOUT2DIV1                      ((uint32_t)0x00000000)        /*!< CKOUT2 clock divided by 1 */
#define  RCC_GCFGR3_CKOUT2DIV2                      ((uint32_t)0x00000100)        /*!< CKOUT2 clock divided by 2 */
#define  RCC_GCFGR3_CKOUT2DIV3                      ((uint32_t)0x00000200)        /*!< CKOUT2 clock divided by 3 */
#define  RCC_GCFGR3_CKOUT2DIV4                      ((uint32_t)0x00000300)        /*!< CKOUT2 clock divided by 4 */
#define  RCC_GCFGR3_CKOUT2DIV5                      ((uint32_t)0x00000400)        /*!< CKOUT2 clock divided by 5 */
#define  RCC_GCFGR3_CKOUT2DIV6                      ((uint32_t)0x00000500)        /*!< CKOUT2 clock divided by 6 */
#define  RCC_GCFGR3_CKOUT2DIV7                      ((uint32_t)0x00000600)        /*!< CKOUT2 clock divided by 7 */
#define  RCC_GCFGR3_CKOUT2DIV8                      ((uint32_t)0x00000700)        /*!< CKOUT2 clock divided by 8 */
#define  RCC_GCFGR3_CKOUT2DIV9                      ((uint32_t)0x00000800)        /*!< CKOUT2 clock divided by 9 */
#define  RCC_GCFGR3_CKOUT2DIV10                     ((uint32_t)0x00000900)        /*!< CKOUT2 clock divided by 10 */
#define  RCC_GCFGR3_CKOUT2DIV11                     ((uint32_t)0x00000A00)        /*!< CKOUT2 clock divided by 11 */
#define  RCC_GCFGR3_CKOUT2DIV12                     ((uint32_t)0x00000B00)        /*!< CKOUT2 clock divided by 12 */
#define  RCC_GCFGR3_CKOUT2DIV13                     ((uint32_t)0x00000C00)        /*!< CKOUT2 clock divided by 13 */
#define  RCC_GCFGR3_CKOUT2DIV14                     ((uint32_t)0x00000D00)        /*!< CKOUT2 clock divided by 14 */
#define  RCC_GCFGR3_CKOUT2DIV15                     ((uint32_t)0x00000E00)        /*!< CKOUT2 clock divided by 15 */
#define  RCC_GCFGR3_CKOUT2DIV16                     ((uint32_t)0x00000F00)        /*!< CKOUT2 clock divided by 16 */
#define  RCC_GCFGR3_CKOUT2DIV17                     ((uint32_t)0x00001000)        /*!< CKOUT2 clock divided by 17 */
#define  RCC_GCFGR3_CKOUT2DIV18                     ((uint32_t)0x00001100)        /*!< CKOUT2 clock divided by 18 */
#define  RCC_GCFGR3_CKOUT2DIV19                     ((uint32_t)0x00001200)        /*!< CKOUT2 clock divided by 19 */
#define  RCC_GCFGR3_CKOUT2DIV20                     ((uint32_t)0x00001300)        /*!< CKOUT2 clock divided by 20 */
#define  RCC_GCFGR3_CKOUT2DIV21                     ((uint32_t)0x00001400)        /*!< CKOUT2 clock divided by 21 */
#define  RCC_GCFGR3_CKOUT2DIV22                     ((uint32_t)0x00001500)        /*!< CKOUT2 clock divided by 22 */
#define  RCC_GCFGR3_CKOUT2DIV23                     ((uint32_t)0x00001600)        /*!< CKOUT2 clock divided by 23 */
#define  RCC_GCFGR3_CKOUT2DIV24                     ((uint32_t)0x00001700)        /*!< CKOUT2 clock divided by 24 */
#define  RCC_GCFGR3_CKOUT2DIV25                     ((uint32_t)0x00001800)        /*!< CKOUT2 clock divided by 25 */
#define  RCC_GCFGR3_CKOUT2DIV26                     ((uint32_t)0x00001900)        /*!< CKOUT2 clock divided by 26 */
#define  RCC_GCFGR3_CKOUT2DIV27                     ((uint32_t)0x00001A00)        /*!< CKOUT2 clock divided by 27 */
#define  RCC_GCFGR3_CKOUT2DIV28                     ((uint32_t)0x00001B00)        /*!< CKOUT2 clock divided by 28 */
#define  RCC_GCFGR3_CKOUT2DIV29                     ((uint32_t)0x00001C00)        /*!< CKOUT2 clock divided by 29 */
#define  RCC_GCFGR3_CKOUT2DIV30                     ((uint32_t)0x00001D00)        /*!< CKOUT2 clock divided by 30 */
#define  RCC_GCFGR3_CKOUT2DIV31                     ((uint32_t)0x00001E00)        /*!< CKOUT2 clock divided by 31 */
#define  RCC_GCFGR3_CKOUT2DIV32                     ((uint32_t)0x00001F00)        /*!< CKOUT2 clock divided by 32 */
#define  RCC_GCFGR3_CKOUT2DIV33                     ((uint32_t)0x00002000)        /*!< CKOUT2 clock divided by 33 */
#define  RCC_GCFGR3_CKOUT2DIV34                     ((uint32_t)0x00002100)        /*!< CKOUT2 clock divided by 34 */
#define  RCC_GCFGR3_CKOUT2DIV35                     ((uint32_t)0x00002200)        /*!< CKOUT2 clock divided by 35 */
#define  RCC_GCFGR3_CKOUT2DIV36                     ((uint32_t)0x00002300)        /*!< CKOUT2 clock divided by 36 */
#define  RCC_GCFGR3_CKOUT2DIV37                     ((uint32_t)0x00002400)        /*!< CKOUT2 clock divided by 37 */
#define  RCC_GCFGR3_CKOUT2DIV38                     ((uint32_t)0x00002500)        /*!< CKOUT2 clock divided by 38 */
#define  RCC_GCFGR3_CKOUT2DIV39                     ((uint32_t)0x00002600)        /*!< CKOUT2 clock divided by 39 */
#define  RCC_GCFGR3_CKOUT2DIV40                     ((uint32_t)0x00002700)        /*!< CKOUT2 clock divided by 40 */
#define  RCC_GCFGR3_CKOUT2DIV41                     ((uint32_t)0x00002800)        /*!< CKOUT2 clock divided by 41 */
#define  RCC_GCFGR3_CKOUT2DIV42                     ((uint32_t)0x00002900)        /*!< CKOUT2 clock divided by 42 */
#define  RCC_GCFGR3_CKOUT2DIV43                     ((uint32_t)0x00002A00)        /*!< CKOUT2 clock divided by 43 */
#define  RCC_GCFGR3_CKOUT2DIV44                     ((uint32_t)0x00002B00)        /*!< CKOUT2 clock divided by 44 */
#define  RCC_GCFGR3_CKOUT2DIV45                     ((uint32_t)0x00002C00)        /*!< CKOUT2 clock divided by 45 */
#define  RCC_GCFGR3_CKOUT2DIV46                     ((uint32_t)0x00002D00)        /*!< CKOUT2 clock divided by 46 */
#define  RCC_GCFGR3_CKOUT2DIV47                     ((uint32_t)0x00002E00)        /*!< CKOUT2 clock divided by 47 */
#define  RCC_GCFGR3_CKOUT2DIV48                     ((uint32_t)0x00002F00)        /*!< CKOUT2 clock divided by 48 */
#define  RCC_GCFGR3_CKOUT2DIV49                     ((uint32_t)0x00003000)        /*!< CKOUT2 clock divided by 49 */
#define  RCC_GCFGR3_CKOUT2DIV50                     ((uint32_t)0x00003100)        /*!< CKOUT2 clock divided by 50 */
#define  RCC_GCFGR3_CKOUT2DIV51                     ((uint32_t)0x00003200)        /*!< CKOUT2 clock divided by 51 */
#define  RCC_GCFGR3_CKOUT2DIV52                     ((uint32_t)0x00003300)        /*!< CKOUT2 clock divided by 52 */
#define  RCC_GCFGR3_CKOUT2DIV53                     ((uint32_t)0x00003400)        /*!< CKOUT2 clock divided by 53 */
#define  RCC_GCFGR3_CKOUT2DIV54                     ((uint32_t)0x00003500)        /*!< CKOUT2 clock divided by 54 */
#define  RCC_GCFGR3_CKOUT2DIV55                     ((uint32_t)0x00003600)        /*!< CKOUT2 clock divided by 55 */
#define  RCC_GCFGR3_CKOUT2DIV56                     ((uint32_t)0x00003700)        /*!< CKOUT2 clock divided by 56 */
#define  RCC_GCFGR3_CKOUT2DIV57                     ((uint32_t)0x00003800)        /*!< CKOUT2 clock divided by 57 */
#define  RCC_GCFGR3_CKOUT2DIV58                     ((uint32_t)0x00003900)        /*!< CKOUT2 clock divided by 58 */
#define  RCC_GCFGR3_CKOUT2DIV59                     ((uint32_t)0x00003A00)        /*!< CKOUT2 clock divided by 59 */
#define  RCC_GCFGR3_CKOUT2DIV60                     ((uint32_t)0x00003B00)        /*!< CKOUT2 clock divided by 60 */
#define  RCC_GCFGR3_CKOUT2DIV61                     ((uint32_t)0x00003C00)        /*!< CKOUT2 clock divided by 61 */
#define  RCC_GCFGR3_CKOUT2DIV62                     ((uint32_t)0x00003D00)        /*!< CKOUT2 clock divided by 62 */
#define  RCC_GCFGR3_CKOUT2DIV63                     ((uint32_t)0x00003E00)        /*!< CKOUT2 clock divided by 63 */
#define  RCC_GCFGR3_CKOUT2DIV64                     ((uint32_t)0x00003F00)        /*!< CKOUT2 clock divided by 64 */

/*****************  Bit definition for RCC_GPLLTCR register  ********************/
#define  RCC_GPLLTCR_PLLTEN                        ((uint32_t)0x10000000)         /*!< PLLT clock enable */
#define  RCC_GPLLTCR_PLLTSTB                       ((uint32_t)0x20000000)         /*!< PLLT Clock Stabilization Flag */

/*****************  Bit definition for RCC_GPLLTIR register  ********************/
#define  RCC_GPLLTIR_PLLTSTBF                      ((uint32_t)0x00000040)         /*!< PLLT stabilization Interrupt flag */
#define  RCC_GPLLTIR_PLLTSTBIE                     ((uint32_t)0x00004000)         /*!< PLLT stabilization Interrupt Enable */
#define  RCC_GPLLTIR_PLLTSTBR                      ((uint32_t)0x00400000)         /*!< PLLT stabilization Interrupt Reset */

/*****************  Bit definition for RCC_PLLTCFGR register  ********************/
/*!< PLLTPS configuration */
#define  RCC_PLLTCFGR_PLLTPS                        ((uint32_t)0x0000003F)        /*!< PLLTPS[5:0] bits Division factor for the main PLL input clock */
#define  RCC_PLLTCFGR_PLLTPS_0                      ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  RCC_PLLTCFGR_PLLTPS_1                      ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  RCC_PLLTCFGR_PLLTPS_2                      ((uint32_t)0x00000004)        /*!< Bit 2 */
#define  RCC_PLLTCFGR_PLLTPS_3                      ((uint32_t)0x00000008)        /*!< Bit 3 */
#define  RCC_PLLTCFGR_PLLTPS_4                      ((uint32_t)0x00000010)        /*!< Bit 4 */
#define  RCC_PLLTCFGR_PLLTPS_5                      ((uint32_t)0x00000020)        /*!< Bit 5 */

/*!< PLLTMF(PLLTMF) configuration */
#define  RCC_PLLTCFGR_PLLTMF                        ((uint32_t)0x00007FC0)        /*!< PLLTMF[8:0] bits PLLT division factor for VCO */
#define  RCC_PLLTCFGR_PLLTMF_0                      ((uint32_t)0x00000040)        /*!< Bit 0 */
#define  RCC_PLLTCFGR_PLLTMF_1                      ((uint32_t)0x00000080)        /*!< Bit 1 */
#define  RCC_PLLTCFGR_PLLTMF_2                      ((uint32_t)0x00000100)        /*!< Bit 2 */
#define  RCC_PLLTCFGR_PLLTMF_3                      ((uint32_t)0x00000200)        /*!< Bit 3 */
#define  RCC_PLLTCFGR_PLLTMF_4                      ((uint32_t)0x00000400)        /*!< Bit 4 */
#define  RCC_PLLTCFGR_PLLTMF_5                      ((uint32_t)0x00000800)        /*!< Bit 5 */
#define  RCC_PLLTCFGR_PLLTMF_6                      ((uint32_t)0x00001000)        /*!< Bit 6 */
#define  RCC_PLLTCFGR_PLLTMF_7                      ((uint32_t)0x00002000)        /*!< Bit 7 */
#define  RCC_PLLTCFGR_PLLTMF_8                      ((uint32_t)0x00004000)        /*!< Bit 8 */

/*!< TCDIPS configuration */
#define  RCC_PLLTCFGR_TLDIPS                        ((uint32_t)0x00030000)        /*!< TLDIPS[1:0] bits division factor for LCD_CLK */
#define  RCC_PLLTCFGR_TLDIPS_0                      ((uint32_t)0x00010000)        /*!< Bit 0 */
#define  RCC_PLLTCFGR_TLDIPS_1                      ((uint32_t)0x00020000)        /*!< Bit 1 */

/*!< PLLTRPS configuration */
#define  RCC_PLLTCFGR_PLLTRPS                       ((uint32_t)0x07000000)        /*!< PLLTRPS[1:0] bits PLLT division factor for LCD clock */
#define  RCC_PLLTCFGR_PLLTRPS_0                     ((uint32_t)0x01000000)        /*!< Bit 0 */
#define  RCC_PLLTCFGR_PLLTRPS_1                     ((uint32_t)0x02000000)        /*!< Bit 1 */
#define  RCC_PLLTCFGR_PLLTRPS_2                     ((uint32_t)0x04000000)        /*!< Bit 2 */

/*!< PLLTSEL configuration */
#define  RCC_PLLTCFGR_PLLTSEL                       ((uint32_t)0x80000000)        /*!< PLLTSEL bit, PLLT entry clock source select*/
#define  RCC_PLLTCFGR_PLLTSEL_HSI                   ((uint32_t)0x00000000)        /*!< HSI clock selected as PLLT clock entry*/
#define  RCC_PLLTCFGR_PLLTSEL_HSE                   ((uint32_t)0x80000000)        /*!< HSE clock selected as PLLT clock entry*/


/******************************************************************************/
/*                                                                            */
/*                           Real-Time Clock (RTC)                            */
/*                                                                            */
/******************************************************************************/

/********************  Bits definition for RTC_TR register  *******************/

/********************  Bits definition for RTC_CTLR1 register  ****************/
#define RTC_CTLR1_SIE                               ((uint8_t)0x01)               /*!< RTC Second Interrupt Enable */
#define RTC_CTLR1_AIE                               ((uint8_t)0x02)               /*!< RTC Alarm Interrupt Enable */
#define RTC_CTLR1_OVIE                              ((uint8_t)0x04)               /*!< RTC OverfloW Interrupt Enable */

/********************  Bits definition for RTC_CTLR2 register  ****************/
#define RTC_CTLR2_SF                                ((uint8_t)0x01)               /*!< RTC Second Flag */
#define RTC_CTLR2_AF                                ((uint8_t)0x02)               /*!< RTC Alarm Flag */
#define RTC_CTLR2_OVF                               ((uint8_t)0x04)               /*!< RTC OverfloW Flag */
#define RTC_CTLR2_RSF                               ((uint8_t)0x08)               /*!< RTC Registers Synchronized Flag */
#define RTC_CTLR2_CMF                               ((uint8_t)0x10)               /*!< RTC Configuration Mode Flag */
#define RTC_CTLR2_LWOFF                             ((uint8_t)0x20)               /*!< RTC Last Write Operation OFF */

/********************  Bits definition for RTC_PLR1 register  *****************/
#define  RTC_PLR1_PLR                               ((uint16_t)0x000F)            /*!< RTC Prescaler Value High */

/********************  Bits definition for RTC_PLR2 register  *****************/
#define  RTC_PLR2_PLR                               ((uint16_t)0xFFFF)            /*!< RTC Prescaler Value Low*/

/********************  Bits definition for RTC_PREDIV1 register  **************/
#define  RTC_PREDIV1_RTC_PREDIV                     ((uint16_t)0x000F)            /*!< RTC  Divider Value High */
									         
/********************  Bits definition for RTC_PREDIV2 register  **************/
#define  RTC_PREDIV2_RTC_PREDIV                     ((uint16_t)0xFFFF)            /*!< RTC  Divider Value Low */

/********************  Bits definition for RTC_CNT1 register  *****************/
#define  RTC_CNT1_RTC_CNT                           ((uint16_t)0xFFFF)            /*!< RTC Counter Value High */

/********************  Bits definition for RTC_CNT2 register  *****************/
#define  RTC_CNT2_RTC_CNT                           ((uint16_t)0xFFFF)            /*!< RTC Counter Value Low */          

/********************  Bits definition for RTC_ALRMR1 register  ***************/
#define  RTC_ALRMR1_RTC_ALRMR                       ((uint16_t)0xFFFF)            /*!< RTC Alarm Value High */

/********************  Bits definition for RTC_ALRMR2 register  ***************/
#define  RTC_ALRMR2_RTC_ALRMR                       ((uint16_t)0xFFFF)            /*!< RTC Alarm Value Low */

/******************************************************************************/
/*                                                                            */
/*                          SD host Interface                                 */
/*                                                                            */
/******************************************************************************/

/******************  Bit definition for SDIO_POWER register  ******************/
#define  SDIO_POWER_PWRSTATE                        ((uint8_t)0x03)               /*!< PWRSTATE[1:0] bits (Power supply control bits) */
#define  SDIO_POWER_PWRSTATE_0                      ((uint8_t)0x01)               /*!< Bit 0 */
#define  SDIO_POWER_PWRSTATE_1                      ((uint8_t)0x02)               /*!< Bit 1 */

/******************  Bit definition for SDIO_CLKCTLR register  *****************/
#define  SDIO_CLKCTLR_DIV                           ((uint16_t)0x00FF)            /*!< Clock divide factor */
#define  SDIO_CLKCTLR_CKEN                          ((uint16_t)0x0100)            /*!< Clock enable bit */
#define  SDIO_CLKCTLR_PWRSAV                        ((uint16_t)0x0200)            /*!< Power saving configuration bit */
#define  SDIO_CLKCTLR_BYPASS                        ((uint16_t)0x0400)            /*!< Clock divider bypass enable bit */

#define  SDIO_CLKCTLR_BUSMODE                       ((uint16_t)0x1800)            /*!< BUSMODE[1:0] bits (Wide bus mode enable bit) */
#define  SDIO_CLKCTLR_BUSMODE_0                     ((uint16_t)0x0800)            /*!< Bit 0 */
#define  SDIO_CLKCTLR_BUSMODE_1                     ((uint16_t)0x1000)            /*!< Bit 1 */

#define  SDIO_CLKCTLR_CKPH                          ((uint16_t)0x2000)            /*!< SDIO_CLK dephasing selection bit */
#define  SDIO_CLKCTLR_HWFL_EN                       ((uint16_t)0x4000)            /*!< HW Flow Control enable */

#define  SDIO_CLKCTLR_DIVH                          ((uint16_t)0x80000000)        /*!< Clock divide factor MSB */

/*******************  Bit definition for SDIO_PARA register  ******************/
#define  SDIO_PARA_CMDARG                           ((uint32_t)0xFFFFFFFF)        /*!< Command argument */
									       
/*******************  Bit definition for SDIO_CMD register  *******************/
#define  SDIO_CMD_CMDINDEX                          ((uint16_t)0x003F)            /*!< Command Index */

#define  SDIO_CMD_WAITRESP                          ((uint16_t)0x00C0)            /*!< WAITRESP[1:0] bits (Wait for response bits) */
#define  SDIO_CMD_WAITRESP_0                        ((uint16_t)0x0040)            /*!<  Bit 0 */
#define  SDIO_CMD_WAITRESP_1                        ((uint16_t)0x0080)            /*!<  Bit 1 */

#define  SDIO_CMD_WAITINT                           ((uint16_t)0x0100)            /*!< CPSM Waits for Interrupt Request */
#define  SDIO_CMD_WAITPD                            ((uint16_t)0x0200)            /*!< CPSM Waits for ends of data transfer (CmdPend internal signal) */
#define  SDIO_CMD_CSMEN                             ((uint16_t)0x0400)            /*!< Command state machine (CSM) Enable bit */
#define  SDIO_CMD_SDIOSUSPEND                       ((uint16_t)0x0800)            /*!< SD I/O suspend command */
#define  SDIO_CMD_ENCMDC                            ((uint16_t)0x1000)            /*!< Enable CMD completion */
#define  SDIO_CMD_NIEN                              ((uint16_t)0x2000)            /*!< Not Interrupt Enable */
#define  SDIO_CMD_ATACMD                            ((uint16_t)0x4000)            /*!< CE-ATA command */

/*****************  Bit definition for SDIO_RESPCMD register  *****************/
#define  SDIO_RESPCMD_RESPCMD                       ((uint8_t)0x3F)               /*!< Response command index */

/******************  Bit definition for SDIO_RESP1 register  ******************/
#define  SDIO_RESP1_CARDSTATE1                      ((uint32_t)0xFFFFFFFF)        /*!< Card State */

/******************  Bit definition for SDIO_RESP2 register  ******************/
#define  SDIO_RESP2_CARDSTATE2                      ((uint32_t)0xFFFFFFFF)        /*!< Card State */

/******************  Bit definition for SDIO_RESP3 register  ******************/
#define  SDIO_RESP3_CARDSTATE3                      ((uint32_t)0xFFFFFFFF)        /*!< Card State */

/******************  Bit definition for SDIO_RESP4 register  ******************/
#define  SDIO_RESP4_CARDSTATE4                      ((uint32_t)0xFFFFFFFF)        /*!< Card State */

/******************  Bit definition for SDIO_DTTR register  *******************/
#define  SDIO_DTTR_DTTIME                           ((uint32_t)0xFFFFFFFF)        /*!< Data timeout period. */

/******************  Bit definition for SDIO_DTLEN register  ******************/
#define  SDIO_DTLEN_DTLEN                           ((uint32_t)0x01FFFFFF)        /*!< Data length value */

/******************  Bit definition for SDIO_DTCTLR register  *****************/
#define  SDIO_DTCTLR_DTTEN                          ((uint16_t)0x0001)            /*!< Data transfer enabled bit */
#define  SDIO_DTCTLR_DTTDIR                         ((uint16_t)0x0002)            /*!< Data transfer direction selection */
#define  SDIO_DTCTLR_DTTMODE                        ((uint16_t)0x0004)            /*!< Data transfer mode selection */
#define  SDIO_DTCTLR_DMAEN                          ((uint16_t)0x0008)            /*!< DMA enabled bit */
									        
#define  SDIO_DTCTLR_DTBLKSIZE                      ((uint16_t)0x00F0)            /*!< DTBLKSIZE[3:0] bits (Data block size) */
#define  SDIO_DTCTLR_DTBLKSIZE_0                    ((uint16_t)0x0010)            /*!< Bit 0 */
#define  SDIO_DTCTLR_DTBLKSIZE_1                    ((uint16_t)0x0020)            /*!< Bit 1 */
#define  SDIO_DTCTLR_DTBLKSIZE_2                    ((uint16_t)0x0040)            /*!< Bit 2 */
#define  SDIO_DTCTLR_DTBLKSIZE_3                    ((uint16_t)0x0080)            /*!< Bit 3 */

#define  SDIO_DTCTLR_RWSTART                        ((uint16_t)0x0100)            /*!< Read wait start */
#define  SDIO_DTCTLR_RWSTOP                         ((uint16_t)0x0200)            /*!< Read wait stop */
#define  SDIO_DTCTLR_RWMODE                         ((uint16_t)0x0400)            /*!< Read wait mode */
#define  SDIO_DTCTLR_SDIOEN                         ((uint16_t)0x0800)            /*!< SD I/O enable functions */

/******************  Bit definition for SDIO_DTCNT register  ******************/
#define  SDIO_DTCNT_DTCNT                           ((uint32_t)0x01FFFFFF)        /*!< Data count value */

/******************  Bit definition for SDIO_STR register  ********************/
#define  SDIO_STR_CCRCFAIL                          ((uint32_t)0x00000001)        /*!< Command response received (CRC check failed) */
#define  SDIO_STR_DTCRCFAIL                         ((uint32_t)0x00000002)        /*!< Data block sent/received (CRC check failed) */
#define  SDIO_STR_CMDTMOUT                          ((uint32_t)0x00000004)        /*!< Command response timeout */
#define  SDIO_STR_DTTMOUT                           ((uint32_t)0x00000008)        /*!< Data timeout */
#define  SDIO_STR_TXURE                             ((uint32_t)0x00000010)        /*!< Transmit FIFO underrun error */
#define  SDIO_STR_RXORE                             ((uint32_t)0x00000020)        /*!< Received FIFO overrun error */
#define  SDIO_STR_CMDREND                           ((uint32_t)0x00000040)        /*!< Command response received (CRC check passed) */
#define  SDIO_STR_CMDSENT                           ((uint32_t)0x00000080)        /*!< Command sent (no response required) */
#define  SDIO_STR_DTEND                             ((uint32_t)0x00000100)        /*!< Data end (data counter, SDIDCOUNT, is zero) */
#define  SDIO_STR_STBITE                            ((uint32_t)0x00000200)        /*!< Start bit not detected on all data signals in wide bus mode */
#define  SDIO_STR_DTBLKEND                          ((uint32_t)0x00000400)        /*!< Data block sent/received (CRC check passed) */
#define  SDIO_STR_CMDRUN                            ((uint32_t)0x00000800)        /*!< Command transfer in progress */
#define  SDIO_STR_TXRUN                             ((uint32_t)0x00001000)        /*!< Data transmit in progress */
#define  SDIO_STR_RXRUN                             ((uint32_t)0x00002000)        /*!< Data receive in progress */
#define  SDIO_STR_TXFIFOHE                          ((uint32_t)0x00004000)        /*!< Transmit FIFO Half Empty: at least 8 words can be written into the FIFO */
#define  SDIO_STR_RXFIFOHF                          ((uint32_t)0x00008000)        /*!< Receive FIFO Half Full: there are at least 8 words in the FIFO */
#define  SDIO_STR_TXFIFOF                           ((uint32_t)0x00010000)        /*!< Transmit FIFO full */
#define  SDIO_STR_RXFIFOF                           ((uint32_t)0x00020000)        /*!< Receive FIFO full */
#define  SDIO_STR_TXFIFOE                           ((uint32_t)0x00040000)        /*!< Transmit FIFO empty */
#define  SDIO_STR_RXFIFOE                           ((uint32_t)0x00080000)        /*!< Receive FIFO empty */
#define  SDIO_STR_TXDTVAL                           ((uint32_t)0x00100000)        /*!< Data available in transmit FIFO */
#define  SDIO_STR_RXDTVAL                           ((uint32_t)0x00200000)        /*!< Data available in receive FIFO */
#define  SDIO_STR_SDIOINT                           ((uint32_t)0x00400000)        /*!< SDIO interrupt received */
#define  SDIO_STR_ATAEND                            ((uint32_t)0x00800000)        /*!< CE-ATA command completion signal received for CMD61 */

/*******************  Bit definition for SDIO_INTCR register  *****************/
#define  SDIO_INTCR_CCRCFAILC                       ((uint32_t)0x00000001)        /*!< CCRCFAIL flag clear bit */
#define  SDIO_INTCR_DTCRCFAILC                      ((uint32_t)0x00000002)        /*!< DTCRCFAIL flag clear bit */
#define  SDIO_INTCR_CMDTMOUTC                       ((uint32_t)0x00000004)        /*!< CMDTMOUT flag clear bit */
#define  SDIO_INTCR_DTTMOUTC                        ((uint32_t)0x00000008)        /*!< DTTMOUT flag clear bit */
#define  SDIO_INTCR_TXUREC                          ((uint32_t)0x00000010)        /*!< TXURE flag clear bit */
#define  SDIO_INTCR_RXOREC                          ((uint32_t)0x00000020)        /*!< RXORE flag clear bit */
#define  SDIO_INTCR_CMDRENDC                        ((uint32_t)0x00000040)        /*!< CMDREND flag clear bit */
#define  SDIO_INTCR_CMDSENTC                        ((uint32_t)0x00000080)        /*!< CMDSENT flag clear bit */
#define  SDIO_INTCR_DTENDC                          ((uint32_t)0x00000100)        /*!< DTEND flag clear bit */
#define  SDIO_INTCR_STBITEC                         ((uint32_t)0x00000200)        /*!< STBITE flag clear bit */
#define  SDIO_INTCR_DTBLKENDC                       ((uint32_t)0x00000400)        /*!< DTBLKEND flag clear bit */
#define  SDIO_INTCR_SDIOINTC                        ((uint32_t)0x00400000)        /*!< SDIOINT flag clear bit */
#define  SDIO_INTCR_ATAENDC                         ((uint32_t)0x00800000)        /*!< ATAEND flag clear bit */

/******************  Bit definition for SDIO_IER register  ********************/
#define  SDIO_IER_CCRCFAILIE                        ((uint32_t)0x00000001)        /*!< Command CRC Fail Interrupt Enable */
#define  SDIO_IER_DTCRCFAILIE                       ((uint32_t)0x00000002)        /*!< Data CRC Fail Interrupt Enable */
#define  SDIO_IER_CMDTMOUTIE                        ((uint32_t)0x00000004)        /*!< Command TimeOut Interrupt Enable */
#define  SDIO_IER_DTTMOUTIE                         ((uint32_t)0x00000008)        /*!< Data TimeOut Interrupt Enable */
#define  SDIO_IER_TXUREIE                           ((uint32_t)0x00000010)        /*!< Tx FIFO UnderRun Error Interrupt Enable */
#define  SDIO_IER_RXOREIE                           ((uint32_t)0x00000020)        /*!< Rx FIFO OverRun Error Interrupt Enable */
#define  SDIO_IER_CMDRENDIE                         ((uint32_t)0x00000040)        /*!< Command Response Received Interrupt Enable */
#define  SDIO_IER_CMDSENTIE                         ((uint32_t)0x00000080)        /*!< Command Sent Interrupt Enable */
#define  SDIO_IER_DTENDIE                           ((uint32_t)0x00000100)        /*!< Data End Interrupt Enable */
#define  SDIO_IER_STBITEIE                          ((uint32_t)0x00000200)        /*!< Start Bit Error Interrupt Enable */
#define  SDIO_IER_DTBLKENDIE                        ((uint32_t)0x00000400)        /*!< Data Block End Interrupt Enable */
#define  SDIO_IER_CMDRUNIE                          ((uint32_t)0x00000800)        /*!< Command Acting Interrupt Enable */
#define  SDIO_IER_TXRUNIE                           ((uint32_t)0x00001000)        /*!< Data Transmit Acting Interrupt Enable */
#define  SDIO_IER_RXRUNIE                           ((uint32_t)0x00002000)        /*!< Data receive acting interrupt enabled */
#define  SDIO_IER_TXFIFOHEIE                        ((uint32_t)0x00004000)        /*!< Tx FIFO Half Empty interrupt Enable */
#define  SDIO_IER_RXFIFOHFIE                        ((uint32_t)0x00008000)        /*!< Rx FIFO Half Full interrupt Enable */
#define  SDIO_IER_TXFIFOFIE                         ((uint32_t)0x00010000)        /*!< Tx FIFO Full interrupt Enable */
#define  SDIO_IER_RXFIFOFIE                         ((uint32_t)0x00020000)        /*!< Rx FIFO Full interrupt Enable */
#define  SDIO_IER_TXFIFOEIE                         ((uint32_t)0x00040000)        /*!< Tx FIFO Empty interrupt Enable */
#define  SDIO_IER_RXFIFOEIE                         ((uint32_t)0x00080000)        /*!< Rx FIFO Empty interrupt Enable */
#define  SDIO_IER_TXDTVALIE                         ((uint32_t)0x00100000)        /*!< Data available in Tx FIFO interrupt Enable */
#define  SDIO_IER_RXDTVALIE                         ((uint32_t)0x00200000)        /*!< Data available in Rx FIFO interrupt Enable */
#define  SDIO_IER_SDIOINTIE                         ((uint32_t)0x00400000)        /*!< SDIO Mode Interrupt Received interrupt Enable */
#define  SDIO_IER_ATAENDIE                          ((uint32_t)0x00800000)        /*!< CE-ATA command completion signal received Interrupt Enable */

/*****************  Bit definition for SDIO_FIFOCNT register  *****************/
#define  SDIO_FIFOCNT_FIFOCNT                       ((uint32_t)0x00FFFFFF)        /*!< Remaining number of words to be written to or read from the FIFO */

/******************  Bit definition for SDIO_FIFO register  *******************/
#define  SDIO_FIFO_FIFODT                           ((uint32_t)0xFFFFFFFF)        /*!< Receive and transmit FIFO data */



/******************************************************************************/
/*                                                                            */
/*                        Serial Peripheral Interface                         */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for SPI_CTLR1 register  ********************/
#define  SPI_CTLR1_SCKPH                            ((uint16_t)0x0001)            /*!< Clock Phase */
#define  SPI_CTLR1_SCKPL                            ((uint16_t)0x0002)            /*!< Clock Polarity */
#define  SPI_CTLR1_MSTMODE                          ((uint16_t)0x0004)            /*!< Master Selection */
																	        
#define  SPI_CTLR1_PSC                              ((uint16_t)0x0038)            /*!< PSC[2:0] bits (Baud Rate Control) */
#define  SPI_CTLR1_PSC_0                            ((uint16_t)0x0008)            /*!< Bit 0 */
#define  SPI_CTLR1_PSC_1                            ((uint16_t)0x0010)            /*!< Bit 1 */
#define  SPI_CTLR1_PSC_2                            ((uint16_t)0x0020)            /*!< Bit 2 */

#define  SPI_CTLR1_SPIEN                            ((uint16_t)0x0040)            /*!< SPI Enable */
#define  SPI_CTLR1_LF                               ((uint16_t)0x0080)            /*!< Frame Format */
#define  SPI_CTLR1_SWNSS                            ((uint16_t)0x0100)            /*!< Internal slave select */
#define  SPI_CTLR1_SWNSSEN                          ((uint16_t)0x0200)            /*!< Software slave management */
#define  SPI_CTLR1_RO                               ((uint16_t)0x0400)            /*!< Receive only */
#define  SPI_CTLR1_FF16                             ((uint16_t)0x0800)            /*!< Data Frame Format */
#define  SPI_CTLR1_CRCNT                            ((uint16_t)0x1000)            /*!< Transmit CRC next */
#define  SPI_CTLR1_CRCEN                            ((uint16_t)0x2000)            /*!< Hardware CRC calculation enable */
#define  SPI_CTLR1_BDOE                             ((uint16_t)0x4000)            /*!< Output enable in bidirectional mode */
#define  SPI_CTLR1_BDM                              ((uint16_t)0x8000)            /*!< Bidirectional data mode enable */

/*******************  Bit definition for SPI_CTLR2 register  ********************/
#define  SPI_CTLR2_DMARE                            ((uint8_t)0x01)               /*!< Rx Buffer DMA Enable */
#define  SPI_CTLR2_DMATE                            ((uint8_t)0x02)               /*!< Tx Buffer DMA Enable */
#define  SPI_CTLR2_NSSDRV                           ((uint8_t)0x04)               /*!< SS Output Enable */
#define  SPI_CTLR2_ERRIE                            ((uint8_t)0x20)               /*!< Error Interrupt Enable */
#define  SPI_CTLR2_RBNEIE                           ((uint8_t)0x40)               /*!< RX buffer Not Empty Interrupt Enable */
#define  SPI_CTLR2_TBEIE                            ((uint8_t)0x80)               /*!< Tx buffer Empty Interrupt Enable */

/********************  Bit definition for SPI_STR register  ********************/
#define  SPI_STR_RBNE                               ((uint8_t)0x01)               /*!< Receive buffer Not Empty */
#define  SPI_STR_TBE                                ((uint8_t)0x02)               /*!< Transmit buffer Empty */
#define  SPI_STR_I2SCH                              ((uint8_t)0x04)               /*!< Channel side */
#define  SPI_STR_TXURE                              ((uint8_t)0x08)               /*!< Underrun flag */
#define  SPI_STR_CRCE                               ((uint8_t)0x10)               /*!< CRC Error flag */
#define  SPI_STR_CONFE                              ((uint8_t)0x20)               /*!< Mode fault */
#define  SPI_STR_RXORE                              ((uint8_t)0x40)               /*!< Overrun flag */
#define  SPI_STR_TRANS                              ((uint8_t)0x80)               /*!< Busy flag */

/********************  Bit definition for SPI_DTR register  ********************/
#define  SPI_DTR_DTR                                ((uint16_t)0xFFFF)            /*!< Data Register */

/*******************  Bit definition for SPI_CRR register  ******************/
#define  SPI_CPR_CPR                                ((uint16_t)0xFFFF)            /*!< CRC polynomial register */

/******************  Bit definition for SPI_RCR register  ******************/
#define  SPI_RCR_RCR                                ((uint16_t)0xFFFF)            /*!< Rx CRC Register */

/******************  Bit definition for SPI_TCR register  ******************/
#define  SPI_TCR_TCR                                ((uint16_t)0xFFFF)            /*!< Tx CRC Register */

/******************  Bit definition for SPI_I2SCTLR register  *****************/
#define  SPI_I2SCTLR_CHLEN                          ((uint16_t)0x0001)            /*!< Channel length (number of bits per audio channel) */

#define  SPI_I2SCTLR_DTLEN                          ((uint16_t)0x0006)            /*!< DTLEN[1:0] bits (Data length to be transferred) */
#define  SPI_I2SCTLR_DTLEN_0                        ((uint16_t)0x0002)            /*!< Bit 0 */
#define  SPI_I2SCTLR_DTLEN_1                        ((uint16_t)0x0004)            /*!< Bit 1 */

#define  SPI_I2SCTLR_CKPL                           ((uint16_t)0x0008)            /*!< steady state clock polarity */
									        
#define  SPI_I2SCTLR_I2SSTD                         ((uint16_t)0x0030)            /*!< I2SSTD[1:0] bits (I2S standard selection) */
#define  SPI_I2SCTLR_I2SSTD_0                       ((uint16_t)0x0010)            /*!< Bit 0 */
#define  SPI_I2SCTLR_I2SSTD_1                       ((uint16_t)0x0020)            /*!< Bit 1 */

#define  SPI_I2SCTLR_PCMSM                          ((uint16_t)0x0080)            /*!< PCM frame synchronization */

#define  SPI_I2SCTLR_I2SOM                          ((uint16_t)0x0300)            /*!< I2SOM[1:0] bits (I2S configuration mode) */
#define  SPI_I2SCTLR_I2SOM_0                        ((uint16_t)0x0100)            /*!< Bit 0 */
#define  SPI_I2SCTLR_I2SOM_1                        ((uint16_t)0x0200)            /*!< Bit 1 */

#define  SPI_I2SCTLR_I2SEN                          ((uint16_t)0x0400)            /*!< I2S Enable */
#define  SPI_I2SCTLR_I2SSEL                         ((uint16_t)0x0800)            /*!< I2S mode selection */

/******************  Bit definition for SPI_I2SCKP register  *******************/
#define  SPI_I2SCKP_DIV                             ((uint16_t)0x00FF)            /*!< I2S Linear prescaler */
#define  SPI_I2SCKP_OF                              ((uint16_t)0x0100)            /*!< Odd factor for the prescaler */
#define  SPI_I2SCKP_MCKOE                           ((uint16_t)0x0200)            /*!< Master Clock Output Enable */

/******************  Bit definition for SPI_QWCR register  *******************/
#define  SPI_QWCR_QMOD                              ((uint16_t)0x0001)            /*!< Quad wire mode Enable */
#define  SPI_QWCR_QRD                               ((uint16_t)0x0002)            /*!< Quad wire read select */
#define  SPI_QWCR_IO34_DRV                          ((uint16_t)0x0004)            /*!< Drive IO3 and IO4 Enable */



/******************************************************************************/
/*                                                                            */
/*                               Timers (TIMER)                               */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for TIMER_CTLR1 register  ********************/
#define  TIMER_CTLR1_CNTE                           ((uint16_t)0x0001)            /*!<Counter enable */
#define  TIMER_CTLR1_UPDIS                          ((uint16_t)0x0002)            /*!<Update disable */
#define  TIMER_CTLR1_UPS                            ((uint16_t)0x0004)            /*!<Update request source */
#define  TIMER_CTLR1_SPM                            ((uint16_t)0x0008)            /*!<One pulse mode */
#define  TIMER_CTLR1_DIR                            ((uint16_t)0x0010)            /*!<Direction */
											 
#define  TIMER_CTLR1_CAM                            ((uint16_t)0x0060)            /*!<CAM[1:0] bits (Center-aligned mode selection) */
#define  TIMER_CTLR1_CAM_0                          ((uint16_t)0x0020)            /*!<Bit 0 */
#define  TIMER_CTLR1_CAM_1                          ((uint16_t)0x0040)            /*!<Bit 1 */

#define  TIMER_CTLR1_ARSE                           ((uint16_t)0x0080)            /*!<Auto-reload preload enable */
												  
#define  TIMER_CTLR1_CDIV                           ((uint16_t)0x0300)            /*!<CDIV[1:0] bits (clock division) */
#define  TIMER_CTLR1_CDIV_0                         ((uint16_t)0x0100)            /*!<Bit 0 */
#define  TIMER_CTLR1_CDIV_1                         ((uint16_t)0x0200)            /*!<Bit 1 */

/*******************  Bit definition for TIMER_CTLR2 register  ********************/
#define  TIMER_CTLR2_CCSE                           ((uint16_t)0x0001)            /*!<Capture/Compare Preloaded Control */
#define  TIMER_CTLR2_CCUC                           ((uint16_t)0x0004)            /*!<Capture/Compare Control Update Selection */
#define  TIMER_CTLR2_DMAS                           ((uint16_t)0x0008)            /*!<Capture/Compare DMA Selection */
												 
#define  TIMER_CTLR2_MMC                            ((uint16_t)0x0070)            /*!<MMC[2:0] bits (Master Mode Selection) */
#define  TIMER_CTLR2_MMC_0                          ((uint16_t)0x0010)            /*!<Bit 0 */
#define  TIMER_CTLR2_MMC_1                          ((uint16_t)0x0020)            /*!<Bit 1 */
#define  TIMER_CTLR2_MMC_2                          ((uint16_t)0x0040)            /*!<Bit 2 */

#define  TIMER_CTLR2_TI1S                           ((uint16_t)0x0080)            /*!<TI1 Selection */
#define  TIMER_CTLR2_ISO1                           ((uint16_t)0x0100)            /*!<Output Idle state 1 (OC1 output) */
#define  TIMER_CTLR2_ISO1N                          ((uint16_t)0x0200)            /*!<Output Idle state 1 (OC1N output) */
#define  TIMER_CTLR2_ISO2                           ((uint16_t)0x0400)            /*!<Output Idle state 2 (OC2 output) */
#define  TIMER_CTLR2_ISO2N                          ((uint16_t)0x0800)            /*!<Output Idle state 2 (OC2N output) */
#define  TIMER_CTLR2_ISO3                           ((uint16_t)0x1000)            /*!<Output Idle state 3 (OC3 output) */
#define  TIMER_CTLR2_ISO3N                          ((uint16_t)0x2000)            /*!<Output Idle state 3 (OC3N output) */
#define  TIMER_CTLR2_ISO4                           ((uint16_t)0x4000)            /*!<Output Idle state 4 (OC4 output) */
								    
/*******************  Bit definition for TIMER_SMC register  *******************/
#define  TIMER_SMC_SMC                              ((uint16_t)0x0007)            /*!<SMC[2:0] bits (Slave mode selection) */
#define  TIMER_SMC_SMC_0                            ((uint16_t)0x0001)            /*!<Bit 0 */
#define  TIMER_SMC_SMC_1                            ((uint16_t)0x0002)            /*!<Bit 1 */
#define  TIMER_SMC_SMC_2                            ((uint16_t)0x0004)            /*!<Bit 2 */
									        
#define  TIMER_SMC_TRGS                             ((uint16_t)0x0070)            /*!<TRGS[2:0] bits (Trigger selection) */
#define  TIMER_SMC_TRGS_0                           ((uint16_t)0x0010)            /*!<Bit 0 */
#define  TIMER_SMC_TRGS_1                           ((uint16_t)0x0020)            /*!<Bit 1 */
#define  TIMER_SMC_TRGS_2                           ((uint16_t)0x0040)            /*!<Bit 2 */

#define  TIMER_SMC_MSM                              ((uint16_t)0x0080)            /*!<Master/slave mode */
											  
#define  TIMER_SMC_ETFC                             ((uint16_t)0x0F00)            /*!<ETFC[3:0] bits (External trigger filter) */
#define  TIMER_SMC_ETFC_0                           ((uint16_t)0x0100)            /*!<Bit 0 */
#define  TIMER_SMC_ETFC_1                           ((uint16_t)0x0200)            /*!<Bit 1 */
#define  TIMER_SMC_ETFC_2                           ((uint16_t)0x0400)            /*!<Bit 2 */
#define  TIMER_SMC_ETFC_3                           ((uint16_t)0x0800)            /*!<Bit 3 */

#define  TIMER_SMC_ETPSC                            ((uint16_t)0x3000)            /*!<ETPSC[1:0] bits (External trigger prescaler) */
#define  TIMER_SMC_ETPSC_0                          ((uint16_t)0x1000)            /*!<Bit 0 */
#define  TIMER_SMC_ETPSC_1                          ((uint16_t)0x2000)            /*!<Bit 1 */
										  
#define  TIMER_SMC_ECM2E                            ((uint16_t)0x4000)            /*!<External clock enable */
#define  TIMER_SMC_ETPL                             ((uint16_t)0x8000)            /*!<External trigger polarity */
											 
/*******************  Bit definition for TIMER_DIE register  *******************/
#define  TIMER_DIE_UPIE                             ((uint16_t)0x0001)            /*!<Update interrupt enable */
#define  TIMER_DIE_CH1IE                            ((uint16_t)0x0002)            /*!<Capture/Compare 1 interrupt enable */
#define  TIMER_DIE_CH2IE                            ((uint16_t)0x0004)            /*!<Capture/Compare 2 interrupt enable */
#define  TIMER_DIE_CH3IE                            ((uint16_t)0x0008)            /*!<Capture/Compare 3 interrupt enable */
#define  TIMER_DIE_CH4IE                            ((uint16_t)0x0010)            /*!<Capture/Compare 4 interrupt enable */
#define  TIMER_DIE_CCUIE                            ((uint16_t)0x0020)            /*!<COM interrupt enable */
#define  TIMER_DIE_TRGIE                            ((uint16_t)0x0040)            /*!<Trigger interrupt enable */
#define  TIMER_DIE_BKIE                             ((uint16_t)0x0080)            /*!<Break interrupt enable */
#define  TIMER_DIE_UPDE                             ((uint16_t)0x0100)            /*!<Update DMA request enable */
#define  TIMER_DIE_CH1DE                            ((uint16_t)0x0200)            /*!<Capture/Compare 1 DMA request enable */
#define  TIMER_DIE_CH2DE                            ((uint16_t)0x0400)            /*!<Capture/Compare 2 DMA request enable */
#define  TIMER_DIE_CH3DE                            ((uint16_t)0x0800)            /*!<Capture/Compare 3 DMA request enable */
#define  TIMER_DIE_CH4DE                            ((uint16_t)0x1000)            /*!<Capture/Compare 4 DMA request enable */
#define  TIMER_DIE_CCUDE                            ((uint16_t)0x2000)            /*!<COM DMA request enable */
#define  TIMER_DIE_TRGDE                            ((uint16_t)0x4000)            /*!<Trigger DMA request enable */

/********************  Bit definition for TIMER_STR register  ********************/
#define  TIMER_STR_UPIF                             ((uint16_t)0x0001)            /*!<Update interrupt Flag */
#define  TIMER_STR_CH1IF                            ((uint16_t)0x0002)            /*!<Capture/Compare 1 interrupt Flag */
#define  TIMER_STR_CH2IF                            ((uint16_t)0x0004)            /*!<Capture/Compare 2 interrupt Flag */
#define  TIMER_STR_CH3IF                            ((uint16_t)0x0008)            /*!<Capture/Compare 3 interrupt Flag */
#define  TIMER_STR_CH4IF                            ((uint16_t)0x0010)            /*!<Capture/Compare 4 interrupt Flag */
#define  TIMER_STR_CCUIF                            ((uint16_t)0x0020)            /*!<COM interrupt Flag */
#define  TIMER_STR_TRGIF                            ((uint16_t)0x0040)            /*!<Trigger interrupt Flag */
#define  TIMER_STR_BKIF                             ((uint16_t)0x0080)            /*!<Break interrupt Flag */
#define  TIMER_STR_CH1OF                            ((uint16_t)0x0200)            /*!<Capture/Compare 1 Overcapture Flag */
#define  TIMER_STR_CH2OF                            ((uint16_t)0x0400)            /*!<Capture/Compare 2 Overcapture Flag */
#define  TIMER_STR_CH3OF                            ((uint16_t)0x0800)            /*!<Capture/Compare 3 Overcapture Flag */
#define  TIMER_STR_CH4OF                            ((uint16_t)0x1000)            /*!<Capture/Compare 4 Overcapture Flag */

/*******************  Bit definition for TIMER_EVG register  ********************/
#define  TIMER_EVG_UPG                              ((uint8_t)0x01)               /*!<Update Generation */
#define  TIMER_EVG_CH1G                             ((uint8_t)0x02)               /*!<Capture/Compare 1 Generation */
#define  TIMER_EVG_CH2G                             ((uint8_t)0x04)               /*!<Capture/Compare 2 Generation */
#define  TIMER_EVG_CH3G                             ((uint8_t)0x08)               /*!<Capture/Compare 3 Generation */
#define  TIMER_EVG_CH4G                             ((uint8_t)0x10)               /*!<Capture/Compare 4 Generation */
#define  TIMER_EVG_CCUG                             ((uint8_t)0x20)               /*!<Capture/Compare Control Update Generation */
#define  TIMER_EVG_TRGG                             ((uint8_t)0x40)               /*!<Trigger Generation */
#define  TIMER_EVG_BKG                              ((uint8_t)0x80)               /*!<Break Generation */

/******************  Bit definition for TIMER_CHCTLR1 register  *******************/
#define  TIMER_CHCTLR1_CH1M                         ((uint16_t)0x0003)            /*!<CH1M[1:0] bits (Capture/Compare 1 Selection) */
#define  TIMER_CHCTLR1_CH1M_0                       ((uint16_t)0x0001)            /*!<Bit 0 */
#define  TIMER_CHCTLR1_CH1M_1                       ((uint16_t)0x0002)            /*!<Bit 1 */

#define  TIMER_CHCTLR1_CH1OFE                       ((uint16_t)0x0004)            /*!<Output Compare 1 Fast enable */
#define  TIMER_CHCTLR1_CH1OSE                       ((uint16_t)0x0008)            /*!<Output Compare 1 Preload enable */
								          
#define  TIMER_CHCTLR1_CH1OM                        ((uint16_t)0x0070)            /*!<CH1OM[2:0] bits (Output Compare 1 Mode) */
#define  TIMER_CHCTLR1_CH1OM_0                      ((uint16_t)0x0010)            /*!<Bit 0 */
#define  TIMER_CHCTLR1_CH1OM_1                      ((uint16_t)0x0020)            /*!<Bit 1 */
#define  TIMER_CHCTLR1_CH1OM_2                      ((uint16_t)0x0040)            /*!<Bit 2 */

#define  TIMER_CHCTLR1_CH1OCE                       ((uint16_t)0x0080)            /*!<Output Compare 1Clear Enable */

#define  TIMER_CHCTLR1_CH2M                         ((uint16_t)0x0300)            /*!<CH2M[1:0] bits (Capture/Compare 2 Selection) */
#define  TIMER_CHCTLR1_CH2M_0                       ((uint16_t)0x0100)            /*!<Bit 0 */
#define  TIMER_CHCTLR1_CH2M_1                       ((uint16_t)0x0200)            /*!<Bit 1 */

#define  TIMER_CHCTLR1_CH2OFE                       ((uint16_t)0x0400)            /*!<Output Compare 2 Fast enable */
#define  TIMER_CHCTLR1_CH2OSE                       ((uint16_t)0x0800)            /*!<Output Compare 2 Preload enable */

#define  TIMER_CHCTLR1_CH2OM                        ((uint16_t)0x7000)            /*!<CH2OM[2:0] bits (Output Compare 2 Mode) */
#define  TIMER_CHCTLR1_CH2OM_0                      ((uint16_t)0x1000)            /*!<Bit 0 */
#define  TIMER_CHCTLR1_CH2OM_1                      ((uint16_t)0x2000)            /*!<Bit 1 */
#define  TIMER_CHCTLR1_CH2OM_2                      ((uint16_t)0x4000)            /*!<Bit 2 */

#define  TIMER_CHCTLR1_CH2OCE                       ((uint16_t)0x8000)            /*!<Output Compare 2 Clear Enable */

/*----------------------------------------------------------------------------*/

#define  TIMER_CHCTLR1_CH1ICP                       ((uint16_t)0x000C)            /*!<CH1ICP[1:0] bits (Input Capture 1 Prescaler) */
#define  TIMER_CHCTLR1_CH1ICP_0                     ((uint16_t)0x0004)            /*!<Bit 0 */
#define  TIMER_CHCTLR1_CH1ICP_1                     ((uint16_t)0x0008)            /*!<Bit 1 */

#define  TIMER_CHCTLR1_CH1ICF                       ((uint16_t)0x00F0)            /*!<CH1ICF[3:0] bits (Input Capture 1 Filter) */
#define  TIMER_CHCTLR1_CH1ICF_0                     ((uint16_t)0x0010)            /*!<Bit 0 */
#define  TIMER_CHCTLR1_CH1ICF_1                     ((uint16_t)0x0020)            /*!<Bit 1 */
#define  TIMER_CHCTLR1_CH1ICF_2                     ((uint16_t)0x0040)            /*!<Bit 2 */
#define  TIMER_CHCTLR1_CH1ICF_3                     ((uint16_t)0x0080)            /*!<Bit 3 */
									       
#define  TIMER_CHCTLR1_CH2ICP                       ((uint16_t)0x0C00)            /*!<CH2ICP[1:0] bits (Input Capture 2 Prescaler) */
#define  TIMER_CHCTLR1_CH2ICP_0                     ((uint16_t)0x0400)            /*!<Bit 0 */
#define  TIMER_CHCTLR1_CH2ICP_1                     ((uint16_t)0x0800)            /*!<Bit 1 */

#define  TIMER_CHCTLR1_CH2ICF                       ((uint16_t)0xF000)            /*!<CH2ICF[3:0] bits (Input Capture 2 Filter) */
#define  TIMER_CHCTLR1_CH2ICF_0                     ((uint16_t)0x1000)            /*!<Bit 0 */
#define  TIMER_CHCTLR1_CH2ICF_1                     ((uint16_t)0x2000)            /*!<Bit 1 */
#define  TIMER_CHCTLR1_CH2ICF_2                     ((uint16_t)0x4000)            /*!<Bit 2 */
#define  TIMER_CHCTLR1_CH2ICF_3                     ((uint16_t)0x8000)            /*!<Bit 3 */

/******************  Bit definition for TIMER_CHCTLR2 register  *******************/
#define  TIMER_CHCTLR2_CH3M                         ((uint16_t)0x0003)            /*!<CH3M[1:0] bits (Capture/Compare 3 Selection) */
#define  TIMER_CHCTLR2_CH3M_0                       ((uint16_t)0x0001)            /*!<Bit 0 */
#define  TIMER_CHCTLR2_CH3M_1                       ((uint16_t)0x0002)            /*!<Bit 1 */
									        
#define  TIMER_CHCTLR2_CH3OFE                       ((uint16_t)0x0004)            /*!<Output Compare 3 Fast enable */
#define  TIMER_CHCTLR2_CH3OSE                       ((uint16_t)0x0008)            /*!<Output Compare 3 Preload enable */
										       
#define  TIMER_CHCTLR2_CH3OM                        ((uint16_t)0x0070)            /*!<CH3OM[2:0] bits (Output Compare 3 Mode) */
#define  TIMER_CHCTLR2_CH3OM_0                      ((uint16_t)0x0010)            /*!<Bit 0 */
#define  TIMER_CHCTLR2_CH3OM_1                      ((uint16_t)0x0020)            /*!<Bit 1 */
#define  TIMER_CHCTLR2_CH3OM_2                      ((uint16_t)0x0040)            /*!<Bit 2 */

#define  TIMER_CHCTLR2_CH3OCE                       ((uint16_t)0x0080)            /*!<Output Compare 3 Clear Enable */

#define  TIMER_CHCTLR2_CH4M                         ((uint16_t)0x0300)            /*!<CH4M[1:0] bits (Capture/Compare 4 Selection) */
#define  TIMER_CHCTLR2_CH4M_0                       ((uint16_t)0x0100)            /*!<Bit 0 */
#define  TIMER_CHCTLR2_CH4M_1                       ((uint16_t)0x0200)            /*!<Bit 1 */

#define  TIMER_CHCTLR2_CH4OFE                       ((uint16_t)0x0400)            /*!<Output Compare 4 Fast enable */
#define  TIMER_CHCTLR2_CH4OSE                       ((uint16_t)0x0800)            /*!<Output Compare 4 Preload enable */

#define  TIMER_CHCTLR2_CH4OM                        ((uint16_t)0x7000)            /*!<CH4OM[2:0] bits (Output Compare 4 Mode) */
#define  TIMER_CHCTLR2_CH4OM_0                      ((uint16_t)0x1000)            /*!<Bit 0 */
#define  TIMER_CHCTLR2_CH4OM_1                      ((uint16_t)0x2000)            /*!<Bit 1 */
#define  TIMER_CHCTLR2_CH4OM_2                      ((uint16_t)0x4000)            /*!<Bit 2 */

#define  TIMER_CHCTLR2_CH4OCE                       ((uint16_t)0x8000)            /*!<Output Compare 4 Clear Enable */

/*----------------------------------------------------------------------------*/

#define  TIMER_CHCTLR2_CH3ICP                       ((uint16_t)0x000C)            /*!<CH3ICP[1:0] bits (Input Capture 3 Prescaler) */
#define  TIMER_CHCTLR2_CH3ICP_0                     ((uint16_t)0x0004)            /*!<Bit 0 */
#define  TIMER_CHCTLR2_CH3ICP_1                     ((uint16_t)0x0008)            /*!<Bit 1 */

#define  TIMER_CHCTLR2_CH3ICF                       ((uint16_t)0x00F0)            /*!<CH3ICF[3:0] bits (Input Capture 3 Filter) */
#define  TIMER_CHCTLR2_CH3ICF_0                     ((uint16_t)0x0010)            /*!<Bit 0 */
#define  TIMER_CHCTLR2_CH3ICF_1                     ((uint16_t)0x0020)            /*!<Bit 1 */
#define  TIMER_CHCTLR2_CH3ICF_2                     ((uint16_t)0x0040)            /*!<Bit 2 */
#define  TIMER_CHCTLR2_CH3ICF_3                     ((uint16_t)0x0080)            /*!<Bit 3 */

#define  TIMER_CHCTLR2_CH4ICP                       ((uint16_t)0x0C00)            /*!<CH4ICP[1:0] bits (Input Capture 4 Prescaler) */
#define  TIMER_CHCTLR2_CH4ICP_0                     ((uint16_t)0x0400)            /*!<Bit 0 */
#define  TIMER_CHCTLR2_CH4ICP_1                     ((uint16_t)0x0800)            /*!<Bit 1 */

#define  TIMER_CHCTLR2_CH4ICF                       ((uint16_t)0xF000)            /*!<CH4ICF[3:0] bits (Input Capture 4 Filter) */
#define  TIMER_CHCTLR2_CH4ICF_0                     ((uint16_t)0x1000)            /*!<Bit 0 */
#define  TIMER_CHCTLR2_CH4ICF_1                     ((uint16_t)0x2000)            /*!<Bit 1 */
#define  TIMER_CHCTLR2_CH4ICF_2                     ((uint16_t)0x4000)            /*!<Bit 2 */
#define  TIMER_CHCTLR2_CH4ICF_3                     ((uint16_t)0x8000)            /*!<Bit 3 */

/*******************  Bit definition for TIMER_CHE register  *******************/
#define  TIMER_CHE_CH1E                             ((uint16_t)0x0001)            /*!<Capture/Compare 1 output enable */
#define  TIMER_CHE_CH1P                             ((uint16_t)0x0002)            /*!<Capture/Compare 1 output Polarity */
#define  TIMER_CHE_CH1NE                            ((uint16_t)0x0004)            /*!<Capture/Compare 1 Complementary output enable */
#define  TIMER_CHE_CH1NP                            ((uint16_t)0x0008)            /*!<Capture/Compare 1 Complementary output Polarity */
#define  TIMER_CHE_CH2E                             ((uint16_t)0x0010)            /*!<Capture/Compare 2 output enable */
#define  TIMER_CHE_CH2P                             ((uint16_t)0x0020)            /*!<Capture/Compare 2 output Polarity */
#define  TIMER_CHE_CH2NE                            ((uint16_t)0x0040)            /*!<Capture/Compare 2 Complementary output enable */
#define  TIMER_CHE_CH2NP                            ((uint16_t)0x0080)            /*!<Capture/Compare 2 Complementary output Polarity */
#define  TIMER_CHE_CH3E                             ((uint16_t)0x0100)            /*!<Capture/Compare 3 output enable */
#define  TIMER_CHE_CH3P                             ((uint16_t)0x0200)            /*!<Capture/Compare 3 output Polarity */
#define  TIMER_CHE_CH3NE                            ((uint16_t)0x0400)            /*!<Capture/Compare 3 Complementary output enable */
#define  TIMER_CHE_CH3NP                            ((uint16_t)0x0800)            /*!<Capture/Compare 3 Complementary output Polarity */
#define  TIMER_CHE_CH4E                             ((uint16_t)0x1000)            /*!<Capture/Compare 4 output enable */
#define  TIMER_CHE_CH4P                             ((uint16_t)0x2000)            /*!<Capture/Compare 4 output Polarity */
#define  TIMER_CHE_CH4NP                            ((uint16_t)0x8000)            /*!<Capture/Compare 4 Complementary output Polarity */

/*******************  Bit definition for TIMER_CNT register  ********************/
#define  TIMER_CNT_CNT                              ((uint16_t)0xFFFF)            /*!<Counter Value */

/*******************  Bit definition for TIMER_PSC register  ********************/
#define  TIMER_PSC_PSC                              ((uint16_t)0xFFFF)            /*!<Prescaler Value */

/*******************  Bit definition for TIMER_CARL register  ********************/
#define  TIMER_CARL_CARL                            ((uint16_t)0xFFFF)            /*!<actual auto-reload Value */

/*******************  Bit definition for TIMER_CREP register  ********************/
#define  TIMER_CREP_CREP                            ((uint8_t)0xFF)               /*!<Repetition Counter Value */

/*******************  Bit definition for TIMER_CHCC1 register  *******************/
#define  TIMER_CHCC1_CHCC1                          ((uint16_t)0xFFFF)            /*!<Capture/Compare 1 Value */

/*******************  Bit definition for TIMER_CHCC2 register  *******************/
#define  TIMER_CHCC2_CHCC2                          ((uint16_t)0xFFFF)            /*!<Capture/Compare 2 Value */

/*******************  Bit definition for TIMER_CHCC3 register  *******************/
#define  TIMER_CHCC3_CHCC3                          ((uint16_t)0xFFFF)            /*!<Capture/Compare 3 Value */

/*******************  Bit definition for TIMER_CHCC4 register  *******************/
#define  TIMER_CHCC4_CHCC4                          ((uint16_t)0xFFFF)            /*!<Capture/Compare 4 Value */

/*******************  Bit definition for TIMER_BKDT register  *******************/
#define  TIMER_BKDT_DT                              ((uint16_t)0x00FF)            /*!<DT[0:7] bits (Dead-TIMERe Generator set-up) */
#define  TIMER_BKDT_DT_0                            ((uint16_t)0x0001)            /*!<Bit 0 */
#define  TIMER_BKDT_DT_1                            ((uint16_t)0x0002)            /*!<Bit 1 */
#define  TIMER_BKDT_DT_2                            ((uint16_t)0x0004)            /*!<Bit 2 */
#define  TIMER_BKDT_DT_3                            ((uint16_t)0x0008)            /*!<Bit 3 */
#define  TIMER_BKDT_DT_4                            ((uint16_t)0x0010)            /*!<Bit 4 */
#define  TIMER_BKDT_DT_5                            ((uint16_t)0x0020)            /*!<Bit 5 */
#define  TIMER_BKDT_DT_6                            ((uint16_t)0x0040)            /*!<Bit 6 */
#define  TIMER_BKDT_DT_7                            ((uint16_t)0x0080)            /*!<Bit 7 */

#define  TIMER_BKDT_LK                              ((uint16_t)0x0300)            /*!<LK[1:0] bits (Lock Configuration) */
#define  TIMER_BKDT_LK_0                            ((uint16_t)0x0100)            /*!<Bit 0 */
#define  TIMER_BKDT_LK_1                            ((uint16_t)0x0200)            /*!<Bit 1 */

#define  TIMER_BKDT_IOS                             ((uint16_t)0x0400)            /*!<Off-State Selection for Idle mode */
#define  TIMER_BKDT_ROS                             ((uint16_t)0x0800)            /*!<Off-State Selection for Run mode */
#define  TIMER_BKDT_BRKE                            ((uint16_t)0x1000)            /*!<Break enable */
#define  TIMER_BKDT_BRKP                            ((uint16_t)0x2000)            /*!<Break Polarity */
#define  TIMER_BKDT_OAE                             ((uint16_t)0x4000)            /*!<Automatic Output enable */
#define  TIMER_BKDT_POE                             ((uint16_t)0x8000)            /*!<Main Output enable */

/*******************  Bit definition for TIMER_DCTLR register  ********************/
#define  TIMER_DCTLR_DBAR                           ((uint16_t)0x001F)            /*!<DBAR[4:0] bits (DMA Base Address) */
#define  TIMER_DCTLR_DBAR_0                         ((uint16_t)0x0001)            /*!<Bit 0 */
#define  TIMER_DCTLR_DBAR_1                         ((uint16_t)0x0002)            /*!<Bit 1 */
#define  TIMER_DCTLR_DBAR_2                         ((uint16_t)0x0004)            /*!<Bit 2 */
#define  TIMER_DCTLR_DBAR_3                         ((uint16_t)0x0008)            /*!<Bit 3 */
#define  TIMER_DCTLR_DBAR_4                         ((uint16_t)0x0010)            /*!<Bit 4 */

#define  TIMER_DCTLR_DBLTH                          ((uint16_t)0x1F00)            /*!<DBLTH[4:0] bits (DMA Burst Length) */
#define  TIMER_DCTLR_DBLTH_0                        ((uint16_t)0x0100)            /*!<Bit 0 */
#define  TIMER_DCTLR_DBLTH_1                        ((uint16_t)0x0200)            /*!<Bit 1 */
#define  TIMER_DCTLR_DBLTH_2                        ((uint16_t)0x0400)            /*!<Bit 2 */
#define  TIMER_DCTLR_DBLTH_3                        ((uint16_t)0x0800)            /*!<Bit 3 */
#define  TIMER_DCTLR_DBLTH_4                        ((uint16_t)0x1000)            /*!<Bit 4 */

/*******************  Bit definition for TIMER_DTRSF register  *******************/
#define  TIMER_DTRSF_DTRSF                          ((uint16_t)0xFFFF)            /*!<DMA register for burst accesses */

/******************************************************************************/
/*                                                                            */
/*      Universal Synchronous Asynchronous Receiver Transmitter (USART)       */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for USART_STR register  *******************/
#define  USART_STR_PE                               ((uint32_t)0x00000001)        /*!< Parity Error */
#define  USART_STR_FE                               ((uint32_t)0x00000002)        /*!< Framing Error */
#define  USART_STR_NE                               ((uint32_t)0x00000004)        /*!< Noise Error Flag */
#define  USART_STR_ORE                              ((uint32_t)0x00000008)        /*!< OverRun Error */
#define  USART_STR_IDLEF                            ((uint32_t)0x00000010)        /*!< IDLE Line Detected Flag*/
#define  USART_STR_RBNE                             ((uint32_t)0x00000020)        /*!< Read Data Buffer Not Empty */
#define  USART_STR_TC                               ((uint32_t)0x00000040)        /*!< Transmission Complete */
#define  USART_STR_TBE                              ((uint32_t)0x00000080)        /*!< Transmit Data Buffer Empty */
#define  USART_STR_LBDF                             ((uint32_t)0x00000100)        /*!< LIN Break Detected Flag */
#define  USART_STR_CTSF                             ((uint32_t)0x00000200)        /*!< CTS Flag */

/*******************  Bit definition for USART_DR register  *******************/
#define  USART_DR_DR                                ((uint32_t)0x000001FF)        /*!< Data value */

/*******************  Bit definition for USART_BRR register *******************/
#define  USART_BRR_BRRF                             ((uint32_t)0x0000000F)        /*!< Fraction of USARTDIV */
#define  USART_BRR_BRRM                             ((uint32_t)0x0000FFF0)        /*!< Mantissa of USARTDIV */

/*******************  Bit definition for USART_CTLR1 register  *******************/
#define  USART_CTLR1_SBKCMD                         ((uint32_t)0x00000001)        /*!< Send Break Command */
#define  USART_CTLR1_RWU                            ((uint32_t)0x00000002)        /*!< Receiver Wakeup */
#define  USART_CTLR1_REN                            ((uint32_t)0x00000004)        /*!< Receiver Enable */
#define  USART_CTLR1_TEN                            ((uint32_t)0x00000008)        /*!< Transmitter Enable */
#define  USART_CTLR1_IDLEIE                         ((uint32_t)0x00000010)        /*!< IDLE Interrupt Enable */
#define  USART_CTLR1_RBNEIE                         ((uint32_t)0x00000020)        /*!< RBNE Interrupt Enable */
#define  USART_CTLR1_TCIE                           ((uint32_t)0x00000040)        /*!< Transmission Complete Interrupt Enable */
#define  USART_CTLR1_TBEIE                          ((uint32_t)0x00000080)        /*!< TBE Interrupt Enable */
#define  USART_CTLR1_PEIE                           ((uint32_t)0x00000100)        /*!< PE Interrupt Enable */
#define  USART_CTLR1_PM                             ((uint32_t)0x00000200)        /*!< Parity Selection */
#define  USART_CTLR1_PCEN                           ((uint32_t)0x00000400)        /*!< Parity Control Enable */
#define  USART_CTLR1_WM                             ((uint32_t)0x00000800)        /*!< Wakeup method */
#define  USART_CTLR1_WL                             ((uint32_t)0x00001000)        /*!< Word Length */
#define  USART_CTLR1_UEN                            ((uint32_t)0x00002000)        /*!< USART Enable */

/******************  Bit definition for USART_CTLR2 register  *******************/
#define  USART_CTLR2_ADD                            ((uint32_t)0x0000000F)        /*!< Address of the USART node */
#define  USART_CTLR2_LBDL                           ((uint32_t)0x00000020)        /*!< LIN Break Detection Length */
#define  USART_CTLR2_LBDIE                          ((uint32_t)0x00000040)        /*!< LIN Break Detection Interrupt Enable */
#define  USART_CTLR2_LBCP                           ((uint32_t)0x00000100)        /*!< Last Bit Clock pulse */
#define  USART_CTLR2_CPH                            ((uint32_t)0x00000200)        /*!< Clock Phase */
#define  USART_CTLR2_CPL                            ((uint32_t)0x00000400)        /*!< Clock Polarity */
#define  USART_CTLR2_CKEN                           ((uint32_t)0x00000800)        /*!< Clock Enable */

#define  USART_CTLR2_STB                            ((uint32_t)0x00003000)        /*!< STOP[1:0] bits (STOP bits) */
#define  USART_CTLR2_STB_0                          ((uint32_t)0x00001000)        /*!< Bit 0 */
#define  USART_CTLR2_STB_1                          ((uint32_t)0x00002000)        /*!< Bit 1 */

#define  USART_CTLR2_LMEN                           ((uint32_t)0x00004000)        /*!< LIN Mode Enable */

/******************  Bit definition for USART_CTLR3 register  *******************/
#define  USART_CTLR3_ERIE                           ((uint32_t)0x00000001)        /*!< Error Interrupt Enable */
#define  USART_CTLR3_IREN                           ((uint32_t)0x00000002)        /*!< IrDA Mode Enable */
#define  USART_CTLR3_IRLP                           ((uint32_t)0x00000004)        /*!< IrDA Low-Power */
#define  USART_CTLR3_HDEN                           ((uint32_t)0x00000008)        /*!< Half-Duplex Enable */
#define  USART_CTLR3_NACK                           ((uint32_t)0x00000010)        /*!< Smartcard NACK Enable */
#define  USART_CTLR3_SCEN                           ((uint32_t)0x00000020)        /*!< Smartcard Mode Enable */
#define  USART_CTLR3_DENR                           ((uint32_t)0x00000040)        /*!< DMA Enable For Receiver */
#define  USART_CTLR3_DENT                           ((uint32_t)0x00000080)        /*!< DMA Enable For Transmitter */
#define  USART_CTLR3_RTSEN                          ((uint32_t)0x00000100)        /*!< RTS Enable */
#define  USART_CTLR3_CTSEN                          ((uint32_t)0x00000200)        /*!< CTS Enable */
#define  USART_CTLR3_CTSIE                          ((uint32_t)0x00000400)        /*!< CTS Interrupt Enable */

/******************  Bit definition for USART_GTPR register  ******************/
#define  USART_GTPR_PSC                             ((uint32_t)0x000000FF)        /*!< PSC[7:0] bits (Prescaler value) */
#define  USART_GTPR_PSC_0                           ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  USART_GTPR_PSC_1                           ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  USART_GTPR_PSC_2                           ((uint32_t)0x00000004)        /*!< Bit 2 */
#define  USART_GTPR_PSC_3                           ((uint32_t)0x00000008)        /*!< Bit 3 */
#define  USART_GTPR_PSC_4                           ((uint32_t)0x00000010)        /*!< Bit 4 */
#define  USART_GTPR_PSC_5                           ((uint32_t)0x00000020)        /*!< Bit 5 */
#define  USART_GTPR_PSC_6                           ((uint32_t)0x00000040)        /*!< Bit 6 */
#define  USART_GTPR_PSC_7                           ((uint32_t)0x00000080)        /*!< Bit 7 */

#define  USART_GTPR_GT                              ((uint32_t)0x0000FF00)        /*!< Guard time value */

/*******************  Bit definition for USART_CTLR4 register  *******************/
#define  USART_CTLR4_RTEN                           ((uint32_t)0x00000001)        /*!< Receiver timeout enable */
#define  USART_CTLR4_SCRTNUM                        ((uint32_t)0x0000000E)        /*!< SCRTNUM[2:0] bits (SmartCard Auto-Retry Count) */
#define  USART_CTLR4_SCRTNUM_0                      ((uint32_t)0x00000002)        /*!< Bit 0 */
#define  USART_CTLR4_SCRTNUM_1                      ((uint32_t)0x00000004)        /*!< Bit 1 */
#define  USART_CTLR4_SCRTNUM_2                      ((uint32_t)0x00000008)        /*!< Bit 2 */
#define  USART_CTLR4_RTIE                           ((uint32_t)0x00000010)        /*!< Interrupt enable bit of receive timeout flag */
#define  USART_CTLR4_EBIE                           ((uint32_t)0x00000020)        /*!< Interrupt enable bit of end of block flag */
#define  USART_CTLR4_RINV                           ((uint32_t)0x00000100)        /*!< RX pin level inversion */
#define  USART_CTLR4_TINV                           ((uint32_t)0x00000200)        /*!< TX pin level inversion */
#define  USART_CTLR4_DINV                           ((uint32_t)0x00000400)        /*!< Parity Control Enable */
#define  USART_CTLR4_MSBF                           ((uint32_t)0x00000800)        /*!< Most significant bit first */

/*******************  Bit definition for USART_RTR register  *****************/
#define  USART_RTR_RT                               ((uint32_t)0x00FFFFFF)        /*!< Receiver Time Out Value */
#define  USART_RTR_BL                               ((uint32_t)0xFF000000)        /*!< Block Length */

/*******************  Bit definition for USART_STR2 register  *******************/
#define  USART_STR2_RTF                             ((uint32_t)0x00000800)        /*!< Receiver timeout flag */
#define  USART_STR2_EBF                             ((uint32_t)0x00001000)        /*!< End of block flag */
#define  USART_STR2_BSY                             ((uint32_t)0x00010000)        /*!< Busy flag */

/******************************************************************************/
/*                                                                            */
/*                         Window WATCHDOG (WWDG)                             */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for WWDG_CTLR register  ********************/
#define  WWDG_CTLR_CNT                              ((uint8_t)0x7F)               /*!< CNT[6:0] bits (7-Bit counter (MSB to LSB)) */
#define  WWDG_CTLR_CNT0                             ((uint8_t)0x01)               /*!< Bit 0 */
#define  WWDG_CTLR_CNT1                             ((uint8_t)0x02)               /*!< Bit 1 */
#define  WWDG_CTLR_CNT2                             ((uint8_t)0x04)               /*!< Bit 2 */
#define  WWDG_CTLR_CNT3                             ((uint8_t)0x08)               /*!< Bit 3 */
#define  WWDG_CTLR_CNT4                             ((uint8_t)0x10)               /*!< Bit 4 */
#define  WWDG_CTLR_CNT5                             ((uint8_t)0x20)               /*!< Bit 5 */
#define  WWDG_CTLR_CNT6                             ((uint8_t)0x40)               /*!< Bit 6 */

#define  WWDG_CTLR_WDGEN                            ((uint8_t)0x80)               /*!< Activation bit */

/*******************  Bit definition for WWDG_CFR register  *******************/
#define  WWDG_CFR_WIN                               ((uint16_t)0x007F)            /*!< WIN[6:0] bits (7-bit window value) */
#define  WWDG_CFR_WIN0                              ((uint16_t)0x0001)            /*!< Bit 0 */
#define  WWDG_CFR_WIN1                              ((uint16_t)0x0002)            /*!< Bit 1 */
#define  WWDG_CFR_WIN2                              ((uint16_t)0x0004)            /*!< Bit 2 */
#define  WWDG_CFR_WIN3                              ((uint16_t)0x0008)            /*!< Bit 3 */
#define  WWDG_CFR_WIN4                              ((uint16_t)0x0010)            /*!< Bit 4 */
#define  WWDG_CFR_WIN5                              ((uint16_t)0x0020)            /*!< Bit 5 */
#define  WWDG_CFR_WIN6                              ((uint16_t)0x0040)            /*!< Bit 6 */

#define  WWDG_CFR_PS                                ((uint16_t)0x0180)            /*!< PS[1:0] bits (Timer Base) */
#define  WWDG_CFR_PS0                               ((uint16_t)0x0080)            /*!< Bit 0 */
#define  WWDG_CFR_PS1                               ((uint16_t)0x0100)            /*!< Bit 1 */

#define  WWDG_CFR_EWI                               ((uint16_t)0x0200)            /*!< Early Wakeup Interrupt */

/*******************  Bit definition for WWDG_STR register  ********************/
#define  WWDG_STR_EWIF                              ((uint8_t)0x01)               /*!< Early Wakeup Interrupt Flag */

    
/******************************************************************************/
/*                         Peripheral Registers_Bits_Definition               */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*                                    HAU                                    */
/*                                                                            */
/******************************************************************************/
/******************  Bits definition for HAU_CTLR register  ********************/
#define HAU_CTLR_START                              ((uint32_t)0x00000004)        /*!< Start message digest calculation */
#define HAU_CTLR_DMAE                               ((uint32_t)0x00000008)        /*!< DMA enable */ 
#define HAU_CTLR_DATAS                              ((uint32_t)0x00000030)        /*!< Data swapping selection */
#define HAU_CTLR_DATAS_0                            ((uint32_t)0x00000010)
#define HAU_CTLR_DATAS_1                            ((uint32_t)0x00000020)
#define HAU_CTLR_HMS                                ((uint32_t)0x00000040)        /*!< HMAC mode selection */
#define HAU_CTLR_ALGS                               ((uint32_t)0x00040080)        /*!< Algorithm selection */
#define HAU_CTLR_ALGS_0                             ((uint32_t)0x00000080)
#define HAU_CTLR_ALGS_1                             ((uint32_t)0x00040000)        
#define HAU_CTLR_NOW                                ((uint32_t)0x00000F00)        /*!< Number of words in IN FIFO */
#define HAU_CTLR_NOW_0                              ((uint32_t)0x00000100)
#define HAU_CTLR_NOW_1                              ((uint32_t)0x00000200)
#define HAU_CTLR_NOW_2                              ((uint32_t)0x00000400)
#define HAU_CTLR_NOW_3                              ((uint32_t)0x00000800)
#define HAU_CTLR_DINE                               ((uint32_t)0x00001000)        /*!< DI register is not empty */
#define HAU_CTLR_MDS                                ((uint32_t)0x00002000)        /*!< Multiple DMA Selection */
#define HAU_CTLR_LKS                                ((uint32_t)0x00010000)        /*!< HMAC Long key selection */

/******************  Bits definition for HAU_STR register  *******************/
#define HAU_STR_NOB                                 ((uint32_t)0x0000001F)        /*!< Number of valid bits in the last word of the message */
#define HAU_STR_NOB_0                               ((uint32_t)0x00000001)
#define HAU_STR_NOB_1                               ((uint32_t)0x00000002)
#define HAU_STR_NOB_2                               ((uint32_t)0x00000004)
#define HAU_STR_NOB_3                               ((uint32_t)0x00000008)
#define HAU_STR_NOB_4                               ((uint32_t)0x00000010)
#define HAU_STR_SCAL                                ((uint32_t)0x00000100)        /*!< Start message padding and digest calculation */

/******************  Bits definition for HAU_IER register  *******************/
#define HAU_IER_DIIE                                ((uint32_t)0x00000001)        /*!< Data input interrupt enable */
#define HAU_IER_CCIE                                ((uint32_t)0x00000002)        /*!< Calculation completion interrupt enable */

/******************  Bits definition for HAU_IFR register  ********************/
#define HAU_IFR_DIIF                                ((uint32_t)0x00000001)        /*!< Data input interrupt status flag */
#define HAU_IFR_CCIF                                ((uint32_t)0x00000002)        /*!< Calculation completion interrupt status flag */
#define HAU_IFR_DMAF                                ((uint32_t)0x00000004)        /*!< DMA Status Flag */
#define HAU_IFR_BSY                                 ((uint32_t)0x00000008)        /*!< Busy flag bit */

/******************************************************************************/
/*                                                                            */
/*                            CAUto Processor                                */
/*                                                                            */
/******************************************************************************/
/******************* Bits definition for CAU_CTLR register  ********************/
#define CAU_CTLR_ALGDIR                             ((uint32_t)0x00000004)        /*!< Algorithm direction */
#define CAU_CTLR_ALGS                               ((uint32_t)0x00080038)        /*!< Algorithm mode selection */
#define CAU_CTLR_ALGS_0                             ((uint32_t)0x00000008)
#define CAU_CTLR_ALGS_1                             ((uint32_t)0x00000010)
#define CAU_CTLR_ALGS_2                             ((uint32_t)0x00000020)
#define CAU_CTLR_ALGS_TDES_ECB                      ((uint32_t)0x00000000)        /*!< triple-DES Electronic codebook */
#define CAU_CTLR_ALGS_TDES_CBC                      ((uint32_t)0x00000008)        /*!< triple-DES Cipher block chaining */
#define CAU_CTLR_ALGS_DES_ECB                       ((uint32_t)0x00000010)        /*!< simple DES Electronic codebook */
#define CAU_CTLR_ALGS_DES_CBC                       ((uint32_t)0x00000018)        /*!< simple DES Cipher block chaining */
#define CAU_CTLR_ALGS_AES_ECB                       ((uint32_t)0x00000020)        /*!< AES Electronic codebook */
#define CAU_CTLR_ALGS_AES_CBC                       ((uint32_t)0x00000028)        /*!< AES Cipher block chaining */
#define CAU_CTLR_ALGS_AES_CTR                       ((uint32_t)0x00000030)        /*!< AES counter mode */
#define CAU_CTLR_ALGS_AES_KEY                       ((uint32_t)0x00000038)        /*!< AES decryption key preparation mode */

#define CAU_CTLR_DATAS                              ((uint32_t)0x000000C0)        /*!< Data swapping selection */
#define CAU_CTLR_DATAS_0                            ((uint32_t)0x00000040)
#define CAU_CTLR_DATAS_1                            ((uint32_t)0x00000080)
#define CAU_CTLR_KEYLEN                             ((uint32_t)0x00000300)        /*!< Key length selection when AES mode */
#define CAU_CTLR_KEYLEN_0                           ((uint32_t)0x00000100)
#define CAU_CTLR_KEYLEN_1                           ((uint32_t)0x00000200)
#define CAU_CTLR_FLUSH                              ((uint32_t)0x00004000)        /*!< FIFO flush */
#define CAU_CTLR_CAUEN                              ((uint32_t)0x00008000)        /*!< Cryptographic module enable */

/****************** Bits definition for CAU_SFR register  *********************/
#define CAU_SFR_IFEF                                ((uint32_t)0x00000001)        /*!< IN FIFO empty flag */
#define CAU_SFR_IFNFF                               ((uint32_t)0x00000002)        /*!< IN FIFO not full flag */
#define CAU_SFR_OFNEF                               ((uint32_t)0x00000004)        /*!< OUT FIFO not empty flag */
#define CAU_SFR_OFFF                                ((uint32_t)0x00000008)        /*!< OUT FIFO full flag */
#define CAU_SFR_BSY                                 ((uint32_t)0x00000010)        /*!< Busy flag bit */
/****************** Bits definition for CAU_DCTLR register  ******************/
#define CAU_DCTLR_DIE                               ((uint32_t)0x00000001)        /*!< IN FIFO DMA enable */
#define CAU_DCTLR_DOE                               ((uint32_t)0x00000002)        /*!< OUT FIFO DMA enable */
/*****************  Bits definition for CAU_IER register  ******************/
#define CAU_IER_INIE                                ((uint32_t)0x00000001)        /*!< IN FIFO interrupt enable */
#define CAU_IER_OUTIE                               ((uint32_t)0x00000002)        /*!< OUT FIFO interrupt enable */
/****************** Bits definition for CAU_IFR register  *******************/
#define CAU_IFR_INIF                                ((uint32_t)0x00000001)        /*!< IN FIFO interrupt flag */
#define CAU_IFR_OUTIF                               ((uint32_t)0x00000002)        /*!< OUT FIFO interrupt flag */
/****************** Bits definition for CAU_EIFR register  *******************/
#define CAU_EIFR_INEIF                              ((uint32_t)0x00000001)        /*!< IN FIFO enabled interrupt flag */
#define CAU_EIFR_OUTEIF                             ((uint32_t)0x00000002)        /*!< OUT FIFO enabled interrupt flag */


/******************************************************************************/
/*                                                                            */
/*                                    RNG                                     */
/*                                                                            */
/******************************************************************************/
/********************  Bits definition for RNG_CTLR register  *******************/
#define RNG_CTLR_RNGEN                              ((uint32_t)0x00000004)        /*!< RNG enable bit */
#define RNG_CTLR_IE                                 ((uint32_t)0x00000008)        /*!< Interrupt enable bit */

/********************  Bits definition for RNG_STR register  *******************/
#define RNG_STR_DRDY                                ((uint32_t)0x00000001)        /*!< Random Data ready status bit */
#define RNG_STR_CECS                                ((uint32_t)0x00000002)        /*!< Clock error current status */
#define RNG_STR_SECS                                ((uint32_t)0x00000004)        /*!< Seed error current status */
#define RNG_STR_CEIS                                ((uint32_t)0x00000020)        /*!< Clock error interrupt status */
#define RNG_STR_SEIS                                ((uint32_t)0x00000040)        /*!< Seed error interrupt status */


/******************************************************************************/
/*                                                                            */
/*                                    DCI                                    */
/*                                                                            */
/******************************************************************************/
/********************  Bits definition for DCI_CR register  ******************/
#define DCI_CTLR_CAP                                ((uint32_t)0x00000001)        /*!< Capture Enable */
#define DCI_CTLR_SNAP                               ((uint32_t)0x00000002)        /*!< Snapshot Mode */
#define DCI_CTLR_WDEN                               ((uint32_t)0x00000004)        /*!< Window Enable */
#define DCI_CTLR_JM                                 ((uint32_t)0x00000008)        /*!< JPEG Mode */
#define DCI_CTLR_ESM                                ((uint32_t)0x00000010)        /*!< Embedded Synchronous Mode */
#define DCI_CTLR_CKPOL                              ((uint32_t)0x00000020)        /*!< Clock Polarity */
#define DCI_CTLR_HPOL                               ((uint32_t)0x00000040)        /*!< Hsync Polarity */
#define DCI_CTLR_VPOL                               ((uint32_t)0x00000080)        /*!< Vsync Polarity */
#define DCI_CTLR_FR_0                               ((uint32_t)0x00000100)        /*!< Frame Rate */
#define DCI_CTLR_FR_1                               ((uint32_t)0x00000200)        
#define DCI_CTLR_DCIF_0                             ((uint32_t)0x00000400)        /*!< Digital Camera Interface Format */
#define DCI_CTLR_DCIF_1                             ((uint32_t)0x00000800)
#define DCI_CTLR_DCIEN                              ((uint32_t)0x00004000)

/********************  Bits definition for DCI_SR register  ******************/
#define DCI_STR_HS                                  ((uint32_t)0x00000001)        /*!< HS line status */
#define DCI_STR_VS                                  ((uint32_t)0x00000002)        /*!< VS line status */
#define DCI_STR_FV                                  ((uint32_t)0x00000004)        /*!< FIFO Valid */

/********************  Bits definition for DCI_FIR register  ****************/
#define DCI_FIR_ESFF                                ((uint32_t)0x00000001)        /*!< Embedded Synchronous Error Flag */
#define DCI_FIR_VSF                                 ((uint32_t)0x00000002)        /*!< Vsync Flag */
#define DCI_FIR_OVRF                                ((uint32_t)0x00000004)        /*!< FIFO Overrun Flag */
#define DCI_FIR_EFF                                 ((uint32_t)0x00000008)        /*!< End of Frame Flag */
#define DCI_FIR_ELF                                 ((uint32_t)0x00000010)        /*!< End of Line Flag */

/********************  Bits definition for DCI_IMR register  *****************/
#define DCI_IMR_ESEM                                ((uint32_t)0x00000001)        /*!< Embedded Synchronous Error Interrupt Mask */
#define DCI_IMR_VSM                                 ((uint32_t)0x00000002)        /*!< Vsync Interrupt Mask */
#define DCI_IMR_OVRM                                ((uint32_t)0x00000004)        /*!< FIFO Overrun Interrupt Mask */
#define DCI_IMR_EFM                                 ((uint32_t)0x00000008)        /*!< End of Frame Interrupt Mask */
#define DCI_IMR_ELM                                 ((uint32_t)0x00000010)        /*!< End of Line Interrupt Mask */

/********************  Bits definition for DCI_MFR register  ****************/
#define DCI_MFR_ESEMF                               ((uint32_t)0x00000001)        /*!< Masked Embedded Synchronous Error Flag */
#define DCI_MFR_VSMF                                ((uint32_t)0x00000002)        /*!< Masked Vsync Flag */
#define DCI_MFR_OVRMF                               ((uint32_t)0x00000004)        /*!< Masked FIFO Overrun Flag */
#define DCI_MFR_EFMF                                ((uint32_t)0x00000008)        /*!< Masked End of Frame Flag */
#define DCI_MFR_ELMF                                ((uint32_t)0x00000010)        /*!< Masked End of Line Flag */

/********************  Bits definition for DCI_CFR register  *****************/
#define DCI_CFR_CESEF                               ((uint32_t)0x00000001)        /*!< Clear Embedded Synchronous Error Flag */
#define DCI_CFR_CVSF                                ((uint32_t)0x00000002)        /*!< Clear Vsync Flag */
#define DCI_CFR_COVRF                               ((uint32_t)0x00000004)        /*!< Clear FIFO Overrun Flag */
#define DCI_CFR_CEFF                                ((uint32_t)0x00000008)        /*!< Clear End of Frame Flag */
#define DCI_CFR_CELF                                ((uint32_t)0x00000010)        /*!< Clear End of Line Flag */
/********************  Bits definition for DCI_ECR register  *****************/
#define DCI_ECR_FS                                  ((uint32_t)0x000000FF)        /*!< Frame Start Code in Embedded Synchronous Mode */
#define DCI_ECR_LS                                  ((uint32_t)0x0000FF00)        /*!< Line Start Code in Embedded Synchronous Mode */
#define DCI_ECR_LE                                  ((uint32_t)0x00FF0000)        /*!< Line End Code in Embedded Synchronous Mode */
#define DCI_ECR_FE                                  ((uint32_t)0xFF000000)        /*!< Frame End Code in Embedded Synchronous Mode */
/********************  Bits definition for DCI_ECMR register  *****************/
#define DCI_ECMR_FSM                                ((uint32_t)0x000000FF)        /*!< Frame Start Code Mask Bits in Embedded Synchronous Mode */
#define DCI_ECMR_LSM                                ((uint32_t)0x0000FF00)        /*!< Line Start Code Mask Bits in Embedded Synchronous Mode */
#define DCI_ECMR_LEM                                ((uint32_t)0x00FF0000)        /*!< Line End Code Mask Bits in Embedded Synchronous Mode */
#define DCI_ECMR_FEM                                ((uint32_t)0xFF000000)        /*!< Frame End Code Mask Bits in Embedded Synchronous Mode */
/********************  Bits definition for DCI_WSPR register  *****************/
#define DCI_WSPR_WHPS                               ((uint32_t)0x00003FFF)        /*!< Window Horizontal Start Position */
#define DCI_WSPR_WVPS                               ((uint32_t)0x1FFF0000)        /*!< Window Vertical Start Position */
/********************  Bits definition for DCI_WSZR register  *****************/
#define DCI_WSZR_WHSZ                               ((uint32_t)0x00003FFF)        /*!< Window Horizontal Size */
#define DCI_WSZR_WVSZ                               ((uint32_t)0x3FFF0000)        /*!< Window Vertical Size */
/********************  Bits definition for DCI_PBR register  *****************/
#define DCI_PBR_PB1                                 ((uint32_t)0x000000FF)        /*!< Pixel Byte 1 */
#define DCI_PBR_PB2                                 ((uint32_t)0x0000FF00)        /*!< Pixel Byte 2 */
#define DCI_PBR_PB3                                 ((uint32_t)0x00FF0000)        /*!< Pixel Byte 3 */
#define DCI_PBR_PB4                                 ((uint32_t)0xFF000000)        /*!< Pixel Byte 4 */
/******************************************************************************/
/*                                                                            */
/*                      TFT(LCD) Display Interface  (TLDI)                     */
/*                                                                            */
/******************************************************************************/

/********************  Bit definition for TLDI_PSCR register  *****************/

#define TLDI_PSCR_VPS                               ((uint32_t)0x00000FFF)        /*!< Vertical Synchronization Height */
#define TLDI_PSCR_HPS                               ((uint32_t)0x0FFF0000)        /*!< Horizontal Synchronization Width */

/********************  Bit definition for TLDI_BPSCR register  *****************/

#define TLDI_BPSCR_VBPS                             ((uint32_t)0x00000FFF)        /*!< Accumulated Vertical Back Porch */
#define TLDI_BPSCR_HBPS                             ((uint32_t)0x0FFF0000)        /*!< Accumulated Horizontal Back Porch */
									         
/********************  Bit definition for TLDI_ASCR register  *****************/

#define TLDI_ASCR_VAS                               ((uint32_t)0x00000FFF)        /*!< Accumulated Active heigh */
#define TLDI_ASCR_HAS                               ((uint32_t)0x0FFF0000)        /*!< Accumulated Active Width */

/********************  Bit definition for TLDI_TSCR register  *****************/

#define TLDI_TSCR_VTS                               ((uint32_t)0x00000FFF)        /*!< Total Heigh */
#define TLDI_TSCR_HTS                               ((uint32_t)0x0FFF0000)        /*!< Total Width */

/********************  Bit definition for TLDI_CTLR register  ******************/

#define TLDI_CTLR_TLDIEN                            ((uint32_t)0x00000001)        /*!< LCD-TFT controller enable bit */
#define TLDI_CTLR_BDB                               ((uint32_t)0x00000070)        /*!< Dither Blue Width */
#define TLDI_CTLR_GDB                               ((uint32_t)0x00000700)        /*!< Dither Green Width */
#define TLDI_CTLR_RDB                               ((uint32_t)0x00007000)        /*!< Dither Red Width */
#define TLDI_CTLR_DEN                               ((uint32_t)0x00010000)        /*!< Dither Enable */
#define TLDI_CTLR_CLKPS                             ((uint32_t)0x10000000)        /*!< Pixel Clock Polarity */
#define TLDI_CTLR_DEPS                              ((uint32_t)0x20000000)        /*!< Data Enable Polarity */
#define TLDI_CTLR_VSPS                              ((uint32_t)0x40000000)        /*!< Vertical Synchronization Polarity */
#define TLDI_CTLR_HSPS                              ((uint32_t)0x80000000)        /*!< Horizontal Synchronization Polarity */

/********************  Bit definition for TLDI_RLCR register  *****************/

#define TLDI_RLCR_RQR                               ((uint32_t)0x00000001)        /*!< Immediate Reload */
#define TLDI_RLCR_FBR                               ((uint32_t)0x00000002)        /*!< Vertical Blanking Reload */

/********************  Bit definition for TLDI_BVCR register  *****************/

#define TLDI_BVCR_BVB                               ((uint32_t)0x000000FF)        /*!< Background Blue value */
#define TLDI_BVCR_BVG                               ((uint32_t)0x0000FF00)        /*!< Background Green value */
#define TLDI_BVCR_BVR                               ((uint32_t)0x00FF0000)        /*!< Background Red value */

/********************  Bit definition for TLDI_IMR register  ******************/

#define TLDI_IMR_CRIM                               ((uint32_t)0x00000001)        /*!< Line Interrupt Enable */
#define TLDI_IMR_FEIM                               ((uint32_t)0x00000002)        /*!< FIFO Underrun Interrupt Enable */
#define TLDI_IMR_TEIM                               ((uint32_t)0x00000004)        /*!< Transfer Error Interrupt Enable */
#define TLDI_IMR_LRIM                               ((uint32_t)0x00000008)        /*!< Register Reload interrupt enable */

/********************  Bit definition for TLDI_STR register  ******************/

#define TLDI_STR_CLR                                ((uint32_t)0x00000001)        /*!< Line Interrupt Flag */
#define TLDI_STR_FE                                 ((uint32_t)0x00000002)        /*!< FIFO Underrun Interrupt Flag */
#define TLDI_STR_TE                                 ((uint32_t)0x00000004)        /*!< Transfer Error Interrupt Flag */
#define TLDI_STR_LR                                 ((uint32_t)0x00000008)        /*!< Register Reload interrupt Flag */

/********************  Bit definition for TLDI_CFR register  ******************/

#define TLDI_CFR_CLCR                               ((uint32_t)0x00000001)        /*!< Clears the Line Interrupt Flag */
#define TLDI_CFR_CFE                                ((uint32_t)0x00000002)        /*!< Clears the FIFO Underrun Interrupt Flag */
#define TLDI_CFR_CTE                                ((uint32_t)0x00000004)        /*!< Clears the Transfer Error Interrupt Flag */
#define TLDI_CFR_CLR                                ((uint32_t)0x00000008)        /*!< Clears Register Reload interrupt Flag */

/********************  Bit definition for TLDI_LIPCR register  ****************/

#define TLDI_LRFR_LRPOS                             ((uint32_t)0x000007FF)        /*!< Line Interrupt Position */

/********************  Bit definition for TLDI_CPPR register  *****************/

#define TLDI_CPPR_VPOS                              ((uint32_t)0x0000FFFF)        /*!< Current Y Position */
#define TLDI_CPPR_HPOS                              ((uint32_t)0xFFFF0000)        /*!< Current X Position */

/********************  Bit definition for TLDI_CISR register  *****************/

#define TLDI_CISR_VDE                               ((uint32_t)0x00000001)        /*!< Vertical Data Enable Status */
#define TLDI_CISR_HDE                               ((uint32_t)0x00000002)        /*!< Horizontal Data Enable Status */
#define TLDI_CISR_VS                                ((uint32_t)0x00000004)        /*!< Vertical Synchronization Status */
#define TLDI_CISR_HS                                ((uint32_t)0x00000008)        /*!< Horizontal Synchronization Status */

/********************  Bit definition for TLDI_LxCR register  *****************/

#define TLDI_LxCTLR_LEN                             ((uint32_t)0x00000001)        /*!< Layer Enable */
#define TLDI_LxCTLR_COKYEN                          ((uint32_t)0x00000002)        /*!< Color Keying Enable */
#define TLDI_LxCTLR_LUTEN                           ((uint32_t)0x00000010)        /*!< Color Lockup Table Enable */

/********************  Bit definition for TLDI_LxHWCR register  **************/

#define TLDI_LxHWCR_WRP                             ((uint32_t)0x00000FFF)        /*!< Window Horizontal Start Position */
#define TLDI_LxHWCR_WLP                             ((uint32_t)0x0FFF0000)        /*!< Window Horizontal Stop Position */

/********************  Bit definition for TLDI_LxWVPCR register  **************/

#define TLDI_LxVWCR_WBP                             ((uint32_t)0x000007FF)        /*!< Window Vertical Start Position */
#define TLDI_LxVWCR_WTP                             ((uint32_t)0x07FF0000)        /*!< Window Vertical Stop Position */

/********************  Bit definition for TLDI_LxCKVR register  ***************/

#define TLDI_LxCKVR_CKB                             ((uint32_t)0x000000FF)        /*!< Color Key Blue value */
#define TLDI_LxCKVR_CKG                             ((uint32_t)0x0000FF00)        /*!< Color Key Green value */
#define TLDI_LxCKVR_CKR                             ((uint32_t)0x00FF0000)        /*!< Color Key Red value */

/********************  Bit definition for TLDI_LxPFR register  ***************/

#define TLDI_LxPFR_PF                               ((uint32_t)0x00000007)        /*!< Pixel Format */

/********************  Bit definition for TLDI_LxSAR register  ***************/

#define TLDI_LxSAR_SALPHA                           ((uint32_t)0x000000FF)        /*!< Constant Alpha */

/********************  Bit definition for TLDI_LxDCR register  ***************/

#define TLDI_LxDCR_DCB                              ((uint32_t)0x000000FF)        /*!< Default Color Blue */
#define TLDI_LxDCR_DCG                              ((uint32_t)0x0000FF00)        /*!< Default Color Green */
#define TLDI_LxDCR_DCR                              ((uint32_t)0x00FF0000)        /*!< Default Color Red */
#define TLDI_LxDCR_DCA                              ((uint32_t)0xFF000000)        /*!< Default Color Alpha */
                                
/********************  Bit definition for TLDI_LxBMR register  ***************/

#define TLDI_LxBMR_ACM2                             ((uint32_t)0x00000007)        /*!< Blending Factor 2 */
#define TLDI_LxBMR_ACM1                             ((uint32_t)0x00000700)        /*!< Blending Factor 1 */

/********************  Bit definition for TLDI_LxFBSAR register  **************/

#define TLDI_LxFBSAR_FBSA                           ((uint32_t)0xFFFFFFFF)        /*!< Color Frame Buffer Start Address */

/********************  Bit definition for TLDI_LxFBLR register  **************/

#define TLDI_LxFBLR_FBL                             ((uint32_t)0x00001FFF)        /*!< Color Frame Buffer Line Length */
#define TLDI_LxFBLR_FBOFFSET                        ((uint32_t)0x1FFF0000)        /*!< Color Frame Buffer Pitch in bytes */

/********************  Bit definition for TLDI_LxFBLNR register  *************/

#define TLDI_LxFBLNR_FBLN                           ((uint32_t)0x000007FF)        /*!< Frame Buffer Line Number */

/********************  Bit definition for TLDI_LxLUTWR register  *************/

#define TLDI_LxLUTWR_BLUE                           ((uint32_t)0x000000FF)        /*!< Blue value */
#define TLDI_LxLUTWR_GREEN                          ((uint32_t)0x0000FF00)        /*!< Green value */
#define TLDI_LxLUTWR_RED                            ((uint32_t)0x00FF0000)        /*!< Red value */
#define TLDI_LxLUTWR_ADDRESS                        ((uint32_t)0xFF000000)        /*!< CLUT address */


/**
  * @}
  */

 /**
  * @}
  */ 

#ifdef USE_STDPERIPH_DRIVER
#include "gd32f20x_conf.h"
#endif

/** @addtogroup Exported_macro
  * @{
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __GD32F20X_H */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2015 GIGADEVICE *****END OF FILE****/
