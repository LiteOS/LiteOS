/*!
 *****************************************************************************
   @file:    adi_uart_config.h
   @brief:   Configuration options for UART driver.
             This is specific to the UART driver and will be included by the driver.
             It is not required for the application to include this header file.
  -----------------------------------------------------------------------------

Copyright (c) 2016 Analog Devices, Inc.

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  - Modified versions of the software must be conspicuously marked as such.
  - This software is licensed solely and exclusively for use with processors
    manufactured by or for Analog Devices, Inc.
  - This software may not be combined or merged with other code in any manner
    that would cause the software to become subject to terms and conditions
    which differ from those listed here.
  - Neither the name of Analog Devices, Inc. nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.
  - The use of this software may or may not infringe the patent rights of one
    or more patent holders.  This license does not release you from the
    requirement that you obtain separate licenses from these patent holders
    to use this software.

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF CLAIMS OF INTELLECTUAL
PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*****************************************************************************/

#ifndef ADI_UART_CONFIG_H
#define ADI_UART_CONFIG_H

/** @addtogroup UART_Driver_Config Static Configuration
 *  @ingroup UART_Driver
 *  @{
 */


#include <adi_global_config.h>

/************** Common UART Driver configurations ************** */ 
/*!
   Enable the autobaud detection. \n
   Range: 0 to 1.
*/
#define ADI_UART_CFG_ENABLE_AUTOBAUD                 1


/** @addtogroup UART0_Driver_Config UART0 Static Configuration
 *  @ingroup UART_Driver_Config
 *  @{
 */
 
/************** UART Driver configurations FOR UART 0 ************** */
/*!
   Word length Select. \n
   0 -  5 Bits word length. \n
   1 -  6 Bits word length. \n
   2 -  7 Bits word length. \n
   3 -  8 Bits word length.
*/
#define ADI_UART0_CFG_WORD_LENGTH                  3


/*!
   Stop bit selection. \n
   0 -  Send 1 stop bit regardless of the word length. \n
   1 -  Send a number of stop bits based on the word length. \n
        WORD-LENGTH 5 Bits       => 1.5 Stop Bits. \n
        WORD-LENGTH (6/7/8) Bits => 2   Stop Bits.
*/
#define ADI_UART0_CFG_STOP_BIT                      1


/*!
   Parity Enable. Used to control the parity bit. \n
   0 -  Parity will not be transmitted or checked. \n
   1 -  Parity will be transmitted and checked.
*/
#define ADI_UART0_CFG_ENABLE_PARITY                 0


/*!
   Parity Select. This bit only has meaning if parity is enabled. \n
   0 - Odd parity will be transmitted and checked. \n
   1 - Even parity will be transmitted and checked.
*/
#define ADI_UART0_CFG_PARITY_SELECTION              0


/*!
   Stick Parity. Used to force parity to defined values. \n
   0 -  Parity will not be forced. \n
   1 -  Set parity based on the following bit settings: \n
        EPS = 1 and PEN = 1, parity will be forced to 0. \n
        EPS = 0 and PEN = 1, parity will be forced to 1. \n
        EPS = 1/0 and PEN = 0, no parity will be transmitted.
*/
#define ADI_UART0_CFG_ENABLE_STICKY_PARITY          0


/*
  Table 21-2: Baud Rate Examples Based on 26 MHz PCLK
  Baud Rate OSR COMDIV DIVM DIVN
  9600        3 24     3    1078
  19200       3 12     3    1078
  38400       3 8      2    1321
  57600       3 4      3    1078
  115200      3 4      1    1563
  230400      3 2      1    1563
  460800      3 1      1    1563
  921,600     2 1      1    1563
  1,000,000   2 1      1    1280
  1,500,000   2 1      1    171

These are calculated with the UarDivCalculator tool.
*/

/*!
   Fractional baud rate N divide value. \n
   Range: 0 to 2047.
*/
#define ADI_UART0_CFG_DIVN                           1078


/*!
   Fractional baud rate M divide value. \n
   Range: 1 to 3.
*/
#define ADI_UART0_CFG_DIVM                           3


/*!
   Fractional baud rate C divide value. \n
   Range: 1 to 65535.
*/
#define ADI_UART0_CFG_DIVC                           24


/*!
   Over Sample Rate value. \n
   Range: 0 to 3. \n
   0 - Over sample by 4. \n
   1 - Over sample by 8. \n
   2 - Over sample by 16. \n
   3 - Over sample by 32.

*/
#define ADI_UART0_CFG_OSR                            3


/*!
   Enable Internal FIFO. \n
   Range: 0 to 1.
*/
#define ADI_UART0_CFG_ENABLE_FIFO                    1


/*!
   TRIG Level for  UART device. \n
   Range: 0 to 3. \n
   0 - 1 byte to trig RX interrupt. \n
   1 - 4 bytes to trig RX interrupt. \n
   2 - 8 bytes to trig RX interrupt. \n
   3 - 14 bytes to trig RX interrupt.
*/
#define ADI_UART0_CFG_TRIG_LEVEL                     0


/*!
   Hold TX while RX is active.  \n
   Range: 0 to 1.
*/
#define ADI_UART0_CFG_HOLD_TX                        0


/*!
   Disable RX when TX is active. \n
   Range: 0 to 1. \n
   0 - 1 byte to trig RX interrupt. \n
   1 - 4 bytes to trig RX interrupt.
*/
#define ADI_UART0_CFG_DISABLE_RX                     0


/*!
   Configure the SOUT de-assertion earlier than full stop bit(s). \n
   Range: 0 to 1. \n
   0 - SOUT_EN de-assert same time as full stop bit(s). \n
   1 - SOUT_EN de-assert half-bit earlier than full stop bit(s).
*/
#define ADI_UART0_CFG_DEASSERTION                     0


/*!
   Set the SOUT polarity low. \n
   Range: 0 to 1. \n
   0 - Active high. \n
   1 - Active low.
*/
#define ADI_UART0_CFG_SOUT_POLARITY                   0

/*!
   Enable the RX status interrupt. \n
   Range: 0 to 1.
*/
#define ADI_UART0_CFG_ENABLE_RX_STATUS_INTERRUPT      1


/*!
   Enable the Modem status interrupt. \n
   Range: 0 to 1.
*/
#define ADI_UART0_CFG_ENABLE_MODEM_STATUS_INTERRUPT   0

/*! @} */


/***************  UART Driver configurations FOR UART 1 **************/

/** @addtogroup UART1_Driver_Config UART1 Static Configuration
 *  @ingroup UART_Driver_Config
 *  @{
 */

/*!
   Word length Select. \n
   0 -  5 Bits word length. \n
   1 -  6 Bits word length. \n
   2 -  7 Bits word length. \n
   3 -  8 Bits word length.
*/
#define ADI_UART1_CFG_WORD_LENGTH                     3


/*!
   Stop bit selection.\n
   0 -  Send 1 stop bit regardless of the word length. \n
   1 -  Send a number of stop bits based on the word length. \n
        WORD-LENGTH 5 Bits       => 1.5 Stop Bits. \n
        WORD-LENGTH (6/7/8) Bits => 2   Stop Bits.
*/
#define ADI_UART1_CFG_STOP_BIT                        1


/*!
   Parity Enable. Used to control the parity bit. \n
   0 -  Parity will not be transmitted or checked. \n
   1 -  Parity will be transmitted and checked.
*/
#define ADI_UART1_CFG_ENABLE_PARITY                   0


/*!
   Parity Select. This bit only has meaning if parity is enabled. \n
   0 - Odd parity will be transmitted and checked. \n
   1 - Even parity will be transmitted and checked.
*/
#define ADI_UART1_CFG_PARITY_SELECTION                0


/*!
   Stick Parity. Used to force parity to defined values. \n
   0 -  Parity will not be forced. \n
   1 -  Set parity based on the following bit settings: \n
        EPS = 1 and PEN = 1, parity will be forced to 0. \n
        EPS = 0 and PEN = 1, parity will be forced to 1. \n
        EPS = 1/0 and PEN = 0, no parity will be transmitted.
*/
#define ADI_UART1_CFG_ENABLE_STICKY_PARITY            0


/*
  Table 21-2: Baud Rate Examples Based on 26 MHz PCLK
  Baud Rate OSR COMDIV DIVM DIVN
  9600        3 24     3    1078
  19200       3 12     3    1078
  38400       3 8      2    1321
  57600       3 4      3    1078
  115200      3 4      1    1563
  230400      3 2      1    1563
  460800      3 1      1    1563
  921,600     2 1      1    1563
  1,000,000   2 1      1    1280
  1,500,000   2 1      1    171

These are calculated with the UarDivCalculator tool.
*/

/*!
   Fractional baud rate N divide value. \n
   Range: 0 to 2047.
*/
#define ADI_UART1_CFG_DIVN                              1563


/*!
   Fractional baud rate M divide value. \n
   Range: 1 to 3.
*/
#define ADI_UART1_CFG_DIVM                             1


/*!
   Fractional baud rate C divide value. \n
   Range: 1 to 65535.
*/
#define ADI_UART1_CFG_DIVC                             1


/*!
   Over Sample Rate value. \n
   Range: 0 to 3. \n
   0 - Over sample by 4. \n
   1 - Over sample by 8. \n
   2 - Over sample by 16. \n
   3 - Over sample by 32.

*/
#define ADI_UART1_CFG_OSR                              3


/*!
   Enable Internal FIFO. \n
   Range: 0 to 1.
*/
#define ADI_UART1_CFG_ENABLE_FIFO                      1


/*!
   TRIG Level for  UART device. \n
   Range: 0 to 3. \n
   0 - 1 byte to trig RX interrupt. \n
   1 - 4 bytes to trig RX interrupt. \n
   2 - 8 bytes to trig RX interrupt. \n
   3 - 14 bytes to trig RX interrupt.
*/
#define ADI_UART1_CFG_TRIG_LEVEL                       0


/*!
   Hold TX while RX is active.  \n
   Range: 0 to 1.
*/
#define ADI_UART1_CFG_HOLD_TX                          0


/*!
   Disable RX when TX is active. \n
   Range: 0 to 1. \n
   0 - 1 byte to trig RX interrupt. \n
   1 - 4 bytes to trig RX interrupt.
*/
#define ADI_UART1_CFG_DISABLE_RX                       0


/*!
   Configure the SOUT de-assertion earlier than full stop bit(s). \n
   Range: 0 to 1. \n
   0 - SOUT_EN de-assert same time as full stop bit(s). \n
   1 - SOUT_EN de-assert half-bit earlier than full stop bit(s).
*/
#define ADI_UART1_CFG_DEASSERTION                     0


/*!
   Set the SOUT polarity low. \n
   Range: 0 to 1. \n
   0 - Active high. \n
   1 - Active low.
*/
#define ADI_UART1_CFG_SOUT_POLARITY                   0

/*!
   Enable the RX status interrupt. \n
   Range: 0 to 1.
*/
#define ADI_UART1_CFG_ENABLE_RX_STATUS_INTERRUPT       1


/*!
   Enable the Modem status interrupt. \n
   Range: 0 to 1.
*/
#define ADI_UART1_CFG_ENABLE_MODEM_STATUS_INTERRUPT    0
/*! @} */

/*! @} */


/***************  UART Driver Debug Checks ************** */

/* Check word length */
#if (((ADI_UART0_CFG_WORD_LENGTH < 0) || (ADI_UART0_CFG_WORD_LENGTH > 3))  || ((ADI_UART1_CFG_WORD_LENGTH < 0) || (ADI_UART1_CFG_WORD_LENGTH > 3)))
#error "Word length needs to be between 0 and 3"
#endif

/* Check stop bit */
#if (((ADI_UART0_CFG_STOP_BIT < 0) || (ADI_UART0_CFG_STOP_BIT > 1))  || ((ADI_UART1_CFG_STOP_BIT < 0) || (ADI_UART1_CFG_STOP_BIT > 1)))
#error "Stop bit selection needs to be 0 or 1"
#endif

/* Check parity enable  */
#if (((ADI_UART0_CFG_ENABLE_PARITY < 0) || (ADI_UART0_CFG_ENABLE_PARITY > 1))  || ((ADI_UART1_CFG_ENABLE_PARITY < 0) || (ADI_UART1_CFG_ENABLE_PARITY > 1)))
#error "Parity Enable bit needs to be 0 or 1"
#endif

/* Check parity select */
#if (((ADI_UART0_CFG_PARITY_SELECTION < 0) || (ADI_UART0_CFG_PARITY_SELECTION > 1))  || ((ADI_UART1_CFG_PARITY_SELECTION < 0) || (ADI_UART1_CFG_PARITY_SELECTION > 1)))
#error "Parity bit selection needs to be 0 or 1"
#endif

/* Check enable sticky parity */
#if (((ADI_UART0_CFG_ENABLE_STICKY_PARITY < 0) || (ADI_UART0_CFG_ENABLE_STICKY_PARITY > 1))  || ((ADI_UART1_CFG_ENABLE_STICKY_PARITY < 0) || (ADI_UART1_CFG_ENABLE_STICKY_PARITY > 1)))
#error "Sticky parity enable needs to be 0 or 1"
#endif

/* Check fractional baudrate N divider value */
#if (((ADI_UART0_CFG_DIVN < 0) || (ADI_UART0_CFG_DIVN > 2047))  || ((ADI_UART1_CFG_DIVN < 0) || (ADI_UART1_CFG_DIVN > 2047)))
#error "Fractional baudrate N divider value needs to be between 0 and 2047"
#endif

/* Check fractional baudrate M divider value */
#if (((ADI_UART0_CFG_DIVM < 1) || (ADI_UART0_CFG_DIVM > 3))  || ((ADI_UART1_CFG_DIVM < 1) || (ADI_UART1_CFG_DIVM > 3)))
#error "Fractional baudrate M divider value needs to be between 1 and 3"
#endif

/* Check fractional baudrate C divider value */
#if (((ADI_UART0_CFG_DIVC < 1) || (ADI_UART0_CFG_DIVC > 65535))  || ((ADI_UART1_CFG_DIVC < 1) || (ADI_UART1_CFG_DIVC > 65535)))
#error "Fractional baudrate C divider value needs to be between 1 and 65535"
#endif

/* Check over same rate value */
#if (((ADI_UART0_CFG_OSR < 0) || (ADI_UART0_CFG_OSR > 3))  || ((ADI_UART1_CFG_OSR < 0) || (ADI_UART1_CFG_OSR > 3)))
#error "over sample rate value needs to be between 0 and 3"
#endif

/* Check enable internal FIFO */
#if (((ADI_UART0_CFG_ENABLE_FIFO < 0) || (ADI_UART0_CFG_ENABLE_FIFO > 1))  || ((ADI_UART1_CFG_ENABLE_FIFO < 0) || (ADI_UART1_CFG_ENABLE_FIFO > 1)))
#error "Enable internal FIFO needs to be 0 or 1"
#endif

/* Check UART trig level */
#if (((ADI_UART0_CFG_TRIG_LEVEL < 0) || (ADI_UART0_CFG_TRIG_LEVEL > 3))  || ((ADI_UART1_CFG_TRIG_LEVEL < 0) || (ADI_UART1_CFG_TRIG_LEVEL > 3)))
#error "Trig level for the UART device needs to be 0 or 1"
#endif

/* Check value for holding tx while rx is active */
#if (((ADI_UART0_CFG_HOLD_TX < 0) || (ADI_UART0_CFG_HOLD_TX > 1))  || ((ADI_UART1_CFG_HOLD_TX < 0) || (ADI_UART1_CFG_HOLD_TX > 1)))
#error "Value for holding Tx while Rx is active needs to be 0 or 1"
#endif

/* Check value de-assertion */
#if (((ADI_UART0_CFG_DEASSERTION < 0) || (ADI_UART0_CFG_DEASSERTION > 1))  || ((ADI_UART1_CFG_DEASSERTION < 0) || (ADI_UART1_CFG_DEASSERTION > 1)))
#error "Value for de-assertion needs to be 0 or 1"
#endif

/* Check value for SOUT polarity */
#if (((ADI_UART0_CFG_SOUT_POLARITY < 0) || (ADI_UART0_CFG_SOUT_POLARITY > 1))  || ((ADI_UART1_CFG_SOUT_POLARITY < 0) || (ADI_UART1_CFG_SOUT_POLARITY > 1)))
#error "Value for SOUT polarity needs to be 0 or 1"
#endif

/* Check value to enable autobaud detection */
#if ((ADI_UART_CFG_ENABLE_AUTOBAUD < 0) || (ADI_UART_CFG_ENABLE_AUTOBAUD > 1))
#error "Value for autobaud enable needs to be 0 or 1"
#endif

/* Check value to enable Rx status interrupt */
#if (((ADI_UART0_CFG_ENABLE_RX_STATUS_INTERRUPT < 0) || (ADI_UART0_CFG_ENABLE_RX_STATUS_INTERRUPT > 1))  || ((ADI_UART1_CFG_ENABLE_RX_STATUS_INTERRUPT < 0) || (ADI_UART1_CFG_ENABLE_RX_STATUS_INTERRUPT > 1)))
#error "Value to enable Rx status interrupt needs to be 0 or 1"
#endif

/* Check value to enable modem status interrupt */
#if (((ADI_UART0_CFG_ENABLE_MODEM_STATUS_INTERRUPT < 0) || (ADI_UART0_CFG_ENABLE_MODEM_STATUS_INTERRUPT > 1))  || ((ADI_UART1_CFG_ENABLE_MODEM_STATUS_INTERRUPT < 0) || (ADI_UART1_CFG_ENABLE_MODEM_STATUS_INTERRUPT > 1)))
#error "Value to enable modem status interrupt needs to be 0 or 1"
#endif

#endif /* ADI_UART_CONFIG_H */
