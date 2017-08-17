/*!
    \file  gd32f1x0_pmu.c
    \brief PMU driver
*/

/*
    Copyright (C) 2016 GigaDevice

    2014-12-26, V1.0.0, firmware for GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, firmware for GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#include "gd32f1x0_pmu.h"
#include "core_cm3.h"
/*!
    \brief      reset PMU register
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_deinit(void)
{
    /* reset PMU */
    rcu_periph_reset_enable(RCU_PMURST);
    rcu_periph_reset_disable(RCU_PMURST);
}

/*!
    \brief      select low voltage detector threshold
    \param[in]  pmu_lvdt_n:
      \arg        PMU_LVDT_0: voltage threshold is 2.2V (GD32F130_150) or 2.4V (GD32F170_190)
      \arg        PMU_LVDT_1: voltage threshold is 2.3V (GD32F130_150) or 2.7V (GD32F170_190)
      \arg        PMU_LVDT_2: voltage threshold is 2.4V (GD32F130_150) or 3.0V (GD32F170_190)
      \arg        PMU_LVDT_3: voltage threshold is 2.5V (GD32F130_150) or 3.3V (GD32F170_190)
      \arg        PMU_LVDT_4: voltage threshold is 2.6V (GD32F130_150) or 3.6V (GD32F170_190)
      \arg        PMU_LVDT_5: voltage threshold is 2.7V (GD32F130_150) or 3.9V (GD32F170_190)
      \arg        PMU_LVDT_6: voltage threshold is 2.8V (GD32F130_150) or 4.2V (GD32F170_190)
      \arg        PMU_LVDT_7: voltage threshold is 2.9V (GD32F130_150) or 4.5V (GD32F170_190)
    \param[out] none
    \retval     none
*/
void pmu_lvd_select(uint32_t pmu_lvdt_n)
{
    /* disable LVD */
    PMU_CTL &= ~PMU_CTL_LVDEN;
    /* clear LVDT bits */
    PMU_CTL &= ~PMU_CTL_LVDT;
    /* set LVDT bits according to pmu_lvdt_n */
    PMU_CTL |= pmu_lvdt_n;
    /* enable LVD */
    PMU_CTL |= PMU_CTL_LVDEN;

}

/*!
    \brief      PMU lvd disable
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_lvd_disable(void)
{
    /* disable LVD */
    PMU_CTL &= ~PMU_CTL_LVDEN;
}

/*!
    \brief      PMU work at sleep mode
    \param[in]  sleepmodecmd:
      \arg        WFI_CMD:  use WFI command
      \arg        WFE_CMD:  use WFE command
    \param[out] none
    \retval     none
*/
void pmu_to_sleepmode(uint8_t sleepmodecmd)
{
    /* clear sleepdeep bit of Cortex-M3 system control register */
    SCB->SCR &= ~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);
    
    /* select WFI or WFE command to enter sleep mode */
    if(WFI_CMD == sleepmodecmd){
        __WFI();
    }else{
        __WFE();
    }
}

/*!
    \brief      PMU work at deepsleep mode
    \param[in]  pmu_ldo
      \arg        PMU_LDO_NORMAL: LDO normal work when pmu enter deepsleep mode
      \arg        PMU_LDO_LOWPOWER: LDO work at low power mode when pmu enter deepsleep mode
    \param[in]  deepsleepmodecmd: 
      \arg        WFI_CMD: use WFI command
      \arg        WFE_CMD: use WFE command
    \param[out] none
    \retval     none
*/
void pmu_to_deepsleepmode(uint32_t pmu_ldo,uint8_t deepsleepmodecmd)
{
    /* clear stbmod and ldolp bits */
    PMU_CTL &= ~((uint32_t)(PMU_CTL_STBMOD | PMU_CTL_LDOLP));
    
    /* set ldolp bit according to pmu_ldo */
    PMU_CTL |= pmu_ldo;
    
    /* set sleepdeep bit of Cortex-M3 system control register */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    
    /* select WFI or WFE command to enter deepsleep mode */
    if(WFI_CMD == deepsleepmodecmd){
        __WFI();
    }else{
        __SEV();
        __WFE();
        __WFE();
    }
    /* reset sleepdeep bit of Cortex-M3 system control register */
    SCB->SCR &= ~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);
}

/*!
    \brief      pmu work at standby mode
    \param[in]  standbymodecmd:
      \arg        WFI_CMD: use WFI command
      \arg        WFE_CMD: use WFE command
    \param[out] none
    \retval     none
*/
void pmu_to_stdbymode(uint8_t standbymodecmd)
{
    /* set sleepdeep bit of Cortex-M3 system control register */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    /* set stbmod bit */
    PMU_CTL |= PMU_CTL_STBMOD;
        
    /* reset wakeup flag */
    PMU_CTL |= PMU_CTL_WURST;
    
    /* select WFI or WFE command to enter standby mode */
    if(WFI_CMD == standbymodecmd){
        __WFI();
    }else{
        __WFE();
    }
}

/*!
    \brief      reset flag bit
    \param[in]  flag_reset:
      \arg        PMU_FLAG_RESET_WAKEUP: reset wakeup flag
      \arg        PMU_FLAG_RESET_STANDBY: reset standby flag
    \param[out] none
    \retval     none
*/
void pmu_flag_reset(uint32_t flag_reset)
{
    switch(flag_reset){
    case PMU_FLAG_RESET_WAKEUP:
        /* reset wakeup flag */
        PMU_CTL |= PMU_CTL_WURST;
        break;
    case PMU_FLAG_RESET_STANDBY:
        /* reset standby flag */
        PMU_CTL |= PMU_CTL_STBRST;
        break;
    default :
        break;
    }
}

/*!
    \brief      get flag status
    \param[in]  pmu_flag:         
      \arg        PMU_FLAG_WAKEUP: wakeup flag status
      \arg        PMU_FLAG_STANDBY: standby flag status
      \arg        PMU_FLAG_LVD: lvd flag status
    \param[out] none
    \retval     FlagStatus SET or RESET
*/
FlagStatus pmu_flag_get(uint32_t pmu_flag )
{
    if(PMU_CS & pmu_flag)
        return  SET;
    return  RESET;
}

/*!
    \brief      backup domain write enable
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_backup_write_enable(void)
{
    PMU_CTL |= PMU_CTL_BKPWEN;
}

/*!
    \brief      backup domain write disable
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_backup_write_disable(void)
{
    PMU_CTL &= ~PMU_CTL_BKPWEN;
}

/*!
    \brief      wakeup pin enable
    \param[in]  wakeup_pin:
      \arg        PMU_WAKEUP_PIN0: wakeup pin 0
      \arg        PMU_WAKEUP_PIN1: wakeup pin 1
    \param[out] none
    \retval     none
*/
void pmu_wakeup_pin_enable(uint32_t wakeup_pin )
{
    PMU_CS |= wakeup_pin;
}

/*!
    \brief      wakeup pin disable
    \param[in]  wakeup_pin:
      \arg        PMU_WAKEUP_PIN0: wakeup pin 0
      \arg        PMU_WAKEUP_PIN1: wakeup pin 1
    \param[out] none
    \retval     none
*/
void pmu_wakeup_pin_disable(uint32_t wakeup_pin )
{
    PMU_CS &= ~wakeup_pin;

}
