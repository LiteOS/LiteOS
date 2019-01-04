#include <stm32f0xx_gpio.h>
#include <stm32f0xx_usart.h>
#include <stm32f0xx_rcc.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include "uart.h"


// USART1 init function
void MX_USART2_USART_Init(void) {
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    // USART2
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);   //使能USART2
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  //开启GPIOA时钟

    //USART 初始化设置
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);

    USART_InitStructure.USART_BaudRate = 9600;//串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Tx;    //收发模式

    USART_Init(USART2, &USART_InitStructure); //初始化串口2
    USART_Cmd(USART2, ENABLE);                    //使能串口2

    //USART2_TX   GPIOA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用推挽输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.2

    //USART2_RX    GPIOA.3初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.3
}

/********************************************************************************************************
**函数信息 ：uart_send(USART_TypeDef* USARTx,char c)
**功能描述 ：串口发送字节
**输入参数 ：USART_TypeDef* USARTx ，选择USART1、USART2、USART3
**输入参数 ：char c ,串口发送的字节
**输出参数 ：无
********************************************************************************************************/
void uart_send(USART_TypeDef *USARTx, char c) {
    USART_SendData(USARTx, (uint16_t)c);
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET) {}
}

void uart2_printf(char *fmt, ...) {
#ifdef DEBUG_IGUIYU
    va_list ap;//初始化指向可变参数列表的指针
    char data_array[64] = {0};

    va_start(ap, fmt); //将第一个可变参数的地址付给ap，即ap指向可变参数列表的开始
    vsprintf(data_array, fmt, ap); //将参数fmt、ap指向的可变参数一起转换成格式
    //化字符串，放data_array数组中，其作用同
    //sprintf（），只是参数类型不同
    for (int i = 0; i < strlen((const char *)data_array); ++i)
        uart_send(USART2, data_array[i]);
    va_end(ap);    //ap付值为0，没什么实际用处，主要是为程序健壮性
#endif
}
