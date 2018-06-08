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
#include "los_bsp_key.h"
#include "los_bsp_led.h"
#include "usart.h"   //OneNET

/* while use bsp code to start system tick, don't use LOS header */
#define INCLUDE_LOS_HEADER
#ifdef INCLUDE_LOS_HEADER
#include "los_tick.ph"
#include "los_base.h"
#include "los_task.ph"
#include "los_swtmr.h"
#include "los_hwi.h"
#endif

/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
#ifdef LOS_STM32F103ZE
#include "stm32f10x.h"    //OneNET
#include "stm32f10x_it.h"
#endif



/*****************************************************************************
	global var
 *****************************************************************************/


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
extern void LOS_AppTickHandler (void);


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
	LOS_EvbLedInit();    //OneNET
	LOS_EvbKeyInit();   //OneNET

  //OneNET
	LOS_HwiCreate(EXTI0_IRQn, 0,0,EXTI0_IRQHandler,NULL); //OneNET
	return ;
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
    //LOS_EvbUartWriteStr(str);
    return;
}

#if 0 //OneNET
/*****************************************************************************
 Function    : LOS_DelgyUs()
 Description : 
 Input       : 
 Output      : None
 Return      : None
 *****************************************************************************/
 void LOS_DelayUs( UINT32 num )
{	
	UINT32 ticks;
	UINT32 told,tnow,tcnt = 0;
	UINT32 reload = SysTick->LOAD;				//	    	 
	ticks = num * (sys_clk_freq / 1000000); 	// 
	//
	told = SysTick->VAL;        				//
	while(1)
	{
		tnow = SysTick->VAL;	
		if( tnow != told )
		{	    
			if( tnow < told ) tcnt += told - tnow;	//
			else tcnt += reload - tnow + told;	    
			told = tnow;
			if( tcnt >= ticks) break;			///
		}  
	};
	//	    
}

#endif 


#if 1 //OneNET
//for printf function  //OneNET

#ifndef USE_SW4STM32_SU
#pragma import(__use_no_semihosting)             
              
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
   
void _sys_exit(int x) 
{ 
	x = x; 
} 

int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0); 
    USART1->DR = (char) ch;      
	return ch;
}
#endif //OneNET

//for GCC printf function
#ifdef USE_SW4STM32_SU  //OneNET

int _write (int fd, char *pBuffer, int size)  
{  
       for (int i = 0; i < size; i++)  
       {  
           while (!(USART1->SR & USART_SR_TXE))  
           {  
           }  
           //USART_SendData(USART1, pBuffer[i]);  
           USART1->DR = (char) pBuffer[i];
       }  
       return size;  
}  

#endif //OneNET

#endif



