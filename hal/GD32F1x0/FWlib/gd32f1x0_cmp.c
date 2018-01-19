/*!
    \file  gd32f1x0_cmp.c
    \brief cmp driver
*/

/*
    Copyright (C) 2016 GigaDevice

    2014-12-26, V1.0.0, firmware for GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, firmware for GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#include "gd32f1x0_cmp.h"

/*!
    \brief      deinitialize comparator 
    \param[in]  none
    \param[out] none
    \retval     none
*/
void cmp_deinit(void)
{
    CMP_CS = ((uint32_t)0x00000000);
}

/*!
    \brief      initialize comparator mode 
    \param[in]  cmp_channal
      \arg        CMP_CHANNEL_CMP0
      \arg        CMP_CHANNEL_CMP1
    \param[in]  cmp_operating_mode
      \arg        CMP_HIGHSPEED
      \arg        CMP_MIDDLESPEED
      \arg        CMP_LOWSPEED
      \arg        CMP_VERYLOWSPEED
    \param[in]  cmp_inverting_input 
      \arg        CMP_1_4VREFINT
      \arg        CMP_1_2VREFINT
      \arg        CMP_3_4VREFINT
      \arg        CMP_VREFINT
      \arg        CMP_DAC1
      \arg        CMP_PA5
      \arg        CMP_PA0
    \param[in]  cmp_hysteresis
      \arg        CMP_HYSTERESIS_NO
      \arg        CMP_HYSTERESIS_LOW
      \arg        CMP_HYSTERESIS_MIDDLE
      \arg        CMP_HYSTERESIS_HIGH
    \param[out] none
    \retval     none
*/
void cmp_mode_init(uint32_t cmp_channal, operating_mode_enum cmp_operating_mode, inverting_input_enum cmp_inverting_input, cmp_hysteresis_enum output_hysteresis)
{
    /* initialize comparator mode of channel0 */
    if(CMP_CHANNEL_CMP0 == cmp_channal){
        /* initialize comparator mode of channel0 */
        CMP_CS |= CS_CMP0M(cmp_operating_mode) | CS_CMP0MSEL(cmp_inverting_input) | CS_CMP0HST(output_hysteresis);
    }else{
        /* initialize comparator mode of channel1 */
        CMP_CS |= CS_CMP1M(cmp_operating_mode) | CS_CMP1MSEL(cmp_inverting_input) | CS_CMP1HST(output_hysteresis);
    }
}

/*!
    \brief      initialize comparator output
    \param[in]  cmp_channal
      \arg        CMP_CHANNEL_CMP0
      \arg        CMP_CHANNEL_CMP1
    \param[in]  cmp_output 
      \arg        CMP_OUTPUT_NONE
      \arg        CMP_OUTPUT_TIM0BKIN
      \arg        CMP_OUTPUT_TIM0IC0
      \arg        CMP_OUTPUT_TIM0OCREFCLR
      \arg        CMP_OUTPUT_TIM1IC3
      \arg        CMP_OUTPUT_TIM1OCREFCLR
      \arg        CMP_OUTPUT_TIM2IC0
      \arg        CMP_OUTPUT_TIM2OCREFCLR
    \param[in]  cmp_output_polarity 
      \arg        CMP_OUTPUT_POLARITY_INVERTED
      \arg        CMP_OUTPUT_POLARITY_NOINVERTED
    \param[out] none
    \retval     none
*/
void cmp_output_init(uint32_t cmp_channal, cmp_output_enum cmp_output_slection, uint32_t cmp_output_polarity)
{
    /* initialize comparator output of channel0 */
    if(CMP_CHANNEL_CMP0 == cmp_channal){
        CMP_CS |= CS_CMP0OSEL(cmp_output_slection);
        /* output polarity */
        if(CMP_OUTPUT_POLARITY_INVERTED == cmp_output_polarity){
            CMP_CS |= CMP_CS_CMP0PL;
        }else{ 
            CMP_CS &= ~CMP_CS_CMP0PL;
        }
    }else{
        CMP_CS |= CS_CMP1OSEL(cmp_output_slection);
        
        if(CMP_OUTPUT_POLARITY_INVERTED == cmp_output_polarity){
            CMP_CS |= CMP_CS_CMP1PL;
        }else{ 
            CMP_CS &= ~CMP_CS_CMP1PL;
        }
    }
}

/*!
    \brief      enable comparator channel
    \param[in]  cmp_channal
      \arg        CMP_CHANNEL_CMP0
      \arg        CMP_CHANNEL_CMP1
    \param[out] none
    \retval     none
*/
void cmp_channel_enable(uint32_t cmp_channal)
{
    if(CMP_CHANNEL_CMP0 == cmp_channal){
        CMP_CS |= CMP_CS_CMP0EN;
    }else{
        CMP_CS |= CMP_CS_CMP1EN;
    }
}

/*!
    \brief      disable comparator channel
    \param[in]  cmp_channal
      \arg        CMP_CHANNEL_CMP0
      \arg        CMP_CHANNEL_CMP1
    \param[out] none
    \retval     none
*/
void cmp_channel_disable(uint32_t cmp_channal)
{
    if(CMP_CHANNEL_CMP0 == cmp_channal){
        CMP_CS &= ~CMP_CS_CMP0EN;
    }else{
        CMP_CS &= ~CMP_CS_CMP1EN;
    }
}

/*!
    \brief      enable comparator switch
    \param[in]  none
    \param[out] none
    \retval     none
*/
void cmp_channel0_switch_enable(void)
{
    CMP_CS |= CMP_CS_CMP0S;
}

/*!
    \brief      disable comparator switch
    \param[in]  cmp_channal
      \arg        CMP_CS_CMP0S 
    \param[out] none
    \retval     none
*/
void cmp_channel0_switch_disable(void)
{
    CMP_CS &= ~CMP_CS_CMP0S;
}

/*!
    \brief      get output level
    \param[in]  cmp_channal
      \arg        CMP_CHANNEL_CMP0
      \arg        CMP_CHANNEL_CMP1
    \param[out] none
    \retval     the output level
*/
uint32_t cmp_output_level(uint32_t cmp_channal)
{
    if(CMP_CHANNEL_CMP0 == cmp_channal){
        if(CMP_CS & CMP_CS_CMP0O){
            return CMP_OUTPUTLEVEL_HIGH;
        }else{
            return CMP_OUTPUTLEVEL_LOW;
        }
    }else{
        if(CMP_CS & CMP_CS_CMP1O){
            return CMP_OUTPUTLEVEL_HIGH;
        }else{
            return CMP_OUTPUTLEVEL_LOW;
        }
    }
}

/*!
    \brief      enable the window mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void cmp_window_enable(void)
{
    CMP_CS |= CMP_CS_WNDEN;
}

/*!
    \brief      disable the window mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void cmp_window_disable(void)
{
    CMP_CS &= ~CMP_CS_WNDEN;
}

/*!
    \brief      lock the comparator
    \param[in]  cmp_channal
      \arg        CMP_CHANNEL_CMP0
      \arg        CMP_CHANNEL_CMP1
    \param[out] none
    \retval     none
*/
void cmp_lock_enable(uint32_t cmp_channal)
{
    if(CMP_CHANNEL_CMP0 == cmp_channal){
        CMP_CS |= CMP_CS_CMP0LK;
    }else{
        CMP_CS |= CMP_CS_CMP1LK;
    }
}

/*!
    \brief      unlock the comparator
    \param[in]  cmp_channal
      \arg        CMP_CHANNEL_CMP0
      \arg        CMP_CHANNEL_CMP1
    \param[out] none
    \retval     none
*/
void cmp_lock_disable(uint32_t cmp_channal)
{
    if(CMP_CHANNEL_CMP0 == cmp_channal){
        CMP_CS &= ~CMP_CS_CMP0LK;
    }else{
        CMP_CS &= ~CMP_CS_CMP1LK;
    }
}
