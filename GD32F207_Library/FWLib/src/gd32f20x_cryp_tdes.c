/**
  ******************************************************************************
  * @file    gd32f20x_cryp_tdes.c
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
#define TDESBSY_TIMEOUT    ((uint32_t) 0x00010000)

/* Private functions ---------------------------------------------------------*/


/** @defgroup CAU_Private_Functions
  * @{
  */ 

/**
  * @brief  Encrypt and decrypt using TDES in ECB Mode
  * @param  Mode: encryption or decryption Mode (MODE_ENCRYPT or MODE_DECRYPT).
  * @param  Key: Key used for TDES algorithm.
  * @param  Ilength: length of the Input buffer, must be a multiple of 8.
  * @param  Input: pointer to the Input buffer.
  * @param  Output: pointer to the returned buffer.
  * @retval The value of result (SUCCESS OR ERROR).
  */
TypeState CAU_TDES_ECB(uint8_t Mode, uint8_t Key[24], uint8_t *Input, 
                          uint32_t Ilength, uint8_t *Output)
{
    CAU_InitPara TDES_CAU_InitStructure;
    CAU_KeyInitPara TDES_CAU_KeyInitStructure;

    __IO uint32_t counter = 0;
    uint32_t busystatus = 0;
    uint32_t keyaddr    = (uint32_t)Key;
    uint32_t inputaddr  = (uint32_t)Input;
    uint32_t outputaddr = (uint32_t)Output;
    uint32_t i = 0;

    /* CAU structures initialisation*/
    CAU_KeyStructInit(&TDES_CAU_KeyInitStructure);

    /* CAU Init for Encryption process */
    if(Mode == MODE_ENCRYPT) /* TDES encryption */
    {
        TDES_CAU_InitStructure.CAU_AlgoDir = CAU_ALGODIR_ENCRYPT;
    }
    else /*if(Mode == MODE_DECRYPT)*/ /* TDES decryption */
    {
        TDES_CAU_InitStructure.CAU_AlgoDir = CAU_ALGODIR_DECRYPT;
    }

    TDES_CAU_InitStructure.CAU_AlgoMode = CAU_ALGOMODE_TDES_ECB;
    TDES_CAU_InitStructure.CAU_DataType = CAU_DATAS_8BIT;
    CAU_Init(&TDES_CAU_InitStructure);

    /* Key Initialisation */
    TDES_CAU_KeyInitStructure.CAU_Key1High = __REV(*(uint32_t*)(keyaddr));
    keyaddr+=4;
    TDES_CAU_KeyInitStructure.CAU_Key1Low= __REV(*(uint32_t*)(keyaddr));
    keyaddr+=4;
    TDES_CAU_KeyInitStructure.CAU_Key2High = __REV(*(uint32_t*)(keyaddr));
    keyaddr+=4;
    TDES_CAU_KeyInitStructure.CAU_Key2Low= __REV(*(uint32_t*)(keyaddr));
    keyaddr+=4;
    TDES_CAU_KeyInitStructure.CAU_Key3High = __REV(*(uint32_t*)(keyaddr));
    keyaddr+=4;
    TDES_CAU_KeyInitStructure.CAU_Key3Low= __REV(*(uint32_t*)(keyaddr));
    CAU_KeyInit(& TDES_CAU_KeyInitStructure);

    /* Flush IN/OUT FIFO */
    CAU_FIFOFlush();

    /* Enable CAU processor */
    CAU_Enable(ENABLE);

    if(CAU_GetEnableState() == DISABLE)
    {
        /* The CAU peripheral clock is not enabled or the device doesn't embedd 
           the CAU peripheral */
        return(ERROR);
    }
    for(i=0; i<Ilength; i+=8)
    {
        /* Write the Input block in the Input FIFO */
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
        }while ((counter != TDESBSY_TIMEOUT) && (busystatus != RESET));

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
        }
    }

    /* Disable CAU */
    CAU_Enable(DISABLE);

    return SUCCESS; 
}

/**
  * @brief  Encrypt and decrypt using TDES in CBC Mode
  * @param  Mode: encryption or decryption Mode (MODE_ENCRYPT or MODE_DECRYPT).
  * @param  Key: Key used for TDES algorithm.
  * @param  InitVectors: Initialisation Vectors used for TDES algorithm.
  * @param  Input: pointer to the Input buffer.
  * @param  Ilength: length of the Input buffer, must be a multiple of 8.
  * @param  Output: pointer to the returned buffer.
  * @retval The value of result (SUCCESS OR ERROR).
  */
TypeState CAU_TDES_CBC(uint8_t Mode, uint8_t Key[24], uint8_t InitVectors[8],
                          uint8_t *Input, uint32_t Ilength, uint8_t *Output)
{
    CAU_InitPara TDES_CAU_InitStructure;
    CAU_KeyInitPara TDES_CAU_KeyInitStructure;
    CAU_IVInitPara TDES_CAU_IVInitStructure;

    __IO uint32_t counter = 0;
    uint32_t busystatus = 0;
    uint32_t keyaddr    = (uint32_t)Key;
    uint32_t inputaddr  = (uint32_t)Input;
    uint32_t outputaddr = (uint32_t)Output;
    uint32_t ivaddr     = (uint32_t)InitVectors;
    uint32_t i = 0;

    /* CAU structures initialisation*/
    CAU_KeyStructInit(&TDES_CAU_KeyInitStructure);

    /* CAU Init for Encryption process */
    if(Mode == MODE_ENCRYPT) /* TDES encryption */
    {
        TDES_CAU_InitStructure.CAU_AlgoDir = CAU_ALGODIR_ENCRYPT;
    }
    else
    {
        TDES_CAU_InitStructure.CAU_AlgoDir = CAU_ALGODIR_DECRYPT;
    }
    TDES_CAU_InitStructure.CAU_AlgoMode = CAU_ALGOMODE_TDES_CBC;
    TDES_CAU_InitStructure.CAU_DataType = CAU_DATAS_8BIT;

    CAU_Init(&TDES_CAU_InitStructure);

    /* Key Initialisation */
    TDES_CAU_KeyInitStructure.CAU_Key1High = __REV(*(uint32_t*)(keyaddr));
    keyaddr+=4;
    TDES_CAU_KeyInitStructure.CAU_Key1Low= __REV(*(uint32_t*)(keyaddr));
    keyaddr+=4;
    TDES_CAU_KeyInitStructure.CAU_Key2High = __REV(*(uint32_t*)(keyaddr));
    keyaddr+=4;
    TDES_CAU_KeyInitStructure.CAU_Key2Low= __REV(*(uint32_t*)(keyaddr));
    keyaddr+=4;
    TDES_CAU_KeyInitStructure.CAU_Key3High = __REV(*(uint32_t*)(keyaddr));
    keyaddr+=4;
    TDES_CAU_KeyInitStructure.CAU_Key3Low= __REV(*(uint32_t*)(keyaddr));
    CAU_KeyInit(& TDES_CAU_KeyInitStructure);

    /* Initialization Vectors */
    TDES_CAU_IVInitStructure.CAU_IV0High = __REV(*(uint32_t*)(ivaddr));
    ivaddr+=4;
    TDES_CAU_IVInitStructure.CAU_IV0Low= __REV(*(uint32_t*)(ivaddr));
    CAU_IVInit(&TDES_CAU_IVInitStructure);

    /* Flush IN/OUT FIFO */
    CAU_FIFOFlush();

    /* Enable CAU processor */
    CAU_Enable(ENABLE);

    if(CAU_GetEnableState() == DISABLE)
    {
        /* The CAU peripheral clock is not enabled or the device doesn't embedd 
           the CAU peripheral */
        return(ERROR);
    }

    for(i=0; i<Ilength; i+=8)
    {
        /* Write the Input block in the Input FIFO */
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
        }while ((counter != TDESBSY_TIMEOUT) && (busystatus != RESET));

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
