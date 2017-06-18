/**
  ******************************************************************************
  * @file    gd32f20x_hash_sha.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    15-Jul-2015
  * @brief   HAU high level functions of the firmware library.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f20x_hash.h"

/** @addtogroup GD32F20x_Firmware
  * @{
  */

/** @defgroup HAU 
  * @brief HAU driver modules
  * @{
  */

/* Private define ------------------------------------------------------------*/
#define SHABSY_TIMEOUT    ((uint32_t) 0x00010000)

/* Private functions ---------------------------------------------------------*/

/** @defgroup HAU_Private_Functions
  * @{
  */ 
  
/**
  * @brief  Compute the HAU SHA1 digest.
  * @param  Input: pointer to the Input buffer.
  * @param  Ilen: length of the Input buffer.
  * @param  Output: the result digest
  * @retval The value of result (SUCCESS OR ERROR).
  */
TypeState HAU_SHA1(uint8_t *Input, uint32_t Ilen, uint8_t Output[20])
{
    HAU_InitPara SHA1_HAU_InitStructure;
    HAU_MsgDigest SHA1_MessageDigest;
    
    __IO uint16_t nbvalidbitsdata = 0;
    uint32_t i = 0;
    __IO uint32_t counter = 0;
    uint32_t busystatus = 0;
    uint32_t inputaddr  = (uint32_t)Input;
    uint32_t outputaddr = (uint32_t)Output;

    /* Number of valid bits in last word of the Input data */
    nbvalidbitsdata = 8 * (Ilen % 4);

    /* HAU peripheral initialization */
    HAU_DeInit();

    /* HAU Configuration */
    SHA1_HAU_InitStructure.HAU_AlgoSelection = HAU_ALGOSELECTION_SHA1;
    SHA1_HAU_InitStructure.HAU_AlgoMode = HAU_ALGOMODE_HASH;
    SHA1_HAU_InitStructure.HAU_DataType = HAU_DATAS_8BIT;
    HAU_Init(&SHA1_HAU_InitStructure);

    /* Configure the number of valid bits in last word of the data */
    HAU_SetLastWordValidBitsNbr(nbvalidbitsdata);

    /* Write the Input block in the IN FIFO */
    for(i=0; i<Ilen; i+=4)
    {
        HAU_PutData(*(uint32_t*)inputaddr);
        inputaddr+=4;
    }

    /* Start the HAU processor */
    HAU_StartDigest();

    /* wait until the Busy flag is RESET */
    do
    {
        busystatus = HAU_GetBitState(HAU_FLAG_BSY);
        counter++;
    }while ((counter != SHABSY_TIMEOUT) && (busystatus != RESET));

    if (busystatus != RESET)
    {
        return ERROR;
    }
    else
    {
        /* Read the message digest */
        HAU_GetDigest(&SHA1_MessageDigest);
        *(uint32_t*)(outputaddr)  = __REV(SHA1_MessageDigest.Data[0]);
        outputaddr+=4;
        *(uint32_t*)(outputaddr)  = __REV(SHA1_MessageDigest.Data[1]);
        outputaddr+=4;
        *(uint32_t*)(outputaddr)  = __REV(SHA1_MessageDigest.Data[2]);
        outputaddr+=4;
        *(uint32_t*)(outputaddr)  = __REV(SHA1_MessageDigest.Data[3]);
        outputaddr+=4;
        *(uint32_t*)(outputaddr)  = __REV(SHA1_MessageDigest.Data[4]);
    }
    return SUCCESS;
}

/**
  * @brief  Compute the HMAC SHA1 digest.
  * @param  Key: pointer to the Key used for HMAC.
  * @param  Keylen: length of the Key used for HMAC.  
  * @param  Input: pointer to the Input buffer.
  * @param  Ilen: length of the Input buffer.
  * @param  Output: the result digest
  * @retval The value of result (SUCCESS OR ERROR).
  */
TypeState HMAC_SHA1(uint8_t *Key, uint32_t Keylen, uint8_t *Input,
                      uint32_t Ilen, uint8_t Output[20])
{
    HAU_InitPara SHA1_HAU_InitStructure;
    HAU_MsgDigest SHA1_MessageDigest;

    __IO uint16_t nbvalidbitsdata = 0;
    __IO uint16_t nbvalidbitskey = 0;
    uint32_t i = 0;
    __IO uint32_t counter = 0;
    uint32_t busystatus = 0;
    uint32_t keyaddr    = (uint32_t)Key;
    uint32_t inputaddr  = (uint32_t)Input;
    uint32_t outputaddr = (uint32_t)Output;

    /* Number of valid bits in last word of the Input data */
    nbvalidbitsdata = 8 * (Ilen % 4);

    /* Number of valid bits in last word of the Key */
    nbvalidbitskey = 8 * (Keylen % 4);

    /* HAU peripheral initialization */
    HAU_DeInit();

    /* HAU Configuration */
    SHA1_HAU_InitStructure.HAU_AlgoSelection = HAU_ALGOSELECTION_SHA1;
    SHA1_HAU_InitStructure.HAU_AlgoMode = HAU_ALGOMODE_HMAC;
    SHA1_HAU_InitStructure.HAU_DataType = HAU_DATAS_8BIT;
    if(Keylen > 64)
    {
        /* HMAC long Key */
        SHA1_HAU_InitStructure.HAU_HMACKeyType = HAU_HMACKEYTYPE_LONGKEY;
    }
    else
    {
        /* HMAC short Key */
        SHA1_HAU_InitStructure.HAU_HMACKeyType = HAU_HMACKEYTYPE_SHORTKEY;
    }
    HAU_Init(&SHA1_HAU_InitStructure);

    /* Configure the number of valid bits in last word of the Key */
    HAU_SetLastWordValidBitsNbr(nbvalidbitskey);

    /* Write the Key */
    for(i=0; i<Keylen; i+=4)
    {
        HAU_PutData(*(uint32_t*)keyaddr);
        keyaddr+=4;
    }

    /* Start the HAU processor */
    HAU_StartDigest();

    /* Wait until the Busy flag is RESET */
    do
    {
        busystatus = HAU_GetBitState(HAU_FLAG_BSY);
        counter++;
    }while ((counter != SHABSY_TIMEOUT) && (busystatus != RESET));

    if (busystatus != RESET)
    {
        return ERROR;
    }
    else
    {
        /* Configure the number of valid bits in last word of the Input data */
        HAU_SetLastWordValidBitsNbr(nbvalidbitsdata);

        /* Write the Input block in the IN FIFO */
        for(i=0; i<Ilen; i+=4)
        {
            HAU_PutData(*(uint32_t*)inputaddr);
            inputaddr+=4;
        }

        /* Start the HAU processor */
        HAU_StartDigest();


        /* Wait until the Busy flag is RESET */
        counter =0;
        do
        {
            busystatus = HAU_GetBitState(HAU_FLAG_BSY);
            counter++;
        }while ((counter != SHABSY_TIMEOUT) && (busystatus != RESET));

        if (busystatus != RESET)
        {
            return ERROR;
        }
        else
        {  
            /* Configure the number of valid bits in last word of the Key */
            HAU_SetLastWordValidBitsNbr(nbvalidbitskey);

            /* Write the Key */
            keyaddr = (uint32_t)Key;
            for(i=0; i<Keylen; i+=4)
            {
                HAU_PutData(*(uint32_t*)keyaddr);
                keyaddr+=4;
            }

            /* Start the HAU processor */
            HAU_StartDigest();

            /* Wait until the Busy flag is RESET */
            counter =0;
            do
            {
                busystatus = HAU_GetBitState(HAU_FLAG_BSY);
                counter++;
            }while ((counter != SHABSY_TIMEOUT) && (busystatus != RESET));

            if (busystatus != RESET)
            {
                return ERROR;
            }
            else
            {
                /* Read the message digest */
                HAU_GetDigest(&SHA1_MessageDigest);
                *(uint32_t*)(outputaddr)  = __REV(SHA1_MessageDigest.Data[0]);
                outputaddr+=4;
                *(uint32_t*)(outputaddr)  = __REV(SHA1_MessageDigest.Data[1]);
                outputaddr+=4;
                *(uint32_t*)(outputaddr)  = __REV(SHA1_MessageDigest.Data[2]);
                outputaddr+=4;
                *(uint32_t*)(outputaddr)  = __REV(SHA1_MessageDigest.Data[3]);
                outputaddr+=4;
                *(uint32_t*)(outputaddr)  = __REV(SHA1_MessageDigest.Data[4]);
            }
        }  
    }
    return SUCCESS;  
}

/**
  * @brief  Compute the HAU SHA224 digest.
  * @param  Input: pointer to the Input buffer.
  * @param  Ilen: length of the Input buffer.
  * @param  Output: the result digest
  * @retval The value of result (SUCCESS OR ERROR).
  */
TypeState HAU_SHA224(uint8_t *Input, uint32_t Ilen, uint8_t Output[28])
{
    HAU_InitPara SHA224_HAU_InitStructure;
    HAU_MsgDigest SHA224_MessageDigest;
    
    __IO uint16_t nbvalidbitsdata = 0;
    uint32_t i = 0;
    __IO uint32_t counter = 0;
    uint32_t busystatus = 0;
    uint32_t inputaddr  = (uint32_t)Input;
    uint32_t outputaddr = (uint32_t)Output;

    /* Number of valid bits in last word of the Input data */
    nbvalidbitsdata = 8 * (Ilen % 4);

    /* HAU peripheral initialization */
    HAU_DeInit();

    /* HAU Configuration */
    SHA224_HAU_InitStructure.HAU_AlgoSelection = HAU_ALGOSELECTION_SHA224;
    SHA224_HAU_InitStructure.HAU_AlgoMode = HAU_ALGOMODE_HASH;
    SHA224_HAU_InitStructure.HAU_DataType = HAU_DATAS_8BIT;
    HAU_Init(&SHA224_HAU_InitStructure);

    /* Configure the number of valid bits in last word of the data */
    HAU_SetLastWordValidBitsNbr(nbvalidbitsdata);

    /* Write the Input block in the IN FIFO */
    for(i=0; i<Ilen; i+=4)
    {
        HAU_PutData(*(uint32_t*)inputaddr);
        inputaddr+=4;
    }

    /* Start the HAU processor */
    HAU_StartDigest();

    /* wait until the Busy flag is RESET */
    do
    {
        busystatus = HAU_GetBitState(HAU_FLAG_BSY);
        counter++;
    }while ((counter != SHABSY_TIMEOUT) && (busystatus != RESET));

    if (busystatus != RESET)
    {
        return ERROR;
    }
    else
    {
        /* Read the message digest */
        HAU_GetDigest(&SHA224_MessageDigest);
        *(uint32_t*)(outputaddr)  = __REV(SHA224_MessageDigest.Data[0]);
        outputaddr+=4;
        *(uint32_t*)(outputaddr)  = __REV(SHA224_MessageDigest.Data[1]);
        outputaddr+=4;
        *(uint32_t*)(outputaddr)  = __REV(SHA224_MessageDigest.Data[2]);
        outputaddr+=4;
        *(uint32_t*)(outputaddr)  = __REV(SHA224_MessageDigest.Data[3]);
        outputaddr+=4;
        *(uint32_t*)(outputaddr)  = __REV(SHA224_MessageDigest.Data[4]);
        outputaddr+=4;
        *(uint32_t*)(outputaddr)  = __REV(SHA224_MessageDigest.Data[5]);
        outputaddr+=4;
        *(uint32_t*)(outputaddr)  = __REV(SHA224_MessageDigest.Data[6]);
    }
    return SUCCESS;
}

/**
  * @brief  Compute the HMAC SHA224 digest.
  * @param  Key: pointer to the Key used for HMAC.
  * @param  Keylen: length of the Key used for HMAC.  
  * @param  Input: pointer to the Input buffer.
  * @param  Ilen: length of the Input buffer.
  * @param  Output: the result digest
  * @retval The value of result (SUCCESS OR ERROR).
  */
TypeState HMAC_SHA224(uint8_t *Key, uint32_t Keylen, uint8_t *Input,
                      uint32_t Ilen, uint8_t Output[28])
{
    HAU_InitPara SHA224_HAU_InitStructure;
    HAU_MsgDigest SHA224_MessageDigest;

    __IO uint16_t nbvalidbitsdata = 0;
    __IO uint16_t nbvalidbitskey = 0;
    uint32_t i = 0;
    __IO uint32_t counter = 0;
    uint32_t busystatus = 0;
    uint32_t keyaddr    = (uint32_t)Key;
    uint32_t inputaddr  = (uint32_t)Input;
    uint32_t outputaddr = (uint32_t)Output;

    /* Number of valid bits in last word of the Input data */
    nbvalidbitsdata = 8 * (Ilen % 4);

    /* Number of valid bits in last word of the Key */
    nbvalidbitskey = 8 * (Keylen % 4);

    /* HAU peripheral initialization */
    HAU_DeInit();

    /* HAU Configuration */
    SHA224_HAU_InitStructure.HAU_AlgoSelection = HAU_ALGOSELECTION_SHA224;
    SHA224_HAU_InitStructure.HAU_AlgoMode = HAU_ALGOMODE_HMAC;
    SHA224_HAU_InitStructure.HAU_DataType = HAU_DATAS_8BIT;
    if(Keylen > 64)
    {
        /* HMAC long Key */
        SHA224_HAU_InitStructure.HAU_HMACKeyType = HAU_HMACKEYTYPE_LONGKEY;
    }
    else
    {
        /* HMAC short Key */
        SHA224_HAU_InitStructure.HAU_HMACKeyType = HAU_HMACKEYTYPE_SHORTKEY;
    }
    HAU_Init(&SHA224_HAU_InitStructure);

    /* Configure the number of valid bits in last word of the Key */
    HAU_SetLastWordValidBitsNbr(nbvalidbitskey);

    /* Write the Key */
    for(i=0; i<Keylen; i+=4)
    {
        HAU_PutData(*(uint32_t*)keyaddr);
        keyaddr+=4;
    }

    /* Start the HAU processor */
    HAU_StartDigest();

    /* Wait until the Busy flag is RESET */
    do
    {
        busystatus = HAU_GetBitState(HAU_FLAG_BSY);
        counter++;
    }while ((counter != SHABSY_TIMEOUT) && (busystatus != RESET));

    if (busystatus != RESET)
    {
        return ERROR;
    }
    else
    {
        /* Configure the number of valid bits in last word of the Input data */
        HAU_SetLastWordValidBitsNbr(nbvalidbitsdata);

        /* Write the Input block in the IN FIFO */
        for(i=0; i<Ilen; i+=4)
        {
            HAU_PutData(*(uint32_t*)inputaddr);
            inputaddr+=4;
        }

        /* Start the HAU processor */
        HAU_StartDigest();


        /* Wait until the Busy flag is RESET */
        counter =0;
        do
        {
            busystatus = HAU_GetBitState(HAU_FLAG_BSY);
            counter++;
        }while ((counter != SHABSY_TIMEOUT) && (busystatus != RESET));

        if (busystatus != RESET)
        {
            return ERROR;
        }
        else
        {  
            /* Configure the number of valid bits in last word of the Key */
            HAU_SetLastWordValidBitsNbr(nbvalidbitskey);

            /* Write the Key */
            keyaddr = (uint32_t)Key;
            for(i=0; i<Keylen; i+=4)
            {
                HAU_PutData(*(uint32_t*)keyaddr);
                keyaddr+=4;
            }

            /* Start the HAU processor */
            HAU_StartDigest();

            /* Wait until the Busy flag is RESET */
            counter =0;
            do
            {
                busystatus = HAU_GetBitState(HAU_FLAG_BSY);
                counter++;
            }while ((counter != SHABSY_TIMEOUT) && (busystatus != RESET));

            if (busystatus != RESET)
            {
                return ERROR;
            }
            else
            {
                /* Read the message digest */
                HAU_GetDigest(&SHA224_MessageDigest);
                *(uint32_t*)(outputaddr)  = __REV(SHA224_MessageDigest.Data[0]);
                outputaddr+=4;
                *(uint32_t*)(outputaddr)  = __REV(SHA224_MessageDigest.Data[1]);
                outputaddr+=4;
                *(uint32_t*)(outputaddr)  = __REV(SHA224_MessageDigest.Data[2]);
                outputaddr+=4;
                *(uint32_t*)(outputaddr)  = __REV(SHA224_MessageDigest.Data[3]);
                outputaddr+=4;
                *(uint32_t*)(outputaddr)  = __REV(SHA224_MessageDigest.Data[4]);
                outputaddr+=4;
                *(uint32_t*)(outputaddr)  = __REV(SHA224_MessageDigest.Data[5]);
                outputaddr+=4;
                *(uint32_t*)(outputaddr)  = __REV(SHA224_MessageDigest.Data[6]);
            }
        }  
    }
    return SUCCESS;  
}

/**
  * @brief  Compute the HAU SHA256 digest.
  * @param  Input: pointer to the Input buffer.
  * @param  Ilen: length of the Input buffer.
  * @param  Output: the result digest
  * @retval The value of result (SUCCESS OR ERROR).
  */
TypeState HAU_SHA256(uint8_t *Input, uint32_t Ilen, uint8_t Output[32])
{
    HAU_InitPara SHA256_HAU_InitStructure;
    HAU_MsgDigest SHA256_MessageDigest;
    
    __IO uint16_t nbvalidbitsdata = 0;
    uint32_t i = 0;
    __IO uint32_t counter = 0;
    uint32_t busystatus = 0;
    uint32_t inputaddr  = (uint32_t)Input;
    uint32_t outputaddr = (uint32_t)Output;

    /* Number of valid bits in last word of the Input data */
    nbvalidbitsdata = 8 * (Ilen % 4);

    /* HAU peripheral initialization */
    HAU_DeInit();

    /* HAU Configuration */
    SHA256_HAU_InitStructure.HAU_AlgoSelection = HAU_ALGOSELECTION_SHA256;
    SHA256_HAU_InitStructure.HAU_AlgoMode = HAU_ALGOMODE_HASH;
    SHA256_HAU_InitStructure.HAU_DataType = HAU_DATAS_8BIT;
    HAU_Init(&SHA256_HAU_InitStructure);

    /* Configure the number of valid bits in last word of the data */
    HAU_SetLastWordValidBitsNbr(nbvalidbitsdata);

    /* Write the Input block in the IN FIFO */
    for(i=0; i<Ilen; i+=4)
    {
        HAU_PutData(*(uint32_t*)inputaddr);
        inputaddr+=4;
    }

    /* Start the HAU processor */
    HAU_StartDigest();

    /* wait until the Busy flag is RESET */
    do
    {
        busystatus = HAU_GetBitState(HAU_FLAG_BSY);
        counter++;
    }while ((counter != SHABSY_TIMEOUT) && (busystatus != RESET));

    if (busystatus != RESET)
    {
        return ERROR;
    }
    else
    {
        /* Read the message digest */
        HAU_GetDigest(&SHA256_MessageDigest);
        *(uint32_t*)(outputaddr)  = __REV(SHA256_MessageDigest.Data[0]);
        outputaddr+=4;
        *(uint32_t*)(outputaddr)  = __REV(SHA256_MessageDigest.Data[1]);
        outputaddr+=4;
        *(uint32_t*)(outputaddr)  = __REV(SHA256_MessageDigest.Data[2]);
        outputaddr+=4;
        *(uint32_t*)(outputaddr)  = __REV(SHA256_MessageDigest.Data[3]);
        outputaddr+=4;
        *(uint32_t*)(outputaddr)  = __REV(SHA256_MessageDigest.Data[4]);
        outputaddr+=4;
        *(uint32_t*)(outputaddr)  = __REV(SHA256_MessageDigest.Data[5]);
        outputaddr+=4;
        *(uint32_t*)(outputaddr)  = __REV(SHA256_MessageDigest.Data[6]);
        outputaddr+=4;
        *(uint32_t*)(outputaddr)  = __REV(SHA256_MessageDigest.Data[7]);
    }
    return SUCCESS;
}

/**
  * @brief  Compute the HMAC SHA256 digest.
  * @param  Key: pointer to the Key used for HMAC.
  * @param  Keylen: length of the Key used for HMAC.  
  * @param  Input: pointer to the Input buffer.
  * @param  Ilen: length of the Input buffer.
  * @param  Output: the result digest
  * @retval The value of result (SUCCESS OR ERROR).
  */
TypeState HMAC_SHA256(uint8_t *Key, uint32_t Keylen, uint8_t *Input,
                      uint32_t Ilen, uint8_t Output[32])
{
    HAU_InitPara SHA256_HAU_InitStructure;
    HAU_MsgDigest SHA256_MessageDigest;

    __IO uint16_t nbvalidbitsdata = 0;
    __IO uint16_t nbvalidbitskey = 0;
    uint32_t i = 0;
    __IO uint32_t counter = 0;
    uint32_t busystatus = 0;
    uint32_t keyaddr    = (uint32_t)Key;
    uint32_t inputaddr  = (uint32_t)Input;
    uint32_t outputaddr = (uint32_t)Output;

    /* Number of valid bits in last word of the Input data */
    nbvalidbitsdata = 8 * (Ilen % 4);

    /* Number of valid bits in last word of the Key */
    nbvalidbitskey = 8 * (Keylen % 4);

    /* HAU peripheral initialization */
    HAU_DeInit();

    /* HAU Configuration */
    SHA256_HAU_InitStructure.HAU_AlgoSelection = HAU_ALGOSELECTION_SHA256;
    SHA256_HAU_InitStructure.HAU_AlgoMode = HAU_ALGOMODE_HMAC;
    SHA256_HAU_InitStructure.HAU_DataType = HAU_DATAS_8BIT;
    if(Keylen > 64)
    {
        /* HMAC long Key */
        SHA256_HAU_InitStructure.HAU_HMACKeyType = HAU_HMACKEYTYPE_LONGKEY;
    }
    else
    {
        /* HMAC short Key */
        SHA256_HAU_InitStructure.HAU_HMACKeyType = HAU_HMACKEYTYPE_SHORTKEY;
    }
    HAU_Init(&SHA256_HAU_InitStructure);

    /* Configure the number of valid bits in last word of the Key */
    HAU_SetLastWordValidBitsNbr(nbvalidbitskey);

    /* Write the Key */
    for(i=0; i<Keylen; i+=4)
    {
        HAU_PutData(*(uint32_t*)keyaddr);
        keyaddr+=4;
    }

    /* Start the HAU processor */
    HAU_StartDigest();

    /* Wait until the Busy flag is RESET */
    do
    {
        busystatus = HAU_GetBitState(HAU_FLAG_BSY);
        counter++;
    }while ((counter != SHABSY_TIMEOUT) && (busystatus != RESET));

    if (busystatus != RESET)
    {
        return ERROR;
    }
    else
    {
        /* Configure the number of valid bits in last word of the Input data */
        HAU_SetLastWordValidBitsNbr(nbvalidbitsdata);

        /* Write the Input block in the IN FIFO */
        for(i=0; i<Ilen; i+=4)
        {
            HAU_PutData(*(uint32_t*)inputaddr);
            inputaddr+=4;
        }

        /* Start the HAU processor */
        HAU_StartDigest();


        /* Wait until the Busy flag is RESET */
        counter =0;
        do
        {
            busystatus = HAU_GetBitState(HAU_FLAG_BSY);
            counter++;
        }while ((counter != SHABSY_TIMEOUT) && (busystatus != RESET));

        if (busystatus != RESET)
        {
            return ERROR;
        }
        else
        {  
            /* Configure the number of valid bits in last word of the Key */
            HAU_SetLastWordValidBitsNbr(nbvalidbitskey);

            /* Write the Key */
            keyaddr = (uint32_t)Key;
            for(i=0; i<Keylen; i+=4)
            {
                HAU_PutData(*(uint32_t*)keyaddr);
                keyaddr+=4;
            }

            /* Start the HAU processor */
            HAU_StartDigest();

            /* Wait until the Busy flag is RESET */
            counter =0;
            do
            {
                busystatus = HAU_GetBitState(HAU_FLAG_BSY);
                counter++;
            }while ((counter != SHABSY_TIMEOUT) && (busystatus != RESET));

            if (busystatus != RESET)
            {
                return ERROR;
            }
            else
            {
                /* Read the message digest */
                HAU_GetDigest(&SHA256_MessageDigest);
                *(uint32_t*)(outputaddr)  = __REV(SHA256_MessageDigest.Data[0]);
                outputaddr+=4;
                *(uint32_t*)(outputaddr)  = __REV(SHA256_MessageDigest.Data[1]);
                outputaddr+=4;
                *(uint32_t*)(outputaddr)  = __REV(SHA256_MessageDigest.Data[2]);
                outputaddr+=4;
                *(uint32_t*)(outputaddr)  = __REV(SHA256_MessageDigest.Data[3]);
                outputaddr+=4;
                *(uint32_t*)(outputaddr)  = __REV(SHA256_MessageDigest.Data[4]);
                outputaddr+=4;
                *(uint32_t*)(outputaddr)  = __REV(SHA256_MessageDigest.Data[5]);
                outputaddr+=4;
                *(uint32_t*)(outputaddr)  = __REV(SHA256_MessageDigest.Data[6]);
                outputaddr+=4;
                *(uint32_t*)(outputaddr)  = __REV(SHA256_MessageDigest.Data[7]);
            }
        }  
    }
    return SUCCESS;  
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
