/******************************************************************************
 * @file     dac.h
 * @version  V1.00
 * $Revision: 4 $
 * $Date: 14/09/08 12:31p $ 
 * @brief    NANO100 series DAC driver header file
 *
 * @note
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/ 
#ifndef __DAC_H__
#define __DAC_H__

#ifdef __cplusplus
extern "C"
{
#endif


/** @addtogroup NANO100_Device_Driver NANO100 Device Driver
  @{
*/

/** @addtogroup NANO100_DAC_Driver DAC Driver
  @{
*/


/** @addtogroup NANO100_DAC_EXPORTED_CONSTANTS DAC Exported Constants
  @{
*/
#define DAC_WRITE_DAT_TRIGGER  (0UL << DAC_CTL_DACLSEL_Pos)   ///< Write DACx_DAT trigger  \hideinitializer
#define DAC_PDMA_TRIGGER       (1UL << DAC_CTL_DACLSEL_Pos)   ///< PDMA trigger \hideinitializer
#define DAC_TIMER0_TRIGGER     (2UL << DAC_CTL_DACLSEL_Pos)   ///< Timer 0 trigger \hideinitializer
#define DAC_TIMER1_TRIGGER     (3UL << DAC_CTL_DACLSEL_Pos)   ///< Timer 1 trigger \hideinitializer
#define DAC_TIMER2_TRIGGER     (4UL << DAC_CTL_DACLSEL_Pos)   ///< Timer 2 trigger \hideinitializer
#define DAC_TIMER3_TRIGGER     (5UL << DAC_CTL_DACLSEL_Pos)   ///< Timer 3 trigger \hideinitializer

#define DAC_REFSEL_POWER       (0UL << DAC_COMCTL_REFSEL_Pos)  ///< DAC reference voltage source selection set to power  \hideinitializer
#define DAC_REFSEL_INT_VREF    (1UL << DAC_COMCTL_REFSEL_Pos)  ///< DAC reference voltage source selection set to Int_VREF  \hideinitializer
#define DAC_REFSEL_VREF        (2UL << DAC_COMCTL_REFSEL_Pos)  ///< DAC reference voltage source selection set to VREF  \hideinitializer

/*@}*/ /* end of group NANO100_DAC_EXPORTED_CONSTANTS */



/** @addtogroup NANO100_DAC_EXPORTED_FUNCTIONS DAC Exported Functions
  @{
*/

/**
  * @brief Write data for conversion. 
  * @param[in] dac Base address of DAC module.
  * @param[in] u32Ch DAC channel number, could be 0 or 1
  * @param[in] u32Data Decides the data for conversion, valid range are between 0~0xFFF.
  * @return None
  * \hideinitializer
  */
#define DAC_WRITE_DATA(dac, u32Ch, u32Data) do {\
                                                if(u32Ch) {\
                                                    DAC->DATA1 = u32Data;\
                                                } else {\
                                                    DAC->DATA0 = u32Data;\
                                                }\
                                            }while(0)


/**
  * @brief Enable DAC group mode 
  * @param[in] dac Base address of DAC module.
  * @return None
  * \hideinitializer
  */
#define DAC_ENABLE_GROUP_MODE(dac) (DAC->COMCTL |= DAC_COMCTL_DAC01GRP_Msk)

/**
  * @brief Disable DAC group mode 
  * @param[in] dac Base address of DAC module.
  * @return None
  * \hideinitializer
  */
#define DAC_DISABLE_GROUP_MODE(dac) (DAC->COMCTL &= ~DAC_COMCTL_DAC01GRP_Msk)

/**
  * @brief Get the busy state of DAC. 
  * @param[in] dac Base address of DAC module.
  * @param[in] u32Ch DAC channel number, could be 0 or 1
  * @return If DAC is able to convert or not.  
  * @retval 0 DAC is in idle state.
  * @retval 1 DAC is in busy state, or DAC is not in ready state.
  * @details If this macro returns 1, DAC is \b not in ready state. Ether DAC is busy or not in ready state.
  * \hideinitializer
  */
#define DAC_IS_BUSY(dac, u32Ch) (inp32(DAC_BASE + 0x8 + 0x10 * (u32Ch)) & DAC_STS_BUSY_Msk ? 1 : 0)
                                    

/**
  * @brief Get the interrupt flag of specified channel. 
  * @param[in] dac Base address of DAC module.
  * @param[in] u32Ch DAC channel number, could be 0 or 1
  * @return Returns the interrupt flag of selected channel.
  * @retval 0 DAC interrupt flag is not set.
  * @retval 1 DAC interrupt flag is set.
  * \hideinitializer
  */
#define DAC_GET_INT_FLAG(dac, u32Ch) (inp32(DAC_BASE + 0x8 + 0x10 * (u32Ch)) & DAC_STS_DACIFG_Msk ? 1 : 0)

/**
  * @brief This macro clear the interrupt status bit of specified channel. 
  * @param[in] dac Base address of DAC module.
  * @param[in] u32Ch DAC channel number, could be 0 or 1
  * @return None
  * \hideinitializer
  */
#define DAC_CLR_INT_FLAG(dac, u32Ch) do {\
                                         if(u32Ch)\
                                             DAC->STS1 = DAC_STS_DACIFG_Msk;\
                                         else\
                                             DAC->STS0 = DAC_STS_DACIFG_Msk;\
                                     }while(0)


/**
  * @brief Set the DAC reference voltage. This setting affects both DAC channel
  * @param[in] dac Base address of DAC module
  * @param[in] u32Ref The reference voltage selection. Valid values are:
  *                 - \ref DAC_REFSEL_POWER
  *                 - \ref DAC_REFSEL_INT_VREF
  *                 - \ref DAC_REFSEL_VREF
  * @return None
  * \hideinitializer
  */
#define DAC_SET_REF_VOLTAGE(dac, u32Ref) (DAC->COMCTL = ((DAC->COMCTL) & ~DAC_COMCTL_REFSEL_Msk) | u32Ref)

/**
  * @brief This macro enable the interrupt of specified channel. 
  * @param[in] dac Base address of DAC module.
  * @param[in] u32Ch DAC channel number, could be 0 or 1
  * @return None
  * \hideinitializer
  */
#define DAC_ENABLE_INT(dac, u32Ch) do {\
                                       if(u32Ch)\
                                           DAC->CTL1 |= DAC_CTL_DACIE_Msk;\
                                       else\
                                           DAC->CTL0 |= DAC_CTL_DACIE_Msk;\
                                   }while(0)

/**
  * @brief This macro disable the interrupt of specified channel.
  * @param[in] dac Base address of DAC module.
  * @param[in] u32Ch DAC channel number, could be 0 or 1
  * @return None
  * \hideinitializer
  */
#define DAC_DISABLE_INT(dac, u32Ch) do {\
                                        if(u32Ch)\
                                            DAC->CTL1 &= ~DAC_CTL_DACIE_Msk;\
                                        else\
                                            DAC->CTL0 &= ~DAC_CTL_DACIE_Msk;\
                                    }while(0)
                                   
void DAC_Open(DAC_T *dac, uint32_t u32Ch, uint32_t u32TrgSrc);
void DAC_Close(DAC_T *dac, uint32_t u32Ch);
int DAC_SetDelayTime(DAC_T *dac, uint32_t u32Delay);

/*@}*/ /* end of group NANO100_DAC_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO100_DAC_Driver */

/*@}*/ /* end of group NANO100_Device_Driver */

#ifdef __cplusplus
}
#endif

#endif //__DAC_H__

/*** (C) COPYRIGHT 2013 Nuvoton Technology Corp. ***/
