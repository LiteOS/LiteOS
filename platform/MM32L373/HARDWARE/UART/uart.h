#ifndef __UART_H
#define __UART_H
#include "stdio.h"	
#include "sys.h" 
//串口1初始化		   
void uart_initwBaudRate(u32 bound);
//发送一个字节
char uartWriteByte(char ch);
#endif


