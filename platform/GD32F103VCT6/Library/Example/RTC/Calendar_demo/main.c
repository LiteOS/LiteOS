/**
  ******************************************************************************
  * @file    RTC/Calendar_demo/main.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Main program
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"
#include "rtc.h"

/* Private define ------------------------------------------------------------*/
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
    /* USART1 config */
    USART1_Config();

    /* NVIC config */
    NVIC_Configuration();

    printf( "\r\n This is a RTC demo...... \r\n" );

    if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
    {
        /* Backup data register value is not correct or not yet programmed
        (when the first time the program is executed) */
        printf("\r\nThis is a RTC demo!\r\n");
        printf("\r\n\n RTC not yet configured....");

        /* RTC Configuration */
        RTC_Configuration();

        printf("\r\n RTC configured....");

        /* Adjust time by values entred by the user on the hyperterminal */
        Time_Adjust();

        BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
    }
    else
    {
        /* Check if the Power On Reset flag is set */
        if (RCC_GetBitState(RCC_FLAG_POPDRST) != RESET)
        {
            printf("\r\n\n Power On Reset occurred....");
        }
        /* Check if the Pin Reset flag is set */
        else if (RCC_GetBitState(RCC_FLAG_EPRST) != RESET)
        {
            printf("\r\n\n External Reset occurred....");
        }

        printf("\r\n No need to configure RTC....");
        /* Wait for RTC registers synchronization */
        RTC_WaitRSF();

        /* Enable the RTC Second */
        RTC_INT_Enable(RTC_INT_SF, ENABLE);

        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitLWOFF();
    }

#ifdef RTCClockOutput_Enable
    /* Enable PWR and BKP clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);

    /* Disable the Tamper Pin */
    BKP_TamperPinCmd(DISABLE);

    /* Enable RTC Clock Output on Tamper Pin */
    BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);
#endif

    /* Clear reset flags */
    RCC_ClearBitState();

    /* Display time in infinite loop */
    Time_Show();

    while (1)
    {

    }
}

/**
  * @brief  Retarget the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
    /* Place your implementation of fputc here */
    USART_DataSend( USART1 , (uint8_t) ch );
    
    /* Loop until transmit data register is empty */
    while (USART_GetBitState( USART1 , USART_FLAG_TBE) == RESET)
    {
    }

    return ch;
}

/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
