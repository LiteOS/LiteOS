/*!
 *****************************************************************************
   @file:    adi_spi_config.h
   @brief:   Configuration options for SPI driver.
             This is specific to the SPI driver and will be included by the driver.
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

#ifndef ADI_SPI_CONFIG_H__
#define ADI_SPI_CONFIG_H__
#include <adi_global_config.h>
/** @addtogroup SPI_Driver_Config Static Configuration
 *  @ingroup SPI_Driver
 *  @{
 */


/*! Set this macro to the system clock frequency in hertz*/
#define ADI_CFG_SYSTEM_CLOCK_HZ                    (26000000u)

/************* SPI controller configurations ***************/

/* There are three SPI instances SPI0, SPI1 and SPI2       */
/* Each SPI has its own configuration macros                */


/*----------------------------------------------------------*/
/* -------------------- SPI0 -------------------------------*/
/*----------------------------------------------------------*/

/** @addtogroup SPI_Driver_Config_SPI0 SPI0 Static Configuration
 *  @ingroup SPI_Driver_Config
 *  @{
 */

 /*! If using SPI0 in master mode set this macro to 1. For slave mode set this macro to 0. */
#define ADI_SPI0_MASTER_MODE               (1u)


/*! Set this macro to the SPI0 bit rate in hertz */
#define ADI_SPI0_CFG_BIT_RATE               (2000000u)

/*! SPI0 enable\n
    SPI configuration register: Bit[0]\n
    1 -  Enable SPI\n
    0 -  Disable SPI */
#define ADI_SPI0_CFG_ENABLE                         (0u)

/*! SPI0 clock phase mode\n
    SPI configuration register: Bit[2]\n
    1 - Serial clock pulses at the beginning of each serial bit transfer.\n
    0 - Serial clock pulses at the end of each serial bit transfer. */
#define ADI_SPI0_CFG_CLK_PHASE                      (0u)





/*! SPI0 clock polarity\n
    SPI configuration register: Bit[3]\n
    1 - Serial clock idles high.\n
    0 - Serial clock idles low. */
#define ADI_SPI0_CFG_CLK_POLARITY                   (0u)


/*! SPI0 wired OR mode\n
    SPI configuration register: Bit[4]\n
    1 - Enables open circuit output enable.\n
    0 - Normal output levels. */
#define ADI_SPI0_CFG_WIRED_OR                       (0u)


/*! SPI0 LSB/MSB\n
    SPI configuration register: Bit[5]\n
    1 - MSB transmitted first.\n
    0 - LSB transmitted first. */
#define ADI_SPI0_CFG_LSB_MSB                        (0u)


/*! SPI0 transfer initiate\n
    SPI configuration register: Bit[6]\n
    1 - SPI transfer is initiated with write to Tx FIFO register. Interrupts when Tx is empty.\n
    0 - SPI transfer is initiated with a read of the Rx FIFO register. Interrupts when Rx is full.*/
#define ADI_SPI0_CFG_TRANSFER_INITIATE              (0u)


/*! SPI0 Tx FIFO transfers zeros or last bit upon underflow\n
    SPI configuration register: Bit[7]\n
    1 - Tx FIFO sends zeros upon underflow.\n
    0 - Tx FIFO repeats last bit upon underflow. */
#define ADI_SPI0_CFG_TX_UNDERFLOW                   (0u)


/*! SPI0 Rx FIFO overflows with received data or data is discarded\n
    SPI configuration register: Bit[8]\n
    1 - Rx FIFO receives data upon overflow.\n
    0 - Rx FIFO discards received data upon overflow. */
#define ADI_SPI0_CFG_RX_OVERFLOW                    (0u)


/*! SPI0 slave mode MISO enable\n
    SPI configuration register: Bit[9]\n
    1 - MISO operates as normal in slave mode.\n
    0 - MISO is disabled in slave mode. */
#define ADI_SPI0_CFG_MISO_ENABLE                    (0u)


/*! SPI0 internal loopback enable\n
    SPI configuration register: Bit[10]\n
    1 - MISO and MOSI is loopbacked internally.\n
    0 - MISO and MOSI operates normally. */
#define ADI_SPI0_CFG_LOOPBACK                       (0u)

/*! SPI0 transfer and interrupt mode\n
    SPI configuration register: Bit[11]\n
    1 - SPI continuous transfers in which CS remains asserted until Tx is empty.\n
    0 - SPI disable continuous transfer, each transfer consists of 8 bits of data.*/
#define ADI_SPI0_CFG_CONTINUOUS                     (0u)

/*! SPI0 Rx FIFO flush enable\n
    SPI configuration register: Bit[12]\n
    1 - Rx FIFO is flushed and all rx data is ignored and no interrupts are generated.\n
    0 - Rx FIFO flush is disabled. */
#define ADI_SPI0_CFG_RX_FLUSH                       (0u)


/*! SPI0 Tx FIFO flush enable\n
    SPI configuration register: Bit[13]\n
    1 - Tx FIFO is flushed.\n
    0 - Tx FIFO flush is disabled. */
#define ADI_SPI0_CFG_TX_FLUSH                       (0u)


/*! Reset Mode for CSERR.       \n
    SPI0 configuration register: Bit[14]\n
    0 -  To  continue from where it stopped. SPI can receive the remaining bits
         when CS gets asserted and Cortex has to ignore the CSERR interrupt.\n
    1 -  To enable resetting the bit counter and  reset if there is  a
         CS error condition and the Cortex is expected to clear the SPI_EN bit.
*/
#define ADI_SPI0_CFG_CSERR_RESET                       (0u)


/*! SPI0 clock divide\n
    SPI baud rate selection register: Bit[0:5]\n
    Value between 0-63 that is used to divide the UCLK to generate
    the SPI serial clock. */
#define ADI_SPI0_CFG_CLK_DIV                        (0u)


/*! SPI0 high frequency mode\n
    SPI baud rate selection register: Bit[6]\n
    1 - High frequency mode enabled.\n
    0 - High frequency mode disabled. */
#define ADI_SPI0_CFG_HFM                            (0u)


/*! SPI0 reset mode for CSERR\n
    SPI baud rate selection register: Bit[7]\n
    1 - clear bit counter on CS error.\n
    0 - do not clear bit counter on CS error. */
#define ADI_SPI0_CFG_CS_ERR                         (0u)


/*! SPI0 CS interrupt\n
    SPI baud rate selection register: Bit[8]\n
    1 - In continuous mode, generate interrupt on CS.\n
    0 - In continuous mode, do not generate interrupt on CS. */
#define ADI_SPI0_CFG_CS_IRQ                         (0u)


/*! @} */

/*----------------------------------------------------------*/
/* -------------------- SPI1 -------------------------------*/
/*----------------------------------------------------------*/

/** @addtogroup SPI_Driver_Config_SPI1 SPI1 Static Configuration
 *  @ingroup SPI_Driver_Config
 *  @{
 */

 /*! If using SPI1 in master mode set this macro to 1. For slave mode set this macro to 0. */
#define ADI_SPI1_MASTER_MODE               (1u)
 
/*! Set this macro to the SPI1 bit rate in hertz */
#define ADI_SPI1_CFG_BIT_RATE               (2000000u)

/*! SPI1 enable\n
    SPI configuration register: Bit[0]\n
    1 -  Enable SPI\n
    0 -  Disable SPI */
#define ADI_SPI1_CFG_ENABLE                         (0u)

/*! SPI1 clock phase mode\n
    SPI configuration register: Bit[2]\n
    1 - Serial clock pulses at the beginning of each serial bit transfer.\n
    0 - Serial clock pulses at the end of each serial bit transfer. */
#define ADI_SPI1_CFG_CLK_PHASE                      (0u)





/*! SPI1 clock polarity\n
    SPI configuration register: Bit[3]\n
    1 - Serial clock idles high.\n
    0 - Serial clock idles low. */
#define ADI_SPI1_CFG_CLK_POLARITY                   (0u)


/*! SPI1 wired OR mode\n
    SPI configuration register: Bit[4]\n
    1 - Enables open circuit output enable.\n
    0 - Normal output levels. */
#define ADI_SPI1_CFG_WIRED_OR                       (0u)


/*! SPI1 LSB/MSB\n
    SPI configuration register: Bit[5]\n
    1 - MSB transmitted first.\n
    0 - LSB transmitted first. */
#define ADI_SPI1_CFG_LSB_MSB                        (0u)


/*! SPI1 transfer initiate\n
    SPI configuration register: Bit[6]\n
    1 - SPI transfer is initiated with write to Tx FIFO register. Interrupts when Tx is empty.\n
    0 - SPI transfer is initiated with a read of the Rx FIFO register. Interrupts when Rx is full.*/
#define ADI_SPI1_CFG_TRANSFER_INITIATE              (0u)


/*! SPI1 Tx FIFO transfers zeros or last bit upon underflow\n
    SPI configuration register: Bit[7]\n
    1 - Tx FIFO sends zeros upon underflow.\n
    0 - Tx FIFO repeats last bit upon underflow. */
#define ADI_SPI1_CFG_TX_UNDERFLOW                   (0u)


/*! SPI1 Rx FIFO overflows with received data or data is discarded\n
    SPI configuration register: Bit[8]\n
    1 - Rx FIFO receives data upon overflow.\n
    0 - Rx FIFO discards received data upon overflow. */
#define ADI_SPI1_CFG_RX_OVERFLOW                    (0u)


/*! SPI1 slave mode MISO enable\n
    SPI configuration register: Bit[9]\n
    1 - MISO operates as normal in slave mode.\n
    0 - MISO is disabled in slave mode. */
#define ADI_SPI1_CFG_MISO_ENABLE                    (0u)


/*! SPI1 internal loopback enable\n
    SPI configuration register: Bit[10]\n
    1 - MISO and MOSI is loopbacked internally.\n
    0 - MISO and MOSI operates normally. */
#define ADI_SPI1_CFG_LOOPBACK                       (0u)

/*! SPI1 transfer and interrupt mode\n
    SPI configuration register: Bit[11]\n
    1 - SPI continuous transfers in which CS remains asserted until Tx is empty.\n
    0 - SPI disable continuous transfer, each transfer consists of 8 bits of data.*/
#define ADI_SPI1_CFG_CONTINUOUS                     (0u)

/*! SPI1 Rx FIFO flush enable\n
    SPI configuration register: Bit[12]\n
    1 - Rx FIFO is flushed and all rx data is ignored and no interrupts are generated.\n
    0 - Rx FIFO flush is disabled. */
#define ADI_SPI1_CFG_RX_FLUSH                       (0u)


/*! SPI1 Tx FIFO flush enable\n
    SPI configuration register: Bit[13]\n
    1 - Tx FIFO is flushed.\n
    0 - Tx FIFO flush is disabled. */
#define ADI_SPI1_CFG_TX_FLUSH                       (0u)


/*! Reset Mode for CSERR.       \n
    SPI1 configuration register: Bit[14]\n
    0 -  To  continue from where it stopped. SPI can receive the remaining bits
         when CS gets asserted and Cortex has to ignore the CSERR interrupt.\n
    1 -  To enable resetting the bit counter and  reset if there is  a
         CS error condition and the Cortex is expected to clear the SPI_EN bit.
*/
#define ADI_SPI1_CFG_CSERR_RESET                       (0u)


/*! SPI1 clock divide\n
    SPI baud rate selection register: Bit[0:5]\n
    Value between 0-63 that is used to divide the UCLK to generate
    the SPI serial clock. */
#define ADI_SPI1_CFG_CLK_DIV                        (0u)


/*! SPI1 high frequency mode\n
    SPI baud rate selection register: Bit[6]\n
    1 - High frequency mode enabled.\n
    0 - High frequency mode disabled. */
#define ADI_SPI1_CFG_HFM                            (0u)


/*! SPI1 reset mode for CSERR\n
    SPI baud rate selection register: Bit[7]\n
    1 - clear bit counter on CS error.\n
    0 - do not clear bit counter on CS error. */
#define ADI_SPI1_CFG_CS_ERR                         (0u)


/*! SPI1 CS interrupt\n
    SPI baud rate selection register: Bit[8]\n
    1 - In continuous mode, generate interrupt on CS.\n
    0 - In continuous mode, do not generate interrupt on CS. */
#define ADI_SPI1_CFG_CS_IRQ  

/*! @} */

/*----------------------------------------------------------*/
/* -------------------- SPI2 -------------------------------*/
/*----------------------------------------------------------*/

/** @addtogroup SPI_Driver_Config_SPI2 SPI2 Static Configuration
 *  @ingroup SP2_Driver_Config
 *  @{
 */

/*! If using SPI2 in master mode set this macro to 1. For slave mode set this macro to 0. */
#define ADI_SPI2_MASTER_MODE               (1u)

/*! Set this macro to the SPI2 bit rate in hertz */
#define ADI_SPI2_CFG_BIT_RATE               (2000000u)

/*! SPI2 enable\n
    SPI configuration register: Bit[0]\n
    1 -  Enable SPI\n
    0 -  Disable SPI */
#define ADI_SPI2_CFG_ENABLE                         (0u)

/*! SPI2 clock phase mode\n
    SPI configuration register: Bit[2]\n
    1 - Serial clock pulses at the beginning of each serial bit transfer.\n
    0 - Serial clock pulses at the end of each serial bit transfer. */
#define ADI_SPI2_CFG_CLK_PHASE                      (0u)





/*! SPI2 clock polarity\n
    SPI configuration register: Bit[3]\n
    1 - Serial clock idles high.\n
    0 - Serial clock idles low. */
#define ADI_SPI2_CFG_CLK_POLARITY                   (0u)


/*! SPI2 wired OR mode\n
    SPI configuration register: Bit[4]\n
    1 - Enables open circuit output enable.\n
    0 - Normal output levels. */
#define ADI_SPI2_CFG_WIRED_OR                       (0u)


/*! SPI2 LSB/MSB\n
    SPI configuration register: Bit[5]\n
    1 - MSB transmitted first.\n
    0 - LSB transmitted first. */
#define ADI_SPI2_CFG_LSB_MSB                        (0u)


/*! SPI2 transfer initiate\n
    SPI configuration register: Bit[6]\n
    1 - SPI transfer is initiated with write to Tx FIFO register. Interrupts when Tx is empty.\n
    0 - SPI transfer is initiated with a read of the Rx FIFO register. Interrupts when Rx is full.*/
#define ADI_SPI2_CFG_TRANSFER_INITIATE              (0u)


/*! SPI2 Tx FIFO transfers zeros or last bit upon underflow\n
    SPI configuration register: Bit[7]\n
    1 - Tx FIFO sends zeros upon underflow.\n
    0 - Tx FIFO repeats last bit upon underflow. */
#define ADI_SPI2_CFG_TX_UNDERFLOW                   (0u)


/*! SPI2 Rx FIFO overflows with received data or data is discarded\n
    SPI configuration register: Bit[8]\n
    1 - Rx FIFO receives data upon overflow.\n
    0 - Rx FIFO discards received data upon overflow. */
#define ADI_SPI2_CFG_RX_OVERFLOW                    (0u)


/*! SPI2 slave mode MISO enable\n
    SPI configuration register: Bit[9]\n
    1 - MISO operates as normal in slave mode.\n
    0 - MISO is disabled in slave mode. */
#define ADI_SPI2_CFG_MISO_ENABLE                    (0u)


/*! SPI2 internal loopback enable\n
    SPI configuration register: Bit[10]\n
    1 - MISO and MOSI is loopbacked internally.\n
    0 - MISO and MOSI operates normally. */
#define ADI_SPI2_CFG_LOOPBACK                       (0u)

/*! SPI2 transfer and interrupt mode\n
    SPI configuration register: Bit[11]\n
    1 - SPI continuous transfers in which CS remains asserted until Tx is empty.\n
    0 - SPI disable continuous transfer, each transfer consists of 8 bits of data.*/
#define ADI_SPI2_CFG_CONTINUOUS                     (0u)

/*! SPI2 Rx FIFO flush enable\n
    SPI configuration register: Bit[12]\n
    1 - Rx FIFO is flushed and all rx data is ignored and no interrupts are generated.\n
    0 - Rx FIFO flush is disabled. */
#define ADI_SPI2_CFG_RX_FLUSH                       (0u)


/*! SPI2 Tx FIFO flush enable\n
    SPI configuration register: Bit[13]\n
    1 - Tx FIFO is flushed.\n
    0 - Tx FIFO flush is disabled. */
#define ADI_SPI2_CFG_TX_FLUSH                       (0u)


/*! Reset Mode for CSERR.       \n
    SPI2 configuration register: Bit[14]\n
    0 -  To  continue from where it stopped. SPI can receive the remaining bits
         when CS gets asserted and Cortex has to ignore the CSERR interrupt.\n
    1 -  To enable resetting the bit counter and  reset if there is  a
         CS error condition and the Cortex is expected to clear the SPI_EN bit.
*/
#define ADI_SPI2_CFG_CSERR_RESET                       (0u)


/*! SPI2 clock divide\n
    SPI baud rate selection register: Bit[0:5]\n
    Value between 0-63 that is used to divide the UCLK to generate
    the SPI serial clock. */
#define ADI_SPI2_CFG_CLK_DIV                        (0u)


/*! SPI2 high frequency mode\n
    SPI baud rate selection register: Bit[6]\n
    1 - High frequency mode enabled.\n
    0 - High frequency mode disabled. */
#define ADI_SPI2_CFG_HFM                            (0u)


/*! SPI2 reset mode for CSERR\n
    SPI baud rate selection register: Bit[7]\n
    1 - clear bit counter on CS error.\n
    0 - do not clear bit counter on CS error. */
#define ADI_SPI2_CFG_CS_ERR                         (0u)


/*! SPI2 CS interrupt\n
    SPI baud rate selection register: Bit[8]\n
    1 - In continuous mode, generate interrupt on CS.\n
    0 - In continuous mode, do not generate interrupt on CS. */
#define ADI_SPI2_CFG_CS_IRQ  

/*! @} */

/************** Macro validation *****************************/

#if ( ADI_SPI0_CFG_BIT_RATE > (13000000u) ) || \
    ( ADI_SPI0_CFG_BIT_RATE > (13000000u) ) || \
    ( ADI_SPI0_CFG_BIT_RATE > (13000000u) )
#error "Invalid configuration"
#endif

#if ( ADI_SPI0_CFG_ENABLE > 1u ) || \
    ( ADI_SPI1_CFG_ENABLE > 1u ) || \
    ( ADI_SPI2_CFG_ENABLE > 1u )
#error "Invalid configuration"
#endif

#if ( ADI_SPI0_CFG_CLK_PHASE > 1u ) || \
    ( ADI_SPI1_CFG_CLK_PHASE > 1u ) || \
    ( ADI_SPI2_CFG_CLK_PHASE > 1u )
#error "Invalid configuration"
#endif

#if ( ADI_SPI0_CFG_CLK_POLARITY > 1u ) || \
    ( ADI_SPI1_CFG_CLK_POLARITY > 1u ) || \
    ( ADI_SPI2_CFG_CLK_POLARITY > 1u )
#error "Invalid configuration"
#endif

#if ( ADI_SPI0_CFG_WIRED_OR > 1u ) || \
    ( ADI_SPI1_CFG_WIRED_OR > 1u ) || \
    ( ADI_SPI2_CFG_WIRED_OR > 1u )
#error "Invalid configuration"
#endif

#if ( ADI_SPI0_CFG_LSB_MSB > 1u ) || \
    ( ADI_SPI1_CFG_LSB_MSB > 1u ) || \
    ( ADI_SPI2_CFG_LSB_MSB > 1u )
#error "Invalid configuration"
#endif

#if ( ADI_SPI0_CFG_TRANSFER_INITIATE > 1u ) || \
    ( ADI_SPI1_CFG_TRANSFER_INITIATE > 1u ) || \
    ( ADI_SPI2_CFG_TRANSFER_INITIATE > 1u )
#error "Invalid configuration"
#endif

#if ( ADI_SPI0_CFG_TX_UNDERFLOW > 1u ) || \
    ( ADI_SPI1_CFG_TX_UNDERFLOW > 1u ) || \
    ( ADI_SPI2_CFG_TX_UNDERFLOW > 1u )
#error "Invalid configuration"
#endif

#if ( ADI_SPI0_CFG_RX_OVERFLOW > 1u ) || \
    ( ADI_SPI1_CFG_RX_OVERFLOW > 1u ) || \
    ( ADI_SPI2_CFG_RX_OVERFLOW > 1u )
#error "Invalid configuration"
#endif

#if ( ADI_SPI0_CFG_MISO_ENABLE > 1u ) || \
    ( ADI_SPI1_CFG_MISO_ENABLE > 1u ) || \
    ( ADI_SPI2_CFG_MISO_ENABLE > 1u )
#error "Invalid configuration"
#endif

#if ( ADI_SPI0_CFG_LOOPBACK > 1u ) || \
    ( ADI_SPI1_CFG_LOOPBACK > 1u ) || \
    ( ADI_SPI2_CFG_LOOPBACK > 1u )
#error "Invalid configuration"
#endif

#if ( ADI_SPI0_CFG_CONTINUOUS > 1u ) || \
    ( ADI_SPI1_CFG_CONTINUOUS > 1u ) || \
    ( ADI_SPI2_CFG_CONTINUOUS > 1u )
#error "Invalid configuration"
#endif

#if ( ADI_SPI0_CFG_RX_FLUSH > 1u ) || \
    ( ADI_SPI1_CFG_RX_FLUSH > 1u ) || \
    ( ADI_SPI2_CFG_RX_FLUSH > 1u )
#error "Invalid configuration"
#endif

#if ( ADI_SPI0_CFG_TX_FLUSH > 1u ) || \
    ( ADI_SPI1_CFG_TX_FLUSH > 1u ) || \
    ( ADI_SPI2_CFG_TX_FLUSH > 1u )
#error "Invalid configuration"
#endif


/*! @} */

#endif /* ADI_SPI_CONFIG_H__ */
