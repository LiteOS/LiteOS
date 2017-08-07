/*!
    \file  gd32f20x_cau_tdes.c
    \brief CAU_TDES driver
*/

/*
    Copyright (C) 2017 GigaDevice

    2015-07-15, V1.0.0, firmware for GD32F20x
    2017-06-05, V2.0.0, firmware for GD32F20x
*/

#include "gd32f20x_cau.h"

#define TDESBSY_TIMEOUT    ((uint32_t)0x00010000U)

/* TDES calculate process */
static ErrStatus cau_tdes_calculate(uint8_t *input, uint32_t in_length, uint8_t *output);

/*!
    \brief      encrypt and decrypt using TDES in ECB mode
    \param[in]  algo_dir: algorithm direction
                only one parameter can be selected which is shown as below
      \arg        CAU_ENCRYPT: encrypt
      \arg        CAU_DECRYPT: decrypt
    \param[in]  key: key used for TDES algorithm  
    \param[in]  input: pointer to the input buffer     
    \param[in]  in_length: length of the input buffer, must be a multiple of 8  
    \param[in]  output: pointer to the returned buffer    
    \param[out] none
    \retval     ErrStatus: SUCCESS or ERROR
*/  
ErrStatus cau_tdes_ecb(uint32_t algo_dir, uint8_t key[24], uint8_t *input, uint32_t in_length, uint8_t *output)
{
    ErrStatus ret = ERROR;
    cau_key_parameter_struct key_initpara;

    uint32_t keyaddr = (uint32_t)key;

    /* key structure initialization */
    cau_key_parameter_init(&key_initpara);
    /* initialize the CAU peripheral */
    cau_init(algo_dir, CAU_MODE_TDES_ECB, CAU_SWAPPING_8BIT);

    /* key initialization */
    key_initpara.key_1_high = __REV(*(uint32_t*)(keyaddr));
    keyaddr += 4U;
    key_initpara.key_1_low= __REV(*(uint32_t*)(keyaddr));
    keyaddr += 4U;
    key_initpara.key_2_high = __REV(*(uint32_t*)(keyaddr));
    keyaddr += 4U;
    key_initpara.key_2_low= __REV(*(uint32_t*)(keyaddr));
    keyaddr += 4U;
    key_initpara.key_3_high = __REV(*(uint32_t*)(keyaddr));
    keyaddr += 4U;
    key_initpara.key_3_low= __REV(*(uint32_t*)(keyaddr));    
    cau_key_init(&key_initpara);

    /* flush the IN and OUT FIFOs */
    cau_fifo_flush();

    /* enable the CAU peripheral */
    cau_enable();
    /* TDES calculate process */
    ret = cau_tdes_calculate(input, in_length, output);
    /* disable the CAU peripheral */
    cau_disable();

    return ret; 
}

/*!
    \brief      encrypt and decrypt using TDES in CBC mode
    \param[in]  algo_dir: algorithm direction
                only one parameter can be selected which is shown as below
      \arg        CAU_ENCRYPT: encrypt
      \arg        CAU_DECRYPT: decrypt
    \param[in]  key: key used for TDES algorithm
    \param[in]  iv: initialization vectors used for TDES algorithm 
    \param[in]  input: pointer to the input buffer     
    \param[in]  in_length: length of the input buffer, must be a multiple of 8  
    \param[in]  output: pointer to the returned buffer    
    \param[out] none
    \retval     ErrStatus: SUCCESS or ERROR
*/ 
ErrStatus cau_tdes_cbc(uint32_t algo_dir, uint8_t key[24], uint8_t iv[8], uint8_t *input, uint32_t in_length, uint8_t *output)                         
{
    ErrStatus ret = ERROR;
    cau_key_parameter_struct key_initpara;
    cau_iv_parameter_struct iv_initpara;
  
    uint32_t keyaddr = (uint32_t)key;
    uint32_t ivaddr  = (uint32_t)iv;

    /* key structure initialization */
    cau_key_parameter_init(&key_initpara);
    /* initialize the CAU peripheral */
    cau_init(algo_dir, CAU_MODE_TDES_CBC, CAU_SWAPPING_8BIT);
  
    /* key initialization */
    key_initpara.key_1_high = __REV(*(uint32_t*)(keyaddr));
    keyaddr += 4U;
    key_initpara.key_1_low= __REV(*(uint32_t*)(keyaddr));
    keyaddr += 4U;
    key_initpara.key_2_high = __REV(*(uint32_t*)(keyaddr));
    keyaddr += 4U;
    key_initpara.key_2_low= __REV(*(uint32_t*)(keyaddr));
    keyaddr += 4U;
    key_initpara.key_3_high = __REV(*(uint32_t*)(keyaddr));
    keyaddr += 4U;
    key_initpara.key_3_low= __REV(*(uint32_t*)(keyaddr));
    cau_key_init(&key_initpara);

    /* vectors initialization */
    iv_initpara.iv_0_high = __REV(*(uint32_t*)(ivaddr));
    ivaddr += 4U;
    iv_initpara.iv_0_low= __REV(*(uint32_t*)(ivaddr));
    cau_iv_init(&iv_initpara);

    /* flush the IN and OUT FIFOs */
    cau_fifo_flush();

    /* enable the CAU peripheral */
    cau_enable();
    /* TDES calculate process */
    ret = cau_tdes_calculate(input, in_length, output);
    /* disable the CAU peripheral */
    cau_disable();

    return ret; 
}

/*!
    \brief      TDES calculate process
    \param[in]  input: pointer to the input buffer     
    \param[in]  in_length: length of the input buffer, must be a multiple of 8  
    \param[in]  output: pointer to the returned buffer    
    \param[out] none
    \retval     ErrStatus: SUCCESS or ERROR
*/
static ErrStatus cau_tdes_calculate(uint8_t *input, uint32_t in_length, uint8_t *output)
{
    uint32_t inputaddr  = (uint32_t)input;
    uint32_t outputaddr = (uint32_t)output;
    uint32_t i = 0U;
    __IO uint32_t counter = 0U;
    uint32_t busystatus = 0U;  
  
    /* the clock is not enabled or there is no embeded CAU peripheral */           
    if(DISABLE == cau_enable_state_get()){     
        return ERROR;
    }

    for(i = 0U; i < in_length; i += 8U){    
        /* write data to the IN FIFO */
        cau_data_write(*(uint32_t*)(inputaddr));
        inputaddr += 4U;
        cau_data_write(*(uint32_t*)(inputaddr));
        inputaddr += 4U;

        /* wait until the complete message has been processed */
        counter = 0U;
        do{        
            busystatus = cau_flag_get(CAU_FLAG_BUSY);
            counter++;
        }while((TDESBSY_TIMEOUT != counter) && (RESET != busystatus));

        if(RESET != busystatus){       
            return ERROR;
        }else{
            /* read the output block from the output FIFO */
            *(uint32_t*)(outputaddr) = cau_data_read();
            outputaddr += 4U;
            *(uint32_t*)(outputaddr) = cau_data_read();
            outputaddr += 4U;
        }
    }
  
    return SUCCESS;
}
