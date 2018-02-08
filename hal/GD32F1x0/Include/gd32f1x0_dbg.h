/*!
    \file  gd32f1x0_dbg.h
    \brief definitions for the dbg
*/

/*
    Copyright (C) 2016 GigaDevice

    2014-12-26, V1.0.0, firmware for GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, firmware for GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#ifndef GD32F1X0_DBG_H
#define GD32F1X0_DBG_H

#include "gd32f1x0.h"

/* DBG definitions */
#define DBG                      DBG_BASE

/* registers definitions */
#define DBG_ID                   REG32(DBG + 0x00) /*!< DBG_ID code register */
#define DBG_CTL0                 REG32(DBG + 0x04) /*!< DBG control register 0 */
#define DBG_CTL1                 REG32(DBG + 0x08) /*!< DBG control register 1 */

/* bits definitions */
/* DBG_ID */
#define DBG_ID_ID_CODE           BITS(0,31) /*!< DBG ID code values */

/* DBG_CTL0 */
#define DBG_CTL0_SLEEP_HOLD      BIT(0)  /*!< keep debugger connection during sleep mode */
#define DBG_CTL0_DEEPSLEEP_HOLD  BIT(1)  /*!< keep debugger connection during deepsleep mode */
#define DBG_CTL0_STDBY_HOLD      BIT(2)  /*!< keep debugger connection during standby mode */
#define DBG_CTL0_FWDGT_HOLD      BIT(8)  /*!< debug fwdgt kept when core is halted */
#define DBG_CTL0_WWDGT_HOLD      BIT(9)  /*!< debug wwdgt kept when core is halted */
#define DBG_CTL0_TM0_HOLD        BIT(10) /*!< timer0 counter kept when core is halted */
#define DBG_CTL0_TM1_HOLD        BIT(11) /*!< timer1 counter kept when core is halted */
#define DBG_CTL0_TM2_HOLD        BIT(12) /*!< timer2 counter kept when core is halted */
#define DBG_CTL0_I2C0_HOLD       BIT(15) /*!< hold i2c0 smbus when core is halted */
#define DBG_CTL0_I2C1_HOLD       BIT(16) /*!< hold i2c1 smbus when core is halted */
#define DBG_CTL0_I2C2_HOLD       BIT(17) /*!< hold i2c2 smbus when core is halted */

#ifdef GD32F170_190
#define DBG_CTL0_CAN0_HOLD       BIT(18) /*!< can0 counter kept when core is halted */
#endif /* GD32F170_190 */

#define DBG_CTL0_TM5_HOLD        BIT(19) /*!< hold timer5 counter when core is halted */

#ifdef GD32F170_190
#define DBG_CTL0_CAN1_HOLD       BIT(21) /*!< hold can1 counter when core is halted */
#endif /* GD32F170_190 */

#define DBG_CTL0_TM13_HOLD       BIT(27) /*!< hold timer13 counter when core is halted */

/* DBG_CTL1 */
#define DBG_CTL1_RTC_HOLD        BIT(10) /*!< hold rtc calendar and wakeup counter when core is halted */
#define DBG_CTL1_TM14_HOLD       BIT(16) /*!< hold timer14 counter when core is halted */
#define DBG_CTL1_TM15_HOLD       BIT(17) /*!< hold timer15 counter when core is halted */
#define DBG_CTL1_TM16_HOLD       BIT(18) /*!< hold timer16 counter when core is halted */

/* function declarations */
/* read DBG_ID code register */
uint32_t dbg_id_get(void);

/* enable DBG_CTL0 peripheral behavior when the MCU is in debug mode */
void dbg_ctl0_periph_enable(uint32_t dbg_ctl0_periph);
/* disable DBG_CTL0 peripheral behavior when the MCU is in debug mode */
void dbg_ctl0_periph_disable(uint32_t dbg_ctl0_periph);

/* enable DBG_CTL1 peripheral behavior when the MCU is in debug mode */
void dbg_ctl1_periph_enable(uint32_t dbg_ctl1_periph);
/* disable DBG_CTL1 peripheral behavior when the MCU is in debug mode */
void dbg_ctl1_periph_disable(uint32_t dbg_ctl1_periph);

#endif /* GD32F1X0_DBG_H */
