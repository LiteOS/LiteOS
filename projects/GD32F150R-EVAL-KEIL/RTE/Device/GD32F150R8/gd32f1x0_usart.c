/*!
    \file  gd32f1x0_usart.c
    \brief usart driver
*/

/*
    Copyright (C) 2016 GigaDevice

    2014-12-26  V1.0.0, platform GD32F1x0(x=3,5)
    2016-12-23  V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#include "gd32f1x0_usart.h"
#include "gd32f1x0_rcu.h"

/*!
    \brief      reset USART
    \param[in]  usart_periph: USARTx(x=0,1)
    \param[out] none
    \retval     none
*/
void usart_deinit(uint32_t usart_periph)
{
    switch(usart_periph){
    case USART0:
        rcu_periph_reset_enable(RCU_USART0RST);
        rcu_periph_reset_disable(RCU_USART0RST);
        break;
    case USART1:
        rcu_periph_reset_enable(RCU_USART1RST);
        rcu_periph_reset_disable(RCU_USART1RST);        
        break;
    default:
        break;
    }
}

/*!
    \brief      configure usart baud rate value
    \param[in]  usart_periph: USARTx(x=0,1)
    \param[in]  baudval: baud rate value
    \param[out] none
    \retval     none
*/ 
void usart_baudrate_set(uint32_t usart_periph,uint32_t baudval)
{
    uint32_t uclk=0,intdiv=0,fradiv=0,udiv=0;
    switch(usart_periph){
    case USART0:
         uclk=rcu_clock_freq_get(CK_USART);
         break;
    case USART1:
         uclk=rcu_clock_freq_get(CK_APB1);
         break;
    default:
         break;        
    }
    if(USART_CTL0(usart_periph)&USART_CTL0_OVSMOD){
        /* when oversampling by 8,configure the value of USART_BAUD */
        udiv=((2*uclk)+baudval/2)/baudval;
        intdiv=udiv&0xfff0;
        fradiv=udiv&0x7;
        USART_BAUD(usart_periph) |=((USART_BAUD_FRADIV|USART_BAUD_INTDIV)&( intdiv|fradiv));
    }else{
        /* when oversampling by 16,configure the value of USART_BAUD */
        udiv=(uclk+baudval/2)/baudval;
        intdiv=udiv&0xfff0;
        fradiv=udiv&0xf;
        USART_BAUD(usart_periph) |=((USART_BAUD_FRADIV|USART_BAUD_INTDIV) &( intdiv|fradiv));
    }   
}

/*!
    \brief     configure usart parity function
    \param[in] usart_periph: USARTx(x=0.1)
    \param[in] paritycfg:    usart parity configure
      \arg       USART_PM_NONE: no parity
      \arg       USART_PM_ODD:  odd parity
      \arg       USART_PM_EVEN: even parity 
    \param[out] none
    \retval     none
*/
void usart_parity_config(uint32_t usart_periph,uint32_t paritycfg)
{
    /* disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
    /* clear USART_CTL0 PM,PCEN Bits */
    USART_CTL0(usart_periph) &=~(USART_CTL0_PM|USART_CTL0_PCEN); 
     /* configure usart parity mode */
    USART_CTL0(usart_periph) |=paritycfg ;
    

}

/*!
    \brief     configure usart word length
    \param[in] usart_periph: USARTx(x=0.1)
    \param[in] wlen: usart word length configure
      \arg       USART_WL_8BIT: 8 bits
      \arg       USART_WL_9BIT: 9 bits   
    \param[out] none
    \retval     none
*/
void usart_word_length_set(uint32_t usart_periph,uint32_t wlen)
{
    /* disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
    /* clear USART_CTL0 WL bit */
    USART_CTL0(usart_periph) &=~USART_CTL0_WL; 
    
    /* configure usart word length */
    USART_CTL0(usart_periph) |=wlen ;        
}

/*!
    \brief     configure usart stop bit length
    \param[in] usart_periph: USARTx(x=0.1)
    \param[in] stblen:       usart stop bit configure
      \arg       USART_STB_1BIT:   1 bit
      \arg       USART_STB_2BIT:   2 bit
      \arg       USART_STB_1_5BIT: 1.5bit
    \param[out] none
    \retval     none
*/
void usart_stop_bit_set(uint32_t usart_periph,uint32_t stblen)
{
    /* disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
    /* clear USART_CTL1 STB bits */
    USART_CTL1(usart_periph) &=~USART_CTL1_STB; 
    
    USART_CTL1(usart_periph) |=stblen;
}
/*!
    \brief      enable usart
    \param[in]  usart_periph: USARTx(x=0,1)
    \param[out] none
    \retval     none
*/
void usart_enable(uint32_t usart_periph)
{
    USART_CTL0(usart_periph) |=USART_CTL0_UEN;
}

/*!
    \brief     disable usart
    \param[in] usart_periph: USARTx(x=0,1)
    \param[out] none
    \retval     none
*/
void usart_disable(uint32_t usart_periph)
{
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
}
/*!
    \brief      USART transfer config
    \param[in]  usart_periph: USARTx(x=0,1)
    \param[in]  rtconfig: REN or TEN
      \arg        USART_RTEN_DISABLE: transmit function and receiver function disable
      \arg        USART_REN_ENABLE: enable for reception
      \arg        USART_TEN_ENABLE: enable for transmission
      \arg        USART_REN_DISABLE: disable for reception
      \arg        USART_TEN_DISABLE: disable for transmission
      \arg        USART_RTEN_ENABLE: enable for transmission and reception 
    \param[out] none
    \retval     none
*/
void usart_transfer_config(uint32_t usart_periph,usart_transfer_enum rtconfig)
{
    switch(rtconfig){
    case USART_RTEN_ENABLE:
        USART_CTL0(usart_periph) |=(USART_CTL0_REN|USART_CTL0_TEN);
        break;
    case USART_REN_ENABLE:
        USART_CTL0(usart_periph) |=USART_CTL0_REN;
        break;
    case USART_TEN_ENABLE:
        USART_CTL0(usart_periph) |=USART_CTL0_TEN;
        break;
    case USART_REN_DISABLE:
        USART_CTL0(usart_periph) &=(~USART_CTL0_REN);
        break;
    case USART_TEN_DISABLE:
        USART_CTL0(usart_periph) &=(~USART_CTL0_TEN);
        break;
    case USART_RTEN_DISABLE:
        USART_CTL0(usart_periph) &=(~(USART_CTL0_REN|USART_CTL0_TEN));
        break;
    default:
        break;
    }
    USART_CTL0(usart_periph) |=USART_CTL0_UEN;
}

/*!
    \brief      usart transmit data function
    \param[in]  usart_periph: USARTx(x=0,1)
    \param[in]  data: data of transmission 
    \param[out] none
    \retval     none
*/
void usart_data_transmit(uint32_t usart_periph,uint32_t data)
{
    USART_TDATA(usart_periph) =(USART_TDATA_TDATA & data);
}

/*!
    \brief      usart receive data function
    \param[in]  usart_periph: USARTx(x=0,1)
    \param[out] none
    \retval     data of received
*/
uint16_t usart_data_receive(uint32_t usart_periph)
{
    return (uint16_t)(USART_RDATA(usart_periph)& USART_RDATA_RDATA); 
}

/*!
    \brief      data is transmitted/received with the LSB/MSB first
    \param[in]  usart_periph: USARTx(x=0,1)
    \param[in]  msbf: LSB/MSB
      \arg        USART_MSBF_LSB: LSB first
      \arg        USART_MSBF_MSB: MSB first
    \param[out] none
    \retval     none
*/
void usart_data_first_config(uint32_t usart_periph,uint32_t msbf)
{
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
    USART_CTL1(usart_periph) &=~(USART_CTL1_MSBF); 
    USART_CTL1(usart_periph) |=(USART_CTL1_MSBF & msbf) ;
}

/*!
    \brief      USART inverted configure
    \param[in]  usart_periph: USARTx(x=0,1)
    \param[in]  invertpara: refer to enum USART_INVERT_CONFIG
    \param[out] none
    \retval     none
*/
void usart_invert_config(uint32_t usart_periph,usart_invert_enum invertpara)
{
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
    /* inverted or not the specified siginal */ 
    switch(invertpara){
    case USART_DINV_ENABLE:
        USART_CTL1(usart_periph) |= USART_CTL1_DINV;
        break;
    case USART_DINV_DISABLE:
        USART_CTL1(usart_periph) &=~(USART_CTL1_DINV);
        break;
    case USART_TXPIN_DISABLE:
        USART_CTL1(usart_periph) &=~(USART_CTL1_TINV);
        break;
    case USART_RXPIN_DISABLE:
        USART_CTL1(usart_periph) &=~(USART_CTL1_RINV);
        break;
    case USART_SWAP_DISABLE:
        USART_CTL1(usart_periph) &=~(USART_CTL1_STRP);
        break;
    case USART_TXPIN_ENABLE:
        USART_CTL1(usart_periph) |= USART_CTL1_TINV;
        break;
    case USART_RXPIN_ENABLE:
        USART_CTL1(usart_periph) |= USART_CTL1_RINV;
        break;
    case USART_SWAP_ENABLE:
        USART_CTL1(usart_periph) |= USART_CTL1_STRP;
        break;
    default:
        break;
    }
}

/*!
    \brief      overrun functionality is enabled
    \param[in]  usart_periph: USARTx(x=0,1)
    \param[out] none
    \retval     none
*/
void usart_overrun_enable(uint32_t usart_periph)
{
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
    USART_CTL2(usart_periph) &=~(USART_CTL2_OVRD); 
}

/*!
    \brief      overrun functionality is disabled
    \param[in]  usart_periph: USARTx(x=0,1)
    \param[out] none
    \retval     none
*/
void usart_overrun_disable(uint32_t usart_periph)
{
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
    USART_CTL2(usart_periph) |= USART_CTL2_OVRD; 
}

/*!
    \brief      configure the USART oversample mode 
    \param[in]  usart_periph: USARTx(x=0,1)
    \param[in]  oversamp: oversample value
      \arg        USART_OVSMOD_8: 8 bits
      \arg        USART_OVSMOD_16: 16 bits
    \param[out] none
    \retval     none
*/
void usart_oversample_config(uint32_t usart_periph,uint32_t oversamp)
{
    /*  disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
    /*  clear OM bit */
    USART_CTL0(usart_periph) &=~(USART_CTL0_OVSMOD);
    USART_CTL0(usart_periph) |= oversamp;
}

/*!
    \brief      sample bit method configure
    \param[in]  usart_periph: USARTx(x=0,1)
    \param[in]  osb: sample bit
      \arg        USART_OSB_1bit: 1 bit
      \arg        USART_OSB_3bit: 3 bits
    \param[out] none
    \retval     none
*/
void usart_sample_bit_config(uint32_t usart_periph,uint32_t osb)
{
    /*  disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
    USART_CTL2(usart_periph) &=~(USART_CTL2_OSB); 
    USART_CTL2(usart_periph) |= (osb);
}

/*!
    \brief      auto baud rate detection enable
    \param[in]  usart_periph: USARTx(x=0)
    \param[out] none
    \retval     none
*/
void usart_autobaud_detection_enable(uint32_t usart_periph)
{
    USART_CTL1(usart_periph) |= USART_CTL1_ABDEN;
}

/*!
    \brief      auto baud rate detection disable
    \param[in]  usart_periph: USARTx(x=0)
    \param[out] none
    \retval     none
*/
void usart_autobaud_detection_disable(uint32_t usart_periph)
{
    USART_CTL1(usart_periph) &=~(USART_CTL1_ABDEN);
}

/*!
    \brief      auto baud rate detection mode configure
    \param[in]  usart_periph: USARTx(x=0)
    \param[in]  abdmod: auto baud rate detection mode
      \arg        USART_ABDM_FTOR :fall to rise
      \arg        USART_ABDM_FT0F :fall to fall
    \param[out] none
    \retval     none
*/
void usart_autobaud_detection_mode_config(uint32_t usart_periph,uint32_t abdmod)
{
    /*  reset ABDM bits */
    USART_CTL1(usart_periph) &=~(USART_CTL1_ABDM);
    USART_CTL1(usart_periph) |= (abdmod);
}

/*!
    \brief      mute mode enable 
    \param[in]  usart_periph: USARTx(x=0,1)
    \param[out] none
    \retval     none
*/
void usart_mute_mode_enable(uint32_t usart_periph)
{
    USART_CTL0(usart_periph) |= USART_CTL0_MEN;
}

/*!
    \brief      mute mode disable 
    \param[in]  usart_periph: USARTx(x=0,1)
    \param[out] none
    \retval     none
*/
void usart_mute_mode_disable(uint32_t usart_periph)
{
    USART_CTL0(usart_periph) &=~(USART_CTL0_MEN);
}

/*!
    \brief      wakeup method in mute mode configure
    \param[in]  usart_periph: USARTx(x=0,1)
    \param[in]  wmehtod: two method be used to enter or exit the mute mode.
      \arg        USART_WM_IDLE: Idle Line
      \arg        USART_WM_ADDR: address mark
    \param[out] none
    \retval     none
*/
void usart_mute_mode_wakeup_config(uint32_t usart_periph,uint32_t wmehtod)
{
    /*  disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
    USART_CTL0(usart_periph) &=~(USART_CTL0_WM );    
    USART_CTL0(usart_periph) |= wmehtod;
}

/*!
    \brief      address detection mode configure
    \param[in]  usart_periph: USARTx(x=0,1)
    \param[in]  addmod: address detection mode
      \arg        USART_ADDM_4B: 4bits
      \arg        USART_ADDM_7B: 7bits
    \param[out] none
    \retval     none
*/
void usart_address_detection_mode_config(uint32_t usart_periph,uint32_t addmod)
{
    /* disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
    USART_CTL1(usart_periph) &=~(USART_CTL1_ADDM );    
    USART_CTL1(usart_periph) |= (USART_CTL1_ADDM & (addmod));
}

/*!
    \brief      address of the USART terminal
    \param[in]  usart_periph: USARTx(x=0,1)
    \param[in]  usartaddr: address of usart node
    \param[out] none
    \retval     none
*/
void usart_address_config(uint32_t usart_periph,uint8_t usartaddr)
{
    /*  disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
    USART_CTL1(usart_periph) &=~(USART_CTL1_ADDR);    
    USART_CTL1(usart_periph) |= (USART_CTL1_ADDR & ((usartaddr)<<24));
}

/*!
    \brief      receiver timeout enable
    \param[in]  usart_periph: USARTx(x=0)
    \param[out] none
    \retval     none
*/
void usart_receiver_timeout_enable(uint32_t usart_periph)
{   
    USART_CTL1(usart_periph) |= USART_CTL1_RTEN;
}

/*!
    \brief      receiver timeout disable.
    \param[in]  usart_periph: USARTx(x=0,1)
    \param[out] none
    \retval     none
*/
void usart_receiver_timeout_disable(uint32_t usart_periph)
{   
    USART_CTL1(usart_periph) &=~(USART_CTL1_RTEN);
}

/*!
    \brief      receiver timeout threshold configure
    \param[in]  usart_periph: USARTx(x=0,1)
    \param[in]  rtimeout: receiver timeout value in terms of number of baud clocks. 
    \param[out] none
    \retval     none
*/
void usart_receiver_timeout_config(uint32_t usart_periph,uint32_t rtimeout)
{
    USART_RT(usart_periph) &=~(USART_RT_RT);
    
    USART_RT(usart_periph) |=(rtimeout);
}

/*!
    \brief      LIN mode enable
    \param[in]  usart_periph: USARTx(x=0)
    \param[out] none
    \retval     none
*/
void usart_lin_mode_enable(uint32_t usart_periph)
{   
    USART_CTL1(usart_periph) |=USART_CTL1_LMEN;
}

/*!
    \brief      LIN mode disable.
    \param[in]  usart_periph: USARTx(x=0)
    \param[out] none
    \retval     none
*/
void usart_lin_mode_disable(uint32_t usart_periph)
{   
    USART_CTL1(usart_periph) &=~(USART_CTL1_LMEN);
}

/*!
    \brief      lin break detection length
    \param[in]  usart_periph: USARTx(x=0)
    \param[in]  lblen: lin break detection length
      \arg        USART_LBLEN_10B: 10 bits break detection
      \arg        USART_LBLEN_11B: 11 bits break detection
    \param[out] none
    \retval     none
*/
void usart_lin_break_dection_length_config(uint32_t usart_periph,uint32_t lblen)
{
    /* disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
    
    USART_CTL1(usart_periph) |=(USART_CTL1_LBLEN & (lblen));
}

/*!
    \brief      half-duplex enable
    \param[in]  usart_periph: USARTx(x=0)
    \param[out] none
    \retval     none
*/
void usart_halfduplex_enable(uint32_t usart_periph)
{   
    /* disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
    
    USART_CTL2(usart_periph) |=USART_CTL2_HDEN;
}

/*!
    \brief      half-duplex disable
    \param[in]  usart_periph: USARTx(x=0)
    \param[out] none
    \retval     none
*/
void usart_halfduplex_disable(uint32_t usart_periph)
{  
    /* disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
    
    USART_CTL2(usart_periph) &=~(USART_CTL2_HDEN);
}

/*!
    \brief      clock enable
    \param[in]  usart_periph: USARTx(x=0)
    \param[out] none
    \retval     none
*/
void usart_clock_enable(uint32_t usart_periph)
{   
    /* disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
    
    USART_CTL1(usart_periph) |=USART_CTL1_CKEN;
}

/*!
    \brief      clock disable
    \param[in]  usart_periph: USARTx(x=0)
    \param[out] none
    \retval     none
*/
void usart_clock_disable(uint32_t usart_periph)
{  
    /* disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
    
    USART_CTL1(usart_periph) &=~(USART_CTL1_CKEN);
}

/*!
    \brief      configure usart synchronous mode parameters
    \param[in]  usart_periph: USARTx(x=0,1)
    \param[in]  clen: last bit clock pulse
      \arg        USART_CLEN_NONE: clock pulse of the last data bit (MSB) is not output to the CK pin 
      \arg        USART_CLEN_EN:   clock pulse of the last data bit (MSB) is output to the CK pin    
    \param[in]  cph: clock phase
      \arg        USART_CPH_1CK: first clock transition is the first data capture edge 
      \arg        USART_CPH_2CK: second clock transition is the first data capture edge
    \param[in]  cpl: clock polarity 
      \arg        USART_CPL_LOW:  steady low value on CK pin 
      \arg        USART_CPL_HIGH: steady high value on CK pin
    \param[out] none
    \retval     none
*/
void usart_synchronous_clock_config(uint32_t usart_periph,uint32_t clen,uint32_t cph,uint32_t cpl)
{
    /* disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);    
    /*  reset USART_CTL1 CLEN,CPH,CPL bits */
    USART_CTL1(usart_periph) &=~(USART_CTL1_CLEN|USART_CTL1_CPH|USART_CTL1_CPL);  
      

    USART_CTL1(usart_periph) |=(USART_CTL1_CLEN & clen) ;     

    USART_CTL1(usart_periph) |=(USART_CTL1_CPH & cph) ;

    USART_CTL1(usart_periph) |=(USART_CTL1_CPL & cpl) ;  
}

/*!
    \brief      smartcard mode enable
    \param[in]  usart_periph: USARTx(x=0)
    \param[out] none
    \retval     none
*/
void usart_smartcard_mode_enable(uint32_t usart_periph)
{   
    /* disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
    
    USART_CTL2(usart_periph) |=USART_CTL2_SCEN;
}

/*!
    \brief      smartcard mode disable
    \param[in]  usart_periph: USARTx(x=0)
    \param[out] none
    \retval     none
*/
void usart_smartcard_mode_disable(uint32_t usart_periph)
{  
    /* disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
    
    USART_CTL2(usart_periph) &=~(USART_CTL2_SCEN);
}

/*!
    \brief      NACK enable in smartcard mode
    \param[in]  usart_periph: USARTx(x=0)
    \param[out] none
    \retval     none
*/
void usart_smartcard_mode_nack_enable(uint32_t usart_periph)
{   
    /* disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
    
    USART_CTL2(usart_periph) |=USART_CTL2_NKEN;
}

/*!
    \brief      NACK disable in smartcard mode
    \param[in]  usart_periph: USARTx(x=0)
    \param[out] none
    \retval     none
*/
void usart_smartcard_mode_nack_disable(uint32_t usart_periph)
{  
    /* disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
    
    USART_CTL2(usart_periph) &=~(USART_CTL2_NKEN);
}

/*!
    \brief      guard time value configure in smartcard mode
    \param[in]  usart_periph: USARTx(x=0)
    \param[in]  guat: guard time value
    \param[out] none
    \retval     none
*/
void usart_guard_time_config(uint32_t usart_periph,uint32_t guat)
{
    /* disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
    
    USART_GP(usart_periph) &=~(USART_GP_GUAT);     
    USART_GP(usart_periph) |=(USART_GP_GUAT & ((guat)<<8));
}

/*!
    \brief      block length configure
    \param[in]  usart_periph: USARTx(x=0)
    \param[in]  bl: guard time value
    \param[out] none
    \retval     none
*/
void usart_block_length_config(uint32_t usart_periph,uint32_t bl)
{
    /* disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
    
    USART_RT(usart_periph) &=~(USART_RT_BL);        
    USART_RT(usart_periph) |=(USART_RT_BL & ((bl)<<24));
}
/*!
    \brief      smartcard auto-retry number configure
    \param[in]  usart_periph: USARTx(x=0)
    \param[in]  scrtnum: smartcard auto-retry number
    \param[out] none
    \retval     none
*/
void usart_smartcard_autoretry_config(uint32_t usart_periph,uint32_t scrtnum)
{   
    USART_CTL2(usart_periph) &=~(USART_CTL2_SCRTNUM);
    USART_CTL2(usart_periph) &=~(USART_CTL2_SCRTNUM);    
    USART_CTL2(usart_periph) |=(USART_CTL2_SCRTNUM & ((scrtnum)<<17));
}

/*!
    \brief      IrDA mode enable
    \param[in]  usart_periph: USARTx(x=0)
    \param[out] none
    \retval     none
*/
void usart_irda_mode_enable(uint32_t usart_periph)
{   
    /* disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
    
    USART_CTL2(usart_periph) |=USART_CTL2_IREN;
}

/*!
    \brief      IrDA mode disable
    \param[in]  usart_periph: USARTx(x=0)
    \param[out] none
    \retval     none
*/
void usart_irda_mode_disable(uint32_t usart_periph)
{  
    /* disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
    
    USART_CTL2(usart_periph) &=~(USART_CTL2_IREN);
}

/*!
    \brief      IrDA low-power configure
    \param[in]  usart_periph: USARTx(x=0)
    \param[in]  irlp: IrDA low-power or normal
      \arg        USART_IRLP_LOW:    low-power
      \arg        USART_IRLP_NORMAL: normal
    \param[out] none
    \retval     none
*/
void usart_irda_lowpower_config(uint32_t usart_periph,uint32_t irlp)
{   
    /* disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
    
    USART_CTL2(usart_periph) |=(USART_CTL2_IRLP & irlp);
}


/*!
    \brief      hardware flow control configure
    \param[in]  usart_periph: USARTx(x=0,1)
    \param[in]  hardwareflow: RTS or CTS
      \arg        USART_CTSRTS_NONE:   no hardware flow control
      \arg        USART_RTS_ENABLE:    RTS enable
      \arg        USART_CTS_ENABLE:    CTS enable
      \arg        USART_RTS_DISABLE:    RTS disable
      \arg        USART_CTS_DISABLE:    CTS disable
      \arg        USART_CTSRTS_ENABLE: RTS,CTS enable
    \param[out] none
    \retval     none
*/
void usart_hardware_flow_config(uint32_t usart_periph,usart_hardware_flow_enum hardwareflow)
{   
    /* disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
    switch(hardwareflow){
    case USART_CTSRTS_ENABLE:
        USART_CTL2(usart_periph) |=(USART_CTL2_RTSEN|USART_CTL2_CTSEN);
        break;
    case USART_RTS_ENABLE:
        USART_CTL2(usart_periph) |=USART_CTL2_RTSEN;
        break;
    case USART_CTS_ENABLE:
        USART_CTL2(usart_periph) |=USART_CTL2_CTSEN;
        break;
    case USART_CTS_DISABLE:
        USART_CTL2(usart_periph) &=(~USART_CTL2_CTSEN);
        break;
    case USART_RTS_DISABLE:
        USART_CTL2(usart_periph) &=(~USART_CTL2_RTSEN);
        break;
    case USART_CTSRTS_NONE:
        USART_CTL2(usart_periph) &=(~(USART_CTL2_CTSEN|USART_CTL2_RTSEN));
        break;
    default:
        break;
    }    
}

/*!
    \brief      RS485 driver enable
    \param[in]  usart_periph: USARTx(x=0,1)
    \param[out] none
    \retval     none
*/
void usart_rs485_driver_enable(uint32_t usart_periph)
{   
    /*  disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
    
    USART_CTL2(usart_periph) |=USART_CTL2_DEM;
}

/*!
    \brief      RS485 driver disable 
    \param[in]  usart_periph: USARTx(x=0,1)
    \param[out] none
    \retval     none
*/
void usart_rs485_driver_disable(uint32_t usart_periph)
{  
    /* disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
    
    USART_CTL2(usart_periph) &=~(USART_CTL2_DEM);
}

/*!
    \brief      driver enable assertion time configure
    \param[in]  usart_periph: USARTx(x=0)
    \param[in]  deatime: driver enable assertion time
    \param[out] none
    \retval     none
*/
void usart_driver_assertime_config(uint32_t usart_periph,uint32_t deatime)
{
    /* disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
    
    USART_CTL0(usart_periph) &=~(USART_CTL0_DEA);   
    USART_CTL0(usart_periph) |=(USART_CTL0_DEA & ((deatime)<<21));
}

/*!
    \brief      driver enable de-assertion time configure
    \param[in]  usart_periph: USARTx(x=0)
    \param[in]  dedtime:   driver enable de-assertion time
    \param[out] none
    \retval     none
*/
void usart_driver_deassertime_config(uint32_t usart_periph,uint32_t dedtime)
{
    /* disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);
    
    USART_CTL0(usart_periph) &=~(USART_CTL0_DED);       
    USART_CTL0(usart_periph) |=(USART_CTL0_DED & ((dedtime)<<16));
}

/*!
    \brief      configure driver enable polarity mode
    \param[in]  usart_periph: USARTx(x=0,1)
    \param[in]  cpl:          clock polarity 
      \arg        USART_DEP_HIGH: DE signal is active high 
      \arg        USART_DEP_LOW:  DE signal is active low
    \param[out] none
    \retval     none
*/
void usart_depolarity_config(uint32_t usart_periph,uint32_t dep)
{
    /* disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);    
    /*  reset USART_CTL2 DEP Bit */
    USART_CTL2(usart_periph) &=~(USART_CTL2_DEP);  
      
    USART_CTL2(usart_periph) |=(USART_CTL2_DEP & dep) ;     

}

/*!
    \brief      DMA transfer config
    \param[in]  usart_periph: USARTx(x=0,1)
    \param[in]  dmacmd: DENT or DENR
      \arg        USART_DMART_DISABLE: DMA disable
      \arg        USART_DENR_ENABLE:   DMA enable for reception
      \arg        USART_DENT_ENABLE:   DMA enable for transmission
      \arg        USART_DENR_DISABLE:   DMA disable for reception
      \arg        USART_DENT_DISABLE:   DMA disable for transmission
      \arg        USART_DMART_ENABLE:  DMA enable for transmission and reception 
    \param[out] none
    \retval     none
*/
void usart_dma_transfer_config(uint32_t usart_periph,usart_dma_transfer_enum dmacmd)
{
    switch(dmacmd){
    case USART_DMART_ENABLE:
        USART_CTL2(usart_periph) |=(USART_CTL2_DENR|USART_CTL2_DENT);
        break;
    case USART_DENR_ENABLE:
        USART_CTL2(usart_periph) |=USART_CTL2_DENR;
        break;
    case USART_DENT_ENABLE:
        USART_CTL2(usart_periph) |=USART_CTL2_DENT;
        break;
    case USART_DENR_DISABLE:
        USART_CTL2(usart_periph) &=(~USART_CTL2_DENR);
        break;
    case USART_DENT_DISABLE:
        USART_CTL2(usart_periph) &=(~USART_CTL2_DENT);
        break;
    case USART_DMART_DISABLE:
        USART_CTL2(usart_periph) &=(~(USART_CTL2_DENR|USART_CTL2_DENT));
        break;
    default:
        break;
    }
}

/*!
    \brief      disable DMA on reception error
    \param[in]  usart_periph: USARTx(x=0,1)
    \param[out] none
    \retval     none
*/
void usart_reception_error_dma_disable(uint32_t usart_periph)
{ 
    /* disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);      
    USART_CTL2(usart_periph) |=USART_CTL2_DDRE;
}

/*!
    \brief      not disable DMA on reception error 
    \param[in]  usart_periph: USARTx(x=0,1)
    \param[out] none
    \retval     none
*/
void usart_reception_error_dma_enable(uint32_t usart_periph)
{ 
    /* disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);      
    USART_CTL2(usart_periph) &=~(USART_CTL2_DDRE);
}

/*!
    \brief      USART able to wake up the MCU from deep-sleep mode
    \param[in]  usart_periph: USARTx(x=0)
    \param[out] none
    \retval     none
*/
void usart_wakeup_enable(uint32_t usart_periph)
{      
    USART_CTL0(usart_periph) |=USART_CTL0_UESM;
}

/*!
    \brief      USART not able to wake up the MCU from deep-sleep mode 
    \param[in]  usart_periph: USARTx(x=0)
    \param[out] none
    \retval     none
*/
void usart_wakeup_disable(uint32_t usart_periph)
{    
    USART_CTL0(usart_periph) &=~(USART_CTL0_UESM);
}

/*!
    \brief      wakeup mode from deep-sleep mode
    \param[in]  usart_periph: USARTx(x=0)
    \param[in]  wum: wakeup mode 
      \arg        USART_WUM_ADDR:   WUF active on address match 
      \arg        USART_WUM_STARTB: WUF active on start bit
      \arg        USART_WUM_RBNE:   WUF active on RBNE  
    \param[out] none
    \retval     none
*/
void usart_wakeup_mode_config(uint32_t usart_periph,uint32_t wum)
{
    /* disable USART */
    USART_CTL0(usart_periph) &=~(USART_CTL0_UEN);    
    /*  reset USART_CTL2 DEP bit */
    USART_CTL2(usart_periph) &=~(USART_CTL2_WUM);  
      
    USART_CTL2(usart_periph) |=(USART_CTL2_WUM & (wum)) ;     

}

/*!
    \brief      enable usart interrupt
    \param[in]  usart_periph: USARTx(x=0)
    \param[in]  inttype:      interrupt type 
      \arg        USART_INT_IDLEIE:   idle interrupt 
      \arg        USART_INT_RBNEIE: read data buffer not empty interrupt and 
                                   overrun error interrupt enable interrupt 
      \arg        USART_INT_TCIE:   transmission complete interrupt   
      \arg        USART_INT_TBEIE:  transmit data register empty interrupt  
      \arg        USART_INT_PERRIE:   parity error interrupt 
      \arg        USART_INT_AMIE:   address match interrupt    
      \arg        USART_INT_RTIE:   receiver timeout interrupt   
      \arg        USART_INT_EBIE:   end of block interrupt  
      \arg        USART_INT_LBDIE:  LIN break detection interrupt    
      \arg        USART_INT_ERRIE:   error interrupt enable in multibuffer communication  
      \arg        USART_INT_CTSIE:  CTS interrupt 
      \arg        USART_INT_WUIE:   wakeup from Deep-sleep mode interrupt    
    \param[out] none
    \retval     none
*/
void usart_interrupt_enable(uint32_t usart_periph,uint32_t inttype)
{
    uint32_t reg=0,intdex;
    /* get the interrupt register */
    reg=(BITS(28,29)&(inttype));
    reg=reg>>28;
    /* get the interrupt */
    intdex=((~BITS(28,29))&(inttype));
    switch(reg){
    /* CTL0 */
    case USART_CTL0_INDEX:
        USART_CTL0(usart_periph) |=(intdex);
        break;
    /* CTL1 */
    case USART_CTL1_INDEX:
        USART_CTL1(usart_periph) |=(intdex);
        break;
    /* CTL2 */
    case USART_CTL2_INDEX:
        USART_CTL2(usart_periph) |=(intdex);
    default:
        break;
    }     
}

/*!
    \brief      disable usart interrupt
    \param[in]  usart_periph: USARTx(x=0)
    \param[in]  inttype:      interrupt type 
      \arg        USART_INT_IDLEIE:   idle interrupt 
      \arg        USART_INT_RBNEIE: read data buffer not empty interrupt and 
                                   overrun error interrupt 
      \arg        USART_INT_TCIE:   transmission complete interrupt   
      \arg        USART_INT_TBEIE:  transmit data register empty interrupt  
      \arg        USART_INT_PERRIE:   parity error interrupt 
      \arg        USART_INT_AMIE:   address match interrupt    
      \arg        USART_INT_RTIE:   receiver timeout interrupt   
      \arg        USART_INT_EBIE:   end of block interrupt  
      \arg        USART_INT_LBDIE:  LIN break detection interrupt    
      \arg        USART_INT_ERRIE:   error interrupt enable in multibuffer communication  
      \arg        USART_INT_CTSIE:  CTS interrupt 
      \arg        USART_INT_WUIE:   wakeup from deep-sleep mode interrupt   
    \param[out] none
    \retval     none
*/
void usart_interrupt_disable(uint32_t usart_periph,uint32_t inttype)
{
    uint32_t reg=0,intdex;
    /* get the interrupt register */
    reg=(BITS(28,29)&(inttype));
    /* get the interrupt */
    intdex=((~BITS(28,19))&(inttype));
    switch(reg){
    /* CTL0 */
    case USART_CTL0_INDEX:
        USART_CTL0(usart_periph) &=~(intdex);
        break;
    /* CTL1 */
    case USART_CTL1_INDEX:
        USART_CTL1(usart_periph) &=~(intdex);
        break;
    /* CTL2 */
    case USART_CTL2_INDEX:
        USART_CTL2(usart_periph) &=~(intdex);
        break;
    default:
        break;
        
    }    
}

/*!
    \brief      enable usart command
    \param[in]  usart_periph: USARTx(x=0)
    \param[in]  cmdtype:      command type 
      \arg        USART_CMD_ABDCMD: transmit data flush request 
      \arg        USART_CMD_SBKCMD: receive data flush command
      \arg        USART_CMD_MMCMD:  mute mode command  
      \arg        USART_CMD_RXFCMD: receive data flush command 
      \arg        USART_CMD_TXFCMD: transmit data flush request
    \param[out] none
    \retval     none
*/
void usart_command_enable(uint32_t usart_periph,uint32_t cmdtype)
{
    USART_CMD(usart_periph) |=(cmdtype);   
}

/*!
    \brief      check usart status
    \param[in]  usart_periph: USARTx(x=0)
    \param[in]  status: status type 
      \arg        USART_STAT_PERR:    PERR state 
      \arg        USART_STAT_FERR:    FERR state 
      \arg        USART_STAT_NERR:    NERR state  
      \arg        USART_STAT_ORERR:   ORERR state 
      \arg        USART_STAT_IDLEF: idle state 
      \arg        USART_STAT_RBNE:  RBNE state  
      \arg        USART_STAT_TC:    TC state  
      \arg        USART_STAT_TBE:   TBE state 
      \arg        USART_STAT_LBDF:  LBDF state  
      \arg        USART_STAT_CTSF:  CTSF state  
      \arg        USART_STAT_CTS:   CTS state 
      \arg        USART_STAT_RTF:   receive timeout state  
      \arg        USART_STAT_EBF:   end of block state 
      \arg        USART_STAT_ABDE:  auto baudrate detection error
      \arg        USART_STAT_ABDF:  auto baudrate detection flag    
      \arg        USART_STAT_BSY:   bsy state  
      \arg        USART_STAT_AMF:   address match flag state 
      \arg        USART_STAT_SBF:   send break flag   
      \arg        USART_STAT_RWU:   receiver wakeup from mute mode.
      \arg        USART_STAT_WUF:   wakeup from deep-sleep mode flag 
      \arg        USART_STAT_TEA:   transmit enable acknowledge flag   
      \arg        USART_STAT_REA:   receive enable acknowledge flag 
    \param[out] none      
    \retval     state of stat bit
*/
FlagStatus usart_flag_get(uint32_t usart_periph,uint32_t status)
{
    if(USART_STAT(usart_periph)&(status))
        return SET;

    return RESET; 
}

/*!
    \brief      clear usart status
    \param[in]  usart_periph: USARTx(x=0)
    \param[in]  status:       status type 
      \arg        USART_INC_PEC:   PE state clear
      \arg        USART_INC_FEC:   FE state clear
      \arg        USART_INC_NEC:   NE state clear 
      \arg        USART_INC_OREC:  ORE state clear
      \arg        USART_INC_IDLEC: idle state clear
      \arg        USART_INC_TCC:   idle state clear 
      \arg        USART_INC_LBDC:  LIN break detected clear 
      \arg        USART_INC_CTSC:  CTS change clear
      \arg        USART_INC_RTC:   receiver timeout clear 
      \arg        USART_INC_EBC:   end of timeout clear  
      \arg        USART_INC_AMC:   address match clear
      \arg        USART_INC_WUC:   wakeup from deep-sleep mode clear  
    \param[out] none 
    \retval     none
*/
void usart_flag_clear(uint32_t usart_periph,uint32_t status)
{
    USART_INTC(usart_periph) |=(status);
}

/*!
    \brief      get usart interrupt status
    \param[in]  usart_periph: USARTx(x=0)
    \param[in]  state: status type 
      \arg        USART_STAT_PERR:    PE state 
      \arg        USART_STAT_FERR:    FE state 
      \arg        USART_STAT_NERR:    NE state  
      \arg        USART_STAT_ORERR:   ORE state 
      \arg        USART_STAT_IDLEF: idle state 
      \arg        USART_STAT_RBNE:  RBNE state  
      \arg        USART_STAT_TC:    TC state  
      \arg        USART_STAT_TBE:   TBE state 
      \arg        USART_STAT_LBDF:  LBDF state  
      \arg        USART_STAT_CTSF:  CTSF state  
      \arg        USART_STAT_RTF:   receive timeout state  
      \arg        USART_STAT_EBF:   end of block state  
      \arg        USART_STAT_AMF:   address match flag state 
      \arg        USART_STAT_WUF:   wakeup from deep-sleep mode flag 
    \param[in]  inttype:      interrupt type 
      \arg        USART_INT_IDLEIE:   idle interrupt 
      \arg        USART_INT_RBNEIE: read data buffer not empty interrupt and 
                                   overrun error interrupt  
      \arg        USART_INT_TCIE:   transmission complete interrupt   
      \arg        USART_INT_TBEIE:  transmit data register empty interrupt  
      \arg        USART_INT_PERRIE:   parity error interrupt 
      \arg        USART_INT_AMIE:   address match interrupt    
      \arg        USART_INT_RTIE:   receiver timeout interrupt   
      \arg        USART_INT_EBIE:   end of block interrupt  
      \arg        USART_INT_LBDIE:  LIN break detection interrupt    
      \arg        USART_INT_ERRIE:   error interrupt in multibuffer communication  
      \arg        USART_INT_CTSIE:  CTS interrupt 
      \arg        USART_INT_WUIE:   wakeup from deep-sleep mode interrupt  
    \param[out] none      
    \retval     state of stat bit
*/
FlagStatus usart_interrupt_flag_get(uint32_t usart_periph,uint32_t state,uint32_t inttype)
{
    uint32_t reg=0,intdex,intenable=0,intstate=0;
    /* get the interrupt register */
    reg=(BITS(28,29)&(inttype));
    reg=reg>>28;
    /* get the interrupt */
    intdex=((~BITS(28,29))&(inttype));
    switch(reg){
    /* CTL0 */
    case USART_CTL0_INDEX:
        intenable=USART_CTL0(usart_periph) & (intdex);
        break;
    /* CTL1 */
    case USART_CTL1_INDEX:
        intenable=USART_CTL1(usart_periph) & (intdex);
        break;
    /* CTL2 */
    case USART_CTL2_INDEX:
        intenable=USART_CTL2(usart_periph) & (intdex);
        break;
    default:
        break;
    }
    intstate=USART_STAT(usart_periph)&(state);
    if(intstate && intenable)
        return SET;

    return RESET; 
}

