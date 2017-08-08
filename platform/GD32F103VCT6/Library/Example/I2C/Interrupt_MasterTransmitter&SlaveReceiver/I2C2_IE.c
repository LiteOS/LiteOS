/**
  ******************************************************************************
  * @file    I2C/Interrupt_MasterTransmitter&SlaveReceiver/I2C2_IE.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   I2C2_SlaveReceiver interrupt program.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x_i2c.h"
#include "I2C_IE.h"

/* Private variables ---------------------------------------------------------*/
uint32_t    event2;

/**
  * @brief  Handle I2C2 event interrupt request.
  * @param  None
  * @retval None
  */
void I2C2_EventIRQ_Handler(void)
{
    if(I2C_GetBitState(I2C2, I2C_FLAG_ADDSEND))
    {
        /* clear ADDSEND bit by reading I2C_STR1 and I2C_STR2 */
        event2 = I2C_GetCurrentState(I2C2);
    }
    else if(I2C_GetBitState(I2C2, I2C_FLAG_RBNE))
    {
        /* if reception data register is not empty ,I2C2 will read a data from I2C_DTR */
        *I2C_ReadBuffer++ = I2C_ReceiveData(I2C2);
    }
    else if(I2C_GetBitState(I2C2, I2C_FLAG_STPSEND))
    {
        Status=end;
        /* Clear the STPDET bit */
        I2C_Enable(I2C2,ENABLE);
        /* Disable I2C2 interrupt */
        I2C_INTConfig(I2C2,I2C_INT_EIE | I2C_INT_EE | I2C_INT_BIE, DISABLE);
    }
}

/**
  * @brief  Handle I2C2 error interrupt request.
  * @param  None
  * @retval None
  */
void I2C2_ErrorIRQ_Handler(void)
{
    /* No acknowledge received */
    if(I2C_GetBitState(I2C2, I2C_FLAG_AE))
    {
        I2C_ClearBitState(I2C2, I2C_FLAG_AE);
    }

    /* SMBus Alert */
    if(I2C_GetBitState(I2C2, I2C_FLAG_SMBALTS))
    {
        I2C_ClearBitState(I2C2, I2C_FLAG_SMBALTS);
    }

    /* Bus timeout in SMBus mode */
    if(I2C_GetBitState(I2C2, I2C_FLAG_SMBTO))
    {
        I2C_ClearBitState(I2C2, I2C_FLAG_SMBTO);
    }

    /* Over-run or under-run when SCL stretch is disabled */
    if(I2C_GetBitState(I2C2, I2C_FLAG_RXORE))
    {
       I2C_ClearBitState(I2C2, I2C_FLAG_RXORE);
    }

    /* Arbitration lost */
    if(I2C_GetBitState(I2C2, I2C_FLAG_LOSTARB))
    {
        I2C_ClearBitState(I2C2, I2C_FLAG_LOSTARB);
    }

    /* Bus error */
    if(I2C_GetBitState(I2C2, I2C_FLAG_BE))
    {
        I2C_ClearBitState(I2C2, I2C_FLAG_BE);
    }

    /* CRC value doesn't match */
    if(I2C_GetBitState(I2C2, I2C_FLAG_PECE))
    {
        I2C_ClearBitState(I2C2, I2C_FLAG_PECE);
    }

    /* Disable the error interrupt */
    I2C_INTConfig(I2C2,I2C_INT_EIE | I2C_INT_EE | I2C_INT_BIE, DISABLE);
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
