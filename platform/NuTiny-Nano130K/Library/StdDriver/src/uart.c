/**************************************************************************//**
 * @file     uart.c
 * @version  V1.00
 * $Revision: 11 $
 * $Date: 15/06/26 1:28p $
 * @brief    Nano100 series Smartcard UART mode (UART) driver source file
 *
 * @note
 * Copyright (C) 2013 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/

#include <stdio.h>
#include "Nano100Series.h"

/** @addtogroup NANO100_Device_Driver NANO100 Device Driver
  @{
*/

/** @addtogroup NANO100_UART_Driver UART Driver
  @{
*/


/** @addtogroup NANO100_UART_EXPORTED_FUNCTIONS UART Exported Functions
  @{
*/
/// @cond HIDDEN_SYMBOLS
extern uint32_t SysGet_PLLClockFreq(void);
/// @endcond /* HIDDEN_SYMBOLS */


/**
 *    @brief The function is used to clear UART specified interrupt flag.
 *
 *    @param[in] uart                The base address of UART module.
 *    @param[in] u32InterruptFlag    The specified interrupt of UART module..
 *
 *    @return None
 */
void UART_ClearIntFlag(UART_T* uart , uint32_t u32InterruptFlag)
{

    if(u32InterruptFlag & UART_ISR_RLS_IS_Msk) { /* clear Receive Line Status Interrupt */
        uart->FSR |= UART_FSR_BI_F_Msk | UART_FSR_FE_F_Msk | UART_FSR_FE_F_Msk;
        uart->TRSR |= UART_TRSR_RS485_ADDET_F_Msk;
    }

    if(u32InterruptFlag & UART_ISR_MODEM_IS_Msk)  /* clear Modem Interrupt */
        uart->MCSR |= UART_MCSR_DCT_F_Msk;

    if(u32InterruptFlag & UART_ISR_BUF_ERR_IS_Msk) { /* clear Buffer Error Interrupt */
        uart->FSR |= UART_FSR_RX_OVER_F_Msk | UART_FSR_TX_OVER_F_Msk;
    }

    if(u32InterruptFlag & UART_ISR_WAKE_IS_Msk) { /* clear wake up Interrupt */
        uart->ISR |= UART_ISR_WAKE_IS_Msk;
    }

    if(u32InterruptFlag & UART_ISR_ABAUD_IS_Msk) { /* clear auto-baud rate Interrupt */
        uart->TRSR |= UART_TRSR_ABAUD_TOUT_F_Msk | UART_TRSR_ABAUD_F_Msk;
    }

    if(u32InterruptFlag & UART_ISR_LIN_IS_Msk) { /* clear LIN break Interrupt */
        uart->TRSR |= UART_TRSR_LIN_TX_F_Msk | UART_TRSR_LIN_RX_F_Msk | UART_TRSR_BIT_ERR_F_Msk;
    }

}


/**
 *  @brief The function is used to disable UART.
 *
 *  @param[in] uart        The base address of UART module.
 *
 *  @return None
 */
void UART_Close(UART_T* uart)
{
    uart->IER = 0;
}


/**
 *  @brief The function is used to disable UART auto flow control.
 *
 *  @param[in] uart        The base address of UART module.
 *
 *  @return None
 */
void UART_DisableFlowCtrl(UART_T* uart)
{
    uart->CTL &= ~(UART_CTL_AUTO_RTS_EN_Msk | UART_CTL_AUTO_CTS_EN_Msk);
}


/**
 *    @brief    The function is used to disable UART specified interrupt and disable NVIC UART IRQ.
 *
 *    @param[in]    uart                The base address of UART module.
 *    @param[in]    u32InterruptFlag    The specified interrupt of UART module.
 *                                - \ref UART_IER_LIN_IE_Msk : LIN interrupt
 *                                - \ref UART_IER_ABAUD_IE_Msk : Auto baudrate interrupt
 *                                - \ref UART_IER_WAKE_IE_Msk : Wakeup interrupt
 *                                - \ref UART_IER_ABAUD_IE_Msk : Auto Baud-rate interrupt
 *                                - \ref UART_IER_BUF_ERR_IE_Msk : Buffer Error interrupt
 *                                - \ref UART_IER_RTO_IE_Msk : Rx time-out interrupt
 *                                - \ref UART_IER_MODEM_IE_Msk : Modem interrupt
 *                                - \ref UART_IER_RLS_IE_Msk : Rx Line status interrupt
 *                                - \ref UART_IER_THRE_IE_Msk : Tx empty interrupt
 *                                - \ref UART_IER_RDA_IE_Msk : Rx ready interrupt
 *
 *    @return    None
 */
void UART_DisableInt(UART_T*  uart, uint32_t u32InterruptFlag )
{
    uart->IER &= ~ u32InterruptFlag;
}



/**
 *    @brief    The function is used to Enable UART auto flow control.
 *
 *    @param[in]    uart    The base address of UART module.
 *
 *    @return    None
 */
void UART_EnableFlowCtrl(UART_T* uart )
{
    uart->MCSR |= UART_MCSR_LEV_RTS_Msk | UART_MCSR_LEV_CTS_Msk;
    uart->CTL |= UART_CTL_AUTO_RTS_EN_Msk | UART_CTL_AUTO_CTS_EN_Msk;
}


/**
 *    @brief    The function is used to enable UART specified interrupt and disable NVIC UART IRQ.
 *
 *    @param[in]    uart                The base address of UART module.
 *    @param[in]    u32InterruptFlag    The specified interrupt of UART module:
 *                                - \ref UART_IER_LIN_IE_Msk : LIN interrupt
 *                                - \ref UART_IER_ABAUD_IE_Msk : Auto baudrate interrupt
 *                                - \ref UART_IER_WAKE_IE_Msk : Wakeup interrupt
 *                                - \ref UART_IER_ABAUD_IE_Msk : Auto Baud-rate interrupt
 *                                - \ref UART_IER_BUF_ERR_IE_Msk : Buffer Error interrupt
 *                                - \ref UART_IER_RTO_IE_Msk : Rx time-out interrupt
 *                                - \ref UART_IER_MODEM_IE_Msk : Modem interrupt
 *                                - \ref UART_IER_RLS_IE_Msk : Rx Line status interrupt
 *                                - \ref UART_IER_THRE_IE_Msk : Tx empty interrupt
 *                                - \ref UART_IER_RDA_IE_Msk : Rx ready interrupt
 *
 *    @return None
 */
void UART_EnableInt(UART_T*  uart, uint32_t u32InterruptFlag )
{
    uart->IER |= u32InterruptFlag;
}


/**
 *    @brief    This function use to enable UART function and set baud-rate.
 *
 *    @param[in]    uart    The base address of UART module.
 *    @param[in]    u32baudrate    The baudrate of UART module.
 *
 *    @return    None
 */
void UART_Open(UART_T* uart, uint32_t u32baudrate)
{
    uint8_t u8UartClkSrcSel;
    uint32_t u32ClkTbl[4] = {__HXT, __LXT, 0, __HIRC12M};
    uint32_t u32Baud_Div;
    uint32_t u32SrcFreq;
    uint32_t u32SrcFreqDiv;

    u8UartClkSrcSel = (CLK->CLKSEL1 & CLK_CLKSEL1_UART_S_Msk) >> CLK_CLKSEL1_UART_S_Pos;

    u32SrcFreq = u32ClkTbl[u8UartClkSrcSel];

    u32SrcFreqDiv = (((CLK->CLKDIV0 & CLK_CLKDIV0_UART_N_Msk) >> CLK_CLKDIV0_UART_N_Pos) + 1);

    if(u32SrcFreq == 0) {
        u32SrcFreq = SysGet_PLLClockFreq() / u32SrcFreqDiv;
    } else {
        u32SrcFreq = u32SrcFreq / u32SrcFreqDiv;
    }

    uart->FUN_SEL = UART_FUNC_SEL_UART;
    uart->TLCTL = UART_WORD_LEN_8 | UART_PARITY_NONE | UART_STOP_BIT_1 |
                  UART_TLCTL_RFITL_1BYTE | UART_TLCTL_RTS_TRI_LEV_1BYTE;

    if(u32baudrate != 0) {
        u32Baud_Div = UART_BAUD_MODE0_DIVIDER(u32SrcFreq, u32baudrate);

        if(u32Baud_Div > 0xFFFF)
            uart->BAUD = (UART_BAUD_MODE1 | UART_BAUD_MODE1_DIVIDER(u32SrcFreq, u32baudrate));
        else
            uart->BAUD = (UART_BAUD_MODE0 | u32Baud_Div);
    }
}


/**
 *    @brief    The function is used to read Rx data from RX FIFO and the data will be stored in pu8RxBuf.
 *
 *    @param[in]    uart            The base address of UART module.
 *    @param[out]   pu8RxBuf        The buffer to receive the data of receive FIFO.
 *    @param[in]    u32ReadBytes    The the read bytes number of data.
 *
 *  @return     u32Count: Receive byte count
 *
 */
uint32_t UART_Read(UART_T* uart, uint8_t *pu8RxBuf, uint32_t u32ReadBytes)
{
    uint32_t  u32Count, u32delayno;

    for(u32Count=0; u32Count < u32ReadBytes; u32Count++) {
        u32delayno = 0;

        while(uart->FSR & UART_FSR_RX_EMPTY_F_Msk) { /* Check RX empty => failed */
            u32delayno++;
            if( u32delayno >= 0x40000000 )
                return FALSE;
        }
        pu8RxBuf[u32Count] = uart->RBR;    /* Get Data from UART RX  */
    }

    return u32Count;

}


/**
 *    @brief    This function use to config UART line setting.
 *
 *    @param[in]    uart            The base address of UART module.
 *    @param[in]    u32baudrate     The register value of baudrate of UART module.
 *                                  if u32baudrate = 0, UART baudrate will not change.
 *    @param[in]    u32data_width   The data length of UART module.
 *    @param[in]    u32parity       The parity setting (odd/even/none) of UART module.
 *    @param[in]    u32stop_bits    The stop bit length (1/1.5 bit) of UART module.
 *
 *    @return    None
 */
void UART_SetLine_Config(UART_T* uart, uint32_t u32baudrate, uint32_t u32data_width, uint32_t u32parity, uint32_t  u32stop_bits)
{
    uint8_t u8UartClkSrcSel;
    uint32_t u32ClkTbl[4] = {__HXT, __LXT, 0, __HIRC12M};
    uint32_t u32Baud_Div = 0;
    uint32_t u32SrcFreq;
    uint32_t u32SrcFreqDiv;

    u8UartClkSrcSel = (CLK->CLKSEL1 & CLK_CLKSEL1_UART_S_Msk) >> CLK_CLKSEL1_UART_S_Pos;

    u32SrcFreq = u32ClkTbl[u8UartClkSrcSel];

    u32SrcFreqDiv = (((CLK->CLKDIV0 & CLK_CLKDIV0_UART_N_Msk) >> CLK_CLKDIV0_UART_N_Pos) + 1);

    if(u32SrcFreq == 0) {
        u32SrcFreq = SysGet_PLLClockFreq() / u32SrcFreqDiv;
    } else {
        u32SrcFreq = u32SrcFreq / u32SrcFreqDiv;
    }

    if(u32baudrate != 0) {
        u32Baud_Div = UART_BAUD_MODE0_DIVIDER(u32SrcFreq, u32baudrate);

        if(u32Baud_Div > 0xFFFF)
            uart->BAUD = (UART_BAUD_MODE1 | UART_BAUD_MODE1_DIVIDER(u32SrcFreq, u32baudrate));
        else
            uart->BAUD = (UART_BAUD_MODE0 | u32Baud_Div);
    }

    uart->TLCTL = u32data_width | u32parity | u32stop_bits;
}


/**
 *    @brief    This function use to set Rx timeout count.
 *
 *    @param[in]    uart    The base address of UART module.
 *    @param[in]    u32TOC    Rx timeout counter.
 *
 *    @return    None
 */
void UART_SetTimeoutCnt(UART_T* uart, uint32_t u32TOC)
{
    uart->TMCTL = (uart->TMCTL & ~UART_TMCTL_TOIC_Msk)| (u32TOC);
    uart->IER |= UART_IER_RTO_IE_Msk;
}


/**
 *    @brief    The function is used to configure IrDA relative settings. It consists of TX or RX mode and baudrate.
 *
 *    @param[in]    uart            The base address of UART module.
 *    @param[in]    u32Buadrate        The baudrate of UART module.
 *    @param[in]    u32Direction    The direction(transmit:1/receive:0) of UART module in IrDA mode.
 *
 *    @return    None
 */
void UART_SelectIrDAMode(UART_T* uart, uint32_t u32Buadrate, uint32_t u32Direction)
{
    uint8_t u8UartClkSrcSel;
    uint32_t u32ClkTbl[4] = {__HXT, __LXT, 0, __HIRC12M};
    uint32_t u32SrcFreq;
    uint32_t u32SrcFreqDiv;

    u8UartClkSrcSel = (CLK->CLKSEL1 & CLK_CLKSEL1_UART_S_Msk) >> CLK_CLKSEL1_UART_S_Pos;

    u32SrcFreq = u32ClkTbl[u8UartClkSrcSel];

    u32SrcFreqDiv = (((CLK->CLKDIV0 & CLK_CLKDIV0_UART_N_Msk) >> CLK_CLKDIV0_UART_N_Pos) + 1);

    if(u32SrcFreq == 0) {
        u32SrcFreq = SysGet_PLLClockFreq() / u32SrcFreqDiv;
    } else {
        u32SrcFreq = u32SrcFreq / u32SrcFreqDiv;
    }

    uart->BAUD = UART_BAUD_MODE1 | UART_BAUD_MODE1_DIVIDER(u32SrcFreq, u32Buadrate);

    uart->IRCR    &=  ~UART_IRCR_INV_TX_Msk;
    uart->IRCR |=     UART_IRCR_INV_RX_Msk;
    uart->IRCR    = u32Direction ? uart->IRCR | UART_IRCR_TX_SELECT_Msk : uart->IRCR &~ UART_IRCR_TX_SELECT_Msk;
    uart->FUN_SEL = (0x2 << UART_FUN_SEL_FUN_SEL_Pos);
}


/**
 *    @brief    The function is used to set RS485 relative setting.
 *
 *    @param[in]    uart        The base address of UART module.
 *    @param[in]    u32Mode        The operation mode( \ref UART_ALT_CTL_RS485_NMM_Msk / \ref UART_ALT_CTL_RS485_AUD_Msk / \ref UART_ALT_CTL_RS485_AAD_Msk).
 *    @param[in]    u32Addr        The RS485 address.
 *
 *    @return    None
 */
void UART_SelectRS485Mode(UART_T* uart, uint32_t u32Mode, uint32_t u32Addr)
{
    uart->FUN_SEL = UART_FUNC_SEL_RS485;
    uart->ALT_CTL = 0;
    uart->ALT_CTL |= u32Mode | (u32Addr << UART_ALT_CTL_ADDR_PID_MATCH_Pos);
}

/**
 *    @brief        Select and configure LIN function
 *
 *    @param[in]    uart            The pointer of the specified UART module.
 *    @param[in]    u32Mode         The LIN direction :
 *                                  - UART_ALT_CTL_LIN_TX_EN_Msk
 *                                  - UART_ALT_CTL_LIN_RX_EN_Msk
 *                                  - (UART_ALT_CTL_LIN_TX_EN_Msk|UART_ALT_CTL_LIN_RX_EN_Msk)
 *    @param[in]    u32BreakLength  The breakfield length.
 *
 *    @return       None
 *
 *    @details      The function is used to set LIN relative setting.
 */
void UART_SelectLINMode(UART_T* uart, uint32_t u32Mode, uint32_t u32BreakLength)
{
    /* Select LIN function mode */
    uart->FUN_SEL = UART_FUNC_SEL_LIN;

    /* Select LIN function setting : Tx enable, Rx enable and break field length */
    uart->FUN_SEL = UART_FUNC_SEL_LIN;
    uart->ALT_CTL &= ~(UART_ALT_CTL_LIN_TX_BCNT_Msk | UART_ALT_CTL_LIN_RX_EN_Msk | UART_ALT_CTL_LIN_TX_EN_Msk);
    uart->ALT_CTL |= u32BreakLength & UART_ALT_CTL_LIN_TX_BCNT_Msk;
    uart->ALT_CTL |= u32Mode;
}

/**
 *    @brief    The function is to write data into TX buffer to transmit data by UART.
 *
 *    @param[in]    uart            The base address of UART module.
 *    @param[in]    pu8TxBuf        The buffer to send the data to UART transmission FIFO.
 *    @param[in]    u32WriteBytes    The byte number of data.
 *
 *  @return u32Count: transfer byte count
 */
uint32_t UART_Write(UART_T* uart,uint8_t *pu8TxBuf, uint32_t u32WriteBytes)
{
    uint32_t  u32Count, u32delayno;

    for(u32Count=0; u32Count != u32WriteBytes; u32Count++) {
        u32delayno = 0;
        while((uart->FSR & UART_FSR_TX_EMPTY_F_Msk) == 0) { /* Wait Tx empty and Time-out manner */
            u32delayno++;
            if( u32delayno >= 0x40000000 )
                return FALSE;
        }
        uart->THR = pu8TxBuf[u32Count];    /* Send UART Data from buffer */
    }

    return u32Count;

}


/*@}*/ /* end of group NANO100_UART_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO100_UART_Driver */

/*@}*/ /* end of group NANO100_Device_Driver */

/*** (C) COPYRIGHT 2013 Nuvoton Technology Corp. ***/



