
/*
 * 用于初始化串口，使用非接管中断
 * */
#if LOSCFG_PLATFORM_HWI

#if SHOW_PRECOMPILE_INFO
#pragma Message("接管中断模式 --> USART")
#endif

#else
#if SHOW_PRECOMPILE_INFO
#pragma Message("非接管中断模式 --> USART")
#endif
	
#include <stdio.h>
#include <stdint.h>
#include "drv_usart.h"
#include "soc.h"
#include <csi_config.h>
#include <csi_core.h>
#include "pin.h"


 extern usart_handle_t console_handle;
 //extern void ioreuse_initial(void);
 void CSKY_USART_INIT()
{
	//ioreuse_initial();//主函数中已调用
	drv_pinmux_config(CONSOLE_TXD2, CONSOLE_TXD2_FUNC);
    drv_pinmux_config(CONSOLE_RXD2, CONSOLE_RXD2_FUNC); 
	
	/* 初始化控制台 指定串口2*/
    console_handle = csi_usart_initialize(CONSOLE_IDX2, NULL);
	/* 串口配置 */
    csi_usart_config(console_handle, 115200, USART_MODE_ASYNCHRONOUS, USART_PARITY_NONE, USART_STOP_BITS_1, USART_DATA_BITS_8);

}

#endif