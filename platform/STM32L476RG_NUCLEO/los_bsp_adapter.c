/*
    here we can include some standard header file
*/
#include <stdio.h>
#include <string.h>
/*
    here include some special hearder file you need
*/
//#include "cmsis_os.h"

#include "los_bsp_adapter.h"
#include "los_bsp_led.h"
#include "los_bsp_key.h"
#include "los_bsp_uart.h"


#ifdef LOS_STM32L476xx
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo.h"
#endif


/* while use bsp code to start system tick, don't use LOS header */
#define INCLUDE_LOS_HEADER
#ifdef INCLUDE_LOS_HEADER
#include "los_tick.ph"
#include "los_base.h"
#include "los_task.ph"
#include "los_swtmr.h"
#include "los_hwi.h"
#endif

/*****************************************************************************
    global var
 *****************************************************************************/
/* current system default HZ , should be set according to the microchip */
#ifdef LOS_STM32L476xx
const unsigned int sys_clk_freq = 80000000;
#else
const unsigned int sys_clk_freq = 16000000;
#endif

/* tick count per second , don't change this */
const unsigned int tick_per_second = 1000;
static unsigned int g_ucycle_per_tick = 0;

/*
    if g_use_ram_vect == 1, we should use sct file STM32F429I-LiteOS.sct
    and we can use LOS_HwiCreate(), LOS_HwiDelete() dynamically regist a irq func
    if g_use_ram_vect == 0, we use default vector table in rom start at address 0x00000000
*/
const unsigned char g_use_ram_vect = 0;

/*****************************************************************************
    LOS function extern 
 *****************************************************************************/
extern void LOS_SetTickSycle(unsigned int);
extern void LOS_TickHandler(void);
extern unsigned int osGetVectorAddr(void);

#ifdef LOS_STM32L476xx
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 80000000
  *            HCLK(Hz)                       = 80000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 1
  *            PLL_N                          = 40
  *            PLL_R                          = 2
  *            PLL_P                          = 7
  *            PLL_Q                          = 4
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};

    /* MSI is enabled after System reset, activate PLL with MSI as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
    RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 40;
    RCC_OscInitStruct.PLL.PLLR = 2;
    RCC_OscInitStruct.PLL.PLLP = 7;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        /* Initialization Error */
        while(1);
    }

    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
    {
        /* Initialization Error */
        while(1);
    }
}
#endif

/*****************************************************************************
 Function    : osTickStart
 Description : Set system tick reqister, and start system tick exception
              Note that This function is called by LOS_Start() In LiteOS kernel.
              so, please don't init system tick before LiteOS kernel Inited
              in your own system.
 Input       : None
 Output      : None
 Return      : LOS_OK
 *****************************************************************************/
UINT32 osTickStart(void)
{
    unsigned int uwRet = 0;
    
    /* This code section LOS need, so don't change it */
    g_ucycle_per_tick = sys_clk_freq / tick_per_second;
    LOS_SetTickSycle(g_ucycle_per_tick);
    
    /*
      Set system tick relaod register valude, current register valude and start
      system tick exception.
      Note: here can be replaced by some function , for example in Stm32 bsp
      you can just call SysTick_Config(sys_clk_freq/tick_per_second);
    */
#ifdef LOS_STM32L476xx
    SystemClock_Config();
    SysTick_Config(g_ucycle_per_tick);
#else
    *(volatile UINT32 *)OS_SYSTICK_RELOAD_REG = g_ucycle_per_tick - 1;
    *((volatile UINT8 *)OS_NVIC_EXCPRI_BASE + (((UINT32)(-1) & 0xF) - 4)) = ((7 << 4) & 0xff);
    *(volatile UINT32 *)OS_SYSTICK_CURRENT_REG = 0;
    *(volatile UINT32 *)OS_SYSTICK_CONTROL_REG = (1 << 2) | (1 << 1) | (1 << 0);
#endif
  
    return uwRet;

}

/*****************************************************************************
 Function    : SysTick_Handler
 Description : system tick interrupt handler.
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void SysTick_Handler(void)
{
    /*
        LOS need call LOS_TickHandler() in SysTick_Handler, don't change it,
        otherwise, LiteOS will not work.
    */
    LOS_TickHandler();
    
    /*add your code here */
#ifdef LOS_STM32L476xx
      HAL_IncTick();
#endif
    return ;
}

/*****************************************************************************
 Function    : LosAdapIntInit
 Description : config vector table in ram. please see g_use_ram_vect illustration
               this is used for arm cortex-M3/M4
 Input       : None
 Output      : None
 Return      : LOS_OK
 *****************************************************************************/
void LosAdapIntInit(void)
{
    /*
        1:ReLoad vector table address at ram . if do nothing , vector table is
        located in rom 0x00000000
        2:set nvic irq priority group
  
        Note: here can be replaced by some function , for example in Stm32 bsp
        you can just call SCB->VTOR = osGetVectorAddr(); and
        NVIC_SetPriorityGrouping(OS_NVIC_AIRCR_PRIGROUP);
    */
    if (g_use_ram_vect)
    {
        *(volatile UINT32 *)OS_NVIC_VTOR = osGetVectorAddr();
        *(volatile UINT32 *)OS_NVIC_AIRCR = (0x05FA0000 | OS_NVIC_AIRCR_PRIGROUP << 8);
    }

    return;
}

/*****************************************************************************
 Function    : LosAdapIrqEnable
 Description : external interrupt enable, and set priority
               this function is called by LOS_HwiCreate(),
               so here can use bsp func to inplemente it 
               Note : if don't use LOS_HwiCreate(), leave it empty
 Input       : irqnum: external interrupt number
               prior: priority of this interrupt
 Output      : None
 Return      : None
 *****************************************************************************/
void LosAdapIrqEnable(unsigned int irqnum, unsigned short prior)
{
    /*
        enable irq , for example in stm32 bsp you can use 
        NVIC_EnableIRQ((IRQn_Type)irqnum);
    */
    nvicSetIRQ(irqnum);
    /*
        set irq priority , for example in stm32 bsp you can use 
        NVIC_SetPriority((IRQn_Type)irqnum, prior);
    */
    nvicSetIrqPRI(irqnum, prior << 4);
    return;
}

/*****************************************************************************
 Function    : LosAdapIrqDisable
 Description : external interrupt disable
               this function is called by LOS_HwiDelete(), so use bsp func 
               to inplemente it
               Note : if don't use LOS_HwiDelete(), leave it empty
 Input       : irqnum: external interrupt number
 Output      : None
 Return      : None
 *****************************************************************************/
void LosAdapIrqDisable(unsigned int irqnum)
{
    /*
        disable irq, for example in stm32 bsp you can use 
        NVIC_DisableIRQ((IRQn_Type)irqnum);
    */
    nvicClrIRQ(irqnum);
    return;
}

/*****************************************************************************
 Function    : LOS_EvbSetup
 Description : Init device on the board
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbSetup(void)
{
    LOS_EvbUartInit();
    LOS_EvbLedInit();
    LOS_EvbKeyInit();
    return;
}

/*****************************************************************************
 Function    : LOS_EvbTrace
 Description : Bsp printf function
 Input       : const char *str
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbTrace(const char *str)
{
    LOS_EvbUartWriteStr(str);
    return;
}

