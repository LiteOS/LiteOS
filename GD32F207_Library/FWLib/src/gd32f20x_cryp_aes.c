/**
  ******************************************************************************
  * @file    gd32f20x_cryp_aes.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    15-Jul-2015
  * @brief   CAU high level functions of the firmware library.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f20x_cryp.h"

/** @addtogroup GD32F20x_Firmware
  * @{
  */

/** @defgroup CAU 
  * @brief CAU driver modules
  * @{
  */

/* Private define ------------------------------------------------------------*/
#define AESBSY_TIMEOUT    ((uint32_t) 0x00010000)

/* Private functions ---------------------------------------------------------*/

/** @defgroup CAU_Private_Functions
  * @{
  */ 

/**
  * @brief  Encrypt and decrypt using AES in ECB Mode
  * @param  Mode: encryption or decryption Mode (MODE_ENCRYPT or MODE_DECRYPT).
  * @param  Key: Key used for AES algorithm.
  * @param  Keysize: length of the Key, must be a 128, 192 or 256.
  * @param  Input: pointer to the Input buffer.
  * @param  Ilength: length of the Input buffer, must be a multiple of 16.
  * @param  Output: pointer to the returned buffer.
  * @retval The value of result (SUCCESS OR ERROR).
  */
TypeState CAU_AES_ECB(uint8_t Mode, uint8_t* Key, uint16_t Keysize,
                         uint8_t* Input, uint32_t Ilength, uint8_t* Output)
{
    CAU_InitPara AES_CAU_InitStructure;
    CAU_KeyInitPara AES_CAU_KeyInitStructure;
    __IO uint32_t counter = 0;
    uint32_t busystatus = 0;
    uint32_t keyaddr    = (uint32_t)Key;
    uint32_t inputaddr  = (uint32_t)Input;
    uint32_t outputaddr = (uint32_t)Output;
    uint32_t i = 0;

    /* CAU structures initialisation*/
    CAU_KeyStructInit(&AES_CAU_KeyInitStructure);

    switch(Keysize)
    {
        case 128:
        AES_CAU_InitStructure.CAU_KeySize = CAU_KEYSIZE_128BIT;
        AES_CAU_KeyInitStructure.CAU_Key2High = __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key2Low= __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key3High = __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key3Low= __REV(*(uint32_t*)(keyaddr));
        break;
        case 192:
        AES_CAU_InitStructure.CAU_KeySize  = CAU_KEYSIZE_192BIT;
        AES_CAU_KeyInitStructure.CAU_Key1High = __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key1Low= __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key2High = __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key2Low= __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key3High = __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key3Low= __REV(*(uint32_t*)(keyaddr));
        break;
        case 256:
        AES_CAU_InitStructure.CAU_KeySize  = CAU_KEYSIZE_256BIT;
        AES_CAU_KeyInitStructure.CAU_Key0High = __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key0Low= __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key1High = __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key1Low= __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key2High = __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key2Low= __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key3High = __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key3Low= __REV(*(uint32_t*)(keyaddr));
        break;
        default:
        break;
    }

    /* AES Decryption */
    if(Mode == MODE_DECRYPT) /* AES decryption */
    {
        /* Flush IN/OUT FIFOs */
        CAU_FIFOFlush();

        /* CAU Init for Key preparation for decryption process */
        AES_CAU_InitStructure.CAU_AlgoDir = CAU_ALGODIR_DECRYPT;
        AES_CAU_InitStructure.CAU_AlgoMode = CAU_ALGOMODE_AES_KEY;
        AES_CAU_InitStructure.CAU_DataType = CAU_DATAS_32BIT;
        CAU_Init(&AES_CAU_InitStructure);

        /* Key Initialisation */
        CAU_KeyInit(&AES_CAU_KeyInitStructure);

        /* Enable CAU processor */
        CAU_Enable(ENABLE);

        /* Wait until the Busy flag is RESET */
        do
        {
            busystatus = CAU_GetBitState(CAU_FLAG_BSY);
            counter++;
        }while ((counter != AESBSY_TIMEOUT) && (busystatus != RESET));

        if (busystatus != RESET)
        {
            return ERROR;
        }
        else
        {
            /* CAU Init for decryption process */  
            AES_CAU_InitStructure.CAU_AlgoDir = CAU_ALGODIR_DECRYPT;
        }
    }
    /* AES Encryption */
    else /* AES encryption */
    {
        CAU_KeyInit(&AES_CAU_KeyInitStructure);

        /* CAU Init for Encryption process */
        AES_CAU_InitStructure.CAU_AlgoDir  = CAU_ALGODIR_ENCRYPT;
    }

    AES_CAU_InitStructure.CAU_AlgoMode = CAU_ALGOMODE_AES_ECB;
    AES_CAU_InitStructure.CAU_DataType = CAU_DATAS_8BIT;
    CAU_Init(&AES_CAU_InitStructure);

    /* Flush IN/OUT FIFOs */
    CAU_FIFOFlush();

    /* Enable CAU processor */
    CAU_Enable(ENABLE);

    if(CAU_GetEnableState() == DISABLE)
    {
        /* The CAU peripheral clock is not enabled or the device doesn't embedd 
        the CAU peripheral (please check the device sales type */
        return(ERROR);
    }
  
    for(i=0; i<Ilength; i+=16)
    {
        /* Write the Input block in the IN FIFO */
        CAU_PutData(*(uint32_t*)(inputaddr));
        inputaddr+=4;
        CAU_PutData(*(uint32_t*)(inputaddr));
        inputaddr+=4;
        CAU_PutData(*(uint32_t*)(inputaddr));
        inputaddr+=4;
        CAU_PutData(*(uint32_t*)(inputaddr));
        inputaddr+=4;

        /* Wait until the complete message has been processed */
        counter = 0;
        do
        {
            busystatus = CAU_GetBitState(CAU_FLAG_BSY);
            counter++;
        }while ((counter != AESBSY_TIMEOUT) && (busystatus != RESET));

        if (busystatus != RESET)
        {
            return ERROR;
        }
        else
        {  
            /* Read the Output block from the Output FIFO */
            *(uint32_t*)(outputaddr) = CAU_GetData();
            outputaddr+=4;
            *(uint32_t*)(outputaddr) = CAU_GetData();
            outputaddr+=4;
            *(uint32_t*)(outputaddr) = CAU_GetData();
            outputaddr+=4;
            *(uint32_t*)(outputaddr) = CAU_GetData(); 
            outputaddr+=4;
        }
    }

    /* Disable CAU */
    CAU_Enable(DISABLE);

    return SUCCESS; 
}

/**
  * @brief  Encrypt and decrypt using AES in CBC Mode
  * @param  Mode: encryption or decryption Mode (MODE_ENCRYPT or MODE_DECRYPT).
  * @param  InitVectors: Initialisation Vectors used for AES algorithm.
  * @param  Key: Key used for AES algorithm.
  * @param  Keysize: length of the Key, must be a 128, 192 or 256.
  * @param  Input: pointer to the Input buffer.
  * @param  Ilength: length of the Input buffer, must be a multiple of 16.
  * @param  Output: pointer to the returned buffer.
  * @retval The value of result (SUCCESS OR ERROR).
  */
TypeState CAU_AES_CBC(uint8_t Mode, uint8_t InitVectors[16], uint8_t *Key,
                         uint16_t Keysize, uint8_t *Input, uint32_t Ilength,
                         uint8_t *Output)
{
    CAU_InitPara AES_CAU_InitStructure;
    CAU_KeyInitPara AES_CAU_KeyInitStructure;
    CAU_IVInitPara AES_CAU_IVInitStructure;
    
    __IO uint32_t counter = 0;
    uint32_t busystatus = 0;
    uint32_t keyaddr    = (uint32_t)Key;
    uint32_t inputaddr  = (uint32_t)Input;
    uint32_t outputaddr = (uint32_t)Output;
    uint32_t ivaddr = (uint32_t)InitVectors;
    uint32_t i = 0;

    /* CAU structures initialisation */
    CAU_KeyStructInit(&AES_CAU_KeyInitStructure);

    switch(Keysize)
    {
        case 128:
        AES_CAU_InitStructure.CAU_KeySize = CAU_KEYSIZE_128BIT;
        AES_CAU_KeyInitStructure.CAU_Key2High = __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key2Low= __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key3High = __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key3Low= __REV(*(uint32_t*)(keyaddr));
        break;
        case 192:
        AES_CAU_InitStructure.CAU_KeySize  = CAU_KEYSIZE_192BIT;
        AES_CAU_KeyInitStructure.CAU_Key1High = __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key1Low= __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key2High = __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key2Low= __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key3High = __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key3Low= __REV(*(uint32_t*)(keyaddr));
        break;
        case 256:
        AES_CAU_InitStructure.CAU_KeySize  = CAU_KEYSIZE_256BIT;
        AES_CAU_KeyInitStructure.CAU_Key0High = __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key0Low= __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key1High = __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key1Low= __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key2High = __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key2Low= __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key3High = __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key3Low= __REV(*(uint32_t*)(keyaddr));
        break;
        default:
        break;
    }
    
    /* CAU Initialization Vectors */
    AES_CAU_IVInitStructure.CAU_IV0High = __REV(*(uint32_t*)(ivaddr));
    ivaddr+=4;
    AES_CAU_IVInitStructure.CAU_IV0Low= __REV(*(uint32_t*)(ivaddr));
    ivaddr+=4;
    AES_CAU_IVInitStructure.CAU_IV1High = __REV(*(uint32_t*)(ivaddr));
    ivaddr+=4;
    AES_CAU_IVInitStructure.CAU_IV1Low= __REV(*(uint32_t*)(ivaddr));


    /* AES Decryption */
    if(Mode == MODE_DECRYPT) /* AES decryption */
    {
        /* Flush IN/OUT FIFOs */
        CAU_FIFOFlush();

        /* CAU Init for Key preparation for decryption process */
        AES_CAU_InitStructure.CAU_AlgoDir = CAU_ALGODIR_DECRYPT;
        AES_CAU_InitStructure.CAU_AlgoMode = CAU_ALGOMODE_AES_KEY;
        AES_CAU_InitStructure.CAU_DataType = CAU_DATAS_32BIT;

        CAU_Init(&AES_CAU_InitStructure);

        /* Key Initialisation */
        CAU_KeyInit(&AES_CAU_KeyInitStructure);

        /* Enable CAU processor */
        CAU_Enable(ENABLE);

        /* wait until the Busy flag is RESET */
        do
        {
            busystatus = CAU_GetBitState(CAU_FLAG_BSY);
            counter++;
        }while ((counter != AESBSY_TIMEOUT) && (busystatus != RESET));

        if (busystatus != RESET)
        {
            return ERROR;
        }
        else
        {
            /* CAU Init for decryption process */  
            AES_CAU_InitStructure.CAU_AlgoDir = CAU_ALGODIR_DECRYPT;
        }
    }
    /* AES Encryption */
    else /* AES encryption */
    {
        CAU_KeyInit(&AES_CAU_KeyInitStructure);

        /* CAU Init for Encryption process */
        AES_CAU_InitStructure.CAU_AlgoDir  = CAU_ALGODIR_ENCRYPT;
    }
    AES_CAU_InitStructure.CAU_AlgoMode = CAU_ALGOMODE_AES_CBC;
    AES_CAU_InitStructure.CAU_DataType = CAU_DATAS_8BIT;
    CAU_Init(&AES_CAU_InitStructure);
    
    /* CAU Initialization Vectors */
    CAU_IVInit(&AES_CAU_IVInitStructure);

    /* Flush IN/OUT FIFOs */
    CAU_FIFOFlush();

    /* Enable CAU processor */
    CAU_Enable(ENABLE);

    if(CAU_GetEnableState() == DISABLE)
    {
        /* The CAU peripheral clock is not enabled or the device doesn't embedd 
           the CAU peripheral */
        return(ERROR);
    }
  
    for(i=0; i<Ilength; i+=16)
    {
        /* Write the Input block in the IN FIFO */
        CAU_PutData(*(uint32_t*)(inputaddr));
        inputaddr+=4;
        CAU_PutData(*(uint32_t*)(inputaddr));
        inputaddr+=4;
        CAU_PutData(*(uint32_t*)(inputaddr));
        inputaddr+=4;
        CAU_PutData(*(uint32_t*)(inputaddr));
        inputaddr+=4;
        /* Wait until the complete message has been processed */
        counter = 0;
        do
        {
            busystatus = CAU_GetBitState(CAU_FLAG_BSY);
            counter++;
        }while ((counter != AESBSY_TIMEOUT) && (busystatus != RESET));

        if (busystatus != RESET)
        {
            return ERROR;
        }
        else
        {
            /* Read the Output block from the Output FIFO */
            *(uint32_t*)(outputaddr) = CAU_GetData();
            outputaddr+=4;
            *(uint32_t*)(outputaddr) = CAU_GetData();
            outputaddr+=4;
            *(uint32_t*)(outputaddr) = CAU_GetData();
            outputaddr+=4;
            *(uint32_t*)(outputaddr) = CAU_GetData();
            outputaddr+=4;
        }
    }
    /* Disable CAU */
    CAU_Enable(DISABLE);

    return SUCCESS;
}

/**
  * @brief  Encrypt and decrypt using AES in CTR Mode
  * @param  Mode: encryption or decryption Mode (MODE_ENCRYPT or MODE_DECRYPT).
  * @param  InitVectors: Initialisation Vectors used for AES algorithm.
  * @param  Key: Key used for AES algorithm.
  * @param  Keysize: length of the Key, must be a 128, 192 or 256.
  * @param  Input: pointer to the Input buffer.
  * @param  Ilength: length of the Input buffer, must be a multiple of 16.
  * @param  Output: pointer to the returned buffer.
  * @retval The value of result (SUCCESS OR ERROR).
  */
TypeState CAU_AES_CTR(uint8_t Mode, uint8_t InitVectors[16], uint8_t *Key, 
                         uint16_t Keysize, uint8_t *Input, uint32_t Ilength,
                         uint8_t *Output)
{
    CAU_InitPara AES_CAU_InitStructure;
    CAU_KeyInitPara AES_CAU_KeyInitStructure;
    CAU_IVInitPara AES_CAU_IVInitStructure;
        
    __IO uint32_t counter = 0;
    uint32_t busystatus = 0;
    uint32_t keyaddr    = (uint32_t)Key;
    uint32_t inputaddr  = (uint32_t)Input;
    uint32_t outputaddr = (uint32_t)Output;
    uint32_t ivaddr     = (uint32_t)InitVectors;
    uint32_t i = 0;

    /* CAU structures initialisation*/
    CAU_KeyStructInit(&AES_CAU_KeyInitStructure);

    switch(Keysize)
    {
        case 128:
        AES_CAU_InitStructure.CAU_KeySize = CAU_KEYSIZE_128BIT;
        AES_CAU_KeyInitStructure.CAU_Key2High = __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key2Low= __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key3High = __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key3Low= __REV(*(uint32_t*)(keyaddr));
        break;
        case 192:
        AES_CAU_InitStructure.CAU_KeySize  = CAU_KEYSIZE_192BIT;
        AES_CAU_KeyInitStructure.CAU_Key1High = __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key1Low= __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key2High = __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key2Low= __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key3High = __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key3Low= __REV(*(uint32_t*)(keyaddr));
        break;
        case 256:
        AES_CAU_InitStructure.CAU_KeySize  = CAU_KEYSIZE_256BIT;
        AES_CAU_KeyInitStructure.CAU_Key0High = __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key0Low= __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key1High = __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key1Low= __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key2High = __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key2Low= __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key3High = __REV(*(uint32_t*)(keyaddr));
        keyaddr+=4;
        AES_CAU_KeyInitStructure.CAU_Key3Low= __REV(*(uint32_t*)(keyaddr));
        break;
        default:
        break;
    }
    /* CAU Initialization Vectors */
      AES_CAU_IVInitStructure.CAU_IV0High = __REV(*(uint32_t*)(ivaddr));
    ivaddr+=4;
    AES_CAU_IVInitStructure.CAU_IV0Low = __REV(*(uint32_t*)(ivaddr));
    ivaddr+=4;
    AES_CAU_IVInitStructure.CAU_IV1High = __REV(*(uint32_t*)(ivaddr));
    ivaddr+=4;
    AES_CAU_IVInitStructure.CAU_IV1Low = __REV(*(uint32_t*)(ivaddr));

    /* Key Initialisation */
    CAU_KeyInit(&AES_CAU_KeyInitStructure);

    /* AES Decryption */
    if(Mode == MODE_DECRYPT) /* AES decryption */
    {
        /* CAU Init for decryption process */
        AES_CAU_InitStructure.CAU_AlgoDir = CAU_ALGODIR_DECRYPT;
    }
    /* AES Encryption */
    else /* AES encryption */
    {
        /* CAU Init for Encryption process */
        AES_CAU_InitStructure.CAU_AlgoDir = CAU_ALGODIR_ENCRYPT;
    }
    AES_CAU_InitStructure.CAU_AlgoMode = CAU_ALGOMODE_AES_CTR;
    AES_CAU_InitStructure.CAU_DataType = CAU_DATAS_8BIT;
    CAU_Init(&AES_CAU_InitStructure);

    /* CAU Initialization Vectors */
    CAU_IVInit(&AES_CAU_IVInitStructure);

    /* Flush IN/OUT FIFOs */
    CAU_FIFOFlush();

    /* Enable CAU processor */
    CAU_Enable(ENABLE);

    if(CAU_GetEnableState() == DISABLE)
    {
        /* The CAU peripheral clock is not enabled or the device doesn't embedd 
           the CAU peripheral */
        return(ERROR);
    }
  
    for(i=0; i<Ilength; i+=16)
    {

        /* Write the Input block in the IN FIFO */
        CAU_PutData(*(uint32_t*)(inputaddr));
        inputaddr+=4;
        CAU_PutData(*(uint32_t*)(inputaddr));
        inputaddr+=4;
        CAU_PutData(*(uint32_t*)(inputaddr));
        inputaddr+=4;
        CAU_PutData(*(uint32_t*)(inputaddr));
        inputaddr+=4;
        /* Wait until the complete message has been processed */
        counter = 0;
        do
        {
            busystatus = CAU_GetBitState(CAU_FLAG_BSY);
            counter++;
        }while ((counter != AESBSY_TIMEOUT) && (busystatus != RESET));

        if (busystatus != RESET)
        {
            return ERROR;
        }
        else
        {

            /* Read the Output block from the Output FIFO */
            *(uint32_t*)(outputaddr) = CAU_GetData();
            outputaddr+=4;
            *(uint32_t*)(outputaddr) = CAU_GetData();
            outputaddr+=4;
            *(uint32_t*)(outputaddr) = CAU_GetData();
            outputaddr+=4;
            *(uint32_t*)(outputaddr) = CAU_GetData();
            outputaddr+=4;
        }
    }
    /* Disable CAU */
    CAU_Enable(DISABLE);

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

