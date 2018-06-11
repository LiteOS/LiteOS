/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#if 0 //OneNET
#include "core_cm3.h"
#include "stdio.h"
#include "usart1.h"
#include "usart3.h"
#endif

#include "los_bsp_led.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFaultHandler(unsigned int *arg)
{
  /* Go to infinite loop when Hard Fault exception occurs */
	/*
   unsigned int stacked_r0,stacked_r1,stacked_r2,stacked_r3,stacked_r12,stacked_lr,stacked_pc,stacked_psr;
   stacked_r0=((unsigned long)arg[0]);
   stacked_r1=((unsigned long)arg[1]);
   stacked_r2=((unsigned long)arg[2]);
   stacked_r3=((unsigned long)arg[3]);
   stacked_r12=((unsigned long)arg[4]);
       stacked_lr=((unsigned long)arg[5]);
       stacked_pc=((unsigned long)arg[6]);
       stacked_psr=((unsigned long)arg[7]);
       
       printf("R0= 0x%x\n",stacked_r0);
       printf("R1= 0x%x\n",stacked_r1);
       printf("R2= 0x%x\n",stacked_r2);
       printf("R3= 0x%x\n",stacked_r3);
       printf("R12= 0x%x\n",stacked_r12);
       printf("LR= 0x%x\n",stacked_lr);
       printf("PC= 0x%x\n",stacked_pc);
       printf("PSR= 0x%x\n",stacked_psr);
       
       printf("HFSR= 0x%x\n",SCB->HFSR);
       
       printf("BFAR= 0x%x\n",SCB->BFAR);
       

       
       SCB->HFSR=0xFFFFFFFF;*/
 
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}
#if 0
/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}
#endif

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

//外部中断，存/取PE[15:7]数据
//extern void Key_test(void);
void EXTI15_10_IRQHandler(void)
{
	/*
    if(EXTI_GetITStatus(EXTI_Line11))  
        {  
            if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11)==0)//按键按下  
                                       //按键松开  
            {  
                Key_test();
            }  
              
        }  
      
    EXTI_ClearITPendingBit(EXTI_Line11); 
*/  
}


//extern void Key_test(void);
void EXTI0_IRQHandler(void)
{

    if(EXTI_GetITStatus(EXTI_Line0))  
        {  
            if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0)==0)//按键按下       //按键松开  
            {  
                LED3_TOGGLE;
            }  
              
        }  
      
    EXTI_ClearITPendingBit(EXTI_Line0); 

}


/**
  * @brief  This function handles usart1 global interrupt request.
  * @param  None
  * @retval : None
  */

/**
  * @brief  This function handles usart2 global interrupt request.
  * @param  None
  * @retval : None
  */


/**
  * @brief  This function handles RTC global interrupt request.
  * @param  None
  * @retval : None
  */
void RTC_IRQHandler(void)
{
   
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
