/*!
    \file  gd32f1x0_rcu.c
    \brief rcu driver
*/

/*
    Copyright (C) 2016 GigaDevice

    2014-12-26, V1.0.0, firmware for GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, firmware for GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#include "gd32f1x0_rcu.h"

#define SEL_IRC8M       0x00
#define SEL_HXTAL       0x01
#define SEL_PLL         0x02

/*!
    \brief      deinitialize the RCU
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_deinit(void)
{
    /* enable IRC8M */
    RCU_CTL0 |= RCU_CTL0_IRC8MEN;
    while(0 == (RCU_CTL0 & RCU_CTL0_IRC8MSTB)) ;
    /* reset RCU */
#ifdef GD32F130_150
    RCU_CFG0 &= ~(RCU_CFG0_SCS | RCU_CFG0_AHBPSC | RCU_CFG0_APB1PSC | RCU_CFG0_APB2PSC |\
                  RCU_CFG0_ADCPSC | RCU_CFG0_CKOUTSEL | RCU_CFG0_CKOUTDIV | RCU_CFG0_PLLDV);
#elif defined (GD32F170_190)
    RCU_CFG0 &= ~(RCU_CFG0_SCS | RCU_CFG0_AHBPSC | RCU_CFG0_APB1PSC | RCU_CFG0_APB2PSC |\
                  RCU_CFG0_ADCPSC | RCU_CFG0_CKOUT0SEL | RCU_CFG0_CKOUT0DIV | RCU_CFG0_PLLDV);
#endif /* GD32F130_150 */
    RCU_CFG0 &= ~(RCU_CFG0_PLLSEL | RCU_CFG0_PLLMF | RCU_CFG0_PLLDV);
#ifdef GD32F130_150
    RCU_CFG0 &= ~(RCU_CFG0_USBDPSC);
#endif /* GD32F130_150 */
    RCU_CTL0 &= ~(RCU_CTL0_HXTALEN | RCU_CTL0_CKMEN | RCU_CTL0_PLLEN | RCU_CTL0_HXTALBPS);
    RCU_CFG1 &= ~RCU_CFG1_HXTALPREDV;
    RCU_CFG2 &= ~(RCU_CFG2_USART0SEL | RCU_CFG2_CECSEL | RCU_CFG2_ADCSEL);
#ifdef GD32F130_150
    RCU_CTL1 &= ~RCU_CTL1_IRC14MEN;
#elif defined (GD32F170_190)
    RCU_CFG2 &= ~RCU_CFG2_IRC28MDIV;
    RCU_CTL1 &= ~RCU_CTL1_IRC28MEN;
    RCU_CFG3 &= ~RCU_CFG3_CKOUT1SRC;
    RCU_CFG3 &= ~RCU_CFG3_CKOUT1DIV;
#endif /* GD32F130_150 */
    RCU_INT = 0x00000000;
}

/*!
    \brief      enable the peripherals clock
    \param[in]  periph: RCU peripherals, refer to rcu_periph_enum
      \arg        RCU_GPIOx (x=A,B,C,D,F): GPIO ports clock
      \arg        RCU_DMA: DMA clock
      \arg        RCU_SRAM: SRAM clock
      \arg        RCU_FMC: FMC clock
      \arg        RCU_CRC: CRC clock
      \arg        RCU_TSI: TSI clock
      \arg        RCU_CFGCMP: CFGCMP clock
      \arg        RCU_ADC: ADC clock
      \arg        RCU_TIMERx (x=0,1,2,5,13,14,15,16): TIMER clock
      \arg        RCU_SPIx (x=0,1,2): SPI clock
      \arg        RCU_USARTx (x=0,1): USART clock
      \arg        RCU_SLCD: SLCD clock, only in GD32F170_190
      \arg        RCU_WWDGT: WWDGT clock
      \arg        RCU_I2Cx (x=0,1,2): I2C clock
      \arg        RCU_USBD: USBD clock, only in GD32F130_150
      \arg        RCU_CANx (x=0,1): CAN clock, only in GD32F170_190
      \arg        RCU_PMU: PMU clock
      \arg        RCU_DAC: DAC clock
      \arg        RCU_CEC: CEC clock
      \arg        RCU_OPAIVREF: OPAIVREF clock, only in GD32F170_190
      \arg        RCU_RTC: RTC clock
    \param[out] none
    \retval     none
*/
void rcu_periph_clock_enable(rcu_periph_enum periph)
{
    REG_VAL(periph) |= BIT(BIT_POS(periph));
}

/*!
    \brief      disable the peripherals clock
    \param[in]  periph: RCU peripherals, refer to rcu_periph_enum
      \arg        RCU_GPIOx (x=A,B,C,D,F): GPIO ports clock
      \arg        RCU_DMA: DMA clock
      \arg        RCU_SRAM: SRAM clock
      \arg        RCU_FMC: FMC clock
      \arg        RCU_CRC: CRC clock
      \arg        RCU_TSI: TSI clock
      \arg        RCU_CFGCMP: CFGCMP clock
      \arg        RCU_ADC: ADC clock
      \arg        RCU_TIMERx (x=0,1,2,5,13,14,15,16): TIMER clock
      \arg        RCU_SPIx (x=0,1,2): SPI clock
      \arg        RCU_USARTx (x=0,1): USART clock
      \arg        RCU_SLCD: SLCD clock, only in GD32F170_190
      \arg        RCU_WWDGT: WWDGT clock
      \arg        RCU_I2Cx (x=0,1,2): I2C clock
      \arg        RCU_USBD: USBD clock only in GD32F130_150
      \arg        RCU_CANx (x=0,1): CAN clock, only in GD32F170_190
      \arg        RCU_PMU: PMU clock
      \arg        RCU_DAC: DAC clock
      \arg        RCU_CEC: CEC clock
      \arg        RCU_OPAIVREF: OPAIVREF clock, only in GD32F170_190
      \arg        RCU_RTC: RTC clock
    \param[out] none
    \retval     none
*/
void rcu_periph_clock_disable(rcu_periph_enum periph)
{
    REG_VAL(periph) &= ~BIT(BIT_POS(periph));
}

/*!
    \brief      reset the peripherals
    \param[in]  periph_reset: RCU peripherals reset, refer to rcu_periph_reset_enum
      \arg        RCU_GPIOxRST (x=A,B,C,D,F): reset GPIO ports
      \arg        RCU_TSIRST: reset TSI
      \arg        RCU_CFGCMPRST: reset CFGCMP
      \arg        RCU_ADCRST: reset ADC
      \arg        RCU_TIMERxRST (x=0,1,2,5,13,14,15,16): reset TIMER
      \arg        RCU_SPIxRST (x=0,1,2): reset SPI
      \arg        RCU_USARTxRST (x=0,1): reset USART
      \arg        RCU_SLCDRST: reset SLCD, only in GD32F170_190
      \arg        RCU_WWDGTRST: reset WWDGT
      \arg        RCU_I2CxRST (x=0,1,2): reset I2C
      \arg        RCU_USBDRST: reset USBD, only in GD32F130_150
      \arg        RCU_CANxRST (x=0,1): reset CAN, only in GD32F170_190
      \arg        RCU_PMURST: reset PMU
      \arg        RCU_DACRST: reset DAC
      \arg        RCU_CECRST: reset CEC
      \arg        RCU_OPAIVREFRST: reset OPAIVREF, only in GD32F170_190
    \param[out] none
    \retval     none
*/
void rcu_periph_reset_enable(rcu_periph_reset_enum periph_reset)
{
    REG_VAL(periph_reset) |= BIT(BIT_POS(periph_reset));
}

/*!
    \brief      disable reset the peripheral
    \param[in]  periph_reset: RCU peripherals reset, refer to rcu_periph_reset_enum
      \arg        RCU_GPIOxRST (x=A,B,C,D,F): reset GPIO ports
      \arg        RCU_TSIRST: reset TSI
      \arg        RCU_CFGCMPRST: reset CFGCMP
      \arg        RCU_ADCRST: reset ADC
      \arg        RCU_TIMERxRST (x=0,1,2,5,13,14,15,16): reset TIMER
      \arg        RCU_SPIxRST (x=0,1,2): reset SPI
      \arg        RCU_USARTxRST (x=0,1): reset USART
      \arg        RCU_SLCDRST: reset SLCD, only in GD32F170_190
      \arg        RCU_WWDGTRST: reset WWDGT
      \arg        RCU_I2CxRST (x=0,1,2): reset I2C
      \arg        RCU_USBDRST: reset USBD, only in GD32F130_150
      \arg        RCU_CANxRST (x=0,1): reset CAN, only in GD32F170_190
      \arg        RCU_PMURST: reset PMU
      \arg        RCU_DACRST: reset DAC
      \arg        RCU_CECRST: reset CEC
      \arg        RCU_OPAIVREFRST: reset OPAIVREF, only in GD32F170_190
    \param[out] none
    \retval     none
*/
void rcu_periph_reset_disable(rcu_periph_reset_enum periph_reset)
{
    REG_VAL(periph_reset) &= ~BIT(BIT_POS(periph_reset));
}

/*!
    \brief      reset the BKP
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_bkp_reset_enable(void)
{
    RCU_BDCTL |= RCU_BDCTL_BKPRST;
}

/*!
    \brief      disable the BKP reset
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_bkp_reset_disable(void)
{
    RCU_BDCTL &= ~RCU_BDCTL_BKPRST;
}

/*!
    \brief      configure the system clock source
    \param[in]  ck_sys: system clock source select
      \arg        RCU_CKSYSSRC_IRC8M: select CK_IRC8M as the CK_SYS source
      \arg        RCU_CKSYSSRC_HXTAL: select CK_HXTAL as the CK_SYS source
      \arg        RCU_CKSYSSRC_PLL: select CK_PLL as the CK_SYS source
    \param[out] none
    \retval     none
*/
void rcu_system_clock_source_config(uint32_t ck_sys)
{
    uint32_t cksys_source = 0;
    cksys_source = RCU_CFG0;
    /* reset the SCS bits and set according to ck_sys */
    cksys_source &= ~RCU_CFG0_SCS;
    RCU_CFG0 = (ck_sys | cksys_source);
}

/*!
    \brief      get the system clock source
    \param[in]  none
    \param[out] none
    \retval     which clock is selected as CK_SYS source
      \arg        RCU_SCSS_IRC8M: select CK_IRC8M as the CK_SYS source
      \arg        RCU_SCSS_HXTAL: select CK_HXTAL as the CK_SYS source
      \arg        RCU_SCSS_PLL: select CK_PLL as the CK_SYS source
*/
uint32_t rcu_system_clock_source_get(void)
{
    return (RCU_CFG0 & 0x0000000C);
}

/*!
    \brief      configure the AHB clock prescaler selection
    \param[in]  ck_ahb: AHB clock prescaler selection
      \arg        RCU_AHB_CKSYS_DIVx, x=1, 2, 4, 8, 16, 64, 128, 256, 512
    \param[out] none
    \retval     none
*/
void rcu_ahb_clock_config(uint32_t ck_ahb)
{
    uint32_t ahbpsc = 0;
    ahbpsc = RCU_CFG0;
    /* reset the AHBPSC bits and set according to ck_ahb */
    ahbpsc &= ~RCU_CFG0_AHBPSC;
    RCU_CFG0 = (ck_ahb | ahbpsc);
}

/*!
    \brief      configure the APB1 clock prescaler selection
    \param[in]  ck_apb1: APB1 clock prescaler selection
      \arg        RCU_APB1_CKAHB_DIV1: select CK_AHB as CK_APB1
      \arg        RCU_APB1_CKAHB_DIV2: select CK_AHB/2 as CK_APB1
      \arg        RCU_APB1_CKAHB_DIV4: select CK_AHB/4 as CK_APB1
      \arg        RCU_APB1_CKAHB_DIV8: select CK_AHB/8 as CK_APB1
      \arg        RCU_APB1_CKAHB_DIV16: select CK_AHB/16 as CK_APB1
    \param[out] none
    \retval     none
*/
void rcu_apb1_clock_config(uint32_t ck_apb1)
{
    uint32_t apb1psc = 0;
    apb1psc = RCU_CFG0;
    /* reset the APB1PSC and set according to ck_apb1 */
    apb1psc &= ~RCU_CFG0_APB1PSC;
    RCU_CFG0 = (ck_apb1 | apb1psc);
}

/*!
    \brief      configure the APB2 clock prescaler selection
    \param[in]  ck_apb2: APB2 clock prescaler selection
      \arg        RCU_APB2_CKAHB_DIV1: select CK_AHB as CK_APB2
      \arg        RCU_APB2_CKAHB_DIV2: select CK_AHB/2 as CK_APB2
      \arg        RCU_APB2_CKAHB_DIV4: select CK_AHB/4 as CK_APB2
      \arg        RCU_APB2_CKAHB_DIV8: select CK_AHB/8 as CK_APB2
      \arg        RCU_APB2_CKAHB_DIV16: select CK_AHB/16 as CK_APB2
    \param[out] none
    \retval     none
*/
void rcu_apb2_clock_config(uint32_t ck_apb2)
{
    uint32_t apb2psc = 0;
    apb2psc = RCU_CFG0;
    /* reset the APB2PSC and set according to ck_apb2 */
    apb2psc &= ~RCU_CFG0_APB2PSC;
    RCU_CFG0 = (ck_apb2 | apb2psc);
}

/*!
    \brief      configure the ADC clock prescaler selection
    \param[in]  ck_adc: ADC clock prescaler selection, refer to rcu_adc_clock_enum
      \arg        RCU_ADCCK_IRC14M: select CK_IRC14M as CK_ADC, only in GD32F130_150
      \arg        RCU_ADCCK_IRC28M_DIV2: select CK_IRC28M/2 as CK_ADC, only in GD32F170_190
      \arg        RCU_ADCCK_IRC28M: select CK_IRC28M as CK_ADC, only in GD32F170_190
      \arg        RCU_ADCCK_APB2_DIV2: select CK_APB2/2 as CK_ADC
      \arg        RCU_ADCCK_APB2_DIV4: select CK_APB2/4 as CK_ADC
      \arg        RCU_ADCCK_APB2_DIV6: select CK_APB2/6 as CK_ADC
      \arg        RCU_ADCCK_APB2_DIV8: select CK_APB2/8 as CK_ADC
    \param[out] none
    \retval     none
*/
void rcu_adc_clock_config(rcu_adc_clock_enum ck_adc)
{
    /* reset the ADCPSC, ADCSEL, IRC28MDIV bits */
    RCU_CFG0 &= ~RCU_CFG0_ADCPSC;
#ifdef GD32F130_150
    RCU_CFG2 &= ~RCU_CFG2_ADCSEL;
#elif defined (GD32F170_190)
    RCU_CFG2 &= ~(RCU_CFG2_ADCSEL | RCU_CFG2_IRC28MDIV);
#endif /* GD32F130_150 */
    /* set the ADC clock according to ck_adc */
    switch(ck_adc){
#ifdef GD32F130_150
    case RCU_ADCCK_IRC14M:
        RCU_CFG2 &= ~RCU_CFG2_ADCSEL;
        break;
#elif defined (GD32F170_190)
    case RCU_ADCCK_IRC28M_DIV2:
        RCU_CFG2 &= ~RCU_CFG2_IRC28MDIV;
        RCU_CFG2 &= ~RCU_CFG2_ADCSEL;
        break;
    case RCU_ADCCK_IRC28M:
        RCU_CFG2 |= RCU_CFG2_IRC28MDIV;
        RCU_CFG2 &= ~RCU_CFG2_ADCSEL;
        break;
#endif /* GD32F130_150 */
    case RCU_ADCCK_APB2_DIV2:
        RCU_CFG0 |= RCU_ADC_CKAPB2_DIV2;
        RCU_CFG2 |= RCU_CFG2_ADCSEL;
        break;
    case RCU_ADCCK_APB2_DIV4:
        RCU_CFG0 |= RCU_ADC_CKAPB2_DIV4;
        RCU_CFG2 |= RCU_CFG2_ADCSEL;
        break;
    case RCU_ADCCK_APB2_DIV6: 
        RCU_CFG0 |= RCU_ADC_CKAPB2_DIV6;
        RCU_CFG2 |= RCU_CFG2_ADCSEL;
        break;
    case RCU_ADCCK_APB2_DIV8: 
        RCU_CFG0 |= RCU_ADC_CKAPB2_DIV8;
        RCU_CFG2 |= RCU_CFG2_ADCSEL;
        break;
    default:
        break;
    }
}

#ifdef GD32F130_150
/*!
    \brief      configure the USBD clock prescaler selection
    \param[in]  ck_usbd: USBD clock prescaler selection
      \arg        RCU_USBD_CKPLL_DIV1_5: select CK_PLL/1.5 as CK_USBD
      \arg        RCU_USBD_CKPLL_DIV1: select CK_PLL as CK_USBD
      \arg        RCU_USBD_CKPLL_DIV2_5: select CK_PLL/2.5 as CK_USBD
      \arg        RCU_USBD_CKPLL_DIV2: select CK_PLL/2 as CK_USBD
    \param[out] none
    \retval     none
*/
void rcu_usbd_clock_config(uint32_t ck_usbd)
{
    /* reset the USBDPSC bits and set according to ck_usbd */
    RCU_CFG0 &= ~RCU_CFG0_USBDPSC;
    RCU_CFG0 |= ck_usbd;
}

/*!
    \brief      configure the CK_OUT clock source and divider
    \param[in]  ckout_src: CK_OUT clock source selection
      \arg        RCU_CKOUTSEL_NONE: no clock selected
      \arg        RCU_CKOUTSEL_IRC14M: IRC14M selected
      \arg        RCU_CKOUTSEL_IRC40K: IRC40K selected
      \arg        RCU_CKOUTSEL_LXTAL: LXTAL selected
      \arg        RCU_CKOUTSEL_CKSYS: CKSYS selected
      \arg        RCU_CKOUTSEL_IRC8M: IRC8M selected
      \arg        RCU_CKOUTSEL_HXTAL: HXTAL selected
      \arg        RCU_CKOUTSEL_CKPLL_DIV1: CK_PLL selected
      \arg        RCU_CKOUTSEL_CKPLL_DIV2: CK_PLL/2 selected
    \param[in]  ckout_div: CK_OUT divider 
      \arg        RCU_CKOUT_DIVx(x=1,2,4,8,16,32,64,128): CK_OUT is divided by x
    \param[out] none
    \retval     none
*/
void rcu_ckout_config(uint32_t ckout_src, uint32_t ckout_div)
{
    uint32_t ckout = 0;
    ckout = RCU_CFG0;
    /* reset the CKOUTSEL, CKOUTDIV and PLLDV bits and set according to ckout_src and ckout_div */
    ckout &= ~(RCU_CFG0_CKOUTSEL | RCU_CFG0_CKOUTDIV | RCU_CFG0_PLLDV);
    RCU_CFG0 = (ckout | ckout_src | ckout_div);
}
#elif defined (GD32F170_190)
/*!
    \brief      configure the CK_OUT0 clock source and divider
    \param[in]  ckout0_src: CK_OUT0 clock source selection
      \arg        RCU_CKOUT0SEL_NONE: no clock selected
      \arg        RCU_CKOUT0SEL_IRC28M: IRC28M selected
      \arg        RCU_CKOUT0SEL_IRC40K: IRC40K selected
      \arg        RCU_CKOUT0SEL_LXTAL: LXTAL selected
      \arg        RCU_CKOUT0SEL_CKSYS: CKSYS selected
      \arg        RCU_CKOUT0SEL_IRC8M: IRC8M selected
      \arg        RCU_CKOUT0SEL_HXTAL: HXTAL selected
      \arg        RCU_CKOUT0SEL_CKPLL_DIV1: CK_PLL selected
      \arg        RCU_CKOUT0SEL_CKPLL_DIV2: CK_PLL/2 selected
    \param[in]  ckout0_div: CK_OUT0 divider 
      \arg        RCU_CKOUT0_DIVx(x=1,2,4,8,16,32,64,128): CK_OUT0 is divided by x
    \param[out] none
    \retval     none
*/
void rcu_ckout0_config(uint32_t ckout0_src, uint32_t ckout0_div)
{
    uint32_t ckout0 = 0;
    ckout0 = RCU_CFG0;
    /* reset the CKOUT0SEL, CKOUT0DIV and PLLDV bits and set according to ckout0_src and ckout0_div */
    ckout0 &= ~(RCU_CFG0_CKOUT0SEL | RCU_CFG0_CKOUT0DIV | RCU_CFG0_PLLDV);
    RCU_CFG0 = (ckout0 | ckout0_src | ckout0_div);
}

/*!
    \brief      configure the CK_OUT1 clock source and divider
    \param[in]  ckout1_src: CK_OUT1 clock source selection
      \arg        RCU_CKOUT1SRC_NONE: no clock selected
      \arg        RCU_CKOUT1SRC_IRC28M: IRC28M selected
      \arg        RCU_CKOUT1SRC_IRC40K: IRC40K selected
      \arg        RCU_CKOUT1SRC_LXTAL: LXTAL selected
      \arg        RCU_CKOUT1SRC_CKSYS: CKSYS selected
      \arg        RCU_CKOUT1SRC_IRC8M: IRC8M selected
      \arg        RCU_CKOUT1SRC_HXTAL: HXTAL selected
      \arg        RCU_CKOUT1SRC_CKPLL_DIV1: CK_PLL selected
      \arg        RCU_CKOUT1SRC_CKPLL_DIV2: CK_PLL/2 selected
    \param[in]  ckout1_div: CK_OUT1 divider 
      \arg        RCU_CKOUT1_DIVx(x=1..64): CK_OUT1 is divided by x
    \param[out] none
    \retval     none
*/
void rcu_ckout1_config(uint32_t ckout1_src, uint32_t ckout1_div)
{
    uint32_t ckout1 = 0;
    ckout1 = RCU_CFG3;
    /* reset the CKOUT1SRC, CKOUT1DIV bits and set according to ckout1_src and ckout1_div */
    ckout1 &= ~(RCU_CFG3_CKOUT1SRC | RCU_CFG3_CKOUT1DIV);
    if(RCU_CKOUT1SRC_CKPLL_DIV1 == ckout1_src){
        RCU_CFG0 |= RCU_CFG0_PLLDV;
        ckout1_src = CFG3_CKOUT1SRC(7);
    }else if(RCU_CKOUT1SRC_CKPLL_DIV2 == ckout1_src){
        RCU_CFG0 &= ~RCU_CFG0_PLLDV;
        ckout1_src = CFG3_CKOUT1SRC(7);
    }
    RCU_CFG3 = (ckout1 | ckout1_src | ckout1_div);
}
#endif /* GD32F130_150 */

/*!
    \brief      configure the PLL clock source selection and PLL multiply factor
    \param[in]  pll_src: PLL clock source selection
      \arg        RCU_PLLSRC_IRC8M_DIV2: select CK_IRC8M/2 as PLL source clock
      \arg        RCU_PLLSRC_HXTAL: select HXTAL as PLL source clock
    \param[in]  pll_mul: PLL multiply factor
      \arg        RCU_PLL_MULx(x=2..32): PLL source clock * x
    \param[out] none
    \retval     none
*/
void rcu_pll_config(uint32_t pll_src, uint32_t pll_mul)
{
    RCU_CFG0 &= ~(RCU_CFG0_PLLSEL | RCU_CFG0_PLLMF);
    RCU_CFG0 |= (pll_src | pll_mul);
}

/*!
    \brief      configure the USART clock source selection
    \param[in]  ck_usart: USART clock source selection
      \arg        RCU_USART0SRC_CKAPB2: CK_USART0 select CK_APB2
      \arg        RCU_USART0SRC_CKSYS: CK_USART0 select CK_SYS
      \arg        RCU_USART0SRC_LXTAL: CK_USART0 select CK_LXTAL
      \arg        RCU_USART0SRC_IRC8M: CK_USART0 select CK_IRC8M
    \param[out] none
    \retval     none
*/
void rcu_usart_clock_config(uint32_t ck_usart)
{
    /* reset the USART0SEL bits and set according to ck_usart */
    RCU_CFG2 &= ~RCU_CFG2_USART0SEL;
    RCU_CFG2 |= ck_usart;
}

/*!
    \brief      configure the CEC clock source selection
    \param[in]  ck_cec: CEC clock source selection
      \arg        RCU_CECSRC_IRC8M_DIV244: CK_CEC select CK_IRC8M/244
      \arg        RCU_CECSRC_LXTAL: CK_CEC select CK_LXTAL
    \param[out] none
    \retval     none
*/
void rcu_cec_clock_config(uint32_t ck_cec)
{
    /* reset the CECSEL bit and set according to ck_cec */
    RCU_CFG2 &= ~RCU_CFG2_CECSEL;
    RCU_CFG2 |= ck_cec;
}

/*!
    \brief      configure the RTC clock source selection
    \param[in]  rtc_clock_source: RTC clock source selection
      \arg        RCU_RTC_NONE: no clock selected
      \arg        RCU_RTC_LXTAL: CK_LXTAL selected as RTC source clock
      \arg        RCU_RTC_IRC40K: CK_IRC40K selected as RTC source clock
      \arg        RCU_RTC_HXTAL_DIV32: CK_HXTAL/32 selected as RTC source clock
    \param[out] none
    \retval     none
*/
void rcu_rtc_clock_config(uint32_t rtc_clock_source)
{
    /* reset the RTCSRC bits and set according to rtc_clock_source */
    RCU_BDCTL &= ~RCU_BDCTL_RTCSRC;
    RCU_BDCTL |= rtc_clock_source;
}

#ifdef GD32F170_190
/*!
    \brief      configure the SLCD clock source selection
    \param[in]  slcd_clock_source: SLCD clock source selection
      \arg        RCU_SLCD_NONE: no clock selected
      \arg        RCU_SLCD_LXTAL: CK_LXTAL selected as SLCD source clock
      \arg        RCU_SLCD_IRC40K: CK_IRC40K selected as SLCD source clock
      \arg        RCU_SLCD_HXTAL_DIV32: CK_HXTAL/32 selected as SLCD source clock
    \param[out] none
    \retval     none
*/
void rcu_slcd_clock_config(uint32_t slcd_clock_source)
{
    /* reset the bits and set according to rtc_clock_source */
    RCU_BDCTL &= ~RCU_BDCTL_RTCSRC;
    RCU_BDCTL |= slcd_clock_source;
}
#endif /* GD32F170_190 */

/*!
    \brief      configure the HXTAL divider used as input of PLL
    \param[in]  hxtal_prediv: HXTAL divider used as input of PLL
      \arg        RCU_PLL_HXTAL_DIVx(x=1..16): HXTAL divided x used as input of PLL
    \param[out] none
    \retval     none
*/
void rcu_hxtal_prediv_config(uint32_t hxtal_prediv)
{
    uint32_t prediv = 0;
    prediv = RCU_CFG1;
    /* reset the HXTALPREDV bits and set according to hxtal_prediv */
    prediv &= ~RCU_CFG1_HXTALPREDV;
    RCU_CFG1 = (prediv | hxtal_prediv);
}

/*!
    \brief      configure the LXTAL drive capability
    \param[in]  lxtal_dricap: drive capability of LXTAL
      \arg        RCU_LXTAL_LOWDRI: lower driving capability
      \arg        RCU_LXTAL_MED_LOWDRI: medium low driving capability
      \arg        RCU_LXTAL_MED_HIGHDRI: medium high driving capability
      \arg        RCU_LXTAL_HIGHDRI: higher driving capability
    \param[out] none
    \retval     none
*/
void rcu_lxtal_drive_capability_config(uint32_t lxtal_dricap)
{
    /* reset the LXTALDRI bits and set according to lxtal_dricap */
    RCU_BDCTL &= ~RCU_BDCTL_LXTALDRI;
    RCU_BDCTL |= lxtal_dricap;
}

/*!
    \brief      get the clock stabilization and periphral reset flags
    \param[in]  flag: the clock stabilization and periphral reset flags, refer to rcu_flag_enum
      \arg        RCU_FLAG_IRC40KSTB: IRC40K stabilization flag
      \arg        RCU_FLAG_LXTALSTB: LXTAL stabilization flag
      \arg        RCU_FLAG_IRC8MSTB: IRC8M stabilization flag
      \arg        RCU_FLAG_HXTALSTB: HXTAL stabilization flag
      \arg        RCU_FLAG_PLLSTB: PLL stabilization flag
      \arg        RCU_FLAG_IRC14MSTB: IRC14M stabilization flag, only in GD32F130_150
      \arg        RCU_FLAG_IRC28MSTB: IRC28M stabilization flag, only in GD32F170_190
      \arg        RCU_FLAG_V12RST: V12 domain Power reset flag
      \arg        RCU_FLAG_OBLRST: Option byte loader reset flag
      \arg        RCU_FLAG_EPRST: External PIN reset flag
      \arg        RCU_FLAG_PMURST: Power reset flag
      \arg        RCU_FLAG_SWRST: Software reset flag
      \arg        RCU_FLAG_FWDGTRST: Free watchdog timer reset flag
      \arg        RCU_FLAG_WWDGTRST: Window watchdog timer reset flag
      \arg        RCU_FLAG_LPRST: Low-power reset flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus rcu_flag_get(rcu_flag_enum flag)
{
    if(RESET != (REG_VAL(flag) & BIT(BIT_POS(flag)))){
        return SET;
    }else{
        return RESET;
    }
}

/*!
    \brief      clear the reset flag
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_reset_flag_clear(void)
{
    RCU_RSTSCK |= RCU_RSTSCK_RSTFC;
}

/*!
    \brief      get the clock stabilization interrupt and ckm flags
    \param[in]  int_flag: interrupt and ckm flags, refer to rcu_int_flag_enum
      \arg        RCU_INT_FLAG_IRC40KSTB: IRC40K stabilization interrupt flag
      \arg        RCU_INT_FLAG_LXTALSTB: LXTAL stabilization interrupt flag
      \arg        RCU_INT_FLAG_IRC8MSTB: IRC8M stabilization interrupt flag
      \arg        RCU_INT_FLAG_HXTALSTB: HXTAL stabilization interrupt flag
      \arg        RCU_INT_FLAG_PLLSTB: PLL stabilization interrupt flag
      \arg        RCU_INT_FLAG_IRC14MSTB: IRC14M stabilization interrupt flag, only in GD32F130_150
      \arg        RCU_INT_FLAG_IRC28MSTB: IRC28M stabilization interrupt flag, only in GD32F170_190
      \arg        RCU_INT_FLAG_CKM: HXTAL clock stuck interrupt flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus rcu_interrupt_flag_get(rcu_int_flag_enum int_flag)
{
    if(RESET != (RCU_INT & int_flag)){
        return SET;
    }else{
        return RESET;
    }
}

/*!
    \brief      clear the interrupt flags
    \param[in]  int_flag_clear: clock stabilization and stuck interrupt flags clear, refer to rcu_int_flag_clear_enum
      \arg        RCU_INT_FLAG_IRC40KSTB_CLR: IRC40K stabilization interrupt flag clear
      \arg        RCU_INT_FLAG_LXTALSTB_CLR: LXTAL stabilization interrupt flag clear
      \arg        RCU_INT_FLAG_IRC8MSTB_CLR: IRC8M stabilization interrupt flag clear
      \arg        RCU_INT_FLAG_HXTALSTB_CLR: HXTAL stabilization interrupt flag clear
      \arg        RCU_INT_FLAG_PLLSTB_CLR: PLL stabilization interrupt flag clear
      \arg        RCU_INT_FLAG_IRC14MSTB_CLR: IRC14M stabilization interrupt flag clear, only in GD32F130_150
      \arg        RCU_INT_FLAG_IRC28MSTB_CLR: IRC28M stabilization interrupt flag clear, only in GD32F170_190
      \arg        RCU_INT_FLAG_CKM_CLR: clock stuck interrupt flag clear
    \param[out] none
    \retval     none
*/
void rcu_interrupt_flag_clear(rcu_int_flag_clear_enum int_flag_clear)
{
    RCU_INT |= int_flag_clear;
}

/*!
    \brief      enable the stabilization interrupt
    \param[in]  stab_int: clock stabilization interrupt, refer to rcu_stab_int_enum
      \arg        RCU_INT_IRC40KSTB: IRC40K stabilization interrupt enable
      \arg        RCU_INT_LXTALSTB: LXTAL stabilization interrupt enable
      \arg        RCU_INT_IRC8MSTB: IRC8M stabilization interrupt enable
      \arg        RCU_INT_HXTALSTB: HXTAL stabilization interrupt enable
      \arg        RCU_INT_PLLSTB: PLL stabilization interrupt enable
      \arg        RCU_INT_IRC14MSTB: IRC14M stabilization interrupt enable, only in GD32F130_150
      \arg        RCU_INT_IRC28MSTB: IRC28M stabilization interrupt enable, only in GD32F170_190
    \param[out] none
    \retval     none
*/
void rcu_stab_interrupt_enable(rcu_stab_int_enum stab_int)
{
    RCU_INT |= stab_int;
}


/*!
    \brief      disable the stabilization interrupt
    \param[in]  stab_int: clock stabilization interrupt, refer to rcu_stab_int_enum
      \arg        RCU_INT_IRC40KSTB: IRC40K stabilization interrupt disable
      \arg        RCU_INT_LXTALSTB: LXTAL stabilization interrupt disable
      \arg        RCU_INT_IRC8MSTB: IRC8M stabilization interrupt disable
      \arg        RCU_INT_HXTALSTB: HXTAL stabilization interrupt disable
      \arg        RCU_INT_PLLSTB: PLL stabilization interrupt disable
      \arg        RCU_INT_IRC14MSTB: IRC14M stabilization interrupt disable, only in GD32F130_150
      \arg        RCU_INT_IRC28MSTB: IRC28M stabilization interrupt disable, only in GD32F170_190
    \param[out] none
    \retval     none
*/
void rcu_stab_interrupt_disable(rcu_stab_int_enum stab_int)
{
    RCU_INT &= ~stab_int;
}

/*!
    \brief      wait until HXTAL stabilization flag is SET, or HXTAL startup is timeout
    \param[in]  none
    \param[out] none
    \retval     ErrStatus: SUCCESS or ERROR
*/
ErrStatus rcu_hxtal_stab_wait(void)
{
    FlagStatus hxtal_stat = RESET;
    uint32_t stb_cnt = 0;
    /* wait until HXTAL is stabilization and stb_cnt is not more than timeout */
    while((RESET == hxtal_stat) && (HXTAL_STARTUP_TIMEOUT != stb_cnt++)){
        hxtal_stat = rcu_flag_get(RCU_FLAG_HXTALSTB);
    }
    if(RESET != rcu_flag_get(RCU_FLAG_HXTALSTB)){
        return SUCCESS;
    }else{
        return ERROR;
    }
}

/*!
    \brief      wait until IRC8M stabilization flag is SET, or IRC8M startup is timeout
    \param[in]  none
    \param[out] none
    \retval     ErrStatus: SUCCESS or ERROR
*/
ErrStatus rcu_irc8m_stab_wait(void)
{
    FlagStatus irc8m_stat = RESET;
    uint32_t stb_cnt = 0;
    /* wait until IRC8M is stabilization and stb_cnt is not more than timeout */
    while((RESET == irc8m_stat) && (IRC8M_STARTUP_TIMEOUT != stb_cnt++)){
        irc8m_stat = rcu_flag_get(RCU_FLAG_IRC8MSTB);
    }
    if(RESET != rcu_flag_get(RCU_FLAG_IRC8MSTB)){
        return SUCCESS;
    }else{
        return ERROR;
    }
}

/*!
    \brief      wait until oscillator stabilization flags is SET
    \param[in]  osci: oscillator types, refer to rcu_osci_type_enum
      \arg        RCU_HXTAL: HXTAL
      \arg        RCU_LXTAL: LXTAL
      \arg        RCU_IRC8M: IRC8M
      \arg        RCU_IRC14M: IRC14M, only in GD32F130_150
      \arg        RCU_IRC28M: IRC28M, only in GD32F170_190
      \arg        RCU_IRC40K: IRC40K
      \arg        RCU_PLL: PLL
    \param[out] none
    \retval     none
*/
void rcu_osci_stab_wait(rcu_osci_type_enum osci)
{
    switch(osci){
    case RCU_HXTAL:
        while(RESET == rcu_flag_get(RCU_FLAG_HXTALSTB)) ;
        break;
    case RCU_LXTAL:
        while(RESET == rcu_flag_get(RCU_FLAG_LXTALSTB)) ;
        break;
    case RCU_IRC8M:
        while(RESET == rcu_flag_get(RCU_FLAG_IRC8MSTB)) ;
        break;
#ifdef GD32F130_150
    case RCU_IRC14M:
        while(RESET == rcu_flag_get(RCU_FLAG_IRC14MSTB)) ;
        break;
#elif defined (GD32F170_190)
    case RCU_IRC28M:
        while(RESET == rcu_flag_get(RCU_FLAG_IRC28MSTB)) ;
        break;
#endif /* GD32F130_150 */
    case RCU_IRC40K:
        while(RESET == rcu_flag_get(RCU_FLAG_IRC40KSTB)) ;
        break;
    case RCU_PLL:
        while(RESET == rcu_flag_get(RCU_FLAG_PLLSTB)) ;
        break;
    default:
        break;
    }
}

/*!
    \brief      turn on the oscillator
    \param[in]  osci: oscillator types, refer to rcu_osci_type_enum
      \arg        RCU_HXTAL: HXTAL
      \arg        RCU_LXTAL: LXTAL
      \arg        RCU_IRC8M: IRC8M
      \arg        RCU_IRC14M: IRC14M, only in GD32F130_150
      \arg        RCU_IRC28M: IRC28M, only in GD32F170_190
      \arg        RCU_IRC40K: IRC40K
      \arg        RCU_PLL: PLL
    \param[out] none
    \retval     none
*/
void rcu_osci_on(rcu_osci_type_enum osci)
{
    REG_VAL(osci) |= BIT(BIT_POS(osci));
}

/*!
    \brief      turn off the oscillator
    \param[in]  osci: oscillator types, refer to rcu_osci_type_enum
      \arg        RCU_HXTAL: HXTAL
      \arg        RCU_LXTAL: LXTAL
      \arg        RCU_IRC8M: IRC8M
      \arg        RCU_IRC14M: IRC14M, only in GD32F130_150
      \arg        RCU_IRC28M: IRC28M, only in GD32F170_190
      \arg        RCU_IRC40K: IRC40K
      \arg        RCU_PLL: PLL
    \param[out] none
    \retval     none
*/
void rcu_osci_off(rcu_osci_type_enum osci)
{
    REG_VAL(osci) &= ~BIT(BIT_POS(osci));
}

/*!
    \brief      enable the oscillator bypass mode
    \param[in]  osci: oscillator types, refer to rcu_osci_type_enum
      \arg        RCU_HXTAL: HXTAL
      \arg        RCU_LXTAL: LXTAL
    \param[out] none
    \retval     none
*/
void rcu_osci_bypass_mode_enable(rcu_osci_type_enum osci)
{
    uint32_t reg;
    switch(osci){
    case RCU_HXTAL:
        /* HXTALEN must be reset before enable the oscillator bypass mode */
        reg = RCU_CTL0;
        RCU_CTL0 &= ~RCU_CTL0_HXTALEN;
        RCU_CTL0 = (reg | RCU_CTL0_HXTALBPS);
        break;
    case RCU_LXTAL:
        /* LXTALEN must be reset before enable the oscillator bypass mode */
        reg = RCU_BDCTL;
        RCU_BDCTL &= ~RCU_BDCTL_LXTALEN;
        RCU_BDCTL = (reg | RCU_BDCTL_LXTALBPS);
        break;
    case RCU_IRC8M:
#ifdef GD32F130_150
    case RCU_IRC14M:
#elif defined (GD32F170_190)
    case RCU_IRC28M:
#endif /* GD32F130_150 */
    case RCU_IRC40K:
    case RCU_PLL:
        break;
    default:
        break;
    }
}

/*!
    \brief      disable the oscillator bypass mode
    \param[in]  osci: oscillator types, refer to rcu_osci_type_enum
      \arg        RCU_HXTAL: HXTAL
      \arg        RCU_LXTAL: LXTAL
    \param[out] none
    \retval     none
*/
void rcu_osci_bypass_mode_disable(rcu_osci_type_enum osci)
{
    uint32_t reg;
    switch(osci){
    case RCU_HXTAL:
        /* HXTALEN must be reset before disable the oscillator bypass mode */
        reg = RCU_CTL0;
        RCU_CTL0 &= ~RCU_CTL0_HXTALEN;
        RCU_CTL0 = (reg & (~RCU_CTL0_HXTALBPS));
        break;
    case RCU_LXTAL:
        /* LXTALEN must be reset before disable the oscillator bypass mode */
        reg = RCU_BDCTL;
        RCU_BDCTL &= ~RCU_BDCTL_LXTALEN;
        RCU_BDCTL =(reg & (~RCU_BDCTL_LXTALBPS));
        break;
    case RCU_IRC8M:
#ifdef GD32F130_150
    case RCU_IRC14M:
#elif defined (GD32F170_190)
    case RCU_IRC28M:
#endif /* GD32F130_150 */
    case RCU_IRC40K:
    case RCU_PLL:
        break;
    default:
        break;
    }
}

/*!
    \brief      enable the HXTAL clock monitor
    \param[in]  none
    \param[out] none
    \retval     none
*/

void rcu_hxtal_clock_monitor_enable(void)
{
    RCU_CTL0 |= RCU_CTL0_CKMEN;
}

/*!
    \brief      disable the HXTAL clock monitor
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_hxtal_clock_monitor_disable(void)
{
    RCU_CTL0 &= ~RCU_CTL0_CKMEN;
}

/*!
    \brief      set the IRC8M adjust value
    \param[in]  irc8m_adjval: IRC8M adjust value, must be between 0 and 0x1F
    \param[out] none
    \retval     none
*/
void rcu_irc8m_adjust_value_set(uint8_t irc8m_adjval)
{
    uint32_t adjust = 0;
    adjust = RCU_CTL0;
    /* reset the IRC8MADJ bits and set according to irc8m_adjval */
    adjust &= ~RCU_CTL0_IRC8MADJ;
    RCU_CTL0 = (adjust | ((irc8m_adjval)<<3));
}

#ifdef GD32F130_150
/*!
    \brief      set the IRC14M adjust value
    \param[in]  irc14m_adjval: IRC14M adjust value, must be between 0 and 0x1F
    \param[out] none
    \retval     none
*/
void rcu_irc14m_adjust_value_set(uint8_t irc14m_adjval)
{
    uint32_t adjust = 0;
    adjust = RCU_CTL1;
    /* reset the IRC14MADJ bits and set according to irc14m_adjval */
    adjust &= ~RCU_CTL1_IRC14MADJ;
    RCU_CTL1 = (adjust | ((irc14m_adjval)<<3));
}
#elif defined (GD32F170_190)
/*!
    \brief      set the IRC28M adjust value
    \param[in]  irc28m_adjval: IRC28M adjust value, must be between 0 and 0x1F
    \param[out] none
    \retval     none
*/
void rcu_irc28m_adjust_value_set(uint8_t irc28m_adjval)
{
    uint32_t adjust = 0;
    adjust = RCU_CTL1;
    /* reset the IRC28MADJ bits and set according to irc28m_adjval */
    adjust &= ~RCU_CTL1_IRC28MADJ;
    RCU_CTL1 = (adjust | ((irc28m_adjval)<<3));
}
#endif /* GD32F130_150 */

/*!
    \brief      unlock the voltage key
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_voltage_key_unlock(void)
{
    /* reset the KEY bits and set 0x1A2B3C4D */
    RCU_VKEY &= ~RCU_VKEY_KEY;
    RCU_VKEY |= RCU_VKEY_UNLOCK;
}

/*!
    \brief      set voltage in deep sleep mode
    \param[in]  dsvol: deep sleep mode voltage
      \arg        RCU_DEEPSLEEP_V_1_2: the core voltage is 1.2V, only in GD32F130_150
      \arg        RCU_DEEPSLEEP_V_1_1: the core voltage is 1.1V, only in GD32F130_150
      \arg        RCU_DEEPSLEEP_V_1_0: the core voltage is 1.0V, only in GD32F130_150
      \arg        RCU_DEEPSLEEP_V_0_9: the core voltage is 0.9V, only in GD32F130_150

      \arg        RCU_DEEPSLEEP_V_1_8: the core voltage is 1.8V, only in GD32F170_190
      \arg        RCU_DEEPSLEEP_V_1_6: the core voltage is 1.6V, only in GD32F170_190
      \arg        RCU_DEEPSLEEP_V_1_4: the core voltage is 1.4V, only in GD32F170_190
      \arg        RCU_DEEPSLEEP_V_1_2: the core voltage is 1.2V, only in GD32F170_190
    \param[out] none
    \retval     none
*/
void rcu_deepsleep_voltage_set(uint32_t dsvol)
{
    /* reset the DSLPVS bits and set according to dsvol */
    RCU_DSV &= ~RCU_DSV_DSLPVS;
    RCU_DSV |= dsvol;
}

#ifdef GD32F130_150
/*!
    \brief      set the power down voltage
    \param[in]  pdvol: power down voltage select
      \arg        RCU_PDR_V_2_6: power down voltage is 2.6V
      \arg        RCU_PDR_V_1_8: power down voltage is 1.8V
    \param[out] none
    \retval     none
*/
void rcu_power_down_voltage_set(uint32_t pdvol)
{
    /* reset the PDRVS bits and set according to pdvol */
    RCU_PDVSEL &= ~RCU_PDVSEL_PDRVS;
    RCU_PDVSEL |= pdvol;
}
#endif /* GD32F130_150 */

/*!
    \brief      get the system clock, bus and peripheral clock frequency
    \param[in]  clock: the clock frequency which to get
      \arg        CK_SYS: system clock frequency
      \arg        CK_AHB: AHB clock frequency
      \arg        CK_APB1: APB1 clock frequency
      \arg        CK_APB2: APB2 clock frequency
      \arg        CK_ADC: ADC clock frequency
      \arg        CK_CEC: CEC clock frequency
      \arg        CK_USART: USART clock frequency
    \param[out] none
    \retval     clock frequency of system, AHB, APB1, APB2, ADC, CEC or USRAT
*/
uint32_t rcu_clock_freq_get(rcu_clock_freq_enum clock)
{
    uint32_t sws = 0, adcps = 0, ck_freq = 0;
    uint32_t cksys_freq = 0, ahb_freq = 0, apb1_freq = 0, apb2_freq = 0;
    uint32_t adc_freq = 0, cec_freq = 0, usart_freq = 0;
    uint32_t pllmf = 0, pllmf4 = 0, pllsel = 0, prediv = 0, idx = 0, exp = 0;
    /* exponent of AHB, APB1 and APB2 clock divider */
    const uint8_t ahb_exp[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
    const uint8_t apb1_exp[8] = {0, 0, 0, 0, 1, 2, 3, 4};
    const uint8_t apb2_exp[8] = {0, 0, 0, 0, 1, 2, 3, 4};

    sws = GET_BITS(RCU_CFG0, 2, 3);
    switch(sws){
    /* IRC8M is selected as CK_SYS */
    case SEL_IRC8M:
        cksys_freq = IRC8M_VALUE;
        break;
    /* HXTAL is selected as CK_SYS */
    case SEL_HXTAL:
        cksys_freq = HXTAL_VALUE;
        break;
    /* PLL is selected as CK_SYS */
    case SEL_PLL:
        /* get the value of PLLMF[3:0] */
        pllmf = GET_BITS(RCU_CFG0, 18, 21);
        pllmf4 = GET_BITS(RCU_CFG0, 27, 27);
        /* high 16 bits */
        if(1 == pllmf4){
            pllmf += 17;
        }else{
            pllmf += 2;
        }
        /* PLL clock source selection, HXTAL or IRC8M/2 */
        pllsel = GET_BITS(RCU_CFG0, 16, 16);
        if(0 != pllsel){
            prediv = (GET_BITS(RCU_CFG1,0, 3) + 1);
            cksys_freq = (HXTAL_VALUE / prediv) * pllmf;
        }else{
            cksys_freq = (IRC8M_VALUE >> 1) * pllmf;
        }
        break;
    /* IRC8M is selected as CK_SYS */
    default:
        cksys_freq = IRC8M_VALUE;
        break;
    }
    /* calculate AHB clock frequency */
    idx = GET_BITS(RCU_CFG0, 4, 7);
    exp = ahb_exp[idx];
    ahb_freq = cksys_freq >> exp;
    
    /* calculate APB1 clock frequency */
    idx = GET_BITS(RCU_CFG0, 8, 10);
    exp = apb1_exp[idx];
    apb1_freq = ahb_freq >> exp;
    
    /* calculate APB2 clock frequency */
    idx = GET_BITS(RCU_CFG0, 11, 13);
    exp = apb2_exp[idx];
    apb2_freq = ahb_freq >> exp;
    
    /* return the clocks frequency */
    switch(clock){
    case CK_SYS:
        ck_freq = cksys_freq;
        break;
    case CK_AHB:
        ck_freq = ahb_freq;
        break;
    case CK_APB1:
        ck_freq = apb1_freq;
        break;
    case CK_APB2:
        ck_freq = apb2_freq;
        break;
    case CK_ADC:
        /* calculate ADC clock frequency */
        if(RCU_ADCSRC_APB2DIV != (RCU_CFG2 & RCU_CFG2_ADCSEL)){
#ifdef GD32F130_150
            adc_freq = IRC14M_VALUE;
#elif defined (GD32F170_190)
            if(RCU_ADC_IRC28M_DIV1 != (RCU_CFG2 & RCU_CFG2_IRC28MDIV)){
                adc_freq = IRC28M_VALUE >> 1;
            }else{
                adc_freq = IRC28M_VALUE;
            }
#endif /* GD32F130_150 */
        }else{
            /* ADC clock select CK_APB2 divided by 2/4/6/8 */
            adcps = GET_BITS(RCU_CFG0, 14, 15);
            switch(adcps){
            case 0:
                adc_freq = apb2_freq / 2;
                break;
            case 1:
                adc_freq = apb2_freq / 4;
                break;
            case 2:
                adc_freq = apb2_freq / 6;
                break;
            case 3:
                adc_freq = apb2_freq / 8;
                break;
            default:
                break;
            }
        }
        ck_freq = adc_freq;
        break;
    case CK_CEC:
        /* calculate CEC clock frequency */
        if(RCU_CECSRC_LXTAL != (RCU_CFG2 & RCU_CFG2_CECSEL)){
            cec_freq = IRC8M_VALUE / 244;
        }else{
            cec_freq = LXTAL_VALUE;
        }
        ck_freq = cec_freq;
        break;
    case CK_USART:
        /* calculate USART clock frequency */
        if(RCU_USART0SRC_CKAPB2 == (RCU_CFG2 & RCU_CFG2_USART0SEL)){
            usart_freq = apb2_freq;
        }else if(RCU_USART0SRC_CKSYS == (RCU_CFG2 & RCU_CFG2_USART0SEL)){
            usart_freq = cksys_freq;
        }else if(RCU_USART0SRC_LXTAL == (RCU_CFG2 & RCU_CFG2_USART0SEL)){
            usart_freq = LXTAL_VALUE;
        }else if(RCU_USART0SRC_IRC8M == (RCU_CFG2 & RCU_CFG2_USART0SEL)){
            usart_freq = IRC8M_VALUE;
        }
        ck_freq = usart_freq;
        break;
    default:
        break;
    }
    return ck_freq;
}
