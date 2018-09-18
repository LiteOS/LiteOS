
/****************************************************************************************************//**
 * @file    FM33A0XX.h
 *
 * @brief    CMSIS Cortex-M0 Peripheral Access Layer Header File for
 *           FM33A0XX from Keil.
 *
 * @version  V1.1
 * @date     30-11-2017
 *
 * @note     Generated with SVDConv V2.87e 
 *           from CMSIS SVD File 'FM32L0XX.SVD' Version 1.0,
 *
 * @par      ARM Limited (ARM) is supplying this software for use with Cortex-M
 *           processor based microcontroller, but can be equally used for other
 *           suitable processor architectures. This file can be freely distributed.
 *           Modifications to this file shall be clearly marked.
 *           
 *           THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 *           OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 *           MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *           ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 *           CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER. 
 *
 *******************************************************************************************************/



/** @addtogroup Keil
  * @{
  */

/** @addtogroup FM33A0XX
  * @{
  */

#ifndef FM33A0XX_H
#define FM33A0XX_H

#ifdef __cplusplus
extern "C" {
#endif

    

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus, FlagState, IFState;
typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState, FunState;
typedef enum {FAIL = 0, PASS = !FAIL} ErrorStatus, ResState;
typedef enum {
	SCU_num = 0,	PMU_num	=	1,	RTC_num = 2,	LPTRC_num = 3,	CRC_num = 4,	TRNG_num = 5,
	ADC_num = 6,	LCD_num = 7,	DMA_num = 8,	FLS_num = 9,	AES_num = 10,	SPI1_num = 11,
	SPI2_num = 12,	HSPI_num = 13,	UART0_num = 14,	UART1_num = 15,	UART2_num = 16,
	UART3_num = 17,	UART4_num = 18,	UART5_num = 19,	U78160_num = 20,	U78161_num = 21,	
	I2C_num = 22,	BT1_num = 23,	BT2_num = 24,	ET1_num = 25,	ET2_num = 26,	ET3_num = 27,
	ET4_num = 28,		LPTFC_num = 29,	IWDT_num = 30,	PDC_num = 31,	ANA_num = 32,
	EXTI0_num = 33, EXTI1_num = 34, EXTI2_num = 35, DCU_num = 36, RAMBIST_num = 37,
	WWDT_num = 38, UART_COM_num = 39,
	
}Periph_Type;

#define __RCHF_INITIAL_CLOCK    (8000000)		/* Value of the Internal RC HIGH oscillator in Hz */
#define __RCLP_CLOCK    				(32000)			/* Value of the Internal RC LOW oscillator in Hz */
#define __XTLF_CLOCK    				(32768)			/* Value of the EXTERNAL oscillator in Hz */



/* -------------------------  Interrupt Number Definition  ------------------------ */

typedef enum {
/* -------------------  Cortex-M0 Processor Exceptions Numbers  ------------------- */
  Reset_IRQn                    =  -15,             /*!<   1  Reset Vector, invoked on Power up and warm reset                 */
  NonMaskableInt_IRQn           =  -14,             /*!<   2  Non maskable Interrupt, cannot be stopped or preempted           */
  HardFault_IRQn                =  -13,             /*!<   3  Hard Fault, all classes of Fault                                 */
  SVCall_IRQn                   =  -5,              /*!<  11  System Service Call via SVC instruction                          */
  PendSV_IRQn                   =  -2,              /*!<  14  Pendable request for system service                              */
  SysTick_IRQn                  =  -1,              /*!<  15  System Tick Timer                                                */
/* ---------------------  FM32L0XX Specific Interrupt Numbers  -------------------- */
  WWDT_IRQn                   	=  0,               /*!<  0   WWDT             */
  SVD_IRQn               		=  1,               /*!<  1   SVD_IRQn          */
  RTC_IRQn               		=  2,               /*!<  2    RTC_IRQn         */
  NVMIF_IRQn                  	=  3,               /*!<  3   NVMIF_IRQn          */ 
  FDET_IRQn                 	=  4,               /*!<  4   FDET_IRQn           */
  ADC_IRQn                  	=  5,               /*!<  5   ADC_IRQn             */
  HSPI_IRQn                     =  6,               /*!<  6   HSPI_IRQn             */
  SPI1_IRQn                     =  7,               /*!<  7   SPI1_IRQn             */
  SPI2_IRQn                     =  8,               /*!<  8   SPI2_IRQn            */
  UART0_IRQn                 	=  9,               /*!<  9   UART0_IRQn             */
  UART1_IRQn                    =  10,              /*!<  10   UART1_IRQn            */
  UART2_IRQn                    =  11,              /*!<  11   UART2_IRQn            */
  UART3_IRQn                    =  12,              /*!<  12   UART3_IRQn	          */
  UART4_IRQn                    =  13,              /*!<  13   UART4_IRQn                  */ 
  UART5_IRQn                    =  14,              /*!<  14   UART5_IRQn          */
  U78160_IRQn                   =  15,              /*!<  15   U78160_IRQn          */
  U78161_IRQn                   =  16,              /*!<  16   U78161_IRQn	          */
  I2C_IRQn                      =  17,              /*!<  17   I2C_IRQn           */
  LCD_IRQn                      =  18,              /*!<  18   LCD_IRQn 	          */
  AES_IRQn                      =  19,              /*!<  19   AES_IRQn            */ 
  LPTIM_IRQn                    =  20,              /*!<  20   LPTIM_IRQn          */
  DMA_IRQn                 		=  21,              /*!<  21   DMA_IRQn           */
  TRNG_IRQn		                =  22,              /*!<  22   TRNG_IRQnSPI2_IRQn            */
  COMP_IRQn 		            =  23,              /*!<  23   COMP_IRQn            */
  BTIM1_IRQn                 	=  24,              /*!<  24    BTIMER1_IRQn            */
  BTIM2_IRQn 		            =  25,              /*!<  25    BTIMER2_IRQn             */
  ETIM1_IRQn                    =  26,              /*!<  26    ETIMER1_IRQn          */
  ETIM2_IRQn                    =  27,              /*!<  27     ETIMER2_IRQn         */
  ETIM3_IRQn                    =  28,              /*!<  28   ETIMER3_IRQn             */
  ETIM4_IRQn		            =  29,              /*!<  29   ETIMER4_IRQn             */
  GPIO_IRQn                  	=  30,              /*!<  30   GPIO_IRQn                 */
	
} IRQn_Type;


/** @addtogroup Configuration_of_CMSIS
  * @{
  */


/* ================================================================================ */
/* ================      Processor and Core Peripheral Section     ================ */
/* ================================================================================ */

/* ----------------Configuration of the Cortex-M0 Processor and Core Peripherals---------------- */
#define __CM0_REV					0x0100			/*!< Cortex-M0 Core Revision                                               */
#define __MPU_PRESENT				1				/*!< MPU present or not                                                    */
#define __VTOR_PRESENT				1				/*!< VTOR present or not                                                   */
#define __NVIC_PRIO_BITS			2				/*!< Number of Bits used for Priority Levels                               */
#define __Vendor_SysTickConfig		0				/*!< Set to 1 if different SysTick Config is used                          */
/** @} */ /* End of group Configuration_of_CMSIS */

#include "core_cm0plus.h"                           /*!< Cortex-M0 processor and core peripherals                           */
#include "system_FM33A0XX.h"                        /*!< FM33A0XX System                                                     */

/* ================================================================================ */
/* ================       Device Specific Peripheral Section       ================ */
/* ================================================================================ */


/** @addtogroup Device_Peripheral_Registers
  * @{
  */


/* -------------------  Start of section using anonymous unions  ------------------ */
#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__ICCARM__)
  #pragma language=extended
#elif defined(__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined(__TMS470__)
/* anonymous unions are enabled by default */
#elif defined(__TASKING__)
  #pragma warning 586
#else
  #warning Not supported compiler type
#endif



/* ================================================================================ */
/* ================                       SCU                      ================ */
/* ================================================================================ */
typedef struct
{
	__I uint32_t SYSCON; 							    /*!<  SYSTEM STAT */
	__IO uint32_t MCUDBGCR;   							/*!<  DEBUG CONFIG */
	__IO uint32_t HDFFLAG;  							/*!<  HARDFAULT FLAG */
	
}SCU_Type;


/* ================================================================================ */
/* ================                       PMU                      ================ */
/* ================================================================================ */
typedef struct
{
	__IO uint32_t LPMCFG; 							  		/*!<  LOW POWER CONFIG */
	__IO uint32_t WKDLYCON;   								/*!<  WAKEUP DELAY CONFIG */
	__IO uint32_t WKPFLAG;  								/*!<  WKPFLAG FLAG */
	__IO uint32_t LPREIE;									/*!<  LPREIE INTERRUPT ENABLE*/
	__IO uint32_t LPREIF;									/*!<  LPREIF INTERRUPT FLAG*/
}PMU_Type;

/* ================================================================================ */
/* ================                       uart                     ================ */
/* ================================================================================ */

/* memory mapping struct for uart IE */
typedef struct 
{
	__IO uint32_t 	UARTIE;                         /*!< UART interrupt enable */
	__IO uint32_t 	UARTIF;                         /*!< UART interrupt flag */
	__IO uint32_t	IRCON;													/*!< UART infrared control */
   			
} UART_common_Type;
	 
/* memory mapping struct for uart module */
typedef struct 
{
	__IO uint32_t RXSTA;                         /*!< UART receive stat Register*/
	__IO uint32_t TXSTA;                         /*!< UART send stat Register */
	__IO uint32_t RXREG;                         /*!< UART receive data Register *8 */
	__IO uint32_t TXREG;                         /*!< UART send data Register *8 */
	__IO uint32_t SPBRG;                         /*!< UART communication baud rate Register */
	__IO uint32_t TXBUFSTA;                      /*!< UART send stat flag Register */
	__IO uint32_t RXBUFSTA;                      /*!< UART receive stat flag Register */
	__IO uint32_t RTXCON;                        /*!< UART signal reverse conctol Register */
} UARTx_Type;

/* ================================================================================ */
/* ================                       RCC                      ================ */
/* ================================================================================ */
typedef struct
{
	__IO uint32_t RSTCFG;									/*!<  RESET CONFIG REGISTER*/
	__IO uint32_t SOFTRST;								/*!<  SOFTWARE RESET REGISTER*/
	__IO uint32_t RSTFLAG;								/*!<  RESET FLAG REGISTER */
	__IO uint32_t SYSCLKSEL; 							/*!<  SYSCLK SELECT */
	__IO uint32_t RCHFCON;   							/*!<  RCHF CONFIG   */
	__IO uint32_t RCHFTRIM;  							/*!<  RCHF TRIM  */
	__IO uint32_t PLLCON;  								/*!<  PLL CONFIG */
	__IO uint32_t RCLPCON;  							/*!<  RCLP CONFIG  */	
	__IO uint32_t RCLPTRIM;  							/*!<  RCLP TRIM  */
	__IO uint32_t XTLFIPW;  							/*!<  XTLF WORD STAT SELECT */
	__IO uint32_t PERCLKCON1;  							/*!<  PERRIPH RCC SELECT 1 */
	__IO uint32_t PERCLKCON2;  							/*!<  PERRIPH RCC SELECT 2 */
	__IO uint32_t PERCLKCON3;  							/*!<  PERRIPH RCC SELECT 3 */
	__IO uint32_t PERCLKCON4;  							/*!<  PERRIPH RCC SELECT 4 */
    __IO uint32_t RSV0;                      	 		/*!<  238	*/
    __IO uint32_t RSV1;                       			/*!<  23C	*/
    __IO uint32_t RSV2;                       			/*!<  240	*/
	__IO uint32_t MPRIL;  								/*!<  PERRIPH RCC SELECT 5 */
}RCC_Type;


/* ================================================================================ */
/* ================                       DMA                      ================ */
/* ================================================================================ */
typedef struct
{
	__IO uint32_t GLOBALCTRL; 							/*!<  GLOBAL EMANLE REGISTER */
	__IO uint32_t CH0CTRL;   							/*!<  CHANNEL0 CTRL REGISTER  */
	__IO uint32_t CH0RAMAD;  							/*!<  CHANNEL0 RAM START ADDR */
	__IO uint32_t CH1CTRL;  							/*!<  CHANNEL1 CTRL REGISTER */
	__IO uint32_t CH1RAMAD;  							/*!<  CHANNEL1 RAM START ADDR  */	
	__IO uint32_t CH2CTRL;  							/*!<  CHANNEL2 CTRL REGISTER */
	__IO uint32_t CH2RAMAD;  							/*!<  CHANNEL2 RAM START ADDR */
	__IO uint32_t CH3CTRL;  							/*!<  CHANNEL3 CTRL REGISTER */
	__IO uint32_t CH3RAMAD;  							/*!<  CHANNEL3 RAM START ADDR */
	__IO uint32_t CH4CTRL;  							/*!<  CHANNEL4 CTRL REGISTER */
	__IO uint32_t CH4RAMAD;  							/*!<  CHANNEL4 RAM START ADDR */
	__IO uint32_t CH5CTRL;  							/*!<  CHANNEL5 CTRL REGISTER */
	__IO uint32_t CH5RAMAD;  							/*!<  CHANNEL5 RAM START ADDR */
	__IO uint32_t CH6CTRL;  							/*!<  CHANNEL6 CTRL REGISTER */
	__IO uint32_t CH6RAMAD;  							/*!<  CHANNEL6 RAM START ADDR */
	__IO uint32_t CH7CTRL;  							/*!<  CHANNEL7 CTRL REGISTER */
	__IO uint32_t CH7FLSAD;  							/*!<  CHANNEL7 FLASH START ADDR */
	__IO uint32_t CH7RAMAD;  							/*!<  CHANNEL7 RAM START ADDR */
	__IO uint32_t CHSTATUS;  							/*!<  INTERRUPT FLAG REGISTER */
}DMA_Type;


/* ================================================================================ */
/* ================                       HSPI                     ================ */
/* ================================================================================ */
typedef struct
{
	__IO uint32_t SPICR1; 								/*!<  SPI CONTROL REGISTER 1 */
	__IO uint32_t SPICR2;   							/*!<  SPI CONTROL REGISTER 2 */
	__IO uint32_t SPICR3;  								/*!<  SPI CONTROL REGISTER 3 */
	__IO uint32_t SPIIE;  		  						/*!<  SPI INTERRUPT ENABLE REGISTER */
	__IO uint32_t SPIIF;  								/*!<  SPI STATE REGISTER */	
	__IO uint32_t SPITXBUF;  								/*!<  SPI TX REGISTER */
	__IO uint32_t SPIRXBUF;  								/*!<  SPI RX REGISTER */
}SPIx_Type;


/* ================================================================================ */
/* ================                       GPIO                     ================ */
/* ================================================================================ */
typedef struct
{
	__IO uint32_t INEN; 								/*!<  INPUT ENABLE CONFIG */
	__IO uint32_t PUEN;   								/*!<  PULLUP ENABLE CONFIG */
	__IO uint32_t ODEN;  								/*!<  OD ENABLE CONFIG */
	__IO uint32_t FCR;  								/*!<  FUNCTION CONFIG */
	__IO uint32_t DO;  									/*!<  output data register  */	
	__O uint32_t DSET;  								/*!<  bit set HIGH register  */
	__O uint32_t DRESET;  								/*!<  bit reset low register */
	__I uint32_t DIN;  									/*!<  input data register */
}GPIOx_Type;

typedef struct
{
	__IO uint32_t EXTI0_SEL; 							/*!<  EDGE TRIGER SELECT CONFIG 0,			Address offset: 0x00 */
	__IO uint32_t EXTI1_SEL;   							/*!<  EDGE TRIGER SELECT CONFIG 1,			Address offset: 0x04 */
	__IO uint32_t EXTI2_SEL;  							/*!<  EDGE TRIGER SELECT CONFIG 2,			Address offset: 0x08 */
	__IO uint32_t EXTI0IF;  							/*!<  triger flag 0,										Address offset: 0x0C */
	__IO uint32_t EXTI1IF;  							/*!<  triger flag 1,										Address offset: 0x10 */	
	__IO uint32_t EXTI2IF;  							/*!<  triger flag 2,										Address offset: 0x14 */
	__IO uint32_t FOUTSEL;  							/*!<  fout select config,								Address offset: 0x18 */
	__IO uint32_t HDSEL;  								/*!<  high driver config,								Address offset: 0x1C */
	__IO uint32_t ANASEL;  								/*!<  analog function select,						Address offset: 0x20 */
	__IO uint32_t IODF;									/*!<  input digit filter select,				Address offset: 0x24 */
	__IO uint32_t PINWKEN;								/*!<  pin wakeup enable,								Address offset: 0x28 */
}GPIO_COMMON_Type;


/* ================================================================================ */
/* ================                       IWDG                     ================ */
/* ================================================================================ */
typedef struct
{
	__IO uint32_t IWDTSERV;   		/*!< IWDG Key register,       Address offset: 0x00 */
	__IO uint32_t IWDTCFG;   		/*!< IWDG CONFIG register, 		Address offset: 0x04 */
	__I uint32_t 	IWDTCNT;  		/*!< IWDG Reload register,    Address offset: 0x08 */
} IWDT_Type;

/* ================================================================================ */
/* ================                      NVMIF                     ================ */
/* ================================================================================ */
typedef struct
{
	__IO uint32_t FLSRDCON;   	/*!< NVMIF read control register,       Address offset: 0x00 */
	__IO uint32_t PRFTCON;   	/*!< NVMIF prdfatch control register, 		Address offset: 0x04 */
	__I uint32_t 	OPTBR;  	/*!< NVMIF port protect stat register,    Address offset: 0x08 */
	__IO uint32_t ACLOCK1;		/*!< NVMIF flash block0-31 protect register,wr 1 only    Address offset: 0x0C */
	__IO uint32_t ACLOCK2;		/*!< NVMIF flash block32-63 protect register,wr 1 only    Address offset: 0x10 */
	__IO uint32_t EPCON;		/*!< NVMIF erase & program start register    Address offset: 0x14 */
	__IO uint32_t FLSKEY;		/*!< NVMIF flash key register    Address offset: 0x18 */
	__IO uint32_t FLSIE;		/*!< NVMIF flash IE register    Address offset: 0x1C */
	__IO uint32_t FLSIF;		/*!< NVMIF flash IF register    Address offset: 0x20 */
	__IO uint32_t RAMRPR0;		/*!< NVMIF RAM REPAIR0 register    Address offset: 0x24 */
	__IO uint32_t RAMRPR1;		/*!< NVMIF RAM REPAIR1 register    Address offset: 0x28 */
	__IO uint32_t RAMRPR2;		/*!< NVMIF RAM REPAIR2 register    Address offset: 0x2C */
	__IO uint32_t RAMRPR3;		/*!< NVMIF RAM REPAIR3 register    Address offset: 0x30 */	
} NVMIF_Type;

/* ================================================================================ */
/* ================                      RAMBIST                   ================ */
/* ================================================================================ */
typedef struct
{
	__IO uint32_t RAMBISTCON; 	/*!< RAMBIST control register,       	Address offset: 0x00 */
	__IO uint32_t RAMFF;   		/*!< RAMBIST error flag register, 		Address offset: 0x04 */
} RAMBIST_Type;

/* ================================================================================ */
/* ================                      BTIM                      ================ */
/* ================================================================================ */
typedef struct
{
	__IO uint32_t BTCR1;   				/*!< BTIMER CONTROL REGISTER 1,       	Address offset: 0x00 */
	__IO uint32_t BTCR2;   			  	/*!< BTIMER CONTROL REGISTER 2, 		    Address offset: 0x04 */
	__IO uint32_t BTCFG1;  		    	/*!< BTIMER CONFIG REGISTER 1,    			Address offset: 0x08 */
	__IO uint32_t BTCFG2;					/*!< BTIMER CONFIG REGISTER 2,          Address offset: 0x0C */
	__IO uint32_t BTPRES;					/*!< BTIMER PRESCALE VALUE,				    	Address offset: 0x10 */
	__IO uint32_t BTLOADCR;				/*!< BTIMER LOAD CONTROL REGISTER,	    Address offset: 0x14 */
	__IO uint32_t BTCNTL;					/*!< BTIMER COUNTER LOW BYTE,			    	Address offset: 0x18 */
	__IO uint32_t BTCNTH;					/*!< BTIMER COUNTER HIGH BYTE,  				Address offset: 0x1C */
	__IO uint32_t BTPRESET;				/*!< ETIMER PRESET VALUE,  							Address offset: 0x20 */
	__IO uint32_t BTLOADL;					/*!< ETIMER LOAD VALUE LOW BYTE,  			Address offset: 0x24 */
	__IO uint32_t BTLOADH;					/*!< ETIMER LOAD VALUE HIGH BYTE,  			Address offset: 0x28 */
	__IO uint32_t BTCMPL;					/*!< ETIMER CMP LOW,  									Address offset: 0x2C */
	__IO uint32_t BTCMPH;					/*!< ETIMER CMP HIGH,  									Address offset: 0x30 */
	__IO uint32_t BTOUTCNT;				/*!< ETIMER OUTPUT PULSE VALUE,  				Address offset: 0x34 */
	__IO uint32_t BTOCR;					/*!< ETIMER OUTPUT CONTROL REGISTER,    Address offset: 0x38 */
	__IO uint32_t BTIE;					/*!< ETIMER INTERRUPT ENABLE,  					Address offset: 0x3C */
	__IO uint32_t BTIF;					/*!< ETIMER INTERRUPT FLAG & STATUS,  	Address offset: 0x40 */
} BTIMx_Type;

/* ================================================================================ */
/* ================                      ETIM                      ================ */
/* ================================================================================ */
typedef struct
{
	__IO uint32_t ETxCR;   					/*!< ETIMER CONTROL REGISTER,       	Address offset: 0x00 */
	__IO uint32_t ETxINSEL;   				/*!< ETIMER INPUT SELECT REGISTER, 		Address offset: 0x04 */
	__IO uint32_t ETxPESCALE1;  			/*!< ETIMER PRESCALE REGISTER 1,    	Address offset: 0x08 */
	__IO uint32_t ETxPESCALE2;				/*!< ETIMER PRESCALE REGISTER 2,      Address offset: 0x0C */
	__IO uint32_t ETxIVR;					/*!< ETIMER INITIAL VALUE,				    Address offset: 0x10 */
	__IO uint32_t ETxCMP;					/*!< ETIMER CMP REGISTER,					    Address offset: 0x14 */
	__IO uint32_t ETxIE;					/*!< ETIMER INTERRUPT ENABLE,			    Address offset: 0x18 */
	__IO uint32_t ETxIF;					/*!< ETIMER INTERRUPT FLAG & STATUS,  Address offset: 0x1C */
} ETIMx_Type;

/* ================================================================================ */
/* ================                      LPTIM                     ================ */
/* ================================================================================ */
typedef struct
{
	__IO uint32_t LPTCFG;   				/*!< LPTIM CONFIG REGISTER,       		Address offset: 0x00 */
	__IO uint32_t LPTCNT;   				/*!< LPTIM COUNT REGISTER, 						Address offset: 0x04 */
	__IO uint32_t LPTCMP;  					/*!< LPTIM COMPARE REGISTER,    			Address offset: 0x08 */
	__IO uint32_t LPTTARGET;				/*!< LPTIM TARGET REGISTER,      			Address offset: 0x0C */
	__IO uint32_t LPTIE;					/*!< LPTIM INTERRUPT ENABLE,				  Address offset: 0x10 */
	__IO uint32_t LPTIF;					/*!< LPTIM INTERRUPT FLAG REGISTER,	  Address offset: 0x14 */
	__IO uint32_t LPTCTRL;					/*!< LPTIM CONTROL REGISTER,			    Address offset: 0x18 */
} LPTIM_Type;


/* ================================================================================ */
/* ================                      I2C                       ================ */
/* ================================================================================ */
typedef struct
{
	__IO uint32_t I2CCTRL;   				/*!< I2C CONFIG REGISTER,       			Address offset: 0x00 */
	__IO uint32_t I2CSTA;   				/*!< I2C STATE REGISTER, 							Address offset: 0x04 */
	__IO uint32_t I2CBRG;  					/*!< I2C BAUD SETTING REGISTER,    		Address offset: 0x08 */
	__IO uint32_t I2CBUF;					/*!< I2C BUFF REGISTER,      					Address offset: 0x0C */
	__IO uint32_t I2CIR;					/*!< I2C INTERRUPT ENABLE & FLAG,	  	Address offset: 0x10 */
	__IO uint32_t I2CFSM;					/*!< I2C FSM STATE REGISTER,	 				Address offset: 0x14 */
	__IO uint32_t I2CERR;					/*!< I2C ERR FLAG REGISTER,			    	Address offset: 0x18 */
} I2C_Type;

/* ================================================================================ */
/* ================                      ANAC                      ================ */
/* ================================================================================ */
typedef struct
{
	__IO uint32_t PDRCON;   				/*!< ANAC PDR CONFIG REGISTER,       			Address offset: 0x00 */
	__IO uint32_t BORCON;   				/*!< ANAC BOR CONFIG REGISTER, 						Address offset: 0x04 */
	__IO uint32_t LDO15CON;  				/*!< ANAC LDO15CON CONFIG REGISTER,  			Address offset: 0x08 */
	__IO uint32_t VREFTRIM;					/*!< ANAC VREF TRIM REGISTER,      				Address offset: 0x0C */
	__IO uint32_t ULPRCON;					/*!< ANAC ULP LDO CONFIG REGISTER,	 			Address offset: 0x10 */
	__IO uint32_t SVDCFG;					/*!< ANAC SVD CONFIG REGISTER,	 					Address offset: 0x14 */
	__IO uint32_t SVDCON;					/*!< ANAC SVD CONTROL REGISTER,			 			Address offset: 0x18 */
	__IO uint32_t SVDSIF;					/*!< ANAC SVD INTERRUPT FLAG REGISTER,		Address offset: 0x1C */
	__IO uint32_t FDETIE;					/*!< ANAC FDET INTERRUPT ENABLE REGISTER,	Address offset: 0x20 */
	__IO uint32_t FDETIF;					/*!< ANAC FDET INTERRUPT FLAG REGISTER,		Address offset: 0x24 */
	__IO uint32_t ADCINSEL;					/*!< ANAC INPUT SELECT REGISTER,						Address offset: 0x28 */
	__IO uint32_t ADCCON;					/*!< ANAC ADC CONFIG REGISTER,						Address offset: 0x2C */
	__IO uint32_t ADCTRIM;					/*!< ANAC ADC TRIM REGISTER,							Address offset: 0x30 */
	__IO uint32_t ADCDATA;					/*!< ANAC ADC DATA REGISTER,							Address offset: 0x34 */
	__IO uint32_t ADCIF;					/*!< ANAC ADC INTERRUPT FLAG REGISTER,		Address offset: 0x38 */
	__IO uint32_t TRNGCON;					/*!< ANAC TRNF ENABLE REGISTER,						Address offset: 0x3C */
	__IO uint32_t COMP1CR;					/*!< ANAC COMP1 CONTROL REGISTER,					Address offset: 0x40 */
	__IO uint32_t COMP2CR;					/*!< ANAC COMP2 CONTROL REGISTER,					Address offset: 0x44 */
	__IO uint32_t COMPICR;					/*!< ANAC COMP12 INTERRUPT CONTROL REGISTER,		Address offset: 0x48 */
	__IO uint32_t COMPIF;					/*!< ANAC COMP INTERRUPT FLAG REGISTER,		Address offset: 0x4C */
	__IO uint32_t RSV0;
	__IO uint32_t RSV1;
	__IO uint32_t RSV2;	
	__IO uint32_t SVDALM;					/*!< ANAC SVDALM REGISTER,			Address offset: 0x5C */
} ANAC_Type;

/* ================================================================================ */
/* ================                      CRC                      ================ */
/* ================================================================================ */
typedef struct
{
	__IO uint32_t CRCDR;   					/*!< CRC DATA  REGISTER,       					Address offset: 0x00 */
	__IO uint32_t CRCCR;   					/*!< CRC CONTROL REGISTER, 							Address offset: 0x04 */
	__IO uint32_t CRCLFSR;  				/*!< CRC LFSR REGISTER,  								Address offset: 0x08 */
	__IO uint32_t CRCXOR;					/*!< CRC XOR DATA REGISTER,      				Address offset: 0x0C */
	__IO uint32_t CRCFLSEN;					/*!< CRC FLASH CRC VERIFY EN REGISTER,	Address offset: 0x10 */
	__IO uint32_t CRCFLSAD;					/*!< CRC FLASH ADRESS REGISTER,	 				Address offset: 0x14 */
	__IO uint32_t CRCFLSSIZE;				/*!< CRC VERIFY SIZE REGISTER,					Address offset: 0x18 */
} CRC_Type;


/* ================================================================================ */
/* ================                      DISP                      ================ */
/* ================================================================================ */
typedef struct
{
	__IO uint32_t DISPCTRL;   				/*!< DISP CONTROL REGISTER,    					Address offset: 0x00 */
	__IO uint32_t LCDTEST;   				/*!< DISP TEST CONTROL REGISTER, 				Address offset: 0x04 */
	__IO uint32_t DF;  						/*!< DISP DIVIDE CLK SETTING REGISTER,  Address offset: 0x08 */
	__IO uint32_t TON;						/*!< DISP ON SETTING REGISTER,      		Address offset: 0x0C */
	__IO uint32_t TOFF;						/*!< DISP OFF SETTING REGISTER,					Address offset: 0x10 */
	__IO uint32_t DISPIE;					/*!< DISP INTERRUPT CONTROL REGISTER,	 	Address offset: 0x14 */
	__IO uint32_t DISPIF;					/*!< DISP INTERRUPT FLAG REGISTER,			Address offset: 0x18 */
	__IO uint32_t LCDSET;					/*!< DISP SETTING REGISTER,							Address offset: 0x1C */	
	__IO uint32_t LCDDRV;					/*!< DISP DRV SETTING REGISTER,					Address offset: 0x20 */	
	__IO uint32_t DISPDATA0;				/*!< DISP DATA REGISTER 0,							Address offset: 0x24 */	
	__IO uint32_t DISPDATA1;				/*!< DISP DATA REGISTER 1,							Address offset: 0x28 */
	__IO uint32_t DISPDATA2;				/*!< DISP DATA REGISTER 2,							Address offset: 0x2C */
	__IO uint32_t DISPDATA3;				/*!< DISP DATA REGISTER 3,							Address offset: 0x30 */
	__IO uint32_t DISPDATA4;				/*!< DISP DATA REGISTER 4,							Address offset: 0x34 */
	__IO uint32_t DISPDATA5;				/*!< DISP DATA REGISTER 5,							Address offset: 0x38 */
	__IO uint32_t DISPDATA6;				/*!< DISP DATA REGISTER 6,							Address offset: 0x3C */
	__IO uint32_t DISPDATA7;				/*!< DISP DATA REGISTER 7,							Address offset: 0x40 */
	__IO uint32_t DISPDATA8;				/*!< DISP DATA REGISTER 8,							Address offset: 0x44 */
	__IO uint32_t DISPDATA9;				/*!< DISP DATA REGISTER 9,							Address offset: 0x48 */
	__IO uint32_t LCDBIAS;					/*!< DISP BIAS SETTING REGISTER,				Address offset: 0x4C */
	__IO uint32_t COM_EN;					/*!< DISP COM SETTING REGISTER,					Address offset: 0x50 */
	__IO uint32_t SEG_EN0;					/*!< DISP SEG SETTING REGISTER,					Address offset: 0x54 */
	__IO uint32_t SEG_EN1;					/*!< DISP SEG SETTING REGISTER,					Address offset: 0x58 */
	__IO uint32_t LCDBSTCON;				/*!< DISP BOOST CONFIG REGISTER,				Address offset: 0x5C */
	
} DISP_Type;

/* ================================================================================ */
/* ================                      RTC                      ================ */
/* ================================================================================ */
typedef struct
{
	__IO uint32_t RTCWE;   					/*!< RTC WRITE PROTECT REGISTER,    		Address offset: 0x00 */
	__IO uint32_t RTCIE;   					/*!< RTC INTERRUPT CONTROL REGISTER, 		Address offset: 0x04 */
	__IO uint32_t RTCIF;  					/*!< RTC INTERRUPT FLAG REGISTER,  			Address offset: 0x08 */
	__IO uint32_t BCDSEC;					/*!< RTC SECOND IN BCD REGISTER,      	Address offset: 0x0C */
	__IO uint32_t BCDMIN;					/*!< RTC MINITE IN BCD REGISTER,				Address offset: 0x10 */
	__IO uint32_t BCDHOUR;					/*!< RTC HOUR IN BCD REGISTER,	 				Address offset: 0x14 */
	__IO uint32_t BCDDATE;					/*!< RTC DATE IN BCD REGISTER,					Address offset: 0x18 */
	__IO uint32_t BCDWEEK;					/*!< RTC WEEK IN BCD REGISTER,					Address offset: 0x1C */	
	__IO uint32_t BCDMONTH;					/*!< RTC MONTH IN BCD REGISTER,					Address offset: 0x20 */	
	__IO uint32_t BCDYEAR;					/*!< RTC YEAR IN BCD REGISTER,					Address offset: 0x24 */	
	__IO uint32_t ALARM;					/*!< RTC ALARM SETTING REGISTER,				Address offset: 0x28 */
	__IO uint32_t FSEL;						/*!< RTC OUTPUT SELECT REGISTER,				Address offset: 0x2C */
	__IO uint32_t ADJUST;					/*!< RTC LTBC ADJUST REGISTER,					Address offset: 0x30 */
	__IO uint32_t ADSIGN;					/*!< RTC LTBC ADJUST SIGN REGISTER,			Address offset: 0x34 */
	__IO uint32_t PR1SEN;					/*!< RTC LTBC 1SEC ENABLE REGISTER,			Address offset: 0x38 */
	__IO uint32_t MSECCNT;					/*!< RTC MS COUNTER REGISTER,						Address offset: 0x3C */
	__IO uint32_t STAMPEN;					/*!< RTC TIME STAMP REGISTER,						Address offset: 0x40 */
	__IO uint32_t CLKSTAMP0R;				/*!< RTC STAMP AT PB4 RISE REGISTER 0,	Address offset: 0x44 */
	__IO uint32_t CALSTAMP0R;				/*!< RTC STAMP AT PB4 RISE REGISTER 0,	Address offset: 0x48 */
	__IO uint32_t CLKSTAMP0F;				/*!< RTC STAMP AT PB4 FALL REGISTER 0,	Address offset: 0x4C */
	__IO uint32_t CALSTAMP0F;				/*!< RTC STAMP AT PB4 FALL REGISTER 0,	Address offset: 0x50 */
	__IO uint32_t CLKSTAMP1R;				/*!< RTC STAMP AT PB4 RISE REGISTER 1,	Address offset: 0x54 */
	__IO uint32_t CALSTAMP1R;				/*!< RTC STAMP AT PB4 RISE REGISTER 1,	Address offset: 0x58 */
	__IO uint32_t CLKSTAMP1F;				/*!< RTC STAMP AT PB4 FALL REGISTER 1,	Address offset: 0x5C */
	__IO uint32_t CALSTAMP1F;				/*!< RTC STAMP AT PB4 FALL REGISTER 1,	Address offset: 0x60 */	
} RTC_Type;

/* ================================================================================ */
/* ================                      AES                       ================ */
/* ================================================================================ */
typedef struct
{
	__IO uint32_t AESCR;   					/*!< AES CONTROL REGISTER,    				Address offset: 0x00 */
	__IO uint32_t AESIF;   					/*!< AES INTERRUPT FLAG REGISTER, 			Address offset: 0x04 */
	__IO uint32_t AESDIN;  					/*!< AES DATA INPUT REGISTER,  				Address offset: 0x08 */
	__IO uint32_t AESDOUT;					/*!< AES DATA OUTPUT REGISTER,      		Address offset: 0x0C */
	__IO uint32_t AESKEY0;					/*!< AES KEY LOW WORD REGISTER 0,			Address offset: 0x10 */
	__IO uint32_t AESKEY1;					/*!< AES KEY REGISTER 1,	 				Address offset: 0x14 */
	__IO uint32_t AESKEY2;					/*!< AES KEY REGISTER 2,					Address offset: 0x18 */
	__IO uint32_t AESKEY3;					/*!< AES KEY REGISTER 3,					Address offset: 0x1C */	
	__IO uint32_t AESKEY4;					/*!< AES KEY REGISTER 4,					Address offset: 0x20 */	
	__IO uint32_t AESKEY5;					/*!< AES KEY REGISTER 5,					Address offset: 0x24 */	
	__IO uint32_t AESKEY6;					/*!< AES KEY REGISTER 6,					Address offset: 0x28 */
	__IO uint32_t AESKEY7;					/*!< AES KEY REGISTER 7,					Address offset: 0x2C */
	__IO uint32_t AESIVR0;					/*!< AES INITIAL DATA REGISTER 0,			Address offset: 0x30 */
	__IO uint32_t AESIVR1;					/*!< AES INITIAL DATA REGISTER 1,			Address offset: 0x34 */
	__IO uint32_t AESIVR2;					/*!< AES INITIAL DATA REGISTER 2,			Address offset: 0x38 */
	__IO uint32_t AESIVR3;					/*!< AES INITIAL DATA REGISTER 3,			Address offset: 0x3C */
} AES_Type;

/* ================================================================================ */
/* ================                      U7816                      ================ */
/* ================================================================================ */
typedef struct
{
	__IO uint32_t U7816CTRL;   			/*!< U7816 CONTROL REGISTER,    					Address offset: 0x00 */
	__IO uint32_t U7816FRC;   			/*!< U7816 FRAME CONTROL REGISTER, 				Address offset: 0x04 */
	__IO uint32_t U7816EGT;  				/*!< U7816 EGT CONFIG REGISTER,  					Address offset: 0x08 */
	__IO uint32_t U7816CLKDIV;			/*!< U7816 CLK DIVIDED REGISTER,      		Address offset: 0x0C */
	__IO uint32_t U7816PDIV;				/*!< U7816 PREDIVIDED REGISTER,						Address offset: 0x10 */
	__IO uint32_t U7816RXBUF;				/*!< U7816 RX BUFFER REGISTER,	 					Address offset: 0x14 */
	__IO uint32_t U7816TXBUF;				/*!< U7816 TX BUFFER REGISTER,						Address offset: 0x18 */
	__IO uint32_t U7816IE;					/*!< U7816 INTERRUPT ENABLE REGISTER,			Address offset: 0x1C */	
	__IO uint32_t U7816IF;					/*!< U7816 INTERRUPT FLAG REGISTER,				Address offset: 0x20 */	
	__IO uint32_t U7816ERR;					/*!< U7816 ERR FLAG REGISTER,							Address offset: 0x24 */	
	__IO uint32_t U7816STA;					/*!< U7816 STATE REGISTER,								Address offset: 0x28 */
} U7816x_Type;

/* ================================================================================ */
/* ================                      WWDT                      ================ */
/* ================================================================================ */
typedef struct
{
	__IO uint32_t WWDTCON;   				/*!< WWDT CONTROL REGISTER,    						Address offset: 0x00 */
	__IO uint32_t WWDTCFG;   				/*!< WWDT CONFIG REGISTER, 								Address offset: 0x04 */
	__IO uint32_t WWDTCNT;  				/*!< WWDT COUNTER REGISTER,  							Address offset: 0x08 */
	__IO uint32_t WWDTIE;						/*!< WWDT INTERRUPT ENABLE REGISTER,      Address offset: 0x0C */
	__IO uint32_t WWDTIF;						/*!< WWDT INTERRUPT FLAG REGISTER,				Address offset: 0x10 */
	__IO uint32_t WWDTDIV;						/*!< WWDT WWDTDIV REGISTER,				Address offset: 0x14 */
} WWDT_Type;

/* ================================================================================ */
/* ================                      TRNG                      ================ */
/* ================================================================================ */
typedef struct
{
	__IO uint32_t RSV0;   				/*!< RESERVED    						Address offset: 0x00 */
	__IO uint32_t RNGOUT;   			/*!< TRNG OUTPUT REGISTER, 				Address offset: 0x04 */
	__IO uint32_t RSV1;					/*!< RESERVED						    Address offset: 0x08 */	
	__IO uint32_t RNGIE;				/*!< TRNG INTERRUPT ENABLE REGISTER,    Address offset: 0x0C */
	__IO uint32_t RNGIF;				/*!< TRNG INTERRUPT FLAG REGISTER,		Address offset: 0x10 */
	__IO uint32_t CRCCON;				/*!< TRNG CRC CONTROL REGISTER,			Address offset: 0x14 */	
	__IO uint32_t CRCIN;				/*!< TRNG CRC INPUT REGISTER,			Address offset: 0x18 */	
	__IO uint32_t CRCFLAG;				/*!< TRNG CRC CRCFLAG REGISTER,			Address offset: 0x1C */	
} TRNG_Type;


/* --------------------  End of section using anonymous unions  ------------------- */
#if defined(__CC_ARM)
  #pragma pop
#elif defined(__ICCARM__)
  /* leave anonymous unions enabled */
#elif defined(__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined(__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined(__TASKING__)
  #pragma warning restore
#else
  #warning Not supported compiler type
#endif

/* ================================================================================ */
/* ================              CPU memory map                    ================ */
/* ================================================================================ */
/* Peripheral and SRAM base address */
#define FLASH_BASE            ((     uint32_t)0x00000000)
#define SRAM_BASE             ((     uint32_t)0x20000000)
#define PERIPH_BASE           ((     uint32_t)0x40000000)

/* ================================================================================ */
/* ================              Peripheral memory map             ================ */
/* ================================================================================ */
/* Peripheral memory map */
#define SCU_BASE						(PERIPH_BASE				+0x00000000)
#define PMU_BASE						(PERIPH_BASE				+0x00000100)
#define RCC_BASE						(PERIPH_BASE				+0x00000200)
#define DMA_BASE						(PERIPH_BASE				+0x00000400)
#define HSPI_BASE						(PERIPH_BASE				+0x00000800)
#define SPI1_BASE						(PERIPH_BASE				+0x00000840)
#define SPI2_BASE						(PERIPH_BASE				+0x00000880)

#define GPIOA_BASE  				(PERIPH_BASE				+0x00000C00)
#define GPIOB_BASE					(PERIPH_BASE				+0x00000C20)
#define GPIOC_BASE					(PERIPH_BASE				+0x00000C40)
#define GPIOD_BASE  				(PERIPH_BASE				+0x00000C60)
#define GPIOE_BASE					(PERIPH_BASE				+0x00000C80)
#define GPIOF_BASE					(PERIPH_BASE				+0x00000CA0)
#define GPIOG_BASE					(PERIPH_BASE				+0x00000CC0)
#define GPIO_COMMON_BASE			(PERIPH_BASE				+0x00000CE0)

#define NVMIF_BASE					(PERIPH_BASE				+0x00001000)

#define CRC_BASE					(PERIPH_BASE				+0x00010000)	

#define DISP_BASE					(PERIPH_BASE				+0x00010C00)	

#define RTC_BASE					(PERIPH_BASE				+0x00011000)

#define IWDT_BASE					(PERIPH_BASE				+0x00011400)	

#define WWDT_BASE					(PERIPH_BASE				+0x00011800)

#define U78160_BASE					(PERIPH_BASE				+0x00011C00)	
#define U78161_BASE					(PERIPH_BASE				+0x00011C2C)	

#define UART_COMMON_BASE			(PERIPH_BASE				+0x00012000)
#define UART0_BASE					(PERIPH_BASE				+0x0001200C)	
#define UART1_BASE					(PERIPH_BASE				+0x0001202C)	
#define UART2_BASE					(PERIPH_BASE				+0x0001204C)	
#define UART3_BASE					(PERIPH_BASE				+0x0001206C)	
#define UART4_BASE					(PERIPH_BASE				+0x0001208C)	
#define UART5_BASE					(PERIPH_BASE				+0x000120AC)	

#define I2C_BASE					(PERIPH_BASE				+0x00012400)

#define ANAC_BASE					(PERIPH_BASE				+0x00012800)

#define RAMBIST_BASE				(PERIPH_BASE				+0x00012C00)

#define BTIMER1_BASE				(PERIPH_BASE				+0x00013000)
#define BTIMER2_BASE				(PERIPH_BASE				+0x00013044)

#define ETIMER1_BASE				(PERIPH_BASE				+0x00013090)
#define ETIMER2_BASE				(PERIPH_BASE				+0x000130B0)
#define ETIMER3_BASE				(PERIPH_BASE				+0x000130D0)
#define ETIMER4_BASE				(PERIPH_BASE				+0x000130F0)

#define LPTIM_BASE					(PERIPH_BASE				+0x00013400)

#define AES_BASE					(PERIPH_BASE				+0x00013800)

#define TRNG_BASE					(PERIPH_BASE				+0x00013C00)

/* ================================================================================ */
/* ================             Peripheral declaration             ================ */
/* ================================================================================ */
#define SCU							((SCU_Type             		*) SCU_BASE					)
#define PMU							((PMU_Type             		*) PMU_BASE					)
#define RCC							((RCC_Type             		*) RCC_BASE					)
#define DMA							((DMA_Type             		*) DMA_BASE					)

#define HSPI							((SPIx_Type            		*) HSPI_BASE				)
#define SPI1							((SPIx_Type            		*) SPI1_BASE				)
#define SPI2							((SPIx_Type            		*) SPI2_BASE				)

#define NVMIF							((NVMIF_Type           		*) NVMIF_BASE				)
#define FLASH							((NVMIF_Type           		*) NVMIF_BASE				)
#define UART							((UART_common_Type        *) UART_COMMON_BASE	)
#define UART0							((UARTx_Type              *) UART0_BASE				)
#define UART1							((UARTx_Type              *) UART1_BASE				)
#define UART2							((UARTx_Type              *) UART2_BASE				)
#define UART3							((UARTx_Type              *) UART3_BASE				)
#define UART4							((UARTx_Type              *) UART4_BASE				)
#define UART5							((UARTx_Type              *) UART5_BASE				)

#define IWDT							((IWDT_Type               *) IWDT_BASE				)

#define GPIOA							((GPIOx_Type         			*) GPIOA_BASE				)
#define GPIOB							((GPIOx_Type         			*) GPIOB_BASE				)
#define GPIOC							((GPIOx_Type         			*) GPIOC_BASE				)
#define GPIOD							((GPIOx_Type         			*) GPIOD_BASE				)
#define GPIOE							((GPIOx_Type         			*) GPIOE_BASE				)
#define GPIOF							((GPIOx_Type         			*) GPIOF_BASE				)
#define GPIOG							((GPIOx_Type         			*) GPIOG_BASE				)
#define GPIO							((GPIO_COMMON_Type   			*) GPIO_COMMON_BASE	)

#define RAMBIST							((RAMBIST_Type   				*) RAMBIST_BASE			)

#define BTIM1							((BTIMx_Type         		*) BTIMER1_BASE			)
#define BTIM2							((BTIMx_Type         		*) BTIMER2_BASE			)

#define ETIM1							((ETIMx_Type         		*) ETIMER1_BASE			)
#define ETIM2							((ETIMx_Type         		*) ETIMER2_BASE			)
#define ETIM3							((ETIMx_Type         		*) ETIMER3_BASE			)
#define ETIM4							((ETIMx_Type         		*) ETIMER4_BASE			)

#define LPTIM							((LPTIM_Type         			*) LPTIM_BASE				)
#define ANAC							((ANAC_Type        				*) ANAC_BASE				)
#define WWDT							((WWDT_Type						*) WWDT_BASE				)

#define I2C								((I2C_Type         				*) I2C_BASE					)
#define CRC								((CRC_Type        				*) CRC_BASE					)
#define LCD								((DISP_Type        				*) DISP_BASE				)
#define RTC								((RTC_Type        				*) RTC_BASE					)
#define U78160							((U7816x_Type        			*) U78160_BASE				)
#define U78161							((U7816x_Type        			*) U78161_BASE				)

#define AES								((AES_Type        				*) AES_BASE					)
#define TRNG							((TRNG_Type        				*) TRNG_BASE				)

/* ================================================================================ */
/* ================             Peripheral include                 ================ */
/* ================================================================================ */

/** @} */ /* End of group Device_Peripheral_Registers */
/** @} */ /* End of group FM32L0XX */
/** @} */ /* End of group Keil */

#ifdef __cplusplus
}
#endif


#endif  /* FM33A0XX_H */

