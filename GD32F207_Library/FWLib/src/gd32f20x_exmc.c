/**
  ******************************************************************************
  * @file    gd32f20x_exmc.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    15-Jul-2015
  * @brief   EXMC functions of the firmware library.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f20x_exmc.h"
#include "gd32f20x_rcc.h"

/** @addtogroup GD32F20x_Firmware
  * @{
  */

/** @defgroup EXMC 
  * @brief EXMC driver modules
  * @{
  */ 

/** @defgroup EXMC_Private_Defines
  * @{
  */

/* EXMC Bank1 SCTLRx_BAKEN and SCTLRx_NORAEN Mask */
#define SCTLR_BAKEN_SET                              ((uint32_t)0x00000001)
#define SCTLR_BAKEN_RESET                            ((uint32_t)0x000FFFFE)
                                                    
/* EXMC Bank1 SCTLRx_NOREN Mask */                  
#define SCTLR_NOREN_SET                              ((uint32_t)0x00000040)

/* EXMC Bank2/3/4 CTLRx_BAKEN Mask */               
#define CTLR_BAKEN_SET                               ((uint32_t)0x00000004)
#define CTLR_BAKEN_RESET                             ((uint32_t)0x000FFFFB)
                                                    
/* EXMC Bank2/3/4 CTLRx_ECCEN Mask */               
#define CTLR_ECCEN_SET                               ((uint32_t)0x00000040)
#define CTLR_ECCEN_RESET                             ((uint32_t)0x000FFFBF)
                                                    
/* EXMC Bank2/3/4 CTLRx_MTYPE Mask */
#define CTLR_EMTYP_NAND                              ((uint32_t)0x00000008)

/* EXMC Bank1 Register Reset mask */
#define BANK1_SCTLR1_RESET                           ((uint32_t)0x000030DB)
#define BANK1_SCTLR_RESET                            ((uint32_t)0x000030D2)
#define BANK1_STR_RESET                              ((uint32_t)0x0FFFFFFF)
#define BANK1_SWTR_RESET                             ((uint32_t)0x0FFFFFFF)

/* EXMC Bank1 SQPI-PSRAM Register Reset mask */
#define BANK1_SQPI_SINIR_RESET                       ((uint32_t)0x18010000)
#define BANK1_SQPI_SRCR_RESET                        ((uint32_t)0x00000000)
#define BANK1_SQPI_SWCR_RESET                        ((uint32_t)0x00000000)
#define BANK1_SQPI_SIDLR_RESET                       ((uint32_t)0x00000000)
#define BANK1_SQPI_SIDHR_RESET                       ((uint32_t)0x00000000)

/* EXMC Bank2/3 Register Reset mask */
#define BANK2_3_CTLR_RESET                           ((uint32_t)0x00000018)
#define BANK2_3_SIR_RESET                            ((uint32_t)0x00000040)
#define BANK2_3_COMTR_RESET                          ((uint32_t)0xFCFCFCFC)
#define BANK2_3_ATTR_RESET                           ((uint32_t)0xFCFCFCFC)

/* EXMC Bank4 Register Reset mask */
#define BANK4_CTLR_RESET                             ((uint32_t)0x00000018)
#define BANK4_SIR_RESET                              ((uint32_t)0x00000000)
#define BANK4_COMTR_RESET                            ((uint32_t)0xFCFCFCFC)
#define BANK4_ATTR_RESET                             ((uint32_t)0xFCFCFCFC)
#define BANK4_IOTR_RESET                             ((uint32_t)0xFCFCFCFC)

/* EXMC Bank5 and Bank6 Register Reset mask */
#define BANK5_6_SDCTLR_RESET                         ((uint32_t)0x000002D0)
#define BANK5_6_SDTR_RESET                           ((uint32_t)0x0FFFFFFF)
#define BANK5_6_SDCMDR_RESET                         ((uint32_t)0x00000000)
#define BANK5_6_SDARFIR_RESET                        ((uint32_t)0x00000000)
#define BANK5_6_SDSTR_RESET                          ((uint32_t)0x00000000)  
#define BANK5_6_SDRSCR_RESET                         ((uint32_t)0x00000000)

/* EXMC register bit offset */
#define STR_AHT_OFFSET                               ((uint32_t)0x00000004)    
#define STR_DST_OFFSET                               ((uint32_t)0x00000008)    
#define STR_BUSLAT_OFFSET                            ((uint32_t)0x00000010)    
#define STR_CDIV_OFFSET                              ((uint32_t)0x00000014)    
#define STR_DLAT_OFFSET                              ((uint32_t)0x00000018)    

#define SWTR_AHT_OFFSET                              ((uint32_t)0x00000004)    
#define SWTR_DST_OFFSET                              ((uint32_t)0x00000008)     
#define SWTR_CDIV_OFFSET                             ((uint32_t)0x00000014)      
#define SWTR_DLAT_OFFSET                             ((uint32_t)0x00000018)      

#define CTLR_CTR_OFFSET                              ((uint32_t)0x00000009)      
#define CTLR_ATR_OFFSET                              ((uint32_t)0x0000000D)      

#define COMTR_COMWT_OFFSET                           ((uint32_t)0x00000008)      
#define COMTR_COMHT_OFFSET                           ((uint32_t)0x00000010)     
#define COMTR_COMHIZT_OFFSET                         ((uint32_t)0x00000018)     

#define ATTR_ATTWT_OFFSET                            ((uint32_t)0x00000008)     
#define ATTR_ATTHT_OFFSET                            ((uint32_t)0x00000010)     
#define ATTR_ATTHIZT_OFFSET                          ((uint32_t)0x00000018)     

#define IOTR_IOTWT_OFFSET                            ((uint32_t)0x00000008)      
#define IOTR_IOHT_OFFSET                             ((uint32_t)0x00000010)     
#define IOTR_IOHIZT_OFFSET                           ((uint32_t)0x00000018)  

#define SIR_INTEN_OFFSET                             ((uint32_t)0x00000003)

#define SDTR_RCD_OFFSET                              ((uint32_t)0x00000018)
#define SDTR_RPD_OFFSET                              ((uint32_t)0x00000014)
#define SDTR_WRD_OFFSET                              ((uint32_t)0x00000010)
#define SDTR_ARFD_OFFSET                             ((uint32_t)0x0000000C)
#define SDTR_RASD_OFFSET                             ((uint32_t)0x00000008)
#define SDTR_XSRD_OFFSET                             ((uint32_t)0x00000004)

#define SDSTR_STA1_OFFSET                            ((uint32_t)0x00000001)
#define SDSTR_STA2_OFFSET                            ((uint32_t)0x00000003)

#define SDCMDR_NARF_OFFSET                           ((uint32_t)0x00000005)
#define SDCMDR_MRC_OFFSET                            ((uint32_t)0x00000009)

#define SDARFIR_ARFITV_OFFSET                        ((uint32_t)0x00000001)

#define SDRSCR_SDSC_OFFSET                           ((uint32_t)0x00000004)

#define SINIR_ADRBIT_OFFSET                          ((uint32_t)0x00000018)

#define SRCR_RWAITCYCLE_OFFSET                       ((uint32_t)0x00000010)

#define SWCR_WWAITCYCLE_OFFSET                       ((uint32_t)0x00000010)

#define SDRSCR_SSCR_OFFSET                           ((uint32_t)0x00000001)

/**
  * @}
  */

/** @defgroup EXMC_Private_Functions
  * @{
  */

/**
  * @brief  Reset the Bank1 NOR/SRAM registers.
  * @param  EXMC_NORSRAMBank: specifies the Region of Bank1.
  *   This parameter can be one of the following values:
  *     @arg EXMC_BANK1_NORSRAM1: the Region1 of Bank1
  *     @arg EXMC_BANK1_NORSRAM2: the Region2 of Bank1
  *     @arg EXMC_BANK1_NORSRAM3: the Region3 of Bank1
  *     @arg EXMC_BANK1_NORSRAM4: the Region4 of Bank1
  * @retval None
  */
void EXMC_NORSRAM_DeInit(uint32_t EXMC_NORSRAMBank)
{
    switch(EXMC_NORSRAMBank)
    {
        case EXMC_BANK1_NORSRAM1:
        {
            /* the Region1 of Bank1 */
            EXMC_BANK1->SCTLR1 = BANK1_SCTLR1_RESET;
            EXMC_BANK1->STR1 = BANK1_STR_RESET;
            EXMC_BANK1_WT->SWTR1 = BANK1_SWTR_RESET;
            break; 
        } 
        case EXMC_BANK1_NORSRAM2:
        {
            /* the Region2 of Bank1 */
            EXMC_BANK1->SCTLR2 = BANK1_SCTLR_RESET;
            EXMC_BANK1->STR2 = BANK1_STR_RESET;
            EXMC_BANK1_WT->SWTR2 = BANK1_SWTR_RESET;
            break; 
        }
        case EXMC_BANK1_NORSRAM3:
        {
            /* the Region3 of Bank1 */
            EXMC_BANK1->SCTLR3 = BANK1_SCTLR_RESET;
            EXMC_BANK1->STR3 = BANK1_STR_RESET;
            EXMC_BANK1_WT->SWTR3 = BANK1_SWTR_RESET;
            break;
        }
        case EXMC_BANK1_NORSRAM4:
        {
            /* the Region4 of Bank1 */
            EXMC_BANK1->SCTLR4 = BANK1_SCTLR_RESET;
            EXMC_BANK1->STR4 = BANK1_STR_RESET;
            EXMC_BANK1_WT->SWTR4 = BANK1_SWTR_RESET;
            break;
        }
        default:
            break;
    }
}

/**
  * @brief  Reset the Bank2 or Bank3 NAND registers.
  * @param  EXMC_NANDBank: specifies the Bank2 or Bank3 to be used.
  *   This parameter can be one of the following values:
  *     @arg EXMC_BANK2_NAND: Bank2
  *     @arg EXMC_BANK3_NAND: Bank3
  * @retval None
  */
void EXMC_NAND_DeInit(uint32_t EXMC_NANDBank)
{
    if(EXMC_NANDBank == EXMC_BANK2_NAND)
    {
        /* Reset the Bank2 NAND registers */
        EXMC_BANK2->CTLR2 = BANK2_3_CTLR_RESET;
        EXMC_BANK2->SIR2 = BANK2_3_SIR_RESET;
        EXMC_BANK2->COMTR2 = BANK2_3_COMTR_RESET;
        EXMC_BANK2->ATTR2 = BANK2_3_ATTR_RESET;  
    }
    /* EXMC_Bank3_NAND */
    else
    {
        /* Reset the Bank3 NAND registers */
        EXMC_BANK3->CTLR3 = BANK2_3_CTLR_RESET;
        EXMC_BANK3->SIR3 = BANK2_3_SIR_RESET;
        EXMC_BANK3->COMTR3 = BANK2_3_COMTR_RESET;
        EXMC_BANK3->ATTR3 = BANK2_3_ATTR_RESET; 
    }
}

/**
  * @brief  Reset the Bank4 PCCARD registers.
  * @param  None
  * @retval None
  */
void EXMC_PCCARD_DeInit(void)
{
    /* Reset EXMC Bank4 PCCARD registers */
    EXMC_BANK4->CTLR4 = BANK4_CTLR_RESET;
    EXMC_BANK4->SIR4 = BANK4_SIR_RESET;
    EXMC_BANK4->COMTR4 = BANK4_COMTR_RESET;
    EXMC_BANK4->ATTR4 = BANK4_ATTR_RESET;
    EXMC_BANK4->IOTR4 = BANK4_IOTR_RESET;
}

/**
  * @brief  Initializes the EXMC Bank1 NOR/SRAM parameters.
  * @param  EXMC_NORSRAMInitParaStruct : the struct EXMC_NORSRAMInitPara pointer  
  * @retval None
  */
void EXMC_NORSRAM_Init(EXMC_NORSRAMInitPara* EXMC_NORSRAMInitParaStruct)
{ 
    uint32_t temp_SCTLR = 0x00000000 ,temp_STR = 0x00000000 ,temp_SWTR = 0x00000000;

    temp_SCTLR = (uint32_t)EXMC_NORSRAMInitParaStruct->EXMC_AddressDataMux |
                 EXMC_NORSRAMInitParaStruct->EXMC_MemoryType |
                 EXMC_NORSRAMInitParaStruct->EXMC_DatabusWidth |
                 EXMC_NORSRAMInitParaStruct->EXMC_BurstMode |
                 EXMC_NORSRAMInitParaStruct->EXMC_NWAITPolarity |
                 EXMC_NORSRAMInitParaStruct->EXMC_WrapBurstMode |
                 EXMC_NORSRAMInitParaStruct->EXMC_NWAITConfig |
                 EXMC_NORSRAMInitParaStruct->EXMC_MemoryWrite |
                 EXMC_NORSRAMInitParaStruct->EXMC_NWAITSignal |
                 EXMC_NORSRAMInitParaStruct->EXMC_ExtendedMode |
                 EXMC_NORSRAMInitParaStruct->EXMC_AsynWait |
                 EXMC_NORSRAMInitParaStruct->EXMC_WriteMode;

    temp_STR = (uint32_t)EXMC_NORSRAMInitParaStruct->EXMC_ReadWriteTimingParaStruct->EXMC_AsynAddressSetupTime |
               (EXMC_NORSRAMInitParaStruct->EXMC_ReadWriteTimingParaStruct->EXMC_AsynAddressHoldTime << STR_AHT_OFFSET) |
               (EXMC_NORSRAMInitParaStruct->EXMC_ReadWriteTimingParaStruct->EXMC_AsynDataSetupTime << STR_DST_OFFSET) |
               (EXMC_NORSRAMInitParaStruct->EXMC_ReadWriteTimingParaStruct->EXMC_BusLatency << STR_BUSLAT_OFFSET) |
               (EXMC_NORSRAMInitParaStruct->EXMC_ReadWriteTimingParaStruct->EXMC_SynCLKDivision << STR_CDIV_OFFSET) |
               (EXMC_NORSRAMInitParaStruct->EXMC_ReadWriteTimingParaStruct->EXMC_SynDataLatency << STR_DLAT_OFFSET) |
               EXMC_NORSRAMInitParaStruct->EXMC_ReadWriteTimingParaStruct->EXMC_AsynAccessMode;

    if(EXMC_NORSRAMInitParaStruct->EXMC_MemoryType == EXMC_MEMORY_TYPE_NOR)
    {
        temp_SCTLR |= (uint32_t)SCTLR_NOREN_SET;
    }

    if(EXMC_NORSRAMInitParaStruct->EXMC_ExtendedMode == EXMC_EXTENDED_MODE_ENABLE)
    {
        temp_SWTR = (uint32_t)EXMC_NORSRAMInitParaStruct->EXMC_WriteTimingParaStruct->EXMC_AsynAddressSetupTime |
                    (EXMC_NORSRAMInitParaStruct->EXMC_WriteTimingParaStruct->EXMC_AsynAddressHoldTime << SWTR_AHT_OFFSET )|
                    (EXMC_NORSRAMInitParaStruct->EXMC_WriteTimingParaStruct->EXMC_AsynDataSetupTime << SWTR_DST_OFFSET) |
                    (EXMC_NORSRAMInitParaStruct->EXMC_WriteTimingParaStruct->EXMC_SynCLKDivision << SWTR_CDIV_OFFSET) |
                    (EXMC_NORSRAMInitParaStruct->EXMC_WriteTimingParaStruct->EXMC_SynDataLatency << SWTR_DLAT_OFFSET) |
                    EXMC_NORSRAMInitParaStruct->EXMC_WriteTimingParaStruct->EXMC_AsynAccessMode;
    }
    else
    {
        temp_SWTR=BANK1_SWTR_RESET;
    }

    switch(EXMC_NORSRAMInitParaStruct->EXMC_NORSRAMBank)
    {
        case EXMC_BANK1_NORSRAM1:
        {
            /* Bank1 NOR/SRAM Region1 registers configuration */
            EXMC_BANK1->SCTLR1 = temp_SCTLR;
            EXMC_BANK1->STR1 = temp_STR;
            EXMC_BANK1_WT->SWTR1 = temp_SWTR;
            break;
        }
        case EXMC_BANK1_NORSRAM2:
        {
            /* Bank1 NOR/SRAM Region2 registers configuration */
            EXMC_BANK1->SCTLR2 = temp_SCTLR;
            EXMC_BANK1->STR2 = temp_STR;
            EXMC_BANK1_WT->SWTR2 = temp_SWTR;
            break;
        }
        case EXMC_BANK1_NORSRAM3:
        {
            /* Bank1 NOR/SRAM Region3 registers configuration */
            EXMC_BANK1->SCTLR3 = temp_SCTLR;
            EXMC_BANK1->STR3 = temp_STR;
            EXMC_BANK1_WT->SWTR3 = temp_SWTR;
            break;
        }
        case EXMC_BANK1_NORSRAM4:
        {
            /* Bank1 NOR/SRAM Region4 registers configuration */
            EXMC_BANK1->SCTLR4 = temp_SCTLR;
            EXMC_BANK1->STR4 = temp_STR;
            EXMC_BANK1_WT->SWTR4 = temp_SWTR;
            break;
        }
        default:
            break;
  }
}

/**
  * @brief  Initialize the EXMC Bank2 or Bank3 NAND parameters.
  * @param  EXMC_NANDInitParaStruct : the struct EXMC_NANDInitPara pointer  
  * @retval None
  */
void EXMC_NAND_Init(EXMC_NANDInitPara* EXMC_NANDInitParaStruct)
{
    uint32_t temp_CTLR = 0x00000000, temp_COMTR = 0x00000000, temp_ATTR = 0x00000000; 
    
    temp_CTLR = (uint32_t)EXMC_NANDInitParaStruct->EXMC_WaitFeature |
                CTLR_EMTYP_NAND |
                EXMC_NANDInitParaStruct->EXMC_DatabusWidth |
                EXMC_NANDInitParaStruct->EXMC_ECCLogic |
                EXMC_NANDInitParaStruct->EXMC_ECCSize |
                (EXMC_NANDInitParaStruct->EXMC_CTRLatency << CTLR_CTR_OFFSET )|
                (EXMC_NANDInitParaStruct->EXMC_ATRLatency << CTLR_ATR_OFFSET);

    temp_COMTR = (uint32_t)EXMC_NANDInitParaStruct->EXMC_CommonSpaceTimingParaStruct->EXMC_SetupTime |
                (EXMC_NANDInitParaStruct->EXMC_CommonSpaceTimingParaStruct->EXMC_WaitTime << COMTR_COMWT_OFFSET) |
                (EXMC_NANDInitParaStruct->EXMC_CommonSpaceTimingParaStruct->EXMC_HoldTime << COMTR_COMHT_OFFSET)|
                (EXMC_NANDInitParaStruct->EXMC_CommonSpaceTimingParaStruct->EXMC_DatabusHiZTime << COMTR_COMHIZT_OFFSET); 

    temp_ATTR = (uint32_t)EXMC_NANDInitParaStruct->EXMC_AttributeSpaceTimingParaStruct->EXMC_SetupTime |
                (EXMC_NANDInitParaStruct->EXMC_AttributeSpaceTimingParaStruct->EXMC_WaitTime << ATTR_ATTWT_OFFSET) |
                (EXMC_NANDInitParaStruct->EXMC_AttributeSpaceTimingParaStruct->EXMC_HoldTime << ATTR_ATTHT_OFFSET)|
                (EXMC_NANDInitParaStruct->EXMC_AttributeSpaceTimingParaStruct->EXMC_DatabusHiZTime << ATTR_ATTHIZT_OFFSET);

    if(EXMC_NANDInitParaStruct->EXMC_NANDBank == EXMC_BANK2_NAND)
    {
        /* Configure the EXMC_Bank2 NAND registers using the value of parameters */
        EXMC_BANK2->CTLR2 = temp_CTLR;
        EXMC_BANK2->COMTR2 = temp_COMTR;
        EXMC_BANK2->ATTR2 = temp_ATTR;
    }
    else
    {
        /* EXMC_Bank3_NAND registers configuration */
        EXMC_BANK3->CTLR3 = temp_CTLR;
        EXMC_BANK3->COMTR3 = temp_COMTR;
        EXMC_BANK3->ATTR3 = temp_ATTR;
    }
}

/**
  * @brief  Initialize the EXMC Bank4 PCCARD parameters.
  * @param  EXMC_PCCARDInitParaStruct : the struct EXMC_PCCARDInitPara pointer.
  * @retval None
  */
void EXMC_PCCARD_Init(EXMC_PCCARDInitPara* EXMC_PCCARDInitParaStruct)
{
    /* Configure the EXMC_Bank4_PCCARD CTLR4 register using the value of parameters */
    EXMC_BANK4->CTLR4 = (uint32_t)EXMC_PCCARDInitParaStruct->EXMC_WaitFeature |
                        EXMC_DATABUS_WIDTH_16B |  
                        (EXMC_PCCARDInitParaStruct->EXMC_CTRLatency << CTLR_CTR_OFFSET) |
                        (EXMC_PCCARDInitParaStruct->EXMC_ATRLatency << CTLR_ATR_OFFSET);
            
    /* Configure the EXMC_Bank4_PCCARD COMTR4 register using the value of parameters */
    EXMC_BANK4->COMTR4 = (uint32_t)EXMC_PCCARDInitParaStruct->EXMC_CommonSpaceTimingParaStruct->EXMC_SetupTime |
                        (EXMC_PCCARDInitParaStruct->EXMC_CommonSpaceTimingParaStruct->EXMC_WaitTime << COMTR_COMWT_OFFSET) |
                        (EXMC_PCCARDInitParaStruct->EXMC_CommonSpaceTimingParaStruct->EXMC_HoldTime << COMTR_COMHT_OFFSET)|
                        (EXMC_PCCARDInitParaStruct->EXMC_CommonSpaceTimingParaStruct->EXMC_DatabusHiZTime << COMTR_COMHIZT_OFFSET);

    /* Configure the EXMC_Bank4_PCCARD ATTR4 register using the value of parameters */
    EXMC_BANK4->ATTR4 = (uint32_t)EXMC_PCCARDInitParaStruct->EXMC_AttributeSpaceTimingParaStruct->EXMC_SetupTime |
                        (EXMC_PCCARDInitParaStruct->EXMC_AttributeSpaceTimingParaStruct->EXMC_WaitTime << ATTR_ATTWT_OFFSET) |
                        (EXMC_PCCARDInitParaStruct->EXMC_AttributeSpaceTimingParaStruct->EXMC_HoldTime << ATTR_ATTHT_OFFSET)|
                        (EXMC_PCCARDInitParaStruct->EXMC_AttributeSpaceTimingParaStruct->EXMC_DatabusHiZTime << ATTR_ATTHIZT_OFFSET);

    /* Configure the EXMC_Bank4_PCCARD IOTR4 register using the value of parameters */
    EXMC_BANK4->IOTR4 = (uint32_t)EXMC_PCCARDInitParaStruct->EXMC_IOSpaceTimingParaStruct->EXMC_SetupTime |
                        (EXMC_PCCARDInitParaStruct->EXMC_IOSpaceTimingParaStruct->EXMC_WaitTime << IOTR_IOTWT_OFFSET) |
                        (EXMC_PCCARDInitParaStruct->EXMC_IOSpaceTimingParaStruct->EXMC_HoldTime << IOTR_IOHT_OFFSET)|
                        (EXMC_PCCARDInitParaStruct->EXMC_IOSpaceTimingParaStruct->EXMC_DatabusHiZTime << IOTR_IOHIZT_OFFSET);
}

/**
  * @brief  Initialize the struct EXMC_NORSRAMInitPara
  * @param  EXMC_NORSRAMInitParaStruct: the struct EXMC_NORSRAMInitPara pointer
  * @retval None
  */
void EXMC_NORSRAMStruct_Init(EXMC_NORSRAMInitPara* EXMC_NORSRAMInitParaStruct)
{
    EXMC_NORSRAMInitParaStruct->EXMC_NORSRAMBank = EXMC_BANK1_NORSRAM1;
    EXMC_NORSRAMInitParaStruct->EXMC_AddressDataMux = EXMC_ADDRESS_DATA_MUX_ENABLE;
    EXMC_NORSRAMInitParaStruct->EXMC_MemoryType = EXMC_MEMORY_TYPE_SRAM;
    EXMC_NORSRAMInitParaStruct->EXMC_DatabusWidth = EXMC_DATABUS_WIDTH_8B;
    EXMC_NORSRAMInitParaStruct->EXMC_BurstMode = EXMC_BURST_MODE_DISABLE;
    EXMC_NORSRAMInitParaStruct->EXMC_NWAITPolarity = EXMC_NWAIT_POLARITY_LOW;
    EXMC_NORSRAMInitParaStruct->EXMC_WrapBurstMode = EXMC_WRAP_BURST_MODE_DISABLE;
    EXMC_NORSRAMInitParaStruct->EXMC_NWAITConfig = EXMC_NWAIT_CONFIG_BEFORE;
    EXMC_NORSRAMInitParaStruct->EXMC_MemoryWrite = EXMC_MEMORY_WRITE_ENABLE;
    EXMC_NORSRAMInitParaStruct->EXMC_NWAITSignal = EXMC_NWAIT_SIGNAL_ENABLE;
    EXMC_NORSRAMInitParaStruct->EXMC_ExtendedMode = EXMC_EXTENDED_MODE_DISABLE;
    EXMC_NORSRAMInitParaStruct->EXMC_AsynWait = EXMC_ASYN_WAIT_DISABLE;
    EXMC_NORSRAMInitParaStruct->EXMC_WriteMode = EXMC_ASYN_WRITE;
    EXMC_NORSRAMInitParaStruct->EXMC_ReadWriteTimingParaStruct->EXMC_AsynAddressSetupTime = 0xF;
    EXMC_NORSRAMInitParaStruct->EXMC_ReadWriteTimingParaStruct->EXMC_AsynAddressHoldTime = 0xF;
    EXMC_NORSRAMInitParaStruct->EXMC_ReadWriteTimingParaStruct->EXMC_AsynDataSetupTime = 0xFF;
    EXMC_NORSRAMInitParaStruct->EXMC_ReadWriteTimingParaStruct->EXMC_BusLatency = 0xF;
    EXMC_NORSRAMInitParaStruct->EXMC_ReadWriteTimingParaStruct->EXMC_SynCLKDivision = 0xF;
    EXMC_NORSRAMInitParaStruct->EXMC_ReadWriteTimingParaStruct->EXMC_SynDataLatency = 0xF;
    EXMC_NORSRAMInitParaStruct->EXMC_ReadWriteTimingParaStruct->EXMC_AsynAccessMode = EXMC_ACCESS_MODE_A; 
    EXMC_NORSRAMInitParaStruct->EXMC_WriteTimingParaStruct->EXMC_AsynAddressSetupTime = 0xF;
    EXMC_NORSRAMInitParaStruct->EXMC_WriteTimingParaStruct->EXMC_AsynAddressHoldTime = 0xF;
    EXMC_NORSRAMInitParaStruct->EXMC_WriteTimingParaStruct->EXMC_AsynDataSetupTime = 0xFF;
    EXMC_NORSRAMInitParaStruct->EXMC_WriteTimingParaStruct->EXMC_BusLatency = 0xF;
    EXMC_NORSRAMInitParaStruct->EXMC_WriteTimingParaStruct->EXMC_SynCLKDivision = 0xF;
    EXMC_NORSRAMInitParaStruct->EXMC_WriteTimingParaStruct->EXMC_SynDataLatency = 0xF;
    EXMC_NORSRAMInitParaStruct->EXMC_WriteTimingParaStruct->EXMC_AsynAccessMode = EXMC_ACCESS_MODE_A;
}

/**
  * @brief  Initialize the struct EXMC_NANDInitPara
  * @param  EXMC_NANDInitParaStruct: the struct EXMC_NANDInitPara pointer.
  * @retval None
  */
void EXMC_NANDStruct_Init(EXMC_NANDInitPara* EXMC_NANDInitParaStruct)
{ 
    EXMC_NANDInitParaStruct->EXMC_NANDBank = EXMC_BANK2_NAND;
    EXMC_NANDInitParaStruct->EXMC_WaitFeature = EXMC_WAIT_FEATURE_DISABLE;
    EXMC_NANDInitParaStruct->EXMC_DatabusWidth = EXMC_DATABUS_WIDTH_8B;
    EXMC_NANDInitParaStruct->EXMC_ECCLogic = EXMC_ECC_LOGIC_DISABLE;
    EXMC_NANDInitParaStruct->EXMC_ECCSize = EXMC_ECC_SIZE_256BYTES;
    EXMC_NANDInitParaStruct->EXMC_CTRLatency = 0x0;
    EXMC_NANDInitParaStruct->EXMC_ATRLatency = 0x0;
    EXMC_NANDInitParaStruct->EXMC_CommonSpaceTimingParaStruct->EXMC_SetupTime = 0xFC;
    EXMC_NANDInitParaStruct->EXMC_CommonSpaceTimingParaStruct->EXMC_WaitTime = 0xFC;
    EXMC_NANDInitParaStruct->EXMC_CommonSpaceTimingParaStruct->EXMC_HoldTime = 0xFC;
    EXMC_NANDInitParaStruct->EXMC_CommonSpaceTimingParaStruct->EXMC_DatabusHiZTime = 0xFC;
    EXMC_NANDInitParaStruct->EXMC_AttributeSpaceTimingParaStruct->EXMC_SetupTime = 0xFC;
    EXMC_NANDInitParaStruct->EXMC_AttributeSpaceTimingParaStruct->EXMC_WaitTime = 0xFC;
    EXMC_NANDInitParaStruct->EXMC_AttributeSpaceTimingParaStruct->EXMC_HoldTime = 0xFC;
    EXMC_NANDInitParaStruct->EXMC_AttributeSpaceTimingParaStruct->EXMC_DatabusHiZTime = 0xFC;      
}

/**
  * @brief  Initialize the struct EXMC_PCCARDInitParaStruct
  * @param  EXMC_PCCARDInitPara: the struct EXMC_PCCARDInitParaStruct pointer.
  * @retval None
  */
void EXMC_PCCARDStruct_Init(EXMC_PCCARDInitPara* EXMC_PCCARDInitParaStruct)
{
    EXMC_PCCARDInitParaStruct->EXMC_WaitFeature = EXMC_WAIT_FEATURE_DISABLE;
    EXMC_PCCARDInitParaStruct->EXMC_CTRLatency = 0x0;
    EXMC_PCCARDInitParaStruct->EXMC_ATRLatency = 0x0;
    EXMC_PCCARDInitParaStruct->EXMC_CommonSpaceTimingParaStruct->EXMC_SetupTime = 0xFC;
    EXMC_PCCARDInitParaStruct->EXMC_CommonSpaceTimingParaStruct->EXMC_WaitTime = 0xFC;
    EXMC_PCCARDInitParaStruct->EXMC_CommonSpaceTimingParaStruct->EXMC_HoldTime = 0xFC;
    EXMC_PCCARDInitParaStruct->EXMC_CommonSpaceTimingParaStruct->EXMC_DatabusHiZTime = 0xFC;
    EXMC_PCCARDInitParaStruct->EXMC_AttributeSpaceTimingParaStruct->EXMC_SetupTime = 0xFC;
    EXMC_PCCARDInitParaStruct->EXMC_AttributeSpaceTimingParaStruct->EXMC_WaitTime = 0xFC;
    EXMC_PCCARDInitParaStruct->EXMC_AttributeSpaceTimingParaStruct->EXMC_HoldTime = 0xFC;
    EXMC_PCCARDInitParaStruct->EXMC_AttributeSpaceTimingParaStruct->EXMC_DatabusHiZTime = 0xFC;    
    EXMC_PCCARDInitParaStruct->EXMC_IOSpaceTimingParaStruct->EXMC_SetupTime = 0xFC;
    EXMC_PCCARDInitParaStruct->EXMC_IOSpaceTimingParaStruct->EXMC_WaitTime = 0xFC;
    EXMC_PCCARDInitParaStruct->EXMC_IOSpaceTimingParaStruct->EXMC_HoldTime = 0xFC;
    EXMC_PCCARDInitParaStruct->EXMC_IOSpaceTimingParaStruct->EXMC_DatabusHiZTime = 0xFC;
}

/**
  * @brief  Enable or disable the specified Region of Bank1.
  * @param  EXMC_NORSRAMBank: specifies the Region of Bank1.
  *   This parameter can be one of the following values:
  *     @arg EXMC_BANK1_NORSRAM1: the Region1 of Bank1
  *     @arg EXMC_BANK1_NORSRAM2: the Region2 of Bank1
  *     @arg EXMC_BANK1_NORSRAM3: the Region3 of Bank1
  *     @arg EXMC_BANK1_NORSRAM4: the Region4 of Bank1
  * @param  NewValue: new value of the specified Region of Bank1.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void EXMC_NORSRAM_Enable(uint32_t EXMC_NORSRAMBank, TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        /* Enable the selected NOR/SRAM Bank by setting the PBKEN bit in the BCRx register */
        switch(EXMC_NORSRAMBank)
        {
            case EXMC_BANK1_NORSRAM1:
                EXMC_BANK1->SCTLR1 |= SCTLR_BAKEN_SET;
                break;
            case EXMC_BANK1_NORSRAM2:
                EXMC_BANK1->SCTLR2 |= SCTLR_BAKEN_SET;
                break;
            case EXMC_BANK1_NORSRAM3:
                EXMC_BANK1->SCTLR3 |= SCTLR_BAKEN_SET;
                break;
            case EXMC_BANK1_NORSRAM4:
                EXMC_BANK1->SCTLR4 |= SCTLR_BAKEN_SET;
                break;
            default:
                break;
        }
    }
    else
    {
        /* Disable the selected NOR/SRAM Bank by clearing the PBKEN bit in the BCRx register */
        switch(EXMC_NORSRAMBank)
        {
            case EXMC_BANK1_NORSRAM1:
                EXMC_BANK1->SCTLR1 &= SCTLR_BAKEN_RESET;
                break;
            case EXMC_BANK1_NORSRAM2:
                EXMC_BANK1->SCTLR2 &= SCTLR_BAKEN_RESET;
                break;
            case EXMC_BANK1_NORSRAM3:
                EXMC_BANK1->SCTLR3 &= SCTLR_BAKEN_RESET;
                break;
            case EXMC_BANK1_NORSRAM4:
                EXMC_BANK1->SCTLR4 &= SCTLR_BAKEN_RESET;
                break;
            default:
                break;
        }
    }
}

/**
  * @brief  Enable or disable the specified NAND Bank2 or Bank3.
  * @param  EXMC_NANDBank: specifies the NAND Bank.
  *   This parameter can be one of the following values:
  *     @arg EXMC_BANK2_NAND: the NAND Bank2
  *     @arg EXMC_BANK3_NAND: the NAND Bank3
  * @param  NewValue: new value of the specified Bank.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void EXMC_NAND_Enable(uint32_t EXMC_NANDBank, TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        /* Enable the selected NAND Bank by setting the PBKEN bit in the PCRx register */
        if(EXMC_NANDBank == EXMC_BANK2_NAND)
        {
            EXMC_BANK2->CTLR2 |= CTLR_BAKEN_SET;
        }
        else
        {
            EXMC_BANK3->CTLR3 |= CTLR_BAKEN_SET;
        }
    }
    else
    {
        /* Disable the selected NAND Bank by clearing the PBKEN bit in the PCRx register */
        if(EXMC_NANDBank == EXMC_BANK2_NAND)
        {
            EXMC_BANK2->CTLR2 &= CTLR_BAKEN_RESET;
        }
        else
        {
            EXMC_BANK3->CTLR3 &= CTLR_BAKEN_RESET;
        }
    }
}

/**
  * @brief  Enable or disable the Bank4 of PCCARD.
  * @param  NewValue: new value of the PCCARD Bank. 
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void EXMC_PCCARD_Enable(TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        /* Enable the PCCARD Bank */
        EXMC_BANK4->CTLR4 |= CTLR_BAKEN_SET;
    }
    else
    {
        /* Disable the PCCARD Bank */
        EXMC_BANK4->CTLR4 &= CTLR_BAKEN_RESET;
    }
}

/**
  * @brief  Enable or disable the EXMC_CTLR1_ECCEN control bit.
  * @param  EXMC_NANDBank: specifies the NAND Bank.
  *   This parameter can be one of the following values:
  *     @arg EXMC_BANK2_NAND: the NAND Bank2
  *     @arg EXMC_BANK3_NAND: the NAND Bank3
  * @param  NewValue: new value of the EXMC Bank2 or Bank3 ECC calculation.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void EXMC_NANDECC_Enable(uint32_t EXMC_NANDBank, TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        /* Enable the selected NAND Bank ECC function by setting the ECCEN bit in the PCRx register */
        if(EXMC_NANDBank == EXMC_BANK2_NAND)
        {
            EXMC_BANK2->CTLR2 |= CTLR_ECCEN_SET;
        }
        else
        {
            EXMC_BANK3->CTLR3 |= CTLR_ECCEN_SET;
        }
    }
    else
    {
        /* Disable the selected NAND Bank ECC function by clearing the ECCEN bit in the PCRx register */
        if(EXMC_NANDBank == EXMC_BANK2_NAND)
        {
            EXMC_BANK2->CTLR2 &= CTLR_ECCEN_RESET;
        }
        else
        {
            EXMC_BANK3->CTLR3 &= CTLR_ECCEN_RESET;
        }
    }
}

/**
  * @brief  Get the EXMC_ECCR value.
  * @param  EXMC_NANDBank: specifies the NAND Bank.
  *   This parameter can be one of the following values:
  *     @arg EXMC_BANK2_NAND: the NAND Bank2
  *     @arg EXMC_BANK3_NAND: the NAND Bank3
  * @retval The Error Correction Code (ECC) value.
  */
uint32_t EXMC_GetECC(uint32_t EXMC_NANDBank)
{
    if(EXMC_NANDBank == EXMC_BANK2_NAND)
    {
        return(EXMC_BANK2->ECCR2);
    }
    else
    {
        return(EXMC_BANK3->ECCR3);
    }
}

/**
  * @brief  Enable or disable the specified EXMC Bank2/3/4/5/6 interrupts.
  * @param  EXMC_Bank: specifies the NAND Bank , PCCARD Bank or SDRAM Bank.
  *   This parameter can be one of the following values:
  *     @arg EXMC_BANK2_NAND: the NAND Bank2
  *     @arg EXMC_BANK3_NAND: the NAND Bank3
  *     @arg EXMC_BANK4_PCCARD: the PCCARD Bank4
  *     @arg EXMC_BANK5_SDRAM: the SDRAM Bank5
  *     @arg EXMC_BANK6_SDRAM: the SDRAM Bank6
  * @param  EXMC_INT: the EXMC interrupt sources.
  *   This parameter can be any combination of the following values:
  *     @arg EXMC_INT_RISE: Rising edge detection interrupt
  *     @arg EXMC_INT_LEVEL: Level edge detection interrupt
  *     @arg EXMC_INT_FALL: Falling edge detection interrupt
  *     @arg EXMC_INT_REFRESH: Refresh error interrupt
  * @param  NewValue: new value of the specified EXMC interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void EXMC_INTConfig(uint32_t EXMC_Bank, uint32_t EXMC_INT, TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        if(EXMC_Bank == EXMC_BANK2_NAND)
        {
            /* Enable the specified EXMC_Bank2 interrupts */
            EXMC_BANK2->SIR2 |= EXMC_INT;
        }
        else if (EXMC_Bank == EXMC_BANK3_NAND)
        {
            /* Enable the specified EXMC_Bank3 interrupts */
            EXMC_BANK3->SIR3 |= EXMC_INT;
        }
        else if(EXMC_Bank == EXMC_BANK4_PCCARD)
        {
            /* Enable the specified EXMC_Bank4 interrupts */
            EXMC_BANK4->SIR4 |= EXMC_INT;
        }
        else
        {
            /* Enable the specified EXMC_Bank5 or Bank6 interrupts */
            EXMC_BANK5_6->SDARFIR |= EXMC_INT;
        }
    }
    else
    {
        if(EXMC_Bank == EXMC_BANK2_NAND)
        {
            /* Disable the specified EXMC_Bank2 interrupts */
            EXMC_BANK2->SIR2 &= ~((uint32_t)EXMC_INT);
        }
        else if (EXMC_Bank == EXMC_BANK3_NAND)
        {
            /* Disable the specified EXMC_Bank3 interrupts */
            EXMC_BANK3->SIR3 &= ~((uint32_t)EXMC_INT);
        }
        else if(EXMC_Bank == EXMC_BANK4_PCCARD)
        {
            /* Disable the specified EXMC_Bank4 interrupts */
            EXMC_BANK4->SIR4 &= ~((uint32_t)EXMC_INT);
        }
        else
        {
            /* Disable the specified EXMC_Bank5 or Bank6 interrupts */
            EXMC_BANK5_6->SDARFIR &= ~((uint32_t)EXMC_INT);
        }
    }
}

/**
  * @brief  Get the bit flag of EXMC_SIRx or EXMC_SDSTR register.
  * @param  EXMC_Bank: specifies the NAND Bank , PCCARD Bank or SDRAM Bank.
  *   This parameter can be one of the following values: 
  *     @arg EXMC_BANK2_NAND: the NAND Bank2
  *     @arg EXMC_BANK3_NAND: the NAND Bank3
  *     @arg EXMC_BANK4_PCCARD: the PCCARD Bank4
  *     @arg EXMC_BANK5_SDRAM: the SDRAM Bank5
  *     @arg EXMC_BANK6_SDRAM: the SDRAM Bank6
  * @param  EXMC_FLAG: the flag of EXMC_SIRx register.
  *   This parameter can be one of the following values:
  *     @arg EXMC_FLAG_RISE: Rising egde detection flag
  *     @arg EXMC_FLAG_LEVEL: Level detection Flag
  *     @arg EXMC_FLAG_FALL: Falling egde detection flag
  *     @arg EXMC_FLAG_FIFOE: FIFO empty flag
  *     @arg EXMC_FLAG_REFRESH: Refresh error flag.
  *     @arg EXMC_FLAG_READY: Not ready status flag.
  * @retval The new value of EXMC_FLAG (SET or RESET).
  */
TypeState EXMC_GetBitState(uint32_t EXMC_Bank, uint32_t EXMC_FLAG)
{
    uint32_t temp = 0x00000000;

    if(EXMC_Bank == EXMC_BANK2_NAND)
    {
        /* NAND Bank2 */
        temp = EXMC_BANK2->SIR2;
    }
    else if(EXMC_Bank == EXMC_BANK3_NAND)
    {
        /* NAND Bank3 */
        temp = EXMC_BANK3->SIR3;
    }
    else if(EXMC_Bank == EXMC_BANK4_PCCARD)
    {
        /* PCCARD Bank4 */
        temp = EXMC_BANK4->SIR4;
    }
    else
    {
         /* SDRAM Bank5 or Bank6 */
        temp = EXMC_BANK5_6->SDSTR;
    } 
    
    if ((temp & EXMC_FLAG) != (uint32_t)EXMC_FLAG )
    {
        /* EXMC_FLAG is reset */
        return RESET;
    }
    else
    {
        /* EXMC_FLAG is set */
        return SET;
    }
}

/**
  * @brief  Clear the bit flag of EXMC_SIRx or EXMC_SDSTR register.
  * @param  EXMC_Bank: specifies the NAND Bank , PCCARD Bank or SDRAM Bank.
  *   This parameter can be one of the following values:
  *     @arg EXMC_BANK2_NAND: the NAND Bank2
  *     @arg EXMC_BANK3_NAND: the NAND Bank3
  *     @arg EXMC_BANK4_PCCARD: the PCCARD Bank4
  *     @arg EXMC_BANK5_SDRAM: the SDRAM Bank5
  *     @arg EXMC_BANK6_SDRAM: the SDRAM Bank6
  * @param  EXMC_FLAG: the flag of EXMC_SIRx or EXMC_SDSTR register.
  *   This parameter can be any combination of the following values:
  *     @arg EXMC_FLAG_RISE: Rising egde detection flag
  *     @arg EXMC_FLAG_LEVEL: Level detection flag
  *     @arg EXMC_FLAG_FALL: Falling egde detection flag
  *     @arg EXMC_FLAG_REFRESH: Refresh error flag.
  * @retval None
  */
void EXMC_ClearBitState(uint32_t EXMC_Bank, uint32_t EXMC_FLAG)
{
    if(EXMC_Bank == EXMC_BANK2_NAND)
    { 
        /* EXMC Bank2_NAND */
        EXMC_BANK2->SIR2 &= ~EXMC_FLAG; 
    }  
    else if(EXMC_Bank == EXMC_BANK3_NAND)
    {
        /* EXMC Bank3_NAND */
        EXMC_BANK3->SIR3 &= ~EXMC_FLAG;
    }
    else if(EXMC_Bank == EXMC_BANK4_PCCARD)
    {
        /* EXMC_Bank4_PCCARD*/
        EXMC_BANK4->SIR4 &= ~EXMC_FLAG;
    }
    else
    {
        /* EXMC_Bank5 or Bank6 SDRAM */
        EXMC_BANK5_6->SDARFIR |= EXMC_SDARFIR_CRFE;
    }
}

/**
  * @brief  Get the interrupt bit flag.
  * @param  EXMC_Bank: specifies the NAND Bank , PCCARD Bank or SDRAM Bank.
  *   This parameter can be one of the following values:
  *     @arg EXMC_BANK2_NAND: the NAND Bank2
  *     @arg EXMC_BANK3_NAND: the NAND Bank3
  *     @arg EXMC_BANK4_PCCARD: the PCCARD Bank4
  *     @arg EXMC_BANK5_SDRAM: the SDRAM Bank5
  *     @arg EXMC_BANK6_SDRAM: the SDRAM Bank6
  * @param  EXMC_INT: the interrupt bit flag.
  *   This parameter can be one of the following values:
  *     @arg EXMC_INT_RISE: Rising edge detection interrupt
  *     @arg EXMC_INT_LEVEL: Level edge detection interrupt
  *     @arg EXMC_INT_FALL: Falling edge detection interrupt
  *     @arg EXMC_INT_REFRESH: Refresh error interrupt
  * @retval The new value of EXMC_INT (SET or RESET).
  */ 
TypeState EXMC_GetIntBitState(uint32_t EXMC_Bank, uint32_t EXMC_INT)
{
    uint32_t temp_SIR = 0x00000000, temp_INTState = 0x00000000, temp_INTEnable = 0x00000000; 
    
    if(EXMC_Bank == EXMC_BANK2_NAND)
    {
        /* EXMC_Bank2_NAND */
        temp_SIR = EXMC_BANK2->SIR2;
    }  
    else if(EXMC_Bank == EXMC_BANK3_NAND)
    {
        /* EXMC_Bank3_NAND */
        temp_SIR = EXMC_BANK3->SIR3;
    }
    else if(EXMC_Bank == EXMC_BANK4_PCCARD)
    {
        /* EXMC_Bank4_PCCARD */
        temp_SIR = EXMC_BANK4->SIR4;
    } 
    else
    {
        /* EXMC_Bank5 or Bank6 SDRAM */
        temp_SIR = EXMC_BANK5_6->SDARFIR;
    }
    
    /* Interrupt enable or disable status */
    temp_INTEnable = temp_SIR & EXMC_INT;
    
    /* Interrupt flag bit status */
    if((EXMC_Bank==EXMC_BANK5_SDRAM) || (EXMC_Bank==EXMC_BANK6_SDRAM) )
    {
        temp_INTState = EXMC_BANK5_6->SDSTR & EXMC_SDSTR_RFE;
    }
    else
    {
        temp_INTState = temp_SIR & (EXMC_INT >> SIR_INTEN_OFFSET);
    }
    
    if ((temp_INTState != (uint32_t)RESET)  && (temp_INTEnable != (uint32_t)RESET))
    {
        return SET;
    }
    else
    {
        return RESET;
    }
}

/**
  * @brief  Clear the interrupt bit flag.
  * @param  EXMC_Bank: specifies the NAND Bank, PCCARD Bank or SDRAM Banks.
  *   This parameter can be one of the following values:
  *     @arg EXMC_BANK2_NAND: the NAND Bank2
  *     @arg EXMC_BANK3_NAND: the NAND Bank3
  *     @arg EXMC_BANK4_PCCARD: the PCCARD Bank4
  *     @arg EXMC_BANK5_SDRAM: the SDRAM Bank5
  *     @arg EXMC_BANK6_SDRAM: the SDRAM Bank6
  * @param  EXMC_INT: the interrupt bit flag.
  *   This parameter can be any combination of the following values:
  *     @arg EXMC_FLAG_RISE: Rising edge detection interrupt flag
  *     @arg EXMC_FLAG_LEVEL: Level edge detection interrupt flag
  *     @arg EXMC_FLAG_FALL: Falling edge detection interrupt flag
  *     @arg EXMC_FLAG_REFRESH: Refresh error flag
  * @retval None
  */
void EXMC_ClearIntBitState(uint32_t EXMC_Bank, uint32_t EXMC_INT)
{
    if(EXMC_Bank == EXMC_BANK2_NAND)
    {
        /* EXMC_Bank2_NAND */
        EXMC_BANK2->SIR2 &= ~(EXMC_INT >> SIR_INTEN_OFFSET); 
    }
    else if(EXMC_Bank == EXMC_BANK3_NAND)
    {
        /* EXMC_Bank3_NAND */
        EXMC_BANK3->SIR3 &= ~(EXMC_INT >> SIR_INTEN_OFFSET);
    }
    else if(EXMC_Bank == EXMC_BANK4_PCCARD)
    {
        /* EXMC_Bank4_PCCARD */
        EXMC_BANK4->SIR4 &= ~(EXMC_INT >> SIR_INTEN_OFFSET);
    }
    else
    {
        /* EXMC_Bank5 or Bank6 SDRAM */
        EXMC_BANK5_6->SDARFIR |= EXMC_SDARFIR_CRFE;
    }
}

/**
  * @brief  Reset the Bank5 or Bank6 SDRAM registers.
  * @param  EXMC_SDRAMBank: specifies the EXMC Bank5 or Bank6 to be used.
  *   This parameter can be one of the following values:
  *     @arg EXMC_BANK5_SDRAM: the SDRAM Bank5
  *     @arg EXMC_BANK6_SDRAM: the SDRAM Bank6
  * @retval None
  */
void EXMC_SDRAM_DeInit(uint32_t EXMC_SDRAMBank)
{
    if(EXMC_SDRAMBank==EXMC_BANK5_SDRAM)
    {
        EXMC_BANK5_6->SDCTLR1 = BANK5_6_SDCTLR_RESET;
        EXMC_BANK5_6->SDTR1 = BANK5_6_SDTR_RESET;
    }
    else
    {
        EXMC_BANK5_6->SDCTLR2 = BANK5_6_SDCTLR_RESET;
        EXMC_BANK5_6->SDTR2 = BANK5_6_SDTR_RESET;
    }
    EXMC_BANK5_6->SDCMDR = BANK5_6_SDCMDR_RESET;
    EXMC_BANK5_6->SDARFIR = BANK5_6_SDARFIR_RESET;
    EXMC_BANK5_6->SDRSCR = BANK5_6_SDRSCR_RESET;
}

/**
  * @brief  Initialize the EXMC Bank5 or Bank6 SDRAM parameters.
  * @param  EXMC_SDRAMInitParaStruct : the struct EXMC_SDRAMInitPara pointer  
  * @retval None
  */
void EXMC_SDRAM_Init(EXMC_SDRAMInitPara* EXMC_SDRAMInitParaStruct)
{
    uint32_t temp1, temp2;

    /* Configuration EXMC_SDSTLR1 or EXMC_SDCTLR2 */ 
    if(EXMC_SDRAMInitParaStruct->EXMC_SDRAMBank == EXMC_BANK5_SDRAM )
    {
        /* Configuration EXMC_SDCTLR1 */
        EXMC_BANK5_6->SDCTLR1  = (uint32_t)EXMC_SDRAMInitParaStruct->EXMC_ColumnAddressWidth |
                                 EXMC_SDRAMInitParaStruct->EXMC_RowAddressWidth |
                                 EXMC_SDRAMInitParaStruct->EXMC_DataWidth |
                                 EXMC_SDRAMInitParaStruct->EXMC_InternalBankNumber |
                                 EXMC_SDRAMInitParaStruct->EXMC_CASLatency |
                                 EXMC_SDRAMInitParaStruct->EXMC_WriteProtection |
                                 EXMC_SDRAMInitParaStruct->EXMC_SDClockConfig |
                                 EXMC_SDRAMInitParaStruct->EXMC_BrustReadSwitch | 
                                 EXMC_SDRAMInitParaStruct->EXMC_PipelineReadDelay;
        
        /* Configuration EXMC_SDTR1 */
        EXMC_BANK5_6->SDTR1 = (uint32_t)((EXMC_SDRAMInitParaStruct->EXMC_SDRAMTimingParaStruct->EXMC_LoadModeRegisterDelay)-1) |
                              (((EXMC_SDRAMInitParaStruct->EXMC_SDRAMTimingParaStruct->EXMC_ExitSelfRefreshDelay)-1) << SDTR_XSRD_OFFSET) |
                              (((EXMC_SDRAMInitParaStruct->EXMC_SDRAMTimingParaStruct->EXMC_RowAddressSelectDelay)-1) << SDTR_RASD_OFFSET) |
                              (((EXMC_SDRAMInitParaStruct->EXMC_SDRAMTimingParaStruct->EXMC_AutoRefreshDelay)-1) << SDTR_ARFD_OFFSET) |
                              (((EXMC_SDRAMInitParaStruct->EXMC_SDRAMTimingParaStruct->EXMC_WriteRecoveryDelay)-1) << SDTR_WRD_OFFSET) |
                              (((EXMC_SDRAMInitParaStruct->EXMC_SDRAMTimingParaStruct->EXMC_RowPrechargeDelay)-1) << SDTR_RPD_OFFSET) |
                              (((EXMC_SDRAMInitParaStruct->EXMC_SDRAMTimingParaStruct->EXMC_RowToColumnDelay)-1) << SDTR_RCD_OFFSET);
    }
    else
    {
        /* Configuration EXMC_SDCTLR1 and EXMC_SDCTLR2 */
        /* Some bits in the EXMC_SDCTLR2 register are reserved */
        temp1 = EXMC_BANK5_6->SDCTLR1 & (~( EXMC_SDCTLR1_PRD | EXMC_SDCTLR1_BR | EXMC_SDCTLR1_MCLK ));
        
        temp1 |= (uint32_t)EXMC_SDRAMInitParaStruct->EXMC_SDClockConfig |
                          EXMC_SDRAMInitParaStruct->EXMC_BrustReadSwitch | 
                          EXMC_SDRAMInitParaStruct->EXMC_PipelineReadDelay;
        
        temp2 = (uint32_t)EXMC_SDRAMInitParaStruct->EXMC_ColumnAddressWidth |
                          EXMC_SDRAMInitParaStruct->EXMC_RowAddressWidth |
                          EXMC_SDRAMInitParaStruct->EXMC_DataWidth |
                          EXMC_SDRAMInitParaStruct->EXMC_InternalBankNumber |
                          EXMC_SDRAMInitParaStruct->EXMC_CASLatency |
                          EXMC_SDRAMInitParaStruct->EXMC_WriteProtection ;

        EXMC_BANK5_6->SDCTLR1 = temp1;
        EXMC_BANK5_6->SDCTLR2 = temp2;
        
        /* Configuration EXMC_SDTR1 and EXMC_SDTR2 */
        /* Some bits in the EXMC_SDTR2 register are reserved */
        temp1 = EXMC_BANK5_6->SDTR1 & (~(EXMC_SDTR1_ARFD | EXMC_SDTR1_RPD));

        temp1 |= (uint32_t)(((EXMC_SDRAMInitParaStruct->EXMC_SDRAMTimingParaStruct->EXMC_AutoRefreshDelay)-1) << SDTR_ARFD_OFFSET) |
                          (((EXMC_SDRAMInitParaStruct->EXMC_SDRAMTimingParaStruct->EXMC_RowPrechargeDelay)-1) << SDTR_RPD_OFFSET) |
                          (((EXMC_SDRAMInitParaStruct->EXMC_SDRAMTimingParaStruct->EXMC_WriteRecoveryDelay)-1) << SDTR_WRD_OFFSET);

        temp2 = (uint32_t)((EXMC_SDRAMInitParaStruct->EXMC_SDRAMTimingParaStruct->EXMC_LoadModeRegisterDelay)-1) |
                          (((EXMC_SDRAMInitParaStruct->EXMC_SDRAMTimingParaStruct->EXMC_ExitSelfRefreshDelay)-1) << SDTR_XSRD_OFFSET) |
                          (((EXMC_SDRAMInitParaStruct->EXMC_SDRAMTimingParaStruct->EXMC_RowAddressSelectDelay)-1) << SDTR_RASD_OFFSET) |
                          (((EXMC_SDRAMInitParaStruct->EXMC_SDRAMTimingParaStruct->EXMC_RowToColumnDelay)-1) << SDTR_RCD_OFFSET);    

        EXMC_BANK5_6->SDTR1 = temp1;
        EXMC_BANK5_6->SDTR2 = temp2;
    }
}


/**
  * @brief  Initialize the struct EXMC_SDRAMInitPara
  * @param  EXMC_SDRAMInitParaStruct: the struct EXMC_SDRAMInitPara pointer.
  * @retval None
  */
void EXMC_SDRAMStruct_Init(EXMC_SDRAMInitPara* EXMC_SDRAMInitParaStruct)
{
    EXMC_SDRAMInitParaStruct->EXMC_SDRAMBank = EXMC_BANK5_SDRAM;
    EXMC_SDRAMInitParaStruct->EXMC_ColumnAddressWidth = EXMC_COLUMN_ADDRESS_WIDTH_8;
    EXMC_SDRAMInitParaStruct->EXMC_RowAddressWidth = EXMC_ROW_ADDRESS_WIDTH_11; 
    EXMC_SDRAMInitParaStruct->EXMC_DataWidth = EXMC_DATA_WIDTH_16;
    EXMC_SDRAMInitParaStruct->EXMC_InternalBankNumber = EXMC_INTERNAL_BANK_NUMBER_4; 
    EXMC_SDRAMInitParaStruct->EXMC_CASLatency = EXMC_CAS_LATENCY_1;  
    EXMC_SDRAMInitParaStruct->EXMC_WriteProtection = EXMC_WRITE_PROTECTION_ENABLE;
    EXMC_SDRAMInitParaStruct->EXMC_SDClockConfig = EXMC_SDCLK_DISABLE;
    EXMC_SDRAMInitParaStruct->EXMC_BrustReadSwitch = EXMC_BRUST_READ_DISABLE;
    EXMC_SDRAMInitParaStruct->EXMC_PipelineReadDelay = EXMC_PIPELINE_READ_DELAY_0; 

    EXMC_SDRAMInitParaStruct->EXMC_SDRAMTimingParaStruct->EXMC_LoadModeRegisterDelay = 16;
    EXMC_SDRAMInitParaStruct->EXMC_SDRAMTimingParaStruct->EXMC_ExitSelfRefreshDelay = 16;
    EXMC_SDRAMInitParaStruct->EXMC_SDRAMTimingParaStruct->EXMC_RowAddressSelectDelay = 16;
    EXMC_SDRAMInitParaStruct->EXMC_SDRAMTimingParaStruct->EXMC_AutoRefreshDelay = 16;
    EXMC_SDRAMInitParaStruct->EXMC_SDRAMTimingParaStruct->EXMC_WriteRecoveryDelay = 16;
    EXMC_SDRAMInitParaStruct->EXMC_SDRAMTimingParaStruct->EXMC_RowPrechargeDelay = 16;
    EXMC_SDRAMInitParaStruct->EXMC_SDRAMTimingParaStruct->EXMC_RowToColumnDelay = 16;
}

/**
  * @brief  Configure the SDRAM memory command
  * @param  EXMC_SDRAMCommandParaStruct: the struct EXMC_SDRAMCommandPara pointer.
  * @retval None
  */
void EXMC_SDRAM_CmdConfig(EXMC_SDRAMCommandPara* EXMC_SDRAMCommandParaStruct)
{
    EXMC_BANK5_6->SDCMDR = (uint32_t)((EXMC_SDRAMCommandParaStruct->EXMC_Command) |
                                      (EXMC_SDRAMCommandParaStruct->EXMC_BankSelect) |
                                      (((EXMC_SDRAMCommandParaStruct->EXMC_AutoRefreshNumber)-1)<<SDCMDR_NARF_OFFSET) |
                                      ((EXMC_SDRAMCommandParaStruct->EXMC_ModeRegisterContent)<<SDCMDR_MRC_OFFSET) );
}

/**
  * @brief  Set auto-refresh interval
  * @param  EXMC_Count: the number SDRAM clock cycles unit between two successive auto-refresh commands.
  * @retval None
  */
void EXMC_SDRAM_SetRefreshCount(uint32_t EXMC_Count)
{
    uint32_t temp;
    temp = EXMC_BANK5_6->SDARFIR & (~EXMC_SDARFIR_ARFITV);
    EXMC_BANK5_6->SDARFIR = temp | (uint32_t)((EXMC_Count << SDARFIR_ARFITV_OFFSET) & EXMC_SDARFIR_ARFITV);
}

/**
  * @brief  Set the number of successive auto-refresh command
  * @param  EXMC_Number: number of successive auto-refresh.
  * @retval None
  */
void EXMC_SDRAM_SetAutoRefreshNumber(uint32_t EXMC_Number)
{
    uint32_t temp;
    temp = EXMC_BANK5_6->SDCMDR & (~EXMC_SDCMDR_NARF);
    EXMC_BANK5_6->SDCMDR = temp | (uint32_t)((EXMC_Number << SDCMDR_NARF_OFFSET) & EXMC_SDCMDR_NARF) ;
}

/**
  * @brief  Enable or disable the write protection function.
  * @param  EXMC_SDRAMBank: specifies the SDRAM Bank.
  *   This parameter can be one of the following values:
  *     @arg EXMC_BANK5_SDRAM: the SDRAM Bank5
  *     @arg EXMC_BANK6_SDRAM: the SDRAM Bank6
  * @param  NewValue: new value of the EXMC Bank5 or Bank6 write protection.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void EXMC_SDRAM_WriteProtection_Enable(uint32_t EXMC_SDRAMBank, TypeState NewValue)
{
    if(EXMC_SDRAMBank==EXMC_BANK5_SDRAM)
    {
        if (NewValue != DISABLE)
        {
            EXMC_BANK5_6->SDCTLR1 |= EXMC_WRITE_PROTECTION_ENABLE;
        }
        else
        {
            EXMC_BANK5_6->SDCTLR1 &= ~((uint32_t)EXMC_SDCTLR1_WP);
        }
    }
    else
    {
        if (NewValue != DISABLE)
        {
            EXMC_BANK5_6->SDCTLR2 |= EXMC_WRITE_PROTECTION_ENABLE;
        }
        else
        {
            EXMC_BANK5_6->SDCTLR2 &= ~((uint32_t)EXMC_SDCTLR2_WP);
        }
    }
}

/**
  * @brief  Get the status of SDRAM Bank5 or Bank6.
  * @param  EXMC_SDRAMBank: specifies the SDRAM Bank.
  *   This parameter can be one of the following values:
  *     @arg EXMC_BANK5_SDRAM: the SDRAM Bank5
  *     @arg EXMC_BANK6_SDRAM: the SDRAM Bank6
  * @retval the status of SDRAM Bank
  *   This parameter can be one of the following values:
  *     @arg EXMC_BANK_STATUS_NORMAL : Normal status
  *     @arg EXMC_BANK_STATUS_SELF_REFRESH:  Self-refresh status
  *     @arg EXMC_BANK_STATUS_POWER_DOWN: Power-down status
  */
uint32_t EXMC_SDRAM_GetBankStatus(uint32_t EXMC_SDRAMBank)
{
    uint32_t temp = 0;

    if(EXMC_SDRAMBank == EXMC_BANK5_SDRAM)
    {
        temp = ((uint32_t)(EXMC_BANK5_6->SDSTR & EXMC_SDSTR_STA1) >> SDSTR_STA1_OFFSET);
    }
    else
    {
        temp = ((uint32_t)(EXMC_BANK5_6->SDSTR & EXMC_SDSTR_STA2) >> SDSTR_STA2_OFFSET);
    }

    return temp;
}

/**
  * @brief  Configure the delayed sample clock of read data
  * @param  EXMC_DelayCell: specifies the number of clock delay unit.
  * @param  EXMC_ExtraHCLK: specifies extra HCLK cycle to the read data sample clock besides the delay chain.
  * @retval None.
  */
void EXMC_SDRAM_ReadSampleConfig(uint32_t EXMC_DelayCell, uint32_t EXMC_ExtraHCLK)
{
    uint32_t temp;
    
    temp=EXMC_BANK5_6->SDRSCR & (~EXMC_SDRSCR_SDSC | EXMC_SDRSCR_SSCR);
    temp |= (uint32_t)((EXMC_DelayCell << SDRSCR_SDSC_OFFSET) & EXMC_SDRSCR_SDSC) |
                      ((EXMC_ExtraHCLK << SDRSCR_SSCR_OFFSET) & EXMC_SDRSCR_SSCR);
    EXMC_BANK5_6->SDRSCR = temp;
}

/**
  * @brief  Enable or disable read sample 
  * @param  NewValue: new value of read sample. 
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void EXMC_SDRAM_ReadSample_Enable(TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        EXMC_BANK5_6->SDRSCR |=  EXMC_SDRSCR_RSEN;
    }
    else
    {
        EXMC_BANK5_6->SDRSCR &= (uint32_t)(~EXMC_SDRSCR_RSEN);
    }
}

/**
  * @brief  Reset the Bank1 SQPIPSRAM registers.
  * @param  None
  * @retval None
  */
void EXMC_SQPIPSRAM_DeInit(void)
{
    EXMC_BANK1_SQPI->SINIR = BANK1_SQPI_SINIR_RESET; 
    EXMC_BANK1_SQPI->SRCR = BANK1_SQPI_SRCR_RESET;
    EXMC_BANK1_SQPI->SWCR = BANK1_SQPI_SWCR_RESET;
    EXMC_BANK1_SQPI->SIDLR = BANK1_SQPI_SIDLR_RESET;
    EXMC_BANK1_SQPI->SIDHR = BANK1_SQPI_SIDHR_RESET;
}

/**
  * @brief  Initialize the Bank1 SQPIPSRAM parameters.
  * @param  EXMC_SQPIPSRAMInitParaStruct : the struct EXMC_SQPIPSRAMInitPara pointer  
  * @retval None
  */
void EXMC_SQPIPSRAM_Init(EXMC_SQPIPSRAMInitPara* EXMC_SQPIPSRAMInitParaStruct)
{
    EXMC_BANK1_SQPI->SINIR = (uint32_t)EXMC_SQPIPSRAMInitParaStruct->EXMC_SamplePolarity |
                                       EXMC_SQPIPSRAMInitParaStruct->EXMC_IDLength |
                                       ((EXMC_SQPIPSRAMInitParaStruct->EXMC_AddressBits) << SINIR_ADRBIT_OFFSET) |
                                       EXMC_SQPIPSRAMInitParaStruct->EXMC_CommandBits;
}

/**
  * @brief  Initialize the struct EXMC_SQPIPSRAMInitPara
  * @param  EXMC_SQPIPSRAMInitParaStruct: the struct EXMC_SQPIPSRAMInitPara pointer.
  * @retval None
  */
void EXMC_SQPIPSRAMStruct_Init(EXMC_SQPIPSRAMInitPara* EXMC_SQPIPSRAMInitParaStruct)
{
    EXMC_SQPIPSRAMInitParaStruct->EXMC_SamplePolarity = EXMC_SAMPLE_RISE;
    EXMC_SQPIPSRAMInitParaStruct->EXMC_IDLength = EXMC_ID_LENGTH_64;
    EXMC_SQPIPSRAMInitParaStruct->EXMC_AddressBits = 24;
    EXMC_SQPIPSRAMInitParaStruct->EXMC_CommandBits = EXMC_COMMAND_BITS_8;
}

/**
  * @brief  Set the read command
  * @param  EXMC_RCmdMode: specifies the read command mode
  *   This parameter can be one of the following values:
  *     @arg EXMC_CmdMode_NOTSPI: not SPI
  *     @arg EXMC_CmdMode_SPI: SPI mode
  *     @arg EXMC_CmdMode_SQPI: SQPI mode
  *     @arg EXMC_CmdMode_QPI: QPI mode
  * @param  EXMC_RWaitCycle: the number of read wait cycle after address phase.
  * @param  EXMC_RCmdCode: read command code for AHB read transfer
  * @retval None
  */
void EXMC_SQPIPSRAM_SetReadCmd(uint32_t EXMC_RCmdMode,uint32_t EXMC_RWaitCycle,uint32_t EXMC_RCmdCode)
{
    uint32_t temp;
    
    temp = (uint32_t) EXMC_RCmdMode |
                     ((EXMC_RWaitCycle << SRCR_RWAITCYCLE_OFFSET) & EXMC_SRCR_RWAITCYCLE) |
                     ((EXMC_RCmdCode & EXMC_SRCR_RCMD));
    EXMC_BANK1_SQPI->SRCR = temp;
}

/**
  * @brief  Set the write command
  * @param  EXMC_WCmdMode: specifies the write command mode
  *   This parameter can be one of the following values:
  *     @arg EXMC_CmdMode_NOTSPI: not SPI
  *     @arg EXMC_CmdMode_SPI: SPI mode
  *     @arg EXMC_CmdMode_SQPI: SQPI mode
  *     @arg EXMC_CmdMode_QPI: QPI mode
  * @param  EXMC_WWaitCycle: the number of write wait cycle after address phase.
  * @param  EXMC_WCmdCode: write command code for AHB write transfer
  * @retval None
  */
void EXMC_SQPIPSRAM_SetWriteCmd(uint32_t EXMC_WCmdMode,uint32_t EXMC_WWaitCycle,uint32_t EXMC_WCmdCode)
{
    uint32_t temp;
    
    temp = (uint32_t) EXMC_WCmdMode |
                     ((EXMC_WWaitCycle << SWCR_WWAITCYCLE_OFFSET) & EXMC_SWCR_WWAITCYCLE) |
                     ((EXMC_WCmdCode & EXMC_SWCR_WCMD));
    EXMC_BANK1_SQPI->SWCR = temp;
}


/**
  * @brief  Get the EXMC SPI ID low data.
  * @param  None
  * @retval The ID low data.
  */
uint32_t EXMC_SQPIPSRAM_GetIDLow(void)
{

    return (EXMC_BANK1_SQPI->SIDLR);
}

/**
  * @brief  Get the EXMC SPI ID high data.
  * @param  None
  * @retval The ID high data.
  */
uint32_t EXMC_SQPIPSRAM_GetIDHigh(void)
{
    return (EXMC_BANK1_SQPI->SIDHR);
}

/**
  * @brief  Get the bit value of EXMC send write command bit or read ID command.
  * @param  EXMC_SendCmdFlag: specifies the send command flag
  *   This parameter can be one of the following values:
  *     @arg EXMC_SendCmdFlag_RDID: EXMC_SRCR_RDID bit
  *     @arg EXMC_SendCmdFlag_SC: EXMC_SWCR_SC bit
  * @retval The new value of EXMC_SendCmdFlag (SET or RESET).
  */
TypeState EXMC_SQPIPSRAM_GetSendCmdState(uint32_t EXMC_SendCmdFlag)
{
    uint32_t temp = 0x00000000;
    
    if(EXMC_SendCmdFlag == EXMC_SRCR_RDID) 
    {
        temp = EXMC_BANK1_SQPI->SRCR ;
    }
    else if(EXMC_SendCmdFlag == EXMC_SWCR_SC)
    {
        temp = EXMC_BANK1_SQPI->SWCR ;
    }
    
    if ((temp & EXMC_SendCmdFlag) != (uint32_t)RESET )
    {
        /* EXMC_SendCmdFlag is set */
        return SET;
    }
    else
    {
        /* EXMC_SendCmdFlag is reset */
        return RESET;
    }
}

/**
  * @brief  Send SPI read ID Command
  * @param  None.
  * @retval None
  */
void EXMC_SQPIPSRAM_SendReadIDCmd(void)
{
    EXMC_BANK1_SQPI->SRCR |= EXMC_SRCR_RDID;
}

/**
  * @brief  Send SPI special command which does not have address and data phase
  * @param  None.
  * @retval None
  */
void EXmC_SQPIPSRAM_SendWriteCmd(void)
{
    EXMC_BANK1_SQPI->SWCR |= EXMC_SWCR_SC;
}

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
