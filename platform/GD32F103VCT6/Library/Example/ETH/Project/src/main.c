/**
  ******************************************************************************
  * @file    ETH/Project/src/main.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Main program body.  
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"
#include "netconf.h"
#include "platform_cfg.h"
#include "helloworld.h"
#include "HelloGigaDevice.h"
#include <stdio.h>

/* Private define ------------------------------------------------------------*/
#define SYSTEMTICK_PERIOD_MS  10
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Private variables ---------------------------------------------------------*/
__IO uint32_t LocalTime = 0; /* This variable is used to create a time reference incremented by 10ms */
uint32_t timingdelay;

/* Private function prototypes -----------------------------------------------*/
void System_Periodic_Handle(void);
void Delay_Count(__IO uint32_t nCount);
void System_Setup(void);
void Show_Msg(void);
u16 Show_ETH_PHY(u16 PHYRegAddr);
void Check_ETH_PHY(void);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Delay.
  * @param  None
  * @retval None
  */
void Delay_Count(__IO uint32_t nCount)
{
    for (; nCount != 0; nCount--);
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{ 
    /* Setup GD32 system (clocks, Ethernet, GPIO, NVIC)
       and GD32107C-EVAL resources */
    System_Setup();

    /* Initilaize the LwIP satck */
    LwIP_Init();

    Check_ETH_PHY(); 
    
    Show_Msg();
    
    /* Initilaize the HelloWorld module */
    HelloWorld_init();
  
    /* Initilaize the helloGigadevice module  telnet 8000 */
    HelloGigaDevice_init();
    
    /* Infinite loop */
    while (1)
    { 
        /* Periodic tasks */
        System_Periodic_Handle();
    }
}

/**
  * @brief  Update the system local time
  * @param  None
  * @retval None
  */
void Time_Update(void)
{
    LocalTime += SYSTEMTICK_PERIOD_MS;
}


/**
  * @brief  Handle the periodic tasks of the system
  * @param  None
  * @retval None
  */
void System_Periodic_Handle(void)
{
    /* Manage the IP address setting */
    Display_Periodic_Handle(LocalTime);
  
    /* LwIP periodic services are done here */
    LwIP_Periodic_Handle(LocalTime);
}   

/**
  * @brief  Show the configuration information of example
  * @param  None
  * @retval None
  */
void Show_Msg(void)
{
    printf("\r\n\n\n ================ GD32  %s configured ==============", "USART1");
    printf("\n\r GD32 Connectivity Line Device\n\r");
    printf("\n\r TCPServer;telnet;ping;....\n\r");
    printf("\n\r IP address is: %d.%d.%d.%d\n\r", IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
}

/**
  * @brief  Show the phy register value
  * @param  None
  * @retval None
  */
u16 Show_ETH_PHY(u16 PHYRegAddr)
{
    u16 PHYRegData;
    PHYRegData = ETH_GetPHYRegisterValue(PHY_ADDRESS,PHYRegAddr);
    printf("\n\rETH_ReadPHYRegister(1,%d):0x%X", PHYRegAddr, PHYRegData);
    return PHYRegData;
}

/**
  * @brief  Check the phy register value
  * @param  None
  * @retval None
  */
void Check_ETH_PHY(void)
{
    uint16_t regvalue;
    Show_ETH_PHY(0);
    Show_ETH_PHY(1);
    Show_ETH_PHY(2);
    Show_ETH_PHY(3);
    Show_ETH_PHY(4);
    Show_ETH_PHY(5);
    Show_ETH_PHY(6);
    regvalue = Show_ETH_PHY(16);
    Show_ETH_PHY(23);
    if(regvalue & 0x0002)
    {  
        /* Set Ethernet speed to 10M following the autonegotiation */    
        printf("\n\r==>ETH_Speed_10M!");      
    }   
    else
    {   
        /* Set Ethernet speed to 100M following the autonegotiation */ 
        printf("\n\r==>ETH_Speed_100M!");       
    } 
    /* Configure the MAC with the Duplex Mode fixed by the autonegotiation process */
    if((regvalue & 0x0004) != (uint32_t)RESET)
    {
        /* Set Ethernet duplex mode to FullDuplex following the autonegotiation */
        printf("\n\r==>ETH_Mode_FullDuplex!");     
    }
    else
    {
        /* Set Ethernet duplex mode to HalfDuplex following the autonegotiation */
        printf("\n\r==>ETH_Mode_HalfDuplex!");            
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
    /* Loop until the end of transmission */
    while (USART_GetBitState(USART1, USART_FLAG_TC) == RESET)
    {}
  
    /* Write a character to the USART */
    USART_DataSend(USART1, (uint8_t) ch);

    return ch;
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
