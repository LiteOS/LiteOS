/*!
    \file  gd32f1x0_i2c.c
    \brief i2c driver

*/

/*
    Copyright (C) 2016 GigaDevice

    2014-12-26  V1.0.0, platform GD32F1x0(x=3,5)
    2016-12-23  V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#include "gd32f1x0_i2c.h"
#include "gd32f1x0_rcu.h"

#define I2CCLK_MAX                0x3f        /*!< i2cclk max value */

/*!
    \brief      reset I2C
    \param[in]  i2c_periph: I2Cx(x=0,1,2)
    \param[out] none
    \retval     none
*/
void i2c_deinit(uint32_t i2c_periph)
{
    switch(i2c_periph){
    case I2C0:
        rcu_periph_reset_enable(RCU_I2C0RST);
        rcu_periph_reset_disable(RCU_I2C0RST);
        break;
    case I2C1:
        rcu_periph_reset_enable(RCU_I2C1RST);
        rcu_periph_reset_disable(RCU_I2C1RST);
        break;
#ifdef GD32F170_190
    case I2C2:
        rcu_periph_reset_enable(RCU_I2C2RST);
        rcu_periph_reset_disable(RCU_I2C2RST);
        break;
#endif /* GD32F170_190 */
    default:
        break;
    
    }
}

/*!
    \brief      I2C clock configure
    \param[in]  i2c_periph: I2Cx(x=0,1,2)
    \param[in]  clkspeed: i2c clock speed   
    \param[in]  dutycyc: duty cycle in fast mode
      \arg        I2C_DTCY_2:    T_low/T_high=2 
      \arg        I2C_DTCY_16_9: T_low/T_high=16/9
    \param[out] none
    \retval     none
*/
void i2c_clock_config(uint32_t i2c_periph,uint32_t clkspeed,uint32_t dutycyc)
{
    uint32_t pclk1=0,clkc=0,i2cclk=0,risetime=0;
    pclk1=rcu_clock_freq_get(CK_APB1);
    /* I2C Peripheral clock frequency */
    i2cclk=(pclk1/1000000);
    if(i2cclk>=I2CCLK_MAX){
        i2cclk=I2CCLK_MAX;
    }
        
    I2C_CTL1(i2c_periph) |=(I2C_CTL1_I2CCLK & i2cclk) ;
    
    if(100000>=clkspeed){
         /* standard mode the maximum SCL rise time in standard mode is 1000ns  */
        risetime=(uint16_t)((pclk1/1000000)+1);
        if(risetime>=I2CCLK_MAX){
            I2C_RT(i2c_periph) |= I2CCLK_MAX;
        }else{
            I2C_RT(i2c_periph) |= (uint16_t)((pclk1/1000000)+1);
        }
        clkc = (uint16_t)(pclk1 / (clkspeed*2));
        if(clkc < 0x04){
            /* The CLKC in standard mode minmum value is 4*/
            clkc = 0x04;
        }
        I2C_CKCFG(i2c_periph) |= (I2C_CKCFG_CLKC &clkc);        

    }else{
        /* fast mode the maximum SCL rise time in standard mode is 300ns  */
        I2C_RT(i2c_periph) |= (uint16_t)(((i2cclk * (uint16_t)300) / (uint16_t)1000) + (uint16_t)1);
        if(dutycyc==I2C_DTCY_2){
            /* I2C_DutyCycle == 2 */
            clkc = (uint16_t)(pclk1 / (clkspeed * 3));
        } else{
            /* I2C_DutyCycle == 16/9 */
            clkc = (uint16_t)(pclk1 / (clkspeed * 25));
            I2C_CKCFG(i2c_periph) |= I2C_CKCFG_DTCY;
        }
        if((clkc & I2C_CKCFG_CLKC) == 0){
            /* The CLKC in standard mode minmum value is 1*/
            clkc |= (uint16_t)0x0001;  
        }
        I2C_CKCFG(i2c_periph) |= clkc;
    }
}

/*!
    \brief      I2C address configure
    \param[in]  i2c_periph: I2Cx(x=0,1,2)
    \param[in]  I2C_MODE: refer to enum I2C_MODE   
    \param[in]  addformat: 7bits or 10bits
      \arg        I2C_ADDFORMAT_7BITS:  7bits
      \arg        I2C_ADDFORMAT_10BITS: 10bitsy
    \param[in]  addr: I2C address
    \param[out] none
    \retval     none
*/
void i2c_addr_config(uint32_t i2c_periph,i2c_mode_enum i2cmod,uint32_t addformat,uint32_t addr)
{
    I2C_CTL0(i2c_periph) &=~(I2C_CTL0_SMBEN|I2C_CTL0_SMBSEL);
    I2C_CTL0(i2c_periph) |= i2cmod;
    I2C_SADDR0(i2c_periph) |= (addformat|addr);
    
}
/*!
    \brief      whether or not to send an ACK
    \param[in]  i2c_periph: I2Cx(x=0,1,2)
    \param[in]  ack: world length
      \arg        I2C_ACK_ENABLE: ACK will be sent  
      \arg        I2C_ACK_DISABLE: ACK will not be sent    
    \param[out] none
    \retval     none
*/
void i2c_ack_config(uint32_t i2c_periph,i2c_ack_enum ack)
{
    switch(ack){
    case I2C_ACK_ENABLE:
        I2C_CTL0(i2c_periph) |= I2C_CTL0_ACKEN ;
        break;
    case I2C_ACK_DISABLE:
        I2C_CTL0(i2c_periph) &= (~I2C_CTL0_ACKEN);
        break;
    default:
        break;
    
    }    
}

/*!
    \brief      I2C POAP position configure
    \param[in]  i2c_periph: I2Cx(x=0,1,2)
    \param[in]  dmastste: refer to enum i2c_ackpos_enum
    \param[out] none
    \retval     none
*/
void i2c_ackpos_config(uint32_t i2c_periph,i2c_ackpos_enum pos)
{
    /* check the interrupt bits */
    switch(pos){
    case I2C_ACKPOS_NEXT:
        I2C_CTL0(i2c_periph) |= I2C_CTL0_POAP;
        break;
    case I2C_ACKPOS_CURRENT:
        I2C_CTL0(i2c_periph) &= ~(I2C_CTL0_POAP);
        break;
    default:
        break;
    }

}
/*!
    \brief      master send slave address
    \param[in]  i2c_periph: I2Cx(x=0,1,2)
    \param[in]  addr: slave address  
    \param[in]  Trandirection: transmitter or receiver
      \arg        I2C_TRANSMITTER: transmitter  
      \arg        I2C_RECEIVER:    receiver  
    \param[out] none
    \retval     none
*/
void i2c_master_addressing(uint32_t i2c_periph,uint8_t addr,uint32_t trandirection)
{
    if(I2C_TRANSMITTER==trandirection){
        addr=(addr&I2C_TRANSMITTER);
    }else{
        addr=(addr|I2C_RECEIVER);
    }
    I2C_DATA(i2c_periph) = addr;
}

/*!
    \brief      dual-address mode switch
    \param[in]  i2c_periph: I2Cx(x=0,1,2)
    \param[in]  dualaddr: enum i2c_dualaddr_enum  
    \param[out] none
    \retval     none
*/
void i2c_dualaddr_enable(uint32_t i2c_periph,i2c_dualaddr_enum dualaddr)
{
    if(dualaddr){
       I2C_SADDR0(i2c_periph) |= I2C_SADDR0_DUADEN;  
    }else{
       I2C_SADDR0(i2c_periph) &= ~(I2C_SADDR0_DUADEN);  
    }        
}
/*!
    \brief      enable i2c
    \param[in]  i2c_periph: I2Cx(x=0,1,2) 
    \param[out] none
    \retval     none
*/
void i2c_enable(uint32_t i2c_periph)
{
    I2C_CTL0(i2c_periph) |= I2C_CTL0_I2CEN;
    
}

/*!
    \brief      disable i2c
    \param[in]  i2c_periph: I2Cx(x=0,1,2) 
    \param[out] none
    \retval     none
*/
void i2c_disable(uint32_t i2c_periph)
{
    I2C_CTL0(i2c_periph) &= ~(I2C_CTL0_I2CEN);
    
}

/*!
    \brief      generate a START condition on I2C bus
    \param[in]  i2c_periph: I2Cx(x=0,1,2)
    \param[out] none
    \retval     state of bus busy
*/
void i2c_start_on_bus(uint32_t i2c_periph)
{
    I2C_CTL0(i2c_periph) |= I2C_CTL0_START;
}

/*!
    \brief      generate a STOP condition on I2C bus
    \param[in]  i2c_periph: I2Cx(x=0,1,2)
    \param[out] none
    \retval     state of bus busy
*/
void i2c_stop_on_bus(uint32_t i2c_periph)
{
    I2C_CTL0(i2c_periph) |= I2C_CTL0_STOP;
}

/*!
    \brief      i2c transmit data function
    \param[in]  i2c_periph: I2Cx(x=0,1,2)
    \param[in]  data:       data of transmission 
    \param[out] none
    \retval     none
*/
void i2c_transmit_data(uint32_t i2c_periph,uint8_t data)
{
    I2C_DATA(i2c_periph) = data;
}

/*!
    \brief      i2c receive data function
    \param[in]  i2c_periph: I2Cx(x=0,1,2)
    \param[out] none
    \retval     data of received
*/
uint8_t i2c_receive_data(uint32_t i2c_periph)
{
    return (uint8_t)I2C_DATA(i2c_periph); 
}

/*!
    \brief      I2C DMA function configure
    \param[in]  i2c_periph: I2Cx(x=0,1,2)
    \param[in]  dmastste:   refer to enum i2c_dmaconfig_enum
    \param[out] none
    \retval     none
*/
void i2c_dma_config(uint32_t i2c_periph,i2c_dmaconfig_enum dmastste)
{
    /* check the interrupt bits */
    switch(dmastste){
    case I2C_DMA_ON:
        I2C_CTL1(i2c_periph) |= I2C_CTL1_DMAON;
        break;
    case I2C_DMA_OFF:
        I2C_CTL1(i2c_periph) &= ~(I2C_CTL1_DMAON);
        break;
    case I2C_DMALST_ON:
        I2C_CTL1(i2c_periph) |= I2C_CTL1_DMALST;
        break;
    case I2C_DMALST_OFF:
        I2C_CTL1(i2c_periph) &= ~(I2C_CTL1_DMALST);
        break;    
    default:
        break;
    }    
}

/*!
    \brief      whether to stretch SCL low when data is not ready in slave mode 
    \param[in]  i2c_periph: I2Cx(x=0,1,2)
    \param[in]  stretchpara:   refer to enum i2c_stretchscl_enum
    \param[out] none
    \retval     none
*/
void i2c_stretch_scl_low_config(uint32_t i2c_periph,i2c_stretchscl_enum stretchpara )
{
    I2C_CTL0(i2c_periph) &= ~(I2C_CTL0_DISSTRC);
    I2C_CTL0(i2c_periph) |= ((I2C_CTL0_DISSTRC)&(stretchpara));
}

/*!
    \brief      whether or not to response to a general Cal 
    \param[in]  i2c_periph: I2Cx(x=0,1,2)
    \param[in]  gcallpara:   refer to enum i2c_gcall_config_enum
    \param[out] none
    \retval     none
*/
void i2c_slave_response_to_gcall_config(uint32_t i2c_periph, i2c_gcall_config_enum gcallpara )
{
    I2C_CTL0(i2c_periph) &= ~(I2C_CTL0_GCEN);
    I2C_CTL0(i2c_periph) |= ((I2C_CTL0_GCEN)&(gcallpara));
}

/*!
    \brief      software reset I2C 
    \param[in]  i2c_periph: I2Cx(x=0,1,2)
    \param[in]  sreset:   refer to enum i2c_software_reset_enum
    \param[out] none
    \retval     none
*/
void i2c_software_reset_config(uint32_t i2c_periph, i2c_software_reset_enum sreset )
{
    I2C_CTL0(i2c_periph) &= ~(I2C_CTL0_SRESET);
    I2C_CTL0(i2c_periph) |= ((I2C_CTL0_SRESET)&(sreset));
    
}

/*!
    \brief      check i2c state
    \param[in]  i2c_periph: I2Cx(x=0,1,2)
    \param[in]  state: state type, refer to enum I2C_state
    \param[out] none
    \retval     state of i2c
*/
FlagStatus i2c_flag_get(uint32_t i2c_periph,i2c_state_enum state )
{
    uint32_t stat0=0,stat1=0,stat=0;
    stat0=I2C_STAT0(i2c_periph);
    stat1=I2C_STAT1(i2c_periph);
    stat=stat0|(stat1<<16);
    if((stat)&(state))
        return SET;

    return RESET;
}

/*!
    \brief      clear i2c state
    \param[in]  i2c_periph: I2Cx(x=0,1,2)
    \param[in]  state: state type 
      \@arg       I2C_STAT0_SMBALTS: level of txframe signal
      \@arg       I2C_STAT0_SMBTO: timeout signal in SMBus mode
      \@arg       I2C_STAT0_PECERR: PEC error when receiving data
      \@arg       I2C_STAT0_OUERR: over-run or under-run situation occurs in slave mode    
      \@arg       I2C_STAT0_AERR: acknowledge error
      \@arg       I2C_STAT0_LOSTARB: arbitration lost in master mode   
      \@arg       I2C_STAT0_BE: a bus error   
    \param[out] none
    \retval     none
*/
void i2c_flag_clear(uint32_t i2c_periph,uint32_t state )
{
    I2C_STAT0(i2c_periph) &= ~(state);
}

/*!
    \brief      enable i2c interrupt
    \param[in]  i2c_periph: I2Cx(x=0,1,2)
    \param[in]  inttype:      interrupt type 
      \arg        I2C_CTL1_ERRIE: error interrupt enable 
      \arg        I2C_CTL1_EVIE:  event interrupt enable 
      \arg        I2C_CTL1_BUFIE: buffer interrupt enable   
    \param[out] none
    \retval     none
*/
void i2c_interrupt_enable(uint32_t i2c_periph,uint32_t inttype)
{
    I2C_CTL1(i2c_periph) |= (inttype);   
}
/*!
    \brief      disable i2c interrupt
    \param[in]  i2c_periph: I2Cx(x=0,1,2)
    \param[in]  inttype: interrupt type 
      \arg        I2C_CTL1_ERRIE: error interrupt enable 
      \arg        I2C_CTL1_EVIE:  event interrupt enable 
      \arg        I2C_CTL1_BUFIE: buffer interrupt enable   
    \param[out] none
    \retval     none
*/
void i2c_interrupt_disable(uint32_t i2c_periph,uint32_t inttype)
{
    I2C_CTL1(i2c_periph) &= ~(inttype);   
}

/*!
    \brief      I2C PEC function configure
    \param[in]  i2c_periph: I2Cx(x=0,1,2)
    \param[in]  pecpara: refer to enum i2c_pecconfig_enum
    \param[out] none
    \retval     none
*/
void i2c_pec_config(uint32_t i2c_periph,i2c_pecconfig_enum pecpara)
{
    /* check the interrupt bits */
    switch(pecpara){
    case I2C_PEC_ENABLE:
        I2C_CTL0(i2c_periph) |= I2C_CTL0_PECEN;
        break;
    case I2C_PEC_DISABLE:
        I2C_CTL0(i2c_periph) &= ~(I2C_CTL0_PECEN);
        break;
    case I2C_POAP_NEXT:
        I2C_CTL0(i2c_periph) |= I2C_CTL0_POAP;
        break;
    case I2C_POAP_CURRENT:
        I2C_CTL0(i2c_periph) &= ~(I2C_CTL0_POAP);
        break;
    case I2C_PECTRANS_ENABLE:
        I2C_CTL0(i2c_periph) |= I2C_CTL0_PECTRANS;
        break;
    case I2C_PECTRANS_DISABLE:
        I2C_CTL0(i2c_periph) &= ~(I2C_CTL0_PECTRANS);
    default:
        break;
    }     
}

/*!
    \brief      packet error checking value 
    \param[in]  i2c_periph: I2Cx(x=0,1,2)
    \param[out] none
    \retval     PEC value
*/
uint8_t i2c_pec_value(uint32_t i2c_periph)
{
    return  ((I2C_STAT1(i2c_periph) &I2C_STAT1_ECV)>>8);
}

/*!
    \brief      I2C SMBus configure 
    \param[in]  i2c_periph: I2Cx(x=0,1,2)
    \param[in]  smbuspara:   refer to enum I2C_SMBus_config
    \param[out] none
    \retval     none
*/
void i2c_smbus_config(uint32_t i2c_periph,i2c_smbusconfig_enum smbuspara )
{
    /* check the interrupt bits */
    switch(smbuspara){
    case I2C_SALTSEND_ENABLE:
        I2C_CTL0(i2c_periph) |= I2C_CTL0_SALT;
        break;
    case I2C_SALTSEND_DISABLE:
        I2C_CTL0(i2c_periph) &= ~(I2C_CTL0_SALT);
        break;
    case I2C_ARP_ENABLE:
        I2C_CTL0(i2c_periph) |= I2C_CTL0_ARPEN;
        break;
    case I2C_ARP_DISABLE:
        I2C_CTL0(i2c_periph) &= ~(I2C_CTL0_ARPEN);
        break;
    default:
        break;
    } 
}
#ifdef GD32F170_190

/*!
    \brief      enable SAM_V interface
    \param[in]  i2c_periph: I2Cx(x=0,1,2)
    \param[out] none
    \retval     none
*/
void i2c_sam_enable(uint32_t i2c_periph)
{
    I2C_SAMCS(i2c_periph) |= I2C_SAMCS_SAMEN;  
}

/*!
    \brief      disable SAM_V interface
    \param[in]  i2c_periph: I2Cx(x=0,1,2)
    \param[out] none
    \retval     none
*/
void i2c_sam_disable(uint32_t i2c_periph)
{
    I2C_SAMCS(i2c_periph) &= ~(I2C_SAMCS_SAMEN);  
}

/*!
    \brief      enable SAM_V interface timeout detect
    \param[in]  i2c_periph: I2Cx(x=0,1,2)
    \param[out] none
    \retval     none
*/
void i2c_sam_timeout_enable(uint32_t i2c_periph)
{
    I2C_SAMCS(i2c_periph) |= I2C_SAMCS_STOEN;  
}

/*!
    \brief      disable SAM_V interface timeout detect
    \param[in]  i2c_periph: I2Cx(x=0,1,2)
    \param[out] none
    \retval     none
*/
void i2c_sam_timeout_disable(uint32_t i2c_periph)
{
    I2C_SAMCS(i2c_periph) &= ~(I2C_SAMCS_STOEN);  
}

/*!
    \brief      enable the specified I2C SAM interrupt
    \param[in]  i2c_periph: I2Cx(x=0,1,2)
    \param[in]  inttype: interrupt type 
      \@arg       I2C_SAMCS_TFFIE: txframe fall interrupt
      \@arg       I2C_SAMCS_TFRIE: txframe rise interrupt
      \@arg       I2C_SAMCS_RFFIE: rxframe fall interrupt
      \@arg       I2C_SAMCS_RFRIE: rxframe rise interrupt   
    \param[out]  none
    \retval      none
*/
void i2c_sam_interrupt_enable(uint32_t i2c_periph,uint32_t inttype)
{
    I2C_SAMCS(i2c_periph) |= (inttype);   
}

/*!
    \brief      disable i2c interrupt
    \param[in]  i2c_periph: I2Cx(x=0,1,2)
    \param[in]  inttype:      interrupt type 
      \@arg       I2C_SAMCS_TFFIE: txframe fall interrupt
      \@arg       I2C_SAMCS_TFRIE: txframe rise interrupt
      \@arg       I2C_SAMCS_RFFIE: rxframe fall interrupt
      \@arg       I2C_SAMCS_RFRIE: rxframe rise interrupt    
    \param[out] none
    \retval     none
*/
void i2c_sam_interrupt_disable(uint32_t i2c_periph,uint32_t inttype)
{
    I2C_SAMCS(i2c_periph) &= ~(inttype);   
}

/*!
    \brief      check i2c SAM state
    \param[in]  i2c_periph: I2Cx(x=0,1,2)
    \param[in]  samstate: state type 
      \@arg       I2C_SAMCS_TXF: level of txframe signal
      \@arg       I2C_SAMCS_RXF: level of rxframe signal
      \@arg       I2C_SAMCS_TFF: txframe fall flag
      \@arg       I2C_SAMCS_TFR: txframe rise flag    
      \@arg       I2C_SAMCS_RFF: rxframe fall flag
      \@arg       I2C_SAMCS_RFR: rxframe rise flag   
    \param[out] none
    \retval     state of i2c SAM
*/
FlagStatus i2c_sam_flag_get(uint32_t i2c_periph,uint32_t samstate)
{
    if(I2C_SAMCS(i2c_periph)&samstate)
        return SET;

    return RESET;   
}
/*!
    \brief      clear i2c SAM state
    \param[in]  i2c_periph: I2Cx(x=0,1,2)
    \param[in]  samstate: state type 
      \@arg       I2C_SAMCS_TFF: txframe fall flag
      \@arg       I2C_SAMCS_TFR: txframe rise flag    
      \@arg       I2C_SAMCS_RFF: rxframe fall flag
      \@arg       I2C_SAMCS_RFR: rxframe rise flag   
    \param[out] none
    \retval     none
*/
void i2c_sam_flag_clear(uint32_t i2c_periph,uint32_t samstate)
{
    I2C_SAMCS(i2c_periph)&=~(samstate);
  
}

#endif /* GD32F170_190 */
