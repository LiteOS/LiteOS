#include "sys.h"
#include "uart.h"	  
//////////////////////////////////////////////////////////////////
#ifdef __GNUC__
 
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
		
#endif 

#ifdef USE_IAR
PUTCHAR_PROTOTYPE
{
	while((UART1->CSR&UART_IT_TXIEN)==0);//循环发送,直到发送完毕   
	UART1->TDR = (ch & (uint16_t)0x00FF);      
	return ch;
}

#else
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

//FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
int _sys_exit(int x) 
{ 
	x = x; 
	return 0;
}  
char uartWriteByte(char ch)
{      
	while((UART1->CSR&UART_IT_TXIEN)==0);//循环发送,直到发送完毕   
//	UART1->TDR = (ch & (uint16_t)0x00FF); 
	UART1->TDR = (ch & (uint16_t)0x00FF);	
	return ch;
}

#endif 
  
void uart_initwBaudRate(u32 bound){
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	UART_InitTypeDef UART_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能UART1，GPIOA时钟

	//UART 初始化设置
	UART_InitStructure.UART_BaudRate = bound;//串口波特率
	UART_InitStructure.UART_WordLength = UART_WordLength_8b;//字长为8位数据格式
	UART_InitStructure.UART_StopBits = UART_StopBits_1;//一个停止位
	UART_InitStructure.UART_Parity = UART_Parity_No;//无奇偶校验位
	UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;//无硬件数据流控制
	UART_InitStructure.UART_Mode = UART_Mode_Tx;	//收发模式

	UART_Init(UART1, &UART_InitStructure); //初始化串口1
	UART_Cmd(UART1, ENABLE);                    //使能串口1 

	//UART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9

	//UART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  
	
}


