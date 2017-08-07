/*!
    \file  gd32f1x0_dbg.c
    \brief dbg driver
*/

/*
    Copyright (C) 2016 GigaDevice

    2014-12-26, V1.0.0, firmware for GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, firmware for GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#include "gd32f1x0_dbg.h"

/*!
    \brief      read DBG_ID code register
    \param[in]  none
    \param[out] none
    \retval     DBG_ID code
*/
uint32_t dbg_id_get(void)
{
    return DBG_ID;
}

/*!
    \brief      enable DBG_CTL0 peripheral behavior when the mcu is in debug mode
    \param[in]  dbg_ctl0_periph: DBG_CTL0 peripheral
                for gd32f130 or gd32f150 devices, this parameter can be any combination of 
                the following values:
      \arg        DBG_CTL0_SLEEP_HOLD: keep debugger connection during sleep mode
      \arg        DBG_CTL0_DEEPSLEEP_HOLD: keep debugger connection during deepsleep mode
      \arg        DBG_CTL0_STDBY_HOLD: keep debugger connection during standby mode
      \arg        DBG_CTL0_FWDGT_HOLD: debug fwdgt kept when core is halted
      \arg        DBG_CTL0_WWDGT_HOLD: debug wwdgt kept when core is halted
      \arg        DBG_CTL0_TM0_HOLD: timer0 counter kept when core is halted
      \arg        DBG_CTL0_TM1_HOLD: timer1 counter kept when core is halted
      \arg        DBG_CTL0_TM2_HOLD: timer2 counter kept when core is halted
      \arg        DBG_CTL0_I2C0_HOLD: hold i2c0 smbus when core is halted
      \arg        DBG_CTL0_I2C1_HOLD: hold i2c1 smbus when core is halted
      \arg        DBG_CTL0_I2C2_HOLD: hold i2c2 smbus when core is halted
      \arg        DBG_CTL0_TM5_HOLD: hold timer5 counter when core is halted
      \arg        DBG_CTL0_TM13_HOLD: hold timer13 counter when core is halted
              
                for  gd32f170 or gd32f190 devices, this parameter can be any combination of 
                the following values:
      \arg        DBG_CTL0_SLEEP_HOLD: keep debugger connection during sleep mode
      \arg        DBG_CTL0_DEEPSLEEP_HOLD: keep debugger connection during deepsleep mode
      \arg        DBG_CTL0_STDBY_HOLD: keep debugger connection during standby mode
      \arg        DBG_CTL0_FWDGT_HOLD: debug fwdgt kept when core is halted
      \arg        DBG_CTL0_WWDGT_HOLD: debug wwdgt kept when core is halted
      \arg        DBG_CTL0_TM0_HOLD: timer0 counter kept when core is halted
      \arg        DBG_CTL0_TM1_HOLD: timer1 counter kept when core is halted
      \arg        DBG_CTL0_TM2_HOLD: timer2 counter kept when core is halted
      \arg        DBG_CTL0_CAN0_HOLD: can0 counter kept when core is halted
      \arg        DBG_CTL0_I2C0_HOLD: hold i2c0 smbus when core is halted
      \arg        DBG_CTL0_I2C1_HOLD: hold i2c1 smbus when core is halted
      \arg        DBG_CTL0_I2C2_HOLD: hold i2c2 smbus when core is halted
      \arg        DBG_CTL0_TM5_HOLD: hold timer5 counter when core is halted
      \arg        DBG_CTL0_CAN1_HOLD: hold can1 counter when core is halted
      \arg        DBG_CTL0_TM13_HOLD: hold timer13 counter when core is halted
    \param[out] none
    \retval     none
*/
void dbg_ctl0_periph_enable(uint32_t dbg_ctl0_periph)
{
    DBG_CTL0 |= dbg_ctl0_periph;
}

/*!
    \brief      disable DBG_CTL0 peripheral behavior when the mcu is in debug mode
    \param[in]  dbg_ctl0_periph: DBG_CTL0 peripheral
                for gd32f130 or gd32f150 devices, this parameter can be any combination of 
                the following values:
        \arg      DBG_CTL0_SLEEP_HOLD: keep debugger disconnection during sleep mode
        \arg      DBG_CTL0_DEEPSLEEP_HOLD: keep debugger disconnection during deepsleep mode
        \arg      DBG_CTL0_STDBY_HOLD: keep debugger disconnection during standby mode
        \arg      DBG_CTL0_FWDGT_HOLD: debug fwdgt is not kept when core is halted
        \arg      DBG_CTL0_WWDGT_HOLD: debug wwdgt is not kept when core is halted
        \arg      DBG_CTL0_TM0_HOLD: timer0 counter is not kept when core is halted
        \arg      DBG_CTL0_TM1_HOLD: timer1 counter is not kept when core is halted
        \arg      DBG_CTL0_TM2_HOLD: timer2 counter is not kept when core is halted
        \arg      DBG_CTL0_I2C0_HOLD: donot hold i2c0 smbus when core is halted
        \arg      DBG_CTL0_I2C1_HOLD: donot hold i2c1 smbus when core is halted
        \arg      DBG_CTL0_I2C2_HOLD: donot hold i2c2 smbus when core is halted
        \arg      DBG_CTL0_TM5_HOLD: donot hold timer5 counter when core is halted
        \arg      DBG_CTL0_TM13_HOLD: donot hold timer13 counter when core is halted
    
                for gd32f170 or gd32f190 devices, this parameter can be any combination of
                the following values:
        \arg      DBG_CTL0_SLEEP_HOLD: keep debugger disconnection during sleep mode
        \arg      DBG_CTL0_DEEPSLEEP_HOLD: keep debugger disconnection during deepsleep mode
        \arg      DBG_CTL0_STDBY_HOLD: keep debugger disconnection during standby mode
        \arg      DBG_CTL0_FWDGT_HOLD: debug fwdgt is not kept when core is halted
        \arg      DBG_CTL0_WWDGT_HOLD: debug wwdgt is not kept when core is halted
        \arg      DBG_CTL0_TM0_HOLD: timer0 counter is not kept when core is halted
        \arg      DBG_CTL0_TM1_HOLD: timer1 counter is not kept when core is halted
        \arg      DBG_CTL0_TM2_HOLD: timer2 counter is not kept when core is halted
        \arg      DBG_CTL0_CAN0_HOLD: can0 counter is not kept when core is halted
        \arg      DBG_CTL0_I2C0_HOLD: donot hold i2c0 SMBUS when core is halted
        \arg      DBG_CTL0_I2C1_HOLD: donot hold i2c1 SMBUS when core is halted
        \arg      DBG_CTL0_I2C2_HOLD: donot hold i2c2 SMBUS when core is halted
        \arg      DBG_CTL0_TM5_HOLD: donot hold timer5 counter when core is halted
        \arg      DBG_CTL0_CAN1_HOLD: donot hold can1 counter when core is halted
        \arg      DBG_CTL0_TM13_HOLD: donot hold timer13 counter when core is halted
    \param[out] none
    \retval     none
*/
void dbg_ctl0_periph_disable(uint32_t dbg_ctl0_periph)
{
    DBG_CTL0 &= ~dbg_ctl0_periph;
}

/*!
    \brief      enable DBG_CTL1 peripheral behavior when the mcu is in debug mode
    \param[in]  dbg_ctl1_periph: DBG_CTL1 peripheral
      \arg        DBG_CTL1_RTC_HOLD: hold rtc calendar and wakeup counter when core is halted
      \arg        DBG_CTL1_TM14_HOLD: hold timer14 counter when core is halted
      \arg        DBG_CTL1_TM15_HOLD: hold timer15 counter when core is halted
      \arg        DBG_CTL1_TM16_HOLD: hold timer16 counter when core is halted
    \param[out] none
    \retval     none
*/
void dbg_ctl1_periph_enable(uint32_t dbg_ctl1_periph)
{
    DBG_CTL1 |= dbg_ctl1_periph;
}

/*!
    \brief      disable DBG_CTL1 peripheral behavior when the mcu is in debug mode
    \param[in]  dbg_ctl1_periph: DBG_CTL1 peripheral
      \arg      DBG_CTL1_RTC_HOLD: donot hold rtc calendar and wakeup counter when core is halted
      \arg      DBG_CTL1_TM14_HOLD: donot hold timer14 counter when core is halted
      \arg      DBG_CTL1_TM15_HOLD: donot hold timer15 counter when core is halted
      \arg      DBG_CTL1_TM16_HOLD: donot hold timer16 counter when core is halted
    \param[out] none
    \retval     none
*/
void dbg_ctl1_periph_disable(uint32_t dbg_ctl1_periph)
{
    DBG_CTL1 &= ~dbg_ctl1_periph;
}
