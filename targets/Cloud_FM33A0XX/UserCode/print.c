#include "define_all.h"  
#include "print.h"



void Init_PrintUartx(void)
{	
#if( __DEBUG_PRINT__ != 0 )
	Uartx_Init(PRINT_UART);//初始化uart配置
	
	UARTx_RXSTA_RXEN_Setable(PRINT_UART, ENABLE);		//打开接收使能
	UARTx_TXSTA_TXEN_Setable(PRINT_UART, ENABLE);		//打开发送使能	
#endif
}


void Init_lptimer_for_runtime_stats(void)
{
	LPTIM_InitTypeDef init_para;

	//使能LPTIMER的外设时钟
	RCC_PERCLK_SetableEx( LPTFCLK, 		ENABLE );		//LPTIM功能时钟使能
	RCC_PERCLK_SetableEx( LPTRCLK, 		ENABLE );		//LPTIM总线时钟使能

	
	init_para.LPTIM_TMODE = LPTIM_LPTCFG_TMODE_PWMIM;//设置工作模式为带波形输出的普通定时器模式
	init_para.LPTIM_MODE = LPTIM_LPTCFG_MODE_CONTINUE;//设置计数模式为连续计数模式

	init_para.LPTIM_PWM = LPTIM_LPTCFG_PWM_PWM;//选择脉宽编码调制方式
    init_para.LPTIM_POLAR = LPTIM_LPTCFG_POLARITY_POS;//第一次计数值=比较值是产生上升沿

	init_para.LPTIM_TRIG_CFG = LPTIM_LPTCFG_TRIGCFG_POS;//外部输入信号上升沿trigger
	init_para.LPTIM_FLTEN = ENABLE;//使能数字滤波
	
	init_para.LPTIM_LPTIN_EDGE = LPTIM_LPTCFG_EDGESEL_POS;
	
	init_para.LPTIM_CLK_SEL = LPTIM_LPTCFG_CLKSEL_LSCLK;//选择LPTIMER的时钟源
	init_para.LPTIM_CLK_DIV = LPTIM_LPTCFG_DIVSEL_64;//设置分频值,512Hz,只能正确运行120秒统计

	init_para.LPTIM_compare_value = 0;//设置比较值
    init_para.LPTIM_target_value = 0xffff;//设置目标值

	LPTIM_Init(&init_para);//初始化LPTIMER

    //使能LPTIMER的外设中断
	NVIC_DisableIRQ(LPTIM_IRQn);
//	NVIC_SetPriority(LPTIM_IRQn, 2);
//	NVIC_EnableIRQ(LPTIM_IRQn);		

	LPTIM_LPTCTRL_LPTEN_Setable(ENABLE);//LPTIMER模块使能
}

#if 1 //suzhen
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
#endif 


// 重定向c库函数printf
int fputc(int ch, FILE *f)
{
	UARTx_TXREG_Write(PRINT_UART, ch);		//将发送数据写入发送寄存器

	while(SET == UARTx_TXBUFSTA_TXFF_Chk(PRINT_UART));	//等待发送完成		

	return (ch);
}

// 重定向c库函数scanf
int fgetc(FILE *f)
{
	while (RESET == UARTx_RXBUFSTA_RXFF_Chk(PRINT_UART));

	return (int)UARTx_RXREG_Read(PRINT_UART);
}


