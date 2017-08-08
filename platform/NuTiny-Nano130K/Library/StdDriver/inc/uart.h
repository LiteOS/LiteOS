/**************************************************************************//**
 * @file     uart.h
 * @version  V1.00
 * $Revision: 9 $
 * $Date: 15/06/26 1:36p $
 * @brief    Nano100 Series uart control header file.
 *
 * @note
 * Copyright (C) 2013 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/

#ifndef __UART_H__
#define __UART_H__

#ifdef __cplusplus
extern "C"
{
#endif


/** @addtogroup NANO100_Device_Driver NANO100 Device Driver
  @{
*/

/** @addtogroup NANO100_UART_Driver UART Driver
  @{
*/

/** @addtogroup NANO100_UART_EXPORTED_CONSTANTS UART Exported Constants
  @{
*/


/*---------------------------------------------------------------------------------------------------------*/
/* UA_LCR constants definitions                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
#define UART_WORD_LEN_5        (0) /*!< UART_TLCTL setting to set UART word length to 5 bits */
#define UART_WORD_LEN_6        (1) /*!< UART_TLCTL setting to set UART word length to 6 bits */
#define UART_WORD_LEN_7        (2) /*!< UART_TLCTL setting to set UART word length to 7 bits */
#define UART_WORD_LEN_8        (3) /*!< UART_TLCTL setting to set UART word length to 8 bits */

#define UART_PARITY_NONE    (0x0 << UART_TLCTL_PBE_Pos) /*!< UART_TLCTL setting to set UART as no parity   */
#define UART_PARITY_ODD     (0x1 << UART_TLCTL_PBE_Pos) /*!< UART_TLCTL setting to set UART as odd parity  */
#define UART_PARITY_EVEN    (0x3 << UART_TLCTL_PBE_Pos) /*!< UART_TLCTL setting to set UART as even parity */
#define UART_PARITY_MARK    (0x5 << UART_TLCTL_PBE_Pos) /*!< UART_TLCTL setting to keep parity bit as '1'  */
#define UART_PARITY_SPACE   (0x7 << UART_TLCTL_PBE_Pos) /*!< UART_TLCTL setting to keep parity bit as '0'  */

#define UART_STOP_BIT_1     (0x0 << UART_TLCTL_NSB_Pos) /*!< UART_TLCTL setting for one stop bit  */
#define UART_STOP_BIT_1_5   (0x1 << UART_TLCTL_NSB_Pos) /*!< UART_TLCTL setting for 1.5 stop bit when 5-bit word length  */
#define UART_STOP_BIT_2     (0x1 << UART_TLCTL_NSB_Pos) /*!< UART_TLCTL setting for two stop bit when 6, 7, 8-bit word length */

#define UART_TLCTL_RFITL_1BYTE        (0x0 << UART_TLCTL_RFITL_Pos)   /*!< UART_TLCTL setting to set RX FIFO Trigger Level to 1 bit */
#define UART_TLCTL_RFITL_4BYTES       (0x1 << UART_TLCTL_RFITL_Pos)   /*!< UART_TLCTL setting to set RX FIFO Trigger Level to 4 bits */
#define UART_TLCTL_RFITL_8BYTES       (0x2 << UART_TLCTL_RFITL_Pos)   /*!< UART_TLCTL setting to set RX FIFO Trigger Level to 8 bits */
#define UART_TLCTL_RFITL_14BYTES      (0x3 << UART_TLCTL_RFITL_Pos)   /*!< UART_TLCTL setting to set RX FIFO Trigger Level to 14 bits */

#define UART_TLCTL_RTS_TRI_LEV_1BYTE        (0x0 << UART_TLCTL_RTS_TRI_LEV_Pos)  /*!< UART_TLCTL setting to set RTS Trigger Level to 1 bit */
#define UART_TLCTL_RTS_TRI_LEV_4BYTES       (0x1 << UART_TLCTL_RTS_TRI_LEV_Pos)  /*!< UART_TLCTL setting to set RTS Trigger Level to 4 bits */
#define UART_TLCTL_RTS_TRI_LEV_8BYTES       (0x2 << UART_TLCTL_RTS_TRI_LEV_Pos)  /*!< UART_TLCTL setting to set RTS Trigger Level to 8 bits */
#define UART_TLCTL_RTS_TRI_LEV_14BYTES      (0x3 << UART_TLCTL_RTS_TRI_LEV_Pos)  /*!< UART_TLCTL setting to set RTS Trigger Level to 14 bits */


/*---------------------------------------------------------------------------------------------------------*/
/* UART RTS LEVEL TRIGGER constants definitions                                                            */
/*---------------------------------------------------------------------------------------------------------*/
#define UART_RTS_IS_HIGH_LEV_TRG    (0x1 << UART_MCSR_LEV_RTS_Pos) /*!< Set RTS is High Level Trigger   */
#define UART_RTS_IS_LOW_LEV_TRG     (0x0 << UART_MCSR_LEV_RTS_Pos) /*!< Set RTS is Low Level Trigger    */

/*---------------------------------------------------------------------------------------------------------*/
/* UA_FUNC_SEL constants definitions                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#define UART_FUNC_SEL_UART    (0x0 << UART_FUN_SEL_FUN_SEL_Pos) /*!< UART_FUN_SEL setting to set UART Function  (Default) */
#define UART_FUNC_SEL_LIN     (0x1 << UART_FUN_SEL_FUN_SEL_Pos) /*!< UART_FUN_SEL setting to set LIN Function             */
#define UART_FUNC_SEL_IrDA    (0x2 << UART_FUN_SEL_FUN_SEL_Pos) /*!< UART_FUN_SEL setting to set IrDA Function            */
#define UART_FUNC_SEL_RS485   (0x3 << UART_FUN_SEL_FUN_SEL_Pos) /*!< UART_FUN_SEL setting to set RS485 Function           */


/*@}*/ /* end of group NANO100_UART_EXPORTED_CONSTANTS */


/** @addtogroup NANO100_UART_EXPORTED_FUNCTIONS UART Exported Functions
  @{
*/

/**
 *    @brief    Calculate UART baudrate mode0 divider
 *
 *    @param    None
 *
 *    @return    UART baudrate mode0 register setting value
 *
 */
#define UART_BAUD_MODE0        (0)

/**
 *    @brief    Calculate UART baudrate mode0 divider
 *
 *    @param    None
 *
 *    @return    UART baudrate mode1 register setting value
 *
 */
#define UART_BAUD_MODE1        (UART_BAUD_DIV_16_EN_Msk)


/**
 *    @brief    Calculate UART baudrate mode0 divider
 *
 *    @param[in]    u32SrcFreq      UART clock frequency
 *    @param[in]    u32BaudRate     Baudrate of UART module
 *
 *    @return    UART baudrate mode1 divider
 *
 */
#define UART_BAUD_MODE1_DIVIDER(u32SrcFreq, u32BaudRate)    (((u32SrcFreq + (u32BaudRate*8)) / u32BaudRate >> 4)-1)

/**
 *    @brief    Calculate UART baudrate mode2 divider
 *
 *    @param[in]    u32SrcFreq    UART clock frequency
 *    @param[in]    u32BaudRate    Baudrate of UART module
 *
 *    @return    UART baudrate mode0 divider
 */
#define UART_BAUD_MODE0_DIVIDER(u32SrcFreq, u32BaudRate)    (((u32SrcFreq + (u32BaudRate/2)) / u32BaudRate)-1)


/**
 *    @brief    Write Data to Tx data register
 *
 *    @param[in]    uart        The base address of UART module.
 *    @param[in]    u8Data  Data byte to transmit
 *
 *    @return    None
 */
#define UART_WRITE(uart, u8Data)    (uart->THR = (u8Data))

/**
 *    @brief    Read Rx data register
 *
 *    @param[in]    uart        The base address of UART module.
 *
 *    @return    The oldest data byte in RX FIFO
 */
#define UART_READ(uart)    (uart->RBR)


/**
 *    @brief    Get Tx empty register value.
 *
 *    @param[in]    uart        The base address of UART module
 *
 *    @return    Tx empty register value.
 */
#define UART_GET_TX_EMPTY(uart)    (uart->FSR & UART_FSR_TX_EMPTY_F_Msk)


/**
 *    @brief    Get Rx empty register value.
 *
 *    @param[in]    uart        The base address of UART module
 *
 *    @return    Rx empty register value.
 */
#define UART_GET_RX_EMPTY(uart)    (uart->FSR & UART_FSR_RX_EMPTY_F_Msk)

/**
 *    @brief    Check specified uart port transmission is over.
 *
 *    @param[in]    uart        The base address of UART module
 *
 *    @return    TE_Flag.
 */
#define UART_IS_TX_EMPTY(uart)    ((uart->FSR & UART_FSR_TE_F_Msk) >> UART_FSR_TE_F_Pos)


/**
 *    @brief    Wait specified uart port transmission is over
 *
 *    @param[in]    uart        The base address of UART module
 *
 *    @return    None
 */
#define UART_WAIT_TX_EMPTY(uart)    while(!(((uart->FSR) & UART_FSR_TX_EMPTY_F_Msk) >> UART_FSR_TX_EMPTY_F_Pos))

/**
 *    @brief    Check RDA_IF is set or not
 *
 *    @param[in]    uart        The base address of UART module
 *
 *    @return
 *            0 : The number of bytes in the RX FIFO is less than the RFITL
 *            1 : The number of bytes in the RX FIFO equals or larger than RFITL
 */
#define UART_IS_RX_READY(uart)    ((uart->ISR & UART_ISR_RDA_IS_Msk)>>UART_ISR_RDA_IS_Pos)


/**
 *    @brief    Check TX FIFO is full or not
 *
 *    @param[in]    uart        The base address of UART module
 *
 *    @return
 *            1 = TX FIFO is full
 *            0 = TX FIFO is not full
 */
#define UART_IS_TX_FULL(uart)    ((uart->FSR & UART_FSR_TX_FULL_F_Msk)>>UART_FSR_TX_FULL_F_Pos)

/**
 *    @brief    Check RX FIFO is full or not
 *
 *    @param[in]    uart        The base address of UART module
 *
 *    @return
 *            1 = RX FIFO is full
 *            0 = RX FIFO is not full
 *
 */
#define UART_IS_RX_FULL(uart)    ((uart->FSR & UART_FSR_RX_FULL_F_Msk)>>UART_FSR_RX_FULL_F_Pos)


/**
 *    @brief    Get Tx full register value
 *
 *    @param[in]    uart        The base address of UART module
 *
 *    @return    Tx full register value
 */
#define UART_GET_TX_FULL(uart)    (uart->FSR & UART_FSR_TX_FULL_F_Msk)


/**
 *    @brief    Get Rx full register value
 *
 *    @param[in]    uart        The base address of UART module
 *
 *    @return    Rx full register value
 */
#define UART_GET_RX_FULL(uart)    (uart->FSR & UART_FSR_RX_FULL_F_Msk)


/**
 *    @brief    Enable specified interrupt
 *
 *    @param[in]    uart        The base address of UART module
 *    @param[in]    u32eIntSel    Interrupt type select
 *                        - \ref UART_IER_LIN_IE_Msk : LIN interrupt
 *                        - \ref UART_IER_ABAUD_IE_Msk : Auto baudrate interrupt
 *                        - \ref UART_IER_WAKE_IE_Msk : Wakeup interrupt
 *                        - \ref UART_IER_BUF_ERR_IE_Msk : Buffer Error interrupt
 *                        - \ref UART_IER_RTO_IE_Msk : Rx time-out interrupt
 *                        - \ref UART_IER_MODEM_IE_Msk : Modem interrupt
 *                        - \ref UART_IER_RLS_IE_Msk : Rx Line status interrupt
 *                        - \ref UART_IER_THRE_IE_Msk : Tx empty interrupt
 *                        - \ref UART_IER_RDA_IE_Msk : Rx ready interrupt
 *
 *    @return    None
 */
#define UART_ENABLE_INT(uart, u32eIntSel)    (uart->IER |= (u32eIntSel))


/**
 *    @brief    Disable specified interrupt
 *
 *    @param[in]    uart        The base address of UART module
 *    @param[in]    u32eIntSel    Interrupt type select
 *                        - \ref UART_IER_LIN_IE_Msk : LIN interrupt
 *                        - \ref UART_IER_ABAUD_IE_Msk : Auto baudrate interrupt
 *                        - \ref UART_IER_WAKE_IE_Msk : Wakeup interrupt
 *                        - \ref UART_IER_BUF_ERR_IE_Msk : Buffer Error interrupt
 *                        - \ref UART_IER_RTO_IE_Msk : Rx time-out interrupt
 *                        - \ref UART_IER_MODEM_IE_Msk : Modem interrupt
 *                        - \ref UART_IER_RLS_IE_Msk : Rx Line status interrupt
 *                        - \ref UART_IER_THRE_IE_Msk : Tx empty interrupt
 *                        - \ref UART_IER_RDA_IE_Msk : Rx ready interrupt
 *    @return    None
 */
#define UART_DISABLE_INT(uart, u32eIntSel)    (uart->IER &= ~ (u32eIntSel))


/**
 *    @brief    Get specified interrupt flag/status
 *
 *    @param[in]    uart            The base address of UART module
 *    @param[in]    u32eIntTypeFlag    Interrupt Type Flag,should be
 *                        - \ref UART_ISR_LIN_IS_Msk : LIN interrupt flag
 *                        - \ref UART_ISR_ABAUD_IS_Msk : Auto baudrate interrupt flag
 *                        - \ref UART_ISR_WAKE_IS_Msk : Wakeup interrupt flag
 *                        - \ref UART_ISR_BUF_ERR_IS_Msk : Buffer Error interrupt flag
 *                        - \ref UART_ISR_RTO_IS_Msk : Rx time-out interrupt flag
 *                        - \ref UART_ISR_MODEM_IS_Msk : Modem interrupt flag
 *                        - \ref UART_ISR_RLS_IS_Msk : Rx Line status interrupt flag
 *                        - \ref UART_ISR_THRE_IS_Msk : Tx empty interrupt flag
 *                        - \ref UART_ISR_RDA_IS_Msk : Rx ready interrupt flag
 *
 *    @return
 *            0 = The specified interrupt is not happened.
 *            1 = The specified interrupt is happened.
 */
#define UART_GET_INT_FLAG(uart,u32eIntTypeFlag)    ((uart->ISR & (u32eIntTypeFlag))?1:0)


/**
 *    @brief    Set RTS pin is low
 *
 *    @param[in]    uart        The base address of UART module
 *    @return    None
 */
__INLINE void UART_CLEAR_RTS(UART_T* uart)
{
    uart->MCSR |= UART_MCSR_LEV_RTS_Msk;
}

/**
 *    @brief    Set RTS pin is high
 *
 *    @param[in]    uart        The base address of UART module
 *    @return    None
 */
__INLINE void UART_SET_RTS(UART_T* uart)
{
    uart->MCSR &= ~UART_MCSR_LEV_RTS_Msk;
}

/**
 *  @brief  Clear RS-485 Address Byte Detection Flag
 *
 *  @param[in]  uart    The base address of UART module
 *  @return None
 */
#define UART_RS485_CLEAR_ADDR_FLAG(uart)    (uart->TRSR  |= UART_TRSR_RS485_ADDET_F_Msk)


/**
 *    @brief    Get RS-485 Address Byte Detection Flag
 *
 *    @param[in]    uart        The base address of UART module
 *    @return    RS-485 Address Byte Detection Flag
 */
#define UART_RS485_GET_ADDR_FLAG(uart)    ((uart->TRSR  & UART_TRSR_RS485_ADDET_F_Msk) >> UART_TRSR_RS485_ADDET_F_Pos)


void UART_ClearIntFlag(UART_T* uart , uint32_t u32InterruptFlag);
void UART_Close(UART_T* uart );
void UART_DisableFlowCtrl(UART_T* uart );
void UART_DisableInt(UART_T*  uart, uint32_t u32InterruptFlag );
void UART_EnableFlowCtrl(UART_T* uart );
void UART_EnableInt(UART_T*  uart, uint32_t u32InterruptFlag );
void UART_Open(UART_T* uart, uint32_t u32baudrate);
uint32_t UART_Read(UART_T* uart, uint8_t *pu8RxBuf, uint32_t u32ReadBytes);
void UART_SetLine_Config(UART_T* uart, uint32_t u32baudrate, uint32_t u32data_width, uint32_t u32parity, uint32_t  u32stop_bits);
void UART_SetTimeoutCnt(UART_T* uart, uint32_t u32TOC);
void UART_SelectIrDAMode(UART_T* uart, uint32_t u32Buadrate, uint32_t u32Direction);
void UART_SelectRS485Mode(UART_T* uart, uint32_t u32Mode, uint32_t u32Addr);
void UART_SelectLINMode(UART_T* uart, uint32_t u32Mode, uint32_t u32BreakLength);
uint32_t UART_Write(UART_T* uart,uint8_t *pu8TxBuf, uint32_t u32WriteBytes);


/*@}*/ /* end of group NANO100_UART_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO100_UART_Driver */

/*@}*/ /* end of group NANO100_Device_Driver */

#ifdef __cplusplus
}
#endif

#endif //__UART_H__

/*** (C) COPYRIGHT 2013 Nuvoton Technology Corp. ***/








