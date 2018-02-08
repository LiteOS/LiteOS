/*!
    \file  gd32f1x0_adc.c
    \brief adc driver
*/

/*
    Copyright (C) 2016 GigaDevice

    2014-12-26, V1.0.0, firmware for GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, firmware for GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#include "gd32f1x0_adc.h"
#include "gd32f1x0_rcu.h"

/*!
    \brief      ADC reset
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_deinit(void)
{
    rcu_periph_reset_enable(RCU_ADCRST);
    rcu_periph_reset_disable(RCU_ADCRST);
}

/*!
    \brief      get the ADC flag bits
    \param[in]  adc_flag: the adc flag bits
      \arg        ADC_FLAG_WDE: analog watchdog event flag
      \arg        ADC_FLAG_EOC: end of group conversion flag
      \arg        ADC_FLAG_EOIC: end of inserted group conversion flag
      \arg        ADC_FLAG_STIC: start flag of inserted channel group
      \arg        ADC_FLAG_STRC: start flag of regular channel group
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus adc_flag_get(uint8_t adc_flag)
{
    if(ADC_STAT&adc_flag)
        return SET;
    return RESET;

}


/*!
    \brief      clear the ADC status flag
    \param[in]  adc_flag: the adc flag bits
      \arg        ADC_FLAG_WDE: analog watchdog event flag
      \arg        ADC_FLAG_EOC: end of group conversion flag
      \arg        ADC_FLAG_EOIC: end of inserted group conversion flag
      \arg        ADC_FLAG_STIC: start flag of inserted channel group
      \arg        ADC_FLAG_STRC: start flag of regular channel group
    \param[out] none
    \retval     none
*/
void adc_flag_clear(uint8_t adc_flag)
{
    ADC_STAT &= (uint32_t)~(adc_flag);
}

/*!
    \brief      get the ADC interrupt bits
    \param[in]  adc_interrupt: the adc interrupt bits
      \arg        ADC_INT_WDE: analog watchdog interrupt
      \arg        ADC_INT_EOC: end of group conversion interrupt
      \arg        ADC_INT_EOIC: end of inserted group conversion interrupt
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus adc_interrupt_flag_get(uint8_t adc_interrupt)
{
    FlagStatus interrupt_flag = RESET;
    /* check the interrupt bits */
    switch(adc_interrupt){
    case ADC_INT_WDE:
        if((ADC_CTL0&ADC_CTL0_WDEIE)&&(ADC_STAT&ADC_STAT_WDE))
            interrupt_flag = SET;
        break;
    case ADC_INT_EOC:
        if((ADC_CTL0&ADC_CTL0_EOCIE)&&(ADC_STAT&ADC_STAT_EOC))
            interrupt_flag = SET;
        break;
    case ADC_INT_EOIC:
        if((ADC_CTL0&ADC_CTL0_EOICIE)&&(ADC_STAT&ADC_STAT_EOIC))
            interrupt_flag = SET;
        break;
    default:
        break;
    }
    return interrupt_flag;
}

/*!
    \brief      clear the ADC interrupt bits
    \param[in]  adc_interrupt: the adc interrupt bits
      \arg        ADC_INT_WDE: analog watchdog interrupt
      \arg        ADC_INT_EOC: end of group conversion interrupt
      \arg        ADC_INT_EOIC: end of inserted group conversion interrupt
    \param[out] none
    \retval     none
*/
void adc_interrupt_flag_clear(uint8_t adc_interrupt)
{
    switch(adc_interrupt){
    case ADC_INT_WDE:
        ADC_STAT &= (uint32_t)~(ADC_FLAG_WDE);
        break;
    case ADC_INT_EOC:
        ADC_STAT &= (uint32_t)~(ADC_FLAG_EOC);
        break;
    case ADC_INT_EOIC:
        ADC_STAT &= (uint32_t)~(ADC_FLAG_EOIC);
        break;
    default:
        break;
    }
}

/*!
    \brief      ADC interrupt config
    \param[in]  adc_flag: the adc interrupt flag
      \arg        ADC_INT_WDE: analog watchdog interrupt flag
      \arg        ADC_INT_EOC: end of group conversion interrupt flag
      \arg        ADC_INT_EOIC: end of inserted group conversion interrupt flag
    \param[in]  newvalue: ENABLE or DISABLE
    \param[out] none
    \retval     none
*/
void adc_interrupt_config(uint8_t adc_int,ControlStatus newvalue)
{
    /* ENABLE or DISABLE the interrupt */
    if(newvalue){
        /* select the interrupt source */
        switch(adc_int){
        case ADC_INT_WDE:
            ADC_CTL0 |= (uint32_t) ADC_CTL0_WDEIE;
            break;
        case ADC_INT_EOC:
            ADC_CTL0 |= (uint32_t) ADC_CTL0_EOCIE;
            break;
        case ADC_INT_EOIC:
            ADC_CTL0 |= (uint32_t) ADC_CTL0_EOICIE;
            break;
        default:
            break;
        }
    }else{
        switch(adc_int){
        /* select the interrupt source */
        case ADC_INT_WDE:
            ADC_CTL0 &= (uint32_t)~(ADC_CTL0_WDEIE);
            break;
        case ADC_INT_EOC:
            ADC_CTL0 &= (uint32_t)~(ADC_CTL0_EOCIE);
            break;
        case ADC_INT_EOIC:
            ADC_CTL0 &= (uint32_t)~(ADC_CTL0_EOICIE);
            break;
        default:
            break;
        }
    }
}

/*!
    \brief      ADC interrupt enable
    \param[in]  adc_flag: the adc interrupt flag
      \arg        ADC_INT_WDE: analog watchdog interrupt flag
      \arg        ADC_INT_EOC: end of group conversion interrupt flag
      \arg        ADC_INT_EOIC: end of inserted group conversion interrupt flag
    \param[out] none
    \retval     none
*/
void adc_interrupt_enable(uint8_t adc_int)
{
    switch(adc_int){
    case ADC_INT_WDE:
        ADC_CTL0 |= (uint32_t) ADC_CTL0_WDEIE;
        break;
    case ADC_INT_EOC:
        ADC_CTL0 |= (uint32_t) ADC_CTL0_EOCIE;
        break;
    case ADC_INT_EOIC:
        ADC_CTL0 |= (uint32_t) ADC_CTL0_EOICIE;
        break;
    default:
        break;
    }
}

/*!
    \brief      ADC interrupt disable
    \param[in]  adc_flag: the adc interrupt flag
      \arg        ADC_INT_WDE: analog watchdog interrupt flag
      \arg        ADC_INT_EOC: end of group conversion interrupt flag
      \arg        ADC_INT_EOIC: end of inserted group conversion interrupt flag
    \param[out] none
    \retval     none
*/
void adc_interrupt_disable(uint8_t adc_int)
{
    switch(adc_int){
    /* select the interrupt source */
    case ADC_INT_WDE:
        ADC_CTL0 &= (uint32_t)~(ADC_CTL0_WDEIE);
        break;
    case ADC_INT_EOC:
        ADC_CTL0 &= (uint32_t)~(ADC_CTL0_EOCIE);
        break;
    case ADC_INT_EOIC:
        ADC_CTL0 &= (uint32_t)~(ADC_CTL0_EOICIE);
        break;
    default:
        break;
    }
}

/*!
    \brief      ADC analog watchdog single channel config
    \param[in]  adc_channel: the selected ADC channel
      \arg        ADC_CHANNEL_x: ADC Channelx(x=0..18)
    \param[out] none
    \retval     none
*/
void adc_wde_single_channel_enable(uint8_t adc_channel)
{
    ADC_CTL0 &= (uint32_t)~(ADC_CTL0_RWDEN | ADC_CTL0_IWDEN | ADC_CTL0_WDSC | ADC_CTL0_WDCHSEL);

    ADC_CTL0 |= (uint32_t)adc_channel;
    ADC_CTL0 |= (uint32_t)(ADC_CTL0_RWDEN | ADC_CTL0_IWDEN | ADC_CTL0_WDSC);
}

/*!
    \brief      adc analog watchdog group channel config
    \param[in]  adc_channel_group: the channel group use analog watchdog 
      \arg        ADC_REGULAR_CHANNEL: regular channel group
      \arg        ADC_INSERTED_CHANNEL: inserted channel group
      \arg        ADC_REGULAR_INSERTED_CHANNEL: both regular and inserted group
    \param[out] none
    \retval     none
*/
void adc_wde_group_channel_enable(uint8_t adc_channel_group)
{
    ADC_CTL0 &= (uint32_t)~(ADC_CTL0_RWDEN | ADC_CTL0_IWDEN | ADC_CTL0_WDSC);
    /* select the group */
    switch(adc_channel_group){
    case ADC_REGULAR_CHANNEL:
        ADC_CTL0 |= (uint32_t) ADC_CTL0_RWDEN;
        break;
    case ADC_INSERTED_CHANNEL:
        ADC_CTL0 |= (uint32_t) ADC_CTL0_IWDEN;
        break;
    case ADC_REGULAR_INSERTED_CHANNEL:
        ADC_CTL0 |= (uint32_t)(ADC_CTL0_RWDEN | ADC_CTL0_IWDEN);
        break;
    default:
        break;
    }
}

/*!
    \brief      ADC analog watchdog threshold config
    \param[in]  low_threshold: analog watchdog low threshold
    \param[in]  high_threshold: analog watchdog high threshold
    \param[out] none
    \retval     none
*/
void adc_wde_threshold_config(uint16_t low_threshold,uint16_t high_threshold)
{
    ADC_WDLT = low_threshold;
    ADC_WDHT = high_threshold;
}

/*!
    \brief      ADC analog watchdog disable
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_wde_disable(void)
{
    ADC_CTL0 &= (uint32_t)~(ADC_CTL0_RWDEN | ADC_CTL0_IWDEN | ADC_CTL0_WDSC | ADC_CTL0_WDCHSEL);
}

/*!
    \brief      config the length of regular channel group or inserted channel group
    \param[in]  adc_channel_group: select the channel group
      \arg        ADC_REGULAR_CHANNEL: regular channel group
      \arg        ADC_INSERTED_CHANNEL: inserted channel group
    \param[in]  length: the length of the channel
                        regular channel 1-18
                        inserted channel 1-4
    \param[out] none
    \retval     none
*/
void adc_channel_length_config(uint8_t adc_channel_group,uint8_t length)
{
    switch(adc_channel_group){
    case ADC_REGULAR_CHANNEL:
        ADC_RSQ0 &= (uint32_t)~((length-1) << 20);
        ADC_RSQ0 |= (uint32_t)((length-1) << 20);
        break;
    case ADC_INSERTED_CHANNEL:
        ADC_ISQ &= (uint32_t)~((length-1) << 20);
        ADC_ISQ |= (uint32_t)((length-1) << 20);
        break;
    default:
        break;
    }
}

/*!
    \brief      ADC regular channel config
    \param[in]  rank: the regular group sequencer rank,this parameter must be between 1 to 16
    \param[in]  adc_channel: the selected ADC channel
      \arg        ADC_CHANNEL_x(x=0..18): ADC Channelx
    \param[in]  sample_time: the sample time value
      \arg        ADC_SAMPLETIME_1POINT5: 1.5 cycles
      \arg        ADC_SAMPLETIME_7POINT5: 7.5 cycles
      \arg        ADC_SAMPLETIME_13POINT5: 13.5 cycles
      \arg        ADC_SAMPLETIME_28POINT5: 28.5 cycles
      \arg        ADC_SAMPLETIME_41POINT5: 41.5 cycles
      \arg        ADC_SAMPLETIME_55POINT5: 55.5 cycles
      \arg        ADC_SAMPLETIME_71POINT5: 71.5 cycles
      \arg        ADC_SAMPLETIME_239POINT5: 239.5 cycles
    \param[out] none
    \retval     none
*/
void adc_regular_channel_config(uint8_t rank,uint8_t adc_channel,uint32_t sample_time)
{
    uint32_t rsq,sampt;
    
    /* ADC regular sequence config */
    if(rank < 7){
        rsq = ADC_RSQ2;
        rsq &= (uint32_t)~(ADC_RSQX_RSQn << (5*(rank-1)));
        rsq |= (uint32_t)(adc_channel << (5*(rank-1)));
        ADC_RSQ2 = rsq;
    }else if(rank < 13){
        rsq = ADC_RSQ1;
        rsq &= (uint32_t)~(ADC_RSQX_RSQn << (5*(rank-7)));
        rsq |= (uint32_t)(adc_channel << (5*(rank-7)));
        ADC_RSQ1 = rsq;
    }else if(rank < 17){
        rsq = ADC_RSQ0;
        rsq &= (uint32_t)~(ADC_RSQX_RSQn << (5*(rank-13)));
        rsq |= (uint32_t)(adc_channel << (5*(rank-13)));
        ADC_RSQ0 = rsq;
    }
    
    /* ADC sampling time config */
    if(adc_channel < 10){
        sampt = ADC_SAMPT1;
        sampt &= (uint32_t)~(ADC_SAMPTX_SPTN << (3*adc_channel));
        sampt |= (uint32_t)(sample_time << (3*adc_channel));
        ADC_SAMPT1= sampt;
    }else if(adc_channel < 19){
        sampt = ADC_SAMPT0;
        sampt &= (uint32_t)~(ADC_SAMPTX_SPTN << (3*(adc_channel-10)));
        sampt |= (uint32_t)(sample_time << (3*(adc_channel-10)));
        ADC_SAMPT0= sampt;
    }
}

/*!
    \brief      ADC inserted channel config
    \param[in]  rank: the inserted group sequencer rank,this parameter must be between 1 to 4
    \param[in]  adc_channel: the selected ADC channel
      \arg        ADC_CHANNEL_x(x=0..18): ADC Channelx
    \param[in]  sample_time: The sample time value
      \arg        ADC_SAMPLETIME_1POINT5: 1.5 cycles
      \arg        ADC_SAMPLETIME_7POINT5: 7.5 cycles
      \arg        ADC_SAMPLETIME_13POINT5: 13.5 cycles
      \arg        ADC_SAMPLETIME_28POINT5: 28.5 cycles
      \arg        ADC_SAMPLETIME_41POINT5: 41.5 cycles
      \arg        ADC_SAMPLETIME_55POINT5: 55.5 cycles
      \arg        ADC_SAMPLETIME_71POINT5: 71.5 cycles
      \arg        ADC_SAMPLETIME_239POINT5: 239.5 cycles
    \param[out] none
    \retval     none
*/
void adc_inserted_channel_config(uint8_t rank,uint8_t adc_channel,uint8_t sample_time)
{
    uint8_t inserted_length;
    uint32_t isq,sampt;

    inserted_length = GET_BITS(ADC_ISQ, 20, 21);
    if(rank < 5){
        isq = ADC_ISQ;
        isq &= (uint32_t)~(ADC_ISQ_ISQn << (15-(inserted_length-rank+1)*5));
        isq |= (uint32_t)(adc_channel << (15-(inserted_length-rank+1)*5));
        ADC_ISQ = isq;
    }

    if(adc_channel < 10){
        sampt = ADC_SAMPT1;
        sampt &= (uint32_t)~(ADC_SAMPTX_SPTN << (3*adc_channel));
        sampt |= (uint32_t) sample_time << (3*adc_channel);
        ADC_SAMPT1 = sampt;
    }else if(adc_channel < 19){
        sampt = ADC_SAMPT0;
        sampt &= (uint32_t)~(ADC_SAMPTX_SPTN << (3*(adc_channel-10)));
        sampt |= (uint32_t)(sample_time << (3*(adc_channel-10)));
        ADC_SAMPT0 = sampt;
    }
}

/*!
    \brief      adc special function enable or disable
    \param[in]  function: select the function to config
      \arg        ADC_SCAN_MODE: scan mode select
      \arg        ADC_INSERTED_CHANNEL_AUTO: inserted channel group convert automatically
      \arg        ADC_VBAT_CHANNEL_SWITCH: VBAT channel select
      \arg        ADC_TEMP_VREF_CHANNEL_SWITCH: channel 16 and 17 select
      \arg        ADC_DMA_REQUEST: DMA request select
      \arg        ADC_CONTINUOUS_MODE: continuous mode select
    \param[in]  newvalue: ENABLE or DISABLE
    \param[out] none
    \retval     none
*/
void adc_special_function_config(uint8_t function,ControlStatus newvalue)
{
    if(newvalue){
        switch(function){
        case ADC_SCAN_MODE:
            ADC_CTL0 |= (uint32_t)ADC_CTL0_SM;
            break;
        case ADC_INSERTED_CHANNEL_AUTO:
            ADC_CTL0 |= (uint32_t)ADC_CTL0_ICA;
            break;
        case ADC_VBAT_CHANNEL_SWITCH:
            ADC_CTL1 |= (uint32_t)ADC_CTL1_VBETEN;
            break;
        case ADC_TEMP_VREF_CHANNEL_SWITCH:
            ADC_CTL1 |= (uint32_t)ADC_CTL1_TSVREN;
            break;
        case ADC_DMA_REQUEST:
            ADC_CTL1 |= (uint32_t)ADC_CTL1_DMA;
            break;
        case ADC_CONTINUOUS_MODE:
            ADC_CTL1 |= (uint32_t)ADC_CTL1_CTN;
            break;
        default:
            break;
        }
    }else{
        switch(function){
        case ADC_SCAN_MODE:
            ADC_CTL0 &= (uint32_t)~(ADC_CTL0_SM);
            break;
        case ADC_INSERTED_CHANNEL_AUTO:
            ADC_CTL0 &= (uint32_t)~(ADC_CTL0_ICA);
            break;
        case ADC_VBAT_CHANNEL_SWITCH:
            ADC_CTL1 &= (uint32_t)~(ADC_CTL1_VBETEN);
            break;
        case ADC_TEMP_VREF_CHANNEL_SWITCH:
            ADC_CTL1 &= (uint32_t)~(ADC_CTL1_TSVREN);
            break;
        case ADC_DMA_REQUEST:
            ADC_CTL1 &= (uint32_t)~(ADC_CTL1_DMA);
            break;
        case ADC_CONTINUOUS_MODE:
            ADC_CTL1 &= (uint32_t)~(ADC_CTL1_CTN);
            break;
        default:
            break;
        }
    }
}

/*!
    \brief      adc discontinuous mode config
    \param[in]  adc_channel_group: select the channel group
      \arg        ADC_REGULAR_CHANNEL: regular channel group
      \arg        ADC_INSERTED_CHANNEL: inserted channel group
    \param[in]  length: number of conversions in discontinuous mode,the number can be 1..8
                        for regular channel ,the number is insignificant for inserted channel
    \param[out] none
    \retval     none
*/
void adc_discontinuous_mode_config(uint8_t adc_channel_group,uint8_t length)
{
    ADC_CTL0 &= (uint32_t)~( ADC_CTL0_DISRC | ADC_CTL0_DISIC );
    switch(adc_channel_group){
    case ADC_REGULAR_CHANNEL:
        /* config the number of conversions in discontinuous mode  */
        ADC_CTL0 &= (uint32_t)~(ADC_CTL0_DISNUM);
        ADC_CTL0 |= (uint32_t)(length - 1) << 13;

        ADC_CTL0 |= (uint32_t)ADC_CTL0_DISRC;
        break;
    case ADC_INSERTED_CHANNEL:
        ADC_CTL0 |= (uint32_t)ADC_CTL0_DISIC;
        break;
    default:
        break;
    }
    

}

/*!
    \brief      adc external trigger enable
    \param[in]  adc_channel_group: select the channel group
      \arg        ADC_REGULAR_CHANNEL: regular channel group
      \arg        ADC_INSERTED_CHANNEL: inserted channel group
    \param[in]  newvalue: ENABLE or DISABLE
    \param[out] none
    \retval     none
*/
void adc_external_trigger_config(uint8_t adc_channel_group,ControlStatus newvalue)
{
    if(newvalue){
        switch(adc_channel_group){
        case ADC_REGULAR_CHANNEL:
            ADC_CTL1 |= (uint32_t)ADC_CTL1_ETERC;
            break;
        case ADC_INSERTED_CHANNEL:
            ADC_CTL1 |= (uint32_t)ADC_CTL1_ETEIC;
            break;
        default:
            break;
        }
    }else{
        switch(adc_channel_group){
        case ADC_REGULAR_CHANNEL:
            ADC_CTL1 &= (uint32_t)~(ADC_CTL1_ETERC);
            break;
        case ADC_INSERTED_CHANNEL:
            ADC_CTL1 &= (uint32_t)~(ADC_CTL1_ETEIC);
            break;
        default:
            break;
        }
    }
}

/*!
    \brief      adc external trigger source config
    \param[in]  adc_channel_group: select the channel group
      \arg        ADC_REGULAR_CHANNEL: regular channel group
      \arg        ADC_INSERTED_CHANNEL: inserted channel group
    \param[in]  external_trigger_source: regular or inserted group trigger source
                for regular channel:
      \arg        ADC_EXTTRIG_REGULAR_T0_CC0: external trigger timer 0 CC0 event select for regular channel
      \arg        ADC_EXTTRIG_REGULAR_T0_CC1: external trigger timer 0 CC1 event select for regular channel
      \arg        ADC_EXTTRIG_REGULAR_T0_CC2: external trigger timer 0 CC2 event select for regular channel
      \arg        ADC_EXTTRIG_REGULAR_T1_CC1: external trigger timer 1 CC1 event select for regular channel
      \arg        ADC_EXTTRIG_REGULAR_T2_TRGO: external trigger timer 2 TRGO event select for regular channel
      \arg        ADC_EXTTRIG_REGULAR_T14_CC0: external trigger timer 14 CC0 event select for regular channel
      \arg        ADC_EXTTRIG_REGULAR_EXT_IT11: external trigger extiline 11 select for regular channel
      \arg        ADC_EXTTRIG_REGULAR_SWRCST: software trigger select for regular channel
                for inserted channel:
      \arg        ADC_EXTTRIG_INSERTED_T0_TRGO: timer0 TRGO event
      \arg        ADC_EXTTRIG_INSERTED_T0_CC3: timer0 capture compare4
      \arg        ADC_EXTTRIG_INSERTED_T1_TRGO: timer1 TRGO event
      \arg        ADC_EXTTRIG_INSERTED_T1_CC0: timer1 capture compare1
      \arg        ADC_EXTTRIG_INSERTED_T2_CC3: timer2 capture compare4
      \arg        ADC_EXTTRIG_INSERTED_T14_TRGO: timer14 TRGO event
      \arg        ADC_EXTTRIG_INSERTED_EXT_IT15: external interrupt line 15
      \arg        ADC_EXTTRIG_INSERTED_SWRCST: software trigger select for inserted channel
    \param[out] none
    \retval     none
*/
void adc_external_trigger_source_config(uint8_t adc_channel_group,uint32_t external_trigger_source)
{   
    switch(adc_channel_group){
    case ADC_REGULAR_CHANNEL:
        ADC_CTL1 &= (uint32_t)~(ADC_CTL1_ETSRC);
        ADC_CTL1 |= (uint32_t)external_trigger_source;
        break;
    case ADC_INSERTED_CHANNEL:
        ADC_CTL1 &= (uint32_t)~(ADC_CTL1_ETSIC);
        ADC_CTL1 |= (uint32_t)external_trigger_source;
        break;
    default:
        break;
    }
}

/*!
    \brief      adc software trigger enable
    \param[in]  adc_channel_group: select the channel group
      \arg        ADC_REGULAR_CHANNEL: regular channel group
      \arg        ADC_INSERTED_CHANNEL: inserted channel group
    \param[out] none
    \retval     none
*/
void adc_software_trigger_enable(uint8_t adc_channel_group)
{
    switch(adc_channel_group){
    case ADC_REGULAR_CHANNEL:
        ADC_CTL1 |= (uint32_t)ADC_CTL1_SWRCST;
        break;
    case ADC_INSERTED_CHANNEL:
        ADC_CTL1 |= (uint32_t)ADC_CTL1_SWICST;
        break;
    default:
        break;
    }
}

/*!
    \brief      adc regular group data register read
    \param[in]  none
    \param[out] none
    \retval     the conversion value
*/
uint16_t adc_regular_data_read(void)
{
    return (ADC_RDATA);
}

/*!
    \brief      adc inserted group data register read
    \param[in]  inserted_channel : insert channel select
      \arg        ADC_INSERTED_CHANNEL_0: inserted Channel0
      \arg        ADC_INSERTED_CHANNEL_1: inserted channel1
      \arg        ADC_INSERTED_CHANNEL_2: inserted Channel2
      \arg        ADC_INSERTED_CHANNEL_3: inserted Channel3
    \param[out] none
    \retval     the conversion value
*/
uint16_t adc_inserted_data_read(uint8_t inserted_channel)
{
    uint16_t idata;
    /* read the data of the selected channel */
    switch(inserted_channel){
    case ADC_INSERTED_CHANNEL_0:
        idata = ADC_IDATA0;
        break;
    case ADC_INSERTED_CHANNEL_1:
        idata = ADC_IDATA1;
        break;
    case ADC_INSERTED_CHANNEL_2:
        idata = ADC_IDATA2;
        break;
    case ADC_INSERTED_CHANNEL_3:
        idata = ADC_IDATA3;
        break;
    default:
        idata = 0;
        break;
    }
    return idata;
}

/*!
    \brief      ADC calibration and reset calibration
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_calibration_enable(void)
{
    /* reset the selected ADC1 calibration registers */
    ADC_CTL1 |= (uint32_t) ADC_CTL1_RSTCLB;
    /* check the RSTCLB bit state */
    while((ADC_CTL1 & ADC_CTL1_RSTCLB));
    /* enable ADC calibration process */
    ADC_CTL1 |= ADC_CTL1_CLB;
    /* check the CLB bit state */
    while((ADC_CTL1 & ADC_CTL1_CLB));
}

/*!
    \brief      adc data alignment config
    \param[in]  inserted_channel : insert channel select
      \arg        ADC_DATAALIGN_RIGHT: LSB alignment
      \arg        ADC_DATAALIGN_LEFT: MSB alignment
    \param[out] none
    \retval     none
*/
void adc_data_alignment_config(uint8_t data_alignment)
{
    if(data_alignment){
        ADC_CTL1 |= ADC_CTL1_DAL;
    }else{
        ADC_CTL1 &= (uint32_t)~(ADC_CTL1_DAL);
    }
}

/*!
    \brief      adc inserted channel offset config
    \param[in]  inserted_channel : insert channel select
      \arg        ADC_INSERTED_CHANNEL_0: inserted Channel0
      \arg        ADC_INSERTED_CHANNEL_1: inserted channel1
      \arg        ADC_INSERTED_CHANNEL_2: inserted Channel2
      \arg        ADC_INSERTED_CHANNEL_3: inserted Channel3
    \param[in]  offset : the offset data
    \param[out] none
    \retval     the conversion value
*/
void adc_inserted_channel_offset_config(uint8_t inserted_channel,uint16_t offset)
{
    /* config the offset of the selected channels */
    if(ADC_INSERTED_CHANNEL_0 == inserted_channel)
        ADC_IOFF0 = offset;
    else if(ADC_INSERTED_CHANNEL_1 == inserted_channel)
        ADC_IOFF1 = offset;
    else if(ADC_INSERTED_CHANNEL_2 == inserted_channel)
        ADC_IOFF2 = offset;
    else if(ADC_INSERTED_CHANNEL_3 == inserted_channel)
        ADC_IOFF3 = offset;
}

/*!
    \brief      enable ADC interface
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_enable(void)
{
    if(RESET == (ADC_CTL1 & ADC_CTL1_ADCON))
        ADC_CTL1 |= (uint32_t)ADC_CTL1_ADCON;
}

/*!
    \brief      disable ADC interface
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_disable(void)
{
    ADC_CTL1 &= (uint32_t)~(ADC_CTL1_ADCON);
}


#ifdef GD32F170_190
/*!
    \brief      adc resolution config
    \param[in]  resolution: ADC resolution
      \arg        ADC_RESOLUTION_12B: 12-bit ADC resolution
      \arg        ADC_RESOLUTION_10B: 10-bit ADC resolution
      \arg        ADC_RESOLUTION_8B: 8-bit ADC resolution
      \arg        ADC_RESOLUTION_6B: 6-bit ADC resolution
    \param[out] none
    \retval     none
*/
void adc_resolution_config(uint32_t resolution)
{
    ADC_CTL0 &= (uint32_t)~(ADC_CTL0_DRES);
    ADC_CTL0 |= (uint32_t)resolution;
}

/*!
    \brief      adc oversample mode config
    \param[in]  mode: ADC oversampling mode
      \arg        ADC_OVERSAMPLING_ALL_CONVERT: all oversampled conversions for a channel 
                are done consecutively after a trigger
      \arg        ADC_OVERSAMPLING_ONE_CONVERT: each oversampled conversion for a channel 
                needs a trigger
    \param[in]  shift: ADC oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_NONE: no oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_1B: 1-bit oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_2B: 2-bit oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_3B: 3-bit oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_4B: 3-bit oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_5B: 5-bit oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_6B: 6-bit oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_7B: 7-bit oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_8B: 8-bit oversampling shift
    \param[in]  ratio: ADC oversampling ratio
      \arg        ADC_OVERSAMPLING_RATIO_2X: oversampling ratio X2
      \arg        ADC_OVERSAMPLING_RATIO_4X: oversampling ratio X4
      \arg        ADC_OVERSAMPLING_RATIO_8X: oversampling ratio X8
      \arg        ADC_OVERSAMPLING_RATIO_16X: oversampling ratio X16
      \arg        ADC_OVERSAMPLING_RATIO_32X: oversampling ratio X32
      \arg        ADC_OVERSAMPLING_RATIO_64X: oversampling ratio X64
      \arg        ADC_OVERSAMPLING_RATIO_128X: oversampling ratio X128
      \arg        ADC_OVERSAMPLING_RATIO_256X: oversampling ratio X256
    \param[out] none
    \retval     none
*/
void adc_oversample_mode_config(uint8_t mode,uint16_t shift,uint8_t ratio)
{
    if(mode){
        ADC_OVCTL |= (uint32_t)ADC_OVCTL_TOVS;
    }else{
        ADC_OVCTL &= (uint32_t)~(ADC_OVCTL_TOVS);
    }
    /* config the shift and ratio */
    ADC_OVCTL &= (uint32_t)~(ADC_OVCTL_OVSR | ADC_OVCTL_OVSS);
    ADC_OVCTL |= (uint32_t)(shift | ratio);
}

/*!
    \brief      ADC oversample mode enable
    \param[in]  newvalue: ENBALE or DISABLE
    \param[out] none
    \retval     none
*/
void adc_oversample_mode_enable(ControlStatus newvalue)
{
    if(newvalue){
        ADC_OVCTL |= ADC_OVCTL_OVSEN;
    }
    else{
        ADC_OVCTL &= (uint32_t)~(ADC_OVCTL_OVSEN);
    }
}

#endif /* GD32F170_190 */
