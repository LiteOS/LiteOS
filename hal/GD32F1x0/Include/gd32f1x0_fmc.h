/*!
    \file  gd32f1x0_fmc.h
    \brief definitions for the fmc
*/

/*
    Copyright (C) 2016 GigaDevice

    2014-12-26, V1.0.0, firmware for GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, firmware for GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#ifndef GD32F1X0_FMC_H
#define GD32F1X0_FMC_H

#include "gd32f1x0.h"

/* FMC and option byte definition */
#define FMC                     FMC_BASE                /*!< FMC register base address */
#define OB                      OB_BASE                 /*!< option byte base address */

/* registers definitions */
#define FMC_WS                  REG32((FMC) + 0x00)     /*!< FMC wait state register */
#define FMC_KEY                 REG32((FMC) + 0x04)     /*!< FMC unlock key register */
#define FMC_OBKEY               REG32((FMC) + 0x08)     /*!< FMC option byte unlock key register */
#define FMC_STAT                REG32((FMC) + 0x0C)     /*!< FMC option byte unlock key register */
#define FMC_CTL                 REG32((FMC) + 0x10)     /*!< FMC status register */
#define FMC_ADDR                REG32((FMC) + 0x14)     /*!< FMC address register */
#define FMC_OBSTAT              REG32((FMC) + 0x1C)     /*!< FMC option byte status register */
#define FMC_WP                  REG32((FMC) + 0x20)     /*!< FMC write protection register */
#define FMC_WSEN                REG32((FMC) + 0xFC)     /*!< FMC wait state enable register  */
#define FMC_PID                 REG32((FMC) + 0x100)    /*!< FMC product ID register */

#define OB_SPC                  REG16((OB) + 0x00)      /*!< option byte security protection value */
#define OB_USER                 REG16((OB) + 0x02)      /*!< option byte user value*/
#define OB_WP0                  REG16((OB) + 0x08)      /*!< option byte write protection 0 */
#define OB_WP1                  REG16((OB) + 0x0A)      /*!< option byte write protection 1 */

/* bits definitions */
/* FMC_WS */
#define FMC_WC_WSCNT0           BIT(0)                  /*!< one wait state added */
#define FMC_WC_WSCNT1           BIT(1)                  /*!< two wait state added */
#define FMC_WC_WSCNT            BITS(0,2)               /*!< wait state mask */

/* FMC_KEY */
#define FMC_KEY_UKEY            BITS(0,31)              /*!< FMC main flash key bits */

/* FMC_OBKEY */
#define FMC_OBKEY_OBKEY         BITS(0,31)              /*!< option byte key bits */

/* FMC_STAT */
#define FMC_STAT_BUSY           BIT(0)                  /*!< flash busy flag bit */
#define FMC_STAT_PGERR          BIT(2)                  /*!< flash program error flag bit */
#define FMC_STAT_WPERR          BIT(4)                  /*!< flash write protection error flag bit */
#define FMC_STAT_END            BIT(5)                  /*!< flash end of operation flag bit */

/* FMC_CTL */
#define FMC_CTL_PG              BIT(0)                  /*!< main flash program command bit */
#define FMC_CTL_PER             BIT(1)                  /*!< main flash page erase bit */
#define FMC_CTL_MER             BIT(2)                  /*!< main flash mass erase bit */
#define FMC_CTL_OBPG            BIT(4)                  /*!< option byte program command bit */
#define FMC_CTL_OBER            BIT(5)                  /*!< option byte erase command bit */
#define FMC_CTL_START           BIT(6)                  /*!< send erase command to FMC bit */
#define FMC_CTL_LK              BIT(7)                  /*!< flash lock bit */
#define FMC_CTL_OBWE            BIT(9)                  /*!< option byte erase/program enable bit */
#define FMC_CTL_ERRIE           BIT(10)                 /*!< error interrupt enable bit */
#define FMC_CTL_ENDIE           BIT(11)                 /*!< end of operation interrupt enable bit */
#define FMC_CTL_OBRLD           BIT(13)                 /*!< option byte reload bit */

/* FMC_ADDR */
#define FMC_ADDR_AR             BITS(0,31)              /*!< flash command address bits */

/* FMC_OBSTAT */
#define FMC_OBSTAT_OBER         BIT(0)                  /*!< option byte read error bit */
#define FMC_OBSTAT_PLEVEL0      BIT(1)                  /*!< protection bit 0 */
#define FMC_OBSTAT_PLEVEL1      BIT(2)                  /*!< protection bit 1 */
#define FMC_OBSTAT_USER         BITS(8,15)              /*!< option byte user bits */
#define FMC_OBSTAT_DATA         BITS(16,31)             /*!< option byte data bits */

/* FMC_WP */
#define FMC_WP_WP0              BIT(0)                  /*!< sector 0 write protection bit */
#define FMC_WP_WP1              BIT(1)                  /*!< sector 1 write protection bit */
#define FMC_WP_WP2              BIT(2)                  /*!< sector 2 write protection bit */
#define FMC_WP_WP3              BIT(3)                  /*!< sector 3 write protection bit */
#define FMC_WP_WP4              BIT(4)                  /*!< sector 4 write protection bit */
#define FMC_WP_WP5              BIT(5)                  /*!< sector 5 write protection bit */
#define FMC_WP_WP6              BIT(6)                  /*!< sector 6 write protection bit */
#define FMC_WP_WP7              BIT(7)                  /*!< sector 7 write protection bit */
#define FMC_WP_WP8              BIT(8)                  /*!< sector 8 write protection bit */
#define FMC_WP_WP9              BIT(9)                  /*!< sector 9 write protection bit */
#define FMC_WP_WP10             BIT(10)                 /*!< sector 10 write protection bit */
#define FMC_WP_WP11             BIT(11)                 /*!< sector 11 write protection bit */
#define FMC_WP_WP12             BIT(12)                 /*!< sector 12 write protection bit */
#define FMC_WP_WP13             BIT(13)                 /*!< sector 13 write protection bit */
#define FMC_WP_WP14             BIT(14)                 /*!< sector 14 write protection bit */
#define FMC_WP_WP15             BIT(15)                 /*!< sector 15 write protection bit */
#define FMC_WP_WPALL            BITS(0,15)              /*!< all sector write protection bit */

/* FMC_WSEN */
#define FMC_WSEN_WSEN           BIT(0)                  /*!< FMC wait state enable bit */
#ifdef GD32F170_190
#define FMC_WSEN_BPEN           BIT(1)                  /*!< FMC bit program enable bit */
#endif /* GD32F170_190 */

/* FMC_PID */
#define FMC_PID_PID             BITS(0,31)              /*!< product ID bits */

/* constants definitions */
/* fmc state */
typedef enum
{
    FMC_READY,
    FMC_BUSY,
    FMC_WPERR,
    FMC_PGERR,
    FMC_TOERR
}fmc_state_enum;

/* unlock key */
#define UNLOCK_KEY0             0x45670123              /*!< unlock key 0 */
#define UNLOCK_KEY1             0xCDEF89AB              /*!< unlock key 1 */

/* read protect configure */
#define FMC_NSPC                0xA5                    /*!< no security protection */
#define FMC_LSPC                0xBB                    /*!< low security protection */
#define FMC_HSPC                0xCC                    /*!< high security protection */

/* option byte write protection */
#define OB_LWP                  ((uint32_t)0x000000FF)  /*!< write protection low bits */
#define OB_HWP                  ((uint32_t)0x0000FF00)  /*!< write protection high bits */

/* option byte software/hardware free watch dog timer */
#define OB_FWDGT_SW             ((uint8_t)0x01)         /*!< software free watchdog */
#define OB_FWDGT_HW             ((uint8_t)0x00)         /*!< hardware free watchdog */

/* option byte reset or not entering deep sleep mode */
#define OB_DEEPSLEEP_NRST       ((uint8_t)0x02)         /*!< generate a reset instead of entering deepsleep mode */
#define OB_DEEPSLEEP_RST        ((uint8_t)0x00)         /*!< no reset when entering deepsleep mode */

/* option byte reset or not entering standby mode */
#define OB_STDBY_NRST           ((uint8_t)0x04)         /*!< generate a reset instead of entering standby mode */
#define OB_STDBY_RST            ((uint8_t)0x00)         /*!< no reset when entering deepsleep mode */

/* option byte boot reset/set */
#define OB_BOOT1_RESET          ((uint8_t)0x00)         /*!< BOOT1 bit is 1 */
#define OB_BOOT1_SET            ((uint8_t)0x10)         /*!< BOOT1 bit is 0 */

/* option byte VDDA monitor on/off */
#define OB_VDDA_ANALOG_ON       ((uint8_t)0x00)         /*!< disable VDDA monitor */
#define OB_VDDA_ANALOG_OFF      ((uint8_t)0x20)         /*!< enable VDDA monitor */

/* option byte SRAM parity enable/disable */
#define OB_SRAM_PARITY_SET      ((uint8_t)0x00)         /*!< disable sram parity check */
#define OB_SRAM_PARITY_RESET    ((uint8_t)0x40)         /*!< enable sram parity check */

/* option byte user mask */
#define OB_USER_MASK            ((uint8_t)0x00)         /*!< option byte user mask */

/* option byte data address */
#define OB_DATA_ADDR0           ((uint32_t)0x1FFFF804)  /*!< option byte data address 0 */
#define OB_DATA_ADDR1           ((uint32_t)0x1FFFF806)  /*!< option byte data address 1 */

/* FMC flags */
#define FMC_FLAG_BUSY           FMC_STAT_BUSY           /*!< FMC busy flag */
#define FMC_FLAG_PGERR          FMC_STAT_PGERR          /*!< FMC programming error flag */
#define FMC_FLAG_WPERR          FMC_STAT_WPERR          /*!< FMC write protection error flag */
#define FMC_FLAG_END            FMC_STAT_END            /*!< FMC end of programming flag */

/* FMC interrupt enable */
#define FMC_INTEN_EOP           FMC_CTL_ENDIE           /*!< enable FMC end of program interrupt */
#define FMC_INTEN_ERR           FMC_CTL_ERRIE           /*!< enable FMC error interrupt */

/* FMC time out */
#define FMC_TIMEOUT_COUNT       ((uint32_t)0x000F0000)  /*!< enable FMC error timeout */

/* function declarations */

/* FMC main memory programming functions */
/* unlock the main FMC operation */
void fmc_unlock(void);
/* lock the main FMC operation */
void fmc_lock(void);
/* FMC erase page */
fmc_state_enum fmc_page_erase(uint32_t page_address);
/* FMC erase whole chip */
fmc_state_enum fmc_mass_erase(void);
/* FMC program a word at the corresponding address */
fmc_state_enum fmc_word_program(uint32_t address, uint32_t data);
/* FMC program a half word at the corresponding address */
fmc_state_enum fmc_halfword_program(uint32_t address, uint16_t data);
#ifdef GD32F170_190
/* FMC program a word at the corresponding address without erasing */
fmc_state_enum fmc_word_reprogram(uint32_t address, uint32_t data);
#endif /* GD32F170_190 */

/* FMC option bytes programming functions */
/* unlock the option byte operation */
void ob_unlock(void);
/* lock the option byte operation */
void ob_lock(void);
/* generate a system reset to reload the option byte */
void ob_reset(void);
/* erase option byte */
fmc_state_enum ob_erase(void);
/* enable write protect */
fmc_state_enum ob_write_protection_enable(uint32_t ob_wrp);
/* configure read out protect */
fmc_state_enum ob_security_protection_config(uint8_t ob_rdp);
/* write the FMC option byte user */
fmc_state_enum ob_user_write(uint8_t ob_user);
/* write the FMC option byte data */
fmc_state_enum ob_data_program(uint32_t address, uint8_t data);
/* get the FMC option byte user */
uint8_t ob_user_get(void);
/* get the FMC option byte write protection */
uint32_t ob_write_protection_get(void);
/* get the FMC option byte read protection */
FlagStatus ob_security_protection_get(void);

/* FMC interrupts and flags management functions */
/* enable FMC interrupt */
void fmc_interrupt_enable(uint32_t fmc_int);
/* disable FMC interrupt */
void fmc_interrupt_disable(uint32_t fmc_int);
/* get flag set or reset */
FlagStatus fmc_flag_get(uint32_t fmc_flag);
/* clear the FMC pending flag */
void fmc_flag_clear(uint32_t fmc_flag);
/* return the FMC state */
fmc_state_enum fmc_state_get(void);
/* check FMC ready or not */
fmc_state_enum fmc_ready_wait(uint32_t count);

#endif /* GD32F1X0_FMC_H */
