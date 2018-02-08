/*!
    \file  gd32f1x0_i2c.h
    \brief definitions for the i2c
*/

/*
    Copyright (C) 2016 GigaDevice

    2014-12-26, V1.0.0, firmware for GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, firmware for GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#ifndef GD32F1X0_I2C_H
#define GD32F1X0_I2C_H

#include "gd32f1x0.h"

/* I2Cx(x=0,1,2) definitions */
#define I2C0                          I2C_BASE
#define I2C1                          (I2C_BASE+(0x5800-0x5400))
#ifdef GD32F170_190
#define I2C2                          (I2C_BASE+(0xc000-0x5400))
#endif /* GD32F170_190 */

/* registers definitions */
#define I2C_CTL0(i2cx)                REG32((i2cx) + 0x00)      /*!< I2C control register 0 */
#define I2C_CTL1(i2cx)                REG32((i2cx) + 0x04)      /*!< I2C control register 1 */
#define I2C_SADDR0(i2cx)              REG32((i2cx) + 0x08)      /*!< I2C slave address register 0*/
#define I2C_SADDR1(i2cx)              REG32((i2cx) + 0x0C)      /*!< I2C slave address register */
#define I2C_DATA(i2cx)                REG32((i2cx) + 0x10)      /*!< I2C transfer buffer register */
#define I2C_STAT0(i2cx)               REG32((i2cx) + 0x14)      /*!< I2C transfer status register 0 */
#define I2C_STAT1(i2cx)               REG32((i2cx) + 0x18)      /*!< I2C transfer status register */
#define I2C_CKCFG(i2cx)               REG32((i2cx) + 0x1C)      /*!< I2C clock configure register */
#define I2C_RT(i2cx)                  REG32((i2cx) + 0x20)      /*!< I2C rise time register */
#ifdef GD32F170_190
#define I2C_SAMCS(i2cx)               REG32((i2cx) + 0x80)      /*!< I2C SAM control and status register */
#endif /* GD32F170_190 */

/* bits definitions */
/* I2Cx_CTL0 */
#define I2C_CTL0_I2CEN                BIT(0)        /*!< peripheral enable */
#define I2C_CTL0_SMBEN                BIT(1)        /*!< SMBus mode */
#define I2C_CTL0_SMBSEL               BIT(3)        /*!< SMBus type */
#define I2C_CTL0_ARPEN                BIT(4)        /*!< ARP enable */
#define I2C_CTL0_PECEN                BIT(5)        /*!< PEC enable */
#define I2C_CTL0_GCEN                 BIT(6)        /*!< general call enable */
#define I2C_CTL0_DISSTRC              BIT(7)        /*!< clock stretching disable (slave mode) */
#define I2C_CTL0_START                BIT(8)        /*!< start generation */
#define I2C_CTL0_STOP                 BIT(9)        /*!< stop generation */
#define I2C_CTL0_ACKEN                BIT(10)       /*!< acknowledge enable */
#define I2C_CTL0_POAP                 BIT(11)       /*!< acknowledge/PEC position (for data reception) */
#define I2C_CTL0_PECTRANS             BIT(12)       /*!< packet error checking */
#define I2C_CTL0_SALT                 BIT(13)       /*!< SMBus alert */
#define I2C_CTL0_SRESET               BIT(15)       /*!< software reset */

/* I2Cx_CTL1 */
#define I2C_CTL1_I2CCLK               BITS(0,5)     /*!< I2CCLK[5:0] bits (peripheral clock frequency) */
#define I2C_CTL1_ERRIE                BIT(8)        /*!< error interrupt inable */
#define I2C_CTL1_EVIE                 BIT(9)        /*!< event interrupt enable */
#define I2C_CTL1_BUFIE                BIT(10)       /*!< buffer interrupt enable */
#define I2C_CTL1_DMAON                BIT(11)       /*!< DMA requests enable */
#define I2C_CTL1_DMALST               BIT(12)       /*!< DMA last transfer */

/* I2Cx_SADDR0 */
#define I2C_SADDR0_ADDRESS0           BIT(0)        /*!< bit 0 of a 10-bit address */
#define I2C_SADDR0_ADDRESS            BITS(1,7)     /*!< 7-bit address or bits 7:1 of a 10-bit address */
#define I2C_SADDR0_ADDRESS_H          BITS(8,9)     /*!< highest two bits of a 10-bit address */
#define I2C_SADDR0_ADDFORMAT          BIT(15)       /*!< address mode for the I2C slave */

/* I2Cx_SADDR1 */
#define I2C_SADDR0_DUADEN             BIT(0)        /*!< aual-address mode switch */
#define I2C_SADDR0_ADDRESS2           BITS(1,7)     /*!< second I2C address for the slave in dual-address mode */

/* I2Cx_DATA */
#define I2C_DATA_TRB                  BITS(0,7)     /*!< 8-bit data register */

/* I2Cx_STAT0 */
#define I2C_STAT0_SBSEND              BIT(0)        /*!< start bit (master mode) */
#define I2C_STAT0_ADDSEND             BIT(1)        /*!< address sent (master mode)/matched (slave mode) */
#define I2C_STAT0_BTC                 BIT(2)        /*!< byte transfer finished */
#define I2C_STAT0_ADD10SEND           BIT(3)        /*!< 10-bit header sent (master mode) */
#define I2C_STAT0_STPDET              BIT(4)        /*!< stop detection (slave mode) */
#define I2C_STAT0_RBNE                BIT(6)        /*!< data register not empty (receivers) */
#define I2C_STAT0_TBE                 BIT(7)        /*!< data register empty (transmitters) */
#define I2C_STAT0_BERR                BIT(8)        /*!< bus error */
#define I2C_STAT0_LOSTARB             BIT(9)        /*!< arbitration lost (master mode) */
#define I2C_STAT0_AERR                BIT(10)       /*!< acknowledge failure */
#define I2C_STAT0_OUERR               BIT(11)       /*!< overrun/underrun */
#define I2C_STAT0_PECERR              BIT(12)       /*!< PEC error in reception */
#define I2C_STAT0_SMBTO               BIT(14)       /*!< timeout signal in SMBus mode */
#define I2C_STAT0_SMBALT              BIT(15)       /*!< SMBus alert status */

/* I2Cx_STAT1 */
#define I2C_STAT1_MASTER              BIT(0)        /*!< master/slave */
#define I2C_STAT1_I2CBSY              BIT(1)        /*!< bus busy */
#define I2C_STAT1_TRS                 BIT(2)        /*!< transmitter/receiver */
#define I2C_STAT1_RXGC                BIT(4)        /*!< general call address (slave mode) */
#define I2C_STAT1_DEFSMB              BIT(5)        /*!< SMBus device default address (slave mode) */
#define I2C_STAT1_HSTSMB              BIT(6)        /*!< SMBus host header (slave mode) */
#define I2C_STAT1_DUMODF              BIT(7)        /*!< dual flag (slave mode) */
#define I2C_STAT1_ECV                 BITS(8,15)    /*!< packet error checking register */

/* I2Cx_CKCFG */
#define I2C_CKCFG_CLKC                BITS(0,11)    /*!< clock control register in fast/standard mode (master mode) */
#define I2C_CKCFG_DTCY                BIT(14)       /*!< fast mode duty cycle */
#define I2C_CKCFG_FAST                BIT(15)       /*!< I2C speed selection in master mode */

/* I2Cx_RT */
#define I2C_RT_RISETIME               BITS(0,5)     /*!< maximum rise time in fast/standard mode (Master mode) */

#ifdef GD32F170_190
/* I2Cx_SAMCS */
#define I2C_SAMCS_SAMEN               BIT(0)        /*!< SAM_V interface enable */
#define I2C_SAMCS_STOEN               BIT(1)        /*!< SAM_V interface timeout detect enable */
#define I2C_SAMCS_TFFIE               BIT(4)        /*!< txframe fall interrupt enable */
#define I2C_SAMCS_TFRIE               BIT(5)        /*!< txframe rise interrupt enable */
#define I2C_SAMCS_RFFIE               BIT(6)        /*!< rxframe fall interrupt enable */
#define I2C_SAMCS_RFRIE               BIT(7)        /*!< rxframe rise interrupt enable */
#define I2C_SAMCS_TXF                 BIT(8)        /*!< level of txframe signal */
#define I2C_SAMCS_RXF                 BIT(9)        /*!< level of rxframe signal */
#define I2C_SAMCS_TFF                 BIT(12)       /*!< txframe fall flag, cleared by software write 0 */
#define I2C_SAMCS_TFR                 BIT(13)       /*!< txframe rise flag, cleared by software write 0 */
#define I2C_SAMCS_RFF                 BIT(14)       /*!< rxframe fall flag, cleared by software write 0 */
#define I2C_SAMCS_RFR                 BIT(15)       /*!< rxframe rise flag, cleared by software write 0 */
#endif /* GD32F170_190 */

/* constants definitions */
#define BIT_MASK(x)                   ((uint16_t)(~BIT(x)))

/* whether or not to send an ACK */
typedef enum {
     I2C_ACK_ENABLE,                                                                /*!< ACK will be sent */
     I2C_ACK_DISABLE                                                                /*!< ACK will be not sent */
}i2c_ack_enum;
/* I2C transfer direction */
#define I2C_TRANSMITTER               (~BIT(0))                                     /*!< transmitter */
#define I2C_RECEIVER                  BIT(0)                                        /*!< receiver */

/* SMBus/I2C mode switch and SMBus type selection */
typedef enum{
    I2C_SMBUS_DEVICE              = ((BIT(3) & ((0) << 3))|BIT(1)),                 /*!< SMBus mode device type */
    I2C_SMBUS_HOST                = (BIT(3)|BIT(1)),                                /*!< SMBus mode host type */
    I2C_I2CMODE_ENABLE            = ((BIT(3) & ((0) << 3))|(BIT(1) & ((0) << 1)))   /*!< I2C mode */
}i2c_mode_enum;

#define BITS_HIGH(regval)             (((uint32_t)regval) << 16)
/* I2C state */
typedef enum {
    /* I2C bit state */
    I2C_SBSEND                    =BIT(0),              /*!< start condition sent out in master mode */
    I2C_ADDSEND                   =BIT(1),              /*!< address is sent in master mode or received and matches in slave mode */
    I2C_BTC                       =BIT(2),              /*!< byte Transmission Finishes */
    I2C_ADD10SEND                 =BIT(3),              /*!< header of 10-bit address is sent in master mode */
    I2C_STPDET                    =BIT(4),              /*!< etop condition detected in slave mode */
    I2C_RBNE                      =BIT(6),              /*!< TRBR is not empty during receiving */
    I2C_TBE                       =BIT(7),              /*!< I2C_DATA is empty during transmitting */
    I2C_BERR                      =BIT(8),              /*!< a bus error occurs indication a unexpected start or stop condition on I2C bus */
    I2C_LOSTARB                   =BIT(9),              /*!< arbitration lost in master mode */
    I2C_AERR                      =BIT(10),             /*!< acknowledge Failure */
    I2C_OUERR                     =BIT(11),             /*!< over-run or under-run situation occurs in slave mode */
    I2C_PECERR                    =BIT(12),             /*!< PEC error when receiving data */
    I2C_SMBTO                     =BIT(14),             /*!< timeout signal in SMBus mode */
    I2C_SMBALTS                   =BIT(15),             /*!< SMBus alert status */
    I2C_MASTER                    =BITS_HIGH(BIT(0)),   /*!< a flag indicating whether I2C block is in master or slave mode */
    I2C_I2CBSY                    =BITS_HIGH(BIT(1)),   /*!< busy flag  */
    I2C_TRS                       =BITS_HIGH(BIT(2)),   /*!< whether the I2C is a transmitter or a receiver */
    I2C_RXGC                      =BITS_HIGH(BIT(4)),   /*!< general call address (00h) received */
    I2C_DEFSMB                    =BITS_HIGH(BIT(5)),   /*!< SMBus host header in slave mode */
    I2C_HSTSMB                    =BITS_HIGH(BIT(6)),   /*!< SMBus Host Header detected in slave mode */
    I2C_DUMODF                    =BITS_HIGH(BIT(7))    /*!< dual flag in slave mode indicating which address is matched in dual-address mode */
}i2c_state_enum;

/* I2C dual-address mode switch */
typedef enum {
    I2C_DUADEN_DISABLE             =0,                  /*!< dual-address mode disabled */
    I2C_DUADEN_ENABLE              =1                   /*!< dual-address mode enabled */
}i2c_dualaddr_enum;

/* I2C DMA mode configure */
typedef enum{
    /* DMA mode switch */
    I2C_DMA_ON,                                         /*!< DMA mode enabled */
    I2C_DMA_OFF,                                        /*!< DMA mode disabled */
    /* flag indicating DMA last transfer */
    I2C_DMALST_ON,                                      /*!< next DMA EOT is the last transfer */
    I2C_DMALST_OFF                                      /*!< next DMA EOT is not the last transfer */
}i2c_dmaconfig_enum;

/* I2C PEC configure */
typedef enum{
    /* PEC enable */
    I2C_PEC_ENABLE,                                     /*!< PEC calculation on */
    I2C_PEC_DISABLE,                                    /*!< PEC calculation off */
    /* position of PEC meaning */
    I2C_POAP_NEXT,                                      /*!< PEC bit indicates that PECTRANS is in the next byte */
    I2C_POAP_CURRENT,                                   /*!< PEC bit indicates that PECTRANS is in shift register */
    /* PEC transfer */
    I2C_PECTRANS_ENABLE,                                /*!< transfer PEC */
    I2C_PECTRANS_DISABLE                                /*!< not transfer PEC value */
}i2c_pecconfig_enum;

/* I2C POAP position*/
typedef enum{
    I2C_ACKPOS_CURRENT,                                 /*!< ACKEN bit decides whether to send ACK or not for the current */
    I2C_ACKPOS_NEXT                                     /*!< ACKEN bit decides whether to send ACK or not for the next byte */
}i2c_ackpos_enum;

/* software reset I2C */
typedef enum{
    I2C_SRESET_SET   =BIT(15),                          /*!< I2C is under reset */
    I2C_SRESET_RESET =BIT_MASK(15)                      /*!< I2C is not under reset */
}i2c_software_reset_enum;

/* I2C SMBus configure */
typedef enum{
    /* issue or not alert through SMBA pin */
    I2C_SALTSEND_ENABLE,                                /*!< issue alert through SMBA pin */
    I2C_SALTSEND_DISABLE,                               /*!< not issue alert through SMBA */
    /* ARP protocol in SMBus switch */
    I2C_ARP_ENABLE,                                     /*!< ARP is enabled */
    I2C_ARP_DISABLE                                     /*!< ARP is disabled */
}i2c_smbusconfig_enum;

/* whether or not to stretch SCL low */
typedef enum {
    I2C_SCLSTRETCH_ENABLE  =BIT_MASK(7),                /*!< SCL stretching is enabled */
    I2C_SCLSTRETCH_DISABLE =BIT(7)                      /*!< SCL stretching is disabled */
}i2c_stretchscl_enum;

/* whether or not to response to a General Call */
typedef enum {
    I2C_GCEN_ENABLE  =BIT(6),                           /*!< slave will response to a general Call */
    I2C_GCEN_DISABLE =BIT_MASK(6)                       /*!< slave will not response to a general Call */
}i2c_gcall_config_enum;

/* I2C duty cycle in fast mode */
#define CKCFG_DTCY(regval)            (BIT(14) & ((regval) << 14))
#define I2C_DTCY_2                    CKCFG_DTCY(0)                                 /*!< I2C fast mode Tlow/Thigh = 2 */
#define I2C_DTCY_16_9                 CKCFG_DTCY(1)                                 /*!< I2C fast mode Tlow/Thigh = 16/9 */

/* address mode for the I2C slave */
#define SADDR0_ADDFORMAT(regval)      (BIT(15) & ((regval) << 15))
#define I2C_ADDFORMAT_7BITS           SADDR0_ADDFORMAT(0)                           /*!< address:7 bits */
#define I2C_ADDFORMAT_10BITS          SADDR0_ADDFORMAT(1)                           /*!< address:10 bits */

/* function declarations */
/* reset I2C */
void i2c_deinit(uint32_t i2c_periph);
/* I2C clock configure */
void i2c_clock_config(uint32_t i2c_periph,uint32_t clkspeed,uint32_t dutycyc);
/* I2C address configure */
void i2c_addr_config(uint32_t i2c_periph,i2c_mode_enum i2cmod,uint32_t addformat,uint32_t addr);
/* whether or not to send an ACK */
void i2c_ack_config(uint32_t i2c_periph,i2c_ack_enum ack);
/* I2C POAP position configure */
void i2c_ackpos_config(uint32_t i2c_periph,i2c_ackpos_enum pos);
/* master send slave address */
void i2c_master_addressing(uint32_t i2c_periph,uint8_t addr,uint32_t trandirection);
/* dual-address mode switch */
void i2c_dualaddr_enable(uint32_t i2c_periph, i2c_dualaddr_enum dualaddr);

/* enable i2c */
void i2c_enable(uint32_t i2c_periph);
/* disable i2c */
void i2c_disable(uint32_t i2c_periph);
/* generate a START condition on I2C bus */
void i2c_start_on_bus(uint32_t i2c_periph);
/* generate a STOP condition on I2C bus */
void i2c_stop_on_bus(uint32_t i2c_periph);
/* i2c transmit data function */
void i2c_transmit_data(uint32_t i2c_periph,uint8_t data);
/* i2c receive data function */
uint8_t i2c_receive_data(uint32_t i2c_periph);
/* I2C DMA function configure */
void i2c_dma_config(uint32_t i2c_periph,i2c_dmaconfig_enum dmastste);
/* whether to stretch SCL low when data is not ready in slave mode  */
void i2c_stretch_scl_low_config(uint32_t i2c_periph,i2c_stretchscl_enum stretchpara );
/* whether or not to response to a general call  */
void i2c_slave_response_to_gcall_config(uint32_t i2c_periph,i2c_gcall_config_enum gcallpara );
/* software reset I2C  */
void i2c_software_reset_config(uint32_t i2c_periph, i2c_software_reset_enum sreset );

/* check i2c state */
FlagStatus i2c_flag_get(uint32_t i2c_periph,i2c_state_enum state );
/* clear i2c state */
void i2c_flag_clear(uint32_t i2c_periph,uint32_t state );
/* enable i2c interrupt */
void i2c_interrupt_enable(uint32_t i2c_periph,uint32_t inttype);
/* disable i2c interrupt */
void i2c_interrupt_disable(uint32_t i2c_periph,uint32_t inttype);

/* I2C PEC function configure */
void i2c_pec_config(uint32_t i2c_periph,i2c_pecconfig_enum pecpara);
/* packet error checking value  */
uint8_t i2c_pec_value(uint32_t i2c_periph);
/* I2C SMBus configure  */
void i2c_smbus_config(uint32_t i2c_periph,i2c_smbusconfig_enum smbuspara );

#ifdef GD32F170_190
/* enable SAM_V interface */
void i2c_sam_enable(uint32_t i2c_periph);
/* disable SAM_V interface */
void i2c_sam_disable(uint32_t i2c_periph);
/* enable SAM_V interface timeout detect */
void i2c_sam_timeout_enable(uint32_t i2c_periph);
/* disable SAM_V interface timeout detect */
void i2c_sam_timeout_disable(uint32_t i2c_periph);
/* enable the specified I2C SAM interrupt */
void i2c_sam_interrupt_enable(uint32_t i2c_periph,uint32_t inttype);
/* disable the specified I2C SAM interrupt */
void i2c_sam_interrupt_disable(uint32_t i2c_periph,uint32_t inttype);
/* check i2c SAM state */
FlagStatus i2c_sam_flag_get(uint32_t i2c_periph,uint32_t samstate);
/* clear i2c SAM state */
void i2c_sam_flag_clear(uint32_t i2c_periph,uint32_t samstate);
#endif /*GD32F170_190*/

#endif /* GD32F1X0_I2C_H */
