/**************************************************************************//**
 * @file     i2s.h
 * @version  V1.00
 * $Revision: 5 $
 * $Date: 15/06/08 4:59p $
 * @brief    Nano100 series I2S driver header file
 *
 * @note
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#ifndef __I2S_H__
#define __I2S_H__

#ifdef __cplusplus
extern "C"
{
#endif

/** @addtogroup NANO100_Device_Driver NANO100 Device Driver
  @{
*/

/** @addtogroup NANO100_I2S_Driver I2S Driver
  @{
*/

/** @addtogroup NANO100_I2S_EXPORTED_CONSTANTS I2S Exported Constants
  @{
*/
#define I2S_DATABIT_8           (0 << I2S_CTRL_WORDWIDTH_Pos)       /*!< I2S data width is 8-bit */
#define I2S_DATABIT_16          (1 << I2S_CTRL_WORDWIDTH_Pos)       /*!< I2S data width is 16-bit */
#define I2S_DATABIT_24          (2 << I2S_CTRL_WORDWIDTH_Pos)       /*!< I2S data width is 24-bit */
#define I2S_DATABIT_32          (3 << I2S_CTRL_WORDWIDTH_Pos)       /*!< I2S data width is 32-bit */

/* Audio Format */
#define I2S_MONO                I2S_CTRL_MONO_Msk                   /*!< Mono channel */
#define I2S_STEREO              0                                  /*!< Stereo channel */

/* I2S Data Format */
#define I2S_FORMAT_MSB          I2S_CTRL_FORMAT_Msk                 /*!< MSB data format */
#define I2S_FORMAT_I2S          0                                  /*!< I2S data format */

/* I2S Interface */
#define I2S_I2S                 0                                  /*!< I2S interface is selected */

/* I2S Operation mode */
#define I2S_MODE_SLAVE          I2S_CTRL_SLAVE_Msk                  /*!< As slave mode */
#define I2S_MODE_MASTER         0                                  /*!< As master mode */

/* I2S FIFO Threshold */
#define I2S_FIFO_TX_LEVEL_WORD_0    0                              /*!< TX threshold is 0 word */
#define I2S_FIFO_TX_LEVEL_WORD_1    (1 << I2S_CTRL_TXTH_Pos)        /*!< TX threshold is 1 word */
#define I2S_FIFO_TX_LEVEL_WORD_2    (2 << I2S_CTRL_TXTH_Pos)        /*!< TX threshold is 2 words */
#define I2S_FIFO_TX_LEVEL_WORD_3    (3 << I2S_CTRL_TXTH_Pos)        /*!< TX threshold is 3 words */
#define I2S_FIFO_TX_LEVEL_WORD_4    (4 << I2S_CTRL_TXTH_Pos)        /*!< TX threshold is 4 words */
#define I2S_FIFO_TX_LEVEL_WORD_5    (5 << I2S_CTRL_TXTH_Pos)        /*!< TX threshold is 5 words */
#define I2S_FIFO_TX_LEVEL_WORD_6    (6 << I2S_CTRL_TXTH_Pos)        /*!< TX threshold is 6 words */
#define I2S_FIFO_TX_LEVEL_WORD_7    (7 << I2S_CTRL_TXTH_Pos)        /*!< TX threshold is 7 words */

#define I2S_FIFO_RX_LEVEL_WORD_1    0                              /*!< RX threshold is 1 word */
#define I2S_FIFO_RX_LEVEL_WORD_2    (1 << I2S_CTRL_RXTH_Pos)        /*!< RX threshold is 2 words */
#define I2S_FIFO_RX_LEVEL_WORD_3    (2 << I2S_CTRL_RXTH_Pos)        /*!< RX threshold is 3 words */
#define I2S_FIFO_RX_LEVEL_WORD_4    (3 << I2S_CTRL_RXTH_Pos)        /*!< RX threshold is 4 words */
#define I2S_FIFO_RX_LEVEL_WORD_5    (4 << I2S_CTRL_RXTH_Pos)        /*!< RX threshold is 5 words */
#define I2S_FIFO_RX_LEVEL_WORD_6    (5 << I2S_CTRL_RXTH_Pos)        /*!< RX threshold is 6 words */
#define I2S_FIFO_RX_LEVEL_WORD_7    (6 << I2S_CTRL_RXTH_Pos)        /*!< RX threshold is 7 words */
#define I2S_FIFO_RX_LEVEL_WORD_8    (7 << I2S_CTRL_RXTH_Pos)        /*!< RX threshold is 8 words */

/* I2S Record Channel */
#define I2S_MONO_RIGHT          0                                  /*!< Record mono right channel */
#define I2S_MONO_LEFT           I2S_CTRL_RXLCH_Msk                  /*!< Record mono left channel */

/* I2S Channel */
#define I2S_RIGHT               0                                  /*!< Select right channel */
#define I2S_LEFT                1                                  /*!< Select left channel */

/*@}*/ /* end of group NANO100_I2S_EXPORTED_CONSTANTS */

/** @addtogroup NANO100_I2S_EXPORTED_FUNCTIONS I2S Exported Functions
  @{
*/
/**
  * @brief  Enable zero cross detect function.
  * @param[in] i2s is the base address of I2S module.
  * @param[in] u32ChMask is the mask for left or right channel. Valid values are:
  *                    - \ref I2S_RIGHT
  *                    - \ref I2S_LEFT
  * @return none
  * \hideinitializer
  */
static __INLINE void I2S_ENABLE_TX_ZCD(I2S_T *i2s, uint32_t u32ChMask)
{
    if(u32ChMask == I2S_RIGHT)
        i2s->CTRL |= I2S_CTRL_RCHZCEN_Msk;
    else
        i2s->CTRL |= I2S_CTRL_LCHZCEN_Msk;
}

/**
  * @brief  Disable zero cross detect function.
  * @param[in] i2s is the base address of I2S module.
  * @param[in] u32ChMask is the mask for left or right channel. Valid values are:
  *                    - \ref I2S_RIGHT
  *                    - \ref I2S_LEFT
  * @return none
  * \hideinitializer
  */
static __INLINE void I2S_DISABLE_TX_ZCD(I2S_T *i2s, uint32_t u32ChMask)
{
    if(u32ChMask == I2S_RIGHT)
        i2s->CTRL &= ~I2S_CTRL_RCHZCEN_Msk;
    else
        i2s->CTRL &= ~I2S_CTRL_LCHZCEN_Msk;
}

/**
  * @brief  Enable I2S Tx DMA function. I2S requests DMA to transfer data to Tx FIFO.
  * @param[in] i2s is the base address of I2S module.
  * @return none
  * \hideinitializer
  */
#define I2S_ENABLE_TXDMA(i2s) ( (i2s)->CTRL |= I2S_CTRL_TXDMA_Msk )

/**
  * @brief  Disable I2S Tx DMA function. I2S requests DMA to transfer data to Tx FIFO.
  * @param[in] i2s is the base address of I2S module.
  * @return none
  * \hideinitializer
  */
#define I2S_DISABLE_TXDMA(i2s) ( (i2s)->CTRL &= ~I2S_CTRL_TXDMA_Msk )

/**
  * @brief  Enable I2S Rx DMA function. I2S requests DMA to transfer data from Rx FIFO.
  * @param[in] i2s is the base address of I2S module.
  * @return none
  * \hideinitializer
  */
#define I2S_ENABLE_RXDMA(i2s) ( (i2s)->CTRL |= I2S_CTRL_RXDMA_Msk )

/**
  * @brief  Disable I2S Rx DMA function. I2S requests DMA to transfer data from Rx FIFO.
  * @param[in] i2s is the base address of I2S module.
  * @return none
  * \hideinitializer
  */
#define I2S_DISABLE_RXDMA(i2s) ( (i2s)->CTRL &= ~I2S_CTRL_RXDMA_Msk )

/**
  * @brief  Enable I2S Tx function .
  * @param[in] i2s is the base address of I2S module.
  * @return none
  * \hideinitializer
  */
#define I2S_ENABLE_TX(i2s)  ( (i2s)->CTRL |= I2S_CTRL_TXEN_Msk )

/**
  * @brief  Disable I2S Tx function .
  * @param[in] i2s is the base address of I2S module.
  * @return none
  * \hideinitializer
  */
#define I2S_DISABLE_TX(i2s) ( (i2s)->CTRL &= ~I2S_CTRL_TXEN_Msk )

/**
  * @brief  Enable I2S Rx function .
  * @param[in] i2s is the base address of I2S module.
  * @return none
  * \hideinitializer
  */
#define I2S_ENABLE_RX(i2s) ( (i2s)->CTRL |= I2S_CTRL_RXEN_Msk )

/**
  * @brief  Disable I2S Rx function .
  * @param[in] i2s is the base address of I2S module.
  * @return none
  * \hideinitializer
  */
#define I2S_DISABLE_RX(i2s) ( (i2s)->CTRL &= ~I2S_CTRL_RXEN_Msk )

/**
  * @brief  Enable Tx Mute function .
  * @param[in] i2s is the base address of I2S module.
  * @return none
  * \hideinitializer
  */
#define I2S_ENABLE_TX_MUTE(i2s) ( (i2s)->CTRL |= I2S_CTRL_MUTE_Msk )

/**
  * @brief  Disable Tx Mute function .
  * @param[in] i2s is the base address of I2S module.
  * @return none
  * \hideinitializer
  */
#define I2S_DISABLE_TX_MUTE(i2s) ( (i2s)->CTRL &= ~I2S_CTRL_MUTE_Msk )

/**
  * @brief  Clear Tx FIFO. Internal pointer is reset to FIFO start point.
  * @param[in] i2s is the base address of I2S module.
  * @return none
  * \hideinitializer
  */
#define I2S_CLR_TX_FIFO(i2s) ( (i2s)->CTRL |= I2S_CTRL_CLR_TXFIFO_Msk )

/**
  * @brief  Clear Rx FIFO. Internal pointer is reset to FIFO start point.
  * @param[in] i2s is the base address of I2S module.
  * @return none
  * \hideinitializer
  */
#define I2S_CLR_RX_FIFO(i2s) ( (i2s)->CTRL |= I2S_CTRL_CLR_RXFIFO_Msk )

/**
  * @brief  This function sets the recording source channel when mono mode is used.
  * @param[in] i2s is the base address of I2S module.
  * @param[in] u32Ch left or right channel. Valid values are:
  *                - \ref I2S_MONO_LEFT
  *                - \ref I2S_MONO_RIGHT
  * @return none
  * \hideinitializer
  */
#define I2S_SET_MONO_RX_CHANNEL(i2s, u32Ch) ( u32Ch == I2S_MONO_LEFT ? ((i2s)->CTRL |= I2S_CTRL_RXLCH_Msk) : ((i2s)->CTRL &= ~I2S_CTRL_RXLCH_Msk) )

/**
  * @brief  Write data to I2S Tx FIFO.
  * @param[in] i2s is the base address of I2S module.
  * @param[in] u32Data The data written to FIFO.
  * @return none
  * \hideinitializer
  */
#define I2S_WRITE_TX_FIFO(i2s, u32Data) ( (i2s)->TXFIFO = u32Data )

/**
  * @brief  Read Rx FIFO.
  * @param[in] i2s is the base address of I2S module.
  * @return Data in Rx FIFO.
  * \hideinitializer
  */
#define I2S_READ_RX_FIFO(i2s) ( (i2s)->RXFIFO )

/**
  * @brief  This function gets the interrupt flag according to the mask parameter.
  * @param[in] i2s is the base address of I2S module.
  * @param[in] u32Mask is the mask for the all interrupt flags.
  * @return The masked bit value of interrupt flag.
  * \hideinitializer
  */
#define I2S_GET_INT_FLAG(i2s, u32Mask) ((i2s)->STATUS & (u32Mask))

/**
  * @brief  This function clears the interrupt flag according to the mask parameter.
  * @param[in] i2s is the base address of I2S module.
  * @param[in] u32Mask is the mask for the all interrupt flags.
  * @return none
  * \hideinitializer
  */
#define I2S_CLR_INT_FLAG(i2s, u32Mask) ( (i2s)->STATUS |= (u32Mask) )

/**
  * @brief  Get transmit FIFO level
  * @param[in] i2s is the base address of I2S module.
  * @return FIFO level
  * \hideinitializer
  */
#define I2S_GET_TX_FIFO_LEVEL(i2s) ((((i2s)->STATUS & I2S_STATUS_TX_LEVEL_Msk) >> I2S_STATUS_TX_LEVEL_Pos) & 0xF)

/**
  * @brief  Get receive FIFO level
  * @param[in] i2s is the base address of I2S module.
  * @return FIFO level
  * \hideinitializer
  */
#define I2S_GET_RX_FIFO_LEVEL(i2s) ((((i2s)->STATUS & I2S_STATUS_RX_LEVEL_Msk) >> I2S_STATUS_RX_LEVEL_Pos) & 0xF)

uint32_t I2S_Open(I2S_T *i2s, uint32_t u32MasterSlave, uint32_t u32SampleRate, uint32_t u32WordWidth, uint32_t u32Channels, uint32_t u32DataFormat, uint32_t u32AudioInterface);
void I2S_Close(I2S_T *i2s);
void I2S_EnableInt(I2S_T *i2s, uint32_t u32Mask);
void I2S_DisableInt(I2S_T *i2s, uint32_t u32Mask);
uint32_t I2S_EnableMCLK(I2S_T *i2s, uint32_t u32BusClock);
void I2S_DisableMCLK(I2S_T *i2s);
void I2S_SetFIFO(I2S_T *i2s, uint32_t u32TxThreshold, uint32_t u32RxThreshold);

/*@}*/ /* end of group NANO100_I2S_EXPORTED_FUNCTIONS */


/*@}*/ /* end of group NANO100_I2S_Driver */

/*@}*/ /* end of group NANO100_Device_Driver */

#ifdef __cplusplus
}
#endif

#endif //__I2S_H__

/*** (C) COPYRIGHT 2014 Nuvoton Technology Corp. ***/

