/*!
    \file  gd32f1x0_cec.h
    \brief definitions for the cec
*/

/*
    Copyright (C) 2016 GigaDevice

    2014-12-26, V1.0.0, firmware for GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, firmware for GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#ifndef GD32F1X0_CEC_H
#define GD32F1X0_CEC_H

#include "gd32f1x0.h"

/* CEC definitions */
#define CEC                                CEC_BASE          /*!< CEC base address */

/* registers definitions */
#define CEC_CTL                            REG32(CEC + 0x00) /*!< CEC control register */
#define CEC_CFG                            REG32(CEC + 0x04) /*!< CEC configuration register */
#define CEC_TDATA                          REG32(CEC + 0x08) /*!< CEC transmit data register */
#define CEC_RDATA                          REG32(CEC + 0x0C) /*!< CEC receive data register */
#define CEC_INTF                           REG32(CEC + 0x10) /*!< CEC interrupt flag Register */
#define CEC_INTEN                          REG32(CEC + 0x14) /*!< CEC interrupt enable register */

/* bits definitions */
/* CEC_CTL */
#define CEC_CTL_CECEN                      BIT(0)   /*!< enable or disable HDMI-CEC controller bit */
#define CEC_CTL_SOM                        BIT(1)   /*!< start of sending a message. */
#define CEC_CTL_ENDOM                      BIT(2)   /*!< EOM bit value in the next frame in TX mode */

/* CEC_CFG */
#define CEC_CFG_SFT                        BITS(0,2)   /*!< signal free time */
#define CEC_CFG_RTOL                       BIT(3)      /*!< reception bit timing tolerance */
#define CEC_CFG_RBRESTP                    BIT(4)      /*!< whether stop receive message when detected RBRE */
#define CEC_CFG_RBREGEN                    BIT(5)      /*!< generate Error-bit when detected RBRE in singlecast */
#define CEC_CFG_RLBPEGEN                   BIT(6)      /*!< generate Error-bit when detected RLBPE in singlecast */
#define CEC_CFG_BCNG                       BIT(7)      /*!< do not generate Error-bit in broadcast message */
#define CEC_CFG_SFTOPT                     BIT(8)      /*!< The SFT start option bit */
#define CEC_CFG_OWN_ADDRESS                BITS(16,30) /*!< own address */
#define CEC_CFG_LMEN                       BIT(31)     /*!< listen mode enable bit */

/* CEC_INTF */
#define CEC_INTF_RBR                       BIT(0)  /*!< RX-byte data received */
#define CEC_INTF_REND                      BIT(1)  /*!< end of reception */
#define CEC_INTF_RO                        BIT(2)  /*!< RX overrun */
#define CEC_INTF_RBRE                      BIT(3)  /*!< bit rising error */
#define CEC_INTF_RSBPE                     BIT(4)  /*!< short bit period error */
#define CEC_INTF_RLBPE                     BIT(5)  /*!< long bit period error */
#define CEC_INTF_RAE                       BIT(6)  /*!< RX ACK error */
#define CEC_INTF_LSTARB                    BIT(7)  /*!< arbitration lost */
#define CEC_INTF_TBR                       BIT(8)  /*!< TX-byte data request */
#define CEC_INTF_TEND                      BIT(9)  /*!< transmission successfully end */
#define CEC_INTF_TU                        BIT(10) /*!< TX data buffer underrun */
#define CEC_INTF_TERR                      BIT(11) /*!< TX-error */
#define CEC_INTF_TAERR                     BIT(12) /*!< TX ACK error flag */

/* CEC_INTEN */
#define CEC_INTEN_RBRIE                    BIT(0)  /*!< RBR interrupt enable */
#define CEC_INTEN_RENDIE                   BIT(1)  /*!< REND interrupt enable */
#define CEC_INTEN_ROIE                     BIT(2)  /*!< RO interrupt enable */
#define CEC_INTEN_RBREIE                   BIT(3)  /*!< RBRE interrupt enable. */
#define CEC_INTEN_RSBPEIE                  BIT(4)  /*!< RSBPE interrupt enable */
#define CEC_INTEN_RLBPEIE                  BIT(5)  /*!< RLBPE interrupt enable. */
#define CEC_INTEN_RAEIE                    BIT(6)  /*!< RAE interrupt enable */
#define CEC_INTEN_LSTARBIE                 BIT(7)  /*!< ALRLST interrupt enable */
#define CEC_INTEN_TBRIE                    BIT(8)  /*!< TBR interrupt enable */
#define CEC_INTEN_TENDIE                   BIT(9)  /*!< TEND interrupt enable */
#define CEC_INTEN_TUIE                     BIT(10) /*!< TU interrupt enable */
#define CEC_INTEN_TERRIE                   BIT(11) /*!< TE interrupt enable */
#define CEC_INTEN_TAERRIE                  BIT(12) /*!< TAE interrupt enable */

/* constants definitions */
/* signal free time */
#define CFG_SFT(regval)                    (BITS(0, 2) & ((regval) << 0)) /*!< signal free time */
#define CEC_SFT_PROTOCOL_PERIOD            CFG_SFT(0)  /*!< the signal free time will perform as HDMI-CEC protocol description */
#define CEC_SFT_1POINT5_PERIOD             CFG_SFT(1)  /*!< 1.5 nominal data bit periods */
#define CEC_SFT_2POINT5_PERIOD             CFG_SFT(2)  /*!< 2.5 nominal data bit periods */
#define CEC_SFT_3POINT5_PERIOD             CFG_SFT(3)  /*!< 3.5 nominal data bit periods */
#define CEC_SFT_4POINT5_PERIOD             CFG_SFT(4)  /*!< 4.5 nominal data bit periods */
#define CEC_SFT_5POINT5_PERIOD             CFG_SFT(5)  /*!< 5.5 nominal data bit periods */
#define CEC_SFT_6POINT5_PERIOD             CFG_SFT(6)  /*!< 6.5 nominal data bit periods */
#define CEC_SFT_7POINT5_PERIOD             CFG_SFT(7)  /*!< 7.5 nominal data bit periods */

/* signal free time start option */
#define CEC_SFT_START_SOM                  ((uint32_t)0x0)  /*!< signal free time counter starts counting when SOM is asserted */
#define CEC_SFT_START_LAST                 CEC_CFG_SFTOPT  /*!< signal free time counter starts automatically after transmission/reception end */

/* own address */
#define CEC_OWN_ADDRESS_CLEAR              ((uint32_t)0x0)  /*!< own address is cleared */
#define CEC_OWN_ADDRESS0                   BIT(16)  /*!< own address is 0 */
#define CEC_OWN_ADDRESS1                   BIT(17)  /*!< own address is 1 */
#define CEC_OWN_ADDRESS2                   BIT(18)  /*!< own address is 2 */
#define CEC_OWN_ADDRESS3                   BIT(19)  /*!< own address is 3 */
#define CEC_OWN_ADDRESS4                   BIT(20)  /*!< own address is 4 */
#define CEC_OWN_ADDRESS5                   BIT(21)  /*!< own address is 5 */
#define CEC_OWN_ADDRESS6                   BIT(22)  /*!< own address is 6 */
#define CEC_OWN_ADDRESS7                   BIT(23)  /*!< own address is 7 */
#define CEC_OWN_ADDRESS8                   BIT(24)  /*!< own address is 8 */
#define CEC_OWN_ADDRESS9                   BIT(25)  /*!< own address is 9 */
#define CEC_OWN_ADDRESS10                  BIT(26)  /*!< own address is 10 */
#define CEC_OWN_ADDRESS11                  BIT(27)  /*!< own address is 11 */
#define CEC_OWN_ADDRESS12                  BIT(28)  /*!< own address is 12 */
#define CEC_OWN_ADDRESS13                  BIT(29)  /*!< own address is 13 */
#define CEC_OWN_ADDRESS14                  BIT(30)  /*!< own address is 14 */

/* error-bit generate */
#define CEC_BROADCAST_ERROR_BIT_ON         ((uint32_t)0x00) /*!< generate Error-Bit in broadcast */
#define CEC_BROADCAST_ERROR_BIT_OFF        CEC_CFG_BCNG     /*!< do not generate Error-Bit in broadcast */
#define CEC_LONG_PERIOD_ERROR_BIT_OFF      ((uint32_t)0x00) /*!< generate Error-Bit on long bit period error */
#define CEC_LONG_PERIOD_ERROR_BIT_ON       CEC_CFG_RLBPEGEN /*!< do not generate Error-Bit on long bit period error */
#define CEC_RISING_PERIOD_ERROR_BIT_OFF    ((uint32_t)0x00) /*!< generate Error-Bit on bit rising error */ 
#define CEC_RISING_PERIOD_ERROR_BIT_ON     CEC_CFG_RBREGEN  /*!< do not generate Error-Bit on bit rising error */ 

/* whether stop receive message when detected bit rising error */
#define CEC_STOP_RISING_ERROR_BIT_ON       ((uint32_t)0x00)  /*!< stop reception when detected bit rising error */
#define CEC_STOP_RISING_ERROR_BIT_OFF      ((uint32_t)0x01)  /*!< do not stop reception when detected bit rising error */

/* clear interrupt flag */
#define CEC_INTF_RBRC                      BIT(0)  /*!< clear Rx-byte data received flag */
#define CEC_INTF_RENDC                     BIT(1)  /*!< clear end of reception flag */
#define CEC_INTF_ROC                       BIT(2)  /*!< clear RX overrun flag */
#define CEC_INTF_RBREC                     BIT(3)  /*!< clear bit rising error flag */
#define CEC_INTF_RSBPEC                    BIT(4)  /*!< clear short bit period error flag */
#define CEC_INTF_RLBPEC                    BIT(5)  /*!< clear long bit period error flag */
#define CEC_INTF_RAEC                      BIT(6)  /*!< clear Rx ACK error flag */
#define CEC_INTF_LSTARBC                   BIT(7)  /*!< clear arbitration lost flag */
#define CEC_INTF_TBRC                      BIT(8)  /*!< clear Tx-byte data request flag */
#define CEC_INTF_TENDC                     BIT(9)  /*!< clear transmission successfully end flag */
#define CEC_INTF_TUC                       BIT(10) /*!< clear Tx data buffer underrun flag */ 
#define CEC_INTF_TEC                       BIT(11) /*!< clear Tx-error flag */
#define CEC_INTF_TAEC                      BIT(12) /*!< clear Tx ACK error flag */


/* function declarations */
/* reset HDMI-CEC controller */
void cec_deinit(void);
/* configure signal free time,the signal free time counter start option,own address */
void cec_init(uint32_t sftmopt,uint32_t sft,uint32_t address);
/* configure generate Error-bit, whether stop receive message when detected bit rising error */
void cec_error_config(uint32_t broadcast,uint32_t singlecast_lbpe,uint32_t singlecast_bre,uint32_t rxbrestp);
/* enable HDMI-CEC controller */
void cec_enable(void);
/* disable HDMI-CEC controller */
void cec_disable(void);

/* start CEC message transmission */
void cec_transmission_start(void);
/* end CEC message transmission */
void cec_transmission_end(void);
/* enable CEC listen mode */
void cec_listen_mode_enable(void);
/* disable CEC listen mode */
void cec_listen_mode_disable(void);
/* configure and clear own address */
void cec_own_address_config(uint32_t address);
/* configure signal free time and the signal free time counter start option */
void cec_sft_config(uint32_t sftmopt,uint32_t sft);
/* configure generate Error-bit when detected some abnormal situation or not */
void cec_generate_errorbit_config(uint32_t broadcast,uint32_t singlecast_lbpe,uint32_t singlecast_bre);
/* whether stop receive message when detected bit rising error */
void cec_stop_receive_bre_config(uint32_t rxbrestp);
/* enable reception bit timing tolerance */
void cec_reception_tolerance_enable(void);
/* disable reception bit timing tolerance */
void cec_reception_tolerance_disable(void);
/* send a data by the CEC peripheral */
void cec_data_send(uint8_t data);
/* receive a data by the CEC peripheral */
uint8_t cec_data_receive(void);

/* clear CEC int flag and status */
FlagStatus cec_interrupt_flag_get(uint32_t flag);
/* clear CEC flag */
void cec_interrupt_flag_clear(uint32_t flag);
/* enable interrupt */
void cec_interrupt_enable(uint32_t flag);
/* disable interrupt */
void cec_interrupt_disable(uint32_t flag);
/* get CEC status */
FlagStatus cec_flag_get(uint32_t status);

#endif /* GD32F1X0_CEC_H */
