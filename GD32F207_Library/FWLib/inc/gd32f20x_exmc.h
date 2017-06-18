/**
  ******************************************************************************
  * @file    gd32f20x_exmc.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    15-Jul-2015
  * @brief   EXMC header file of the firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F20X_EXMC_H
#define __GD32F20X_EXMC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gd32f20x.h"

/** @addtogroup GD32F20x_Firmware
  * @{
  */

/** @addtogroup EXMC
  * @{
  */

/** @defgroup EXMC_Exported_Types
  * @{
  */

/** 
  * @brief  Initial Timing Parameters For NOR/SRAM Banks
  */
typedef struct
{
    uint32_t EXMC_AsynAccessMode;       /*!< The asynchronous access mode, detailed in @ref EXMC_AsynAccess_Mode*/
    uint32_t EXMC_SynDataLatency;       /*!< The number of CLK cycles to configure the data latency, 
                                             which may assume a value between 0x0 and 0xF. */
    uint32_t EXMC_SynCLKDivision;       /*!< The number of  HCLK cycles to configure the clock divide ratio,
                                             which can be a value between 0x0 and 0xF. */
    uint32_t EXMC_BusLatency;           /*!< The number of HCLK cycles to configure the bus latency, 
                                             which can be a value between 0x0 and 0xF. */
    uint32_t EXMC_AsynDataSetupTime;    /*!< The number of  HCLK cycles to configure the data setup time 
                                             while in the asynchronous access mode, which can be a value 
                                             between 0x00 and 0xFF. */
    uint32_t EXMC_AsynAddressHoldTime;  /*!< The number of  HCLK cycles to configure the address hold time 
                                             while in the asynchronous access mode, which can be a value 
                                             between 0x0 and 0xF. */
    uint32_t EXMC_AsynAddressSetupTime; /*!< The number of  HCLK cycles to configure the data setup time 
                                             while in the asynchronous access mode, which can be a value 
                                             between 0x0 and 0xF. */
}EXMC_NORSRAMTimingInitPara;

/**
  * @brief  EXMC NOR/SRAM Init structure definition
  */
typedef struct
{
    uint32_t EXMC_NORSRAMBank;                                  /*!< The specified region of NORSRAM Bank1, 
                                                                     choose one from @ref EXMC_NORSRAMBank. */
    uint32_t EXMC_WriteMode;                                    /*!< The write mode, details in @ref EXMC_WriteMode. */
    uint32_t EXMC_ExtendedMode;                                 /*!< Enable or Disable the extended mode, details in 
                                                                     @ref EXMC_ExtendedMode. */
    uint32_t EXMC_AsynWait;                                     /*!< Enable or disable the asynchronous wait feature,detial
                                                                     in @ref EXMC_AsynWait. */
    uint32_t EXMC_NWAITSignal;                                  /*!< Enable or Disable the NWAIT signal while in synchronous
                                                                     bust mode, details in @ref EXMC_NWAITSignal. */
    uint32_t EXMC_MemoryWrite;                                  /*!< Enable or Disable the write operation, details in 
                                                                     @ref EXMC_MemoryWrite. */
    uint32_t EXMC_NWAITConfig;                                  /*!< NWAIT signal configuration, details in @ref EXMC_NWAITConfig */
    uint32_t EXMC_WrapBurstMode;                                /*!< Enable or Disable the wrap burst mode, details in 
                                                                     @ref EXMC_WrapBurstMode. */
    uint32_t EXMC_NWAITPolarity;                                /*!< Specifies the polarity of NWAIT signal from memory, 
                                                                     details in @ref EXMC_NWAITPolarity. */
    uint32_t EXMC_BurstMode;                                    /*!< Enable or Disable the burst mode, details in 
                                                                     @ref EXMC_BurstMode. */
    uint32_t EXMC_DatabusWidth;                                 /*!< Specifies the databus width of external memory, 
                                                                     details in @ref EXMC_DatabusWidth. */
    uint32_t EXMC_MemoryType;                                   /*!< Specifies the type of external memory, details in 
                                                                     @ref EXMC_MemoryType. */
    uint32_t EXMC_AddressDataMux;                               /*!< Specifies whether the data bus and address bus are multiplexed
                                                                     or not,details in @ref EXMC_AddressDataMux. */

    EXMC_NORSRAMTimingInitPara* EXMC_ReadWriteTimingParaStruct; /*!< The struct EXMC_NORSRAMTimingInitPara pointer,which is 
                                                                     used to define the timing parameters for read and write 
                                                                     if the ExtendedMode is not used or define the timing 
                                                                     parameters for read if the ExtendedMode is used. */

    EXMC_NORSRAMTimingInitPara* EXMC_WriteTimingParaStruct;     /*!< The struct EXMC_NORSRAMTimingInitPara pointer,which is 
                                                                     only used to define the timing parameters for write when 
                                                                     the ExtendedMode is used. */
}EXMC_NORSRAMInitPara;

/** 
  * @brief  Timing parameters For EXMC NAND and PCCARD Banks
  */

typedef struct
{
    uint32_t EXMC_DatabusHiZTime;       /*!< The number of  HCLK cycles to configure the dadtabus HiZ time 
                                             for write operation, which can be a value between 0x00 and 0xFF. */
    uint32_t EXMC_HoldTime;             /*!< The number of HCLK cycles to configure the address hold time 
                                             (or the data hold time for write operation),which can be a value 
                                             between 0x00 and 0xFF. */
    uint32_t EXMC_WaitTime;             /*!< The number of HCLK cycles to configure the minimum wait time, 
                                             which can be a value between 0x00 and 0xFF. */
    uint32_t EXMC_SetupTime;            /*!< The number of HCLK cycles to configure the address setup time ,
                                             which can be a value between 0x00 and 0xFF. */
}EXMC_NAND_PCCARDTimingInitPara;

/** 
  * @brief  EXMC NAND Init structure definition
  */
typedef struct
{
    uint32_t EXMC_NANDBank;                                                /*!< The specified Bank of NAND FLASH, choose one 
                                                                                from @ref EXMC_NANDBank. */
    uint32_t EXMC_ECCSize;                                                 /*!< The page size for the ECC calculation,details 
                                                                                in @ref EXMC_ECCSize. */
    uint32_t EXMC_ATRLatency;                                              /*!< The number of HCLK cycles to configure the 
                                                                                latency of ALE low to RB low, which can be a 
                                                                                value between 0x0 and 0xF. */
    uint32_t EXMC_CTRLatency;                                              /*!< The number of HCLK cycles to configure the 
                                                                                latency of CLE low to RB low, which can be a 
                                                                                value between 0x0 and 0xF. */
    uint32_t EXMC_ECCLogic;                                                /*!< Enable or Disable the ECC calculation logic,
                                                                                details in @ref EXMC_ECCLogic. */
    uint32_t EXMC_DatabusWidth;                                            /*!< the NAND flash databus width, details in 
                                                                                @ref EXMC_DatabusWidth. */
    uint32_t EXMC_WaitFeature;                                             /*!< Enables or Disables the Wait feature,details 
                                                                                in @ref EXMC_WaitFeature. */
    EXMC_NAND_PCCARDTimingInitPara*  EXMC_CommonSpaceTimingParaStruct;     /*!< The struct EXMC_NAND_PCCARDTimingInitPara 
                                                                                pointer, which is used to define the timing 
                                                                                parameters for NAND flash Common Space. */
    EXMC_NAND_PCCARDTimingInitPara*  EXMC_AttributeSpaceTimingParaStruct;  /*!< The struct EXMC_NAND_PCCARDTimingInitPara 
                                                                                pointer, which is used to define the timing 
                                                                                parameters for NAND flash Attribute Space. */
}EXMC_NANDInitPara;

/** 
  * @brief  EXMC PCCARD Init structure definition
  */

typedef struct
{
    uint32_t EXMC_ATRLatency;                                               /*!< The number of HCLK cycles to configure 
                                                                                 the latency of ALE low to RB low, which can 
                                                                                  be a value between 0x0 and 0xF. */
    uint32_t EXMC_CTRLatency;                                               /*!< The number of HCLK cycles to configure 
                                                                                 the latency of CLE low to RB low, which can 
                                                                                 be a value between 0x0 and 0xF. */
    uint32_t EXMC_WaitFeature;                                              /*!< Enables or Disables the Wait feature,details 
                                                                                 in @ref EXMC_WaitFeature. */
    EXMC_NAND_PCCARDTimingInitPara*  EXMC_CommonSpaceTimingParaStruct;      /*!< The struct EXMC_NAND_PCCARDTimingInitPara 
                                                                                 pointer, which is used to define the timing 
                                                                                 parameters for PC CARD Common Space. */
    EXMC_NAND_PCCARDTimingInitPara*  EXMC_AttributeSpaceTimingParaStruct;   /*!< The struct EXMC_NAND_PCCARDTimingInitPara 
                                                                                 pointer, which is used to define the timing 
                                                                                 parameters for PC CARD Attribute Space. */
    EXMC_NAND_PCCARDTimingInitPara*  EXMC_IOSpaceTimingParaStruct;          /*!< The struct EXMC_NAND_PCCARDTimingInitPara 
                                                                                 pointer, which is used to define the timing 
                                                                                 parameters for PC CARD I/O Space. */
}EXMC_PCCARDInitPara;

/** 
  * @brief  Initial parameters For SQPI-PSRAM Banks  
  */
typedef struct{
    uint32_t EXMC_SamplePolarity;                    /*!< Read data sample polarity, details in 
                                                          @ref EXMC_SamplePolarity */ 
    uint32_t EXMC_IDLength;                          /*!< SPI PSRAM ID Length, details in 
                                                          @ref EXMC_SamplePolarity */    
    uint32_t EXMC_AddressBits;                       /*!< Bit number of SPI PSRAM address phase, which can
                                                          be a value between 1 to 26(default:24)*/
    uint32_t EXMC_CommandBits;                       /*!< Bit number of SPI PSRAM command phase, details in 
                                                          @ref EXMC_CommandBits */
}EXMC_SQPIPSRAMInitPara;


/** 
  * @brief  Timing parameters for EXMC SDRAM Banks
  */
typedef struct
{
    uint32_t EXMC_RowToColumnDelay;                       /*!< The number of SDCLK cycles to configure the row to 
                                                               column delay, which can be a value between 0x0 and 0xF. */
    uint32_t EXMC_RowPrechargeDelay;                      /*!< The number of SDCLK cycles to configure the row precharge 
                                                               delay, which can be a value between 0x0 and 0xF. */
    uint32_t EXMC_WriteRecoveryDelay;                     /*!< The number of SDCLK cycles to configure the write recovery 
                                                               delay, which can be a value between 0x0 and 0xF. */
    uint32_t EXMC_AutoRefreshDelay;                       /*!< The number of SDCLK cycles to configure the auto refresh 
                                                               delay, which can be a value between 0x0 and 0xF. */       
    uint32_t EXMC_RowAddressSelectDelay;                  /*!< The number of SDCLK cycles to configure the row address 
                                                               select delay, which can be a value between 0x0 and 0xF. */
    uint32_t EXMC_ExitSelfRefreshDelay;                   /*!< The number of SDCLK cycles to configure the exit self-refresh 
                                                               delay, which can be a value between 0x0 and 0xF. */
    uint32_t EXMC_LoadModeRegisterDelay;                  /*!< The number of SDCLK cycles to configure the load mode register
                                                               delay, which can be a value between 0x0 and 0xF. */
}EXMC_SDRAMTimingInitPara;

/** 
  * @brief  Command parameters for EXMC SDRAM Banks
  */
typedef struct
{
    uint32_t EXMC_ModeRegisterContent;                      /*!< Specified the SDRAM Mode Register content */

    uint32_t EXMC_AutoRefreshNumber;                        /*!< Defines the number of successive auto-refresh cycles will be 
                                                                 send when CMD = 011,which can be a value between 0x0 and 0xE. */

    uint32_t EXMC_BankSelect;                               /*!< Specified the bank which command will be sent to,choose one
                                                                 from @ref EXMC_Command_BankSelect. */

    uint32_t EXMC_Command;                                  /*!< Specified the commands that will be sent to SDRAM, choose one
                                                                 from @ref EXMC_Command.*/

}EXMC_SDRAMCommandPara;

/** 
  * @brief  EXMC SDRAM Init structure definition
  */

typedef struct
{
    uint32_t EXMC_SDRAMBank;                                    /*!< The specified Bank of SDRAM, choose one 
                                                                     from @ref EXMC_SDRAM_Bank. */
    uint32_t EXMC_PipelineReadDelay;                            /*!< Define the delay for reading data after CAS latency 
                                                                     in HCLK clock cycles,choose one from @ref EXMC_PipelineReadDelay. */
    uint32_t EXMC_BrustReadSwitch;                              /*!< Enables or disables the burst read,choose one 
                                                                     from @ref EXMC_BrustReadSwitch. */
    uint32_t EXMC_SDClockConfig;                                /*!< Define the SDCLK memory clock for both SDRAM Banks,details 
                                                                     in @ref EXMC_SDClockConfig. */
    uint32_t EXMC_WriteProtection;                              /*!< Enables the SDRAM bank write protection function, choose one 
                                                                     from @ref EXMC_WriteProtection. */
    uint32_t EXMC_CASLatency;                                   /*!< The number of SDCLK cycles to configure the SDRAM CAS latency, 
                                                                     details in @ref EXMC_CASLatency. */
    uint32_t EXMC_InternalBankNumber;                           /*!< Define the number internal banks,choose one from
                                                                     @ref EXMC_InternalBankNumber. */
    uint32_t EXMC_DataWidth;                                    /*!< Specifies the databus width of SDRAM memory, 
                                                                     details in @ref EXMC_DataWidth. */
    uint32_t EXMC_RowAddressWidth;                              /*!< Specifies the bit width of a row address, 
                                                                     details in @ref EXMC_RowAddressWidth. */
    uint32_t EXMC_ColumnAddressWidth;                           /*!< Specifies the bit width of a column address, 
                                                                     details in @ref EXMC_ColumnAddressWidth. */
    EXMC_SDRAMTimingInitPara* EXMC_SDRAMTimingParaStruct;       /*!< The struct EXMC_SDRAMTimingInitPara pointer,which is only used 
                                                                     to define the timing parameters for write and read SDRAM. */
}EXMC_SDRAMInitPara;

/**
  * @}
  */

/** @defgroup EXMC_Exported_Constants
  * @{
  */

/** @defgroup EXMC_NORSRAMBank 
  * @{
  */
#define EXMC_BANK1_NORSRAM1                              ((uint32_t)0x00000001)
#define EXMC_BANK1_NORSRAM2                              ((uint32_t)0x00000002)
#define EXMC_BANK1_NORSRAM3                              ((uint32_t)0x00000003)
#define EXMC_BANK1_NORSRAM4                              ((uint32_t)0x00000004)

/**
  * @}
  */

/** @defgroup EXMC_NANDBank 
  * @{
  */
#define EXMC_BANK2_NAND                                  ((uint32_t)0x00000010)
#define EXMC_BANK3_NAND                                  ((uint32_t)0x00000100)

/**
  * @}
  */

/** @defgroup EXMC_PCCARD_Bank 
  * @{
  */
#define EXMC_BANK4_PCCARD                                ((uint32_t)0x00001000)

/**
  * @}
  */
  
/** @defgroup EXMC_SDRAM_Bank 
  * @{
  */
#define EXMC_BANK5_SDRAM                                 ((uint32_t)0x00010000)
#define EXMC_BANK6_SDRAM                                 ((uint32_t)0x00100000)

/**
  * @}
  */
  
/** @defgroup NORSRAM_Controller 
  * @{
  */

/** @defgroup EXMC_AddressDataMux 
  * @{
  */
#define EXMC_ADDRESS_DATA_MUX_DISABLE                    ((uint32_t)0x00000000)
#define EXMC_ADDRESS_DATA_MUX_ENABLE                     ((uint32_t)0x00000002)

/**
  * @}
  */

/** @defgroup EXMC_MemoryType 
  * @{
  */
#define EXMC_MEMORY_TYPE_SRAM                            ((uint32_t)0x00000000)
#define EXMC_MEMORY_TYPE_PSRAM                           ((uint32_t)0x00000004)
#define EXMC_MEMORY_TYPE_NOR                             ((uint32_t)0x00000008)

/**
  * @}
  */

/** @defgroup EXMC_DatabusWidth 
  * @{
  */
#define EXMC_DATABUS_WIDTH_8B                            ((uint32_t)0x00000000)
#define EXMC_DATABUS_WIDTH_16B                           ((uint32_t)0x00000010)

/**
  * @}
  */

/** @defgroup EXMC_NORFlash_Access
  * @{
  */
#define EXMC_NORFLASH_ACCESS_DISABLE                     ((uint32_t)0x00000000)
#define EXMC_NORFLASH_ACCESS_ENABLE                      ((uint32_t)0x00000040)

/**
  * @}
  */

/** @defgroup EXMC_BurstMode 
  * @{
  */
#define EXMC_BURST_MODE_DISABLE                          ((uint32_t)0x00000000) 
#define EXMC_BURST_MODE_ENABLE                           ((uint32_t)0x00000100)

/**
  * @}
  */
  
/** @defgroup EXMC_AsynWait 
  * @{
  */
#define EXMC_ASYN_WAIT_DISABLE                           ((uint32_t)0x00000000)
#define EXMC_ASYN_WAIT_ENABLE                            ((uint32_t)0x00008000)

/**
  * @}
  */
  
/** @defgroup EXMC_NWAITPolarity 
  * @{
  */
#define EXMC_NWAIT_POLARITY_LOW                          ((uint32_t)0x00000000)
#define EXMC_NWAIT_POLARITY_HIGH                         ((uint32_t)0x00000200)

/**
  * @}
  */

/** @defgroup EXMC_WrapBurstMode 
  * @{
  */
#define EXMC_WRAP_BURST_MODE_DISABLE                     ((uint32_t)0x00000000)
#define EXMC_WRAP_BURST_MODE_ENABLE                      ((uint32_t)0x00000400) 

/**
  * @}
  */

/** @defgroup EXMC_NWAITConfig 
  * @{
  */
#define EXMC_NWAIT_CONFIG_BEFORE                         ((uint32_t)0x00000000)
#define EXMC_NWAIT_CONFIG_DURING                         ((uint32_t)0x00000800) 

/**
  * @}
  */

/** @defgroup EXMC_MemoryWrite 
  * @{
  */
#define EXMC_MEMORY_WRITE_DISABLE                        ((uint32_t)0x00000000)
#define EXMC_MEMORY_WRITE_ENABLE                         ((uint32_t)0x00001000)

/**
  * @}
  */

/** @defgroup EXMC_NWAITSignal 
  * @{
  */
#define EXMC_NWAIT_SIGNAL_DISABLE                        ((uint32_t)0x00000000)
#define EXMC_NWAIT_SIGNAL_ENABLE                         ((uint32_t)0x00002000) 

/**
  * @}
  */

/** @defgroup EXMC_ExtendedMode 
  * @{
  */
#define EXMC_EXTENDED_MODE_DISABLE                       ((uint32_t)0x00000000)
#define EXMC_EXTENDED_MODE_ENABLE                        ((uint32_t)0x00004000)

/**
  * @}
  */

/** @defgroup EXMC_WriteMode 
  * @{
  */
#define EXMC_ASYN_WRITE                                  ((uint32_t)0x00000000)
#define EXMC_SYN_WRITE                                   ((uint32_t)0x00080000) 

/**
  * @}
  */

/** @defgroup EXMC_AsynAccess_Mode 
  * @{
  */
#define EXMC_ACCESS_MODE_A                               ((uint32_t)0x00000000)
#define EXMC_ACCESS_MODE_B                               ((uint32_t)0x10000000) 
#define EXMC_ACCESS_MODE_C                               ((uint32_t)0x20000000)
#define EXMC_ACCESS_MODE_D                               ((uint32_t)0x30000000)

/**
  * @}
  */

/**
  * @}
  */
  
/** @defgroup NAND_PCCARD_Controller 
  * @{
  */

/** @defgroup EXMC_WaitFeature 
  * @{
  */
#define EXMC_WAIT_FEATURE_DISABLE                        ((uint32_t)0x00000000)
#define EXMC_WAIT_FEATURE_ENABLE                         ((uint32_t)0x00000002)

/**
  * @}
  */

/** @defgroup EXMC_ECCLogic 
  * @{
  */
#define EXMC_ECC_LOGIC_DISABLE                           ((uint32_t)0x00000000)
#define EXMC_ECC_LOGIC_ENABLE                            ((uint32_t)0x00000040)

/**
  * @}
  */

/** @defgroup EXMC_ECCSize 
  * @{
  */
#define EXMC_ECC_SIZE_256BYTES                           ((uint32_t)0x00000000)
#define EXMC_ECC_SIZE_512BYTES                           ((uint32_t)0x00020000)
#define EXMC_ECC_SIZE_1024BYTES                          ((uint32_t)0x00040000)
#define EXMC_ECC_SIZE_2048BYTES                          ((uint32_t)0x00060000)
#define EXMC_ECC_SIZE_4096BYTES                          ((uint32_t)0x00080000)
#define EXMC_ECC_SIZE_8192BYTES                          ((uint32_t)0x000A0000)
          
/**
  * @}    
  */
          
/** @defgroup EXMC_Interrupt_Source
  * @{    
  */
#define EXMC_INT_RISE                                    ((uint32_t)0x00000008)
#define EXMC_INT_LEVEL                                   ((uint32_t)0x00000010)
#define EXMC_INT_FALL                                    ((uint32_t)0x00000020)
          
/**
  * @}    
  */
          
/** @defgroup EXMC_FLAG
  * @{    
  */
#define EXMC_FLAG_RISE                                   ((uint32_t)0x00000001)
#define EXMC_FLAG_LEVEL                                  ((uint32_t)0x00000002)
#define EXMC_FLAG_FALL                                   ((uint32_t)0x00000004)
#define EXMC_FLAG_FIFOE                                  ((uint32_t)0x00000040)

/**
  * @}
  */

/**
  * @}
  */
  
/** @defgroup SQPI-PSRAM_Controller
  * @{ 
  */
  
/** @defgroup EXMC_SamplePolarity 
  * @{       
  */
#define EXMC_SAMPLE_RISE                                 ((uint32_t)0x00000000) 
#define EXMC_SAMPLE_FALL                                 ((uint32_t)0x80000000) 
      
/**   
  * @} 
  */

/** @defgroup EXMC_IDLength     
  * @{      
  */
#define EXMC_ID_LENGTH_8                                 ((uint32_t)0x60000000)
#define EXMC_ID_LENGTH_16                                ((uint32_t)0x40000000)
#define EXMC_ID_LENGTH_32                                ((uint32_t)0x20000000)
#define EXMC_ID_LENGTH_64                                ((uint32_t)0x00000000)
     
/**  
  * @}
  */

/** @defgroup EXMC_CommandBits 
  * @{          
  */
#define EXMC_COMMAND_BITS_4                              ((uint32_t)0x00000000) 
#define EXMC_COMMAND_BITS_8                              ((uint32_t)0x00010000) 
#define EXMC_COMMAND_BITS_16                             ((uint32_t)0x00020000)

/** 
  * @} 
  */

/** @defgroup EXMC_CmdMode 
  * @{
  */
#define EXMC_CmdMode_NOTSPI                              ((uint32_t)0x00000000)
#define EXMC_CmdMode_SPI                                 ((uint32_t)0x00100000)
#define EXMC_CmdMode_SQPI                                ((uint32_t)0x00200000)
#define EXMC_CmdMode_QPI                                 ((uint32_t)0x00300000)

/**   
  * @} 
  */

/** @defgroup EXMC_SendCmdFlag 
  * @{
  */
#define EXMC_SendCmdFlag_SC                              EXMC_SWCR_SC
#define EXMC_SendCmdFlag_RDID                            EXMC_SRCR_RDID

/**   
  * @} 
  */

/**
  * @} 
  */
 
/** @defgroup SDRAM_Controller
  * @{
  */
 
/** @defgroup EXMC_Command_BankSelect
  * @{  
  */
#define EXMC_BANK_SELECT_1                               ((uint32_t)0x00000010)
#define EXMC_BANK_SELECT_2                               ((uint32_t)0x00000008)
#define EXMC_BANK_SELECT_1_2                             ((uint32_t)0x00000018)

/** 
  * @} 
  */
  
/** @defgroup EXMC_SDRAM_Interrupt_Source
  * @{    
  */
#define EXMC_INT_REFRESH                                 ((uint32_t)0x00004000)
 
 /** 
  * @} 
  */
 
/** @defgroup EXMC_SDRAM_Flag
  * @{    
  */
#define EXMC_FLAG_REFRESH                                ((uint32_t)0x00000001)
#define EXMC_FLAG_READY                                  ((uint32_t)0x00000020)
 
 /** 
  * @} 
  */ 
 
/** @defgroup EXMC_Command
  * @{  
  */
#define EXMC_COMMAND_NORMAL                              ((uint32_t)0x00000000)
#define EXMC_COMMAND_CKE_HIGH                            ((uint32_t)0x00000001)
#define EXMC_COMMAND_PRECHARGE                           ((uint32_t)0x00000002) 
#define EXMC_COMMAND_AUTO_REFRESH                        ((uint32_t)0x00000003)
#define EXMC_COMMAND_LOAD                                ((uint32_t)0x00000004)
#define EXMC_COMMAND_SELF_REFRESH                        ((uint32_t)0x00000005)
#define EXMC_COMMAND_POWER_DOWN                          ((uint32_t)0x00000006)
  
/**
  * @}  
  */

/** @defgroup EXMC_PipelineReadDelay 
  * @{ 
  */
#define EXMC_PIPELINE_READ_DELAY_0                       ((uint32_t)0x00000000)  
#define EXMC_PIPELINE_READ_DELAY_1                       ((uint32_t)0x00002000)
#define EXMC_PIPELINE_READ_DELAY_2                       ((uint32_t)0x00004000)
         
/**
  * @}     
  */

/** @defgroup EXMC_BrustReadSwitch
  * @{  
  */
#define EXMC_BRUST_READ_DISABLE                          ((uint32_t)0x00000000)
#define EXMC_BRUST_READ_ENABLE                           ((uint32_t)0x00001000)
   
/**
  * @}  
  */

/** @defgroup EXMC_SDClockConfig 
  * @{   
  */
#define EXMC_SDCLK_DISABLE                               ((uint32_t)0x00000000) 
#define EXMC_SDCLK_PERIOD_2                              ((uint32_t)0x00000800) 
#define EXMC_SDCLK_PERIOD_3                              ((uint32_t)0x00000C00) 
  
/** 
  * @}   
  */
  
/** @defgroup EXMC_WriteProtection   
  * @{ 
  */
#define EXMC_WRITE_PROTECTION_DISABLE                    ((uint32_t)0x00000000) 
#define EXMC_WRITE_PROTECTION_ENABLE                     ((uint32_t)0x00000200)
       
/**
  * @} 
  */

/** @defgroup EXMC_CASLatency 
  * @{ 
  */
#define EXMC_CAS_LATENCY_1                               ((uint32_t)0x00000080)
#define EXMC_CAS_LATENCY_2                               ((uint32_t)0x00000100)
#define EXMC_CAS_LATENCY_3                               ((uint32_t)0x00000180)
 
/**
  * @} 
  */

/** @defgroup EXMC_InternalBankNumber 
  * @{ 
  */
#define EXMC_INTERNAL_BANK_NUMBER_2                      ((uint32_t)0x00000000)
#define EXMC_INTERNAL_BANK_NUMBER_4                      ((uint32_t)0x00000040)
           
/**  
  * @}
  */

/** @defgroup EXMC_DataWidth 
  * @{ 
  */
#define EXMC_DATA_WIDTH_8                                ((uint32_t)0x00000000) 
#define EXMC_DATA_WIDTH_16                               ((uint32_t)0x00000010)
#define EXMC_DATA_WIDTH_32                               ((uint32_t)0x00000020) 
  
/**
  * @} 
  */

/** @defgroup EXMC_RowAddressWidth     
  * @{ 
  */
#define EXMC_ROW_ADDRESS_WIDTH_11                        ((uint32_t)0x00000000) 
#define EXMC_ROW_ADDRESS_WIDTH_12                        ((uint32_t)0x00000004) 
#define EXMC_ROW_ADDRESS_WIDTH_13                        ((uint32_t)0x00000008) 
       
/**
  * @} 
  */

/** @defgroup EXMC_ColumnAddressWidth 
  * @{   
  */
#define EXMC_COLUMN_ADDRESS_WIDTH_8                      ((uint32_t)0x00000000) 
#define EXMC_COLUMN_ADDRESS_WIDTH_9                      ((uint32_t)0x00000001) 
#define EXMC_COLUMN_ADDRESS_WIDTH_10                     ((uint32_t)0x00000002) 
#define EXMC_COLUMN_ADDRESS_WIDTH_11                     ((uint32_t)0x00000003) 
 
/** 
  * @} 
  */

/** @defgroup EXMC_BankStatus 
  * @{   
  */
#define EXMC_BANK_STATUS_NORMAL                          ((uint32_t)0x00000000)
#define EXMC_BANK_STATUS_SELF_REFRESH                    ((uint32_t)0x00000001)
#define EXMC_BANK_STATUS_POWER_DOWN                      ((uint32_t)0x00000002)

/** 
  * @} 
  */
  
/** 
  * @} 
  */

/** 
  * @} 
  */
  
/** @defgroup EXMC_Exported_Functions
  * @{
  */
/************************ Functions of NOR/SRAM Controller ************************/
void EXMC_NORSRAM_DeInit(uint32_t EXMC_NORSRAMBank);
void EXMC_NORSRAM_Init(EXMC_NORSRAMInitPara* EXMC_NORSRAMInitParaStruct);
void EXMC_NORSRAMStruct_Init(EXMC_NORSRAMInitPara* EXMC_NORSRAMInitParaStruct);
void EXMC_NORSRAM_Enable(uint32_t EXMC_NORSRAMBank, TypeState NewValue);

/*********************** Functions of SQPI-PSRAM Controller ***********************/
void EXMC_SQPIPSRAM_DeInit(void);
void EXMC_SQPIPSRAM_Init(EXMC_SQPIPSRAMInitPara* EXMC_SQPIPSRAMInitParaStruct);
void EXMC_SQPIPSRAMStruct_Init(EXMC_SQPIPSRAMInitPara* EXMC_SQPIPSRAMInitParaStruct);
void EXMC_SQPIPSRAM_SetReadCmd(uint32_t EXMC_RCmdMode,uint32_t EXMC_RWaitCycle,uint32_t EXMC_RCmdCode);
void EXMC_SQPIPSRAM_SetWriteCmd(uint32_t EXMC_WCmdMode,uint32_t EXMC_WWaitCycle,uint32_t EXMC_WCmdCode);
void EXMC_SQPIPSRAM_SendReadIDCmd(void);
void EXmC_SQPIPSRAM_SendWriteCmd(void);
uint32_t EXMC_SQPIPSRAM_GetIDLow(void);
uint32_t EXMC_SQPIPSRAM_GetIDHigh(void); 
TypeState EXMC_SQPIPSRAM_GetSendCmdState(uint32_t EXMC_SendCmdFlag);

/************************** Functions of NAND Controller **************************/
void EXMC_NAND_DeInit(uint32_t EXMC_NANDBank);
void EXMC_NAND_Init(EXMC_NANDInitPara* EXMC_NANDInitParaStruct);
void EXMC_NANDStruct_Init(EXMC_NANDInitPara* EXMC_NANDInitParaStruct);
void EXMC_NAND_Enable(uint32_t EXMC_NANDBank, TypeState NewValue);
void EXMC_NANDECC_Enable(uint32_t EXMC_NANDBank, TypeState NewValue);
uint32_t EXMC_GetECC(uint32_t EXMC_NANDBank);

/************************* Functions of PCCARD Controller *************************/     
void EXMC_PCCARD_DeInit(void);
void EXMC_PCCARD_Init(EXMC_PCCARDInitPara* EXMC_PCCARDInitParaStruct);
void EXMC_PCCARDStruct_Init(EXMC_PCCARDInitPara* EXMC_PCCARDInitParaStruct);
void EXMC_PCCARD_Enable(TypeState NewValue);

/************************* Functions of SDRAM Controller **************************/
void EXMC_SDRAM_DeInit(uint32_t EXMC_SDRAMBank);
void EXMC_SDRAM_Init(EXMC_SDRAMInitPara* EXMC_SDRAMInitParaStruct);
void EXMC_SDRAMStruct_Init(EXMC_SDRAMInitPara* EXMC_SDRAMInitParaStruct);
void EXMC_SDRAM_CmdConfig(EXMC_SDRAMCommandPara* EXMC_SDRAMCommandParaStruct);
void EXMC_SDRAM_SetRefreshCount(uint32_t EXMC_Count);
void EXMC_SDRAM_SetAutoRefreshNumber(uint32_t EXMC_Number);
void EXMC_SDRAM_WriteProtection_Enable(uint32_t EXMC_SDRAMBank, TypeState NewValue);
uint32_t EXMC_SDRAM_GetBankStatus(uint32_t EXMC_SDRAMBank);

void EXMC_SDRAM_ReadSampleConfig(uint32_t EXMC_DelayCell, uint32_t EXMC_ExtraHCLK);
void EXMC_SDRAM_ReadSample_Enable(TypeState NewValue);

/****************** Functions of Interrupts and State management ******************/
void EXMC_INTConfig(uint32_t EXMC_Bank, uint32_t EXMC_INT, TypeState NewValue);
TypeState EXMC_GetBitState(uint32_t EXMC_Bank, uint32_t EXMC_FLAG);
void EXMC_ClearBitState(uint32_t EXMC_Bank, uint32_t EXMC_FLAG);
TypeState EXMC_GetIntBitState(uint32_t EXMC_Bank, uint32_t EXMC_INT); 
void EXMC_ClearIntBitState(uint32_t EXMC_Bank, uint32_t EXMC_INT);

#ifdef __cplusplus
}
#endif

#endif /*__GD32F20x_EXMC_H */
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
