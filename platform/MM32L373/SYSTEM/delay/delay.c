#include "delay.h"

static u8  fac_us=0;							//us延时倍乘数			   
	
//初始化延迟函数
void delay_init()
{
	fac_us = SystemCoreClock/1000000;								
}								    

	    								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt = 0;
	u32 reload = SysTick->LOAD;				    	 
	ticks = nus*fac_us; 									
	told = SysTick->VAL;        			
	while(1)
	{
		tnow = SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt += told - tnow;	
			else tcnt += reload - tnow + told;	    
			told = tnow;
			if(tcnt >= ticks) break;			
		}  
	}
}  









































