#include "uart.h"

/*
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif 

#ifdef USE_IAR
PUTCHAR_PROTOTYPE {
    while((UART1->CSR & ((uint16_t)0x0001)) == 0 );//循环发送,直到发送完毕   
    UART1->TDR = (ch & (uint16_t)0x00FF);      
    return ch;
}

#else
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE { 
    int handle; 
};

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) { 
    x = x; 
}

//重定义fputc函数 
int fputc(int ch, FILE *f) {      
    while((UART1->CSR & ((uint16_t)0x0001)) == 0 );//循环发送,直到发送完毕   
    UART1->TDR = (ch & (uint16_t)0x00FF);      
    return ch;
}
#endif 
*/

void Uart1_Init(void) {
	//GPIO端口设置	 
	RCC->AHBENR |= 1<<17;   //使能PORTA口时钟  
	RCC->APB2ENR |= 1<<14;  //使能串口时钟 
		
	GPIOA->AFRH &= 0xF00F;
	GPIOA->AFRH |= 0x110;
	
	//UART 初始化设置
	UART1->BRR = (sys_clk_freq / 16) / (115200);
  UART1->FRA = (sys_clk_freq / 16) % (115200);
	UART1->CCR |= 0x30; //110000
	UART1->GCR |= 0x19;  //11001;
	UART1->GCR |= 0x0001;                    //使能串口1 
	
	GPIOA->CRH &= 0XFFFFF00F;//IO状态设置
	GPIOA->CRH |= 0X000008B0;//IO状态设置
}
