/****************************************************************************//**
 * @file     i2c.c
 * @version  V0.10
 * $Revision: 7 $
 * $Date: 15/06/05 5:04p $
 * @brief    NANO100 series I2C driver source file
 *
 * @note
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/

#include "nano100series.h"

/** @addtogroup NANO100_Device_Driver NANO100 Device Driver
  @{
*/

/** @addtogroup NANO100_I2C_Driver I2C Driver
  @{
*/


/** @addtogroup NANO100_I2C_EXPORTED_FUNCTIONS I2C Exported Functions
  @{
*/

/**
  * @brief This function make I2C module be ready and set the wanted bus clock.
  * @param[in] i2c is the base address of I2C module.
  * @param[in] u32BusClock is the target bus speed of I2C module.
  * @return Actual I2C bus clock frequency.
  */
uint32_t I2C_Open(I2C_T *i2c, uint32_t u32BusClock)
{
    uint32_t u32Div;

    u32Div = (uint32_t) (((SystemCoreClock * 10)/(u32BusClock * 4) + 5) / 10 - 1); /* Compute proper divider for I2C clock */
    i2c->DIV = u32Div;

    /* Enable I2C */
    i2c->CON |= I2C_CON_IPEN_Msk;

    return ( SystemCoreClock / ((u32Div+1)<<2) );
}

/**
  * @brief  This function closes the I2C module.
  * @param[in] i2c is the base address of I2C module.
  * @return none
  */
void I2C_Close(I2C_T *i2c)
{
    /* Reset I2C */
    if((uint32_t)i2c == I2C0_BASE) {
        SYS->IPRST_CTL2 |= SYS_IPRST_CTL2_I2C0_RST_Msk;
        SYS->IPRST_CTL2 &= ~SYS_IPRST_CTL2_I2C0_RST_Msk;
    } else {
        SYS->IPRST_CTL2 |= SYS_IPRST_CTL2_I2C1_RST_Msk;
        SYS->IPRST_CTL2 &= ~SYS_IPRST_CTL2_I2C1_RST_Msk;
    }

    /* Disable I2C */
    i2c->CON &= ~I2C_CON_IPEN_Msk;
}

/**
  * @brief This function clears the timeout flag.
  * @param[in] i2c is the base address of I2C module.
  * @return none
  */
void I2C_ClearTimeoutFlag(I2C_T *i2c)
{
    i2c->INTSTS |= I2C_INTSTS_TIF_Msk;
}

/**
  * @brief This function sets the control bit of the I2C module.
  * @param[in] i2c is the base address of I2C module.
  * @param[in] u8Start sets START bit to I2C module.
  * @param[in] u8Stop sets STOP bit to I2C module.
  * @param[in] u8Si sets SI bit to I2C module.
  * @param[in] u8Ack sets ACK bit to I2C module.
  * @return none
  */
void I2C_Trigger(I2C_T *i2c, uint8_t u8Start, uint8_t u8Stop, uint8_t u8Si, uint8_t u8Ack)
{
    uint32_t u32Reg = 0;
    uint32_t u32Val = i2c->CON & ~(I2C_STA | I2C_STO | I2C_AA);

    if (u8Start)
        u32Reg |= I2C_STA;
    if (u8Stop)
        u32Reg |= I2C_STO;
    if (u8Si)
        u32Reg |= I2C_SI;
    if (u8Ack)
        u32Reg |= I2C_AA;

    i2c->CON = u32Val | u32Reg;
}

/**
  * @brief This function disables the interrupt of I2C module.
  * @param[in] i2c is the base address of I2C module.
  * @return none
  */
void I2C_DisableInt(I2C_T *i2c)
{
    i2c->CON &= ~I2C_CON_INTEN_Msk;
}

/**
  * @brief This function enables the interrupt (EI bit) of I2C module.
  * @param[in] i2c is the base address of I2C module.
  * @return none
  */
void I2C_EnableInt(I2C_T *i2c)
{
    i2c->CON |= I2C_CON_INTEN_Msk;
}

/**
  * @brief This function returns the real bus clock of I2C module.
  * @param[in] i2c is the base address of I2C module.
  * @return Actual I2C bus clock frequency.
  */
uint32_t I2C_GetBusClockFreq(I2C_T *i2c)
{
    uint32_t u32Divider = i2c->DIV;

    return ( SystemCoreClock / ((u32Divider+1)<<2) );
}

/**
  * @brief This function sets bus frequency of I2C module.
  * @param[in] i2c is the base address of I2C module.
  * @param[in] u32BusClock is the target bus speed of I2C module.
  * @return Actual I2C bus clock frequency.
  */
uint32_t I2C_SetBusClockFreq(I2C_T *i2c, uint32_t u32BusClock)
{
    uint32_t u32Div;

    u32Div = (uint32_t) (((SystemCoreClock * 10)/(u32BusClock * 4) + 5) / 10 - 1); /* Compute proper divider for I2C clock */
    i2c->DIV = u32Div;

    return ( SystemCoreClock / ((u32Div+1)<<2) );
}

/**
  * @brief This function gets the interrupt flag of I2C module.
  * @param[in] i2c is the base address of I2C module.
  * @return Interrupt flag.
  * @retval 0 Flag is not set.
  * @retval 1 Flag is set.
  */
uint32_t I2C_GetIntFlag(I2C_T *i2c)
{
    return ( (i2c->INTSTS & I2C_INTSTS_INTSTS_Msk) == I2C_INTSTS_INTSTS_Msk ? 1:0 );
}

/**
  * @brief This function clears the interrupt flag of I2C module.
  * @param[in] i2c is the base address of I2C module.
  * @return none
  */
void I2C_ClearIntFlag(I2C_T *i2c)
{
    i2c->INTSTS |= I2C_INTSTS_INTSTS_Msk;
}

/**
  * @brief This function returns the status of I2C module.
  * @param[in] i2c is the base address of I2C module.
  * @return I2C status
  */
uint32_t I2C_GetStatus(I2C_T *i2c)
{
    return ( i2c->STATUS );
}

/**
  * @brief This function returns the data stored in data register of I2C module.
  * @param[in] i2c is the base address of I2C module.
  * @return Data.
  */
uint32_t I2C_GetData(I2C_T *i2c)
{
    return ( i2c->DATA );
}

/**
  * @brief This function writes the data to data register of I2C module.
  * @param[in] i2c is the base address of I2C module.
  * @param[in] u8Data is the data which will be write to data register of I2C module.
  * @return none
  */
void I2C_SetData(I2C_T *i2c, uint8_t u8Data)
{
    i2c->DATA = u8Data;
}

/**
  * @brief Configure slave address and enable GC mode.
  * @param[in] i2c is the base address of I2C module.
  * @param[in] u8SlaveNo is the set number of salve address.
  * @param[in] u8SlaveAddr is the slave address.
  * @param[in] u8GCMode GC mode enable or not. Valid values are:
  *              - \ref I2C_GCMODE_ENABLE
  *              - \ref I2C_GCMODE_DISABLE
  * @return none
  */
void I2C_SetSlaveAddr(I2C_T *i2c, uint8_t u8SlaveNo, uint8_t u8SlaveAddr, uint8_t u8GCMode)
{
    switch (u8SlaveNo) {
    case 0:
        i2c->SADDR0  = (u8SlaveAddr << 1) | u8GCMode;
        break;
    case 1:
        i2c->SADDR1  = (u8SlaveAddr << 1) | u8GCMode;
        break;
    }
}

/**
  * @brief Configure the mask of slave address. The corresponding address bit is "Don't Care".
  * @param[in] i2c is the base address of I2C module.
  * @param[in] u8SlaveNo is the set number of salve address.
  * @param[in] u8SlaveAddrMask is the slave address mask.
  * @return none
  */
void I2C_SetSlaveAddrMask(I2C_T *i2c, uint8_t u8SlaveNo, uint8_t u8SlaveAddrMask)
{
    switch (u8SlaveNo) {
    case 0:
        i2c->SAMASK0  = u8SlaveAddrMask << 1;
        break;
    case 1:
        i2c->SAMASK1  = u8SlaveAddrMask << 1;
        break;
    }
}

/**
  * @brief This function enables timeout function and configures DIV4 function to support long timeout.
  * @param[in] i2c is the base address of I2C module.
  * @param[in] u8LongTimeout Enable timeout counter input clock is divide by 4.
  * @return none
  */
void I2C_EnableTimeout(I2C_T *i2c, uint8_t u8LongTimeout)
{
    if(u8LongTimeout)
        i2c->TOUT |= I2C_TOUT_DIV4_Msk;
    else
        i2c->TOUT &= ~I2C_TOUT_DIV4_Msk;

    i2c->TOUT |= I2C_TOUT_TOUTEN_Msk;
}

/**
  * @brief This function disables timeout function.
  * @param[in] i2c is the base address of I2C module.
  * @return none
  */
void I2C_DisableTimeout(I2C_T *i2c)
{
    i2c->TOUT &= ~I2C_TOUT_TOUTEN_Msk;
}

/**
  * @brief This function enables the wakeup function of I2C module.
  * @param[in] i2c is the base address of I2C module.
  * @return none
  */
void I2C_EnableWakeup(I2C_T *i2c)
{
    i2c->WKUPCON |= I2C_WKUPCON_WKUPEN_Msk;
}

/**
  * @brief This function disables the wakeup function of I2C module.
  * @param[in] i2c is the base address of I2C module.
  * @return none
  */
void I2C_DisableWakeup(I2C_T *i2c)
{
    i2c->WKUPCON &= ~I2C_WKUPCON_WKUPEN_Msk;
}

/*@}*/ /* end of group NANO100_I2C_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO100_I2C_Driver */

/*@}*/ /* end of group NANO100_Device_Driver */

/*** (C) COPYRIGHT 2014 Nuvoton Technology Corp. ***/
