/**
  ******************************************************************************
  * @file    gd32f20x_hash.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    15-Jul-2015
  * @brief   HAU header file of the firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F20x_HASH_H
#define __GD32F20x_HASH_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gd32f20x.h"

/** @addtogroup GD32F20x_Firmware
  * @{
  */

/** @addtogroup HAU
  * @{
  */ 

/** @defgroup HAU_Exported_Types
  * @{
  */

/** 
  * @brief   HAU Init structure definition
  */ 
typedef struct
{
    uint32_t HAU_AlgoSelection; /*!< SHA-1, SHA-224, SHA-256 or MD5. This parameter
                                    can be a value of @ref HASH_Algo_Selection */
    uint32_t HAU_AlgoMode;      /*!< HASH or HMAC. This parameter can be a value 
                                    of @ref HASH_processor_Algorithm_Mode */
    uint32_t HAU_DataType;      /*!< 32-bit data, 16-bit data, 8-bit data or 
                                    bit string. This parameter can be a value of
                                    @ref HASH_Data_Type */
    uint32_t HAU_HMACKeyType;   /*!< HMAC Short key or HMAC Long Key. This parameter
                                    can be a value of @ref HASH_HMAC_Long_key_only_for_HMAC_mode */
}HAU_InitPara;

/** 
  * @brief  HAU message digest result structure definition  
  */ 
typedef struct
{
    uint32_t Data[8];      /*!< Message digest result : 8x 32bit wors for SHA-256,
                                                        7x 32bit wors for SHA-224,
                                                        5x 32bit words for SHA-1 or
                                                        4x 32bit words for MD5  */
} HAU_MsgDigest; 

/**
  * @}
  */

/** @defgroup HAU_Exported_Constants
  * @{
  */ 

/** @defgroup HAU_Algo_Selection 
  * @{
  */ 
#define HAU_ALGOSELECTION_SHA1           ((uint32_t)0x0000)  /*!< HAU function is SHA1   */
#define HAU_ALGOSELECTION_SHA224         HAU_CTLR_ALGS_1     /*!< HAU function is SHA224 */
#define HAU_ALGOSELECTION_SHA256         HAU_CTLR_ALGS       /*!< HAU function is SHA256 */
#define HAU_ALGOSELECTION_MD5            HAU_CTLR_ALGS_0     /*!< HAU function is MD5    */

/**
  * @}
  */

/** @defgroup HAU_processor_Algorithm_Mode 
  * @{
  */ 
#define HAU_ALGOMODE_HASH                ((uint32_t)0x00000000) /*!< Algorithm is HASH */ 
#define HAU_ALGOMODE_HMAC                HAU_CTLR_HMS           /*!< Algorithm is HMAC */

/**
  * @}
  */

/** @defgroup HAU_Data_Type  
  * @{
  */  
#define HAU_DATAS_32BIT                  ((uint32_t)0x0000) /*!< 32-bit data. No swapping                     */
#define HAU_DATAS_16BIT                  HAU_CTLR_DATAS_0   /*!< 16-bit data. Each half word is swapped       */
#define HAU_DATAS_8BIT                   HAU_CTLR_DATAS_1   /*!< 8-bit data. All bytes are swapped            */
#define HAU_DATAS_1BIT                   HAU_CTLR_DATAS     /*!< 1-bit data. In the word all bits are swapped */

/**
  * @}
  */

/** @defgroup HAU_HMAC_Long_key_only_for_HMAC_mode  
  * @{
  */ 
#define HAU_HMACKEYTYPE_SHORTKEY         ((uint32_t)0x00000000) /*!< HMAC Key is <= 64 bytes */
#define HAU_HMACKEYTYPE_LONGKEY          HAU_CTLR_LKS           /*!< HMAC Key is > 64 bytes  */

/**
  * @}
  */
  
/** @defgroup HAU_interrupts_definition   
  * @{
  */  
#define HAU_INT_DIIE                     HAU_IER_DIIE   /*!< A new block can be entered into the input buffer (DI) */
#define HAU_INT_CCIE                     HAU_IER_CCIE   /*!< Calculation complete                            */
  
/**
  * @}
  */

/** @defgroup HAU_flags_definition   
  * @{
  */  
#define HAU_FLAG_DIIF                    HAU_IFR_DIIF   /*!< 16 locations are free in the DI : A new block can be entered into the input buffer  */
#define HAU_FLAG_CCIF                    HAU_IFR_CCIF   /*!< Digest calculation complete                                                         */
#define HAU_FLAG_DMAF                    HAU_IFR_DMAF   /*!< DMA interface is enabled (DMAE=1) or a transfer is ongoing                          */
#define HAU_FLAG_BSY                     HAU_IFR_BSY    /*!< The HAU core is Busy : processing a block of data                                   */
#define HAU_FLAG_DINE                    HAU_CTLR_DINE  /*!< DI not empty : The input buffer contains at least one word of data                  */                         

/**
  * @}
  */ 

/**
  * @}
  */ 

/** @defgroup HAU_Exported_Functions
  * @{
  */  
/*  Function used to set the HAU configuration to the default reset state ****/
void HAU_DeInit(void);

/* HAU Configuration function ************************************************/
void HAU_Init(HAU_InitPara* HAU_InitParaStruct);
void HAU_ParaInit(HAU_InitPara* HAU_InitParaStruct);
void HAU_Reset(void);

/* HAU Message Digest generation functions ***********************************/
void HAU_PutData(uint32_t Data);
uint8_t HAU_GetInFIFOWordsNbr(void);
void HAU_SetLastWordValidBitsNbr(uint16_t ValidNumber);
void HAU_StartDigest(void);
void HAU_AutoStartDigest(TypeState NewValue);
void HAU_GetDigest(HAU_MsgDigest* HAU_MessageDigest);

/* HAU DMA interface function ************************************************/
void HAU_DMA_Enable(TypeState NewValue);

/* HAU Interrupts and flags management functions *****************************/
void HAU_INTConfig(uint32_t HAU_INT, TypeState NewValue);
TypeState HAU_GetBitState(uint32_t HAU_FLAG);
void HAU_ClearBitState(uint32_t HAU_FLAG);
TypeState HAU_GetIntBitState(uint32_t HAU_INT);
void HAU_ClearIntBitState(uint32_t HAU_INT);

/* High Level SHA1 functions **************************************************/
TypeState HAU_SHA1(uint8_t *Input, uint32_t Ilen, uint8_t Output[20]);
TypeState HMAC_SHA1(uint8_t *Key, uint32_t Keylen,
                    uint8_t *Input, uint32_t Ilen,
                    uint8_t Output[20]);
                    
/* High Level SHA224 functions ***************************************************/
TypeState HAU_SHA224(uint8_t *Input, uint32_t Ilen, uint8_t Output[28]);
TypeState HMAC_SHA224(uint8_t *Key, uint32_t Keylen,
                    uint8_t *Input, uint32_t Ilen,
                    uint8_t Output[28]);                    

/* High Level SHA256 functions ***************************************************/
TypeState HAU_SHA256(uint8_t *Input, uint32_t Ilen, uint8_t Output[32]);
TypeState HMAC_SHA256(uint8_t *Key, uint32_t Keylen,
                    uint8_t *Input, uint32_t Ilen,
                    uint8_t Output[32]);
                    
/* High Level MD5 functions ***************************************************/
TypeState HAU_MD5(uint8_t *Input, uint32_t Ilen, uint8_t Output[16]);
TypeState HMAC_MD5(uint8_t *Key, uint32_t Keylen,
                   uint8_t *Input, uint32_t Ilen,
                   uint8_t Output[16]);

#ifdef __cplusplus
}
#endif

#endif /*__GD32F20x_HASH_H */

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
