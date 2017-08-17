/**
  ******************************************************************************
  * @file    I2C/Interrupt_MasterReceiver&SlaveTransmitter/I2C1_IE.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   I2C1_MasterReceiver interrupt program.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x_i2c.h"
#include "I2C_IE.h"

/* Private variables ---------------------------------------------------------*/
uint32_t    event1;

/* Private function ----------------------------------------------------------*/
/**
  * @brief  Handle I2C1 event interrupt request.
  * @param  None
  * @retval None
  */
void I2C1_EventIRQ_Handler(void)
{
    if(I2C_GetBitState(I2C1, I2C_FLAG_SBSEND) )
    {
        /* The master sends slave address */
        I2C_AddressingDevice_7bit(I2C1, I2C2_SLAVE_ADDRESS7, I2C_DIRECTION_RECEIVER);
    }
    else if(I2C_GetBitState(I2C1, I2C_FLAG_ADDSEND))
    {
        if((I2C_nBytes==1)||(I2C_nBytes==2))
        {
            /* Clear the ACKEN before the ADDSEND is cleared */
            I2C_Acknowledge_Enable(I2C1, DISABLE);

            /* Clear the ADDSEND bit */
            event1=I2C_GetCurrentState(I2C1);
        }
        else
        {
            /* Clear the ADDSEND bit */
            event1=I2C_GetCurrentState(I2C1);
        }
    }
    else if(I2C_GetBitState(I2C1, I2C_FLAG_RBNE))
    {
        if(I2C_nBytes>0)
        {
            if(I2C_nBytes==3)
            {
                /* Wait until the second last data byte is received into the shift register */
                while(!I2C_GetBitState(I2C1, I2C_FLAG_BTC));

              /* Send a NACK for the last data byte */
                I2C_Acknowledge_Enable(I2C1, DISABLE);
            }
              /* Read a data byte from I2C_DTR*/
            *I2C_ReadBuffer++ = I2C_ReceiveData(I2C1);
            I2C_nBytes--;
            if(I2C_nBytes==0)
            {
                I2C_StopOnBus_Enable(I2C1, ENABLE);
                Status=end;
                I2C_Acknowledge_Enable(I2C1, ENABLE);
                I2C_NACKPosition_Enable(I2C1,I2C_NACKPOSITION_CURRENT);
                I2C_INTConfig(I2C1,I2C_INT_EIE | I2C_INT_EE | I2C_INT_BIE, DISABLE);
            }
        }
    }
}

/**
  * @brief  Handle I2C1 error interrupt request.
  * @param  None
  * @retval None
  */
void I2C1_ErrorIRQ_Handler(void)
{
    /* No acknowledge received */
    if(I2C_GetBitState(I2C1, I2C_FLAG_AE))
    {
        I2C_ClearBitState(I2C1, I2C_FLAG_AE);
    }

    /* SMBus Alert */
    if(I2C_GetBitState(I2C1, I2C_FLAG_SMBALTS))
    {
        I2C_ClearBitState(I2C1, I2C_FLAG_SMBALTS);
    }

    /* Bus timeout in SMBus mode */
    if(I2C_GetBitState(I2C1, I2C_FLAG_SMBTO))
    {
        I2C_ClearBitState(I2C1, I2C_FLAG_SMBTO);
    }

    /* Over-run or under-run when SCL stretch is disabled */
    if(I2C_GetBitState(I2C1, I2C_FLAG_RXORE))
    {
        I2C_ClearBitState(I2C1, I2C_FLAG_RXORE);
    }

    /* Arbitration lost */
    if(I2C_GetBitState(I2C1, I2C_FLAG_LOSTARB))
    {
        I2C_ClearBitState(I2C1, I2C_FLAG_LOSTARB);
    }

    /* Bus error */
    if(I2C_GetBitState(I2C1, I2C_FLAG_BE))
    {
        I2C_ClearBitState(I2C1, I2C_FLAG_BE);
    }

    /* CRC value doesn't match */
    if(I2C_GetBitState(I2C1, I2C_FLAG_PECE))
    {
        I2C_ClearBitState(I2C1, I2C_FLAG_PECE);
    }

    /* Disable the error interrupt */
    I2C_INTConfig(I2C1,I2C_INT_EIE | I2C_INT_EE | I2C_INT_BIE, DISABLE);
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
