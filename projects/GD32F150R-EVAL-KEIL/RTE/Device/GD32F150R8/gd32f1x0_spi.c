/*!
    \file  gd32f1x0_spi.c
    \brief SPI driver
*/

/*
    Copyright (C) 2016 GigaDevice

    2014-12-26, V1.0.0, firmware for GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, firmware for GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#include "gd32f1x0_rcu.h"
#include "gd32f1x0_spi.h"

/*!
    \brief      SPI and I2S reset
    \param[in]  spi_periph: SPIx(x=0,1,2)
    \param[out] none
    \retval     none
*/
void spi_i2s_deinit(uint32_t spi_periph)
{
    switch(spi_periph){
    case SPI0:
        /* reset SPI0 and I2S0 */
        rcu_periph_reset_enable(RCU_SPI0RST);
        rcu_periph_reset_disable(RCU_SPI0RST);
        break;
    case SPI1:
        /* reset SPI1 */
        rcu_periph_reset_enable(RCU_SPI1RST);
        rcu_periph_reset_disable(RCU_SPI1RST);
        break;
    case SPI2:
        /* reset SPI2 and I2S2 */
        rcu_periph_reset_enable(RCU_SPI2RST);
        rcu_periph_reset_disable(RCU_SPI2RST);
        break;
    default :
        break;
    }
}

/*!
    \brief      SPI parameter initialization
    \param[in]  spi_periph: SPIx(x=0,1,2)
    \param[in]  spi_struct: SPI parameter initialization stuct
                members of the structure and the member values are shown as below:
                  spi_devicemode: SPI_MASTER, SPI_SLAVE.
                  spi_transmode : SPI_TRANSMODE_FULLDUPLEX, SPI_TRANSMODE_RECEIVEONLY,
                                  SPI_TRANSMODE_BDRECEIVE, SPI_TRANSMODE_BDTRANSMIT
                  spi_framesize : SPI_FRAMESIZE_16BIT, SPI_FRAMESIZE_8BIT
                  spi_nss:      : SPI_NSS_SOFT, SPI_NSS_HARD
                  spi_endian    : SPI_ENDIAN_MSB, SPI_ENDIAN_LSB
                  spi_ck_pl_ph  : SPI_CK_PL_LOW_PH_1EDGE, SPI_CK_PL_HIGH_PH_1EDGE
                                  SPI_CK_PL_LOW_PH_2EDGE, SPI_CK_PL_HIGH_PH_2EDGE
                  spi_psc       : SPI_PSC_n (n=2,4,8,16,32,64,128,256)
    \param[out] none
    \retval     none
*/
void spi_init(uint32_t spi_periph,spi_parameter_struct* spi_struct)
{   
    uint32_t reg =0;
    reg = SPI_CTL0(spi_periph);
    /* (1) select SPI as master or slave */
    reg |=spi_struct->spi_devicemode;
    /* (2) select SPI transfer mode */
    reg |=spi_struct->spi_transmode;
    /* (3) select SPI frame size */
    reg |=spi_struct->spi_framesize;
    /* (4) select SPI nss use hardware or software */
    reg |=spi_struct->spi_nss;
    /* (5) select SPI LSB or MSB */
    reg |=spi_struct->spi_endian;
    /* (6) select SPI polarity and phase */
    reg |=spi_struct->spi_ck_pl_ph;
    /* (7) select SPI prescale to adjust transmit speed */
    reg |=spi_struct->spi_psc;
    SPI_CTL0(spi_periph) = reg;

}

/*!
    \brief      SPI enable
    \param[in]  spi_periph: SPIx(x=0,1,2)
    \param[out] none
    \retval     none
*/
void spi_enable(uint32_t spi_periph)
{
    SPI_CTL0(spi_periph) |= SPI_CTL0_SPIEN;
}

/*!
    \brief      SPI disable
    \param[in]  spi_periph: SPIx(x=0,1,2)
    \param[out] none
    \retval     none
*/
void spi_disable(uint32_t spi_periph)
{
    SPI_CTL0(spi_periph) &= ~SPI_CTL0_SPIEN;
}

/*!
    \brief      I2S prescale config
    \param[in]  spi_periph: SPIx(only x=0,2)
    \param[in]  audiosample:
      \arg        I2S_AUDIOSAMPLE_8K: audio sample rate is 8khz
      \arg        I2S_AUDIOSAMPLE_11K: audio sample rate is 11khz
      \arg        I2S_AUDIOSAMPLE_16K: audio sample rate is 16khz
      \arg        I2S_AUDIOSAMPLE_22K: audio sample rate is 22khz
      \arg        I2S_AUDIOSAMPLE_32K: audio sample rate is 32khz
      \arg        I2S_AUDIOSAMPLE_44K: audio sample rate is 44khz
      \arg        I2S_AUDIOSAMPLE_48K: audio sample rate is 48khz
      \arg        I2S_AUDIOSAMPLE_96K: audio sample rate is 96khz
      \arg        I2S_AUDIOSAMPLE_192K: audio sample rate is 192khz
    \param[in]  frameformat:
      \arg        I2S_FRAMEFORMAT_DT16B_CH16B: I2S data length is 16 bit and channel length is 16 bit
      \arg        I2S_FRAMEFORMAT_DT16B_CH32B: I2S data length is 16 bit and channel length is 32 bit
      \arg        I2S_FRAMEFORMAT_DT24B_CH32B: I2S data length is 24 bit and channel length is 32 bit
      \arg        I2S_FRAMEFORMAT_DT32B_CH32B: I2S data length is 32 bit and channel length is 32 bit
    \param[in]  mckout:
      \arg        I2S_MCKOUT_ENABLE: I2S master clock output enable
      \arg        I2S_MCKOUT_DISABLE: I2S master clock output disable
    \param[out] none
    \retval     none
*/
void i2s_psc_config(uint32_t spi_periph,uint32_t audiosample,uint32_t frameformat,uint32_t mckout)
{
    uint32_t div = 2, of = 0;
    uint32_t temp = 0;
    uint32_t i2sclock = 0;

    /* deinit SPI_I2SPSC register */
    SPI_I2SPSC(spi_periph) = 0x0002;
    /* get system clock */
    i2sclock =rcu_clock_freq_get(CK_SYS);    
    /* config the prescaler depending on the mclk output state, the frame format and audio sample rate */
    if(I2S_MCKOUT_ENABLE == mckout){
        temp = (((i2sclock / 256) * 10) / audiosample);
    }else{
        if(I2S_FRAMEFORMAT_DT16B_CH16B == frameformat){
            temp = (((i2sclock / 32) *10 ) / audiosample);
        }else{
            temp = (((i2sclock / 64) *10 ) / audiosample);
        }
    }
    /* remove the flaoting point */
    temp = (temp+5) / 10;
    of = (temp & 0x00000001);
    div = ((temp - of) / 2);
    of = (of << 8);
     
    /* set the default values */
    if((div < 2) || (div > 255)){
        div = 2;
        of = 0;
    }
    /* configure SPI_I2SPSC */
    SPI_I2SPSC(spi_periph) = (div | of | mckout);
    SPI_I2SCTL(spi_periph) |= frameformat;
}

/*!
    \brief      I2S parameter configuration
    \param[in]  spi_periph: SPIx(only x=0,2)
    \param[in]  i2s_mode: 
      \arg        I2S_MODE_SLAVETX : I2S slave transmit mode
      \arg        I2S_MODE_SLAVERX : I2S slave receive mode
      \arg        I2S_MODE_MASTERTX : I2S master transmit mode
      \arg        I2S_MODE_MASTERRX : I2S master receive mode
    \param[in]  i2s_std: 
      \arg        I2S_STD_PHILLIPS : I2S phillips standard  
      \arg        I2S_STD_MSB : I2S MSB standard
      \arg        I2S_STD_LSB : I2S LSB standard
      \arg        I2S_STD_PCMSHORT : I2S PCM short standard
      \arg        I2S_STD_PCMLONG : I2S PCM long standard
    \param[in]  i2s_ckpl: 
      \arg        I2S_CKPL_LOW : I2S clock polarity low level
      \arg        I2S_CKPL_HIGH : I2S clock polarity high level
    \param[out] none
    \retval     none
*/
void i2s_init(uint32_t spi_periph,uint32_t i2s_mode,uint32_t i2s_std,uint32_t i2s_ckpl)
{
    uint32_t reg= 0;
    reg= SPI_I2SCTL(spi_periph);
    /* enable I2S */
    reg |= SPI_I2SCTL_I2SSEL; 
    /* select I2S mode */
    reg |= i2s_mode;
    /* select I2S standard */
    reg |= i2s_std;
    /* select I2S polarity */
    reg |= i2s_ckpl;
    SPI_I2SCTL(spi_periph) = reg;
}

/*!
    \brief      I2S enable
    \param[in]  spi_periph: SPIx(x=0,2)
    \param[out] none
    \retval     none
*/
void i2s_enable(uint32_t spi_periph)
{
    SPI_I2SCTL(spi_periph) |= SPI_I2SCTL_I2SEN;
}

/*!
    \brief      I2S disable
    \param[in]  spi_periph: SPIx(x=0,2)
    \param[out] none
    \retval     none
*/
void i2s_disable(uint32_t spi_periph)
{
    SPI_I2SCTL(spi_periph) &= ~SPI_I2SCTL_I2SEN;
}

/*!
    \brief      SPI nss output enable
    \param[in]  spi_periph: SPIx(x=0,1,2)
    \param[out] none
    \retval     none
*/
void spi_nss_output_enable(uint32_t spi_periph)
{
    SPI_CTL1(spi_periph) |= SPI_CTL1_NSSDRV;
}

/*!
    \brief      SPI nss output disable
    \param[in]  spi_periph: SPIx(x=0,1,2)
    \param[out] none
    \retval     none
*/
void spi_nss_output_disable(uint32_t spi_periph)
{
    SPI_CTL1(spi_periph) &= ~SPI_CTL1_NSSDRV;
}

/*!
    \brief      SPI nss pin high level in software mode
    \param[in]  spi_periph: SPIx(x=0,1,2)
    \param[out] none
    \retval     none
*/
void spi_nss_internal_high(uint32_t spi_periph)
{
    SPI_CTL0(spi_periph) |= SPI_CTL0_SWNSS;
}

/*!
    \brief      SPI nss pin low level in software mode
    \param[in]  spi_periph: SPIx(x=0,1,2)
    \param[out] none
    \retval     none
*/
void spi_nss_internal_low(uint32_t spi_periph)
{
    SPI_CTL0(spi_periph) &= ~SPI_CTL0_SWNSS;
}

/*!
    \brief      SPI dma send or receive enable
    \param[in]  spi_periph: SPIx(x=0,1,2)
    \param[in]  spi_dma: 
      \arg        SPI_DMA_SEND: enable DMA send
      \arg        SPI_DMA_RECEIVE: enable DMA receive
    \param[out] none
    \retval     none
*/
void spi_dma_enable(uint32_t spi_periph,uint8_t spi_dma)
{
    if(SPI_DMA_SEND == spi_dma){
        SPI_CTL1(spi_periph) |= SPI_CTL1_DMATEN;
    }else{
        SPI_CTL1(spi_periph) |= SPI_CTL1_DMAREN;
    }
}

/*!
    \brief      SPI dma send or receive diable
    \param[in]  spi_periph: SPIx(x=0,1,2)
    \param[in]  spi_dma: 
      \arg        SPI_DMA_SEND: disable DMA send
      \arg        SPI_DMA_RECEIVE: disable DMA receive
    \param[out] none
    \retval     none
*/
void spi_dma_disable(uint32_t spi_periph,uint8_t spi_dma)
{
    if(SPI_DMA_SEND == spi_dma){
        SPI_CTL1(spi_periph) &= ~SPI_CTL1_DMATEN;
    }else{
        SPI_CTL1(spi_periph) &= ~SPI_CTL1_DMAREN;
    }
}

/*!
    \brief      send data
    \param[in]  spi_periph: SPIx(x=0,1,2)
    \param[in]  data
    \param[out] none
    \retval     none
*/
void spi_i2s_data_send(uint32_t spi_periph,uint16_t data)
{
    SPI_DATA(spi_periph) = data;
}

/*!
    \brief      receive data
    \param[in]  spi_periph: SPIx(x=0,1,2)
    \param[out] none
    \retval     data
*/
uint16_t spi_i2s_data_receive(uint32_t spi_periph)
{
    return (uint16_t)SPI_DATA(spi_periph);
}

/*!
    \brief      SPI and I2S interrupt enable
    \param[in]  spi_periph: SPIx(x=0,1,2)
    \param[in]  spi_i2s_int:
      \arg        SPI_I2S_INT_TBE: transmit buffer empty interrupt.
      \arg        SPI_I2S_INT_RBNE: receive buffer not empty interrupt.
      \arg        SPI_I2S_INT_ERR: CRC error interrupt.
    \param[out] none
    \retval     none
*/
void spi_i2s_interrupt_enable(uint32_t spi_periph,uint8_t spi_i2s_int)
{
    switch(spi_i2s_int){
    case SPI_I2S_INT_TBE :
        SPI_CTL1(spi_periph) |= SPI_CTL1_TBEIE;
        break;
    case SPI_I2S_INT_RBNE :
        SPI_CTL1(spi_periph) |= SPI_CTL1_RBNEIE;
        break;
    case SPI_I2S_INT_ERR :
        SPI_CTL1(spi_periph) |= SPI_CTL1_ERRIE;
        break;
    default :
        break;
    }
}

/*!
    \brief      SPI and I2S interrupt disable
    \param[in]  spi_periph: SPIx(x=0,1,2)
    \param[in]  spi_i2s_int:
      \arg        SPI_I2S_INT_TBE: transmit buffer empty interrupt.
      \arg        SPI_I2S_INT_RBNE: receive buffer not empty interrupt.
      \arg        SPI_I2S_INT_ERR: CRC error interrupt.
    \param[out] none
    \retval     none
*/
void spi_i2s_interrupt_disable(uint32_t spi_periph,uint8_t spi_i2s_int)
{
    switch(spi_i2s_int){
    case SPI_I2S_INT_TBE :
        SPI_CTL1(spi_periph) &= ~SPI_CTL1_TBEIE;
        break;
    case SPI_I2S_INT_RBNE :
        SPI_CTL1(spi_periph) &= ~SPI_CTL1_RBNEIE;
        break;
    case SPI_I2S_INT_ERR :
        SPI_CTL1(spi_periph) &= ~SPI_CTL1_ERRIE;
        break;
    default :
        break;
    }
}
/*!
    \brief      get interrupt status
    \param[in]  spi_periph: SPIx(x=0,1,2)
    \param[in]  spi_i2s_int:
      \arg        SPI_I2S_INT_TBE: transmit buffer empty interrupt.
      \arg        SPI_I2S_INT_RBNE: receive buffer not empty interrupt.
      \arg        SPI_I2S_INT_RXORERR: overrun interrupt.
      \arg        SPI_INT_CONFERR: config error interrupt.
      \arg        SPI_INT_CRCERR: CRC error interrupt.
      \arg        I2S_INT_TXURERR: underrun error interrupt.
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus spi_i2s_interrupt_flag_get(uint32_t spi_periph,uint8_t spi_i2s_int)
{
    uint32_t ret = 0;
    uint32_t reg1 = SPI_STAT(spi_periph);
    uint32_t reg2 = SPI_CTL1(spi_periph);
    switch(spi_i2s_int){
    case SPI_I2S_INT_TBE :
        ret=(reg1 &= SPI_STAT_TBE) && (reg2 &= SPI_CTL1_TBEIE);
        break;
    case SPI_I2S_INT_RBNE :
        ret=(reg1 &= SPI_STAT_RBNE) && (reg2 &= SPI_CTL1_RBNEIE);
        break;
    case SPI_I2S_INT_RXORERR :
        ret=(reg1 &= SPI_STAT_RXORERR) && (reg2 &= SPI_CTL1_ERRIE);
        break;
    case SPI_INT_CONFERR :
        ret=(reg1 &= SPI_STAT_CONFERR) && (reg2 &= SPI_CTL1_ERRIE);
        break;
    case SPI_INT_CRCERR :
        ret=(reg1 &= SPI_STAT_CRCERR) && (reg2 &= SPI_CTL1_ERRIE);
        break;
    case I2S_INT_TXURERR :
        ret=(reg1 &= SPI_STAT_TXURERR) && (reg2 &= SPI_CTL1_ERRIE);
        break;
    default :
        break;
    }
    return (FlagStatus)ret;
}


/*!
    \brief      get flag status
    \param[in]  spi_periph: SPIx(x=0,1,2)
    \param[in]  spi_i2s_flag:
      \arg        SPI_FLAG_TBE: transmit buffer empty flag.
      \arg        SPI_FLAG_RBNE: receive buffer not empty flag.
      \arg        SPI_FLAG_TRANS: transmit on-going flag.
      \arg        SPI_FLAG_RXORERR: receive Overrun flag.
      \arg        SPI_FLAG_CONFERR: mode config error flag.
      \arg        SPI_FLAG_CRCERR: CRC error flag.
      \arg        I2S_FLAG_TBE: transmit buffer empty flag.
      \arg        I2S_FLAG_RBNE: receive buffer not empty flag.
      \arg        I2S_FLAG_TRANS: transmit on-going flag.
      \arg        I2S_FLAG_RXORERR: overrun flag.
      \arg        I2S_FLAG_TXURERR: underrun error flag.
      \arg        I2S_FLAG_CH: channel side flag.
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus spi_i2s_flag_get(uint32_t spi_periph,uint32_t spi_i2s_flag)
{
    if(SPI_STAT(spi_periph) & spi_i2s_flag)
        return SET;
    return RESET;
}


/*!
    \brief      clear SPI CRC error flag status
    \param[in]  spi_periph: SPIx(x=0,1,2)
    \param[out] none
    \retval     none
*/
void spi_crc_error_clear(uint32_t spi_periph)
{
    SPI_STAT(spi_periph) &= ~SPI_FLAG_CRCERR;
}

/*!
    \brief      crc function turn on
    \param[in]  spi_periph: SPIx(x=0,1,2)
    \param[out] none
    \retval     none
*/
void spi_crc_on(uint32_t spi_periph)
{
    SPI_CTL0(spi_periph) |= SPI_CTL0_CRCEN;
}

/*!
    \brief      crc function turn off
    \param[in]  spi_periph: SPIx(x=0,1,2)
    \param[out] none
    \retval     none
*/
void spi_crc_off(uint32_t spi_periph)
{
    SPI_CTL0(spi_periph) &= ~SPI_CTL0_CRCEN;
}

/*!
    \brief      CRC polynomial set
    \param[in]  spi_periph: SPIx(x=0,1,2)
    \param[in]  crc_poly: CRC polynomial value
    \param[out] none
    \retval     none
*/
void spi_crc_polynomial_set(uint32_t spi_periph,uint16_t crc_poly)
{
    /* enable SPI CRC */
    SPI_CTL0(spi_periph) |= SPI_CTL0_CRCEN;
    /* set SPI CRC polynomial */
    SPI_CRCPOLY(spi_periph) = (uint32_t)crc_poly;
}

/*!
    \brief      crc polynomial get
    \param[in]  spi_periph: SPIx(x=0,1,2)
    \param[out] none
    \retval     crc polynomial
*/
uint16_t spi_crc_polynomial_get(uint32_t spi_periph)
{
    return (uint16_t)SPI_CRCPOLY(spi_periph);
}

/*!
    \brief      SPI next is crc
    \param[in]  spi_periph: SPIx(x=0,1,2)
    \param[out] none
    \retval     none
*/
void spi_crc_next(uint32_t spi_periph)
{
    SPI_CTL0(spi_periph) |= SPI_CTL0_CRCNT;
}

/*!
    \brief      get SPI crc
    \param[in]  spi_periph: SPIx(x=0,1,2)
    \param[in]  spi_crc: 
      \arg        SPI_CRC_TX: get transmit crc value  
      \arg        SPI_CRC_RX: get receive crc value
    \param[out] none
    \retval     crc value
*/
uint16_t spi_crc_get(uint32_t spi_periph,uint8_t spi_crc)
{
    if(SPI_CRC_TX == spi_crc)
        return (uint16_t)(SPI_TCRC(spi_periph));
    else
        return (uint16_t)(SPI_RCRC(spi_periph));
}

#ifdef GD32F170_190
/*!
    \brief      quad wire SPI enable 
    \param[in]  spi_periph: SPIx(only x=1)
    \param[out] none
    \retval     none
*/
void qspi_enable(uint32_t spi_periph)
{
    SPI_QCTL(spi_periph) |= SPI_QCTL_QMOD;
}

/*!
    \brief      quad wire SPI disable
    \param[in]  spi_periph: SPIx(only x=1)
    \param[out] none
    \retval     none
*/
void qspi_disable(uint32_t spi_periph)
{
    SPI_QCTL(spi_periph) &= ~SPI_QCTL_QMOD ;
}

/*!
    \brief      quad wire SPI write enable
    \param[in]  spi_periph: SPIx(only x=1)
    \param[out] none
    \retval     none
*/
void qspi_write_enable(uint32_t spi_periph)
{
    SPI_QCTL(spi_periph) &= ~SPI_QCTL_QRD;
}

/*!
    \brief      quad wire SPI read enable
    \param[in]  spi_periph: SPIx(only x=1)
    \param[out] none
    \retval     none
*/
void qspi_read_enable(uint32_t spi_periph)
{
    SPI_QCTL(spi_periph) |= SPI_QCTL_QRD;
}

/*!
    \brief      SPI_IO2 and SPI_IO3 pin output enable 
    \param[in]  spi_periph: SPIx(only x=1)
    \param[out] none
    \retval     none
*/
void qspi_io23_output_enable(uint32_t spi_periph)
{
    SPI_QCTL(spi_periph) |= SPI_QCTL_IO23_DRV;
}

 /*!
    \brief      SPI_IO2 and SPI_IO3 pin output disable
    \param[in]  spi_periph: SPIx(only x=1)
    \param[out] none
    \retval     none
*/
 void qspi_io23_output_disable(uint32_t spi_periph)
{
    SPI_QCTL(spi_periph) &=~ SPI_QCTL_IO23_DRV;
}
#endif  /* GD32F170_190 */
