/*!
    \file  gd32f1x0_crc.c
    \brief crc driver
*/

/*
    Copyright (C) 2016 GigaDevice

    2014-12-26, V1.0.0, firmware for GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, firmware for GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#include "gd32f1x0_crc.h"

/*!
    \brief      deinit CRC calculation unit
    \param[in]  none
    \param[out] none
    \retval     none
*/
void crc_deinit(void)
{
    CRC_IDATA = 0xFFFFFFFF;
    CRC_DATA  = 0xFFFFFFFF;
    CRC_FDATA = 0x00;
    CRC_CTL   = CRC_CTL_RST;
}

/*!
    \brief      enable the reverse operation of output data
    \param[in]  none
    \param[out] none
    \retval     none
*/
void crc_reverse_output_data_enable(void)
{
    CRC_CTL &= (uint32_t)(~ CRC_CTL_REV_O);
    CRC_CTL |= (uint32_t)CRC_CTL_REV_O;
}

/*!
    \brief      disable the reverse operation of output data
    \param[in]  none
    \param[out] none
    \retval     none
*/
void crc_reverse_output_data_disable(void)
{
    CRC_CTL &= (uint32_t)(~ CRC_CTL_REV_O);
}

/*!
    \brief      reset data register to the value of initializaiton data register
    \param[in]  none
    \param[out] none
    \retval     none
*/
void crc_data_register_reset(void)
{
    CRC_CTL |= (uint32_t)CRC_CTL_RST;
}

/*!
    \brief      read the data register 
    \param[in]  none
    \param[out] none
    \retval     32-bit value
*/
uint32_t crc_data_register_read(void)
{
    uint32_t data;
    data = (GET_BITS(CRC_DATA, 0, 31));
    return (data);
}

/*!
    \brief      read the free data register
    \param[in]  none
    \param[out] none
    \retval     32-bit value of the free data register
*/
uint32_t crc_free_data_register_read(void)
{
    uint32_t fdata;
    fdata = GET_BITS(CRC_FDATA, 0, 7);
    return (fdata);
}

/*!
    \brief      write the free data register
    \param[in]  crc_free_data: specify 32-bit data
    \param[out] none
    \retval     none
*/
void crc_free_data_register_write(uint32_t crc_free_data)
{
    CRC_FDATA = (uint32_t)crc_free_data;
}
/*!
    \brief      write the initializaiton data register
    \param[in]  crc_init_data:specify 32-bit data
    \param[out] none
    \retval     none
*/
void crc_init_data_register_write(uint32_t crc_init_data)
{
    CRC_IDATA = (uint32_t)crc_init_data;
}

/*!
    \brief      configure the CRC input data function
    \param[in]  crc_data_reverse: specify input data reverse function
      \arg        CRC_INPUT_DATA_NOT: input data is not reversed
      \arg        CRC_INPUT_DATA_BYTE: input data is reversed on 8 bits
      \arg        CRC_INPUT_DATA_HALFWORD: input data is reversed on 16 bits
      \arg        CRC_INPUT_DATA_WORD: input data is reversed on 32 bits
    \param[out] none
    \retval     none
*/
void crc_input_data_reverse_config(uint32_t crc_data_reverse)
{
    CRC_CTL &= (uint32_t)(~CRC_CTL_REV_I);
    CRC_CTL |= (uint32_t)crc_data_reverse;
}

/*!
    \brief      CRC calculate a 32-bit data
    \param[in]  sdata: specify 32-bit data
    \param[out] none
    \retval     32-bit CRC calculate value
*/
uint32_t crc_single_data_calculate(uint32_t sdata)
{
    CRC_DATA = sdata;
    return (CRC_DATA);
}

/*!
    \brief      CRC calculate a 32-bit data array
    \param[in]  array: pointer to an array of 32 bit data words
    \param[in]  size: size of the array
    \param[out] none
    \retval     32-bit CRC calculate value
*/
uint32_t crc_block_data_calculate(uint32_t *array, uint32_t size)
{
    uint32_t index;
    for(index=0;index<size;index++){
        CRC_DATA = array[index];
    }
    return (CRC_DATA);
}
