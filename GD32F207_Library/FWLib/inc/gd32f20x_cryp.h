/**
  ******************************************************************************
  * @file    gd32f20x_cryp.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    15-Jul-2015
  * @brief   CAU header file of the firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F20x_CRYP_H
#define __GD32F20x_CRYP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gd32f20x.h"

/** @addtogroup GD32F20x_Firmware
  * @{
  */

/** @addtogroup CAU
  * @{
  */ 

/** @defgroup CAU_Exported_Types
  * @{
  */

/** 
  * @brief   CAU Init structure definition  
  */ 
typedef struct
{
    uint32_t CAU_AlgoDir;   /*!< Encrypt or Decrypt. This parameter can be a 
                                value of @ref CAU_Algorithm_Direction */
    uint32_t CAU_AlgoMode;  /*!< TDES-ECB, TDES-CBC, DES-ECB, DES-CBC, AES-ECB, 
                                AES-CBC, AES-CTR, AES-Key.
                                This parameter can be a value of @ref CAU_Algorithm_Mode */
    uint32_t CAU_DataType;  /*!< 32-bit data, 16-bit data, bit data or bit string.
                                This parameter can be a value of @ref CAU_Data_Type */ 
    uint32_t CAU_KeySize;   /*!< Used only in AES mode only : 128, 192 or 256 bit 
                                key length. This parameter can be a value of 
                                @ref CAU_Key_Size_for_AES_only */
}CAU_InitPara;

/** 
  * @brief   CAU Key(s) structure definition  
  */ 
typedef struct
{
    uint32_t CAU_Key0High;  /*!< Key 0 High */
    uint32_t CAU_Key0Low;   /*!< Key 0 Low  */
    uint32_t CAU_Key1High;  /*!< Key 1 High */
    uint32_t CAU_Key1Low;   /*!< Key 1 Low  */
    uint32_t CAU_Key2High;  /*!< Key 2 High */
    uint32_t CAU_Key2Low;   /*!< Key 2 Low  */
    uint32_t CAU_Key3High;  /*!< Key 3 High */
    uint32_t CAU_Key3Low;   /*!< Key 3 Low  */
}CAU_KeyInitPara;

/** 
  * @brief   CAU Initialization Vectors (IV) structure definition  
  */ 
typedef struct
{
    uint32_t CAU_IV0High;   /*!< Init Vector 0 High */
    uint32_t CAU_IV0Low;    /*!< Init Vector 0 Low  */
    uint32_t CAU_IV1High;   /*!< Init Vector 1 High */
    uint32_t CAU_IV1Low;    /*!< Init Vector 1 Low  */
}CAU_IVInitPara;

/**
  * @}
  */

/** @defgroup CAU_Exported_Constants
  * @{
  */

/** @defgroup CAU_Algorithm_Direction 
  * @{
  */
#define CAU_ALGODIR_ENCRYPT      ((uint16_t)0x0000)
#define CAU_ALGODIR_DECRYPT      ((uint16_t)0x0004)

/**
  * @}
  */ 
 
/** @defgroup CAU_Algorithm_Mode 
  * @{
  */

/*!< TDES Modes */
#define CAU_ALGOMODE_TDES_ECB    ((uint32_t)0x00000000)
#define CAU_ALGOMODE_TDES_CBC    ((uint32_t)0x00000008)

/*!< DES Modes */
#define CAU_ALGOMODE_DES_ECB     ((uint32_t)0x00000010)
#define CAU_ALGOMODE_DES_CBC     ((uint32_t)0x00000018)

/*!< AES Modes */
#define CAU_ALGOMODE_AES_ECB     ((uint32_t)0x00000020)
#define CAU_ALGOMODE_AES_CBC     ((uint32_t)0x00000028)
#define CAU_ALGOMODE_AES_CTR     ((uint32_t)0x00000030)
#define CAU_ALGOMODE_AES_KEY     ((uint32_t)0x00000038)

/**
  * @}
  */ 

/** @defgroup CAU_Data_Type 
  * @{
  */
#define CAU_DATAS_32BIT          ((uint16_t)0x0000)
#define CAU_DATAS_16BIT          ((uint16_t)0x0040)
#define CAU_DATAS_8BIT           ((uint16_t)0x0080)
#define CAU_DATAS_1BIT           ((uint16_t)0x00C0)

/**
  * @}
  */
                                     
/** @defgroup CAU_Key_Size_for_AES_only 
  * @{
  */
#define CAU_KEYSIZE_128BIT       ((uint16_t)0x0000)
#define CAU_KEYSIZE_192BIT       ((uint16_t)0x0100)
#define CAU_KEYSIZE_256BIT       ((uint16_t)0x0200)

/**
  * @}
  */

/** @defgroup CAU_flags_definition 
  * @{
  */
#define CAU_FLAG_BSY             ((uint8_t)0x10)    /*!< The CAU core is currently 
                                                        processing a block of data 
                                                        or a key preparation (for 
                                                        AES decryption). */
#define CAU_FLAG_IFEF            ((uint8_t)0x01)    /*!< Input Fifo Empty */
#define CAU_FLAG_IFNFF           ((uint8_t)0x02)    /*!< Input Fifo is Not Full */
#define CAU_FLAG_INIF            ((uint8_t)0x22)    /*!< IN FIFO interrupt status */
#define CAU_FLAG_OFNEF           ((uint8_t)0x04)    /*!< OUT FIFO not empty */
#define CAU_FLAG_OFFF            ((uint8_t)0x08)    /*!< Output Fifo is Full */
#define CAU_FLAG_OUTIF           ((uint8_t)0x21)    /*!< OUT FIFO interrupt status */

/**
  * @}
  */

/** @defgroup CAU_interrupts_definition 
  * @{
  */
#define CAU_INT_INIE             ((uint8_t)0x01)    /*!< IN Fifo Interrupt */
#define CAU_INT_OUTIE            ((uint8_t)0x02)    /*!< OUT Fifo Interrupt */

/**
  * @}
  */

/** @defgroup CAU_Encryption_Decryption_modes_definition 
  * @{
  */
#define MODE_ENCRYPT             ((uint8_t)0x01)
#define MODE_DECRYPT             ((uint8_t)0x00)

/**
  * @}
  */

/** @defgroup CAU_DMA_transfer_requests 
  * @{
  */
#define CAU_DMAREQ_DATAIN        ((uint8_t)0x01)
#define CAU_DMAREQ_DATAOUT       ((uint8_t)0x02)

/**
  * @}
  */ 

/**
  * @}
  */ 

/** @defgroup CAU_Exported_Functions
  * @{
  */
/*  Function used to set the CAU configuration to the default reset state ****/
void CAU_DeInit(void);

/* CAU Initialization and Configuration functions ****************************/
void CAU_Init(CAU_InitPara* CAU_InitParaStruct);
void CAU_ParaInit(CAU_InitPara* CAU_InitParaStruct);
void CAU_KeyInit(CAU_KeyInitPara* CAU_KeyInitParaStruct);
void CAU_KeyStructInit(CAU_KeyInitPara* CAU_KeyInitParaStruct);
void CAU_IVInit(CAU_IVInitPara* CAU_IVInitParaStruct);
void CAU_IVStructInit(CAU_IVInitPara* CAU_IVInitParaStruct);
void CAU_Enable(TypeState NewValue);
void CAU_FIFOFlush(void);
/* CAU Data processing functions *********************************************/
void CAU_PutData(uint32_t Data);
uint32_t CAU_GetData(void);

/* CAU DMA interface function ************************************************/
void CAU_DMA_Enable(uint8_t CAU_DMAReq, TypeState NewValue);

/* Interrupts and flags management functions **********************************/
void CAU_INTConfig(uint8_t CAU_INT, TypeState NewValue);
TypeState CAU_GetIntBitState(uint8_t CAU_INT);
TypeState CAU_GetEnableState(void);
TypeState CAU_GetBitState(uint8_t CAU_FLAG);

/* High Level AES functions **************************************************/
TypeState CAU_AES_ECB(uint8_t Mode,
                      uint8_t *Key, uint16_t Keysize,
                      uint8_t *Input, uint32_t Ilength,
                      uint8_t *Output);

TypeState CAU_AES_CBC(uint8_t Mode,
                      uint8_t InitVectors[16],
                      uint8_t *Key, uint16_t Keysize,
                      uint8_t *Input, uint32_t Ilength,
                      uint8_t *Output);

TypeState CAU_AES_CTR(uint8_t Mode,
                      uint8_t InitVectors[16],
                      uint8_t *Key, uint16_t Keysize,
                      uint8_t *Input, uint32_t Ilength,
                      uint8_t *Output);

/* High Level TDES functions **************************************************/
TypeState CAU_TDES_ECB(uint8_t Mode,
                       uint8_t Key[24], 
                       uint8_t *Input, uint32_t Ilength,
                       uint8_t *Output);

TypeState CAU_TDES_CBC(uint8_t Mode,
                       uint8_t Key[24],
                       uint8_t InitVectors[8],
                       uint8_t *Input, uint32_t Ilength,
                       uint8_t *Output);

/* High Level DES functions **************************************************/
TypeState CAU_DES_ECB(uint8_t Mode,
                      uint8_t Key[8],
                      uint8_t *Input, uint32_t Ilength,
                      uint8_t *Output);

TypeState CAU_DES_CBC(uint8_t Mode,
                      uint8_t Key[8],
                      uint8_t InitVectors[8],
                      uint8_t *Input,uint32_t Ilength,
                      uint8_t *Output);

#ifdef __cplusplus
}
#endif

#endif /*__GD32F20x_CRYP_H */

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
