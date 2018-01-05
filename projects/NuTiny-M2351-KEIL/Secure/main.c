/**************************************************************************//**
 * @file     main.c
 * @version  V1.00
 * @brief    Secure sample code for TrustZone
 *
 * @note
 * Copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/

#include <arm_cmse.h>
#include <stdio.h>
#include "M2351.h"                      /* Device header */
#include "partition_M2351.h"


#define NEXT_BOOT_BASE  0x10010000
#define JUMP_HERE       0xe7fee7ff      /* Instruction Code of "B ." */

/* typedef for NonSecure callback functions */
typedef __NONSECURE_CALL int32_t (*NonSecure_funcptr)(uint32_t);
typedef int32_t (*Secure_funcptr)(uint32_t);


/*----------------------------------------------------------------------------
  Secure functions exported to NonSecure application
  Must place in Non-secure Callable
 *----------------------------------------------------------------------------*/
__NONSECURE_ENTRY
int32_t Secure_func(void)
{
    printf("Secure NSC func\n");
    PC14 ^= 1; // Toggle LED   
    return 1;
}

void SYS_Init(void);
void DEBUG_PORT_Init(void);
void Boot_Init(uint32_t u32BootBase);


/*----------------------------------------------------------------------------
    Boot_Init function is used to jump to next boot code.
 *----------------------------------------------------------------------------*/
void Boot_Init(uint32_t u32BootBase)
{
    NonSecure_funcptr fp;

    /* SCB_NS.VTOR points to the Non-secure vector table base address. */
    SCB_NS->VTOR = u32BootBase;

    /* 1st Entry in the vector table is the Non-secure Main Stack Pointer. */
    __TZ_set_MSP_NS(*((uint32_t *)SCB_NS->VTOR));      /* Set up MSP in Non-secure code */

    /* 2nd entry contains the address of the Reset_Handler (CMSIS-CORE) function */
    fp = ((NonSecure_funcptr)(*(((uint32_t *)SCB_NS->VTOR) + 1)));

    /* Clear the LSB of the function address to indicate the function-call
       will cause a state switch from Secure to Non-secure */
    fp = cmse_nsfptr_create(fp);

    /* Check if the Reset_Handler address is in Non-secure space */
    if(cmse_is_nsfptr(fp) && (((uint32_t)fp & 0xf0000000) == 0x10000000))
    {
        printf("Execute non-secure code ...\n");
        fp(0); /* Non-secure function call */
    }
    else
    {
        /* Something went wrong */
        printf("No code in non-secure region!\n");
        printf("CPU will halted at non-secure state\n");
        
        /* Set nonsecure MSP in nonsecure region */
        __TZ_set_MSP_NS(NON_SECURE_SRAM_BASE+512); 

        /* Try to halted in non-secure state (SRAM) */
        M32(NON_SECURE_SRAM_BASE) = JUMP_HERE;
        fp = (NonSecure_funcptr)(NON_SECURE_SRAM_BASE+1);
        fp(0);


        while(1);
    }
}



/*----------------------------------------------------------------------------
  Main function
 *----------------------------------------------------------------------------*/
int main(void)
{
		int i = 0;
    SYS_UnlockReg();
    SYS_Init();

    /* UART is configured as debug port */
    DEBUG_PORT_Init();

    printf("Secure is running ...\n");
	    // Init PC14
    PC->MODE = GPIO_MODE_OUTPUT << 14*2;
    PC14 = 1;

    
    // Blinky for 5 times;
    for(i=0;i<10;i++)
    {
        CLK_SysTickDelay(100000);
        PC14 ^= 1;
    }
    
    PC14 = 1;
    Boot_Init(NEXT_BOOT_BASE);
    
    
    do{__WFI();}while(1);
}


void SYS_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Enable PLL */
    CLK->PLLCTL = CLK_PLLCTL_48MHz_HIRC;
    /* Waiting for PLL stable */
    while((CLK->STATUS & CLK_STATUS_PLLSTB_Msk) == 0);

    /* Set HCLK divider to 1 */
    CLK->CLKDIV0 = (CLK->CLKDIV0 & (~CLK_CLKDIV0_HCLKDIV_Msk));

    /* Switch HCLK clock source to PLL */
    CLK->CLKSEL0 = (CLK->CLKSEL0 & (~CLK_CLKSEL0_HCLKSEL_Msk)) | CLK_CLKSEL0_HCLKSEL_PLL;

    /* Enable UART clock */
    CLK->APBCLK0 |= CLK_APBCLK0_UART0CKEN_Msk;

    /* Select UART clock source */
    CLK->CLKSEL1 = (CLK->CLKSEL1 & (~CLK_CLKSEL1_UART0SEL_Msk)) | CLK_CLKSEL1_UART0SEL_HIRC;
    
    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate PllClock, SystemCoreClock and CycylesPerUs automatically. */
    //SystemCoreClockUpdate();
    PllClock        = 48000000;            // PLL
    SystemCoreClock = 48000000 / 1;        // HCLK
    CyclesPerUs     = 48000000 / 1000000;  // For SYS_SysTickDelay()

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Set multi-function pins for RXD and TXD */
    SYS->GPD_MFPL &= ~(UART0_RXD_PD0_Msk | UART0_TXD_PD1_Msk);
    SYS->GPD_MFPL |= UART0_RXD_PD0 | UART0_TXD_PD1;


    /*---------------------------------------------------------------------------------------------------------*/
    /* Init for nonsecure code                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
    CLK->APBCLK0 |= CLK_APBCLK0_UART1CKEN_Msk;
    CLK->CLKSEL1 = (CLK->CLKSEL1 & (~CLK_CLKSEL1_UART1SEL_Msk)) | CLK_CLKSEL1_UART1SEL_HIRC;
    SYS->GPA_MFPL &= ~(UART1_TXD_PA0_Msk | UART1_RXD_PA1_Msk);
    SYS->GPA_MFPL |= UART1_TXD_PA0 | UART1_RXD_PA1;
    

}
void DEBUG_PORT_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init UART                                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/

    DEBUG_PORT->BAUD = UART_BAUD_MODE2 | UART_BAUD_MODE2_DIVIDER(__HIRC, 115200);
    DEBUG_PORT->LINE = UART_WORD_LEN_8 | UART_PARITY_NONE | UART_STOP_BIT_1;
}
