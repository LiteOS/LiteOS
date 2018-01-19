/*!
    \file  gd32f1x0_usart.h
    \brief definitions for the usart
*/

/*
    Copyright (C) 2016 GigaDevice

    2014-12-26, V1.0.0, firmware for GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, firmware for GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#ifndef GD32F1X0_USART_H
#define GD32F1X0_USART_H

#include "gd32f1x0.h"

/* USARTx(x=0,1) definitions */
#define USART0                        (USART_BASE+0x0000F400)
#define USART1                        USART_BASE

/* registers definitions */
#define USART_CTL0(usartx)            REG32((usartx) + 0x00)        /*!< USART control register 0 */
#define USART_CTL1(usartx)            REG32((usartx) + 0x04)        /*!< USART control register 1 */
#define USART_CTL2(usartx)            REG32((usartx) + 0x08)        /*!< USART control register 2 */
#define USART_BAUD(usartx)            REG32((usartx) + 0x0C)        /*!< USART baud rate register */
#define USART_GP(usartx)              REG32((usartx) + 0x10)        /*!< USART guard time and prescaler register */
#define USART_RT(usartx)              REG32((usartx) + 0x14)        /*!< USART receiver timeoutregister */
#define USART_CMD(usartx)             REG32((usartx) + 0x18)        /*!< USART command register*/
#define USART_STAT(usartx)            REG32((usartx) + 0x1C)        /*!< USART status register*/
#define USART_INTC(usartx)            REG32((usartx) + 0x20)        /*!< USART status clear register*/
#define USART_RDATA(usartx)           REG32((usartx) + 0x24)        /*!< USART receive data register*/
#define USART_TDATA(usartx)           REG32((usartx) + 0x28)        /*!< USART transmit data register*/

/* bits definitions */
/* USARTx_CTL0 */
#define USART_CTL0_UEN                BIT(0)       /*!< USART enable */
#define USART_CTL0_UESM               BIT(1)       /*!< USART enable in deep-sleep mode */
#define USART_CTL0_REN                BIT(2)       /*!< receiver enable */
#define USART_CTL0_TEN                BIT(3)       /*!< transmitter enable */
#define USART_CTL0_IDLEIE             BIT(4)       /*!< idle line detected interrupt enable */
#define USART_CTL0_RBNEIE             BIT(5)       /*!< read data buffer not empty interrupt and overrun error interrupt enable */
#define USART_CTL0_TCIE               BIT(6)       /*!< transmission complete interrupt enable */
#define USART_CTL0_TBEIE              BIT(7)       /*!< transmitter register empty interrupt enable */
#define USART_CTL0_PERRIE             BIT(8)       /*!< parity error interrupt enable */
#define USART_CTL0_PM                 BIT(9)       /*!< parity mode */
#define USART_CTL0_PCEN               BIT(10)      /*!< parity control enable */
#define USART_CTL0_WM                 BIT(11)      /*!< wakeup method in mute mode */
#define USART_CTL0_WL                 BIT(12)      /*!< word length */
#define USART_CTL0_MEN                BIT(13)      /*!< mute mode enable */
#define USART_CTL0_AMIE               BIT(14)      /*!< addr match interrupt enable */
#define USART_CTL0_OVSMOD             BIT(15)      /*!< oversample mode */
#define USART_CTL0_DED                BITS(16,20)  /*!< driver Enable deassertion time */
#define USART_CTL0_DEA                BITS(21,26)  /*!< driver Enable assertion time */
#define USART_CTL0_RTIE               BIT(26)      /*!< receiver timeout interrupt enable */
#define USART_CTL0_EBIE               BIT(27)      /*!< end of Block interrupt enable */

/* USARTx_CTL1 */
#define USART_CTL1_ADDM               BIT(4)       /*!< address detection mode */
#define USART_CTL1_LBLEN              BIT(5)       /*!< LIN break detection length */
#define USART_CTL1_LBDIE              BIT(6)       /*!< LIN break detection interrupt enable */
#define USART_CTL1_CLEN               BIT(8)       /*!< last bit clock pulse */
#define USART_CTL1_CPH                BIT(9)       /*!< clock phase */
#define USART_CTL1_CPL                BIT(10)      /*!< clock polarity */
#define USART_CTL1_CKEN               BIT(11)      /*!< ck pin enable */
#define USART_CTL1_STB                BITS(12,13)  /*!< stop bits length */
#define USART_CTL1_LMEN               BIT(14)      /*!< LIN mode enable */
#define USART_CTL1_STRP               BIT(15)      /*!< swap TX/RX pins */
#define USART_CTL1_RINV               BIT(16)      /*!< RX pin level inversion */
#define USART_CTL1_TINV               BIT(17)      /*!< TX pin level inversion */
#define USART_CTL1_DINV               BIT(18)      /*!< data bit level inversion */
#define USART_CTL1_MSBF               BIT(19)      /*!< most significant bit first */
#define USART_CTL1_ABDEN              BIT(20)      /*!< auto baud rate enable */
#define USART_CTL1_ABDM               BITS(21,22)  /*!< auto baud rate mode */
#define USART_CTL1_RTEN               BIT(23)      /*!< receiver timeout enable */
#define USART_CTL1_ADDR               BITS(24,31)  /*!< address of the USART terminal */

/* USARTx_CTL2  */
#define USART_CTL2_ERRIE              BIT(0)       /*!< error interrupt enable in multibuffer Communication */
#define USART_CTL2_IREN               BIT(1)       /*!< IrDA mode enable */
#define USART_CTL2_IRLP               BIT(2)       /*!< IrDA low-power */
#define USART_CTL2_HDEN               BIT(3)       /*!< half-duplex enable */
#define USART_CTL2_NKEN               BIT(4)       /*!< NACK enable in smartcard mode */
#define USART_CTL2_SCEN               BIT(5)       /*!< smartcard mode enable */
#define USART_CTL2_DENR               BIT(6)       /*!< DMA enable for reception */
#define USART_CTL2_DENT               BIT(7)       /*!< DMA enable for transmission */
#define USART_CTL2_RTSEN              BIT(8)       /*!< RTS enable */
#define USART_CTL2_CTSEN              BIT(9)       /*!< CTS enable */
#define USART_CTL2_CTSIE              BIT(10)      /*!< CTS interrupt enable */
#define USART_CTL2_OSB                BIT(11)      /*!< one sample bit mode */
#define USART_CTL2_OVRD               BIT(12)      /*!< overrun disable */
#define USART_CTL2_DDRE               BIT(13)      /*!< disable DMA on reception error */
#define USART_CTL2_DEM                BIT(14)      /*!< driver enable mode */
#define USART_CTL2_DEP                BIT(15)      /*!< driver enable polarity mode */
#define USART_CTL2_SCRTNUM            BITS(17,19)  /*!< smartcard auto-retry number */
#define USART_CTL2_WUM                BITS(20,21)  /*!< wakeup mode from Deep-sleep mode */
#define USART_CTL2_WUIE               BIT(22)      /*!< wakeup from deep-sleep mode interrupt enable */

/* USARTx_BAUD */
#define USART_BAUD_FRADIV             BITS(0,3)    /*!< fraction of baud-rate divider */
#define USART_BAUD_INTDIV             BITS(4,15)   /*!< integer of baud-rate divider */

/* USARTx_GP */
#define USART_GP_PSC                  BIT(0,7)     /*!< prescaler value for dividing the system clock */
#define USART_GP_GUAT                 BITS(8,15)   /*!< guard time value in smartcard mode */

/* USARTx_RT */
#define USART_RT_RT                   BITS(0,23)   /*!< receiver timeout threshold */
#define USART_RT_BL                   BITS(24,31)  /*!< block Length */

/* USARTx_CMD */
#define USART_CMD_ABDCMD              BIT(0)       /*!< auto baudrate detection command */
#define USART_CMD_SBKCMD              BIT(1)       /*!< send break command */
#define USART_CMD_MMCMD               BIT(2)       /*!< mute mode command */
#define USART_CMD_RXFCMD              BIT(3)       /*!< receive data flush command */
#define USART_CMD_TXFCMD              BIT(4)       /*!< transmit data flush request */

/* USARTx_STAT */
#define USART_STAT_PERR               BIT(0)       /*!< parity error */
#define USART_STAT_FERR               BIT(1)       /*!< framing error */
#define USART_STAT_NERR               BIT(2)       /*!< noise error flag */
#define USART_STAT_ORERR              BIT(3)       /*!< overrun error */
#define USART_STAT_IDLEF              BIT(4)       /*!< idle line detected flag */
#define USART_STAT_RBNE               BIT(5)       /*!< read data buffer not empty */
#define USART_STAT_TC                 BIT(6)       /*!< transmission completed */
#define USART_STAT_TBE                BIT(7)       /*!< transmit data register empty */
#define USART_STAT_LBDF               BIT(8)       /*!< LIN break detected flag */
#define USART_STAT_CTSF               BIT(9)       /*!< CTS change flag */
#define USART_STAT_CTS                BIT(10)      /*!< CTS level */
#define USART_STAT_RTF                BIT(11)      /*!< receiver timeout flag */
#define USART_STAT_EBF                BIT(12)      /*!< end of block flag */
#define USART_STAT_ABDE               BIT(14)      /*!< auto baudrate detection error */
#define USART_STAT_ABDF               BIT(15)      /*!< auto baudrate detection flag */
#define USART_STAT_BSY                BIT(16)      /*!< busy flag */
#define USART_STAT_AMF                BIT(17)      /*!< address match flag */
#define USART_STAT_SBF                BIT(18)      /*!< send break flag */
#define USART_STAT_RWU                BIT(19)      /*!< receiver wakeup from mute mode */
#define USART_STAT_WUF                BIT(20)      /*!< wakeup from deep-sleep mode flag */
#define USART_STAT_TEA                BIT(21)      /*!< transmit enable acknowledge flag */
#define USART_STAT_REA                BIT(22)      /*!< receive enable acknowledge flag */

/* USARTx_INTC */
#define USART_INC_PEC                 BIT(0)       /*!< parity error clear */
#define USART_INC_FEC                 BIT(1)       /*!< framing error clear */
#define USART_INC_NEC                 BIT(2)       /*!< noise detected clear */
#define USART_INC_OREC                BIT(3)       /*!< overrun error clear */
#define USART_INC_IDLEC               BIT(4)       /*!< idle line detected clear */
#define USART_INC_TCC                 BIT(6)       /*!< transmission complete clear */
#define USART_INC_LBDC                BIT(8)       /*!< LIN break detected clear */
#define USART_INC_CTSC                BIT(9)       /*!< CTS change clear */
#define USART_INC_RTC                 BIT(11)      /*!< receiver timeout clear */
#define USART_INC_EBC                 BIT(12)      /*!< end of timeout clear  */
#define USART_INC_AMC                 BIT(17)      /*!< address match clear */
#define USART_INC_WUC                 BIT(20)      /*!< wakeup from deep-sleep mode clear  */

/* USARTx_RDATA */
#define USART_RDATA_RDATA             BITS(0,8)    /*!< receive data value */

/* USARTx_TDATA */
#define USART_TDATA_TDATA             BITS(0,8)    /*!< transmit data value */

/* constants definitions */
/* USART hardware flow control configure */
typedef enum{
    USART_CTSRTS_NONE,                                                 /*!< no hardware flow control */
    USART_RTS_ENABLE,                                                  /*!< RTS enable */
    USART_CTS_ENABLE,                                                  /*!< CTS enable */
    USART_RTS_DISABLE,                                                 /*!< RTS disable */
    USART_CTS_DISABLE,                                                 /*!< CTS disable */
    USART_CTSRTS_ENABLE                                                /*!< RTS,CTS enable */
}usart_hardware_flow_enum;

/* USART DMA transfer config */
typedef enum {
    USART_DMART_DISABLE,                                               /*!< DMA disable */
    USART_DENR_ENABLE,                                                 /*!< DMA enable for reception */
    USART_DENT_ENABLE,                                                 /*!< DMA enable for transmission */
    USART_DENR_DISABLE,                                                /*!< DMA enable for reception */
    USART_DENT_DISABLE,                                                /*!< DMA enable for transmission */
    USART_DMART_ENABLE,                                                /*!< DMA enable for transmission and reception */
}usart_dma_transfer_enum;

/* USART transfer config */
typedef enum {
    USART_RTEN_DISABLE,                                                /*!< transmit function and receiver function disable */
    USART_REN_ENABLE,                                                  /*!< enable for reception */
    USART_TEN_ENABLE,                                                  /*!< enable for transmission */
    USART_REN_DISABLE,                                                 /*!< disable for reception */
    USART_TEN_DISABLE,                                                 /*!< disable for transmission */
    USART_RTEN_ENABLE                                                  /*!< enable for transmission and reception */
}usart_transfer_enum;

/* USART invert configure */
typedef enum {
    /* data bit level inversion */
    USART_DINV_ENABLE,                                                 /*!< data bit level inversion */
    USART_DINV_DISABLE,                                                /*!< data bit level not inversion */
    /* TX pin level inversion */
    USART_TXPIN_ENABLE,                                                /*!< TX pin level inversion */               
    USART_TXPIN_DISABLE,                                               /*!< TX pin level not inversion */
    /* RX pin level inversion */
    USART_RXPIN_ENABLE,                                                /*!< RX pin level inversion */
    USART_RXPIN_DISABLE,                                               /*!< RX pin level not inversion */
    /* swap TX/RX pins */
    USART_SWAP_ENABLE,                                                 /*!< swap TX/RX pins */                
    USART_SWAP_DISABLE,                                                /*!< not swap TX/RX pins */
}usart_invert_enum;

/* USART control register index definitions */
#define USART_CTL0_INDEX              0x00                             /*!< USART control register 0 */
#define USART_CTL1_INDEX              0x01                             /*!< USART control register 1 */
#define USART_CTL2_INDEX              0x02                             /*!< USART control register 1 */

/* USART world length definitions */
#define CTL0_WL(regval)               (BIT(12) & (regval << 12))
#define USART_WL_8BIT                 CTL0_WL(0)                       /*!< 8 bits */
#define USART_WL_9BIT                 CTL0_WL(1)                       /*!< 9 bits */

/* USART stop bits definitions */
#define CTL1_STB(regval)              (BITS(12,13) & (regval << 12))
#define USART_STB_1BIT                CTL1_STB(0)                      /*!< 1 bit */
#define USART_STB_2BIT                CTL1_STB(2)                      /*!< 2 bit */
#define USART_STB_1_5BIT              CTL1_STB(3)                      /*!< 1.5 bit */

/* USART parity bits definitions */
#define CTL0_PM(regval)               (BITS(9,10) & (regval << 9))
#define USART_PM_NONE                 CTL0_PM(0)                       /*!< no parity */
#define USART_PM_ODD                  CTL0_PM(2)                       /*!< odd parity */
#define USART_PM_EVEN                 CTL0_PM(3)                       /*!< even parity */

/* USART auto baud rate detection mode bits definitions */
#define CTL1_ABDM(regval)             (BITS(21,22) & (regval << 21))
#define USART_ABDM_FTOR               CTL1_ABDM(0)                     /*!< falling edge to rising edge measurement */
#define USART_ABDM_FT0F               CTL1_ABDM(1)                     /*!< falling edge to falling edge measurement */

/* USART wakeup method in mute mode */
#define CTL0_WM(regval)               (BIT(11) & (regval << 11))
#define USART_WM_IDLE                 CTL0_WM(0)                       /*!< idle Line */
#define USART_WM_ADDR                 CTL0_WM(1)                       /*!< address mark */

/* USART address detection mode */
#define CTL1_ADDM(regval)             (BIT(4) & (regval << 4))
#define USART_ADDM_4B                 CTL1_ADDM(0)                     /*!< 4 bits */
#define USART_ADDM_7B                 CTL1_ADDM(1)                     /*!< 7 bits */

/* USART data is transmitted/received with the LSB/MSB first */
#define CTL1_MSBF(regval)             (BIT(19) & (regval << 19))
#define USART_MSBF_LSB                CTL1_MSBF(0)                     /*!< LSB first */
#define USART_MSBF_MSB                CTL1_MSBF(1)                     /*!< MSB first */

/* USART LIN break detection length */
#define CTL1_LBLEN(regval)            (BIT(5) & (regval << 5))
#define USART_LBLEN_10B               CTL1_LBLEN(0)                     /*!< 10 bits break detection */
#define USART_LBLEN_11B               CTL1_LBLEN(1)                     /*!< 11 bits break detection */

/* USART last bit clock pulse */
#define CTL1_CLEN(regval)             (BIT(8) & (regval << 8))
#define USART_CLEN_NONE               CTL1_CLEN(0)                     /*!< clock pulse of the last data bit (MSB) is not output to the CK pin */
#define USART_CLEN_EN                 CTL1_CLEN(1)                     /*!< clock pulse of the last data bit (MSB) is output to the CK pin */

/* USART clock phase */
#define CTL1_CPH(regval)              (BIT(9) & (regval << 9))
#define USART_CPH_1CK                 CTL1_CPH(0)                      /*!< first clock transition is the first data capture edge */
#define USART_CPH_2CK                 CTL1_CPH(1)                      /*!< second clock transition is the first data capture edge */

/* USART clock polarity */
#define CTL1_CPL(regval)              (BIT(10) & (regval << 10))
#define USART_CPL_LOW                 CTL1_CPL(0)                      /*!< steady low value on CK pin */
#define USART_CPL_HIGH                CTL1_CPL(1)                      /*!< steady high value on CK pin */

/* USART driver enable polarity mode */
#define CTL2_DEP(regval)              (BIT(15) & (regval << 15))
#define USART_DEP_HIGH                CTL2_DEP(0)                      /*!< DE signal is active high */
#define USART_DEP_LOW                 CTL2_DEP(1)                      /*!< DE signal is active low */

/* USART sameple rate */
#define CTL0_OVSMOD(regval)           (BIT(15) & (regval << 15))
#define USART_OVSMOD_8                CTL0_OVSMOD(1)                       /*!< 8 bits */
#define USART_OVSMOD_16               CTL0_OVSMOD(0)                       /*!< 16 bits */

/* USART one sample bit method configure */
#define CTL2_OSB(regval)              (BIT(11) & ((regval) << 11))
#define USART_OSB_1bit                CTL2_OSB(1)                     /*!< 1 bit */
#define USART_OSB_3bit                CTL2_OSB(0)                     /*!< 3 bits */

/* USART IrDA low-power enable */
#define CTL2_IRLP(regval)             (BIT(2) & ((regval) << 2))
#define USART_IRLP_LOW                CTL2_IRLP(1)                     /*!< low-power */
#define USART_IRLP_NORMAL             CTL2_IRLP(0)                     /*!< normal */

/* USART wakeup mode from deep-sleep mode */
#define CTL2_WUM(regval)              (BITS(20,21) & (regval << 20))
#define USART_WUM_ADDR                CTL2_WUM(0)                      /*!< WUF active on address match  */
#define USART_WUM_STARTB              CTL2_WUM(2)                      /*!< WUF active on start bit */
#define USART_WUM_RBNE                CTL2_WUM(3)                      /*!< WUF active on RBNE */

/* USART interrupt definitions */
#define REG_INDEX(regval)             (BITS(28,29) & (regval << 28))
#define USART_INT_IDLEIE              (USART_CTL0_IDLEIE|REG_INDEX(0))   /*!< IDLE line detected interrupt enable */
#define USART_INT_RBNEIE              (USART_CTL0_RBNEIE|REG_INDEX(0)) /*!< read data buffer not empty interrupt and overrun error interrupt enable */
#define USART_INT_TCIE                (USART_CTL0_TCIE|REG_INDEX(0))   /*!< transmission complete interrupt enable */
#define USART_INT_TBEIE               (USART_CTL0_TBEIE|REG_INDEX(0))  /*!< transmitter register empty interrupt enable */
#define USART_INT_PERRIE              (USART_CTL0_PERRIE|REG_INDEX(0))   /*!< parity error interrupt enable */
#define USART_INT_AMIE                (USART_CTL0_AMIE|REG_INDEX(0))   /*!< addr match interrupt enable */
#define USART_INT_RTIE                (USART_CTL0_RTIE|REG_INDEX(0))   /*!< receiver timeout interrupt enable */
#define USART_INT_EBIE                (USART_CTL0_EBIE|REG_INDEX(0))   /*!< end of block interrupt enable */
#define USART_INT_LBDIE               (USART_CTL1_LBDIE|REG_INDEX(1))  /*!< LIN break detection interrupt enable */
#define USART_INT_ERRIE               (USART_CTL2_ERRIE|REG_INDEX(2))   /*!< error interrupt enable in multibuffer communication */
#define USART_INT_CTSIE               (USART_CTL2_CTSIE|REG_INDEX(2))  /*!< CTS interrupt enable */
#define USART_INT_WUIE                (USART_CTL2_WUIE|REG_INDEX(2))   /*!< wakeup from deep-sleep mode interrupt enable */

/* function declarations */
/* reset USART */
void usart_deinit(uint32_t usart_periph);
/* configure usart baud rate value */
void usart_baudrate_set(uint32_t usart_periph,uint32_t baudval);
/* configure usart parity function */
void usart_parity_config(uint32_t usart_periph,uint32_t paritycfg);
/* configure usart word length */
void usart_word_length_set(uint32_t usart_periph,uint32_t wlen);
/* configure usart stop bit length */
void usart_stop_bit_set(uint32_t usart_periph,uint32_t stblen);
/* enable usart */
void usart_enable(uint32_t usart_periph);
/* disable usart */
void usart_disable(uint32_t usart_periph);
/* USART transfer config */
void usart_transfer_config(uint32_t usart_periph,usart_transfer_enum rtconfig);
/* USART transmit data function */
void usart_data_transmit(uint32_t usart_periph,uint32_t data);
/* USART receive data function */
uint16_t usart_data_receive(uint32_t usart_periph);
/* data is transmitted/received with the LSB/MSB first */
void usart_data_first_config(uint32_t usart_periph,uint32_t msbf);
/* USART inverted configure*/
void usart_invert_config(uint32_t usart_periph,usart_invert_enum invertpara);
/* overrun functionality is enabled */
void usart_overrun_enable(uint32_t usart_periph);
/* overrun functionality is disabled */
void usart_overrun_disable(uint32_t usart_periph);    
/* configure the USART oversample mode  */
void usart_oversample_config(uint32_t usart_periph,uint32_t oversamp);
/* sample bit method configure */
void usart_sample_bit_config(uint32_t usart_periph,uint32_t osb);

/* auto baud rate detection */
/* auto baud rate detection enable */
void usart_autobaud_detection_enable(uint32_t usart_periph);
/* auto baud rate detection disable */
void usart_autobaud_detection_disable(uint32_t usart_periph);
/* auto baud rate detection mode configure */
void usart_autobaud_detection_mode_config(uint32_t usart_periph,uint32_t abdmod);

/* multi-processor communication */
/* mute mode enable */
void usart_mute_mode_enable(uint32_t usart_periph);
/* mute mode disable */
void usart_mute_mode_disable(uint32_t usart_periph);
/* wakeup method in mute mode configure */
void usart_mute_mode_wakeup_config(uint32_t usart_periph,uint32_t wmehtod);
/* address detection mode configure */
void usart_address_detection_mode_config(uint32_t usart_periph,uint32_t addmod);
/* address of the USART terminal */
void usart_address_config(uint32_t usart_periph,uint8_t usartaddr);
/* receiver timeout enable */
void usart_receiver_timeout_enable(uint32_t usart_periph);
/* receiver timeout disable */
void usart_receiver_timeout_disable(uint32_t usart_periph);
/* receiver timeout threshold configure */
void usart_receiver_timeout_config(uint32_t usart_periph,uint32_t rtimeout);

/* LIN mode enable */
void usart_lin_mode_enable(uint32_t usart_periph);
/* LIN mode disable */
void usart_lin_mode_disable(uint32_t usart_periph);
/* LIN break detection length */
void usart_lin_break_dection_length_config(uint32_t usart_periph,uint32_t lblen);

/* half-duplex enable */
void usart_halfduplex_enable(uint32_t usart_periph);
/* half-duplex disable */
void usart_halfduplex_disable(uint32_t usart_periph);

/* clock enable */
void usart_clock_enable(uint32_t usart_periph);
/* clock disable*/
void usart_clock_disable(uint32_t usart_periph);
/* configure usart synchronous mode parameters */
void usart_synchronous_clock_config(uint32_t usart_periph,uint32_t clen,uint32_t cph,uint32_t cpl);

/* smartcard mode enable */
void usart_smartcard_mode_enable(uint32_t usart_periph);
/* smartcard mode disable */
void usart_smartcard_mode_disable(uint32_t usart_periph);
/* NACK disable in smartcard mode */
void usart_smartcard_mode_nack_enable(uint32_t usart_periph);
/* NACK disable in smartcard mode */
void usart_smartcard_mode_nack_disable(uint32_t usart_periph);
/* guard time value configure in smartcard mode */
void usart_guard_time_config(uint32_t usart_periph,uint32_t gaut);
/* block length configure */
void usart_block_length_config(uint32_t usart_periph,uint32_t bl);
/* smartcard auto-retry number configure */
void usart_smartcard_autoretry_config(uint32_t usart_periph,uint32_t SCRTNUM);

/* IrDA mode enable */
void usart_irda_mode_enable(uint32_t usart_periph);
/* IrDA mode disable */
void usart_irda_mode_disable(uint32_t usart_periph);
/* IrDA low-power configure */
void usart_irda_lowpower_config(uint32_t usart_periph,uint32_t irlp);

/* hardware flow control configure */
void usart_hardware_flow_config(uint32_t usart_periph,usart_hardware_flow_enum hardwareflow);
/* RS485 driver enable  */
void usart_rs485_driver_enable(uint32_t usart_periph);
/* RS485 driver disable  */
void usart_rs485_driver_disable(uint32_t usart_periph);
/* driver enable assertion time configure */
void usart_driver_assertime_config(uint32_t usart_periph,uint32_t deatime);
/* driver enable de-assertion time configure */
void usart_driver_deassertime_config(uint32_t usart_periph,uint32_t dedtime);
/* configure driver enable polarity mode */
void usart_depolarity_config(uint32_t usart_periph,uint32_t dep);

/* USART DMA */
/* DMA transfer config */
void usart_dma_transfer_config(uint32_t usart_periph,usart_dma_transfer_enum dmacmd);
/* disable DMA on reception error */
void usart_reception_error_dma_disable(uint32_t usart_periph);
/* not disable DMA on reception error */
void usart_reception_error_dma_enable(uint32_t usart_periph);

/* USART able to wake up the mcu from deep-sleep mode */
void usart_wakeup_enable(uint32_t usart_periph);
/* USART not able to wake up the mcu from deep-sleep mode */
void usart_wakeup_disable(uint32_t usart_periph);
/* wakeup mode from deep-sleep mode */
void usart_wakeup_mode_config(uint32_t usart_periph,uint32_t wum);
/* enable usart interrupt */
void usart_interrupt_enable(uint32_t usart_periph,uint32_t inttype);
/* disable usart interrupt */
void usart_interrupt_disable(uint32_t usart_periph,uint32_t inttype);
/* enable usart command */
void usart_command_enable(uint32_t usart_periph,uint32_t cmdtype);
/* check usart status*/
FlagStatus usart_flag_get(uint32_t usart_periph,uint32_t status);
/* clear usart status */
void usart_flag_clear(uint32_t usart_periph,uint32_t status);
/* get usart interrupt status */
FlagStatus usart_interrupt_flag_get(uint32_t usart_periph,uint32_t status,uint32_t inttype);
#endif /* GD32F1X0_USART_H */
